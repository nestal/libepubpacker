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

using namespace xml;

TEST(DocTest, TestMoveCtor)
{
	Doc subject;
	
	Doc s2{std::move(subject)};
}