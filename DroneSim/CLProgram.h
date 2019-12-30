#pragma once

#include "Core.h"
#include "IGPURunnable.h"
#include "Pack.h"
#include "CLBuffer.h"
#include "CLGroupSize.h"

#include <CL/cl.h>

namespace DroneSim::GPU {
    // TODO: Lazy writing (sync on execute.)
    class CLProgram : public IGPURunnable<CLProgram, CLBuffer, u32> {
    public:
        CLProgram(cl_program program, cl_kernel kernel, cl_context* context, cl_command_queue* queue)
            : program(program), kernel(kernel), context(context), queue(queue) {}


        CLProgram(const CLProgram&) = delete;
        CLProgram& operator=(const CLProgram&) = delete;


        CLProgram(CLProgram&& other) 
            : program(other.program), kernel(other.kernel), context(other.context), queue(other.queue) 
        {
            other.program = nullptr;
            other.kernel = nullptr;
        }


        // One should avoid compiling programs just to overwrite them.
        CLProgram& operator=(CLProgram&& other) {
            if (program) clReleaseProgram(program);
            if (kernel) clReleaseKernel(kernel);

            program = other.program;
            kernel  = other.kernel;
            context = other.context;
            queue   = other.queue;

            other.program = nullptr;
            other.kernel  = nullptr;

            return *this;
        }


        ~CLProgram(void) {
            if (program) clReleaseProgram(program);
            if (kernel) clReleaseKernel(kernel);
        }




        template <typename T> constexpr static bool IsUniformAllowed(void) {
            return std::is_fundamental_v<T>;
        }


        void execute(std::size_t count) {
            const std::size_t global_count = CLGroupSize::GetNextValidGlobalWorkCount(count);
            const std::size_t local_count  = CLGroupSize::WORK_GROUP_SIZE;

            setUniform(count, 0);   // Load number of threads as uniform, so possible overflow threads can detect that they don't need to run.
            i32 status = clEnqueueNDRangeKernel(*queue, kernel, 1, nullptr, &global_count, &local_count, 0, nullptr, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to enqueue kernel.", status);
        }


        template <typename T> void addBuffer(const CLBuffer<T>& buffer, u32 id) {
            i32 status = clSetKernelArg(kernel, id, sizeof(cl_mem*), buffer.getBuffer());
            if (status != CL_SUCCESS) terminate("OpenCL failed to set kernel buffer argument.", status);
        }


        template <typename T>
        void setUniform(const T& uniform, u32 id) {
            i32 status = clSetKernelArg(kernel, id, sizeof(T), &uniform);
            if (status != CL_SUCCESS) terminate("OpenCL failed to set kernel argument.", status);
        }
    private:
        cl_program program;
        cl_kernel kernel;

        cl_context* context;
        cl_command_queue* queue;
    };
}