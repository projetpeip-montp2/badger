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
        BAUD_50,
        BAUD_75,
        BAUD_110,
        BAUD_134,
        BAUD_150,
        BAUD_200,
        BAUD_300,
        BAUD_600,
        BAUD_1200,
        BAUD_1800,
        BAUD_2400,
        BAUD_4800,
        BAUD_9600,
        BAUD_19200,
        BAUD_38400,
        BAUD_57600,
        BAUD_115200,
        BAUD_230400,
        BAUD_460800
    };


    namespace priv
    {
        class serialstreamImpl;

    } // namespace priv


    class serialstream
    {
        public:
            serialstream();
            serialstream(std::string port, BaudRate baud);

            ~serialstream();

            void open(std::string port, BaudRate baud);
            void close();

            bool isOpen();

            std::vector<byte> readBytes(byte terminaisonByte);
            byte readByte();

            void writeBytes(const std::vector<byte> &b);
            void writeByte(byte b);

        private:
            void checkAvailablity() const;

            std::shared_ptr<priv::serialstreamImpl> m_serialImpl;
    };


} // namespace serial

#endif // SERIAL_SERIALSTREAM_HPP

