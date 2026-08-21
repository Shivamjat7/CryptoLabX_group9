from pathlib import Path

from shift_cipher import encrypt
from brute_force_dictionary import crack_shift_cipher as dictionary_attack
from chi_square_attack import crack_shift_cipher as chi_square_attack


BASE_DIR = Path(__file__).resolve().parent.parent

DICTIONARY_PATH = (
    BASE_DIR / "dictionary" / "english_words.txt"
)


def run_test_case(
    test_case: int,
    plaintext: str,
    actual_key: int
):
    """
    Encrypt plaintext and run both cryptanalysis attacks.
    """

    ciphertext = encrypt(plaintext, actual_key)

    print("\n" + "=" * 70)
    print(f"TEST CASE {test_case}")
    print("=" * 70)

    print(f"Actual Key : {actual_key}")
    print(f"Plaintext  : {plaintext}")
    print(f"Ciphertext : {ciphertext}")

    # Dictionary attack
    dictionary_key, dictionary_plaintext, dictionary_score = (
        dictionary_attack(
            ciphertext,
            str(DICTIONARY_PATH)
        )
    )

    # Chi-Square attack
    chi_key, chi_plaintext, chi_score = (
        chi_square_attack(ciphertext)
    )

    print("\n" + "-" * 70)
    print("FINAL COMPARISON")
    print("-" * 70)

    print(f"Actual Key       : {actual_key}")

    print(
        f"Dictionary Key   : {dictionary_key} "
        f"({'Correct' if dictionary_key == actual_key else 'Wrong'})"
    )

    print(
        f"Chi-Square Key   : {chi_key} "
        f"({'Correct' if chi_key == actual_key else 'Wrong'})"
    )

    print(f"Dictionary Score  : {dictionary_score}")
    print(f"Chi-Square Score  : {chi_score:.2f}")

    return {
        "test_case": test_case,
        "actual_key": actual_key,
        "dictionary_key": dictionary_key,
        "chi_square_key": chi_key,
        "dictionary_correct": dictionary_key == actual_key,
        "chi_square_correct": chi_key == actual_key,
    }


def main():

    test_cases = [
        (
            1,
            "HELLO WORLD",
            3
        ),
        (
            2,
            "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG",
            7
        ),
        (
            3,
            "THIS IS A SHIFT CIPHER TEST",
            13
        ),
        (
            4,
            "CRYPTOGRAPHY IS IMPORTANT FOR COMPUTER SECURITY",
            19
        ),
        (
            5,
            "THE STUDENT IS LEARNING COMPUTER SCIENCE",
            5
        ),
    ]

    results = []

    for test_case, plaintext, key in test_cases:

        result = run_test_case(
            test_case,
            plaintext,
            key
        )

        results.append(result)

    print("\n\n")
    print("=" * 90)
    print("OVERALL RESULTS")
    print("=" * 90)

    print(
        f"{'Test':<8}"
        f"{'Actual':<10}"
        f"{'Dictionary':<15}"
        f"{'Chi-Square':<15}"
        f"{'Dict Correct':<15}"
        f"{'Chi Correct':<15}"
    )

    print("-" * 90)

    for result in results:

        print(
            f"{result['test_case']:<8}"
            f"{result['actual_key']:<10}"
            f"{result['dictionary_key']:<15}"
            f"{result['chi_square_key']:<15}"
            f"{str(result['dictionary_correct']):<15}"
            f"{str(result['chi_square_correct']):<15}"
        )


if __name__ == "__main__":
    main()