


#ifndef utils_h
#define utils_h




#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iterator>
#include <cmath>
#include <thread>
#include <cerrno>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>


#include <boost/algorithm/string.hpp>








using namespace std;
using namespace boost::algorithm;

extern string configpath;

extern void writelog(string, string);
extern string get_formFile(string, string);
extern void clearlog(string);
extern void manageConfig(string, string);

extern bool check_fileEmpty(string);

#endif