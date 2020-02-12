#pragma once
///////////////////////////////////////////////////////////////////////
// RegexFilter.h - class to select files by a list of Regex patterns
// ver 1.0                                                          
// Language:    C++, Visual Studio 2017   
// Platform:      Macbook Pro , Win 10  
// Application: Project #3 - Code Publisher Client - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                 xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides functions to elect files by a list of Regex 
* patterns.
*
* Required Files:
* ---------------
*  FileSystem.h
*  FileSystem.cpp
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
#ifndef REGEXFILTER_H
#define REGEXFILTER_H
#include <vector>
#include <iostream>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include <regex>

namespace FileSystem
{
	class RegexFilter
	{
	public:
		using filelist = std::vector<std::string>;
		using Regexes = std::vector<std::string>;

		RegexFilter(const  filelist& path);
		void showReglist();

		// Find the matched files
		void MatchRegexPattern(const Regexes& RegPatt);
		void MatchRegex(const std::string& RegPatt);

		// get file name from full path
		std::string getFileName(const std::string& fullpath);

		// return target list (files that match regex)
		filelist TargetList();
	private:
		std::vector<std::string> filelist_;
		filelist targetlist_;
		Regexes RegPatt_;
	};

	//----< construct RegexFilter instance with default pattern >-----

	inline RegexFilter::RegexFilter(const filelist& filelist) : filelist_(filelist)
	{

	}

	//----< handle regex patterns >---------------------------------------------

	inline void RegexFilter::MatchRegexPattern(const Regexes& RegPatt)
	{
		RegPatt_ = RegPatt;
		for (size_t i = 0; i < RegPatt_.size(); i++) {//RegPatt_.at(i) -> std::string
			MatchRegex(RegPatt_.at(i));
		}
	}

	//----< find files that match the regex >---------------------------------------------

	inline void RegexFilter::MatchRegex(const std::string& RegPatt)//flist
	{
		std::smatch result;
		std::regex pattern(RegPatt); // matching the regex
		std::string filename;
		std::cout << "\n  Files that mathced pattern[" << RegPatt << "]:\n";
		for (size_t i = 0; i < filelist_.size(); i++) {
			filename = getFileName(filelist_.at(i));
			if (std::regex_match(filename, result, pattern)) {
				std::cout << "  " << i + 1 << ": " << result[0] << std::endl;// store the result
				targetlist_.push_back(filelist_.at(i));
			}
		}

		if (targetlist_.size() == 0) {
			std::cout << "  No file matched." << std::endl;
		}
	}

	//----< show the files that satisfy the regexes >---------------------------------------------

	inline void RegexFilter::showReglist()//list selected
	{
		std::cout << "\n  File List that matched Regex: \n";
		if (targetlist_.size() == 0) {
			std::cout << "  No file matched." << std::endl;
		}
		else {
			for (size_t i = 0; i < targetlist_.size(); i++)
				std::cout << "  " << i + 1 << ": " << targetlist_.at(i) << std::endl;
		}
	}

	//----< get file name from full path >---------------------------------------------

	inline std::string RegexFilter::getFileName(const std::string& fullpath)//get filename from full patg 
	{
		std::string filename = Path::getName(fullpath, true);
		return filename;
	}

	//----< get the selected list >--------------------------------------------- 

	inline RegexFilter::filelist RegexFilter::TargetList()//get filename from full path
	{
		return targetlist_;
	}
}

#endif 