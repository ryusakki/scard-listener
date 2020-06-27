#include "SCardListener.h"
#include <thread>

SCardListener::SCardListener() : m_Readers(0), m_ActiveProtocol(0)
{
	long lResult = SCardEstablishContext(SCARD_SCOPE_USER, nullptr, nullptr, &m_Context);
	if (lResult != SCARD_S_SUCCESS)
	{
		std::printf("Failed to establish card context. Err=(%ld)", lResult);
		std::exit(lResult);
	}

	lResult = SCardListReaders(m_Context, nullptr, nullptr, &m_Readers);
	if (lResult != SCARD_S_SUCCESS)
	{
		std::printf("Failed to list card readers. Err=(%ld)", lResult);
		std::exit(lResult);
	}
	
	m_strReaderList.resize(m_Readers);
	lResult = SCardListReaders(m_Context, nullptr, const_cast<LPSTR>(m_strReaderList.data()), &m_Readers);
	if (lResult != SCARD_S_SUCCESS)
	{
		std::printf("Failed to list card readers. Err=(%ld)", lResult);
		std::exit(lResult);
	}

	lResult = SCardConnect(m_Context, m_strReaderList.data(), SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &m_Card, &m_ActiveProtocol);
	if (lResult != SCARD_S_SUCCESS)
	{
		std::printf("SCard connection failure. Err=(%ld)", lResult);
		std::exit(lResult);
	}

	m_ReaderStates.dwEventState = 0;
	m_ReaderStates.dwCurrentState = SCARD_STATE_UNAWARE;
	m_ReaderStates.szReader = m_strReaderList.data();
}

void SCardListener::operator()()
{
	while (true)
	{
		auto lResult = SCardGetStatusChangeA(m_Context, INFINITE, &m_ReaderStates, 1);
		if (lResult == SCARD_S_SUCCESS)
		{
			(m_ReaderStates.dwEventState & SCARD_STATE_PRESENT) ? std::printf("Token connected\n") : std::printf("Token disconnected\n");
		}
		Sleep(150);
	}
}

int main() 
{
	std::thread{ SCardListener{} }.join();
}