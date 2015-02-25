// �ļ�: tokens.h
// ����: ����
// ����: 2010/4/3
// ����: ����ʷ��Ǻ���ص���Ϣ����

#ifndef CHS_TOKENS_H_
#define CHS_TOKENS_H_

enum TOKEN_TIDS {
  TID_INVALID,      // ��Ч�Ǻ�
  TID_NOUN,         // ����
  TID_PRONOUN,      // ����
  TID_VERB,         // ����
  TID_QUALIFIER,    // ���δʣ����ݴʻ򸱴ʣ�
  TID_NUMERIAL,     // ���ʣ�һ����������
  TID_QUANTIFIER,   // ����
  TID_PREPOSITION,  // ���
  TID_AUXILIARY,    // ����
  TID_CONJUNCT,	    // ����
  TID_IDIOM,        // ϰ������ȣ�
  TID_PUNCTUATION,  // ���ı�����
  TID_NUMBER,       // ����
  TID_ENGLISH,      // Ӣ�ﵥ��
  TID_OTHER         // ����δ֪���ַ�
};

struct TokenInfo
{
  int tid;    // �Ǻű��
  char* desc; // �Ǻ�����
  char* abbr; // �Ǻ���д��������ʾ�Ǻż���ȡ�ֵ��ļ���
};

extern struct TokenInfo tokens[];

// �ʷ������ļǺŽṹ
struct Token
{
  union {
    wchar word[MAX_TOKEN_WORD_LEN]; // ����
    char str[MAX_TOKEN_WORD_LEN * 2]; // Ӣ�ĵ���
    int num;  //����
  };
  int tid;
};

#endif // CHS_TOKENS_H_
