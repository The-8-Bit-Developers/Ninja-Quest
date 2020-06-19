mkdir build
cd build
cmake .. -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target NinjaQuest --config Debug
copy NinjaQuest\Debug\NinjaQuest.exe .\NinjaQuest\.
cd NinjaQuest
.\NinjaQuest.exe
pause