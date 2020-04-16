#pragma once

#include <string>
#include <glm/glm.hpp>

inline std::string vertexSrc = R"(
    #version 330 core
    
    layout(location = 0) in vec3 a_Position;
    layout( location = 1 ) in vec3 a_Normals;
    layout( location = 2 ) in vec2 a_TexCoords;
    
    uniform mat4 u_Projection;
    uniform mat4 u_View;
    uniform mat4 u_Model;
    // the position of our fragments 
    out vec3 v_FragPos;
    out vec3 v_Normals;
    out vec2 v_TexCoords;
    
    void main() {
        
        v_FragPos = vec3( u_Model * vec4( a_Position, 1.0 ) );
        v_Normals = mat3( transpose( inverse( u_Model ) ) ) * a_Normals;
        v_TexCoords = a_TexCoords;
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	
    }
)";

inline std::string fragmentSrc = R"(
    #version 330 core

    layout(location = 0) out vec4 color;
    
    struct Material {
        
        sampler2D diffuse;
        sampler2D specular;
        float     shininess;
    
    };
    
    struct Light {
    
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        
        float constant;
        float linear;
        float quadratic;
    
    };
    
    uniform Material material;
    uniform Light light;
    // this is the camera position
    uniform vec3 viewPos;
    
    in vec3 v_Normals;
    in vec3 v_FragPos;
    in vec2 v_TexCoords;    
    
    void main() {
            
        // ambient is a simplistic model of global illumination, making it look like there 
        // is always some scattered light even when there’s not a direct light source.
        vec3 ambient = light.ambient * texture( material.diffuse, v_TexCoords ).rgb;

        // norm is normal vector that is a vector perpendicular to the fragment’s surface
        vec3 norm = normalize( v_Normals );
        // make sure you always normalize the relevant vectors to ensure they’re actual unit vectors
        vec3 lightDir = normalize( light.position - v_FragPos );
        // to get (only) the cosine of the angle between both vectors we will work with
        // unit vectors (vectors of length 1) so we need to make sure all the vectors are normalized,
        // otherwise the dot product returns more than just the cosine, here the max() func ensure that
        // the dot product is always positive, exm: max( value, 0.0f )
        float diff = max( dot( norm, lightDir ), 0.0f );
        // Diffuse lighting gives the object more brightness the closer its fragments are 
        // aligned to the light rays from a light source
        vec3 diffuse = light.diffuse * (diff * texture( material.diffuse, v_TexCoords ).rgb );

        // define a specular intensity value to give the specular highlight a medium-bright color
        //float specularStrength = 0.5f;
        vec3 viewDir = normalize( viewPos - v_FragPos );
        // Note that we negate the lightDir vector. The reflect function expects the first vector to
        // point from the light source towards the fragment’s position, but the lightDir vector is currently
        // pointing the other way around from the fragment towards the light source, and the second arg is the normal
        vec3 reflectDir = reflect( -lightDir, norm );
        // We first calculate the dot product between the view direction and the reflect direction (and make
        // sure it’s not negative) and then raise it to the power of 32. This 32 value is the shininess value of
        // the highlight, The higher the shininess value of an object, the more it properly reflects the light
        float spec = pow( max( dot( viewDir, reflectDir ), 0.0f ), material.shininess );
        vec3 specular = light.specular * ( spec * texture( material.specular, v_TexCoords ).rgb );

        /*
        The following formula divides the
        light’s intensity of a fragment by a calculated attenuation value based on the distance to the light source:
        F_att = I / K_c + K_l ∗ d + K_q ∗ d 2

        Here I is the intensity of the light at the current fragment and d represents the distance from
        the fragment to the light source. Then to calculate the attenuation value we define 3 (configurable)
        terms: a constant term K_c, a linear term K_l and a quadratic term K_q.
        */
        // We do need the distance to the light source for the formula to work though
        float distance = length( light.position - v_FragPos );
        /*
        The constant term is usually kept at 1.0 which is mainly there to make sure the resulting
        denominator never gets smaller than 1 since it would otherwise boost the intensity with
        certain distances, which is not the effect we’re looking for.
    •   The linear term is multiplied with the distance value that reduces the intensity in a linear
        fashion.
    •   The quadratic term is multiplied with the quadrant of the distance and sets a quadratic decrease
        of intensity for the light source. The quadratic term will be less significant compared to
        the linear term when the distance is small, but gets much larger than the linear term as the distance grows.
        */
        float attenuation = 1.0f/ ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

        // Then we include this attenuation value in the lighting calculations by multiplying the attenuation
        // value with the ambient, diffuse and specular colors
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        
        
        vec3 result = ( ambient + diffuse + specular );
            
        color = vec4( result, 1.0 );
    }
)";

// Set up vertex data (and buffer(s)) and attribute pointers
inline float vertices[] = {

    // Positions            // Normals              // Texture Coords
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f

};

inline float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

inline float planeVertices[] = {

        // positions                            // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         1.0f, -0.5f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -1.0f, -0.5f,  1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -1.0f, -0.5f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

         1.0f, -0.5f,  1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -1.0f, -0.5f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -0.5f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f

};
inline float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
// Positions all containers
inline glm::vec3 cubePositions[] = {
        
        glm::vec3(  1.0f,   0.0f,   0.0f ),
        glm::vec3(  2.0f,   5.0f,   -15.0f ),
        glm::vec3(  -1.5f,  -2.2f,  -2.5f ),
        glm::vec3(  -3.8f,  -2.0f,  -12.3f ),
        glm::vec3(  2.4f,   -0.4f,  -3.5f ),
        glm::vec3(  -1.7f,  3.0f,   -7.5f ),
        glm::vec3(  1.3f,   -2.0f,  -2.5f ),
        glm::vec3(  1.5f,   2.0f,   -2.5f ),
        glm::vec3(  1.5f,   0.2f,   -1.5f ),
        glm::vec3(  -1.3f,  1.0f,   -1.5f )
    
};

inline std::string modelVertexSrc = R"(
    #version 330 core
    
    layout(location = 0) in vec3 a_Position;
    layout( location = 1 ) in vec3 a_Normals;
    layout( location = 2 ) in vec2 a_TexCoords;
    
    uniform mat4 u_Projection;
    uniform mat4 u_View;
    uniform mat4 u_Model;
    // the position of our fragments 
    out vec3 v_FragPos;
    out vec3 v_Normals;
    out vec2 v_TexCoords;
    
    void main() {
        
        v_FragPos = vec3( u_Model * vec4( a_Position, 1.0 ) );
        v_Normals = mat3( transpose( inverse( u_Model ) ) ) * a_Normals;
        v_TexCoords = a_TexCoords;
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);	
    }
)";

inline std::string modelFragmentSrc = R"(
    #version 330 core

    layout(location = 0) out vec4 color;
    
    in vec3 v_Normals;
    in vec3 v_FragPos;
    in vec2 v_TexCoords;    
    
    uniform sampler2D texture_diffuse1;
    //uniform sampler2D texture_specular1;
    
    void main() {
            
            
        color = texture(texture_diffuse1, v_TexCoords);
    }
)";
