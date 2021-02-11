@echo off

mkdir build
pushd build
cls

set includes=-I../core -I../engine
set files=../core/main.cpp ../engine/render.cpp ../engine/shaders.cpp -L../engine/SOIL.lib
set link=-L../engine/assimp.lib
set options=-s MAX_WEBGL_VERSION=2 -s FULL_ES3=1 -s EXPORTED_FUNCTIONS=["_main","_print_hello","cwrap"] -s EXPORTED_RUNTIME_METHODS=["ccall","cwrap"]

start "Emscripten CMD" cmd /c emcc %includes% %files% %link% %options% -o backend.js"

popd
::Give it a little time to compile so the python server doesn't start on the old program before the new one is done
echo Compiling...
timeout /T 5
cls
start python -m http.server
start "" http://localhost:8000/build/main.html