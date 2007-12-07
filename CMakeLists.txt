cmake_minimum_required(VERSION 2.4.7)
project(edmonds-alg)

set(COMMANDLINENAME edmonds-alg)
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "9")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

add_subdirectory(src/c++)
add_subdirectory(src/docbook)

OPTION(BUILD_DOXYGEN "build Doxygen html documentation" OFF )
OPTION(BUILD_DOCBOOK "build Docbook html documentation" OFF )

# after running cmake, we can generate source tarballs ( in tar.gz and zip ) by running
# make package_source
#

SET(CPACK_GENERATOR TGZ)
SET(CPACK_SOURCE_GENERATOR TGZ;ZIP)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Edmonds's algorithm is a c++ library of various maximum branching and maximum arborescence algorithms (building on the work by Edmonds and Karp). It also contains a command line tool with xml in- and output that uses the library.")
SET(CPACK_PACKAGE_VENDOR "Erik Sjölund, Ali Tofigh")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION}")
INCLUDE(CPack)
