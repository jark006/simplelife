// life_simulation.cpp: 定义应用程序的入口点。
//

#include "body.h"
#include "brain.h"
#include "sha1.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

void test();

int main()
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
	if (c == 'Y' || c == 'y')
	{
		cout << endl << "Saving..." << endl;
		string filename = "D:\\brain_" + to_string(time(nullptr)) + ".bin";
		mybrain.saveBrain(filename);
		cout << "File \"" << filename << "\" saved!" << endl;
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
	cout << sizeof(BrainFileHeader) << endl;

	exit(1);
}


