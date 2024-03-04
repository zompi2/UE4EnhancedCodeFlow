// Copyright (c) 2024 Damian Nowakowski. All rights reserved.

#pragma once

#ifdef __cpp_impl_coroutine

#include <coroutine>

/**
 * Defining coroutine handlers and promises in order to get coroutines work.
 */

struct FECFCoroutinePromise;
using FECFCoroutineHandle = std::coroutine_handle<FECFCoroutinePromise>;

struct FECFCoroutine : FECFCoroutineHandle
{
	using promise_type = ::FECFCoroutinePromise;
};

struct FECFCoroutinePromise
{
	FECFCoroutine get_return_object() { return { FECFCoroutine::from_promise(*this) }; }
	std::suspend_never initial_suspend() { return {}; }
	std::suspend_never final_suspend() noexcept { return {}; }
	void return_void() {}
	void unhandled_exception() {}
};

#else

/**
 * Create dummy implementations of coroutine handles if coroutines are not supported by a compiler.
 */

using FECFCoroutine = void;

struct FECFCoroutineHandle 
{
	void resume() {};
};

#define co_await static_assert(false, "Trying to use co_await without coroutine support!")

#endif