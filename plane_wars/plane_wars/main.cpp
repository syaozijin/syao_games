#include<iostream>
#include"body.h"
#include"map.h"
#include"bullet.h"
#include"userplane.h"
#include"bullet_manage.h"
#include"enemyp_maenage.h"
#include"common.h"
#include<thread>

int main()
{
	using namespace std;
	using namespace syao;
	//����ûɶ�õ��Ǻÿ��ĵ�ͼ
	Map map;
	map.map_init();

	//������ҷɻ����Ĳ���
	Boer wenbohao;
	wenbohao.boer_init();

	//���ɵз��ɻ�������
	enemyP_queue p1;

	//�����ӵ�������
	BManager  p2;

	//3���߳�
	thread wenbo(wenbohao);
	thread enemy(p1);
	thread bulletmanage(p2);

	wenbo.join();
	enemy.join();
	bulletmanage.join();
		

	return 0;
}