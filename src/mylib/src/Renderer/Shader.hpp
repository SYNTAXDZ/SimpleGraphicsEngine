#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <memory>

typedef unsigned int GLenum;

namespace ENGINE {

    class Shader {

    public:
        Shader( const std::string& filepath );
        Shader( const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc );
        ~Shader();

        void Bind() const;
        void UnBind() const;

        const std::string& GetName() const { return m_Name; }

		void UploadUniformInt( const std::string& name, int value );

		void UploadUniformFloat( const std::string& name, float value );
		void UploadUniformFloat2( const std::string& name, const glm::vec2& value );
		void UploadUniformFloat3( const std::string& name, const glm::vec3& value );
		void UploadUniformFloat4( const std::string& name, const glm::vec4& value );

		void UploadUniformMat3( const std::string& name, const glm::mat3& matrix );
		void UploadUniformMat4( const std::string& name, const glm::mat4& matrix );

        static std::shared_ptr<Shader> Create( const std::string& filepath );
        static std::shared_ptr<Shader> Create( const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc );

    private:
        void Compile( const std::unordered_map<GLenum, std::string>& shaderSource );
        std::string ReadFile( const std::string& filepath );
        std::unordered_map<GLenum, std::string> PreProcess( const std::string& source );

    private:
        unsigned int m_RendererID;
        std::string m_Name;

    };

/*
    class Shader {
    
    public:
        Shader( const char *vertexPath, const char *fragmentPath, const char *geometrypath = nullptr );
        ~Shader();

        void Bind() const;
        void SetInt( const std::string &name, int value ) const;
        void SetFloat( const std::string &name, float value ) const;
        void SetVec3( const std::string &name, const glm::vec3& values ) const;
        void SetMat4( const std::string &name, const glm::mat4& mat ) const;


        static std::shared_ptr<Shader> Create( const char* vertexPath, const char* fragmentPath, const char* geometrypath = nullptr );
    private:
        unsigned int m_RendererID;

    };

*/
}
