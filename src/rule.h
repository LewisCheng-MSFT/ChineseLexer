// �ļ�: rule.h
// ����: ����
// ����: 2010/4/3
// ����: ���ԵĽӿ�

#ifndef CHS_RULE_H_
#define CHS_RULE_H_

// ƥ��Ľṹ��
struct Match
{
  wchar word[MAX_TOKEN_WORD_LEN];   // ƥ��Ĵ����ַ���
  int tid;                          // ��������
  int rule;                         // ���Ա��
  unsigned next_pos;                // ָ����ļ��д���֮���λ��
  struct Match* link;               // ָ����һ��ƥ��
};

// ��ƥ�䡱�����ͷ��㡣
extern struct Match match_head;

// ��������ļ���
bool load_rule_file();

// �����ƥ������ѡ����õ�һ����
struct Match* select_best_match();

#endif