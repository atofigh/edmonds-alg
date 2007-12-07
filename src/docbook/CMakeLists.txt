IF(BUILD_DOCBOOK)
  FIND_PROGRAM(XMLTO xmlto)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/index.xml.cmake ${CMAKE_CURRENT_BINARY_DIR}/index.xml)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/create-images.sh.cmake ${CMAKE_CURRENT_BINARY_DIR}/create-images.sh )
 GET_TARGET_PROPERTY(EDMONDS_ALG_EXE edmonds-alg LOCATION)

 FILE(GLOB commands RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/commands ${CMAKE_CURRENT_SOURCE_DIR}/commands/[^.]*[^~])
 FILE(GLOB examples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/examples ${CMAKE_CURRENT_SOURCE_DIR}/examples/input*[^~])

FOREACH(j ${examples})

  ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${j}.result
   COMMAND ${EDMONDS_ALG_EXE} ${CMAKE_CURRENT_SOURCE_DIR}/examples/${j} > ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${j}.result
   DEPENDS edmonds-alg
  )
  LIST( APPEND depfiles ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${j}.result )

  ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/images/${j}.svg
   COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/create-images.sh  ${CMAKE_CURRENT_SOURCE_DIR}/examples/${j} ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${j}.result  ${CMAKE_CURRENT_BINARY_DIR}/images/${j}.svg ${CMAKE_CURRENT_BINARY_DIR}/images/${j}.result.svg svg
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${j}.result
  )
  LIST( APPEND depfiles ${CMAKE_CURRENT_BINARY_DIR}/images/${j}.svg )
  LIST( APPEND depfiles ${CMAKE_CURRENT_BINARY_DIR}/images/${j}.result.svg )

ENDFOREACH(j)


FOREACH(i ${commands})
  ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${i}
   COMMAND /bin/sh ${CMAKE_CURRENT_SOURCE_DIR}/helper.sh ${CMAKE_BINARY_DIR}/src/c++ ${CMAKE_CURRENT_SOURCE_DIR}/commands/${i} > ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${i}
   DEPENDS edmonds-alg
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples
  )
  LIST( APPEND depfiles ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/${i} )
ENDFOREACH(i)




MAKE_DIRECTORY( ${CMAKE_CURRENT_BINARY_DIR}/xincluded_files )
MAKE_DIRECTORY( ${CMAKE_CURRENT_BINARY_DIR}/images )
ADD_CUSTOM_TARGET(docbook-doc ALL ${XMLTO} --skip-validation -m ${CMAKE_CURRENT_SOURCE_DIR}/params.xsl xhtml-nochunks -o ${CMAKE_CURRENT_BINARY_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/index.xml 
    DEPENDS ${depfiles} )
  INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/style DESTINATION share/doc/edmonds-alg PATTERN ".svn" EXCLUDE )
  INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION share/doc/edmonds-alg PATTERN ".svn" EXCLUDE )
  INSTALL(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/images DESTINATION share/doc/edmonds-alg PATTERN ".svn" EXCLUDE )
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/index.html DESTINATION share/doc/edmonds-alg )
ENDIF(BUILD_DOCBOOK)
