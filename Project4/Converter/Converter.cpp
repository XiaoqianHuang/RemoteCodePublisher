///////////////////////////////////////////////////////////////////////
// Converter.cpp - class to convert a file to decorated html file
// ver 2.0                                                          
// Language:    C++, Visual Studio 2017   
// Platform:      Macbook Pro , Win 10  
// Application: Project #2 - Source Code Publisher - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
*  This package provides functions to process file list to be converted
*  one by one through the converter and a test stub. 
*  Creating Parsers for locating posistions to insert tags.
*
* Required Files:
* ---------------
*  StringUtilities.h
*  CodeUtilities.h
*  FileSystem.h
*  Converter.h
*  FileSystem.cpp
*  StringUtilities.h
*  Display.h
*  Dependency.h
*
* Maintenance History:
* --------------------
* ver 2.0 : 1 Mar 2019
* - modified to meet the requrments in project#2
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

#include <vector>
#include <iostream>
#include <regex>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "../DirectoryNavigator/Utilities/StringUtilities/StringUtilities.h"
#include "../DirectoryNavigator/Utilities/CodeUtilities/CodeUtilities.h"
#include "../Converter/Converter.h"
#include "../Display/Display.h"
#include "../Dependency/Dependency.h"

using namespace FileSystem;
using namespace Utilities;
using namespace CodeAnalysis;

namespace Convert {

	//----< constructor >------------------------------------------------------------------------

	ConvertList::ConvertList(const  std::vector<std::string>& filelist) {
		
		filelist_ = filelist;
	}

	//----< processing list of files to convert the text >---------------------------------------------

	void ConvertList::Convertlist() {
		Dependency dep;
		std::vector<std::string> textList;
		for (size_t i = 0; i < filelist_.size(); i++) {
			std::string file = filelist_.at(i);
			std::string temp;
			std::string filename = Path::getName(file, true);
			std::cout << "\n  ------------------------------------------------------------------";
			std::cout << "\n  Processing file " + filename;
			ASTNode* pScope = CreateParser(file);
			HTMLConverter ct(file);
			temp = ct.Openfile(file);
			temp = ct.transMarkup(temp);
			std::cout << "\n    Markups are translated in [" << filename << "]...";
			Display dp(temp);
			temp = dp.insertTag(temp, pScope);
			std::cout << "\n    Tags(Comment, Function, Class) are inserted to [" << filename << "]...";
			temp = ct.insertPreTag(temp);
			dep.addDep(temp, filename, pScope);
			textList.push_back(temp);
			std::cout << "\n  ------------------------------------------------------------------\n";
		}
		std::cout << "\n ----------------------------------------------------------------------------------------";
		std::cout << "\n  Demonstrating Dependency..";
		std::cout << "\n  - Dependency Table is created.";
		std::cout << "\n  - Dependency Link will be added to the html files.";
		std::cout << "\n  - If the html file not exists, the link will be point to error page \"404NotFound\".";
		std::cout << "\n ----------------------------------------------------------------------------------------";
		for (size_t i = 0; i < filelist_.size(); i++) {
			std::cout << "\n  ------------------------------------------------------------------";
			std::string file = filelist_.at(i);
			std::string temp = textList.at(i);
			std::string filename = Path::getName(file, true);
			temp = dep.addTag(filename) + temp;
			std::cout << "\n    Dependency links are added to [" << filename << "]...";
			HTMLConverter ct(file);
			temp = ct.insertTem(temp);
			ct.createFile(temp, filename);
			generatedlist_.push_back(ct.generatedfile());
			std::cout << "  ------------------------------------------------------------------\n";
		}
	}

	//----< return generated list of converted files >---------------------------------------------

	std::vector<std::string> ConvertList::generatedlist() {
		return generatedlist_;
	}

	//----< create parser for a specific file >---------------------------------------------

	ASTNode* ConvertList::CreateParser(const std::string& file) {
		pParser = configure.Build();
		std::string name;
		try
		{
			if (pParser)
			{
				name = FileSystem::Path::getName(file);
				if (!configure.Attach(file))
				{
					std::cout << "\n  could not open file " << name << std::endl;
					return 0;
				}
			}
			else
			{
				std::cout << "\n\n  Parser not built\n\n";
				return 0;
			}
			// save current package name
			pRepo = Repository::getInstance();
			pRepo->package() = name;

			// parse the package
			while (pParser->next())
			{
				pParser->parse();
			}
			std::cout << "\n";

			// final AST operations
			ASTNode* pGlobalScope = pRepo->getGlobalScope();
			return pGlobalScope;
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
			std::cout << "\n  exception caught at line " << __LINE__ << " ";
			std::cout << "\n  in package \"" << name << "\"";
			return 0;
		}
	}
}

//----< test stub >---------------------------------------------

#ifdef TEST_CONVERTER  // only compile the following when defined
int main(int argc, char *argv[])
{
	Title("Test stub of Converter");
	std::vector<std::string> filelist, RegPatt;
	std::string file1, file2, file3, file4, file5;

	file1 = FileSystem::Path::getFullFileSpec("../DirectoryNavigator/DirExplorer-Naive/DirExplorerN.h");
	file2 = FileSystem::Path::getFullFileSpec("../DirectoryNavigator/Utilities/CodeUtilities/CodeUtilities.h");
	file3 = FileSystem::Path::getFullFileSpec("./Converter.h");
	file4 = FileSystem::Path::getFullFileSpec("../Executive/Executive.cpp");
	file5 = FileSystem::Path::getFullFileSpec("../Parser/Parser.h");

	filelist.push_back(file1);
	filelist.push_back(file2);
	filelist.push_back(file3);
	filelist.push_back(file4);
	filelist.push_back(file5);

	std::cout << "\n  Filelist:" << std::endl;
	for (int i = 0; i < filelist.size(); i++) {
		std::cout << "  file " << i + 1 << ":" << filelist.at(i) << std::endl;
	}

	Convert::ConvertList cl(filelist);
	cl.Convertlist();
	// open each file and display a few lines of text
	return 0;
}

#endif