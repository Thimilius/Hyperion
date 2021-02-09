#pragma once

#include <mono/metadata/object.h>

namespace Hyperion::Scripting {

    class MonoBindingsEntity final {
    public:
        static void Bind();
    private:
        MonoBindingsEntity() = delete;
        ~MonoBindingsEntity() = delete;
    };

}