#version 440 core

layout(location = 0) out vec4 color;
// out vec4 color;

in vec2 v_TexCoords;

uniform sampler2D texture_sampler;

void main() {
	
    //color = vec4( 0.5, 0.6, 0.5, 1.0 );
    color = texture( texture_sampler, v_TexCoords );

}