#pragma once

#include "Core.h"
#include "IGPUBuffer.h"
#include "CLGroupSize.h"

#include <CL/cl.h>

namespace DroneSim::GPU {
    // TODO: Lazy writing (sync on execute.)
    template <typename T> class CLBuffer : public IGPUBuffer<CLBuffer<T>, T, true, true> {
    public:
        // Reserve memory, but don't upload any data.
        CLBuffer(cl_context* context, cl_command_queue* queue, std::size_t size) : context(context), queue(queue), size(size) {
            i32 status;

            storage = clCreateBuffer(*context, flags, sizeof(T) * size, nullptr, &status);
            if (status != CL_SUCCESS) terminate("OpenCL failed to create buffer.", status);
        }


        // Reserve memory and upload data. 
        // If the size of the data is not equal to the number of threads in the kernel, this constructor should not be used.
        CLBuffer(cl_context* context, cl_command_queue* queue, const std::vector<T>& data) : CLBuffer(context, queue, CLGroupSize::GetNextValidGlobalWorkCount(data.size())) {
            modify(0, data);
        }


        CLBuffer(const CLBuffer&) = delete;
        CLBuffer& operator=(const CLBuffer&) = delete;


        CLBuffer(CLBuffer&& other) : buffer(other.buffer), size(other.size) {
            other.buffer = nullptr;
        }


        // One should avoid allocating buffers just to overwrite them.
        CLBuffer& operator=(CLBuffer&& other) {
            if (buffer) clReleaseMemObject(buffer);

            buffer = other.buffer;
            size   = other.size;

            other.buffer = nullptr;

            return *this;
        }

        
        ~CLBuffer(void) {
            if (buffer) clReleaseMemObject(buffer);
        }




        void modify(std::size_t where, const std::vector<T>& data) {
            i32 status;

            status = clEnqueueWriteBuffer(*queue, storage, CL_TRUE, where * sizeof(T), data.size() * sizeof(T), &data[0], nullptr, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to write buffer to device.", status);
        }


        std::vector<T> fetch(void) {
            std::vector<T> result;
            result.resize(size);

            i32 status = clEnqueueReadBuffer(*queue, storage, CL_TRUE, 0, sizeof(T) * size, &result[0], 0, nullptr, nullptr);
            if (status != CL_SUCCESS) terminate("OpenCL failed to read buffer from device.", status);

            return result;
        }


        const cl_mem* getBuffer(void) const {
            return &buffer;
        }
    private:
        cl_mem buffer;
        std::size_t size;

        cl_context* context;
        cl_command_queue* queue;
    };
}