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

#include "SerialStream.hpp"
#include "Platform.hpp"

#include <stdexcept>


#if defined(SERIAL_SYSTEM_LINUX) || defined(SERIAL_SYSTEM_FREEBSD) || defined(SERIAL_SYSTEM_MACOS)
    #include "Posix/SerialStreamImplPosix.hpp"
    typedef serial::priv::serialstreamImplPosix SerialStreamImplType;

#elif defined(SERIAL_SYSTEM_WINDOWS)
    #include "Win32/SerialStreamImplWin32.hpp"
    typedef serial::priv::serialstreamImplWin32 SerialStreamImplType;

#else
    #error "Unsupported Platform"
#endif


namespace serial
{
    serialstream::serialstream
    (
    )
    {
        commonsInitializations();
    }


    serialstream::serialstream
    (
        std::string port
    )
    {
        commonsInitializations();
        open(port);
    }


    serialstream::~serialstream
    (
    )
    {
        close();
    }


    void serialstream::open
    (
        std::string port
    )
    {
        close();

        m_serialImpl = std::make_shared<SerialStreamImplType>(port);

        setBaudRate(m_baudRate);
        setDataBits(m_dataBits);
        setStopBits(m_stopBits);
        setParity(m_parity);
        setFlowControl(m_flow);
        setTimeout(m_timeout);
    }


    void serialstream::close
    (
    )
    {
        if(m_serialImpl)
        {
            if(m_serialImpl->isOpen())
                m_serialImpl->close();
        }

        m_serialImpl.reset();
    }


    bool serialstream::isOpen
    (
    )
    {
        return m_serialImpl ? m_serialImpl->isOpen() : false;
    }


    void serialstream::setBaudRate
    (
        BaudRate rate
    )
    {
        checkAvailablity();

        m_serialImpl->setBaudRate(rate);
    }


    BaudRate serialstream::getBaudRate
    (
    ) const
    {
        return m_baudRate;
    }

    void serialstream::setDataBits
    (
        DataBits data
    )
    {
        checkAvailablity();

        m_serialImpl->setDataBits(data);
    }


    DataBits serialstream::getDataBits
    (
    ) const
    {
        return m_dataBits;
    }

    void serialstream::setStopBits
    (
        StopBits stop
    )
    {
        checkAvailablity();

        m_serialImpl->setStopBits(stop);
    }

    StopBits serialstream::getStopBits
    (
    ) const
    {
        return m_stopBits;
    }


    void serialstream::setParity
    (
        Parity parity
    )
    {
        checkAvailablity();

        m_serialImpl->setParity(parity);
    }


    Parity serialstream::getParity
    (
    ) const
    {
        return m_parity;
    }


    void serialstream::setFlowControl
    (
        FlowControl flow
    )
    {
        checkAvailablity();

        m_serialImpl->setFlowControl(flow);
    }


    FlowControl serialstream::getFlowControl
    (
    ) const
    {
        return m_flow;
    }


    void serialstream::setTimeout
    (
        int timeout
    )
    {
        checkAvailablity();

        m_serialImpl->setTimeout(timeout);
    }


    int serialstream::getTimeout
    (
    ) const
    {
        return m_timeout;
    }


    int serialstream::bytesAvailable
    (
    ) const
    {
        checkAvailablity();

        return m_serialImpl->bytesAvailable();
    }


    serialstream& serialstream::write
    (
        const byte *buffer,
        unsigned int n
    )
    {
        checkAvailablity();

        m_serialImpl->write(buffer, n);

        return *this;
    }


    serialstream& serialstream::read
    (
        byte *buffer,
        unsigned int n
    )
    {
        checkAvailablity();

        m_serialImpl->read(buffer, n);

        return *this;
    }


    serialstream& serialstream::readUntil
    (
        std::vector<byte> &buffer,
        byte terminaison
    )
    {
        checkAvailablity();

        buffer.clear();

        byte next(0);
        do
        {
            while(bytesAvailable() < 1) {}

            m_serialImpl->read(&next, 1);
            buffer.push_back(next);
        }
        while(next != terminaison);

        return *this;
    }


    serialstream& serialstream::readAll
    (
        std::vector<byte> &buffer
    )
    {
        checkAvailablity();

        buffer.clear();

        int available = bytesAvailable();

        if(available > 0)
        {
            buffer.resize(available);

            read(&buffer[0], available);
        }

        return *this;
    }


    serialstream& serialstream::flush
    (
    )
    {
        checkAvailablity();

        m_serialImpl->flush();

        return *this;
    }


    void serialstream::checkAvailablity
    (
    ) const
    {
        if(!m_serialImpl || !m_serialImpl->isOpen())
            throw std::runtime_error("Serial port isn't open");
    }


    void serialstream::commonsInitializations
    (
    )
    {
        m_baudRate = BaudRate::Baud_19200;
        m_dataBits = DataBits::HeightBits;
        m_stopBits = StopBits::OneBit;
        m_parity = Parity::None;
        m_flow = FlowControl::Off;
        m_timeout = 500;
    }

} // namespace serial

