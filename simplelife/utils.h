#pragma once
#include<iostream>
#include<ctime>
#include<cmath>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<string>
#include<set>
#include<map>
#include<iomanip>
#include<fstream>


void mySrand();
extern inline double sigmod(double in);
extern inline uint32_t myRand();     //0 ~ uint32_max
extern inline double myRand_1to1();  //precision: 3e-8
extern inline double myRand_0to1();  //precision: 6e-8
int roundPoint[][2];
