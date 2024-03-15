To run this project on linux you have to:

1. Clone the entire framework:
   git clone https://github.com/UPB-Graphics/gfx-framework
2. To have installed g++ (minimum version 5)
3. Install an editor (Visual Studio Code)
4. Install the build tools:
  a. use your package manager to install cmake
  b. check the version using cmake --version
  c. run ./tools/install-cmake.sh && . ~/.profile in a terminal
5. Building:
  mkdir build
  cd build
  cmake ..
  cmake --build . or just make
6. Paste the directory World of Tanks to: .../gfx-framework/src/lab_m1
7. In lab_list.h add this line: #include "lab_m1/Tema1/World of Tanks"
8. Go into main.cpp:
  Find this line: World *world = new gfxc::SimpleScene();
  Replace it with: World *world = new m1::World-of-Tanks();
9. Run project: ./bin/Debug/GFXFramework
