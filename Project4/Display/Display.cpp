///////////////////////////////////////////////////////////////////////
// Display.cpp - test stub to test functions in Display
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
*  This package provides a test stub to test functions in RegexFilter.h
*
* Required Files:
* ---------------
*  Display.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 1 Mar 2019
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

#ifdef TEST_DISPLAY  // only compile the following when defined

#include <vector>
#include <iostream>
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "./Display.h"

using namespace CodeAnalysis;
using namespace Convert;
using namespace FileSystem;

//----< open file and get all the text >---------------------------------------------

std::string Openfile(std::string filename) {
	File file(filename);
	file.open(File::in);
	if (!file.isGood())
	{
		std::cout << "\n  Can't open file " << file.name();
		std::cout << "\n  Here's what the program can't find:\n  " << Path::getFullFileSpec(file.name());
	}
	std::string temp = std::string("\n  --Processing file ") + filename;
	std::cout << temp;
	std::string text_ = file.readAll(true);
	return text_;
}

int main(int argc, char *argv[])
{
	Parser* pParser;
	Repository* pRepo;
	ConfigParseForCodeAnal configure;
	ASTNode* pScope;

	std::string file;

	file = FileSystem::Path::getFullFileSpec("../DirectoryNavigator/DirExplorer-Naive/DirExplorerN.h");

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
		pScope = pRepo->getGlobalScope();
		std::string temp = Openfile(file);
		Display dp(temp);
		temp = dp.insertTag(temp, pScope);
		std::cout << "\n  Added text: " << temp;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
		std::cout << "\n  exception caught at line " << __LINE__ << " ";
		std::cout << "\n  in package \"" << name << "\"";
		return 0;
	}
	return 0;
}

#endif