#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<string.h>
#include"HuffmanTree.h"
#pragma warning (disable:4996);



int main(int argc,char *argv[])
{//�����в���
//argv[0]��ʾ���г�����
//argv[1]��ʾ��ѹ������
//argv[2]��ʾ�������ļ���
//argv[3]�ļ�������
		int flag = 0;		// ��־��flag
		int i=0;
		int operation=0;
		char str_1[] = "-zr";//ѹ���ļ�
		char str_2[] = "-ur";//��ѹ�ļ�
		if (strcmp(argv[1], str_1) == 0) operation = 1;//����ѹ������
		else if (strcmp(argv[1], str_2) == 0) operation = 2;//���н�ѹ����
		switch (operation)
		{
		case 1: flag = compress(argv[2], argv[3]);	// ѹ��
				break;
		case 2: flag = unzip(argv[2], argv[3]);		// ��ѹ
				break;
		default:printf("������Ĳ�������\n");
		}
			if (flag == -1)
				printf("��Ǹ�������ļ��� \"%s\" ������!\n", argv[2]);		// �����ļ������ھͱ���
			else {
				switch (operation) {
				case 1:printf("ѹ�����"); break;
				case 2:printf("��ѹ���");
				}
			}
		return 0;
}