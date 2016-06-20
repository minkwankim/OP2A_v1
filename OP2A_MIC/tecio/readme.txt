This package contains four files per platform
---------------------------------------------
  tecio    shared library
  teciompi shared library
  tecio    static library
  teciompi static library

These files have different naming conventions depending on the platform (windows, linux, mac)
Currently only the dynamic libraries are provided with the Tecplot installation package.
See the Tecplot Dataformat Guide for instructions on how to use these libraries.  



Static vs. Dynamic
--------------------

Advantages of Dynamic libraries:
  - You can run your application on a different platform than was used to create the library.
  - You can drop in replace the tecio library without relinking your application
  - (Windows only) You can use it with any version of Visual Studio


Advantages of Static libraries:
  - You don't have to provide a path to the tecio library in order to run your application
    (On windows this is usually not a big deal because the path to the executable is the default).
  - You don't have to supply another library.  Everything is baked in.

