# Namaste3D
Progression of Software Rasterization Experiments  
- [Eight Basic Exercises Demo on YouTube](https://www.youtube.com/watch?v=E1UpdbCpP9g)  
- [Scene Parsing and VF Culling Demo on YouTube](https://youtu.be/hHTu3iuzaeY)  

## Build / Project Generation

**Prerequisites**

* The root folder must contain the `res` directory.
* CMake must be installed.

**Steps**

1. Create a directory named `build` in the project root.
2. Open a terminal in the project root and run:

```bash
cd build
cmake ../
```

This will generate the project files inside the `build` directory.
For windows+visual studio projects, after building the project, search for `SDL3.dll`  
and copy it in the folder where visual studio solution file resides.