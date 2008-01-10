FIND_PACKAGE(Boost)
FIND_PACKAGE(LibXml2)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${LIBXML2_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
link_directories(${LIBXML2_LIBRARY} ${Boost_LIBRARY_DIRS})

add_executable (edmonds-alg cmdline.cpp xml.cpp)
add_executable (test-algorithm test.cpp xml.cpp)

target_link_libraries (edmonds-alg xml2)
target_link_libraries (test-algorithm xml2)


add_subdirectory(tests)




install(TARGETS edmonds-alg DESTINATION bin)


IF(BUILD_DOXYGEN)
  FIND_PACKAGE(Doxygen)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  MAKE_DIRECTORY(${CMAKE_BINARY_DIR}/doc)
  MAKE_DIRECTORY(${CMAKE_BINARY_DIR}/doc/doxygen)
  ADD_CUSTOM_TARGET(doxygen-doc ALL ${DOXYGEN} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )
  INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/doc/doxygen DESTINATION share/doc/edmonds-alg )
ENDIF(BUILD_DOXYGEN)
