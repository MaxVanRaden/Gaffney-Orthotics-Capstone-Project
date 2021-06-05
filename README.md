# Gaffney Orthotics Capstone Project

## Contributors

Max Van Raden,
Corbin Stark,
Forest Pearson,
Phillip Chen,
Stephanie Beagle,
Chris Teters,
Jacob Odell,
Carlos Sotello

## Installation
* Ensure you have [Node.js](https://nodejs.org/en/) and [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) installed
* Navigate to *frontend/app* directory and run:
```
npm install
```
* Change line 3 in build.bat to point to emsdk in the form:
  ```set EMSDK=C:/.../emsdk```
* Change line 4 in CMakeLists.txt to point to emsdk:
```
set(EMSDK "C:/.../emsdk")
```  
* Navigate to the root directory and run:
```
build
```
* After the project is built a dev server will start. 
For an optomized build navigate to frontend/app and run ```npm run-script build```
## Documentation

### Front End

#### index.html

This is the main HTML page we will be working on. Currently it only has the canvas for rendering and some debug UI like a console output.
It loads the backend code by importing scripts like so:

```html
<!-- The backend scripts -->
<script type='text/javascript' src="load_web_assembly.js"></script>
<script type="text/javascript" src="backend.js"></script>
```

It also has some tests that call a function from the backend code as an example.

```html
<!-- Testing ways to call C functions from Javascript -->
<script>
	Module.onRuntimeInitialized = () => {
		_print_hello(5)
		_print_hello(15)
		_print_hello(25)
		_print_hello(55)
	}
</script>
```
#### index.js

This acts as the entry point for our React code in index.html by rendering our base App component. 

#### App.js

Base React component which contains our React code imported from the components directory.

## Testing
* Navigate to *frontend/app* directory and run:
```
npm test
```

### Back End

#### defines.h

For accuracy purposes, a lot of this code uses stdint.h types such as uint8_t etc. in order to guarantee that an int has exactly the amount of bits you want it to have.
It uses #defines to abbreviate these type names.

```cpp
//These are guaranteed to always be the same length.
//If you just declare a int for example, it's USUALLY 4 bytes, but sometimes with different systems it could be 8 by default.
//An i32 will always be 32 bits, 4 bytes. (really important for things like unions, bitwise operations, etc.)
#define i8 int8_t
#define u8 uint8_t
#define i16 int16_t
#define u16 uint16_t
#define i32 int32_t
#define u32 uint32_t
#define i64 int64_t
#define u64 uint64_t
#define real32 float
#define real64 double
#define f32 float
#define f64 double
#define b32 int32_t
```

It also redefine static as a few different keywords because static has a few different applications and is kind of a vague keyword.

```cpp
#define internal static
#define local static
#define global static
```

#### render.h

```cpp
struct Vertex {
    vec3 position;
    vec3 normal; //normal vector for calculating lighting as well as other mathematical operations down the line most likely.
    vec2 uv; //uv = texture coordinates. Might be used, might not be used.
};
struct Mesh {
    GLuint vbo; //vertex buffer object, the buffer where vertex data is stored _on the GPU_
    GLuint ebo; //element buffer object, indexing the vertices saves space
    std::vector<Vertex> vertices; //storing vertex data on the _CPU_ so that it can be manipulated. Have to upload this data to the GPU again for changes to actually be rendered though.
    u32 indexcount;
    u32 material;
};
struct Model {
    std::vector<Mesh> meshes; //models are made up of multiple meshes in plenty of cases, where each one has a different material
    std::vector<Material> materials;

    vec3 pos;
    vec3 rotation;
    vec3 scale;
};

//self-explanatory function calls available in this module
//the library Assimp is used to load models, should support both .obj and .stl but I have to get it working first to test
void dispose_mesh(Mesh* mesh);
void dispose_model(Model* model);
Mesh create_mesh(std::vector<Vertex> vertices, std::vector<GLushort> indices);
void load_mesh(Model* model, u32 i, const aiMesh* paiMesh);
Model load_model(const char* filename);
void draw_mesh(Mesh mesh);
void draw_mesh(Mesh mesh, Texture tex);
void draw_model(Model* model, Texture tex);
```
#### maths.h

2d vectors, 3d vectors, 4d vectors, basic quaternion stuff, and 4x4 matrices

```cpp
f32 deg_to_rad(f32 deg);
f32 rad_to_deg(f32 rad);

f32 length(vec2 vec);
f32 length(vec3 vec);
f32 length(vec4 vec);

void normalize(vec2* vec);
void normalize(vec3* vec);
void normalize(vec4* vec);

vec2 normalize(vec2 vec);
vec2 normalize(vec3 vec);
vec2 normalize(vec4 vec);

f32 dot(vec2 a, vec2 b);
f32 dot(vec3 a, vec3 b);
f32 dot(vec4 a, vec4 b);

vec3 cross(vec3 a, vec3 b);

mat4 identity();
mat4 translation(const f32 x, const f32 y, const f32 z);
mat4 translation(const vec3 translation_vec);
mat4 scale(const f32 x, const f32 y, const f32 z);
mat4 scale(const vec3& scale_vec);
mat4 rotateX(f32 angle);
mat4 rotateY(f32 angle);
mat4 rotateZ(f32 angle);
mat4 rotation(f32 angle, f32 x, f32 y, f32 z);
mat4 rotation(f32 angle, const vec3 axis);
mat4 orthographic_projection(f32 x, f32 y, f32 width, f32 height, f32 near_plane, f32 far_plane);
mat4 perspective_projection(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane);
mat4 create_transformation_matrix(f32 x, f32 y, f32 z, f32 rotX, f32 rotY, f32 rotZ, f32 scaleX, f32 scaleY, f32 scaleZ);
mat4 create_transformation_matrix(const vec3 translation, const vec3 rotation, const vec3 scale_vec);
mat4 inverse(const mat4 mat);
bool point_inside_triangle(vec3 point, vec3 tri1, vec3 tri2, vec3 tri3);
mat4 look_at(const vec3 camera, const vec3 center, const vec3 up = V3(0, 1, 0));

mat4 create_view_matrix(Camera cam);
void move_cam_forward(Camera cam, f32 units);
void move_cam_backwards(Camera cam, f32 units);
void move_cam_right(Camera cam, f32 units);
void move_cam_left(Camera cam, f32 units);
```

## Contact

//TODO: Other people's emails here

Max Van Raden (vanraden@pdx.edu),
Corbin Stark (starkcorbin@gmail.com),
Forest Pearson(fpearson@pdx.edu),
Phillip Chen (philchen@pdx.edu),
Stephanie Beagle (beagle@pdx.edu),
Christopher Teters (cteters@pdx.edu),
Jacob Odell (jeo@pdx.edu),
Carlos Sotello (csotello@pdx.edu)

# License

Open Asset Import Library (assimp)

Copyright (c) 2006-2016, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



******************************************************************************

AN EXCEPTION applies to all files in the ./test/models-nonbsd folder.
These are 3d models for testing purposes, from various free sources
on the internet. They are - unless otherwise stated - copyright of
their respective creators, which may impose additional requirements
on the use of their work. For any of these models, see
<model-name>.source.txt for more legal information. Contact us if you
are a copyright holder and believe that we credited you inproperly or
if you don't want your files to appear in the repository.


******************************************************************************

Poly2Tri Copyright (c) 2009-2010, Poly2Tri Contributors
http://code.google.com/p/poly2tri/

All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
* Neither the name of Poly2Tri nor the names of its contributors may be
  used to endorse or promote products derived from this software without specific
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
