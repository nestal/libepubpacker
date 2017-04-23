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
#include <iostream>

namespace {

auto OStreamOutputBuffer(std::ostream& os, xmlCharEncoding enc = XML_CHAR_ENCODING_UTF8)
{
	return ::xmlOutputBufferCreateIO(
		// write callback function: return # of bytes written
		[](void *ctx, const char *data, int len)
		{
			auto sbuf = reinterpret_cast<std::streambuf*>(ctx);
			return static_cast<int>(sbuf->sputn(data, len));
		},
		// close callback function: return 0 on success and -1 on errors
		[](void *ctx)
		{
			auto sbuf = reinterpret_cast<std::streambuf*>(ctx);
			return sbuf->pubsync();
		},
		os.rdbuf(),
		::xmlGetCharEncodingHandler(enc)
	);
}

} // end of local namespace

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

Doc::Doc(Doc&& rhs) :
	Doc{}
{
	BOOST_ASSERT(m_doc);
	
	using namespace std;
	swap(m_doc, rhs.m_doc);
}

std::ostream& operator<<(std::ostream& os, const Doc& doc)
{
	::xmlSaveFileTo(OStreamOutputBuffer(os), doc.m_doc, ::xmlGetCharEncodingName(XML_CHAR_ENCODING_UTF8));
	return os;
}

Node Doc::Root()
{
	return {::xmlDocGetRootElement(m_doc)};
}

Node::Node(::xmlNodePtr node) : m_node{node}
{
	BOOST_ASSERT(m_node);
}

std::ostream& operator<<(std::ostream& os, const Node& node)
{
	BOOST_ASSERT(node.m_node);
	BOOST_ASSERT(node.m_node->doc);

	::xmlNodeDumpOutput(
		OStreamOutputBuffer(os),
		node.m_node->doc,
		node.m_node,
		0,
		1,
		::xmlGetCharEncodingName(XML_CHAR_ENCODING_UTF8)
	);
	os.rdbuf()->pubsync();
	return os;
}

std::string Node::Name() const
{
	BOOST_ASSERT(m_node);
	return reinterpret_cast<const char*>(m_node->name);
}

Node Node::AppendChild(const std::string& name)
{
	BOOST_ASSERT(m_node);
	return {::xmlNewChild(m_node, nullptr, BAD_CAST name.c_str(), nullptr)};
}

} // end of namespace
