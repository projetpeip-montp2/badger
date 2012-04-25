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

#ifndef BADGER_DATABASE_HPP
#define BADGER_DATABASE_HPP

#include <string>
#include <vector>
#include <iostream>

#include <mysql/mysql.h>

#include "Date.hpp"
#include "Time.hpp"

namespace badger
{
    class Database
    {
    public:
        Database(const std::string &host, const std::string &user, const std::string &passwd, const std::string &database, unsigned int port);

        ~Database();

        void connect(const std::string &host, const std::string &user, const std::string &passwd, const std::string &database, unsigned int port);

        void close();

        void query(const std::string &query);
   
        void prepare(const std::string &query);

        void execute(bool deleteRequest);

        void addParameterLong(long *data);

        void addParameterString(std::string *data);

    private:
        std::string lastError();

        std::string lastErrorSTMT();

        void deleteSTMT();

        bool hasRequestWaiting;

        MYSQL m_link;
        MYSQL_STMT *m_stmt;

        std::vector<MYSQL_BIND> m_binds;
        std::vector<MYSQL_TIME> m_ts;
    };

} // namespace badger

#endif // BADGER_DATABASE_HPP

