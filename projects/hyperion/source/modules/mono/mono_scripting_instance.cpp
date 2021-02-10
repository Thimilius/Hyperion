#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_instance.hpp"

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

namespace Hyperion::Scripting {

    MonoScriptingInstance::MonoScriptingInstance(MonoObject *mono_object) {
        m_mono_object = mono_object;
    }

    void MonoScriptingInstance::OnDestroy() {
        MonoScriptingDriver::UnregisterObject(m_mono_object);

        delete this; // I don't really know how I feel about this...
    }

}