#ifndef UTIL_HPP
#define UTIL_HPP

#include <cmath>
#include <limits>
#include <numbers>
#include <type_traits>

template <typename T>
concept sqrt_compatible = std::floating_point<T> && requires(T val) {
  { val * val } -> std::convertible_to<T>;
  { val / val } -> std::convertible_to<T>;
};

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
  constexpr int max_iterations = std::is_same_v<T, float> ? 6 : 10;
  constexpr T epsilon = std::numeric_limits<T>::epsilon() * T{100};

  for (int i = 0; i < max_iterations; ++i) {
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

template <typename T>
concept arithmetic_type = std::is_arithmetic_v<T> && !std::same_as<std::remove_cvref_t<T>, bool>;

template <std::floating_point T> inline constexpr T infinity_v = std::numeric_limits<T>::infinity();

template <std::floating_point T> inline constexpr T epsilon_v = std::numeric_limits<T>::epsilon();

#endif // UTIL_HPP