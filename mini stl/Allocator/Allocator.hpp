#define DEBUG
#ifdef DEBUG
#include "iostream"
#endif
#ifndef _ALLOCATE_
#define _ALLOCATE_
#include "type_traits"
//simple allocator with std::malloc
class malloc_alloc{

	//the function handler of oom
	static void* oom_malloc(std::size_t);
	static void(*malloc_oom_handler)();

public:

	static void* allocate(std::size_t n)
	{
		void *result = std::malloc(n);
		if (result == nullptr) result = oom_malloc(n);
		return result;
	}

	static void deallocate(void *p, size_t)
	{
		std::free(p);
	}

	//set the fucntion handler for oom
	static void(*set_malloc_handler(void(*f)()))()
	{
		void(*old)() = malloc_oom_handler;
		malloc_oom_handler = f;
		return old;
	}
};

void(* malloc_alloc::malloc_oom_handler)() = 0;

void* malloc_alloc::oom_malloc(std::size_t n)
{
	void(*oom_handler)();
	void *result;
	while (true)
	{
		oom_handler = malloc_oom_handler;
		if (oom_handler == nullptr) exit(0);
		(*oom_handler)();
		result = std::malloc(n);
		if (result != nullptr) return result;
	}
}
/*--------------------------------------------------------------------------------------*/
// allocate with memory pool
enum : std::size_t
{
	Align = 8,// the size of every block
	MaxSize = 128, //the maxsize of every block
	ListNum = MaxSize / Align, //the number of the free_list
};

class default_alloc{

	//round up n to the number
	static std::size_t Round_Up(std::size_t size)
	{
		return (((size)+Align - 1) & ~(Align - 1));
	}

	//the union of the memory list
	union _slot{
		union _slot * _next;
		char _data[1];
	};

	//free_list 
	static _slot * volatile free_list[ListNum];

	typedef _slot* slot_ptr;

	//convert size to the index of free_list
	static std::size_t FreeList_Index(std::size_t size)
	{
		return (((size)+Align - 1) / Align - 1);
	}

	//create the memory chunk and return the pointer whitch point to the memory
	static void* refill(std::size_t n)
	{
		int nodjs = 20;
		char* chunk = chunk_alloc(n, nodjs);

		slot_ptr volatile *my_free_list = nullptr;
		slot_ptr result = nullptr;
		slot_ptr cur_slot = nullptr;
		slot_ptr next_slot = nullptr;

		//the number of memory chunk return 1 chunk 
		if (nodjs == 1) return chunk;

		//add the memory chunk to the free_list
		my_free_list = free_list + FreeList_Index(n);
		result = reinterpret_cast<slot_ptr>(chunk);
		*my_free_list = next_slot = reinterpret_cast<slot_ptr>(chunk + n);

		for (int i = 1;; i++)
		{
			cur_slot = next_slot;
			next_slot = reinterpret_cast<slot_ptr>(reinterpret_cast<char*>(next_slot) + n);

			if (nodjs - 1 == i)
			{
				cur_slot->_next = nullptr;
				break;
			}
			else
			{
				cur_slot->_next = next_slot;
			}
		}
		return result;
	}

	static char* chunk_alloc(std::size_t size, int &nodes)
	{

		char* result = nullptr;
		std::size_t total_size = size * nodes;
		std::size_t size_left = end_free - start_free;

		if (size_left > total_size)
		{
			result = start_free;
			start_free += total_size;
			return result;
		}
		else if (size_left > size)
		{
			//the size left less than total size and bigger than size, so recalculate the nodes of the chunk
			nodes = size_left / size;
			total_size = size * nodes;
			result = start_free;
			start_free += total_size;
			return result;
		}
		else
		{
			std::size_t size_to_get = 2 * total_size + Round_Up(heap_size >> 4);

			if (size_left > 0)
			{
				//try to use the left free memory of the memory pool, and add them to free_list
				slot_ptr volatile *my_free_list = free_list + FreeList_Index(size_left);
				reinterpret_cast<slot_ptr>(start_free)->_next = *my_free_list;
				*my_free_list = reinterpret_cast<slot_ptr>(start_free);
			}
			
			start_free = reinterpret_cast<char*>(std::malloc(size_to_get));

			if (start_free == nullptr)
			{
				// if oom try to take the memory of free_list to memory pool
				slot_ptr volatile *my_free_list = nullptr;
				slot_ptr p = nullptr;

				for (int i = size; i < MaxSize; i += Align)
				{
					my_free_list = free_list + FreeList_Index(i);
					p = *my_free_list;
					if (p != nullptr)
					{
						*my_free_list = p->_next;
						start_free = reinterpret_cast<char*>(p);
						end_free = start_free + i;
						return chunk_alloc(size, nodes);
					}
				}
				end_free = nullptr;
				start_free = reinterpret_cast<char*>(malloc_alloc::allocate(size_to_get));
			}

			heap_size += size_to_get;
			end_free = start_free + size_to_get;
			return chunk_alloc(size, nodes);
		}
	}

	static char* start_free;
	static char* end_free;
	static std::size_t heap_size;

public:
	static void* allocate(std::size_t n)
	{
		slot_ptr volatile *my_free_list = nullptr;
		slot_ptr result = nullptr;

		if (n > (std::size_t)MaxSize)
		{
			return malloc_alloc::allocate(n);
		}

		my_free_list = free_list + FreeList_Index(n);
		result = *my_free_list;

		if (result == nullptr)
		{
			void * r = refill(Round_Up(n));
			return r;
		}

		*my_free_list = result->_next;
		return result;
	}
	static void deallocate(void *p, std::size_t n)
	{
		slot_ptr volatile *my_free_list = nullptr;
		slot_ptr q = reinterpret_cast<_slot*>(p);

		if (n > (std::size_t)MaxSize)
		{
			malloc_alloc::deallocate(p, n);
			return;
		}

		my_free_list = free_list + FreeList_Index(n);
		q->_next = *my_free_list;
		*my_free_list = q;
 	}
};

char* default_alloc::start_free = nullptr;
char* default_alloc::end_free = nullptr;

std::size_t default_alloc::heap_size = 0;

default_alloc::_slot * volatile default_alloc::free_list[ListNum] = { nullptr };

//define the allocator with malloc or memory pool
#ifdef USE_MALLOC
typedef malloc_alloc alloc;
#else
typedef default_alloc alloc;
#endif

//the pack of the allocate
template<class T, class Alloc = alloc>
class simple_alloc{
public:

	static T* allocate()
	{
		return reinterpret_cast<T*>(Alloc::allocate(sizeof(T)));
	}

	static T* allocate(std::size_t n)
	{
		return n == 0 ? 0 : reinterpret_cast<T*>(Alloc::allocate(n * sizeof(T)));
	}

	static void deallocate(void *p)
	{
		Alloc::deallocate(p,sizeof(T));
	}

	static void deallocate(void *p, std::size_t n)
	{
		if(n != 0) alloc::deallocate(p, n * sizeof(T));
	}
};
//construct and the destroy
template<class T1, class T2>
inline void construct(T1 *p, const T2 &v)
{
	new(p)T1(v);
}

template<class T>
inline void destroy(T *ptr)
{
	ptr->~T();
}

template<class Iterator>
inline void destroy(Iterator first, Iterator last)
{
	_destroy(first, last, first);
}

template<class Iterator, class T>
inline void _destroy(Iterator first, Iterator last, T*)
{
	bool is_destroy = std::has_trivial_destructor<T>::value;
	if (!is_destroy)
	{
		for (; first != last; ++first)
		{
			destroy(&*(first));
		}
	}
}

#endif
