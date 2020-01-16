#pragma once

#include "Core.h"
#include "ExtractVectorDetails.h"
#include "ExtractArrayDetails.h"

#include <GL/glew.h>

#include <vector>

// We use offsetof on classes with members split between base and derived class,
// making it a non-standard layout type.
// While this is UB in C++14, we are using C++17, so it is conditionally supported.
#pragma clang diagnostic ignored "-Winvalid-offsetof"


#define DRONESIM_GEN_LAYOUT_OBJ(Class, Member)                                                                  \
DroneSim::GPU::LayoutComponent {                                                                                \
    #Member,                                                                                                    \
    offsetof(Class, Member),                                                                                    \
    sizeof(Class::Member),                                                                                      \
    DroneSim::Traits::GetCount<decltype(Class::Member)>(),                                                      \
    DroneSim::GPU::LayoutComponent::GetComponentType<decltype(Class::Member)>()                                 \
}


namespace DroneSim::GPU {
    struct LayoutComponent {
        const char* name;
        std::size_t offset, size, count;

        enum ComponentType : GLenum {
            BYTE    = GL_BYTE, 
            UBYTE   = GL_UNSIGNED_BYTE, 
            SHORT   = GL_SHORT, 
            USHORT  = GL_UNSIGNED_SHORT, 
            INT     = GL_INT, 
            UINT    = GL_UNSIGNED_INT, 
            FLOAT   = GL_FLOAT, 
            DOUBLE  = GL_DOUBLE
        } type;


        template <typename T> constexpr static ComponentType GetComponentType(void) {
            using type = typename Traits::ExtractVectorDetails<T>::type;

            // There is a bug in OpenGL where it will reinterpret integers as floating point numbers, and then convert them back
            // to integers when using anything other than GL_FLOAT.
            if constexpr (std::is_same_v<type, f64>) return LayoutComponent::DOUBLE;
            else return LayoutComponent::FLOAT;
        }
    };


    template <typename D> class ILayoutObject {
    public:
        // returns std::array<LayoutComponent>, but auto because of varying size.
        constexpr static auto GetObjectLayout(void) {
            static_assert(Traits::ExtractArrayDetails<decltype(D::GetObjectLayout())>::is_array);

            return D::GetObjectLayout();
        }
    protected:
        ILayoutObject(void) = default;
    };
}