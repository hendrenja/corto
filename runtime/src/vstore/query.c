/* This is a managed file. Do not delete this comment. */

#include <corto/corto.h>
uint32_t corto_query_cardinality(
    corto_query* this)
{
    if (!strchr(this->select, '*') || strstr(this->select, "//")) {
        return -1;
    } else {
        return 1;
    }

}

bool corto_query_match(
    corto_query* this,
    corto_result *result)
{
    corto_assert(result->id != NULL, "no id specified in result");
    corto_assert(result->parent != NULL, "no parent specified in result");
    corto_assert(result->type != NULL, "no type specified in result");

    if (this->type && this->type[0] && strcmp(this->type, result->type)) {
        return false;
    }

    if (this->from && this->from[0] && strcmp(this->from, result->parent)) {
        return false;
    }

    if (this->select && !corto_idmatch(this->select, result->id)) {
        return false;
    }

    return true;
}

