//static const WORD MAX_CONSOLE_LINES = 500;
//void RedirectIOToConsole();
//
//
//
//
//
//class VirtualConsole {
//	HANDLE near_end, //Our end of the pipe, where we'll write.
//		far_end, //The other end.
//		process; //The client.
//public:
//	bool good;
//	VirtualConsole(const std::string& name, ULONG color);
//	~VirtualConsole();
//	void put(const char* str, size_t size = 0);
//	void put(const std::string& str) {
//		this->put(str.c_str(), str.size());
//	}
//};
//
//VirtualConsole::VirtualConsole(const std::string& name, ULONG color) {
//	this->good = 0;
//	SECURITY_ATTRIBUTES sa;
//	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	sa.bInheritHandle = 1;
//	sa.lpSecurityDescriptor = 0;
//	if (!CreatePipe(&this->far_end, &this->near_end, &sa, 0)) {
//		assert(this->near_end == INVALID_HANDLE_VALUE);
//		return;
//	}
//	SetHandleInformation(this->near_end, HANDLE_FLAG_INHERIT, 0);
//	PROCESS_INFORMATION pi;
//	STARTUPINFO si;
//	ZeroMemory(&pi, sizeof(pi));
//	ZeroMemory(&si, sizeof(si));
//	si.cb = sizeof(STARTUPINFO);
//	si.hStdInput = this->far_end;
//	si.dwFlags |= STARTF_USESTDHANDLES;
//	TCHAR program[] = TEXT("console.exe");
//	TCHAR arguments[100];
//#ifndef UNICODE
//	sprintf_s(arguments, "%d", color);
//#else
//	swprintf(arguments, L"0 %d", color);
//#endif
//	if (!CreateProcess(program, arguments, 0, 0, 1, CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, 0, 0, &si, &pi))
//		return;
//	this->process = pi.hProcess;
//	CloseHandle(pi.hThread);
//	this->good = 1;
//
//	this->put(name);
//	this->put("\n", 1);
//}
//
//VirtualConsole::~VirtualConsole() {
//	if (this->near_end != INVALID_HANDLE_VALUE) {
//		if (this->process != INVALID_HANDLE_VALUE) {
//			TerminateProcess(this->process, 0);
//			CloseHandle(this->process);
//		}
//		CloseHandle(this->near_end);
//		CloseHandle(this->far_end);
//	}
//}
//
//void VirtualConsole::put(const char* str, size_t size) {
//	if (!this->good)
//		return;
//	if (!size)
//		size = strlen(str);
//	DWORD l;
//	WriteFile(this->near_end, str, size, &l, 0);
//}
//
//
//void RedirectIOToConsole()
//{
//	int hConHandle;
//	long lStdHandle;
//	CONSOLE_SCREEN_BUFFER_INFO coninfo;
//	FILE* fp;
//
//	// allocate a console for this app
//	AllocConsole();
//
//	// set the screen buffer to be big enough to let us scroll text
//	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
//	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
//	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
//
//	// redirect unbuffered STDOUT to the console
//	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
//	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//	fp = _fdopen(hConHandle, "w");
//
//	*stdout = *fp;
//
//	setvbuf(stdout, NULL, _IONBF, 0);
//
//	// redirect unbuffered STDIN to the console
//
//	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
//	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//	fp = _fdopen(hConHandle, "r");
//	*stdin = *fp;
//	setvbuf(stdin, NULL, _IONBF, 0);
//
//	// redirect unbuffered STDERR to the console
//	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
//	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//	fp = _fdopen(hConHandle, "w");
//
//	*stderr = *fp;
//
//	setvbuf(stderr, NULL, _IONBF, 0);
//
//	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
//	// point to console as well
//	ios::sync_with_stdio();
//}
//
//
//
//
////RedirectIOToConsole("again hello");
//
////thread CommandThread2(GotCommand, "Shakir");
//
//
//CHAR chBuf[BUFFER_SIZE];
//DWORD dwRead;
//HANDLE hStdin;
//BOOL fSuccess;
//
//hStdin = GetStdHandle(STD_INPUT_HANDLE);
//if (hStdin == INVALID_HANDLE_VALUE)
//ExitProcess(1);
//
//HANDLE out = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
//if (out == INVALID_HANDLE_VALUE)
//return 1;
//
//WORD color = atoi("0");
//SetConsoleTextAttribute(out, color);
//
//
//while (1) {
//	fSuccess = ReadFile(hStdin, chBuf, BUFFER_SIZE, &dwRead, NULL);
//	if (!fSuccess || !dwRead)
//		break;
//	WriteFile(out, chBuf, dwRead, &dwRead, 0);
//}
//
//VirtualConsole cvf = VirtualConsole("put", ULONG(0));