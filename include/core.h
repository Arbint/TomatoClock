#pragma once

#include <memory>

namespace TC
{
    template<typename T>
    using Shared = std::shared_ptr<T>;

    template<typename T>
    using PrivateProp = std::unique_ptr<T>;

    #define LOG(M, ...) printf(M "\n",##__VA_ARGS__)
} // namespace ly


