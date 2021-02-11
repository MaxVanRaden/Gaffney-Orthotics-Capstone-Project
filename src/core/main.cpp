#include <stdlib.h>
#include <stdio.h>
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "../engine/maths.h"
#include "../engine/texture.h"
#include "../engine/shaders.h"
#include "../engine/render.h"

int initialize();
void mainloop();

//globals for now just for testing
//look in defines.h if you want to see what this "global" type is
global OrthoShader ortho;
global StaticShader basic;
global Texture tex;
global Model cube;
global mat4 transform;
global Camera camera;
global Mesh mesh;
global GLuint vertexPosObject;

global bool initialized = false;

//Cube Mesh Data (just a shit ton of 3d points in space to define all the triangles that make up the cube, I found it on google)
//once we have the .obj loader working then we will just load models instead of hardcoding them.
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

void test() {
	printf("This was printed from a C++ function, called from a C function.\n");
}

extern "C" {
	
	int print_hello(int x) {
		printf("Hello emscripten! This is my parameter: %d\n", x);
		test();
		return x;
	}
	
	bool is_ready() {
		return initialized;
	}
	
}

int main(void) 
{
	if (initialize() == GL_TRUE) {		
		glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
		
		ortho.load();
		basic.load();        
		basic.set_shadows_on(false);
		camera = {0};
		
		initialized = true;
		emscripten_set_main_loop(mainloop, 0, 1);

		//tex = load_texture("data/textures/lava.png", GL_LINEAR);
		//cube = load_model("data/models/cube.obj");

	}
		
	glfwTerminate();

	return 0;
}

int initialize()
{
	const int width = 1000,
	         height = 640;

	if (glfwInit() != GL_TRUE) {
		printf("glfwInit() failed\n");
		return GL_FALSE;
	}
	//set webgl context settings
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//enable anti-aliasing
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	
	if (glfwOpenWindow(width, height, 8, 8, 8, 8, 16, 0, GLFW_WINDOW) != GL_TRUE) {
		printf("glfwOpenWindow() failed\n");
    	return GL_FALSE;
    }
	
    glfwSwapInterval(1);
	
	printf("|||||||||||||||||||||||||||||||||\n");
	printf("|  Gaffney Orthotics Capstone   |\n");
	printf("|||||||||||||||||||||||||||||||||\n");
	printf("\n");
	
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL Version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("OpenGL Vendor:  %s\n", glGetString(GL_VENDOR));
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetWindowTitle("Gaffney Orthotics");
	
	//load the cube vertex data into it's buffer
	glGenBuffers(1, &mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	//load the triangle vertex data into it's buffer
	GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
                           -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f };

    glGenBuffers(1, &vertexPosObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
    glBufferData(GL_ARRAY_BUFFER, 9*4, vVertices, GL_STATIC_DRAW);

    return GL_TRUE;
}

void mainloop()
{	
	//clear the screen of anything that might have been on there last frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set the viewport to the same as the windows resolution (feel free to mess around with the numbers if you want to see what it does)
	glViewport(0, 0, 1000, 640);
	
	local float rotation = 0.0f;
	rotation+=0.2f;
	//create a transformation matrix to transform the cube vertices from model space to world space (translate it by x,y,z, rotate it by rotX,rotY,rotZ, scale by scaleX,scaleY,scaleZ)
	transform = create_transformation_matrix( {-2, 0, 0}, {rotation, rotation, rotation}, {1, 1, 1} );
	
	local float zoom = 15.0f;
	zoom -= 0.025f;
	
	//bind the shader to prepare it for rendering
	basic.bind();
	//set the model transform to the transform we created.
	basic.set_transform(transform);
	//set the view matrix (the matrix that defines how the camera is viewing the world) to look at (0, 0, 0) from a position of (zoom, zoom, zoom)
	basic.set_view(look_at({zoom, zoom, zoom}, {0, 0, 0}));
	
	//Prepare cube mesh for rendering (bind the buffer storing the mesh data, vertices)
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	//then activate that buffers attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
    glEnableVertexAttribArray(0);

	//call the rendering (this runs the shader program on the GPU)
    glDrawArrays(GL_TRIANGLES, 0, 12*3); 

    transform = create_transformation_matrix( {2, 0, 0}, {rotation, rotation, rotation}, {2, 2, 2} );
    basic.set_transform(transform);

    //Prepare triangle mesh for rendering (bind the buffer storing the mesh data, vertices)
    glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays ( GL_TRIANGLES, 0, 3 );
 
    glfwSwapBuffers();
    glfwPollEvents();
}