/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the booksrv
    distribution for more details.
*/

//
// Created by nestal on 5/6/17.
//

#include "epub/ISODate.hh"

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
