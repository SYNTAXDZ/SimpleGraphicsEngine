/*
To summarize, gamma correction allows you to work/visualize your renders in linear space.
Because linear space makes sense in the physical world, most physical equations now actually give
good results like real light attenuation.
*/


#version 450
layout(location = 0) out vec4 color;

in VS_OUT {

    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    //mat3 TBN;

} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec3 u_ViewPos;
uniform vec3 u_LightPos;
//uniform bool u_Blinn;

bool blinn = false;

void main() {
	
    vec3 ambient, diffuse, specular;

    vec3 normals = texture( texture_normal1, fs_in.TexCoords ).rgb;
    normals = normalize( normals * 2.0 - 1.0 );
    //normals = normalize( fs_in.TBN * normals );
    
    vec3 lightDir = normalize( fs_in.TangentLightPos - fs_in.TangentFragPos );
    vec3 viewDir = normalize( fs_in.TangentViewPos - fs_in.TangentFragPos );
    vec3 halfwayDir = normalize( lightDir + viewDir );

    float diff = max( dot( normals, lightDir ), 0.0f );
    diffuse = vec3( 0.5 ) * diff * texture( texture_diffuse1, fs_in.TexCoords ).rgb;

    float spec = pow( max( dot( normals, halfwayDir ), 0.0 ), 32.0 );

    ambient = vec3( 0.1 ) * texture( texture_diffuse1, fs_in.TexCoords ).rgb;
    specular = vec3( 0.2 ) * spec;

    vec3 result = ambient + diffuse + specular;

    float gamma = 2.2;
    // appliying gamma correction ( back to linear space)
    //vec3 color_srgb = pow( result, vec3( 1.0/gamma ) );
    color = vec4( result, 1.0 );

}