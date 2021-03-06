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
#include <chrono>

namespace epub {

class Epub
{
public:
	Epub(const std::string& unique_id);
	
	void SetTitle(const std::string& title);
	void AddAuthor(const std::string& author);
	void AddPublisher(const std::string& publisher);
	void SetDate(std::chrono::system_clock::time_point date);
	void SetModifiedTime(std::chrono::system_clock::time_point mtime);
	void SetCreationTime(std::chrono::system_clock::time_point ctime);
	void SetDescription(const std::string& text);
	void AddSubject(const std::string& subject);
	void SetRights(const std::string& rights);
	void SetLanguage(const std::string& lang);
	void SetFormat(const std::string& format, const std::string& extent);
	
	// links: https://idpf.github.io/epub-vocabs/package/link/
	void AddLink(const std::string& rel, const std::string& href);
	void Acquire(const std::string& href);
	
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
	xml::Namespace m_dc;
	xml::Namespace m_dcterms;
	
	xml::Node m_manifest;
	xml::Node m_spine;
	xml::Node m_metadata;
	
	int m_counter{0};
	
	std::unordered_map<std::string, boost::filesystem::path> m_files;
};

} // end of namespace
