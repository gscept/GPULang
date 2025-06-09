#pragma once
//------------------------------------------------------------------------------
/**
    @file containers.h

    String types

    2024 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "memory.h"
#include <algorithm>
#include <cmath>

namespace GPULang
{

//------------------------------------------------------------------------------
/**
 A StackArray lives on the thread-local 'stack' buffer which allows for dirt cheap linear allocation.
 
 Frees its memory at destruction
 */
template<typename TYPE>
struct StackArray
{
    TYPE* ptr = nullptr;
    size_t size = 0;
    size_t capacity = 0;
    StackArray() {}
    StackArray(size_t count)
    {
        if (count != 0)
        {
            this->ptr = AllocStack<TYPE>(count);
            this->capacity = count;
        }
    }
    
    StackArray(StackArray&& rhs) noexcept
    {
        this->ptr = rhs.ptr;
        this->size = rhs.size;
        this->capacity = rhs.capacity;
        rhs.ptr = nullptr;
        rhs.size = 0;
        rhs.capacity = 0;
    }
    
    ~StackArray()
    {
        if (this->ptr != nullptr)
            DeallocStack(this->capacity, this->ptr);
        this->ptr = nullptr;
        this->capacity = 0;
        this->size = 0;
    }
    
    void operator=(StackArray&& rhs) noexcept
    {
        this->ptr = rhs.ptr;
        this->size = rhs.size;
        this->capacity = rhs.capacity;
        rhs.ptr = nullptr;
        rhs.size = 0;
        rhs.capacity = 0;
    }
    
    void Append(const TYPE& t)
    {
        assert(this->size + 1 <= this->capacity);
        this->ptr[this->size++] = t;
    }
    
    const TYPE& operator[](const size_t index) const
    {
        return this->ptr[index];
    }
    
    TYPE& operator[](const size_t index)
    {
        return this->ptr[index];
    }
    
    void Clear()
    {
        this->size = 0;
    }
    
    bool Full()
    {
        return this->size == this->capacity;
    }
    
    const TYPE& front() const
    {
        return this->buf[0];
    }
    
    TYPE& front()
    {
        return this->buf[0];
    }
    
    const TYPE& back() const
    {
        return this->ptr[this->size - 1];
    }
    
    TYPE& back()
    {
        return this->ptr[this->size - 1];
    }
    
    TYPE* begin()
    {
        return this->ptr;
    }
    
    TYPE* end()
    {
        if (this->ptr == nullptr)
            return nullptr;
        else
            return this->ptr + this->size;
    }
    
    const TYPE* begin() const
    {
        return this->ptr;
    }
    
    const TYPE* end() const
    {
        if (this->ptr == nullptr)
            return nullptr;
        else
            return this->ptr + this->size;
    }
};


//------------------------------------------------------------------------------
/**
 A PinnedArray is an array which can grow in size and retain pointer validity by using virtual memory
 to commit new pages as it needs them.
 
 Frees its memory at destruction.
 */
template<typename TYPE>
struct PinnedArray
{
    PinnedArray()
        : data(nullptr)
        , maxAllocationCount(0)
        , committedPages(0)
        , size(0)
        , capacity(0)
    {
        
    }
    
    PinnedArray(size_t maxAllocationCount)
        : size(0)
        , capacity(0)
        , committedPages(0)
    {
        this->maxAllocationCount = maxAllocationCount;
    }
    
    PinnedArray(const PinnedArray<TYPE>& rhs)
    {
        if (this->alloc != nullptr)
            DeallocVirtual(this->alloc);
        this->alloc = nullptr;
        this->data = nullptr;
        this->maxAllocationCount = rhs.maxAllocationCount;
        this->capacity = 0;
        this->size = 0;
        this->committedPages = 0;
        this->Grow(rhs.size);
        if (std::is_trivially_copyable<TYPE>::value)
        {
            memcpy(this->data + this->size, rhs.data, rhs.size * sizeof(TYPE));
            this->size += rhs.size;
        }
        else
        {
            for (const auto& elem : rhs)
            {
                this->data[this->size++] = elem;
            }
        }
    }
    
    PinnedArray(PinnedArray<TYPE>&& rhs)
    {
        if (this->alloc != nullptr)
            DeallocVirtual(this->alloc);
        this->alloc = rhs.alloc;
        this->data = rhs.data;
        this->maxAllocationCount = rhs.maxAllocationCount;
        this->size = rhs.size;
        this->capacity = rhs.capacity;
        this->committedPages = rhs.committedPages;
        rhs.alloc = nullptr;
        rhs.data = nullptr;
        rhs.maxAllocationCount = 0;
        rhs.size = 0;
        rhs.capacity = 0;
        rhs.committedPages = 0;
    }
    
    ~PinnedArray()
    {
        this->Free();
    }
    
    void operator=(PinnedArray<TYPE>&& rhs)
    {
        if (this->alloc != nullptr)
            DeallocVirtual(this->alloc);
        this->alloc = rhs.alloc;
        this->data = rhs.data;
        this->maxAllocationCount = rhs.maxAllocationCount;
        this->size = rhs.size;
        this->capacity = rhs.capacity;
        this->committedPages = rhs.committedPages;
        rhs.alloc = nullptr;
        rhs.data = nullptr;
        rhs.maxAllocationCount = 0;
        rhs.size = 0;
        rhs.capacity = 0;
        rhs.committedPages = 0;
    }
    
    void operator=(const PinnedArray<TYPE>& rhs)
    {
        if (this->alloc != nullptr)
            DeallocVirtual(this->alloc);
        this->alloc = nullptr;
        this->data = nullptr;
        this->capacity = 0;
        this->size = 0;
        this->committedPages = 0;
        this->maxAllocationCount = rhs.maxAllocationCount;
        this->Grow(rhs.size);
        if (std::is_trivially_copyable<TYPE>::value)
        {
            memcpy(this->data + this->size, rhs.data, rhs.size * sizeof(TYPE));
            this->size += rhs.size;
        }
        else
        {
            for (const auto& elem : rhs)
            {
                this->data[this->size++] = elem;
            }
        }
    }
    
    void operator=(const StackArray<TYPE>& rhs)
    {
        if (this->alloc != nullptr)
            DeallocVirtual(this->alloc);
        this->alloc = nullptr;
        this->data = nullptr;
        this->maxAllocationCount = rhs.size * sizeof(TYPE);
        this->committedPages = 0;
        this->capacity = 0;
        this->Grow(rhs.size);
        if (std::is_trivially_copyable<TYPE>::value)
        {
            memcpy(this->data + this->size, rhs.ptr, rhs.size * sizeof(TYPE));
            this->size += rhs.size;
        }
        else
        {
            for (const auto& elem : rhs)
            {
                this->data[this->size++] = elem;
            }
        }
    }
    
    void Grow(size_t numNeededMoreElements)
    {
        assert(this->maxAllocationCount > 0 && "PinnedArray not initialized");
        assert((this->size < this->maxAllocationCount) && "PinnedArray over allocation is not allowed");
        if (this->alloc == nullptr)
        {
            this->alloc = AllocVirtual<TYPE>(maxAllocationCount);
            this->data = (TYPE*)this->alloc->mem;
        }
        if (this->size + numNeededMoreElements > this->capacity)
        {
            float numElementsPerPage = SystemPageSize / (float)sizeof(TYPE);
            size_t numCommitedPages = ceil(this->capacity / numElementsPerPage);
            size_t numNeededPages = ceil((this->capacity + 1) / numElementsPerPage);
            if (numNeededPages > numCommitedPages)
            {
                size_t numBytesToCommit = (numNeededPages - numCommitedPages) * SystemPageSize;
                if (numBytesToCommit > 0)
                {
                    vcommit(this->data + this->capacity, numBytesToCommit);
                    size_t numNewObjects = numBytesToCommit / sizeof(TYPE);
                    if (std::is_trivially_default_constructible<TYPE>::value)
                        memset(this->data + this->capacity, 0x0, numBytesToCommit);
                    else
                        new (this->data + this->capacity) TYPE[numNewObjects];
                    this->capacity += numNewObjects;
                    this->committedPages = numNeededPages;
                }
            }
        }
    }
    
    void Append(const TYPE& element)
    {
        this->Grow(1);
        this->data[this->size++] = element;
    }
    
    void Append(const PinnedArray<TYPE>& elements)
    {
        this->Grow(elements.size);
        if (std::is_trivially_copyable<TYPE>::value)
        {
            memcpy(this->data + this->size, elements.data, elements.size * sizeof(TYPE));
            this->size += elements.size;
        }
        else
        {
            for (const auto& elem : elements)
            {
                this->data[this->size++] = elem;
            }
        }
    }
    
    void Erase(TYPE* it)
    {
        assert(it >= this->data && it <= this->data + this->size);
        memmove(it, it + 1, this->size - (this->data + this->size - it));
    }
    
    void Prepend(const PinnedArray<TYPE>& elements)
    {
        this->Grow(elements.size);
        memmove(this->data + elements.size, this->data, this->size * sizeof(TYPE));
        if (std::is_trivially_copyable<TYPE>::value)
        {
            memcpy(this->data, elements.data, elements.size * sizeof(TYPE));
            this->size += elements.size;
        }
        else
        {
            size_t it = 0;
            for (const auto& elem : elements)
            {
                this->data[it++] = elem;
            }
        }
    }
    
    void Clear()
    {
        this->size = 0;
    }
    
    void Free()
    {
        if (this->alloc != nullptr)
            DeallocVirtual(this->alloc);
        this->alloc = nullptr;
        this->data = nullptr;
        this->size = 0;
        this->capacity = 0;
    }
    
    void Insert(const TYPE& element, size_t index)
    {
        assert(index < this->size);
        this->Grow(1);
        memmove(this->data + index + 1, this->data + index, (this->size - index) * sizeof(TYPE));
        this->data[index] = element;
        this->size++;
    }
    
    const TYPE& operator[](const size_t index) const
    {
        return this->data[index];
    }
    
    TYPE& operator[](const size_t index)
    {
        return this->data[index];
    }
    
    TYPE* begin()
    {
        return this->data;
    }
    
    TYPE* end()
    {
        if (this->data == nullptr)
            return nullptr;
        else
            return this->data + this->size;
    }
    
    const TYPE* begin() const
    {
        return this->data;
    }
    
    const TYPE* end() const
    {
        if (this->data == nullptr)
            return nullptr;
        else
            return this->data + this->size;
    }
    
    size_t maxAllocationCount, committedPages;
    size_t size, capacity;
    TYPE* data = nullptr;
    Allocator::VAlloc* alloc = nullptr;
};

//------------------------------------------------------------------------------
/**
 StaticArray allocates it's memory from the global Static allocator, use with objects with permanent life time.
 The StaticArray can be initialized by size or from a StackArray.
 
 Does not need to free its memory.
 */
extern size_t LeakedStaticArrayBytes;
template<typename T>
struct StaticArray
{
    T* buf = nullptr;
    size_t capacity = 0;
    size_t size = 0;
    
    StaticArray() {}

    StaticArray(const std::initializer_list<T>& list)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedStaticArrayBytes += this->capacity;
        }
        this->buf = StaticAllocArray<T>(list.size());
        this->capacity = list.size();
        this->size = 0;
        for (auto& val : list)
        {
            this->buf[this->size++] = val;
        }
    }
    
    StaticArray(const size_t size)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedStaticArrayBytes += this->capacity;
        }
        this->capacity = size;
        this->size = 0;
        this->buf = StaticAllocArray<T>(size);
    }

    void operator=(const std::initializer_list<T>& list)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedStaticArrayBytes += this->capacity;
        }
        this->buf = StaticAllocArray<T>(list.size());
        this->capacity = list.size();
        this->size = 0;
        for (auto& val : list)
        {
            this->buf[this->size++] = val;
        }
    }
    
    void operator=(const StackArray<T>& vec)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedStaticArrayBytes += this->capacity;
        }
        this->capacity = vec.size;
        this->buf = StaticAllocArray<T>(vec.size);
        
        if (vec.size > 0)
        {
            // If mempcy suffices, do it
            if (std::is_trivially_copy_constructible<T>::value)
            {
                this->size = vec.size;
                memcpy(this->buf, vec.ptr, vec.size * sizeof(T));
            }
            else
            {
                // Otherwise, run copy constructors for every element
                for (auto& v : vec)
                {
                    this->buf[this->size++] = v;
                }
            }
        }
    }
    
    StaticArray(const StackArray<T>& vec)
    {
        *this = vec;
    }
    
    template <typename U>
    StaticArray(const StackArray<U>& vec)
    {
        static_assert(std::is_assignable<T, U>::value, "No explicit assignment exists between types");
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedStaticArrayBytes += this->capacity;
        }
        this->capacity = vec.capacity;
        this->buf = StaticAllocArray<T>(vec.capacity);
        
        if (vec.size > 0)
        {
            // Otherwise, run copy constructors for every element
            for (auto& v : vec)
            {
                this->buf[this->size++] = v;
            }
        }
    }
    
    const T& operator[](const size_t index) const
    {
        return this->buf[index];
    }
    
    T& operator[](const size_t index)
    {
        return this->buf[index];
    }
    
    T* begin()
    {
        return this->buf;
    }
    
    T* end()
    {
        if (this->buf == nullptr)
            return nullptr;
        else
            return this->buf + this->size;
    }
    
    const T* begin() const
    {
        return this->buf;
    }
    
    const T* end() const
    {
        if (this->buf == nullptr)
            return nullptr;
        else
            return this->buf + this->size;
    }
};

//------------------------------------------------------------------------------
/**
 FixedArray is an array of fixed size (not growing, like PinnedArray) which can be used to consume
 either a StackArray or a StaticArray.
 
 Consumption of a StaticArray is free in terms of memory, and will make this FixedArray point to
 memory allocated with the Static allocator.
 
 Does not need to free its memory explicitly.
 */
extern size_t LeakedFixedArrayBytes;
template<typename T>
struct FixedArray
{
    T* buf = nullptr;
    size_t capacity = 0;
    size_t size = 0;
    
    FixedArray() {}

    FixedArray(const std::initializer_list<T>& list)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->buf = AllocArray<T>(list.size());
        this->capacity = list.size();
        this->size = 0;
        for (auto& val : list)
        {
            this->buf[this->size++] = val;
        }
    }
    
    FixedArray(const size_t size)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->capacity = size;
        this->size = 0;
        this->buf = AllocArray<T>(size);
    }
    
    FixedArray(const std::vector<T>& vec)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->capacity = vec.size();
        this->buf = AllocArray<T>(vec.size());
        
        if (!vec.empty())
        {
            // If mempcy suffices, do it
            if (std::is_trivially_copy_constructible<T>::value)
            {
                this->size = vec.size();
                memcpy(this->buf, vec.data(), vec.size() * sizeof(T));
            }
            else
            {
                // Otherwise, run copy constructors for every element
                for (auto& v : vec)
                {
                    this->buf[this->size++] = v;
                }
            }
        }
    }
    
    FixedArray(FixedArray<T>&& rhs)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->capacity = rhs.capacity;
        this->size = rhs.size;
        this->buf = rhs.buf;
        rhs.buf = nullptr;
        rhs.capacity = 0;
        rhs.size = 0;
    }
    
    FixedArray(const FixedArray<T>& vec)
    {
        *this = vec;
    }

    void operator=(const std::initializer_list<T>& list)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->buf = AllocArray<T>(list.size());
        this->capacity = list.size();
        this->size = 0;
        for (auto& val : list)
        {
            this->buf[this->size++] = val;
        }
    }
    
    void operator=(const FixedArray<T>& vec)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->capacity = vec.capacity;
        this->buf = AllocArray<T>(vec.capacity);
        this->size = 0;
        
        if (vec.size > 0)
        {
            // If mempcy suffices, do it
            if (std::is_trivially_copy_constructible<T>::value)
            {
                this->size = vec.size;
                memcpy(this->buf, vec.buf, vec.size * sizeof(T));
            }
            else
            {
                // Otherwise, run copy constructors for every element
                for (auto& v : vec)
                {
                    this->buf[this->size++] = v;
                }
            }
        }
    }

    void operator=(FixedArray<T>&& vec)
    {
        this->buf = vec.buf;
        this->capacity = vec.capacity;
        this->size = vec.size;
        vec.buf = nullptr;
        vec.capacity = 0;
        vec.size = 0;
    }
    
    void operator=(const StaticArray<T>& vec)
    {
        this->buf = vec.buf;
        this->size = vec.size;
        this->capacity = vec.size; // intentional to avoid more appending
    }

    void operator=(StaticArray<T>&& vec)
    {
        this->buf = vec.buf;
        this->size = vec.size;
        this->capacity = vec.capacity; // intentional to avoid more appending
        vec.buf = nullptr;
        vec.capacity = 0;
        vec.size = 0;
    }
    
    void operator=(const StackArray<T>& vec)
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->capacity = vec.size;
        this->buf = AllocArray<T>(vec.size);
        this->size = 0;
        
        if (vec.size > 0)
        {
            // If mempcy suffices, do it
            if (std::is_trivially_copy_constructible<T>::value)
            {
                this->size = vec.size;
                memcpy(this->buf, vec.ptr, vec.size * sizeof(T));
            }
            else
            {
                // Otherwise, run copy constructors for every element
                for (auto& v : vec)
                {
                    this->buf[this->size++] = v;
                }
            }
        }
    }
    
    FixedArray(const StackArray<T>& vec)
    {
        *this = vec;
    }
    
    template<typename U>
    void operator=(const StackArray<U>& vec)
    {
        static_assert(std::is_assignable<T, U>::value, "No explicit assignment exists between types");
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
        this->capacity = vec.size;
        this->buf = AllocArray<T>(vec.size);
        
        if (vec.size > 0)
        {
            // Otherwise, run copy constructors for every element
            for (auto& v : vec)
            {
                this->buf[this->size++] = v;
            }
        }
    }
    
    ~FixedArray()
    {
        if (this->buf != nullptr)
        {
            // leak memory
            LeakedFixedArrayBytes += this->capacity;
        }
    }
    
    void Append(const T& t)
    {
        assert(this->size < this->capacity);
        this->buf[this->size++] = t;
    }
    
    // Allow stack allocated arrays to insert themselves, given enough space is available
    void Append(const StackArray<T>& vec)
    {
        assert(this->size + vec.size < this->capacity);
        // If mempcy suffices, do it
        if (std::is_trivially_copy_constructible<T>::value)
        {
            this->size = vec.size();
            memcpy(this->buf, vec.data(), vec.size() * sizeof(T));
        }
        else
        {
            // Otherwise, run copy constructors for every element
            for (auto& v : vec)
            {
                this->buf[this->size++] = v;
            }
        }
    }
    
    const T& operator[](const size_t index) const
    {
        return this->buf[index];
    }
    
    T& operator[](const size_t index)
    {
        return this->buf[index];
    }
    
    const T& front() const
    {
        return this->buf[0];
    }
    
    T& front()
    {
        return this->buf[0];
    }
    
    const T& back() const
    {
        return this->buf[this->size - 1];
    }
    
    T& back()
    {
        return this->buf[this->size - 1];
    }
    
    T* begin()
    {
        return this->buf;
    }
    
    T* end()
    {
        if (this->buf == nullptr)
            return nullptr;
        else
            return this->buf + this->size;
    }
    
    const T* begin() const
    {
        return this->buf;
    }
    
    const T* end() const
    {
        if (this->buf == nullptr)
            return nullptr;
        else
            return this->buf + this->size;
    }
};

//------------------------------------------------------------------------------
/**
 A StackMap is an ideally small map that lives on the stack
 
 Frees its memory upon destruction.
 */
template <typename K, typename V>
struct StackMap
{
    using item = std::pair<K, V>;
    StackMap()
        : searchValid(true)
    {
        
    }
    
    StackMap(size_t size)
        : searchValid(true)
    {
        this->data = StackArray<item>(size);
    }
    
    void Insert(const K& key, const V& value)
    {
        this->data.Append(std::make_pair(key, value));
        if (this->searchValid)
        {
            this->Sort();
        }
    }
    
    item* Emplace(const K& key)
    {
        assert(this->searchValid);
        this->data.Append(std::make_pair(key, V()));
        this->Sort();
        return this->Find(key);
    }
    
    void Erase(const K& key)
    {
        auto it = this->Find(key);
        this->data.Erase(it);
    }
    
    void BeginBulkAdd()
    {
        this->searchValid = false;
    }
    
    void EndBulkAdd()
    {
        this->Sort();
    }
    
    void Sort()
    {
        std::sort(this->data.begin(), this->data.end(), [](const item& lhs, const item& rhs)
        {
            return lhs.first < rhs.first;
        });
        this->searchValid = true;
    }
    
    item* Find(const K& key)
    {
        assert(this->searchValid);
        struct Comp
        {
            bool operator()(const K& key, const item& item) { return key < item.first; }
            bool operator()(const item& item, const K& key) { return item.first < key; }
        };
        auto it = std::equal_range(this->data.begin(), this->data.end(), key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    const item* Find(const K& key) const
    {
        assert(this->searchValid);
        struct Comp
        {
            bool operator()(const K& key, const item& item) { return key < item.first; }
            bool operator()(const item& item, const K& key) { return item.first < key; }
        };
        auto it = std::equal_range(this->data.begin(), this->data.end(), key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    void Clear()
    {
        this->data.Clear();
    }
    
    bool Empty()
    {
        return this->data.size == 0;
    }
    
    item* begin()
    {
        return this->data.begin();
    }
    
    item* end()
    {
        return this->data.end();
    }
    
    const item* begin() const
    {
        return this->data.begin();
    }
    
    const item* end() const
    {
        return this->data.end();
    }
    
    bool searchValid;
    StackArray<item> data;
};

//------------------------------------------------------------------------------
/**
 Much like PinnedArray, a PinnedMap implements a binary search and sort for it's items,
 which are stored as key-value pairs.
 
 Frees its memory upon destruction.
 */
template <typename K, typename V>
struct PinnedMap
{
    using item = std::pair<K, V>;
    PinnedMap()
    : searchValid(true)
    {
        
    }
    
    PinnedMap(size_t maxAllocationCount)
    : searchValid(true)
    {
        this->data = PinnedArray<item>(maxAllocationCount);
    }
    
    void Insert(const K& key, const V& value)
    {
        this->data.Append(std::make_pair(key, value));
        if (this->searchValid)
        {
            this->Sort();
        }
    }
    
    item* Emplace(const K& key)
    {
        assert(this->searchValid);
        this->data.Append(std::make_pair(key, V()));
        this->Sort();
        return this->Find(key);
    }
    
    void Erase(const K& key)
    {
        auto it = this->Find(key);
        this->data.Erase(it);
    }
    
    void BeginBulkAdd()
    {
        this->searchValid = false;
    }
    
    void EndBulkAdd()
    {
        this->Sort();
    }
    
    void Sort()
    {
        std::sort(this->data.begin(), this->data.end(), [](const item& lhs, const item& rhs)
        {
            return lhs.first < rhs.first;
        });
        this->searchValid = true;
    }
    
    item* Find(const K& key)
    {
        assert(this->searchValid);
        struct Comp
        {
            bool operator()(const K& key, const item& item) { return key < item.first; }
            bool operator()(const item& item, const K& key) { return item.first < key; }
        };
        auto it = std::equal_range(this->data.begin(), this->data.end(), key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    const item* Find(const K& key) const
    {
        assert(this->searchValid);
        struct Comp
        {
            bool operator()(const K& key, const item& item) { return key < item.first; }
            bool operator()(const item& item, const K& key) { return item.first < key; }
        };
        auto it = std::equal_range(this->data.begin(), this->data.end(), key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    const std::pair<const item*, const item*> FindRange(const K& key) const
    {
        assert(this->searchValid);
        struct Comp
        {
            bool operator()(const K& key, const item& item) { return key < item.first; }
            bool operator()(const item& item, const K& key) { return item.first < key; }
        };
        auto it = std::equal_range(this->data.begin(), this->data.end(), key, Comp{});
        
        return it;
    }
    
    void Clear()
    {
        this->data.Clear();
    }
    
    item* begin()
    {
        return this->data.begin();
    }
    
    item* end()
    {
        return this->data.end();
    }
    
    const item* begin() const
    {
        return this->data.begin();
    }
    
    const item* end() const
    {
        return this->data.end();
    }
    
    bool searchValid;
    PinnedArray<item> data;
};

//------------------------------------------------------------------------------
/**
 StaticMap is a map which unlike PinnedMap has a fixed size, and it's life time is the duration of the
 application.
 
 Does not need to free its memory.
 */
template <typename K, typename V>
struct StaticMap
{
    using item = std::pair<K, V>;
    StaticMap(const std::initializer_list<item>& items)
    {
        this->data = StaticAllocArray<item>(items.size());
        this->size = 0;
        for (auto& item : items)
        {
            this->data[this->size++] = std::make_pair(item.first, item.second);
        }
        
        std::sort(this->data, this->data + this->size, [](const item& lhs, const item& rhs)
                  {
            return lhs.first < rhs.first;
        });
    }
    
    const item* Find(const K& key) const
    {
        struct Comp
        {
            bool operator()(const K& key, const item& item) { return key < item.first; }
            bool operator()(const item& item, const K& key) { return item.first < key; }
        };
        auto it = std::equal_range(this->data, this->data + this->size, key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    template <typename U>
    const item* Find(const U& key) const
    {
        struct Comp
        {
            bool operator()(const U& key, const item& item) { return key < item.first; }
            bool operator()(const item& item, const U& key) { return item.first < key; }
        };
        auto it = std::equal_range(this->data, this->data + this->size, key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    item* begin()
    {
        return this->data;
    }
    
    item* end()
    {
        return this->data + this->size;
    }
    
    const item* begin() const
    {
        return this->data;
    }
    
    const item* end() const
    {
        return this->data + this->size;
    }
    
    item* data;
    size_t size;
};

//------------------------------------------------------------------------------
/**
 StaticSet is a non-growing Set which has to be initialized to the items it needs.
 
 Does not need to free its memory.
 */
template <typename K>
struct StaticSet
{
    StaticSet(const std::initializer_list<K>& items)
    {
        this->data = StaticAllocArray<K>(items.size());
        this->size = 0;
        for (auto& item : items)
        {
            this->data[this->size++] = item;
        }
        
        std::sort(this->data, this->data + this->size, [](const K& lhs, const K& rhs)
                  {
            return lhs < rhs;
        });
        
        // Remove duplicates
        for (size_t i = 0; i < this->size; i++)
        {
            if (i > 0)
            {
                if (this->data[i] == this->data[i-1])
                {
                    assert(false && "Duplicate entries in StaticSet are not allowed");
                    /* If we want to allow this, fix the set like so:
                     memmove(this->data + i, this->data + i + 1, (this->size - i) * sizeof(K));
                     i--;
                     this->size--;
                     */
                }
            }
        }
    }
    
    const K* Find(const K& key) const
    {
        struct Comp
        {
            bool operator()(const K& key, const K& item) { return key < item; }
        };
        auto it = std::equal_range(this->data, this->data + this->size, key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    K* begin()
    {
        return this->data;
    }
    
    K* end()
    {
        return this->data + this->size;
    }
    
    const K* begin() const
    {
        return this->data;
    }
    
    const K* end() const
    {
        return this->data + this->size;
    }
    
    K* data;
    size_t size;
};

//------------------------------------------------------------------------------
/**
 Like PinnedMap and PinnedArray, PinnedSet is a growing set type
 */
template <typename K>
struct PinnedSet
{
    PinnedSet()
    : searchValid(true)
    {
    }
    
    PinnedSet(size_t maxAllocationCount)
    : searchValid(true)
    {
        this->data = PinnedArray<K>(maxAllocationCount);
    }
    
    void Insert(const K& key)
    {
        this->data.Append(key);
        if (this->searchValid)
        {
            this->Sort();
        }
    }
    
    void Insert(const StaticSet<K>& set)
    {
        this->BeginBulkAdd();
        for (auto& val : set)
        {
            this->Insert(val);
        }
        this->EndBulkAdd();
    }
    
    template<typename K2>
    void Insert(const StaticSet<K2>& set)
    {
        this->BeginBulkAdd();
        for (auto& val : set)
        {
            this->Insert(K(val));
        }
        this->EndBulkAdd();
    }
    
    void BeginBulkAdd()
    {
        this->searchValid = false;
    }
    
    void EndBulkAdd()
    {
        this->Sort();
    }
    
    void Sort()
    {
        std::sort(this->data.begin(), this->data.end(), [](const K& lhs, const K& rhs)
                  {
            return lhs < rhs;
        });
        this->searchValid = true;
    }
    
    void Clear()
    {
        this->data.Clear();
    }
    
    const K* Find(const K& key) const
    {
        struct Comp
        {
            bool operator()(const K& key, const K& item) { return key < item; }
        };
        auto it = std::equal_range(this->data.begin(), this->data.end(), key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    size_t size()
    {
        return this->data.size;
    }
    
    K* begin()
    {
        return this->data.begin();
    }
    
    K* end()
    {
        return this->data.end();
    }
    
    const K* begin() const
    {
        return this->data.begin();
    }
    
    const K* end() const
    {
        return this->data.end();
    }
    
    bool searchValid;
    PinnedArray<K> data;
};

template<typename K>
struct FixedSet
{
    FixedSet()
        : searchValid(true)
    {
    }
    
    FixedSet(size_t maxAllocationCount)
        : searchValid(true)
    {
        this->data = FixedArray<K>(maxAllocationCount);
    }
    
    void Insert(const K& key)
    {
        this->data.Append(key);
        if (this->searchValid)
        {
            this->Sort();
        }
    }
    
    void Insert(const StaticSet<K>& set)
    {
        this->BeginBulkAdd();
        for (auto& val : set)
        {
            this->Insert(val);
        }
        this->EndBulkAdd();
    }
    
    template<typename K2>
    void Insert(const StaticSet<K2>& set)
    {
        this->BeginBulkAdd();
        for (auto& val : set)
        {
            this->Insert(K(val));
        }
        this->EndBulkAdd();
    }
    
    void BeginBulkAdd()
    {
        this->searchValid = false;
    }
    
    void EndBulkAdd()
    {
        this->Sort();
    }
    
    void Sort()
    {
        std::sort(this->data.begin(), this->data.end(), [](const K& lhs, const K& rhs)
        {
            return lhs < rhs;
        });
        this->searchValid = true;
    }
    
    void Clear()
    {
        this->data.Clear();
    }
    
    const K* Find(const K& key) const
    {
        struct Comp
        {
            bool operator()(const K& key, const K& item) { return key < item; }
        };
        auto it = std::equal_range(this->data.begin(), this->data.end(), key, Comp{});
        
        auto [beginRange, endRange] = it;
        return beginRange == endRange ? this->end() : beginRange;
    }
    
    size_t size()
    {
        return this->data.size;
    }
    
    K* begin()
    {
        return this->data.begin();
    }
    
    K* end()
    {
        return this->data.end();
    }
    
    const K* begin() const
    {
        return this->data.begin();
    }
    
    const K* end() const
    {
        return this->data.end();
    }
    
    bool searchValid;
    FixedArray<K> data;
};

} // namespace GPULang
