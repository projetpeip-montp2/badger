////////////////////////////////////////////////////////////
// Copyright (c) 2011 - Hiairrassary Victor
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


#include "ConsoleViewStdOut.hpp"

#include <cstdio>
#include <iostream>

namespace plt
{
    ConsoleViewStdOut::~ConsoleViewStdOut
    (
    )    
    {
    }


    void ConsoleViewStdOut::update
    (
    )  
    {
    }


    void ConsoleViewStdOut::draw
    (
    ) const      
    {
    }


    void ConsoleViewStdOut::show
    (
        bool visible
    )   
    {
        if(visible)
            std::cout << "Console enable" << std::endl;

        else
            std::cout << "Console disable" << std::endl;
    }


    void ConsoleViewStdOut::commandCalled
    (
        const std::string &result
    )    
    {
        if(result != "")
            std::cout << std::endl << result << std::endl;
    }


    void ConsoleViewStdOut::textChanged
    (
        const std::string &newText
    )    
    {
        std::cout << "\x0d" << "\033[K" << std::flush;

        std::cout << newText << std::flush;
    }


    void ConsoleViewStdOut::error
    (
        const std::string &message
    )      
    {
        std::cout << std::endl << message << std::endl;
    }


} // namespace plt


