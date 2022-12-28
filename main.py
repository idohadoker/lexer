from lexer import *


def main():
    filename = input('enter file\n')
    result = search_file(os.getcwd(), filename)
    header_list = search_for_includes(result)
    header_list.insert(0, result)
    header_list = convert_to_include(header_list)
    tokens_tuple = lex(header_list)
    for token in tokens_tuple:
        print(token)
    print(len(tokens_tuple))


if __name__ == '__main__':
    main()
