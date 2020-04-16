//#include "ExampleLayer.hpp"
//#include "PointLightLayer.hpp"
#include "ModelLayer.hpp"
//#include "AdvencedLighting.hpp"
//#include "CubeMapLayer.hpp"
//#include "NormalMap.hpp"

using namespace ENGINE;

class Sandbox : public Application {

public:
    Sandbox() {
        
        PushLayer( new ModelLayer() );
        //PushLayer( new NormalMappingingLayer() );
        //PushLayer( new CubemapLayer() );

    }
    ~Sandbox() {}

};

int main() {

    auto sandbox = new Sandbox();
    sandbox->Run();

    delete sandbox;

    return 0;

}
