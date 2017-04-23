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

#include <stdexcept>
#include <iostream>

namespace {

const std::string opf_path = "EPUB/package.opf";
const std::string pub_id   = "pub-id";

}

namespace epub {

Epub::Epub(const std::string& unique_id, const std::string& title) :
	m_container{"container"}, m_opf{"package"}
{
	auto cns = m_container.Root().NewNS("urn:oasis:names:tc:opendocument:xmlns:container", {});
	m_container.Root().SetNS(cns);
	m_container.Root().SetAttribute(cns, "version", "1.0");

	auto rfiles = m_container.Root().AppendChild("rootfiles", cns);
	auto rfile  = rfiles.AppendChild("rootfile", cns);
	rfile.SetAttribute(cns, "full-path", opf_path);
	rfile.SetAttribute(cns, "media-type", "application/oebps-package+xml");
	
	m_idpf = m_opf.Root().NewNS("http://www.m_idpf.org/2007/opf", {});
	m_opf.Root().SetNS(m_idpf);
	m_opf.Root().SetAttribute({}, "xml:lang", "en");
	m_opf.Root().SetAttribute(m_idpf, "unique-identifier", pub_id);
	m_opf.Root().SetAttribute(m_idpf, "version", "3.1");
	
	auto metadata = m_opf.Root().AppendChild("metadata", m_idpf);
	auto dc = metadata.NewNS("http://purl.org/dc/elements/1.1/", "dc");
	
	metadata.AppendChild("identifier", dc, unique_id).SetAttribute({}, "id", pub_id);
	metadata.AppendChild("language", dc, "en-US");
	metadata.AppendChild("title", dc, title);
	
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
		zip.AddFile(
			"EPUB/" + file.second.filename().string(),
			file.second
		);
}

void Epub::Add(const boost::filesystem::path& file)
{
	auto id = "itemid" + std::to_string(m_counter++);
	auto mitem = m_manifest.AppendChild("item", m_idpf);
	mitem.SetAttribute({},     "id",   id);
	mitem.SetAttribute(m_idpf, "href", file.filename().string());
	mitem.SetAttribute(m_idpf, "media-type", "application/xhtml+xml");
	
	auto spitem = m_spine.AppendChild("itemref", m_idpf);
	spitem.SetAttribute(m_idpf, "idref", id);
	
	m_files.insert({id, file});
}

} // end of namespace
