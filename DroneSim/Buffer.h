#pragma once

#include "Core.h"

namespace DroneSim {
    // RAII wrapper around malloc / free.
    class Buffer {
    public:
        Buffer(std::size_t size) {
            storage = malloc(size);
        }

        ~Buffer(void) {
            if (storage) free(storage);
        }

        Buffer(Buffer&& other) : storage(other.storage) {
            other.storage = nullptr;
        }

        Buffer& operator=(Buffer&& other) {
            free(storage);

            storage = other.storage;
            other.storage = nullptr;

            return *this;
        }


        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;


        operator void*(void) {
            return storage;
        }
    protected:
        void* storage;
    };


    template <typename T> class TypeBuffer : public Buffer {
    public:
        using Buffer::Buffer;

        operator T*(void) {
            return reinterpret_cast<T*>(storage);
        }
    };
}