#include "memory.h"
#include <algorithm>
#include <stdexcept>

MemoryResource::MemoryResource(size_t size) : total_size(size) {
    buffer_start = new char[size];
    free_blocks.push_back({buffer_start, size});
}


void* MemoryResource::do_allocate(size_t bytes, size_t alignment) {
    for (auto it = free_blocks.begin(); it != free_blocks.end(); ++it) {
        void* ptr = it->ptr;
        size_t space = it->size;
        
        void* aligned_ptr = ptr;
        if (std::align(alignment, bytes, aligned_ptr, space)) {
            size_t padding = static_cast<char*>(aligned_ptr) - static_cast<char*>(ptr);
            size_t total_needed = bytes + padding;

            if (it->size >= total_needed) {
                used_blocks.push_back({aligned_ptr, bytes});

                if (it->size > total_needed) {
                    it->ptr = static_cast<char*>(ptr) + total_needed;
                    it->size -= total_needed;
                } else {
                    free_blocks.erase(it);
                }
                return aligned_ptr;
            }
        }
    }
    throw std::bad_alloc();
}

void MemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    auto it = std::find_if(used_blocks.begin(), used_blocks.end(), 
                          [p](const Info& b) { return b.ptr == p; });

    if (it != used_blocks.end()) {
        free_blocks.push_back({p, bytes});
        used_blocks.erase(it);
    }
}

bool MemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

MemoryResource::~MemoryResource() {
    delete[] static_cast<char*>(buffer_start);
}