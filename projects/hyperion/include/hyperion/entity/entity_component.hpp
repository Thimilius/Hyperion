#pragma once

namespace Hyperion {

    class Entity;

    class EntityComponent {
    private:
        Entity *m_entity;
    public:
        virtual ~EntityComponent() = default;

        inline Entity *GetEntity() const { return m_entity; }
    protected:
        EntityComponent() = default;

        friend class Entity;
    };

}