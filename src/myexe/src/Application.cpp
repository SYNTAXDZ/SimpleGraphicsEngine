//#include "ExampleLayer.hpp"
//#include "PointLightLayer.hpp"
//#include "ModelLayer.hpp"
//#include "AdvencedLighting.hpp"
//#include "CubeMapLayer.hpp"
//#include "NormalMap.hpp"
//#include "HdrLayer.hpp"
//#include "PBR.hpp"
#include "PBR_IBL.hpp"

using namespace ENGINE;

class Sandbox : public Application {

public:
    Sandbox() {
        
        //PushLayer( new ModelLayer() );
        //PushLayer( new NormalMappingingLayer() );
        //PushLayer( new CubemapLayer() );
        //PushLayer( new HDRLayer() );
        PushLayer( new PBRIBLLayer() );

    }
    ~Sandbox() {}

};

int main() {

    auto sandbox = new Sandbox();
    sandbox->Run();

    delete sandbox;

    return 0;

}
