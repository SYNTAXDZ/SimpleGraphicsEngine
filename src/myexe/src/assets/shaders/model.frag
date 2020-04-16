#version 440 core

layout(location = 0) out vec4 color;
// out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D texture_diffuse1;
//uniform sampler2D skybox;


void main() {
	
    
    // dont't forget to flip the uv's
    color = texture( texture_diffuse1, v_TexCoords );

}
