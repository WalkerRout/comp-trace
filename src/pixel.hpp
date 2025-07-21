#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <cassert>
#include <cstdint>

#include "colour.hpp"

template <typename T>
concept pixel_value_type_compatible = requires {
  requires colour_value_type_compatible<T>;
  requires std::is_trivially_copyable_v<T>;
  requires std::is_trivially_default_constructible_v<T>;
};

template <pixel_value_type_compatible T> class pixel {
public:
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

private:
  colour<value_type> m_colour;
};

using pixel_u8 = pixel<std::uint8_t>;

#endif // PIXEL_HPP