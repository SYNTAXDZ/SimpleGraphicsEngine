#version 440 core

layout(location = 0) out vec4 color;

/*
 We sample the environment map using its interpolated vertex cube positions that directly 
 correspond to the correct direction vector to sample. Seeing as the camera’s translation
 components are ignored, rendering this shader over a cube should give you the environment
 map as a non-moving background. Also, note that as we directly output the environment map’s
 HDR values to the default LDR framebuffer we want to properly tone map the color values.
 Furthermore, almost all HDR maps are in linear color space by default so we need to apply
 gamma correction before writing to the default framebuffer.
*/

in vec3 v_TexCoords;
in vec3 v_WorldPos;

uniform samplerCube skybox;

void main() {
	
    
    // dont't forget to flip the uv's
    color = texture( skybox, -1*v_TexCoords );

}
