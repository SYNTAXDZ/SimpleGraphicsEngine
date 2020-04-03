#version 440 core

layout(location = 0) out vec4 color;
// out vec4 color;

in vec3 v_TexCoords;

uniform samplerCube skybox;

void main() {
	
    //color = vec4( gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0 );
    
    color = texture( skybox, v_TexCoords );

}
