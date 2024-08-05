/*
 * Lexing / scanning
 *
 * - Step 1.
 * - Input: character stream.
 * - Output: list of words with their syntactic categories (tokens).
 *
 * - Notes...
 *   Syntactic categories: non-terminals.
 *   Literal words and character-set members: terminals.
 *
 *   * * * * * * * * * * * * *
 *
 * - Language syntax summary...
 *
 *   token:
 *     keyword
 *     identifier
 *     constant
 *     string-literal
 *     punctuator
 *
 *   preprocessing-token:
 *     header-name
 *     identifier
 *     pp-number
 *     character-constant
 *     string-literal
 *     punctuator
 *     each non-white-space character that cannot be one of the above
 */

#include "lexer_defs.h"

/*
 * Declarations
 */

static Keyword *keywords;
static Token *tokens;

static C_TOKEN _lexCurToken;
static char _lexBuf[LEXER_BUFSIZE];
static int _lexBufIdx;

static int _lexlinenum = 1;
static int _lexlinepos = 1;
static int _lexlinenumcurw = 1;
static int _lexlineposcurw = 1;

/*
 * API
 */

void lexUngetC(int c, FILE *src)
{
    _lexlinepos--;
    ungetc(c, src);
}

int lexGetC(FILE *src)
{
    _lexlinepos++;
    return getc(src);
}

void lexPrintToken(FILE *out, Token t)
{
    char *toktype;
    if (t.type == C_TOKEN_IDENT)
        toktype = "identifier";
    if (t.type == C_TOKEN_KEYWORD)
        toktype = "keyword";
    if (t.type == C_TOKEN_CONST)
        toktype = "constant";
    if (t.type == C_TOKEN_STRLIT)
        toktype = "string literal";
    if (t.type == C_TOKEN_PUNCT)
        toktype = "punctuation";
    if (t.type == C_TOKEN_PP_HEADERNAME)
        toktype = "preprocessing header-name";
    if (t.type == C_TOKEN_PP_IDENT)
        toktype = "preprocessing identifier";
    if (t.type == C_TOKEN_PP_NUMBER)
        toktype = "preprocessing number";
    if (t.type == C_TOKEN_PP_CHARCONST)
        toktype = "preprocessing character-constant";
    if (t.type == C_TOKEN_PP_STRLIT)
        toktype = "preprocessing string-literal";
    if (t.type == C_TOKEN_PP_PUNCT)
        toktype = "preprocessing punctuation";
    if (t.type == C_TOKEN_PP_WHITESPACE)
        toktype = "preprocessing white-space";
    fprintf(out, "T:[%s (%s) | l:%d,p:%d]", t.value, toktype,
        t.linenum, t.linepos);
}

void lexFault(const char *strarg)
{
    fprintf(stderr, "Error during lexing (%s (l:%d,p:%d)): %s\n",
        _lexBuf, _lexlinenum, _lexlinepos, strarg);
}

void lexInit()
{
    for (int i = 0; i < sizeof(c_keyword_strs) / sizeof(char *);
         ++i) {
        shput(keywords, c_keyword_strs[i], c_keyword_strs[i]);
    }
}

void lexClearBuf()
{
    memset(_lexBuf, 0, _lexBufIdx);
    _lexBufIdx = 0;
}

void lexPushBuf(char c) { _lexBuf[_lexBufIdx++] = c; }

void lexPushToken(C_TOKEN type)
{
    Token t = { type, strdup(_lexBuf), _lexlinenum,
        _lexlinepos - strlen(_lexBuf) };
    arrput(tokens, t);
    lexClearBuf();
}

void lexPPNum(FILE *src) { }

void lexMultiComment(FILE *src)
{
    int c;
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (c == '*') {
            if ((c = lexGetC(src)) == '/') {
                return;
            }
            lexUngetC(c, src);
        }
    }
}

void lexComment(FILE *src)
{
    int c;
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (c == '\n') {
            lexUngetC(c, src);
            return;
        }
    }
}

void lexPP(FILE *src)
{
    int c;
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (c == '\n') {
            lexPushToken(C_TOKEN_PP_HEADERNAME);
            lexUngetC(c, src);
            return;
        } else {
            lexPushBuf(c);
        }
    }
}

void lexCharConst(FILE *src)
{
    int c = lexGetC(src);
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (c == '\'') {
            lexPushToken(C_TOKEN_CONST);
            return;
        } else {
            lexPushBuf(c);
        }
    }
}

void lexConst(FILE *src)
{
    int c; // TODO: handle weird constants.
    bool is_float = false;
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (c == '.' && !is_float) {
            is_float = true;
            lexPushBuf(c);
        } else if (isdigit(c)) {
            lexPushBuf(c);
        } else {
            lexPushToken(C_TOKEN_CONST);
            lexUngetC(c, src);
            return;
        }
    }
}

void lexWord(FILE *src)
{
    int c;
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (!(isalnum(c) || c == '_')) {
            int keyword_idx = shgeti(keywords, _lexBuf);
            if (keyword_idx < 0)
                lexPushToken(C_TOKEN_IDENT);
            else
                lexPushToken(C_TOKEN_KEYWORD);
            lexUngetC(c, src);
            return;
        } else {
            lexPushBuf(c);
        }
    }
}

void lexPunct(FILE *src)
{
    // TODO: handle escape-char
    int c;
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (!(ispunct(c))) {
            lexPushToken(C_TOKEN_PUNCT);
            lexUngetC(c, src);
            return;
        } else if (c == '/') {
            c = lexGetC(src);
            if (c == '/')
                lexComment(src);
            else if (c == '*')
                lexMultiComment(src);
            else
                lexUngetC(c, src);
        } else
            lexPushBuf(c);
    }
}

void lexHexDigit(FILE *src)
{
}

void lexDigit(FILE *src)
{
    int c;
    u8 dot_allowed = 1, exp_allowed = 1, sign_allowed = 1;
    while (!(feof(src)) && !(ferror(src))) {
        int c = lexGetC(src);
        if (isdigit(c))
        else if (c == '.')
        else if (toupper(c) == 'F')
        else if (toupper(c) == 'U')
        else if (toupper(c) == 'L')
        else if (c == '-' || c == '+') {
            if (sign_allowed) {
                lexPushBuf(c);
                continue;
            }
            else {
                lexPushToken(C_TOKEN_CONST);
                lexUngetC(c, src);
                return;
            }
        }
        sign_allowed = 0;
        lexPushBuf(c);
    }
}

void lexZero(FILE *src)
{
    int c;
    c = lexGetC(src);
    lexPushBuf(c);
    if (feof(src) || ferror(src))
        lexFault("encountered eof after 0");
    c = lexGetC(src);
    lexPushBuf(c);
    if (toupper(c) == 'X')
        lexHexDigit(src);
    else
        lexDigit(src);
}

void lexStr(FILE *src)
{
    bool is_valid_char = false;
    if (strlen(_lexBuf) == 0)
        is_valid_char = true;
    else {
        for (int i = 0; i < sizeof(c_str_prefixes) / sizeof(char *);
             ++i)
            if (strcmp(_lexBuf, c_str_prefixes[i]) == 0)
                is_valid_char = true;
    }
    if (!is_valid_char) {
        lexFault("invalid string-literal format");
        return;
    }

    int c;
    while (!(feof(src)) && !(ferror(src))) {
        int c = lexGetC(src);
        if (c == '\\') {
            switch (c = lexGetC(src)) {
            case ('n'): lexPushBuf('\n'); break;
            case ('r'): lexPushBuf('\r'); break;
            case ('t'): lexPushBuf('\t'); break;
            default: lexPushBuf(c); break;
            }
        } else if (c == '\"') {
            lexPushToken(C_TOKEN_STRLIT);
            return;
        } else
            lexPushBuf(c);
    }
    lexFault("encountered eof while parsing string");
}

void lexChar(FILE *src)
{
    bool is_valid_char = false;
    if (strlen(_lexBuf) == 0)
        is_valid_char = true;
    else {
        for (int i = 0; i < sizeof(c_char_prefixes) / sizeof(char *);
             ++i)
            if (strcmp(_lexBuf, c_char_prefixes[i]) == 0)
                is_valid_char = true;
    }
    if (!is_valid_char) {
        lexFault("invalid char format");
        return;
    }
    int c;
    while (!(feof(src)) && !(ferror(src))) {
        int c = lexGetC(src);
        if (c == '\'')
            lexPushToken(C_TOKEN_CONST);
        else
            lexPushBuf(c);
    }
}

void lexAlpha(FILE *src)
{
    int c;
    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        if (isalnum(c)) {
            lexPushBuf(c);
        } else if (c == '\'')
            lexChar(src);
        else if (c == '\"')
            lexStr(src);
        else {
            int found_keyword = shgeti(keywords, _lexBuf);
            if (found_keyword >= 0)
                lexPushToken(C_TOKEN_KEYWORD);
            else
                lexPushToken(C_TOKEN_IDENT);
            lexUngetC(c, src);
        }
    }
}

int lex(FILE *src)
{
    printf("\n >>> Lexing...\n\n");
    int c = lexGetC(src), retval = 0;
    if (c == '#')
        lexPP(src);

    while (!(feof(src)) && !(ferror(src))) {
        c = lexGetC(src);
        // printf("%c\n", c);
        if (c == '\n') {
            _lexlinenum++;
            _lexlinepos = 1;
            _lexlinenumcurw = _lexlinenum;
            _lexlineposcurw = _lexlinepos;
        }
        if (isspace(c)) {
            if ((c = lexGetC(src)) == '#')
                lexPP(src);
            else
                lexUngetC(c, src);
            continue;
        }
        lexUngetC(c, src);

        if (isalpha(c))
            lexAlpha(src);
        if (ispunct(c))
            lexPunct(src);
        if (c == '0')
            lexZero(src);
        if (isdigit(c))
            lexDigit(src);
    }

    if (ferror(src)) {
        lexFault(strerror(errno));
        return ferror(src);
    }

    printf("Tokens:");
    for (int i = 0; i < arrlen(tokens); ++i) {
        if (i % 1 == 0)
            printf("\n");
        else
            printf("\t");
        lexPrintToken(stdout, tokens[i]);
    }
    printf("\n");
    fflush(stdout);

    return retval;
}
