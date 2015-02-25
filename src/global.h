// �ļ�: global.h
// ����: ����
// ����: 2010/4/3
// ����: ȫ�ֲ����ͱ����Ķ���

#ifndef CHS_GLOBAL_H_
#define CHS_GLOBAL_H_

#include <assert.h>
#include <ctype.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �Ƿ�رյ���ģʽ
#define NDEBUG

// �Ǻ��������
#define MAX_TOKEN_COUNT 15

// �ǺŴ��ﳤ��
#define MAX_TOKEN_WORD_LEN 32

// �Ǻ���д����
#define MAX_TOKEN_ABBR_LEN 10

// �ֵ��л����С
#define MAX_DICT_BUF_SIZE 100

// ��ӡtrieʱ�������
#define TRIE_INDENT_WIDTH 4

// ���Ա���ansi��չ�ַ�������
typedef unsigned short wchar;

// �ļ���
extern FILE* text;

// ��־����¼������Ϣ
extern FILE* logging;

// ����ϸģʽ��ӡ�Ǻš�
extern bool human_readable;

#endif // CHS_GLOBAL_H_