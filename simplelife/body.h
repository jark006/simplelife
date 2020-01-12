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
#include "brain_node.h"

using namespace std;

class Body //: public BrainNode
{
public:

	//Body(BrainNode& parentBrain, variationType v_type, double ratio);
	//Body(string filePath);
	//Body(uint64_t input_num, uint64_t neuralNode, uint64_t out_num, uint64_t _score);
	Body();
	~Body();


	double direction = 0;
	double speed = 0;

	//pair<double, double> pos;
	double pos_x = 1;
	double pos_y = 1;

	uint64_t x = 0;
	uint64_t y = 0;

};

