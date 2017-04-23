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
using Namespace = ::xmlNsPtr;

class Doc
{
public:
	Doc() = default;
	Doc(const std::string& root);
	Doc(Doc&& rhs);
	~Doc();
	
	friend std::ostream& operator<<(std::ostream& os, const Doc& doc);
	
	Node Root();
	
private:
	::xmlDocPtr m_doc{::xmlNewDoc(BAD_CAST "1.0")};
};

class Node
{
public:
	Node() = default;
	
	friend std::ostream& operator<<(std::ostream& os, const Node& node);
	
	std::string Name() const;
	Node AppendChild(const std::string& name, Namespace ns = {}, const std::string& content = {});
	
	Namespace NewNS(const std::string& href, const std::string& prefix);
	
private:
	friend class Doc;
	Node(::xmlNodePtr node);
	
private:
	::xmlNodePtr m_node{};
};

} // end of namespace
