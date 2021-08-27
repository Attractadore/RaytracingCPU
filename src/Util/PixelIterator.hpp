#pragma once

#include <glm/vec3.hpp>
#include <iterator>

template <typename I>
concept PixelIterator = std::is_same_v<typename I::value_type, glm::vec3>;

template <typename I>
concept PixelForwardIterator = std::forward_iterator<I> and PixelIterator<I>;
