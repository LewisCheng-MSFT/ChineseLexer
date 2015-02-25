// �ļ�: trie.h
// ����: ����
// ����: 2010/4/3
// ����: trie�Ľӿ�

// �ֵ�����ṹ
struct DictItem
{
  wchar word[MAX_TOKEN_WORD_LEN]; // ����
  int tid; // ���
  int rule; // ����id��-1��ʾû�У�
  struct DictItem* link;
};

// trie���ṹ
struct TrieNode
{
  wchar ch;                     // �ý��洢�ĺ���
  bool terminable;              // �ý���Ƿ��ǿ���ֹ��㣬���Ƿ��дӸ����ý��Ĵ��
  int rule;                     // �����սᣬָ���Ӧ����id��-1��ʾû�С�
  int tid;                      // �����սᣬ��ʾ�ô�������
  union {
    struct TrieNode* mem_ptr;   // �ڴ��е�ָ��
    unsigned raw_ptr;           // �ļ��е�ƫ��
  } first_child;                // ָ���һ�����ӽ���ָ��
  union {
    struct TrieNode* mem_ptr;
    unsigned raw_ptr;
  } sibling;                    // ָ����һ���ֵܽ���ָ��
};

// �����û��ֵ䡣
bool load_user_dict();

// ʹ�ø���wchar�ַ������ֵ䣬���ض�Ӧ����ڵ㣨δ�ҵ��򷵻�NULL����
struct DictItem* lookup_user_dict(wchar* wstr);

// �����Ѵ��ڵ�trie�ļ���
bool load_from_trie_file();

// �����û��ֵ䣬����trie��
void create_trie_from_dict();

// ������trie���־û���trie�ļ���
// �Լӿ��´η����ٶȣ�
// ���߼��ϣ������û��ֵ��ǵȼ۵ģ�
// �������û��ֵ䷢���˱仯����Ӧ
// �����½���trie�ļ���
// �㷨����˼�룺ʹ��ǰ��ݹ����trie����
// ����ָ��ķ������ڴ��е�ָ�����ļ��е�ƫ�ƹ���һ���ռ䡣
void save_to_trie_file();

// �ж��Ƿ��Ǳ�㡣
bool is_punct(wchar ch);

// ʹ�ø���wchar�ַ�������trie�������ض�Ӧ�Ľڵ㣨δ�ҵ��򷵻�NULL����
struct TrieNode* lookup_trie(wchar* wstr);