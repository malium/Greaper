/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

namespace greaper
{
	template<class T> struct Span;
	template<class T> struct CSpan;

	namespace Impl
	{
		template<class T>
		class SpanIterator
		{
		public:
			INLINE constexpr SpanIterator()noexcept = default;
			INLINE constexpr SpanIterator(Span<T> span, ssizet index)noexcept
				:m_Span(std::move(span))
				,m_Index(index)
			{

			}
			INLINE constexpr SpanIterator(const SpanIterator& other)noexcept = default;
			INLINE constexpr SpanIterator(SpanIterator&& other)noexcept = default;
			INLINE constexpr SpanIterator& operator=(const SpanIterator& other)noexcept = default;
			INLINE constexpr SpanIterator& operator=(SpanIterator&& other)noexcept = default;
			INLINE ~SpanIterator()noexcept = default;

			INLINE constexpr SpanIterator<T>& operator++()noexcept
			{
				++m_Index;
				return *this;
			}
			INLINE constexpr SpanIterator<T> operator++(int)noexcept
			{
				return SpanIterator<T>{m_Span, m_Index++};
			}
			INLINE constexpr SpanIterator<T>& operator--()noexcept
			{
				--m_Index;
				return *this;
			}
			INLINE constexpr SpanIterator<T> operator--(int)noexcept
			{
				return SpanIterator<T>{m_Span, m_Index--};
			}
			INLINE constexpr T* operator->()const noexcept
			{
				return &m_Span.GetElementFn((std::size_t)m_Index);
			}
			INLINE constexpr T& operator*()const noexcept
			{
				return m_Span.GetElementFn((std::size_t)m_Index);
			}

			template<class T2>
			friend constexpr bool operator==(const SpanIterator<T2>&, const SpanIterator<T2>&)noexcept;

		private:
			Span<T> m_Span;
			ssizet m_Index;
		};

		template<class T>
		INLINE constexpr bool operator==(const SpanIterator<T>& left, const SpanIterator<T>& right)noexcept
		{
			return left.m_Span == right.m_Span && left.m_Index == right.m_Index;
		}
		template<class T>
		INLINE constexpr bool operator!=(const SpanIterator<T>& left, const SpanIterator<T>& right)noexcept
		{
			return !(left == right);
		}

		template<class T>
		class SpanConstIterator
		{
		public:
			INLINE constexpr  SpanConstIterator()noexcept = default;
			INLINE constexpr SpanConstIterator(CSpan<T> span, ssizet index)noexcept
				:m_Span(std::move(span))
				,m_Index(index)
			{

			}
			INLINE constexpr SpanConstIterator(const SpanConstIterator& other)noexcept = default;
			INLINE constexpr SpanConstIterator(SpanConstIterator&& other)noexcept = default;
			INLINE constexpr SpanConstIterator& operator=(const SpanConstIterator& other)noexcept = default;
			INLINE constexpr SpanConstIterator& operator=(SpanConstIterator&& other)noexcept = default;
			INLINE ~SpanConstIterator()noexcept = default;

			INLINE constexpr SpanConstIterator<T>& operator++()noexcept
			{
				++m_Index;
				return *this;
			}
			INLINE constexpr SpanConstIterator<T> operator++(int)noexcept
			{
				return SpanConstIterator<T>{m_Span, m_Index++};
			}
			INLINE constexpr SpanConstIterator<T>& operator--()noexcept
			{
				--m_Index;
				return *this;
			}
			INLINE constexpr SpanConstIterator<T> operator--(int)noexcept
			{
				return SpanConstIterator<T>{m_Span, m_Index--};
			}
			INLINE constexpr const T* operator->()const noexcept
			{
				return &m_Span.GetElementFn((std::size_t)m_Index);
			}
			INLINE constexpr const T& operator*()const noexcept
			{
				return m_Span.GetElementFn((std::size_t)m_Index);
			}

			template<class T2>
			friend constexpr bool operator==(const SpanConstIterator<T2>&, const SpanConstIterator<T2>&)noexcept;
		private:
			CSpan<T> m_Span;
			ssizet m_Index;
		};

		template<class T>
		INLINE constexpr bool operator==(const SpanConstIterator<T>& left, const SpanConstIterator<T>& right)noexcept
		{
			return left.m_Span == right.m_Span && left.m_Index == right.m_Index;
		}
		template<class T>
		INLINE constexpr bool operator!=(const SpanConstIterator<T>& left, const SpanConstIterator<T>& right)noexcept
		{
			return !(left == right);
		}

		template<class T>
		class SpanReverseIterator
		{
		public:
			INLINE constexpr  SpanReverseIterator()noexcept = default;
			INLINE constexpr SpanReverseIterator(Span<T> span, ssizet index)noexcept
				:m_Span(std::move(span))
				, m_Index(index)
			{

			}
			INLINE constexpr SpanReverseIterator(const SpanReverseIterator& other)noexcept = default;
			INLINE constexpr SpanReverseIterator(SpanReverseIterator&& other)noexcept = default;
			INLINE constexpr SpanReverseIterator& operator=(const SpanReverseIterator& other)noexcept = default;
			INLINE constexpr SpanReverseIterator& operator=(SpanReverseIterator&& other)noexcept = default;
			INLINE ~SpanReverseIterator()noexcept = default;

			INLINE constexpr SpanReverseIterator<T>& operator++()noexcept
			{
				--m_Index;
				return *this;
			}
			INLINE constexpr SpanReverseIterator<T> operator++(int)noexcept
			{
				return SpanReverseIterator<T>{m_Span, m_Index--};
			}
			INLINE constexpr SpanReverseIterator<T>& operator--()noexcept
			{
				++m_Index;
				return *this;
			}
			INLINE constexpr SpanReverseIterator<T> operator--(int)noexcept
			{
				return SpanReverseIterator<T>{m_Span, m_Index++};
			}
			INLINE constexpr T* operator->()const noexcept
			{
				return &m_Span.GetElementFn((std::size_t)m_Index);
			}
			INLINE constexpr T& operator*()const noexcept
			{
				return m_Span.GetElementFn((std::size_t)m_Index);
			}

			template<class T2>
			friend constexpr bool operator==(const SpanReverseIterator<T2>&, const SpanReverseIterator<T2>&)noexcept;
		private:
			Span<T> m_Span;
			ssizet m_Index;
		};

		template<class T>
		INLINE constexpr bool operator==(const SpanReverseIterator<T>& left, const SpanReverseIterator<T>& right)noexcept
		{
			return left.m_Span == right.m_Span && left.m_Index == right.m_Index;
		}
		template<class T>
		INLINE constexpr bool operator!=(const SpanReverseIterator<T>& left, const SpanReverseIterator<T>& right)noexcept
		{
			return !(left == right);
		}

		template<class T>
		class SpanConstReverseIterator
		{
		public:
			INLINE constexpr  SpanConstReverseIterator()noexcept = default;
			INLINE constexpr SpanConstReverseIterator(CSpan<T> span, ssizet index)noexcept
				:m_Span(std::move(span))
				, m_Index(index)
			{

			}
			INLINE constexpr SpanConstReverseIterator(const SpanConstReverseIterator& other)noexcept = default;
			INLINE constexpr SpanConstReverseIterator(SpanConstReverseIterator&& other)noexcept = default;
			INLINE constexpr SpanConstReverseIterator& operator=(const SpanConstReverseIterator& other)noexcept = default;
			INLINE constexpr SpanConstReverseIterator& operator=(SpanConstReverseIterator&& other)noexcept = default;
			INLINE ~SpanConstReverseIterator()noexcept = default;

			INLINE constexpr SpanConstReverseIterator<T>& operator++()noexcept
			{
				--m_Index;
				return *this;
			}
			INLINE constexpr SpanConstReverseIterator<T> operator++(int)noexcept
			{
				return SpanConstReverseIterator<T>{m_Span, m_Index--};
			}
			INLINE constexpr SpanConstReverseIterator<T>& operator--()noexcept
			{
				++m_Index;
				return *this;
			}
			INLINE constexpr SpanConstReverseIterator<T> operator--(int)noexcept
			{
				return SpanConstReverseIterator<T>{m_Span, m_Index++};
			}
			INLINE constexpr const T* operator->()const noexcept
			{
				return &m_Span.GetElementFn((std::size_t)m_Index);
			}
			INLINE constexpr const T& operator*()const noexcept
			{
				return m_Span.GetElementFn((std::size_t)m_Index);
			}

			template<class T2>
			friend constexpr bool operator==(const SpanConstReverseIterator<T2>&, const SpanConstReverseIterator<T2>&)noexcept;
		private:
			CSpan<T> m_Span;
			ssizet m_Index;
		};

		template<class T>
		INLINE constexpr bool operator==(const SpanConstReverseIterator<T>& left, const SpanConstReverseIterator<T>& right)noexcept
		{
			return left.m_Span == right.m_Span && left.m_Index == right.m_Index;
		}
		template<class T>
		INLINE constexpr bool operator!=(const SpanConstReverseIterator<T>& left, const SpanConstReverseIterator<T>& right)noexcept
		{
			return !(left == right);
		}
	}

	template<class T>
	struct Span
	{
		std::function<std::size_t()> GetSizeFn = nullptr;
		std::function<T& (std::size_t)> GetElementFn = nullptr;

		INLINE constexpr Span()noexcept = default;
		INLINE constexpr Span(std::function<std::size_t()> getSizeFn, std::function<T& (std::size_t)> getElementFn)noexcept
			:GetSizeFn(std::move(getSizeFn))
			,GetElementFn(std::move(getElementFn))
		{

		}
		INLINE constexpr Span(const Span& other)noexcept
			:GetSizeFn(other.GetSizeFn)
			,GetElementFn(other.GetElementFn)
		{

		}
		INLINE constexpr Span(Span&& other)noexcept
			:GetSizeFn(std::move(other.GetSizeFn))
			,GetElementFn(std::move(other.GetElementFn))
		{

		}
		INLINE constexpr Span& operator=(const Span& other)noexcept
		{
			if (this != &other)
			{
				GetSizeFn = other.GetSizeFn;
				GetElementFn = other.GetElementFn;
			}
			return *this;
		}
		INLINE constexpr Span& operator=(Span&& other)noexcept
		{
			if (this != &other)
			{
				GetSizeFn = other.GetSizeFn;
				other.GetSizeFn = nullptr;
				GetElementFn = other.GetElementFn;
				other.GetElementFn = nullptr;
			}
			return *this;
		}
		INLINE ~Span()noexcept = default;

		INLINE constexpr Impl::SpanIterator<T> begin()const noexcept { return Impl::SpanIterator<T>(*this, 0); }
		INLINE constexpr Impl::SpanIterator<T> end()const noexcept { return Impl::SpanIterator<T>(*this, GetSizeFn()); }
		INLINE constexpr Impl::SpanReverseIterator<T> rbegin()const noexcept { return Impl::SpanReverseIterator<T>(*this, GetSizeFn() - 1); }
		INLINE constexpr Impl::SpanReverseIterator<T> rend()const noexcept { return Impl::SpanReverseIterator<T>(*this, -1); }
	};

	template<class T>
	INLINE constexpr bool operator==(const Span<T>& left, const Span<T>& right)noexcept
	{
		return left.GetSizeFn.target_type().hash_code() == right.GetSizeFn.target_type().hash_code() &&
			left.GetElementFn.target_type().hash_code() == right.GetElementFn.target_type().hash_code();
	}

	template<class T>
	INLINE constexpr bool operator!=(const Span<T>& left, const Span<T>& right)noexcept
	{
		return !(left == right);
	}

	template<class T>
	struct CSpan
	{
		std::function<std::size_t()> GetSizeFn;
		std::function<const T& (std::size_t)> GetElementFn;

		INLINE constexpr CSpan()noexcept = default;
		INLINE constexpr CSpan(std::function<std::size_t()> getSizeFn, std::function<const T& (std::size_t)> getElementFn)noexcept
			:GetSizeFn(std::move(getSizeFn))
			, GetElementFn(std::move(getElementFn))
		{

		}
		INLINE constexpr CSpan(const CSpan& other)noexcept
			:GetSizeFn(other.GetSizeFn)
			, GetElementFn(other.GetElementFn)
		{

		}
		INLINE constexpr CSpan(CSpan&& other)noexcept
			:GetSizeFn(std::move(other.GetSizeFn))
			, GetElementFn(std::move(other.GetElementFn))
		{

		}
		INLINE constexpr CSpan& operator=(const CSpan& other)noexcept
		{
			if (this != &other)
			{
				GetSizeFn = other.GetSizeFn;
				GetElementFn = other.GetElementFn;
			}
			return *this;
		}
		INLINE constexpr CSpan& operator=(CSpan&& other)noexcept
		{
			if (this != &other)
			{
				GetSizeFn = other.GetSizeFn;
				other.GetSizeFn = nullptr;
				GetElementFn = other.GetElementFn;
				other.GetElementFn = nullptr;
			}
			return *this;
		}
		INLINE ~CSpan()noexcept = default;

		INLINE constexpr Impl::SpanConstIterator<T> begin()const noexcept { return Impl::SpanConstIterator<T>(*this, 0); }
		INLINE constexpr Impl::SpanConstIterator<T> end()const noexcept { return Impl::SpanConstIterator<T>(*this, GetSizeFn()); }
		INLINE constexpr Impl::SpanConstReverseIterator<T> rbegin()const noexcept { return Impl::SpanConstReverseIterator<T>(*this, GetSizeFn() - 1); }
		INLINE constexpr Impl::SpanConstReverseIterator<T> rend()const noexcept { return Impl::SpanConstReverseIterator<T>(*this, -1); }
	};

	template<class T>
	INLINE constexpr bool operator==(const CSpan<T>& left, const CSpan<T>& right)noexcept
	{
		return left.GetSizeFn.target_type().hash_code() == right.GetSizeFn.target_type().hash_code() &&
			left.GetElementFn.target_type().hash_code() == right.GetElementFn.target_type().hash_code();
	}

	template<class T>
	INLINE constexpr bool operator!=(const CSpan<T>& left, const CSpan<T>& right)noexcept
	{
		return !(left == right);
	}

	template<class T, class _Alloc_ = GenericAllocator>
	INLINE constexpr Span<T> CreateSpan(Vector<T, _Alloc_>& vec)noexcept
	{
		return Span<T>([vec]() {return vec.size(); }, [vec](std::size_t idx) -> T& {return vec.at(idx); });
	}

	template<class T, class _Alloc_ = GenericAllocator>
	INLINE constexpr CSpan<T> CreateSpan(const Vector<T, _Alloc_>& vec)noexcept
	{
		return CSpan<T>([vec]() {return vec.size(); }, [vec](std::size_t idx) -> const T& {return vec.at(idx); });
	}
}