#version 440 core
layout( location = 0 ) out vec4 a_Color;

in vec2 v_TexCoords;

uniform sampler2D screen_texture;

void main() {

    //a_Color = vec4( vec3(  1.0 - texture( screen_texture, v_TexCoords ) ), 1.0 );
    a_Color = texture( screen_texture, v_TexCoords );
    
}
