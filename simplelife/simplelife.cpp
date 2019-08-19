// simplelife.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include "brain.h"
#include "sha1.h"
#include <iostream>
#include <iomanip>

int main()
{
	srand(time(NULL));

	vector<double> input = { 1, 6 ,9, };
	vector<double> output(2);
	
	brain mybrain(input.size(), 302, output.size(), 0);
	//brain mybrain("brain.bin");

	//mybrain.printLink();



	mybrain.input_iterate(input);

	int t = 10;
	while (t--) {

		mybrain.brain_iterate();
		mybrain.output_iterate(output);
		for (auto i : output) {
			cout <<  i << ' ';
		}

		cout << endl;
	}

	cout << endl;
	mybrain.reRandomWeight();


	mybrain.input_iterate(input);

	t = 10;
	while (t--) {

		mybrain.brain_iterate();
		mybrain.output_iterate(output);
		for (auto i : output) {
			cout << i << ' ';
		}

		cout << endl;
	}


	cout << "Do you want to save the brain(y/n)? : ";
	char c;
	cin >> c;
	if (c == 'Y' || c == 'y')
	{
		cout << endl << "Saving..." << endl;
		string filename = "brain_" + to_string(time(NULL)) + ".bin";
		mybrain.saveBrain(filename);
		cout << "File \"" << filename << "\" saved!" << endl;
	}


}




