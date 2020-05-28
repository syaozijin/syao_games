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
			int		 speed;				//�ӵ�����ٶ�
			char	 heading;			//�ӵ��������
			bool	 flag_alive;		//�ӵ����ű�־
			bullet_data()
			{
				speed = 80;
				heading = 's';
				flag_alive = 1;
			}
		};
	private:
		point					 pos;				//�ӵ�λ������
		struct	bullet_data		 param;				//�ӵ�������

	public:
		//���첢��ʼ��
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

		//�ӵ��������,��ʼ��Ϊ���·�
		void sethead(char tmp)
		{
			param.heading = tmp;
		}

		//�޸��ӵ�λ�ú���
		void set_bullet_pos(int x, int y)
		{
			pos.x = x; 
			pos.y = y;
		}


		//�ӵ�����ս�����������У�ֱ������Ϊֹ
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

		//�����ô���
		/*	void show()
			{
				std::cout << pos.x << std::endl;
				std::cout << pos.y << std::endl;
			}*/

	};//end of calss Base


};//end of namespace syao;





#endif 
