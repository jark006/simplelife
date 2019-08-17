#include "pch.h"
#include "brain.h"

inline double brain::sigmod(double in)
{
	return 1.0 / (1.0 + exp(-in));
}

inline uint32_t brain::myRand()
{
	return pcg32_random_r(&rng);
}

inline double brain::myRand_1to1()
{
	return ((int)(pcg32_random_r(&rng) & 0xffff) - 0x7fff) / ((double)0x7fff);
}

brain::brain()
{
	brain(2, 20, 1, 0);
}

brain::brain(string filename)
{
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf,
		(intptr_t)&printf);

	size_t temp;
	int temp_int;
	double temp_double;
	size_t input_num, neuralNode, output_num;

	cout << "Load brain from file: \"" << filename << '\"' << endl;
	ifstream f(filename, ios::binary);
	if (!f)
	{
		cout << "Fail to read file!" << endl;
		exit(-1);
	}

	f.read((char*)&input_num, sizeof(size_t));
	f.read((char*)&neuralNode, sizeof(size_t));
	f.read((char*)&output_num, sizeof(size_t));
	f.read((char*)&temp, sizeof(size_t));

	cout << "InputSize: " << input_num << endl;
	cout << "neuralSize: " << neuralNode << endl;
	cout << "OutputSize: " << output_num << endl << endl;

	input.resize(input_num);
	neural.resize(neuralNode);
	output.resize(output_num);
	score = temp;

	for (auto i = 0; i < input.size(); i++) {
		f.read((char*)&temp, sizeof(size_t));
		while (temp--)
		{
			f.read((char*)&temp_int, sizeof(int));
			f.read((char*)&temp_double, sizeof(double));

			input[i].targetLink.insert(temp_int);
			input[i].targetSign.insert(pair<int, double>(temp_int, temp_double));
		}
	}
	for (auto i = 0; i < neural.size(); i++) {
		f.read((char*)&temp, sizeof(size_t));
		while (temp--)
		{
			f.read((char*)&temp_int, sizeof(int));
			f.read((char*)&temp_double, sizeof(double));

			neural[i].targetLink.insert(temp_int);
			neural[i].targetSign.insert(pair<int, double>(temp_int, temp_double));
		}
	}
	for (auto i = 0; i < output.size(); i++) {
		f.read((char*)&temp, sizeof(size_t));
		while (temp--)
		{
			f.read((char*)&temp_int, sizeof(int));
			f.read((char*)&temp_double, sizeof(double));

			output[i].targetLink.insert(temp_int);
			output[i].targetSign.insert(pair<int, double>(temp_int, temp_double));
		}
	}
	f.close();
}

brain::brain(int input_num, int neuralNode, int output_num, uint32_t _score)
{

	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf,
		(intptr_t)&printf);

	cout << "Cread brain by random neural link." << endl;

	cout << "InputSize: " << input_num << endl;
	cout << "neuralSize: " << neuralNode << endl;
	cout << "OutputSize: " << output_num << endl << endl;



	input.resize(input_num);
	neural.resize(neuralNode);
	output.resize(output_num);
	score = _score;


	for (auto i = 0; i < input_num; i++) {
		int linkNum = myRand() % (int)(input2nodeMax * neuralNode);
		if (linkNum == 0)
			linkNum = 1;
		while (linkNum--) {
			auto target = myRand() % neuralNode;
			double weight = myRand_1to1();
			if (target == i) {
				linkNum++;
				continue;
			}
			input[i].targetLink.insert(target);
			input[i].targetSign.insert(pair<int, double>(target, weight));
		}
	}

	for (auto i = 0; i < neuralNode; i++) {
		int linkNum = myRand() % (int)(node2nodeMax * neuralNode);
		if (linkNum == 0)
			linkNum = 1;
		while (linkNum--) {
			auto target = myRand() % neuralNode;
			double weight = myRand_1to1();
			if (target == i) {
				linkNum++;
				continue;
			}
			neural[i].targetLink.insert(target);
			neural[i].targetSign.insert(pair<int, double>(target, weight));
		}
		neural[i].bias = myRand_1to1();
		neural[i].sum = myRand_1to1();
		neural[i].out = sigmod(neural[i].sum);
	}

	for (auto i = 0; i < output_num; i++) {
		int linkNum = myRand() % (int)(node2outputMax * neuralNode);
		if (linkNum == 0)
			linkNum = 1;
		while (linkNum--) {
			auto target = myRand() % neuralNode;
			double weight = myRand_1to1();
			if (target == i) {
				linkNum++;
				continue;
			}
			output[i].targetLink.insert(target);
			output[i].targetSign.insert(pair<int, double>(target, weight));
		}
	}
}

brain::~brain()
{
}

void brain::input_iterate(vector<double>& _input)
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

	for (auto i = 0; i < neural.size(); i++)
	{
		neural[i].sum_in = 0;
	}

	for (auto i = 0; i < input.size(); i++) {
		for (auto it = input[i].targetLink.begin(); it != input[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = input[i].targetSign.find(*it);
			if (it2 != input[i].targetSign.end())
				weight = it2->second;
			neural[*it].sum_in += (input[i].out * weight);
		}
	}
}

void brain::brain_iterate()
{	
	for (auto i=0; i < neural.size(); i++)
	{
		neural[i].out = sigmod(neural[i].bias + neural[i].sum + neural[i].sum_in);
		neural[i].sum = 0;
	}

	for (auto i = 0; i < neural.size(); i++) {
		for (auto it = neural[i].targetLink.begin(); it != neural[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = neural[i].targetSign.find(*it);
			if (it2 != neural[i].targetSign.end())
				weight = it2->second;
			neural[*it].sum += (neural[i].out * weight);
		}
	}

}

void brain::output_iterate(vector<double>& _output)
{
	for (auto i = 0; i < output.size(); i++)
	{
		output[i].sum = 0;
	}

	for (auto i = 0; i < output.size(); i++) {
		for (auto it = output[i].targetLink.begin(); it != output[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = neural[i].targetSign.find(*it);
			if (it2 != neural[i].targetSign.end())
				weight = it2->second;
			output[i].sum += (neural[*it].out * weight);
		}
	}

	for (auto i = 0; i < output.size(); i++)
	{
		output[i].out = sigmod(output[i].sum);
		_output[i] = output[i].out;
	}
}

void brain::printLink()
{
	for (auto i = 0; i < input.size(); i++) {
		cout << "input[" << i << "]:";
		for (auto it = input[i].targetLink.begin(); it != input[i].targetLink.end(); it++) {
			cout << *it << ' ';
		}
		cout << endl;
	}
	cout << endl;
	for (auto i = 0; i < neural.size(); i++) {
		cout << "neural[" << i << "]:";
		for (auto it = neural[i].targetLink.begin(); it != neural[i].targetLink.end(); it++) {
			cout << *it << ' ';
		}
		cout << endl;
	}
	cout << endl;
	for (auto i = 0; i < output.size(); i++) {
		cout << "output[" << i << "]:";
		for (auto it = output[i].targetLink.begin(); it != output[i].targetLink.end(); it++) {
			cout << *it << ' ';
		}
		cout << endl;
	}

}

void brain::reRandomWeight()
{
	for (auto i = 0; i < input.size(); i++) {
		for (auto it = input[i].targetSign.begin(); it != input[i].targetSign.end(); it++) {
			it->second = myRand_1to1();
		}
	}

	for (auto i = 0; i < neural.size(); i++) {
		for (auto it = neural[i].targetSign.begin(); it != neural[i].targetSign.end(); it++) {
			it->second = myRand_1to1();
		}
		neural[i].bias = myRand_1to1();
		neural[i].sum = myRand_1to1();
		neural[i].out = sigmod(neural[i].sum);
	}

	for (auto i = 0; i < output.size(); i++) {
		for (auto it = output[i].targetSign.begin(); it != output[i].targetSign.end(); it++) {
			it->second = myRand_1to1();
		}
	}
}

void brain::saveBrain(string filename)
{
	size_t temp;
	int temp_int;
	double temp_double;
	ofstream f(filename, ios::binary);
	if (!f)
	{
		cout << "创建文件失败" << endl;
		return;
	}
	temp = input.size();
	f.write((char*)&temp, sizeof(size_t));
	temp = neural.size();
	f.write((char*)&temp, sizeof(size_t));
	temp = output.size();
	f.write((char*)&temp, sizeof(size_t));
	temp = score;
	f.write((char*)&temp, sizeof(size_t));

	
	for (auto i = 0; i < input.size(); i++) {
		temp = input[i].targetLink.size();
		f.write((char*)&temp, sizeof(size_t));
		for (auto it = input[i].targetLink.begin(); it != input[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = input[i].targetSign.find(*it);
			if (it2 != input[i].targetSign.end())
				weight = it2->second;

			temp_int = *it;
			temp_double = weight;

			f.write((char*)&temp_int, sizeof(int));
			f.write((char*)&temp_double, sizeof(double));
		}
	}

	for (auto i = 0; i < neural.size(); i++) {
		temp = neural[i].targetLink.size();
		f.write((char*)&temp, sizeof(size_t));
		for (auto it = neural[i].targetLink.begin(); it != neural[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = neural[i].targetSign.find(*it);
			if (it2 != neural[i].targetSign.end())
				weight = it2->second;

			temp_int = *it;
			temp_double = weight;

			f.write((char*)&temp_int, sizeof(int));
			f.write((char*)&temp_double, sizeof(double));
		}
	}

	for (auto i = 0; i < output.size(); i++) {
		temp = output[i].targetLink.size();
		f.write((char*)&temp, sizeof(size_t));
		for (auto it = output[i].targetLink.begin(); it != output[i].targetLink.end(); it++) {
			double weight = 0;
			auto it2 = output[i].targetSign.find(*it);
			if (it2 != output[i].targetSign.end())
				weight = it2->second;

			temp_int = *it;
			temp_double = weight;

			f.write((char*)&temp_int, sizeof(int));
			f.write((char*)&temp_double, sizeof(double));
		}
	}


	f.close();
}
