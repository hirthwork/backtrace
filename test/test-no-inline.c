#include <inline.h>
#include "checks.h"

int main()
{
    REQUIRE_STRINGS_EQUAL(TO_STRING(BACKTRACE_INLINE), "");
    return 0;
}

