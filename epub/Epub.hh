/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the Library GNU
    General Public License.  See the file COPYING in the main directory
    of the libepubpacker distribution for more details.
*/

//
// Created by nestal on 4/23/17.
//

#pragma once

#include "xml/Doc.hh"

namespace epub {

class Epub
{
public:
	Epub();
	
	void Add(const std::string& file);
	
	void Generate(const std::string& outfile) const;
	
private:
	xml::Doc m_container;
	xml::Doc m_opf;
};

} // end of namespace
