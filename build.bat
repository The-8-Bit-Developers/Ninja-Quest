REM Remember to right click 8BitEngine in Visua Studio and set it as the startup item!
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017"
cmake --build .
REM Run the program
copy Debug\8BitEngine.exe .\
.\8BitEngine.exe
pause