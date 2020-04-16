#version 440 core

layout(location = 0) out vec4 color;
// out vec4 color;

in vec3 v_TexCoords;

uniform samplerCube skybox;

void main() {
	
    
    // dont't forget to flip the uv's
    color = texture( skybox, -1*v_TexCoords );

}
