<?php
	require("/home/atchoo/php/logging.php");
	dolog();
	print("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "DTD/xhtml1-strict.dtd">
<html><head><title>gds2pov</title>
<link rel="stylesheet" href="style.css" type="text/css" />
</head><body>

<div>
<h1>gds2pov v0.7.1</h1>
<h2>What is gds2pov?</h2>
<p>GDS2POV is a program to take a GDS2 layout file and output a POV-Ray scene 
description file of the GDS2 data. This allows the creation of attractive 3D pictures of a layout.</p>
<p>Look at an example (click to enlarge):</p>
<p>
<a href="images/example_2D.png"><img src="images/thumbs/t_example_2D.png" width="100" height="76" alt="2D Example"/></a> 
<a href="images/example_3D.png"><img src="images/thumbs/t_example_3D.png" width="100" height="76" alt="3D Example"/></a> 
</p>
<p>POV-Ray is a completely free, cross platform ray tracer. It can be obtained 
from <a href="http://www.povray.org/">http://www.povray.org/</a></p>

<h2>Download</h2>
<p>gds2pov is currently available in binary format for Windows (x86), Linux (x86, x86_64), Solaris (Sparc), FreeBSD (x86) and HP-UX (PA-RISC). It should compile on just about anything though.
I don't have access to other types of machines. If you want it for your machine, get in touch.</p>

<ul>
<li><a href="files/gds2pov_win32_0.7.1.zip">Windows</a> (96.9KB)</li>
<li><a href="files/gds2pov_lnx86_0.7.1.tar.gz">Linux x86</a> (73KB)</li>
<li><a href="files/gds2pov_lnx86_64_0.7.1.tar.gz">Linux x86_64</a> (74.9KB)</li>
<li><a href="files/gds2pov_solsparc32_0.7.1.tar.gz">Solaris Sparc (32 bit executable)</a> (75.6KB)</li>
<li><a href="files/gds2pov_solsparc64_0.7.1.tar.gz">Solaris Sparc (64 bit executable)</a> (77.5KB)</li>
<li><a href="files/gds2pov_hpux_pa_risc_0.7.1.tar.gz">HP-UX PA-RISC</a> (79.1KB)</li>
<li><a href="files/gds2pov_fbsd_x86_0.7.1.tar.gz">FreeBSD x86</a> (73.1KB)</li>
<!-- <li><a href="files/gds2pov__0.7.tar.gz"></a> (KB)</li> -->
</ul>

<h2>License</h2>
<p>You may distribute and use this binary package as you desire as long as 
you only distribute it intact. The author accepts no responsibility for 
loss or damage to any GDS2 or other data used with this program.</p>

<h2>What about the source?</h2>
<p>I haven't quite decided how to license the source code yet. It still needs some work anyway... I will definitely release the source under a Free license 
at some point.</p>

<h2>More Examples</h2>
<p>Please forgive - these images have been resized and saved as jpg to save on file size. It hasn't done them much good!</p>
<p>
<a href="images/look_down.jpg"><img src="images/thumbs/t_look_down.jpg" width="101" height="76" alt="Look Down"/></a> 
<a href="images/fullexample.jpg"><img src="images/thumbs/t_fullexample.jpg" width="101" height="78" alt="Full Example"/></a> 
<a href="images/complex.jpg"><img src="images/thumbs/t_complex.jpg" width="101" height="76" alt="Complex Example"/></a> 
</p>

<h2>Changes</h2>
<p>Since 0.7:</p>
<ul>
<li>Fixed command line parsing problem. Thanks to S. Lin for spotting the problem.</li>
</ul>
<p>Since 0.6:</p>
<ul>
<li>Added option to specify the top cell name as suggested by Markus Dorn.</li>
<li>Added option to output a bounding box the same size and shape as the layout 
  instead of the layout. This renders almost instantaneously so can be used to
  place the camera more effectively.</li>
<li>Empty cells no longer break the boundary detection.</li>
<li>Fixed bug causing the wrong cell to be selected in boundary calculation which was leading to crashes in certain situations (Guenter Grau).</li>
</ul>
<p>Since 0.5:</p>
<ul>
<li>Added verbose output option for helping debug problems with gds2 files.</li>
<li>Default font used is crystal.ttf (supplied with POV-Ray) and font is definable in config.txt</li>
<li>Invalid characters in structure names are replaced with _. Thanks to Guenter Grau for highlighting this problem.</li>
<li>Support for offset and variable paths added. Octagonal paths are treated as offset paths.</li>
<li>XY records for unsupported elements (BOX, NODE) will now be parsed completely so the whole file can be parsed.</li>
<li>If Datatype is not specified in a layer definition in the process file, the 
  layer definition will be used for all datatypes in that layer. Thanks to 
  Guenter Grau for this suggestion.</li>
</ul>
<h2>Contact</h2>
<p>gds2pov was written by Roger Light. Questions, bug reports, suggestions 
or requests for new features (especially if they concern the incomplete 
support for the GDS2 format) are very welcome.</p>

<p>I am also very interested in seeing any pretty pictures made with gds2pov!</p>

<p>Contact me on roger_atchoo_org. Replace the _ with @ and . respectively.</p>
</div>
</body></html>
