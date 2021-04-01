#include <stdlib.h>
#include <stdio.h>
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
#include "backend/src/engine/maths.h"
#include "backend/src/engine/texture.h"
#include "backend/src/engine/shaders.h"
#include "backend/src/engine/render.h"
#include "MeshEditor.h"

int initialize();
void mainloop();

//globals for now just for testing
//look in defines.h if you want to see what this "global" type is
global MeshEditor* editor;
global bool initialized = false;

static const int width = 1000;
static const int height = 640;

int main(void) 
{
	if (initialize() == GL_TRUE) {		
		glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
		editor = new MeshEditor();
		initialized = true;
		emscripten_set_main_loop(mainloop, 60, 1);
	}

	glfwTerminate();
	return 0;
}

void mainloop()
{
    //clear the screen of anything that might have been on there last frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //set the viewport to the same as the windows resolution (feel free to mess around with the numbers if you want to see what it does)
    glViewport(0, 0, width, height);

    editor->run();

    glfwSwapBuffers();
    glfwPollEvents();
}

int initialize()
{

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

    return GL_TRUE;
}

void test() {
    printf("This was printed from a C++ function, called from a C function.\n");
}

extern "C" {
    int print_hello(int x) {
        printf("Hello emscripten! This is my parameter: %d\n", x);
        test();
        return x;
    }

    int import_model(char* str, int len, char target){
        int i, count = 0;
        for(i = 0; i < len; i++){
            if(str[i] == target){
                count++;
            }
        }
        for(i = 0; i < len; i++){
            printf("%c", str[i]);
        }
        printf("\n");
        return count;
    }

    char* export_model(int ID, const char* fileformat) {
        return editor->export_model(ID, fileformat);
    }

    bool is_ready() {
        return initialized;
    }
}