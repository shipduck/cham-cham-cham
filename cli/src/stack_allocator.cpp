// Ŭnicode please
#include "stdafx.h"
#include "stack_allocator.h"

//할당한 순서의 역순으로 해제되어야 스택이 꺠지지 않는다
//디버그 모드에서는 이를 확실히 확인하기 위해서 추가 속성을 할당햇다
struct StackAllocHeader {
	StackAllocHeader() : alloc_size(0) {}
	int alloc_size;
};

StackAllocator::StackAllocator(int stack_size) 
	: data_(NULL),
	top_(NULL), 
	stack_size_(stack_size)
{
	data_ = static_cast<unsigned char*>(::malloc(stack_size));
	top_  = data_;
}
StackAllocator::~StackAllocator() 
{
	if(data_ != NULL) {
		::free(data_);
		data_ = NULL;
		top_ = NULL;
		stack_size_ = 0;
	}
}

void *StackAllocator::malloc(size_t x) 
{
	int alloc_size = x + sizeof(StackAllocHeader);

	//할당 가능한 크기가 x보다 작아야 할당 가능
	int curr_allocated_size = top_ - data_;
	int avail_alloc_size = stack_size_ - curr_allocated_size;
	if(avail_alloc_size < alloc_size) {
		SR_ASSERT(!"cannot alloc, more big stack required");
		return NULL;
	}

	unsigned char *raw_data = top_;
	top_ += alloc_size;
	StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>(raw_data);
	header->alloc_size = x;
	unsigned char *ptr = (unsigned char*)raw_data + sizeof(StackAllocHeader);

	return ptr;
}

void StackAllocator::free(void *ptr) 
{
	//data가 stack의 범위에 존재하는 메모리 좌표인가?
	SR_ASSERT((unsigned char*)data_ <= ptr);
	SR_ASSERT((unsigned char*)data_ + stack_size_ > ptr);

	StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>((unsigned char*)ptr - sizeof(StackAllocHeader));
	int alloc_size = header->alloc_size;
	//가장마지막에 할당한것을 해제한다면 top와 할당크기가 연관되어잇다.
	//그렇지 않다면 top가 올바르지 않을것이다
	int expected_alloc_size = (unsigned char*)top_ - (unsigned char*)ptr;
	if(expected_alloc_size == alloc_size) {
		top_ -= sizeof(StackAllocHeader) + alloc_size;
	} else {
		SR_ASSERT(!"stack break?");
	}

}

void StackAllocator::freeToMarker(Marker marker) 
{
	unsigned char *marker_ptr = reinterpret_cast<unsigned char*>(marker);
	SR_ASSERT(top_ >= marker_ptr);
	top_ = marker_ptr;
}

StackAllocator::Marker StackAllocator::getMarker() 
{
	return reinterpret_cast<Marker>(top_);
}

int StackAllocator::getAllocHeaderSize() const 
{
	return sizeof(StackAllocHeader);
}

void StackAllocator::clear() 
{
	top_ = data_;
}

//////////////////////////

DoubleStackAllocator::DoubleStackAllocator(int stack_size)
	: data_(NULL),
	lower_(NULL), 
	upper_(NULL), 
	stack_size_(stack_size) 
{
	data_ = reinterpret_cast<unsigned char*>(::malloc(stack_size));
	lowerClear();
	upperClear();
}
DoubleStackAllocator::~DoubleStackAllocator() 
{
	if(data_ != NULL) {
		::free(data_);
		data_ = NULL;
		lower_ = NULL;
		upper_ = NULL;
		stack_size_ = 0;
	}
}

int DoubleStackAllocator::remain_size() const 
{
	return upper_ - lower_ + 1;
}
int DoubleStackAllocator::getAllocHeaderSize() const 
{
	return sizeof(StackAllocHeader);
}

void* DoubleStackAllocator::lowerMalloc(size_t x) 
{
	//stack할당자와 다른것은 할당후 upper에 닿는지 확인해야한다는 점이다
	int alloc_size = x + getAllocHeaderSize();
	if(upper_ - lower_ < alloc_size) {
		SR_ASSERT(!"cannot alloc, more big stack required");
		return NULL;
	}

	unsigned char *raw_ptr = lower_;
	StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>(raw_ptr);
	header->alloc_size = x;
	unsigned char *ptr = raw_ptr + getAllocHeaderSize();
	lower_ += alloc_size;

	return ptr;
}
void DoubleStackAllocator::lowerFree(void *ptr) 
{
	//data가 stack의 범위에 존재하는 메모리 좌표인가?
	SR_ASSERT((unsigned char*)data_ <= ptr);
	SR_ASSERT((unsigned char*)lower_ > ptr);

	StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>((unsigned char*)ptr - sizeof(StackAllocHeader));
	int alloc_size = header->alloc_size;
	//가장마지막에 할당한것을 해제한다면 top와 할당크기가 연관되어잇다.
	//그렇지 않다면 top가 올바르지 않을것이다
	int expected_alloc_size = (unsigned char*)lower_ - (unsigned char*)ptr;
	if(expected_alloc_size == alloc_size) {
		lower_ -= (sizeof(StackAllocHeader) + alloc_size);
	} else {
		SR_ASSERT(!"stack break?");
	}
}

void* DoubleStackAllocator::upperMalloc(size_t x) 
{
	int alloc_size = x + getAllocHeaderSize();
	if(upper_ - lower_ < alloc_size) {
		SR_ASSERT(!"cannot alloc, more big stack required");
		return NULL;
	}

	unsigned char *raw_ptr = upper_ - (alloc_size + 1);
	upper_ -= alloc_size;
	StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>(raw_ptr);
	header->alloc_size = x;
	unsigned char *ptr = raw_ptr + getAllocHeaderSize();
	return ptr;
}
void DoubleStackAllocator::upperFree(void *ptr) 
{
	//data가 stack의 범위에 존재하는 메모리 좌표인가?
	SR_ASSERT((unsigned char*)upper_ <= ptr);
	SR_ASSERT((unsigned char*)data_ + stack_size_ > ptr);

	StackAllocHeader *header = reinterpret_cast<StackAllocHeader*>((unsigned char*)ptr - sizeof(StackAllocHeader));
	int alloc_size = header->alloc_size;
	upper_ += (sizeof(StackAllocHeader) + alloc_size);
}

void DoubleStackAllocator::lowerClear() 
{
	lower_ = data_;
}
void DoubleStackAllocator::upperClear() 
{
	upper_ = data_ + stack_size_ - 1;
}
void DoubleStackAllocator::freeToLowerMarker(Marker marker) 
{
	unsigned char *marker_ptr = reinterpret_cast<unsigned char*>(marker);
	lower_ = marker_ptr;
}

void DoubleStackAllocator::freeToUpperMarker(Marker marker) 
{
	unsigned char *marker_ptr = reinterpret_cast<unsigned char*>(marker);
	upper_ = marker_ptr;
}
DoubleStackAllocator::Marker DoubleStackAllocator::getLowerMarker() 
{
	Marker marker= reinterpret_cast<Marker>(lower_);
	return marker;
}
DoubleStackAllocator::Marker DoubleStackAllocator::getUpperMarker() 
{
	Marker marker= reinterpret_cast<Marker>(upper_);
	return marker;
}