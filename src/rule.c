// �ļ�: rule.c
// ����: ����
// ����: 2010/4/3
// ����: ���Ե�ʵ��

#include "global.h"
#include "tokens.h"
#include "rule.h"

// ��ƥ�䡱�����ͷ��㡣
struct Match match_head;

// �����ļ������
static FILE* rule;

// ��������ļ���
bool load_rule_file()
{
  if (rule) return true;
  rule = fopen("data/rule", "r");
  if (!rule)
  {
    fprintf(logging, "�޷��򿪲����ļ���\n");
    return false;
  }
  // Not implemented here!
  fclose(rule);
  return true;
}

// �����ƥ������ѡ����õ�һ����
struct Match* select_best_match()
{
  struct Match* match = match_head.link;
  return match;
}