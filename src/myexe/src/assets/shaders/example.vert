#version 440 core
layout(location = 0) in vec3 a_Position;
layout( location = 1 ) in vec3 a_Normals;
//layout( location = 2 ) in vec2 a_TexCoords;

//out vec2 v_TexCoords;
out vec3 v_Normals;
out vec3 v_Position;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main() {
    
    v_Position = vec3( u_Model * vec4( a_Position, 1.0 ) );
    v_Normals = mat3( transpose( inverse( u_Model ) ) ) * a_Normals;
    //v_TexCoords = a_TexCoords;
    gl_Position = u_Projection * u_View * u_Model * vec4( a_Position, 1.0 );

}
