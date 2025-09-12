<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">


<xsl:template match="DOKULIST">
<xsl:variable name="var_language" select="LANGUAGE" />
<xsl:variable name="var_device" select="DEVICE" />
<xsl:variable name="var_typ" select="TYP" />
<xsl:variable name="var_picfolder" select="PICFOLDER" />
<xsl:variable name="var_prosafolder" select="PROSAFOLDER" />
<xsl:variable name="var_prosa" select="concat($var_prosafolder, $var_device,'/', $var_language)" />
<xsl:variable name="var_prosacom" select="concat($var_prosafolder, $var_language)" />
<xsl:variable name="var_prosatyp" select="concat($var_prosafolder, $var_typ,'/',$var_language,'/')" />


  <html class="noselect" >
  <meta http-equiv="content-type" content="text/html; charset=utf-8"/>
  <head>
<style> 
.noselect {
  -webkit-touch-callout: none; /* iOS Safari */
    -webkit-user-select: none; /* Safari */
     -khtml-user-select: none; /* Konqueror HTML */
       -moz-user-select: none; /* Old versions of Firefox */
        -ms-user-select: none; /* Internet Explorer/Edge */
            user-select: none; /* Non-prefixed version, currently
                                  supported by Chrome, Edge, Opera and Firefox */
}

</style>
</head>

  <body>
  <h1><xsl:copy-of select="$var_device" />_MAN_EXT_<xsl:copy-of select="$var_language" /></h1>
  <xsl:if test="LANGUAGE='de'">
    <p> Diese Beschreibung wurde automatisch erstellt aus: <xsl:value-of select="RELEASE"/></p>
    <xsl:if test="TYP='dc'"><p>Mit DC Option</p></xsl:if>
    </xsl:if>
  <xsl:if test="LANGUAGE='en'">
    <p> This is automatic generated gui description from: <xsl:value-of select="RELEASE"/></p>
    <xsl:if test="TYP='dc'"><p>Including DC option</p></xsl:if>
  </xsl:if>

    <xsl:for-each select="ENTRIES/ENTRY">

        <xsl:if test="DESCRCOM"> <!-- only language (for header mostly)-->
            <xsl:variable name="var_descrcom" select="DESCRCOM" />
            <xsl:copy-of select="document(concat($var_prosacom,'/', $var_descrcom))" />
        <xsl:variable name="var_fotol" select="FOTOLEFT" />
        <xsl:variable name="var_pathfotol" select="concat($var_picfolder,$var_fotol)" />
        
      <!--      <p> Path: <xsl:value-of select="$var_picfolder" /> <br/>
            Picture: <xsl:value-of select="$var_fotol" /> <br/>
            concat: <xsl:value-of select="$var_pathfotol" /> </p> -->
        <p>
          <xsl:if test="FOTOLEFT">
            <img alt="file not found">
              <xsl:attribute name="src">
                <xsl:value-of select="$var_pathfotol" />
                  </xsl:attribute>
            </img>
          </xsl:if>
          <xsl:if test="FOTORIGHT"> <!-- second foto to descripe -->
              <img  width="50%" alt="file not found" style="vertical-align:top">
                  <xsl:attribute name="src">
                        <xsl:value-of select="concat($var_picfolder,FOTORIGHT)" />
                  </xsl:attribute>
              </img>
          </xsl:if>
        </p>
          <xsl:if test="FOTO">
          <xsl:variable name="var_foto" select="FOTO" />
          <xsl:variable name="var_pathfoto" select="concat($var_picfolder,$var_foto)" />
            <img alt="file not found" width="75%" >
              <xsl:attribute name="src">
                <xsl:value-of select="$var_pathfoto" />
                  </xsl:attribute>
            </img>
          </xsl:if>

          <xsl:if test="DESCRCOMTEXT"> <!-- language dependent -->
            <xsl:variable name="var_descrcomtext" select="DESCRCOMTEXT" />
            <xsl:copy-of select="document(concat($var_prosacom,'/', $var_descrcomtext))" />
        </xsl:if>

          <xsl:if test="DESCRTYP"> <!-- different from typ 1k/3k -->
            <xsl:variable name="var_descrtyp" select="DESCRTYP" />
            <xsl:copy-of select="document(concat($var_prosatyp,'/', $var_descrtyp))" />
        </xsl:if> 

        <xsl:if test="DESCR"> <!-- Description individual -->
            <xsl:variable name="var_descr" select="DESCR" />
            <xsl:copy-of select="document(concat($var_prosa,'/', $var_descr))" />
        </xsl:if>
      </xsl:if>
      <div style="break-after:page"></div>
    </xsl:for-each>

   <xsl:copy-of select="document(concat($var_prosacom,'/AnhangMET.html'))" />
   <div style="break-after:page"></div>
   <xsl:copy-of select="document(concat($var_prosacom,'/AnhangEFT.html'))" />
   <div style="break-after:page"></div>
  </body>
  </html>
</xsl:template>

</xsl:stylesheet>