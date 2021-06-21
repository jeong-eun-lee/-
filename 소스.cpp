//2020052179 ������ //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct ListNode {
	int coef;
	int expon;
	struct ListNode *link;}ListNode; //���� ����Ʈ ��� ���� (coef : ���, expon : ����)

typedef struct ListHeader {
	int length;
	ListNode *head;
	ListNode *tail;}ListHeader; //���� ����Ʈ ��� ����

void input_file(ListHeader *plist1, ListHeader *plist2);	//input������ ������ list1�� list2�� �Է����ִ� �Լ�
void output_file(ListHeader plist3, ListHeader plist4);	//���׽� ��� ����� output.txt�� �ű�� �Լ�
void init(ListHeader *plist);							// ����Ʈ ����ʱ�ȭ �Լ�
void insert(ListHeader*plist, int coef, int expon);	//����Ʈ �ڿ� ��带 �߰��ϴ� �Լ�
void poly_add(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3);	// ���׽� ���� �Լ�
void poly_multi(ListHeader *plist1, ListHeader *plist2, ListHeader *plist4);	// ���׽� ���� �Լ�

int main()
{
	ListHeader list1, list2, list3, list4;
	init(&list1);
	init(&list2);
	init(&list3);
	init(&list4);

	input_file(&list1, &list2); // ���� �Է�

	poly_add(&list1, &list2, &list3);		// list3 > �� ���׽��� ����
	poly_multi(&list1, &list2, &list4);		// list4 > �� ���׽��� ���� 

	output_file(list3, list4); // outoput.txt���� ����� �Է�
}

void init(ListHeader *plist) {
	plist->length = 0;
	plist->head = plist->tail = NULL;
} // plist: ���� ����Ʈ�� ����� ����Ű�� ������


void insert(ListHeader*plist, int coef, int expon)
{
	ListNode *temp = (ListNode *)malloc(sizeof(ListNode)); //�� ��� ����
	
	if (temp == NULL)
		perror("�޸� �Ҵ� ����"); //memory�� �Ҵ� ���� Ȯ��
	
	temp->expon = expon; //��忡 ���� ����
	temp->coef = coef; //��忡 ��� ����
	
	temp->link = NULL;
	
	if (plist->tail == NULL) {	//plist�� ����ִ� ���
		plist->head = plist->tail = temp;}
	
	else {	//plist�� ���� �ִ� ���
		plist->tail = temp;
		plist->tail->link = temp;
	}
	
	plist->length++;
}

void poly_add(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3) {
	ListNode *a = plist1->head;
	ListNode *b = plist2->head;
	int sum;
	while (a && b) {
		if (a->expon == b->expon) { // a�� ���� = b�� ���� 
			sum = a->coef + b->coef;
			if (sum != 0) insert(plist3, sum, a->expon);
			a = a->link;
			b = b->link;
		}
		else if (a->expon > b->expon) { // a�� ���� > b�� ���� 
			insert(plist3, a->coef, a->expon);
			a = a->link;
		}
		else { // a�� ���� < b�� ���� 
			insert(plist3, b->coef, b->expon);
			b = b->link;
		}
	}
	for (; a != NULL; a = a->link)
	{
		insert(plist3, a->coef, a->expon);
	}
	for (; b != NULL; b = b->link)
	{
		insert(plist3, b->coef, b->expon);
	}

}

void poly_multi(ListHeader *plist1, ListHeader *plist2, ListHeader *plist4) {
	ListNode *a = plist1->head;
	ListNode *b = plist2->head;
	ListNode *c = plist4->head;
	int tet = 0;
	int now_expon;
	int end = 0;
	for (; a != NULL; a = a->link) {
		if (a == plist1->head) {
			for (; b != NULL; b = b->link)
				insert(plist4, (a->coef)*(b->coef), (a->expon) + (b->expon));
			b = plist2->head;
		}

		else {
			for (; b != NULL; b = b->link)
			{
				tet = 0;
				end = 0;
				now_expon = a->expon + b->expon;
				c = plist4->head;

				while (c->expon > now_expon) {	//plist4���� ���°���� ����
					if (c->link == NULL)
					{
						end = 1;
						break;
					}
					c = c->link;
					tet++;
				}
				if (end == 1)	// plist4�� �������� ���� ���� �߰�
					insert(plist4, (a->coef)*(b->coef), (a->expon) + (b->expon));

				else if (c->expon == now_expon) // �ش� ������ ���� �̹� plist4�� ���� ��
					c->coef += (a->coef)*(b->coef);	// �ش� ����� a�� b ����� ���� ����

				else {	//�ش� ������ ���� plist4�� ���� ���
					c=plist4->head;
					for (int i = 1; i < tet; i++)
					{c = c->link;}
					
					ListNode *node = (ListNode *)malloc(sizeof(ListNode));
					
					if (node == NULL) perror("�޸� �Ҵ翡��");
					
					node->expon = now_expon;
					node->coef = (a->coef)*(b->coef);
					node->link = c->link;
					
					c->link = node;}}
			b = plist2->head;}}}

void input_file(ListHeader *plist1, ListHeader *plist2) {
	int coef, expon, list1len = 0;
	FILE* fp= fopen("input.txt", "r");	 //input.txt�� �б� �������� ���� ���� Ÿ�� ���� fp�� ����
	char string[10];
	
	if (fp == NULL) {	//������ ���� ��� ����
		perror(" ");
	}

	while (feof(fp) == 0) {
		fgets(string, 10, fp);
		if (strlen(string) == 1)	//plist1�� ���� ���ϱ�
			break;
		list1len++;
	}

	fclose(fp);	//���� �ݱ�

	fp = fopen("input.txt", "r");
	while (list1len--) {	//input������ ù��° ���׽��� plist1�� ����
		fscanf(fp, "%d %d", &coef, &expon);
		insert(plist1, coef, expon);
	}
	while (1) {	//input������ �ι�° ���׽��� plist2�� ����
		if (feof(fp)) 
			break;	// �����ִ� ��� ���� �Է�
		fscanf(fp, "%d %d", &coef, &expon);
		insert(plist2, coef, expon);
	}
	
	fclose(fp);	//���� �ݱ�
}

void output_file(ListHeader plist3, ListHeader plist4) {
	FILE* outfile;
	outfile = fopen("output.txt", "w");	//output.txt�� ���� �������� ����� ���� Ÿ�� ���� outf�� ����

	ListNode *p = plist3.head;

	fprintf(outfile, "%s", "Addition\n");
	for (; p; p=p->link) {
		fprintf(outfile, "%d %d\n", p->coef, p->expon);	//output.txt�� ������� �Է�
	}

	p = plist4->head;

	fprintf(outfile, "%s", "Multiplication\n");
	for (; p; p = p->link) {
		fprintf(outfile, "%d %d\n", p->coef, p->expon);	//output.txt�� ������� �Է�
	}

	fclose(outfile);	//���� �ݱ�
}

