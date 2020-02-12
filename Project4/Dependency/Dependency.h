#pragma once
///////////////////////////////////////////////////////////////////////
// Dependency.h - class to generate dependency code
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
*  This package provides functions to generate dependency code for
*  converter. The codes in html languange will be created.
*
* Required Files:
* ---------------
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 Mar 2019
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


#ifndef DEPENDENCY_H
#define DEPENDENCY_H
#include <vector>
#include <iostream>
#include <fstream>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../DependencyTable/DependencyTable.h"

namespace Convert
{
	///////////////////////////////////////////////////////////////
    // generate dependency code in html language
	// maintaining the dependency table

	class Dependency {
	public:
		std::string addDepLink(const std::string& filename);
		std::string addTag(const std::string& filename);
		std::string getDir();
		void addDep(const std::string& temp, const std::string& filename, CodeAnalysis::ASTNode* pItem);
		bool checkLink(const std::string filename);
	private:
		DependencyTable depTable;
	};

	//----< create link sentence >---------------------------------------------

	inline std::string Dependency::addDepLink(const std::string& filename) {
		return " <a href=\"" + filename + ".html\">" + filename + "</a>";
	}

	//----< insert html tags >---------------------------------------------

	inline std::string Dependency::addTag(const std::string& filename) {
		std::string addedtext;
		std::string code;
		if (depTable.has(filename)) {
			auto deps = depTable.getDependencies(filename);
			for (size_t i = 0; i < deps.size(); i++) {
				if (checkLink(deps.at(i))) { // check if the file exists
					code = code + addDepLink(deps.at(i));
				}
				else { 
					code = code + " <a href=\"ErrorPage/404NotFound.html\">" + deps.at(i) + "</a>";
				}
			}
				addedtext = "    <h4>";
				addedtext = addedtext + "\n        Dependency Link: ";
				addedtext = addedtext + code;
				addedtext = addedtext + "\n    </h4>";
		}
		else { // no dependency link in file
			addedtext = "    <h4>";
			addedtext = addedtext + "\n        Dependency Link: None";
			addedtext = addedtext + "\n    </h4>";
		}
		return addedtext;
	}

	//----< check if the file exists>-------------------------------------------------------------

	inline bool Dependency::checkLink(const std::string filename) {
	    std::string fullpath = getDir() + filename + ".html";
		bool isExist = FileSystem::File::exists(fullpath);
		bool isGenerated = false;
		if (depTable.has(filename)) { 
			isGenerated = true;
		}
		if (isExist == true || isGenerated == true) { // determine if the file is exists or will be generated
			return true;
		}
		else {
			return false;
		}

	}

	//----< acquire path of convertedPages folder >---------------------------------------------

	inline std::string Dependency::getDir() {
		std::string path = FileSystem::Directory::getCurrentDirectory();
		size_t pos = path.find_last_of("/");
		if (pos >= path.length())
		{
			pos = path.find_last_of("\\");
			if (pos >= path.length())
			{
				std::cout << "convertedPages folder doesn't exist, please create first!" << std::endl;
			}
			else {
				std::string filepath(path.substr(0, pos));
				filepath += "\\";
				filepath += "ConvertedPage\\";
				return filepath;
			}
		}
		else {
			std::string filepath(path.substr(0, pos));
			filepath += "/";
			filepath +="ConvertedPage/";
			return filepath;
		}
		return 0;
	}

	//----< add dependencies >---------------------------------------------

	inline void Dependency::addDep(const std::string& temp, const std::string& currentfilename, CodeAnalysis::ASTNode* pItem) {
		auto iter = pItem->statements_.begin();
		while (iter != pItem->statements_.end()) {
			std::string statement = (*iter)->ToString(); // retrieving statement
			auto pos = statement.find('\"');
			if (pos != std::string::npos) { // determine if is the statement that store dependency
				size_t posS = statement.find_last_of("/");
				size_t posS2 = statement.find_last_of("\\");
				size_t posE = statement.find_last_of("\"");
				std::string filename;
				if (posS != std::string::npos && posE != std::string::npos) {
					filename = statement.substr(posS + 1, posE - posS - 1);
				}
				else if (posS2 != std::string::npos && posE != std::string::npos) {
					filename = statement.substr(posS2 + 1, posE - posS2 - 1);
				}
				else {
					filename = statement.substr(pos + 1, statement.size() - 13); // handle include #"filename"
				}
				std::string processingfile = FileSystem::Path::getName(currentfilename, true);
				depTable.addDependency(currentfilename, filename);
			}
			++iter;
		}
	}
}

#endif