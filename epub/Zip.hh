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
#include <zip.h>

namespace epub {

class Zip
{
public:
	Zip(const std::string& filename);
	~Zip();
	
	std::uint64_t Add(const std::string& filename, const std::string& content);
	void SetNoCompression(std::uint64_t index);
	
private:
	::zip_t *m_zip;
};

} // end of namespace epub
