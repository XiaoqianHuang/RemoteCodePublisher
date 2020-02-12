#pragma once
///////////////////////////////////////////////////////////////////////
// Display.h - class to insert tag to text
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
*  This package provides functions to insert tag to text
*  Add specific tag to Comment, Function and Class
*
* Required Files:
* ---------------
*  Converter.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Feb 2019
* - first release
* - demonstrate the application meet all the requirement in project#2
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


#ifndef DISPLAY_H
#define DISPLAY_H
#include <vector>
#include <iostream>
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

	class Display {
	public:
		Display(const  std::string& text);
		std::string InsertWalk(CodeAnalysis::ASTNode* pItem, size_t& count, const std::string& text);
		std::string InsertTheTag(const std::string& temp, const size_t& startLineCount, const size_t& endLineCount, std::string type);
		std::string findComment(const std::string & temp, bool* flag);
		size_t checkPos(const size_t& pos, const std::string& line);
		std::string insertTag(const std::string& temp, CodeAnalysis::ASTNode* pScope);
		std::string insertComment(const std::string& temp);
		std::string insertClassAndFunction(const std::string& temp, CodeAnalysis::ASTNode* pScope);

	private:
		CodeAnalysis::ASTNode* pScope;
		std::string text_;
	};

	Display::Display(const  std::string& text) {
		text_ = text;
	}

	//----< insert tag >---------------------------------------------

	inline std::string Display::insertTag(const std::string& temp, CodeAnalysis::ASTNode* pScope) {
		std::string insertedtext = temp;
		insertedtext = insertClassAndFunction(insertedtext, pScope);
		insertedtext = insertComment(insertedtext);
		return insertedtext;
	}

	//----< insert comment tag >---------------------------------------------

	inline std::string Display::insertComment(const std::string& temp) {
		std::string insertedtext;
		std::string line = "";
		bool CommentFlag = false; // flag to record whether is processing multiple(/* */) comment
		size_t linecount = 1;
		for (char c : temp) {
			line = line + c;
			if (c == '\n') {
				line = findComment(line, &CommentFlag);
				linecount++;
				insertedtext = insertedtext + line;
				line = "";
			}
		}
		return insertedtext;
	}

	//----< insert comment tag in line>---------------------------------------------

	inline std::string Display::findComment(const std::string & temp, bool* flag) {
		std::string line = temp;
		auto pos = line.find("//");
		auto pos2 = line.find("/*");
		auto pos3 = line.find("*/");
		if (checkPos(pos, line) != std::string::npos && *flag == false && (pos < pos2 || pos2 == std::string::npos)) { // When single line comment(//) occur, the whole line will be identified as comment
			std::string tagStart = "<div class = \"Comment\">";// "//" is before the "/*"
			line.insert(pos, tagStart);
			std::string tagEnd = "</div>";
			line.insert(line.size() - 1, tagEnd);
		}
		else if (*flag == true) { // determine if in multiple comment(/* */)
			if (checkPos(pos3, line) != std::string::npos) {
				std::string tagEnd = "</div>";
				line.insert(pos3 + 2, tagEnd); // insert tag after "*/" 
				*flag = false;
			}
		}
		else if (checkPos(pos2, line) != std::string::npos && *flag == false) {
			std::string tagStart = "<div class = \"Comment\">";
			line.insert(pos2, tagStart);
			if (pos3 != std::string::npos) { // When "*/" is at the same line with "/*"
				std::string tagEnd = "</div>";
				line.insert(pos3 + tagStart.size() + 2, tagEnd);
				std::string cutline = line.substr(pos3 + tagStart.size() + 2);
				line = /*A demo comment to special case*/ line.substr(0, pos3 + tagStart.size() + 2) + /*Recursive part*/findComment(cutline, flag); // recurse to handle special cases: determine whether there is a single comment after the end of multiple comment
			}
			else {
				*flag = true;
			}
		}
		return line;
	}

	//----< determine if the pos of char is quoted >---------------------------------------------

	inline size_t Display::checkPos(const size_t& pos, const std::string& line) {
		bool qFlag = false;
		size_t charcount = 0;
		for (char c : line) {
			if (c == '\"') {
				qFlag = !qFlag;
			}
			if (charcount == pos && qFlag == true) { // if the char in this position is quoted, the pos is invalid
				return std::string::npos;
			}
			charcount++;
		}
		return pos;
	}

	//----< traverse to insert specific tag>---------------------------------------------

	inline std::string Display::insertClassAndFunction(const std::string& temp, CodeAnalysis::ASTNode* pScope) {
		std::string insertedtext = temp;
		size_t initialcount = 0;
		insertedtext = InsertWalk(pScope, initialcount, insertedtext);

		return insertedtext;
	}

	//----< insert specific tag for each ASTNode >--------------------

	inline std::string Display::InsertWalk(CodeAnalysis::ASTNode* pItem, size_t& count, const std::string& text)
	{
		size_t inCount = ++count;
		std::string convertedText = text;
		auto iter = pItem->children_.begin();
		while (iter != pItem->children_.end())
		{
			convertedText = InsertWalk(*iter, count, convertedText);
			++iter;
		} // or create table to store line, type, end/start
		if (pItem->type_ == "function") {
			convertedText = InsertTheTag(convertedText, pItem->startLineCount_, pItem->endLineCount_, "Function");
		}
		else if (pItem->type_ == "class") {
			convertedText = InsertTheTag(convertedText, pItem->startLineCount_, pItem->endLineCount_, "Class");
		}
		return convertedText;
	}

	//----< insert specific tag for the indicating lines >--------------------

	std::string Display::InsertTheTag(const std::string& temp, const size_t& startLineCount, const size_t& endLineCount, std::string type) {
		std::string convertedText; 
		std::string line = "";
		int linecount = 1;
		for (char c : temp) {
			line = line + c;
			if (c == '\n') {
				if (linecount == startLineCount) {
					auto pos = line.find('{');
					std::string tag = "<div class = \"" + type + "\">";
					line.insert(pos, tag);
				}
				if (linecount == endLineCount - 1 && type == "Class") {
					auto pos = line.find("};");
					std::string tag = "</div>";
					line.insert(pos + 2, tag);
				}
				if (linecount == endLineCount && type != "Class") {
					auto pos = line.find('}');
					std::string tag = "</div>";
					line.insert(pos + 1, tag); 
				}
				linecount++;
				convertedText = convertedText + line;
				line = "";
			}
		}
		return convertedText;
	}
}
#endif