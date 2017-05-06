/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the booksrv
    distribution for more details.
*/

//
// Created by nestal on 5/6/17.
//

#include "ISODate.hh"

#include <iomanip>

namespace epub {

ISODate::ISODate(std::chrono::system_clock::time_point tp) : m_timepoint{tp}
{
}

ISODate::ISODate(struct tm tm) : m_timepoint{std::chrono::system_clock::from_time_t(timegm(&tm))}
{
}

std::istream& operator>>(std::istream& is, ISODate& date)
{
	struct tm tm{};
	if (is >> std::get_time(&tm, "%F"))
		date.m_timepoint = std::chrono::system_clock::from_time_t(timegm(&tm));
	
	return is;
}

std::ostream& operator<<(std::ostream& os, const ISODate& date)
{
	auto tm = date.Tm();
	return os << std::put_time(&tm, "%FT%TZ");
}

ISODate::ISODate(int year, int month, int day, int hour, int minute, int second)
{
	struct tm tm{};
	tm.tm_year = year-1900;
	tm.tm_mon  = month;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min  = minute;
	tm.tm_sec  = second;
	
	m_timepoint = std::chrono::system_clock::from_time_t(timegm(&tm));
}

struct tm ISODate::Tm() const
{
	auto tt = std::chrono::system_clock::to_time_t(m_timepoint);
	
	struct tm result{};
	::gmtime_r(&tt, &result);
	
	return result;
}

std::string ISODate::Str() const
{
	std::ostringstream ss;
	ss << *this;
	return ss.str();
}

std::string to_string(ISODate date)
{
	return date.Str();
}

} // end of namespace
