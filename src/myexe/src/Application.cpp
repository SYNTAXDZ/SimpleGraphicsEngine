//#include "ExampleLayer.hpp"
//#include "PointLightLayer.hpp"
//#include "ModelLayer.hpp"
// #include "AdvencedGLLayer.hpp"
#include "CubeMapLayer.hpp"


using namespace ENGINE;

class Sandbox : public Application {

public:
    Sandbox() {

        // PushLayer( new ExampleLayer() );
        PushLayer( new CubemapLayer() );

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
