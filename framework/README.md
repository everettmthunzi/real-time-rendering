# RTR: CMake-framework (How-to)

Guide for including an application into the framework

## Getting Started - Brief

* **application-files** *(main.cpp)* to be placed in **apps/**
* **class-files** *(.h, .hpp, .cpp, ...)* to be placed in **rendering/** 
* **resources** *(shaders, meshes, textures)* to be placed in  **resources/**


### 1. Add new Application  : apps/

* copy any existing application in **apps/**
* rename it, following the convention *rtr_newapp*

### 2. Modify CMakeLists.txt  : apps/rtr_new_app/CMakeLists.txt

* replace existing project name in line 25

```
... InitApp(${CMAKE_PROJECT_NAME}_new_app) ...
```

* replace main.cpp with your own

### 3. Add Source-files  : rendering/src/rtr

* add all source-files in **rendering/src/rtr**

*for convenience and organization, creating **subdirectories** is recommended*

```
rendering/src/rtr/some_module
```

### 4. Add Header-files  : rendering/include/rtr

* add all header-files in **rendering/include/rtr**

*for convenience and organization, creating **subdirectories** is recommended*

```
rendering/include/rtr/some_module
```

### 5. Modify CMakeLists.txt  : rendering/CMakeLists.txt:

* add new subdirectories ***some_module*** to **rendering/CMakeLists.txt:**

```
...	# header files 
		install(DIRECTORY ${PROJECT_INCLUDE_DIR}/rtr/ren DESTINATION include/rtr FILES_MATCHING PATTERN "*.*h")
		install(DIRECTORY ${PROJECT_INCLUDE_DIR}/rtr/some_module DESTINATION include/rtr FILES_MATCHING PATTERN "*.*h")
```

### 6. Add resources

```
place resource files in according subdirectories in resources/

included file types can be found in top-level CMakeLists.txt (end of file).

if a file format isn't included yet, add it there
```

### 7. INCLUDE:	 Header-files & Resources-files

* include Header-files from respective directories

```
#include <rtr/ren/some_class.hpp>
#include <rtr/some_module/some_other_class.hpp>
```

* include Resource-files from respective directories

```
e.g. texture: "../resources/textures/some_texture.png"
e.g. shaders: "../resources/shaders/some_shader.frag/vert/geom"
e.g. objects: "../resources/objects/some_object.obj"

```

### 8. Run CMake !

* for the first run i.e. fresh clone, fork or pull:

```
-> create framework/build directory
-> in framework/build, run ccmake ..
-> press 'c' 
-> if errors occur, see if libraries mentioned in error message are installed and in standard paths
-> press 'c' until you see the option 'g' (generate) appear at the bottom of the terminal
-> run make install
-> done!
-> run new app from install/bin
```

* adding "install" to the make command is important! otherwise, changes in shaders won't be added to install folder#


### Libraries linux set-up

* **Boost**
```
sudo apt-get install libboost-all-dev
sudo apt-getinstall libboost-dev libboost-test-dev
```

* **CMAKE**
```
sudo apt-get install cmake cmake-qt-gui cmake-curses-gui pkgconf
```

* **GLM**
```
sudo apt-get install libglm-dev
```

* **GLEW**
```
sudo apt-get install libglew-dev glew-utils libglfw3-dev 
```

* **SDL2**
```
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl-mixer1.2-dev
```

* **FREEGLUT3**
```
sudo apt-get install freeglut3-dev 
```

* **ASSIMP**
```
libassimp-dev
```

* **SOIL**
```
sudo apt-get install libsoil-dev
```

* **DevIL**
```
sudo apt-get install libdevil-dev
sudo apt-get install libdevil1c2
```

* **FREEIMAGE**
```
sudo apt install libfreeimage-dev libfreeimageplus-dev libfreeimage3  libfreeimageplus3
```

* **CCMAKE (one-liner)**
```
sudo apt-get install build-essential g++ libboost-all-dev cmake cmake-qt-gui cmake-curses-gui pkgconf
```

* **ogl (one-liner)**
```
sudo apt-get install libglm-dev libglew-dev glew-utils libglfw3-dev libsdl2-dev freeglut3-dev libassimp-dev libsoil-dev libdevil-dev libdevil1c2 libfreeimage-dev libfreeimageplus-dev libfreeimage3  libfreeimageplus3

```


###  Verify installed libraries 

FLAGS

```
$ pkg-config --cflags freetype2

  -I/usr/include/freetype2
```

LIB

```
$ pkg-config --libs freetype2

-lfreetype 
```
