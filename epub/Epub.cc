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

#include <boost/assert.hpp>

#include <stdexcept>
#include <iostream>

namespace {

const std::string opf_path = "EPUB/package.opf";
const std::string pub_id   = "pub-id";

}

namespace epub {

Epub::Epub(const std::string& unique_id, const std::string& title, const std::vector<std::string>& authors) :
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
	
	auto metadata = m_opf.Root().AppendChild("metadata", m_idpf);
	auto dc = metadata.NewNS("http://purl.org/dc/elements/1.1/", "dc");
	
	metadata.AppendChild("identifier", dc, "urn:isbn:" + unique_id).SetAttribute({}, "id", pub_id);
	metadata.AppendChild("language", dc, "en-US");
	metadata.AppendChild("title", dc, title);
	
	for (auto&& author : authors)
		metadata.AppendChild("creator", dc, author);
	
	metadata.AppendChild("meta", m_idpf, "isbn").
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

} // end of namespace
