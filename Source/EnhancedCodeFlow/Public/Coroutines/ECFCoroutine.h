// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#if defined(__cpp_impl_coroutine) && __has_include(<coroutine>)

#define ECF_WITH_COROUTINES 1

#include <coroutine>
#include "ECFHandle.h"

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
	std::suspend_never initial_suspend() noexcept { return {}; }
	std::suspend_never final_suspend() noexcept { return {}; }
	void return_void() { bHasFinished = true; }
	void unhandled_exception() {}
	bool bHasFinished = false;
	FECFHandle ActionHandle;
};

#else

/**
 * Create dummy implementations of coroutine handles if coroutines are not supported by a compiler.
 */

#include "ECFHandle.h"

#define ECF_WITH_COROUTINES 0

using FECFCoroutine = void;

struct FECFCoroutinePromise
{
	bool bHasFinished = false;
	FECFHandle ActionHandle;
};

struct FECFCoroutineHandle 
{
	void resume() {}
	void destroy() {}

	FECFCoroutinePromise CoroPromise;
	FECFCoroutinePromise& promise() { return CoroPromise; }
};

#define co_await static_assert(false, "Trying to use co_await without coroutine support!")

#endif