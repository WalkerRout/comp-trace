#ifndef SCENE_HPP
#define SCENE_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <span>

#include "ray.hpp"
#include "sphere.hpp"

template <typename T>
concept scene_compatible = requires(T obj, ray<double> r, double t_min, double t_max) {
  { obj.hit(r, t_min, t_max) } -> std::same_as<std::optional<hit_record<double>>>;
};

template <scene_compatible T, std::size_t N> class scene {
public:
  using value_type = T;
  using size_type = std::size_t;
  using float_type = typename T::value_type;

  [[nodiscard]] constexpr scene() noexcept = default;

  constexpr void add(const T& object) noexcept {
    assert(m_count < N && "scene capacity exceeded");
    m_objects[m_count] = object;
    m_count += 1;
  }

  [[nodiscard]] constexpr auto hit(const ray<float_type>& r, const float_type t_min,
                                   const float_type t_max) const noexcept
      -> std::optional<hit_record<float_type>> {
    std::optional<hit_record<float_type>> closest_hit;
    float_type closest_so_far = t_max;

    for (size_type i = 0; i < m_count; ++i) {
      if (auto hit_opt = m_objects[i].hit(r, t_min, closest_so_far)) {
        closest_hit = hit_opt;
        closest_so_far = hit_opt->t;
      }
    }

    return closest_hit;
  }

  [[nodiscard]] constexpr auto size() const noexcept -> size_type {
    return m_count;
  }

private:
  std::array<T, N> m_objects{};
  std::size_t m_count{0};
};

#endif // SCENE_HPP