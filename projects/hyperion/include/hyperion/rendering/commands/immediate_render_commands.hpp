#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/object/resource.hpp"

#include <variant>

// NOTE: Immediate render commands are special commands that get executed immediately by the Render Thread.
// They are synchronized, meaning the Main Thread waits for the Render Thread to execute them.
// Their primary purpose is to return data from the Render Thread back to the Main Thread.

namespace Hyperion::Rendering {

    enum class ImmediateRenderCommandType {
        GetTextureData
    };

    struct ImmediateRenderCommandGetTextureData {
        ResourceId texture_id;
        Vector<uint8> *data;
    };

    struct ImmediateRenderCommand {
        ImmediateRenderCommandType type;

        std::variant<ImmediateRenderCommandGetTextureData> command;
    };

}