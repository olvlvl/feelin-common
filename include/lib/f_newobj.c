#define NO_TAG_CALL

#include <stdarg.h>
#include <proto/feelin.h>
#include <feelin/machine.h>

FObject F_NewObj(STRPTR Name,...)
{
    va_list msg;
    va_start(msg,Name);

    return IFEELIN F_NewObjA(Name, (struct TagItem *) msg);
}

