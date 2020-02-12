///////////////////////////////////////////////////////////////////////
// Publisher.cpp - process functions required by WPF and return the result.                                                         
// Language:    C++, Visual Studio 2019   
// Platform:      Macbook Pro , Win 10  
// Application: Project #4 - Code Publisher Client - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is responsible for translating managed types into
* native types and for delegating managed calls to native calls.
*
* Required Files:
* ---------------
*  IPublisher.h
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
#include "IPublisher.h"
#include "../DirectoryNavigator/DirExplorer-Naive/DirExplorerN.h"
#include "../DirectoryNavigator/FileSystem/FileSystem.h"
#include "../Converter/Converter.h"
#include "../RegexFilter/RegexFilter.h"
#include<cstring>

using namespace FileSystem;
using namespace Convert;

class Publisher : public IPublisher {
	public:

		//----< get converted file list from Publisher >---------------------------------------------

		virtual std::vector<std::string> convertFiles(const std::vector<std::string>& filelist) override {
			std::vector<std::string> convertedlist;
			std::cout << "\n\n ======================================================";
			std::cout << "\n  Publisher: converting selected list..";
			std::cout << "\n ======================================================\n";
			if (filelist.size() > 0) {

				ConvertList cl(filelist);
				demoCon();
				cl.Convertlist();
				convertedlist = cl.generatedlist();
			}
			else {
				std::cout << "\n\n  No file being converted!";
			}
			std::cout << "\n =======================Leaving========================\n";
			return convertedlist;
		}

		//----< get children directories list from Publisher >---------------------------------------------

		virtual std::vector<std::string> getDirs(const std::string& path) override {
			std::vector<std::string> temp;
			try {
				std::cout << "\n\n ======================================================";
				std::cout << "\n  Publisher: geting dirs..";
				std::cout << "\n ======================================================\n";
				std::cout << "\n  Returning dirs:";
				std::vector<std::string> dirlist = FileSystem::Directory::getDirectories(path);
				for (size_t i = 0; i < dirlist.size(); i++) {
					std::cout << "\n  " << i + 1 << ". " << dirlist.at(i);
				}
				std::cout << "\n =======================Leaving========================\n";
				return dirlist;
			}
			catch (std::exception&ex)
			{
				std::cout << ex.what() << "\n";
			}
			return temp;
		}

		//----< get children file list from Publisher >---------------------------------------------

		virtual std::vector<std::string> getFiles(const std::vector<std::string>& args) override {
			std::vector<std::string> temp;
			try {
				std::cout << "\n\n ======================================================";
				std::cout << "\n  Publisher: geting files..";
				std::cout << "\n ======================================================\n";
				char** argv = convertString(args);
				std::vector<std::string> targetlist;
				std::vector<std::string> filelist;
				ProcessCmdLine pcl((int)args.size(), argv);
				if (!pcl.parseError()) {
					DirExplorerN de(pcl.path());
					demoNav();
					preface("Command Line: ");
					pcl.showCmdLine();
					if (pcl.parseError())
					{
						pcl.usage();
					}
					for (auto patt : pcl.patterns())
					{
						de.addPattern(patt);
					}
					if (pcl.hasOption('s'))
					{
						de.recurse();
					}
					de.search();
					de.showStats();
					std::vector<std::string> RegPatt = pcl.regexes();// acquire regex pattern from command line
					filelist = de.Flist();// acquire selected file list from navigator
					RegexFilter rf(filelist);
					demoReg();
					rf.MatchRegexPattern(RegPatt);
					rf.showReglist();// show the list of files selected
					targetlist = rf.TargetList();// selected files
					delete[] argv;
					std::cout << "\n =======================Leaving========================\n";
					return targetlist;
				}
				return targetlist;
			}
			catch (std::exception&ex)
			{
				std::cout << ex.what() << "\n";
			}
			return temp;
		}

		//----< get parent directories list from Publisher >---------------------------------------------

		virtual std::string getParentDir(const std::string& path) override {
			return getParentdir(path);
		}

		//----< get path of Converted Folder from Publisher >--------------------------------------------

		virtual std::string getConvertedFolderDir() override {
			return getConvertedDir();
		}

		//----< get root path from Publisher >---------------------------------------------

		virtual std::string getRoot() override {
			demoRequirement();
			std::string dir = FileSystem::Directory::getCurrentDirectory();
			dir = getParentdir(dir);
			return dir;
		}

		//----< get all existing file of Converted Folder >---------------------------------------------

		virtual std::vector<std::string> getAllConvertedList() override {
			std::string path;
			std::string temp = getConvertedDir();
			for (size_t i = 0; i < temp.size() - 1; i++)
			{
				path = path + temp[i];
			}
			std::vector<std::string> files = FileSystem::Directory::getFiles(path, "*.html");
			return files;
		}

		char** convertString(const std::vector<std::string>& args);
		void demoNav();
		void demoReg();
		void demoCon();
		std::string getConvertedDir();
		std::string getParentdir(const std::string& parentdir);
		void demoRequirement();
};

IPublisher* createPublisher() {
	return new Publisher();
}

//----< convert vector<string> to char** >---------------------------------------------

char** Publisher::convertString(const std::vector<std::string>& args) {
	char** argv = new char* [args.size()];
    for (size_t i = 0; i < args.size(); i++) {
		argv[i] = new char[strlen(args[i].c_str()) + 1];
		memcpy(argv[i], args[i].c_str(), strlen(args[i].c_str()) + 1);
    }
    return argv;
}

//----< demonstrate Navigator and acquire files that satisfy the patterns >---------------------------------------------

void Publisher::demoNav() {

	std::cout << "\n ----------------------------------------------------------------------------------------";
	std::cout << "\n  Navigating..";
	std::cout << "\n  - All the directories and files navigated will be processed.";
	std::cout << "\n ----------------------------------------------------------------------------------------";
}

//----< demonstrate Regex Filter to select the files that satisfy the Regex >---------------------------------------------

void Publisher::demoReg() {
	std::cout << "\n ----------------------------------------------------------------------------------------";
	std::cout << "\n  Processing Regex Filter..";
	std::cout << "\n  - All the files matching the regex pattern from cmd line will be demonstrated.";
	std::cout << "\n ----------------------------------------------------------------------------------------";
}

//----< demonstrate Converter to process the files and saved as html files >---------------------------------------------

void Publisher::demoCon() {
	std::cout << "\n ----------------------------------------------------------------------------------------";
	std::cout << "\n  Converting Files..";
	std::cout << "\n  - All the files matching the regex pattern will be converted to html files with the";
	std::cout << "\n    same name and stored in \"ConvertedPage\" folder.";
	std::cout << "\n  - Support wrapping part for: ";
	std::cout << "\n    1. Comment (including processing special cases linke multiple comments and a single comment in the same line)";
	std::cout << "\n    2. Class";
	std::cout << "\n    3. Function";
	std::cout << "\n ----------------------------------------------------------------------------------------";
}

//----< acquire path of convertedPages folder >---------------------------------------------

std::string Publisher::getConvertedDir() {
	std::string path = FileSystem::Directory::getCurrentDirectory();
	size_t pos = path.find_last_of("/");
	std::string filepath;
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
		filepath += "ConvertedPage/";
		return filepath;
	}
	return path;
}

//----< function to get parent dir >---------------------------------------------

std::string Publisher::getParentdir(const std::string& parentdir) {
	size_t pos = parentdir.find_last_of("/");
	std::string path;
	if (pos >= parentdir.length())
	{
		pos = parentdir.find_last_of("\\");
		if (pos >= parentdir.length())
		{
			std::cout << "convertedPages folder doesn't exist, please create first!" << std::endl;
		}
		else {
			std::string filepath(parentdir.substr(0, pos));
			return filepath;
		}
	}
	else {
		std::string filepath(parentdir.substr(0, pos));
		return filepath;
	}
	return path;
}

//----< demonstrate requirement >---------------------------------------------

void Publisher::demoRequirement(){
	std::cout << "\n\n ======================================================";
	std::cout << "\n  CSE 687 - Project#4 Remote Code Page Management";
	std::cout << "\n  Xiaoqian Huang";
	std::cout << "\n ======================================================\n";

	std::cout << "\n  Requirment#3 - assemble working parts from Projects #1, #2, and #3 into a Client-Server configuration";
	std::cout << "\n  All parts of my project #1, #2 and #3 is integrated.\n";

	std::cout << "\n  Requirment#4 - provide a Graphical User Interface (GUI) for the client that supports navigating remote directories to find a project for conversion, and supports displaying the conversion results in a way that meets Project #3 requirements";
	std::cout << "\n  The GUI that meet the requirment is opened\n";

	std::cout << "\n  Requirment#5 -  provide message designs appropriate for this application";
	std::cout << "\n  Both client and server have different messages with different command to send and receive\n";

	std::cout << "\n  Requirment#6 - support converting source code in the server and, with a separate request, transferring one or more converted files back to the local client, using the communication channel";
	std::cout << "\n  The source code is converted and transfered which would be opened in browser and pop up window in the end of automated demonstration\n";

	std::cout << "\n  Requirment#7 demonstrate correct operations for two or more concurrent clients";
	std::cout << "\n  the concurrent clients will be opened with run.bat command\n";

	std::cout << "\n  Requirment#8 automated test";
	std::cout << "\n  the automated test for two clients will be opened\n";

	std::cout << "\n  Details for processing GUI..";
}

