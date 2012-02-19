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

#ifndef BADGER_BADGER_HPP
#define BADGER_BADGER_HPP

#include <map>
#include <string>

#include "Console.hpp"

#include "Date.hpp"
#include "Time.hpp"

#include "SerialStream.hpp"

namespace badger
{
    class Badger
    {
    public:
        Badger();

        ~Badger();

        void run();

    private:
        struct Record
        {
            Date date;
            Time time;
            std::string data;
        };

    private:
        template<typename Res, typename... Args>
        void addCommand(const std::string &name, const std::function<Res(Args...)> &function, bool mustBeInRoot);

        void open(const std::string &port);

        void close();

        std::string login(const std::string &password);

        std::string logout();

        std::string next();

        std::string rollback();

        std::string erase();

        std::string count();

        std::string get(const Date &date, const Time &begin, const Time &end);

        void quit();

        void needLogin(const std::string &command);

        std::string getReturnCommand();

        void sendCommandOnSerial(const std::vector<serial::byte> &command);

        Record parseRecord(const std::string &str) const;

    private:
        bool m_continuer;
        bool m_logged;

        plt::Console m_console;

        std::map<std::string, bool> m_access;

        serial::serialstream m_serial;
    };

} // namespace badger


#include "Badger.inl"


#endif // BADGER_BADGER_HPP

