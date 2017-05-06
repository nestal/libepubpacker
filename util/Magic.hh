/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the booksrv
    distribution for more details.
*/

//
// Created by nestal on 5/6/17.
//

#pragma once

#include <magic.h>

#include <boost/filesystem/path.hpp>

namespace util {

class Magic
{
public:
	Magic();
	~Magic();
	
	std::string Mime(const boost::filesystem::path& file) const;
	
private:
	::magic_t m_magic{::magic_open(MAGIC_MIME_TYPE)};
};

} // end of namespace
