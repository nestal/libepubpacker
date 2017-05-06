#
#   Copyright © 2016 Wan Wai Ho <me@nestal.net>
#
#   This file is subject to the terms and conditions of the Library GNU
#   General Public License.  See the file COPYING in the main directory
#   of the libepubpacker distribution for more details.
#

cmake_minimum_required(VERSION 3.5)
project(libepubpacker VERSION 0.1)

find_package(Boost REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 14)

include_directories(
	${LIBXML2_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB XML_LIB_FILES		xml/*.hh xml/*.cc)
file(GLOB UTIL_LIB_FILES	util/*.hh util/*.cc)
file(GLOB EPUB_LIB_FILES	epub/*.hh epub/*.cc)
add_library(epubpacker
	${EPUB_LIB_FILES}
	${XML_LIB_FILES}
	${UTIL_LIB_FILES}
)
target_link_libraries(epubpacker ${Boost_LIBRARIES} ${LIBXML2_LIBRARIES})

file(GLOB TEST_FILES test/*.cc)
add_executable(epub_ut ${TEST_FILES})
target_link_libraries(epub_ut epubpacker ${GTEST_MAIN_LIBRARY} ${GTEST_LIBRARIES})
