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

        std::function<std::string(const std::string&)> loginFunction = std::bind(&Badger::login, this, std::placeholders::_1);
        addCommand("login", loginFunction, false);

        std::function<std::string()> logoutFunction = std::bind(&Badger::logout, this);
        addCommand("logout", logoutFunction, false);

        std::function<std::string()> nextFunction = std::bind(&Badger::next, this);
        addCommand("next", nextFunction, true);

        std::function<std::string()> rollbackFunction = std::bind(&Badger::rollback, this);
        addCommand("rollback", rollbackFunction, true);

        std::function<std::string()> eraseFunction = std::bind(&Badger::erase, this);
        addCommand("erase", eraseFunction, true);

        std::function<std::string()> countFunction = std::bind(&Badger::count, this);
        addCommand("count", countFunction, false);

        std::function<void()> quitFunction = std::bind(&Badger::quit, this);
        addCommand("quit", quitFunction, false);


        std::cout << "==================================" << std::endl;
        std::cout << "    Welcome to badger program    " << std::endl;
        std::cout << std::endl;
        std::cout << "To get available options, type '?'" << std::endl;
        std::cout << "==================================" << std::endl;
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
        m_serial.open(port);
    }


    void Badger::close
    (
    )
    {
        m_serial.close();
    }


    std::string Badger::login
    (
        const std::string &password
    )
    {
        std::vector<serial::byte> command;
        command.push_back('L');

        for(unsigned int i(0); i<password.size(); ++i)
            command.push_back(password[i]);

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();

        m_logged = (result == "A") ? true : false;

        return (result == "A") ? "You are logged now" : "Error during login";
    }


    std::string Badger::logout
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('O');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();

        if(result == "A")
            m_logged = false;

        return (result == "A") ? "" : "Error during logout";
    }


    std::string Badger::next
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('G');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();

        return (result == "E") ? "Database is empty or finish (try rollback maybe)" : "Next result :" + result.erase(0,1);
    }


    std::string Badger::rollback
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('R');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();

        return (result == "A") ? "" : "Rollback impossible";
    }


    std::string Badger::erase
    (
    )
    {
        std::string answer;
        std::string result;

        std::cout << "Do you really want to erase all database (yes)? ";
        std::getline(std::cin, answer);

        if(answer == "yes")
        {
            std::cout << "Information: erase duration < 60 seconds";

            std::vector<serial::byte> command;
            command.push_back('E');

            sendCommandOnSerial(command);

            result = getReturnCommand();
        }

        return (result == "A") ? "" : "Erase impossible";

    }


    std::string Badger::count
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('N');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();
        result.erase(result.begin());

        return "There are " + result + " records";
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
            std::cout << "Information: You must be logged to use the function: " << nameFunction << std::endl;

            while(continueToLogin)
            {
                std::string password;

                std::cout << "Password : ";
                std::getline(std::cin, password);

                login(password);

                if(m_logged)
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


    std::string Badger::getReturnCommand
    (
    )
    {
        std::vector<serial::byte> result = m_serial.readBytes('\n');

        //if(result.front() != serial::byte(2) || result.back() != serial::byte(13))
            //throw std::runtime_error("The return command don't begin by STX or don't end by CR");

        result.erase( result.begin() );
        result.erase( --result.end() );
        result.push_back('\0');

        return std::string(&result[0]);
    }


    void Badger::sendCommandOnSerial
    (
        const std::vector<serial::byte> &command
    )
    {
        auto tmp = command;

        // Add STX to begin
        tmp.insert(tmp.begin(), 2);

        // Add CR to end
        tmp.push_back(13);

        m_serial.writeBytes(tmp);
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

