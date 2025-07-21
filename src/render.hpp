#ifndef RENDER_HPP
#define RENDER_HPP

#include <array>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <print>
#include <ranges>

#include "camera.hpp"
#include "colour.hpp"
#include "image.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "util.hpp"

struct render_params {
  std::size_t width;
  std::size_t height;
};

template <std::size_t N> using sphere_scene = scene<sphere_d, N>;

[[nodiscard]] constexpr auto build_scene() noexcept -> sphere_scene<2> {
  sphere_scene<2> world{};
  world.add({{0.0, 0.0, -1.0}, 0.5});
  world.add({{0.0, -100.5, -1.0}, 100});
  return world;
}

// compute pixel with proper colour space handling...
[[nodiscard]] constexpr auto colour_to_pixel(const colour_d& c) noexcept -> pixel_u8 {
  const auto conv = [](double v) {
    const auto x = static_cast<int32_t>(v * 255.999);
    if (x < 0) {
      return static_cast<uint8_t>(0);
    }
    if (x > 255) {
      return static_cast<uint8_t>(255);
    }
    return static_cast<uint8_t>(x);
  };
  return {conv(c.r()), conv(c.g()), conv(c.b())};
}

// colour a ray given some world...
template <scene_compatible S>
[[nodiscard]] constexpr auto ray_colour(const ray_d& r, const S& world) noexcept -> colour_d {
  const auto hit =
      world.hit(r, std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::infinity());
  if (hit) {
    const auto n = hit->normal;
    return {0.5 * (n.x() + 1.0), 0.5 * (n.y() + 1.0), 0.5 * (n.z() + 1.0)};
  }
  // background
  const auto dir = unit_vector(r.direction());
  const double t = 0.5 * (dir.y() + 1.0);
  return (1.0 - t) * colour_d{1.0, 1.0, 1.0} + t * colour_d{0.5, 0.7, 1.0};
}

// render at comptime
template <std::size_t Width, std::size_t Height>
[[nodiscard]] consteval auto render() noexcept -> image<Width, Height> {
  const auto world = build_scene();

  const camera cam;
  image<Width, Height> img;

  for (const auto i : std::views::iota(std::size_t{0}, Height)) {
    for (const auto j : std::views::iota(std::size_t{0}, Width)) {
      const auto u = static_cast<double>(j) / (Width - 1);
      const auto v = static_cast<double>(i) / (Height - 1);
      const ray_d r = cam.get_ray(u, v);
      const colour_d pixel_color = ray_colour(r, world);
      img.set_pixel(j, Height - i - 1, colour_to_pixel(pixel_color));
    }
  }

  return img;
}

#endif // RENDER_HPP