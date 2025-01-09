#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "t_error.h"

#define STR_U32_MAX "4294967295"

typedef uint32_t t_u32;

static bool str_is_all_numbers(const char* s);
static bool will_overflow_u32(const char* s);

t_error checked_atou(const char* s, t_u32* out) {
    if (!s || !str_is_all_numbers(s) || !*s)
        return (E_NOT_A_NUMBER);
    if (*s == '0' && *(s + 1) != '\0')
        return (E_NOT_A_NUMBER);
    if (will_overflow_u32(s))
        return (E_OVERFLOW);
    *out = 0;
    while (*s) {
        *out = 10 * *out + (*s - '0');
        s++;
    }
    return (NO_ERROR);
}

static bool str_is_all_numbers(const char* s) {
    if (!s)
        return (false);
    while (*s) {
        if (*s < '0' || *s > '9')
            return (false);
        s++;
    }
    return (true);
}

static int ft_strlen(const char* s) {
    int len = 0;

    while (*s) {
        s++;
        len++;
    }
    return len;
}

static int ft_strncmp(const char* s1, const char* s2, size_t n) {
    while ((*s1 || *s2) && n--) {
        if (*s1 != *s2)
            return ((unsigned char)*s1 - (unsigned char)*s2);
        s1++;
        s2++;
    }
    return (0);
}

static bool will_overflow_u32(const char* s) {
    if (ft_strlen(s) > ft_strlen(STR_U32_MAX))
        return (true);
    if (ft_strlen(s) < ft_strlen(STR_U32_MAX))
        return (false);
    return (ft_strncmp(s, STR_U32_MAX, ft_strlen(STR_U32_MAX)) > 0);
}
