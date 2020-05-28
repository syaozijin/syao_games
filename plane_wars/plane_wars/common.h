#ifndef  _COMMON
#define  _COMMON
#include<iostream>
#include<Windows.h>
#include<conio.h>  //get keybord
#include<mutex>
namespace syao
{
	//macro define
	#define BODY_RESERVE_X 11  //crossed_linked_list_xlength
	#define BODY_RESERVE_Y 5	//y_length

	#define		MAP_LEN		100  //地图边框数据
	#define	    MAP_HIGH	46

	#define ENEMY_PLANE_MAXSIZE	 8  //敌方飞机最大数量
	#define BULLET_MAXSIZE		 20//子弹最大数量

	//struct define
	typedef struct point 
	{
		int x;
		int y;
		bool flag_from_user;
		point()
		{
			flag_from_user = false;  //用于判断子弹来源，从子弹链表中区分我方子弹和地方子弹
		}
	} point;
	typedef struct _Node  //为了飞机可以花里胡哨的变形，使用十字链表，而是不是静态数组
	{
		int x;
		int y;
		_Node*  up;
		_Node* down;
		_Node* left;
		_Node* rigth;
	}Node;

	//说明：此数组为所有飞机可用子弹的位置信息，
	//由所有飞机发射的子弹产生pkg，由enemy_plan管理类，每回合进行获取pkg状态，并写入该数组中从1到99，第0位是玩家飞机子弹，为了保证玩家体验
	//然后子弹管理类，每回合读取数组值，并产生子弹飞行
	//注意：典型的进程同步问题，敌方飞机和我放飞机不能同时进行写操作，写操作时，不能读操作
	struct  Global_Bulltepos
	{
		point bulletpos[100];
		int bullet_cnt;
		Global_Bulltepos()
		{
			for (int i = 0; i < 100; i++)
			{
				bulletpos[i].x = 0;
				bulletpos[i].x = 0;
				bulletpos[i].flag_from_user = false;
			}
			bullet_cnt = 0;
		}
	};
	typedef struct  Global_Bulltepos G_Bullet_pos;

	/*说明：以下三个数据类型g_userp, g_enemyp，g_bullet，皆为碰撞检测需要的，分别存放userplane的位置，
	典型读者写者的同步问题：写者，飞机U，敌方飞机E，子弹B，都要往对应的全局变量里写数据，AwA, EwE, BwB
	读者，ArE ArB;  ErA ErB; BrA BrE; 时间关系，此处用互斥锁代替*/
	typedef struct G_enemyp
	{
		point data[ENEMY_PLANE_MAXSIZE];
		int  g_enemy_cnt;
		G_enemyp()
		{
			g_enemy_cnt = 0;
		}
	}G_enemyp;

	typedef struct G_bullet
	{
		point data[BULLET_MAXSIZE];
		int  g_bullet_cnt;
		G_bullet()
		{
			g_bullet_cnt = 0;
		}
	}G_bullet;
	
	//global variable
	std::mutex  mu;   //打印函数的互斥锁
	std::mutex  mu_pos;		//全局变量访问的互斥锁
	std::mutex  mu_gameover; //访问游戏结束标志的互斥锁
	std::mutex	g_u;	 //用户飞机坐标互斥锁
	std::mutex	g_e;	//敌方飞机坐标互斥锁
	std::mutex	g_b;	//子弹坐标互斥锁
	G_Bullet_pos gbpos;
	bool flag_gamevoer=0;  //值为1时，游戏结束，只有玩家飞机有权限修改

	point		g_userp;  //飞机坐标
	G_enemyp	g_enemyp; //敌方飞机坐标
	G_bullet	g_bullet; //子弹坐标共享



	//typedef enum casdas
	//{
	//	up = 1, down = 2, left = 3, rigth = 4
	//}navigation;

	//function define
	void setpos(int x, int y)
	{
		HANDLE cur = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD p = { short(x),short(y) };
		SetConsoleCursorPosition(cur, p);
	}

	int  fabs(int x)
	{
		if (x < 0)
		{
			x = -x;
		}
		return x;
	}


	char get_keybord()
	{
		char hit;
		_kbhit();
		hit = _getch();
		return hit;
	}

	void full_screen()
	{
		HWND hwnd = GetForegroundWindow();
		int cx = GetSystemMetrics(SM_CXSCREEN);            /* 屏幕宽度 像素 */
		int cy = GetSystemMetrics(SM_CYSCREEN);            /* 屏幕高度 像素 */

		LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);   /* 获取窗口信息 */
		/* 设置窗口信息 最大化 取消标题栏及边框 */
		//SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);

		SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
	}

}



#endif // ! _COMMON
