///////////////////////////////////////////////////////////////////////
// Dependency.cpp - class to generate depedency link 
// ver 1.0                                                          
// Language:    C++, Visual Studio 2017   
// Platform:      Macbook Pro , Win 10  
// Application: Project #2 - Source Code Publisher - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*  provides test stub to show the usage of functions to show the 
*  generated code
*
* Maintenance History:
* --------------------
* ver 1.0 : 1 Mar 2019
* - created to meet the requrments in project#2
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - More exception handle
*/

#include "Dependency.h"
#include <iostream>
#include <string>

using namespace Convert;

//----< test stub >---------------------------------------------

#ifdef TEST_DEPENDEDNCY  // only compile the following when defined
int main(int argc, char *argv[])
{
	Dependency dep;
	std::cout << "\n  Testing Dependency: ";
	std::string output;
	dep.addDep("Converter.h");
	dep.addDep("Dependency.h");
	dep.addDep("FileSystem.h");
	output = dep.addTag("Converter.h");
	output = dep.addTag("Dependency.h");
	output = dep.addTag("FileSystem.h");
	std::cout << "\n  The output is: ";
	std::cout << output;
	return 0;
}

#endif