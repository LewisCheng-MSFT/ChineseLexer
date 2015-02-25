// �ļ�: util.h
// ����: ����
// ����: 2010/4/3
// ����: ʵ�ú����Ľӿ�

#ifndef CHS_UTIL_H_
#define CHS_UTIL_H_

// ����չ�ַ�ת��Ϊwchar�ַ���
// ech�ַ���Ӧ������Ϊ2.
wchar etow(const char* ech);

// ��wchar�ַ���ת��ΪANSI�ַ�����
char* wtos(const wchar* wstr);

// ��wchar�ַ���ת��ΪANSI�ַ�����
wchar* stow(const char* str);

// �����ļ�������һ���ַ���
// �����ļ������������ַ���ʾ����չ�ַ����纺�֣���
// ��һ���Զ�ȡ�����ء�
wchar nextch(FILE* strm);

// ����ǰ�ַ��Ż��ļ����С�
// ʵ�ʲ����ǻ����ļ�ָ�롣
void backch(FILE* strm);

// ���ڱ��������ַ�����Ϊwchar��
// ������Ҫ�ر�Ĵ�ӡ������
void print_wch(wchar wch);

// ���ڱ������д�����ַ�����Ϊwchar��
// ������Ҫ�ر�Ĵ�ӡ������
void print_wstr(const wchar* wstr);

// �Ƚ�����wchar�ַ����Ƿ����
bool wstrcmp_(const wchar* wstr1, const wchar* wstr2);

// ��wstr2���Ƶ�wstr1��
void wstrcpy_(wchar* wstr1, const wchar* wstr2);

// ����wchar�ַ����ĳ��ȣ���������0��
size_t wstrlen_(const wchar* wstr);

// ��ӡn���ո�
void space_indent(size_t n);

#endif // CHS_UTIL_H_