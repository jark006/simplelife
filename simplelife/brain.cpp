
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


brain::brain(string filePath)
{
	auto t1 = time(nullptr);

	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)& printf,
		(intptr_t)& printf);

	uint32_t heardInfo[16] = { 0 };
	uint32_t temp;
	uint32_t temp2;
	double temp_double;
	uint32_t input_num, neuralNode, output_num;

	cout << "Load brain from file: \"" << filePath << '\"' << endl;
	ifstream f(filePath, ios::binary);
	if (!f)
	{
		cout << "Fail to read file!" << endl;
		exit(-1);
	}

	f.read((char*)& heardInfo, sizeof(heardInfo) / sizeof(heardInfo[0]));

	input_num = heardInfo[0];
	neuralNode = heardInfo[1];
	output_num = heardInfo[2];
	score = heardInfo[3];

	cout << "InputSize: " << input_num << endl;
	cout << "neuralSize: " << neuralNode << endl;
	cout << "OutputSize: " << output_num << endl;

	inputNode.resize(input_num);
	hideNode.resize(neuralNode);
	outputNode.resize(output_num);

	link_Count = 0;
	for (auto i = 0; i < inputNode.size(); i++) {
		f.read((char*)& temp_double, sizeof(double));
		inputNode[i].sum = temp_double;
		f.read((char*)& temp, sizeof(uint32_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(int));
			f.read((char*)& temp_double, sizeof(double));
			inputNode[i].target.push_back(pair<uint32_t, double>(temp2, temp_double));
			link_Count++;
		}
	}
	for (auto i = 0; i < hideNode.size(); i++) {
		f.read((char*)& temp_double, sizeof(double));
		hideNode[i].sum = temp_double;
		f.read((char*)& temp, sizeof(uint32_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(uint32_t));
			f.read((char*)& temp_double, sizeof(double));
			hideNode[i].target.push_back(pair<uint32_t, double>(temp2, temp_double));
			link_Count++;
		}
	}
	for (auto i = 0; i < outputNode.size(); i++) {
		f.read((char*)& temp_double, sizeof(double));
		outputNode[i].sum = temp_double;
		f.read((char*)& temp, sizeof(uint32_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(uint32_t));
			f.read((char*)& temp_double, sizeof(double));
			outputNode[i].target.push_back(pair<uint32_t, double>(temp2, temp_double));
			link_Count++;
		}
	}
	f.close();
	cout << "LinkNum:" << link_Count << endl;
	cout << "Load over." <<  endl;
	cout << "Time use:" << time(nullptr) - t1 << endl << endl;
}

brain::brain(uint32_t input_num, uint32_t neuralNode, uint32_t output_num, uint32_t _score)
{
	auto t1 = time(nullptr);
	set<uint32_t> targetLink;

	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)& printf,
		(intptr_t)& printf);

	cout << "Cread brain by random neural link." << endl;

	cout << "InputSize: " << input_num << endl;
	cout << "neuralSize: " << neuralNode << endl;
	cout << "OutputSize: " << output_num << endl;



	inputNode.resize(input_num);
	hideNode.resize(neuralNode);
	outputNode.resize(output_num);
	score = _score;

	link_Count = 0;
	for (uint32_t i = 0; i < input_num; i++) {
		int linkNum = myRand() % (int)(input2hideMax * neuralNode);
		if (linkNum == 0)
			linkNum = 1;
		targetLink.clear();
		while (linkNum--) {
			auto target = myRand() % neuralNode;
			if (target == i) {
				linkNum++;
				continue;
			}
			targetLink.insert(target);
			link_Count++;
		}
		for (auto it = targetLink.begin(); it != targetLink.end(); it++) {
			inputNode[i].target.push_back(pair<uint32_t, double>(*it, myRand_1to1()));
		}
	}

	for (uint32_t i = 0; i < neuralNode; i++) {
		int linkNum = myRand() % (int)(hide2hideMax * neuralNode);
		if (linkNum == 0)
			linkNum = 1;
		targetLink.clear();
		while (linkNum--) {
			auto target = myRand() % neuralNode;
			if (target == i) {
				linkNum++;
				continue;
			}
			targetLink.insert(target);
			link_Count++;
		}

		for (auto it = targetLink.begin(); it != targetLink.end(); it++) {
			hideNode[i].target.push_back(pair<uint32_t, double>(*it, myRand_1to1()));
		}

		hideNode[i].bias = myRand_1to1();
		hideNode[i].sum = myRand_1to1();
		hideNode[i].out = sigmod(hideNode[i].sum);
	}


	for (uint32_t i = 0; i < output_num; i++) {
		int linkNum = myRand() % (int)(hide2outputMax * neuralNode);
		if (linkNum == 0)
			linkNum = 1;
		targetLink.clear();
		while (linkNum--) {
			auto target = myRand() % neuralNode;
			if (target == i) {
				linkNum++;
				continue;
			}
			targetLink.insert(target);
			link_Count++;
		}
		for (auto it = targetLink.begin(); it != targetLink.end(); it++) {
			outputNode[i].target.push_back(pair<uint32_t, double>(*it, myRand_1to1()));
		}
	}
	cout << "LinkNum:" << link_Count << endl;
	cout << "Cread over." << endl;
	cout << "Time use:" << time(nullptr) - t1 << endl << endl;
}

brain::~brain()
{
}

void brain::input_iterate(vector<double>& input)
{
	if (input.size() < inputNode.size())
	{
		cerr << "InputNum ERROR!" << endl;
		exit(-1);
	}
	for (auto i = 0; i < inputNode.size(); i++) {
		inputNode[i].sum = input[i];
		inputNode[i].out = sigmod(inputNode[i].sum);
	}

	for (auto& h : hideNode)
	{
		h.sum_in = 0;
	}

	for (auto i:inputNode)
		for (auto t:i.target)
			hideNode[t.first].sum_in += (i.out * t.second);
}

void brain::brain_iterate()
{
	//valueKeep
	for (auto& h : hideNode)
	{
		h.out = sigmod(h.bias + h.sum + h.sum_in);
		h.sum_old = h.sum;
		h.sum = 0;
	}

	for (auto& h : hideNode) {
		for (auto t : h.target)
		{
			hideNode[t.first].sum += (h.out * t.second);
		}
	}

	for (auto& h : hideNode)
	{
		h.sum = h.sum_old * valueKeep + h.sum * (1.0 - valueKeep);
	}
}

void brain::output_iterate(vector<double>& output)
{
	for (auto& o : outputNode)
		o.sum = 0;

	for (auto& o : outputNode) {
		for (auto t : o.target) {
			o.sum += (hideNode[t.first].out * t.second);
		}
		o.out = sigmod(o.sum);
	}
	for (auto i = 0; i < outputNode.size(); i++)
		output[i] = outputNode[i].out;
}

void brain::printLink()
{
	int num = 0;
	for (auto i:inputNode) {
		cout << "input[" << num++ << "](" << i.out << "):";
		for (auto j:i.target) {
			cout << j.first << ' '; 
			//cout << j.first << '&' << j.second << ' ';
		}
		cout << endl;
	}
	cout << endl;

	num = 0;
	for (auto h:hideNode) {
		cout << "neural[" << num++ << "](" << h.out << "):";
		for (auto j : h.target) {
			cout << j.first << ' ';
		}
		cout << endl;
	}
	cout << endl;

	num = 0;
	for (auto o:outputNode) {
		cout << "output[" << num++ << "](" << o.out << "):";
		for (auto j : o.target) {
			cout << j.first  << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

void brain::reRandomWeight()
{
	for (auto i : inputNode) {
		for (auto j : i.target) {
			j.second = myRand_1to1();
		}
	}

	for (auto n : hideNode) {
		for (auto j : n.target) {
			j.second = myRand_1to1();
		}
		n.bias = myRand_1to1();
		n.sum = myRand_1to1();
		n.out = sigmod(n.sum);
	}

	for (auto o : outputNode) {
		for (auto j : o.target) {
			j.second = myRand_1to1();
		}
	}
}

void brain::saveBrain(string filePath)
{
	uint32_t temp;
	uint32_t heardInfo[16] = { 0 };
	double temp_double;

	ofstream f(filePath, ios::binary);
	if (!f)
	{
		cerr << "Save brain faile, outputfile error!" << endl;
		return;
	}

	heardInfo[0] = inputNode.size();
	heardInfo[1] = hideNode.size();
	heardInfo[2] = outputNode.size();
	heardInfo[3] = score;

	f.write((char*)& heardInfo, sizeof(heardInfo) / sizeof(heardInfo[0]));

	for (auto i : inputNode) {
		temp_double = i.sum;
		temp = i.target.size();
		f.write((char*)& temp_double, sizeof(double));
		f.write((char*)& temp, sizeof(uint32_t));
		for (auto j : i.target) {
			temp = j.first;
			temp_double = j.second;
			f.write((char*)& temp, sizeof(uint32_t));
			f.write((char*)& temp_double, sizeof(double));
		}
	}

	for (auto h : hideNode) {
		temp_double = h.sum;
		temp = h.target.size();
		f.write((char*)& temp_double, sizeof(double));
		f.write((char*)& temp, sizeof(uint32_t));
		for (auto j : h.target) {
			temp = j.first;
			temp_double = j.second;
			f.write((char*)& temp, sizeof(uint32_t));
			f.write((char*)& temp_double, sizeof(double));
		}
	}

	for (auto o : outputNode) {
		temp_double = o.sum;
		temp = o.target.size();
		f.write((char*)& temp_double, sizeof(double));
		f.write((char*)& temp, sizeof(uint32_t));
		for (auto j : o.target) {
			temp = j.first;
			temp_double = j.second;
			f.write((char*)& temp, sizeof(uint32_t));
			f.write((char*)& temp_double, sizeof(double));
		}
	}

	f.close();
}
