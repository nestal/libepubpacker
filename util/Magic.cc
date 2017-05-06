/*
	Copyright © 2017 Wan Wai Ho <me@nestal.net>
    
    This file is subject to the terms and conditions of the GNU General Public
    License.  See the file COPYING in the main directory of the booksrv
    distribution for more details.
*/

//
// Created by nestal on 5/6/17.
//

#include "Magic.hh"

namespace util {

Magic::Magic()
{
	::magic_load(m_magic, nullptr);
}

Magic::~Magic()
{
	::magic_close(m_magic);
}

std::string Magic::Mime(const boost::filesystem::path& file) const
{
	auto str = file.string();
	return ::magic_file(m_magic, str.c_str());
}

} // end of namespace util
