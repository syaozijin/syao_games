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
	//生成没啥用但是好看的地图
	Map map;
	map.map_init();

	//生成玩家飞机，文博号
	Boer wenbohao;
	wenbohao.boer_init();

	//生成敌方飞机管理类
	enemyP_queue p1;

	//生成子弹管理类
	BManager  p2;

	//3个线程
	thread wenbo(wenbohao);
	thread enemy(p1);
	thread bulletmanage(p2);

	wenbo.join();
	enemy.join();
	bulletmanage.join();
		

	return 0;
}