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

#include "Time.hpp"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace badger
{
    Time::Time
    (
    )
    {
        set(12, 0, 0);
    }


    Time::Time
    (
        unsigned int hour, 
        unsigned int minute, 
        unsigned int second
    )
    {
        set(hour, minute, second);
    }


    void Time::set
    (
        unsigned int hour, 
        unsigned int minute, 
        unsigned int second
    )
    {
        setHour(hour);
        setMinute(minute);
        setSecond(second);
    }


    void Time::setHour
    (
        unsigned int hour
    )
    {
        if(hour == 0 || hour > 24)
            throw std::runtime_error("Bad hour");

        m_hour = hour;
    }


    unsigned int Time::getHour
    (
    ) const
    {
        return m_hour;
    }


    void Time::setMinute
    (
        unsigned int minute
    )
    {
        if(minute > 60)
            throw std::runtime_error("Bad minute");

        m_minute = minute;
    }


    unsigned int Time::getMinute
    (
    ) const
    {
        return m_minute;
    }


    void Time::setSecond
    (
        unsigned int second
    )
    {
        if(second > 60)
            throw std::runtime_error("Bad second");

        m_second = second;
    }


    unsigned int Time::getSecond
    (
    ) const
    {
        return m_second;
    }










    bool operator==
    (
        const Time &left, 
        const Time &right
    )
    {
        return left.getHour() == right.getHour() &&
               left.getMinute() == right.getMinute() &&
               left.getSecond() == right.getSecond();
    }


    bool operator!=
    (
        const Time &left, 
        const Time &right
    )
    {
        return !(left == right);
    }


    bool operator<
    (
        const Time &left, 
        const Time &right
    )
    {
        if(left.getHour() == right.getHour())
        {
            if(left.getMinute() == right.getMinute())
                return left.getSecond() < right.getSecond();

            else 
                return left.getMinute() < right.getMinute();
        }

        else 
            return left.getHour() < right.getHour();
    }


    bool operator>
    (
        const Time &left, 
        const Time &right
    )
    {
        return right < left;
    }


    bool operator<=
    (
        const Time &left, 
        const Time &right
    )
    {
        return !(right < left);
    }


    bool operator>=
    (
        const Time &left, 
        const Time &right
    )
    {
        return !(left < right);
    }







    std::ostream& operator<<
    (
        std::ostream &stream, 
        const Time &time
    )
    {
        return stream << time.getHour() << ':' << time.getMinute() << ':' << time.getSecond();
    }


    std::istream& operator>>
    (
        std::istream &stream, 
        Time &time
    )
    {
        unsigned int hour, minute, second;

        std::string str;
        stream >> str;

        if( std::sscanf(&str[0], "%u:%u:%u", &hour, &minute, &second) != 3)
            throw std::runtime_error("Unable to extract a time"); 

        time.set(hour, minute, second);

        return stream;
    }

} // namespace badger

