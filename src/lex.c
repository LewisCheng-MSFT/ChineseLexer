// �ļ�: lex.c
// ����: ����
// ����: 2010/4/3
// ����: �ʷ���������ʵ��

#include "global.h"
#include "lex.h"
#include "trie.h"
#include "tokens.h"
#include "util.h"
#include "rule.h"

// ��ǰ�Ǻš�
static struct Token curr;

// ɨ��Ӣ�ĵ���
static void scan_eng_word
{
  assert(text);
  char ch;
  int index = 0;
  while (!feof(text) && isalpha(ch = fgetc(text)))
    curr.str[index++] = ch;
  backch(text);
  curr.str[index] = 0;
  curr.tid = TID_ENGLISH;
}

// ɨ������
static void scan_num
{
  assert(text);
  char ch;
  curr.num = 0;
  while (!feof(text) &&isdigit(ch = fgetc(text)))
    curr.num = curr.num * 10 + (ch - '0');
  backch(text);
  curr.tid = TID_NUMBER;
}

// ����trieɨ������п��ܵ�ƥ�䡣
static void scan_all_matches()
{
  struct Match* temp;
  // �������գ������ͷ���ռ䡣
  struct Match* p = match_head.link;
  while (p)
  {
    temp = p->link;
    free(p);
    p = temp;
  }
  match_head.link = 0;
  // ��¼�µ�ǰλ�á�
  unsigned start_pos = ftell(text);
  // ��ǰ��Ҫƥ��ĳ��ȡ�
  int match_len;
  for (match_len = 1; match_len < MAX_TOKEN_WORD_LEN; match_len++)
  {
    // ���뵱ǰ������ƥ����ַ���
    int i;
    for (i = 0; i < match_len; i++)
      curr.word[i] = nextch(text);
    curr.word[i] = 0;
    struct TrieNode* node = lookup_trie(curr.word);
    if (node)
    { 
      // ƥ��ɹ��������ն˽�㣬�������ڵ�
      if (node->terminable)
      {
        temp = (struct Match*)malloc(sizeof(struct Match));
        wstrcpy_(temp->word, curr.word);
        temp->tid = node->tid;
        temp->rule = node->rule;
        temp->next_pos = ftell(text);
        temp->link = match_head.link;
        match_head.link = temp;
      }
      fseek(text, start_pos, SEEK_SET);
    }
    else
    { // match_len����ƥ�䲻�ɹ���
      fseek(text, start_pos, SEEK_SET);
      break;
    }
  }
}

// ɨ����չ�ַ�Ϊ�׵ļǺţ�
// �����Ǳ�㣬�����Ǻ��֣�
// Ҳ�������������֣������������
static void scan_ech
{
  assert(text);
  wchar ch = nextch(text);
  if (is_punct(ch))
  { // �Ǳ��
    curr.word[0] = ch;
    curr.word[1] = 0;
    curr.tid = TID_PUNCTUATION;
    return;
  }
  backch(text);
  // ɨ�������ƥ�䣬�����Ѱ��ӳ��������С�
  scan_all_matches();
  if (match_head.link)
  { // �ҵ�������һ��ƥ�䡣
    struct Match* match;
    // ������ʹ�ò��ԡ�
    match = select_best_match();
    // ʹ�øò������ɼǺš�
    wstrcpy_(curr.word, match->word);
    curr.tid = match->tid;
    fseek(text, match->next_pos, SEEK_SET);
  }
  else
  { // �ֵ����Ҳ���ƥ��Ĵ��
    curr.word[0] = nextch(text);
    curr.word[1] = 0;
    curr.tid = TID_OTHER;
  }
}

// ȡ����һ���Ǻ�
static void next_token()
{
  assert(text);
  wchar ch = nextch(text);
  if (feof(text))
  {
    curr.tid = TID_INVALID;
    return;
  }
  while (ch == ' ' || ch == '\t' || ch == '\n')
  { // �հ��ַ���ԭ�����
    if (!human_readable)
      fprintf(logging, "%c", (char)ch);
    ch = nextch(text);
    continue;
  }
  if (isalpha(ch))
  { // ����Ӣ�ﵥ�ʡ�
    backch(text);
    scan_eng_word;
  }
  else if (isdigit(ch))
  { // �������֡�
    backch(text);
    scan_num;
  }
  else if (ch > 127)
  { // ������չ�ַ���
    backch(text);
    scan_ech;
  }
  else
  { // ����
    curr.word[0] = ch;
    curr.word[1] = 0;
    curr.tid = TID_OTHER;
  }
}

// �����ǰ�Ǻš�
static void print_token()
{
  assert(curr.tid != TID_INVALID);
  if (curr.tid == TID_NUMBER)
  {
    fprintf(logging, "%d", curr.num);
  }
  else if (curr.tid == TID_ENGLISH)
  {
    fprintf(logging, "%s", curr.str);
  }
  else
  {
    print_wstr(curr.word);
  }
  if (human_readable)
    fprintf(logging, "\t\t%s\n", tokens[curr.tid].desc);
  else
    fprintf(logging, "/%s  ", tokens[curr.tid].abbr);
}

// �ʷ�������������
void do_lex()
{
  assert(text);
  while (true)
  {
    next_token();
    if (curr.tid == TID_INVALID)
      break;
    print_token();
  }
}
