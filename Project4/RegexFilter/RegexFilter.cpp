///////////////////////////////////////////////////////////////////////
// RegexFilter.cpp - class to select files by a list of Regex patterns
// ver 1.0                                                          
// Language:    C++, Visual Studio 2017   
// Platform:      Macbook Pro , Win 10  
// Application: Project #1 - Source Code Publisher - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*  This package provides a test stub to test functions in RegexFilter.h
*
* Required Files:
* ---------------
*  StringUtilities.h
*  CodeUtilities.h
*  FileSystem.h
*  FileSystem.cpp
*  RegexFilter.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Feb 2019
* - first release
* - demonstrate the application meet all the requiremnet in project#1
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

//----< test stub >---------------------------------------------

#ifdef TEST_REGEXFILTER  // only compile the following when defined

#include <vector>
#include <iostream>
#include <regex>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "../RegexFilter/RegexFilter.h";
#include "../DirectoryNavigator/Utilities/StringUtilities/StringUtilities.h"
#include "../DirectoryNavigator/Utilities/CodeUtilities/CodeUtilities.h"

using namespace FileSystem;
using namespace Utilities;

int main(int argc, char *argv[])
{
	Title("Test stub of RegexFilter");
	std::vector<std::string> filelist, RegPatt;
	filelist.push_back("Afile");
	filelist.push_back("Bfile");
	filelist.push_back("Cfile");
	filelist.push_back("Dfile");
	filelist.push_back("Efile");
	filelist.push_back("Ffile");
	RegPatt.push_back("[A-C](.*)");
	RegPatt.push_back("[F](.*)");

	RegexFilter rf(filelist);

	std::cout << "\n  Filelist:" << std::endl;
	for (int i = 0; i < filelist.size(); i++) {
		std::cout << "  file " << i + 1 << ":" << filelist.at(i) << std::endl;
	}

	rf.MatchRegexPattern(RegPatt);
	rf.showReglist();

	std::cout << "\n\n";

	std::vector<std::string> targetlist;
	targetlist = rf.TargetList();
	return 0;
}

#endif