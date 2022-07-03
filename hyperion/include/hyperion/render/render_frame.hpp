//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/non_copyable.hpp"
#include "hyperion/render/assets/render_assets_context.hpp"
#include "hyperion/render/object/render_object_context.hpp"
#include "hyperion/render/pipeline/render_pipeline_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderFrame final : public INonCopyable {
  public:
    RenderFrame();
  public:
    void Clear();

    inline RenderAssetContext &GetAssetContext() { return m_asset_context; }
    inline const RenderAssetContext &GetAssetContext() const { return m_asset_context; }
    inline RenderObjectContext &GetObjectContext() { return m_object_context; }
    inline const RenderObjectContext &GetObjectContext() const { return m_object_context; }
    inline RenderPipelineContext &GetPipelineContext() { return m_pipeline_context; }
    inline const RenderPipelineContext &GetPipelineContext() const { return m_pipeline_context; }
  private:
    RenderAssetContext m_asset_context;
    RenderObjectContext m_object_context;
    RenderPipelineContext m_pipeline_context;
  };

}
