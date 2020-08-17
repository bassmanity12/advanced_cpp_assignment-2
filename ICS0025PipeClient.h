#pragma once

#include "Main.h"

#define SERVER_NAME string("ICS0025PipeServer")
#define BUFFER_SIZE 512

class ICS0025PipeClient
{
private:
	ofstream LogFile;
	string CommandMessage;
	map<string, int> Months;
	//thread ConnectionStatus;
	enum class log_flag { error, info, exception };
	mutex temp_mutex;
	bool MessageState;
	bool Connected = 0;
	bool stop = 0;
	
public:
	HANDLE PipeHandle;
	thread Listening;
	thread Writing;
	ICS0025PipeClient();
	~ICS0025PipeClient();
	bool ConnectPipe();
	bool StopPipe();
	bool checkIfConnected();

private:
	void PrintLog(log_flag, string);
	bool ReadFromServer();
	bool WriteToServer();
	void ListeningHandler();
	void WritingHandler();
};



