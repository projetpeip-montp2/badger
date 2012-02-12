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

#ifndef ARTHUINO_FENPRINCIPALE_HPP
#define ARTHUINO_FENPRINCIPALE_HPP

#include <QWidget>

#include "ui_Arthuino.h"

#include "../serial/include/SerialStream.hpp"

namespace arthuino
{
    class ReadSerialThread;

    class FenPrincipale : public QWidget, private Ui::Arthuino
    {
    Q_OBJECT

    public:
        FenPrincipale(QWidget *parent = 0);

        ~FenPrincipale();

    private:
        void connexion();
        void deconnexion();

    private slots:
        void on_utiliserMFR120U_clicked();

        void on_boutonConnexion_clicked();
        void on_boutonEnvoyer_clicked();
        void on_message_returnPressed();

        void writeMessage(const QByteArray &messageRead);

        void closeEvent(QCloseEvent *event);

    private:
        serial::serialstream *m_serialStream;
        ReadSerialThread *m_threadReader;

    };

} // namespace arthuino

#endif // ARTHUINO_FENPRINCIPALE_HPP


