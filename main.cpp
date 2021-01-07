#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<string.h>
#include"HuffmanTree.h"
#pragma warning (disable:4996);



int main(int argc,char *argv[])
{//命令行参数
//argv[0]表示运行程序名
//argv[1]表示解压缩操作
//argv[2]表示待操作文件名
//argv[3]文件重命名
		int flag = 0;		// 标志域flag
		int i=0;
		int operation=0;
		char str_1[] = "-zr";//压缩文件
		char str_2[] = "-ur";//解压文件
		if (strcmp(argv[1], str_1) == 0) operation = 1;//进行压缩操作
		else if (strcmp(argv[1], str_2) == 0) operation = 2;//进行解压操作
		switch (operation)
		{
		case 1: flag = compress(argv[2], argv[3]);	// 压缩
				break;
		case 2: flag = unzip(argv[2], argv[3]);		// 解压
				break;
		default:printf("您输入的操作有误\n");
		}
			if (flag == -1)
				printf("抱歉，输入文件： \"%s\" 不存在!\n", argv[2]);		// 输入文件不存在就报错
			else {
				switch (operation) {
				case 1:printf("压缩完成"); break;
				case 2:printf("解压完成");
				}
			}
		return 0;
}