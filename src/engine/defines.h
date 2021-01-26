#ifndef DEFINES_H
#define DEFINES_H

#include <math.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

//static is kind of a bad keyword because it means three different things depending on context, so I used defines to make it clearer
//static used within a function just makes a variable local to that function,
//static used in global scope just means "only available to be used in this translation unit (usually just means the .cpp file it was declared in),
//static used in function declarations is similar to the last one but different in a few way (internal)
//just a random pet peeve, you can ignore this if you want
#define internal static
#define local static
#define global static
#define external extern
#define MAX_FORMAT_TEXT_SIZE 128

//just so I have to type less often, these are guaranteed to always be the same length.
//if you just declare an int for example, it's USUALLY 4 bytes, but sometimes with different systems it could be 8 by default.
//an i32 will always be 32 bits, 4 bytes. (really important for things like unions, bitwise operations, etc.)
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

#define FATAL_ERROR 0
#define MINOR_ERROR 1
#define INFO 2
#define WARNING 3
#define DEBUG 4

internal inline 
void LOG(u8 TYPE, const char* format, ...) {
	switch (TYPE) {
	case FATAL_ERROR: fprintf(stderr, "FATAL ERROR: "); break;
	case MINOR_ERROR: fprintf(stderr, "ERROR: ");       break;
	case INFO:        fprintf(stderr, "INFO: ");        break;
	case WARNING:     fprintf(stderr, "WARNING: ");     break;
	case DEBUG:       fprintf(stderr, "DEBUG: ");       break;
	default:          break;
	}

	va_list v1;
	va_start(v1, format);
	vfprintf(stderr, format, v1);
	va_end(v1);

	fprintf(stderr, "\n");

	if (TYPE == FATAL_ERROR) exit(1);
}

internal inline
f32 absolute(f32 value) {
	if (value < 0)
		return -value;
	return value;
}

internal inline
double getDistanceM(f32 x1, f32 y1, f32 x2, f32 y2) {
	double dx = absolute(x1 - x2);
	double dy = absolute(y1 - y2);
	if(dx > dy)
		return dx;
	else
		return dy;
}

internal inline
double getDistanceE(f32 x1, f32 y1, f32 x2, f32 y2) {
	double dx = x1 - x2;
	double dy = y1 - y2;
	return absolute(sqrt(dx * dx + dy * dy));
}

struct Rect {
	float x;
	float y;
	float width;
	float height;
};

internal inline
struct Rect rect(float x, float y, float width, float height) {
	struct Rect r;
	r.x = x;
	r.y = y;
	r.width = width;
	r.height = height;
	return r;
}

internal inline
char* read_file(const char* filename) {
	FILE* file = fopen(filename, "rt");
	fseek(file, 0, SEEK_END);
	unsigned long length = ftell(file);
	char* data = new char[length + 1];
	memset(data, 0, length + 1);
	fseek(file, 0, SEEK_SET);
	fread(data, 1, length, file);
	fclose(file);

	return data;
}

//Altered GLFW3 #defines to remove the GLFW_ and make it less verbose to type.
//Original written by Marcus Geelnard and Camilla Berglund

#define RELEASE                0

#define PRESS                  1

#define REPEAT                 2
#define KEY_UNKNOWN            -1

#define KEY_SPACE              32
#define KEY_APOSTROPHE         39 
#define KEY_COMMA              44 
#define KEY_MINUS              45 
#define KEY_PERIOD             46 
#define KEY_SLASH              47 
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59 
#define KEY_EQUAL              61 
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91 
#define KEY_BACKSLASH          92 
#define KEY_RIGHT_BRACKET      93 
#define KEY_GRAVE_ACCENT       96 
#define KEY_WORLD_1            161
#define KEY_WORLD_2            162 

#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348
#define KEY_LAST               GLFW_KEY_MENU

#ifndef MOD_SHIFT
#define MOD_SHIFT           0x0001
#endif

#define MOD_CONTROL         0x0002

#ifndef MOD_ALT
#define MOD_ALT             0x0004
#endif

#define MOD_SUPER           0x0008

#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3

#define JOYSTICK_1             0
#define JOYSTICK_2             1
#define JOYSTICK_3             2
#define JOYSTICK_4             3
#define JOYSTICK_5             4
#define JOYSTICK_6             5
#define JOYSTICK_7             6
#define JOYSTICK_8             7
#define JOYSTICK_9             8
#define JOYSTICK_10            9
#define JOYSTICK_11            10
#define JOYSTICK_12            11
#define JOYSTICK_13            12
#define JOYSTICK_14            13
#define JOYSTICK_15            14
#define JOYSTICK_16            15
#define JOYSTICK_LAST          GLFW_JOYSTICK_16

#endif