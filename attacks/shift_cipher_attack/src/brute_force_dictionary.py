from pathlib import Path
from shift_cipher import decrypt


def load_dictionary(dictionary_path: str) -> set[str]:
    """
    Load English words from the dictionary file.
    """

    words = set()

    with open(dictionary_path, "r", encoding="utf-8") as file:
        for line in file:
            word = line.strip().lower()

            if word:
                words.add(word)

    return words


def dictionary_score(text: str, dictionary: set[str]) -> int:
    """
    Count how many words in the text exist in the dictionary.
    """

    words = text.lower().split()

    score = 0

    for word in words:
        # Remove punctuation around words
        clean_word = "".join(
            char for char in word
            if char.isalpha()
        )

        if clean_word in dictionary:
            score += 1

    return score


def crack_shift_cipher(
    ciphertext: str,
    dictionary_path: str
) -> tuple[int, str, int]:
    """
    Try all 26 Shift Cipher keys and return
    the key with the highest dictionary score.

    Returns:
        best_key
        best_plaintext
        best_score
    """

    dictionary = load_dictionary(dictionary_path)

    best_key = 0
    best_plaintext = ""
    best_score = -1

    for key in range(26):
        plaintext = decrypt(ciphertext, key)

        score = dictionary_score(
            plaintext,
            dictionary
        )

        print(
            f"Key: {key:2d} | "
            f"Score: {score:2d} | "
            f"Plaintext: {plaintext}"
        )

        if score > best_score:
            best_score = score
            best_key = key
            best_plaintext = plaintext

    return best_key, best_plaintext, best_score


if __name__ == "__main__":

    ciphertext = "KHOOR ZRUOG WKLV LV D VKLIW FLSKHU"

    dictionary_path = (
        Path(__file__).parent.parent
        / "dictionary"
        / "english_words.txt"
    )

    key, plaintext, score = crack_shift_cipher(
        ciphertext,
        str(dictionary_path)
    )

    print("\n--- Result ---")
    print("Predicted Key :", key)
    print("Plaintext     :", plaintext)
    print("Dictionary Score:", score)