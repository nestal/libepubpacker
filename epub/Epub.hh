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

#include <boost/filesystem/path.hpp>
#include <unordered_map>

namespace epub {

class Epub
{
public:
	Epub(const std::string& unique_id, const std::string& title, const std::vector<std::string>& authors);
	
	void AddSpine(
		const std::string& dest,
		const boost::filesystem::path& src,
		const std::string& mime = "application/xhtml+xml",
		const std::string& property = {}
	);
	std::string Add(
		const std::string& dest,
		const boost::filesystem::path& src,
		const std::string& mime,
		const std::string& property = {}
	);
	
	void Generate(const std::string& outfile) const;
	
private:
	xml::Doc m_container;
	xml::Doc m_opf;
	
	xml::Namespace m_idpf;
	
	xml::Node m_manifest;
	xml::Node m_spine;
	
	int m_counter{0};
	
	std::unordered_map<std::string, boost::filesystem::path> m_files;
};

} // end of namespace
