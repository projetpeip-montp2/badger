////////////////////////////////////////////////////////////
// Copyright (C) 2009-2012 HIAIRRASSARY
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, write to
// the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301 USA
////////////////////////////////////////////////////////////

#include "ReadSerialThread.hpp"

#include "FenPrincipale.hpp"

#include <QString>

#include <string>

namespace arthuino
{
    ReadSerialThread::ReadSerialThread
    (
        serial::serialstream *serial
    ) : 
    m_serialStream(serial),
    m_terminaisonByte('\r')
    {

    }



    void ReadSerialThread::setTerminaisonByte
    (
        serial::byte terminaisonByte
    )
    {
        m_terminaisonByte = terminaisonByte;
    }


    void ReadSerialThread::run
    (
    )
    {
        // Dans ce thread, on lis en permanence les données envoyées par 
        // le module. Dès quelles se finissent par '\r', on envoie tout 
        // sur l'IHM.
        // On passe par un QByteArray à cause du système de signaux de Qt
        std::vector<serial::byte> dataRead;
        forever
        {
            dataRead = m_serialStream->readBytes(m_terminaisonByte);

            emit message( QByteArray(&dataRead[0], dataRead.size()) );
        }
    }

} // namespace arthuino
