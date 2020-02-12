///////////////////////////////////////////////////////////////////////
// Server.cpp - package that performing functionalities of Publisher
// and response to clients
// Language:    C++, Visual Studio 2019   
// Platform:      Macbook Pro , Win 10  
// Application: Project #4 - Remote Code Page Management - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* ---------------------
*  Package contains one class, Server, that contains a Message-Passing Communication
*  facility. It processes each message by invoking an installed callable object
*  defined by the message's command key.
*
*  Message handling runs on a child thread, so the Server main thread is free to do
*  any necessary background processing (none, so far).
*
*  Required Files:
* -----------------
*  ServerPrototype.h, ServerPrototype.cpp
*  Comm.h, Comm.cpp, IComm.h
*  Message.h, Message.cpp
*  FileSystem.h, FileSystem.cpp
*  Utilities.h
*  IPublisher.h, IPublisher.cpp
*
*  Maintenance History:
* ----------------------
*  ver 2.0 : 4/30/2018
*  - modify to meet reqiurement in Project#4
*  ver 1.0 : 3/27/2018
*  - first release
*/

#include "Server.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../IPubliser/IPublisher.h"
#include <chrono>


namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

std::function<Msg(Msg)> echo = [](Msg msg) {
	Msg reply = msg;
	reply.to(msg.from());
	reply.from(msg.to());
	return reply;
};

template<typename T>
void show(const T& t, const std::string& msg)
{
	std::cout << "\n  " << msg.c_str();
	for (auto item : t)
	{
		std::cout << "\n    " << item.c_str();
	}
}

//----< get children file list from Publisher >---------------------------------------------

std::function<Msg(Msg)> getFiles = [](Msg msg) {
	msg.show();
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getFiles");
	std::string path = msg.value("path");
	if (path != "")
	{
		IPublisher* pPublisher = createPublisher();
		std::vector<std::string> args, filelist;
		if (path == "root") {
			std::string root = pPublisher->getRoot();
			args.push_back(root);
		}
		else {
			args.push_back(path);
		}

		Msg::Attributes attributes = msg.attributes();
		for (Msg::Attributes::iterator iter = attributes.begin(); iter != attributes.end(); iter++) {
			if (iter->first.find("arg") != std::string::npos) {
				args.push_back(iter->second);
			}
		}
		filelist = pPublisher->getFiles(args); // filter the files by specific argument
		size_t count = 0;
		for (size_t i = 0; i < filelist.size(); i++) {
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, filelist[i]);
		}

		delete pPublisher;
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;
};

//----< get children directories list from Publisher >---------------------------------------------

std::function<Msg(Msg)> getDirs = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getDirs");
	std::string path = msg.value("path");
	if (path != "")
	{
		IPublisher* pPublisher = createPublisher();
		std::string rootPath = pPublisher->getRoot();
		if (path == "root") {
			path = rootPath;
		}
		std::string restrictdir = pPublisher->getParentDir(rootPath); // restrict the access from client
		if (path != restrictdir) {
			std::vector<std::string> dirlist = FileSystem::Directory::getDirectories(path);
			for (size_t i = 0; i < dirlist.size(); i++) {
				std::cout << "\n  " << i + 1 << ". " << dirlist.at(i);
			}
			size_t count = 0;
			for (size_t i = 0; i < dirlist.size(); i++) {
				std::string countStr = Utilities::Converter<size_t>::toString(++count);
				std::string key = "dir" + countStr;
				reply.attribute(key, dirlist.at(i));
			}
		}
		else {
			path = rootPath;
		}
		delete pPublisher;
	}
	else
	{
		std::cout << "\n  getDirs message did not define a path attribute";
	}
	return reply;
};

//----< get root from Publisher >---------------------------------------------

std::function<Msg(Msg)> getRoot = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getRoot");
	reply.show();
	IPublisher* pPublisher = createPublisher();
	std::string root = pPublisher->getRoot();
	reply.attribute("root", root);
	delete pPublisher;
	return reply;
};

//----< execute publish from Publisher >---------------------------------------------

std::function<Msg(Msg)> publish = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getConvertedList");

	std::vector<std::string> filelist, convertedFilelist;
	Msg::Attributes attributes = msg.attributes();
	for (Msg::Attributes::iterator iter = attributes.begin(); iter != attributes.end(); iter++) {
		if (iter->first.find("filePath") != std::string::npos) {
			filelist.push_back(iter->second);
		}
	}
	IPublisher* pPublisher = createPublisher();
	convertedFilelist = pPublisher->convertFiles(filelist);
	size_t count = 0;
	for (size_t i = 0; i < convertedFilelist.size(); i++) {
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("convertedFile" + countStr, convertedFilelist[i]);
	}
	delete pPublisher;
	return reply;
};

//----< get converted pages folder dir from Publisher >---------------------------------------------

std::function<Msg(Msg)> getConvertedFolderDir = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getConvertedFolderDir");

	IPublisher* pPublisher = createPublisher();
	std::string dir = pPublisher->getConvertedFolderDir();
	reply.attribute("convertedFolderDir", dir);
	delete pPublisher;
	return reply;
};

//----< get all converted file list from Publisher >---------------------------------------------

std::function<Msg(Msg)> getAllConvertedFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getAllConvertedList");

	std::vector<std::string> filelist;
	IPublisher* pPublisher = createPublisher();
	filelist = pPublisher->getAllConvertedList();
	size_t count = 0;
	for (size_t i = 0; i < filelist.size(); i++) {
		std::string countStr = Utilities::Converter<size_t>::toString(++count);
		reply.attribute("allConvertedFileName" + countStr, filelist[i]);
	}
	delete pPublisher;
	return reply;
};

//----< receive file from Publisher >---------------------------------------------

std::function<Msg(Msg)> receiveFile = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("sendFile");
	reply.attribute("sendingFile", msg.value("fileName"));
	reply.attribute("verbose", "show me");

	//send file operation
	return reply;
};

//----< server quit >---------------------------------------------

std::function<Msg(Msg)> serverQuit = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("serverQuit");
	return reply;
};

//----< main function to start the server >---------------------------------------------

int main(int argc, char *argv[])
{
	if (argc > 2) {
		size_t portNum = atoi(argv[1]);
		MsgPassingCommunication::EndPoint serverEndPoint("localhost", portNum);
		Server server(serverEndPoint, "Project#4 Server - Xiaoqian Huang");
		server.start(argv[2]);

		std::cout << "\n  testing message processing";
		std::cout << "\n ----------------------------";
		server.addMsgProc("getRoot", getRoot); // initialize
		server.addMsgProc("getFiles", getFiles);
		server.addMsgProc("getDirs", getDirs);
		server.addMsgProc("publish", publish);
		server.addMsgProc("getConvertedFolderDir", getConvertedFolderDir);
		server.addMsgProc("getAllConvertedFiles", getAllConvertedFiles);
		server.addMsgProc("receiveFile", receiveFile);
		server.addMsgProc("echo", echo);
		server.addMsgProc("serverQuit", echo);

		//start processing message
		server.processMessages();

		Msg msg(serverEndPoint, serverEndPoint);  // send to self
		msg.name("msgToSelf");
		msg.command("echo");
		msg.attribute("verbose", "show me");
		server.postMessage(msg);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "\n  press enter to exit";
		std::cin.get();
		std::cout << "\n";

		msg.command("serverQuit");
		server.postMessage(msg);
		server.stop();
	}
	else {
		std::cout << "arg setting error!\n";
	}
	return 0;
}

