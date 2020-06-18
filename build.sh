mkdir build
# Copy libraries into build folder
cp -r SFML/lib/. build/
cp lua/liblua53.dylib build/
# Build the project
cd build
cmake ..
Make
./8BitEngine