#include "hyppch.hpp"

#include "hyperion/core/system/engine_loop.hpp"

#include "hyperion/core/memory/memory.hpp"

namespace Hyperion {

    EngineLoopSystem EngineLoopSystem::Default() {
        EngineLoopSystem result;
        
        result.initilization.name = "Initilization";
        result.initilization.sub_systems = {
            { "MemoryStatsInitilization", MemoryStats::ResetFrameMemory },
            { "TimeInitilization", Engine::TimeInitilization },
            { "InputInitilization", Engine::InputInitilization }
        };

        result.fixed_update.name = "FixedUpdate";
        result.fixed_update.sub_systems = {
            { "PhysicsEngineFixedUpdate", Engine::PhysicsEngineFixedUpdate },
            { "ApplicationFixedUpdate", Engine::ApplicationFixedUpdate },
            { "TimeFixedUpdate", Engine::TimeFixedUpdate }
        };

        result.tick.name = "Tick";
        result.tick.sub_systems = {
            { "ApplicationTick", Engine::ApplicationTick }
        };

        result.update.name = "Update";
        result.update.sub_systems = {
            { "WorldManagerUpdate", Engine::WorldManagerUpdate },
            { "ApplicationUpdate", Engine::ApplicationUpdate }
        };

        result.late_update.name = "LateUpdate";
        result.late_update.sub_systems = {
            { "ObjectManagerLateUpdate", Engine::ObjectManagerLateUpdate },
            { "RenderEngineLateUpdate", Engine::RenderEngineLateUpdate }
        };

        return result;
    }

}