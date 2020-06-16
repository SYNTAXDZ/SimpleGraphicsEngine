#version 450
layout( location = 0 ) in vec3 a_Pos;
layout( location = 1 ) in vec2 a_TexCoords;
layout( location = 2 ) in vec3 a_Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;


out vec2 v_TexCoords;
out vec3 v_WorldPos;
out vec3 v_Normal;

void main() {

    v_Normal = mat3( u_Model ) * a_Normal;
    v_TexCoords = a_TexCoords;
    v_WorldPos = vec3( u_Model * vec4( a_Pos,1.0 ) );

    gl_Position = u_Projection * u_View * vec4( v_WorldPos, 1.0 );

}