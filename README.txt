This project contains an implementation of a "multiple choice" mesh simplfication algorithm. Over a number of iterations a random fraction of the total edges in the supplied mesh are processed with a subset of these processed edges collapsed (the lowest scoring collapses win when a collision occurs).

The only non-standard dependency is the qef_simd.h single file header which you can find in my "qef" project, a version is also included here.

The demo depends on Dear ImGui, SDL2, GLM and GLEW. I don't think the particular version matters, the demo project expects these to exist in the solution directory.

A pre-compiled x64 executable is included with the SDL2 and glew DLLs.

The controls are:
	- hold left mouse and drag to rotate the view
	- use the mouse wheel to zoom in/out
	- press F1 to render a wireframe

Send any questions to nick.gildea@gmail.com or @ngildea85 on Twitter

(https://raw.github.com/nickgildea/ng_mesh_simplify/master/example.png)
