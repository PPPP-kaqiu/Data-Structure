#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
typedef struct{
    float coef;//ϵ��
    int expn;//ָ��
}Term;
typedef struct polynomial{//����ʽ 
    Term term;
    polynomial* next;
} polynomial,*LinkList;
void InitList(LinkList &P){
    P= (polynomial*)malloc(sizeof(polynomial));//ͷ���
    P->term.coef=0.0;
    P->term.expn=0;
    P->next=NULL;//�������ÿ� 
}//��ʼ������
int cmp(Term a,Term b){
    if(a.expn<b.expn) return -1;
    else if(a.expn==b.expn) return 0;
    else return 1;
}//��a��ָ��ֵ<(��=)(��>)b��ָ��ֵ,�ֱ𷵻�-1��0��+1 
void InsertNode(LinkList &P,Term e){
    polynomial* q=P;
    while(q->next!=NULL){
        if(cmp(q->next->term,e)>0)//�����ǰ���q����һ������ָ�� ���� Ҫ����Ľ���ָ��
            q=q->next;//qָ����һ�����
        else break;//�ҵ�����λ�õ�ǰ��
    }
    if(q->next!=NULL&&cmp(q->next->term,e)==0) {//ָ����ͬ��ϵ�����
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
        polynomial* node =(polynomial*)malloc(sizeof(polynomial));//Ϊ�½ڵ㿪�ٿռ� 
        node->term.coef=e.coef;
        node->term.expn=e.expn;
        if(q->next==NULL)
            node->next=NULL; //���q���Ϊβ��㣬��node����ÿ� 
        else
            node->next=q->next; //node��ָ����ָ��q����һ����� 
        q->next=node;//��node������������
    }
}//�����������ʽ������ʵ�λ��,���ϲ�ͬ���� 
void CreatPolynomial(LinkList &P){
    int m;
	printf("Pleaseb input the number of the items of the polynomial��\n");
	scanf("%d",&m); 
	Term e;
    InitList(P);
    for(int i=1; i<=m; i++)
    {
        printf("the sequence of the item and the index of the polynomial is%d and input them��\n",i);
        scanf("%f%d",&e.coef,&e.expn);
        if(e.coef==0.0) continue;
        else InsertNode(P,e);
    }
}//�������ʽ�������Լ�ϵ��ָ��������һԪ����ʽ 
void AddPolynomial(LinkList &Pc,LinkList Pa,LinkList Pb){
    polynomial* q;
    for(q=Pa->next; q!=NULL; q=q->next)
    {
        InsertNode(Pc,q->term);//��Pa��ÿһ����뵽Pc��
    }
    for(q=Pb->next; q!=NULL; q=q->next) //��Pb��ÿһ����뵽Pc��
    {
        InsertNode(Pc,q->term);
    }
}//��ɶ���ʽ�������,�������µĶ���ʽPc���д洢,��Pc=Pa+Pb 
void SubtracatPolynomial(LinkList &Pc,LinkList Pa,LinkList Pb){
    polynomial* q;
    polynomial* e=(polynomial*)malloc(sizeof(polynomial));//Ϊ�½ڵ㿪�ٿռ� 
    for(q=Pa->next; q!=NULL; q=q->next)
    {
        InsertNode(Pc,q->term);//��Pa��ÿһ����뵽Pc��
    }
    for(q=Pb->next; q!=NULL; q=q->next)
    {
        e->term.coef = -(q->term.coef); //��ϵ������෴��,�ٽ�����Ӳ�������ΪPa-Pb
        e->term.expn=q->term.expn;
        InsertNode(Pc,e->term);//��Pb��ÿһ����뵽Pc��
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
            term.coef=(q->term.coef)*(p->term.coef);//ϵ�����
            term.expn=(q->term.expn)+(p->term.expn);// ָ�����
            InsertNode(Pc,term);
        }
    }
}//��ɶ���ʽ�������,�������µĶ���ʽPc���д洢,��Pc=Pa*Pb
void DerivativePolynomial(LinkList &P,LinkList Pa){
    polynomial* q;
    Term term;
    for(q=Pa->next;q!=NULL;q=q->next){
        if(q->term.expn==0)
        {
            continue;//ָ��Ϊ0ʱ������Ϊ0 �������˴�ѭ��
        }
        else
        {
            term.coef=(q->term.coef)*(q->term.expn); //ϵ������ָ��
            term.expn=(q->term.expn)-1;//ָ����һ
            InsertNode(P,term);
        }
    }
}//��Pa�ĵ����������������P�н��д洢 
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
}//�������ʽ��x����ֵ
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
     	   q=q->next;//�ҵ���һ���λ�� 
     	   printf("%.2f ",q->term.coef);
     	   printf("%d\n",q->term.expn);
  		  }
	}
	printf("\n");
}//��ӡһԪ����ʽ
void PrintPolynomialPlus(LinkList P){
    int w=0;
	polynomial* q; q=P;
    int tag;
    if(q->next==NULL) printf("the valuo of this polynomial is 0");
    else while(q->next!=NULL){
        q=q->next;//�ҵ���һ���λ�� 
        if(q->term.expn==0){//ָ��Ϊ0��Ϊ������ 
       		if(q->term.coef>0){
       			if(w==0) printf("%.2f",q->term.coef);
       			else printf("+%.2f",q->term.coef);
       		}
      		else printf("%.2f",q->term.coef);
        }
       	else{
       		if(q->term.coef==1||q->term.coef==-1){//ϵ��Ϊ1
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
} //�Ż�������ʽ 
int main()
{
	int n,i,j,k,z=1;
	LinkList Polynomials[100];
	printf("Welcome to PPPP����'s Calculator!\n");
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
