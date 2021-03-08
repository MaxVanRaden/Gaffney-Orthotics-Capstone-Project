@echo off
echo Configuring Emscripten environment variables
set EMSDK=C:/git/emsdk

:: Expected to already exist since
mkdir build

:: Enter the emsdk environment
call %EMSDK%/emsdk_env.bat

pushd build

    mkdir backend
    pushd backend
        :: Because we sourced the Emscripten environment variables, we can use the 'EMSCRIPTEN' var to know where the current SDK can be found, which we need so we can locate the 'Emscripten.cmake' toolchain file.
        set EMSCRIPTEN_CMAKE_PATH=%EMSDK%/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

        :: We ask CMake to configure itself against the parent folder, but unlike our other platform targets, we will tell CMake to use the Emscripten CMake toolchain which knows how to perform Emscripten builds.
        echo:
        echo Emscripten CMake path: %EMSCRIPTEN_CMAKE_PATH%
        cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=%EMSCRIPTEN_CMAKE_PATH% ../..

        :: Start the actual build.
        echo:
        echo "Building project ..."
        make

        :: Move and rename the Gaffney-Orthotics-Captone-Project .js and .wasm files to backend.js and backend.wasm up one directory
        echo:
        echo "Moving compiled .js and .wasm files -- will fail if build fails"
        MOVE /Y backend.js ../backend.js
        MOVE /Y backend.wasm ../backend.wasm
    popd
popd

:: Launch python shell and fire up browser to main.html
start python -m http.server
start "" http://localhost:8000/src/frontend/main.html