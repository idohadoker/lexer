RE_RESERVED_WORDS = [r'while',
                     r'do',
                     r'for',
                     r'if',
                     r'else if',
                     r'else',
                     r'switch',
                     r'case',
                     r'break',
                     r'continue',
                     r'union',
                     r'default',
                     r'typedef',  # done
                     r'struct',  # need
                     r'typedef struct',  # need
                     r'enum',  # done
                     r'sizeof',
                     r'#include',  # done
                     r'#define',  # done
                     r'#if',
                     r'#endif',
                     r'goto,'
                     r'#elif',
                     r'#else',
                     r'return']  # need
RE_COMMENT = r'//'  # done
RE_COMMENT_END = [r'*', r'/']  # done
RE_COMMENT_START = [r'/', r'*']  # done
RE_VARIABLES_TYPE = [r'int', r'long', r'short', r'double', r'char', r'float', r'auto', r'void']  # done
RE_MODIFIER = [r'const', r'signed', r'unsigned', r'static', r'volatile', r'register', r'extern']  # done
RE_ARITHMETIC_OPERATOR = [r'-', r'+', r'*', r'/', r'%']  # done
RE_ASSIGNMENTS_OPERATOR = [r'=', r'+=', r'-=', r'*=', r'/=', r'%=']  # done
RE_BITWISE_ASSIGNMENT_OPERATOR = [r'<<=', r'>>=', r'&=', r'|=', r'^=']  # done
RE_UNARY_OPERATOR = [r'++', r'--']  # done
RE_RELATIONAL_OPERATOR = [r'<=', r'<', r'>=', r'>', r'==', r'!=']
RE_LOGICAL_OPERATOR = [r'&&', r'||']
RE_BITWISE_OPERATOR = [r'&', r',', r',', r'^', r'<<', r'>>', r'~']  # done
RE_Special_Characters = [r'[', r']', r'{', r'}', r'.', r'\"']
RE_lPAREN = r'('
RE_rPAREN = r')'
RE_lBRACKET = r'{'
RE_rBRACKETS = r'}'
RE_number = r'\d+'
RE_Identifiers = r'^[a-zA-Z_]+[a-zA-Z0-9_]*'
RE_Headers = r'[a-zA-Z]+\.[h]'
RE_Function = r'[a-zA-Z_][a-zA-Z0-9_]*\('
# -----------------------------------------------------------------------------------------------------------------
header_files_list = []

define_list = []
define_dictionary: dict[str, int] = {}

typedef_dictionary: dict[str, int] = {}
typedef_list = []


class Define:
    def __init__(self, name: str, expression: str, text_pointer: int):
        self.name = name
        self.expression = expression
        self.text_pointer = text_pointer

    def __str__(self):
        return f' name : {self.name} | expression : {self.expression}'


class Token:
    def __int__(self, id: str, name: str):
        self.id = id
        self.name = name

    def __str__(self):
        return f' id | {self.id} name | {self.name}'


class Typedef:
    def __init__(self, name: str, term: str, text_pointer: int):
        self.name = name
        self.term = term
        self.text_pointer = text_pointer

    def __str__(self):
        return f' name:  {self.name} | term: {self.term}'


class Include:
    def __init__(self, name: str, visited: bool):
        self.name = name
        self.visited = visited

    def __str__(self):
        return f'name: {self.name}  | visited: {self.visited}'
