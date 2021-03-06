////////////////////////////////////////////////////////////
// Copyright (c) 2012 Polytech Montpellier
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

#include "Badger.hpp"

#include "ConsoleViewBadger.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

namespace badger
{

    namespace
    {
        void sendCommand(plt::Console &c, const std::string &command)
        {
            for(auto it = command.begin(); it!=command.end(); ++it)
                c.sendChar(*it);

            c.sendChar('\n');
        }
    }


    Badger::Badger
    (
    ) :
    m_continuer(true)
    {
        // Important, a console need a view, otherwise seg fault :)
        m_console.setView( std::make_shared<ConsoleViewBadger>() );

        m_console.enable(true);

        std::function<void(const std::string&)> openFunction = std::bind(&Badger::open, this, std::placeholders::_1);
        addCommand("open", openFunction, false);

        std::function<void()> closeFunction = std::bind(&Badger::close, this);
        addCommand("close", closeFunction, true);

        std::function<std::string()> minMaxDateFunction = std::bind(&Badger::minMaxDate, this);
        addCommand("minMaxDate", minMaxDateFunction, true);

        std::function<std::string()> nextFunction = std::bind(&Badger::next, this);
        addCommand("next", nextFunction, true);

        std::function<std::string()> rollbackFunction = std::bind(&Badger::rollback, this);
        addCommand("rollback", rollbackFunction, true);

        std::function<std::string()> eraseFunction = std::bind(&Badger::erase, this);
        addCommand("erase", eraseFunction, true);

        std::function<std::string()> countFunction = std::bind(&Badger::count, this);
        addCommand("count", countFunction, true);

        std::function<std::string()> getDateTimeFunction = std::bind(&Badger::getDateTime, this);
        addCommand("getDateTime", getDateTimeFunction, true);

        std::function<std::string(const Date&, const Time&)> setDateTimeFunction = std::bind(&Badger::setDateTime, this, std::placeholders::_1, std::placeholders::_2);
        addCommand("setDateTime", setDateTimeFunction, true);

        std::function<std::string(const Date&, const Time&, const Time&)> displayFunction = std::bind(&Badger::display, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        addCommand("display", displayFunction, true);

        std::function<std::string(const Date&, const Time&, const Time&)> csvFunction = std::bind(&Badger::csv, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        addCommand("csv", csvFunction, true);

        std::function<void()> quitFunction = std::bind(&Badger::quit, this);
        addCommand("quit", quitFunction, false);
        addCommand("exit", quitFunction, false);


        std::cout << "==================================" << std::endl;
        std::cout << "    Welcome to badger program    " << std::endl;
        std::cout << std::endl;
        std::cout << "To get available options, type '?'" << std::endl;
        std::cout << "By Badger Team" << std::endl;
        std::cout << "==================================" << std::endl;
    }



    Badger::~Badger
    (
    )
    {
        close();
    }


    void Badger::open
    (
        const std::string &port
    )
    {
        m_serial.open(port);
        m_serial.setBaudRate(serial::BaudRate::Baud_38400);
    }


    void Badger::close
    (
    )
    {
        if(m_serial.isOpen())
            m_serial.close();
    }


    std::string Badger::next
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('G');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();

        std::string begin(result, 0, 1);

        return (begin != "A") ? "Database is empty (E) or finish (F), result: " + begin : result.erase(0,1);
    }


    std::string Badger::rollback
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('R');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();

        return (result == "A") ? "" : "Rollback impossible";
    }


    std::string Badger::erase
    (
    )
    {
        std::string answer;
        std::string result;

        std::cout << "Do you really want to erase all database (yes)? ";
        std::getline(std::cin, answer);

        if(answer == "yes")
        {
            std::cout << "Wait..." << std::endl;

            std::vector<serial::byte> command;
            command.push_back('E');

            sendCommandOnSerial(command);

            result = getReturnCommand();
        }

        return (result == "A") ? "" : "Erase impossible";

    }


    std::string Badger::count
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('N');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();
        result.erase(result.begin());

        return result;
    }


    std::string Badger::getDateTime
    (
    )
    {
        std::vector<serial::byte> command;
        command.push_back('T');

        sendCommandOnSerial(command);
        std::string result = getReturnCommand();
        result.erase(result.begin());

        Date date;
        Time time;

        std::istringstream iss( std::string(result, 0, 2) + '-' + std::string(result, 2, 2) + '-' + std::string(result, 4, 4) );
        iss >> date;

        iss.clear();
        iss.str( std::string(result, 8, 2) + ':' + std::string(result, 10, 2) + ':' + std::string(result, 12, 2) );
        iss >> time;


        std::ostringstream oss;
        oss << "Date: " << date << ",\tTime:" << time;
        return oss.str();
    }


    std::string Badger::setDateTime
    (
        const Date &date, 
        const Time &time
    )
    {
        std::ostringstream oss;
        oss << std::setfill('0');
        oss << std::setw(2) << date.getDay();
        oss << std::setw(2) << date.getMonth();
        oss << std::setw(4) << date.getYear();

        oss << std::setw(2) << time.getHour();
        oss << std::setw(2) << time.getMinute();
        oss << std::setw(2) << time.getSecond();
        oss << 0; // Event


        std::vector<serial::byte> command;
        command.push_back('S');
       
        auto datetime = oss.str();
        for(auto c : datetime)
            command.push_back(c);


        sendCommandOnSerial(command);
        std::string result = getReturnCommand();
        result.erase(result.begin());

        oss.clear();
        oss.str("");
        oss << "Date: " << date << ",\tTime:" << time;
        return oss.str();
    }


    std::string Badger::minMaxDate
    (
    )
    {
        auto records = getAll();

        std::string result = "No records!";

        if(!records.empty())
        {
            Date min = records[0].date;
            Date max = records[0].date;

            for(unsigned int i(1); i<records.size(); ++i)
            {
                if(records[i].date < min)
                    min = records[i].date;

                if(max < records[i].date)
                    max = records[i].date;
            }

            std::ostringstream oss;
            oss << "min: " << min << '\n' << "max: " << max;

            result = oss.str();
        }


        return result;
    }


    std::string Badger::display
    (
        const Date &date, 
        const Time &begin, 
        const Time &end
    )
    {
        auto records = get(date, begin, end);

        for(unsigned int i(0); i<records.size(); ++i)
            std::cout << records[i].date << ' ' << records[i].time << ' ' << records[i].data << std::endl;

        return "";
    }


    std::string Badger::csv
    (
        const Date &date, 
        const Time &begin, 
        const Time &end
    )
    {
        auto records = get(date, begin, end);

        std::ofstream out("badging.csv", std::ios::out | std::ios::trunc);

        if(!out)
            throw std::runtime_error("Cannot open file for csv export");

        for(unsigned int i(0); i<records.size(); ++i)
        {
            if( !(out << '"' << records[i].date << "\",\"" << records[i].time << "\",\"" << records[i].data << '"' << std::endl) )
                throw std::runtime_error("Error during csv export");
        }

        return "";
    }


    void Badger::quit
    (
    )
    {
        m_continuer = false;
    }


    void Badger::tryToSendCommand
    (
        const std::string &command
    )
    {
        std::string nameFunction;
        std::istringstream iss(command);

        iss >> nameFunction;

        bool canBeSend = true;

        auto it = m_access.find(nameFunction);

        if(it != m_access.end())
        {
            if(m_access[nameFunction] && !m_serial.isOpen())
            {
                std::cout << "Serial port isn't open" << std::endl;
                canBeSend = false;
            }
        }

        if(canBeSend)
            sendCommand(m_console, command);
    }


    std::string Badger::getReturnCommand
    (
    )
    {
        std::vector<serial::byte> result;

        // After readUntil, flush the stream to avoid some read problems
        m_serial.readUntil(result, '\r').flush();

        if(result.front() != serial::byte(2) || result.back() != serial::byte(13))
            throw std::runtime_error("The return command don't begin by STX or don't end by CR");

        result.erase( result.begin() );
        result.erase( --result.end() );
        result.push_back('\0');

        return std::string(&result[0]);
    }


    void Badger::sendCommandOnSerial
    (
        const std::vector<serial::byte> &command
    )
    {
        std::vector<serial::byte> tmp = command;

        // Add STX to begin
        tmp.insert(tmp.begin(), 2);

        // Add CR to end
        tmp.push_back(13);

        m_serial.write(&tmp[0], tmp.size());
    }


    Badger::Record Badger::parseRecord
    (
        const std::string &str
    ) const
    {
        std::string day(str, 0, 2);
        std::string month(str, 2, 2);
        std::string year(str, 4, 4);

        std::string hour(str, 8, 2);
        std::string minute(str, 10, 2);
        std::string seconde(str, 12, 2);

        Record rcd;

        rcd.date = Date(std::atoi(&day[0]), std::atoi(&month[0]), std::atoi(&year[0]));
        rcd.time = Time(std::atoi(&hour[0]), std::atoi(&minute[0]), std::atoi(&seconde[0]));
        rcd.data = std::string(str, 17);

        return rcd;
    }


    std::vector<Badger::Record> Badger::getAll
    (
    )
    {
        unsigned int numberOfRecords;

        std::string countStr = count();
        std::istringstream iss(countStr);

        if(!(iss >> numberOfRecords))
            throw std::runtime_error("Unable to parse number of records");
    
        rollback();

        std::vector<Record> records;

        for(unsigned int i(0); i<numberOfRecords; ++i)
        {
            Record rcd = parseRecord(next());

            records.push_back(rcd);
        }

        return records;
    }


    std::vector<Badger::Record> Badger::get
    (
        const Date &date, 
        const Time &begin, 
        const Time &end
    )
    {
        auto records = getAll();

        std::vector<Record> tmp;

        for(unsigned int i(0); i<records.size(); ++i)
        {
            if(records[i].date == date && records[i].time >= begin && records[i].time <= end)
                tmp.push_back(records[i]);
        }

        return tmp;
    }



    void Badger::run
    (
    )
    {
        while(m_continuer)
        {
            std::string command;

            std::cout << "> ";
            std::getline(std::cin, command);

            tryToSendCommand(command);
        }
    }

} // namespace badger

