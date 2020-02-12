#pragma once
///////////////////////////////////////////////////////////////////////
// Converter.h - class to convert a file to decorated html file
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
*  This package provides functions to convert a file to a decorated 
*  html file that can be demonstrated in readable style including 
*  five main steps:
*  1. translate markup: "< => &lt;" "> => &gt;"
*  2. add dependency links
*  3. add <pre> </pre> to the text
*  4. insert the text to html template which stored in an .txt
*  5. create the html with the generated text and store it  in
*      "convertedPages" folder
*
* Required Files:
* ---------------
*  CodeUtilities.h
*  FileSystem.h
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


#ifndef CONVERTER_H
#define CONVERTER_H
#include <vector>
#include <iostream>
#include <fstream>
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "../DirectoryNavigator/Utilities/CodeUtilities/CodeUtilities.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"

namespace Convert
{
	///////////////////////////////////////////////////////////////
    // class to provide functions for processing list of files and 
	// creating parser

	class ConvertList {
	public:
		ConvertList(const  std::vector<std::string>& path);
		using filelist = std::vector<std::string>;
		void Convertlist();
		std::vector<std::string> generatedlist();
		CodeAnalysis::ASTNode* CreateParser(const std::string& file);

	private:
		filelist filelist_;
		filelist generatedlist_;
		CodeAnalysis::Parser* pParser;
		CodeAnalysis::Repository* pRepo;
		CodeAnalysis::ConfigParseForCodeAnal configure;
		CodeAnalysis::ASTNode* pScope;
	};

	///////////////////////////////////////////////////////////////
    // class to provide functions for converting text and generate
    // html file

	class HTMLConverter
	{
	public:
		using file = std::string;

		HTMLConverter(const  file& path);

		std::string Openfile(std::string filename) ;
		std::string transMarkup(const std::string& temptext);
		std::string replaceChar(const std::string& text, const char& from, const std::string& to);
		std::string insertPreTag(const std::string& temp);
		std::string insertTem(const std::string& temp);
		bool createFile(const std::string& filetext, const std::string& filename);
		std::string generatePath(const std::string& filename);
		std::string generatedfile();
		// std::string addDep(const std::string& temp, CodeAnalysis::ASTNode* pItem);

	private:
		file file_;
		std::string text_;
		std::string generatedfile_;
	};

	//----< construct Converter instance with default pattern >-----

	inline HTMLConverter::HTMLConverter(const  file& file) : file_(file){}

	//----< get the generated file name >---------------------------------------------

	inline std::string HTMLConverter::generatedfile() {
		return generatedfile_;
	}

	//----< open file and get all the text >---------------------------------------------

	inline std::string HTMLConverter :: Openfile(std::string filename) {
		FileSystem::File file(filename);
		file.open(FileSystem::File::in);
		if (!file.isGood())
		{
			std::cout << "\n  Can't open file " << file.name();
			std::cout << "\n  Here's what the program can't find:\n  " << FileSystem::Path::getFullFileSpec(file.name());
		}
		std::string temp = std::string("\n  --Processing file ") + filename;
		std::cout << temp;
		text_ = file.readAll(true);
		return text_;
	}

	//----< translate markup >---------------------------------------------

	inline std::string HTMLConverter::transMarkup(const std::string& temptext) {
		std::string trantext = temptext;
		trantext = replaceChar(trantext,'<',"&lt;");
		trantext = replaceChar(trantext, '>', "&gt;");
		return trantext;
	}

	//----< replace a char to other string >---------------------------------------------

	inline std::string HTMLConverter::replaceChar(const std::string& temptext, const char& from, const std::string& to) {
		std::string text = temptext;
		size_t pos = 0;
		bool done = 0;
		while (!done) {
			pos = text.find(from);//find the first possition of char
			if (pos == std::string::npos)// judge if there exists the char
			{
				done = 1;
			}
			else {
				text.replace(pos, 1, to);// replace char with other string
			}
		}
		return text;
	}

	//----< insert tag >---------------------------------------------

	inline std::string HTMLConverter::insertPreTag(const std::string& temp) {
		std::string insertedtext = temp;
		insertedtext = "\n    <pre>\n" + insertedtext + "\n    </pre>";
		return insertedtext;
	}

	//----< insert the text into an html template >---------------------------------------------

	inline std::string HTMLConverter::insertTem(const std::string& temp) {
		std::string filepath = FileSystem::Path::getFullFileSpec("../Converter/htmlTemplate.txt");
		std::string templatetext = Openfile(filepath);
		try {
			size_t pos = 0;
			bool done = 0;
			pos = templatetext.find("</body>");//find the end of template
			std::string temptext = temp + "\n";
			templatetext.replace(pos, 0, temptext);// insert code into the template
			}
		catch (...) {
			std::cout << "\n  Errors in opening html template!" << std::endl;
		}
		return templatetext;
	}

	//----< create the html files according to the file name >---------------------------------------------

	inline bool HTMLConverter::createFile(const std::string& filetext, const std::string& filename) {
		std::string filepath = generatePath(filename);
		try {
			std::ofstream newfile(filepath);
			newfile << filetext << std::endl;  // write the text
			std::cout << "\n  >>File [" << filename << ".html] is successfully created!" << std::endl;
			newfile.close(); // close the file stream
			generatedfile_ = filename + ".html"; // store the file name of generated list
			return true;
		}
		catch (...) {
			std::cout << "Errors in creating html file!" << std::endl;
			return false;
		}
		return true;
	}

	//----< generate relative path >---------------------------------------------

		inline std::string HTMLConverter::generatePath(const std::string& filename) {
			std::string temp;
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
						filepath += "\\ConvertedPage\\";
						filepath += filename;
						filepath += ".html";
						return filepath;
				}
			}
			else {
				std::string filepath(path.substr(0, pos));
					filepath += "/ConvertedPage/";
					filepath += filename;
					filepath += ".html";
					return filepath;
			}
			throw("No return value");
		}
}
#endif