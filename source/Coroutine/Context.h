/*
 *  This file is part of the "Coroutine" project and released under the MIT License.
 *
 *  Created by Samuel Williams on 3/11/2018.
 *  Copyright, 2018, by Samuel Williams. All rights reserved.
*/

#pragma once

#include <assert.h>
#include <string.h>

#if __cplusplus
extern "C" {
#endif

#define COROUTINE __attribute__((noreturn, fastcall)) void

const size_t COROUTINE_REGISTERS = 4;

typedef struct
{
	void **stack_pointer;
} CoroutineContext;

typedef COROUTINE(* CoroutineStart)(CoroutineContext *from, CoroutineContext *self, void * argument) __attribute__((fastcall));

static inline void coroutine_initialize(
	CoroutineContext *context,
	CoroutineStart start,
	void *argument,
	void *stack_pointer,
	size_t stack_size
) {
	/* The i386 System V ABI requires that ESP+4 is 16B-aligned on entry to the function. */
	context->stack_pointer = (void**)((uintptr_t)stack_pointer & ~0xF);
	
	*--context->stack_pointer = NULL;
	*--context->stack_pointer = NULL;
	*--context->stack_pointer = NULL;
	*--context->stack_pointer = argument;
	*--context->stack_pointer = NULL;
	*--context->stack_pointer = (void*)start;
	
	context->stack_pointer -= COROUTINE_REGISTERS;
	memset(context->stack_pointer, 0, sizeof(void*) * COROUTINE_REGISTERS);
}

CoroutineContext * coroutine_transfer(CoroutineContext * current, CoroutineContext * target) __attribute__((fastcall));

#if __cplusplus
}
#endif
