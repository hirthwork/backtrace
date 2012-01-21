#include <backtrace/record.h>
#include <backtrace/record.hpp>

int main()
{
    TBacktraceRecord record = {0, 0, 0, 0, 0};
    NBacktrace::TDemangledBacktraceRecord demangled(record);
}

