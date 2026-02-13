/**
 * DUA math library
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

template<typename TypeT>
inline constexpr TypeT PI = static_cast<TypeT>(3.141592653589793238462643383279502884L);

// ---------- Angle-related functions ----------

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] inline constexpr TypeT deg_to_rad(TypeT deg) noexcept
{
  return deg * (PI<TypeT>/ static_cast<TypeT>(180));
}

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] inline constexpr TypeT rad_to_deg(TypeT rad) noexcept
{
  return rad * (static_cast<TypeT>(180) / PI<TypeT>);
}

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] inline TypeT normalize_angle(TypeT angle) noexcept
{
  return std::atan2(std::sin(angle), std::cos(angle));
}

// ---------- Numeric functions ----------

template<typename TypeT, std::enable_if_t<std::is_integral<TypeT>::value, int> = 0>
[[nodiscard]] constexpr TypeT apply_modulo(TypeT value, TypeT modulus) noexcept
{
  TypeT r = value % modulus;
  return (r < 0) ? r + modulus : r;
}

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] constexpr TypeT apply_deadzone(TypeT value, TypeT dead) noexcept
{
  dead = std::abs(dead);
  return (std::abs(value) <= dead) ? static_cast<TypeT>(0) : value;
}

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] constexpr TypeT apply_saturation(TypeT value, TypeT hi) noexcept
{
  hi = std::abs(hi);
  if (value > hi) {return hi;}
  if (value < -hi) {return -hi;}
  return value;
}

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] constexpr TypeT apply_deadzone_saturation(
  TypeT value, TypeT dead, TypeT lo, TypeT hi) noexcept
{
  dead = std::abs(dead);
  lo = std::abs(lo);
  hi = std::abs(hi);
  if (hi < lo) {lo = hi;}
  if (lo < dead) {lo = dead;}
  const TypeT abs_value = std::abs(value);
  if (abs_value <= dead) {
    return static_cast<TypeT>(0);
  } else if (abs_value <= lo) {
    return (value > static_cast<TypeT>(0)) ? lo : -lo;
  } else if (abs_value >= hi) {
    return (value > static_cast<TypeT>(0)) ? hi : -hi;
  } else {
    return value;
  }
}

template<typename TypeT, std::enable_if_t<std::is_arithmetic<TypeT>::value, int> = 0>
[[nodiscard]] inline constexpr TypeT apply_clamp(TypeT value, TypeT lo, TypeT hi) noexcept
{
  return (value < lo) ? lo : (value > hi) ? hi : value;
}

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] constexpr TypeT apply_low_pass(TypeT current, TypeT previous, TypeT alpha) noexcept
{
  alpha = apply_clamp(alpha, static_cast<TypeT>(0), static_cast<TypeT>(1));
  return (alpha * current) + ((static_cast<TypeT>(1) - alpha) * previous);
}

// ---------- Quaternion functions ----------

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] inline bool is_normalized(
  TypeT x, TypeT y, TypeT z, TypeT w,
  TypeT eps = static_cast<TypeT>(1e-6)) noexcept
{
  const TypeT norm_sq = x * x + y * y + z * z + w * w;
  return std::abs(norm_sq - static_cast<TypeT>(1)) < eps;
}

template<typename TypeT, std::enable_if_t<std::is_floating_point<TypeT>::value, int> = 0>
[[nodiscard]] inline bool is_normalized(
  const Eigen::Quaternion<TypeT> & q,
  TypeT eps = static_cast<TypeT>(1e-6)) noexcept
{
  return is_normalized(q.x(), q.y(), q.z(), q.w(), eps);
}

[[nodiscard]] inline bool is_normalized(
  const tf2::Quaternion & q,
  double eps = 1e-6) noexcept
{
  return is_normalized(q.x(), q.y(), q.z(), q.w(), eps);
}

[[nodiscard]] inline bool is_normalized(
  const geometry_msgs::msg::Quaternion & q,
  double eps = 1e-6) noexcept
{
  return is_normalized(q.x, q.y, q.z, q.w, eps);
}

// ---------- Matrix functions ----------

[[nodiscard]] inline Eigen::Matrix3d skew(const Eigen::Vector3d & v)
{
  return (Eigen::Matrix3d() << 0, -v.z(), v.y(), v.z(), 0, -v.x(), -v.y(), v.x(), 0).finished();
}

[[nodiscard]] inline Eigen::Matrix<double, 6, 6,
  Eigen::RowMajor> adjoint(const Eigen::Isometry3d & T)
{
  const Eigen::Matrix3d R = T.rotation();
  const Eigen::Vector3d t = T.translation();
  Eigen::Matrix<double, 6, 6, Eigen::RowMajor> adj =
    Eigen::Matrix<double, 6, 6, Eigen::RowMajor>::Zero();
  adj.block<3, 3>(0, 0) = R;
  adj.block<3, 3>(0, 3) = skew(t) * R;
  adj.block<3, 3>(3, 0).setZero();
  adj.block<3, 3>(3, 3) = R;
  return adj;
}

} // namespace dua_math
