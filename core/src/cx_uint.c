/* cx_uint.c
 *
 * This file contains the implementation for the generated interface.
 *
 * Don't mess with the begin and end tags, since these will ensure that modified
 * code in interface functions isn't replaced when code is re-generated.
 */

#include "cx.h"

/* ::corto::lang::uint::init() */
cx_int16 cx_uint_init(cx_uint _this) {
/* $begin(::corto::lang::uint::init) */
    cx_primitive(_this)->kind = CX_UINTEGER;
    return cx_primitive_init((cx_primitive)_this);
/* $end */
}
