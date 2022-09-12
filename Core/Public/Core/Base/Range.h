/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

namespace greaper
{
	template<typename T> struct Range;
	template<typename T> struct CRange;

	template<typename T>
	class RangeIterator
	{
		Range<T>& m_Range;
		std::size_t m_Index;
	public:
		RangeIterator(Range<T>& range, std::size_t index)noexcept;

		T* operator->()const noexcept;
		T& operator*()const noexcept;
		RangeIterator<T>& operator++()noexcept;
		RangeIterator<T> operator++(int)noexcept;
		RangeIterator<T>& operator--()noexcept;
		RangeIterator<T> operator--(int)noexcept;
		bool operator==(const RangeIterator& other)const noexcept;
		bool operator!=(const RangeIterator& other)const noexcept;
	};
	template<typename T>
	class ConstRangeIterator
	{
		CRange<T>& m_Range;
		std::size_t m_Index;
	public:
		ConstRangeIterator(CRange<T>& range, std::size_t index)noexcept;

		const T* operator->()const noexcept;
		const T& operator*()const noexcept;
		ConstRangeIterator<T>& operator++()noexcept;
		ConstRangeIterator<T> operator++(int)noexcept;
		ConstRangeIterator<T>& operator--()noexcept;
		ConstRangeIterator<T> operator--(int)noexcept;
		bool operator==(const ConstRangeIterator& other)const noexcept;
		bool operator!=(const ConstRangeIterator& other)const noexcept;
	};
	template<typename T>
	struct Range
	{
		std::function<std::size_t()> SizeFn = nullptr;
		std::function<T& (std::size_t idx)> GetItemFn = nullptr;

		INLINE Range(std::function<std::size_t()> sizeFn, std::function<T& (std::size_t idx)> getItemFn)noexcept
			:SizeFn(std::move(sizeFn))
			,GetItemFn(std::move(getItemFn))
		{

		}

		INLINE operator bool()const noexcept
		{
			return SizeFn != nullptr && GetItemFn != nullptr;
		}

		INLINE RangeIterator<T> begin()noexcept
		{
			return RangeIterator<T>(*this, 0);
		}
		INLINE RangeIterator<T> end()noexcept
		{
			return RangeIterator<T>(*this, SizeFn());
		}
		INLINE RangeIterator<T> rbegin()noexcept
		{
			return RangeIterator<T>(*this, SizeFn() - 1);
		}
		INLINE RangeIterator<T> rend()noexcept
		{
			return RangeIterator<T>(*this, -1);
		}
	};
	template<typename T>
	struct CRange
	{
		std::function<std::size_t()> SizeFn = nullptr;
		std::function<const T& (std::size_t idx)> GetItemFn = nullptr;

		INLINE CRange(std::function<std::size_t()> sizeFn, std::function<const T& (std::size_t idx)> getItemFn) noexcept
			:SizeFn(std::move(sizeFn))
			,GetItemFn(std::move(getItemFn))
		{

		}

		INLINE operator bool()const noexcept
		{
			return SizeFn != nullptr && GetItemFn != nullptr;
		}

		INLINE ConstRangeIterator<T> begin()noexcept
		{
			return ConstRangeIterator<T>(*this, 0);
		}
		INLINE ConstRangeIterator<T> end()noexcept
		{
			return ConstRangeIterator<T>(*this, SizeFn());
		}
		INLINE ConstRangeIterator<T> rbegin()noexcept
		{
			return ConstRangeIterator<T>(*this, SizeFn() - 1);
		}
		INLINE ConstRangeIterator<T> rend()noexcept
		{
			return ConstRangeIterator<T>(*this, -1);
		}
	};

	template<typename T, typename A = StdAlloc<T>>
	Range<T> CreateRange(Vector<T, A>& vec)
	{
		Range<T> r;
		r.SizeFn = std::bind(&Vector<T, A>::size, &vec);
		r.GetItemFn = std::bind([](Vector<T, A>& v, std::size_t i) -> T& { return v[i]; }, vec, std::placeholders::_1);
		return r;
	}

	template<typename T, typename A = StdAlloc<T>>
	CRange<T> CreateRange(const Vector<T, A>& vec)
	{
		return CRange<T>(std::bind(&Vector<T, A>::size, &vec), std::bind([](const Vector<T, A>& v, std::size_t i) -> const T& { return v[i]; }, vec, std::placeholders::_1));
	}

	template<typename T>
	INLINE RangeIterator<T>::RangeIterator(Range<T>& range, std::size_t index) noexcept
		:m_Range(range)
		, m_Index(index)
	{

	}

	template<typename T>
	INLINE T* RangeIterator<T>::operator->()const noexcept
	{
		auto size = m_Range.SizeFn();
		VerifyWithinInclusive(m_Index, 0, size, "Trying to dereference a RangeIterator with an invalid index.");
		return &m_Range.GetItemFn(m_Index);
	}

	template<typename T>
	INLINE T& RangeIterator<T>::operator*()const noexcept
	{
		auto size = m_Range.SizeFn();
		VerifyWithinInclusive(m_Index, 0, size, "Trying to dereference a RangeIterator with an invalid index.");
		return m_Range.GetItemFn(m_Index);
	}

	template<typename T>
	INLINE RangeIterator<T>& RangeIterator<T>::operator++() noexcept
	{
		++m_Index;
		return *this;
	}

	template<typename T>
	INLINE RangeIterator<T> RangeIterator<T>::operator++(int) noexcept
	{
		RangeIterator<T> old = *this;
		++(*this);
		return old;
	}

	template<typename T>
	INLINE RangeIterator<T>& RangeIterator<T>::operator--() noexcept
	{
		--m_Index;
		return *this;
	}

	template<typename T>
	INLINE RangeIterator<T> RangeIterator<T>::operator--(int) noexcept
	{
		RangeIterator<T> old = *this;
		--(*this);
		return old;
	}

	template<typename T>
	INLINE bool RangeIterator<T>::operator==(const RangeIterator& other) const noexcept
	{
		return &m_Range == &other.m_Range && m_Index == other.m_Index;
	}

	template<typename T>
	INLINE bool RangeIterator<T>::operator!=(const RangeIterator& other) const noexcept
	{
		return !(*this == other);
	}

	template<typename T>
	INLINE ConstRangeIterator<T>::ConstRangeIterator(CRange<T>& range, std::size_t index) noexcept
		:m_Range(range)
		, m_Index(index)
	{

	}

	template<typename T>
	INLINE const T* ConstRangeIterator<T>::operator->()const noexcept
	{
		auto size = m_Range.SizeFn();
		VerifyWithinInclusive(m_Index, 0, size, "Trying to dereference a RangeIterator with an invalid index.");
		return &m_Range.GetItemFn(m_Index);
	}

	template<typename T>
	INLINE const T& ConstRangeIterator<T>::operator*()const noexcept
	{
		auto size = m_Range.SizeFn();
		VerifyWithinInclusive(m_Index, 0, size, "Trying to dereference a RangeIterator with an invalid index.");
		return m_Range.GetItemFn(m_Index);
	}

	template<typename T>
	INLINE ConstRangeIterator<T>& ConstRangeIterator<T>::operator++() noexcept
	{
		++m_Index;
		return *this;
	}

	template<typename T>
	INLINE ConstRangeIterator<T> ConstRangeIterator<T>::operator++(int) noexcept
	{
		ConstRangeIterator<T> old = *this;
		++(*this);
		return old;
	}

	template<typename T>
	INLINE ConstRangeIterator<T>& ConstRangeIterator<T>::operator--() noexcept
	{
		--m_Index;
		return *this;
	}

	template<typename T>
	INLINE ConstRangeIterator<T> ConstRangeIterator<T>::operator--(int) noexcept
	{
		ConstRangeIterator<T> old = *this;
		--(*this);
		return old;
	}

	template<typename T>
	INLINE bool ConstRangeIterator<T>::operator==(const ConstRangeIterator& other) const noexcept
	{
		return &m_Range == &other.m_Range && m_Index == other.m_Index;
	}

	template<typename T>
	INLINE bool ConstRangeIterator<T>::operator!=(const ConstRangeIterator& other) const noexcept
	{
		return !(*this == other);
	}

}