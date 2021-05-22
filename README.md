# quetzal

C++ libraries and sample command-line and Windows programs for 3D modeling and Direct3D 11 rendering. This all revolves around the creation and transformation of polygonal meshes based on the halfedge-based boundary representation in the brep library. The Wavefront .obj file format is used for file I/O.

Much of this uses template classes and functions with some of the primary use cases being the use of different floating point data types (float for Direct3D11 and double for 3D printing applications, for example), and so that the geometry primitives can work in 2 or 3 (or 4...) dimensions.

## Environment

-   Windows 7 and up
-   Microsoft Visual Studio Community 2019

## Top level directories

-   application - Direct3D 11 Windows applications
-   library - The libraries that provide the core functionality
-   model - command line applications that produce 3D model output files

## Libraries

-   brep - 3D mesh using boundary representation with a half-edge data structure
-   common - general utility classes and functions
-   direct3d11 - Direct3D 11 rendering engine
-   framework - application and window facilities
-   geometry - general geometry classes and utilities, Point, Line, Plane, Sphere, etc.
-   math - general math utility classes and functions, Vector class with data type and dimension template parameters
-   model - 3D model generation and manipulation, built on top of brep
-   modx - renderable 3D objects, built on top of brep and direct3d11
-   svg - SVG file format writer
-   triangulation - constrained Delaunay triangulation, etc.
-   wavefront_obj - Wavefront .obj file format parser and writer (primary 3D output format)
-   xml - XML file format parser and writer
