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
// unsigned intERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////

#include "Date.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace badger
{
    bool Date::isLeapYear
    (
        unsigned int year
    )
    {
        return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
    }






    Date::Date
    (
    )
    {
        set(1, 1, 1990);
    }


    Date::Date
    (
        unsigned int day, 
        unsigned int month, 
        unsigned int year
    )
    {
        set(day, month, year);
    }


    void Date::set
    (
        unsigned int day, 
        unsigned int month, 
        unsigned int year
    )
    {
        // Set the month and the year before the day for checking this last.
        setMonth(month);
        setYear(year);

        setDay(day);
    }


    void Date::setDay
    (
        unsigned int day
    )
    {
        unsigned int lastDay = 31;

        if(m_month == 2)
            lastDay = isLeapYear(m_year) ? 29 : 28;

        else if(m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11)
            lastDay = 30;


        // Now check if the day is valid in the current month
        if(day == 0 || day > lastDay)
            throw std::runtime_error("Invalid day");

        m_day = day;
    }


    unsigned int Date::getDay
    (
    ) const
    {
        return m_day;
    }


    void Date::setMonth
    (
        unsigned int month
    )
    {
        if(month == 0 || month > 12)
            throw std::runtime_error("Invalid month");

        m_month = month;
    }


    unsigned int Date::getMonth
    (
    ) const
    {
        return m_month;
    }


    void Date::setYear
    (
        unsigned int year
    )
    {
        m_year = year;
    }


    unsigned int Date::getYear
    (
    ) const
    {
        return m_year;
    }










    bool operator==
    (
        const Date &left, 
        const Date &right
    )
    {
        return left.getDay() == right.getDay() &&
               left.getMonth() == right.getMonth() &&
               left.getYear() == right.getYear();
    }


    bool operator!=
    (
        const Date &left, 
        const Date &right
    )
    {
        return !(left == right);
    }


    bool operator<
    (
        const Date &left, 
        const Date &right
    )
    {
        if(left.getYear() == right.getYear())
        {
            if(left.getMonth() == right.getMonth())
                return left.getDay() < right.getDay();

            else 
                return left.getMonth() < right.getMonth();
        }

        else 
            return left.getYear() < right.getYear();
    }






    std::ostream& operator<<
    (
        std::ostream &stream, 
        const Date &date
    )
    {
        return stream << date.getDay() << '/' << date.getMonth() << '/' << date.getYear();
    }


    std::istream& operator>>
    (
        std::istream &stream, 
        Date &date
    )
    {
        unsigned int day, month, year;

        std::string str;
        stream >> str;

        if( std::sscanf(&str[0], "%u/%u/%u", &day, &month, &year) != 3)
            throw std::runtime_error("Unable to extract a date"); 

        date.set(day, month, year);

        return stream;
    }

} // namespace badger

