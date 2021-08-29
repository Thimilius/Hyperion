#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ISystem {
    public:
        virtual ~ISystem() = default;
    public:
        virtual void Run() = 0;
    };

}