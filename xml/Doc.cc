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
#include <iostream>
#include <sstream>

namespace {

auto OStreamOutputBuffer(std::ostream& os, xmlCharEncoding enc = XML_CHAR_ENCODING_UTF8)
{
	auto buf = ::xmlOutputBufferCreateIO(
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
	if (!buf)
		throw -1;
	return buf;
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

std::string Doc::Dump() const
{
	std::ostringstream ss;
	ss << *this;
	return ss.str();
}

CNode Doc::Root() const
{
	return {::xmlDocGetRootElement(m_doc)};
}

Node  Doc::Root()
{
	return {::xmlDocGetRootElement(m_doc)};
}

CNode::CNode(::xmlNodePtr node) : m_node{node}
{
	BOOST_ASSERT(m_node);
}

std::string CNode::Attribute(const std::string& name) const
{
	return reinterpret_cast<const char*>(::xmlGetProp(m_node, BAD_CAST name.c_str()));
}

std::ostream& operator<<(std::ostream& os, const CNode& node)
{
	BOOST_ASSERT(node.m_node);
	BOOST_ASSERT(node.m_node->doc);

	auto buf = OStreamOutputBuffer(os);
	::xmlNodeDumpOutput(
		buf,
		nullptr,
		node.m_node,
		0,
		1,
		::xmlGetCharEncodingName(XML_CHAR_ENCODING_UTF8)
	);
	::xmlOutputBufferFlush(buf);
	
	return os;
}

std::string CNode::Name() const
{
	BOOST_ASSERT(m_node);
	return reinterpret_cast<const char*>(m_node->name);
}

::xmlNodePtr CNode::NodePtr() const
{
	BOOST_ASSERT(m_node);
	return m_node;
}

Node Node::AppendChild(const std::string& name, Namespace ns, const std::string& content)
{
	return {::xmlNewChild(NodePtr(), ns, BAD_CAST name.c_str(), BAD_CAST content.c_str())};
}

Namespace Node::NewNS(const std::string& href, const std::string& prefix)
{
	// if "prefix" is empty, this will be the default namespace
	return ::xmlNewNs(NodePtr(), BAD_CAST href.c_str(), prefix.empty() ? nullptr : BAD_CAST prefix.c_str());
}

Node Node::SetAttribute(Namespace ns, const std::string& name, const std::string& value)
{
	::xmlSetNsProp(NodePtr(), ns, BAD_CAST name.c_str(), BAD_CAST value.c_str());
	return *this;
}

void Node::SetNS(Namespace ns)
{
	::xmlSetNs(NodePtr(), ns);
}

} // end of namespace
