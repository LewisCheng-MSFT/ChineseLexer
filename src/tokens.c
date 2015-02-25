// �ļ�: tokens.c
// ����: ����
// ����: 2010/4/3
// ����: ����ʷ��Ǻ���ص���Ϣ����

#include "global.h"
#include "tokens.h"

#define TOK(TID, DESC, ABBR) { TID, DESC, ABBR } 

struct TokenInfo tokens[] = {
  TOK(TID_INVALID, "��Ч", "invalid"),
  TOK(TID_NOUN, "����", "n"),
  TOK(TID_PRONOUN, "����", "pron"),
  TOK(TID_VERB, "����", "v"),
  TOK(TID_QUALIFIER, "���δ�", "qual"),
  TOK(TID_NUMERIAL, "����", "numrl"),
  TOK(TID_QUANTIFIER, "����", "quan"),
  TOK(TID_PREPOSITION, "���", "prep"),
  TOK(TID_AUXILIARY, "����", "aux"),
  TOK(TID_CONJUNCT, "����", "conj"),
  TOK(TID_IDIOM, "ϰ��", "idiom"),
  TOK(TID_PUNCTUATION, "���", "punct"),
  TOK(TID_NUMBER, "����", "num"),
  TOK(TID_ENGLISH, "Ӣ�ﵥ��", "eng"),
  TOK(TID_OTHER, "����", "o")
};
