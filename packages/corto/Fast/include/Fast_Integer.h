/* Fast_Integer.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef Fast_Integer_H
#define Fast_Integer_H

#include "corto.h"
#include "Fast_Literal.h"
#include "Fast__type.h"
#include "Fast__api.h"
#include "Fast__meta.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ::corto::Fast::Integer::init() */
cx_int16 _Fast_Integer_init(Fast_Integer _this);
#define Fast_Integer_init(_this) _Fast_Integer_init(Fast_Integer(_this))

/* ::corto::Fast::Integer::serialize(type dstType,word dst) */
cx_int16 _Fast_Integer_serialize(Fast_Integer _this, cx_type dstType, cx_word dst);
#define Fast_Integer_serialize(_this, dstType, dst) _Fast_Integer_serialize(Fast_Integer(_this), cx_type(dstType), dst)

/* virtual ::corto::Fast::Integer::toIc(ic::program program,ic::storage storage,bool stored) */
ic_node Fast_Integer_toIc(Fast_Integer _this, ic_program program, ic_storage storage, cx_bool stored);

/* ::corto::Fast::Integer::toIc(ic::program program,ic::storage storage,bool stored) */
ic_node _Fast_Integer_toIc_v(Fast_Integer _this, ic_program program, ic_storage storage, cx_bool stored);
#define Fast_Integer_toIc_v(_this, program, storage, stored) _Fast_Integer_toIc_v(Fast_Integer(_this), ic_program(program), ic_storage(storage), stored)

#ifdef __cplusplus
}
#endif
#endif

