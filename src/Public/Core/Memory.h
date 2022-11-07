/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef GREAPER_CORE_MEMORY_H
#define GREAPER_CORE_MEMORY_H 1

#include <vector>
#include <list>
#include <forward_list>
#include <queue>
#include <deque>
#include <stack>
#include <string>
#include <string_view>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstdarg>
#include <functional>
#if PLT_LINUX
#include <iostream>
#endif

#if COMPILER_MSVC
#define Break(msg, ...) greaper::Impl::_TriggerBreak(greaper::Format("STOP! at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__))
#else
#define Break(msg, ...) greaper::Impl::_TriggerBreak(greaper::Format("STOP! at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__))
#endif

#if GREAPER_ENABLE_BREAK
#if PLT_WINDOWS
#define Verify(exp, msg, ...) { if(!(exp)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #exp " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyNot(exp, msg, ...) { if((exp)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: !" #exp " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyInequal(a, b, msg, ...) { if((a) == (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " != " #b " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyEqual(a, b, msg, ...) { if((a) != (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " == " #b " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyGreater(a, b, msg, ...) { if((a) <= (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " > " #b " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyGreaterEqual(a, b, msg, ...) { if((a) < (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " >= " #b " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyLess(a, b, msg, ...) { if((a) >= (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " < " #b " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyLessEqual(a, b, msg, ...) { if((a) > (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " <= " #b " not verified, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyNull(ptr, msg, ...) { if((ptr) != nullptr) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #ptr " is not nullptr, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyNotNull(ptr, msg, ...) { if((ptr) == nullptr) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #ptr " is nullptr, at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyWithin(value, min, max, msg, ...) { if((value) <= (min)|| (value) >= (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not inside of (" #min ", " #max ") range , at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyWithinInclusive(value, min, max, msg, ...) { if((value) < (min) || (value) > (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not inside of [" #min ", " #max "] range , at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyNotWithin(value, min, max, msg, ...) { if((value) >= (min) && (value) <= (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not outside of (" #min ", " #max ") range , at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#define VerifyNotWithinInclusive(value, min, max, msg, ...) { if((value) > (min) && (value) < (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not outside of [" #min ", " #max "] range , at: " FUNCTION_FULL ", message: " msg, __VA_ARGS__)); }
#else
#define Verify(exp, msg, ...) { if(!(exp)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #exp " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyNot(exp, msg, ...) { if((exp)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: !" #exp " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyInequal(a, b, msg, ...) { if((a) == (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " != " #b " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyEqual(a, b, msg, ...) { if((a) != (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " == " #b " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyGreater(a, b, msg, ...) { if((a) <= (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " > " #b " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyGreaterEqual(a, b, msg, ...) { if((a) < (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " >= " #b " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyLess(a, b, msg, ...) { if((a) >= (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " < " #b " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyLessEqual(a, b, msg, ...) { if((a) > (b)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #a " <= " #b " not verified, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyNull(ptr, msg, ...) { if((ptr) != nullptr) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #ptr " is not nullptr, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyNotNull(ptr, msg, ...) { if((ptr) == nullptr) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #ptr " is nullptr, at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyWithin(value, min, max, msg, ...) { if((value) <= (min)|| (value) >= (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not inside of (" #min ", " #max ") range , at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyWithinInclusive(value, min, max, msg, ...) { if((value) < (min) || (value) > (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not inside of [" #min ", " #max "] range , at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyNotWithin(value, min, max, msg, ...) { if((value) >= (min) && (value) <= (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not outside of (" #min ", " #max ") range , at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#define VerifyNotWithinInclusive(value, min, max, msg, ...) { if((value) > (min) && (value) < (max)) greaper::Impl::_TriggerBreak(greaper::Format("STOP! exp: " #value " not outside of [" #min ", " #max "] range , at: %s, message: " msg, FUNCTION_FULL, ##__VA_ARGS__)); }
#endif
#else
#define Verify(exp, ...) { volatile bool res = (exp); }
#define VerifyNot(exp, ...) { volatile bool res = !(exp); }
#define VerifyInequal(a, b, ...) { volatile bool res = (a) != (b); }
#define VerifyEqual(a, b, ...) { volatile bool res = (a) == (b); }
#define VerifyGreater(a, b, ...) { volatile bool res = (a) > (b); }
#define VerifyGreaterEqual(a, b, ...) { volatile bool res = (a) >= (b); }
#define VerifyLess(a, b, ...) { volatile bool res = (a) < (b); }
#define VerifyLessEqual(a, b, ...) { volatile bool res = (a) <= (b); }
#define VerifyNull(ptr, ...) { volatile bool res = (ptr) == nullptr; }
#define VerifyNotNull(ptr, ...) { volatile bool res = (ptr) != nullptr; }
#define VerifyWithin(val, min, max, ...) { volatile bool res = (value) > (min) && (value) < (max); }
#define VerifyWithinInclusive(val, min, max, ...) { volatile bool res = (value) >= (min) && (value) <= (max); }
#define VerifyNotWithin(val, min, max, ...) { volatile bool res = (value) < (min) && (value) > (max); }
#define VerifyNotWithinInclusive(val, min, max, ...) { volatile bool res = (value) <= (min) && (value) >= (max); }
#endif

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace greaper
{
	template<class T>
	class MemoryAllocator
	{
	public:
		static void* Allocate(sizet byteSize);

		static void* AllocateAligned(sizet byteSize, sizet alignment);

		static void Deallocate(void* mem);

		static void DeallocateAligned(void* mem);
	};

	class GenericAllocator { };

	template<class _Alloc_ = GenericAllocator>
	INLINE void* Alloc(sizet byteSize)
	{
		return MemoryAllocator<_Alloc_>::Allocate(byteSize);
		//return malloc(byteSize);
	}

	template<class T, class _Alloc_ = GenericAllocator>
	INLINE T* AllocN(sizet N)
	{
		return static_cast<T*>(Alloc<_Alloc_>(sizeof(T) * N));
	}

	template<class T, class _Alloc_ = GenericAllocator>
	INLINE T* AllocT()
	{
		return AllocN<T, _Alloc_>(1);
	}

	template<class T, class _Alloc_ = GenericAllocator, class... Args>
	INLINE T* ConstructN(sizet count, Args&&... args)
	{
		T* mem = AllocN<T, _Alloc_>(count);
		for (sizet i = 0; i < count; ++i)
			new (reinterpret_cast<void*>(&mem[i]))T(std::forward<Args>(args)...);
		return mem;
	}

	template<class T, class _Alloc_ = GenericAllocator, class... Args>
	INLINE T* Construct(Args&&... args)
	{
		return ConstructN<T, _Alloc_>(1, args...);
	}
	
	template<class _Alloc_ = GenericAllocator>
	INLINE void Dealloc(void* mem)
	{
		MemoryAllocator<_Alloc_>::Deallocate(mem);
		//free(mem);
	}

	template<class T, class _Alloc_ = GenericAllocator>
	INLINE void Destroy(T* ptr, sizet count = 1)
	{
		for (sizet i = 0; i < count; ++i)
			ptr[i].~T();
		Dealloc<_Alloc_>(ptr);
	}

/**
 * @brief Makes a class friend of the base construction/destruction functions
 * 
 */
#define MemoryFriend() \
template<class _T_, class _Alloc_, class... Args> friend _T_* greaper::ConstructN(sizet, Args&&...); \
template<class _T_, class _Alloc_> friend void greaper::Destroy(_T_*, sizet)

	/*template<typename T>
	using SPtr = std::shared_ptr<T>;
	template<typename T>
	using WeakSPtr = std::weak_ptr<T>;
	template<typename T>
	using UPtr = std::unique_ptr<T>;
	 */

	template<class T, class _Alloc_ = GenericAllocator>
	class Destructor
	{
	public:
		constexpr Destructor() noexcept = default;

		template<class T2, std::enable_if_t<std::is_convertible<T2*, T*>::value, int> = 0>
		constexpr Destructor(const Destructor<T2, _Alloc_>&) {	}

		void operator()(T* ptr)const
		{
			Destroy<T, _Alloc_>(ptr, 1);
		}
	};

	template<class T, class _Alloc_ = GenericAllocator>
	class StdAlloc
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using size_type = sizet;
		using difference_type = ptrint;

		StdAlloc()noexcept = default;
		StdAlloc(StdAlloc&&)noexcept = default;
		StdAlloc(const StdAlloc&)noexcept = default;

		template<class U, class Alloc2> INLINE StdAlloc(const StdAlloc<U, Alloc2>&) {  }
		template<class U, class Alloc2> INLINE constexpr bool operator==(const StdAlloc<U, Alloc2>&) const noexcept { return true; }
		template<class U, class Alloc2> INLINE constexpr bool operator!=(const StdAlloc<U, Alloc2>&) const noexcept { return false; }

		template<class U> class rebind { public: using other = StdAlloc<U, _Alloc_>; };

		INLINE T* allocate(const size_t num)
		{
#if GREAPER_DEBUG_ALLOCATION
			if (num == 0)
				return nullptr;
			if (num > max_size())
				return nullptr;

			void*const p = AllocN<T, _Alloc_>(num);
			if (p == nullptr)
				return nullptr;
			return static_cast<T*>(p);
#else
			return reinterpret_cast<T*>(AllocN<T, _Alloc_>(num));
#endif
		}

		INLINE void deallocate(pointer p, size_type)
		{
			Dealloc<_Alloc_>(p);
		}

		INLINE constexpr size_t max_size() { return std::numeric_limits<size_type>::max() / sizeof(T); }
		INLINE void destroy(pointer p) { (p)->~T(); }

		template<class... Args>
		INLINE void construct(pointer p, Args&&... args)noexcept { new(p)T(std::forward<Args>(args)...); }
	};

	template<typename T, typename A = StdAlloc<T>>
	using BasicString = std::basic_string<T, std::char_traits<T>, A>;
	template<typename T>
	using BasicStringView = std::basic_string_view<T, std::char_traits<T>>;

	template<typename T, typename A = StdAlloc<T>>
	using BasicStringStream = std::basic_stringstream<T, std::char_traits<T>, A>;

	using String = BasicString<achar>;
	using StringView = BasicStringView<achar>;
	using StringStream = BasicStringStream<achar>;
	using WString = BasicString<wchar>;
	using WStringView = BasicStringView<wchar>;
	using WStringStream = BasicStringStream<wchar>;
	using U16String = BasicString<u16char>;
	using U16StringStream = BasicStringStream<u16char>;
	using U32String = BasicString<u32char>;
	using U32StringStream = BasicStringStream<u32char>;

	namespace Impl
	{
		void _LogBreak(const String& msg);
		void _TriggerBreak(const String& str);
	}

	template<typename T, typename A = StdAlloc<T>>
	using Vector = std::vector<T, A>;

	using StringVec = Vector<String>;
	using WStringVec = Vector<WString>;

	template<typename T, typename A = StdAlloc<T>>
	using Queue = std::queue<T, std::deque<T, A>>;

	template<typename T, typename A = StdAlloc<T>>
	using Deque = std::deque<T, A>;

	template<typename T, typename A = StdAlloc<T>>
	using List = std::list<T, A>;

	template<typename T, typename A = StdAlloc<T>>
	using ForwardList = std::forward_list<T, A>;

	template<typename T, typename A = StdAlloc<T>>
	using Stack = std::stack<T, std::deque<T, A>>;

	template<typename K, typename P = std::less<K>, typename A = StdAlloc<K>>
	using Set = std::set<K, P, A>;

	template<typename K, typename V, typename P = std::less<K>, typename A = StdAlloc<std::pair<const K, V>>>
	using Map = std::map<K, V, P, A>;

	template<typename K, typename P = std::less<K>, typename A = StdAlloc<K>>
	using MultiSet = std::multiset<K, P, A>;

	template<typename K, typename V, typename P = std::less<K>, typename A = StdAlloc<std::pair<const K, V>>>
	using MultiMap = std::multimap<K, V, P, A>;

	struct EnumClassHash
	{
		template<typename T>
		INLINE constexpr std::size_t operator()(T t)const
		{
			return static_cast<std::size_t>(t);
		}
	};
	template<typename Key>
	using HashType = typename std::conditional_t<std::is_enum_v<Key>, EnumClassHash, std::hash<Key>>;

	template<typename T, typename H = HashType<T>, typename C = std::equal_to<T>, typename A = StdAlloc<T>>
	using UnorderedSet = std::unordered_set<T, H, C, A>;

	template<typename K, typename V, typename H = HashType<K>, typename C = std::equal_to<K>, typename A = StdAlloc<std::pair<const K, V>>>
	using UnorderedMap = std::unordered_map<K, V, H, C, A>;

	template<typename K, typename V, typename H = HashType<K>, typename C = std::equal_to<K>, typename A = StdAlloc<std::pair<const K, V>>>
	using UnorderedMultiMap = std::unordered_multimap<K, V, H, C, A>;

	template<typename T, typename H = HashType<T>, typename C = std::equal_to<T>, typename A = StdAlloc<T>>
	using UnorderedMultiSet = std::unordered_multiset<T, H, C, A>;

	struct Snprintf
	{
		static int Fn(achar* buffer, size_t bufferLen, const achar* fmt, va_list argList)
		{
			return vsnprintf(buffer, bufferLen, fmt, argList);
		}
		static int Fn(wchar* buffer, size_t bufferLen, const wchar* fmt, va_list argList)
		{
			return vswprintf(buffer, bufferLen, fmt, argList);
		}
	};

	template<typename T, class _Alloc_ = GenericAllocator>
	[[nodiscard]] BasicString<T, StdAlloc<T, _Alloc_>> Format(const T* fmt, ...)
	{
		va_list argList;
		va_start(argList, fmt);

		const auto size = (ssizet)Snprintf::Fn((T*)nullptr, 0, fmt, argList);

		VerifyGreaterEqual(size, 0, "Error while formatting");

		va_end(argList);
		va_start(argList, fmt);

		BasicString<T, StdAlloc<T, _Alloc_>> str {};
		str.resize(size + 1, (T)0);

		Snprintf::Fn(str.data(), str.size(), fmt, argList);

		va_end(argList);

		return str;
	}

	template<typename T, class _Alloc_ = GenericAllocator>
	[[nodiscard]] BasicString<T, StdAlloc<T, _Alloc_>> Format(const BasicString<T, _Alloc_>& fmt, ...)
	{
		va_list argList;
		va_start(argList, fmt.c_str());

		const auto size = (ssizet)Snprintf::Fn((T*)nullptr, 0, fmt.c_str(), argList);

		VerifyGreaterEqual(size, 0, "Error while formatting");

		va_end(argList);
		va_start(argList, fmt);

		BasicString<T, StdAlloc<T, _Alloc_>> str{};
		str.resize(size + 1, (T)0);

		Snprintf::Fn(str.data(), str.size(), fmt.c_str(), argList);

		va_end(argList);

		return str;
	}

#define I8_HEX_FMT "0x%02X"
#define I16_HEX_FMT "0x%04X"
#define I32_HEX_FMT "0x%08X"
#define I64_HEX_FMT "0x%016I64X"

#if ARCHITECTURE_X64
#define PTR_HEX_FMT I64_HEX_FMT
#else
#define PTR_HEX_FMT I32_HEX_FMT
#endif

	namespace Impl
	{
		template<class T, class _Alloc_ = GenericAllocator>
		INLINE void DefaultDeleter(T* ptr)
		{
			Destroy<T, _Alloc_>(ptr);
		}
		template<class T>
		INLINE void EmptyDeleter(UNUSED T* ptr)
		{

		}
		template<class T, class _Alloc_ = GenericAllocator>
		INLINE void CStrucDeleter(T* ptr)
		{
			Dealloc<_Alloc_>(ptr);
		}
		template<class T>
		using SPtrDeleterFn_t = std::function<void(T*)>;
	}
}

struct SourceLocation
{
	greaper::StringView Function;
	greaper::StringView File;
	uint32 Line = 0;

	constexpr SourceLocation()noexcept = default;
};
#define GETSOURCELOC() SourceLocation{ __FUNCSIG__, __FILE__, (uint32)__LINE__ }

#include "Base/UPtr.h"
#include "Base/SPtr.h"
#include "Base/WPtr.h"

#include "Base/Span.h"

namespace greaper::Impl
{
	INLINE void _LogBreak(const String& str)
	{
		FILE* file = nullptr;
#if PLT_WINDOWS
		const auto res = fopen_s(&file, "ERROR.log", "w");
#elif PLT_LINUX
		uint32 res = 0;
		file = fopen("ERROR.log", "w");
#endif
		if(res == 0 && file != nullptr)
		{
			const auto written = fwrite(str.data(), sizeof(String::value_type), str.size(), file);
			if (written != str.size())
				DEBUG_OUTPUT("Couldn't write all ERROR.log contents.");

			fclose(file);
		}
	}
	
	INLINE void _TriggerBreak(const String& str)
	{
#if PLT_WINDOWS
		constexpr UINT mbType = MB_ICONERROR
#if GREAPER_DEBUG_BREAK
			| MB_ABORTRETRYIGNORE
#else
			| MB_OK
#endif
			| MB_TASKMODAL | MB_TOPMOST;

		const auto retVal = MessageBoxA(nullptr, str.data(), "Greaper Assertion", mbType);

		if (retVal == IDRETRY)
		{
			TRIGGER_BREAKPOINT();
		}
		else if (retVal == IDIGNORE)
		{
			return;
		}
#else // ^^^ PLT_WINDOWS // PLT_OTHER vvv
		std::cerr << "Greaper Assertion: " << str;
#endif

#if GREAPER_DEBUG_BREAK
		TRIGGER_BREAKPOINT();
#endif
		_LogBreak(str);
		exit(EXIT_FAILURE);
	}
}

template<class T>
void* greaper::MemoryAllocator<T>::Allocate(sizet byteSize)
{
#if GREAPER_DEBUG_ALLOCATION
	if (byteSize == 0)
		return nullptr;

	void* mem = PlatformAlloc(byteSize);

	VerifyNotNull(mem, "Nullptr detected after asking to OS for %lld bytes.", byteSize);
	return mem;
#else
	return PlatformAlloc(byteSize);
#endif
}

template<class T>
void* greaper::MemoryAllocator<T>::AllocateAligned(sizet byteSize, sizet alignment)
{
#if GREAPER_DEBUG_ALLOCATION
	if (byteSize == 0)
		return nullptr;
	if (alignment == 0)
		return Allocate(byteSize);

	void* mem = PlatformAlignedAlloc(byteSize, alignment);
	VerifyNotNull(mem, "Nullptr detected after asking to OS for %lld bytes aligned %lld.", byteSize, alignment);

	return mem;
#else
	return PlatformAlignedAlloc(byteSize, alignment);
#endif
}

template<class T>
void greaper::MemoryAllocator<T>::Deallocate(void* mem)
{
#if GREAPER_DEBUG_ALLOCATION
#if GREAPER_ENABLE_BREAK
	VerifyNotNull(mem, "Detected nullptr, maybe use after free.");
#else
	if (mem == nullptr)
	{
		return;
	}
#endif
#endif
	PlatformDealloc(mem);
}

template<class T>
void greaper::MemoryAllocator<T>::DeallocateAligned(void* mem)
{
#if GREAPER_DEBUG_ALLOCATION
#if GREAPER_ENABLE_BREAK
	VerifyNotNull(mem, "Detected nullptr, maybe use after free.");
#else
	if (mem == nullptr)
	{
		return;
	}
#endif
#endif
	PlatformAlignedDealloc(mem);
}

namespace std
{
	template<>
	struct hash<greaper::String>
	{
		INLINE size_t operator()(const greaper::String& str)const noexcept
		{
#if COMPILER_MSVC
			return std::_Hash_array_representation(str.data(), str.size());
#else
			return std::_Hash_bytes(str.data(), str.size(), 0);
#endif
		}
	};

	template<>
	struct hash<greaper::WString>
	{
		INLINE size_t operator()(const greaper::WString& str)const noexcept
		{
#if COMPILER_MSVC
			return std::_Hash_array_representation(str.data(), str.size());
#else
			return std::_Hash_bytes(str.data(), str.size(), 0);
#endif
		}
	};

	template<class T>
	struct hash<greaper::UPtr<T>>
	{
		INLINE size_t operator()(const greaper::UPtr<T>& ptr)const noexcept
		{
			return std::hash<T>()(ptr.Get());
		}
	};

	template<class T>
	struct hash<greaper::SharedPointer<T>>
	{
		INLINE size_t operator()(const greaper::SharedPointer<T>& ptr)const noexcept
		{
			return std::hash<T>()(ptr.Get());
		}
	};

	template<class T>
	struct hash<greaper::WeakPointer<T>>
	{
		INLINE size_t operator()(const greaper::WeakPointer<T>& ptr)const noexcept
		{
			if (!ptr.Expired())
				return std::hash<greaper::SharedPointer<T>>()(ptr.Lock());
			return std::hash<ptruint>()(0);
		}
	};
}

/***********************************************************************************
*                            CONTAINER HELPER FUNCITONS                            *
***********************************************************************************/
namespace greaper
{
	template<class T, class _Alloc_>
	INLINE ssizet IndexOf(const Vector<T, _Alloc_>& container, const T& element) noexcept
	{
		for (auto it = container.cbegin(); it != container.cend(); ++it)
		{
			if ((*it) == element)
				return std::distance(container.begin(), it);
		}
		/*for (std::size_t i = 0; i < container.size(); ++i)
		{
			if (container[i] == element)
				return (ssizet)i;
		}*/
		return -1;
	}
	template<class T, class _Alloc_>
	INLINE ssizet IndexOf(const Deque<T, _Alloc_>& container, const T& element) noexcept
	{
		for (auto it = container.cbegin(); it != container.cend(); ++it)
		{
			if ((*it) == element)
				return std::distance(container.begin(), it);
		}
		return -1;
	}
	template<class T, std::size_t N>
	INLINE ssizet IndexOf(T(&arr)[N], const T& element) noexcept
	{
		for (std::size_t i = 0; i < N; ++i)
		{
			if (arr[i] == element)
				return (ssizet)i;
		}
		return -1;
	}
	template<class T, class _Alloc_>
	INLINE bool Contains(const Vector<T, _Alloc_>& container, const T& elem) noexcept
	{
		return IndexOf<T, _Alloc_>(container, elem) >= 0;
	}
	template<class T, class _Alloc_>
	INLINE bool Contains(const Deque<T, _Alloc_>& container, const T& elem) noexcept
	{
		return IndexOf<T, _Alloc_>(container, elem) >= 0;
	}
	template<class T, std::size_t N>
	INLINE bool Contains(T(&arr)[N], const T& elem) noexcept
	{
		return IndexOf<T, N>(arr, elem);
	}
}
#endif /* GREAPER_CORE_MEMORY_H */