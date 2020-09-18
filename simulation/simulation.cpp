/* 变异全靠随机，看老天赏口饭吃
	JARK006@qq.com
	2020-9-18 16:31:04
*/


/*
找食物模拟
初始先使用 BrainNode parent(IN_NODE_NUM, HIDE_NODE_NUM, OUT_NODE_NUM, 0);创建随机大脑
然后变异成 POPULATION 个不同的大脑
看看有没有能主动找食物的，没有就关闭程序再来。。。一直循环（大概不超过10次就能找到）
如果有能主动找食物的，等待最后结果，保存最高分的大脑，其他的也可以保存

下次运行就使用 BrainNode parent("D:\\brain\\brain_1597140372_750.bin"); 构建大脑
然后变异成 POPULATION 个不同的大脑，不断迭代，可以得到找食物最积极的家伙
*/
#include <iostream>
#include <iomanip>
#include <chrono>
#include <mutex>
#include <thread>
#include <ctime>

#include <graphics.h>
#include <conio.h>
#include "brain_node.h"
#include "utils.h"


const int scoreInit = 50;
const int POPULATION = 64; //群体数量

//单体的输入输出神经元，隐藏神经元数量
const int IN_NODE_NUM  = 20;
const int OUT_NODE_NUM = 2;
const int HIDE_NODE_NUM = 500; 

const int mapX = 900, mapY = 900;
const int x_init =mapX / 2, y_init = mapY / 2;

struct body
{
	vector<double> in;
	vector<double> out;
	BrainNode* brain;
	double x = x_init, y = y_init;
	double xbk = x, ybk = y;

	int score = scoreInit;
	int scoreMax = scoreInit;
	bool isAlive = true;
	int id;
};

vector<body> baby(POPULATION);
int aliveCnt = POPULATION;


double x_target = 300;
double y_target = 300;

int main(int argc, char* argv[])
{
	using namespace std::chrono;
	cout << argv[0] << endl;
	mySrand();

	int id = 0;

	//BrainNode parent("D:\\brain\\brain_1597140372_750.bin");
	BrainNode parent(IN_NODE_NUM, HIDE_NODE_NUM, OUT_NODE_NUM, 0);

	for (auto& b : baby)
	{
		b.in.resize(IN_NODE_NUM);
		b.out.resize(OUT_NODE_NUM);

		for (auto& i : b.in)i = 0;
		b.score = 10000;
		b.id = id++;

		if (b.id == 0)
			b.brain = &parent;
		else
			b.brain = new BrainNode(parent, variationType::WEIGHT_CHANGE, 0.001);
	}


	x_target = (myRand() % (int)(mapX * 0.75)) + mapX * 0.125;
	y_target = (myRand() % (int)(mapY * 0.75)) + mapY * 0.125;


	initgraph(mapX, mapY, 1);
	setbkcolor(WHITE);
	setlinecolor(RED);
	int runtime = 100000;

	solidcircle(x_target, y_target, 5);
	while (runtime)
	{

		for (auto& b : baby) {
			if (!b.isAlive) continue;

			b.in[0] = b.x - x_target;
			b.in[1] = b.y - y_target;

			b.in[2] = b.x;
			b.in[3] = b.y;
			b.in[4] = mapX - b.x;
			b.in[5] = mapY - b.y;

			b.in[7] = b.score;

			if (b.in[0] >= 0 && b.in[0] < 1.0) b.in[0] = 1.0;
			else if (b.in[0] < 0 && b.in[0] > -1.0) b.in[0] = -1.0;

			if (b.in[1] >= 0 && b.in[1] < 1.0) b.in[1] = 1.0;
			else if (b.in[1] < 0 && b.in[1] > -1.0) b.in[1] = -1.0;

			b.brain->input_iterate(b.in);
			b.brain->inner_iterate();//隐藏节点间迭代几次都行
			b.brain->inner_iterate();
			//b.brain->inner_iterate();
			b.brain->output_iterate(b.out);

			b.xbk = b.x;
			b.ybk = b.y;
			b.x += b.out[0];
			b.y += b.out[1];

			if (b.x < 0)b.x = 0;
			else if (b.x > mapX) b.x = mapX - 1;
			if (b.y < 0) b.y = 0;
			else if (b.y > mapY) b.y = mapY - 1;

			if (b.x <= 0 || b.x >= mapX - 1 || b.y <= 0 || b.y >= mapY - 1) b.score-=10;
			if (b.score <= 0) {
				b.isAlive = false;
				aliveCnt--;
			}

			//某个体吃到食物后，产生新的食物位置
			if (fabs(b.x - x_target) < 1.0 && fabs(b.y - y_target) < 1.0)
			{
				setfillcolor(BLACK);
				solidcircle(x_target, y_target, 5);

				x_target = (myRand() % (int)(mapX * 0.75)) + mapX * 0.125;
				y_target = (myRand() % (int)(mapY * 0.75)) + mapY * 0.125;

				setfillcolor(RED);
				solidcircle(x_target, y_target, 5);

				b.score += 3000;
				cout << "ID:" << b.id << " +3000. Now:" << b.score << endl;

				/*for (auto& b : baby) {
					b.x = x_init;
					b.y = y_init;
				}*/
			}

			if (b.score > b.scoreMax) b.scoreMax = b.score;

			//b.score--; //走一步饥饿度减一

			setfillcolor(BLACK);
			solidcircle(b.xbk, b.ybk, 2);
			setfillcolor(WHITE);
			solidcircle(b.x, b.y, 2);
		}

		if (aliveCnt <= 0)
			break;
		if((runtime & 0x1ff) == 0)
		cout << runtime << endl;
		////Sleep(100);

		if (--runtime == 0) {
			cout << "Press [q] exit:" << endl;
			if (_getch() != 'q')
				runtime = 50000;

			int zeroCnt = 0;
			int scoreMax = 0;
			int idMax = 999;
			id = 0;
			for (auto& b : baby) {
				cout << "ID:" << b.id << " score:" << b.score << " scoreMax:" << b.scoreMax << endl;
				if (b.scoreMax <= scoreInit)
					zeroCnt++;

				if (b.scoreMax > scoreMax) {
					idMax = id;
					scoreMax = b.scoreMax;
				}
				id++;
			}

			cout << endl << "MaxScore:" << scoreMax << endl << endl;

		}
	}


	//if (zeroCnt == POPULATION)
	//	exit(-1);
	cout << "save something?(s/n)" << endl;

	char ch = _getch();
	if (ch == 's' || ch == 'S') {
		for (auto& b : baby) {
			cout << "ID:" << b.id << " score:" << b.score << " scoreMax:" << b.scoreMax;
			cout << " Press [s] to save:";

			char ch = _getch();
			if (ch == 's' || ch == 'S') {
				string filename = "D:\\brain\\brain_" + to_string(time(nullptr)) + "_" + to_string(b.scoreMax) + ".bin";
				b.brain->saveBrainNode(filename);
				cout << filename << " saved" << endl;
			}
			else {
				cout << "Don\'t save." << endl;
			}
		}
	}
	else {
		cout << "Save nothing." << endl;
	}
	closegraph();			// 关闭绘图窗口
	return 0;

}

