#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class INonCopyable {
    public:
        INonCopyable() = default;
        INonCopyable(const INonCopyable &other) = delete;
        INonCopyable &operator=(const INonCopyable &other) = delete;
        virtual ~INonCopyable() = default;
    };

}