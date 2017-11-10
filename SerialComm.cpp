#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commdlg.h>

#include "SerialComm.h"

HANDLE SerialComm::_OpenSerial(int com, int baudrate) {
	HANDLE hSerial;
	COMMCONFIG dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts;// = { 0 };
	
	std::string port = "\\\\.\\COM" + std::to_string((_ULonglong)com); //"\\\\.\\"
	wchar_t* wport = charToWchar(port.c_str());
	hSerial = CreateFile(wport,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);
	delete[] wport;

	if (hSerial == INVALID_HANDLE_VALUE) {
		CloseHandle(hSerial);
		hSerial = NULL;
		return NULL;
	}

	//DCB dcbSerialParams = { 0 };
	if (!GetCommState(hSerial, &dcbSerialParams.dcb)) {
		CloseHandle(hSerial);
		hSerial = NULL;
		return NULL;
	}

	dcbSerialParams.dcb.DCBlength = sizeof(dcbSerialParams.dcb);

	dcbSerialParams.dcb.BaudRate = baudrate;
	dcbSerialParams.dcb.ByteSize = 8;
	dcbSerialParams.dcb.StopBits = ONESTOPBIT;
	dcbSerialParams.dcb.Parity = NOPARITY;

	if (!SetCommState(hSerial, &dcbSerialParams.dcb)) {
		CloseHandle(hSerial);
		hSerial = NULL;
		return NULL;
	}
	
	GetCommTimeouts(hSerial, &timeouts);
	
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		CloseHandle(hSerial);
		hSerial = NULL;
	}
	
	return hSerial;
}

std::string  SerialComm::_ReadSerial(HANDLE hSerial, unsigned byte_read) {
	if (hSerial == NULL) {
		return "";
	}
	PurgeComm(hSerial, PURGE_RXABORT | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR);

	while (true) {
		//char* szBuff = (char *)malloc((byte_read + 1) * sizeof(char));
		char szBuff[64];
		DWORD dwBytesRead = 0;
		if (!ReadFile(hSerial, &szBuff, byte_read, &dwBytesRead, NULL)) {
			return "";
		}

		if (dwBytesRead >= byte_read) {
			std::string resp(szBuff); //, dwBytesRead
			return resp;
		}
	}
	return "";
}
