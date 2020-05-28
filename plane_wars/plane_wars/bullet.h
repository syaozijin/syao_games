#ifndef _BULLET
#define _BULLET
#include "common.h"

namespace syao
{
	class Base
	{
	private:
		struct bullet_data
		{
			int		 speed;				//子弹射击速度
			char	 heading;			//子弹射击方向
			bool	 flag_alive;		//子弹活着标志
			bullet_data()
			{
				speed = 80;
				heading = 's';
				flag_alive = 1;
			}
		};
	private:
		point					 pos;				//子弹位置坐标
		struct	bullet_data		 param;				//子弹参数表

	public:
		//构造并初始化
		Base(int xx=0,int yy=0) 
		{
			pos.x = xx;
			pos.y = yy;				
		}
		Base(Base& t)
		{
		}	
	public:
		void show_bullet()
		{
			setpos(pos.x, pos.y);
			std::cout << "*" << std::endl;
		}
		int getx() { return pos.x;}
		int gety() { return pos.y;}
		int getspeed() { return param.speed; }
		char getheading() { return param.heading; }
		bool get_life() { return param.flag_alive; }
		const point& getpos()
		{
			return pos;
		}
	public:
		void setspeed(int x)
		{
			param.speed = x;
		}

		void reverse_life()
		{
			if (param.flag_alive)
				param.flag_alive = 0;
			else
				param.flag_alive = 1;
		}

		//子弹方向控制,初始化为向下飞
		void sethead(char tmp)
		{
			param.heading = tmp;
		}

		//修改子弹位置函数
		void set_bullet_pos(int x, int y)
		{
			pos.x = x; 
			pos.y = y;
		}


		//子弹进入战场，持续飞行，直到灭亡为止
		/*void move()
		{				
			Sleep(param.speed);
			mu.lock();
			switch (param.heading)
			{
			case 'w':
				show_bullet();
				pos.y -= 1;
				setpos(pos.x, pos.y + 1);
				std::cout << " " << std::endl;
				show_bullet();
				
				break;
			case 's':
				show_bullet();
				pos.y += 1;
				setpos(pos.x, pos.y - 1);
				std::cout << " " << std::endl;
				show_bullet();
				break;
			default:
				break;
			}		
			mu.unlock();
		}*/

		//测试用代码
		/*	void show()
			{
				std::cout << pos.x << std::endl;
				std::cout << pos.y << std::endl;
			}*/

	};//end of calss Base


};//end of namespace syao;





#endif 
