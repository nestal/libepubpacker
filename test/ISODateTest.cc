/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the booksrv
    distribution for more details.
*/

//
// Created by nestal on 5/6/17.
//

#include "util/ISODate.hh"

#include <gtest/gtest.h>

using namespace epub;

TEST(ISODateTest, NormalTest)
{
	ISODate epoch{1970, 1, 1, 0, 0, 0};
	ASSERT_EQ("1970-02-01T00:00:00Z", epoch.Str());
	
	auto tm = epoch.Tm();
	ASSERT_EQ(70, tm.tm_year);
	ASSERT_EQ(1,  tm.tm_mon);
	ASSERT_EQ(1,  tm.tm_mday);
	ASSERT_EQ(0,  tm.tm_hour);
	ASSERT_EQ(0,  tm.tm_min);
	ASSERT_EQ(0,  tm.tm_sec);
}

TEST(ISODateTest, TestString)
{
	ISODate subject{"2017-11-20T12:34:56Z"};
	
	auto tm = subject.Tm();
	ASSERT_EQ(117, tm.tm_year);
	ASSERT_EQ(10,  tm.tm_mon);
	ASSERT_EQ(20,  tm.tm_mday);
	ASSERT_EQ(12,  tm.tm_hour);
	ASSERT_EQ(34,  tm.tm_min);
	ASSERT_EQ(56,  tm.tm_sec);
}

TEST(ISODateTest, TestStringWithUs)
{
	ISODate subject{"2017-04-13T12:07:39.652584Z"};
	
	auto tm = subject.Tm();
	ASSERT_EQ(117, tm.tm_year);
	ASSERT_EQ(3,  tm.tm_mon);
	ASSERT_EQ(13,  tm.tm_mday);
	ASSERT_EQ(12,  tm.tm_hour);
	ASSERT_EQ(07,  tm.tm_min);
	ASSERT_EQ(39,  tm.tm_sec);
}
