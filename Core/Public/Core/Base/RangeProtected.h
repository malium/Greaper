/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

namespace greaper
{
	template<class T, class M = Mutex>
	struct RangeProtected : public Range<T>
	{
		M& Protection;

		INLINE RangeProtected(std::function<std::size_t()> sizeFn, std::function<T& (std::size_t idx)> getItemFn, M& protection) noexcept
			:Range<T>(std::move(sizeFn), std::move(getItemFn))
			,Protection(protection)
		{

		}
	};

	template<class T, class M = Mutex>
	struct CRangeProtected : public CRange<T>
	{
		M& Protection;

		INLINE CRangeProtected(std::function<std::size_t()> sizeFn, std::function<const T& (std::size_t idx)> getItemFn, M& protection) noexcept
			:CRange<T>(std::move(sizeFn), std::move(getItemFn))
			,Protection(protection)
		{

		}
	};

	template<typename T, class M, typename A = StdAlloc<T>>
	RangeProtected<T, M> CreateRange(Vector<T, A>& vec, M& mutex)
	{
		return RangeProtected<T, M>(
			std::bind(&Vector<T, A>::size, &vec),
			std::bind([](Vector<T, A>& v, std::size_t i) -> T& { return v[i]; }, vec, std::placeholders::_1),
			mutex);
	}

	template<typename T, class M, typename A = StdAlloc<T>>
	CRangeProtected<T, M> CreateRange(const Vector<T, A>& vec, M& mutex)
	{
		return CRangeProtected<T, M>(
			std::bind(&Vector<T, A>::size, &vec),
			std::bind([](const Vector<T, A>& v, std::size_t i) -> const T& { return v[i]; }, vec, std::placeholders::_1),
			mutex);
	}
}