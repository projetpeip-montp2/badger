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

#ifndef BADGER_DATE_HPP
#define BADGER_DATE_HPP

#include <iosfwd>

namespace badger
{
    class Date
    {
    public:
        Date();

        Date(unsigned int day, unsigned int month, unsigned int year);

        void set(unsigned int day, unsigned int month, unsigned int year);

        void setDay(unsigned int day);

        unsigned int getDay() const;

        void setMonth(unsigned int month);

        unsigned int getMonth() const;

        void setYear(unsigned int year);

        unsigned int getYear() const;


        
        static bool isLeapYear(unsigned int year);

    private:
	    ////////////////////////////////////////////////////////////
	    // Member data
	    ////////////////////////////////////////////////////////////
        unsigned int m_day;
        unsigned int m_month;
        unsigned int m_year;
    };


    bool operator==(const Date &left, const Date &right);

    bool operator!=(const Date &left, const Date &right);

    bool operator<(const Date &left, const Date &right);


    std::ostream& operator<<(std::ostream &stream, const Date &date);

    std::istream& operator>>(std::istream &stream, Date &date);

} // namespace badger

#endif // BADGER_DATE_HPP

