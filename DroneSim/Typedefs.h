#pragma once

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#define GLM_SWIZZLE_RGBA
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/vec_swizzle.hpp>

#include <cstdint>
#include <type_traits>


namespace DroneSim {
    constexpr static long double PI  = M_PI;
    constexpr static long double TAU = 2 * M_PI;


    // Guaranteed-size integers & matching floating point typedefs.
    using u8  =  uint8_t;
    using i8  =   int8_t;
    using u16 = uint16_t;
    using i16 =  int16_t;
    using u32 = uint32_t;
    using i32 =  int32_t;
    using u64 = uint64_t;
    using i64 =  int64_t;

    using f32 = float;
    using f64 = double;


    template <typename T> using no_ref  = std::remove_reference_t<T>;
    template <typename T> using no_cref = std::remove_const_t<std::remove_reference_t<T>>;


    // Function Pointer / Pointer to member
    template <typename Ret, typename... Args>                   using FPtr  = Ret(       *)(Args...);
    template <typename Class, typename Ret, typename... Args>   using MFPtr = Ret(Class::*)(Args...);
    template <typename Class, typename T>                       using MDPtr = T  (Class::*);
    


    // Abseil Hash Map / Set.
    template <typename Seed, typename... Ts> Seed HashCombine(Seed&& seed, const Ts&... objects) {
        return absl::HashState::combine(std::forward<Seed>(seed), objects...);
    }


    template <typename T> using Hasher     = absl::container_internal::hash_default_hash<T>;
    template <typename T> using Comparator = absl::container_internal::hash_default_eq<T>;


    template <typename K, typename V, typename Hash = Hasher<K>, typename Eq = Comparator<K>>
    using HashMap = absl::flat_hash_map<K, V, Hash, Eq>;

    template <typename K, typename Hash = Hasher<K>, typename Eq = Comparator<K>>
    using HashSet = absl::flat_hash_map<K, Hash, Eq>;



    // GLM Vector & Matrix types.
    #define DRONESIM_GLM_TYPEDEFS_FOR_MATRIX_SIZE(X, Y)                \
    using Mat##X##x##Y##ub = Mat##X##x##Y<u8 >;                        \
    using Mat##X##x##Y##b  = Mat##X##x##Y<i8 >;                        \
    using Mat##X##x##Y##us = Mat##X##x##Y<u16>;                        \
    using Mat##X##x##Y##s  = Mat##X##x##Y<i16>;                        \
    using Mat##X##x##Y##ui = Mat##X##x##Y<u32>;                        \
    using Mat##X##x##Y##i  = Mat##X##x##Y<i32>;                        \
    using Mat##X##x##Y##ul = Mat##X##x##Y<u64>;                        \
    using Mat##X##x##Y##l  = Mat##X##x##Y<i64>;                        \
                                                                       \
    using Mat##X##x##Y##f = Mat##X##x##Y<f32>;                         \
    using Mat##X##x##Y##d = Mat##X##x##Y<f64>;


    #define DRONESIM_GLM_TYPEDEFS_FOR_SIZE(Size)                       \
    template <typename T> using Vec##Size = glm::vec<Size, T>;         \
                                                                       \
    using Vec##Size##ub = Vec##Size<u8 >;                              \
    using Vec##Size##b  = Vec##Size<i8 >;                              \
    using Vec##Size##us = Vec##Size<u16>;                              \
    using Vec##Size##s  = Vec##Size<i16>;                              \
    using Vec##Size##ui = Vec##Size<u32>;                              \
    using Vec##Size##i  = Vec##Size<i32>;                              \
    using Vec##Size##ul = Vec##Size<u64>;                              \
    using Vec##Size##l  = Vec##Size<i64>;                              \
                                                                       \
    using Vec##Size##f  = Vec##Size<f32>;                              \
    using Vec##Size##d  = Vec##Size<f64>;                              \
                                                                       \
                                                                       \
    template <typename T> using Mat##Size = glm::mat<Size, Size, T>;   \
                                                                       \
    template <typename T> using Mat2x##Size = glm::mat<2, Size, T>;    \
    template <typename T> using Mat3x##Size = glm::mat<3, Size, T>;    \
    template <typename T> using Mat4x##Size = glm::mat<4, Size, T>;    \
                                                                       \
    using Mat##Size##ub = Mat##Size##x##Size<u8 >;                     \
    using Mat##Size##b  = Mat##Size##x##Size<i8 >;                     \
    using Mat##Size##us = Mat##Size##x##Size<u16>;                     \
    using Mat##Size##s  = Mat##Size##x##Size<i16>;                     \
    using Mat##Size##ui = Mat##Size##x##Size<u32>;                     \
    using Mat##Size##i  = Mat##Size##x##Size<i32>;                     \
    using Mat##Size##ul = Mat##Size##x##Size<u64>;                     \
    using Mat##Size##l  = Mat##Size##x##Size<i64>;                     \
                                                                       \
    using Mat##Size##f = Mat##Size##x##Size<f32>;                      \
    using Mat##Size##d = Mat##Size##x##Size<f64>;                      \
                                                                       \
                                                                       \
    DRONESIM_GLM_TYPEDEFS_FOR_MATRIX_SIZE(2, Size);                    \
    DRONESIM_GLM_TYPEDEFS_FOR_MATRIX_SIZE(3, Size);                    \
    DRONESIM_GLM_TYPEDEFS_FOR_MATRIX_SIZE(4, Size);


    DRONESIM_GLM_TYPEDEFS_FOR_SIZE(2);
    DRONESIM_GLM_TYPEDEFS_FOR_SIZE(3);
    DRONESIM_GLM_TYPEDEFS_FOR_SIZE(4);
}