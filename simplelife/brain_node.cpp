#include "brain_node.h"

BrainNode::BrainNode(string filePath)
{
	auto t1 = time(nullptr);
	BrainFileHeader heardInfo;
	uint64_t temp;
	uint64_t temp2;
	double temp_double;

	cout << "Load brain from file: \"" << filePath << '\"' << endl;
	ifstream f(filePath, ios::binary);
	if (!f)
	{
		cout << "Fail to read file!" << endl;
		exit(-1);
	}

	f.read((char*)& heardInfo, sizeof(heardInfo));


	cout << "BrainVersion: " << heardInfo.mainVer_brain << '.' << heardInfo.subVer_brain << endl;
	cout << "Fix to BodyVersion: " << heardInfo.mainVer_body << '.' << heardInfo.subVer_body << endl;
	cout << "InputSize: " << heardInfo.inputNodeSize << endl;
	cout << "InputSize: " << heardInfo.inputNodeSize << endl;
	cout << "neuralSize: " << heardInfo.hideNodeSize << endl;
	cout << "OutputSize: " << heardInfo.outputNodeSize << endl;

	inputNode.resize(heardInfo.inputNodeSize);
	hideNode.resize(heardInfo.hideNodeSize);
	outputNode.resize(heardInfo.outputNodeSize);
	score = heardInfo.score;


	link_Count = 0;
	for (auto& i : inputNode) {
		f.read((char*)& i.id, sizeof(uint64_t));
		f.read((char*)& i.bias, sizeof(double));
		f.read((char*)& i.sum, sizeof(double));
		f.read((char*)& temp, sizeof(uint64_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(uint64_t));
			f.read((char*)& temp_double, sizeof(double));
			i.target.push_back(pair<uint64_t, double>(temp2, temp_double));
			link_Count++;
		}
	}

	for (auto& h : hideNode) {
		f.read((char*)& h.id, sizeof(uint64_t));
		f.read((char*)& h.bias, sizeof(double));
		f.read((char*)& h.sum, sizeof(double));
		f.read((char*)& temp, sizeof(uint64_t));
		while (temp--)
		{
			f.read((char*)& temp2, sizeof(uint64_t));
			f.read((char*)& temp_double, sizeof(double));
			h.target.push_back(pair<uint64_t, double>(temp2, temp_double));
			link_Count++;
		}
	}

	for (auto& o : outputNode) {
		f.read((char*)& o.id, sizeof(uint64_t));
		f.read((char*)& o.bias, sizeof(double));
		f.read((char*)& o.sum, sizeof(double));
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
	cout << "Load over." << endl;
	cout << "Time use:" << time(nullptr) - t1 << endl << endl;
}

BrainNode::BrainNode(uint64_t input_num, uint64_t neuralNode, uint64_t output_num, uint64_t _score)
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
		inputNode[i].id = i;
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
		hideNode[i].id = i;
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
		outputNode[i].id = i;
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
	HIDE_NODE_ADD, //隐藏节点增加
	HIDE_NODE_SUB, //隐藏节点减少 TODO
	LINK_ADD,      //连接目标增加
	LINK_SUB,      //连接目标减少
	WEIGHT_CHANGE, //权重增减
ratio:变异比例
*/
BrainNode::BrainNode(BrainNode& parentBrain, variationType v_type, double ratio)
{

	inputNode.assign(parentBrain.inputNode.begin(), parentBrain.inputNode.end());
	hideNode.assign(parentBrain.hideNode.begin(), parentBrain.hideNode.end());
	outputNode.assign(parentBrain.outputNode.begin(), parentBrain.outputNode.end());

	score = parentBrain.score;
	link_Count = parentBrain.link_Count;

	NeuronNode newNode;
	set<uint64_t> targetLink;
	set<uint64_t> newLink;
	vector<pair<uint64_t, double>> newTarget; //目标节点及权值
	uint64_t link_Count_temp = 0;
	uint64_t addNum;
	uint64_t sizeBefore;
	uint64_t sizeAfter;

	switch (v_type) //TODO
	{
	case variationType::HIDE_NODE_ADD:
	{
		link_Count_temp = 0;
		addNum = (uint64_t)(myRand() % (uint32_t)(hideNode.size() * ratio)) + 1; //新增隐藏节点数量
		sizeBefore = hideNode.size();
		sizeAfter = sizeBefore + addNum;
		for (int i = 0; i < addNum; i++) {
			newNode.id = sizeBefore + i;
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
	}
	case variationType::LINK_ADD: //将会改变默认比例
	{
		for (auto& i : inputNode) {
			targetLink.clear();
			newLink.clear();

			for (auto t : i.target)
				targetLink.insert(t.first);

			for (auto index = 0; index < hideNode.size(); index++) {
				if (myRand_0to1() < ratio)
					if ((targetLink.count(index) == 0) && (newLink.count(index) == 0)) {
						newLink.insert(index);
					}
			}
			for (auto n : newLink)
				i.target.push_back(pair<uint64_t, double>(n, myRand_1to1() / 10.0));
		}


		for (auto& h : hideNode) {
			targetLink.clear();
			newLink.clear();

			for (auto t : h.target)
				targetLink.insert(t.first);

			for (auto index = 0; index < hideNode.size(); index++) {
				if ((myRand_0to1() < ratio) && (index != h.id))
					if ((targetLink.count(index) == 0) && (newLink.count(index) == 0)) {
						newLink.insert(index);
					}
			}
			for (auto n : newLink)
				h.target.push_back(pair<uint64_t, double>(n, myRand_1to1() / 10.0));
		}


		for (auto& o : outputNode) {
			targetLink.clear();
			newLink.clear();

			for (auto t : o.target)
				targetLink.insert(t.first);

			for (auto index = 0; index < hideNode.size(); index++) {
				if (myRand_0to1() < ratio)
					if ((targetLink.count(index) == 0) && (newLink.count(index) == 0)) {
						newLink.insert(index);
					}
			}
			for (auto n : newLink)
				o.target.push_back(pair<uint64_t, double>(n, myRand_1to1() / 10.0));
		}

		break;
	}
	case variationType::LINK_SUB:
	{
		for (auto& i : inputNode) {
			newTarget.clear();
			for (auto t : i.target) {
				if (myRand_0to1() > ratio) {
					newTarget.push_back(t);
				}
			}
			i.target.assign(newTarget.begin(), newTarget.end());
		}

		for (auto& h : hideNode) {
			newTarget.clear();
			for (auto t : h.target) {
				if (myRand_0to1() > ratio) {
					newTarget.push_back(t);
				}
			}
			h.target.assign(newTarget.begin(), newTarget.end());
		}
		for (auto& o : outputNode) {
			newTarget.clear();
			for (auto t : o.target) {
				if (myRand_0to1() > ratio) {
					newTarget.push_back(t);
				}
			}
			o.target.assign(newTarget.begin(), newTarget.end());
		}
		break;
	}
	case variationType::WEIGHT_CHANGE:
	{
		const int sign[4] = { -1, 0, 1, 0 };
		for (auto& i : inputNode)
			for (auto& t : i.target)
				t.second += deltaWeight * sign[myRand() & 0x03];
		for (auto& h : hideNode)
			for (auto& t : h.target)
				t.second += deltaWeight * sign[myRand() & 0x03];
		for (auto& o : outputNode)
			for (auto& t : o.target)
				t.second += deltaWeight * sign[myRand() & 0x03];
		break;
	}
	default:
		break;
	}
}

BrainNode::~BrainNode()
{
}

void BrainNode::input_iterate(vector<double>& input)
{
	if (input.size() < inputNode.size())
	{
		cerr << "InputNum ERROR!" << endl;
		exit(-1);
	}
	for (auto i = 0; i < inputNode.size(); i++) {
		inputNode[i].sum = input[i];
		inputNode[i].out = sigmod(inputNode[i].sum + inputNode[i].bias);
	}

	for (auto& h : hideNode)
		h.sum_in = 0;

	for (auto i : inputNode)
		for (auto t : i.target)
			hideNode[t.first].sum_in += (i.out * t.second);
}

void BrainNode::inner_iterate()
{
	for (auto& h : hideNode)
	{
		h.out = sigmod(h.bias + h.sum + h.sum_in);
		h.sum_old = h.sum;
		h.sum = 0;
	}

	for (auto h : hideNode)
		for (auto t : h.target)
			hideNode[t.first].sum += (h.out * t.second);

	//valueKeep
	for (auto& h : hideNode)
		h.sum = h.sum_old * valueKeep + h.sum * (1.0 - valueKeep);
}

void BrainNode::output_iterate(vector<double>& output)
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

void BrainNode::printLink()
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

void BrainNode::reRandomWeight()
{
	for (auto& i : inputNode) {
		i.bias = myRand_1to1();
		for (auto& t : i.target)
			t.second = myRand_1to1();
	}
	for (auto& h : hideNode) {
		for (auto& t : h.target) {
			t.second = myRand_1to1();
		}
		h.bias = myRand_1to1();
		h.sum = myRand_1to1();
	}

	for (auto& o : outputNode)
		for (auto& t : o.target)
			t.second = myRand_1to1();
}

void BrainNode::saveBrainNode(string filePath)
{
	uint64_t temp;
	BrainFileHeader heardInfo;
	uint64_t link = 0;
	uint64_t persent = 0;

	ofstream f(filePath, ios::binary);
	if (!f)
	{
		cerr << "Save brain faile, outputfile error!" << endl;
		return;
	}

	heardInfo.inputNodeSize = inputNode.size();
	heardInfo.hideNodeSize = hideNode.size();
	heardInfo.outputNodeSize = outputNode.size();
	heardInfo.score = score;

	f.write((char*)& heardInfo, sizeof(heardInfo));

	cout << "0%...";
	for (auto& i : inputNode) {
		temp = i.target.size();
		f.write((char*)& i.id, sizeof(uint64_t));
		f.write((char*)& i.bias, sizeof(double));
		f.write((char*)& i.sum, sizeof(double));
		f.write((char*)& temp, sizeof(uint64_t));
		for (auto t : i.target) {
			f.write((char*)& t.first, sizeof(uint64_t));
			f.write((char*)& t.second, sizeof(double));
			link++;
			if ((int)(100 * link / link_Count) != persent)
			{
				persent = (100 * link / link_Count);
				cout << '\r' << persent << "%...";
			}
		}
	}

	for (auto& h : hideNode) {
		temp = h.target.size();
		f.write((char*)& h.id, sizeof(uint64_t));
		f.write((char*)& h.bias, sizeof(double));
		f.write((char*)& h.sum, sizeof(double));
		f.write((char*)& temp, sizeof(uint64_t));
		for (auto t : h.target) {
			f.write((char*)& t.first, sizeof(uint64_t));
			f.write((char*)& t.second, sizeof(double));
			link++;
			if ((int)(100 * link / link_Count) != persent)
			{
				persent = (100 * link / link_Count);
				cout << '\r' << persent << "%...";
			}
		}
	}

	for (auto o : outputNode) {
		temp = o.target.size();
		f.write((char*)& o.id, sizeof(uint64_t));
		f.write((char*)& o.bias, sizeof(double));
		f.write((char*)& o.sum, sizeof(double));
		f.write((char*)& temp, sizeof(uint64_t));
		for (auto t : o.target) {
			f.write((char*)& t.first, sizeof(uint64_t));
			f.write((char*)& t.second, sizeof(double));
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
