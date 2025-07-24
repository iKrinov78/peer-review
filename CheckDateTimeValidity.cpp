enum class DateFailReason {
    _IS_TOO_SMALL   = 1 << 0, // 1
    _IS_TOO_BIG     = 1 << 1, // 2
    _YEAR           = 1 << 2, // 4
    _MONTH          = 1 << 3, // 8
    _DAY            = 1 << 4, // 16
    _HOUR           = 1 << 5, // 32 
    _MINUTE         = 1 << 6, // 64
    _SECOND         = 1 << 7 // 128
};

void CheckDateTimeValidity(const DateTime& dt) {

    array date = {dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second};
    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    array date_limit = {9999, 12, month_lengths[dt.month - 1], 23, 59, 59};
    int result{0};

    for (int i = 0; i < 6; ++i) {
        if (date[i] < 1) {result = 1 + 1 << (i + 2);}
        if (date[i] > date_limit) {result = 2 + 1 << (i + 2);}
        if (!result) {throw static_cast<DateFailReason>(result);}
    }
}
