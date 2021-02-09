# quetzal

## Environment

-   Windows 7 and up
-   Microsoft Visual Studio Community 2019

## Top level directories

-   application - Direct3D 11 Windows applications
-   library - The libraries that provide most of the functionality
-   model - command line applications that genarally produce 3D model output files

## Libraries

-   brep - 3D mesh using a half-edge based boundary representation
-   common - general utility classes and functions
-   direct3d11 - Direct3D 11 rendering engine
-   framework - application and window facilities
-   geometry - general geometry classes and utilities, Point, Line, Plane, Sphere, etc.
-   math - general math utility classes and functions, Vector class with data type and dimension template parameters
-   model - 3D model generation and manipulation, built on top of brep
-   modx - renderable 3D objects, built on top of brep and direct3d11
-   svg - SVG file format writer
-   triangulation - constrained Delaunay triangulation, etc.
-   wavefront_obj - Wavefront OBJ file format parser and writer (primary 3D output format)
-   xml - XML file format parser and writer
