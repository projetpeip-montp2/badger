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

#ifndef SERIAL_SERIALSTREAM_HPP
#define SERIAL_SERIALSTREAM_HPP

#include <memory>
#include <string>
#include <vector>

namespace serial
{
    typedef char byte;

    enum class BaudRate
    {
        BAUD_9600,
        BAUD_19200
    };


    enum class DataBits
    {
        HeightBits
    };


    enum class StopBits
    {
        OneBit
    };


    enum class Parity
    {
        None
    };


    enum class FlowControl
    {
        Off
    };


    namespace priv
    {
        class serialstreamImpl;

    } // namespace priv


    class serialstream
    {
        public:
            serialstream();
            serialstream(std::string port);

            ~serialstream();

            void open(std::string port);
            void close();

            bool isOpen();

            void setBaudRate(BaudRate rate);
            BaudRate getBaudRate() const;

            void setDataBits(DataBits data);
            DataBits getDataBits() const;

            void setStopBits(StopBits stop);
            StopBits getStopBits() const;

            void setParity(Parity parity);
            Parity getParity() const;

            void setFlowControl(FlowControl flow);
            FlowControl getFlowControl() const;

            void setTimeout(int timeout);
            int getTimeout() const;

            int bytesAvailable() const;

            serialstream& read(byte *buffer, unsigned int n);

            serialstream& write(const byte *buffer, unsigned int n);

            serialstream& readUntil(std::vector<byte> &buffer, byte terminaison);

            serialstream& flush();

        private:
            BaudRate    m_baudRate;
            DataBits    m_dataBits;
            StopBits    m_stopBits;
            Parity      m_parity;
            FlowControl m_flow;

            int m_timeout;

            void checkAvailablity() const;

            void commonsInitializations();

            std::shared_ptr<priv::serialstreamImpl> m_serialImpl;
    };


} // namespace serial

#endif // SERIAL_SERIALSTREAM_HPP

