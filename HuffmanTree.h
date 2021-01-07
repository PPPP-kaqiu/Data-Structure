#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#pragma warning (disable:4996);

// ͳ���ַ�Ƶ�ȵ���ʱ���
typedef struct {
	unsigned char ch;			// ��8bitsΪ��Ԫ���޷����ַ�
	unsigned long weight;		// ÿ�ࣨ�Զ����Ʊ������֣��ַ�����Ƶ��
} TempNode;

// �����������
typedef struct {
	unsigned char ch;				// ��8bitsΪ��Ԫ���޷����ַ�
	unsigned long weight;			// ÿ�ࣨ�Զ����Ʊ������֣��ַ�����Ƶ��
	char* code;						// �ַ���Ӧ�Ĺ��������루��̬����洢�ռ䣩
	int parent, lchild, rchild;		// ����˫�׺����Һ���
} HTNode, * HuffmanTree;

// ѡ����С�ʹ�С��������㣬����������������
void Select(HTNode* HT, unsigned int n, int* s1, int* s2)
{
	unsigned int i;
	unsigned long min = ULONG_MAX;	//��ʼ��Ϊlong�����ֵ
	for (i = 0; i < n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{//�ҵ���С��Ҷ�ӽڵ�
			min = HT[i].weight;
			*s1 = i;		//��s1�����±�
		}
	HT[*s1].parent = 1;   // ��Ǵ˽���ѱ�ѡ�У���ֹ�Ҵ�С��ʱ���ظ�

// �Ҵ�С
	min = ULONG_MAX;
	for (i = 0; i < n; ++i)
		if (HT[i].parent == 0 && HT[i].weight < min)
		{
			min = HT[i].weight;
			*s2 = i;
		}
}

// ������������
void Create_HuffmanTree(HTNode* HT, unsigned int char_kinds, unsigned int node_num)
{//�Գ��ֵ��ַ�������������
	unsigned int i;
	int s1, s2;
	for (i = char_kinds; i < node_num; ++i)
	{
		Select(HT, i, &s1, &s2);		// ѡ����С���������
		HT[s1].parent = HT[s2].parent = i;
		HT[i].lchild = s1;	HT[i].rchild = s2;
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
}

// ���ɹ���������
void HuffmanCode(HTNode* HT, unsigned char_kinds)
{//ֻ�Գ��ֵ��ַ����б��룬��ʡ�ռ�
	unsigned int i;
	int c, f, start;
	char* cd;
	if (!(cd = (char*)malloc(256 * sizeof(char)))) {
		printf("�����ռ�ʧ�ܣ�\n");
		exit(0);
	}		// �ݴ���룬���256��Ҷ�ӣ����볤�Ȳ�����255
	cd[256 - 1] = '\0';			//���������

	for (i = 0; i < char_kinds; ++i)
	{
		start = 256 - 1;	// ��ʼ��

		for (c = i, f = HT[i].parent; f != 0;c = f, f = HT[f].parent)// ��Ҷ�����������������
			if (HT[f].lchild == c)	cd[--start] = '0';	
			else cd[--start] = '1';	
		HT[i].code = (char*)malloc((256 - start) * sizeof(char));	//Ϊ��i���ַ��������ռ�
		if (!HT[i].code)exit(0);
		strcpy(HT[i].code, &cd[start]);     // ��cd���Ʊ��루������BC
	}
	free(cd);		// �ͷű�����ʱ�ռ�
}

// ѹ������
int compress(char* ifname, char* ofname)
{
	unsigned int i, j,k=0;
	unsigned int char_kinds;		// �ַ�����
	unsigned char char_temp;		// �ݴ�8bits�ַ�
	unsigned long file_len1 = 0;
	unsigned long file_len2 = 0;
	FILE* infile, * outfile;
	TempNode node_temp;
	unsigned int node_num;
	HuffmanTree HT;
	char code_buf[256] = "\0";		// ������뻺����
	unsigned int code_len;
	/*
	**����256���ڵ���ʱ�洢���ļ��ж�ȡ����Ϣ
	*/
	TempNode* temp_nodes = (TempNode*)malloc(256 * sizeof(TempNode));
	if (!temp_nodes)exit(0);		//Ϊ��ʱ������ռ�

	for (i = 0; i < 256; ++i)
	{	// ��ʼ���ݴ���
		temp_nodes[i].weight = 0;
		temp_nodes[i].ch = (unsigned char)i;		// �����256���±���256���ַ���Ӧ
	}

	// �����ļ�����ȡ�ַ�Ƶ��
	if (!(infile = fopen(ifname, "rb"))) {
		printf("���ļ�ʧ�ܣ�\n");
		exit(0);
	}
	fread((char*)&char_temp, sizeof(unsigned char), 1, infile);		// ����һ���ַ�
	while (!feof(infile))
	{
		++temp_nodes[char_temp].weight;		// ͳ���±��Ӧ�ַ���Ȩ��
		++file_len1;
		fread((char*)&char_temp, sizeof(unsigned char), 1, infile);		// ����һ���ַ�
	}
	fclose(infile);		//�ر��ļ�

	for (i = 0; i < 256 - 1; ++i)
		for (j = i + 1; j < 256; ++j)
			if (temp_nodes[i].weight < temp_nodes[j].weight)
			{
				node_temp = temp_nodes[i];
				temp_nodes[i] = temp_nodes[j];
				temp_nodes[j] = node_temp;	//����
			}//δ���ֵ��ַ����ں���

	for (i = 0; i < 256; ++i)	if (temp_nodes[i].weight == 0)k++;	// ͳ���ļ���δ���ֵ��ַ�����
	char_kinds = k;

	if (char_kinds == 1)
	{//�ļ���ֻ������һ���ַ�
		printf("�ļ�����ѹ��\n");
		if (!(outfile = fopen(ofname, "wb"))) {
			printf("�����ļ�ʧ�ܣ�\n");
			exit(0);
		}
		fwrite((char*)&char_kinds, sizeof(unsigned int), 1, outfile);		// д���ַ�����
		fwrite((char*)&temp_nodes[0].ch, sizeof(unsigned char), 1, outfile);		// д��Ψһ���ַ�
		fwrite((char*)&temp_nodes[0].weight, sizeof(unsigned long), 1, outfile);		// д���ļ�����
		free(temp_nodes);		//�ͷ���ʱ�ռ�
		fclose(outfile);		//�ر��ļ�
	}
	else
	{
		node_num = 2 * char_kinds - 1;							//���������
		HT = (HTNode*)malloc(node_num * sizeof(HTNode));		// Ϊ�����������䶯̬�ռ�    
		if (!HT)exit(0);

		for (i = 0; i < char_kinds; ++i)
		{// ��ʼ��ǰchar_kinds�����
			// �����ļ�����������Ϣ���������ڵ�
			HT[i].ch = temp_nodes[i].ch;
			HT[i].weight = temp_nodes[i].weight;
			HT[i].parent = 0;
		}
		free(temp_nodes); // �ͷ��ַ�Ƶ��ͳ�Ƶ��ݴ���

		for (; i < node_num; ++i)		// ��ʼ��ʣ������
			HT[i].parent = 0;

		Create_HuffmanTree(HT, char_kinds, node_num);		// ������������

		HuffmanCode(HT, char_kinds);		// ���ɹ���������

		// д���ַ�����ӦȨ�أ�����ѹʱ�ؽ���������
		outfile = fopen(ofname, "wb");					// ��ѹ�������ɵ��ļ�
		fwrite((char*)&char_kinds, sizeof(unsigned int), 1, outfile);		// д���ַ�����
		file_len2++;
		for (i = 0; i < char_kinds; ++i)
		{
			fwrite((char*)&HT[i].ch, sizeof(unsigned char), 1, outfile);			// д���ַ�
			file_len2++;
			fwrite((char*)&HT[i].weight, sizeof(unsigned long), 1, outfile);		// д���ַ���ӦȨ��
			file_len2++;
		}

		fwrite((char*)&file_len1, sizeof(unsigned long), 1, outfile);		// д���ļ�����
		file_len2++;
		infile = fopen(ifname, "rb");		// �ٴδ򿪴�ѹ���ļ�
		fread((char*)&char_temp, sizeof(unsigned char), 1, infile);     // ��ȡ8bits
		while (!feof(infile))
		{
			// Ϊ�ַ�ƥ�����������
			for (i = 0; i < char_kinds; ++i)
				if (char_temp == HT[i].ch)	strcat(code_buf, HT[i].code);

			// ��8λΪ����Ԫ,ֻ����8λ�Ŵ�������ͼ������ļ���֪���ļ�����Ϊֹ
			while (strlen(code_buf) >= 8)
			{
				char_temp = '\0';		// ����ַ��ݴ�ռ�
				for (i = 0; i < 8; ++i)
				{
					char_temp <<= 1;		// ����һλ��Ϊ��һ��bit�ڳ�λ��
					if (code_buf[i] == '1')
						char_temp |= 1;		//����ǰλΪ1�� ͨ�������һλ�����㸳1
				}
				fwrite((char*)&char_temp, sizeof(unsigned char), 1, outfile);		// ����8bitд���ļ�
				file_len2++;
				strcpy(code_buf, code_buf + 8);		// ������һ��8bit��Ԫ
			}
			fread((char*)&char_temp, sizeof(unsigned char), 1, infile);     // ÿ�ζ�ȡ8bits
		}
		// ���������8bits����
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
			char_temp <<= 8 - code_len;       // ֻ�к�code_lenλ����Ч�ģ��������ֶδ�β���Ƶ��ֽڵĸ�λ
			fwrite((char*)&char_temp, sizeof(unsigned char), 1, outfile);       // �������һ���ֽ�
			file_len2++;
		}

		fclose(infile);		//		// �ر��ļ�
		fclose(outfile);

		printf("ѹ���ʣ�%f%%\n", 100*(float)file_len2 / file_len1);
		for (i = 0; i < char_kinds; ++i)//// �ͷ��ڴ�
			free(HT[i].code);
		free(HT);
	}
}//compress



// ��ѹ����
int unzip(char* ifname, char* ofname)
{
	unsigned int i;
	unsigned long file_len;
	unsigned long writen_len = 0;		// �����ļ�д�볤��
	FILE* infile, * outfile;
	unsigned int char_kinds;		// �洢�ַ�����
	unsigned int node_num;
	HuffmanTree HT;
	unsigned char code_temp;		// �ݴ�8bits����
	unsigned int root;		

	if (!(infile = fopen(ifname, "rb"))) {//���ļ����ж��Ƿ�򿪳ɹ�
		printf("���ļ�ʧ�ܣ�\n");
		exit(0);
	}

	// ��ȡ�ļ�ǰ�˱��룬�ؽ���������
	fread((char*)&char_kinds, sizeof(unsigned int), 1, infile);     // ��ȡ�ַ�������
	if (char_kinds == 1)
	{
		fread((char*)&code_temp, sizeof(unsigned char), 1, infile);     // ��ȡΨһ���ַ�
		fread((char*)&file_len, sizeof(unsigned long), 1, infile);     // ��ȡ�ļ�����
		outfile = fopen(ofname, "wb");					// ��ѹ�������ɵ��ļ�
		while (file_len--)
			fwrite((char*)&code_temp, sizeof(unsigned char), 1, outfile);
		fclose(infile);
		fclose(outfile);
	}
	else
	{
		node_num = 2 * char_kinds - 1;		
		HT = (HTNode*)malloc(node_num * sizeof(HTNode));		// ��̬��������������ռ�

		for (i = 0; i < char_kinds; ++i)
		{//�Ӻ���ǰ�����������
			fread((char*)&HT[i].ch, sizeof(unsigned char), 1, infile);		// �����ַ�
			fread((char*)&HT[i].weight, sizeof(unsigned long), 1, infile);	// �����ַ���ӦȨ��
			HT[i].parent = 0;
		}
		// ��ʼ��ʣ�����parent
		for (; i < node_num; ++i)
			HT[i].parent = 0;

		Create_HuffmanTree(HT, char_kinds, node_num);		// �ؽ���������

		// �����ַ���Ȩ����Ϣ�������Ŷ�ȡ�ļ����Ⱥͱ���
		fread((char*)&file_len, sizeof(unsigned long), 1, infile);	// �����ļ�����
		outfile = fopen(ofname, "wb");		// ��ѹ�������ɵ��ļ�
		root = node_num - 1;				//���һ�����洢���Ǹ��ڵ����Ϣ
		while (1)
		{
			fread((char*)&code_temp, sizeof(unsigned char), 1, infile);		// ��ȡһ���ַ����ȵı���

			// ��ѹ��һ��������һ����Ԫ��8bits��
			for (i = 0; i < 8; ++i)
			{
				// �ɸ�����ֱ��Ҷ�ڵ�����ƥ������Ӧ�ַ�
				if (code_temp & 128)	//������λ��1����������Ѱ��
					root = HT[root].rchild;
				else
					root = HT[root].lchild;		//��һλ��0����������Ѱ��

				if (root < char_kinds)
				{//��������Ҷ�ӽڵ㣬��д��
					fwrite((char*)&HT[root].ch, sizeof(unsigned char), 1, outfile);		//д����Ӧ������������ַ�
					++writen_len;
					if (writen_len == file_len) break;		// ������ԭ�ļ�����һ��
					root = node_num - 1;        // ����λ
				}
				code_temp <<= 1;		// �����뻺�����һλ�Ƶ����λ
			}
			if (writen_len == file_len) break;
		}

		// �ر��ļ�
		fclose(infile);
		fclose(outfile);

		// �ͷ��ڴ�
		free(HT);
	}
}//unzip()