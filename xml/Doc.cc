/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the Library GNU
    General Public License.  See the file COPYING in the main directory
    of the libepubpacker distribution for more details.
*/

//
// Created by nestal on 4/23/17.
//

#include "Doc.hh"
#include <algorithm>

#include <boost/assert.hpp>

namespace xml {


Doc::Doc() : m_doc{::xmlNewDoc(BAD_CAST "1.0")}
{
}

Doc::~Doc()
{
	BOOST_ASSERT(m_doc);
	::xmlFreeDoc(m_doc);
}

Doc::Doc(Doc&& rhs) : Doc{}
{
	BOOST_ASSERT(m_doc);
	
	using namespace std;
	swap(m_doc, rhs.m_doc);
}

} // end of namespace
