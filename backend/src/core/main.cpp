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
bool isBinarySTL(char * name);
void load_binary_STL (char * buffer);

//globals for now just for testing
//look in defines.h if you want to see what this "global" type is
global MeshEditor* editor;
global bool initialized = false;

static int width = 800;
static int height = 600;



int main(void) 
{
	if (initialize() == GL_TRUE) {		
		glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
		editor = new MeshEditor();
		initialized = true;
		emscripten_set_main_loop(mainloop, 0, 1);
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

    editor->run(width, height);

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

    void import_model(char* str, int fileformat){
        // fileformat 0: obj
        //            1: stl (ascii)
        //            2: stl (binary)
        //            3: memfs filepath
        //            4: null
        // I'll switch the fileformat order around later making 0 == null
        // no change for now to maintain branch compatibility
        if (fileformat == 4)
            printf("no file sent to import\n");
        else if (fileformat == 2)
            load_binary_STL(str);
        else
            editor->add_model(str, fileformat);
    }

    char* export_model(const char* fileformat) {
        return editor->export_model(".stl"); //.obj hard coded for now
    }

    uint32_t get_export_strlen() {
        return editor->get_export_strlen();
    }
    
	void set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll){
		editor->set_camera(zoom, x, y ,z ,yaw, pitch, roll);
	}

	// Scale every vertex in every mesh in every entity by the factor passed in
	void scale(float factor){
        printf("backend scale got factor %f and attempted to scale by %f%\n", factor, factor*100);
        if(initialized){
            editor->scale_all_entities(factor);
        }
    }

    bool is_ready() {
        return initialized;
    }
    //Set canvas size
    void set_size(int w, int h){
        width = w;
        height = h;
    }

    void on_mouse_up(int x, int y, int x2, int y2){
        printf("(%d,%d),(%d,%d)\n", x, y, x2, y2);
        editor->on_mouse_up(x, y, x2, y2);
    }

    void translate_vertex(){
        editor->translate_vertex();
    }

    // expermental memfs, does not yet work with React
    void import_file(char* file_path){
        FILE *file = fopen(file_path, "r+");
        if (!file)
            printf("cannot open file\n");
        else {
            fseek(file, 0, SEEK_END);
            long fileSize = ftell(file);//Read file size
            fseek(file, 0, SEEK_SET);
            printf("file size: %ld", fileSize);
            char * buffer = (char *) malloc(sizeof(char) * fileSize);//Apply for memory
            long result = fread(buffer, 1, fileSize, file);//File read into buffer
            if (result != fileSize) {
                printf("Reading error or ASCII file: ");
                printf("result = %ld  fileSize = %ld\n", result, fileSize);
            }
            fclose(file);
            load_binary_STL(buffer);
        }
    }
}

bool isBinarySTL(char * name){
    char tag[strlen("solid") +1] = "solid";

    for (int i = 0; i < strlen(tag); i++){
        if (tag[i] != name[i]){
            return true;
        }
    }
    return false;
}

void load_binary_STL (char* buffer) {
    const char *offset = buffer;
    float temp;
    char name[80];
    memcpy(name, offset, 80);//Record file name
    if(isBinarySTL(name)) {
        offset += 80;
        int numTriangles;
        memcpy(&numTriangles, offset, 4);//Record the number of triangles
        offset += 4;

        std::string view;
        view.append("solid name\n");
        for (int i = 0; i < numTriangles; i++) {
            memcpy(&temp, offset, 4);
            float normalI = temp;
            memcpy(&temp, offset + 4, 4);
            float normalJ = temp;
            memcpy(&temp, offset + 8, 4);
            float normalK = temp;
            std::string line1 =
                    "facet normal " +
                    std::to_string(normalI) + " " +
                    std::to_string(normalJ) + " " +
                    std::to_string(normalK) +
                    "\n  outer loop\n";
            view.append(line1);
            offset += 12;
            for (int j = 0; j < 3; j++) {
                memcpy(&temp, offset, 4);
                float verticesX = temp;
                memcpy(&temp, offset + 4, 4);
                float verticesY = temp;
                memcpy(&temp, offset + 8, 4);
                float verticesZ = temp;
                std::string line2 =
                        "    vertex " +
                        std::to_string(verticesX) + " " +
                        std::to_string(verticesY) + " " +
                        std::to_string(verticesZ) + "\n";
                view.append(line2);
                offset += 12;
            }
            view.append("  endloop\nendfacet\n");
            offset += 2;
        }
        view.append("endsolid name\n");

        int len = view.length();
        printf("size of converted file: %d\n", len);

        editor->add_model(&view[0], 1);
    } else {
        editor->add_model(buffer, 1);
    }
}