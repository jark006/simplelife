#pragma once

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
#include "pcg_basic.h"


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
class brain
{
public:
	brain();
	brain(string filename);
	brain(int input_num, int neuralNode, int out_num, uint32_t _score);
	~brain();

	void input_iterate(vector<double>& _input);
	void brain_iterate();
	void output_iterate(vector<double>& _output);
	inline uint32_t myRand();
	inline double myRand_1to1();
	void printLink();

	void reRandomWeight();
	void saveBrain(string filename);

	vector<node> input;
	vector<node> neural;
	vector<node> output;

	uint32_t score;
	uint32_t blood;
	pcg32_random_t rng;
	
	const double input2nodeMax  = 0.35; //每个输入最多连接总神经元节点的百分比
	const double node2nodeMax   = 0.55; //每个节点最多连接其他节点数的百分比
	const double node2outputMax = 0.42; //每个输出最多连接总神经元节点的百分比

private:
	inline double sigmod(double in);
	
};

