#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <conio.h> 
#include <stdbool.h>
#include <time.h>

//方向常量
enum { east = 2, south = 1, west = -2, north = -1 };
//蛇行走的默认方向
int direction = west;

//蛇结构体
struct snake {
	int x;
	int y;
	struct snake *next;
};

//增加蛇节点
struct snake *node(void)
{
	struct snake *new;
	new = malloc(sizeof(struct snake));
	new->next = NULL;
	return new;
}

//速度变量
int sudu = 200;

//欢迎页面
void welcome(void)
{
	printf("\n\n\n");
	printf("                        皮皮蛇");
	printf("\n\n");
	printf("                用 W A S D 控制蛇的方向");
	printf("\n\n");
	printf("                   按空格键开始游戏");
	printf("\n\n");
	printf("      按y可以设置速度,现在的速度是%d,1000等于1秒.",sudu);
	printf("\n\n\n\n\n                  https://github.com/izcp");
}

//等待空格和y
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
			printf("\n\n                 输入整数后回车：  ");
			scanf("%d", &sudu);
			system("cls");
			break;
		}
	}
}

//初始化一条三个节点的蛇
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

//画背景框
void drawBackground(void)
{
	printf("\n\n\n\n      ■■■■■■■■■■■■■■■■■■■■■■■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
           "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■                                          ■\n"
		   "      ■■■■■■■■■■■■■■■■■■■■■■■");
	//行0-21 可用 5-20     列0-52 可用 8-50
}

//分数
int score = 0;
void showScore(void)
{
	COORD pos = { 26, 23 };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
	printf("分数:%d", score);
}

//画蛇
void drawSnake(struct snake *list)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false; //隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态

	struct snake *p;
	for (p = list; p != NULL; p = p->next) {
		COORD pos = { p->y,p->x };
		SetConsoleCursorPosition(handle, pos);
		printf("●");
	}
}
//蛇移动
struct snake *snakeMove(struct snake *list)
{
	//qX,qY代表前一个蛇节点的坐标
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
	//在背景里清除蛇尾
	COORD pos = { qY, qX };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
	printf(" ");

	return list;
}

//检测按键
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

//Fx:行 Fy:列 行0-21 可用 5-20     列0-52 可用 8-50
int fX, fY;
//随机显示食物
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
	printf("★");
}

//蛇增长
struct snake *snakeGrowth(struct snake *list)
{
	//p用来遍历,q保存上一个节点
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

//判断食物有没有被吃
struct snake *eatFood(struct snake *list)
{
	if (list->x == fX && list->y == fY) {
		RandFood(list);
		//蛇增长
		list = snakeGrowth(list);
		score++;
	}
	return list;
}

//判断蛇有没有死亡
bool snakeDie(struct snake *list)
{
	//撞墙
	if (list->x == 4 || list->x == 21 || list->y == 6 || list->y == 50) {
		system("cls");
		puts("snake die!");
		return true;
	}
	//咬到自己
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
	//欢迎页面
	welcome();
	//种种子
	srand((unsigned int)time(NULL));
	while (1) {
		//等待空格
		testSpace();
		//蛇结构体指针
		struct snake *first;
		//初始化蛇和方向
		first = create();
		direction = west;

		//画背景
		drawBackground();
		//随机食物
		RandFood(first);
		while (1) {
			//检测按键
			testKey();
			//蛇移动
			first = snakeMove(first);
			//画蛇
			drawSnake(first);
			//判断食物有没有被吃
			first = eatFood(first);
			//判断蛇是否死亡
			if (snakeDie(first)) break;
			//下端显示分数
			showScore();
			//游戏速度
			Sleep(sudu);
		}

		printf("分数:%d\n\n           按空格键重新开始游戏\n"
			"按y可以设置速度,现在的速度是%d,1000等于1秒.",score,sudu);
		score = 0;
	}
}