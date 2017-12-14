//
// Copyright © 2017 Constanze Pfennig. All rights reserved.
//

#pragma once

#include <type_traits>
#include <cassert>
#include <vector>
#include <numeric>
#include <array>


namespace nuts
{
  namespace math
  {
    template<typename T, std::size_t Dimension>
    class vector
    {
    public:
      static_assert(Dimension >= 2, "Dimension of vector must be 2 or higher.");
      static_assert(std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, "Invalid type used for scalar vector type.");

      using value_type = T;
      using pointer = T * ;
      using const_pointer = const T*;
      using reference = T & ;
      using const_reference = const T&;

      using array_type = std::array<T, Dimension>;
      using size_type = typename array_type::size_type;

      using iterator = typename array_type::iterator;
      using const_iterator = typename array_type::const_iterator;
      using reverse_iterator = typename array_type::reverse_iterator;
      using const_reverse_iterator = typename array_type::const_reverse_iterator;

      vector() = default;
      vector(const vector&) = default;
      vector(vector&&) noexcept = default;

      template<typename T2, std::size_t Dimension2, typename = std::enable_if_t<Dimension2 <= Dimension && std::is_convertible<T2, T>::value>>
      explicit vector(const vector<T2, Dimension2>& other)
      {
        *this = other;
      }

      template<typename... Args, typename = std::enable_if_t<sizeof...(Args) == Dimension && std::conjunction_v<std::is_convertible<T, Args>...>>>
      vector(Args&&... args)
        : data_{static_cast<T>(std::forward<Args>(args))...}
      {
      }

      vector& operator=(const vector&) = default;
      vector& operator=(vector&&) noexcept = default;

      template<typename T2, std::size_t Dimension2, typename = std::enable_if_t<(Dimension2 <= Dimension)>>
      vector& operator=(const vector<T2, Dimension2>& other)
      {
        if(this == reinterpret_cast<const vector*>(&other))
          return *this;

        auto it = std::transform(other.begin(), other.end(), data_.begin(), [](const T2& val)
        {
          return static_cast<T>(val);
        });

        std::fill(it, data_.end(), static_cast<T>(0));
        return *this;
      }

      bool operator<(const vector& other) const
      {
        return data_ < other.data_;
      }

      bool operator==(const vector& other) const
      {
        return data_ == other.data_;
      }

      bool operator!=(const vector& other) const
      {
        return data_ != other.data_;
      }

      bool operator>(const vector& other) const
      {
        return data_ > other.data_;
      }

      bool operator>=(const vector& other) const
      {
        return data_ >= other.data_;
      }

      bool operator<=(const vector& other) const
      {
        return data_ <= other.data_;
      }

      template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
      auto operator+(const vector<T2, Dimension>& other) const
      {
        using result_value_type = decltype(std::declval<T>() + std::declval<T2>());
        using usage_type = vector<result_value_type, Dimension>;

        usage_type result;

        std::transform(data_.begin(), data_.end(), other.begin(), result.begin(), [](const T& val1, const T2& val2)
        {
          return val1 + val2;
        });

        return result;
      }

      template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
      auto operator*(const T2& val) const
      {
        using result_value_type = decltype(std::declval<T>() * std::declval<T2>());
        using usage_type = vector<result_value_type, Dimension>;

        usage_type result;

        std::transform(data_.begin(), data_.end(), result.begin(), [&val](const T& val1)
        {
          return val1 * val;
        });

        return result;
      }

      vector& operator*=(const_reference val)
      {
        *this = *this * val;
        return *this;
      }

      template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
      auto operator-(const vector<T2, Dimension>& other) const
      {
        using result_value_type = decltype(std::declval<T>() - std::declval<T2>());
        using usage_type = vector<result_value_type, Dimension>;

        usage_type result;

        std::transform(data_.begin(), data_.end(), other.begin(), result.begin(), [](const T& val1, const T2& val2)
        {
          return val1 - val2;
        });

        return result;
      }

      vector& operator-() const
      {
        *this = *this * -1;
        return *this;
      }

      vector& operator-=(const vector& other)
      {
        *this = *this - other;
        return *this;
      }

      vector& operator+=(const vector& other)
      {
        *this = *this + other;
        return *this;
      }

      reference operator[](const size_type& index)
      {
        assert(index < Dimension);
        return data_[index];
      }

      const_reference operator[](const size_type& index) const
      {
        assert(index < Dimension);
        return data_[index];
      }

      iterator begin() noexcept
      {
        return data_.begin();
      }

      const_iterator begin() const noexcept
      {
        return data_.begin();
      }

      iterator end() noexcept
      {
        return data_.end();
      }

      const_iterator end() const noexcept
      {
        return data_.end();
      }

      const_iterator cbegin() const noexcept
      {
        return data_.cbegin();
      }

      const_iterator cend() const noexcept
      {
        return data_.cend();
      }

      reverse_iterator rbegin() noexcept
      {
        return data_.rbegin();
      }

      const_reverse_iterator rbegin() const noexcept
      {
        return data_.rbegin();
      }

      reverse_iterator rend() noexcept
      {
        return data_.rend();
      }

      const_reverse_iterator rend() const noexcept
      {
        return data_.rend();
      }

      const_reverse_iterator crbegin() noexcept
      {
        return data_.crbegin();
      }

      const_reverse_iterator crend() noexcept
      {
        return data_.crend();
      }

      constexpr pointer data() noexcept
      {
        return data_.data();
      }

      constexpr const_pointer data() const noexcept
      {
        return data_.data();
      }

      constexpr size_type size() const noexcept
      {
        return data_.size();
      }

      const_reference x() const
      {
        return data_[0];
      }

      reference x()
      {
        return data_[0];
      }

      const_reference y() const
      {
        return data_[1];
      }

      reference y()
      {
        return data_[1];
      }

      template<std::size_t MyDimension = Dimension, typename = std::enable_if_t<MyDimension >= 3>>
      const_reference z() const
      {
        return data_[2];
      }

      template<std::size_t MyDimension = Dimension, typename = std::enable_if_t<MyDimension >= 3>>
      reference z()
      {
        return data_[2];
      }

      template<std::size_t MyDimension = Dimension, typename = std::enable_if_t<MyDimension >= 4>>
      const_reference w() const
      {
        return data_[3];
      }

      template<std::size_t MyDimension = Dimension, typename = std::enable_if_t<MyDimension >= 4>>
      reference w()
      {
        return data_[3];
      }

      value_type dot(const vector& other) const
      {
        return std::inner_product(begin(), end(), other.begin(), T{0});
      }

      value_type length() const
      {
        return sqrt(dot(*this));
      }

    private:
      std::array<T, Dimension> data_;
    };


    template<typename T, std::size_t Dimension, typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
    auto operator*(const T2& val, const vector<T, Dimension>& vec)
    {
      return vec * val;
    }


    template<typename T, std::size_t Dimension, typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
    auto comp_mult(const vector<T, Dimension>& vec, const vector<T2, Dimension>& other)
    {
      using result_value_type = decltype(std::declval<T>() * std::declval<T2>());
      using usage_type = vector<result_value_type, Dimension>;

      usage_type result;

      std::transform(vec.begin(), vec.end(), other.begin(), result.begin(), [](const T& val1, const T2& val2)
      {
        return val1 * val2;
      });

      return result;
    }


    using vector2d = vector<double, 2>;
    using vector3d = vector<double, 3>;
    using vector2i = vector<int, 2>;
    using vector3i = vector<int, 3>;
    using vector2f = vector<float, 2>;
    using vector3f = vector<float, 3>;


    namespace detail
    {
      template<typename T, std::size_t Dimension>
      class comma_initializer
      {
      public:
        template<typename T2>
        comma_initializer(nuts::math::vector<T, Dimension>& vec, T2&& val)
          : vec_{vec}
        {
          *this, std::forward<T2>(val);
        }

        ~comma_initializer()
        {
          assert(index_ == Dimension);
        }

        template<typename T2, typename = std::enable_if_t<std::is_convertible<T2, T>::value>>
        auto& operator,(T2&& val)
        {
          assert(index_ < Dimension);

          vec_[index_] = static_cast<T>(std::forward<T2>(val));
          ++index_;

          return *this;
        }

        template<typename T2, std::size_t Dimension2, typename = std::enable_if_t<Dimension2 <= Dimension>>
        auto& operator,(const nuts::math::vector<T2, Dimension2>& other)
        {
          for(auto& elem : other)
          {
            *this, elem;
          }

          return *this;
        }

      private:
        nuts::math::vector<T, Dimension>& vec_;
        typename nuts::math::vector<T, Dimension>::size_type index_ = 0;
      };
    }


    template<typename T, std::size_t Dimension, typename T2>
    auto operator<<(vector<T, Dimension>& vec, T2&& val)
    {
      return detail::comma_initializer<T, Dimension>(vec, std::forward<T2>(val));
    }


    template<typename T, std::size_t Dimension, typename T2, std::size_t Dimension2>
    detail::comma_initializer<T, Dimension> operator<<(vector<T, Dimension> vec, vector<T2, Dimension2>&& other)
    {
      return detail::comma_initializer<T, Dimension>(vec, std::forward<vector<T2, Dimension2>>(other));
    }
  }
}