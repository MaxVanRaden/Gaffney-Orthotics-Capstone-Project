@echo off

mkdir build
pushd build
cls

:: change the path below to wherever your emscripten folder is so it'll set the environment variables using it's batch script
set emsdk_path=C:\Users\darkz\Downloads\emsdk-master\emsdk-master\emsdk_env.bat

set includes=-I../core -I../engine
set files=../core/main.cpp ../engine/render.cpp ../engine/shaders.cpp
set link=-L../engine/assimp.lib

set functions="_main","_print_hello","_is_ready"
set options=-s MAX_WEBGL_VERSION=2 -s FULL_ES3=1 -s EXPORTED_FUNCTIONS=[%functions%] -s EXPORTED_RUNTIME_METHODS=["ccall","cwrap"]

start "Emscripten CMD" cmd /c "%emsdk_path% & emcc %includes% %files% %link% %options% -o backend.js & echo Compiling... & timeout /T 5"
popd
::Give it a little time to compile so the python server doesn't start on the old program before the new one is done
echo Compiling...
timeout /T 5
cls
start python -m http.server
start "" http://localhost:8000/build/main.html