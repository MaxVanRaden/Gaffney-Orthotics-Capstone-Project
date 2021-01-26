@echo off

mkdir build
pushd build
cls
:: change the path below to wherever your emscripten folder is so it'll set the environment variables using it's batch script
::start "Emscripten CMD" cmd /c "C:\Users\darkz\Downloads\emsdk-master\emsdk-master\emsdk_env.bat & emcc -I../core -I../engine ../core/main.cpp ../engine/render.cpp ../engine/shaders.cpp -L../engine/SOIL.lib -L../engine/assimp.lib -o test.html -s WASM=1 -std=c++1z -g4 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=1 --source-map-base localhost:8000 & echo Compiling... & timeout /T 5"
start "Emscripten CMD" cmd /c "C:\Users\darkz\Downloads\emsdk-master\emsdk-master\emsdk_env.bat & emcc -I../core -I../engine ../core/main.cpp ../engine/render.cpp ../engine/shaders.cpp -L../engine/SOIL.lib -L../engine/assimp.lib -o test.html -s MAX_WEBGL_VERSION=2 -s FULL_ES3=1 & echo Compiling... & timeout /T 5"
:: start cmd /c emcc -I../core -I../engine ../core/main.cpp ../engine/render.cpp ../engine/shaders.cpp -L../engine/SOIL.lib -L../engine/assimp.lib -o test.html
popd
::Give it a little time to compile so the python server doesn't start on the old program before the new one is done
echo Compiling...
timeout /T 5
cls
start python -m http.server
start "" http://localhost:8000/build/test.html