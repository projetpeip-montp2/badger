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

#include "SerialStreamImplLinux.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace serial
{

namespace priv
{
	std::map<BaudRate, speed_t> serialstreamImplLinux::m_baudRates = 
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










    serialstreamImplLinux::serialstreamImplLinux
    (
    ) :
    m_outputFile(-1)
    {

    }


    serialstreamImplLinux::serialstreamImplLinux
    (
        std::string port, 
        BaudRate baud
    ) :
    m_outputFile(-1)
    {
        open(port, baud);
    }


    serialstreamImplLinux::~serialstreamImplLinux
    (
    )
    {
        close();
    }


    void serialstreamImplLinux::open
    (
        std::string port, 
        BaudRate baud
    )
    {
        struct termios opt; // Déclaration de la struct termios pour les options.

        m_outputFile = ::open( port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );

        tcgetattr(m_outputFile, &m_oldConfig); // On récupère les paramètres actuels.
        tcgetattr(m_outputFile, &opt); // On récupère les paramètres actuels.
        
        //cfmakeraw(&opt); // FAKE
        cfsetispeed(&opt, retrieveBaudRate(baud)); // On règle la vitesse en entrée et en sortie.
        cfsetospeed(&opt, retrieveBaudRate(baud));


        opt.c_cflag|=CREAD|CLOCAL;
        opt.c_lflag&=(~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
        //opt.c_iflag&=(~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
        opt.c_oflag&=(~OPOST);
        opt.c_cc[VMIN]= 0;


#ifdef _POSIX_VDISABLE  // Is a disable character available on this system?
        // Some systems allow for per-device disable-characters, so get the
        //  proper value for the configured device
        const long vdisable = fpathconf(m_outputFile, _PC_VDISABLE);
        opt.c_cc[VINTR] = vdisable;
        opt.c_cc[VQUIT] = vdisable;
        opt.c_cc[VSTART] = vdisable;
        opt.c_cc[VSTOP] = vdisable;
        opt.c_cc[VSUSP] = vdisable;
#endif //_POSIX_VDISABLE


        opt.c_cflag&=(~CSIZE);
        opt.c_cflag|=CS8;
        tcsetattr(m_outputFile, TCSAFLUSH, &opt);


        opt.c_cflag&=(~PARENB);
        tcsetattr(m_outputFile, TCSAFLUSH, &opt);

        opt.c_cflag&=(~CSTOPB);
        tcsetattr(m_outputFile, TCSAFLUSH, &opt);


        opt.c_cflag&=(~CRTSCTS);
        opt.c_iflag&=(~(IXON|IXOFF|IXANY));
        tcsetattr(m_outputFile, TCSAFLUSH, &opt);


        struct timeval Posix_Copy_Timeout;
        Posix_Copy_Timeout.tv_sec = 500 / 1000;
        Posix_Copy_Timeout.tv_usec = 500 % 1000;
            fcntl(m_outputFile, F_SETFL, O_SYNC);
        tcgetattr(m_outputFile, &opt);

        opt.c_cc[VTIME] = 500/100;
        tcsetattr(m_outputFile, TCSAFLUSH, & opt);

        tcsetattr(m_outputFile, TCSAFLUSH, &opt);


        opt.c_cflag &= ~CSIZE; // 8 bits de données.
        opt.c_cflag |= CS8;

        opt.c_cflag &= ~PARENB; // Pas de bit de parité.

        opt.c_cflag &= ~CSTOPB; // Un bit de stop.

        opt.c_cc[VMIN] = 1;
        opt.c_cc[VTIME] = 0;


        // On envoie les nouveaux paramètres.
        if (tcsetattr(m_outputFile, TCSANOW, &opt) != 0)
            std::cerr << "Problème : -> Configuration <-" << std::endl;
    }


    void serialstreamImplLinux::close
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


    bool serialstreamImplLinux::isOpen
    (
    )
    {
        return (m_outputFile != -1);
    }


    std::vector<byte> serialstreamImplLinux::readBytes
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


    byte serialstreamImplLinux::readByte
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


    void serialstreamImplLinux::writeBytes
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


    void serialstreamImplLinux::writeByte
    (
        byte b
    )
    {
        while(::write(m_outputFile, &b, 1) < 0)
        {
            // Rien à faire
        }
    }











    speed_t serialstreamImplLinux::retrieveBaudRate
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

