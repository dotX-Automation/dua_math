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

template<typename TypeT>
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

template<typename QuaternionT>
[[nodiscard]] inline bool is_normalized(const QuaternionT & q) noexcept
{
  const double norm = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
  return std::abs(norm - 1.0) < 1e-6;
}

} // namespace dua_math
