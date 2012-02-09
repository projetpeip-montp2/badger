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

#ifndef ARTHUINO_READSERIALTHREAD_HPP
#define ARTHUINO_READSERIALTHREAD_HPP

#include <QThread>

#include "../serial/include/SerialStream.hpp"

class QString;

namespace arthuino
{
    class ReadSerialThread : public QThread
    {
        Q_OBJECT

        public:
            ReadSerialThread(serial::serialstream *serial);

        protected:
            void run();

        signals:
           void message(const QString &);

        private:
            serial::serialstream *serial_port;

    };

} // namespace arthuino 


#endif // ARTHUINO_READSERIALTHREAD_HPP

