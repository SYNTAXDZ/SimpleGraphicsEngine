#version 440
layout( location = 0 ) in vec3 aPos;
layout( location = 1 ) in vec3 aNormal;
layout( location = 2 ) in vec2 aTexCoords;

out VS_OUT {
    
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

} vs_out;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform bool inverse_normals;

void main() {
    
    vs_out.FragPos = vec3( u_Model * vec4( aPos, 1.0 ) );   
    vs_out.TexCoords = aTexCoords;
    
    vec3 n = inverse_normals ? -aNormal : aNormal;
    
    mat3 normalMatrix = transpose( inverse( mat3( u_Model ) ) );
    vs_out.Normal = normalize( normalMatrix * n );
    
    gl_Position = u_Projection * u_View * u_Model * vec4( aPos, 1.0 );

}
