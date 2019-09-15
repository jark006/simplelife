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
#include "utils.h"

using namespace std;

//变异类型
enum variationType {
	//INPUT_NODE_ADD,  //输入节点增加
	//INPUT_NODE_SUB,  //输入节点减少
	//OUTPUT_NODE_ADD, //输出节点增加
	//OUTPUT_NODE_SUB, //输出节点减少
	HIDE_NODE_ADD, //隐藏节点增加
	//HIDE_NODE_SUB, //隐藏节点减少
	LINK_ADD,      //连接目标增加
	LINK_SUB,      //连接目标减少
	WEIGHT_CHANGE, //权重增减
};

struct Node
{
	uint64_t id = 0;
	double bias = 0;
	double sum_in = 0;
	double sum = 0;
	double sum_old = 0;
	double out = 0;
	vector<pair<uint64_t, double>> target; //目标节点及权值
};

struct BrainFileHeader //128 bytes
{
	uint32_t mainVer_brain;
	uint32_t subVer_brain;
	uint32_t mainVer_body;
	uint32_t subVer_body;

	uint64_t inputNodeSize;
	uint64_t hideNodeSize;
	uint64_t outputNodeSize;
	uint64_t score;

	uint64_t reserve[10];

};

class Brain
{
public:
	Brain(string filePath);
	Brain(uint64_t input_num, uint64_t neuralNode, uint64_t out_num, uint64_t _score);
	Brain(Brain& parentBrain, variationType v_type, double ratio);
	~Brain();

	void input_iterate(vector<double>& input);
	void brain_iterate();
	void output_iterate(vector<double>& output);
	void printLink();

	void reRandomWeight();
	void saveBrain(string filePath);


private:
	vector<Node> inputNode;
	vector<Node> hideNode;
	vector<Node> outputNode;

	uint64_t link_Count = 0;
	uint64_t score = 0;

	//以下几个数值凭感觉来的
	const double valueKeep = 1.0 - 0.618; //隐藏节点每次迭代保留的原权值比例 node.newSum = node.oldSum*0.xx + sumFromOthers*(1-0.xx);

	const double input2hideMax = 0.15; //默认每个输入最多连接隐藏节点的比例
	const double hide2hideMax = 0.18; //默认每个隐藏节点最多连接其他隐藏节点数的比例
	const double hide2outputMax = 0.15; //默认每个输出最多连接隐藏节点的比例

	const double deltaWeight = 0.0001; //权重变化值



};

