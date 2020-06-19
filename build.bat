mkdir build
cd build
cmake .. -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=Release
cmake --build . --target NinjaQuest --config Release
copy NinjaQuest\Release\NinjaQuest.exe ..\.
cd ..
.\NinjaQuest.exe
pause