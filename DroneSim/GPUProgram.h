#pragma once

#include "Core.h"
#include "CLBuffer.h"
#include "KernelArgumentTransform.h"
#include "KernelArgumentMemoryAddress.h"

#include <CL/cl.h>

#include <cstddef>

namespace DroneSim::CL {
    template <typename... Args> class GPUProgram {
    public:
        GPUProgram(cl_program program, cl_kernel kernel) : program(program), kernel(kernel) {}

        GPUProgram(GPUProgram&& other) : program(other.program), kernel(other.kernel) {
            other.program = nullptr;
            other.kernel = nullptr;
        }

        GPUProgram& operator=(GPUProgram&& other) {
            if (kernel) clReleaseKernel(kernel);
            if (program) clReleaseProgram(program);

            program = other.program;
            kernel = other.kernel;

            other.program = nullptr;
            other.kernel = nullptr;

            return *this;
        }

        ~GPUProgram(void) {
            if (kernel) clReleaseKernel(kernel);
            if (program) clReleaseProgram(program);
        }

        GPUProgram(const GPUProgram&) = delete;
        GPUProgram& operator=(const GPUProgram&) = delete;


        template <bool Async> void run(std::size_t threads, std::size_t groups, const typename Traits::KernelArgumentTransform<Args>::type&... args) {
            i32 status = CL_SUCCESS;

            // Push arguments
            u32 i = 0;
            (
                [&](const auto& arg) {
                    i32 s = clSetKernelArg(
                        kernel, 
                        i++, 
                        Traits::KernelArgumentMemoryAddress<std::decay_t<decltype(arg)>>::size,
                        (void*) (((std::size_t) &arg) + Traits::KernelArgumentMemoryAddress<std::decay_t<decltype(arg)>>::offset)
                    );

                    status = (s == CL_SUCCESS) ? status : s;
                }(args),
                ...
            );

            if (status != CL_SUCCESS) terminate("OpenCL failed to set one or more kernel arguments.", status);

            // Run kernel
            status = clEnqueueNDRangeKernel(CLCompiler::instance().queue, kernel, 1, nullptr, &threads, &groups, 0, nullptr, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to enqueue kernel.", status);

            // Await completion
            if constexpr (!Async) GPUProgram::await_all();
        }


        static void await_all(void) {
            i32 status = CL_SUCCESS;

            status = clFlush(CLCompiler::instance().queue);
            if (status != CL_SUCCESS) terminate("OpenCL failed to flush queue.", status);

            status = clFinish(CLCompiler::instance().queue);
            if (status != CL_SUCCESS) terminate("OpenCL failed to complete queue.", status);
        }
    private:
        cl_program program;
        cl_kernel kernel;
    };
}