
#include "brain.h"

//inline double Brain::sigmod(double in)
//{
//	return 1.0 / (1.0 + exp(-in));
//}
//
//inline uint32_t Brain::myRand()
//{
//	return pcg32_random_r(&rng);
//}
//
//inline double Brain::myRand_1to1()
//{
//	return ((int)(pcg32_random_r(&rng) & 0xffff) - 0x7fff) / ((double)0x7fff);
//}


Brain::Brain(string filePath)
{
	auto t1 = time(nullptr);
	uint64_t heardInfo[16] = { 0 };
	uint64_t temp;
	uint64_t temp2;
	double temp_double;
	uint64_t input_num, neuralNode, output_num;

	cout << "Load brain from file: \"" << filePath << '\"' << endl;
	ifstream f(filePath, ios::binary);
	if (!f)
	{
		cout << "Fail to read file!" << endl;
		exit(-1);
	}

	f.read((char*)& heardInfo, sizeof(heardInfo));

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
	for (auto& i:inputNode) {
		f.read((char*)& temp_double, sizeof(double));
		i.sum = temp_double;
		f.read((char*)& temp, sizeof(uint64_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(uint64_t));
			f.read((char*)& temp_double, sizeof(double));
			i.target.push_back(pair<uint64_t, double>(temp2, temp_double));
			link_Count++;
		}
	}
	for (auto& h:hideNode) {
		f.read((char*)& temp_double, sizeof(double));
		h.sum = temp_double;
		f.read((char*)& temp, sizeof(uint64_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(uint64_t));
			f.read((char*)& temp_double, sizeof(double));
			h.target.push_back(pair<uint64_t, double>(temp2, temp_double));
			link_Count++;
		}
	}
	for (auto& o:outputNode) {
		f.read((char*)& temp_double, sizeof(double));
		o.sum = temp_double;
		f.read((char*)& temp, sizeof(uint64_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(uint64_t));
			f.read((char*)& temp_double, sizeof(double));
			o.target.push_back(pair<uint64_t, double>(temp2, temp_double));
			link_Count++;
		}
	}
	f.close();
	cout << "LinkNum:" << link_Count << endl;
	cout << "Load over." <<  endl;
	cout << "Time use:" << time(nullptr) - t1 << endl << endl;
}

Brain::Brain(uint64_t input_num, uint64_t neuralNode, uint64_t output_num, uint64_t _score)
{
	auto t1 = time(nullptr);
	set<uint64_t> targetLink;

	cout << "Cread brain by random neural link." << endl;

	cout << "InputSize: " << input_num << endl;
	cout << "neuralSize: " << neuralNode << endl;
	cout << "OutputSize: " << output_num << endl;



	inputNode.resize(input_num);
	hideNode.resize(neuralNode);
	outputNode.resize(output_num);
	score = _score;

	link_Count = 0;
	for (auto i = 0; i < input_num; i++) {
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
		}
		for (auto it = targetLink.begin(); it != targetLink.end(); it++) {
			inputNode[i].target.push_back(pair<uint64_t, double>(*it, myRand_1to1()));
		}
		link_Count += targetLink.size();
	}

	for (auto i = 0; i < neuralNode; i++) {
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
		}

		for (auto it = targetLink.begin(); it != targetLink.end(); it++) {
			hideNode[i].target.push_back(pair<uint64_t, double>(*it, myRand_1to1()));
		}
		link_Count += targetLink.size();

		hideNode[i].bias = myRand_1to1();
		hideNode[i].sum = myRand_1to1();
		hideNode[i].out = sigmod(hideNode[i].sum + hideNode[i].sum_in + hideNode[i].bias);
	}


	for (auto i = 0; i < output_num; i++) {
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
		}
		for (auto it = targetLink.begin(); it != targetLink.end(); it++) {
			outputNode[i].target.push_back(pair<uint64_t, double>(*it, myRand_1to1()));
		}
		link_Count += targetLink.size();
	}

	cout << "LinkNum:" << link_Count << endl;
	cout << "Cread over." << endl;
	cout << "Time use:" << time(nullptr) - t1 << endl << endl;
}

/*
parentBrain: 父母大脑
variationType: 变异类型
	HIDE_NODE_CHANGE, //隐藏节点增减 TODO 目前只增不减
	LINK_CHANGE,      //连接目标增减
	WEIGHT_CHANGE,    //权重增减
ratio:变异比例
*/
Brain::Brain(Brain& parentBrain, variationType v_type, double ratio)
{

	inputNode.assign(parentBrain.inputNode.begin(), parentBrain.inputNode.end());
	hideNode.assign(parentBrain.hideNode.begin(), parentBrain.hideNode.end());
	outputNode.assign(parentBrain.outputNode.begin(), parentBrain.outputNode.end());
	
	score = parentBrain.score;
	link_Count = parentBrain.link_Count;


	Node newNode;
	set<uint64_t> targetLink;


	switch (v_type) //TODO
	{
	case HIDE_NODE_CHANGE:
		int link_Count_temp = 0;
		int addNum = myRand() % (int)(hideNode.size() * ratio) + 1; //新增隐藏节点数量
		int sizeBefore = hideNode.size();
		int sizeAfter = sizeBefore + addNum;
		for (int i = 0; i < addNum; i++) {
			newNode.bias = myRand_1to1();
			newNode.sum = myRand_1to1();
			//newNode.out = sigmod(newNode.sum + newNode.sum_in + newNode.bias);
			hideNode.push_back(newNode);
		}

		for (auto& i : inputNode) {
			for (auto idx = sizeBefore; idx < sizeAfter; idx++) {
				if (myRand_0to1() < hide2hideMax)
				{
					i.target.push_back(pair<uint64_t, double>(idx, myRand_1to1() / 10.0));
					link_Count_temp++;
				}
			}
		}
		for (auto idx = 0; idx < hideNode.size(); idx++) {
			if (idx < sizeBefore) {
				for (auto idx2 = sizeBefore; idx2 < sizeAfter; idx2++) {
					if (myRand_0to1() < hide2hideMax)
					{
						hideNode[idx].target.push_back(pair<uint64_t, double>(idx2, myRand_1to1() / 10.0));
						link_Count_temp++;
					}
				}
			}
			else {
				for (auto idx2 = 0; idx2 < sizeBefore; idx2++) {
					if (myRand_0to1() < hide2hideMax)
					{
						hideNode[idx].target.push_back(pair<uint64_t, double>(idx2, myRand_1to1() / 10.0));
						link_Count_temp++;
					}
				}
			}
		}

		for (auto& o : outputNode) {
			for (auto idx = sizeBefore; idx < sizeAfter; idx++) {
				if (myRand_0to1() < hide2hideMax)
				{
					o.target.push_back(pair<uint64_t, double>(idx, myRand_1to1() / 10.0));
					link_Count_temp++;
				}
			}
		}

		link_Count += link_Count_temp;

		break;
	//case LINK_CHANGE:



	//	break;
	//case WEIGHT_CHANGE:

	//	break;
	//default:
	//	break;
	}
}

Brain::~Brain()
{
}

void Brain::input_iterate(vector<double>& input)
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

void Brain::brain_iterate()
{
	//valueKeep
	for (auto& h : hideNode)
	{
		h.out = sigmod(h.bias + h.sum + h.sum_in);
		h.sum_old = h.sum;
		h.sum = 0;
	}

	for (auto h : hideNode) 
		for (auto t : h.target)
			hideNode[t.first].sum += (h.out * t.second);

	for (auto& h : hideNode)
		h.sum = h.sum_old * valueKeep + h.sum * (1.0 - valueKeep);
}

void Brain::output_iterate(vector<double>& output)
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

void Brain::printLink()
{
	int num = 0;
	for (auto i : inputNode) {
		cout << "input[" << num++ << "](" << i.out << "):";
		for (auto t : i.target) {
			cout << t.first << ' ';
			//cout << j.first << '&' << j.second << ' ';
		}
		cout << endl;
	}
	cout << endl;

	num = 0;
	for (auto h : hideNode) {
		cout << "hideNode[" << num++ << "](" << h.out << "):";
		for (auto t : h.target) {
			cout << t.first << ' ';
		}
		cout << endl;
	}
	cout << endl;

	num = 0;
	for (auto o : outputNode) {
		cout << "output[" << num++ << "](" << o.out << "):";
		for (auto t : o.target) {
			cout << t.first << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

void Brain::reRandomWeight()
{
	for (auto& i : inputNode) 
		for (auto& t : i.target) 
			t.second = myRand_1to1();

	for (auto& h : hideNode) {
		for (auto& t : h.target) {
			t.second = myRand_1to1();
		}
		h.bias = myRand_1to1();
		h.sum = myRand_1to1();
		h.out = sigmod(h.sum);
	}

	for (auto& o : outputNode) 
		for (auto& t : o.target) 
			t.second = myRand_1to1();
}

void Brain::saveBrain(string filePath)
{
	uint64_t temp;
	uint64_t heardInfo[16] = { 0 };
	double temp_double;
	uint64_t link = 0;
	uint64_t persent = 0;

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

	f.write((char*)& heardInfo, sizeof(heardInfo));

	cout << "0%...";
	for (auto i : inputNode) {
		temp_double = i.sum;
		temp = i.target.size();
		f.write((char*)& temp_double, sizeof(double));
		f.write((char*)& temp, sizeof(uint64_t));
		for (auto t : i.target) {
			temp = t.first;
			temp_double = t.second;
			f.write((char*)& temp, sizeof(uint64_t));
			f.write((char*)& temp_double, sizeof(double));
			link++;
			if ((int)(100 * link / link_Count) != persent)
			{
				persent = (100 * link / link_Count);
				cout << '\r' << persent << "%...";
			}
		}
	}

	for (auto h : hideNode) {
		temp_double = h.sum;
		temp = h.target.size();
		f.write((char*)& temp_double, sizeof(double));
		f.write((char*)& temp, sizeof(uint64_t));
		for (auto t : h.target) {
			temp = t.first;
			temp_double = t.second;
			f.write((char*)& temp, sizeof(uint64_t));
			f.write((char*)& temp_double, sizeof(double));
			link++;
			if ((int)(100 * link / link_Count) != persent)
			{
				persent = (100 * link / link_Count);
				cout << '\r' << persent << "%...";
			}
		}
	}

	for (auto o : outputNode) {
		temp_double = o.sum;
		temp = o.target.size();
		f.write((char*)& temp_double, sizeof(double));
		f.write((char*)& temp, sizeof(uint64_t));
		for (auto t : o.target) {
			temp = t.first;
			temp_double = t.second;
			f.write((char*)& temp, sizeof(uint64_t));
			f.write((char*)& temp_double, sizeof(double));
			link++;
			if ((int)(100 * link / link_Count) != persent)
			{
				persent = (100 * link / link_Count);
				cout << '\r' << persent << "%...";
			}
		}
	}
	cout << endl;
	f.close();
}
