#ifndef __HSTRING_H__
#define __HSTRING_H__

/// 文本内容采用顺序字符串，数据结构定义如下：
typedef struct
{  
    char *ch;   ///< 若是非空串,则按串长分配存储区,否则ch为NULL  
    int length; ///< 串长度，不包括终止符
} HString;

/// 1）．生成一个其值等于串常量chars的串T
void hstr_assign(HString* T, const char* chars);

/// 2）．由串S复制得串T
void hstr_copy(const HString* S, HString* T);

/// 3）．若S为空串,则返回TRUE,否则返回FALSE
int hstr_empty(const HString* S);

/// 4）．若S>T,则返回值>0;若S=T,则返回值=0;若S<T,则返回值<0
int hstr_compare(const HString* S, const HString* T);

/// 5）．返回S的元素个数,称为串的长度
int hstr_length(const HString* S);

/// 6）．将S清为空串
void hstr_clear(HString* S);

/// 7）．用T返回由S1和S2联接而成的新串
HString hstr_concat(const HString* S1, const HString* S2);

/// 8）．用Sub返回串S的第pos个字符起长度为len的子串。 其中,1≤pos≤StrLength(S)且0≤len≤StrLength(S)-pos+1
HString hstr_sub(const HString* S, int pos, int len);

/// 9）．初始化(产生空串)字符串T，并设置串长为0
void hstr_init(HString* T);

/// 10）．T为非空串。若主串S中第pos个字符之后存在与T相等的子串,则返回第一个这样的子串在S中的位置,否则返回0
int hstr_find(const HString* str, const HString* sub);

/// 11）．在串S的第pos个字符之前插入串T其中1≤pos≤StrLength(S)+1
void hstr_insert(HString* ori, const HString* ins, int pos);

/// 12）．从串S中删除第pos个字符起长度为len的子串
void hstr_remove(HString* S, int pos, int len);

/// 13）．串S,T和V存在,T是非空串（此函数与串的存储结构无关）操作结果:用V替换主串S中出现的所有与T相等的不重叠的子串
void hstr_replace(HString* S, const HString* T, const HString* V);

/// 14）．输出T字符串
void hstr_print(const HString* str);

#endif