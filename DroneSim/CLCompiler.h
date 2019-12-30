#pragma once

#include "Core.h"
#include "IGPUCompiler.h"
#include "CLProgram.h"

#include <CL/cl.h>


namespace DroneSim::GPU {
    class CLCompiler : public IGPUCompiler<CLCompiler, CLProgram> {
    public:
        using ProgramType = CLProgram;


        static CLCompiler& instance(void);


        CLCompiler(const CLCompiler&) = delete;
        CLCompiler(CLCompiler&&) = delete;

        CLCompiler& operator=(const CLCompiler&) = delete;
        CLCompiler& operator=(CLCompiler&&) = delete;


        ~CLCompiler(void);


        ProgramType compile(const std::string& name);
    private:
        cl_device_id device;
        cl_context context;
        cl_command_queue queue;


        CLCompiler(void);


        static u32 select_best_cl_device(cl_device_id* devices, u32 count);
    };
}