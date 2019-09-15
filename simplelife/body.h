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
#include "brain.h"

using namespace std;

class Body: public Brain
{
public:

	Body(Brain& parentBrain, variationType v_type, double ratio);
	Body(string filePath);
	Body(uint64_t input_num, uint64_t neuralNode, uint64_t out_num, uint64_t _score);
	~Body();


	double direction = 0;
	double speed = 0;

	//pair<double, double> pos;
	double pos_x = 1;
	double pos_y = 1;

};

