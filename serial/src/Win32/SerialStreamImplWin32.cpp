////////////////////////////////////////////////////////////
// Copyright (c) 2012 Polytech Montpellier
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////

#include "SerialStreamImplWin32.hpp"

#include <stdexcept>

namespace serial
{

namespace priv
{
	std::map<BaudRate, DWORD> serialstreamImplWin32::m_baudRates;

    bool serialstreamImplWin32::m_staticBaudRatesInitialized = false;










    serialstreamImplWin32::serialstreamImplWin32
    (
    ) : m_handle(INVALID_HANDLE_VALUE)
    {

    }


    serialstreamImplWin32::serialstreamImplWin32
    (
        const std::string &port
    )
    {
        open(port);
    }


    serialstreamImplWin32::~serialstreamImplWin32
    (
    )
    {
        close();
    }


    void serialstreamImplWin32::open
    (
        std::string port
    )
    {
		m_handle = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (m_handle == INVALID_HANDLE_VALUE)
			throw std::runtime_error("Unable to open the serial port : " + port);

		unsigned long size = sizeof(COMMCONFIG);

        GetCommConfig(m_handle, &m_commConfig, &size);
        GetCommState(m_handle, &(m_commConfig.dcb));

		m_commConfig.dcb.fBinary = TRUE;
		m_commConfig.dcb.fInX = FALSE;
		m_commConfig.dcb.fOutX = FALSE;
		m_commConfig.dcb.fAbortOnError = FALSE;
		m_commConfig.dcb.fNull = FALSE;
    }


    void serialstreamImplWin32::close
    (
    )
    {
        if(m_handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
        }
    }


    bool serialstreamImplWin32::isOpen
    (
    )
    {
        return m_handle != INVALID_HANDLE_VALUE;
    }








    int serialstreamImplWin32::bytesAvailable
    (
    ) const
    {
        DWORD errors;
        COMSTAT status;
        if (ClearCommError(m_handle, &errors, &status))
            return status.cbInQue;

        return int(-1);
    }


    void serialstreamImplWin32::read
    (
        byte *buffer,
        unsigned int n
    )
    {
		DWORD bytesReaded;

		if(!ReadFile(m_handle, buffer, n, &bytesReaded, NULL))
			throw std::runtime_error("Unable to read on serial port");
    }


    void serialstreamImplWin32::write
    (
        const byte *buffer,
        unsigned int n
    )
    {
		DWORD bytesSend;

		if(!WriteFile(m_handle, buffer, n, &bytesSend, NULL))
			throw std::runtime_error("Unable to write on serial port");
    }


    void serialstreamImplWin32::flush
    (
    )
    {
        FlushFileBuffers(m_handle);
    }








    void serialstreamImplWin32::setBaudRate
    (
        BaudRate rate
    )
    {
        m_commConfig.dcb.BaudRate = retrieveBaudRate(rate);
    }


    void serialstreamImplWin32::setDataBits
    (
        DataBits data
    )
    {
        switch(data)
		{
            case DataBits::HeightBits:
                m_commConfig.dcb.ByteSize = 8;
                break;
		}

		updateConfig();
    }


    void serialstreamImplWin32::setStopBits
    (
        StopBits stop
    )
    {
        switch(stop)
		{
            case StopBits::OneBit:
                m_commConfig.dcb.StopBits = ONESTOPBIT;
                break;
		}

		updateConfig();
    }


    void serialstreamImplWin32::setParity
    (
        Parity parity
    )
    {
        switch(parity)
		{
            case Parity::None:
				m_commConfig.dcb.Parity = 0;
				m_commConfig.dcb.fParity = FALSE;
                break;
		}

		updateConfig();
    }


    void serialstreamImplWin32::setFlowControl
    (
        FlowControl flow
    )
    {
        switch(flow)
		{
            case FlowControl::Off:
                m_commConfig.dcb.fOutxCtsFlow = FALSE;
                m_commConfig.dcb.fRtsControl = RTS_CONTROL_DISABLE;
                m_commConfig.dcb.fInX = FALSE;
                m_commConfig.dcb.fOutX = FALSE;
                break;
		}

		updateConfig();
    }


    void serialstreamImplWin32::setTimeout
    (
        int timeout
    )
    {
        m_commTimeouts.ReadIntervalTimeout = timeout;
        m_commTimeouts.ReadTotalTimeoutConstant = timeout;

		m_commTimeouts.ReadTotalTimeoutMultiplier = 0;
		m_commTimeouts.WriteTotalTimeoutMultiplier = timeout;
		m_commTimeouts.WriteTotalTimeoutConstant = 0;

		SetCommTimeouts(m_handle, &m_commTimeouts);
    }


    void serialstreamImplWin32::updateConfig
    (
    )
    {
        if(!SetCommConfig(m_handle, &m_commConfig, sizeof(COMMCONFIG)) )
            throw std::runtime_error("Unable to update config");
    }








	DWORD serialstreamImplWin32::retrieveBaudRate
    (
        BaudRate baud
    ) const
    {
        if(!m_staticBaudRatesInitialized)
        {
            m_baudRates.insert(std::make_pair(BaudRate::Baud_9600,   CBR_9600));
            m_baudRates.insert(std::make_pair(BaudRate::Baud_19200,  CBR_19200));

            m_staticBaudRatesInitialized = true;
        }

        auto it = m_baudRates.find(baud);

        if(it == m_baudRates.end())
            throw std::runtime_error("Unable to retrieve the baud rate");

        return (*it).second;
    }

} // namespace priv

} // namespace serial

