#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

///// interface /////

template <class ToType, class FromType>
constexpr ToType to(const FromType& from);

template <class ToType, class FromType>
constexpr ToType to(FromType&& from);

///// concepts /////

template <typename T>
concept Arithmetic = std::is_arithmetic_v<std::remove_cv_t<T>>;

template <typename T>
concept Container = std::ranges::range<T>;

template <typename T>
concept ContainerArithmetic = Container<T> && std::is_arithmetic_v<typename T::value_type>;

template <typename T>
concept ContainerString = Container<T> && std::same_as<typename T::value_type, std::string>;

template <typename T>
concept Enum = std::is_enum_v<std::remove_cv_t<T>>;

template <typename T>
concept EnumArithmetic = Enum<T> && std::is_arithmetic_v<std::underlying_type_t<T>>;

template <typename T>
concept PairSameType =
    std::same_as<T, std::pair<typename T::first_type, typename T::second_type>> &&
    std::same_as<typename T::first_type, typename T::second_type>;

template <typename T>
concept PairArithmetic = PairSameType<T> && std::is_arithmetic_v<typename T::first_type>;

///// specializations /////

template <class ToType, class FromType>
  requires std::same_as<std::remove_cv_t<ToType>, std::string> &&
           std::same_as<std::remove_cv_t<FromType>, std::string>
ToType to(const FromType& from) {
  return from;
}

template <class ToType, class FromType>
  requires std::same_as<std::remove_cv_t<ToType>, std::string> &&
           std::same_as<std::remove_cv_t<FromType>, std::string>
ToType to(FromType&& from) {
  return std::move(from);
}

template <class ToType, class FromType>
  requires EnumArithmetic<FromType> && Arithmetic<ToType>
constexpr ToType to(const FromType& from) {
  return static_cast<ToType>(from);
}

template <class ToType, class FromType>
  requires EnumArithmetic<FromType> && Arithmetic<ToType>
constexpr ToType to(FromType&& from) {
  return static_cast<ToType>(from);
}

template <class ToType, class FromType>
  requires std::same_as<std::remove_cv_t<ToType>, std::vector<std::string::value_type>> &&
           std::same_as<std::remove_cv_t<FromType>, std::string>
ToType to(const FromType& from) {
  ToType ret{};

  for (const auto& ch : from) {
    ret.emplace_back(ch);
  }

  return ret;
}

template <class ToType, class FromType>
  requires std::same_as<std::remove_cv_t<ToType>, std::vector<std::string::value_type>> &&
           std::same_as<std::remove_cv_t<FromType>, std::string>
ToType to(FromType&& from) {
  ToType ret{};

  for (auto&& ch : from) {
    ret.emplace_back(std::move(ch));
  }

  return ret;
}

template <class ToType, class FromType>
  requires std::is_arithmetic_v<ToType> && std::same_as<std::remove_cv_t<FromType>, std::string>
ToType to(const FromType& from) {
  if constexpr (std::is_integral_v<ToType>) {
    if constexpr (std::is_unsigned_v<ToType>) {
      return static_cast<ToType>(std::stoull(from));
    } else {
      return static_cast<ToType>(std::stoll(from));
    }
  } else if constexpr (std::is_floating_point_v<ToType>) {
    return static_cast<ToType>(std::stold(from));
  }
}

template <class ToType, class FromType>
  requires std::is_arithmetic_v<ToType> && std::same_as<std::remove_cv_t<FromType>, std::string>
ToType to(FromType&& from) {
  if constexpr (std::is_integral_v<ToType>) {
    if constexpr (std::is_unsigned_v<ToType>) {
      return static_cast<ToType>(std::stoull(from));
    } else {
      return static_cast<ToType>(std::stoll(from));
    }
  } else if constexpr (std::is_floating_point_v<ToType>) {
    return static_cast<ToType>(std::stold(from));
  }
}

template <class ToType, class FromType>
  requires ContainerArithmetic<ToType> && ContainerString<FromType>
ToType to(const FromType& from) {
  ToType vec{};

  std::transform(from.begin(), from.end(), std::back_inserter(vec),
                 [](const auto& s) { return to<typename ToType::value_type>(s); });

  return vec;
}

template <class ToType, class FromType>
  requires ContainerArithmetic<ToType> && ContainerString<FromType>
ToType to(FromType&& from) {
  ToType vec{};

  std::transform(from.begin(), from.end(), std::back_inserter(vec),
                 [](const auto& s) { return to<typename ToType::value_type>(s); });

  return vec;
}

template <class ToType, class FromType>
  requires PairSameType<ToType> && Container<FromType> &&
           std::same_as<typename ToType::first_type, typename FromType::value_type>
ToType to(const FromType& from) {
  if (from.size() != 2) {
    throw std::runtime_error(
        fmt::format("Can not construct pair (size=2) from container (actual_size={}): '{}' ",
                    from.size(), fmt::join(from, ",")));
  }

  return {from[0], from[1]};
}

template <class ToType, class FromType>
  requires PairSameType<ToType> && Container<FromType> &&
           std::same_as<typename ToType::first_type, typename FromType::value_type>
ToType to(FromType&& from) {
  if (from.size() != 2) {
    throw std::runtime_error(
        fmt::format("Can not construct pair (size=2) from container (actual_size={}): '{}' ",
                    from.size(), fmt::join(from, ",")));
  }

  return {std::move(from[0]), std::move(from[1])};
}

template <class ToType, class FromType>
  requires PairArithmetic<ToType> && ContainerString<FromType>
ToType to(const FromType& from) {
  const auto vec = to<std::vector<typename ToType::first_type>>(from);
  return to<ToType>(vec);
}

template <class ToType, class FromType>
  requires ContainerArithmetic<ToType> && std::same_as<FromType, std::string>
ToType to(const FromType& from) {
  std::vector<std::string> vec{};
  std::transform(from.begin(), from.end(), std::back_inserter(vec),
                 [](const char& c) { return std::string(1, c); });

  return to<ToType>(const_cast<std::add_lvalue_reference_t<std::add_const_t<decltype(vec)>>>(vec));
}
