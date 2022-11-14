#ifndef __HASH_SYM__
#define __HASH_SYM__

uint64_t GetHash(void* struct_ptr, size_t size);

uint64_t GetHash(void* struct_ptr, size_t size)
{
    if (struct_ptr == nullptr)
        return -1;

    char* ptr = (char*)struct_ptr;

    size_t hash = 5381;
    for(size_t i = 0; i < size; i++)
        hash = (size_t)(ptr[i] + hash*33);

    return hash;
}

#endif  //__HASH_SYM__