////////////////////////////////////////////////////////////
// Copyright (c) 2011 - 2012 Hiairrassary Victor
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


#ifndef PLASTIC_CONSOLE_HPP
#define PLASTIC_CONSOLE_HPP

#include "ConsoleView.hpp"
#include "ConsoleFunctionBase.hpp"
#include "ConsoleFunction.hpp"

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <functional>


namespace plt
{

	/////////////////////////////////////////////////////////////////
	/// 
	///
	/////////////////////////////////////////////////////////////////
    class Console
    {

    public:
        Console();

        template<typename Res, typename... Args>
        void registerCommand(const std::string &name, const std::function<Res(Args...)> &function);

        void sendChar(char Character);

        void setView(const std::shared_ptr<ConsoleView> &view);

        void enable(bool state);

        bool isEnable() const;

    private:
        std::string getCommands() const;

        void processCurrent();


		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
        std::map<std::string, std::shared_ptr<ConsoleFunctionBase> > m_commands;

        std::shared_ptr<ConsoleView> m_view;

        std::string m_current; ///< Current command

        bool m_enabled;
    };


} // namespace plt


#include "Console.inl"


#endif // PLASTIC_CONSOLE_HPP




////////////////////////////////////////////////////////////
/// \class plt::Console
///
/// \todo Avertir sur les précautions (fonctions membres et durée de vie de l'objet appelant)
/// \todo Changer le message des exceptions renvoyer 
/// \todo Mettre en anglais les noms
/// \todo Mettre en inline
/// \todo Renvoyer les exceptions catchés dans processCurrent() dans une vue
/// \todo Pouvoir écrire le prototype d'une fonction enregistrée dans la console de façon non-intrusive
/// \todo Prendre en compte la nullité du pointeur m_view
/// \todo Constructeur par défaut (avec delegating constructor)
/// \todo Proposer une view par défaut
/// \todo Rajouter les fonctions draw() et update()
/// \todo Utilise les rvalue et std::forward() dans l'implementation de ConsoleFunction avec les variadics
///
/// \todo Pouvoir connaitre prototype d'une fonction avec ?nomCommande (donc empeche les noms de commande similaire)
/// \todo Avant d'envoyer une commande, dégager les espaces résiduels et autres caractères à la fin de la ligne de commande
///
////////////////////////////////////////////////////////////
