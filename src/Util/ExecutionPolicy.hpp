#pragma once

#include <execution>

template <typename P>
concept ExecutionPolicy = std::is_execution_policy_v<P>;
