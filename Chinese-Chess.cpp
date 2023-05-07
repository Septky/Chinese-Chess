#include<stdio.h>
#include<easyx.h>

#define ROW 10
#define COL 9
#define INTERVAL 62.2
#define GRID_SIZE 71.4

enum Pieces //����
{
	NONE = -1,
	܇,�R,��,��,��,��,��,��,ʿ,��,�h,��,
	//܇,�R,��,ʿ,��,�h,��,
	BEGIN,END,
};

enum Pieces blackChess[] = { ܇,�R,��,ʿ,��,�h,�� };
enum Pieces redChess[] = { ܇,�R,��,��,��,��,�� };

const char* ChessName[] = { "܇","�R","��","��","��","��","��","��","ʿ","��","�h","��" };

struct Chess                  //ÿһ�����ӵ�����
{
	enum Pieces id;          //��������
	DWORD type;              //�������ͣ��죿�ڣ�
	short x;
	short y;
	//bool isRiver;           //�Ƿ����
};
struct Chess map[ROW][COL];         //��Ϸ��ͼ

struct State
{
	int begr;
	int begc;
	int endr;
	int endc;
	int state;
}state = { NONE,NONE,NONE,NONE,END };

void show()           //��ӡ����
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

void  init()          //��ʼ������
{
	for (size_t i = 0; i < ROW; i++)   //��������
	{
		size_t temp = 0;
		for (size_t k = 0; k < COL; k++)
		{
			map[i][k].id = NONE;            // �Ȱ�������Ϊ��
			if (i<=4)             //����
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
				if (i == 2 && ( k == 1||k == 7 ))        //�h
				{
					map[i][k].id = blackChess[5];
				}
				if (i == 3 && k % 2 == 0)               //��
				{
					map[i][k].id = blackChess[6];
				}
			}
			if (i >4)                 //����
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
				if (i == 7 && (k == 1 || k == 7))//��
				{
					map[i][k].id = redChess[5];
				}
				if (i == 6 && k % 2 == 0)	    //��
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

void draw()                              //����
{
	setfillcolor(RGB(255, 255, 240 ));
	setlinestyle(PS_SOLID,2);
	settextstyle(30, 0, "����");         
	for (size_t i = 0; i < ROW; i++)
	{
		for (size_t k = 0; k < COL; k++)
		{
			if (map[i][k].id == NONE)
				continue;
			settextcolor(map[i][k].type);
			setlinecolor(map[i][k].type);
			fillcircle(map[i][k].x, map[i][k].y,28);        //��������
			fillcircle(map[i][k].x, map[i][k].y, 25);
			outtextxy(map[i][k].x-15, map[i][k].y-15, ChessName[map[i][k].id]);
		}
	}
}

void chessMove();

void mouseEvent()                          //������
{
	ExMessage msg;                         //������Ϣ�ṹ�����
	if (peekmessage(&msg, EX_MOUSE))
	{
		if (msg.message == WM_LBUTTONDOWN)  //����������
		{
			
			int col = (msg.x - INTERVAL) / GRID_SIZE;    //ͨ���������ó�����������±�
			int row = (msg.y - INTERVAL) / GRID_SIZE;    //k * GRID_SIZE + INTERVAL = x

			if (msg.x > map[row][col].x + 28 && msg.y < map[row][col].y + 28)    //�±�У׼
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

void chessMove()                //�ƶ�����
{
	//bool canMove = false;
	//ʲô������ܹ��ƶ�����                       
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
	initgraph(696, 768,EX_SHOWCONSOLE);       //����ͼ�δ���
	setbkmode(TRANSPARENT);                  //���ñ���ģʽ
	IMAGE img_board;                     	 //������
	loadimage(&img_board,"./ChessBoard.png");//����ͼƬ
	init();
	
	BeginBatchDraw();                        //˫�����ͼ����ֹ����

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
