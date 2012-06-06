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


#ifndef PLASTIC_CONSOLEVIEW_HPP
#define PLASTIC_CONSOLEVIEW_HPP

#include <string>

namespace plt
{
	/////////////////////////////////////////////////////////////////
	/// 
	/////////////////////////////////////////////////////////////////
    class ConsoleView
    {
    public:
        // Destructeur
        virtual ~ConsoleView();

        // Fonction appelée lors de la mise à jour de la console
        virtual void update() = 0;

        // Fonction appelée lors de l'affichage de la console
        virtual void draw() const = 0;

        // Fonction appelée lors de l'activation / désactivation de la console
        virtual void show(bool visible) = 0;

        // Fonction appelée après l'appel à une commande
        virtual void commandCalled(const std::string &result) = 0;

        // Fonction appelée à chaque changement de la ligne courante
        virtual void textChanged(const std::string &newText) = 0;

        // Fonction appelée en cas d'erreur
        virtual void error(const std::string &message) = 0;
    };


} // namespace plt


#endif // PLASTIC_CONSOLEVIEW_HPP




////////////////////////////////////////////////////////////
/// \class plt::ConsoleView
///
///
////////////////////////////////////////////////////////////
