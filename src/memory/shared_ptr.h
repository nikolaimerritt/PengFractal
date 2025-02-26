#pragma once

#include "shared_ref.h"

namespace peng
{
	template <typename T>
	class shared_ptr
	{
	public:
		shared_ptr()
			: _ptr()
		{ }

		shared_ptr(std::shared_ptr<T>&& ptr)
			: _ptr(ptr)
		{ }

		shared_ptr(const shared_ref<T>& ref)
			: _ptr(ref.get_impl())
		{ }

		template <typename U>
		requires std::convertible_to<U*, T*>
		shared_ptr(const shared_ptr<U>& other)
			: _ptr(other.get_impl())
		{ }

		template <typename U>
		requires std::convertible_to<U*, T*>
		shared_ptr& operator=(const shared_ref<U>& other)
		{
			_ptr = other.get_impl();
			return *this;
		}

		template <typename U>
		requires std::convertible_to<U*, T*>
		shared_ptr& operator=(const shared_ptr<U>& other)
		{
			_ptr = other.get_impl();
			return *this;
		}

		[[nodiscard]] T* get() const noexcept { return _ptr.get(); }
		[[nodiscard]] T* operator->() const noexcept { return get(); }
		[[nodiscard]] size_t use_count() const noexcept { return _ptr.use_count(); }

		[[nodiscard]] const std::shared_ptr<T>& get_impl() const noexcept
		{ 
			return _ptr;
		}

		[[nodiscard]] shared_ref<T> to_shared_ref() const noexcept
		{
			return shared_ref<T>(_ptr);
		}

		explicit operator bool() const noexcept
		{
			return _ptr != nullptr;
		}

	private:
		std::shared_ptr<T> _ptr;
	};

#pragma region Comparison Operators

	template <typename T, typename U>
	requires std::equality_comparable_with<T*, U*>
	[[nodiscard]] bool operator==(const shared_ptr<T>& a, const shared_ptr<U>& b)
	{
		return a.get() == b.get();
	}

	template <typename T, typename U>
	requires std::equality_comparable_with<T*, U*>
	[[nodiscard]] bool operator==(const shared_ref<T>& a, const shared_ptr<U>& b)
	{
		return a.get() == b.get();
	}

	template <typename T, typename U>
	requires std::equality_comparable_with<T*, U*>
	[[nodiscard]] bool operator==(const shared_ptr<T>& a, const shared_ref<U>& b)
	{
		return a.get() == b.get();
	}

#pragma endregion
}