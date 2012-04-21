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

#include "SerialStreamImplPosix.hpp"

#include <stdexcept>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>


namespace serial
{

namespace priv
{
	std::map<BaudRate, speed_t> serialstreamImplPosix::m_baudRates;

    bool serialstreamImplPosix::m_staticBaudRatesInitialized = false;







    serialstreamImplPosix::serialstreamImplPosix
    (
    ) :
    m_outputFile(-1)
    {

    }


    serialstreamImplPosix::serialstreamImplPosix
    (
        const std::string &port
    ) :
    m_outputFile(-1)
    {
        open(port);
    }


    serialstreamImplPosix::~serialstreamImplPosix
    (
    )
    {
        close();
    }


    void serialstreamImplPosix::open
    (
        const std::string &port
    )
    {
        m_outputFile = ::open( port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

        if(m_outputFile == -1)
            throw std::runtime_error("Unable to open the file \"" + port + '"');

        // On sauvegarde les paramètres actuels.
        tcgetattr(m_outputFile, &m_oldConfig); 

        tcgetattr(m_outputFile, &m_currentConfig);

        // On désactive un possible mode canonique
        m_currentConfig.c_lflag = 0;

        // On désactive un possible mappage (CR -> NL) ou (NL -> CR)
        m_currentConfig.c_iflag = 0;
        
        // Compatibilité Mac OS X
        m_currentConfig.c_cflag|=CREAD|CLOCAL;       

        updateConfig();

        
/*
#ifdef _POSIX_VDISABLE  // Is a disable character available on this system?
        // Some systems allow for per-device disable-characters, so get the
        //  proper value for the configured device
        const long vdisable = fpathconf(m_outputFile, _PC_VDISABLE);
        m_currentConfig.c_cc[VINTR] = vdisable;
        m_currentConfig.c_cc[VQUIT] = vdisable;
        m_currentConfig.c_cc[VSTART] = vdisable;
        m_currentConfig.c_cc[VSTOP] = vdisable;
        m_currentConfig.c_cc[VSUSP] = vdisable;
#endif //_POSIX_VDISABLE
*/
    }


    void serialstreamImplPosix::close
    (
    )
    {
        if(m_outputFile != -1)
        {
            // On restore l'ancienne configuration
            tcsetattr(m_outputFile, TCSAFLUSH | TCSANOW, &m_oldConfig);

            ::close(m_outputFile);
            m_outputFile = -1;
        }
    }


    bool serialstreamImplPosix::isOpen
    (
    ) const
    {
        return (m_outputFile != -1);
    }








    int serialstreamImplPosix::bytesAvailable
    (
    ) const
    {
        int bytesQueued;
        if (ioctl(m_outputFile, FIONREAD, &bytesQueued) == -1)
            return int(-1);

        return bytesQueued;
    }


    void serialstreamImplPosix::read
    (
        byte *buffer, 
        unsigned int n
    )
    {
        auto bytesRead = ::read(m_outputFile, buffer, n);

        if(bytesRead < 0)
            throw std::runtime_error("Unable to read on serial port");
    }


    void serialstreamImplPosix::write
    (
        const byte *buffer, 
        unsigned int n
    )
    {
        auto bytesWrote = ::write(m_outputFile, buffer, n);

        if(bytesWrote < 0)
            throw std::runtime_error("Unable to write on serial port");
    }


    void serialstreamImplPosix::flush
    (
    )
    {
        tcflush(m_outputFile, TCIOFLUSH);
    }








    void serialstreamImplPosix::setBaudRate
    (
        BaudRate rate
    )
    {
        cfsetispeed(&m_currentConfig, retrieveBaudRate(rate));
        cfsetospeed(&m_currentConfig, retrieveBaudRate(rate));

        updateConfig();
    }


    void serialstreamImplPosix::setDataBits
    (
        DataBits data
    )
    {
        switch(data)
        {
            case DataBits::HeightBits:
                m_currentConfig.c_cflag &= (~CSIZE);
                m_currentConfig.c_cflag |= CS8;
                break;
        }

        updateConfig();
    }


    void serialstreamImplPosix::setStopBits
    (
        StopBits stop
    )
    {
        switch(stop)
        {
            case StopBits::OneBit:
                m_currentConfig.c_cflag &= (~CSTOPB);
                break;
        }

        updateConfig();
    }


    void serialstreamImplPosix::setParity
    (
        Parity parity
    )
    {
        switch(parity)
        {
            case Parity::None:
                m_currentConfig.c_cflag &= (~PARENB);
                break;
        }

        updateConfig();
    }


    void serialstreamImplPosix::setFlowControl
    (
        FlowControl flow
    )
    {
        switch(flow)
        {
            case FlowControl::Off:
                m_currentConfig.c_cflag&=(~CRTSCTS);
                m_currentConfig.c_iflag&=(~(IXON|IXOFF|IXANY));
                break;
        }


        updateConfig();
    }


    void serialstreamImplPosix::setTimeout
    (
        const std::chrono::milliseconds &timeout
    )
    {
        fcntl(m_outputFile, F_SETFL, O_SYNC);
        m_currentConfig.c_cc[VTIME] = timeout.count()/100;

        updateConfig();
    }


    void serialstreamImplPosix::updateConfig
    (
    )
    {
        if(tcsetattr(m_outputFile, TCSAFLUSH | TCSANOW, &m_currentConfig) != 0)
            throw std::runtime_error("Unable set the new configuration");
    }











    speed_t serialstreamImplPosix::retrieveBaudRate
    (
        BaudRate baud
    ) const
    {
        if(!m_staticBaudRatesInitialized)
        {
            m_baudRates.insert(std::make_pair(BaudRate::Baud_9600,   B9600));
            m_baudRates.insert(std::make_pair(BaudRate::Baud_19200,  B19200));
            m_baudRates.insert(std::make_pair(BaudRate::Baud_38400,  B38400));

            m_staticBaudRatesInitialized = true;
        }

        auto it = m_baudRates.find(baud);

        if(it == m_baudRates.end())
            throw std::runtime_error("Unable to retrieve the baud rate");

        return (*it).second;        
    }

} // namespace priv

} // namespace serial

