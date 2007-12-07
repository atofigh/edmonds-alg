#!/bin/sh
xsltproc ${CMAKE_CURRENT_SOURCE_DIR}/to-graphviz.xsl $1 | dot -T$5 -o $3
xsltproc --stringparam resultfile $2  ${CMAKE_CURRENT_SOURCE_DIR}/to-graphviz.xsl $1 | dot -T$5 -o $4


