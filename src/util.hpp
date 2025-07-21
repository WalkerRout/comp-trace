#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <limits>
#include <numbers>
#include <type_traits>

// provide some way to access float_values, can specialize to recurse...
template <typename T> struct float_type_of {
  // for basic objects like sphere
  using type = typename T::value_type;
};

template <typename T> using float_type_of_t = typename float_type_of<T>::type;

template <typename T>
concept sqrt_compatible = std::floating_point<T>;

// constexpr sqrt using newton-raphson
template <sqrt_compatible T> [[nodiscard]] constexpr T sqrt_constexpr(const T val) noexcept {
  if (val < T{0}) {
    return std::numeric_limits<T>::quiet_NaN();
  }
  if (val == T{0} || val == T{1}) {
    return val;
  }

  // initial guess...
  T result = val;
  T last{};

  // adapt iter count based on precision
  const std::size_t max_iterations = std::is_same_v<T, float> ? 6 : 10;
  const T epsilon = std::numeric_limits<T>::epsilon() * T{100};

  for (std::size_t i = 0; i < max_iterations; ++i) {
    last = result;
    result = T{0.5} * (result + val / result);

    // instead of doing multiple passes, could specialize float and double
    // to converge faster using fast inverse sqrt...
    const T diff = result > last ? result - last : last - result;
    if (diff < epsilon * result) {
      break;
    }
  }

  return result;
}

#endif // UTIL_HPP`