#pragma once

#include "Core.h"
#include "ExtractVectorDetails.h"

#include <GL/glew.h>

#include <vector>


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
        std::string name;
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

            if constexpr (std::is_same_v<type, u8 >) return LayoutComponent::UBYTE;
            if constexpr (std::is_same_v<type, i8 >) return LayoutComponent::BYTE;
            if constexpr (std::is_same_v<type, u16>) return LayoutComponent::USHORT;
            if constexpr (std::is_same_v<type, i16>) return LayoutComponent::SHORT;
            if constexpr (std::is_same_v<type, i32>) return LayoutComponent::UINT;
            if constexpr (std::is_same_v<type, u32>) return LayoutComponent::INT;
            if constexpr (std::is_same_v<type, f32>) return LayoutComponent::FLOAT;
            else return LayoutComponent::DOUBLE;
        }
    };


    template <typename D> class ILayoutObject {
    public:
        static std::vector<LayoutComponent> GetObjectLayout(void) {
            return D::GetObjectLayout();
        }
    protected:
        ILayoutObject(void) = default;
    };
}