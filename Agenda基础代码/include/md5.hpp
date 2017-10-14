#ifndef MD5_H
#define MD5_H

#include <string>
using namespace std;


void mainLoop(unsigned int M[]);

unsigned int* add(string str);

string changeHex(int a);

string getMD5(string source);

#endif