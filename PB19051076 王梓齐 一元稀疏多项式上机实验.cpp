#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
typedef struct{
    float coef;//系数
    int expn;//指数
}Term;
typedef struct polynomial{//多项式 
    Term term;
    polynomial* next;
} polynomial,*LinkList;
void InitList(LinkList &P){
    P= (polynomial*)malloc(sizeof(polynomial));//头结点
    P->term.coef=0.0;
    P->term.expn=0;
    P->next=NULL;//将链表置空 
}//初始化链表
int cmp(Term a,Term b){
    if(a.expn<b.expn) return -1;
    else if(a.expn==b.expn) return 0;
    else return 1;
}//依a的指数值<(或=)(或>)b的指数值,分别返回-1、0和+1 
void InsertNode(LinkList &P,Term e){
    polynomial* q=P;
    while(q->next!=NULL){
        if(cmp(q->next->term,e)>0)//如果当前结点q的下一个结点的指数 大于 要插入的结点的指数
            q=q->next;//q指向下一个结点
        else break;//找到插入位置的前驱
    }
    if(q->next!=NULL&&cmp(q->next->term,e)==0) {//指数相同，系数相加
        q->next->term.coef+=e.coef;
        if(q->next->term.coef==0.0){
			polynomial* L;
			L=q->next;
			if(L->next==NULL) q->next=NULL;
			else{
				q->next=L->next;
				free(L);
			}				        	
        }
    }
    else
    {
        polynomial* node =(polynomial*)malloc(sizeof(polynomial));//为新节点开辟空间 
        node->term.coef=e.coef;
        node->term.expn=e.expn;
        if(q->next==NULL)
            node->next=NULL; //如果q结点为尾结点，则node后继置空 
        else
            node->next=q->next; //node的指针域指向q的下一个结点 
        q->next=node;//将node结点插入链表中
    }
}//将结点插入多项式链表的适当位置,并合并同类项 
void CreatPolynomial(LinkList &P){
    int m;
	printf("Pleaseb input the number of the items of the polynomial：\n");
	scanf("%d",&m); 
	Term e;
    InitList(P);
    for(int i=1; i<=m; i++)
    {
        printf("the sequence of the item and the index of the polynomial is%d and input them：\n",i);
        scanf("%f%d",&e.coef,&e.expn);
        if(e.coef==0.0) continue;
        else InsertNode(P,e);
    }
}//输入多项式的项数以及系数指数，建立一元多项式 
void AddPolynomial(LinkList &Pc,LinkList Pa,LinkList Pb){
    polynomial* q;
    for(q=Pa->next; q!=NULL; q=q->next)
    {
        InsertNode(Pc,q->term);//将Pa的每一项插入到Pc中
    }
    for(q=Pb->next; q!=NULL; q=q->next) //将Pb的每一项插入到Pc中
    {
        InsertNode(Pc,q->term);
    }
}//完成多项式相加运算,并创建新的多项式Pc进行存储,即Pc=Pa+Pb 
void SubtracatPolynomial(LinkList &Pc,LinkList Pa,LinkList Pb){
    polynomial* q;
    polynomial* e=(polynomial*)malloc(sizeof(polynomial));//为新节点开辟空间 
    for(q=Pa->next; q!=NULL; q=q->next)
    {
        InsertNode(Pc,q->term);//将Pa的每一项插入到Pc中
    }
    for(q=Pb->next; q!=NULL; q=q->next)
    {
        e->term.coef = -(q->term.coef); //把系数变成相反数,再进行相加操作，即为Pa-Pb
        e->term.expn=q->term.expn;
        InsertNode(Pc,e->term);//将Pb的每一项插入到Pc中
    }
}
void MultiplyPolynomial(LinkList &Pc,LinkList Pa,LinkList Pb){
    polynomial *p,*q;
    Term term;
    term.coef=0.0;
    term.expn=0;
    for(q=Pa->next; q!=NULL; q=q->next)
    {
        for(p=Pb->next; p!=NULL; p=p->next)
        {
            term.coef=(q->term.coef)*(p->term.coef);//系数相乘
            term.expn=(q->term.expn)+(p->term.expn);// 指数想加
            InsertNode(Pc,term);
        }
    }
}//完成多项式相乘运算,并创建新的多项式Pc进行存储,即Pc=Pa*Pb
void DerivativePolynomial(LinkList &P,LinkList Pa){
    polynomial* q;
    Term term;
    for(q=Pa->next;q!=NULL;q=q->next){
        if(q->term.expn==0)
        {
            continue;//指数为0时，导数为0 ，跳过此次循环
        }
        else
        {
            term.coef=(q->term.coef)*(q->term.expn); //系数乘以指数
            term.expn=(q->term.expn)-1;//指数减一
            InsertNode(P,term);
        }
    }
}//求Pa的导数，并将结果导入P中进行存储 
void CaculateValue(LinkList P,float x){
	float sum=0.0;
    polynomial* q;q=P;
    if(q->next==NULL) printf("Error");
    while(q->next!=NULL){
    	q=q->next;
		float u=1.0;
	 	for(int h=0;h<q->term.expn;h++) u*=x;
	 	u*=q->term.coef;
	 	sum+=u;
	}
	printf("the caculator is%.2f\n",sum);
}//计算多项式在x处的值
void PrintPolynomial(LinkList P){
    polynomial* q; q=P;
    int s=0;
    if(q->next==NULL) printf("1\n0 0\n");
    else{
		for(q;q->next!=NULL;q=q->next){
  			s++;  	
   		 }
   		 printf("%d\n",s);
   		 q=P; 
		while(q->next!=NULL)
   		 {
     	   q=q->next;//找到第一项的位置 
     	   printf("%.2f ",q->term.coef);
     	   printf("%d\n",q->term.expn);
  		  }
	}
	printf("\n");
}//打印一元多项式
void PrintPolynomialPlus(LinkList P){
    int w=0;
	polynomial* q; q=P;
    int tag;
    if(q->next==NULL) printf("the valuo of this polynomial is 0");
    else while(q->next!=NULL){
        q=q->next;//找到第一项的位置 
        if(q->term.expn==0){//指数为0，为常数项 
       		if(q->term.coef>0){
       			if(w==0) printf("%.2f",q->term.coef);
       			else printf("+%.2f",q->term.coef);
       		}
      		else printf("%.2f",q->term.coef);
        }
       	else{
       		if(q->term.coef==1||q->term.coef==-1){//系数为1
          		if(q->term.expn==1){
               						if(q->term.coef==1){
               							if(w==0) printf("X");
               							else printf("+X");
               						}
               						else printf("-X");
           		}
				else{
           			 if(q->term.coef==1){
										if(w==0) printf("X^%d",q->term.expn);
										else printf("+X^%d",q->term.expn);
						}
               		 else printf("-X^%d",q->term.expn);
         			 }
       			 }
       		else{
      			  if(q->term.coef>0){
      			  	if(w==0) printf("%.2fX^%d",q->term.coef,q->term.expn);
      			  	else printf("+%.2fX^%d",q->term.coef,q->term.expn);
      			  }
       		      else printf("%.2fX^%d",q->term.coef,q->term.expn);
       			}
            }
    		w++;
    }
    printf("\n\n");
} //优化输出表达式 
int main()
{
	int n,i,j,k,z=1;
	LinkList Polynomials[100];
	printf("Welcome to PPPP卡丘's Calculator!\n");
	printf("**********OPERATIONS**********\n");
	printf("*****1.   menu           *****\n");
	printf("*****2.   creat          *****\n");
	printf("*****3.   diaplay        *****\n");
	printf("*****4.   diaplay all    *****\n");
	printf("*****5.   add            *****\n");
	printf("*****6.   sub            *****\n");
	printf("*****7.   delete         *****\n");
	printf("*****8.   mul            *****\n");
	printf("*****9.   calculate      *****\n");
	printf("*****10.  derivative     *****\n");
	printf("*****11.  optimized dis  *****\n");
	printf("**********OPERATIONS**********\n");
	printf("Please input the number of your polynomial:\n");
	scanf("%d",&n);
	for(i=0;i<n;i++) CreatPolynomial(Polynomials[i]);
	while(z!=0){
				printf("Please choose your operations:\n");
				scanf("%d",&k);
				switch(k){
						case 2:CreatPolynomial(Polynomials[i++]);
								break; 
						case 3:printf("please choose the subject:\n");
								int t;scanf("%d",&t);
								if(t>i) {printf("Error\n"); break;}
								printf("\n");
								PrintPolynomial(Polynomials[t-1]);
								PrintPolynomialPlus(Polynomials[t-1]);
								break;
						case 4:printf("\n");
								for(int t=0;t<i;t++){
									PrintPolynomial(Polynomials[t]);
									}
								for(int t=0;t<i;t++){
									PrintPolynomialPlus(Polynomials[t]);
									}
								break;
						case 5:printf("please choose the subject:\n");
								int l1,r1;
								scanf("%d%d",&l1,&r1);
								if(l1>i||r1>i) {printf("Error\n"); break;}
								InitList(Polynomials[i++]);
								AddPolynomial(Polynomials[i-1],Polynomials[l1-1],Polynomials[r1-1]);
								printf("\n");
								PrintPolynomial(Polynomials[i-1]);
								PrintPolynomialPlus(Polynomials[i-1]);
								break;
						case 6:printf("please choose the subject:\n");
								int l2,r2;
								printf("Please choose the substracted:\n");
								scanf("%d",&l2);
								printf("Please choose the substract:\n");
								scanf("%d",&r2);
								if(l2>i||r2>i) {printf("Error\n"); break;}
								InitList(Polynomials[i++]);
								SubtracatPolynomial(Polynomials[i-1],Polynomials[l2-1],Polynomials[r2-1]);
								printf("\n");
								PrintPolynomial(Polynomials[i-1]);
								PrintPolynomialPlus(Polynomials[i-1]);
								break;
						case 7:printf("Please choose the subject:\n");
								int c;
								scanf("%d",&c);
								if(c>i) {printf("Error\n"); break;}
								for(int y=c-1;y<i;y++) Polynomials[y]=Polynomials[y+1];
								Polynomials[--i]=NULL;
								break;
						case 8:printf("please choose the subject:\n");
								int l3,r3;
								scanf("%d%d",&l3,&r3);
								if(l3>i||r3>i) {printf("Error\n"); break;}
								InitList(Polynomials[i++]);
								MultiplyPolynomial(Polynomials[i-1],Polynomials[l3-1],Polynomials[r3-1]);
								printf("\n");
								PrintPolynomial(Polynomials[i-1]);
								PrintPolynomialPlus(Polynomials[i-1]);
								break;
     				    case 9:int a;
     				    		float x;
              				    printf("Please choose the subject:\n");
              				    scanf("%d", &a);
              				    if(a>i) {printf("Error\n"); break;}
             				    printf("Please input the value of 'x':\n");
						   	    scanf("%f",&x);
           					    CaculateValue(Polynomials[a - 1],x);
           					    break;
						case 10:printf("Please choose the subject:\n");
								int c1;
								scanf("%d",&c1);
								if(c1>i) {printf("Error\n"); break;}
								InitList(Polynomials[i++]);
								DerivativePolynomial(Polynomials[i-1],Polynomials[c1-1]);
								printf("\n");
								PrintPolynomial(Polynomials[i-1]);
								PrintPolynomialPlus(Polynomials[i-1]);
								break;
						case 11:printf("Please choose the subject:\n");
								int c2;
								scanf("%d",&c2);
								if(c2>i) {printf("Error\n"); break;}
								printf("\n");
								PrintPolynomialPlus(Polynomials[c2-1]);
		}
		printf("Do you continue?\n");
		scanf("%d",&z);
	}
    return 0;
}
