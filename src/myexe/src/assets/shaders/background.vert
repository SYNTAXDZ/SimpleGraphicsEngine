#version 450
layout( location = 0 ) in vec3 a_Pos;

/*
 Note the xyww trick here that ensures the depth value of the rendered cube fragments always 
 end up at 1.0, the maximum depth value, Do note that we need to change the depth comparison function to GL_LEQUAL
*/

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 v_WorldPos;

void main() {
    
    v_WorldPos = a_Pos;

	mat4 rotView = mat4( mat3( u_View ) );
	vec4 clipPos = u_Projection * rotView * vec4( v_WorldPos, 1.0 );

	gl_Position = clipPos.xyww;

}


