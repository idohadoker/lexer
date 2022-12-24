from lexer import *


def main():
    filename = input('enter file\n')
    result = search_file(os.getcwd(), filename)
    header_files_list = search_for_includes(result)
    header_files_list.insert(0, result)
    header_files_list = remove_duplicates(header_files_list)
    lex(header_files_list)
    for func in function_list:
        print(func)


if __name__ == '__main__':
    main()
