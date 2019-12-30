#pragma once

#include "Core.h"

#include <type_traits>

namespace DroneSim::GPU {
    template <typename D, template <typename... T> typename BT, typename UFID> class IGPURunnable {
    public:
        template <typename T> using BufferType = BT<T>;
        using UniformIdentifier = UFID;
        
        // Interface differs here between OpenCL and OpenGL, since OpenCL treats buffers like uniforms.
        // In OpenGL the amount of kernels is set implicitly by the buffer size, whereas in OpenCL it must be specified manually.
        // In OpenGL each kernel acts on one buffer, whereas in OpenCL it may act on multiple.
        // TODO: Unify this interface.
        // OpenCL:
        //      void execute(std::size_t count);
        //      void addBuffer(const BufferType& buffer, const BufferID& id);
        // OpenGL:
        //      void execute(void);
        //      BufferID addBuffer(const BufferType& buffer);
        //      void removeBuffer(const BufferID& id);


        template <typename T> void setUniform(const T& uniform, const UniformIdentifier& id) {
            static_cast<D&>(*this).setUniform<T>(uniform, id);
        }
    protected:
        IGPURunnable(void) = default;
    };
}