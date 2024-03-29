/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_CPU_H_HEADER_GUARD
#define BASE_CPU_H_HEADER_GUARD

#include "base.h"

namespace base
{
	///
	void readBarrier();

	///
	void writeBarrier();

	///
	void readWriteBarrier();

	///
	void memoryBarrier();

	///
	template<typename Ty>
	Ty atomicFetchAndAdd(volatile Ty* _ptr, Ty _value);

	///
	template<typename Ty>
	Ty atomicAddAndFetch(volatile Ty* _ptr, Ty _value);

	///
	template<typename Ty>
	Ty atomicFetchAndSub(volatile Ty* _ptr, Ty _value);

	///
	template<typename Ty>
	Ty atomicSubAndFetch(volatile Ty* _ptr, Ty _value);

	///
	template<typename Ty>
	Ty atomicCompareAndSwap(volatile Ty* _ptr, Ty _old, Ty _new);

	///
	template<typename Ty>
	Ty atomicFetchTestAndAdd(volatile Ty* _ptr, Ty _test, Ty _value);

	///
	template<typename Ty>
	Ty atomicFetchTestAndSub(volatile Ty* _ptr, Ty _test, Ty _value);

	///
	template<typename Ty>
	Ty atomicFetchAndAddsat(volatile Ty* _ptr, Ty _value, Ty _max);

	///
	template<typename Ty>
	Ty atomicFetchAndSubsat(volatile Ty* _ptr, Ty _value, Ty _min);

	///
	void* atomicExchangePtr(void** _ptr, void* _new);

} // namespace base

#include "inline/cpu.inl"

#endif // BASE_CPU_H_HEADER_GUARD
