#include "shaders.h"

const int FOV = 90;

void OrthoShader::load() {
    char vShaderStr[] =  
      "attribute vec4 vPosition;    \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vPosition;  \n"
      "}                            \n";
   
   char fShaderStr[] =  
      "precision mediump float;\n"\
      "void main()                                  \n"
      "{                                            \n"
      "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
      "}                                            \n";
   
    shader = load_shader_from_strings( vShaderStr, fShaderStr );
	
    start_shader(shader);
	glBindAttribLocation ( shader.ID, 0, "position" );
    upload_int(shader, "sprite", 0);
    transform = glGetUniformLocation(shader.ID, "transform");
    projection = glGetUniformLocation(shader.ID, "projection");
	
    glUniformMatrix4fv(projection, 1, GL_FALSE, (perspective_projection(FOV, 16.0f / 9.0f, 1.0f, 300.0f).elements));
    printf("ortho shader constructed\n");
}

void OrthoShader::dispose() {
    dispose_shader(shader);
}

void OrthoShader::bind() {
    start_shader(shader);
}

void OrthoShader::set_projection(mat4 proj) {
    glUniformMatrix4fv(projection, 1, GL_FALSE, proj.elements);
}

void OrthoShader::set_transform(mat4 tran) {
    glUniformMatrix4fv(transform, 1, GL_FALSE, tran.elements);
}



void PickingShader::load() {
    shader = load_shader("data/shaders/picking_vs.glsl", "data/shaders/picking_fs.glsl");

    start_shader(shader);
    alpha = glGetUniformLocation(shader.ID, "alpha");
    pickID = glGetUniformLocation(shader.ID, "pickID");
    projection = glGetUniformLocation(shader.ID, "projection");
    view = glGetUniformLocation(shader.ID, "view");
    transform = glGetUniformLocation(shader.ID, "transform");
    flip = glGetUniformLocation(shader.ID, "flip");

    glUniformMatrix4fv(projection, 1, GL_FALSE, (perspective_projection(90, 16.0f / 9.0f, 1.0f, 300.0f).elements));
    printf("picking shader constructed\n");
}

void PickingShader::dispose() {
    dispose_shader(shader);
}

void PickingShader::bind() {
    start_shader(shader);
}

void PickingShader::set_flip(bool flip) {
    glUniform1i(this->flip, flip);
}

void PickingShader::set_alpha(bool alpha) {
    glUniform1i(this->alpha, alpha);
}

void PickingShader::set_pickID(vec4 ID) {
    glUniform4f(pickID, ID.x, ID.y, ID.z, ID.w);
}

void PickingShader::set_projection(mat4 proj) {
    glUniformMatrix4fv(projection, 1, GL_FALSE, proj.elements);
}

void PickingShader::set_view(mat4 view) {
    glUniformMatrix4fv(this->view, 1, GL_FALSE, view.elements);
}

void PickingShader::set_transform(mat4 tran) {
    glUniformMatrix4fv(transform, 1, GL_FALSE, tran.elements);
}



void StaticShader::load() {
	//for now just put the shader program code in a string
    char vShaderStr[] =  
"attribute vec3 position;\n"
"attribute vec2 uv;\n"
"attribute vec3 normal;\n"
"varying vec3 pass_pos;\n"
"uniform mat4 projection;\n"
"uniform mat4 transform;\n"
"uniform mat4 view;\n"
"void main() {\n"
"	 pass_pos = position;\n"
"    gl_Position = projection * view * transform * vec4(position, 1.0);\n"
"}";
   
   char fShaderStr[] =  
      "precision mediump float;\n"\
	  "varying vec3 pass_pos;\n"
      "void main()                                  \n"
      "{                                            \n"
      "  gl_FragColor = vec4 ( pass_pos.x, pass_pos.z, pass_pos.y, 1.0 );\n"
      "}                                            \n";
	  
	shader = load_shader_from_strings( vShaderStr, fShaderStr );

	//none of this (except for projection, view, and transform) is actually used yet, but it will be.
    start_shader(shader);
    upload_int(shader, "tex", 0);
    upload_int(shader, "depthbuffer", 1);
	shadowsOn = glGetUniformLocation(shader.ID, "shadowsOn");
    lightColor = glGetUniformLocation(shader.ID, "lightColor");
    lightPos = glGetUniformLocation(shader.ID, "lightPos");
    projection = glGetUniformLocation(shader.ID, "projection");
    transform = glGetUniformLocation(shader.ID, "transform");
    view = glGetUniformLocation(shader.ID, "view");
    lightspace = glGetUniformLocation(shader.ID, "lightspace");

    glUniformMatrix4fv(projection, 1, GL_FALSE, (perspective_projection(90, 16.0f / 9.0f, 1.0f, 300.0f).elements));
	
	set_light_color(1.0, 1.0, 1.0, 1.0);
	set_light_pos(-4, 24, -2);
	set_transform(identity());
	set_view(identity());
	set_lightspace(identity());
	set_shadows_on(true);

    printf("static shader constructed\n");
}

void StaticShader::dispose() {
    dispose_shader(shader);
}

void StaticShader::bind() {
    start_shader(shader);
}

void StaticShader::set_shadows_on(bool on) {
	glUniform1i(shadowsOn, on);
}

void StaticShader::set_light_color(f32 r, f32 g, f32 b, f32 a) {
    glUniform4f(lightColor, r, g, b, a);
}

void StaticShader::set_light_pos(f32 x, f32 y, f32 z) {
    glUniform3f(lightPos, x, y, z);
}

void StaticShader::set_projection(mat4 proj) {
    glUniformMatrix4fv(projection, 1, GL_FALSE, proj.elements);
}

void StaticShader::set_transform(mat4 tran) {
    glUniformMatrix4fv(transform, 1, GL_FALSE, tran.elements);
}

void StaticShader::set_view(mat4 view) {
    glUniformMatrix4fv(this->view, 1, GL_FALSE, view.elements);
}

void StaticShader::set_lightspace(mat4 LSM) {
    glUniformMatrix4fv(lightspace, 1, GL_FALSE, LSM.elements);
}
