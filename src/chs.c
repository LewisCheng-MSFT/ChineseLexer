// �ļ�: chs.c
// ����: ����
// ����: 2010/4/3
// ����: ����ʷ���������������

#include "global.h"
#include "lex.h"
#include "trie.h"
#include "rule.h"
#include "util.h"

FILE* text = NULL;
FILE* logging = NULL;
bool human_readable = false;

int main(int argc, char * argv[])
{
  logging = stdout;
  if (argc == 1)
  {
    fprintf(logging, "�÷���chs <�ļ���>\n");
    return 0;
  }
  // ȷ���Ƿ���Ҫ�����������´���trie�ļ�����������trie��
  if (argc == 2 && !strcmp(argv[1], "-ctf"))
  {
    if (!load_user_dict())
      return 1;
    create_trie_from_dict();
    save_to_trie_file();
    fprintf(logging, "trie�ļ������ɹ���\n");
    return 0;
  }
  // ȷ�������ļ���
  char* filename;
  if (argc == 3)
  {
    if (!strcmp(argv[1], "-h"))
      human_readable = true;
    filename = argv[2];
  }
  else if (argc == 2)
    filename = argv[1];
  else
  {
    fprintf(logging, "����Ĳ���������\n");
    return 1;
  }
  // ���벢���������ļ���
  text = fopen(filename, "r");
  if (!text)
  {
    fprintf(logging, "�޷����ļ�%s��\n", filename);
    return 1;
  }
  if (!load_from_trie_file())
    return 1;
  if (!load_rule_file())
    return 1;
  do_lex();
  fclose(text);
  return 0;
}
