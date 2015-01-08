/* Fast_Block.c
 *
 * This file contains the implementation for the generated interface.
 *
 *    Don't mess with the begin and end tags, since these will ensure that modified
 *    code in interface functions isn't replaced when code is re-generated.
 */

#include "Fast.h"
#include "Fast__meta.h"

/* $header() */
#include "Fast_Expression.h"
#include "Fast_Parser.h"
#include "Fast_While.h"
#include "Fast__api.h"
void Fast_Parser_error(Fast_Parser _this, char* fmt, ...);
Fast_Parser yparser(void);
/* $end */

/* ::cortex::Fast::Block::addStatement(Fast::Node statement) */
void Fast_Block_addStatement(Fast_Block _this, Fast_Node statement) {
/* $begin(::cortex::Fast::Block::addStatement) */
    cx_assert(_this->statements != NULL, "initialization failed");
    cx_llAppend(_this->statements, statement);
    cx_keep_ext(_this, statement, "Add statement to block");
/* $end */
}

/* ::cortex::Fast::Block::declare(string id,Fast::Variable type,bool isParameter,bool isReference) */
Fast_Local Fast_Block_declare(Fast_Block _this, cx_string id, Fast_Variable type, cx_bool isParameter, cx_bool isReference) {
/* $begin(::cortex::Fast::Block::declare) */
    Fast_Local result;
    Fast_LocalKind kind = 0;

    cx_assert(_this->locals != NULL, "initialization failed");
    
    /* Check if variable already exists. Use lookupLocal instead of lookup since the ordinary lookup also
     * looks for member-variables if the block is part of a function. */
    if (Fast_Block_lookupLocal(_this, id)) {
        printf("%s:%d:%d: local '%s' already declared\n", yparser()->filename, yparser()->line, yparser()->column, id);
        goto error;
    }

    /* If variable did not exist, declare it in this block */
    kind = isParameter ? FAST_LocalParameter : FAST_LocalDefault;
    result = Fast_Local__create(id, type, kind, isReference);
    cx_llAppend(_this->locals, result);

    return result;
error:
    return NULL;
/* $end */
}

/* ::cortex::Fast::Block::declareReturnVariable(function function) */
Fast_Local Fast_Block_declareReturnVariable(Fast_Block _this, cx_function function) {
/* $begin(::cortex::Fast::Block::declareReturnVariable) */
    Fast_Local result;
    Fast_Object type;
    cx_id id;

    /* Get name of function from signature */
    cx_signatureName(cx_nameof(function), id);

    cx_assert(_this->locals != NULL, "initialization failed");

    /* If variable did not exist, declare it in this block */
    type = Fast_Object__create(function->returnType);
    Fast_Parser_collect(yparser(), type);
    result = Fast_Local__create(id, Fast_Variable(type), FAST_LocalReturn, function->returnsReference);
    cx_llAppend(_this->locals, result);

    return result;
/* $end */
}

/* ::cortex::Fast::Block::declareTemplate(string id,Fast::Variable type,bool isParameter,bool isReference) */
Fast_Template Fast_Block_declareTemplate(Fast_Block _this, cx_string id, Fast_Variable type, cx_bool isParameter, cx_bool isReference) {
/* $begin(::cortex::Fast::Block::declareTemplate) */
    Fast_Template result;

    cx_assert(_this->locals != NULL, "initialization failed");

    /* Check if variable already exists */
    if (Fast_Block_lookup(_this, id)) {
        printf("%s:%d:%d: variable '%s' already declared\n", yparser()->filename, yparser()->line, yparser()->column, id);
        goto error;
    }

    /* If variable did not exist, declare it in this block */
    result = Fast_Template__create(id, type, isParameter, isReference);
    cx_llInsert(_this->locals, result);

    return result;
error:
    return NULL;
/* $end */
}

/* ::cortex::Fast::Block::lookup(string id) */
Fast_Expression Fast_Block_lookup(Fast_Block _this, cx_string id) {
/* $begin(::cortex::Fast::Block::lookup) */
    Fast_Expression result = NULL;

    result = Fast_Expression(Fast_Block_lookupLocal(_this, id));

    if (!result) {
        if (_this->function) {
            if ((cx_procedure(cx_typeof(_this->function))->kind == CX_METHOD) ||
                ((cx_procedure(cx_typeof(_this->function))->kind == CX_OBSERVER) && cx_observer(_this->function)->template)) {
                if (strcmp(id, "this")) {
                    cx_interface parent;
                    cx_member m;
                    Fast_Expression thisLocal;

                    thisLocal = Fast_Block_lookup(_this, "this");

                    /* If function is scoped use the parentof function to determine it's parent. The current scope can point
                     * to a different object since functions can be forward declared. If the function is anonymous (for example
                     * in the case of anonymous observers) using the parser-scope is safe since these functions can't be forward
                     * declared.
                     */
                    if (cx_checkAttr(_this->function, CX_ATTR_SCOPED)) {
                        parent = cx_parentof(_this->function);
                    } else {
                        parent = Fast_ObjectBase(yparser()->scope)->value;
                    }
                    m = cx_interface_resolveMember(parent, id);

                    /* If 'this' is not yet declared, lookup is used while declaring
                     * function parameters. */
                    if (thisLocal) {
                        /* If a member is found, create memberexpression */
                        if (m) {
                            Fast_String memberIdExpr;
                            memberIdExpr = Fast_String__create(id);
                            result = Fast_Expression(Fast_MemberExpr__create(
                                     thisLocal, Fast_Expression(memberIdExpr)));
                            Fast_Parser_collect(yparser(), memberIdExpr);
                            Fast_Parser_collect(yparser(), result);
                        } else {
                            cx_method m;
                            /* If no member is found, lookup method */
                            m = cx_interface_resolveMethod(parent, id);
                            if (m) {
                                Fast_String memberIdExpr;
                                memberIdExpr = Fast_String__create(id);
                                result = Fast_Expression(Fast_MemberExpr__create(
                                         thisLocal, Fast_Expression(memberIdExpr)));
                                Fast_Parser_collect(yparser(), memberIdExpr);
                                Fast_Parser_collect(yparser(), result);
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
/* $end */
}

/* ::cortex::Fast::Block::lookupLocal(string id) */
Fast_Local Fast_Block_lookupLocal(Fast_Block _this, cx_string id) {
/* $begin(::cortex::Fast::Block::lookupLocal) */
    Fast_Local result = NULL;

    if (_this->locals) {
        cx_iter iter;
        Fast_Local local;
        iter = cx_llIter(_this->locals);
        while(cx_iterHasNext(&iter)) {
            local = cx_iterNext(&iter);
            if (!strcmp(local->name, id)) {
                result = local;
                break;
            }
        }
    }

    return result;
/* $end */
}

/* ::cortex::Fast::Block::resolve(string id) */
Fast_Expression Fast_Block_resolve(Fast_Block _this, cx_string id) {
/* $begin(::cortex::Fast::Block::resolve) */
    Fast_Expression result = NULL;

    if (!(result = Fast_Block_lookup(_this, id))) {
        if (_this->parent && !_this->function) {
            result = Fast_Block_resolve(_this->parent, id);
        }
    }

    return result;
/* $end */
}

/* ::cortex::Fast::Block::resolveLocal(string id) */
Fast_Local Fast_Block_resolveLocal(Fast_Block _this, cx_string id) {
/* $begin(::cortex::Fast::Block::resolveLocal) */
    Fast_Local result = NULL;

    if (!(result = Fast_Block_lookupLocal(_this, id))) {
        if (_this->parent && !_this->function) {
            result = Fast_Block_resolveLocal(_this->parent, id);
        }
    }

    return result;
/* $end */
}

/* ::cortex::Fast::Block::setFunction(function function */
void Fast_Block_setFunction(Fast_Block _this, cx_function function) {
/* $begin(::cortex::Fast::Block::setFunction) */
    _this->function = function;
    cx_keep_ext(_this, function, "Set function for block");
/* $end */
}

/* ::cortex::Fast::Block::toIc(alias{"cx_icProgram"} program,alias{"cx_icStorage"} storage,bool stored) */
cx_ic Fast_Block_toIc_v(Fast_Block _this, cx_icProgram program, cx_icStorage storage, cx_bool stored) {
/* $begin(::cortex::Fast::Block::toIc) */
    cx_icScope scope;
    CX_UNUSED(storage);
    CX_UNUSED(stored);
    
    scope = cx_icProgram_scopePush(program, Fast_Node(_this)->line);
    
    if (!_this->_while) {
        Fast_Block_toIcBody_v(_this, program, storage, stored);
    } else {
        Fast_While_toIc(_this->_while, program, storage, stored);
    }

    cx_icProgram_scopePop(program, Fast_Node(_this)->line);

    return (cx_ic)scope;
/* $end */
}

/* ::cortex::Fast::Block::toIcBody(alias{"cx_icProgram"} program,alias{"cx_icStorage"} storage,bool stored) */
cx_ic Fast_Block_toIcBody_v(Fast_Block _this, cx_icProgram program, cx_icStorage storage, cx_bool stored) {
/* $begin(::cortex::Fast::Block::toIcBody) */
    Fast_Node statement;
    cx_iter statementIter;
    cx_iter localIter;
    Fast_Local local;
    CX_UNUSED(storage);
    CX_UNUSED(stored);
    
    /* Declare locals */
    if (_this->locals) {
        localIter = cx_llIter(_this->locals);
        while(cx_iterHasNext(&localIter)) {
            local = cx_iterNext(&localIter);
            cx_icLocal__create(
                    program,
                    Fast_Node(_this)->line,
                    local->name,
                    Fast_Expression_getType(Fast_Expression(local)),
                    local->kind == FAST_LocalParameter,
                    local->kind == FAST_LocalReturn,
                    TRUE);
        }
    }
    
    if (_this->statements) {
        statementIter = cx_llIter(_this->statements);
        while(cx_iterHasNext(&statementIter)) {
            statement = cx_iterNext(&statementIter);
            Fast_Node_toIc(statement, program, NULL, FALSE);
        }
    }

    return NULL;
/* $end */
}
