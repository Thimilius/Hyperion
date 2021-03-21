//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rendering/widget_renderer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/rect_transform.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {
    
    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(WidgetRenderer)
    HYP_REFLECT_BASE(Component)
    HYP_REFLECT_CONSTRUCTOR([]() { return new WidgetRenderer(); })
    HYP_REFLECT_METADATA_POINTER(MetadataKind::RequireComponent, Type::Get<RectTransform>())
    HYP_REFLECT_END()

}