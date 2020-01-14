// brain_simulation.cpp

#include "body.h"
#include "brain_node.h"
#include "brain_macro.h"
#include "sha1.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <mutex>
#include <thread>

using namespace std;

//single 40000node 0.691s
//multi  50 thread 800 neuron / node 0.0083s


const char AuthorInfo_encodeToSHA256[] =
"223CCAFEA58D3E29F87C55DEBF0B1D3F40A8EC83F95463E753215D2FD19523C2";

const int THREAD_NUM = 4;
const int NODE_LINKS = 8;
const int NEURON_NUM_OF_BRAIN_NODE = 2000;
BrainNode* brainNodes[THREAD_NUM];


int threadFinishCount = 0;
enum class ThreadCmd { run,exit };

vector<std::mutex> mtx_notify(THREAD_NUM);
vector<std::mutex> mtx_ready(THREAD_NUM);
std::mutex threadCntMtx;
std::mutex mtx2;
std::condition_variable cv_main2thread;
std::condition_variable cv_thread2main;
bool ready2main = false;
ThreadCmd threadCmd = ThreadCmd::run;
vector<bool> ready(THREAD_NUM, false);

std::thread threads[THREAD_NUM];


//brainNode Thread
void brainNodeThread(int id) {
	string s1 = "Thread [" + to_string(id) + "]: Ready.\r\n";
	//string s2 = "Thread [" + to_string(id) + "]: Running.\r\n";
	std::cout <<  s1;
	while (true) {
		std::unique_lock<std::mutex> lck0(mtx_notify[id]);
		while (!ready[id]) cv_main2thread.wait(lck0);

		if (threadCmd == ThreadCmd::exit)
			break;

		//inner_iterate
		brainNodes[id]->inner_iterate();


		std::unique_lock<std::mutex> lck(mtx_ready[id]);
		ready[id] = false;
		lck.unlock();

		std::unique_lock<std::mutex> lck2(threadCntMtx);
		threadFinishCount++;
		if (threadFinishCount >= THREAD_NUM) {
			ready2main = true;
			//std::cout << "notify mainThread." << endl;
			cv_thread2main.notify_one();
		}
		lck2.unlock();


	}
}

//MultiThread
int main(int argc, char* argv[]) {
	using namespace std::chrono;

	//std::cout << "Hello world!" << endl;

	mySrand();

	{
		std::cout << "Initial brainNodes..." << endl;
		BrainNode brainTemplate(NODE_LINKS, NEURON_NUM_OF_BRAIN_NODE, NODE_LINKS, 0);
		for (int i = 0; i < THREAD_NUM; i++)
			brainNodes[i] = new BrainNode("D:\\brain_2020-01-14_051216.bin");
		std::cout << "Initial over." << endl;
	}
	

	for (int i = 0; i < THREAD_NUM; ++i)
		threads[i] = std::thread(brainNodeThread, i);
	std::this_thread::sleep_for(2s);

	//TODO   Macro struct of brain




	//TODO   Macro struct of brain
	auto t0 = high_resolution_clock::now();

	int i = 1000;
	int times = i;
	while (i--) {
		//std::cout << "Iterate " << i << endl;
		//STEP1 system INPUT to macro

		//STEP2 macro to brainNode



		//STEP3 brainNode iterate
		threadFinishCount = 0;		
		//std::cout << "Notify all threads to work." << endl;
		for (int i = 0; i < THREAD_NUM; i++)
			ready[i] = true;
		cv_main2thread.notify_all();
		
		//waiting
		std::unique_lock<std::mutex> lck(mtx2);
		while (!ready2main) cv_thread2main.wait(lck);
		ready2main = false;
		//std::cout << "All threads finish." << endl;




		//STEP4 brainNode output to macro

		//STEP5 macro to system OUTPUT



		//TODO exit loop condition

	}

	//Requit all threads Exit.
	threadCmd = ThreadCmd::exit;
	std::cout << "Notify all threads exit." << endl;
	for (int i = 0; i < THREAD_NUM; i++)
		ready[i] = true;
	cv_main2thread.notify_all();

	auto time_span2 = duration_cast<duration<double>>(high_resolution_clock::now() - t0);
	std::cout << "It took  " << 1000.0*time_span2.count() / times << " ms per iterate." << endl;
	std::cout << "" << (double)times / time_span2.count() << " Hz." << endl;

	for (auto& th : threads) th.join();

	std::cout << "All threads exit." << endl;

	for (int i = 0; i < THREAD_NUM; i++)
		delete brainNodes[i];
	std::cout << "All brainNode's memory release." << endl;
	std::cout << "System exit." << endl;


	getchar();


	return 0;
}














void test();

//SingleThread
int main55()
{
	using namespace std::chrono;

	//test();


	mySrand();

	//test();

	vector<double> input = { 1, 2, 3, 4, 5, 6, 7, 8 };
	vector<double> output = { 1, 2, 3, 4, 5, 6, 7, 8 };

	//BrainNode mybrainNode(input.size(), 1000, output.size(), 0);
	BrainNode mybrainNode("D:\\brain_2020-01-14_051216.bin");


	//mybrainNode.printLink();

	mybrainNode.input_iterate(input);

	int t = 1000;
	int times = t;

	auto t0 = high_resolution_clock::now();
	while (t--) {

		//for (auto& i : input)
		//	i = (int)(myRand_1to1() * 10);

		mybrainNode.input_iterate(input);
		mybrainNode.inner_iterate();
		//mybrainNode.inner_iterate();
		mybrainNode.output_iterate(output);

		//cout << endl << t << ':';
		//for (auto o : output)
		//	cout << o << ' ';
		//auto time_span = duration_cast<duration<double>>(high_resolution_clock::now() - t1);
		//std::cout << "It took  " << time_span.count() << " seconds." << endl;

	}


	auto time_span2 = duration_cast<duration<double>>(high_resolution_clock::now() - t0);
	std::cout << "It took  " << 1000.0 * time_span2.count() / times << " ms per iterate." << endl;
	std::cout << "" << (double)times / time_span2.count() << " Hz." << endl;

	//cout << "Do you want to save the brain(y/n)? ";
	//char c;
	//cin >> c;
	//if (c == 'Y' || c == 'y') {
	//	cout << endl << "Saving..." << endl;
	//	string filename = "D:\\brain_" + timeToStr(time(nullptr)) + ".bin";
	//	mybrainNode.saveBrainNode(filename);
	//	cout << "File \"" << filename << "\" saved!" << endl;
	//}
	//else {
	//	cout << endl << "Exit without saving." << endl;
	//}
	return 0;
}



void test() {


	int cc[20] = { 0 };
	for (int i = 0; i < 100000000; i++) {
		//cout << myRand_1to1() << endl;
		int a = 10 * (myRand_1to1() + 1.0);
		cc[a] ++;
	}
	for (auto i : cc) {
		cout << i << endl;
	}


	exit(1);
}



//game simulate test
int mainXXX(int argc, char* argv[]) {
	//cout << "Hello World!" << endl;


	const int wordX = 1000;
	const int wordY = 1000;
	const int brainTimes = 10;

	mySrand();

	vector<double> input(36 * 3);//敌人 同伴 墙，3个感知层每个均为36个复眼一圈的视角
	vector<double> output(2);
	int world[wordX][wordY] = { 0 };
	BrainNode brainTemplate(input.size(), 2000, output.size(), 0);
	BrainNode* brains[20];//

	Body  mybody;
	mybody.x = 100;
	mybody.y = 100;

	for (auto& p : brains) {
		p = new BrainNode(brainTemplate);
	}


	uint32_t times = 5000;
	while (times--) {


		//墙感知
		if (mybody.x < 30 || mybody.x >(wordX - 30) ||
			mybody.y < 30 || mybody.y >(wordY - 30))
		{
			//TODO

		}
		else
		{
			for (int i = 0; i < input.size() / 3; i++)
				input[i] = 0;
		}

		//TODO 其他感知


		//大脑思考
		brainTemplate.input_iterate(input);
		for (int i = 0; i < brainTimes; i++)
			brainTemplate.inner_iterate();
		brainTemplate.output_iterate(output);

		//行动
		if (output[0] > 0.618)
			mybody.x++;
		else if (output[0] < 0.382)
			mybody.x--;
		if (output[1] > 0.618)
			mybody.y++;
		else if (output[1] < 0.382)
			mybody.y--;

		//限制XY
		if (mybody.x < 0)
			mybody.x = 0;
		else if (mybody.x >= wordX)
			mybody.x = wordX - 1;
		if (mybody.y < 0)
			mybody.y = 0;
		else if (mybody.y >= wordY)
			mybody.y = wordY - 1;

		//TODO 显示

	}


	for (auto& p : brains) {
		delete p;
	}

	return 0;
}