#ifndef FORMAT_H
#define FORMAT_H

#include "shide/sh_year_month_day.h"

static const std::string default_month_names[]{
        "Farvardin",
        "Ordibehesht",
        "Khordad",
        "Tir",
        "Mordad",
        "Shahrivar",
        "Mehr",
        "Aban",
        "Azar",
        "Dey",
        "Bahman",
        "Esfand",
        "Far",
        "Ord",
        "Kho",
        "Tir",
        "Mor",
        "Sha",
        "Meh",
        "Aba",
        "Aza",
        "Dey",
        "Bah",
        "Esf"
};
static const std::string default_weekday_names[]{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat"
};

inline
std::pair<const std::string*, const std::string*>
month_names(const std::string nm[]=nullptr)
{
    return (nm == nullptr) ?
        std::make_pair(default_month_names, default_month_names + 24) : std::make_pair(nm, nm + 24);
}

inline
unsigned
extract_month(std::ostream& os, const sh_fields& fds)
{
    if (!fds.ymd.month().ok())
    {
        os.setstate(std::ios::failbit);
        return 0;
    }
    return static_cast<unsigned>(fds.ymd.month());
}

std::ostream&
sh_to_stream(std::ostream& os, const char* fmt, const sh_fields& fds,
    const std::string month_nms[] = nullptr, const std::string weekday_nms[] = nullptr
)
{
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::chrono::minutes;
    using std::chrono::hours;
    using date::detail::save_ostream;
    os.fill(' ');
    os.flags(std::ios::skipws | std::ios::dec);
    os.width(0);
    tm tm{};
    bool insert_negative = fds.has_tod && fds.tod.to_duration() < seconds::zero();
    const char* command = nullptr;
    for (; *fmt; ++fmt)
    {
        switch (*fmt)
        {
        case 'b':
        case 'B':
        case 'h':
            if (command)
            {
                tm.tm_mon = static_cast<int>(extract_month(os, fds)) - 1;
                os << month_names(month_nms).first[tm.tm_mon + 12 * (*fmt != 'B')];
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'd':
        case 'e':
            if (command)
            {
                if (!fds.ymd.day().ok())
                    os.setstate(std::ios::failbit);
                auto d = static_cast<int>(static_cast<unsigned>(fds.ymd.day()));
                save_ostream<char> _(os);
                if (*fmt == char{ 'd' })
                    os.fill('0');
                else
                    os.fill(' ');
                os.flags(std::ios::dec | std::ios::right);
                os.width(2);
                os << d;
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'F':
            if (command)
            {
                if (!fds.ymd.ok())
                    os.setstate(std::ios::failbit);
                auto const& ymd = fds.ymd;
                save_ostream<char> _(os);
                os.imbue(std::locale::classic());
                os.fill('0');
                os.flags(std::ios::dec | std::ios::right);
                os.width(4);
                os << static_cast<int>(ymd.year()) << char{ '-' };
                os.width(2);
                os << static_cast<unsigned>(ymd.month()) << char{ '-' };
                os.width(2);
                os << static_cast<unsigned>(ymd.day());
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'j':
            if (command)
            {
                if (fds.ymd.ok())
                {
                    auto doy = sh_yday(fds.ymd);
                    save_ostream<char> _(os);
                    os.fill('0');
                    os.flags(std::ios::dec | std::ios::right);
                    os.width(3);
                    os << doy.count();
                }
                else
                {
                    os.setstate(std::ios::failbit);
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'm':
            if (command)
            {
                if (!fds.ymd.month().ok())
                    os.setstate(std::ios::failbit);
                auto m = static_cast<unsigned>(fds.ymd.month());
                if (m < 10)
                    os << char{ '0' };
                os << m;
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'n':
            if (command)
            {
                os << char{ '\n' };
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 't':
            if (command)
            {
                os << char{ '\t' };
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'Y':
            if (command)
            {
                if (!fds.ymd.year().ok())
                    os.setstate(std::ios::failbit);
                auto y = fds.ymd.year();
                save_ostream<char> _(os);
                os.imbue(std::locale::classic());
                os << y;
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case '%':
            if (command)
            {
                os << char{ '%' };
                command = nullptr;

            }
            else
                command = fmt;
            break;
        default:
            if (command)
            {
                os << char{ '%' };
                command = nullptr;
            }
            os << *fmt;
            break;
        }
    }
    if (command)
        os << char{ '%' };
    return os;
}

#endif
