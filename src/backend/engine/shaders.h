#ifndef SHADERS_H
#define SHADERS_H

#include "shader.h"
#include "texture.h"

class OrthoShader {
    public:
        void load();
		void dispose();
		
        void bind();

        void set_projection(mat4 proj);
        void set_transform(mat4 tran);

    private:
        Shader shader;
		
        GLint projection;
        GLint transform;
};

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
        void set_light_color(f32 r, f32 g, f32 b, f32 a);
        void set_light_pos(f32 x, f32 y, f32 z);
        void set_projection(mat4 proj);
        void set_transform(mat4 tran);
        void set_view(mat4 view);
        void set_lightspace(mat4 LSM);

    private:
        Shader shader;

		GLint shadowsOn;
        GLint lightColor;
        GLint lightPos;
        GLint projection;
        GLint transform;
        GLint lightspace;
        GLint view;
};  

#endif 
