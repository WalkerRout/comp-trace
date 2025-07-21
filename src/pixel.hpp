#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <cassert>
#include <cstdint>

template <typename T>
concept pixel_compatible = std::unsigned_integral<T> && sizeof(T) == 1;

template <pixel_compatible T> struct pixel {
  static_assert(std::is_trivially_copyable_v<T>, "pixel requires trivially copyable types");
  static_assert(std::is_trivially_default_constructible_v<T>,
                "pixel requires trivially default‐constructible types");

  using value_type = T;

  [[nodiscard]] constexpr pixel() noexcept = default;
  [[nodiscard]] constexpr pixel(const value_type r, const value_type g, const value_type b) noexcept
      : m_colour{r, g, b} {}
  [[nodiscard]] constexpr explicit pixel(const colour<value_type> v) noexcept : m_colour{v} {}

  [[nodiscard]] constexpr auto r() const noexcept -> value_type {
    return m_colour.r();
  }
  [[nodiscard]] constexpr auto g() const noexcept -> value_type {
    return m_colour.g();
  }
  [[nodiscard]] constexpr auto b() const noexcept -> value_type {
    return m_colour.b();
  }

  colour<value_type> m_colour;
};

using pixel_u8 = pixel<std::uint8_t>;

#endif // PIXEL_HPP