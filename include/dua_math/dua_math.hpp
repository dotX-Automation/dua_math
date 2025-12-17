/**
 * DUA Math Library
 *
 * dotX Automation <info@dotxautomation.com>
 *
 * December 3, 2025
 */

/**
 * Copyright 2025 dotX Automation s.r.l.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cmath>
#include <type_traits>
#include <Eigen/Geometry>
#include <geometry_msgs/msg/quaternion.hpp>
#include <tf2/LinearMath/Quaternion.h>

namespace dua_math
{

// ---------- Angle-related functions ----------

template<typename TypeT>
[[nodiscard]] inline constexpr TypeT deg_to_rad(TypeT deg) noexcept
{
  return deg * static_cast<TypeT>(M_PI / 180.0);
}

template<typename TypeT>
[[nodiscard]] inline constexpr TypeT rad_to_deg(TypeT rad) noexcept
{
  return rad * static_cast<TypeT>(180.0 / M_PI);
}

template<typename TypeT>
[[nodiscard]] inline TypeT normalize_angle(TypeT angle) noexcept
{
  return std::atan2(std::sin(angle), std::cos(angle));
}

// ---------- Numeric functions ----------

template<typename TypeT, typename = std::enable_if_t<std::is_integral<TypeT>::value>>
[[nodiscard]] constexpr TypeT apply_modulo(TypeT value, TypeT modulus) noexcept
{
  TypeT r = value % modulus;
  return (r < 0) ? r + modulus : r;
}

template<typename TypeT>
[[nodiscard]] constexpr TypeT apply_deadzone(TypeT value, TypeT threshold) noexcept
{
  return (std::abs(value) < threshold) ? static_cast<TypeT>(0) : value;
}

template<typename TypeT>
[[nodiscard]] constexpr TypeT apply_low_pass(TypeT current, TypeT previous, TypeT alpha) noexcept
{
  return alpha * current + (static_cast<TypeT>(1) - alpha) * previous;
}

// ---------- Quaternion functions ----------

[[nodiscard]] inline bool is_normalized_impl(
  double x, double y, double z, double w,
  double eps) noexcept
{
  const double norm_sq = x * x + y * y + z * z + w * w;
  return std::abs(norm_sq - 1.0) < (2.0 * eps);
}

template<typename TypeT>
[[nodiscard]] inline bool is_normalized(
  const Eigen::Quaternion<TypeT> & q,
  double eps = 1e-6) noexcept
{
  return is_normalized_impl(
    static_cast<double>(q.x()),
    static_cast<double>(q.y()),
    static_cast<double>(q.z()),
    static_cast<double>(q.w()),
    eps
  );
}

[[nodiscard]] inline bool is_normalized(
  const tf2::Quaternion & q,
  double eps = 1e-6) noexcept
{
  return is_normalized_impl(q.x(), q.y(), q.z(), q.w(), eps);
}

[[nodiscard]] inline bool is_normalized(
  const geometry_msgs::msg::Quaternion & q,
  double eps = 1e-6) noexcept
{
  return is_normalized_impl(q.x, q.y, q.z, q.w, eps);
}

} // namespace dua_math
