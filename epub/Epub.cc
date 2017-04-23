/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the Library GNU
    General Public License.  See the file COPYING in the main directory
    of the libepubpacker distribution for more details.
*/

//
// Created by nestal on 4/23/17.
//

#include <iostream>
#include "Epub.hh"

namespace {

const std::string opf_path = "EPUB/package.opf";

}

namespace epub {

Epub::Epub() : m_container{"container"}, m_opf{"package"}
{
	auto cns = m_container.Root().NewNS("urn:oasis:names:tc:opendocument:xmlns:container", {});
	m_container.Root().SetNS(cns);
	m_container.Root().SetAttribute(cns, "version", "1.0");

	auto rfiles = m_container.Root().AppendChild("rootfiles", cns);
	auto rfile  = rfiles.AppendChild("rootfile", cns);
	rfile.SetAttribute(cns, "full-path", opf_path);
	rfile.SetAttribute(cns, "media-type", "application/oebps-package+xml");
	
	auto idpf = m_opf.Root().NewNS("http://www.idpf.org/2007/opf", {});
	m_opf.Root().SetNS(idpf);
	m_opf.Root().SetAttribute({}, "xml:lang", "en");
	m_opf.Root().SetAttribute(idpf, "unique-identifier", "pub-id");
	m_opf.Root().SetAttribute(idpf, "version", "3.1");
	
	auto metadata = m_opf.Root().AppendChild("metadata", idpf);
	metadata.NewNS("http://purl.org/dc/elements/1.1/", "dc");
}

void Epub::Generate(const std::string& outfile) const
{
	std::cout << m_opf << std::endl;
}

} // end of namespace
