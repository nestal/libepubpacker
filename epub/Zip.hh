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

#include <string>
#include <cstdint>

#include <boost/filesystem/path.hpp>

#include <zip.h>

namespace epub {

class Zip
{
public:
	Zip(const std::string& filename);
	~Zip();
	
	std::uint64_t AddData(const std::string& filename, const std::string& content);
	std::uint64_t AddFile(const std::string& filename, const boost::filesystem::path& file);
	void SetNoCompression(std::uint64_t index);
	
	void CreateDir(const std::string& dir);
	
private:
	::zip_t *m_zip;
};

} // end of namespace epub
