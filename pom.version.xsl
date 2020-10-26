<xsl:stylesheet version="2.0" xmlns="http://maven.apache.org/POM/4.0.0" xmlns:pom="http://maven.apache.org/POM/4.0.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    
    <xsl:output method="xml" omit-xml-declaration="yes" indent="yes"/>
    <xsl:strip-space elements="*"/>
    
    <xsl:param name="artifactId" as="string"/>
    <xsl:param name="version"/>
    
    <!--identity template copies everything forward by default-->
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
    </xsl:template>
    
    <xsl:template match="pom:version/text()">
        <xsl:choose>
            <xsl:when test="../../pom:artifactId = $artifactId">
                <xsl:value-of select="$version"/>
            </xsl:when>
            <xsl:otherwise>
                <xsl:copy-of select="."/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    
</xsl:stylesheet>