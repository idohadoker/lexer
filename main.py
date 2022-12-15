from lexer import *


def main():
    file = input('enter file\n')
    header_files_list = search_for_includes(file)
    print(header_files_list)


# header_files_list = remove_duplicates(header_files_list)


#  close_file(file)


if __name__ == '__main__':
    main()
