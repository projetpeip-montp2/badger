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

#ifndef SERIAL_SERIALSTREAMIMPLPOSIX_HPP
#define SERIAL_SERIALSTREAMIMPLPOSIX_HPP

#include <string>
#include <termios.h>

#include <map>

#include "../SerialStreamImpl.hpp"

namespace serial
{

namespace priv
{

    class serialstreamImplPosix : public serialstreamImpl
    {
    public:
        serialstreamImplPosix();
        serialstreamImplPosix(const std::string &port);

        virtual ~serialstreamImplPosix();

        virtual void open(const std::string &port);
        virtual void close();

        virtual bool isOpen();

        virtual void setBaudRate(BaudRate rate);

        virtual void setDataBits(DataBits data);

        virtual void setStopBits(StopBits stop);

        virtual void setParity(Parity parity);

        virtual void setFlowControl(FlowControl flow);

        virtual void setTimeout(int timeout);

        virtual int bytesAvailable() const;

        virtual void read(byte *buffer, unsigned int n);

        virtual void write(const byte *buffer, unsigned int n);

        virtual void flush();

    private:
        speed_t retrieveBaudRate(BaudRate baud) const;

        void updateConfig();

        static std::map<BaudRate, speed_t> m_baudRates;
        static bool m_staticBaudRatesInitialized;

        int m_outputFile;

        struct termios m_oldConfig;
        struct termios m_currentConfig;
    };


} // namespace priv

} // namespace serial

#endif // SERIAL_SERIALSTREAMIMPLPOSIX_HPP

