// simplelife.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "brain.h"
#include "sha1.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

void test();

int main()
{
	//srand((uint32_t)time(nullptr));
	mySrand();

	vector<double> input = { 1, 6 ,9, };
	vector<double> output = { 5, 4 ,3, };
	

	test();


	Brain mybrain(input.size(), 10000, output.size(), 0);
	//Brain mybrain("brain_1567538537.bin");
	
	
	//mybrain.brain_iterate();
	//mybrain.printLink();



	mybrain.input_iterate(input);
	int cc = 1;
	while (cc--) {
		int t = 100;
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
		//mybrain.reRandomWeight();
	}

	//cout << endl;

	//mybrain.printLink();
	//mybrain.reRandomWeight();


	//mybrain.input_iterate(input);




	cout << "Do you want to save the brain(y/n)? ";
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y')
	{
		cout << endl << "Saving..." << endl;
		string filename = "brain_" + to_string(time(nullptr)) + ".bin";
		mybrain.saveBrain(filename);
		cout << "File \"" << filename << "\" saved!" << endl;
	}


}



void test() {
	for (int i = 0; i < 100; i++)
		cout << myRand_0to1() << endl;
	exit(1);
}

