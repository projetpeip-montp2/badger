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

#include "Database.hpp"

#include <sstream>
#include <stdexcept>

namespace badger
{
    Database::Database
    (
        const std::string &host,
        const std::string &user, 
        const std::string &passwd, 
        const std::string &database, 
        unsigned int port
    ) :
    m_hasRequestPrepared(false)
    {
        connect(host, user, passwd, database, port);
    }


    Database::~Database
    (
    )
    {
        close();
    }


    void Database::connect
    (
        const std::string &host,
        const std::string &user, 
        const std::string &passwd, 
        const std::string &database, 
        unsigned int port
    )
    {
        // TODO: Il faut premunir la fonction close de s'il n'y pas eu de connection 
        //close();

        mysql_init(&m_link);
        mysql_options(&m_link, MYSQL_READ_DEFAULT_GROUP, "mysqldatabase");

        if(!mysql_real_connect(&m_link, host.c_str(), user.c_str(), passwd.c_str(), database.c_str(), port, NULL, 0))
            throw std::runtime_error("Impossible de se connecter à la base de donnée.");
    }


    void Database::close
    (
    )
    {
        deletePreparedRequest();

        mysql_close(&m_link);
    }


    std::string Database::lastError
    (
    )
    {
        return mysql_error(&m_link);
    }


    std::string Database::lastErrorSTMT
    (
    )
    {
        return mysql_stmt_error(m_stmt);
    }


    void Database::query
    (
        const std::string &query
    )
    {
        if( mysql_query(&m_link, query.c_str()) )
            throw std::runtime_error("Impossible de d'éxécuter la requête: " + lastError());
    }


    void Database::prepare
    (
        const std::string &query
    )
    {
        if(m_hasRequestPrepared)
            deletePreparedRequest();

        m_stmt = mysql_stmt_init(&m_link);

        if(!m_stmt)
            throw std::runtime_error("Out of memory");

        if(mysql_stmt_prepare(m_stmt, query.c_str(), query.size()))
            throw std::runtime_error("Impossible de preparer la requête: " + lastError());

        m_hasRequestPrepared = true;
    }


    void Database::execute
    (
    )
    {
        if(!m_hasRequestPrepared)
            throw std::runtime_error("No prepared request");

        unsigned int paramCount = mysql_stmt_param_count(m_stmt);
        unsigned int size = m_binds.size();

        std::ostringstream oss;
        oss << paramCount;

        if(size != paramCount)
            throw std::runtime_error("Le nombre de paramètres ne concorde pas: " + oss.str() + " attendue(s)");  

        if(size > 0)
        {
            if(mysql_stmt_bind_param(m_stmt, &m_binds[0]))
                throw std::runtime_error("Erreur lors du bind: " + lastErrorSTMT());  
        }

        if(mysql_stmt_execute(m_stmt))
            throw std::runtime_error("Impossible d'executer la requête: " + lastErrorSTMT());

        m_binds.clear();
        m_ts.clear();
    }


    void Database::deletePreparedRequest
    (
    )
    {
        if(m_hasRequestPrepared)
        {
            if(mysql_stmt_close(m_stmt))
                throw std::runtime_error("Impossible de supprimer la requête: " + lastErrorSTMT());

            m_hasRequestPrepared = false;
        }
    }


    void Database::addParameterLong
    (
        long *data
    )
    {
        MYSQL_BIND bind;
        m_binds.push_back(bind);

        m_binds.back().buffer_type = MYSQL_TYPE_LONG;
        m_binds.back().buffer = reinterpret_cast<char*>(data);
        m_binds.back().is_null = 0;
        m_binds.back().length = 0;
    }


    void Database::addParameterString
    (
        std::string *data
    )
    {
        MYSQL_BIND bind;
        m_binds.push_back(bind);

        m_binds.back().buffer_type = MYSQL_TYPE_STRING;
        m_binds.back().buffer = reinterpret_cast<char*>( &((*data)[0]) );
        m_binds.back().buffer_length = data->size();
        m_binds.back().is_null = 0;
        m_binds.back().length = 0;
    }

} // namespace badger

