#pragma once

namespace Hyperion {

    class Entity;

    class EntityComponent {
    private:
        Entity *m_entity;
    public:
        inline Entity *GetEntity() const { return m_entity; }
    protected:
        virtual ~EntityComponent() = default;
        EntityComponent() = default;
    private:
        // Components can not be copied
        EntityComponent(EntityComponent &other) = delete;
        EntityComponent operator=(EntityComponent &other) = delete;

        friend class Entity;
    };

}