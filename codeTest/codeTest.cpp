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
int main()
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

int maingg()
{
    std::cout << "Hello World!\n";
	
	//vector<pair<uint32_t, double>> a;
	//set<uint32_t> c;
	//map<uint32_t, double> b;

	//int t1;
	//int i;
	//int n = 100000000;

	//t1 = time(nullptr);
	//i = n;
	//while (i--)
	//	a.push_back(pair<uint32_t, double>(i, i));
	//cout << time(nullptr) - t1 << endl;

	//t1 = time(nullptr);
	//i = n;
	//while (i--)
	//	b.insert(pair<uint32_t, double>(i, i));
	//cout << time(nullptr) - t1 << endl;

	//t1 = time(nullptr);
	//i = n;
	//while (i--)
	//	c.insert( i );
	//cout << time(nullptr) - t1 << endl;


	//int y = 0;
	//vector<int> aa(10);
	//for (auto i : aa)
	//{
	//	cout << i;
	//}
	//for (auto i : aa)
	//{
	//	i = 66;
	//}
	//for (auto i : aa)
	//{
	//	cout << i;
	//}

	int p = 0;
	int len = 25;
	for (int i = 0; i <= 100; i++) {

		cout << "\r[";
		for (int j = 0; j < len; j++) {
			if (j < (len * i / 100))
				cout << '=';
			else if (j == (len * i / 100))
				cout << '>';
			else
				cout << ' ';
		}
		cout << "]" << i << "%";
		Sleep(100);
	}

}


