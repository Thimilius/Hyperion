//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct IComponent {
    public:
        virtual ~IComponent() = default;
    protected:
        IComponent() = default;
    };

}