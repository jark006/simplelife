// life_simulation.cpp: 定义应用程序的入口点。
//

#include "body.h"
#include "brain.h"
#include "sha1.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

void test();

int main_sample()
{

	test();


	mySrand();

	vector<double> input = { 1, 6 ,9, };
	vector<double> output = { 5, 4 ,3, };

	Brain mybrain(input.size(), 2000, output.size(), 0);
	//Brain mybrain("D:\\brain_1568040796.bin");


	//mybrain.printLink();

	mybrain.input_iterate(input);

	int t = 20;
	while (t--) {
		mybrain.brain_iterate();
		mybrain.output_iterate(output);
		cout << t << ' ';
		for (auto i : output) {
			cout << i << ' ';
		}
		cout << endl;
	}
	cout << "iterating... " << time(nullptr) << endl << endl;




	cout << "Do you want to save the brain(y/n)? ";
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y') {
		cout << endl << "Saving..." << endl;
		string filename = "D:\\brain_" + to_string(time(nullptr)) + ".bin";
		mybrain.saveBrain(filename);
		cout << "File \"" << filename << "\" saved!" << endl;
	}
	else {
		cout << endl << "Exit without saving." << endl;
	}
	return 0;
}



void test() {

	//vector<int> a = { 1,2, };
	//vector<int> b = { 11,12,13,14 };

	//for (auto i : a)
	//	cout << i << ' ';
	//cout << endl;

	//a.assign(b.begin(), b.end());

	//for (auto i : a)
	//	cout << i << ' ';
	//cout << endl;
	//cout << sizeof(BrainFileHeader) << endl;


	for (int i = 5; i <= 30; i += 5) {
		double sum = i + 0.5;
		sum = sum*sum*PI;
		cout << i << ' ' << (int)sum << endl;
	}


	exit(1);
}


const int wordX = 1000;
const int wordY = 1000;
const int brainTimes = 10;

int main(int argc, char* argv[]) {
	//cout << "Hello World!" << endl;

	mySrand();

	vector<double> input(36*3);
	vector<double> output(2);
	int world[wordX][wordY] = { 0 };
	Brain mybrain(input.size(), 2000, output.size(), 0);
	mybrain.x = 100;
	mybrain.y = 100;


	uint32_t times = 5000;
	while (times--) {


		//墙感知
		if (mybrain.x < 30 || mybrain.x >(wordX - 30) || 
			mybrain.y < 30 || mybrain.y >(wordY - 30))
		{
			//TODO

		}
		else
		{
			for (int i = 0; i < input.size()/3; i++)
				input[i] = 0;
		}

		//TODO 其他感知


		//大脑思考
		mybrain.input_iterate(input);
		for(int i=0; i< brainTimes; i++)
			mybrain.brain_iterate();
		mybrain.output_iterate(output);

		//行动
		if (output[0] > 0.618)
			mybrain.x++;
		else if (output[0] < 0.382)
			mybrain.x--;
		if (output[1] > 0.618)
			mybrain.y++;
		else if (output[1] < 0.382)
			mybrain.y--;

		//限制XY
		if (mybrain.x < 0)
			mybrain.x = 0;
		else if (mybrain.x >= wordX)
			mybrain.x = wordX - 1;
		if (mybrain.y < 0)
			mybrain.y = 0;
		else if (mybrain.y >= wordY)
			mybrain.y = wordY - 1;

		//TODO 显示

	}



	return 0;
}