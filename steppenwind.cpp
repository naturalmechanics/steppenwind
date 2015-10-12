/*include files*/
#include <cstdio>									// standard io C style
#include <iostream>									// istream and ostream is defined here.
#include <fstream>									// file stream
#include <sstream>									// treat strings as streams
#include <cstdlib>									// standard libraries C style
#include <vector>									// stupid C++ vectors
#include <string>									// strings
#include <iterator>									// iterator on unknown vectors
#include <cmath>									// maths
#include <thread>									// multithreading
#include <cerrno>									// standard error numbers for debuging
#include <cstring>

#include <stdio.h>									// still need this
#include <stdlib.h>									// still need this
#include <unistd.h>									// still need this
#include <getopt.h>									// read options / command line switches

#include <boost/algorithm/string.hpp>							// boost string algorithms

#include "utils.cpp"									// use the software specific utils library
#include "pstream.h"									// this captures process outputs from linux shell.


/*globals*/
string configpath;									// path to configuration file
string quitCommand = "";								// quit command, a global such that all threads can see this
static int nomsgbusman_flag = 0;							// a flag, if true then dont use flamenco
static int keeplog_flag = 0;								// a flag, if true, then log everything








/*struct definitions */

struct child{										// node of a linked list
  
  child * prv;										// previous
  string name;										// name of the child
  string id;										// an UNIQUE id to the child
  vector<string> keywords;								// some key words (not used now)
  string tmstr;										// a timer (not used now)
  string uplink;									// semaphore file, to communicate with steppenwind (upstream, child process writes here, steppenwind reads here)
  string downlink;									// semaphore file, to communicate with steppenwind (downstream, child process reads here, steppenwind writes here)
  child * nxt;										// next
};


child * strt ,* curr;									// the first node, and the current node











/*namespace*/

using namespace std;
using namespace boost::algorithm;





//program begins





void readMsgbus()
{											//read uplinks here
  
  
   if(nomsgbusman_flag != 0) 								//if no messagebus manager flag is unset, ie a messagebus manager is permitted to run
   {
    
    string msgBus_man = get_fromFile(configpath, "steppenwind_messagebus_manager"); 	//the messagebus manager executable
    manageConfig(msgBus_man, "messagebus manager"); 					//if this config information is missing then exit, otherwise log it
    trim(msgBus_man);
    
    string msgBus = get_fromFile(configpath, "steppenwind_messagebus"); 		//the message location, the default uplink to all child processes
    manageConfig(msgBus, "messagebus location");
    trim(msgBus_man);
    
    string msgBus_p = get_fromFile(configpath, "steppenwind_messagebus_processed"); 	//the processed messagebus location, manager will creat this file or overwrite it with the processed msgbus contents
    manageConfig(msgBus_p, "messagebus processor");
    trim(msgBus_p);
    
    //TODO next version : use seperate messagebus for each child if prescribed so
    
    //i generally use flamenco, 
    //but you can just ignore it, 
    //if you dont have to use a messagebus manager that must preprocess the messagebus
    //then remove this block.
    //or call with a --no-messagebus.manager switch
    //then this if will fail
    
    //we have to wait for the process to finish
    //do not fork, exec
    
											writelog("attempting to execute " + msgBus_man);
    execl(msgBus_man.c_str(), msgBus.c_str(), msgBus_p.c_str(), (char *) 0);		writelog("external messagebus manager execution was successful");
    
  }
  else  {										writelog( "no messagebus manager is set" );
  }
  
  
  
  
  
  
  string commandLst = get_fromFile(configpath, "steppenwind_msgbuscommands"); 		//this will tell you where messagebus commands are defined
  manageConfig(commandLst, "list of messagebus commands"); 
  
  
  string msgbus = get_fromFile(configpath, "steppenwind_messagebus_processed"); 	//now that we are outside the manager exec branch, find it again.
  manageConfig(msgbus, "preprocessed messagebus location");
  
  
  string msgbusdrvr = get_fromFile(configpath, "steppenwind_messagebus_driver"); 	//need a reader that reads a line and pops it....
  manageConfig(msgbusdrvr , "message driver");
  
  
  string filedrvr = msgbusdrvr ; 							//the same driver
  
  
  
  
  
  
  
  
  
  
  while(quitCommand != "quit") 								//unless the global quitcommand is set to quit
  {											//now for each command in message bus,
  
  string str;										// the entire message in the messagebus
  
                                                                                        //read the command 
                                                                                        //must be read using flamenco, and the read commands be erased
 
  while(!check_fileEmpty(msgbus)) 
  {
    
    redi::ipstream proc(msgbusdrvr + " popfirst " + "\""+msgbus+"\"", redi::pstreams::pstdout);
    
    while(getline(proc.out(),str)){							writelog("messagebus command is " + str);
     
     string zsep = get_fromFile(configpath, "steppenwind_commandinfo_separator");       //the information is seperated by a separator as given in the configfile
                                                                                        //get the first word
     
     vector<string> strs;
     boost::split(strs, str, boost::is_any_of(zsep)); 
     
     if (strs[0] == "READ")
     {
     }
     else if (strs[0] == "RECORD")
     {
        string pda = get_fromFile(configpath, "steppenwind_stack");
	//std::cout << "record command -->" << filedrvr.c_str() << "<-- record -->" << strs[1].c_str() << "<-- -->" << strs[2].c_str() << "<-- -->" << pda.c_str() <<endl;
	
	pid_t pID_ = fork();
	if (pID_ == 0) {								writelog("execution starting : RECORD command");
	  execl(filedrvr.c_str(),filedrvr.c_str(), "record", strs[1].c_str(), strs[2].c_str(), pda.c_str(), (char*)0);
	}
	else if (pID_ < 0) { 								writelog("RECORD command failed");
	  
	}
	else if (pID_ > 0) {								writelog("messagebus reader continues");
	}
     }
     else if (strs[0] == "RUN")
     {
											//find the corresponding action form the (correct) configfile
    string cmdstr = get_fromFile(commandLst, strs[1]);					manageConfig(cmdstr, "external command associated with "+ strs[1]);
    
    
  
    //perform it, also fork and kill
    
    
    pid_t pID = fork();
    if(pID == 0)
    {
      //char const * const argvec[] = {NULL}; //no arguments, you can build arguents later
      //int execret = execvp(strs[0], (char**) argvec);
      
      
      vector<char *> argvec(strs.size() ); 						//one extra for the NULL, no need to explicitely initialize this.. note that counter starts at 1, not at 0
      for(size_t ii = 1; ii != strs.size(); ++ii)
      {
	argvec[ii-1] = &strs[ii][0]; 							//the first argument is the file name itself, so argvec[0] , will be executed, 
                                                                                        //and is the argument to itself
      }
      
      int execret = execvp(argvec[0], argvec.data());
      
      
      
      if(execret == -1)
      {
	std::cout << "command failed. continuing with next one" << endl;
      }
      
      quitCommand = "quit";								writelog("quiting child fork ...");
    }
    
    else if(pID < 0)
    {
      std::cout << "fork failed, process won't be executed .. continuing with next command" << endl; //we have failed
      
    }
    else
    {
      //do nothing
											writelog("parent messagebus reader continues");
    }
     } 											//else if command == run ends here
	 
	 
	 
     
     
    }											//while getline proc ends here
											writelog("one line of messagebus has been processed and popped");
    
  } 											//while ends haha
  
											writelog("messagebus reading complete");
    
   
  } 											//outer while ends here
  
											writelog("messagebus reader exiting");
  exit(1);										//kill - this will only happen when quitcommand is set to quit
}










void readUserCommand()
{
   //and this is the thread where the user interaction takes place ... 
  
  
}
































////////////////


string generateID()
{
  
  child * c;
  c = strt;
  while(c->nxt  != NULL)
  {
    c = c->nxt;
  }
  
  if (c->id == "NULL")
    return "1";
  else
  {
    
    int i = atoi(c->id.c_str());
    stringstream ss;
    ss << i+1;
    return ss.str();
  }
}


























int main(int argc, char ** argv )
{											std::cout << "housekeeping: initiating" << endl;
  
  char * configFlPt ; 									//config file path
  int c; 										//return of getopt long
  
  int option_index = 0; 								//some stuff for getopt, telling us where to start
											
											std::cout << "housekeeping: processing input args" << endl;
											std::cout << "            : setting struct for expected arguments" << endl;
    
  static struct option long_options[] =
  {
    {"no-messagebus-manager", no_argument, &nomsgbusman_flag, 1},			// default is the flag <no messagebus manager> set to 1, and it does not take an argument
    {"keeplog", no_argument, &keeplog_flag, 1},						// default is the flag <keeplog> set to 1, and it does not take an argument
											// the following options does not set a flag, thus the third element in the array is zero
    {"configFilePath", required_argument, 0 , 'a'},					// configFilePath is a required argument, and it followes the switch -a
    {"help", required_argument, 0 , 'h'},						// help, followes switch -h
    {0,0,0,0}
  };											std::cout << "            : set... " << endl;
  
  
  // ALERT : known BUG 
  /** 
   POSIX requires The first non-option stops option processing.
   thus, --no-steppenwind_messagebus_manager will have to follow --configFilePath
   **/
  c = getopt_long(argc, argv, "a:?h", long_options, &option_index); 			//a is the shorthand for configfile , you can then use ? or h to trigger the help, colon (:9 after a means expect value
											std::cout << "            : input args read" << endl;
											std::cout << "housekeeping: preparing to set config file path" << endl;
  
  //things that we will need in case
  stringstream s; 									// a stream that we will need to process the config file
  string z ; 										// the string that hold configfile when input by user
  
  
  switch(c)
  {
    case 0:
      std::cout << "no configfile found" << endl;
      std::cout << "configfile location ? " ;	
      std::cin >> z;									// read in the input in z
      s << z;										// push z in s
      configpath = s.str(); 								//configpath is global
											std::cout << "housekeeping: path set to " + configpath << endl;
      break;
    case 'a':
      s << optarg ;									// if it is given, however, force optargs inside s
      configpath = s.str();
      break;
    case 'h':
    case '?':
      std::cout << "uses : mention a configfile" << endl;
      break;
    default :
      abort();
      
  }
  
  string logpath = get_fromFile(configpath, "steppenwind_log");				std::cout << "housekeeping: log path set to " + logpath << endl; 
  
  if(keeplog_flag == 0)
  {
    clearlog();										std::cout << "housekeeping: log cleared " << endl;
  }											std::cout << "housekeeping: logging can start now " << endl; 
											writelog("initiating session" );

  
  
  /////////////////
  
  
											writelog("creating and splitting threads");
											// edit until the next "end editable block" 
											// to add other threads that can assist automatization
											
  
  // messagebus thread
											std::thread(readMsgbus).detach(); 
											writelog("messagebus reader thread created and detached");
 
  // std::thread(readUserCommand).detach(); <<---- no, threads run in background, we need readusercommand to run with the shell
  
  //end editable block
  
  
 
  
  
  
  
  //the following is in foreground ;)
  
											//curr and start are both global, so after hooking up a new child to strt, you can simply set curr to be strt.
  
  strt = new child;									writelog("first node of the linked list of childs created");
 
  curr = strt;										writelog("curent child set as start child");
  
  curr->prv = NULL; 									//no prev
  curr->id = "NULL"; 									//this is the first one, so ID is null, next one will have id = 1;
  curr->nxt = NULL; 									//no next yet
											//other params wont be set
											writelog("child set!");
  
  
  
  
  while(quitCommand != "quit")								// did not ask you to quit yet
  {											writelog("reading command");
    
    string cmnd;									std::cout << "--> ";
											std::cin >> cmnd;
											writelog("command is : "+ cmnd);
    
    if(cmnd == "quit")
    {
	quitCommand = "quit";								writelog("quit command found");
    }
    else
    {											//generate a new child process
	string ID = generateID();							writelog("generated process id");
	
	child * curchld;
	curchld = new child; 								//a very new child.
											writelog("generated a very new child to hold the new process");
	
	curr->nxt = curchld; 								//the new child comes after what curr currently was pointing to
	curchld->prv = curr; 								//curr is the previous of the new child
	
	curr = curchld; 								//now curr is made to denote the current child
	curr->nxt = NULL; 								//curr has no next yet
	
											writelog("pointers swapped");
	
											//set key words
	curr->keywords = {""}; 								//no keywords set for now
	
											//set id
	curr->id = ID;									writelog("id set");
	
	string userCmdLst = get_fromFile(configpath, "steppenwind_usercommands"); 	//get the user commands
	manageConfig(userCmdLst);
	  
	string cmdstr = get_fromFile(userCmdLst, cmnd); 				// get what the current command means
	
	vector<string> strs;
	string zsep = get_fromFile(configpath, "steppenwind_commandinfo_separator");
	boost::split(strs, cmdstr, boost::is_any_of(zsep));
	
	//some of them may as well point out uplink and downlink
	//and some are arguments
	
	//search
	
	/*
	 * TODO implement following in next version
	 * string upl = get_fromVector(strs, "-uplink");
	 * string dnl = get_fromVector(strs, "-downlink");
	 * string keywords = get_fromVector ......
	 * */
	
	
	//for this version get a bulk uplink form configfile. which is the messagebus
	//get a downlink dir, then set a filename with current id with extension downlink
	
	curr->uplink = get_fromFile(configpath, "steppenwind_messagebus"); 		//for thsi run same as steppenwind_messagebus_processed
	curr->downlink = get_fromFile(configpath, "steppenwind_downlinkfolder")+"/"+curr->id+".downlink";
	strs.push_back(curr->downlink);
	
	pid_t p_id = fork();								//fork
	if(p_id == 0)
	{
											//child
											//execute strs[0] with correct arguments :D
	  vector<char *> argvec(strs.size() + 1); 					//one extra for the NULL, no need to explicitely initialize this..
	  for(size_t ii = 0; ii != strs.size(); ++ii)
	  {
	    argvec[ii] = &strs[ii][0]; 							//the first argument is the file name itself, so argvec[0] , will be executed, and is the argument to itself
											//std::cout << "argument was -->" << strs[ii] << "<--" << endl;
											// argvec is a vector of pointers to the strings, more specifically pointers to the first characters of the strings, thus strs[ii][0]
	  }
	  
											//insert the downlink is already done, uplink is constant, also done
											//std::cout << "executing -->" << argvec[0] << "<--" << "from -->" <<userCmdLst << "<--" << "for -->" << cmnd << "<--" <<endl;
	  
	  int execret = execvp(argvec[0], argvec.data());				// return value ..
											//no check now execution correctness 
	  
	  quitCommand = "quit";								// kill the launcher thread, 
                                                                                        // so execret wont be evaluated
	  
	}
	else if (p_id < 0)
	{										//failed
											std::cout << "failed to fork ... continuing with next user command" << endl;
	}
	else
	{
	  //parent 
	  //do nothing as usuall 
	  //dont need a break command
	  //not using case, but if else
	}
	
	
	
    }
  }
  
  
  
  
  
  
  
  
  
  
  
  
}