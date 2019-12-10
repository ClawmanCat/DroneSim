#pragma once

#include "Core.h"
#include "GPUProgram.h"
#include "CLBuffer.h"
#include "Buffer.h"
#include "FileIO.h"
#include "StringUtils.h"

#include <CL/cl.h>

#include <string_view>
#include <optional>

namespace DroneSim::CL {
    class CLCompiler {
    public:
        CLCompiler(const CLCompiler&) = delete;
        CLCompiler& operator=(const CLCompiler&) = delete;

        CLCompiler(CLCompiler&&) = delete;
        CLCompiler& operator=(CLCompiler&&) = delete;

        ~CLCompiler(void) {
            clReleaseCommandQueue(queue);
            clReleaseContext(context);
        }


        // Singleton Object.
        static CLCompiler& instance(void) {
            static CLCompiler instance { };
            return instance;
        }


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


        CLCompiler(void) {
            i32 status = CL_SUCCESS;

            // Get number of platforms.
            u32 platformCount;
            status = clGetPlatformIDs(0, nullptr, &platformCount);
            if (status != CL_SUCCESS) terminate("OpenCL failed to get platform IDs.", status);

            // Load platform IDs.
            TypeBuffer<cl_platform_id> platforms(sizeof(cl_platform_id) * platformCount);
            status = clGetPlatformIDs(platformCount, platforms, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to get platform IDs.", status);

            // Get number of devices.
            u32 deviceCount;
            status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, nullptr, &deviceCount);
            if (status != CL_SUCCESS) terminate("OpenCL failed to get device IDs.", status);

            // Load device IDs.
            TypeBuffer<cl_device_id> devices(sizeof(cl_device_id) * deviceCount);
            status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, deviceCount, devices, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to get device IDs.", status);

            // Select device.
            u32 best_device = select_best_cl_device(devices, deviceCount);
            device = *(devices + best_device);

            // Create context & queue.
            context = clCreateContext(nullptr, 1, devices + best_device, nullptr, nullptr, &status);
            if (status != CL_SUCCESS) terminate("OpenCL failed to create context.", status);

            queue = clCreateCommandQueue(context, devices[best_device], 0, &status);
        }


        static u32 select_best_cl_device(cl_device_id* devices, u32 deviceCount) {
            // If there's only one GPU, that's the one we pick.
            if (deviceCount == 1) return 0;

            // Multiple devices: approximate which one is more powerful.
            u32 currentID = 0, currentPower = 0;

            for (u32 i = 0; i < deviceCount; ++i) {
                std::size_t frequency, cores;
                i32 status = CL_SUCCESS;


                status = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(u32), nullptr, &frequency);
                if (status != CL_SUCCESS) terminate("OpenCL failed to get device info.", status);

                status = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(u32), nullptr, &cores);
                if (status != CL_SUCCESS) terminate("OpenCL failed to get device info.", status);


                u32 power = frequency * cores;

                if (power > currentPower) {
                    currentID = i;
                    currentPower = power;
                }
            }

            return currentID;
        }

        static std::string get_opencl_error_log(cl_device_id device, cl_program program) {
            std::size_t size;
            clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &size);

            std::string log;
            log.resize(size);

            clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, &log[0], nullptr);

            return log;
        }
    };
}