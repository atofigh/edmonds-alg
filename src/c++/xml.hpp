#ifndef XML_HPP
#define XML_HPP

void printRelaxng();
int parseEdmondsXML(int fd, char * filename, boost::multi_array<double,2> & m, int & root, int & num_vertices, xmlChar ** algorithm );
void validateWithRelaxNG( xmlDocPtr doc_in ,const char * relaxngstr );

#endif /* XML_HPP */
