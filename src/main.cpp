#include <iostream>
#include "memory.h"
#include "stack.h"

struct Point {
    int x, y;
};

int main() {
    MemoryResource res(1024);
    Stack<Point> stack(&res);

    stack.push({10, 20});
    stack.push({30, 40});

    std::cout << "Top: " << stack.top().x << ", " << stack.top().y << std::endl;
    
    for(auto p : stack) {
        std::cout << "[" << p.x << ";" << p.y << "] ";
    }
    std::cout << std::endl;

    return 0;
}