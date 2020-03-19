#type vertex
#version 440 core
layout(location = 0) in vec3 a_Position;
layout( location = 1 ) in vec2 a_TexCoords;
//uniform mat4 u_ViewProjection;
//uniform mat4 u_Transform;
out vec2 TexCoords;

void main() {
	//gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    TexCoords = a_TexCoords;
    gl_Position = vec4( a_Position, 1.0 );
}
#type fragment
#version 440 core

layout(location = 0) out vec4 color;

//uniform vec4 u_Color;

in vec2 TexCoords;

uniform sampler2D texture_sampler;

void main() {
	
    //color = u_Color;
    color = texture( texture_sampler, TexCoords );

}