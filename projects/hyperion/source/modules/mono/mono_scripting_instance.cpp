#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_instance.hpp"

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

namespace Hyperion::Scripting {

    MonoScriptingInstance::MonoScriptingInstance(MonoObject *managed_object) {
        m_managed_object = managed_object;
    }

    void MonoScriptingInstance::OnDestroy() {
        MonoScriptingDriver::UnregisterObject(m_managed_object);

        delete this; // I don't really know how I feel about this...
    }

}