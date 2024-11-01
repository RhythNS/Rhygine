#pragma once

// Copied from Libs/Mimalloc/include/mimalloc-new-delete.h

#include <new>
#include <mimalloc.h>

#if defined(TRACY_MEMORY_EVERYTHING)
#include <tracy/Tracy.hpp>
#define track_alloc(ptr, size) TracyAlloc(ptr, size)
#define track_delete(ptr) TracyFree(ptr)
#else
#define track_alloc(ptr, size)
#define track_delete(ptr)
#endif

#if defined(_MSC_VER) && defined(_Ret_notnull_) && defined(_Post_writable_byte_size_)
// stay consistent with VCRT definitions
#define mi_decl_new(n)          mi_decl_nodiscard mi_decl_restrict _Ret_notnull_ _Post_writable_byte_size_(n)
#define mi_decl_new_nothrow(n)  mi_decl_nodiscard mi_decl_restrict _Ret_maybenull_ _Success_(return != NULL) _Post_writable_byte_size_(n)
#else
#define mi_decl_new(n)          mi_decl_nodiscard mi_decl_restrict
#define mi_decl_new_nothrow(n)  mi_decl_nodiscard mi_decl_restrict
#endif

void operator delete(void* p) noexcept
{
	track_delete(p);
	mi_free(p);
};
void operator delete[](void* p) noexcept
{
	track_delete(p);
	mi_free(p);
};

void operator delete(void* p, const std::nothrow_t&) noexcept
{
	track_delete(p);
	mi_free(p);
}
void operator delete[](void* p, const std::nothrow_t&) noexcept
{
	track_delete(p);
	mi_free(p);
}

mi_decl_new(n) void* operator new(std::size_t n) noexcept(false) 
{ 
	void* ptr = mi_new(n);
	track_alloc(ptr, n);
	return ptr;
}
mi_decl_new(n) void* operator new[](std::size_t n) noexcept(false) 
{
	void* ptr = mi_new(n);
	track_alloc(ptr, n);
	return ptr;
}

mi_decl_new_nothrow(n) void* operator new(std::size_t n, const std::nothrow_t& tag) noexcept 
{ 
	(void)(tag);
	void* ptr = mi_new_nothrow(n);
	track_alloc(ptr, n);
	return ptr;
}
mi_decl_new_nothrow(n) void* operator new[](std::size_t n, const std::nothrow_t& tag)noexcept
{
	(void)(tag);
	void* ptr = mi_new_nothrow(n);
	track_alloc(ptr, n);
	return ptr;
}

#if (__cplusplus >= 201402L || _MSC_VER >= 1916)
void operator delete(void* p, std::size_t n) noexcept
{
	track_delete(p);
	mi_free_size(p, n);
};
void operator delete[](void* p, std::size_t n) noexcept
{
	track_delete(p);
	mi_free_size(p, n);
};
#endif

#if (__cplusplus > 201402L || defined(__cpp_aligned_new))
void operator delete(void* p, std::align_val_t al) noexcept
{
	track_delete(p);
	mi_free_aligned(p, static_cast<size_t>(al));
}
void operator delete[](void* p, std::align_val_t al) noexcept
{
	track_delete(p);
	mi_free_aligned(p, static_cast<size_t>(al));
}
void operator delete(void* p, std::size_t n, std::align_val_t al) noexcept
{
	track_delete(p);
	mi_free_size_aligned(p, n, static_cast<size_t>(al));
};
void operator delete[](void* p, std::size_t n, std::align_val_t al) noexcept
{
	track_delete(p);
	mi_free_size_aligned(p, n, static_cast<size_t>(al));
};
void operator delete(void* p, std::align_val_t al, const std::nothrow_t&) noexcept
{
	track_delete(p);
	mi_free_aligned(p, static_cast<size_t>(al));
}
void operator delete[](void* p, std::align_val_t al, const std::nothrow_t&) noexcept
{
	track_delete(p);
	mi_free_aligned(p, static_cast<size_t>(al));
}

void* operator new(std::size_t n, std::align_val_t al) noexcept(false)
{ 
	void* ptr = mi_new_aligned(n, static_cast<size_t>(al));
	track_alloc(ptr, n);
	return ptr;
}
void* operator new[](std::size_t n, std::align_val_t al) noexcept(false)
{ 
	void* ptr = mi_new_aligned(n, static_cast<size_t>(al));
	track_alloc(ptr, n);
	return ptr;
}
void* operator new(std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept
{ 
	void* ptr = mi_new_aligned_nothrow(n, static_cast<size_t>(al));
	track_alloc(ptr, n);
	return ptr;
}
void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept
{ 
	void* ptr = mi_new_aligned_nothrow(n, static_cast<size_t>(al));
	track_alloc(ptr, n);
	return ptr;
}
#endif
