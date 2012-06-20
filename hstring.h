#ifndef __HSTRING_H__
#define __HSTRING_H__

/// �ı����ݲ���˳���ַ��������ݽṹ�������£�
typedef struct
{  
    char *ch;   ///< ���Ƿǿմ�,�򰴴�������洢��,����chΪNULL  
    int length; ///< �����ȣ���������ֹ��
} HString;

/// 1��������һ����ֵ���ڴ�����chars�Ĵ�T
void hstr_assign(HString* T, const char* chars);

/// 2�����ɴ�S���Ƶô�T
void hstr_copy(const HString* S, HString* T);

/// 3������SΪ�մ�,�򷵻�TRUE,���򷵻�FALSE
int hstr_empty(const HString* S);

/// 4������S>T,�򷵻�ֵ>0;��S=T,�򷵻�ֵ=0;��S<T,�򷵻�ֵ<0
int hstr_compare(const HString* S, const HString* T);

/// 5��������S��Ԫ�ظ���,��Ϊ���ĳ���
int hstr_length(const HString* S);

/// 6������S��Ϊ�մ�
void hstr_clear(HString* S);

/// 7������T������S1��S2���Ӷ��ɵ��´�
HString hstr_concat(const HString* S1, const HString* S2);

/// 8������Sub���ش�S�ĵ�pos���ַ��𳤶�Ϊlen���Ӵ��� ����,1��pos��StrLength(S)��0��len��StrLength(S)-pos+1
HString hstr_sub(const HString* S, int pos, int len);

/// 9������ʼ��(�����մ�)�ַ���T�������ô���Ϊ0
void hstr_init(HString* T);

/// 10����TΪ�ǿմ���������S�е�pos���ַ�֮�������T��ȵ��Ӵ�,�򷵻ص�һ���������Ӵ���S�е�λ��,���򷵻�0
int hstr_find(const HString* str, const HString* sub);

/// 11�����ڴ�S�ĵ�pos���ַ�֮ǰ���봮T����1��pos��StrLength(S)+1
void hstr_insert(HString* ori, const HString* ins, int pos);

/// 12�����Ӵ�S��ɾ����pos���ַ��𳤶�Ϊlen���Ӵ�
void hstr_remove(HString* S, int pos, int len);

/// 13������S,T��V����,T�Ƿǿմ����˺����봮�Ĵ洢�ṹ�޹أ��������:��V�滻����S�г��ֵ�������T��ȵĲ��ص����Ӵ�
void hstr_replace(HString* S, const HString* T, const HString* V);

/// 14�������T�ַ���
void hstr_print(const HString* str);

#endif