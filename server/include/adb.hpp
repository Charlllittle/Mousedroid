#ifndef ADB_HPP
#define ADB_HPP

#ifdef _WIN32

#include <windows.h>
#include <string>

#include "Logger.hpp"

namespace adb
{
	/*
	* Get the local directory of the program
	*/
	std::string dir()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);

		std::string path(buffer);
		return path.substr(0, path.find_last_of("\\/"));
	}

	/*
	* Run adb.exe with the given arguments.
	* Uses CreateProcess so it works correctly in a GUI (windowless) app.
	* Waits up to timeoutMs for the process to finish.
	* Returns the process exit code, or -1 on launch failure.
	*/
	int run(const std::string& args, DWORD timeoutMs = INFINITE)
	{
		std::string exe = dir() + "\\adb.exe";
		std::string cmdline = "\"" + exe + "\" " + args;

		STARTUPINFOA si = {};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		PROCESS_INFORMATION pi = {};

		if (!CreateProcessA(
			exe.c_str(),
			const_cast<char*>(cmdline.c_str()),
			nullptr, nullptr, FALSE,
			CREATE_NO_WINDOW,
			nullptr, nullptr,
			&si, &pi))
		{
			return -1;
		}

		WaitForSingleObject(pi.hProcess, timeoutMs);

		DWORD exitCode = 1;
		GetExitCodeProcess(pi.hProcess, &exitCode);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		return (int)exitCode;
	}

	/*
	* Reverse the given tcp port.
	* adb reverse tcp:<port> tcp:<port>
	*/
	bool start(int port)
	{
		// Kill any stale adb server left over from a previous session
		run("kill-server");

		std::string args = "reverse tcp:" + std::to_string(port) + " tcp:" + std::to_string(port);

		if (run(args) == 0)
		{
			LOG("[ADB:", port, "] Started");
			return true;
		}

		LOG("[ADB:", port, "] Failed to start");
		return false;
	}

	/*
	* Shuts down the adb server, which removes all reverse tunnels.
	* Times out after 3 seconds so the app doesn't hang on close.
	*/
	bool kill(int port)
	{
		if (run("kill-server", 3000) == 0)
		{
			LOG("[ADB:", port, "] Stopped");
			return true;
		}

		LOG("[ADB:", port, "] Failed to stop");
		return false;
	}
}

#endif // _WIN32
#endif // ADB_HPP
