// Ŭnicode please
#include "test_stdafx.h"
#include "base/stack_allocator.h"


TEST(StackAllocator, basic_malloc_free) 
{
	StackAllocator allocator;
	EXPECT_EQ(allocator.stack_size(), allocator.remain_size());
	const int header_size = allocator.getAllocHeaderSize();

	void *ptr1 = allocator.malloc(10);
	EXPECT_EQ(allocator.stack_size() - (10 + header_size), allocator.remain_size());

	void *ptr2 = allocator.malloc(20);
	EXPECT_EQ(allocator.stack_size() - (20 + 10 + header_size * 2), allocator.remain_size());

	EXPECT_NE(20, (unsigned char*)ptr2 - (unsigned char*)ptr1);

	allocator.free(ptr2);
	EXPECT_EQ(allocator.stack_size() - (10 + header_size), allocator.remain_size());

	allocator.free(ptr1);
	EXPECT_EQ(allocator.stack_size(), allocator.remain_size());
}

/*
TEST(StackAllocator, reverse_free) {
//ptr1, ptr2로 할당받고 순서를 뒤집어서 해제하는 경우, 스택이 깨지니 적절히 죽어야한다
StackAllocator allocator;
void *ptr1 = allocator.malloc(10);
void *ptr2 = allocator.malloc(20);
allocator.free(ptr1);
allocator.free(ptr2);
}
*/

TEST(StackAllocator, marker) 
{
	StackAllocator allocator;
	void *ptr1 = allocator.malloc(10);
	void *ptr2 = allocator.malloc(20);
	StackAllocator::Marker makrer_a = allocator.getMarker();
	int remain_size_1 = allocator.remain_size();
	void *ptr3 = allocator.malloc(30);
	void *ptr4 = allocator.malloc(400);
	allocator.freeToMarker(makrer_a);
	EXPECT_EQ(remain_size_1, allocator.remain_size());
}

TEST(StackAllocator, Clear) 
{
	StackAllocator allocator;
	void *ptr1 = allocator.malloc(10);
	allocator.clear();
	void *ptr2 = allocator.malloc(40);
	EXPECT_EQ(ptr1, ptr2);
}


TEST(DoubleStackAllocator, malloc_free) 
{
	DoubleStackAllocator allocator;
	const int header_size = allocator.getAllocHeaderSize();
	const int stack_size = allocator.stack_size();
	EXPECT_EQ(stack_size, allocator.remain_size());

	void *ptr1 = allocator.lowerMalloc(10);
	EXPECT_EQ(stack_size - (10 + header_size), allocator.remain_size());

	void *ptr2 = allocator.lowerMalloc(20);
	EXPECT_EQ(stack_size - (10 + 20 + header_size * 2), allocator.remain_size());

	void *ptr3 = allocator.upperMalloc(30);
	EXPECT_EQ(stack_size - (10 + 20 + 30 + header_size * 3), allocator.remain_size());

	void *ptr4 = allocator.upperMalloc(20);
	EXPECT_EQ(stack_size - (10 + 20 + 30 + 20 + header_size * 4), allocator.remain_size());

	//free start
	allocator.lowerFree(ptr2);
	EXPECT_EQ(stack_size - (10 + 30 + 20 + header_size * 3), allocator.remain_size());

	allocator.upperFree(ptr4);
	EXPECT_EQ(stack_size - (10 + 30 + header_size * 2), allocator.remain_size());

	allocator.upperFree(ptr3);
	EXPECT_EQ(stack_size - (10 + header_size), allocator.remain_size());

	allocator.lowerFree(ptr1);
	EXPECT_EQ(stack_size, allocator.remain_size());
}

TEST(DoubleStackAllocator, Clear) 
{
	DoubleStackAllocator allocator;
	const int header_size = allocator.getAllocHeaderSize();
	const int stack_size = allocator.stack_size();
	EXPECT_EQ(stack_size, allocator.remain_size());

	void *ptr1 = allocator.lowerMalloc(10);
	EXPECT_EQ(stack_size - (10 + header_size), allocator.remain_size());

	void *ptr2 = allocator.lowerMalloc(20);
	EXPECT_EQ(stack_size - (10 + 20 + header_size * 2), allocator.remain_size());

	void *ptr3 = allocator.upperMalloc(30);
	EXPECT_EQ(stack_size - (10 + 20 + 30 + header_size * 3), allocator.remain_size());

	void *ptr4 = allocator.upperMalloc(20);
	EXPECT_EQ(stack_size - (10 + 20 + 30 + 20 + header_size * 4), allocator.remain_size());

	//free start
	allocator.lowerClear();
	EXPECT_EQ(stack_size - (30 + 20 + header_size * 2), allocator.remain_size());

	allocator.upperClear();
	EXPECT_EQ(stack_size, allocator.remain_size());
}

TEST(DoubleStackAllocator, Marker) 
{
	DoubleStackAllocator allocator;
	const int header_size = allocator.getAllocHeaderSize();
	const int stack_size = allocator.stack_size();
	EXPECT_EQ(stack_size, allocator.remain_size());

	void *ptr1 = allocator.lowerMalloc(10);
	void *ptr2 = allocator.lowerMalloc(20);
	DoubleStackAllocator::Marker lower_marker = allocator.getLowerMarker();
	void *ptr_lower_a = allocator.lowerMalloc(5);

	void *ptr3 = allocator.upperMalloc(30);
	void *ptr4 = allocator.upperMalloc(20);
	DoubleStackAllocator::Marker upper_marker = allocator.getUpperMarker();
	void *ptr_upper_a = allocator.upperMalloc(5);

	allocator.freeToLowerMarker(lower_marker);
	void *ptr_lower_b = allocator.lowerMalloc(9);
	EXPECT_EQ(ptr_lower_a, ptr_lower_b);

	allocator.freeToUpperMarker(upper_marker);
	void *ptr_upper_b = allocator.upperMalloc(5);
	EXPECT_EQ(ptr_upper_a, ptr_upper_b);
}