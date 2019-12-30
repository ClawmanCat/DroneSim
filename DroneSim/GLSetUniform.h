#pragma once

#include "Core.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <boost/preprocessor.hpp>

#include <type_traits>


// Converts signed integral types to GLint, unsigned ones to GLuint, etc.
namespace DroneSim::GPU::Detail {
    template <typename T, typename = void> struct ToGLUniformT {};

    template <typename T> struct ToGLUniformT<T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>> { 
        using type = GLuint; 
    };

    template <typename T> struct ToGLUniformT<T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>> {
        using type = GLint;
    };

    template <> struct ToGLUniformT<f32> { using type = GLfloat;  };
    template <> struct ToGLUniformT<f64> { using type = GLdouble; };
}



// Create overloads for scalars and vectors
// Unfortunately, we have to construct a temporary, since there is only one glUniform method for all integer sizes.
#define DRONESIM_GL_UNIFORMS_NOMAT(Type, Suffix)                                                   \
template <> struct SetImpl<Type> {                                                                 \
    static void Set(GLint index, const Type& value) {                                              \
        using UT = typename DroneSim::GPU::Detail::ToGLUniformT<Type>::type;                       \
        UT v(value);                                                                               \
        glUniform1##Suffix(index, 1, & v);                                                         \
    }                                                                                              \
};                                                                                                 \
                                                                                                   \
                                                                                                   \
template <> struct SetImpl<glm::vec<2, Type>> {                                                    \
    static void Set(GLint index, const glm::vec<2, Type>& value) {                                 \
        using UT = typename DroneSim::GPU::Detail::ToGLUniformT<Type>::type;                       \
        glm::vec<2, UT> v(value);                                                                  \
        glUniform2##Suffix(index, 1, glm::value_ptr(v));                                           \
    }                                                                                              \
};                                                                                                 \
                                                                                                   \
template <> struct SetImpl<glm::vec<3, Type>> {                                                    \
    static void Set(GLint index, const glm::vec<3, Type>& value) {                                 \
        using UT = typename DroneSim::GPU::Detail::ToGLUniformT<Type>::type;                       \
        glm::vec<3, UT> v(value);                                                                  \
        glUniform3##Suffix(index, 1, glm::value_ptr(v));                                           \
    }                                                                                              \
};                                                                                                 \
                                                                                                   \
template <> struct SetImpl<glm::vec<4, Type>> {                                                    \
    static void Set(GLint index, const glm::vec<4, Type>& value) {                                 \
        using UT = typename DroneSim::GPU::Detail::ToGLUniformT<Type>::type;                       \
        glm::vec<4, UT> v(value);                                                                  \
        glUniform4##Suffix(index, 1, glm::value_ptr(v));                                           \
    }                                                                                              \
};


// Create an overload for an X by Y matrix.
#define DRONESIM_GL_UNIFORMS_MAT_XY_0(...)
#define DRONESIM_GL_UNIFORMS_MAT_XY_1(Type, Suffix, X, Y)                                          \
template <> struct SetImpl<glm::mat<X, Y, Type>> {                                                 \
    static void Set(GLint index, const glm::mat<X, Y, Type>& value) {                              \
        glUniformMatrix##X##x##Y##Suffix(index, 1, GL_FALSE, glm::value_ptr(value));               \
    }                                                                                              \
};                                                                                                  


// Create overloads for matrices of size Size x N for N in [2, 4]
#define DRONESIM_GL_UNIFORMS_ONEMAT(Type, Suffix, Size)                                            \
template <> struct SetImpl<glm::mat<Size, Size, Type>> {                                           \
    static void Set(GLint index, const glm::mat<Size, Size, Type>& value) {                        \
        glUniformMatrix##Size##Suffix(index, 1, GL_FALSE, glm::value_ptr(value));                  \
    }                                                                                              \
};                                                                                                 \
                                                                                                   \
                                                                                                   \
BOOST_PP_CAT(DRONESIM_GL_UNIFORMS_MAT_XY_, BOOST_PP_NOT_EQUAL(Size, 2))(Type, Suffix, Size, 2)     \
BOOST_PP_CAT(DRONESIM_GL_UNIFORMS_MAT_XY_, BOOST_PP_NOT_EQUAL(Size, 3))(Type, Suffix, Size, 3)     \
BOOST_PP_CAT(DRONESIM_GL_UNIFORMS_MAT_XY_, BOOST_PP_NOT_EQUAL(Size, 4))(Type, Suffix, Size, 4)


// Create overloads for scalars, vectors and matrices
#define DRONESIM_GL_UNIFORMS_MAT(Type, Suffix)  \
DRONESIM_GL_UNIFORMS_NOMAT(Type, Suffix)        \
                                                \
DRONESIM_GL_UNIFORMS_ONEMAT(Type, Suffix, 2)    \
DRONESIM_GL_UNIFORMS_ONEMAT(Type, Suffix, 3)    \
DRONESIM_GL_UNIFORMS_ONEMAT(Type, Suffix, 4)



namespace DroneSim::GPU::GLSetUniform {
    namespace Detail {
        template <typename T> struct SetImpl {};


        DRONESIM_GL_UNIFORMS_NOMAT(bool, uiv)

        DRONESIM_GL_UNIFORMS_NOMAT(u8,  uiv)
        DRONESIM_GL_UNIFORMS_NOMAT(i8,  iv )
        DRONESIM_GL_UNIFORMS_NOMAT(u16, uiv)
        DRONESIM_GL_UNIFORMS_NOMAT(i16, iv )
        DRONESIM_GL_UNIFORMS_NOMAT(u32, uiv)
        DRONESIM_GL_UNIFORMS_NOMAT(i32, iv )

        DRONESIM_GL_UNIFORMS_MAT(f32, fv)
        DRONESIM_GL_UNIFORMS_MAT(f64, dv)
    }


    template <typename T> inline void Set(GLint index, const T& value) {
        Detail::SetImpl<T>::Set(index, value);
    }
}