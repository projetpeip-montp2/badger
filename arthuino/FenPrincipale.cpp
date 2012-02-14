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
#include <thread>
#include <iostream>
#include <chrono>

namespace arthuino
{

    FenPrincipale::FenPrincipale
    (
        QWidget *parent
    ) : 
    QWidget(parent)
    {
        setupUi(this);

        m_serialStream = new serial::serialstream;
        m_threadReader = new ReadSerialThread(m_serialStream);

        connect(m_threadReader, SIGNAL(message(const QByteArray&)), this, SLOT(writeMessage(const QByteArray&)));

        connect(plusMoinsBouton, SIGNAL(toggled(bool)), reglagesAvances, SLOT(setVisible(bool)));
        reglagesAvances->setVisible(false);
        plusMoinsBouton->click();
    }


    FenPrincipale::~FenPrincipale()
    {
        deconnexion();

        delete m_threadReader;
        delete m_serialStream;
    }


    void FenPrincipale::connexion
    (
    )
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


        m_serialStream->open(port->text().toStdString());
        m_serialStream->setBaudRate(optionBaud);
        m_serialStream->setDataBits(serial::DataBits::HeightBits);
        m_serialStream->setStopBits(serial::StopBits::OneBit);
        m_serialStream->setParity(serial::Parity::None);
        m_serialStream->setFlowControl(serial::FlowControl::Off);
        m_serialStream->setTimeout(500);

        if (m_serialStream->isOpen())
        {
            m_threadReader->setTerminaisonByte('\n');
            m_threadReader->start();

            listeMessages->append(tr("<em>Connection <strong>ok</strong> !</em>"));
            boutonConnexion->setText(tr("Deconnexion"));

            message->setFocus();
        }
        else
        {
            listeMessages->append(tr("<em>Connection <strong>wrong</strong> !</em>"));
        }
    }


    void FenPrincipale::deconnexion
    (
    )
    {
        m_threadReader->terminate();
        m_threadReader->wait();
        m_serialStream->close();

        listeMessages->append(tr("<em>Deconnexion !</em>"));
        boutonConnexion->setText(tr("Connexion"));
    }


    void FenPrincipale::on_utiliserMFR120U_clicked()
    {
        port->setText("/dev/ttyUSB0");

        commencerSTXBox->setChecked(true);
        finirCRBox->setChecked(true);

        enleverSTXBox->setChecked(true);
        enleverCRBox->setChecked(true);
    }


    void FenPrincipale::on_boutonConnexion_clicked
    (
    )
    {
        boutonConnexion->setEnabled(false);

        if (!m_serialStream->isOpen())
            connexion();
        else
            deconnexion();

        boutonConnexion->setEnabled(true);
    }


    void FenPrincipale::on_boutonEnvoyer_clicked
    (
    )
    {
        if (m_serialStream->isOpen() && !message->text().isEmpty())
        {
            listeMessages->append("<- " + message->text());

            std::vector<serial::byte> maChaine;
        
            std::string tmp = message->text().toStdString();

            for(unsigned int i(0); i!=tmp.size(); ++i)
                maChaine.push_back(tmp[i]);

            if(commencerSTXBox->isChecked())
                maChaine.insert(maChaine.begin(), 2);

            if(finirCRBox->isChecked())
                maChaine.push_back(13);

            if(finirLFBox->isChecked())
                maChaine.push_back('\n');

            m_serialStream->writeBytes(maChaine);

            message->clear();
        }

        else if(!m_serialStream->isOpen())
            listeMessages->append(tr("<em><strong>Erreur</strong> : Aucune connection !</em>"));

        else if(message->text().isEmpty())
            listeMessages->append(tr("<em><strong>Erreur</strong> : Aucune commande !</em>"));

        else
            listeMessages->append(tr("<em><strong>Erreur</strong> !</em>"));

        message->setFocus();
    }


    void FenPrincipale::on_message_returnPressed
    (
    )
    {
        on_boutonEnvoyer_clicked();
    }


    void FenPrincipale::writeMessage
    (
        const QByteArray &messageRead
    )
    {
        std::vector<serial::byte> tmp(messageRead.data(), messageRead.data()+messageRead.count());

        if(enleverSTXBox->isChecked())
            tmp.erase( tmp.begin() );

        if(enleverCRBox->isChecked())
            tmp.erase( --tmp.end() );

        tmp.push_back('\0');

        listeMessages->append("<strong>>> </strong> \"" + QString(&tmp[0]) + '"');
    }


    void FenPrincipale::closeEvent
    (
        QCloseEvent *event
    )
    {
        deconnexion();

        event->accept();
    }


} // namespace arthuino
