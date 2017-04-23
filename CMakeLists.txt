#
# Copyright © 2016 Wan Wai Ho <me@nestal.net>
# All Rights Reserved.
#

cmake_minimum_required(VERSION 3.5)
project(libepubpacker VERSION 0.1)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 14)

file(GLOB XML_LIB_FILES	xml/*.hh xml/*.cc)
add_library(xmllib
	${XML_LIB_FILES}
)
