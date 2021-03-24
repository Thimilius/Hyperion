//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/system/engine_loop.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/memory/memory.hpp"
#include "hyperion/core/system/engine.hpp"

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
            { "PhysicsEngineFixedUpdate", Engine::PhysicsEngineFixedUpdate },
            { "ScriptingEngineFixedUpdate", Engine::ScriptingEngineFixedUpdate },
            { "ApplicationFixedUpdate", Engine::ApplicationFixedUpdate },
            { "TimeFixedUpdate", Engine::TimeFixedUpdate }
        };

        result.tick.name = "Tick";
        result.tick.sub_systems = {
            { "TimeTick", Engine::TimeTick },
            { "ApplicationTick", Engine::ApplicationTick }
        };

        result.update.name = "Update";
        result.update.sub_systems = {
            { "AssetManagerUpdate", Engine::AssetManagerUpdate },
            { "UiEngineUpdatge", Engine::UiEngineUpdate },
            { "ScriptingEngineUpdate", Engine::ScriptingEngineUpdate },
            { "ApplicationUpdate", Engine::ApplicationUpdate }
        };

        result.late_update.name = "LateUpdate";
        result.late_update.sub_systems = {
            { "ObjectManagerLateUpdate", Engine::ObjectManagerLateUpdate },
            { "UiEngineLateUpdate", Engine::UiEngineLateUpdate },
            { "RenderEngineLateUpdate", Engine::RenderEngineLateUpdate }
        };

        return result;
    }

}