#pragma once

namespace Hyperion::Scripting {

    class MonoBindingsInput final {
    public:
        static void Bind();
    private:
        MonoBindingsInput() = delete;
        ~MonoBindingsInput() = delete;
    };

}