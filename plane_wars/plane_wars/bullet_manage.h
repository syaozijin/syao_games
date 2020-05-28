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
		bnode*			head;				//�ӵ�ͷ
		int				bullet_cnt;			//�ӵ�����
	public:
		BManager()
		{
			head		 = new bnode;  //����ڵ�����Ϊ��0��0��
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
		//����ӵ�
		void add_bullet(int x, int y,bool flag)
		{
			if (bullet_cnt < BULLET_MAXSIZE)
			{
				bullet_cnt++;
				//�����ӵ������������ݣ�pos��heading��speed�ȵȣ�
				bnode* p = new bnode(x, y);  
				if (flag)  //�ҷ������ӵ������Ϸ�
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
		
		//�����������ӵ�������������
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
					if (tmp->data->get_life()==false)  //���ӵ�����ֵ0������������
					{
						//����ok�������ͷ��ڴ���
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

		//��ȡ�ӵ�
		void reloading()
		{
			mu_pos.lock();  //�����ٽ�����Ҫ����
			for (int i = 0; i < gbpos.bullet_cnt; i++)
			{
				//add ������������ bullet_cnt++; 
				//warnig bug,userpд��ʱҪ�������£���Ȼ��һֱ��ײ�Լ��������ӵ���ʧ
				add_bullet(gbpos.bulletpos[i].x, gbpos.bulletpos[i].y+1, gbpos.bulletpos[i].flag_from_user);
				gbpos.bulletpos[i].flag_from_user = false;   //����ȫ�ֻ�������flag����Ȼ�����ӵ����ҷ�
			}
			gbpos.bullet_cnt = 0;  //����һ��֮�����㣬Ϊ��һ�ַɻ���д����׼��
			mu_pos.unlock();

		}

		//һ���ƶ������ӵ�����
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
	
		//��gloablд������
		void sendtoglobal()
		{
			bnode* p = head;
			g_b.lock();
			//д֮ǰ����cnt
			g_bullet.g_bullet_cnt = 0;

			while (p->next != nullptr)
			{
				p = p->next;
				g_bullet.data[g_bullet.g_bullet_cnt] = p->data->getpos();
				g_bullet.g_bullet_cnt++;
			}
			g_b.unlock();
		}

		//��ײ�жϣ������ӵ�����״̬
		void judge_blife()
		{
			//1.�ӵ���ǽ�����ײ���_ok
			bnode* p = head;
			while (p->next != nullptr)
			{
				p = p->next;
				if (p->data->gety()>=MAP_HIGH-1||p->data->gety()<=1)
				{
					p->data->reverse_life();
				}
			}

			//2.�ӵ����ӵ���ײ��⣺ �˴��㷨���Ż�������ʱ�临�Ӷ�Ŀǰ�ӽ�o��n������
					//��Ϊ�˽�Լʱ�䣬���Ҹ���Ϸ�У����㷨����
			p = head;
			bnode* tmp = nullptr;
			bool  flag_p_alive=true;
			while (p->next != nullptr)//��������
			{
				flag_p_alive = true;  //��������ѭ�������ı�־
				p = p->next;
				tmp = p;
				while (tmp->next != nullptr&&flag_p_alive)
				{
					tmp = tmp->next;
					if (tmp->data->get_life())
					{
						//p�ڵ��tmp�ڵ㷢����ײ���������������Ϊ����,p�ڵ�������־��Ϊ0;
						if ((tmp->data->gety() == p->data->gety())
							&& (tmp->data->getx() == p->data->getx()))
						{
							p->data->reverse_life();
							tmp->data->reverse_life();
							flag_p_alive = false;
						}
					}
				} //�ڲ�while
			}//���while

		/*	3.�ӵ��͵з��ɻ�������ײ   ����bug��û����,�����߳��ȼ�⵽��ײ������ӵ���������Ϊ������
		Ȼ���ڵз��ɻ������ǿ�ͳ����ӵ�ʧЧ��û�򵽵����������ײ��⣬��Ӱ����Ϸ���飬����ע�͵���
			��������ÿ���ӵ��������ɻ����������ײ��⣬��������ײ���ɻ�Ѫ��-1���ӵ�����*/
			//p = head;
			//while (p->next != nullptr)
			//{
			//	p = p->next;
			//	if (p->data->get_life())  //��ǰ�ڵ�û������
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

			//4.�ӵ����ҷ��ɻ�������ײ���
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
				//1.��ȡ�ӵ�
				reloading();  //�������ٽ���

				//2.��ʾ�ӵ�
				if (bullet_cnt > 0)  //��ӡ����Դ����
				{
					Sleep(180);    //�ӵ��ٶȣ�Խ��Խ��
					mu.lock();
					move_bmanage();
					mu.unlock();
				} //if
						
				//3.�ӵ���ײ��������global��������
				
					judge_blife();

				//4.�����ӵ�����״̬�������ڵ�ȥ��
					clean_bullet();
					sendtoglobal();

				//5.�ж���Ϸ�Ƿ����
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
