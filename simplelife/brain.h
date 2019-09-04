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
#include<iomanip>
#include<fstream>
#include "pcg_basic.h"
#include "utils.h"

using namespace std;

//变异类型
enum variationType { 
	HIDE_NODE_CHANGE, //隐藏节点增减
	LINK_CHANGE,      //连接目标增减
	WEIGHT_CHANGE,    //权重增减

};

struct node
{
	double bias = 0;
	double sum_in = 0;
	double sum = 0;
	double sum_old = 0;
	double out = 0;
	vector<pair<uint64_t, double>> target;
};
class Brain
{
public:
	Brain(string filePath);
	Brain(uint64_t input_num, uint64_t neuralNode, uint64_t out_num, uint64_t _score);
	//Brain(Brain& parentBrain, variationType v_type, double ratio);
	~Brain();

	void input_iterate(vector<double>& input);
	void brain_iterate();
	void output_iterate(vector<double>& output);
	//inline uint32_t myRand();
	//inline double myRand_1to1();
	void printLink();

	void reRandomWeight();
	void saveBrain(string filePath);

	vector<node> inputNode;
	vector<node> hideNode;
	vector<node> outputNode;

	uint64_t link_Count = 0;
	uint64_t score = 0;
	//pcg32_random_t rng;

	//以下几个数值凭感觉来的
	const double valueKeep = 1.0 - 0.618; //隐藏节点每次迭代保留的原权值百分比 node.newSum = node.oldSum*0.xx + sumFromOthers*(1-0.xx);

	const double input2hideMax  = 0.15; //每个输入最多连接隐藏节点的百分比
	const double hide2hideMax   = 0.18; //每个隐藏节点最多连接其他隐藏节点数的百分比
	const double hide2outputMax = 0.15; //每个输出最多连接隐藏节点的百分比

private:
	//inline double sigmod(double in);
	
};

