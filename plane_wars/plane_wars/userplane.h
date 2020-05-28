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
			char order;			//键盘指令
			int	 health;		//飞机血量

			Plane_data()
			{
				order = 'j';
				health = 3;
			}
		};

		struct Shoot_pkg
		{
			bool flag_shoot;  //暂时无用节点
			point  shoot_pos;

			Shoot_pkg()
			{
				flag_shoot = 0;
				shoot_pos.x = 0;
				shoot_pos.y = 0;
			}
		};

	protected:
		Body						body;		//机身参数构造并初始化，初始化到Body类才能看到，俩表，cneter=中心靠下方，
		struct	Plane_data			data;		//飞机参数状态构造并初始化，初始化本类可以看到
		struct Shoot_pkg			pkg;			//子弹发射包对外接口，初始化本类可以看到
	public:
		void sendpkgtoglobal()
		{
			if (gbpos.bullet_cnt < 100)
			{
				gbpos.bulletpos[gbpos.bullet_cnt].x = body.getcenter().x;
				gbpos.bulletpos[gbpos.bullet_cnt].y = body.getcenter().y;
				gbpos.bullet_cnt++;		//在写入子弹类的时候清零；
			}
		}

		//若子弹发射标志为真，子弹位置打包pkg传输到其他类中处理，
		void plane_shoot(char key_hit)
		{
			if (key_hit == 'j')
			{
				//对共享数据加锁
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
			//初始化飞机核心，并构造飞机身体
			body.init_bodycenter(x, y);
			body.new_point(1, 1);
			body.new_point(-1, 1);
		}

		//获取健康状态
		int gethealth(void){return data.health;}		
		void sethealth(int x){data.health = x;}
		void decrease_health(){data.health--;}

		void plane_move()
		{
			mu.lock();
			plane_show();
			mu.unlock();
			data.order = get_keybord();
			//机身移动处理
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

		//默认构造函数
		void enemy_init()
		{
			body.init_bodycenter(rand()%30+4, 4);
			body.new_point(-1, 0);
			body.new_point(1, 0);
		}

		//模拟ai飞机的指令生成
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

		//子类函数和父类同名，则父类被隐藏
		void plane_move(void)
		{
			data.order =rand_order();
			//机身移动处理
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
				gbpos.bulletpos[gbpos.bullet_cnt].flag_from_user = true;   //我方飞机发射的子弹包
				gbpos.bullet_cnt++;		//在写入子弹类的时候清零；
			}
		}
		void plane_shoot(char key_hit)
		{
			if (key_hit == 'j')
			{
				//对共享数据加锁			
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
			//机身移动处理
			mu.lock();
			body.move(data.order);
			mu.unlock();

			plane_shoot(data.order);
		}

		//输出坐标到global_ok
		void output_global()
		{
			g_u.lock();
			g_userp = body.getcenter();
			/*std::cout << g_userp.x << " " << g_userp.y;*/
			g_u.unlock();
		}
		void crash_analyze()
		{
			//1.判断是否撞墙 _ok
			if (body.getcenter().x > MAP_LEN -3 || body.getcenter().x <3)//左右撞墙了
				data.health = 0;
			else if (body.getcenter().y > MAP_HIGH - 2 || body.getcenter().x < 1)//上下撞墙
				data.health = 0;
			//2.与敌方飞机碰撞检测_			但只能我撞他，他撞我没事
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
			//3。与子弹碰撞检测
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
				//1.飞机移动
				plane_move();	
				mu.lock();
				setpos(MAP_LEN + 3, 24);
				std::cout << "当前生命值(不太准)为： " << data.health;
				mu.unlock();
				
				//2.飞机坐标写到global中
				output_global();
				//3.碰撞判断
				crash_analyze();
				//4.判断飞机是否死亡，来结束游戏
				if (!gethealth()||data.order=='p')
				{
					mu_gameover.lock();
					flag_gamevoer = 1;   //玩家飞机死亡
					mu_gameover.unlock();
					exit(0);
				}
			}
		
		}

	};


};





#endif
