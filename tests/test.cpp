#include <gtest/gtest.h>
#include "../src/memory.h"
#include "../src/stack.h"

// структура для проверки (как сложные данные)
struct ComplexData {
    int id;
    double value;
    bool operator==(const ComplexData& other) const {
        return id == other.id && value == other.value;
    }
};


TEST(StackTest, BasicOperations) {
    MemoryResource res(1024);
    Stack<int> stack(&res);

    EXPECT_TRUE(stack.empty());
    
    stack.push(10);
    stack.push(20);
    
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.top(), 20);
    
    stack.pop();
    EXPECT_EQ(stack.top(), 10);
    
    stack.pop();
    EXPECT_TRUE(stack.empty());
}

// Тест 2: Работа со сложными структурами
TEST(StackTest, ComplexStructs) {
    MemoryResource res(1024);
    Stack<ComplexData> stack(&res);

    stack.push({1, 3.14});
    stack.push({2, 6.28});

    EXPECT_EQ(stack.top().id, 2);
    EXPECT_DOUBLE_EQ(stack.top().value, 6.28);
}

// Тест 3: Проверка итератора
TEST(StackTest, IteratorTraversal) {
    MemoryResource res(1024);
    Stack<int> stack(&res);

    stack.push(10);
    stack.push(20);
    stack.push(30);

    std::vector<int> expected = {30, 20, 10};
    std::vector<int> actual;

    for (auto val : stack) {
        actual.push_back(val);
    }

    EXPECT_EQ(actual, expected);
}

// Тест 4: Исключения при пустом стеке
TEST(StackTest, ExceptionOnEmpty) {
    MemoryResource res(1024);
    Stack<int> stack(&res);

    EXPECT_THROW(stack.pop(), std::out_of_range);
    EXPECT_THROW(stack.top(), std::out_of_range);
}

// Тест 5: Переполнение памяти
TEST(AllocatorTest, OutOfMemory) {
    MemoryResource res(24); 
    Stack<int> stack(&res);

    EXPECT_NO_THROW(stack.push(1)); 
    
    // ne vlezet
    EXPECT_THROW(stack.push(2), std::bad_alloc);
}

// Тест 6: Переиспользование памяти
TEST(AllocatorTest, MemoryReuse) {
    MemoryResource res(1024);
    Stack<int> stack(&res);

    stack.push(100);
    const int* ptr1 = &(*stack.begin());
    
    stack.pop();
    
    stack.push(200);
    const int* ptr2 = &(*stack.begin());
    // address have to be same
    EXPECT_EQ(ptr1, ptr2);
}