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

#include "Badger.hpp"

#include "ConsoleViewBadger.hpp"

#include <iostream>
#include <sstream>

namespace badger
{

    namespace
    {
        void sendCommand(plt::Console &c, const std::string &command)
        {
            for(auto it = command.begin(); it!=command.end(); ++it)
                c.sendChar(*it);

            c.sendChar('\n');
        }
    }





    Badger::Badger
    (
    ) :
    m_continuer(true),
    m_logged(false)
    {
        // Important : A console need a view, otherwise seg fault :)
        m_console.setView( std::make_shared<ConsoleViewBadger>() );

        m_console.enable(true);

        std::function<void(const std::string&)> openFunction = std::bind(&Badger::open, this, std::placeholders::_1);
        addCommand("open", openFunction, false);

        std::function<void()> closeFunction = std::bind(&Badger::logout, this);
        addCommand("close", closeFunction, false);

        std::function<void(const std::string&)> loginFunction = std::bind(&Badger::login, this, std::placeholders::_1);
        addCommand("login", loginFunction, false);

        std::function<void()> logoutFunction = std::bind(&Badger::logout, this);
        addCommand("logout", logoutFunction, false);

        std::function<void()> nextFunction = std::bind(&Badger::next, this);
        addCommand("next", nextFunction, true);

        std::function<void()> rollbackFunction = std::bind(&Badger::rollback, this);
        addCommand("rollback", rollbackFunction, true);

        std::function<void()> eraseFunction = std::bind(&Badger::erase, this);
        addCommand("erase", eraseFunction, true);

        std::function<void()> countFunction = std::bind(&Badger::count, this);
        addCommand("count", countFunction, false);

        std::function<void()> quitFunction = std::bind(&Badger::quit, this);
        addCommand("quit", quitFunction, false);
    }



    Badger::~Badger
    (
    )
    {
        if(m_logged)
            sendCommand(m_console, "logout");

        m_serial.close();
    }


    void Badger::open
    (
        const std::string &port
    )
    {
        std::cout << "open : " << port << std::endl;
    }


    void Badger::close
    (
    )
    {
        std::cout << "close" << std::endl;
    }


    bool Badger::login
    (
        const std::string &password
    )
    {
        std::cout << "login : " << password << std::endl;

        return false;
    }


    void Badger::logout
    (
    )
    {
        std::cout << "logout" << std::endl;
    }


    void Badger::next
    (
    )
    {
        std::cout << "next" << std::endl;
    }


    void Badger::rollback
    (
    )
    {
        std::cout << "rollback" << std::endl;
    }


    void Badger::erase
    (
    )
    {
        std::cout << "erase" << std::endl;
    }


    void Badger::count
    (
    )
    {
        std::cout << "count" << std::endl;
    }


    void Badger::quit
    (
    )
    {
        m_continuer = false;
    }


    void Badger::needLogin
    (
        const std::string &command
    )
    {
        bool continueToLogin = true;

        std::string nameFunction;
        std::istringstream iss(command);

        iss >> nameFunction;

        auto it = m_access.find(nameFunction);

        // Don't change order of this if :)
        if(it != m_access.end() && m_access[nameFunction] && !m_logged)
        {
            std::cout << "DEBUG: Cette fonction nécessite d'être root: " << nameFunction << std::endl;

            while(continueToLogin)
            {
                std::string password;

                std::cout << "Password : ";
                std::getline(std::cin, password);

                if(login(password))
                {
                    sendCommand(m_console, command);
                    continueToLogin = false;
                }

                else
                {
                    std::string answer;

                    std::cout << "Bad password! Retry (yes)?: ";
                    std::getline(std::cin, answer);

                    if(answer != "yes")
                        continueToLogin = false;
                }
            }
        }

        else
            sendCommand(m_console, command);
    }


    void Badger::run
    (
    )
    {
        while(m_continuer)
        {
            std::string command;

            std::cout << "> ";
            std::getline(std::cin, command);

            needLogin(command);
        }
    }

} // namespace badger

