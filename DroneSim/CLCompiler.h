#pragma once

#include "Core.h"
#include "GPUProgram.h"
#include "FileIO.h"
#include "StringUtils.h"

#include <CL/cl.h>

#include <string_view>
#include <optional>

namespace DroneSim::CL {
    class CLCompiler {
    public:
        // Singleton Object.
        static CLCompiler& instance(void);


        CLCompiler(const CLCompiler&) = delete;
        CLCompiler& operator=(const CLCompiler&) = delete;

        CLCompiler(CLCompiler&&) = delete;
        CLCompiler& operator=(CLCompiler&&) = delete;

        ~CLCompiler(void);


        template <typename... Args> GPUProgram<Args...> compile(std::string_view name, std::string_view entry_point) {
            auto source = FileIO::ReadText(std::string(Paths::PATH_KERNELS) + std::string(name));
            if (!source) terminate("Failed to read program source from file.", -1);
            
            std::string cat  = StringUtils::cat(source.value());
            const char* cstr = cat.c_str();

            i32 status = CL_SUCCESS;

            cl_program program = clCreateProgramWithSource(context, 1, &cstr, nullptr, &status);
            if (status != CL_SUCCESS) terminate("OpenCL failed to create program from source.", status);

            status = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to build program.", status);

            cl_kernel kernel = clCreateKernel(program, entry_point.data(), &status);
            if (status != CL_SUCCESS) terminate("OpenCL failed to create kernel.", status);

            return GPUProgram<Args...>(program, kernel);
        }
    private:
        template <typename T> friend class CLBuffer;
        template <typename... Args> friend class GPUProgram;

        cl_context context;
        cl_command_queue queue;
        cl_device_id device;


        CLCompiler(void);

        static u32 select_best_cl_device(cl_device_id* devices, u32 deviceCount);
        static std::string get_opencl_error_log(cl_device_id device, cl_program program);
    };
}