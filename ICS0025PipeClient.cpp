#include "ICS0025PipeClient.h"

ICS0025PipeClient::ICS0025PipeClient()
{
	this->stop = 0;
	this->Connected = 0;
	this->MessageState = 0;

	char full[_MAX_PATH]; string str_path;
	if (_fullpath(full, ".\\", _MAX_PATH) != NULL)
		str_path = string(full);
	str_path = str_path + string("ICS0025PipeClient.log");
	this->LogFile.open(str_path, ios::out | ios::app);

	this->PrintLog(log_flag::info, "Starting ICS0025PipeClient");
}


ICS0025PipeClient::~ICS0025PipeClient()
{
	try
	{
		this->LogFile.close();
		CloseHandle(this->PipeHandle);
	}
	catch (const std::exception & err)
	{
		this->PrintLog(log_flag::error, string(err.what()));
	}

}

bool ICS0025PipeClient::ConnectPipe()
{
	this->PrintLog(log_flag::info, string("Connecting to " + SERVER_NAME));
	this->PipeHandle = CreateFile(TEXT("\\\\.\\pipe\\ICS0025"),GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (this->PipeHandle == INVALID_HANDLE_VALUE) {
		this->PrintLog(log_flag::exception, string("Connection can not be established with " + SERVER_NAME));
		this->Connected = 0;
		return FALSE;
	}
	else {
		this->PrintLog(log_flag::info, string("Connection is established with " + SERVER_NAME));
		this->stop = 0;
		this->Connected = 1;
		this->MessageState = 0; // 0 = write to server, 1 = read from server
		this->CommandMessage = "ready";
		this->Listening = thread(&ICS0025PipeClient::ListeningHandler, this);
		this->Writing = thread(&ICS0025PipeClient::WritingHandler, this);
		return TRUE;
	}
}

bool ICS0025PipeClient::checkIfConnected()
{
	return this->Connected;
}


bool ICS0025PipeClient::StopPipe()
{
	if (!this->Connected) return TRUE;
	this->PrintLog(log_flag::info, "Stopping ICS0025PipeClient");
	this->CommandMessage = "stop";
	this->stop = 1;
	return TRUE;
}


void ICS0025PipeClient::PrintLog(log_flag flag, string s)
{
	string sflag;
	if (flag == log_flag::error) sflag = "error";
	else if (flag == log_flag::info) sflag = "info";
	else if (flag == log_flag::exception) sflag = "exception";

	thread::id ThreadId = this_thread::get_id();
	lock_guard<mutex> keyboard_lock(temp_mutex);
	this->LogFile << "thread " << ThreadId << ":" << sflag << ":" << s << endl;
}

bool ICS0025PipeClient::WriteToServer()
{
	char szBUFFER[BUFFER_SIZE];
	DWORD bBytes;
	string s = this->CommandMessage;
	const char* message = s.c_str();
	strcpy_s(szBUFFER, sizeof(szBUFFER), message);
	bool Success = WriteFile(this->PipeHandle, szBUFFER, strlen(szBUFFER) + 1, &bBytes, NULL);
	string log_message = string("Sent ") + to_string(int(bBytes)) + string(" bytes to server");
	if (Success) this->PrintLog(log_flag::info, log_message);
	else this->PrintLog(log_flag::error, "Message sending failed");
	if (Success && bBytes == (strlen(szBUFFER) + 1)) {
		this->PrintLog(log_flag::info, "Sent " + s + string(" message to server"));
	}
	this->MessageState = 1;
	return Success;
}

bool ICS0025PipeClient::ReadFromServer()
{
	char szBUFFER[BUFFER_SIZE];
	DWORD bBytes;
	bool Success = ReadFile(this->PipeHandle, szBUFFER, sizeof(szBUFFER), &bBytes, NULL);
	string log_message = string("Received ") + to_string(int(bBytes)) + string(" bytes from server");
	if (Success) this->PrintLog(log_flag::info, log_message);
	else this->PrintLog(log_flag::error, "Receiving is failed from server");
	if (bBytes == 0)
		this->PrintLog(log_flag::exception, string("No item received from server"));
	else if (Success && bBytes != 0) {
		this->PrintLog(log_flag::info, string("Received text: " + string(szBUFFER))); // it can effectively end here
	}
	this->MessageState = 0;
	return (Success || bBytes == 0);
}

void ICS0025PipeClient::ListeningHandler()
{
	this->PrintLog(log_flag::info, "Listening thread started");
	while (1) {
		if (stop) break;
		if (!this->Connected) break;
		if (this->MessageState == 1)
			if (!this->ReadFromServer()) break;
		//pause(500);
	}
	this->PrintLog(log_flag::info, "Listening thread stopped");
	this->PrintLog(log_flag::exception, "Connection stopped");
	this->Connected = 0;
}

void ICS0025PipeClient::WritingHandler()
{
	this->PrintLog(log_flag::info, "Writing thread started");
	while (1) {
		if (!this->Connected) break;
		if (this->MessageState == 0)
			if (!this->WriteToServer()) break;
		//pause(500);
	}
	this->PrintLog(log_flag::info, "Writing thread stopped");
	this->PrintLog(log_flag::exception, "Connection stopped");
	this->Connected = FALSE;
}



