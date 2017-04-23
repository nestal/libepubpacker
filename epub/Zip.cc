/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the Library GNU
    General Public License.  See the file COPYING in the main directory
    of the libepubpacker distribution for more details.
*/

//
// Created by nestal on 4/23/17.
//

#include "Zip.hh"

#include <stdexcept>

namespace epub {
Zip::Zip(const std::string& filename) :
	m_zip{::zip_open(filename.c_str(), ZIP_TRUNCATE|ZIP_CREATE, nullptr)}
{
	if (!m_zip)
		throw std::runtime_error("cannot open zip file " + filename);
}

Zip::~Zip()
{
	::zip_close(m_zip);
}

std::uint64_t Zip::Add(const std::string& filename, const std::string& content)
{
	auto result = ::zip_file_add(m_zip, filename.c_str(),
		::zip_source_buffer_create(content.c_str(), content.size(), 0, nullptr),
		ZIP_FL_OVERWRITE
	);
	
	if (result < 0)
		throw std::runtime_error("cannot add " + filename + " to zip");
	
	return static_cast<std::uint64_t>(result);
}

void Zip::SetNoCompression(std::uint64_t index)
{
	::zip_set_file_compression(m_zip, index, ZIP_CM_STORE, 0);
}

} // end of namespace
