mkdir build
cd build
cmake .. -G "Visual Studio 15 2017"
cmake --build .
copy NinjaQuest\Debug\NinjaQuest.exe .\NinjaQuest\.
cd NinjaQuest
.\NinjaQuest.exe
pause