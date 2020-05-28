#ifndef _USERPLANE
#define _USERPLANE
#include"common.h"
#include"body.h"

namespace syao
{
	class Plane
	{
	protected:
		struct Plane_data
		{
			char order;			//����ָ��
			int	 health;		//�ɻ�Ѫ��

			Plane_data()
			{
				order = 'j';
				health = 3;
			}
		};

		struct Shoot_pkg
		{
			bool flag_shoot;  //��ʱ���ýڵ�
			point  shoot_pos;

			Shoot_pkg()
			{
				flag_shoot = 0;
				shoot_pos.x = 0;
				shoot_pos.y = 0;
			}
		};

	protected:
		Body						body;		//����������첢��ʼ������ʼ����Body����ܿ���������cneter=���Ŀ��·���
		struct	Plane_data			data;		//�ɻ�����״̬���첢��ʼ������ʼ��������Կ���
		struct Shoot_pkg			pkg;			//�ӵ����������ӿڣ���ʼ��������Կ���
	public:
		void sendpkgtoglobal()
		{
			if (gbpos.bullet_cnt < 100)
			{
				gbpos.bulletpos[gbpos.bullet_cnt].x = body.getcenter().x;
				gbpos.bulletpos[gbpos.bullet_cnt].y = body.getcenter().y;
				gbpos.bullet_cnt++;		//��д���ӵ����ʱ�����㣻
			}
		}

		//���ӵ������־Ϊ�棬�ӵ�λ�ô��pkg���䵽�������д���
		void plane_shoot(char key_hit)
		{
			if (key_hit == 'j')
			{
				//�Թ������ݼ���
				mu_pos.lock();
				sendpkgtoglobal();
				mu_pos.unlock();
				
			}		
		}

		void plane_show()
		{
			body.show_body();
		}

	public:
		void plane_init(int x,int y)
		{
			//��ʼ���ɻ����ģ�������ɻ�����
			body.init_bodycenter(x, y);
			body.new_point(1, 1);
			body.new_point(-1, 1);
		}

		//��ȡ����״̬
		int gethealth(void){return data.health;}		
		void sethealth(int x){data.health = x;}
		void decrease_health(){data.health--;}

		void plane_move()
		{
			mu.lock();
			plane_show();
			mu.unlock();
			data.order = get_keybord();
			//�����ƶ�����
			mu.lock();
			body.move(data.order);
			mu.unlock();
			plane_shoot(data.order);
		}


	};//end of class Plane

	class Enemy_plane :public Plane
	{
	public:
		Enemy_plane()
		{
			enemy_init();
		}

		//Ĭ�Ϲ��캯��
		void enemy_init()
		{
			body.init_bodycenter(rand()%30+4, 4);
			body.new_point(-1, 0);
			body.new_point(1, 0);
		}

		//ģ��ai�ɻ���ָ������
		char rand_order(void)
		{
			char tgt;
			int tmp = rand() %7;
			switch (tmp)
			{
			case 0:
				tgt = 'w';
				break;
			case 1:
				tgt = 's';
				break;
			case 2:
				tgt = 'a';
				break;
			case 3:
				tgt = 'd';
				break;
			case 4:
				tgt = 'j';
				break;
			case 5:
				tgt = 's';
				break;
			case 6:
				tgt = 'd';
				break;
			}
			return tgt;
		}

		//���ຯ���͸���ͬ�������౻����
		void plane_move(void)
		{
			data.order =rand_order();
			//�����ƶ�����
			mu.lock();
			body.move(data.order);
			plane_show();
			mu.unlock();
			plane_shoot(data.order);
		}

		const point& getcenter()
		{
			return body.getcenter();
		}
	
	};

	class Boer :public Plane
	{
	public:
		void boer_init()
		{
			plane_init(50,40);
			data.health = 5;
		}
		void sendpkgtoglobal()
		{
			if (gbpos.bullet_cnt < BULLET_MAXSIZE)
			{		
				gbpos.bulletpos[gbpos.bullet_cnt].x = body.getcenter().x;
				gbpos.bulletpos[gbpos.bullet_cnt].y = body.getcenter().y-2;
				gbpos.bulletpos[gbpos.bullet_cnt].flag_from_user = true;   //�ҷ��ɻ�������ӵ���
				gbpos.bullet_cnt++;		//��д���ӵ����ʱ�����㣻
			}
		}
		void plane_shoot(char key_hit)
		{
			if (key_hit == 'j')
			{
				//�Թ������ݼ���			
				mu_pos.lock();
				sendpkgtoglobal();
				mu_pos.unlock();

			}
		}
		void plane_move()
		{
			mu.lock();
			plane_show();
			mu.unlock();
			data.order = get_keybord();
		/*	mu.lock();
			std::cout << data.order<<" ";
			mu.unlock();*/
			//�����ƶ�����
			mu.lock();
			body.move(data.order);
			mu.unlock();

			plane_shoot(data.order);
		}

		//������굽global_ok
		void output_global()
		{
			g_u.lock();
			g_userp = body.getcenter();
			/*std::cout << g_userp.x << " " << g_userp.y;*/
			g_u.unlock();
		}
		void crash_analyze()
		{
			//1.�ж��Ƿ�ײǽ _ok
			if (body.getcenter().x > MAP_LEN -3 || body.getcenter().x <3)//����ײǽ��
				data.health = 0;
			else if (body.getcenter().y > MAP_HIGH - 2 || body.getcenter().x < 1)//����ײǽ
				data.health = 0;
			//2.��з��ɻ���ײ���_			��ֻ����ײ������ײ��û��
			if (data.health)
			{
				g_e.lock();
				for (int i = 0; i < g_enemyp.g_enemy_cnt; i++)
				{
					if (fabs(body.getcenter().x- g_enemyp.data[i].x)<=1
						&& fabs(body.getcenter().y- g_enemyp.data[i].y)<=1)
					{
						data.health = 0;
						break;
					}
				}
				g_e.unlock();
			}
			//3�����ӵ���ײ���
			if (data.health)
			{
				g_b.lock();
				for (int j = 0; j < g_bullet.g_bullet_cnt; j++)
				{
					/*std::cout<<g_bullet.g_bullet_cnt<<"  ";*/
					//std::cout << g_bullet.data[j].x;
					if (fabs(body.getcenter().x - g_bullet.data[j].x) <= 1 &&
						fabs(body.getcenter().y - g_bullet.data[j].y) <= 1)
					{
						//std::cout << data.health;
						data.health--;
						break;
					}
						
				}
				g_b.unlock();
			}
		}//end of crash analyze

		void operator()()
		{
			while (1)
			{				
				//1.�ɻ��ƶ�
				plane_move();	
				mu.lock();
				setpos(MAP_LEN + 3, 24);
				std::cout << "��ǰ����ֵ(��̫׼)Ϊ�� " << data.health;
				mu.unlock();
				
				//2.�ɻ�����д��global��
				output_global();
				//3.��ײ�ж�
				crash_analyze();
				//4.�жϷɻ��Ƿ���������������Ϸ
				if (!gethealth()||data.order=='p')
				{
					mu_gameover.lock();
					flag_gamevoer = 1;   //��ҷɻ�����
					mu_gameover.unlock();
					exit(0);
				}
			}
		
		}

	};


};





#endif
