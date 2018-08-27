	//2018-8-21
	//$kr$kr
	//https://github.com/Alivewater0803/the-third-time-homework
	//Language: C
	//Platform:Code::github
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

typedef struct snake//定义蛇
{
    int x;
    int y;
    struct snake *next;
}Snake;
int X, Y;
enum STATUS{Up = 1, Down, Left, Right};

Snake *pHead, *pBody;

enum STATUS Direction;
int score=0, scorePerFood=10;//定义总分、定义难度提高后分值
int gameStatus = 0;
int timeInterval = 200;//定义时间间隔
void gameEnd(void);
void setPosition(int x, int y)
{
    COORD pos;
    HANDLE hOutput;
    pos.X = x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput, pos);//设定游戏界面
}
void hideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}//隐藏光标
void creatInterface(void)
{
    int i;
    for(i=0;i<58;i+=2)
    {
        setPosition(i,0);
        printf("■");							
        setPosition(i,26);
        printf("■");
    }
    for(i=1;i<26;i++)
    {
        setPosition(0,i);
        printf("■");
        setPosition(56,i);
        printf("■");
    }
    setPosition(65, 10);
    printf("游戏控制");
    setPosition(63,12);
    printf("↑\t向上.");
    setPosition(63,14);
    printf("↓\t向下.");
    setPosition(63,16);
    printf("←\t向左.");
    setPosition(63,18);
    printf("→\t向右.");
    setPosition(63,20);
    printf("F1\t加速.");
    setPosition(63, 22);
    printf("F2\t减速.");
    setPosition(63, 24);
    printf("空格\t暂停游戏.");
    setPosition(63,26);
    printf("ESC\t暂停游戏.");
    setPosition(63, 4);
    printf("分数:");
    setPosition(63, 6);
    printf("每一个食物所得的分数:");
}//游戏初始界面
void initializeSnake(void)
{
    Snake *pTail;
    int i;
    pTail = (Snake *)malloc(sizeof(Snake));
    pTail->x = 24;
    pTail->y = 5;
    pTail->next = NULL;
    for (i = 1; i < 4; i++)
    {
        pHead = (Snake *)malloc(sizeof(Snake));
        pHead->next = pTail;
        pHead->x = 24 + 2*i;
        pHead->y = 5;
        pTail = pHead;							//蛇的初始位置，4个方块
    }
    while (pTail != NULL)
    {
        setPosition(pTail->x, pTail->y);
        printf("■");
        pTail = pTail->next;
    }
}//吃到果实瞬间任意位置出现方块
int biteSelf(void)
{
    Snake *pSelf = pHead->next;
    while (pSelf != NULL)
    {
        if (pHead->x==pSelf->x && pHead->y==pSelf->y)
        {
            return 1;
        }
        pSelf = pSelf->next;
    }
    return 0;
}//蛇咬到自己后的处理函数
void creatFood(void)
{
    int flag = 0;
    X = Y = 4;
    srand((unsigned)time(NULL));
    do{
        X = rand()%52 + 2;
        Y = rand()%24 + 1;

        if (X%2 != 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }									/
        pBody = pHead;
        while (pBody->next != NULL)
        {
            if(pBody->x==X && pBody->y==Y)
            {
                flag = 1;
            }
            pBody = pBody->next;
        }
    }while(flag==1);
    setPosition(X, Y);
    printf("■");
}//随机果实

void hitWall(void)
{
    if (pHead->x==0 || pHead->x>=56 || pHead->y==0 || pHead->y>=26)
    {
        gameStatus = 1;
        gameEnd();
    }
}//撞墙函数
void snakeMove(void)
{
    Snake *pNextHead;
    hitWall();

    pNextHead = (Snake *)malloc(sizeof(Snake));
    pNextHead->next = pHead;
    switch(Direction)
    {
    case Up:
        pNextHead->x = pHead->x;
        pNextHead->y = pHead->y - 1;
        break;
    case Down:
        pNextHead->x = pHead->x;
        pNextHead->y = pHead->y + 1;
        break;
    case Right:
        pNextHead->x = pHead->x + 2;
        pNextHead->y = pHead->y;
        break;
    case Left:
        pNextHead->x = pHead->x - 2;
        pNextHead->y = pHead->y;
        break;
    default:
        break;
    }//蛇的移动函数
    pHead = pNextHead;
    pBody = pHead;
    if (pNextHead->x == X && pNextHead->y == Y)
    {
        while (pBody != NULL)
        {
            setPosition(pBody->x, pBody->y);
            printf("■");
            pBody = pBody->next;
        }
        score += scorePerFood;
        creatFood();
    }//吃到果实
    else
    {
        setPosition(pBody->x, pBody->y);
        printf("■");
        while (pBody->next->next != NULL)
        {
            pBody = pBody->next;
        }
        setPosition(pBody->next->x, pBody->next->y);
        printf("  ");
        free(pBody->next);
        pBody->next = NULL;
    }//没吃到果实
    if (biteSelf() == 1)
    {
        gameStatus = 2;
        gameEnd();
    }//咬到自己
}//是否吃到果实
void pause(void)
{
    while(1)
    {
        Sleep(300);
        if(GetAsyncKeyState(VK_SPACE))
        {
            break;
        }
    }
}//空格暂停
void gameCircle(void)
{
    Direction = Right;
    while (1)
    {
        setPosition(72, 4);
        printf("%d", score);
        setPosition(80, 6);
        printf("%d ", scorePerFood);
        if (GetAsyncKeyState(VK_UP) && Direction!=Down)
        {
            Direction = Up;
        }
        else if (GetAsyncKeyState(VK_DOWN) && Direction!=Up)
        {
            Direction = Down;
        }
        else if (GetAsyncKeyState(VK_LEFT) && Direction!=Right)
        {
            Direction = Left;
        }
        else if (GetAsyncKeyState(VK_RIGHT) && Direction!=Left)
        {
            Direction = Right;
        }
        else if (GetAsyncKeyState(VK_SPACE))
        {
            pause();
        }//输入↑↓←→后的处理，不能与蛇头方向相反
        else if (GetAsyncKeyState(VK_ESCAPE))
        {
            gameStatus = 3;
            break;
        }
        else if (GetAsyncKeyState(VK_F1))
        {
            if(timeInterval >= 70)
            {
                timeInterval -= 60;
            }
        }
        else if (GetAsyncKeyState(VK_F2))
        {
            if (timeInterval <= 400)
            {
                timeInterval += 60;
            }

        }
        switch (timeInterval)
        {
            case 20: scorePerFood = 20;break;
            case 80: scorePerFood = 15;break;
            case 140: scorePerFood = 12;break;
            case 200: scorePerFood = 10;break;
            case 260: scorePerFood = 8;break;
            case 320: scorePerFood = 6;break;
            case 380: scorePerFood = 4;break;
            case 440: scorePerFood = 2;break;
            default: scorePerFood = 0;break;
        }
        Sleep(timeInterval);
        snakeMove();
    }
}//控速函数，并设置不同速度时的单位得分
void welcomePage(void)
{
    setPosition(35,2);
	printf("欢迎来到游戏	贪吃蛇");
	setPosition(15, 5);
	printf("规则:");
	setPosition(15, 7);
	printf("1.  用键盘 ↑.↓.←.→ 来控制蛇的移动.");
	setPosition(15, 9);
	printf("2.  咬己者死.");
	setPosition(15, 11);
	printf("3.  撞墙者死.");
	setPosition(35, 13);
	printf("开发团队:  $kr$kr ");
    setPosition(15, 15);
    printf("github:  https://github.com/Alivewater0803/the-third-time-homework");
    setPosition(35, 23);
    printf("按下任意键开始");
	setPosition(0, 28);
	getchar();
	system("cls");
}//清屏，显示游戏界面
void gameEnd(void)
{
    system("cls");
    setPosition(32, 10);
    switch(gameStatus)
    {
    case 1:
        printf("您撞墙了");
        break;
    case 2:
        printf("您咬您自己");
        break;
    case 3:
        printf("您自闭了");
        break;
    default:
        break;
    }
    setPosition(32, 14);
    printf("您最后的分数是   %d", score);
    getchar();
    setPosition(0, 25);
    exit(0);
}//游戏结束
void gameStart(void)
{
	system("mode con cols=100 lines=30");		
	welcomePage();
	creatInterface();
	initializeSnake();
	creatFood();
}//游戏开始



							
int main(void)
{
    hideCursor();
	gameStart();
	gameCircle();
	gameEnd();
	return 0;
}
