#  TinyRayTracer *
___ 

**Small RayTracing using raylib and GLSL**

This is a small program that is designed to demonstrate how Ray Tracing works in a simple way, using raylib to access OpenGL API. It serves as an example for understanding the basics of Ray Tracing. While it's not remotly optimized, it can be a helpful reference if needed.

![Alt text](src/resources/ScreenShot.png "Quick peek into the project ")
___

**Build process**

To build the program you need CMake. In the project folder, run the following command in your terminal:

`cmake -S . -B build` 

Once CMake finishes creating the cache, navegate to the source directory and run: 

`cmake --build ../build` 

When the process is complete, you can execute the program located at build/TinyRayTracer-RAYLIB/
