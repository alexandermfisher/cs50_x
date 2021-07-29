

AMEX_LENGTH = 15
MC_LENGTH = 16
VISA_LENGTH_1 = 13
VISA_LENGTH_2 = 16
AMEX_34 = 34
AMEX_37 = 37
MC_51 = 51
MC_52 = 52
MC_53 = 53
MC_54 = 54
MC_55 = 55
VISA = 4


def main():

    # get users card number
    while True:
        try:
            card_number = input("Number:  ")
            card_number = card_number.strip()
            if int(card_number) > 0:
                break
        except ValueError:
            pass
        except EOFError:
            print()
            pass

    # if card is valid check to see if it is either American Express, Mastercard, or Visa.
    if card_is_valid(card_number):

        # store fisrt two digits in
        n = int(card_number[0:2])

        # check if card is American Express:
        if ((len(card_number) == AMEX_LENGTH) and (n == AMEX_34 or n == AMEX_37)):
            print("AMEX")
        # check if card is MASTERCARD:
        elif ((len(card_number) == MC_LENGTH) and (n == MC_51 or n == MC_52 or n == MC_53 or n == MC_54 or n == MC_55)):
            print("MASTERCARD")
        # check if card is visa:
        elif (len(card_number) == VISA_LENGTH_1 or len(card_number) == VISA_LENGTH_2) and (int(card_number[0]) == VISA):
            print("VISA")
        # if no match is found card number is invalid:
        else:
            print("INVALID")

    else:
        print("INVALID")





def card_is_valid(number):
    """
    card_is_valid() performs Luhns algorithm to check validity of a card number.
    One argument  is taken, i.e. a card number (int)
    Returns: True if valid, else False.
    """

    # copy number to local variable card_number and initialise luhns sum to 0.
    card_number = number
    luhns_sum = 0

    # if num. of digits is even pass else and first (most left most) digit to sum
    # and slice to leave the remaining digits
    if len(card_number) % 2 == 0:
        pass
    else:
        luhns_sum += int(card_number[0])
        card_number = card_number[1:]

    # perform lunh algorithm steps (on a card number of an even length (i.e. num. of digits)
    for i in range(int(len(card_number) / 2)):
            if int(card_number[2*i]) * 2 > 9:
                luhns_sum += 1
                luhns_sum += int(card_number[2*i]) * 2 - 10
            else:
                luhns_sum += int(card_number[2*i]) * 2
            luhns_sum += int(card_number[2*i+1])

    # if sum divisible by 10 with no remainder i.e. ones digit is 0 card is valid
    # and return true else return false.
    if luhns_sum % 10 == 0:
        return True

    return False






if __name__ == '__main__':
    main()
