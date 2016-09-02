#pragma once
#include <plog/Log.h>

namespace plog
{
    class PlainFormatter
    {
    public:
        static util::nstring header() // This method returns a header for a new file. In our case it is empty.
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record) // This method returns a string from a record.
        {
            util::nstringstream ss;
            ss << record.getMessage().c_str() << "\n"; // Produce a simple string with a log message.

            return ss.str();
        }
    };
}

