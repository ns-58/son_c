/*
 * IR - Lightweight JIT Compilation Framework
 * (Examples package)
 * Copyright (C) 2023 by IR project.
 * Authors: Anatol Belski <anbelski@linux.microsoft.com>
 */

#include "ir.h"
#include "ir_builder.h"
#include <stdlib.h>

/*
 * int32_t myfunc(int32_t x, int32_t y) {
 *	return x - y;
 * }
 */
typedef int32_t (*myfunc_t)(int32_t, int32_t);

void gen_myfunc(ir_ctx *ctx)
{
	/* Function entry start */
	ir_START();
	/* Declare function parameters */
	ir_ref x = ir_PARAM(IR_I32, "x", 1);
	ir_ref y = ir_PARAM(IR_I32, "y", 2);
	/* Subtract y from x and save it into a new ref. */
	ir_ref cr = ir_SUB_I32(x, y);
	/* Function end */
	ir_RETURN(cr);
}

int main(int argc, char **argv)
{
	// ir_ctx ctx;
	
	ir_ctx *ctx_ptr = ir_create_ctx();
	ir_consistency_check();

	ir_init(ctx_ptr, 256 | IR_OPT_FOLDING, IR_CONSTS_LIMIT_MIN, IR_INSNS_LIMIT_MIN);
	ir_set_i32_ret_type(ctx_ptr);

	gen_myfunc(ctx_ptr);

	size_t size;
	void *entry = ir_jit_compile(ctx_ptr, 2, &size);
	if (entry) {
		printf("1104 - 202 = %d\n", ((myfunc_t)entry)(1104, 202));
	}

	ir_free(ctx_ptr);

	return 0;
}
