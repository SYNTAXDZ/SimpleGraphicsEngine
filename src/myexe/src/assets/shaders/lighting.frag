/*
To summarize, gamma correction allows you to work/visualize your renders in linear space.
Because linear space makes sense in the physical world, most physical equations now actually give
good results like real light attenuation.
*/


#version 450
layout(location = 0) out vec4 color;

struct Light {

    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

struct Material {

    sampler2D diffuse;
    vec3 specular;

};

in vec2 v_TexCoords;
in vec3 v_Normals;
in vec3 v_FragPosition;

uniform Material material;
uniform Light light;

uniform vec3 u_ViewPos;
//uniform bool u_Blinn;

bool blinn = true;

void main() {
	
    vec3 ambient, diffuse, specular;

    vec3 normal = normalize( v_Normals );
    vec3 lightDir = normalize( light.position - v_FragPosition );
    vec3 viewDir = normalize( u_ViewPos - v_FragPosition );
    vec3 halfwayDir = normalize( lightDir + viewDir );

    float diff = max( dot( normal, lightDir ), 0.0f );
    diffuse = light.diffuse * diff * texture( material.diffuse, v_TexCoords ).rgb;

    ambient = light.ambient * texture( material.diffuse, v_TexCoords ).rgb;
    float spec = 0.0;

    if( blinn ) {

        spec = pow( max( dot( normal, halfwayDir ), 0.0 ), 32.0 );

    } else {

        vec3 reflectDir = reflect( -lightDir, normal );
        spec = pow( max( dot( normal, reflectDir ), 0.0 ), 8.0 );

    }

    specular = light.specular * spec * material.specular;

    float distance = length( light.position - v_FragPosition );
    float attenuation = 1.0/ (distance * distance);

    diffuse += attenuation;
    specular += attenuation;

    vec3 result = ambient + diffuse + specular;

    float gamma = 2.2;
    // appliying gamma correction ( back to linear space)
    vec3 color_srgb = pow( result, vec3( 1.0/gamma ) );
    color = vec4( color_srgb, 1.0 );

}