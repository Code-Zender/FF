#pragma once
#include <FF_renderer.h>

namespace FF{
    namespace Context{

    };

    namespace Game{
        class Context{
            private:
                Renderer::Context renderer;
                


            public:
                void init();

        };
    };

    namespace Sonsiges{} // erweiterbar
    //TODO: namespaces

};

