def nmea_checksum(nmea_sentence: str):
    # Remove the starting '$' and the ending '*' and checksum if present
    if nmea_sentence.startswith('$'):
        nmea_sentence = nmea_sentence[1:]
    if '*' in nmea_sentence:
        nmea_sentence = nmea_sentence.split('*')[0]

    # Initialize checksum
    checksum = 0

    # XOR each character in the sentence to calculate the checksum
    for char in nmea_sentence:
        checksum ^= ord(char)

    # Return the checksum as a two-digit hexadecimal string
    return f"{checksum:02X}"

if __name__ == "__main__":
    while True:
        sentence = input("Type command here: ")
        print(nmea_checksum(sentence))

