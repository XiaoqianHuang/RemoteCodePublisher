#pragma once
#pragma once
///////////////////////////////////////////////////////////////////////
// Server.cs - package that performing functionalities of Publisher
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
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include <windows.h>
#include <tchar.h>

namespace Repository
{
	using File = std::string;
	using Files = std::vector<File>;
	using Dir = std::string;
	using Dirs = std::vector<Dir>;
	using SearchPath = std::string;
	using Key = std::string;
	using Msg = MsgPassingCommunication::Message;
	using ServerProc = std::function<Msg(Msg)>;
	using MsgDispatcher = std::unordered_map<Key, ServerProc>;

	const std::string storageRoot = "../Storage";  // root for all server file storage

	class Server
	{
	public:
		Server(MsgPassingCommunication::EndPoint ep, const std::string& name);
		void start(const std::string& path);
		void stop();
		void addMsgProc(Key key, ServerProc proc);
		void processMessages();
		void postMessage(MsgPassingCommunication::Message msg);
		MsgPassingCommunication::Message getMessage();
		static Dirs getDirs(const std::string& path = storageRoot);
		static Files getFiles(const std::string& path = storageRoot);
	private:
		MsgPassingCommunication::Comm comm_;
		MsgDispatcher dispatcher_;
		std::thread msgProcThrd_;
	};
	//----< initialize server endpoint and give server a name >----------

	inline Server::Server(MsgPassingCommunication::EndPoint ep, const std::string& name)
		: comm_(ep, name) {}

	//----< start server's instance of Comm >----------------------------

	inline void Server::start(const std::string& path)
	{
		comm_.setSendFilePath(path);
		comm_.start();
	}
	//----< stop Comm instance >-----------------------------------------

	inline void Server::stop()
	{
		if (msgProcThrd_.joinable())
			msgProcThrd_.join();
		comm_.stop();
	}
	//----< pass message to Comm for sending >---------------------------

	inline void Server::postMessage(MsgPassingCommunication::Message msg)
	{
		comm_.postMessage(msg);
	}
	//----< get message from Comm >--------------------------------------

	inline MsgPassingCommunication::Message Server::getMessage()
	{
		Msg msg = comm_.getMessage();
		return msg;
	}
	//----< add ServerProc callable object to server's dispatcher >------

	inline void Server::addMsgProc(Key key, ServerProc proc)
	{
		dispatcher_[key] = proc;
	}
	//----< start processing messages on child thread >------------------

	inline void Server::processMessages()
	{
		auto proc = [&]()
		{
			if (dispatcher_.size() == 0)
			{
				std::cout << "\n  no server procs to call";
				return;
			}
			while (true)
			{
				Msg msg = getMessage();
				std::cout << "\n  received message: " << msg.command() << " from " << msg.from().toString();
				if (msg.containsKey("verbose"))
				{
					std::cout << "\n";
					msg.show();
				}
				if (msg.command() == "serverQuit")
					break;
				Msg reply = dispatcher_[msg.command()](msg);
				if (msg.to().port != msg.from().port)  // avoid infinite message loop
				{
					postMessage(reply);
					msg.show();
					reply.show();
				}
				else
					std::cout << "\n  server attempting to post to self";
			}
			std::cout << "\n  server message processing thread is shutting down";
		};
		std::thread t(proc);
		//SetThreadPriority(t.native_handle(), THREAD_PRIORITY_HIGHEST);
		std::cout << "\n  starting server thread to process messages";
		msgProcThrd_ = std::move(t);
	}
}