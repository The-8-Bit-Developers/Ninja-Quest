mkdir build
# Copy libraries into build folder
cp -r SFML/lib/. build/NinjaQuest/
cp lua/liblua53.dylib build/NinjaQuest/
# Build the project
cd build
cmake ..  -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target NinjaQuest --config Debug
cd NinjaQuest
DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:.
./NinjaQuest
