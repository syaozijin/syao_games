#ifndef _MANAGE
#define _MANAGE
#include"common.h"
#include"bullet.h"
namespace syao
{

	class BManager
	{
	private:
		struct Bnode
		{
			Base* data;
			Bnode* next;
			Bnode(int x = 0,int y=0)
			{
				data= new Base(x,y);
				next = NULL;
			}
		};
		typedef struct Bnode bnode;
	private:
		bnode*			head;				//子弹头
		int				bullet_cnt;			//子弹个数
	public:
		BManager()
		{
			head		 = new bnode;  //申请节点坐标为（0，0）
			bullet_cnt	 = 0;
		}
		~BManager()
		{
			bnode* pnext = head;
			bnode* p = NULL;
			while (pnext->next != NULL)
			{
				p = pnext;
				pnext = pnext->next;
				delete p;
			}
			delete pnext;
		}

	public:
		//添加子弹
		void add_bullet(int x, int y,bool flag)
		{
			if (bullet_cnt < BULLET_MAXSIZE)
			{
				bullet_cnt++;
				//新增子弹，并更新数据（pos，heading，speed等等）
				bnode* p = new bnode(x, y);  
				if (flag)  //我方发射子弹，向上飞
				{
					p->data->sethead('w');
				}
				
				bnode* tmp = head;
				while (tmp->next != NULL)
				{
					tmp = tmp->next;
				}
				tmp->next = p;
			}
			
		}
		
		//清理死亡的子弹，并修正链表
		void clean_bullet()
		{
			if (bullet_cnt != 0)
			{
				bnode* tmp = head;
				bnode* tmp2 = NULL;
				while (tmp->next != NULL)
				{
					tmp2 = tmp;
					tmp = tmp->next;
					if (tmp->data->get_life()==false)  //该子弹生命值0，可以析构了
					{
						//挂链ok，可以释放内存了
						tmp2->next = tmp->next;

						mu.lock();
						setpos(tmp->data->getx(), tmp->data->gety());
						std::cout << " ";
						mu.unlock();

						delete tmp;
						tmp = tmp2;
						bullet_cnt--;
					}
				}//while				
			}//end of if
		} //claen

		//读取子弹
		void reloading()
		{
			mu_pos.lock();  //访问临界区，要加锁
			for (int i = 0; i < gbpos.bullet_cnt; i++)
			{
				//add 函数里面做了 bullet_cnt++; 
				//warnig bug,userp写入时要处理以下，不然会一直碰撞自己，导致子弹消失
				add_bullet(gbpos.bulletpos[i].x, gbpos.bulletpos[i].y+1, gbpos.bulletpos[i].flag_from_user);
				gbpos.bulletpos[i].flag_from_user = false;   //更新全局缓冲区的flag，不然后续子弹会乱飞
			}
			gbpos.bullet_cnt = 0;  //读完一次之后，清零，为下一轮飞机的写入做准备
			mu_pos.unlock();

		}

		//一次移动所有子弹函数
		void move_bmanage()
		{
			bnode* p = head;
			while (p->next != nullptr)
			{
				p = p->next;
				switch (p->data->getheading())
				{
				case 'w':					
					setpos(p->data->getx(), p->data->gety());
					std::cout << " " << std::endl;

					p->data->set_bullet_pos(p->data->getx(),p->data->gety()-1);
					p->data->show_bullet();
					break;
				case 's':
					setpos(p->data->getx(), p->data->gety());
					std::cout << " " << std::endl;

					p->data->set_bullet_pos(p->data->getx(), p->data->gety()+1);
					p->data->show_bullet();
					break;
				default:

					break;
				}
			} //end of while
		}  //end of move_bmanage
	
		//向gloabl写入数据
		void sendtoglobal()
		{
			bnode* p = head;
			g_b.lock();
			//写之前清零cnt
			g_bullet.g_bullet_cnt = 0;

			while (p->next != nullptr)
			{
				p = p->next;
				g_bullet.data[g_bullet.g_bullet_cnt] = p->data->getpos();
				g_bullet.g_bullet_cnt++;
			}
			g_b.unlock();
		}

		//碰撞判断，计算子弹生命状态
		void judge_blife()
		{
			//1.子弹和墙体的碰撞检测_ok
			bnode* p = head;
			while (p->next != nullptr)
			{
				p = p->next;
				if (p->data->gety()>=MAP_HIGH-1||p->data->gety()<=1)
				{
					p->data->reverse_life();
				}
			}

			//2.子弹和子弹碰撞检测： 此处算法待优化，减少时间复杂度目前接近o（n！），
					//但为了节约时间，而且该游戏中，此算法够用
			p = head;
			bnode* tmp = nullptr;
			bool  flag_p_alive=true;
			while (p->next != nullptr)//遍历链表
			{
				flag_p_alive = true;  //帮助减少循环次数的标志
				p = p->next;
				tmp = p;
				while (tmp->next != nullptr&&flag_p_alive)
				{
					tmp = tmp->next;
					if (tmp->data->get_life())
					{
						//p节点和tmp节点发生碰撞，把两点的生命置为死亡,p节点死亡标志改为0;
						if ((tmp->data->gety() == p->data->gety())
							&& (tmp->data->getx() == p->data->getx()))
						{
							p->data->reverse_life();
							tmp->data->reverse_life();
							flag_p_alive = false;
						}
					}
				} //内层while
			}//外层while

		/*	3.子弹和敌方飞机发生碰撞   ——bug，没调好,若该线程先检测到碰撞，则把子弹的生命改为死亡，
		然后在敌方飞机管理那快就出现子弹失效，没打到的情况，该碰撞检测，不影响游戏体验，所以注释掉把
			遍历链表，每个子弹和其他飞机坐标进行碰撞检测，若发生碰撞，飞机血量-1，子弹死亡*/
			//p = head;
			//while (p->next != nullptr)
			//{
			//	p = p->next;
			//	if (p->data->get_life())  //当前节点没有死亡
			//	{
			//		g_e.lock();
			//		for (int k = 0; k < g_enemyp.g_enemy_cnt; k++)
			//		{
			//			if (fabs(p->data->getx() - g_enemyp.data[k].x) <= 1 &&
			//				fabs(p->data->gety()- g_enemyp.data[k].y) <= 1)
			//			{
			//				p->data->reverse_life();
			//				break;
			//			}
			//		}
			//		g_e.unlock();
			//	}
			//}  //end of while

			//4.子弹和我方飞机进行碰撞检测
			p = head;
			g_u.lock();
			while (p->next != nullptr)
			{
				p = p->next;
				if (p->data->get_life())
				{
					if (fabs(p->data->getx() - g_userp.x) <= 1 &&
						fabs(p->data->gety() - g_userp.y) <= 1)
					{
						p->data->reverse_life();
						break;
					}
						
				}				
			}
			g_u.unlock();

		}


		void operator()()
		{
			while (1)
			{
				//1.读取子弹
				reloading();  //访问了临界区

				//2.显示子弹
				if (bullet_cnt > 0)  //打印机资源争抢
				{
					Sleep(180);    //子弹速度，越大越慢
					mu.lock();
					move_bmanage();
					mu.unlock();
				} //if
						
				//3.子弹碰撞分析并向global发送数据
				
					judge_blife();

				//4.更新子弹链表状态，死亡节点去除
					clean_bullet();
					sendtoglobal();

				//5.判断游戏是否结束
				mu_gameover.lock();
				if (flag_gamevoer == 1)
				{
					mu_gameover.unlock();
					exit(0);
				}
				mu_gameover.unlock();
			} //while(1)

		}//operator

	};



};




#endif 
