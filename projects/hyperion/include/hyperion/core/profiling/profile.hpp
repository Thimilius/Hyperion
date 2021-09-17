#pragma once

#ifdef HYP_PROFILE
#include <optick/optick.h>

#define HYP_PROFILE_FRAME(name) OPTICK_FRAME(name)
#define HYP_PROFILE_THREAD(name) OPTICK_THREAD(name)
#define HYP_PROFILE_SCOPE(...) OPTICK_EVENT(__VA_ARGS__)
#define HYP_PROFILE_CATEGORY(name, category) OPTICK_CATEGORY(name, category)
#define HYP_PROFILE_TAG(name, ...) OPTICK_TAG(name, __VA_ARGS__)

namespace Hyperion {
    using ProfileCategory = Optick::Category;
}

#else
#define HYP_PROFILE_FRAME(name)
#define HYP_PROFILE_THREAD(name)
#define HYP_PROFILE_SCOPE(...)
#define HYP_PROFILE_CATEGORY(name, category)
#define HYP_PROFILE_TAG(name, ...)

namespace Hyperion {
    enum class ProfileCategory {
		None,
		AI,
		Animation,
		Audio,
		Debug,
		Camera,
		Cloth,
		GameLogic,
		Input,
		Navigation,
		Network,
		Physics,
		Rendering,
		Scene,
		Script,
		Streaming,
		UI,
		VFX,
		Visibility,
		Wait,
		WaitEmpty,
		IO,
		GPU_Cloth,
		GPU_Lighting,
		GPU_PostFX,
		GPU_Reflections,
		GPU_Scene,
		GPU_Shadows,
		GPU_UI,
		GPU_VFX,
		GPU_Water,
    };
}
#endif
