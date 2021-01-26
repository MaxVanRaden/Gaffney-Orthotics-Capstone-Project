@echo off

mkdir build
pushd build
cls
start cmd /c emcc -I../core -I../engine ../core/main.cpp ../engine/render.cpp ../engine/shaders.cpp -L../engine/SOIL.lib -L../engine/assimp.lib -o test.html -s MAX_WEBGL_VERSION=2 -s FULL_ES3=1
popd
::Give it a little time to compile so the python server doesn't start on the old program before the new one is done
echo Compiling...
timeout /T 5
cls
start python -m http.server
start "" http://localhost:8000/build/test.html