#ifndef  _BODY
#define  _BODY
#include<iostream>
#include"common.h"
namespace syao
{
	class Body
	{
		private:
			Node* lateral_x[BODY_RESERVE_X];   //mid=5
			Node* vertical_y[BODY_RESERVE_Y];  //mid=2
			Node* center;
		public:
			//test_ok
			Body()			
			{
				//std::cout << "in the constructor of body" << std::endl;
				for (int i = 0; i < BODY_RESERVE_X; i++)
				{
					lateral_x[i] = new Node;   //�����жϷɻ�����߽��ʱ����Ҫ�õ�������Ҫ�����ڴ�
					lateral_x[i]->up = NULL;
					lateral_x[i]->down = NULL;  //used
					lateral_x[i]->left = NULL;
					lateral_x[i]->rigth = NULL;
					lateral_x[i]->x = i;
					lateral_x[i]->y = i;

				}
				for (int j = 0; j < BODY_RESERVE_Y; j++)
				{
					vertical_y[j] = new Node;  
					vertical_y[j]->down = NULL;
					vertical_y[j]->up = NULL;
					vertical_y[j]->left = NULL;
					vertical_y[j]->rigth = NULL;//uesd
					vertical_y[j]->x = j;
					vertical_y[j]->y = j;
				}
				center = new Node;
				lateral_x[BODY_RESERVE_X/2]->down = center;
				center->up = lateral_x[BODY_RESERVE_X / 2];
				vertical_y[BODY_RESERVE_Y / 2]->rigth = center;
				center->left = vertical_y[BODY_RESERVE_Y / 2];
				center->down = NULL;
				center->rigth = NULL;
				//std::cout << "in the end constructor of body" << std::endl;
			}

			//test_ok
			Body(Body &t)
			{
				//std::cout << "in the cpoy_constructor of body" << std::endl;
				Node* tmp			 = NULL;
				Node* tmp_this		 = NULL;
				Node* tmp_t			 = NULL;
				int	  record		 = -1;

				for (int j = 0; j < BODY_RESERVE_Y; j++)
				{
					//�����������Ԫ��ָ��Ŀռ䣬���ѿռ��е����꿽��
					vertical_y[j] = new Node;
					vertical_y[j]->down = NULL;
					vertical_y[j]->up = NULL;
					vertical_y[j]->left = NULL;
					vertical_y[j]->rigth = NULL;
					vertical_y[j]->x = t.vertical_y[j]->x;
					vertical_y[j]->y = t.vertical_y[j]->y;
				}
				for (int i = 0; i < BODY_RESERVE_X; i++)
				{
					//std::cout << i << std::endl;

					//�����������Ԫ��ָ��Ŀռ䣬���ѿռ��е����꿽��,ע��new�ڵ�����ĳ�ʼ��
					lateral_x[i] = new Node;
					lateral_x[i]->down = NULL;
					lateral_x[i]->up = NULL;
					lateral_x[i]->left = NULL;
					lateral_x[i]->rigth = NULL;
					lateral_x[i]->x = t.lateral_x[i]->x;
					lateral_x[i]->y = t.lateral_x[i]->y;
					
					//�����ڵ㲢����lateral_x[]���µ�������¼��center��ֵ
					 tmp_t = t.lateral_x[i];
					 tmp_this = lateral_x[i];

					while (tmp_t->down != NULL)
					{

						tmp_t = tmp_t->down;
						Node* p = new Node;//����ڵ�
						p->x = tmp_t->x;  //��ֵ
						p->y = tmp_t->y;

						tmp_this->down= p;  //�������link
						p->up = tmp_this;
						p->down = NULL;
						tmp_this = tmp_this->down;
						//std::cout << "i am for test" << std::endl;
						//�������
						//1.�������ڵ���к�
						tmp = tmp_t;
						while (tmp->left != NULL)
						{
				/*			std::cout << tmp << std::endl;
							std::cout << tmp->left << std::endl;
							std::cout << tmp->left->left << std::endl;*/
							tmp = tmp->left;
						}

						record = tmp->y;
						//2.�����к�������
						tmp = vertical_y[record];
						while (tmp->rigth != NULL)
						{
							
							tmp = tmp->rigth;
						}
						p->left = tmp;
						tmp->rigth =p;
						p->rigth = NULL;


					

						//������������з�����t��center����ȷ����this->center;
						if (t.center == tmp_t)
						{
							//std::cout << "find core" << std::endl;
							this->center = tmp_this;
						}
						
					}//end of while 
				}//end of for
			}  //end of Body(Body& t)

			//test_ok
			~Body()
			{
				//std::cout << "in the destructor of body" << std::endl;
				Node *tmp;
				//1.���ź����������� ������������Ԫ�صĽڵ�
				for (int i = 0; i < BODY_RESERVE_X; i++)
				{
					 tmp = lateral_x[i];
					//std::cout <<i << std::endl;
					//�ҵ�����ĩβ�ڵ㣬��������					
					while (tmp->down != NULL)
					{
						//std::cout << tmp->down << std::endl;
						tmp = tmp->down;
					}
					
					if (tmp->up != NULL)
					{
						while (tmp != lateral_x[i])
						{
							tmp = tmp->up;
							delete tmp->down;
						}
					}
				}
				
				//2.����x��y���ָ������ָ����ڴ�
				for (int j = 0; j < BODY_RESERVE_X; j++)
				{
					delete lateral_x[j];
				}
				for (int k = 0; k < BODY_RESERVE_Y; k++)
				{
					delete vertical_y[k];
				}
				//std::cout << "the end of destructor of body" << std::endl;
			}
		
		public:
			//test_ok
			void init_bodycenter(int x, int y)
			{
				center->x = x;
				center->y = y;
			}

			//test_ok
			void new_point(int dx, int dy)
			{
				int d_x, d_y;
				Node* tmp = NULL;
				int cnt = 0;
				if ((fabs(dx) > BODY_RESERVE_X / 2) || fabs(dy) > BODY_RESERVE_Y / 2)
				{
					std::cout << "body_error ,out of range" << std::endl;
					exit(0);
				}
				//�����½ڵ������
				Node* p = new Node;
				p->x = center->x + dx;
				p->y = center->y + dy;

				//�½ڵ����ʮ������
				if (dx != 0 || dy != 0)
				{
					d_x = BODY_RESERVE_X / 2 + dx; //�кź��к����
					d_y = BODY_RESERVE_Y / 2 + dy;
			/*		if (dx != 0)
					{*/
						tmp = vertical_y[d_y];
						//�ҵ��½ڵ�p�ڸ����е���λ�ã������ڲ��ҵ��ұ��Ѿ�û��Ԫ����
						for (cnt = 0; cnt < fabs(d_x); cnt++)
						{
							if (tmp->rigth != NULL)
							{
								tmp = tmp->rigth;
							}
							else
							{
								break;
							}
						}
						//tmp�ƶ��������������ұߵ�һ��Ԫ��
						if (tmp != vertical_y[d_y])
						{
							if (tmp->rigth != NULL)
							{
								p->rigth = tmp->rigth;
								tmp->rigth->left = p;
								p->left = tmp;
								tmp->rigth = p;
							}
							else
							{
								p->rigth = NULL;
								tmp->rigth = p;
								p->left = tmp;
							}
						}
						else
						{
							p->rigth = NULL;
							tmp->rigth = p;
							p->left = tmp;
						}
					//}  //end of dx !=0
					//else   
					//{
					//	;
					//}

					//if (dy != 0)
					//{
						tmp = lateral_x[d_x];
						for (cnt = 0; cnt < fabs(d_y); cnt++)
						{
							if (tmp->down != NULL)
							{
								tmp = tmp->down;
							}
							else
							{
								break;
							}
						}
						if (tmp != lateral_x[d_x])
						{
							if (tmp->down != NULL)
							{
								p->down = tmp->down;
								tmp->down->up = p;
								p->up = tmp;
								tmp->down = p;
							}
							else
							{
								p->down = tmp->down;
								tmp->down = p;
								p->up = tmp;
							}
						}
						else
						{
							p->down = tmp->down;
							tmp->down = p;
							p->up = tmp;
						}
					//}  //end of if dy!=0

				}// x��y��ͬͬʱΪ0��if�жϽ���
				else
				{
					std::cout << "body_new_node ,param error x=0&&y=0" << std::endl;
					exit(0);
				}
			}

			//test_ok
			void remove_point(int dx, int dy)
			{
				int i=0;
				int d_x, d_y,rec=-1;

				//��tmp�ƶ���Ҫɾ���Ľڵ�λ��
				Node* tmp = NULL;
				Node* tmp2 = NULL;

				d_x = BODY_RESERVE_X / 2 + dx; //�кź��к����
				d_y = BODY_RESERVE_Y / 2 + dy;
				tmp = vertical_y[d_y];

				//˳�����������ƶ���һ��һ����Ҫɾ����Ԫ��һ���ڸ�������
				while (tmp->rigth != NULL)
				{
				
					tmp = tmp->rigth;
					tmp2 = tmp;
					//�����ƶ����ҵ������кţ��Ƿ���ϣ��������˳�����
					while (tmp2->up != NULL)
					{
						
						tmp2 = tmp2->up;
					}				
					rec = tmp2->x;
					if (rec == d_x)
					{
						break;
					}
				}

				//�޸�����
				if (tmp->rigth != NULL)
				{
					tmp->rigth->left = tmp->left;
					tmp->left->rigth = tmp->rigth;
				}
				else
				{
					tmp->left->rigth = NULL;
				}
				
				if (tmp->down != NULL)
				{
					tmp->up->down = tmp->down;
					tmp->down->up = tmp->up;
				}
				else
				{
					tmp->up->down = NULL;
				}				
				//std::cout << "test" << std::endl;
				delete tmp;
			}

			//test_ok
			void show_body()
			{
				int i = 0;
				Node* tmp = vertical_y[0];
				//���α���ʮ�������ÿһ��Ԫ��
				for (i = 0; i < BODY_RESERVE_Y; i++)
				{
					tmp = vertical_y[i];
					while (tmp->rigth != NULL)
					{
						//��ӡ��ǰ�еĻ���
						tmp = tmp->rigth;
						setpos(tmp->x, tmp->y);
						std::cout << "o";
					}
				}
				//��ӡ�ɻ�����,�������������ڲ�ͬ
				setpos(center->x, center->y);
				std::cout << "Y" << std::endl;
			}

			//test_ok
			void move(char towards)
			{
				Node* p = NULL;
				int i = 0;
				switch(towards)
				{
				case 'w':
					for (i = 0; i < BODY_RESERVE_X; i++)
					{
						p = lateral_x[i];
					
						while (p->down != NULL)
						{
							p = p->down;
							setpos(p->x, p->y);
							std::cout << " " << std::endl;
							p->y-=1;						
						}						
							//setpos(p->x, p->y+1);
							//std::cout << " " << std::endl;
					}
					break;
				case 's':
					for (i = 0; i < BODY_RESERVE_X; i++)
					{
						p = lateral_x[i];
				/*		if (p->down != NULL)
						{
							setpos(p->down->x, p->down->y);
							std::cout << " " << std::endl;
						}*/
						while (p->down != NULL)
						{
							p = p->down;
							setpos(p->x, p->y);
							std::cout << " " << std::endl;
							p->y += 1;

						}
					}
					break;
				case 'a':
					for (i = 0; i < BODY_RESERVE_Y; i++)
					{
						p = vertical_y[i];
						while (p->rigth != NULL)
						{
							p = p->rigth;
							setpos(p->x, p->y);
							std::cout << " " << std::endl;
							p->x-= 1;
						}						
				/*			setpos(p->x+1, p->y);
							std::cout << " " << std::endl;	*/				
					}
					break;
				case 'd':
					for (i = 0; i < BODY_RESERVE_Y; i++)
					{
						p = vertical_y[i];
						/*if (p->rigth != NULL)
						{
							setpos(p->rigth->x, p->rigth->y);
							std::cout << " " << std::endl;
						}*/
						while (p->rigth != NULL)
						{
							p = p->rigth;
							setpos(p->x, p->y);
							std::cout << " " << std::endl;
							p->x+= 1;
						}
					}
					break;
				default:
					;
				}
				
			}

			//�����ú���
			void show_data()
			{
				std::cout << "in_show_data" << std::endl;
				int i = 0;
				Node* tmp;
				//���α���ʮ�������ÿһ��Ԫ��
				for (i = 0; i < BODY_RESERVE_Y; i++)
				{
					tmp = vertical_y[i];
					while (tmp->rigth != NULL)
					{
						//��ӡ��ǰ�еĻ���
						tmp = tmp->rigth;
						std::cout << "x="<<tmp->x<<"y="<<tmp->y<<std::endl;
					}
				}
				std::cout << "end of_show_data" << std::endl;
			}

			point getcenter(void) 
			{
				point tmp;
				tmp.x = center->x;
				tmp.y = center->y;
				return tmp;
			}
	}; //end of class body;

};  //end of namespace syao

#endif // ! _BODY

