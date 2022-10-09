/* C code produced by gperf version 3.1 */
/* Command-line: gperf src/parsercmds.gperf  */
/* Computed positions: -k'1-2' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 11 "src/parsercmds.gperf"

#include "parsercmds.h"
/* maximum key range = 34, duplicates = 0 */

#ifndef GPERF_DOWNCASE
#define GPERF_DOWNCASE 1
static unsigned char gperf_downcase[256] =
  {
      0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
     30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
     45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
     60,  61,  62,  63,  64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255
  };
#endif

#ifndef GPERF_CASE_STRCMP
#define GPERF_CASE_STRCMP 1
static int
gperf_case_strcmp (s1, s2)
     register const char *s1;
     register const char *s2;
{
  for (;;)
    {
      unsigned char c1 = gperf_downcase[(unsigned char)*s1++];
      unsigned char c2 = gperf_downcase[(unsigned char)*s2++];
      if (c1 != 0 && c1 == c2)
        continue;
      return (int)c1 - (int)c2;
    }
}
#endif

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
parserCmd_hash (str, len)
     register const char *str;
     register size_t len;
{
  static const unsigned char asso_values[] =
    {
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 10, 35, 35, 35,  0,
      35,  0, 13,  0, 35, 35,  8, 30, 15,  0,
      35, 25,  5, 10,  5,  0, 35,  5,  0, 35,
      35, 35, 35, 35, 35, 35, 35, 10, 35, 35,
      35,  0, 35,  0, 13,  0, 35, 35,  8, 30,
      15,  0, 35, 25,  5, 10,  5,  0, 35,  5,
       0, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35, 35, 35, 35, 35,
      35, 35, 35, 35, 35, 35
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct parserCmd *
parserCmd_inWordSet (str, len)
     register const char *str;
     register size_t len;
{
  enum
    {
      TOTAL_KEYWORDS = 22,
      MIN_WORD_LENGTH = 1,
      MAX_WORD_LENGTH = 9,
      MIN_HASH_VALUE = 1,
      MAX_HASH_VALUE = 34
    };

  static const struct parserCmd wordlist[] =
    {
#line 36 "src/parsercmds.gperf"
      {"E",         103},
#line 18 "src/parsercmds.gperf"
      {"GO",        1},
#line 21 "src/parsercmds.gperf"
      {"GET",       3},
#line 28 "src/parsercmds.gperf"
      {"EXIT",      99},
#line 37 "src/parsercmds.gperf"
      {"W",         104},
#line 20 "src/parsercmds.gperf"
      {"EXAMINE",   2},
#line 33 "src/parsercmds.gperf"
      {"WEST",      104},
#line 26 "src/parsercmds.gperf"
      {"ITEMS",     6},
#line 35 "src/parsercmds.gperf"
      {"S",         102},
#line 19 "src/parsercmds.gperf"
      {"LOOK",      2},
#line 23 "src/parsercmds.gperf"
      {"USE",       5},
#line 32 "src/parsercmds.gperf"
      {"EAST",      103},
#line 31 "src/parsercmds.gperf"
      {"SOUTH",     102},
#line 34 "src/parsercmds.gperf"
      {"N",         101},
#line 16 "src/parsercmds.gperf"
      {"HELP",      0},
#line 22 "src/parsercmds.gperf"
      {"TALK",      4},
#line 30 "src/parsercmds.gperf"
      {"NORTH",     101},
#line 29 "src/parsercmds.gperf"
      {"AROUND",    100},
#line 24 "src/parsercmds.gperf"
      {"INTERACT",  5},
#line 25 "src/parsercmds.gperf"
      {"INVENTORY", 6},
#line 27 "src/parsercmds.gperf"
      {"QUIT",      99},
#line 17 "src/parsercmds.gperf"
      {"MOVE",      1}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = parserCmd_hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          register const struct parserCmd *resword;

          if (key < 14)
            {
              switch (key - 1)
                {
                  case 0:
                    resword = &wordlist[0];
                    goto compare;
                  case 1:
                    resword = &wordlist[1];
                    goto compare;
                  case 2:
                    resword = &wordlist[2];
                    goto compare;
                  case 3:
                    resword = &wordlist[3];
                    goto compare;
                  case 5:
                    resword = &wordlist[4];
                    goto compare;
                  case 6:
                    resword = &wordlist[5];
                    goto compare;
                  case 8:
                    resword = &wordlist[6];
                    goto compare;
                  case 9:
                    resword = &wordlist[7];
                    goto compare;
                  case 10:
                    resword = &wordlist[8];
                    goto compare;
                  case 11:
                    resword = &wordlist[9];
                    goto compare;
                  case 12:
                    resword = &wordlist[10];
                    goto compare;
                }
            }
          else
            {
              switch (key - 14)
                {
                  case 0:
                    resword = &wordlist[11];
                    goto compare;
                  case 1:
                    resword = &wordlist[12];
                    goto compare;
                  case 2:
                    resword = &wordlist[13];
                    goto compare;
                  case 3:
                    resword = &wordlist[14];
                    goto compare;
                  case 5:
                    resword = &wordlist[15];
                    goto compare;
                  case 6:
                    resword = &wordlist[16];
                    goto compare;
                  case 7:
                    resword = &wordlist[17];
                    goto compare;
                  case 9:
                    resword = &wordlist[18];
                    goto compare;
                  case 10:
                    resword = &wordlist[19];
                    goto compare;
                  case 15:
                    resword = &wordlist[20];
                    goto compare;
                  case 20:
                    resword = &wordlist[21];
                    goto compare;
                }
            }
          return 0;
        compare:
          {
            register const char *s = resword->name;

            if ((((unsigned char)*str ^ (unsigned char)*s) & ~32) == 0 && !gperf_case_strcmp (str, s))
              return resword;
          }
        }
    }
  return 0;
}
