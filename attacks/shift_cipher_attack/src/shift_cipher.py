def encrypt(text: str, key: int) -> str:
    """
    Encrypt text using a Shift Cipher.

    key: number of positions to shift
    """

    result = ""

    for char in text:
        if char.isupper():
            result += chr((ord(char) - ord('A') + key) % 26 + ord('A'))

        elif char.islower():
            result += chr((ord(char) - ord('a') + key) % 26 + ord('a'))

        else:
            result += char

    return result


def decrypt(text: str, key: int) -> str:
    """
    Decrypt text using a Shift Cipher.
    """

    return encrypt(text, -key)


if __name__ == "__main__":
    plaintext = "HELLO WORLD"
    key = 3

    ciphertext = encrypt(plaintext, key)
    decrypted = decrypt(ciphertext, key)

    print("Plaintext :", plaintext)
    print("Key       :", key)
    print("Ciphertext:", ciphertext)
    print("Decrypted :", decrypted)