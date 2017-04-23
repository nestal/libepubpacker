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

#include <boost/assert.hpp>

#include <algorithm>
#include <ostream>

namespace xml {


Doc::Doc(const std::string& root) : Doc{}
{
	auto node = ::xmlNewNode(NULL, BAD_CAST root.c_str());
	::xmlDocSetRootElement(m_doc, node);
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

std::ostream& operator<<(std::ostream& os, const Doc& doc)
{
	auto buffer = ::xmlOutputBufferCreateIO(
		[](void *ctx, const char *buffer, int len)
		{
			auto sbuf = reinterpret_cast<std::streambuf*>(ctx);
			return static_cast<int>(sbuf->sputn(buffer, len));
		},
		[](void *ctx)
		{
			return 0;
		},
		os.rdbuf(),
		::xmlGetCharEncodingHandler(XML_CHAR_ENCODING_UTF8)
	);
	::xmlSaveFileTo(buffer, doc.m_doc, "UTF-8");
	return os;
}

} // end of namespace
