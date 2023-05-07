#include<stdio.h>
#include<easyx.h>

#define ROW 10
#define COL 9
#define INTERVAL 62.2
#define GRID_SIZE 71.4

enum Pieces //棋子
{
	NONE = -1,
	車,馬,相,仕,帥,炮,兵,象,士,将,砲,卒,
	//車,馬,象,士,将,砲,卒,
	BEGIN,END,
};

enum Pieces blackChess[] = { 車,馬,象,士,将,砲,卒 };
enum Pieces redChess[] = { 車,馬,相,仕,帥,炮,兵 };

const char* ChessName[] = { "車","馬","相","仕","帥","炮","兵","象","士","将","砲","卒" };

struct Chess                  //每一个棋子的属性
{
	enum Pieces id;          //棋子名称
	DWORD type;              //棋子类型，红？黑？
	short x;
	short y;
	//bool isRiver;           //是否过河
};
struct Chess map[ROW][COL];         //游戏地图

struct State
{
	int begr;
	int begc;
	int endr;
	int endc;
	int state;
}state = { NONE,NONE,NONE,NONE,END };

void show()           //打印数组
{
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t k = 0; k < COL; k++)
  		{
			printf("%2d ", map[i][k].id);
		}
		printf("\n");
	}
}

void  init()          //初始化数据
{
	for (size_t i = 0; i < ROW; i++)   //遍历棋盘
	{
		size_t temp = 0;
		for (size_t k = 0; k < COL; k++)
		{
			map[i][k].id = NONE;            // 先把棋子置为无
			if (i<=4)             //黑棋
			{
				map[i][k].type = BLACK;
				if (i==0)
				{
					if (k<=4)     // 0 1 2 3 4 3 2 1 0
					{
						temp = k;
					}
					else         //3 2 1 0
					{
					//k == 5
						temp = 4 - (k - 4);
					}
					map[i][k].id = blackChess[temp];
				}
				if (i == 2 && ( k == 1||k == 7 ))        //砲
				{
					map[i][k].id = blackChess[5];
				}
				if (i == 3 && k % 2 == 0)               //卒
				{
					map[i][k].id = blackChess[6];
				}
			}
			if (i >4)                 //红棋
			{
				map[i][k].type = RED;
				if (i == 9)
				{
					
					if (k <= 4)    // 0 1 2 3 4 3 2 1 0
					{
						temp = k;
					}             
					else           //3 2 1 0
					{
					//k == 5
						temp = 4 - (k - 4);
					}
					map[i][k].id = redChess[temp];
				}     
				if (i == 7 && (k == 1 || k == 7))//炮
				{
					map[i][k].id = redChess[5];
				}
				if (i == 6 && k % 2 == 0)	    //兵
				{
					map[i][k].id = redChess[6];
				}
			}
			//map[i][k].isriver = false;
			map[i][k].x = k * GRID_SIZE + INTERVAL;
			map[i][k].y = i * GRID_SIZE + INTERVAL;
		}
	}
 }

void draw()                              //绘制
{
	setfillcolor(RGB(255, 255, 240 ));
	setlinestyle(PS_SOLID,2);
	settextstyle(30, 0, "宋体");         
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t k = 0; k < COL; k++)
		{
			if (map[i][k].id == NONE)
				continue;
			settextcolor(map[i][k].type);
			setlinecolor(map[i][k].type);
			fillcircle(map[i][k].x, map[i][k].y,28);        //绘制棋子
			fillcircle(map[i][k].x, map[i][k].y, 25);
			outtextxy(map[i][k].x-15, map[i][k].y-15, ChessName[map[i][k].id]);
		}
	}
}

void chessMove();

void mouseEvent()                          //鼠标操作
{
	ExMessage msg;                         //定义消息结构体变量
	if (peekmessage(&msg, EX_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN)  //鼠标左键按下
		{
			
			int col = (msg.x - INTERVAL) / GRID_SIZE;    //通过鼠标坐标得出点击的数组下标
			int row = (msg.y - INTERVAL) / GRID_SIZE;    //k * GRID_SIZE + INTERVAL = x

			if (msg.x > map[row][col].x + 28 && msg.y < map[row][col].y + 28)    //下标校准
			{
				col++;
			}
			if (msg.x < map[row][col].x + 28 && msg.y > map[row][col].y + 28)
			{
				row++;
			}
			if (msg.x > map[row][col].x + 28 && msg.y > map[row][col].y + 28)
			{
				row++;
				col++;
			}
			if (state.state == BEGIN)
			{
				state.begr = row;
				state.begc = col;
				state.state = END;
			}
			else if(state.state == END)
			{
				state.endr = row;
				state.endc = col;
				state.state = BEGIN;
			}
			chessMove();
		}
	}
}

void chessMove()                //移动棋子
{
	//bool canMove = false;
	//什么情况下能够移动棋子                       
	//if (//!(state.begr == state.endr && state.begc == state.endc)
		 //state.endr!=-1&& state.begr!=-1&&
		//map[state.begr][state.begc].id!=NONE&&
		//map[state.endr][state.endc].type!= map[state.endr][state.endc].type
		//)
	//{
		//bool canMove = true;
		int a = state.begr;
		state.begr = state.endr;
		state.endr = a;

		int b = state.begc;
		state.begc = state.endc;
		state.endc = b;
	
		map[state.endr][state.endc].id = map[state.begr][state.begc].id;
		//map[state.endr][state.endc].isRiver = map[state.begr][state.begc].isRiver;
		map[state.endr][state.endc].type = map[state.begr][state.begc].type;
		map[state.begr][state.begc].id = NONE;

		printf("canMove\n");
	//}
		
}

void main()
{
	initgraph(696, 768,EX_SHOWCONSOLE);       //创建图形窗口
	setbkmode(TRANSPARENT);                  //设置背景模式
	IMAGE img_board;                     	 //贴棋盘
	loadimage(&img_board,"./ChessBoard.png");//加载图片
	init();
	
	BeginBatchDraw();                        //双缓冲绘图，防止闪屏

	while (1)
	{
		cleardevice();
		putimage(0, 0, &img_board);
		draw();
		mouseEvent();
		FlushBatchDraw();
	}
	EndBatchDraw();  
	show();
	//getchar();
}
