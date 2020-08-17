#include "ICS0025PipeClient.h"


vector<string> split(const string& str, const string& delim) {
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}


void start()
{
	ICS0025PipeClient Client;
	string inputStr = "";
	mutex temp_mutex;
	bool CommandConnect = 0;
	cout << "Valid commands: connect, stop, exit" << endl;
	cout << "Debugging info being logged into ICS0025PipeClient.log" << endl;
	while (1) {
		lock_guard<mutex> kbd_lock(temp_mutex);
		cout << "Type: ";
		std::getline(std::cin >> std::ws, inputStr);
		if (inputStr == "connect") {
			if (CommandConnect && Client.checkIfConnected()) { cout << "Server is running" << endl; continue; }
			else if (CommandConnect && Client.checkIfConnected() == 0) { cout << "Connection lost" << endl; continue; }
			else if (Client.ConnectPipe()) {
				CommandConnect = 1;
				Client.Listening.detach();
				Client.Writing.detach();
				cout << "Connection established, all results are logged in real time in the log file" << endl;
			}
			else cout << "Could not connect to server" << endl;
		}
		else if (inputStr == "stop") {
			if (Client.checkIfConnected() == 0 || !CommandConnect) { cout << "Connection already stopped" << endl; continue; }
			CommandConnect = 0;
			Client.StopPipe();
		}
		else if (inputStr == "exit") {
			CommandConnect = 0;
			Client.StopPipe();
			break;
		}
		else {
			cout << "invalid command" << endl;
		}
	}
}
