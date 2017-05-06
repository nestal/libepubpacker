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

class Doc;
class Node;
class CNode;
using Namespace = ::xmlNsPtr;

class Doc
{
public:
	Doc() = default;
	Doc(const std::string& root);
	Doc(Doc&& rhs);
	~Doc();
	
	friend std::ostream& operator<<(std::ostream& os, const Doc& doc);
	
	CNode Root() const;
	Node  Root();
	
	std::string Dump() const;
	
private:
	::xmlDocPtr m_doc{::xmlNewDoc(BAD_CAST "1.0")};
};

class CNode
{
public:
	CNode() = default;
	CNode(const CNode&) = default;
	CNode(CNode&&) = default;
	CNode& operator=(const CNode&) = default;
	CNode& operator=(CNode&&) = default;
	~CNode() = default;
	
	friend std::ostream& operator<<(std::ostream& os, const CNode& node);
	
	std::string Name() const;
	std::string Attribute(const std::string& name) const;
	
protected:
	friend class Doc;
	CNode(::xmlNodePtr node);
	::xmlNodePtr NodePtr() const;
	
private:
	::xmlNodePtr m_node{};
};

class Node : public CNode
{
public:
	using CNode::CNode;
	using CNode::operator=;
	
	Node AppendChild(const std::string& name, Namespace ns = {}, const std::string& content = {});
	Node SetAttribute(Namespace ns, const std::string& name, const std::string& value);
	
	Namespace NewNS(const std::string& href, const std::string& prefix);
	void SetNS(Namespace ns);
	
private:
	friend class Doc;
};

} // end of namespace
