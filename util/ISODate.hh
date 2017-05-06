/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the booksrv
    distribution for more details.
*/

//
// Created by nestal on 5/6/17.
//

#pragma once

#include <chrono>
#include <iosfwd>
#include <ctime>
#include <string>

namespace util {

class ISODate
{
public:
	ISODate() = default;
	ISODate(std::chrono::system_clock::time_point tp);
	ISODate(struct tm tm);
	ISODate(const std::string& str);
	
	ISODate(int year, int month, int day, int hour, int minute, int second);
	
	struct tm Tm() const;
	
	std::string Str() const;
	
	std::chrono::system_clock::time_point Get() const;
	
	friend std::istream& operator>>(std::istream& is, ISODate& date);
	friend std::ostream& operator<<(std::ostream& is, const ISODate& date);
	
private:
	std::chrono::system_clock::time_point m_timepoint{};
};

std::string to_string(ISODate date);

} // end of namespace
