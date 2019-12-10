#pragma once

#include "Core.h"
#include "CLCompiler.h"

#include <CL/cl.h>

#include <cstddef>
#include <array>


namespace DroneSim::CL {
    template <typename T> class CLBuffer {
    public:
        template <typename Container> CLBuffer(const Container& source, std::size_t size, cl_mem_flags flags) : size(size) {
            i32 status = CL_SUCCESS;

            storage = clCreateBuffer(CLCompiler::instance().context, flags, sizeof(T) * size, nullptr, &status);
            if (status != CL_SUCCESS) terminate("OpenCL failed to create buffer.", status);

            status = clEnqueueWriteBuffer(CLCompiler::instance().queue, storage, CL_TRUE, 0, sizeof(T) * size, (T*) &(*std::begin(source)), 0, nullptr, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to write buffer to device.", status);
        }


        ~CLBuffer(void) {
            if (storage) clReleaseMemObject(storage);
        }


        CLBuffer(CLBuffer&& other) : storage(other.storage), size(other.size) {
            other.storage = nullptr;
        }

        CLBuffer& operator=(CLBuffer&& other) {
            size = other.size;
            storage = other.storage;

            other.storage = nullptr
        }

        CLBuffer(const CLBuffer&) = delete;
        CLBuffer& operator=(const CLBuffer&) = delete;


        std::vector<T> fetch(void) {
            std::vector<T> result;
            result.resize(size);

            i32 status = clEnqueueReadBuffer(CLCompiler::instance().queue, storage, CL_TRUE, 0, sizeof(T) * size, &result[0], 0, nullptr, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to read buffer from device.", status);

            return result;
        }

        constexpr static std::size_t storage_offset(void) { return offsetof(CLBuffer<T>, storage); }
    private:
        cl_mem storage;
        std::size_t size;
    };
}