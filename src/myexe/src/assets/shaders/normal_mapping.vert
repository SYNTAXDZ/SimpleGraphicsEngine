#version 450
layout( location = 0 ) in vec3 a_Position;
layout( location = 1 ) in vec3 a_Normal;
layout( location = 2 ) in vec2 a_TexCoords;
layout( location = 3 ) in vec3 a_Tangent;
layout( location = 4 ) in vec3 a_Bitangent;

out VS_OUT {

    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    //mat3 TBN;

} vs_out;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform vec3 u_ViewPos;
uniform vec3 u_LightPos;

void main() {

    vs_out.FragPos = vec3( u_Model * vec4( a_Position, 1.0 ) );
    vs_out.TexCoords = a_TexCoords;

    vec3 T = normalize( vec3( u_Model * vec4( a_Tangent, 0.0 ) ) );
    //vec3 B = normalize( vec3( u_Model * vec4( a_Bitangent, 0.0 ) ) );
    vec3 N = normalize( vec3( u_Model * vec4( a_Normal, 0.0 ) ) );
    // we did this trick to always getting orthogonal vectros
    T = normalize( T - dot( T, N ) * N  );
    vec3 B = cross( N, T );
    /*we take the inverse of the TBN matrix to transform all relevant
      world-space vectors to the space the sampled normal 
      vectors are in: tangent space*/
    mat3 TBN = transpose( mat3( T, B, N ) );
    vs_out.TangentLightPos = TBN * u_LightPos;
    vs_out.TangentViewPos = TBN * u_ViewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos;
    
    gl_Position = u_Projection * u_View * u_Model * vec4( a_Position, 1.0 );

}
