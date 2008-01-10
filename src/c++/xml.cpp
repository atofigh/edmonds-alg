#include <iostream>
#include <libxml/xpath.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/relaxng.h>
#include <boost/multi_array.hpp>

static const char relaxngstr [] = "<?xml version=\"1.0\"?>\n"
"<element xmlns=\"http://relaxng.org/ns/structure/1.0\" name=\"edmonds-alg-input\" datatypeLibrary=\"http://www.w3.org/2001/XMLSchema-datatypes\">\n"
"  <attribute name=\"num_vertices\">\n"
"    <data type=\"int\"/>\n"
"  </attribute>\n"
"  <choice>\n"
"    <group>\n"
"      <attribute name=\"algorithm\">\n"
"        <value>msa</value>\n"
"      </attribute>\n"
"      <optional>\n"
"        <attribute name=\"root\">\n"
"          <data type=\"int\"/>\n"
"        </attribute>\n"
"      </optional>\n"
"    </group>\n"
"    <group>\n"
"      <attribute name=\"algorithm\">\n"
"        <value>ob</value>\n"
"      </attribute>\n"
"      <optional>\n"
"        <attribute name=\"root\">\n"
"          <data type=\"int\"/>\n"
"        </attribute>\n"
"      </optional>\n"
"    </group>\n"
"  </choice>\n"
"  <element name=\"costmatrix\">\n"
"    <oneOrMore>\n"
"      <element name=\"row\">\n"
"        <oneOrMore>\n"
"          <element name=\"entry\">\n"
"            <data type=\"double\"/>\n"
"          </element>\n"
"        </oneOrMore>\n"
"      </element>\n"
"    </oneOrMore>\n"
"  </element>\n"
  "</element>\n";


void printRelaxng() {
  std::cout << relaxngstr;
}

void validateWithRelaxNG( xmlDocPtr doc_in ,const char * relaxngstr ) {
  xmlRelaxNGParserCtxtPtr parserctxt;
  size_t len = strlen(relaxngstr);
  parserctxt = xmlRelaxNGNewMemParserCtxt(relaxngstr,len);
  xmlRelaxNGSetParserErrors(parserctxt,(xmlRelaxNGValidityErrorFunc) fprintf, (xmlRelaxNGValidityWarningFunc) fprintf, stderr);
  xmlRelaxNGPtr schema = NULL;
  schema = xmlRelaxNGParse(parserctxt);
  xmlRelaxNGFreeParserCtxt(parserctxt);
  int ret;
  xmlRelaxNGValidCtxtPtr validctxt;
  validctxt = xmlRelaxNGNewValidCtxt(schema);
  xmlRelaxNGSetValidErrors(validctxt, (xmlRelaxNGValidityErrorFunc) fprintf, (xmlRelaxNGValidityWarningFunc) fprintf, stderr);
  ret = xmlRelaxNGValidateDoc(validctxt, doc_in);
  if (ret > 0) {
    printf("xml input fails to validate\n");
    exit(EXIT_FAILURE);
  }
  if (ret < 0) {
    printf("internal or API error\n");
    exit(EXIT_FAILURE);
  }
  xmlRelaxNGFreeValidCtxt(validctxt);
}

int parseEdmondsXML(int fd, char * filename, boost::multi_array<double,2> & m, int & root, int & num_vertices, xmlChar ** algorithm ) {

  LIBXML_TEST_VERSION;
  xmlDocPtr doc_in = xmlReadFd(fd, filename, NULL, 0 );
  if ( doc_in == 0 ) {
	  std::cerr << "error: the input is not well formed XML"  <<  std::endl;
	  exit(EXIT_FAILURE);
  }

  validateWithRelaxNG(doc_in, relaxngstr );

  xmlNodePtr rootnode = xmlDocGetRootElement(doc_in);
  if (rootnode == NULL) {
    std::cerr << "error: xml input is not well formed" << std::endl;
    exit(EXIT_FAILURE);
  }

  xmlNode *node1, *node2, *node3;
  xmlChar * num_vertices_str = xmlGetProp(rootnode, (const xmlChar *) "num_vertices") ;
  num_vertices = atoi((const char * ) num_vertices_str  );
  xmlFree(num_vertices_str);
  assert( num_vertices > 0 );
  *algorithm = xmlGetProp(rootnode, (const xmlChar *) "algorithm") ;
  root = -1;
  if ( xmlHasProp(rootnode, (const xmlChar *) "root") != 0 ) { 
    xmlChar * rootstr = xmlGetProp(rootnode, (const xmlChar *) "root") ;
    root = atoi((const char * ) rootstr  );
    assert( root >= 0 and root < num_vertices );
  }
  m.resize(boost::extents[num_vertices][num_vertices]);
  bool m_filled = false;

  for (node1 = rootnode->children; node1; node1 = node1->next) {
     if (node1->type == XML_ELEMENT_NODE and   !xmlStrcmp( node1->name, (const xmlChar *) "costmatrix" )   ) {
      boost::multi_array_types::index i = 0;
      for (node2 = node1->children; node2; node2 = node2->next) {
	if (node2->type == XML_ELEMENT_NODE and   !xmlStrcmp( node2->name, (const xmlChar *) "row" )   ) {
          boost::multi_array_types::index j = 0;
	  for (node3 = node2->children; node3; node3 = node3->next) {
	    if (node3->type == XML_ELEMENT_NODE and  !xmlStrcmp( node3->name, (const xmlChar *) "entry" )  ) { 
	      assert( i < num_vertices );            
	      assert( j < num_vertices );            
	      m[i][j] = xmlXPathCastNodeToNumber(node3);
	      j++;
	    }
	  }
          assert( j == num_vertices );
	  i++;
	}
      }
      assert( i == num_vertices );
      m_filled = true;
    }
  }
  assert( m_filled );
  xmlFreeDoc(doc_in);
  return 0;
};


void writeXML( double sum, std::vector<unsigned> parent) {

  xmlDocPtr doc_out = NULL;   
  xmlNodePtr root_out = NULL;

  doc_out = xmlNewDoc(BAD_CAST "1.0");
  root_out = xmlNewNode(NULL, BAD_CAST "result");
  xmlDocSetRootElement(doc_out, root_out);
  char buff[256];

  snprintf(buff,256,"%f",sum);  
  xmlNewProp(root_out, BAD_CAST "sum", BAD_CAST buff );

  for ( std::vector<unsigned>::const_iterator iter = parent.begin(); iter != parent.end(); ++iter ) {
    snprintf(buff,256,"%i",*iter);
    xmlNodePtr iedge = xmlNewChild(root_out, NULL, BAD_CAST "incomingedge", BAD_CAST buff);
  }

  xmlSaveFormatFileEnc("-",doc_out,"UTF-8",1);
  xmlFreeDoc(doc_out);
  xmlCleanupParser();

};
