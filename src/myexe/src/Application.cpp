// #include "mylib.hpp"
#include "ExampleLayer.hpp"
#include "LightingLayer.hpp"
#include "Core/EntryPoint.hpp"

using namespace ENGINE;

class Sandbox : public Application {

public:
    Sandbox() {

        // PushLayer( new ExampleLayer() );
        PushLayer( new LightingLayer() );

    }
    ~Sandbox() {}

};
/*
Application* CreateApplication() {

    return new Sandbox();

}
*/
int main() {

    auto sandbox = new Sandbox();
    sandbox->Run();

    delete sandbox;

    return 0;

}
