/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the Library GNU
    General Public License.  See the file COPYING in the main directory
    of the libepubpacker distribution for more details.
*/

//
// Created by nestal on 4/23/17.
//

#include "Epub.hh"

#include "Zip.hh"
#include "ISODate.hh"

#include <boost/assert.hpp>

#include <ctime>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace {

const std::string opf_path = "EPUB/package.opf";
const std::string pub_id   = "pub-id";

}

namespace epub {

Epub::Epub(const std::string& unique_id) :
	m_container{"container"}, m_opf{"package"}
{
	auto cns = m_container.Root().NewNS("urn:oasis:names:tc:opendocument:xmlns:container", {});
	m_container.Root().SetNS(cns);
	m_container.Root().SetAttribute(cns, "version", "1.0");

	auto rfiles = m_container.Root().AppendChild("rootfiles", cns);
	auto rfile  = rfiles.AppendChild("rootfile", cns);
	rfile.SetAttribute(cns, "full-path", opf_path);
	rfile.SetAttribute(cns, "media-type", "application/oebps-package+xml");
	
	m_idpf = m_opf.Root().NewNS("http://www.idpf.org/2007/opf", {});
	m_opf.Root().SetNS(m_idpf);
	m_opf.Root().SetAttribute({}, "xml:lang", "en");
	m_opf.Root().SetAttribute(m_idpf, "unique-identifier", pub_id);
	m_opf.Root().SetAttribute(m_idpf, "version", "3.1");
	
	m_metadata = m_opf.Root().AppendChild("metadata", m_idpf);
	m_dc      = m_metadata.NewNS("http://purl.org/dc/elements/1.1/", "dc");
	m_dcterms = m_metadata.NewNS("http://purl.org/dc/terms/", "dcterms");
	
	// See http://idpf.github.io/epub-registries/identifiers/ for identifier schemes
	m_metadata.AppendChild("identifier", m_dc, unique_id).
		SetAttribute({}, "id", pub_id).
		SetAttribute(m_idpf, "scheme", "isbn");
	m_metadata.AppendChild("language", m_dc, "en-US");
	
	m_metadata.AppendChild("meta", m_idpf, "isbn").
		SetAttribute({}, "refines", "#"+pub_id).
		SetAttribute({}, "scheme", "xsd:string");
	
	m_manifest = m_opf.Root().AppendChild("manifest", m_idpf);
	m_spine    = m_opf.Root().AppendChild("spine", m_idpf);
}

void Epub::Generate(const std::string& outfile) const
{
	Zip zip{outfile};
	auto midx = zip.AddData("mimetype", "application/epub+zip");
	zip.SetNoCompression(midx);

	zip.CreateDir("META-INF");
	zip.AddData("META-INF/container.xml", m_container.Dump());
	zip.AddData(opf_path, m_opf.Dump());

	for (auto&& file : m_files)
		zip.AddFile("EPUB/" + file.first, file.second);
}

void Epub::AddSpine(
	const std::string& dest,
	const boost::filesystem::path& src,
	const std::string& mime,
	const std::string& property)
{
	BOOST_ASSERT(m_files.find(dest) == m_files.end());
	
	auto id = Add(dest, src, mime, property);
	BOOST_ASSERT(!id.empty());
	
	auto spitem = m_spine.AppendChild("itemref", m_idpf);
	spitem.SetAttribute(m_idpf, "idref", id);
}

std::string Epub::Add(
	const std::string& dest,
	const boost::filesystem::path& src,
	const std::string& mime,
	const std::string& property)
{
	if (m_files.find(dest) == m_files.end())
	{
		auto id = "itemid" + std::to_string(m_counter++);
		auto mitem = m_manifest.AppendChild("item", m_idpf);
		mitem.SetAttribute({},     "id",   id);
		mitem.SetAttribute(m_idpf, "href", dest);
		mitem.SetAttribute(m_idpf, "media-type", mime);
		
		if (!property.empty())
			mitem.SetAttribute(m_idpf, "properties", property);
		
		m_files.insert({dest, src});
		return id;
	}
	else
	{
		std::cout << "skipping " << dest << std::endl;
		return {};
	}
}

void Epub::SetTitle(const std::string& title)
{
	m_metadata.AppendChild("title", m_dc, title);
	m_metadata.AppendChild("meta", {}, title).SetAttribute({}, "property", "dcterms:title");
}

void Epub::AddAuthor(const std::string& author)
{
	// the role attribute contains the first 3 lowercase character of the relation "author"
	// Defined here: http://www.loc.gov/marc/relators/
	m_metadata.AppendChild("creator", m_dc, author).SetAttribute(m_idpf, "role", "aut");
}

void Epub::AddPublisher(const std::string& publisher)
{
	m_metadata.AppendChild("publisher", m_dc, publisher);
	m_metadata.AppendChild("meta", {}, publisher).SetAttribute({}, "property", "dcterms:publisher");
}

void Epub::SetDate(std::chrono::system_clock::time_point date)
{
	auto str = ISODate{date}.Str();
	
	m_metadata.AppendChild("date", m_dc, str);
	m_metadata.AppendChild("meta", {}, str).SetAttribute({}, "property", "dcterms:date");
}

void Epub::AddLink(const std::string& rel, const std::string& href)
{
	m_metadata.AppendChild("link", m_idpf).
		SetAttribute(m_idpf, "rel", rel).
		SetAttribute(m_idpf, "href", href);
}

void Epub::Acquire(const std::string& href)
{
	AddLink("acquire", href);
}

void Epub::SetModifiedTime(std::chrono::system_clock::time_point date)
{
	m_metadata.AppendChild("meta", {}, ISODate{date}.Str()).
		SetAttribute({}, "property", "dcterms:modified");
}

} // end of namespace
