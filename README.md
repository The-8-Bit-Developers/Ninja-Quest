# Ninja Quest
Ninja Quest is a fun 2D 8-Bit game written for the 154th week of [weeklygamejam.com](http://www.weeklygamejam.com/)

## Supported platforms
* Windows
* MacOS
* Linux support is trivial, but building from source will be nessecary, and CMakeLists.txt will possibly need ammending to accomodate library linking.

## Dependencies
* Ninja Quest uses its own game engine, bundled in the soure code.
* SFML 2.5.1 is required for sound, graphis, windows, etc
* Lua is required for the game's scripts
* Box2D is required for the game's physics

## Building and running from source
1) Download Lua binaries for your platform (or build from source on Linux)
2) Extract the folder into the project directory
3) rename it to "lua", and the include folder inside to "lua" as well
4) Download SFML for your platform (or build from source on Linux). Again, extract it into the project directory, and rename the folder to SFML.
5) Download Box2D. You will need to build this from source, so do (note that when building for release mode, you must build box2d in release mode too, and call the library box2d, not box2d-d):
```
cd ~/Downloads
git clone https://github.com/erincatto/box2d.git
cd box2d
mkdir build
cd build
cmake ..
cmake --build .
```

6) Inside the box2d folder in your downloads, copy the include/box2d folder into build/src/

7) Copy the the src folder inside the build folder to the project directory, like before with Lua and SFML. Rename it to box2d.

8) Run ./buildDebug.sh, build.bat or buildDebug.bat depending on if you're on MacOS or Windows, and if you want debug or release mode.

## Authors
* Luka Warren
* Arthur Owenson