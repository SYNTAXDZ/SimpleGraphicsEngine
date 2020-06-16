#version 440 core

/*
 To convert an equirectangular image into a cubemap we need to render a (unit) cube and project
 the equirectangular map on all of the cube’s faces from the inside and take 6 images of each of
 the cube’s sides as a cubemap face. The vertex shader of this cube simply renders the cube as is
 and passes its local position to the fragment shader as a 3D sample vector
*/

out vec4 FragColor;
in vec3 v_WorldPos;

/*
 For the fragment shader we color each part of the cube as if we neatly folded the 
 equirectangular map onto each side of the cube. To accomplish this, we take the fragment’s
 sample direction as interpolated from the cube’s local position and then use this direction
 vector and some trigonometry magic to sample the equirectangular map as if it’s a cubemap
 itself. We directly store the result onto the cube-face’s fragment which should be all we need to do
*/
uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap( vec3 v ) {
    
    vec2 uv = vec2( atan( v.z, v.x ), asin( v.y ) );
    uv *= invAtan;
    uv += 0.5;
    
    return uv;

    }

void main() {		
    
    vec2 uv = SampleSphericalMap( normalize( v_WorldPos ) );
    vec3 color = texture( equirectangularMap, uv ).rgb;
    
    FragColor = vec4( color, 1.0 );

}


