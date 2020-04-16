#version 440 core
layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main() {
    
    //v_TexCoords = a_Position;
    v_TexCoords = vec3( u_Model * vec4( a_Position, 1.0 ) );
    vec4 pos = u_Projection * u_View * u_Model * vec4( a_Position, 1.0 );
    // make sure the skybox is the last drawn ( z = 1.0 )
    gl_Position = pos.xyww;

}
