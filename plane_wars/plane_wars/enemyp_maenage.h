#ifndef _EPMANAGE
#define _EPMANAGE
#include"userplane.h"
#include"common.h"
namespace syao
{

	class enemyP_queue
	{
	private:
		struct Pnode  //����ڵ�ʱ���й���
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
			head = new pnode;  //����з��ɻ���ͷ�ڵ�
			enemy_pcnt = 0;		//�з��ɻ�����������
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
					if (tmp->data->gethealth()<=0)  //�÷ɻ�����ֵ0������������
					{
						tmp2->next = tmp->next;

						//����ɻ��к�
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
			//д֮ǰ����cnt
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
			//1.��ǽ�������ײ�ж� _ok
			while (p->next != nullptr)
			{
				p = p->next;
				if (p->data->getcenter().x > MAP_LEN - 3 || p->data->getcenter().x < 3)//����ײǽ��
					p->data->sethealth(0);
				else if (p->data->getcenter().y > MAP_HIGH - 2 || p->data->getcenter().y < 2)//����ײǽ
					p->data->sethealth(0);
			}
			
			//2.���ӵ�������ײ�жϣ�����Ҫ��userplane�����ж�
			p = head;
			while (p->next != nullptr)
			{
				p = p->next;				
				if (p->data->gethealth())  //��ǰ�ڵ�û������
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
				//1.��ʱ���ɵз��ɻ�
				if (ct < 15)				
					ct++;							
				else
				{
					ct = 0;
					if (enemy_pcnt < ENEMY_PLANE_MAXSIZE)				
					enemy_addplane();									
				}
				//2.��ȫ�ַ�����Ϣ��bong������ײ�ж�
					sendtoglobal();
					enemy_crash_analyze();

				//3.���зɻ������Լ��Ƿ�����
				enemy_clean();

				//4.���зɻ��ƶ����������ݷɻ������ɵ����order������ÿ���ɻ���pkg
				pnode* p = head;
				while (p->next != NULL)
				{
					p = p->next;
					p->data->plane_move();
				}

				//4.�ж���Ϸ�Ƿ����
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