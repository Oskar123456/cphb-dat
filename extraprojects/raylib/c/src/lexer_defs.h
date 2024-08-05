#define LEXER_BUFSIZE 1024

/*
 * Types
 */

enum LEX_MODE {
    LM_NORMAL,
    LM_PP,
};

enum C_TOKEN {
    C_TOKEN_KEYWORD,
    C_TOKEN_IDENT,
    C_TOKEN_CONST,
    C_TOKEN_STRLIT,
    C_TOKEN_PUNCT,
    C_TOKEN_PP_HEADERNAME,
    C_TOKEN_PP_IDENT,
    C_TOKEN_PP_NUMBER,
    C_TOKEN_PP_CHARCONST,
    C_TOKEN_PP_STRLIT,
    C_TOKEN_PP_PUNCT,
    C_TOKEN_PP_WHITESPACE,
};

struct c_keyword {
    union {
        const char *key;
        const char *value;
    };
};

struct c_token {
    enum C_TOKEN type;
    const char *value;
    int linenum, linepos;
};

typedef enum C_TOKEN C_TOKEN; 
typedef struct c_token Token; 
typedef struct c_keyword Keyword; 

/*
 * Declarations
 */

static const char* c_keyword_strs[] = {
    "alignof", "auto", "break", "case", "char", "const",
    "continue", "default", "do", "double", "else", "enum",
    "extern", "float", "for", "goto", "if", "inline",
    "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "void", "volatile", "while", "_Alignas", "_Atomic",
    "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn", 
    "_Static_assert", "_Thread_local",
};

static const char* c_char_prefixes[] = {
    "L",
    "U",
    "u",
};

static const char* c_str_prefixes[] = {
    "L",
    "U",
    "u",
    "u8",
};
