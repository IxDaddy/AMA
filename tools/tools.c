#include <stdio.h>
#include <time.h>

char* TimestampToHumanString(time_t timestamp)
{
    struct tm  ts;
    char       buf[80];

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&timestamp);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    char* date = buf;
    return date;
}

struct tm TimestampToHumanStruct(time_t timestamp)
{
    struct tm t;
    t = *localtime(&timestamp);
    return t;
}

void HumanToTimestamp(void) {
    struct tm t;
    time_t t_of_day;

    t.tm_year = 2019-1900;  // Year - 1900
    t.tm_mon = 7;           // Month, where 0 = jan
    t.tm_mday = 8;          // Day of the month
    t.tm_hour = 16;
    t.tm_min = 11;
    t.tm_sec = 42;
    t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);

    printf("seconds since the Epoch: %ld\n", (long) t_of_day);
}
