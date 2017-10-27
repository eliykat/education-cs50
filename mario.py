import cs50

def main():
    #get pyramid height
    while True:
        print("Pyramid height:", end="")
        height = cs50.get_int()
        if height < 23 and height > 0:
            break

    #set initial variables
    ihash = 1
    ispace = height - 1

    #make pyramids!
    for i in range(height):
        prchars(ispace, " ")
        prchars(ihash, "#")

        print("  ", end="")

        prchars(ihash, "#")
        print("")

        ispace -= 1
        ihash += 1

def prchars(n, c):
    for i in range(n):
        print(c, end="")

if __name__ == "__main__":
    main()