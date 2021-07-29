def main():
    # promt user for text:
    while True:
        try:
            users_text = input('Text: ')
            break
        except EOFError:
            print()
            pass

    # count number of letters, words, and sentence:
    word_count = len(users_text.split())
    letter_count = len([1 for char in users_text if char.isalpha()])
    sentence_count = len([ 1 for char in users_text if char == '.' or char == '!' or char == '?'])

    # calculate coleman_liau_index:
    L = (letter_count * 100) / word_count
    S = (sentence_count * 100) / word_count
    coleman_liau_index = round(0.0588 * L - 0.296 * S - 15.8)

    # print grade to console:
    if (coleman_liau_index > 1 and coleman_liau_index < 16):
        print(f'Grade {coleman_liau_index}')
    elif (coleman_liau_index < 1):
        print('Before Grade 1')
    else:
        print('Grade 16+')

    return



if __name__ == '__main__':
    main()
