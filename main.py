from lexer import *


def main():
    file = openfile(input('enter file\n'))
    text = get_text(file)
    lex_include(text)
    text = find_replace_typedef_define(text)
    for header in header_files_list:
        print(f'{header}')
    close_file(file)


if __name__ == '__main__':
    main()
