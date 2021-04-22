#include "shaders.h"

const int FOV = 90;
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
   char vShaderStr[] = R"foo(
attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

varying vec3 pass_pos;
varying vec3 pass_normal;

uniform mat4 projection;
uniform mat4 transform;
uniform mat4 view;

void main() {
    pass_pos = vec3(vec4(position, 1.0) * transform);
    //pass_pos = position;
    //pass_normal = transpose(inverse(mat3(transform))) * normal;
    pass_normal = vec3(transform * vec4(normal, 1.0));
    //gl_Position = projection * view * transform * vec4(position, 1.0);
    gl_Position = vec4(position, 1.0) * transform * view * projection;
}
)foo";

   char fShaderStr[] = R"foo(
precision mediump float;
varying vec3 pass_pos;
varying vec3 pass_normal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

void main() {
    vec3 normal = normalize(pass_normal);
    vec3 ambient = pow(0.75 * vec3(1.0, 1.0, 1.0), vec3(2.2));

    vec3 lightDir = normalize(lightPos - pass_pos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - pass_pos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    //spec = spec * spec * spec * spec * spec * spec * spec * spec;
    spec = spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec * spec;
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = (ambient + diffuse + specular) * lightColor;
    gl_FragColor = vec4(lighting, 1.0);
}
)foo";
	shader = load_shader_from_strings( vShaderStr, fShaderStr );

	//none of this (except for projection, view, and transform) is actually used yet, but it will be.
    start_shader(shader);
    upload_int(shader, "tex", 0);
    upload_int(shader, "depthbuffer", 1);
	shadowsOn = glGetUniformLocation(shader.ID, "shadowsOn");
    lightColor = glGetUniformLocation(shader.ID, "lightColor");
    lightPos = glGetUniformLocation(shader.ID, "lightPos");
    cameraPos = glGetUniformLocation(shader.ID, "cameraPos");
    projection = glGetUniformLocation(shader.ID, "projection");
    transform = glGetUniformLocation(shader.ID, "transform");
    view = glGetUniformLocation(shader.ID, "view");
    lightspace = glGetUniformLocation(shader.ID, "lightspace");

    glUniformMatrix4fv(projection, 1, GL_FALSE, (perspective_projection(90, 16.0f / 9.0f, 1.0f, 300.0f).elements));
	
	set_light_color(1.0, 1.0, 1.0);
	set_light_pos(-4, 24, -2);
	set_camera_pos(7, 7, 7);
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

void StaticShader::set_light_color(f32 r, f32 g, f32 b) {
    glUniform3f(lightColor, r, g, b);
}

void StaticShader::set_light_pos(f32 x, f32 y, f32 z) {
    glUniform3f(lightPos, x, y, z);
}

void StaticShader::set_camera_pos(f32 x, f32 y, f32 z) {
    glUniform3f(cameraPos, x, y, z);
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


//BILLBOARDS

void BillboardShader::load() {
    char vShaderStr[] = R"foo(
attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

varying vec2 pass_uv;

uniform mat4 projection;
uniform mat4 transform;
uniform mat4 view;

void main() {
    pass_uv = position.xy + vec2(0.5, 0.5);
    gl_Position = vec4(position, 1.0) * transform * view * projection;
}
)foo";

    char fShaderStr[] = R"foo(
precision mediump float;
varying vec2 pass_uv;
uniform sampler2D tex;
uniform vec4 tint;

void main() {
    vec4 tex = texture2D(tex, pass_uv);
    if(tex.a < 0.45)
        discard;

    gl_FragColor = tex * tint;
    //gl_FragColor = vec4(1, 1, 0, 1);
}
)foo";
    shader = load_shader_from_strings( vShaderStr, fShaderStr );

    start_shader(shader);
    upload_int(shader, "tex", 0);
    projection = glGetUniformLocation(shader.ID, "projection");
    transform = glGetUniformLocation(shader.ID, "transform");
    view = glGetUniformLocation(shader.ID, "view");
    tint = glGetUniformLocation(shader.ID, "tint");

    glUniformMatrix4fv(projection, 1, GL_FALSE, (perspective_projection(90, 16.0f / 9.0f, 1.0f, 300.0f).elements));

    set_transform(identity());
    set_view(identity());
    set_tint({25.0f/255.0f, 25.0f/255.0f, 25.0f/255.0f, 255.0f/255.0f});

    printf("billboard shader constructed\n");
}

void BillboardShader::dispose() {
    dispose_shader(shader);
}

void BillboardShader::bind() {
    start_shader(shader);
}

void BillboardShader::set_projection(mat4 proj) {
    glUniformMatrix4fv(projection, 1, GL_FALSE, proj.elements);
}

void BillboardShader::set_view(mat4 view) {
    glUniformMatrix4fv(this->view, 1, GL_FALSE, view.elements);
}

void BillboardShader::set_transform(mat4 transform) {
    glUniformMatrix4fv(this->transform, 1, GL_FALSE, transform.elements);
}

void BillboardShader::set_tint(vec4 tint) {
    glUniform4f(this->tint, tint.x, tint.y, tint.z, tint.w);
}