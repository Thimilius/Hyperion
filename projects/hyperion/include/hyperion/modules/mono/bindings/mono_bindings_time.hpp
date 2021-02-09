#pragma once

namespace Hyperion::Scripting {

    class MonoBindingsTime final {
    public:
        static void Bind();
    private:
        MonoBindingsTime() = delete;
        ~MonoBindingsTime() = delete;
    };

}