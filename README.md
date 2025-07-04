# GDS2X

## Introduction

GDS2X is a program to process GDS2 integrated circuit layout files into other
formats. It was originally just the gds2pov program, then added support for
gds2svg, and has now added further output options and merged them into a single
program.

GDS2 files are used for storing the geometrical and layer information for
creating an IC or MEMS device. If you do know not what this is, GDS2X is
probably of no interest to you!

## Output formats

* 3mf - a common standard in the 3D printing world
* openscad - script output for processing in the OpenSCAD 3D solid modeller
* povray - script output for processing in the POV-Ray ray tracer, to create 3D renders
* stl - binary 3D model file
* svg - 2D graphical image format

I would love to hear from you if you produce any nice results! See the contact
section at the bottom of this document.


## General Usage

Usage is mostly the same across the programs. You can always try:

program -h

to get the specific usage information.


gds2pov [-i input.gds] [-o output.pov] [-b] [-c config.txt] [-p process.txt] [-t topcell] [-v]

 -c	Specify config file
 -g Generate process file from gds2 file information (suppresses generation of
    output file).
 -h Show usage
 -i Input GDS2 file (stdin if not given)
 -o Output POV file (stdout if not given)
 -p	Specify process file
 -q Quiet output
 -t	Specify top cell name
 -v	Verbose output

First steps - what do you need at the absolute minimum? This program, a GDS2
file and a file describing your process to GDS2POV.

An example process file is provided with this package and is called
example_process.txt. This file should give you all the information you need on
how to create a configuration for your own process. You will also need the layer
information for your process as well as the height and thickness of the layers
if you have it and want the output to be more accurate.

Note: POV-Ray prefers it if objects are not perfectly aligned with one another.
That is to say, it would prefer a box at (0,0,0) extending in the +x direction
and a box at (1,0,0) extending in the -x direction to a box at (0,0,0) extending
in the +x direction and a box at (0,0,0) extending in the -x direction. I hope
that is clear. Anyway, what this means is that when you are defining the height
and thickness for adjacent layers, make sure that they overlap just slightly.
This is the reason for the slightly odd numbers in example_process.txt.

So, you have everything. Just run:

gds2pov -i input.gds -o output.pov -p process.txt

This will produce a POV file with the camera directly above the centre of the
layout looking down. No extra lights are defined.


If you want a more complicated arrangement there are further options.

Firstly, you can edit the camera and lighting settings in the POV file by hand.
GDS2POV outputs a number of comments at the start of the POV file which give
the extent of the GDS2 data. You can use these co-ordinates to help position
lights and the camera where you wish.

The second option is to use a config file. An example config file is provided
and is called config.txt. To use a config file with GDS2POV, try the following:

gds2pov input.gds output.pov -c config.txt

The config file allows you to set the ambient light level, a process file and
the position of the camera, where the camera is looking as well as defining
any number of additional lights. See config.txt for more information.

If gds2pov does not find your top cell correctly, use the -t option to specify the top cell name.


## Using the POV File

It is beyond the scope of this document to describe how best to use POV-Ray.
The POV-Ray documentation is excellent, but there are some settings that may
be particularly useful for GDS2POV generated files.

The most notable is probably Quality. Setting Quality to 1 (the default is 9)
will use a high level of ambient light, no other lights and will ignore shadows
and any special surfaces on objects. This means that the rendering time will be
greatly reduced. Set the Quality to 9 again to render your final image.

To set the quality in a render ini file, use the line

Quality=9

Or to set it on the command line, use

+Q9

If you have suggestions for other useful settings, please send them to me for
inclusion.


## Known Problems

### General

* Some complex/degenerate polygon forms are not supported. The solution is to
  split these polygons into simpler polygons, but that's hard in itself.
* Paths with acute angles aren't properly supported. They produce a point at
  the corner rather than a mitre.
* With the exception of svg output, rounded end paths are not supported.
* For svg only, variable extension paths are not supported.
* Summary only considers a single instance of each object.
* Support for the GDS2 format is incomplete.
* Big files take a long time and a lot of memory to render. Yes... try
  reducing the quality and size of your output picture in POV-Ray. I don't
  really envisage GDS2POV being used on complete chips - POV-Ray is limited
  to 2GB of RAM on 32 bit machines and it is quite possible to use that with
  a complicated GDS2 file.


## License

The GDS2X project is released under the LGPL v2.1+


## Contact

GDS2X was written by Roger Light. Questions, bug reports, suggestions
or requests for new features are welcome.

Contact me on roger@atchoo.org

Updates to GDS2X should be at https://github.com/ralight/gds2pov or http://atchoo.org/gds2pov/
