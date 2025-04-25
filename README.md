# A grid of pixels

You are given a 320x240 grid of pixels, and the graphics library TIGR, through which you are able to set these pixels to different colours.

Your task is to create a program that renders 3D graphics, complete with textures, lighting, maybe even environment mapping, despite these limitations. 

## In more detail

This will be done by mapping a 3D vector space into the 2D vector space of the 320x240 grid of pixels. For this you will use the types defined in the triangles.h file. The basic process is to take in a `triangle_t` struct and a point of view vector, and for this to result in the triangle being appropriately projected onto the screen.

Start thinking about how you would define 3D objects, with textures, lighting information, etc. using this system. How would you even draw a single-coloured triangle?

## How to build and run

1. Ensure that the `make` command line tool is installed for your OS.

2. Open a terminal in this directory and run `make`.

3. There should now be an executable named `renderer` in this directory.