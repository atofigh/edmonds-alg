<?xml version="1.0"?>
<article  
 id="manual">
  <articleinfo>
    <title>Edmonds's algorithm ( edmonds-alg )</title>
  </articleinfo>
  <sect1 id="introduction">
    <title>Introduction</title>
    <para>
         Edmonds's algorithm ( edmonds-alg ) is a c++ library of various maximum branching and maximum arborescence algorithms (building on the work by Edmonds and Karp). It also contains a command line tool with xml in- and output that uses the library. It is licensed under the MIT license.
    </para>
    <para>
       <ulink url="http://en.wikipedia.org/wiki/Edmonds's_algorithm">A Wikipedia article</ulink> describes the original algorithm.
    </para>
    <para>
        The primary URL for this document is
         <ulink url="http://edmonds-alg.sourceforge.net">http://edmonds-alg.sourceforge.net</ulink>.
    </para>
  </sect1>
  <sect1 id="implemented_algorithms">
    <title>Implemented algorithms</title>


    <sect2 id="maximum_spanning_arborescence">
      <title>Maximum spanning arborescence ( Tarjan, n<superscript>2</superscript> )</title>
        <para>
We call this algorithm msa.
      </para>
    </sect2>
    <sect2 id="optimum_branching">
      <title>Optimum branching</title>
        <para>
We call this algorithm ob.
      </para>
    </sect2>
  </sect1>



  <sect1 id="download">
    <title>Download</title>
    <para>                                                                                                                                                              
       Download the software from the <ulink url="http://sourceforge.net/projects/edmonds-alg">sourceforge</ulink> project page. The latest version of edmonds-alg is @PACKAGE_VERSION@.
    </para>
  </sect1>
  <sect1 id="installation">
    <title>Installation</title>
    <sect2 id="building_from_source">
      <title>Building from source</title>
      <para>To build edmonds-alg you need to have this installed
        <itemizedlist mark="bullet">
          <listitem>
            <para>
              <ulink url="http://www.cmake.org">cmake</ulink>
            </para>
          </listitem>
          <listitem>
            <para>
              <ulink url="http://xmlsoft.org/">libxml2</ulink>
 which is needed for the command line tool
            </para>
          </listitem>
          <listitem>
            <para>
              <ulink url="http://www.boost.org/">boost</ulink>
            </para>
          </listitem>
        </itemizedlist>
      </para>
      <para>

First run <command>cmake</command>
<programlisting><![CDATA[
[erik@saturn edmonds-alg-0.9]$ mkdir build
[erik@saturn edmonds-alg-0.9]$ cd build
[erik@saturn build]$ cmake ..
-- Check for working C compiler: /usr/bin/gcc
-- Check for working C compiler: /usr/bin/gcc -- works
-- Check size of void*
-- Check size of void* - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Found LibXml2: /usr/lib/libxml2.so
-- Configuring done
-- Generating done
-- Build files have been written to: build
]]></programlisting>
Then run  <command>make</command>
<programlisting><![CDATA[
[erik@saturn build]$ make
Scanning dependencies of target edmonds-alg
[100%] Building CXX object src/CMakeFiles/edmonds-alg.dir/cmdline.o
Linking CXX executable edmonds-alg
[100%] Built target edmonds-alg
]]></programlisting>
      </para>
    </sect2>
  </sect1>
  <sect1 id="c++-library-api">
    <title>C++ library API</title>
    <para>    
       The API for the edmonds-alg c++ library is documented <ulink url="doxygen/index.html">here</ulink>
    </para>
  </sect1>


  <sect1 id="command_line_tool">
    <title>Command line tool</title>
      <para>edmonds-alg includes a command line tool that give users a quick way to try out the capabilities of edmonds-alg. Using the c++ library directly will of course be faster.</para> 


    <sect2 id="command_line_options">
      <title>Command line options</title>

    <para>



<programlisting><![CDATA[
[erik@saturn ~]$ ]]><xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_SOURCE_DIR}/commands/help">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
<xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/help">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
</programlisting>

      </para>
    </sect2>
    <sect2 id="input_file_format">
      <title>Input file format</title>
        <para>
Input files are in xml format and need to validate against this relaxng schema

<programlisting><![CDATA[
[erik@saturn ~]$ ]]><xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_SOURCE_DIR}/commands/relaxng">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
<xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/relaxng">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
</programlisting>


      </para>
    </sect2>
    <sect2 id="examples">
      <title>Examples</title>
        <para>
<example id="msa_example"><title>msa example</title>
<para>
Let us try out the algorithm in <xref linkend="maximum_spanning_arborescence"/>.
We will use the input file <ulink url="examples/input-msa.xml">input-msa.xml</ulink> that has this content

<programlisting><xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_SOURCE_DIR}/examples/input-msa.xml">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include></programlisting>

<note><para>
The diagonal elements in the cost matrix are not used. 
</para></note>

<figure id="input_msa_image"><title>graphical representation of input-msa.xml</title>
<mediaobject>
  <imageobject>
    <imagedata  depth="300"  format="SVG"  fileref="images/input-msa.xml.svg"/>
  </imageobject>
</mediaobject>

 <caption>
   <para>The root node is gray.
</para>
  </caption>
</figure>


Let us find the maximum spanning arborescence with the n<superscript>2</superscript>-algorithm

<programlisting><![CDATA[
[erik@saturn ~]$ ]]><xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_SOURCE_DIR}/commands/msa">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
<xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/msa">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
</programlisting>


Instead of giving a filename we can also use a pipe and send the input data over the standard input.


<programlisting><![CDATA[
[erik@saturn ~]$ ]]><xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_SOURCE_DIR}/commands/stdin">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
<xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" encoding="UTF-8" href="${CMAKE_CURRENT_BINARY_DIR}/xincluded_files/stdin">
<xi:fallback>
   couldn't xinclude file
</xi:fallback>
</xi:include>
</programlisting>



<figure id="input_msa_image_result"><title>graphical representation of the result from input-msa.xml</title>
<mediaobject>
  <imageobject>
    <imagedata  depth="400"  format="SVG"  fileref="images/input-msa.xml.result.svg"/>
  </imageobject>
</mediaobject>
</figure>



</para>
</example>



      </para>
    </sect2>


  </sect1>
</article>
