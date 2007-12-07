<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text"/>
<xsl:param name="resultfile"/>
  <xsl:template match="/edmonds-alg-input">
    <xsl:text>digraph G { size="5" ; 
</xsl:text>
    <xsl:if test="/edmonds-alg-input[@root]">
      <xsl:text>
node [shape = circle, style= filled ]; 
</xsl:text>
      <xsl:value-of select="/edmonds-alg-input/@root"/>
      <xsl:text> ;
 
</xsl:text>
    </xsl:if>


    <xsl:text>
node [shape = circle, style= unfilled ];

</xsl:text>
    <xsl:for-each select="/edmonds-alg-input/costmatrix/row">


      <xsl:variable name="tail" select="position() - 1"/>
      <xsl:for-each select="entry">
      <xsl:variable name="head" select="position() - 1"/>
<xsl:if test=" $tail != $head and ( ( string-length($resultfile) > 0 and number(document($resultfile)/result/incomingedge[ $head + 1 ]) = $tail ) or  string-length($resultfile) = 0 ) ">



        <xsl:value-of select="$tail"/>
        <xsl:text>-&gt;</xsl:text>
        <xsl:value-of select="$head"/>

        <xsl:text> [ label = "</xsl:text>
        <xsl:value-of select="."/>
        <xsl:text> " ]

</xsl:text>
</xsl:if>

      </xsl:for-each>


    </xsl:for-each>


<xsl:if test="string-length($resultfile) > 0 ">


        <xsl:text> 

	label = "\nSum=</xsl:text>         <xsl:value-of select="document($resultfile)/result/@sum"/>
        <xsl:text>";


</xsl:text>
</xsl:if>
    <xsl:text> }
</xsl:text>




  </xsl:template>
</xsl:stylesheet>
