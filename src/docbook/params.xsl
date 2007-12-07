<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version='1.0'>

<!-- for documentation about the parameters look here http://docbook.sourceforge.net/release/xsl/snapshot/doc/html/ -->

<xsl:param name="html.stylesheet">style/style.css</xsl:param>
<xsl:param name="admon.graphics" select="1"/>
<xsl:param name="admon.graphics.path">../style/images/</xsl:param>
<xsl:param name="admon.graphics.extension" select="'.png'"/>
<xsl:param name="generate.section.toc.level" select="0"></xsl:param>
<xsl:param name="toc.section.depth">3</xsl:param>

<xsl:param name="section.autolabel" select="1"/>



<xsl:param name="generate.toc">
appendix  toc,title
article/appendix  nop
article   toc,title,table,example,equation
book      toc,title,figure,table,example,equation
chapter   toc,title
part      toc,title
preface   toc,title
qandadiv  toc
qandaset  toc
reference toc,title
sect1     toc
sect2     toc
sect3     toc
sect4     toc
sect5     toc
section   toc
set       toc,title
</xsl:param>



<xsl:template name="user.footer.content">
<HR/>
edmonds-alg is hosted at <br/><br/>
<a href="http://sourceforge.net"><img src="http://sflogo.sourceforge.net/sflogo.php?group_id=209627&amp;type=1" width="88" height="31" border="0" alt="SourceForge.net Logo" /></a>  
</xsl:template>

</xsl:stylesheet>
