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
      : r(r), g(g), b(b) {}

  [[nodiscard]] static constexpr auto from(const int r, const int g, const int b) noexcept
      -> pixel {
    assert(!(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) &&
           "color channel values must be in [0, 255]");
    return pixel{static_cast<value_type>(r), static_cast<value_type>(g),
                 static_cast<value_type>(b)};
  }

  template <std::floating_point F>
  [[nodiscard]] static constexpr auto from(const F r, const F g, const F b) noexcept
      -> pixel<value_type> {
    auto clamp_convert = [](const F val) -> value_type {
      if (val <= 0.0)
        return value_type{0};
      if (val >= 1.0)
        return value_type{255};
      return static_cast<value_type>(val * 255.999);
    };

    return pixel{clamp_convert(r), clamp_convert(g), clamp_convert(b)};
  }

  // normalize to floating point...
  [[nodiscard]] constexpr auto to_normalized() const noexcept -> std::array<double, 3> {
    return {static_cast<double>(r) / 255.0, static_cast<double>(g) / 255.0,
            static_cast<double>(b) / 255.0};
  }

  value_type r{};
  value_type g{};
  value_type b{};
};

using pixel_u8 = pixel<std::uint8_t>;

#endif // PIXEL_HPP