#pragma once

#include<iostream>
#include<ctime>
#include<cmath>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<set>
#include<map>

using namespace std;

struct node
{
	double bias;
	double sum_in;
	double sum;
	double out;
	set<int> targetLink;
	map<int, double> targetSign;
};
class brat
{
public:
	brat();
	brat(int input_num, int neuralNode, int out_num, uint32_t _score);
	~brat();

	void input_iterate(vector<double>& input);
	void brain_iterate();
	void output_iterate();


	vector<node> input;
	vector<node> output;

	uint32_t score;

	uint32_t blood;

	const double input2nodeMax  = 0.4; //ÿ�����������������Ԫ�ڵ�İٷֱ�
	const double node2nodeMax   = 0.1; //ÿ���ڵ�������������ڵ����İٷֱ�
	const double node2outputMax = 0.4; //ÿ����������������Ԫ�ڵ�İٷֱ�

	vector<pair<int, int>> pose;

	vector<node> brain;
private:
	double sigmod(double in);
};

