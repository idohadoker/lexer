from utills import *


def lex_define(define_line: list[str], text_pointer: int):
    if len(define_line) < 2:
        return
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
            else:
                math_expression += str(math[i]) + ' '
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
        if len(line) == 0:
            text_pointer += 1
            continue
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
    separate = [';', '++', '--', ',', '*', '{', '}']
    text = file.read().split('\n')
    for i in range(len(text)):
        for sep in separate:
            text[i] = text[i].replace(sep, f' {sep} ')
    return text


def find_library_includes(file: str) -> list[str]:
    with open(file, 'r') as f:
        contents = f.read()

    library_includes = re.findall(r'#include *"([^"]+)"', contents)
    return library_includes


# This function searches for all includes in the given file, and then recursively
# searches for includes in those files, until it reaches a file with no includes
def search_for_includes(file: str) -> list[str]:
    # Find all library includes in the file
    includes = find_library_includes(file)
    # For each include, search for includes in that file
    for include in includes:
        result = search_file(os.getcwd(), include)
        header_files_list.append(result)
        search_for_includes(result)
    return header_files_list


def get_code_file(cfile: str):
    pattern = re.compile(r'([^/]+)$')
    match = pattern.search(cfile)
    return match.group(1)


def remove_duplicates(header_files_list: list[str]) -> list[Include]:
    header_files_list = header_files_list[::-1]
    new_list = []
    for i in range(len(header_files_list) - 1):
        if header_files_list[i] not in new_list:
            code_result = search_file(os.getcwd(), get_code_file(header_files_list[i].replace('.h', '.c')))
            new_list.append(Include(header_files_list[i], code_result))
    new_list.append(Include('none', header_files_list[len(header_files_list) - 1]))
    return new_list


def lex(header_files_list: list[Include]):
    header_pointer = 0
    while header_pointer <= len(header_files_list) - 1:
        if header_files_list[header_pointer].header != 'none':
            file = openfile(header_files_list[header_pointer].header)
            text = get_text(file)
            text = find_replace_typedef_define(text)
            close_file(file)
        # first we check the header file than we check the c file
        file = openfile(header_files_list[header_pointer].code)
        text = get_text(file)
        text = find_replace_typedef_define(text)
        find_functions(text, header_files_list[header_pointer].code)
        close_file(file)
        header_pointer += 1


def find_functions(text: list[str], file: str):
    text_pointer = 0
    while text_pointer < len(text):
        line = text[text_pointer]
        line = line.split()
        # if line[0] in RE_VARIABLES_TYPE + r'static' + struct_list:
        if isfunction(line, text, text_pointer):
            text_pointer = extract_function(text, text_pointer, file)
        text_pointer += 1


def isfunction(function_line: list[str], text: list[str], text_pointer: int) -> bool:
    i = 0
    while i < len(function_line):
        if re.match(RE_Function, function_line[i]):
            if function_line[len(function_line) - 1] == RE_lBRACKET or text[text_pointer + 1].__contains__(RE_lBRACKET):
                return True
        i += 1
    return False


# gets the start line of function, ending line of function,return value,name and initialized variables
def extract_function(function_text: list[str], text_pointer: int, file: str) -> int:
    start_pointer = text_pointer
    function_identifiers_list = []
    name = ''
    return_value = ''
    bracket = 1
    while text_pointer < len(function_text) and bracket:
        separate = ['(', ')']
        for sep in separate:
            function_text[text_pointer] = function_text[text_pointer].replace(sep, f' {sep} ')
        line = function_text[text_pointer]
        line = line.split()
        i = 0
        while i < len(line):
            if text_pointer == start_pointer:
                return_value = get_return_value(line)
                i = len(return_value.split(' '))
                function_identifiers_list = get_variables(line[i + 2:])
                name = line[i]
                break
            match line[i]:
                case r'{':
                    bracket += 1
                case r'}':
                    bracket -= 1
            i += 1
        text_pointer += 1
    function_list.append(Function(name, start_pointer + 1, text_pointer, return_value, function_identifiers_list, file))
    return text_pointer


def get_return_value(line: list[str]) -> str:
    return_value = ''
    if line[0] != r'static':
        return_value += line[0]
    i = 1
    while i < len(line) and not line[i + 1] == RE_lPAREN:
        return_value += ' ' + line[i]
        i += 1
    return return_value


def get_variables(variables_line: list[str]) -> list[Variable]:
    variables_list = []
    i = 0
    while i < len(variables_line) and variables_line[i] != RE_rPAREN:
        modifier = ''
        type = ''
        while i < len(variables_line) - 2 and variables_line[i + 1] != r',' and variables_line[i + 1] != RE_rPAREN:
            if variables_line[i] in RE_MODIFIER:
                modifier += variables_line[i] + ' '
            else:
                type += variables_line[i] + ' '
            i += 1

        if not [variable for variable in variables_list if variable.identifier == variables_line[i]]:
            if modifier == '':
                modifier = 'none'
            variables_list.append(Variable(variables_line[i], type, modifier))
        i += 2
    return variables_list


def search_file(folder: str, filename: str) -> str:
    for item in os.listdir(folder):
        item_path = os.path.join(folder, item)
        if os.path.isfile(item_path) and item == filename:
            return item_path
        elif os.path.isdir(item_path):
            result = search_file(item_path, filename)
            if result is not None:
                return result
