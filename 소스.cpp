//2020052179 이정은 //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct ListNode {
	int coef;
	int expon;
	struct ListNode *link;}ListNode; //연결 리스트 노드 선언 (coef : 계수, expon : 지수)

typedef struct ListHeader {
	int length;
	ListNode *head;
	ListNode *tail;}ListHeader; //연결 리스트 헤더 선언

void input_file(ListHeader *plist1, ListHeader *plist2);	//input파일의 값들을 list1과 list2로 입력해주는 함수
void output_file(ListHeader plist3, ListHeader plist4);	//다항식 계산 결과를 output.txt로 옮기는 함수
void init(ListHeader *plist);							// 리스트 헤더초기화 함수
void insert(ListHeader*plist, int coef, int expon);	//리스트 뒤에 노드를 추가하는 함수
void poly_add(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3);	// 다항식 덧셈 함수
void poly_multi(ListHeader *plist1, ListHeader *plist2, ListHeader *plist4);	// 다항식 곱셈 함수

int main()
{
	ListHeader list1, list2, list3, list4;
	init(&list1);
	init(&list2);
	init(&list3);
	init(&list4);

	input_file(&list1, &list2); // 파일 입력

	poly_add(&list1, &list2, &list3);		// list3 > 두 다항식의 덧셈
	poly_multi(&list1, &list2, &list4);		// list4 > 두 다항식의 곱셈 

	output_file(list3, list4); // outoput.txt으로 결과를 입력
}

void init(ListHeader *plist) {
	plist->length = 0;
	plist->head = plist->tail = NULL;
} // plist: 연결 리스트의 헤더를 가리키는 포인터


void insert(ListHeader*plist, int coef, int expon)
{
	ListNode *temp = (ListNode *)malloc(sizeof(ListNode)); //빈 노드 생성
	
	if (temp == NULL)
		perror("메모리 할당 에러"); //memory의 할당 여부 확인
	
	temp->expon = expon; //노드에 지수 저장
	temp->coef = coef; //노드에 계수 저장
	
	temp->link = NULL;
	
	if (plist->tail == NULL) {	//plist가 비어있는 경우
		plist->head = plist->tail = temp;}
	
	else {	//plist에 값이 있는 경우
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
		if (a->expon == b->expon) { // a의 차수 = b의 차수 
			sum = a->coef + b->coef;
			if (sum != 0) insert(plist3, sum, a->expon);
			a = a->link;
			b = b->link;
		}
		else if (a->expon > b->expon) { // a의 차수 > b의 차수 
			insert(plist3, a->coef, a->expon);
			a = a->link;
		}
		else { // a의 차수 < b의 차수 
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

				while (c->expon > now_expon) {	//plist4에서 몇번째인지 결정
					if (c->link == NULL)
					{
						end = 1;
						break;
					}
					c = c->link;
					tet++;
				}
				if (end == 1)	// plist4의 마지막에 항의 곱을 추가
					insert(plist4, (a->coef)*(b->coef), (a->expon) + (b->expon));

				else if (c->expon == now_expon) // 해당 차수의 항이 이미 plist4에 있을 때
					c->coef += (a->coef)*(b->coef);	// 해당 계수에 a와 b 계수의 곱을 더함

				else {	//해당 차수의 항이 plist4에 없을 경우
					c=plist4->head;
					for (int i = 1; i < tet; i++)
					{c = c->link;}
					
					ListNode *node = (ListNode *)malloc(sizeof(ListNode));
					
					if (node == NULL) perror("메모리 할당에러");
					
					node->expon = now_expon;
					node->coef = (a->coef)*(b->coef);
					node->link = c->link;
					
					c->link = node;}}
			b = plist2->head;}}}

void input_file(ListHeader *plist1, ListHeader *plist2) {
	int coef, expon, list1len = 0;
	FILE* fp= fopen("input.txt", "r");	 //input.txt를 읽기 전용으로 열어 파일 타입 변수 fp로 선언
	char string[10];
	
	if (fp == NULL) {	//파일이 없을 경우 에러
		perror(" ");
	}

	while (feof(fp) == 0) {
		fgets(string, 10, fp);
		if (strlen(string) == 1)	//plist1의 길이 구하기
			break;
		list1len++;
	}

	fclose(fp);	//파일 닫기

	fp = fopen("input.txt", "r");
	while (list1len--) {	//input파일의 첫번째 다항식을 plist1에 저장
		fscanf(fp, "%d %d", &coef, &expon);
		insert(plist1, coef, expon);
	}
	while (1) {	//input파일의 두번째 다항식을 plist2에 저장
		if (feof(fp)) 
			break;	// 남아있는 모든 내용 입력
		fscanf(fp, "%d %d", &coef, &expon);
		insert(plist2, coef, expon);
	}
	
	fclose(fp);	//파일 닫기
}

void output_file(ListHeader plist3, ListHeader plist4) {
	FILE* outfile;
	outfile = fopen("output.txt", "w");	//output.txt를 쓰기 전용으로 만들어 파일 타입 변수 outf로 선언

	ListNode *p = plist3.head;

	fprintf(outfile, "%s", "Addition\n");
	for (; p; p=p->link) {
		fprintf(outfile, "%d %d\n", p->coef, p->expon);	//output.txt에 덧셈결과 입력
	}

	p = plist4->head;

	fprintf(outfile, "%s", "Multiplication\n");
	for (; p; p = p->link) {
		fprintf(outfile, "%d %d\n", p->coef, p->expon);	//output.txt에 곱셈결과 입력
	}

	fclose(outfile);	//파일 닫기
}

