//
// example.cpp
// SerialComm-WinCpp
//
// Created by iolate <iolate@me.com> on 2017. 11. 10..
//

#include "SerialComm.h"

// ========================================
// Simple open, read and close

int comPort = 1;
HANDLE serialHandle = NULL;
serialHandle = SerialComm::OpenSerial(comPort, CBR_9600); // CBR_57600, ...
if (serialHandle != NULL) {
	std::string response = SerialComm::ReadSerial(serialHandle, 16);
	CloseHandle(serialHandle);
}


// ========================================
// Using thread

DWORD WINAPI ProcessResponse(LPVOID lpParameter) {
	std::string response = *static_cast<std::string*>(lpParameter);
	//TODO: do something
	return 0;
}

HANDLE serialHandle = NULL;
DWORD WINAPI _lReadSerial(LPVOID lpParameter) {
	while (true) {
		std::string resp = SerialComm::ReadSerial(serialHandle, 16);
		if (resp.length() == 0) continue;
		CreateThread(NULL, 0, ProcessResponse, (LPVOID)&resp, 0, 0);
	}
	return 0;
}

int comPort = 1;
serialHandle = SerialComm::OpenSerial(comPort, CBR_9600); // CBR_57600, ...
if (serialHandle != NULL) {
	CreateThread(NULL, 0, serialHandle, NULL, 0, 0);
}

// CloseHandle(serialHandle);
