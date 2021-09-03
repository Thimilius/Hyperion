//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/engine_loop.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/engine.hpp"
#include "hyperion/core/memory/memory.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
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
            { "ApplicationFixedUpdate", Engine::ApplicationFixedUpdate },
            { "TimeFixedUpdate", Engine::TimeFixedUpdate }
        };

        result.tick.name = "Tick";
        result.tick.sub_systems = {
            { "ApplicationTick", Engine::ApplicationTick }
        };

        result.update.name = "Update";
        result.update.sub_systems = {
            { "ApplicationUpdate", Engine::ApplicationUpdate }
        };

        result.late_update.name = "LateUpdate";
        result.late_update.sub_systems = {
            { "RenderEngineLateUpdate", Engine::RenderEngineLateUpdate }
        };

        return result;
    }

}