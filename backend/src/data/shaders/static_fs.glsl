#version 330 core

out vec4 color;

in vec2 pass_uv;
in vec3 pass_normal;
in vec3 pass_pos;
in vec4 pass_lightspace;

uniform sampler2D tex;
uniform sampler2D depthbuffer;

//uniform vec3 lightColor = vec3(0.5,  0.5, 0.7);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightPos = vec3(-4, 24, -2);

uniform int shadowsOn = 1;

//
//FUNCTIONS
//

float shadow_calculation(vec4 lightspace, vec3 lightDir) {
    vec3 projCoords = lightspace.xyz / lightspace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(depthbuffer, projCoords.xy).r;
    float currentDepth = projCoords.z;
    //float bias = max(0.05 * (1.0f - dot(pass_normal, lightDir)), 0.005f);
	float bias = 0.001;

    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(depthbuffer, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(depthbuffer, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0f;
    //shadow = currentDepth - 0.001 > closestDepth ? 1.0f : 0.0f;

    return shadow;
}

//
//   MAIN
//

void main() {
	vec3 normal = normalize(pass_normal);
    float gamma = 2.2;
    vec4 texColor = vec4(0.4, 0.4, 0.9, 1.0);
    //vec3 ambient = 1.10 * texColor.rgb;
    vec3 ambient = pow( 0.90 * texColor.rgb, vec3(gamma) );
    vec3 lightDir = normalize(lightPos - pass_pos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

	vec3 lighting;
	if(shadowsOn == 1) {
		float shadow = shadow_calculation(pass_lightspace, lightDir);
		lighting = (ambient + (1.0f - shadow) * (diffuse)) * texColor.rgb;
	}
    else {
		lighting = (ambient + diffuse) * texColor.rgb;
	}

    color = vec4(lighting, 1.0);
}
