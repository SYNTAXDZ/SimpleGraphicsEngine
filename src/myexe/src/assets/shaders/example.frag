#version 440 core

layout(location = 0) out vec4 color;
// out vec4 color;

//in vec2 v_TexCoords;
in vec3 v_Normals;
in vec3 v_Position;

//uniform sampler2D texture_sampler;
uniform samplerCube u_SkyBox;
uniform vec3 u_CameraPos;

void main() {
	
    vec3 I = normalize( v_Position - u_CameraPos );
    
    vec3 R = reflect( I, normalize( v_Normals ) );
    
    color = vec4( texture( u_SkyBox, -1.0*R ).rgb, 1.0 );

}
