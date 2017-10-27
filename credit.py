import cs50
import math

def main():
    print("Input credit card number:", end="")
    ccno = cs50.get_int()

    if checkno(ccno) == True:
        print(getcompany(ccno))
    else:
        print("INVALID")

def checkno(ccno):
    one = 0
    two = 0
    count = 0
    alternate = True

    while ccno > 0:

        if alternate == True:
            #add the first digit to the one variable
            one += math.floor(ccno % 10)

        else:
            #take the first digit from the right, multiply it by two
            tempmod = math.floor((ccno % 10) * 2)

            #if it is greater than 9, we need to break it down further.  If not, add it directly
            if tempmod > 9:
                two += math.floor(tempmod % 10)
                two += math.floor(tempmod / 10)
            else:
                two += tempmod

        ccno = math.floor(ccno/10)  #remove digit from the ccno
        count += 1  #increase the count
        alternate = not alternate  #flip the alternate switch

    if count == 15 or count == 16 or count == 13:
        #valid number of digits, finish the sum

        if (one + two) % 10 == 0:
            return True
        else:
            return False

    else:
        #incorrect number of digits, invalid
        return False

def getcompany(ccno):
    while ccno > 99:
        ccno = math.floor(ccno/10)

    if ccno == 34 or ccno == 37:
        return "AMEX"
    elif ccno >= 51 and ccno <= 55:
        return "MASTERCARD"
    else:
        ccno = math.floor(ccno/10)
        if ccno == 4:
            return "VISA"
        else:
            return "INVALID"

if __name__ == "__main__":
    main()