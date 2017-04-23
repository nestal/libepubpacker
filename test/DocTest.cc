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
	Doc subject{"container"};
	auto ns = subject.Root().NewNS("urn:oasis:names:tc:opendocument:xmlns:container", {});
	subject.Root().SetNS(ns);
	subject.Root().SetAttribute(ns, "version", "1.0");
	subject.Root().SetAttribute({}, "xml:lang", "en");

	auto rfiles = subject.Root().AppendChild("rootfiles", ns);
	auto rfile  = rfiles.AppendChild("rootfile", ns);
	rfile.SetAttribute(ns, "full-path", "somepath");
	rfile.SetAttribute(ns, "media-type", "application/oebps-package+xml");
	
	Doc s2{std::move(subject)};
	
	ASSERT_EQ("container", s2.Root().Name());
	std::cout << s2 << std::endl;
}
