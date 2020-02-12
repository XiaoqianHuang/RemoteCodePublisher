#pragma once
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////
// IPublisher.h - Interface of calling the function of Publisher                                                         
// Language:    C++, Visual Studio 2019   
// Platform:      Macbook Pro , Win 10  
// Application: Project #3 - Code Publisher Client - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is responsible for providing an Interface of Publisher
*
* Required Files:
* ---------------
*  /
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Feb 2019
* - first release
* - demonstrate the application meet all the requiremnet in project#3
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

struct IPublisher
{
	virtual ~IPublisher() {}
	virtual std::vector<std::string> convertFiles(const std::vector<std::string>& args) = 0;
	virtual std::vector<std::string> getFiles(const std::vector<std::string>& args) = 0;
	virtual std::vector<std::string> getDirs(const std::string& path) = 0;
	virtual std::string getParentDir(const std::string& path) = 0;
	virtual std::string getConvertedFolderDir() = 0;
	virtual std::string getRoot() = 0;
	virtual std::vector<std::string> getAllConvertedList() = 0;
};

IPublisher* createPublisher();
