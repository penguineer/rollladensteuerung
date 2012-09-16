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
<description>Netz39 EagleParts

Library of parts for projects, which are not in the standard libraries of eagle.</description>
<packages>
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
</packages>
<symbols>
<symbol name="JS_05_MN_KT">
<wire x1="-12.7" y1="-5.08" x2="-12.7" y2="7.62" width="0.254" layer="94"/>
<wire x1="-12.7" y1="7.62" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="12.7" y2="7.62" width="0.254" layer="94"/>
<wire x1="12.7" y1="7.62" x2="12.7" y2="-5.08" width="0.254" layer="94"/>
<wire x1="12.7" y1="-5.08" x2="10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="-7.62" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-5.08" x2="-7.62" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-7.62" y1="-5.08" x2="-7.62" y2="0" width="0.254" layer="94"/>
<wire x1="-7.62" y1="0" x2="-10.16" y2="0" width="0.254" layer="94"/>
<wire x1="-10.16" y1="0" x2="-10.16" y2="2.54" width="0.254" layer="94"/>
<wire x1="-10.16" y1="2.54" x2="-7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="-7.62" y1="0" x2="-5.08" y2="0" width="0.254" layer="94"/>
<wire x1="-5.08" y1="0" x2="-5.08" y2="2.54" width="0.254" layer="94"/>
<wire x1="-5.08" y1="2.54" x2="-7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="-7.62" y1="2.54" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="7.62" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="5.08" y1="5.08" x2="7.62" y2="5.08" width="0.254" layer="94"/>
<wire x1="7.62" y1="5.08" x2="7.62" y2="0" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="10.16" y2="0" width="0.254" layer="94"/>
<pin name="P$1" x="-7.62" y="-10.16" length="middle" rot="R90"/>
<pin name="P$2" x="-7.62" y="12.7" length="middle" rot="R270"/>
<pin name="P$3" x="5.08" y="12.7" length="middle" rot="R270"/>
<pin name="P$4" x="10.16" y="-10.16" length="middle" rot="R90"/>
</symbol>
<symbol name="JS_05_N_K">
<wire x1="-12.7" y1="-5.08" x2="-12.7" y2="7.62" width="0.254" layer="94"/>
<wire x1="-12.7" y1="7.62" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="12.7" y2="7.62" width="0.254" layer="94"/>
<wire x1="12.7" y1="7.62" x2="12.7" y2="-5.08" width="0.254" layer="94"/>
<wire x1="12.7" y1="-5.08" x2="10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="2.54" y2="-5.08" width="0.254" layer="94"/>
<wire x1="2.54" y1="-5.08" x2="-7.62" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-5.08" x2="-7.62" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-7.62" y1="-5.08" x2="-7.62" y2="0" width="0.254" layer="94"/>
<wire x1="-7.62" y1="0" x2="-10.16" y2="0" width="0.254" layer="94"/>
<wire x1="-10.16" y1="0" x2="-10.16" y2="2.54" width="0.254" layer="94"/>
<wire x1="-10.16" y1="2.54" x2="-7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="-7.62" y1="0" x2="-5.08" y2="0" width="0.254" layer="94"/>
<wire x1="-5.08" y1="0" x2="-5.08" y2="2.54" width="0.254" layer="94"/>
<wire x1="-5.08" y1="2.54" x2="-7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="-7.62" y1="2.54" x2="-7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="-7.62" y1="7.62" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<wire x1="5.08" y1="7.62" x2="5.08" y2="5.08" width="0.254" layer="94"/>
<wire x1="5.08" y1="5.08" x2="7.62" y2="5.08" width="0.254" layer="94"/>
<wire x1="7.62" y1="5.08" x2="7.62" y2="0" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="10.16" y2="0" width="0.254" layer="94"/>
<wire x1="2.54" y1="-5.08" x2="2.54" y2="0" width="0.254" layer="94"/>
<pin name="P$1" x="-7.62" y="-10.16" length="middle" rot="R90"/>
<pin name="P$2" x="-7.62" y="12.7" length="middle" rot="R270"/>
<pin name="P$3" x="5.08" y="12.7" length="middle" rot="R270"/>
<pin name="P$4" x="2.54" y="-10.16" length="middle" rot="R90"/>
<pin name="P$5" x="10.16" y="-10.16" length="middle" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="JS_05_MN_KT">
<description>Schaltrelais JK 05 MN KT

http://www.reichelt.de/Print-Steckrelais/JS-05-MN-KT/3/index.html?;ACTION=3;LA=446;ARTICLE=79417;GROUPID=3293;artnr=JS+05+MN+KT;SID=12UB1s2H8AAAIAACOTJVc2a4e49fe1644fcd32ae1d6e6d3015b7c</description>
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
<deviceset name="JS_05_N_K">
<description>Schaltrelais JK 05 N K

http://www.reichelt.de/Print-Steckrelais/JS-05-N-K/3//index.html?ACTION=3&amp;GROUPID=3293&amp;ARTICLE=79414&amp;SHOW=1&amp;START=0&amp;OFFSET=16&amp;</description>
<gates>
<gate name="G$1" symbol="JS_05_N_K" x="0" y="0"/>
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
</devicesets>
</library>
<library name="transistor">
<description>&lt;b&gt;Transistors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="TO92">
<description>&lt;b&gt;TO 92&lt;/b&gt;</description>
<wire x1="-2.0946" y1="-1.651" x2="-2.6549" y2="-0.254" width="0.127" layer="21" curve="-32.781"/>
<wire x1="-2.6549" y1="-0.254" x2="-0.7863" y2="2.5485" width="0.127" layer="21" curve="-78.3185"/>
<wire x1="0.7863" y1="2.5484" x2="2.0945" y2="-1.651" width="0.127" layer="21" curve="-111.1"/>
<wire x1="-2.0945" y1="-1.651" x2="2.0945" y2="-1.651" width="0.127" layer="21"/>
<wire x1="-2.2537" y1="-0.254" x2="-0.2863" y2="-0.254" width="0.127" layer="51"/>
<wire x1="-2.6549" y1="-0.254" x2="-2.2537" y2="-0.254" width="0.127" layer="21"/>
<wire x1="-0.2863" y1="-0.254" x2="0.2863" y2="-0.254" width="0.127" layer="21"/>
<wire x1="2.2537" y1="-0.254" x2="2.6549" y2="-0.254" width="0.127" layer="21"/>
<wire x1="0.2863" y1="-0.254" x2="2.2537" y2="-0.254" width="0.127" layer="51"/>
<wire x1="-0.7863" y1="2.5485" x2="0.7863" y2="2.5485" width="0.127" layer="51" curve="-34.2936"/>
<pad name="1" x="1.27" y="0" drill="0.8128" shape="octagon"/>
<pad name="2" x="0" y="1.905" drill="0.8128" shape="octagon"/>
<pad name="3" x="-1.27" y="0" drill="0.8128" shape="octagon"/>
<text x="3.175" y="0.635" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="3.175" y="-1.27" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<text x="-0.635" y="0.635" size="1.27" layer="51" ratio="10">2</text>
<text x="-2.159" y="0" size="1.27" layer="51" ratio="10">3</text>
<text x="1.143" y="0" size="1.27" layer="51" ratio="10">1</text>
</package>
</packages>
<symbols>
<symbol name="NPN">
<wire x1="2.54" y1="2.54" x2="0.508" y2="1.524" width="0.1524" layer="94"/>
<wire x1="1.778" y1="-1.524" x2="2.54" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="1.27" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="1.778" y2="-1.524" width="0.1524" layer="94"/>
<wire x1="1.54" y1="-2.04" x2="0.308" y2="-1.424" width="0.1524" layer="94"/>
<wire x1="1.524" y1="-2.413" x2="2.286" y2="-2.413" width="0.254" layer="94"/>
<wire x1="2.286" y1="-2.413" x2="1.778" y2="-1.778" width="0.254" layer="94"/>
<wire x1="1.778" y1="-1.778" x2="1.524" y2="-2.286" width="0.254" layer="94"/>
<wire x1="1.524" y1="-2.286" x2="1.905" y2="-2.286" width="0.254" layer="94"/>
<wire x1="1.905" y1="-2.286" x2="1.778" y2="-2.032" width="0.254" layer="94"/>
<text x="-10.16" y="7.62" size="1.778" layer="95">&gt;NAME</text>
<text x="-10.16" y="5.08" size="1.778" layer="96">&gt;VALUE</text>
<rectangle x1="-0.254" y1="-2.54" x2="0.508" y2="2.54" layer="94"/>
<pin name="B" x="-2.54" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
<pin name="E" x="2.54" y="-5.08" visible="off" length="short" direction="pas" swaplevel="3" rot="R90"/>
<pin name="C" x="2.54" y="5.08" visible="off" length="short" direction="pas" swaplevel="2" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="2N3565" prefix="T">
<description>&lt;b&gt;NPN TRANSISTOR&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="NPN" x="-2.54" y="0"/>
</gates>
<devices>
<device name="" package="TO92">
<connects>
<connect gate="G$1" pin="B" pad="2"/>
<connect gate="G$1" pin="C" pad="1"/>
<connect gate="G$1" pin="E" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="docu-dummy">
<description>Dummy symbols</description>
<packages>
</packages>
<symbols>
<symbol name="RESISTOR">
<wire x1="-2.54" y1="-0.889" x2="2.54" y2="-0.889" width="0.254" layer="94"/>
<wire x1="2.54" y1="0.889" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
<wire x1="2.54" y1="-0.889" x2="2.54" y2="0" width="0.254" layer="94"/>
<wire x1="2.54" y1="0" x2="2.54" y2="0.889" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-0.889" x2="-2.54" y2="0" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
<wire x1="-5.08" y1="0" x2="-2.54" y2="0" width="0.1524" layer="94"/>
<wire x1="2.54" y1="0" x2="5.08" y2="0" width="0.1524" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="R" prefix="R">
<description>&lt;b&gt;RESISTOR&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-samtec">
<description>&lt;b&gt;Samtec Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="SSW-102-02-S-S">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<wire x1="-2.669" y1="1.155" x2="2.669" y2="1.155" width="0.2032" layer="21"/>
<wire x1="2.669" y1="1.155" x2="2.669" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="2.669" y1="-1.155" x2="-2.669" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="-2.669" y1="-1.155" x2="-2.669" y2="1.155" width="0.2032" layer="21"/>
<wire x1="-2.015" y1="0.755" x2="-0.515" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-0.515" y1="0.755" x2="-0.515" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-0.515" y1="-0.745" x2="-2.015" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-2.015" y1="-0.745" x2="-2.015" y2="0.755" width="0.2032" layer="51"/>
<wire x1="0.525" y1="0.755" x2="2.025" y2="0.755" width="0.2032" layer="51"/>
<wire x1="2.025" y1="0.755" x2="2.025" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="2.025" y1="-0.745" x2="0.525" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="0.525" y1="-0.745" x2="0.525" y2="0.755" width="0.2032" layer="51"/>
<pad name="1" x="-1.27" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="2" x="1.27" y="0" drill="1" diameter="1.5" shape="octagon"/>
<text x="-1.778" y="-3.048" size="1.6764" layer="21" font="vector">1</text>
<text x="-3.175" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="4.445" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
</package>
<package name="SSW-102-02-S-S-RA">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<wire x1="-2.669" y1="-8.396" x2="2.669" y2="-8.396" width="0.2032" layer="21"/>
<wire x1="2.669" y1="-8.396" x2="2.669" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="2.669" y1="-0.106" x2="-2.669" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="-2.669" y1="-0.106" x2="-2.669" y2="-8.396" width="0.2032" layer="21"/>
<pad name="1" x="-1.27" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="2" x="1.27" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<text x="-1.865" y="-7.65" size="1.6764" layer="21" font="vector">1</text>
<text x="-3.175" y="-7.62" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="4.445" y="-7.62" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.524" y1="0" x2="-1.016" y2="1.778" layer="51"/>
<rectangle x1="1.016" y1="0" x2="1.524" y2="1.778" layer="51"/>
</package>
<package name="SSW-103-02-S-S">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<wire x1="-3.939" y1="1.155" x2="3.939" y2="1.155" width="0.2032" layer="21"/>
<wire x1="3.939" y1="1.155" x2="3.939" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="3.939" y1="-1.155" x2="-3.939" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="-3.939" y1="-1.155" x2="-3.939" y2="1.155" width="0.2032" layer="21"/>
<wire x1="-3.285" y1="0.755" x2="-1.785" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-1.785" y1="0.755" x2="-1.785" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-1.785" y1="-0.745" x2="-3.285" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-3.285" y1="-0.745" x2="-3.285" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-0.745" y1="0.755" x2="0.755" y2="0.755" width="0.2032" layer="51"/>
<wire x1="0.755" y1="0.755" x2="0.755" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="0.755" y1="-0.745" x2="-0.745" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-0.745" y1="-0.745" x2="-0.745" y2="0.755" width="0.2032" layer="51"/>
<wire x1="1.795" y1="0.755" x2="3.295" y2="0.755" width="0.2032" layer="51"/>
<wire x1="3.295" y1="0.755" x2="3.295" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="3.295" y1="-0.745" x2="1.795" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="1.795" y1="-0.745" x2="1.795" y2="0.755" width="0.2032" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="2" x="0" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="3" x="2.54" y="0" drill="1" diameter="1.5" shape="octagon"/>
<text x="-3.048" y="-3.048" size="1.6764" layer="21" font="vector">1</text>
<text x="-4.445" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="5.715" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
</package>
<package name="SSW-103-02-S-S-RA">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<wire x1="-3.939" y1="-8.396" x2="3.939" y2="-8.396" width="0.2032" layer="21"/>
<wire x1="3.939" y1="-8.396" x2="3.939" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="3.939" y1="-0.106" x2="-3.939" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="-3.939" y1="-0.106" x2="-3.939" y2="-8.396" width="0.2032" layer="21"/>
<pad name="1" x="-2.54" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="2" x="0" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="3" x="2.54" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<text x="-3.135" y="-7.65" size="1.6764" layer="21" font="vector">1</text>
<text x="-4.445" y="-7.62" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="5.715" y="-7.62" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-2.794" y1="0" x2="-2.286" y2="1.778" layer="51"/>
<rectangle x1="-0.254" y1="0" x2="0.254" y2="1.778" layer="51"/>
<rectangle x1="2.286" y1="0" x2="2.794" y2="1.778" layer="51"/>
</package>
<package name="SSW-109-02-S-S">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<wire x1="-11.559" y1="1.155" x2="11.559" y2="1.155" width="0.2032" layer="21"/>
<wire x1="11.559" y1="1.155" x2="11.559" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="11.559" y1="-1.155" x2="-11.559" y2="-1.155" width="0.2032" layer="21"/>
<wire x1="-11.559" y1="-1.155" x2="-11.559" y2="1.155" width="0.2032" layer="21"/>
<wire x1="-10.905" y1="0.755" x2="-9.405" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-9.405" y1="0.755" x2="-9.405" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-9.405" y1="-0.745" x2="-10.905" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-10.905" y1="-0.745" x2="-10.905" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-8.365" y1="0.755" x2="-6.865" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-6.865" y1="0.755" x2="-6.865" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-6.865" y1="-0.745" x2="-8.365" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-8.365" y1="-0.745" x2="-8.365" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-5.825" y1="0.755" x2="-4.325" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-4.325" y1="0.755" x2="-4.325" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-4.325" y1="-0.745" x2="-5.825" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-5.825" y1="-0.745" x2="-5.825" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-3.285" y1="0.755" x2="-1.785" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-1.785" y1="0.755" x2="-1.785" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-1.785" y1="-0.745" x2="-3.285" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-3.285" y1="-0.745" x2="-3.285" y2="0.755" width="0.2032" layer="51"/>
<wire x1="-0.745" y1="0.755" x2="0.755" y2="0.755" width="0.2032" layer="51"/>
<wire x1="0.755" y1="0.755" x2="0.755" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="0.755" y1="-0.745" x2="-0.745" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="-0.745" y1="-0.745" x2="-0.745" y2="0.755" width="0.2032" layer="51"/>
<wire x1="1.795" y1="0.755" x2="3.295" y2="0.755" width="0.2032" layer="51"/>
<wire x1="3.295" y1="0.755" x2="3.295" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="3.295" y1="-0.745" x2="1.795" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="1.795" y1="-0.745" x2="1.795" y2="0.755" width="0.2032" layer="51"/>
<wire x1="4.335" y1="0.755" x2="5.835" y2="0.755" width="0.2032" layer="51"/>
<wire x1="5.835" y1="0.755" x2="5.835" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="5.835" y1="-0.745" x2="4.335" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="4.335" y1="-0.745" x2="4.335" y2="0.755" width="0.2032" layer="51"/>
<wire x1="6.875" y1="0.755" x2="8.375" y2="0.755" width="0.2032" layer="51"/>
<wire x1="8.375" y1="0.755" x2="8.375" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="8.375" y1="-0.745" x2="6.875" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="6.875" y1="-0.745" x2="6.875" y2="0.755" width="0.2032" layer="51"/>
<wire x1="9.415" y1="0.755" x2="10.915" y2="0.755" width="0.2032" layer="51"/>
<wire x1="10.915" y1="0.755" x2="10.915" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="10.915" y1="-0.745" x2="9.415" y2="-0.745" width="0.2032" layer="51"/>
<wire x1="9.415" y1="-0.745" x2="9.415" y2="0.755" width="0.2032" layer="51"/>
<pad name="1" x="-10.16" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="2" x="-7.62" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="3" x="-5.08" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="4" x="-2.54" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="5" x="0" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="6" x="2.54" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="7" x="5.08" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="8" x="7.62" y="0" drill="1" diameter="1.5" shape="octagon"/>
<pad name="9" x="10.16" y="0" drill="1" diameter="1.5" shape="octagon"/>
<text x="-10.668" y="-3.048" size="1.6764" layer="21" font="vector">1</text>
<text x="-12.065" y="-1.27" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="13.335" y="-1.27" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
</package>
<package name="SSW-109-02-S-S-RA">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<wire x1="-11.559" y1="-8.396" x2="11.559" y2="-8.396" width="0.2032" layer="21"/>
<wire x1="11.559" y1="-8.396" x2="11.559" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="11.559" y1="-0.106" x2="-11.559" y2="-0.106" width="0.2032" layer="21"/>
<wire x1="-11.559" y1="-0.106" x2="-11.559" y2="-8.396" width="0.2032" layer="21"/>
<pad name="1" x="-10.16" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="2" x="-7.62" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="3" x="-5.08" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="4" x="-2.54" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="5" x="0" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="6" x="2.54" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="7" x="5.08" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="8" x="7.62" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<pad name="9" x="10.16" y="1.524" drill="1" diameter="1.5" shape="octagon"/>
<text x="-10.755" y="-7.65" size="1.6764" layer="21" font="vector">1</text>
<text x="-12.065" y="-7.62" size="1.27" layer="25" rot="R90">&gt;NAME</text>
<text x="13.335" y="-7.62" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-10.414" y1="0" x2="-9.906" y2="1.778" layer="51"/>
<rectangle x1="-7.874" y1="0" x2="-7.366" y2="1.778" layer="51"/>
<rectangle x1="-5.334" y1="0" x2="-4.826" y2="1.778" layer="51"/>
<rectangle x1="-2.794" y1="0" x2="-2.286" y2="1.778" layer="51"/>
<rectangle x1="-0.254" y1="0" x2="0.254" y2="1.778" layer="51"/>
<rectangle x1="2.286" y1="0" x2="2.794" y2="1.778" layer="51"/>
<rectangle x1="4.826" y1="0" x2="5.334" y2="1.778" layer="51"/>
<rectangle x1="7.366" y1="0" x2="7.874" y2="1.778" layer="51"/>
<rectangle x1="9.906" y1="0" x2="10.414" y2="1.778" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="FPINV">
<wire x1="-1.778" y1="0.508" x2="0" y2="0.508" width="0.254" layer="94"/>
<wire x1="0" y1="0.508" x2="0" y2="-0.508" width="0.254" layer="94"/>
<wire x1="0" y1="-0.508" x2="-1.778" y2="-0.508" width="0.254" layer="94"/>
<text x="-2.54" y="2.54" size="1.778" layer="96">&gt;VALUE</text>
<text x="-3.048" y="0.762" size="1.524" layer="95" rot="R180">&gt;NAME</text>
<pin name="1" x="2.54" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
<symbol name="FPIN">
<wire x1="-1.778" y1="0.508" x2="0" y2="0.508" width="0.254" layer="94"/>
<wire x1="0" y1="0.508" x2="0" y2="-0.508" width="0.254" layer="94"/>
<wire x1="0" y1="-0.508" x2="-1.778" y2="-0.508" width="0.254" layer="94"/>
<text x="-3.048" y="0.762" size="1.524" layer="95" rot="R180">&gt;NAME</text>
<pin name="1" x="2.54" y="0" visible="off" length="short" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SSW-102-02-S-S" prefix="X">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<gates>
<gate name="-1" symbol="FPINV" x="0" y="0" addlevel="always"/>
<gate name="-2" symbol="FPIN" x="0" y="-2.54" addlevel="always"/>
</gates>
<devices>
<device name="" package="SSW-102-02-S-S">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="SSW-102-02-S-S" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="11P9351" constant="no"/>
</technology>
</technologies>
</device>
<device name="-RA" package="SSW-102-02-S-S-RA">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="SSW-102-02-S-S-RA" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="11P9352" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SSW-103-02-S-S" prefix="X">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<gates>
<gate name="-1" symbol="FPINV" x="0" y="2.54" addlevel="always"/>
<gate name="-2" symbol="FPIN" x="0" y="0" addlevel="always"/>
<gate name="-3" symbol="FPIN" x="0" y="-2.54" addlevel="always"/>
</gates>
<devices>
<device name="" package="SSW-103-02-S-S">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
<connect gate="-3" pin="1" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="SSW-103-02-S-S" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="11P9367" constant="no"/>
</technology>
</technologies>
</device>
<device name="-RA" package="SSW-103-02-S-S-RA">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
<connect gate="-3" pin="1" pad="3"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="SSW-103-02-S-S-RA" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="11P9368" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SSW-109-02-S-S" prefix="X">
<description>&lt;b&gt;THROUGH-HOLE .025" SQ POST SOCKET&lt;/b&gt;&lt;p&gt;
Source: Samtec SSW.pdf</description>
<gates>
<gate name="-1" symbol="FPINV" x="0" y="10.16" addlevel="always"/>
<gate name="-2" symbol="FPIN" x="0" y="7.62" addlevel="always"/>
<gate name="-3" symbol="FPIN" x="0" y="5.08" addlevel="always"/>
<gate name="-4" symbol="FPIN" x="0" y="2.54" addlevel="always"/>
<gate name="-5" symbol="FPIN" x="0" y="0" addlevel="always"/>
<gate name="-6" symbol="FPIN" x="0" y="-2.54" addlevel="always"/>
<gate name="-7" symbol="FPIN" x="0" y="-5.08" addlevel="always"/>
<gate name="-8" symbol="FPIN" x="0" y="-7.62" addlevel="always"/>
<gate name="-9" symbol="FPIN" x="0" y="-10.16" addlevel="always"/>
</gates>
<devices>
<device name="" package="SSW-109-02-S-S">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
<connect gate="-3" pin="1" pad="3"/>
<connect gate="-4" pin="1" pad="4"/>
<connect gate="-5" pin="1" pad="5"/>
<connect gate="-6" pin="1" pad="6"/>
<connect gate="-7" pin="1" pad="7"/>
<connect gate="-8" pin="1" pad="8"/>
<connect gate="-9" pin="1" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="SSW-109-02-S-S" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="11P9465" constant="no"/>
</technology>
</technologies>
</device>
<device name="-RA" package="SSW-109-02-S-S-RA">
<connects>
<connect gate="-1" pin="1" pad="1"/>
<connect gate="-2" pin="1" pad="2"/>
<connect gate="-3" pin="1" pad="3"/>
<connect gate="-4" pin="1" pad="4"/>
<connect gate="-5" pin="1" pad="5"/>
<connect gate="-6" pin="1" pad="6"/>
<connect gate="-7" pin="1" pad="7"/>
<connect gate="-8" pin="1" pad="8"/>
<connect gate="-9" pin="1" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="SSW-109-02-S-S-RA" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="11P9466" constant="no"/>
</technology>
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
<part name="U$1" library="n39" deviceset="JS_05_MN_KT" device=""/>
<part name="U$2" library="n39" deviceset="JS_05_N_K" device=""/>
<part name="T1" library="transistor" deviceset="2N3565" device=""/>
<part name="R1" library="docu-dummy" deviceset="R" device=""/>
<part name="D2" library="diode" deviceset="1N4004" device=""/>
<part name="T2" library="transistor" deviceset="2N3565" device=""/>
<part name="R2" library="docu-dummy" deviceset="R" device=""/>
<part name="X1" library="con-samtec" deviceset="SSW-102-02-S-S" device="-RA"/>
<part name="X3" library="con-samtec" deviceset="SSW-103-02-S-S" device="-RA"/>
<part name="X4" library="con-samtec" deviceset="SSW-109-02-S-S" device="-RA"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="D1" gate="1" x="71.12" y="53.34"/>
<instance part="U$1" gate="G$1" x="93.98" y="45.72"/>
<instance part="U$2" gate="G$1" x="93.98" y="20.32"/>
<instance part="T1" gate="G$1" x="60.96" y="48.26"/>
<instance part="R1" gate="G$1" x="50.8" y="48.26"/>
<instance part="D2" gate="1" x="71.12" y="27.94"/>
<instance part="T2" gate="G$1" x="60.96" y="22.86"/>
<instance part="R2" gate="G$1" x="50.8" y="22.86"/>
<instance part="X1" gate="-1" x="38.1" y="71.12"/>
<instance part="X1" gate="-2" x="38.1" y="68.58"/>
<instance part="X3" gate="-1" x="121.92" y="68.58" rot="R180"/>
<instance part="X3" gate="-2" x="121.92" y="71.12" rot="R180"/>
<instance part="X3" gate="-3" x="121.92" y="73.66" rot="R180"/>
<instance part="X4" gate="-1" x="17.78" y="48.26"/>
<instance part="X4" gate="-2" x="17.78" y="45.72"/>
<instance part="X4" gate="-3" x="17.78" y="43.18"/>
<instance part="X4" gate="-4" x="17.78" y="40.64"/>
<instance part="X4" gate="-5" x="17.78" y="38.1"/>
<instance part="X4" gate="-6" x="17.78" y="35.56"/>
<instance part="X4" gate="-7" x="17.78" y="33.02"/>
<instance part="X4" gate="-8" x="17.78" y="30.48"/>
<instance part="X4" gate="-9" x="17.78" y="27.94"/>
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
