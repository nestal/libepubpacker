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

#include <libxml/tree.h>
#include <string>
#include <iosfwd>

namespace xml {

class Doc
{
public:
	Doc() = default;
	Doc(const std::string& root);
	Doc(Doc&& rhs);
	~Doc();
	
	friend std::ostream& operator<<(std::ostream& os, const Doc& doc);
	
private:
	::xmlDocPtr m_doc{::xmlNewDoc(BAD_CAST "1.0")};
};

} // end of namespace
