#pragma once

#include "hyperion/common.hpp"
#include "hyperion/math/math.hpp"

namespace Hyperion::Rendering {

    class CShader {
    public:
        virtual ~CShader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const CString &name, int value);

        virtual void SetFloat(const CString &name, float value);
        virtual void SetFloat2(const CString &name, const Math::SVec2 &value);
        virtual void SetFloat3(const CString &name, const Math::SVec3 &value);
        virtual void SetFloat4(const CString &name, const Math::SVec4 &value);

        virtual void SetMat4(const CString &name, const Math::SMat4 &matrix);

        static CShader *Create(const CString &vertex_source, const CString &fragment_source);
    };

}