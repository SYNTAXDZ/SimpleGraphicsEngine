#version 450
out vec4 FragColor;

in vec2 v_TexCoords;
in vec3 v_WorldPos;
in vec3 v_Normal;

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

/*
 The irradiance map represents the diffuse part of the reflectance integral as accumulated from
 all surrounding indirect light. Seeing as the light doesn’t come from any direct light sources,
 but from the surrounding environment we treat both the diffuse and specular indirect lighting
 as the ambient lighting
*/
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 u_CamPos;

const float PI = 3.14159265359;


/*
 we pass the roughness parameter directly to these functions; this way we can make some 
 term-specific modifications to the original roughness value. Based on observations by Disney
 and adopted by Epic Games the lighting looks more correct squaring the roughness in both
 the geometry and normal distribution function.
*/
// ----------------------------------------------------------------------------
float DistributionGGX( vec3 N, vec3 H, float roughness ) {
    
    // squaring the roughness makes the light more realistic
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max( dot( N, H ), 0.0 );
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = ( NdotH2 * ( a2 - 1.0 ) + 1.0 );
    denom = PI * denom * denom;

    return nom / max( denom, 0.001 ); // prevent divide by zero for roughness=0.0 and NdotH=1.0

}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX( float NdotV, float roughness ) {
    
    float r = ( roughness + 1.0 );
    float k = ( r * r ) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * ( 1.0 - k ) + k;

    return nom / denom;

}
// ----------------------------------------------------------------------------
float GeometrySmith( vec3 N, vec3 V, vec3 L, float roughness ) {
    
    float NdotV = max( dot( N, V ), 0.0 );
    float NdotL = max( dot( N, L ), 0.0 );
    float ggx2 = GeometrySchlickGGX( NdotV, roughness );
    float ggx1 = GeometrySchlickGGX( NdotL, roughness );

    return ggx1 * ggx2;

}
// ----------------------------------------------------------------------------
/*
 we want to do is calculate the ratio between specular and diffuse reflection, or how much
 the surface reflects light versus how much it refracts light. We know from the previous tutorial that the
 Fresnel equation calculates just that; The Fresnel equation returns the ratio of light that gets reflected
 on a surface which we find as k_S in the reflectance equation. The Fresnel-Schlick approximation expects
 a F0 parameter which is known as the surface reflection at zero incidence or how much the surface 
 reflects if looking directly at the surface
*/
vec3 fresnelSchlick( float cosTheta, vec3 F0 ) {
    /*
     The F0 varies per material and is tinted on metals as we find in large material databases. In the PBR metallic
     workflow we make the simplifying assumption that most dielectric surfaces look visually correct with a
     constant F0 of 0.04 while we do specify F0 for metallic surfaces as then given by the albedo value.
    */
    return F0 + ( 1.0 - F0 ) * pow( 1.0 - cosTheta, 5.0 );

}

/*
 To still simulate Fresnel, we calculate the Fresnel from the angle between the normal and view vector.
 However, earlier we used the microsurface halfway vector, influenced by the roughness of the surface,
 as input to the Fresnel equation. As we currently don’t take any roughness into account, the surface’s
 reflective ratio will always end up relatively high. Indirect light follows the same properties of direct
 light so we expect rougher surfaces to reflect less strongly on the surface edges. As we don’t take the surface’s roughness into account
*/
vec3 fresnelSchlickRoughness( float cosTheta, vec3 F0, float roughness ) {

    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);

}

// ----------------------------------------------------------------------------
void main() {

    // Normal Vector		
    vec3 N = normalize(v_Normal);
    // View Vector/Eye Vector
    vec3 V = normalize(u_CamPos - v_WorldPos);

    vec3 R = reflect( -V, N );
    /*
     As you can see, for non-metallic surfaces F0 is always 0.04, while we do vary F0 
     based on the metalness of a surface by linearly interpolating between the original
     F0 and the albedo value given the metallic property.
    */    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3( 0.0 );
    // thi loop represent the integral in the hemisphere
    for( int i = 0; i < 4; ++i ) {
        
        //------------ calculate per-light radiance
        // the lightDir
        vec3 L = normalize( lightPositions[i] - v_WorldPos );
        // the Half way vector
        vec3 H = normalize( V + L );
        // distance between the light pos and the fragment
        float distance = length( lightPositions[i] - v_WorldPos );
        // (dimming of light over distance)
        float attenuation = 1.0 / ( distance * distance );
        // get the invidual radiance for light sources
        vec3 radiance = lightColors[i] * attenuation;

        /*
         for each light we want to calculate the full Cook-Torrance specular BRDF term
         DFG / 4(ω_o · n)(ω_i · n)
        */
        // Calc The NormalDistributionFunction
        float NDF = DistributionGGX(N, H, roughness);
        // Calc The Geometry Function   
        float G   = GeometrySmith(N, V, L, roughness);
        // Calc The FresnelSchlick Function   
        vec3 F    = fresnelSchlick( max( dot( H, V ), 0.0 ), F0 );
        
        // calculate the Cook-Torrance BRDF  
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        // BRDF term:
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
        /*
         Now we can finally calculate each light’s contribution to the reflectance equation.
         As the Fresnel value directly corresponds to k_S we can use F to denote the specular
         contribution of any light that hits the surface. From k_S we can then directly 
         calculate the ratio of refraction k_D.
        */
        // Seeing as kS represents the energy of light that gets reflected
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component ( kD ) should equal 1.0 - kS.
        // the remaining ratio of light energy is the light that gets refracted which we store as kD
        vec3 kD = vec3( 1.0 ) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        // because metallic surfaces don’t refract light and thus have no diffuse reflections
        // we enforce this property by nullifying kD if the surface is metallic.
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max( dot( N, L ), 0.0 );        

        /*
         The R resulting Lo value, or the outgoing radiance, is effectively the result of 
         the reflectance equation’s integral over Ω. We don’t really have to try and solve
         the integral for all possible incoming light directions as we know exactly the 4 incoming
         light directions that can influence the fragment. Because of this, we can
         directly loop over these incoming light directions e.g. the number of lights in the scene.
         It’s important to note that we left kS out of the equation. We already have a multiplication
         of the Fresnel F in the specular BRDF and seeing as kS equals F there’s no need 
         to multiply by the Fresnel again.
        */
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    
    }   

    /*
     the indirect lighting contains both a diffuse and specular part as we’ve seen from the split
     version of the reflectance equation we need to weigh the diffuse part accordingly. 
     Similar to what we did in the previous tutorial we use the Fresnel equation to determine 
     the surface’s indirect reflectance ratio from which we derive the refractive or diffuse ratio
    */   
    vec3 F = fresnelSchlickRoughness( max( dot( N, V ), 0.0 ), F0, roughness );
    
    vec3 kS = F;
    
    vec3 kD = 1.0 - kS;
    vec3 irradiance = texture( irradianceMap, N ).rgb;
    vec3 diffuse = irradiance * albedo;
    
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod( prefilterMap, R,  roughness * MAX_REFLECTION_LOD ).rgb;
    vec2 brdf  = texture (brdfLUT, vec2( max( dot( N, V ), 0.0 ), roughness ) ).rg;
    vec3 specular = prefilteredColor * ( F * brdf.x + brdf.y );

    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;

    /*
     So far we’ve assumed all our calculations to be in linear color space and to account for this we need to
     gamma correct at the end of the shader. Calculating lighting in linear space is incredibly important as PBR
     requires all inputs to be linear, not taking this into account will result in incorrect lighting. Additionally, we
     want light inputs to be close to their physical equivalents such that their radiance or color values can vary
     wildly over a high spectrum of values. As a result Lo can rapidly grow really high which then gets clamped
     between 0.0 and 1.0 due to the default low dynamic range (LDR) output. We fix this by taking Lo and
     tone or exposure map the high dynamic range (HDR) value correctly to LDR before gamma correction
    */
    color = color / ( color + vec3( 1.0 ) );
    /*
     Here we tone map the HDR color using the Reinhard operator, preserving the high dynamic range
     of possibly highly varying irradiance after which we gamma correct the color. We don’t have a separate
     framebuffer or post-processing stage so we can directly apply both the tone mapping step and gamma
     correction step directly at the end of the forward fragment shader.
    */
    color = pow( color, vec3( 1.0/2.2 ) ); 
    // final step to get the colors
    FragColor = vec4( color, 1.0 );

}
