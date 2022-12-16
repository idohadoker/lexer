from lexer import *


def main():
    file = input('enter file\n')
    header_files_list = search_for_includes(file)
    header_files_list.insert(0, file)
    header_files_list = remove_duplicates(header_files_list)
    lex(header_files_list)


# header_files_list = remove_duplicates(header_files_list)


#  close_file(file)


if __name__ == '__main__':
    main()
