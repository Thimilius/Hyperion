#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class CGameObject;

    struct SComponentType {
        CString name;
    };

    class CComponent {
    private:
        CGameObject *m_game_object;
    public:
        inline CGameObject *GetGameObject() const { return m_game_object; }

        inline virtual SComponentType *GetType() const = 0;

        friend class CGameObject;
    };

}