#version 450

/*
 to generate the irradiance map we need to convolute the environment’s lighting as converted to
 a cubemap. Given that for each fragment the surface’s hemisphere is oriented along the normal 
 vector N, convoluting a cubemap equals calculating the total averaged radiance of each direction
 w_i in the hemisphere Ω oriented along N.

 Thankfully, we can directly take the converted cubemap, convolute it in a fragment shader and capture
 its result in a new cubemap using a framebuffer that renders to all 6 face directions. As we’ve
 already set this up for converting the equirectangular environment map to a cubemap, we can take
 the exact same approach but use a different fragment shader
*/

out vec4 FragColor;

in vec3 v_WorldPos;

/*
 the HDR cubemap as converted from the equirectangular HDR environment map.
*/
uniform samplerCube environmentMap;

const float PI = 3.14159265359;
/*
 we’re going to generate a fixed amount of sample vectors for each cubemap texel along a hemisphere
 Ω oriented around the sample direction and average the results. The fixed amount of sample vectors
 will be uniformly spread inside the hemisphere. Note that an integral is a continuous function 
 and discretely sampling its function given a fixed amount of sample vectors will be an approximation.
 The more sample vectors we use, the better we approximate the integral
*/
void main() {

    // the sample direction equals the hemisphere’s orientation
    vec3 normal = normalize( v_WorldPos );

    vec3 irradiance = vec3( 0.0 );

    vec3 up = vec3( 0.0, 1.0, 0.0 );
    vec3 right = cross( up, normal );
    up = cross( normal, right );
    
    /*
     We specify a fixed sampleDelta delta value to traverse the hemisphere; decreasing or increasing
     the sample delta will increase or decrease the accuracy respectively
    */
    float sampleDelta = 0.025;
    /*
     Solving the integral requires us to take a fixed number of discrete samples within the
     hemisphere Ω and averaging their results
    */
    float nrSamples = 0.0;
    
    /*
     The integral of the reflectance equation revolves around the solid angle dw which is rather
     difficult to work with. Instead of integrating over the solid angle dw we’ll integrate over
     its equivalent spherical coordinates θ and φ.
     We use the polar azimuth φ angle to sample around the ring of the hemisphere between 0 and 2π,
     and use the inclination zenith θ angle between 0 and 12 π to sample the increasing rings of the hemisphere.
    */
    for( float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta ) {
        /*
         As we sample both spherical values discretely, each sample will approximate or average an
         area on the hemisphere. Note that (due to the general properties of a spherical shape)
         the hemisphere’s discrete sample area gets smaller the higher the zenith angle θ as 
         the sample regions converge towards the center top. To compensate for the smaller areas,
         we weigh its contribution by scaling the area by sin θ clarifying the added sin
        */
        for( float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta ) {
            
            /* Discretely sampling the hemisphere given the integral’s spherical coordinates for each fragment invocation */

            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3( sin( theta ) * cos( phi ), sin( theta ) * sin( phi ), cos( theta ) );
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;
            
            irradiance += texture( environmentMap, sampleVec ).rgb * cos( theta ) * sin( theta );

            nrSamples++;
        
        }
    
    }

    /*
     From within both loops, we take both spherical coordinates to convert them to a 3D Cartesian
     sample vector, convert the sample from tangent to world space and use this sample vector to directly
     sample the HDR environment map. We add each sample result to irradiance which at the end we 
     divide by the total number of samples taken, giving us the average sampled irradiance.
     Note that we scale the sampled color value by cos(theta) due to the light being weaker at larger
     angles and by sin(theta) to account for the smaller sample areas in the higher hemisphere areas.
    */
    irradiance = PI * irradiance * (1.0 / float( nrSamples ) );

    FragColor = vec4( irradiance, 1.0 );

}