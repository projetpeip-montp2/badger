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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Console.hpp"

#include <exception>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <functional>


namespace plt
{
    Console::Console
    (
    ) :
    m_enabled(false)
    {
        std::function<std::string()> function = std::bind(&Console::getCommands, this);

        registerCommand("?", function);
    }


    void Console::sendChar(char Character)
    {
        // Si la console n'est pas active on ne traite pas le caractère
        if (!m_enabled)
            return;

        // Traitement du caractère
        switch (Character)
        {
            // Saut de ligne : on traite la commande et on efface la ligne
            case '\n' :
            case '\r' :
                if (!m_current.empty())
                {
                    processCurrent();
                    m_current.clear();
                }
                break;

            // Backspace : on efface le dernier caractère
            case '\b' :
                if (!m_current.empty())
                    m_current.erase(m_current.size() - 1);
                break;

            // Tout le reste : on ajoute le caractère à la ligne courante
            default :
                m_current += Character;
                break;
        }

        m_view->textChanged(m_current);
    }


    void Console::setView
    (
        const std::shared_ptr<ConsoleView> &view
    )
    {
        if(!view)
            throw std::runtime_error("The view is an invalid pointer");

        m_view = view;
    }


    std::string Console::getCommands() const
    {
        std::string list;
        for (auto it = m_commands.begin(); it != m_commands.end(); ++it)
            list += it->first + " ";

        return list;
    }


    void Console::processCurrent
    (
    )
    {
        std::string command;

        std::istringstream iss(m_current);

        if( !(iss >> command) )
            throw std::runtime_error("Error when parsing the command name");



        auto it = m_commands.find(command);

        if (it != m_commands.end())
        {
            std::string params;
            std::getline(iss, params);

            try
            {
                m_view->commandCalled(
                                        (it->second)->execute(params)
                                     );
            }

            catch (const std::exception &e)
            {
                m_view->error( e.what() );
            }

        }


        else
            m_view->error("Commande \"" + command + "\" inconnue");
    }


    void Console::enable
    (
        bool state
    )
    {
        m_enabled = state;

        m_view->show(state);
    }


    bool Console::isEnable
    (
    ) const
    {
        return m_enabled;
    }

} // namespace plt
