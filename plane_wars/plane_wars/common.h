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

	#define		MAP_LEN		100  //��ͼ�߿�����
	#define	    MAP_HIGH	46

	#define ENEMY_PLANE_MAXSIZE	 8  //�з��ɻ��������
	#define BULLET_MAXSIZE		 20//�ӵ��������

	//struct define
	typedef struct point 
	{
		int x;
		int y;
		bool flag_from_user;
		point()
		{
			flag_from_user = false;  //�����ж��ӵ���Դ�����ӵ������������ҷ��ӵ��͵ط��ӵ�
		}
	} point;
	typedef struct _Node  //Ϊ�˷ɻ����Ի�����ڵı��Σ�ʹ��ʮ���������ǲ��Ǿ�̬����
	{
		int x;
		int y;
		_Node*  up;
		_Node* down;
		_Node* left;
		_Node* rigth;
	}Node;

	//˵����������Ϊ���зɻ������ӵ���λ����Ϣ��
	//�����зɻ�������ӵ�����pkg����enemy_plan�����࣬ÿ�غϽ��л�ȡpkg״̬����д��������д�1��99����0λ����ҷɻ��ӵ���Ϊ�˱�֤�������
	//Ȼ���ӵ������࣬ÿ�غ϶�ȡ����ֵ���������ӵ�����
	//ע�⣺���͵Ľ���ͬ�����⣬�з��ɻ����ҷŷɻ�����ͬʱ����д������д����ʱ�����ܶ�����
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

	/*˵��������������������g_userp, g_enemyp��g_bullet����Ϊ��ײ�����Ҫ�ģ��ֱ���userplane��λ�ã�
	���Ͷ���д�ߵ�ͬ�����⣺д�ߣ��ɻ�U���з��ɻ�E���ӵ�B����Ҫ����Ӧ��ȫ�ֱ�����д���ݣ�AwA, EwE, BwB
	���ߣ�ArE ArB;  ErA ErB; BrA BrE; ʱ���ϵ���˴��û���������*/
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
	std::mutex  mu;   //��ӡ�����Ļ�����
	std::mutex  mu_pos;		//ȫ�ֱ������ʵĻ�����
	std::mutex  mu_gameover; //������Ϸ������־�Ļ�����
	std::mutex	g_u;	 //�û��ɻ����껥����
	std::mutex	g_e;	//�з��ɻ����껥����
	std::mutex	g_b;	//�ӵ����껥����
	G_Bullet_pos gbpos;
	bool flag_gamevoer=0;  //ֵΪ1ʱ����Ϸ������ֻ����ҷɻ���Ȩ���޸�

	point		g_userp;  //�ɻ�����
	G_enemyp	g_enemyp; //�з��ɻ�����
	G_bullet	g_bullet; //�ӵ����깲��



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
		int cx = GetSystemMetrics(SM_CXSCREEN);            /* ��Ļ��� ���� */
		int cy = GetSystemMetrics(SM_CYSCREEN);            /* ��Ļ�߶� ���� */

		LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);   /* ��ȡ������Ϣ */
		/* ���ô�����Ϣ ��� ȡ�����������߿� */
		//SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);

		SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
	}

}



#endif // ! _COMMON
