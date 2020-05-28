#ifndef _MAP
#define _MAP
#include "common.h"
namespace syao
{
	class Map
	{
		private:
			point	x_top[MAP_LEN];
			point	x_down[MAP_LEN];
			point	y_left[MAP_HIGH];
			point	y_rigth[MAP_HIGH];
			void map_show()
			{
				int i = 0;
				for (i = 0; i < MAP_LEN; i=i+2)
				{
					setpos(x_top[i].x, x_top[i].y);
					std::cout << "■" << std::endl;

					setpos(x_down[i].x, x_down[i].y);
					std::cout << "■" << std::endl;
				}
				for (i = 0; i < MAP_HIGH;i++)
				{
					setpos(y_left[i].x, y_left[i].y);
					std::cout << "■" << std::endl;

					setpos(y_rigth[i].x, y_rigth[i].y);
					std::cout << "■" << std::endl;
				}
			}
		public:
			void tips()
			{
				setpos(MAP_LEN + 3, 1);
				std::cout << "Tips: " <<std::endl;
				setpos(MAP_LEN + 3, 3);
				std::cout << "\t"<<"1.按w a s d 移动(英文)，j键攻击。";
			
				setpos(MAP_LEN + 3, 5);
				std::cout <<"\t"<< "2.玩家飞机死亡或按“P”键结束游戏 " ;

				setpos(MAP_LEN + 3, 7);
				std::cout << "\t " << "3.敌方子弹可以用我方子弹摧毁!";

				setpos(MAP_LEN + 3,9);
				std::cout << "\t " << "4.初始生命值为5，子弹伤害为1";

				setpos(MAP_LEN + 3, 11);
				std::cout << "BUG1: " << "不允许多键位进行操作！" << std::endl;

				setpos(MAP_LEN + 3, 13);
				std::cout << "BUG2: " << "敌方飞机总是从左上向右下移动！好好利用" << std::endl;
				
				setpos(MAP_LEN + 3, 15);
				std::cout << "BUG3: " << "你会被自己的子弹打伤！" << std::endl;
				

			}
			void map_init()
			{
				syao::full_screen();
				int i = 0;
				for (i = 0; i < MAP_LEN; i++)
				{
					x_top[i].x = i;
					x_top[i].y = 0;
					x_down[i].x = i;
					x_down[i].y = MAP_HIGH ;
				}
				for (i = 1; i < MAP_HIGH; i++)
				{
					y_left[i].x = 0;
					y_left[i].y = i;
					y_rigth[i].x = MAP_LEN - 1;
					y_rigth[i].y = i;
				}
				map_show();
				tips();
			}

			

	};  //end of class Map

};//end of namespace





#endif
