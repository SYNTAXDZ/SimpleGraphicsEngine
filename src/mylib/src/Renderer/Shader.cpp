#include "../../pch/pch.hpp"

#include "Shader.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>


using namespace ENGINE;

static GLenum ShaderTypeFromString( const std::string& type ) {

    if( type == "vertex" ) return GL_VERTEX_SHADER;
    if( type == "fragment" ) return GL_FRAGMENT_SHADER;

    return 0;

}
Shader::Shader( const std::string& filepath ) {

    std::string source = ReadFile( filepath );
    auto shaderSources = PreProcess( source );

    Compile( shaderSources );

    ///------- to extract name from file path
    // find the last Slash ( / or \)
    auto lastSlash = filepath.find_last_of( "/" );
    // if lastSlash equal to npos (npos mean don't exists) return 0 else return the string after the Slash
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    // find the last dot
    auto lastDot = filepath.rfind( "." );
    // the count is the size of filepath - the lastSlash if they have no extension in the file
    // else count is the size between lasDot - lastSlash
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    // the Name is the sub string from lastSlash to the count
    m_Name = filepath.substr( lastSlash, count );


}

Shader::Shader( const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc )
    : m_Name( name ) {

    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;

    Compile( sources );

}

Shader::~Shader() {

    glDeleteProgram( m_RendererID );

}

void Shader::Compile( const std::unordered_map<GLenum, std::string>& shaderSource ) {

    std::array<GLenum, 2> shaderIDs;

    int shaderIDIndex = 0;

    GLuint program = glCreateProgram();

    for( auto& key_value : shaderSource ) {

        GLenum shader_type = key_value.first;
        const std::string& shader_source = key_value.second;

        GLint shader = glCreateShader( shader_type );

        const GLchar* source = shader_source.c_str();
        glShaderSource( shader, 1, &source, 0 );

        glCompileShader( shader );

        GLint isCompiled = 0;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );

        if( isCompiled == GL_FALSE ) {

            GLint maxLength = 0;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

            // The maxLength includes the NULL character
      std::vector<GLchar> infoLog( maxLength );
      glGetShaderInfoLog( shader, maxLength, &maxLength, &infoLog[0] );
      // We don't need the shader anymore.
      glDeleteShader( shader );

            break;

        }

        glAttachShader( program, shader );
        shaderIDs[shaderIDIndex++] = shader;

    }

    glLinkProgram( program );

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv( program, GL_LINK_STATUS, ( int* )&isLinked );

    if( isLinked == GL_FALSE ) {

        GLint maxLength = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog( maxLength );
        glGetProgramInfoLog( program, maxLength, &maxLength, &infoLog[0] );

        // We don't need the program anymore.
        glDeleteProgram( program );

        // Don't leak shaders either.
        for( auto id : shaderIDs )
            glDeleteShader( id );

        return;

    }

    for( auto id : shaderIDs )
        glDetachShader( program, id );

    m_RendererID = program;

}

std::string Shader::ReadFile( const std::string& filepath ) {

    std::string result;

    std::ifstream in( filepath, std::ios::in | std::ios::binary );

    if( in ) {

        in.seekg( 0, std::ios::end );
        result.resize( in.tellg() );
        in.seekg( 0, std::ios::beg );
        in.read( &result[0], result.size() );
        in.close();

    } else {

        std::cout<< "Could not open the file \n";

    }

    return result;
    
}

// PreProcess the Shader Source
std::unordered_map<GLenum, std::string> Shader::PreProcess( const std::string& source ) {

    std::unordered_map<GLenum, std::string> shaderSources;

    // the token type, it mean after this token is the type of the shader
    const char* typeToken = "#type";
    size_t typeTokenLen = strlen( typeToken );
    // find in our all shader source code for the typeToken
    size_t pos = source.find( typeToken, 0 );  //Start of shader type declaration line

    // while we keep finding it
    while( pos != std::string::npos ) {

        // find the first instance of the new line, will give us the pos of the new line
        size_t eol = source.find_first_of( "\r\n", pos );  //End of shader type declaration line
        // if we don't have a new line ASSERT a Syntax Error
        //HZ_CORE_ASSERT( eol != std::string::npos, "Syntax Error" );
        // get the begining of our type
        size_t begin = pos + typeTokenLen + 1; //Start of shader type name (after "#type " keyword)
        // the type is simply the sub string between begin and begin minus end of the line.
        std::string type = source.substr( begin, eol - begin );
        //HZ_CORE_ASSERT( ShaderTypeFromString( type ), "Invalid Shader type Specified" );

        // nextLine is actually the first new line after 'eol'
        size_t nextLinePos = source.find_first_not_of( "\r\n", eol ); //Start of shader code after shader type declaration line
        // find the next type token in the new line
        pos = source.find( typeToken, nextLinePos );

        // get the sub string from the newLinePos to the end of the file or just the actual line pos
        //shaderSources[ShaderTypeFromString( type )] = 
        //source.substr( nextLinePos, pos - ( nextLinePos == std::string::npos? source.size() - 1 : nextLinePos ) );

        shaderSources[ShaderTypeFromString( type )] =
				( pos == std::string::npos ) ? source.substr( nextLinePos ) : source.substr( nextLinePos, pos - nextLinePos );

    }

    return shaderSources;
   
}

void Shader::Bind() const {

    glUseProgram( m_RendererID );

}

void Shader::UnBind() const {

        glUseProgram( 0 );

}

void Shader::UploadUniformInt( const std::string& name, int value ) {

    GLint location = glGetUniformLocation( m_RendererID, name.c_str() );

    glUniform1i( location, value );

}

void Shader::UploadUniformFloat( const std::string& name, float value ) {

    GLint location = glGetUniformLocation( m_RendererID, name.c_str() );
  glUniform1f( location, value );

}

void Shader::UploadUniformFloat2( const std::string& name, const glm::vec2& value ) {

    GLint location = glGetUniformLocation( m_RendererID, name.c_str() );
  glUniform2f( location, value.x, value.y );

}

void Shader::UploadUniformFloat3( const std::string& name, const glm::vec3& value ) {

    GLint location = glGetUniformLocation( m_RendererID, name.c_str() );
  glUniform3f( location, value.x, value.y, value.z );

}

void Shader::UploadUniformFloat4( const std::string& name, const glm::vec4& value ) {

    GLint location = glGetUniformLocation( m_RendererID, name.c_str() );
  glUniform4f( location, value.x, value.y, value.z, value.w );

}

void Shader::UploadUniformMat3( const std::string& name, const glm::mat3& matrix ) {

    GLint location = glGetUniformLocation( m_RendererID, name.c_str() );
    glUniformMatrix3fv( location, 1, GL_FALSE, glm::value_ptr( matrix ) );

}

void Shader::UploadUniformMat4( const std::string& name, const glm::mat4& matrix ) {

    GLint location = glGetUniformLocation( m_RendererID, name.c_str() );
    glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( matrix ) );

}


std::shared_ptr<Shader> Shader::Create( const std::string& filepath ) {

    return std::make_shared<Shader>( filepath );

}

std::shared_ptr<Shader>  Shader::Create( const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc ) {

    return std::make_shared<Shader>( name, vertexSrc, fragmentSrc );

}



/*
Shader::Shader( const char *vertexPath, const char *fragmentPath, const char *geometrypath ) {

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    std::ifstream geometryShaderFile;

    vertexShaderFile.exceptions( std::ifstream::badbit );
    fragmentShaderFile.exceptions( std::ifstream::badbit );
    geometryShaderFile.exceptions( std::ifstream::badbit );

    try {

        vertexShaderFile.open( vertexPath );
        fragmentShaderFile.open( fragmentPath );
        geometryShaderFile.open( geometrypath );

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if( geometrypath != nullptr ) {

            geometryShaderFile.open( geometrypath );
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();

            geometryCode = gShaderStream.str();


        }

    } catch( const std::ifstream::failure& error ) {

        std::cout << "ERROR::FILE_NOT_READED_SUCCESSFULY \n";

    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    unsigned int vertex, fragment, geometry;
    int success;
    char infoLog[512];

    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, NULL );
    glCompileShader( vertex );

    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );

    if( !success ) {

        glGetShaderInfoLog( vertex, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    }

    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, NULL );
    glCompileShader( fragment );

    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );

    if( !success ) {

        glGetShaderInfoLog( fragment, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

    }

    if( geometrypath != nullptr ) {

        geometry = glCreateShader( GL_GEOMETRY_SHADER );
        glShaderSource( geometry, 1, &gShaderCode, NULL );
        glCompileShader( geometry );

        glGetShaderiv( geometry, GL_COMPILE_STATUS, &success );

        if( !success ) {

            glGetShaderInfoLog( geometry, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;

        }

    }


    m_RendererID = glCreateProgram();
    glAttachShader( m_RendererID, vertex );
    glAttachShader( m_RendererID, fragment );
    
    if( geometrypath != nullptr )
        glAttachShader( m_RendererID, geometry );
    
    glLinkProgram( m_RendererID );

    glGetProgramiv( m_RendererID, GL_LINK_STATUS, &success );

    if( !success ) {

        glGetProgramInfoLog( m_RendererID, 512, NULL, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

    }

    glDeleteShader( vertex );
    glDeleteShader( fragment );
    if( geometrypath != nullptr )
        glDeleteShader( geometry );

}

Shader::~Shader() {

    glDeleteProgram( m_RendererID );

}

void Shader::Bind() const {

    glUseProgram( m_RendererID );

}

void Shader::SetInt( const std::string &name, int value ) const {

    glUniform1i( glGetUniformLocation( m_RendererID, name.c_str() ), value );

}

void Shader::SetFloat( const std::string &name, float value ) const {

    glUniform1f( glGetUniformLocation( m_RendererID, name.c_str() ), value );

}

void Shader::SetVec3( const std::string &name, const glm::vec3& values ) const {

    glUniform3f( glGetUniformLocation( m_RendererID, name.c_str() ), values.x, values.y, values.z );

}


void Shader::SetMat4( const std::string &name, const glm::mat4 &mat ) const {

    glUniformMatrix4fv( glGetUniformLocation( m_RendererID, name.c_str() ), 1, GL_FALSE, &mat[0][0] );

}

std::shared_ptr<Shader> Shader::Create( const char* vertexPath, const char* fragmentPath, const char* geometrypath ) {

    return std::make_shared<Shader>( vertexPath, fragmentPath, geometrypath );

}
*/
