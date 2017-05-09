#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <conio.h> 
#include <stdbool.h>
#include <time.h>

//������
enum { east = 2, south = 1, west = -2, north = -1 };
//�����ߵ�Ĭ�Ϸ���
int direction = west;

//�߽ṹ��
struct snake {
	int x;
	int y;
	struct snake *next;
};

//�����߽ڵ�
struct snake *node(void)
{
	struct snake *new;
	new = malloc(sizeof(struct snake));
	new->next = NULL;
	return new;
}

//�ٶȱ���
int sudu = 200;

//��ӭҳ��
void welcome(void)
{
	printf("\n\n\n");
	printf("                        ƤƤ��");
	printf("\n\n");
	printf("                �� W A S D �����ߵķ���");
	printf("\n\n");
	printf("                   ���ո����ʼ��Ϸ");
	printf("\n\n");
	printf("      ��y���������ٶ�,���ڵ��ٶ���%d,1000����1��.",sudu);
	printf("\n\n\n\n\n                  https://github.com/izcp");
}

//�ȴ��ո��y
void testSpace()
{
	int ch;
	while (1) {
		ch = _getch();
		if (' ' == ch) {
			system("cls");
			break;
		}
		if ('y' == ch) {
			printf("\n\n                 ����������س���  ");
			scanf("%d", &sudu);
			system("cls");
			break;
		}
	}
}

//��ʼ��һ�������ڵ����
struct snake * create(void)
{
	struct snake *first = NULL;
	struct snake *cur, *prev;
	int i;

	for (i = 0; i < 3; ++i) {
		if (0 == i)
			cur = first = node();
		else {
			prev->next = node();
			cur = cur->next;
		}
		cur->x = 8;
		cur->y = 10 * 2 + 2 * i;
		prev = cur;
	}
	cur->next = NULL;
	return first;
}

//��������
void drawBackground(void)
{
	printf("\n\n\n\n      ����������������������������������������������\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
           "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ��                                          ��\n"
		   "      ����������������������������������������������");
	//��0-21 ���� 5-20     ��0-52 ���� 8-50
}

//����
int score = 0;
void showScore(void)
{
	COORD pos = { 26, 23 };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
	printf("����:%d", score);
}

//����
void drawSnake(struct snake *list)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false; //���ؿ���̨���
	SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬

	struct snake *p;
	for (p = list; p != NULL; p = p->next) {
		COORD pos = { p->y,p->x };
		SetConsoleCursorPosition(handle, pos);
		printf("��");
	}
}
//���ƶ�
struct snake *snakeMove(struct snake *list)
{
	//qX,qY����ǰһ���߽ڵ������
	struct snake *p;
	bool snakeHead = true;
	int qX, qY, tempX, tempY;
	for (p = list; p != NULL; p = p->next) {
		if (snakeHead) {
			snakeHead = false;
			qX = p->x;
			qY = p->y;
			if (direction == east || direction == west)
				p->y += direction;
			else
				p->x += direction;
		}
		else {
			tempX = qX;
			tempY = qY;
			qX = p->x;
			qY = p->y;
			p->x = tempX;
			p->y = tempY;
		}
	}
	//�ڱ����������β
	COORD pos = { qY, qX };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
	printf(" ");

	return list;
}

//��ⰴ��
void testKey() {
	if (GetAsyncKeyState('W') && direction != south)
			direction = north;
	else if (GetAsyncKeyState('A') && direction != east)
				direction = west;
		else if (GetAsyncKeyState('S') && direction != north)
					direction = south;
			else if (GetAsyncKeyState('D') && direction != west)
						direction = east;
}

//Fx:�� Fy:�� ��0-21 ���� 5-20     ��0-52 ���� 8-50
int fX, fY;
//�����ʾʳ��
void RandFood(struct snake *list)
{
	fX = rand() % 15 + 5;
	fY = rand() % 21 * 2 + 8;
	struct snake *p;
	for (p = list; p != NULL; p = p->next) {
		if (p->x == fX && p->y == fY) {
			RandFood(list);
			return;
		}
	}
	COORD pos = { fY, fX };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
	printf("��");
}

//������
struct snake *snakeGrowth(struct snake *list)
{
	//p��������,q������һ���ڵ�
	struct snake *p, *q;
	int tempX, tempY;
	for (p = list; p != NULL; p = p->next) {
		tempX = p->x;
		tempY = p->y;
		q = p;
	}
	q->next = node();
	q->x = tempX;
	q->y = tempY;
	return list;
}

//�ж�ʳ����û�б���
struct snake *eatFood(struct snake *list)
{
	if (list->x == fX && list->y == fY) {
		RandFood(list);
		//������
		list = snakeGrowth(list);
		score++;
	}
	return list;
}

//�ж�����û������
bool snakeDie(struct snake *list)
{
	//ײǽ
	if (list->x == 4 || list->x == 21 || list->y == 6 || list->y == 50) {
		system("cls");
		puts("snake die!");
		return true;
	}
	//ҧ���Լ�
	else {
		struct snake *p;
		for (p = list->next; p != NULL; p = p->next) {
			if (list->x == p->x && list->y == p->y) {
				system("cls");
				puts("snake die!");
				return true;
			}
		}
	}
	return false;
}



int main()
{
	system("cls");
	//��ӭҳ��
	welcome();
	//������
	srand((unsigned int)time(NULL));
	while (1) {
		//�ȴ��ո�
		testSpace();
		//�߽ṹ��ָ��
		struct snake *first;
		//��ʼ���ߺͷ���
		first = create();
		direction = west;

		//������
		drawBackground();
		//���ʳ��
		RandFood(first);
		while (1) {
			//��ⰴ��
			testKey();
			//���ƶ�
			first = snakeMove(first);
			//����
			drawSnake(first);
			//�ж�ʳ����û�б���
			first = eatFood(first);
			//�ж����Ƿ�����
			if (snakeDie(first)) break;
			//�¶���ʾ����
			showScore();
			//��Ϸ�ٶ�
			Sleep(sudu);
		}

		printf("����:%d\n\n           ���ո�����¿�ʼ��Ϸ\n"
			"��y���������ٶ�,���ڵ��ٶ���%d,1000����1��.",score,sudu);
		score = 0;
	}
}