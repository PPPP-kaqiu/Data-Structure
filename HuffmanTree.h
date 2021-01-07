#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#pragma warning (disable:4996);

// 统计字符频度的临时结点
typedef struct {
	unsigned char ch;			// 以8bits为单元的无符号字符
	unsigned long weight;		// 每类（以二进制编码区分）字符出现频度
} TempNode;

// 哈夫曼树结点
typedef struct {
	unsigned char ch;				// 以8bits为单元的无符号字符
	unsigned long weight;			// 每类（以二进制编码区分）字符出现频度
	char* code;						// 字符对应的哈夫曼编码（动态分配存储空间）
	int parent, lchild, rchild;		// 定义双亲和左右孩子
} HTNode, * HuffmanTree;

// 选择最小和次小的两个结点，建立哈夫曼树调用
void Select(HTNode* HT, unsigned int n, int* s1, int* s2)
{
	unsigned int i;
	unsigned long min = ULONG_MAX;	//初始化为long型最大值
	for (i = 0; i < n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{//找到最小的叶子节点
			min = HT[i].weight;
			*s1 = i;		//用s1带回下标
		}
	HT[*s1].parent = 1;   // 标记此结点已被选中，防止找次小的时候重复

// 找次小
	min = ULONG_MAX;
	for (i = 0; i < n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s2 = i;
		}
}

// 建立哈夫曼树
void Create_HuffmanTree(HTNode* HT, unsigned int char_kinds, unsigned int node_num)
{//对出现的字符创建哈夫曼树
	unsigned int i;
	int s1, s2;
	for (i = char_kinds; i < node_num; ++i)
	{
		Select(HT, i, &s1, &s2);		// 选择最小的两个结点
		HT[s1].parent = HT[s2].parent = i;
		HT[i].lchild = s1;	HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
}

// 生成哈夫曼编码
void HuffmanCode(HTNode* HT, unsigned char_kinds)
{//只对出现的字符进行编码，节省空间
	unsigned int i;
	int c, f, start;
	char* cd;
	if (!(cd = (char*)malloc(256 * sizeof(char)))) {
		printf("创建空间失败！\n");
		exit(0);
	}		// 暂存编码，最多256个叶子，编码长度不超多255
	cd[256 - 1] = '\0';			//编码结束符

	for (i = 0; i < char_kinds; ++i)
	{
		start = 256 - 1;	// 初始化

		for (c = i, f = HT[i].parent; f != 0;c = f, f = HT[f].parent)// 从叶子向根逆向遍历求编码
			if (HT[f].lchild == c)	cd[--start] = '0';	
			else cd[--start] = '1';	
		HT[i].code = (char*)malloc((256 - start) * sizeof(char));	//为第i个字符编码分配空间
		if (!HT[i].code)exit(0);
		strcpy(HT[i].code, &cd[start]);     // 从cd复制编码（串）到BC
	}
	free(cd);		// 释放编码临时空间
}

// 压缩函数
int compress(char* ifname, char* ofname)
{
	unsigned int i, j,k=0;
	unsigned int char_kinds;		// 字符种类
	unsigned char char_temp;		// 暂存8bits字符
	unsigned long file_len1 = 0;
	unsigned long file_len2 = 0;
	FILE* infile, * outfile;
	TempNode node_temp;
	unsigned int node_num;
	HuffmanTree HT;
	char code_buf[256] = "\0";		// 待存编码缓冲区
	unsigned int code_len;
	/*
	**分配256个节点暂时存储从文件中读取的信息
	*/
	TempNode* temp_nodes = (TempNode*)malloc(256 * sizeof(TempNode));
	if (!temp_nodes)exit(0);		//为临时结点分配空间

	for (i = 0; i < 256; ++i)
	{	// 初始化暂存结点
		temp_nodes[i].weight = 0;
		temp_nodes[i].ch = (unsigned char)i;		// 数组的256个下标与256种字符对应
	}

	// 遍历文件，获取字符频度
	if (!(infile = fopen(ifname, "rb"))) {
		printf("打开文件失败！\n");
		exit(0);
	}
	fread((char*)&char_temp, sizeof(unsigned char), 1, infile);		// 读入一个字符
	while (!feof(infile))
	{
		++temp_nodes[char_temp].weight;		// 统计下标对应字符的权重
		++file_len1;
		fread((char*)&char_temp, sizeof(unsigned char), 1, infile);		// 读入一个字符
	}
	fclose(infile);		//关闭文件

	for (i = 0; i < 256 - 1; ++i)
		for (j = i + 1; j < 256; ++j)
			if (temp_nodes[i].weight < temp_nodes[j].weight)
			{
				node_temp = temp_nodes[i];
				temp_nodes[i] = temp_nodes[j];
				temp_nodes[j] = node_temp;	//交换
			}//未出现的字符放在后面

	for (i = 0; i < 256; ++i)	if (temp_nodes[i].weight == 0)k++;	// 统计文件中未出现的字符种类
	char_kinds = k;

	if (char_kinds == 1)
	{//文件中只出现了一个字符
		printf("文件无需压缩\n");
		if (!(outfile = fopen(ofname, "wb"))) {
			printf("创建文件失败！\n");
			exit(0);
		}
		fwrite((char*)&char_kinds, sizeof(unsigned int), 1, outfile);		// 写入字符种类
		fwrite((char*)&temp_nodes[0].ch, sizeof(unsigned char), 1, outfile);		// 写入唯一的字符
		fwrite((char*)&temp_nodes[0].weight, sizeof(unsigned long), 1, outfile);		// 写入文件长度
		free(temp_nodes);		//释放临时空间
		fclose(outfile);		//关闭文件
	}
	else
	{
		node_num = 2 * char_kinds - 1;							//分配结点个数
		HT = (HTNode*)malloc(node_num * sizeof(HTNode));		// 为哈夫曼树分配动态空间    
		if (!HT)exit(0);

		for (i = 0; i < char_kinds; ++i)
		{// 初始化前char_kinds个结点
			// 将从文件读出来的信息拷贝到树节点
			HT[i].ch = temp_nodes[i].ch;
			HT[i].weight = temp_nodes[i].weight;
			HT[i].parent = 0;
		}
		free(temp_nodes); // 释放字符频度统计的暂存区

		for (; i < node_num; ++i)		// 初始化剩余个结点
			HT[i].parent = 0;

		Create_HuffmanTree(HT, char_kinds, node_num);		// 创建哈夫曼树

		HuffmanCode(HT, char_kinds);		// 生成哈夫曼编码

		// 写入字符和相应权重，供解压时重建哈夫曼树
		outfile = fopen(ofname, "wb");					// 打开压缩后将生成的文件
		fwrite((char*)&char_kinds, sizeof(unsigned int), 1, outfile);		// 写入字符种类
		file_len2++;
		for (i = 0; i < char_kinds; ++i)
		{
			fwrite((char*)&HT[i].ch, sizeof(unsigned char), 1, outfile);			// 写入字符
			file_len2++;
			fwrite((char*)&HT[i].weight, sizeof(unsigned long), 1, outfile);		// 写入字符对应权重
			file_len2++;
		}

		fwrite((char*)&file_len1, sizeof(unsigned long), 1, outfile);		// 写入文件长度
		file_len2++;
		infile = fopen(ifname, "rb");		// 再次打开待压缩文件
		fread((char*)&char_temp, sizeof(unsigned char), 1, infile);     // 读取8bits
		while (!feof(infile))
		{
			// 为字符匹配哈弗曼编码
			for (i = 0; i < char_kinds; ++i)
				if (char_temp == HT[i].ch)	strcat(code_buf, HT[i].code);

			// 以8位为处理单元,只有满8位才处理，不足就继续读文件，知道文件读完为止
			while (strlen(code_buf) >= 8)
			{
				char_temp = '\0';		// 清空字符暂存空间
				for (i = 0; i < 8; ++i)
				{
					char_temp <<= 1;		// 左移一位，为下一个bit腾出位置
					if (code_buf[i] == '1')
						char_temp |= 1;		//若当前位为1， 通过与最后一位或运算赋1
				}
				fwrite((char*)&char_temp, sizeof(unsigned char), 1, outfile);		// 将这8bit写入文件
				file_len2++;
				strcpy(code_buf, code_buf + 8);		// 处理下一个8bit单元
			}
			fread((char*)&char_temp, sizeof(unsigned char), 1, infile);     // 每次读取8bits
		}
		// 处理最后不足8bits编码
		code_len = strlen(code_buf);
		if (code_len > 0)
		{
			char_temp = '\0';
			for (i = 0; i < code_len; ++i)
			{
				char_temp <<= 1;
				if (code_buf[i] == '1')
					char_temp |= 1;
			}
			char_temp <<= 8 - code_len;       // 只有后code_len位是有效的，将编码字段从尾部移到字节的高位
			fwrite((char*)&char_temp, sizeof(unsigned char), 1, outfile);       // 存入最后一个字节
			file_len2++;
		}

		fclose(infile);		//		// 关闭文件
		fclose(outfile);

		printf("压缩率：%f%%\n", 100*(float)file_len2 / file_len1);
		for (i = 0; i < char_kinds; ++i)//// 释放内存
			free(HT[i].code);
		free(HT);
	}
}//compress



// 解压函数
int unzip(char* ifname, char* ofname)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;		// 控制文件写入长度
	FILE* infile, * outfile;
	unsigned int char_kinds;		// 存储字符种类
	unsigned int node_num;
	HuffmanTree HT;
	unsigned char code_temp;		// 暂存8bits编码
	unsigned int root;		

	if (!(infile = fopen(ifname, "rb"))) {//打开文件并判断是否打开成功
		printf("打开文件失败！\n");
		exit(0);
	}

	// 读取文件前端编码，重建哈夫曼树
	fread((char*)&char_kinds, sizeof(unsigned int), 1, infile);     // 读取字符种类数
	if (char_kinds == 1)
	{
		fread((char*)&code_temp, sizeof(unsigned char), 1, infile);     // 读取唯一的字符
		fread((char*)&file_len, sizeof(unsigned long), 1, infile);     // 读取文件长度
		outfile = fopen(ofname, "wb");					// 打开压缩后将生成的文件
		while (file_len--)
			fwrite((char*)&code_temp, sizeof(unsigned char), 1, outfile);
		fclose(infile);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * char_kinds - 1;		
		HT = (HTNode*)malloc(node_num * sizeof(HTNode));		// 动态分配哈夫曼树结点空间

		for (i = 0; i < char_kinds; ++i)
		{//从后往前存入哈夫曼树
			fread((char*)&HT[i].ch, sizeof(unsigned char), 1, infile);		// 读入字符
			fread((char*)&HT[i].weight, sizeof(unsigned long), 1, infile);	// 读入字符对应权重
			HT[i].parent = 0;
		}
		// 初始化剩余结点的parent
		for (; i < node_num; ++i)
			HT[i].parent = 0;

		Create_HuffmanTree(HT, char_kinds, node_num);		// 重建哈夫曼树

		// 读完字符和权重信息，紧接着读取文件长度和编码
		fread((char*)&file_len, sizeof(unsigned long), 1, infile);	// 读入文件长度
		outfile = fopen(ofname, "wb");		// 打开压缩后将生成的文件
		root = node_num - 1;				//最后一个结点存储的是根节点的信息
		while (1)
		{
			fread((char*)&code_temp, sizeof(unsigned char), 1, infile);		// 读取一个字符长度的编码

			// 与压缩一样，处理一个单元（8bits）
			for (i = 0; i < 8; ++i)
			{
				// 由根向下直至叶节点正向匹配编码对应字符
				if (code_temp & 128)	//如果最高位是1，向右子树寻找
					root = HT[root].rchild;
				else
					root = HT[root].lchild;		//第一位是0，向左子树寻找

				if (root < char_kinds)
				{//当遍历到叶子节点，就写入
					fwrite((char*)&HT[root].ch, sizeof(unsigned char), 1, outfile);		//写入相应哈弗曼编码的字符
					++writen_len;
					if (writen_len == file_len) break;		// 控制与原文件长度一致
					root = node_num - 1;        // 根复位
				}
				code_temp <<= 1;		// 将编码缓存的下一位移到最高位
			}
			if (writen_len == file_len) break;
		}

		// 关闭文件
		fclose(infile);
		fclose(outfile);

		// 释放内存
		free(HT);
	}
}//unzip()