#ifndef _EPMANAGE
#define _EPMANAGE
#include"userplane.h"
#include"common.h"
namespace syao
{

	class enemyP_queue
	{
	private:
		struct Pnode  //申请节点时进行构造
		{
			Enemy_plane* data;
			Pnode* next;
			Pnode()
			{
				data = new Enemy_plane;
				next = NULL;
			}
		};

		typedef struct Pnode pnode;
	private:
		Pnode* head;
		int enemy_pcnt;
	public:
		enemyP_queue() {
			head = new pnode;  //构造敌方飞机的头节点
			enemy_pcnt = 0;		//敌方飞机数量计数器
		}

		~enemyP_queue()
		{
			pnode* pnext = head;
			pnode* p = NULL;
			while (pnext->next!= NULL)
			{
				p = pnext;
				pnext = pnext->next;
				delete p;
			}
			delete pnext;
		}

	public:
		void enemy_addplane()
		{
			enemy_pcnt++;
			pnode* p = new pnode;
			pnode* tmp = head;
			while (tmp->next != NULL)
			{
				tmp = tmp->next;
			}
			tmp->next = p;
		}

		void enemy_clean()
		{
			if(enemy_pcnt!=0)
			{
				pnode* tmp = head;
				pnode* tmp2 = NULL;
				while (tmp ->next!= NULL)
				{
					tmp2= tmp;
					tmp = tmp->next;
					if (tmp->data->gethealth()<=0)  //该飞机生命值0，可以析构了
					{
						tmp2->next = tmp->next;

						//清理飞机残骸
						mu.lock();
						setpos(tmp->data->getcenter().x, tmp->data->getcenter().y);
						std::cout << " ";
						setpos(tmp->data->getcenter().x-1, tmp->data->getcenter().y);
						std::cout << " ";
						setpos(tmp->data->getcenter().x+1, tmp->data->getcenter().y);
						std::cout << " ";
						mu.unlock();

						delete tmp;
						tmp = tmp2;
						enemy_pcnt--;
					}					
				}//while				
			}//end of if
		}

		 //test_ok
		void sendtoglobal()
		{
			pnode* p = head;
			g_e.lock();
			//写之前清零cnt
			g_enemyp.g_enemy_cnt = 0;
			while (p->next != nullptr)
			{
				p = p->next;
				g_enemyp.data[g_enemyp.g_enemy_cnt]=p->data->getcenter();
				g_enemyp.g_enemy_cnt++;
			}
			g_e.unlock();
		}

		void enemy_crash_analyze()
		{
			pnode* p = head;
			//1.和墙体进行碰撞判断 _ok
			while (p->next != nullptr)
			{
				p = p->next;
				if (p->data->getcenter().x > MAP_LEN - 3 || p->data->getcenter().x < 3)//左右撞墙了
					p->data->sethealth(0);
				else if (p->data->getcenter().y > MAP_HIGH - 2 || p->data->getcenter().y < 2)//上下撞墙
					p->data->sethealth(0);
			}
			
			//2.和子弹进行碰撞判断，不需要和userplane进行判断
			p = head;
			while (p->next != nullptr)
			{
				p = p->next;				
				if (p->data->gethealth())  //当前节点没有死亡
				{
					g_b.lock();
					for (int k = 0; k < g_bullet.g_bullet_cnt; k++)
					{
						if (fabs(p->data->getcenter().x - g_bullet.data[k].x) <= 1 &&
							fabs(p->data->getcenter().y - g_bullet.data[k].y) <= 1)
						{
							p->data->decrease_health();
							break;
						}
					}
					g_b.unlock();
				}
			
			}


		}

		void operator()()
		{
			int ct = 0;			
		/*	head = new pnode;*/
			while (1)
			{
				Sleep(300);
				//1.定时生成敌方飞机
				if (ct < 15)				
					ct++;							
				else
				{
					ct = 0;
					if (enemy_pcnt < ENEMY_PLANE_MAXSIZE)				
					enemy_addplane();									
				}
				//2.向全局发送信息，bong进行碰撞判断
					sendtoglobal();
					enemy_crash_analyze();

				//3.所有飞机更新自己是否死亡
				enemy_clean();

				//4.所有飞机移动操作，根据飞机内生成的随机order，更新每个飞机的pkg
				pnode* p = head;
				while (p->next != NULL)
				{
					p = p->next;
					p->data->plane_move();
				}

				//4.判断游戏是否结束
				mu_gameover.lock();
				if (flag_gamevoer == 1)
				{
					mu_gameover.unlock();
					exit(0);
				}
				mu_gameover.unlock();
			}//while(1)
			
			
		}//end of action

	};//end of class





}


#endif