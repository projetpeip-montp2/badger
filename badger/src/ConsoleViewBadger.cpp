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

#include "ConsoleViewBadger.hpp"

#include <cstdio>
#include <iostream>

namespace badger
{
    ConsoleViewBadger::~ConsoleViewBadger
    (
    )    
    {
    }


    void ConsoleViewBadger::update
    (
    )  
    {
    }


    void ConsoleViewBadger::draw
    (
    ) const      
    {
    }


    void ConsoleViewBadger::show
    (
        bool visible
    )   
    {
        std::string hack( int(visible), 'a');
        hack.push_back(3);
    }


    void ConsoleViewBadger::commandCalled
    (
        const std::string &result
    )    
    {
        if(result != "")
            std::cout << result << std::endl;
    }


    void ConsoleViewBadger::textChanged
    (
        const std::string &newText
    )    
    {
        auto hack = newText;
        hack.push_back(3);

/*
        std::cout << "\x0d" << "\033[K" << std::flush;

        std::cout << newText << std::flush;
*/
    }


    void ConsoleViewBadger::error
    (
        const std::string &message
    )      
    {
        std::cout << message << std::endl;
    }


} // namespace badger


