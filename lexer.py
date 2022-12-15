# variables
import re
from utills import *


def lex_define(define_line: list[str], text_pointer: int):
    if re.match(RE_Function, define_line[0]):
        print('macro not supported')
    else:
        name = define_line[0]
        expression = value_assignment(define_line[1:])
        define_list.append(Define(name, expression, text_pointer))
        define_dictionary[name] = len(define_list) - 1


def find_typedef(typedef: list[str], text_pointer: int):
    i = 0
    term = ''
    while i < len(typedef) - 2:
        term += typedef[i] + ' '
        i += 1
    name = typedef[len(typedef) - 2]  # len -1 is ;
    typedef_list.append(Typedef(name, term, text_pointer))
    typedef_dictionary[name] = len(typedef_list) - 1


def value_assignment(math: list[str]) -> str:
    math_expression = ''
    for x in math:
        math_expression += x + ' '
    math = math_expression.split()
    math_expression = ''
    i = 0
    while i < len(math):
        if math[i] == ',' or math[i] == ';':
            break
        if re.match(RE_Identifiers, math[i]):
            if math[i] in define_dictionary:
                define = define_list[define_dictionary[math[i]]]
                math_expression += str(define.expression) + ' '
                i += 1
            else:
                print(f' {math[i]} not found in dictionary  ')
                exit(1)
        else:
            if math[i] == '^':
                math_expression += '**' + ' '
            else:
                math_expression += math[i] + ' '
            i += 1
    return math_expression


def openfile(path: str):
    f = None
    try:
        f = open(path, "r")
    finally:
        if f is None:
            print(f'Could not open file:  {path}')
            exit(1)
    return f


# changes the name of typedef to term and define before code is being tokenized
def find_replace_typedef_define(text: list[str]) -> list[str]:
    global typedef_list
    text_pointer = 0
    while text_pointer < len(text):
        line = text[text_pointer]
        line = line.split()
        if line[0] == r'typedef':
            if not re.match(r'struct', line[1]):
                find_typedef(line[1:], text_pointer)
        if line[0] == r'#define':
            lex_define(line[1:], text_pointer)
        text_pointer += 1
    text_pointer = 0
    while text_pointer <= len(text) - 1:
        line = text[text_pointer]
        line = line.split()
        i = 0
        while i < len(line) - 1:
            if line[i] in define_dictionary.keys():
                define = define_list[define_dictionary[line[i]]]
                if define.text_pointer != text_pointer:
                    text[text_pointer] = text[text_pointer].replace(line[i], define.expression)
            if line[i] in typedef_dictionary.keys():
                typedef = typedef_list[typedef_dictionary[line[i]]]
                if typedef.text_pointer != text_pointer:
                    text[text_pointer] = text[text_pointer].replace(line[i], typedef.term)
            i += 1
        text_pointer += 1
    return text


def close_file(file):
    file.close()


def get_text(file) -> list[str]:
    separate = [';', '++', '--', ',', '*', '{', '}', '(', ')']
    text = file.read().split('\n')
    text = [line for line in text if line != '']
    for i in range(len(text)):
        for sep in separate:
            text[i] = text[i].replace(sep, f' {sep} ')
    return text


def find_library_includes(file: str) -> list[str]:
    # Read the contents of the file
    with open(file, 'r') as f:
        contents = f.read()

    # Use a regular expression to find all include statements that start with "<"
    # (indicating that they are library includes)
    library_includes = re.findall(r'#include *"([^"]+)"', contents)
    return library_includes


# This function searches for all includes in the given file, and then recursively
# searches for includes in those files, until it reaches a file with no includes
def search_for_includes(file: str) -> list[str]:
    # Find all library includes in the file
    includes = find_library_includes(file)
    # For each include, search for includes in that file
    i = 0
    for include in includes:
        header_files_list.append(include)
        search_for_includes(include)
    return header_files_list


def remove_duplicates(header_files_list: list[str]) -> list[str]:
    header_files_list = header_files_list[::-1]
    new_list = []
    for header in header_files_list:
        if header not in new_list:
            new_list.append(header)
    return new_list


def lex(header_files_list: list[str]):
    header_pointer = 0
    while header_pointer < len(header_files_list):
        print(header_files_list[header_pointer])
        header_pointer += 1
