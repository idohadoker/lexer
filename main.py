from lexer import *


def main():
    filename = input('enter file\n')
    result = search_file(os.getcwd(), filename)
    header_list = search_for_includes(result)
    header_list.insert(0, result)
    header_list = remove_duplicates(header_list)
    tokens_tuple = lex(header_list)

if __name__ == '__main__':
    main()
