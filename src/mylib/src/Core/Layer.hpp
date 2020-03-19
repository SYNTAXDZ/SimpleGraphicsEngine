#pragma once

#include "../Events/Event.hpp"

namespace ENGINE {

    class Layer {

    public:
        Layer( const std::string& name = "layer" ); 
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDettach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent( Event& e ) {}

        inline const std::string& GetName() const { return m_DebugName; }
    
    protected:
        std::string m_DebugName;


    };

}