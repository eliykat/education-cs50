import sys
import cs50

ALPHABET_SIZE = 26

#check we have the right number of arguments
if len(sys.argv) != 2:
    print("Error: you must provide the key as a command line argument.")
    exit(1)

#check key is alphanumeric
if str.isalpha(sys.argv[1]) == False:
    print("Error: key must be alphanumeric")
    exit(2)

#get plaintext
print("plaintext:", end="")
plaintext = cs50.get_string()

print("ciphertext:", end="")

#convert key to numbers
key = []
for c in sys.argv[1]:
    key.append(ord(c.upper()) - ord('A'))

j = 0
key_len = len(key)

#iterate through the plaintext string
for c in plaintext:

    if str.isalpha(c) == True:

        if str.isupper(c) == True:
            print( chr( ((ord(c) - ord('A') + key[j]) % ALPHABET_SIZE) + ord('A') ), end="")

        else:
            print( chr( ((ord(c) - ord('a') + key[j]) % ALPHABET_SIZE) + ord('a') ), end="")

        #increment j, which moves us along the key.  Reset to 0 if about to go out of bounds
        if j + 1 > key_len - 1:
            j = 0
        else:
            j += 1

    else:

        #c is not a letter, just reprint it
        print(c, end="")

print("")