// codeTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<ctime>
#include<cmath>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<string>
#include<set>
#include<map>
#include <fstream>
#include <windows.h>

#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
using namespace std;



typedef struct
{
	int x;
	int y;
} dot;
double len(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}
vector<dot> Dot;

void genrateRoundPoint(int r)
{
	int R = 2 * r + 1;
	vector< vector<char> > mat(R, vector<char>(R, 0));
	mat[r][r] = 1;
	int num = 3.1416 * R * R / 4.0;
	while (num--)
	{
		double min = 1.414 * r;//len(0,0,r,r);
		int  minx = R, miny = R;
		for (int i = 0; i < R; i++)
			for (int j = 0; j < R; j++)
			{
				if (mat[i][j])
					continue;
				double l = len(i, j, r, r);
				if (l <= min)
				{
					//cout << "len " << l << " i " << i << " j " << j << endl;
					min = l;
					minx = i;
					miny = j;
				}
			}
		//cout << "********************"<<endl;
		mat[minx][miny] = 1;
		dot temp = { minx,miny };
		Dot.push_back(temp);
	}
}
int maing()
{
	int r = 25;
	genrateRoundPoint(r);
	cout << Dot.size() << endl;
	//ofstream f("round.txt", ios::trunc);

	//for (int i = 0, j = 0; i < Dot.size(); i++, j++)
	//{
	//	if (j >= 10)
	//	{
	//		j = 0;
	//		f << endl;
	//	}
	//	f << "{" << Dot[i].x - r << "," << Dot[i].y - r << '}' << ", ";
	//}
	//f.close();
}

// condition_variable example


vector<std::mutex> mtx_notify(10);
std::mutex mtx2;
std::condition_variable cv_main2thread;
std::condition_variable cv_thread2main;
bool ready2main = false;
bool ready2thread = false;

vector<bool> ready(10, false);


void print_id(int id) {
	while (true)
	{


		std::unique_lock<std::mutex> lck(mtx_notify[id]);
		while (!ready2thread) cv_main2thread.wait(lck);
		// ...
		this_thread::sleep_for(std::chrono::seconds(id));
		std::cout << "thread " << id << '\n';




	}
}

//void go() {
//	//std::unique_lock<std::mutex> lck(mtx_notify[0]);
//	ready2main = true;
//	cv_main2thread.notify_all();
//}

int main()
{
	std::thread threads[10];
	// spawn 10 threads:
	for (int i = 0; i < 10; ++i)
		threads[i] = std::thread(print_id, i);

	std::cout << "10 threads ready to race...\n";
	//go();                       // go!

	while (true)
	{

		for (auto& b : ready)
			b = true;

		ready2thread = true;
		std::cout << "10 threads ready to race...\n";
		cv_main2thread.notify_all();



	}


	for (auto& th : threads) th.join();

	return 0;
}

