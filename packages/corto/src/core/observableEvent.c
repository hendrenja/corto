/* $CORTO_GENERATED
 *
 * observableEvent.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "corto/core/core.h"

corto_void _corto_observableEvent_handle_v(corto_observableEvent this) {
/* $begin(corto/core/observableEvent/handle) */
    if (!corto_readBegin(this->observable)) {
        corto_call(corto_function(this->observer), NULL, this->me, this->observable, this->source);
        corto_readEnd(this->observable);
        corto_event_handle_v(corto_event(this));
    } else {
        /* Error */
        corto_error("%s", corto_lasterr());
    }
/* $end */
}