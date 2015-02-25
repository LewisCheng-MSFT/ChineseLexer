// �ļ�: trie.c
// ����: ����
// ����: 2010/4/3
// ����: trie��ʵ��

#include "global.h"
#include "trie.h"
#include "tokens.h"
#include "util.h"

///////////////////////////////////////////////////////////////////////////////
//                                �û��ֵ�                               //
///////////////////////////////////////////////////////////////////////////////

// �ֵ�ṹ����ͷָ��
static struct DictItem dict_head;

// �ֵ��ļ����
static FILE* dict;

// �л�����
static wchar buf[MAX_DICT_BUF_SIZE];
static size_t buf_len = 0;

// ���ֵ���ȡ��һ�У�����fale��ʾ�ļ�����
static bool getline()
{
  assert(dict);
  buf_len = 0;
  while (!feof(dict))
  {
    wchar ch = nextch(dict);
    if (ch == ' ' || ch == '\t' || ch == '\n')
      continue;
    if (ch == '#')
    { // ����ע���С�
      while (!feof(dict) && (ch = nextch(dict)) !='\n');
      continue;
    }
    // ������ȡ��
    while (!feof(dict) && ch !='\n')
    {
      buf[buf_len++] = ch;
      ch = nextch(dict);
    }
    buf[buf_len] = '\0';
    break;
  }
  if (buf_len == 0)
    return false;
  else
    return true;
}

// ���ֵ���һ�з�����������
static struct DictItem* parse_dict_item()
{
  assert(buf_len > 0);
  size_t buf_index = 0;
  size_t index = 0;
  struct DictItem* itm = (struct DictItem*)malloc(sizeof(struct DictItem));
  // ɨ�������
  while (buf[buf_index] != ':' && index < MAX_TOKEN_WORD_LEN)
  {
    itm->word[index++] = buf[buf_index++];
  }
  itm->word[index] = 0;
  buf_index++;
  // ɨ�������
  char tid_str[MAX_TOKEN_ABBR_LEN];
  index = 0;
  while (buf[buf_index] != ':' && index < MAX_TOKEN_ABBR_LEN)
  {
    tid_str[index++] = buf[buf_index++];
  }
  tid_str[index] = 0;
  int i;
  for (i = 0; i < MAX_TOKEN_COUNT; ++i)
    if (!strcmp(tid_str, tokens[i].abbr))
      break;
  if (i == MAX_TOKEN_COUNT)
  {
    fprintf(logging, "δ֪���ͣ�%s��\n", tid_str);
    return NULL;
  }
  else
  {
    itm->tid = tokens[i].tid;
  }
  buf_index++;
  // ɨ�������id
  if (buf[buf_index] == 'x')
  {
    itm->rule = -1;
    buf_index++;
  }
  else
  {
    itm->rule = 0;
    while (buf_index < buf_len && isdigit(buf[buf_index]))
      itm->rule = itm->rule * 10 + (buf[buf_index++] - '0');
  }
  // �л������е��ַ�Ӧ�����ľ���
  assert(buf_index == buf_len);
  return itm;
}

// ��ӡ�����ֵ�
static void print_user_dict()
{
  assert(dict_head.link);
  size_t count = 1;
  fprintf(logging, "\t\t+--------------+\n"
                   "\t\t|   �û��ֵ�   |\n"
                   "\t\t+--------------+\n"
                   " ���\t\t����\t����\t��д\t����id\n"
         );
  struct DictItem* curr = dict_head.link;
  while (curr)
  {
    fprintf(logging, "[%05d]\t\t", count);
    print_wstr(curr->word);
    fprintf(logging, "\t%s\t%s\t%5d\n", tokens[curr->tid].desc,
            tokens[curr->tid].abbr, curr->rule);
    count++;
    curr = curr->link;
  }
}

// �����û��ֵ䡣
bool load_user_dict()
{
  // �ֵ��Ѿ����롣
  if (dict_head.link) return true;

  dict = fopen("data/dict", "r");
  if (!dict)
  {
    fprintf(logging, "�޷����ֵ��ļ���\n");
    return false;
  }
  struct DictItem* last = &dict_head;
  while (getline(dict))
  {
    struct DictItem* temp = parse_dict_item();
    if (!temp) return false;
    temp->link = NULL;
    last->link = temp;
    last = temp;
  }
  fclose(dict);
  // ����ģʽ����ӡ������û��ֵ䡣
# ifndef NDEBUG
  print_user_dict();
# endif
  return true;
}

// ʹ�ø���wchar�ַ������ֵ䣬���ض�Ӧ����ڵ㡣
struct DictItem* lookup_user_dict(wchar* wstr)
{
  assert(wstr);
  struct DictItem* curr = dict_head.link;
  while (curr)
  {
    if (wstrcmp_(wstr, curr->word))
      return curr;
    curr = curr->link;
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//                                    trie                                   //
///////////////////////////////////////////////////////////////////////////////

// trie�����ڵ�
static struct TrieNode trie_root;

// trie�ļ����
static FILE* trie;

#ifndef NDEBUG
static int counter;
#endif

// ��ָ�����ڵ�����к����в���
static struct TrieNode* lookup_childs(struct TrieNode* root, wchar ch)
{
  assert(root);
  struct TrieNode* child = root->first_child.mem_ptr;
  while (child)
  {
    if (child->ch == ch)
      return child;
    child = child->sibling.mem_ptr;
  }
  return NULL;
}

// ǰ�������trie����������ʽ��ӡ������
static void print_trie(struct TrieNode* root, size_t indent)
{
  if (!root)
    return;
  // ��ӡ�����
  space_indent(indent);
  fprintf(logging, "(");
  wchar s[2];
  s[0] = root->ch;
  s[1] = 0;
  print_wstr(s);
  fprintf(logging, ", ");
  if (root->terminable)
    fprintf(logging, "T, %d, %s)\n", root->rule, tokens[root->tid].abbr);
  else
    fprintf(logging, "N)\n");
  struct TrieNode* child = root->first_child.mem_ptr;
  while (child)
  {
    print_trie(child, indent + TRIE_INDENT_WIDTH);
    child = child->sibling.mem_ptr;
  }
}

// ��trie���л����ļ��������н�ʹ������ָ�������ʽ��
static struct TrieNode* save_trie_rec(struct TrieNode* root)
{
  assert(root);
  assert(trie);
  unsigned my_pos = ftell(trie);
# ifndef NDEBUG
  printf("����д�� - ");
  print_wch(root->ch);
  printf(", ���� = %d, �ļ�ƫ�� = %d\n", ++counter, my_pos);
# endif
  // Ϊ�����Ԥ��λ�á�
  fseek(trie, sizeof(struct TrieNode), SEEK_CUR);
  struct TrieNode* child = root->first_child.mem_ptr;
  // ���õ�һ�����ӵ���ָ�롣
  if (root->first_child.raw_ptr > 0)
    root->first_child.raw_ptr = ftell(trie);
  while (child)
  {
    struct TrieNode* child_sibling = save_trie_rec(child);
    child = child_sibling;
  }
  struct TrieNode* my_sibling = root->sibling.mem_ptr;
  // ������һ���ֵܵ���ָ�롣
  if (root->sibling.raw_ptr > 0)
    root->sibling.raw_ptr = ftell(trie);
  // д�뱾��㡣
  unsigned sibling_pos = ftell(trie);
  fseek(trie, my_pos, SEEK_SET);
  fwrite(root, sizeof(struct TrieNode), 1, trie);
  fseek(trie, sibling_pos, SEEK_SET);
  return my_sibling;
}

// �����Ѵ��ڵ�trie�ļ���
bool load_from_trie_file()
{
  // trie�ļ��Ѿ�����
  if (trie_root.first_child.mem_ptr)
    return true;
  trie = fopen("data/trie", "rb");
  if (!trie)
  {
    fprintf(logging, "�޷���trie�ļ�����������ؽ���\n");
    return false;
  }
  // �����ļ���С������ʵ�����ĵ��ڴ��С��
  fseek(trie, 0, SEEK_END);
  unsigned file_size = ftell(trie);
  fseek(trie, 0, SEEK_SET);
  // ����һ�η��䣬���Լ��������Ч�ʡ�
  struct TrieNode* base = (struct TrieNode*)malloc(file_size);
  fread(base, file_size, 1, trie);
  unsigned item_count = file_size / sizeof(struct TrieNode);
  // �������ṹ�е�ָ�롣
  int i;
  for (i = 0; i < item_count; ++i)
  {
    if (base[i].first_child.raw_ptr > 0)
      base[i].first_child.raw_ptr += (unsigned)base;
    if (base[i].sibling.raw_ptr > 0)
      base[i].sibling.raw_ptr += (unsigned)base;
  }
  fclose(trie);
  // ���ø��ڵ����ݡ�
  memcpy(&trie_root, base, sizeof(struct TrieNode));
  // ����ģʽ����ӡ�����trie��
#ifndef NDEBUG
  fprintf(logging, "\t\t+--------------+\n"
                   "\t\t|  �����trie |\n"
                   "\t\t+--------------+\n");
  print_trie(&trie_root, 0);
#endif
  return true;
}

// �����û��ֵ䣬����trie��
void create_trie_from_dict()
{
  assert(dict_head.link);
  trie_root.ch = etow("��");
  struct DictItem* curr = dict_head.link; // ָ���ֵ������е�ǰ���
  struct TrieNode* root;                  // ָ��ǰ�������������ڵ㡣
  struct TrieNode* child;                 // ָ���ҵ��ĺ��ӽ�㡣
  while (curr)
  {
    root = &trie_root; 
    wchar* p = curr->word; // ָ��ǰ��������һ���ַ�λ�á�
    while (*p)
    {
      // Step 1. ��root�����к��ӽ���в����ַ�*p��
      child = lookup_childs(root, *p);
      if (!child)
      { // û���ҵ������½�һ�����ӽ�㣬�����µ�ǰ���ڵ㡣
        child = (struct TrieNode*)malloc(sizeof(struct TrieNode));
        child->ch = *p;
        child->terminable = false;
        child->rule = -1;
        child->tid = TID_INVALID;
        child->first_child.mem_ptr = NULL;
        child->sibling.mem_ptr = root->first_child.mem_ptr;
        root->first_child.mem_ptr = child;
      }
      root = child;
      p++;
    }
    // ��ǰ����������ϣ�rootָ���иô������һ���ַ��Ľ�㡣
    // ����root��������Ϣ��
    root->terminable = true;
    root->rule = curr->rule;
    root->tid = curr->tid;
    // �ݽ�����һ��������
    curr = curr->link;
  }
  // ����ģʽ����ӡ������trie��
#ifndef NDEBUG
  fprintf(logging, "\t\t+--------------+\n"
                   "\t\t|   ������trie |\n"
                   "\t\t+--------------+\n");
  print_trie(&trie_root, 0);
#endif
}

// ������trie���־û���trie�ļ���
// �Լӿ��´η����ٶȣ�
// ���߼��ϣ������û��ֵ��ǵȼ۵ģ�
// �������û��ֵ䷢���˱仯����Ӧ
// �����½���trie�ļ���
// �㷨����˼�룺ʹ��ǰ��ݹ����trie����
// ����ָ��ķ������ڴ��е�ָ�����ļ��е�ƫ�ƹ���һ���ռ䡣
void save_to_trie_file()
{
  assert(trie_root.first_child.mem_ptr);
  trie = fopen("data/trie", "wb");
  if (!trie)
  {
    fprintf(logging, "�޷���trie�ļ���\n");
    return;
  }
# ifndef NDEBUG
  fprintf(logging, "\t\t+--------------+\n"
                   "\t\t| д��trie�ļ� |\n"
                   "\t\t+--------------+\n");
  counter = 0;
# endif
  save_trie_rec(&trie_root);
# ifndef NDEBUG
  printf("�ܽ�����: %d, trie�ļ���СΪ %d �ֽ�.\n", counter, counter * sizeof(struct TrieNode));
# endif
  fclose(trie);
}

// �ж��Ƿ��Ǳ�㡣
bool is_punct(wchar ch)
{
  assert(trie_root.first_child.mem_ptr);
  struct TrieNode* node = lookup_childs(&trie_root, ch);
  if (node && node->tid == TID_PUNCTUATION)
    return true;
  else
    return false;
}

// ʹ�ø���wchar�ַ�������trie�������ض�Ӧ�Ľڵ㣨δ�ҵ��򷵻�NULL����
struct TrieNode* lookup_trie(wchar* wstr)
{
  assert(wstr);
  assert(trie_root.first_child.mem_ptr);
  struct TrieNode* root = &trie_root;
  while (*wstr && root)
  {
    struct TrieNode* child = lookup_childs(root, *wstr);
    // �����ڸ��ַ�
    if (!child)
      return NULL;
    root = child;
    wstr++;
  }
  // ����trie��Χ
  if (*wstr)
    return NULL;
  return root;
}