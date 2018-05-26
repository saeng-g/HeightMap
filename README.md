# HeightMap
COMP 371 assignment: Height Map modelling using OpenGL

Uses OpenGL, GLFW, GLEW, GLM, and CImg APIs and Libraries
Arrow keys : Turns model
P: points
W: lines
T: triangles
Mouse Scrolling : Zoom in/out

May 26th 2018: Updated the mesh algorithm from one using degenerate triangles and GL_TRIANGLE_STRIPS to one using upper and lower triangles to form grids and using GL_TRIANGLES. This cleaned up the problem where the vertices of one end of the map was connecting to vertices of the other end of the map.
