#include "utils.h"
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

#include <ciso646>

#include <boost/algorithm/string.hpp>



//using namespace std;
//using namespace boost::algorithm;






string get_fromFile(string flnm, string target)
{
  
  string rs;
  rs = "__ERROR"; //<--- obviously that is reserved
 
  ifstream fz;
  fz.open(flnm);
  string ln;
  while(getline(fz,ln))
  {

   if(ln.substr(0,target.size()+3) == target + string(" : "))
   {
     rs = ln.substr(target.size()-1+3);
     break;
   }
  }
  trim(rs);
  return rs;
}







void writelog(string msg, string location = "lookup" )
{
  if (location == "lookup") //that would men that no specific location was given, insted a command : lookup
 {
   location = get_fromFile(configpath, "steppenwind_log");
 }
 trim(location);
 
 ofstream fz;
 fz.open(location.c_str(), ios::out|ios::app);
 if(fz.fail())
   cerr << "Error: " << std::strerror(errno) <<endl;
 fz << msg << endl;
 
 fz.close();
}





void clearlog(string location = "lookup")
{
 if (location == "lookup") //that would men that no specific location was given, insted a command : lookup
 {
    location = get_fromFile(configpath, "steppenwind_log");
 }
 
  trim(location);
 ofstream fz;
 fz.open(location);
 fz.close();
 
}


bool check_fileEmpty(string flpt)
{
  string   s;
  ifstream f( flpt, ios::binary );

  // Check for UTF-8 BOM
  if (f.peek() == 0xEF)
    {
    f.get();
    if (f.get() != 0xBB) return false;
    if (f.get() != 0xBF) return false;
    }

  // Scan every line of the file for non-whitespace characters
  while (getline( f, s ))
    {
    if (s.find_first_not_of(
      " \t\n\v\f\r" // whitespace
      "\0\xFE\xFF"  // non-printing (used in various Unicode encodings)
      ) != string::npos)
      return false;
    }

  // If we get this far, then the file only contains whitespace
  // (or its size is zero)
  return true;
}





void manageConfig(string str, string wht = "")
{
  
  writelog("config information for "+wht+" is : "+str);
  if(str == "")
  { 
    std::cout << "FAILURE : NO CONFIG  FOUND : FORCING EXIT WITH EXIT CODE 2" << endl << endl;
    exit(2);
  }
  
  
}