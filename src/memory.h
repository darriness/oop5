#pragma once
#include <memory_resource>
#include <vector>
#include <cstddef>
#include <iostream>

class MemoryResource : public std::pmr::memory_resource {
private:
    void* buffer_start;
    size_t total_size;

    struct Info {
        void* ptr;
        size_t size;
    };

    std::vector<Info> used_blocks;
    std::vector<Info> free_blocks;

public:
    explicit MemoryResource(size_t size);
    ~MemoryResource();

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
};