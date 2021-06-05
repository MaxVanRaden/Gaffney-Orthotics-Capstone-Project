#ifndef SHADERS_H
#define SHADERS_H

#include "shader.h"
#include "texture.h"

class PickingShader {
    public:
        void load();
		void dispose();

        void bind();
        
        void set_flip(bool flip);
        void set_alpha(bool alpha);
        void set_pickID(vec4 ID);
        void set_projection(mat4 proj);
        void set_transform(mat4 tran);
        void set_view(mat4 view);

    private:
        Shader shader;

        GLint alpha;
        GLint pickID;
        GLint projection;
        GLint view;
        GLint transform;
        GLint flip;
};

class StaticShader {
    public:
        void load();
		void dispose();
		
        void bind();
        
		void set_shadows_on(bool on);
        void set_light_color(f32 r, f32 g, f32 b);
        void set_light_pos(f32 x, f32 y, f32 z);
        void set_camera_pos(f32 x, f32 y, f32 z);
        void set_projection(mat4 proj);
        void set_transform(mat4 tran);
        void set_view(mat4 view);
        void set_lightspace(mat4 LSM);
        void set_alpha(float alpha) const;
        void set_solid_color(bool show) const;

        void set_cross_section_top(float y) const;
        void set_cross_section_bot(float y) const;
        void set_show_cross_section(bool show) const;

    private:
        Shader shader;

		GLint shadowsOn;
        GLint lightColor;
        GLint lightPos;
        GLint cameraPos;
        GLint projection;
        GLint transform;
        GLint lightspace;
        GLint view;
        GLint alpha;
        GLint solidColor;

        GLint crossSectionBot;
        GLint crossSectionTop;
        GLint showCrossSection;
};

class BillboardShader {
public:
    void load();
    void dispose();

    void bind();

    void set_projection(mat4 proj);
    void set_view(mat4 view);
    void set_transform(mat4 transform);
    void set_tint(vec4 tint);

private:
    Shader shader;

    GLint projection;
    GLint view;
    GLint transform;
    GLint tint;
};

#endif 
