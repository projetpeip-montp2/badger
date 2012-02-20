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

namespace serial
{

namespace priv
{

    serialstreamImplWin32::serialstreamImplWin32
    (
    )
    {

    }


    serialstreamImplWin32::serialstreamImplWin32
    (
        std::string port
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

    }


    void serialstreamImplWin32::close
    (
    )
    {

    }


    bool serialstreamImplWin32::isOpen
    (
    )
    {

    }








    int serialstreamImplWin32::bytesAvailable
    (
    ) const
    {

    }


    void serialstreamImplWin32::read
    (
        byte *buffer, 
        unsigned int n
    )
    {

    }


    void serialstreamImplWin32::write
    (
        const byte *buffer, 
        unsigned int n
    )
    {

    }


    void serialstreamImplWin32::flush
    (
    )
    {

    }








    void serialstreamImplWin32::setBaudRate
    (
        BaudRate rate
    )
    {

    }


    void serialstreamImplWin32::setDataBits
    (
        DataBits data
    )
    {

    }


    void serialstreamImplWin32::setStopBits
    (
        StopBits stop
    )
    {

    }


    void serialstreamImplWin32::setParity
    (
        Parity parity
    )
    {

    }


    void serialstreamImplWin32::setFlowControl
    (
        FlowControl flow
    )
    {

    }


    void serialstreamImplWin32::setTimeout
    (
        int timeout
    )
    {

    }



} // namespace priv

} // namespace serial

