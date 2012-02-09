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

#include "FenPrincipale.hpp"

#include "ReadSerialThread.hpp"

#include <QtGui>

#include <string>

namespace arthuino
{

    FenPrincipale::FenPrincipale(QWidget *parent) : QWidget(parent)
    {
        setupUi(this);

        serial_port = new serial::serialstream;
        thread_read = new ReadSerialThread(serial_port);

        connect(thread_read, SIGNAL(message(QString)), this, SLOT(writeMessage(QString)));
    }


    void FenPrincipale::connexion()
    {
        listeMessages->append(tr("<em>Tentative de connexion en cours...</em>"));


        serial::BaudRate optionBaud(serial::BaudRate::BAUD_9600);

        if (baud->currentText() == "300 baud")
            optionBaud = serial::BaudRate::BAUD_300;
        else if (baud->currentText() == "1200 baud")
            optionBaud = serial::BaudRate::BAUD_1200;
        else if (baud->currentText() == "2400 baud")
            optionBaud = serial::BaudRate::BAUD_2400;
        else if (baud->currentText() == "4800 baud")
            optionBaud = serial::BaudRate::BAUD_4800;
        else if (baud->currentText() == "9600 baud")
            optionBaud = serial::BaudRate::BAUD_9600;
        else if (baud->currentText() == "19200 baud")
            optionBaud = serial::BaudRate::BAUD_19200;
        else if (baud->currentText() == "38400 baud")
            optionBaud = serial::BaudRate::BAUD_38400;
        else if (baud->currentText() == "57600 baud")
            optionBaud = serial::BaudRate::BAUD_57600;
        else if (baud->currentText() == "115200 baud")
            optionBaud = serial::BaudRate::BAUD_115200;
        else
            listeMessages->append(tr("<em><strong>Erreur</strong> : Probleme Baud !</em>"));


        serial_port->open(port->text().toStdString(), optionBaud);

        if (serial_port->isOpen())
        {
            thread_read->start();

            listeMessages->append(tr("<em>Connection <strong>ok</strong> !</em>"));
            boutonConnexion->setText(tr("Deconnexion"));

            message->setFocus();
        }
        else
        {
            listeMessages->append(tr("<em>Connection <strong>wrong</strong> !</em>"));
        }
    }


    void FenPrincipale::deconnexion()
    {
        thread_read->terminate();
        serial_port->close();

        listeMessages->append(tr("<em>Deconnexion !</em>"));
        boutonConnexion->setText(tr("Connexion"));
    }


    void FenPrincipale::on_boutonConnexion_clicked()
    {
        boutonConnexion->setEnabled(false);

        if (!serial_port->isOpen())
            connexion();
        else
            deconnexion();

        boutonConnexion->setEnabled(true);
    }


    void FenPrincipale::on_boutonEnvoyer_clicked()
    {
        if (serial_port->isOpen() && !message->text().isEmpty())
        {
            std::string maChaine(message->text().toStdString());

            if(nouvelleLigne->checkState() == Qt::Checked)
                maChaine += '\n';

            serial_port->write(maChaine);

            listeMessages->append("<- " + message->text());

            message->clear();
            message->setFocus();
        }
        else if (!serial_port->isOpen())
        {
            listeMessages->append(tr("<em><strong>Erreur</strong> : Aucune connection !</em>"));
            message->setFocus();
        }
        else if (message->text().isEmpty())
        {
            listeMessages->append(tr("<em><strong>Erreur</strong> : Aucune commande !</em>"));
            message->setFocus();
        }
        else
        {
            listeMessages->append(tr("<em><strong>Erreur</strong> !</em>"));
            message->setFocus();
        }
    }


    void FenPrincipale::on_message_returnPressed()
    {
        on_boutonEnvoyer_clicked();
    }


    void FenPrincipale::on_port_returnPressed()
    {
        on_boutonConnexion_clicked();
    }


    void FenPrincipale::writeMessage(const QString &message)
    {
        listeMessages->append("<strong>>> </strong> " + message);
    }


    void FenPrincipale::closeEvent(QCloseEvent *event)
    {
        thread_read->terminate();
        thread_read->wait();
        serial_port->close();
        event->accept();
    }


} // namespace arthuino
