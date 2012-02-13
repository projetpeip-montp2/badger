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

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace serial
{

namespace priv
{
	std::map<BaudRate, speed_t> serialstreamImplPosix::m_baudRates = 
    {
        std::make_pair(BaudRate::BAUD_50,     B50),
        std::make_pair(BaudRate::BAUD_75,     B75),
        std::make_pair(BaudRate::BAUD_110,    B110),
        std::make_pair(BaudRate::BAUD_134,    B134),
        std::make_pair(BaudRate::BAUD_150,    B150),
        std::make_pair(BaudRate::BAUD_200,    B200),
        std::make_pair(BaudRate::BAUD_300,    B300),
        std::make_pair(BaudRate::BAUD_600,    B600),
        std::make_pair(BaudRate::BAUD_1200,   B1200),
        std::make_pair(BaudRate::BAUD_1800,   B1800),
        std::make_pair(BaudRate::BAUD_2400,   B2400),
        std::make_pair(BaudRate::BAUD_4800,   B4800),
        std::make_pair(BaudRate::BAUD_9600,   B9600),
        std::make_pair(BaudRate::BAUD_19200,  B19200),
        std::make_pair(BaudRate::BAUD_38400,  B38400),
        std::make_pair(BaudRate::BAUD_57600,  B57600),
        std::make_pair(BaudRate::BAUD_115200, B115200),
        std::make_pair(BaudRate::BAUD_230400, B230400),
        std::make_pair(BaudRate::BAUD_460800, B460800)
    };










    serialstreamImplPosix::serialstreamImplPosix
    (
    ) :
    m_outputFile(-1)
    {

    }


    serialstreamImplPosix::serialstreamImplPosix
    (
        std::string port
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
        std::string port
    )
    {
        m_outputFile = ::open( port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

        // On sauvegarde les paramètres actuels.
        tcgetattr(m_outputFile, &m_oldConfig); 

        tcgetattr(m_outputFile, &m_currentConfig);
        
/*
        cfsetispeed(&m_currentConfig, B19200);
        cfsetospeed(&m_currentConfig, B19200);


        m_currentConfig.c_cflag|=CREAD|CLOCAL;
        m_currentConfig.c_lflag&=(~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
        //m_currentConfig.c_iflag&=(~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
        m_currentConfig.c_oflag&=(~OPOST);
        m_currentConfig.c_cc[VMIN]= 0;


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


        m_currentConfig.c_cflag&=(~CSIZE);
        m_currentConfig.c_cflag|=CS8;
        tcsetattr(m_outputFile, TCSAFLUSH, &m_currentConfig);


        m_currentConfig.c_cflag&=(~PARENB);
        tcsetattr(m_outputFile, TCSAFLUSH, &m_currentConfig);

        m_currentConfig.c_cflag&=(~CSTOPB);
        tcsetattr(m_outputFile, TCSAFLUSH, &m_currentConfig);


        m_currentConfig.c_cflag&=(~CRTSCTS);
        m_currentConfig.c_iflag&=(~(IXON|IXOFF|IXANY));
        tcsetattr(m_outputFile, TCSAFLUSH, &m_currentConfig);


        struct timeval Posix_Copy_Timeout;
        Posix_Copy_Timeout.tv_sec = 500 / 1000;
        Posix_Copy_Timeout.tv_usec = 500 % 1000;
            fcntl(m_outputFile, F_SETFL, O_SYNC);
        tcgetattr(m_outputFile, &m_currentConfig);

        m_currentConfig.c_cc[VTIME] = 500/100;

        tcsetattr(m_outputFile, TCSAFLUSH, &m_currentConfig);


        m_currentConfig.c_cflag &= ~CSIZE; // 8 bits de données.
        m_currentConfig.c_cflag |= CS8;

        m_currentConfig.c_cflag &= ~PARENB; // Pas de bit de parité.

        m_currentConfig.c_cflag &= ~CSTOPB; // Un bit de stop.

        m_currentConfig.c_cc[VMIN] = 1;
        m_currentConfig.c_cc[VTIME] = 0;
*/

/*
        // On envoie les nouveaux paramètres.
        if (tcsetattr(m_outputFile, TCSANOW, &m_currentConfig) != 0)
            std::cerr << "Problème : -> Configuration <-" << std::endl;
*/
    }


    void serialstreamImplPosix::close
    (
    )
    {
        if(m_outputFile != -1)
        {
            tcsetattr(m_outputFile, TCSAFLUSH | TCSANOW, &m_oldConfig);

            ::close(m_outputFile);
            m_outputFile = -1;
        }
    }


    bool serialstreamImplPosix::isOpen
    (
    )
    {
        return (m_outputFile != -1);
    }


    std::vector<byte> serialstreamImplPosix::readBytes
    (
        byte terminaisonByte
    )
    {
        // Problème : Que se passe-t-il si à la première lecture échoue et 
        // que terminaisonByte == '0' (valeur par défaut) ?????

        std::vector<byte> result;
        byte next_byte(0);

        do
        {
            if(::read(m_outputFile, &next_byte, 1) > 0)
                result.push_back(next_byte);
        }
        while(next_byte != terminaisonByte); // '\n' est le byte terminateur.

        return result;
    }


    byte serialstreamImplPosix::readByte
    (
    )
    {
        byte b;
        while(::read( m_outputFile, &b, 1 ) < 0)
        {
            // Rien à faire
        }

        return b;
    }


    void serialstreamImplPosix::writeBytes
    (
        const std::vector<byte> &b
    )
    {
        ssize_t num_of_bytes_written(0);
        do
        {
            num_of_bytes_written = ::write(m_outputFile, &b[0]+num_of_bytes_written,  static_cast<int>(b.size())-num_of_bytes_written);
        }
        while( ( num_of_bytes_written < static_cast<int>(b.size()) ) );
    }


    void serialstreamImplPosix::writeByte
    (
        byte b
    )
    {
        while(::write(m_outputFile, &b, 1) < 0)
        {
            // Rien à faire
        }
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
        int timeout
    )
    {
/*
        struct timeval Posix_Copy_Timeout;
        Posix_Copy_Timeout.tv_sec = timeout / 1000;

        Posix_Copy_Timeout.tv_usec = timeout % 1000;
            fcntl(m_outputFile, F_SETFL, O_SYNC);
        tcgetattr(m_outputFile, &m_currentConfig);


        m_currentConfig.c_cc[VTIME] = timeout/100;
*/

        fcntl(m_outputFile, F_SETFL, O_SYNC);
        m_currentConfig.c_cc[VTIME] = timeout/100;

        updateConfig();
    }


    void serialstreamImplPosix::updateConfig
    (
    )
    {
        tcsetattr(m_outputFile, TCSAFLUSH, &m_currentConfig);
    }











    speed_t serialstreamImplPosix::retrieveBaudRate
    (
        BaudRate baud
    ) const
    {
        auto it = m_baudRates.find(baud);

        if(it == m_baudRates.end())
            throw std::runtime_error("");

        return (*it).second;        
    }

} // namespace priv

} // namespace serial

