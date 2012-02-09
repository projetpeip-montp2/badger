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
    m_serialStream(serial)
    {

    }


    void ReadSerialThread::run
    (
    )
    {
        std::string maChaine;
        forever
        {
            maChaine = m_serialStream->read();
            emit message( QString::fromStdString(maChaine) );
        }
    }

} // namespace arthuino