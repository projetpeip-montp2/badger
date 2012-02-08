////////////////////////////////////////////////////////////
// Copyright (C) 2009-2010 HIAIRRASSARY
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

#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QtGui>
#include "ui_Arthuino.h"

#include <iostream>
#include <string>

#include "../serial/include/SerialStream.hpp"

class ReadSerialThread;

class FenPrincipale : public QWidget, private Ui::Arthuino
{
    Q_OBJECT

    public:
        FenPrincipale(QWidget *parent = 0);
        void connexion();
        void deconnexion();

    public slots:
        void on_boutonConnexion_clicked();
        void on_boutonEnvoyer_clicked();
        void on_message_returnPressed();
        void on_port_returnPressed();

        void writeMessage(const QString &message);

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        serial::serialstream *serial_port;
        ReadSerialThread *thread_read;

};


#endif // FENPRINCIPALE_H


