#pragma once

#include <mono/metadata/object.h>

namespace Hyperion::Scripting {

    class MonoBindingsObject final {
    public:
        static void Bind();
    private:
        MonoBindingsObject() = delete;
        ~MonoBindingsObject() = delete;
    };

}