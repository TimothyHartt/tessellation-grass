all:
	cmake -GNinja -DCMAKE_CXX_COMPILER=/usr/bin/clang++-12 -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DCMAKE_BUILD_TYPE=Release 
	ninja -C ./build

