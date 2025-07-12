#ifndef TZDB_H
#define TZDB_H

#include <date/tz.h>
#include <stdbool.h>

namespace tzdb
{
    inline
    bool
        locate_zone(const std::string& name, const date::time_zone*& p_time_zone) {
        try {
            p_time_zone = date::locate_zone(name);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    inline
    bool
        get_local_info(const date::local_seconds& tp,
            const date::time_zone* p_time_zone,
            date::local_info& info) {
        try {
            info = p_time_zone->get_info(tp);
            return true;
        }
        catch (...) {
            return false;
        }
    }

    inline
    bool
        get_sys_info(const date::sys_seconds& tp,
            const date::time_zone* p_time_zone,
            date::sys_info& info) {
        try {
            info = p_time_zone->get_info(tp);
            return true;
        }
        catch (...) {
            return false;
        }
    }
}

#endif
