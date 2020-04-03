#version 440 core

layout(location = 0) out vec4 color;
// out vec4 color;

in vec2 v_TexCoords;
in vec3 v_Normals;

uniform sampler2D texture_sampler;

void main() {
	
    //color = vec4( gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0 );
    
    color = texture( texture_sampler, v_TexCoords );

}
