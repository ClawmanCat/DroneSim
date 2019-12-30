#include "CLCompiler.h"
#include "Buffer.h"
#include "FileIO.h"
#include "StringUtils.h"


namespace DroneSim::GPU {
    CLCompiler& CLCompiler::instance(void) {
        static CLCompiler instance{ };
        return instance;
    }


    CLCompiler::CLCompiler(void) {
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


    CLCompiler::~CLCompiler(void) {
        clReleaseCommandQueue(queue);
        clReleaseContext(context);
    }


    CLCompiler::ProgramType CLCompiler::compile(const std::string& name) {
        // Load source
        auto source = FileIO::ReadText(StringUtils::cat(Paths::PATH_KERNELS, name, ".cl"));
        if (!source) terminate("Failed to read program source from file.");

        std::string cat = StringUtils::cat(source.value());
        const char* cstr = cat.c_str();


        // Compile program
        i32 status;

        cl_program program = clCreateProgramWithSource(context, 1, &cstr, nullptr, &status);
        if (status != CL_SUCCESS) terminate("OpenCL failed to create program from source.", status);

        status = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
        if (status != CL_SUCCESS) terminate("OpenCL failed to build program.", status);

        cl_kernel kernel = clCreateKernel(program, "main", &status);
        if (status != CL_SUCCESS) terminate("OpenCL failed to create kernel.", status);


        return CLProgram(program, kernel, &context, &queue);
    }


    u32 CLCompiler::select_best_cl_device(cl_device_id* devices, u32 count) {
        // If there's only one GPU, that's the one we pick.
        if (count == 1) return 0;

        // Multiple devices: approximate which one is more powerful.
        u32 currentID = 0, currentPower = 0;

        for (u32 i = 0; i < count; ++i) {
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
}