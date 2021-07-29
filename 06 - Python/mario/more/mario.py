MIN_HEIGHT = 1
MAX_HEIGHT = 8


def main():

    # ask user height for hieght of mario pyramid
    while True:
        try:
            height = int(input("Please enter height of Pyramid:  "))
            if height in range(MIN_HEIGHT, MAX_HEIGHT + 1):
                break
            print("Valid heights are in the range 1 to 8, inclusive.")
            continue
        except ValueError:
            print("Make sure to input an integer.")
            continue
        except EOFError:
            return


    # print pyramid
    print_pyramid(height)


def print_pyramid(height, current_row = 1):

    # base case: exit if all rows have been printed, i.e current row exceeds height
    if height < current_row:
        return

    ###---recursively print each row--###
    # print left white space (height - current_row number of empty spaces)
    for i in range(height - current_row):
        print(' ', end='')

    # print left triangle of hashses (current_row number of hashes)
    for i in range(current_row):
        print('#', end='')

    # print middle spaces (i.e. two white spaces)
    print('  ', end='')

    # print right triangle of hashes (current_row number of hashes) and complete row
    for i in range(current_row):
        print('#', end='')
    print()

    # move on to next row
    print_pyramid(height, current_row + 1)


if __name__ == "__main__":
    main()
