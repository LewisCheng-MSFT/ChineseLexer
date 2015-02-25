// �ļ�: util.c
// ����: ����
// ����: 2010/4/3
// ����: ʵ�ú�����ʵ��

#include "global.h"
#include "util.h"

// ����չ�ַ�ת��Ϊwchar�ַ���
// ech�ַ���Ӧ������Ϊ2.
wchar etow(const char* ech)
{
  return ((wchar)((unsigned char)*ech) << 8) | ((unsigned char)*(ech + 1));
}

// ��wchar�ַ���ת��ΪANSI�ַ�����
char* wtos(const wchar* wstr)
{
  assert(false);
  return NULL;
}

// ��wchar�ַ���ת��ΪANSI�ַ�����
wchar* stow(const char* str)
{
  assert(str);
  size_t len = strlen(str) + 1;
  wchar* wstr = (wchar*)malloc(len * sizeof(wchar));
  size_t index = 0;
  while (*str)
  {
    wchar ch = (wchar)((unsigned char)(*str));
    if (ch > 127)
    {
      wchar ch1 = (wchar)((unsigned char)(*(str + 1)));
      wstr[index++] = (ch << 8) | ch1;
      str += 2;
    }
    else
      wstr[index++] = *str++;
  }
  wstr[index] = 0;
  return wstr;
}

// ��¼��ǰ�ַ�ռ�õ��ֽ�����
static int step = 0;

// �����ļ�������һ���ַ���
// �����ļ������������ַ���ʾ����չ�ַ����纺�֣���
// ��һ���Զ�ȡ�����ء�
wchar nextch(FILE* strm)
{
  assert(strm);
  wchar ch = fgetc(strm);
  if (ch > 127)
  { // ���������������ַ���ʾ��չ�ַ���
    ch = (ch << 8) | (fgetc(strm));
    step = 2;
  }
  else
    step = 1;
  return ch;
}

// ����ǰ�ַ��Ż��ļ����С�
// ʵ�ʲ����ǻ����ļ�ָ�롣
void backch(FILE* strm)
{
  assert(strm);
  fseek(strm, -step, SEEK_CUR);
}

// ���ڱ��������ַ�����Ϊwchar��
// ������Ҫ�ر�Ĵ�ӡ������
void print_wch(wchar wch)
{
  char s[3];
  s[2] = 0;
  if (wch > 127)
  { // ��չ�ַ�
    s[0] = wch >> 8;
    s[1] = wch;
    fprintf(logging, "%s", s);
  }
  else
  {
    fprintf(logging, "%c", (char)wch);
  }
}

// ���ڱ������д�����ַ�����Ϊwchar��
// ������Ҫ�ر�Ĵ�ӡ������
void print_wstr(const wchar* wstr)
{
  assert(wstr);
  while (*wstr)
    print_wch(*wstr++);
}

// �Ƚ�����wchar�ַ����Ƿ����
bool wstrcmp_(const wchar* wstr1, const wchar* wstr2)
{
  assert(wstr1);
  assert(wstr2);
  while (*wstr1 && *wstr2 && *wstr1 == *wstr2)
  {
    wstr1++;
    wstr2++;
  }
  if (*wstr1 || *wstr2)
    return false;
  else
    return true;
}

// ��wstr2���Ƶ�wstr1��
void wstrcpy_(wchar* wstr1, const wchar* wstr2)
{
  assert(wstr1);
  assert(wstr2);
  while (*wstr1++ = *wstr2++);
}

// ����wchar�ַ����ĳ��ȣ���������0��
size_t wstrlen_(const wchar* wstr)
{
  assert(wstr);
  size_t len = 0;
  while (*wstr++)
    len++;
  return len;
}

// ��ӡn���ո�
void space_indent(size_t n)
{
  while (n-- > 0)
    fprintf(logging, " ");
}