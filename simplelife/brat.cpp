#include "pch.h"
#include "brat.h"

double brat::sigmod(double in)
{
	return 1.0 / (1.0 + exp(-in));
}

brat::brat()
{
	brat(3, 100, 1, 0);
}

brat::brat(int input_num, int neuralNode, int output_num, uint32_t _score)
{
	input.resize(input_num);
	brain.resize(neuralNode);
	output.resize(output_num);
	score = _score;


	for (auto i = 0; i < input_num; i++) {
		int linkNum = rand() % (int)(input2nodeMax * neuralNode);
		while (linkNum--) {
			auto target = rand() % neuralNode;
			double sign = rand() > (RAND_MAX >> 1) ? 1.0 : (-1.0);
			input[i].targetLink.insert(target);
			input[i].targetSign.insert(pair<int, double>(target, sign));
		}
		//input[i].bias = ((rand() % 200) - 100) / 100.0;
	}

	for (auto i = 0; i < neuralNode; i++) {
		int linkNum = rand() % (int)(node2nodeMax * neuralNode);
		while (linkNum--) {
			auto target = rand() % neuralNode;
			double sign = rand() > (RAND_MAX >> 1) ? 1.0 : (-1.0);
			brain[i].targetLink.insert(target);
			brain[i].targetSign.insert(pair<int, double>(target, sign));
		}
		brain[i].bias = ((rand() % 200) - 100) / 100.0;
		brain[i].sum = ((rand() % 200) - 100) / 100.0;
		brain[i].out = sigmod(brain[i].sum);
	}

	for (auto i = 0; i < output_num; i++) {
		int linkNum = rand() % (int)(node2outputMax * neuralNode);
		while (linkNum--) {
			auto target = rand() % neuralNode;
			double sign = rand() > (RAND_MAX >> 1) ? 1.0 : (-1.0);
			output[i].targetLink.insert(target);
			output[i].targetSign.insert(pair<int, double>(target, sign));
		}
	}




}

brat::~brat()
{
}

void brat::input_iterate(vector<double>& _input)
{
	if (_input.size() < input.size())
	{
		cerr << "InputNum ERROR!" << endl;
		exit(-1);
	}
	for (auto i = 0; i < input.size(); i++) {
		input[i].sum = _input[i];
		input[i].out = sigmod(input[i].sum);
	}

	for (auto i = 0; i < brain.size(); i++)
	{
		brain[i].sum_in = 0;
	}

	for (auto i = 0; i < input.size(); i++) {
		for (auto it = input[i].targetLink.begin(); it != input[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = input[i].targetSign.find(*it);
			if (it2 != input[i].targetSign.end())
				weight = it2->second;
			brain[*it].sum_in += (input[i].out * weight);
		}
	}
}

void brat::brain_iterate()
{
	//vector<double> brainValue(brain.size());
	
	for (auto i=0; i < brain.size(); i++)
	{
		brain[i].out = sigmod(brain[i].sum + brain[i].sum_in);
		brain[i].sum = 0;
	}

	for (auto i = 0; i < brain.size(); i++) {
		for (auto it = brain[i].targetLink.begin(); it != brain[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = brain[i].targetSign.find(*it);
			if (it2 != brain[i].targetSign.end())
				weight = it2->second;
			brain[*it].sum += (brain[i].out * weight);
		}
	}

}

void brat::output_iterate()
{
	for (auto i = 0; i < output.size(); i++)
	{
		output[i].sum = 0;
	}

	for (auto i = 0; i < output.size(); i++) {
		for (auto it = output[i].targetLink.begin(); it != output[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = brain[i].targetSign.find(*it);
			if (it2 != brain[i].targetSign.end())
				weight = it2->second;
			output[i].sum += (brain[*it].out * weight);
		}
	}

	for (auto i = 0; i < output.size(); i++)
	{
		output[i].out = sigmod(output[i].sum);
	}
}
