// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#if defined(__cpp_concepts) && __cpp_concepts >= 201907L && __has_include(<concepts>)

#define ECF_WITH_CONCEPTS 1
#include <concepts>

template<typename T> struct TIsSoftPointer : std::false_type {};
template<typename U> struct TIsSoftPointer<TSoftObjectPtr<U>> : std::true_type {};
template<typename U> struct TIsSoftPointer<TSoftClassPtr<U>> : std::true_type {};

template<typename T>
concept CIsSoftPtrType = TIsSoftPointer<T>::value;

#else

#define ECF_WITH_CONCEPTS 0
#define CIsSoftPtrType typename

#endif
