#pragma once

// #include <pch/pch.hpp>
#include "../../pch/pch.hpp"
#include "Core.hpp"

namespace ENGINE {

    class Input {

    public:
        inline static bool IsKeyPressed( int keycode ) { return s_Instance->IsKeyPressedImpl( keycode ); }
        
        inline static bool IsMouseButtonPressed( int button ) { return s_Instance->IsMouseButtonPressedImpl( button ); }
        inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        inline float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline float GetMouseY() { return s_Instance->GetMouseYImpl(); }

    protected:
        bool IsKeyPressedImpl( int keycode );

        bool IsMouseButtonPressedImpl( int button );
        std::pair<float, float> GetMousePositionImpl();
        float GetMouseXImpl();
        float GetMouseYImpl();
    
    private:
        // static std::shared_ptr<Input> s_Instance;
        static Input* s_Instance;
    };

}