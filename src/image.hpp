#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <vector>

#include "pixel.hpp"

namespace rt {

template <std::size_t W, std::size_t H> struct image_dimensions {
  static constexpr std::size_t width = W;
  static constexpr std::size_t height = H;
  static constexpr std::size_t size = W * H;
};

template <std::size_t W, std::size_t H>
concept valid_image_dimensions =
    (W > 0) && (H > 0) && (W * H <= std::numeric_limits<std::size_t>::max() / 3);

template <std::size_t Width, std::size_t Height>
// an image requires valid dimensions on construction, s.t. we can never have an image with invalid
// Width or Height values...
  requires(valid_image_dimensions<Width, Height>)
class image {
public:
  using value_type = pixel_u8;
  using size_type = std::size_t;

  [[nodiscard]] constexpr image() noexcept = default;

  constexpr void set_pixel(const size_type x, const size_type y, const value_type p) noexcept {
    m_pixels[y * dims::width + x] = p;
  }

  [[nodiscard]] constexpr auto pixels() const noexcept -> std::span<const value_type> {
    return m_pixels;
  }

private:
  using dims = image_dimensions<Width, Height>;

  std::array<pixel_u8, dims::size> m_pixels{};
};

template <std::size_t W, std::size_t H>
inline void dump_bytes(const image<W, H>& img, std::ostream& out = std::cout) {
  // header comes first...
  out << std::format("P6\n{} {}\n255\n", W, H);
  auto pixels = img.pixels();
  for (std::size_t r = 0; r < H; ++r) {
    for (std::size_t c = 0; c < W; ++c) {
      auto& p = pixels[r * W + c];
      // emit each byte, 6 hex digits total...
      out << std::format("{:02X}{:02X}{:02X}", p.r(), p.g(), p.b());
      if (c + 1 < W) {
        out << ' ';
      }
    }
    out << '\n';
  }
}

template <std::size_t W, std::size_t H>
inline void save_ppm(const image<W, H>& img, const std::string& filename) {
  std::ofstream ofs{filename, std::ios::binary};
  if (!ofs) {
    throw std::runtime_error("failed to open file for writing - " + filename);
  }

  // header
  ofs << "P6\n" << W << " " << H << "\n255\n";

  const auto span = img.pixels();
  std::vector<unsigned char> buffer;
  buffer.reserve(span.size() * 3);
  for (const auto& px : span) {
    buffer.push_back(px.r());
    buffer.push_back(px.g());
    buffer.push_back(px.b());
  }

  ofs.write(reinterpret_cast<const char*>(buffer.data()),
            static_cast<std::streamsize>(buffer.size()));
}

} // namespace rt

#endif // IMAGE_HPP