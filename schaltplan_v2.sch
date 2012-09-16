<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="yes" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="6" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="diode">
<description>&lt;b&gt;Diodes&lt;/b&gt;&lt;p&gt;
Based on the following sources:
&lt;ul&gt;
&lt;li&gt;Motorola : www.onsemi.com
&lt;li&gt;Fairchild : www.fairchildsemi.com
&lt;li&gt;Philips : www.semiconductors.com
&lt;li&gt;Vishay : www.vishay.de
&lt;/ul&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="DO41-10">
<description>&lt;B&gt;DIODE&lt;/B&gt;&lt;p&gt;
diameter 2.54 mm, horizontal, grid 10.16 mm</description>
<wire x1="2.032" y1="-1.27" x2="-2.032" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.032" y1="-1.27" x2="2.032" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="1.27" x2="2.032" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.032" y1="1.27" x2="-2.032" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.08" y1="0" x2="4.064" y2="0" width="0.762" layer="51"/>
<wire x1="-5.08" y1="0" x2="-4.064" y2="0" width="0.762" layer="51"/>
<wire x1="-0.635" y1="0" x2="0" y2="0" width="0.1524" layer="21"/>
<wire x1="1.016" y1="0.635" x2="1.016" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.016" y1="-0.635" x2="0" y2="0" width="0.1524" layer="21"/>
<wire x1="0" y1="0" x2="1.524" y2="0" width="0.1524" layer="21"/>
<wire x1="0" y1="0" x2="1.016" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0" y2="0" width="0.1524" layer="21"/>
<wire x1="0" y1="0" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<pad name="A" x="5.08" y="0" drill="1.1176"/>
<pad name="C" x="-5.08" y="0" drill="1.1176"/>
<text x="-2.032" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.032" y="-2.794" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-1.27" x2="-1.143" y2="1.27" layer="21"/>
<rectangle x1="2.032" y1="-0.381" x2="3.937" y2="0.381" layer="21"/>
<rectangle x1="-3.937" y1="-0.381" x2="-2.032" y2="0.381" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="D">
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="-1.27" y2="1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="1.27" y2="-1.27" width="0.254" layer="94"/>
<text x="2.54" y="0.4826" size="1.778" layer="95">&gt;NAME</text>
<text x="2.54" y="-2.3114" size="1.778" layer="96">&gt;VALUE</text>
<pin name="A" x="-2.54" y="0" visible="off" length="short" direction="pas"/>
<pin name="C" x="2.54" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="1N4004" prefix="D">
<description>&lt;B&gt;DIODE&lt;/B&gt;&lt;p&gt;
general purpose rectifier, 1 A</description>
<gates>
<gate name="1" symbol="D" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DO41-10">
<connects>
<connect gate="1" pin="A" pad="A"/>
<connect gate="1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="n39">
<packages>
<package name="JS_05_N_K">
<wire x1="0" y1="0" x2="0" y2="10" width="0.127" layer="21"/>
<wire x1="0" y1="10" x2="29" y2="10" width="0.127" layer="21"/>
<wire x1="29" y1="10" x2="29" y2="0" width="0.127" layer="21"/>
<wire x1="29" y1="0" x2="0" y2="0" width="0.127" layer="21"/>
<pad name="P$2" x="2.1" y="8.81" drill="0.8"/>
<pad name="P$1" x="2.1" y="1.19" drill="0.8"/>
<pad name="P$3" x="24.2" y="8.81" drill="0.8"/>
<pad name="P$4" x="22.6" y="1.19" drill="0.8"/>
<pad name="P$5" x="25.8" y="1.19" drill="0.8"/>
</package>
<package name="JS_05_MN_KT">
<wire x1="0" y1="0" x2="0" y2="10" width="0.127" layer="21"/>
<wire x1="0" y1="10" x2="29" y2="10" width="0.127" layer="21"/>
<wire x1="29" y1="10" x2="29" y2="0" width="0.127" layer="21"/>
<wire x1="29" y1="0" x2="0" y2="0" width="0.127" layer="21"/>
<pad name="P$1" x="2.1" y="1.19" drill="0.8"/>
<pad name="P$2" x="2.1" y="8.71" drill="0.8"/>
<pad name="P$3" x="21" y="8.71" drill="0.8"/>
<pad name="P$4" x="26" y="1" drill="0.8"/>
</package>
</packages>
<symbols>
<symbol name="RELAIS">
<pin name="P$1" x="-35.56" y="25.4" length="middle" rot="R270"/>
<pin name="P$2" x="-35.56" y="0" length="middle" rot="R90"/>
<pin name="P$3" x="-12.7" y="25.4" length="middle" rot="R270"/>
<pin name="P$4" x="-17.78" y="0" length="middle" rot="R90"/>
<pin name="P$5" x="-7.62" y="0" length="middle" rot="R90"/>
<wire x1="-43.18" y1="20.32" x2="-43.18" y2="5.08" width="0.254" layer="94"/>
<wire x1="-43.18" y1="5.08" x2="-35.56" y2="5.08" width="0.254" layer="94"/>
<wire x1="-35.56" y1="5.08" x2="-7.62" y2="5.08" width="0.254" layer="94"/>
<wire x1="-7.62" y1="5.08" x2="-5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="-5.08" y1="5.08" x2="-5.08" y2="20.32" width="0.254" layer="94"/>
<wire x1="-5.08" y1="20.32" x2="-12.7" y2="20.32" width="0.254" layer="94"/>
<wire x1="-12.7" y1="20.32" x2="-35.56" y2="20.32" width="0.254" layer="94"/>
<wire x1="-35.56" y1="20.32" x2="-43.18" y2="20.32" width="0.254" layer="94"/>
<wire x1="-40.64" y1="15.24" x2="-40.64" y2="10.16" width="0.254" layer="94"/>
<wire x1="-40.64" y1="10.16" x2="-35.56" y2="10.16" width="0.254" layer="94"/>
<wire x1="-35.56" y1="10.16" x2="-30.48" y2="10.16" width="0.254" layer="94"/>
<wire x1="-30.48" y1="10.16" x2="-30.48" y2="15.24" width="0.254" layer="94"/>
<wire x1="-30.48" y1="15.24" x2="-35.56" y2="15.24" width="0.254" layer="94"/>
<wire x1="-35.56" y1="15.24" x2="-40.64" y2="15.24" width="0.254" layer="94"/>
<wire x1="-40.64" y1="15.24" x2="-35.56" y2="15.24" width="0.254" layer="94"/>
<wire x1="-35.56" y1="15.24" x2="-35.56" y2="20.32" width="0.254" layer="94"/>
<wire x1="-35.56" y1="20.32" x2="-35.56" y2="15.24" width="0.254" layer="94"/>
<wire x1="-35.56" y1="15.24" x2="-30.48" y2="15.24" width="0.254" layer="94"/>
<wire x1="-30.48" y1="15.24" x2="-30.48" y2="10.16" width="0.254" layer="94"/>
<wire x1="-30.48" y1="10.16" x2="-35.56" y2="10.16" width="0.254" layer="94"/>
<wire x1="-35.56" y1="10.16" x2="-35.56" y2="5.08" width="0.254" layer="94"/>
<wire x1="-35.56" y1="5.08" x2="-17.78" y2="5.08" width="0.254" layer="94"/>
<wire x1="-17.78" y1="5.08" x2="-17.78" y2="10.16" width="0.254" layer="94"/>
<wire x1="-7.62" y1="5.08" x2="-7.62" y2="10.16" width="0.254" layer="94"/>
<wire x1="-12.7" y1="20.32" x2="-12.7" y2="15.24" width="0.254" layer="94"/>
<wire x1="-12.7" y1="15.24" x2="-15.24" y2="15.24" width="0.254" layer="94"/>
<wire x1="-15.24" y1="15.24" x2="-15.24" y2="10.16" width="0.254" layer="94"/>
</symbol>
<symbol name="JS_05_MN_KT">
<wire x1="0" y1="0" x2="0" y2="12.7" width="0.254" layer="94"/>
<wire x1="0" y1="12.7" x2="5.08" y2="12.7" width="0.254" layer="94"/>
<wire x1="5.08" y1="12.7" x2="33.02" y2="12.7" width="0.254" layer="94"/>
<wire x1="33.02" y1="12.7" x2="33.02" y2="0" width="0.254" layer="94"/>
<wire x1="33.02" y1="0" x2="30.48" y2="0" width="0.254" layer="94"/>
<wire x1="30.48" y1="0" x2="22.86" y2="0" width="0.254" layer="94"/>
<wire x1="22.86" y1="0" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="5.08" y2="0" width="0.254" layer="94"/>
<wire x1="5.08" y1="0" x2="5.08" y2="2.54" width="0.254" layer="94"/>
<wire x1="5.08" y1="2.54" x2="5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="5.08" y1="5.08" x2="2.54" y2="5.08" width="0.254" layer="94"/>
<wire x1="2.54" y1="5.08" x2="2.54" y2="7.62" width="0.254" layer="94"/>
<wire x1="2.54" y1="7.62" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="7.62" y1="7.62" x2="7.62" y2="5.08" width="0.254" layer="94"/>
<wire x1="7.62" y1="5.08" x2="5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="5.08" y1="5.08" x2="7.62" y2="5.08" width="0.254" layer="94"/>
<wire x1="7.62" y1="5.08" x2="7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="7.62" y1="7.62" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="5.08" y2="12.7" width="0.254" layer="94"/>
<wire x1="5.08" y1="12.7" x2="25.4" y2="12.7" width="0.254" layer="94"/>
<wire x1="25.4" y1="12.7" x2="25.4" y2="10.16" width="0.254" layer="94"/>
<wire x1="25.4" y1="10.16" x2="27.94" y2="10.16" width="0.254" layer="94"/>
<wire x1="27.94" y1="10.16" x2="27.94" y2="5.08" width="0.254" layer="94"/>
<wire x1="30.48" y1="0" x2="30.48" y2="5.08" width="0.254" layer="94"/>
<pin name="P$1" x="5.08" y="-5.08" length="middle" rot="R90"/>
<pin name="P$2" x="5.08" y="17.78" length="middle" rot="R270"/>
<pin name="P$3" x="25.4" y="17.78" length="middle" rot="R270"/>
<pin name="P$4" x="30.48" y="-5.08" length="middle" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="JS_05_N_K">
<description>Schaltrelais JK 05 N K

http://www.reichelt.de/Print-Steckrelais/JS-05-N-K/3//index.html?ACTION=3&amp;GROUPID=3293&amp;ARTICLE=79414&amp;SHOW=1&amp;START=0&amp;OFFSET=16&amp;</description>
<gates>
<gate name="G$1" symbol="RELAIS" x="25.4" y="-2.54"/>
</gates>
<devices>
<device name="" package="JS_05_N_K">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
<connect gate="G$1" pin="P$3" pad="P$3"/>
<connect gate="G$1" pin="P$4" pad="P$4"/>
<connect gate="G$1" pin="P$5" pad="P$5"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JS_05_MN_KT">
<gates>
<gate name="G$1" symbol="JS_05_MN_KT" x="0" y="0"/>
</gates>
<devices>
<device name="" package="JS_05_MN_KT">
<connects>
<connect gate="G$1" pin="P$1" pad="P$1"/>
<connect gate="G$1" pin="P$2" pad="P$2"/>
<connect gate="G$1" pin="P$3" pad="P$3"/>
<connect gate="G$1" pin="P$4" pad="P$4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="D1" library="diode" deviceset="1N4004" device=""/>
<part name="D2" library="diode" deviceset="1N4004" device=""/>
<part name="U$1" library="n39" deviceset="JS_05_N_K" device=""/>
<part name="U$2" library="n39" deviceset="JS_05_MN_KT" device=""/>
<part name="U$3" library="n39" deviceset="JS_05_MN_KT" device=""/>
<part name="U$4" library="n39" deviceset="JS_05_MN_KT" device=""/>
<part name="U$5" library="n39" deviceset="JS_05_MN_KT" device=""/>
<part name="U$6" library="n39" deviceset="JS_05_N_K" device=""/>
<part name="U$7" library="n39" deviceset="JS_05_N_K" device=""/>
<part name="U$8" library="n39" deviceset="JS_05_N_K" device=""/>
<part name="D3" library="diode" deviceset="1N4004" device=""/>
<part name="D4" library="diode" deviceset="1N4004" device=""/>
<part name="D5" library="diode" deviceset="1N4004" device=""/>
<part name="D6" library="diode" deviceset="1N4004" device=""/>
<part name="D7" library="diode" deviceset="1N4004" device=""/>
<part name="D8" library="diode" deviceset="1N4004" device=""/>
<part name="D9" library="diode" deviceset="1N4004" device=""/>
<part name="D10" library="diode" deviceset="1N4004" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="D1" gate="1" x="38.1" y="58.42" rot="R90"/>
<instance part="D2" gate="1" x="38.1" y="38.1" rot="R90"/>
<instance part="U$1" gate="G$1" x="96.52" y="20.32"/>
<instance part="U$2" gate="G$1" x="53.34" y="48.26"/>
<instance part="U$3" gate="G$1" x="96.52" y="48.26"/>
<instance part="U$4" gate="G$1" x="134.62" y="48.26"/>
<instance part="U$5" gate="G$1" x="134.62" y="30.48"/>
<instance part="U$6" gate="G$1" x="96.52" y="2.54"/>
<instance part="U$7" gate="G$1" x="137.16" y="20.32"/>
<instance part="U$8" gate="G$1" x="137.16" y="2.54"/>
<instance part="D3" gate="1" x="38.1" y="27.94" rot="R90"/>
<instance part="D4" gate="1" x="38.1" y="20.32" rot="R90"/>
<instance part="D5" gate="1" x="33.02" y="27.94" rot="R90"/>
<instance part="D6" gate="1" x="33.02" y="38.1" rot="R90"/>
<instance part="D7" gate="1" x="33.02" y="58.42" rot="R90"/>
<instance part="D8" gate="1" x="22.86" y="30.48" rot="R90"/>
<instance part="D9" gate="1" x="25.4" y="45.72" rot="R90"/>
<instance part="D10" gate="1" x="33.02" y="20.32" rot="R90"/>
</instances>
<busses>
</busses>
<nets>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
