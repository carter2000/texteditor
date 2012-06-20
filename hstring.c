#include "hstring.h"

#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>

#define CHARS_STOP  '\0'// ×Ö·û´®ÖÕÖ¹·û

static char* _chars_malloc(int size)
{
    char* chars = (char*)malloc(size*sizeof(char));
    memset(chars, 0, size);
    return chars;
}

static int _chars_length(const char* chars)
{
    const char* iter = chars;
    while (*iter++ != CHARS_STOP) {
    }
    return iter - chars - 1;
}

static char* _chars_copy(const char* src, char* dst)
{
    char* p = dst;
    while (*src != CHARS_STOP) {
        *p++ = *src++;
    }
    *p = CHARS_STOP;
    return dst;
}

static char* _chars_n_copy(const char* src, char* dst, int n)
{
    char* p = dst;
    while (*src != CHARS_STOP && n > 0) {
        *p++ = *src++;
        --n;
    }
    *p = CHARS_STOP;
    return dst;
}

static int _chars_compare(const char* left, const char* right)
{
    assert(left);
    assert(right);
    while (*left != CHARS_STOP && *right != CHARS_STOP) {
        if (*left == *right) {
            ++left;
            ++right;
            continue;
        }
        return *left - *right;
    }

    if (*left != CHARS_STOP) {
        return 1;
    }
    else if (*right != CHARS_STOP) {
        return -1;
    }
    else {
        return 0;
    }
}

static int _chars_n_compare(const char* left, const char* right, int n)
{
    assert(left);
    assert(right);

    if (n <= 0) {
        return 0;
    }

    while (*left != CHARS_STOP && *right != CHARS_STOP) {
        if (--n == 0) {
            return *left - *right;
        }
        if (*left == *right) {
            ++left;
            ++right;
            continue;
        }
        return *left - *right;
    }

    if (*left != CHARS_STOP) {
        return 1;
    }
    else if (*right != CHARS_STOP) {
        return -1;
    }
    else {
        return 0;
    }
}

void hstr_assign(HString* str, const char* chars)
{
    assert(str);

    free(str->ch);

    if (chars == NULL) {
        str->ch = NULL;
        str->length = 0;
    }
    else {
        str->length = _chars_length(chars);
        str->ch = _chars_malloc(str->length + 1);
        _chars_copy(chars, str->ch);
    }
}

void hstr_copy(const HString* src, HString* dst)
{
    assert(src);
    assert(dst);

    free(dst->ch);

    if (src->ch == NULL) {
        assert(src->length == 0);
        dst->ch = NULL;
    }
    else {
        dst->ch = _chars_malloc(src->length + 1);
        _chars_copy(src->ch, dst->ch);
    }
    dst->length = src->length;
}

int hstr_empty(const HString* str)
{
    assert(str);
    return str->ch == NULL;
}

int hstr_compare(const HString* left, const HString* right)
{
    assert(left);
    assert(right);

    if (!hstr_empty(left)) {
        return hstr_empty(right) ? 1 : _chars_compare(left->ch, right->ch);
    }
    else {
        return hstr_empty(right) ? 0 : -1;
    }
}

int hstr_length(const HString* str)
{
    assert(str);
    return str->length;
}

void hstr_clear(HString* str)
{
    assert(str);
    free(str->ch);
    str->ch = NULL;
    str->length = 0;
}

HString hstr_concat(const HString* left, const HString* right)
{
    HString ret;
    assert(left);
    assert(right);

    hstr_init(&ret);

    if (hstr_empty(left)) {
        hstr_copy(right, &ret);
    }
    else if (hstr_empty(right))
    {
        hstr_copy(left, &ret);
    }
    else {
        ret.length = left->length + right->length;
        ret.ch = _chars_malloc(ret.length + 1);
        _chars_copy(left->ch, ret.ch);
        _chars_copy(right->ch, ret.ch + left->length);
    }

    return ret;
}

HString hstr_sub(const HString* src, int pos, int len)
{
    HString sub;
    assert(src);
    assert(pos >= 1);
    assert(pos <= hstr_length(src));
    assert(len >= 0);
    assert(len <= hstr_length(src) - pos + 1);
    hstr_init(&sub);
    if (len > 0) {
        sub.ch = _chars_malloc(len + 1);
        _chars_n_copy(src->ch + pos, sub.ch, len);
        sub.length = len;
    }
    return sub;
}

void hstr_init(HString* str)
{
    assert(str);
    str->ch = NULL;
    str->length = 0;
}

int hstr_find(const HString* str, const HString* sub)
{
    int pos = 0;
    int maxpos = 0;

    assert(str);
    assert(sub);

    if (hstr_empty(sub)) {
        return 0;
    }

    maxpos = str->length - sub->length;
    if (maxpos < 0) {
        return 0;
    }

    pos = 0;
    while (pos <= maxpos) {
        if (_chars_n_compare(str->ch + pos, sub->ch, sub->length) == 0) {
            break;
        }
        ++pos;
    }

    return (pos <= maxpos) ? pos + 1 : 0;
}

void hstr_insert(HString* ori, const HString* ins, int pos)
{
    int taillen = 0;
    char* newchars = NULL;

    assert(ori);
    assert(pos >= 1);
    assert(pos <= hstr_length(ori) + 1);

    if (hstr_empty(ins)) {
        return;
    }

    taillen = ori->length - pos + 1;
    ori->length = ori->length + ins->length;
    newchars = _chars_malloc(ori->length + 1);

    if (pos > 1) {
        _chars_n_copy(ori->ch, newchars, pos - 1);
    }

    _chars_n_copy(ins->ch, newchars + pos - 1, ins->length);
    
    if (taillen > 0) {
        _chars_n_copy(ori->ch + pos - 1, newchars + ins->length + pos - 1, taillen);
    }

    free(ori->ch);
    ori->ch = newchars;
}

void hstr_remove(HString* str, int pos, int len)
{
    int taillen = 0;

    assert(str);
    pos -= 1;
    if (pos < 0 || pos >= str->length || len <= 0) {
        return;
    }

    if (len + pos > str->length) {
        len = str->length - pos;
    }

    taillen = str->length - pos - len;
    assert(taillen >= 0);
    _chars_n_copy(str->ch + pos + len, str->ch + pos, taillen);
    str->length = str->length - len;
    str->ch[str->length] = CHARS_STOP;
}

void hstr_replace(HString* ori, const HString* sub, const HString* rep)
{
    int pos = 0;
    int lastpos = 0;
    int maxpos = 0;

    assert(ori);
    assert(sub && !hstr_empty(sub));
    assert(rep);

    while ((maxpos = ori->length - sub->length) >= 0) {
        pos = lastpos;
        while (pos <= maxpos) {
            if (_chars_n_compare(ori->ch + pos, sub->ch, sub->length) == 0) {
                break;
            }
            ++pos;
        }
        if (pos > maxpos) {
            break;
        }
        hstr_remove(ori, pos + 1, sub->length);
        hstr_insert(ori, rep, pos + 1);
        lastpos = pos + rep->length;
    }
}

void hstr_print(const HString* str)
{
    assert(str);
    if (!hstr_empty(str)) {
        printf("%s", str->ch);
    }
}
