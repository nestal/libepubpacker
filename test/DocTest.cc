/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the Library GNU
    General Public License.  See the file COPYING in the main directory
    of the libepubpacker distribution for more details.
*/

//
// Created by nestal on 4/23/17.
//

#include <gtest/gtest.h>

#include "xml/Doc.hh"
#include <iostream>

using namespace xml;

TEST(DocTest, TestMoveCtor)
{
	Doc subject{"someroot"};
	subject.Root().AppendChild("child");
	
	Doc s2{std::move(subject)};
	
	ASSERT_EQ("someroot", s2.Root().Name());
	std::cout << s2 << std::endl;
}
