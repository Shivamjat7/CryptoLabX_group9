from collections import Counter
from shift_cipher import decrypt


# English letter frequencies in percentage
ENGLISH_FREQUENCIES = {
    "a": 8.167,
    "b": 1.492,
    "c": 2.782,
    "d": 4.253,
    "e": 12.702,
    "f": 2.228,
    "g": 2.015,
    "h": 6.094,
    "i": 6.966,
    "j": 0.153,
    "k": 0.772,
    "l": 4.025,
    "m": 2.406,
    "n": 6.749,
    "o": 7.507,
    "p": 1.929,
    "q": 0.095,
    "r": 5.987,
    "s": 6.327,
    "t": 9.056,
    "u": 2.758,
    "v": 0.978,
    "w": 2.360,
    "x": 0.150,
    "y": 1.974,
    "z": 0.074,
}


def calculate_chi_square(text: str) -> float:
    """
    Calculate the Chi-Square statistic between
    the text's letter frequencies and English frequencies.
    """

    letters = [
        char.lower()
        for char in text
        if char.isalpha()
    ]

    total_letters = len(letters)

    if total_letters == 0:
        return float("inf")

    counts = Counter(letters)

    chi_square = 0.0

    for letter in "abcdefghijklmnopqrstuvwxyz":
        observed = counts.get(letter, 0)

        expected = (
            ENGLISH_FREQUENCIES[letter] / 100
        ) * total_letters

        if expected > 0:
            chi_square += (
                (observed - expected) ** 2
            ) / expected

    return chi_square


def crack_shift_cipher(ciphertext: str) -> tuple[int, str, float]:
    """
    Try all 26 keys and select the key
    with the lowest Chi-Square value.
    """

    best_key = 0
    best_plaintext = ""
    best_score = float("inf")

    for key in range(26):

        plaintext = decrypt(ciphertext, key)

        score = calculate_chi_square(plaintext)

        print(
            f"Key: {key:2d} | "
            f"Chi-Square: {score:10.2f} | "
            f"Plaintext: {plaintext}"
        )

        if score < best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext

    return best_key, best_plaintext, best_score


if __name__ == "__main__":

    ciphertext = (
        "KHOOR ZRUOG WKLV LV D VKLIW FLSKHU"
    )

    key, plaintext, score = crack_shift_cipher(
        ciphertext
    )

    print("\n--- Chi-Square Result ---")
    print("Predicted Key :", key)
    print("Plaintext     :", plaintext)
    print("Chi-Square    :", round(score, 2))