#pragma once
#include "stdafx.h"
#include <string>

class SerialComm {
public:
	static HANDLE OpenSerial(int com, int baudrate);
	static std::string  ReadSerial(HANDLE hSerial, unsigned byte_read);
};

// CloseHandle(HANDLE);
