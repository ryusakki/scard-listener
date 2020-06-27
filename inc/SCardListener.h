#pragma once

#include <iostream>
#include <Windows.h>
#include <vector>
#include <cassert>
#include <functional>

#pragma comment(lib, "Winscard.lib")

class SCardListener {
	SCARDCONTEXT m_Context;
	SCARDHANDLE m_Card;
	SCARD_READERSTATE m_ReaderStates;
	DWORD m_Readers, m_ActiveProtocol;
	std::string m_strReaderList;
public:
	SCardListener();
	void operator()();
};