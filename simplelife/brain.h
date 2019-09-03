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


using namespace std;

//enum brainType { STRUCT_ONLY = 0, FULL_BRAIN = 1, };

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
	~Brain();

	void input_iterate(vector<double>& input);
	void brain_iterate();
	void output_iterate(vector<double>& output);
	inline uint32_t myRand();
	inline double myRand_1to1();
	void printLink();

	void reRandomWeight();
	void saveBrain(string filePath);

	vector<node> inputNode;
	vector<node> hideNode;
	vector<node> outputNode;

	uint64_t link_Count = 0;
	uint64_t score = 0;
	pcg32_random_t rng;

	//���¼�����ֵƾ�о�����
	const double valueKeep = 1.0 - 0.618; //���ؽڵ�ÿ�ε���������ԭȨֵ�ٷֱ� node.newSum = node.oldSum*0.xx + sumFromOthers*(1-0.xx);

	const double input2hideMax  = 0.15; //ÿ����������������ؽڵ�İٷֱ�
	const double hide2hideMax   = 0.18; //ÿ�����ؽڵ���������������ؽڵ����İٷֱ�
	const double hide2outputMax = 0.15; //ÿ���������������ؽڵ�İٷֱ�

private:
	inline double sigmod(double in);
	
};

