from pathlib import Path

from shift_cipher import encrypt
from brute_force_dictionary import (
    crack_shift_cipher as dictionary_attack
)
from chi_square_attack import (
    crack_shift_cipher as chi_square_attack
)


BASE_DIR = Path(__file__).resolve().parent.parent

DICTIONARY_PATH = (
    BASE_DIR / "dictionary" / "english_words.txt"
)

TESTCASE_PATH = (
    BASE_DIR / "testcases" / "testcases.txt"
)


def load_test_cases():
    """
    Load test cases from testcases.txt.
    """

    test_cases = []

    with open(TESTCASE_PATH, "r", encoding="utf-8") as file:

        for line in file:

            line = line.strip()

            if not line or line.startswith("#"):
                continue

            parts = line.split("|", 2)

            if len(parts) != 3:
                continue

            test_number = int(parts[0])
            actual_key = int(parts[1])
            plaintext = parts[2]

            test_cases.append(
                (
                    test_number,
                    actual_key,
                    plaintext
                )
            )

    return test_cases


def run_test_case(
    test_case,
    plaintext,
    actual_key
):
    """
    Encrypt plaintext and run both attacks.
    """

    ciphertext = encrypt(
        plaintext,
        actual_key
    )

    print("\n" + "=" * 80)
    print(f"TEST CASE {test_case}")
    print("=" * 80)

    print(f"Actual Key : {actual_key}")
    print(f"Plaintext  : {plaintext}")
    print(f"Ciphertext : {ciphertext}")

    print("\nRunning Dictionary Attack...")

    dictionary_key, dictionary_plaintext, dictionary_score = (
        dictionary_attack(
            ciphertext,
            str(DICTIONARY_PATH)
        )
    )

    print("\nRunning Chi-Square Attack...")

    chi_key, chi_plaintext, chi_score = (
        chi_square_attack(ciphertext)
    )

    dictionary_correct = (
        dictionary_key == actual_key
    )

    chi_square_correct = (
        chi_key == actual_key
    )

    print("\n" + "-" * 80)
    print("FINAL RESULT")
    print("-" * 80)

    print(f"Actual Key          : {actual_key}")
    print(f"Dictionary Key      : {dictionary_key}")
    print(f"Dictionary Plaintext: {dictionary_plaintext}")
    print(f"Dictionary Score    : {dictionary_score}")

    print(f"\nChi-Square Key      : {chi_key}")
    print(f"Chi-Square Plaintext: {chi_plaintext}")
    print(f"Chi-Square Score    : {chi_score:.2f}")

    print(
        f"\nDictionary Correct  : "
        f"{'YES' if dictionary_correct else 'NO'}"
    )

    print(
        f"Chi-Square Correct  : "
        f"{'YES' if chi_square_correct else 'NO'}"
    )

    return {
        "test_case": test_case,
        "actual_key": actual_key,
        "dictionary_key": dictionary_key,
        "chi_square_key": chi_key,
        "dictionary_correct": dictionary_correct,
        "chi_square_correct": chi_square_correct
    }


def main():

    test_cases = load_test_cases()

    results = []

    print("=" * 80)
    print("SHIFT CIPHER CRYPTANALYSIS")
    print("Brute Force + Dictionary Scoring + Chi-Square")
    print("=" * 80)

    for test_number, key, plaintext in test_cases:

        result = run_test_case(
            test_number,
            plaintext,
            key
        )

        results.append(result)

    print("\n\n")
    print("=" * 95)
    print("OVERALL RESULTS")
    print("=" * 95)

    print(
        f"{'Test':<8}"
        f"{'Actual':<10}"
        f"{'Dictionary':<15}"
        f"{'Chi-Square':<15}"
        f"{'Dict Correct':<15}"
        f"{'Chi Correct':<15}"
    )

    print("-" * 95)

    for result in results:

        print(
            f"{result['test_case']:<8}"
            f"{result['actual_key']:<10}"
            f"{result['dictionary_key']:<15}"
            f"{result['chi_square_key']:<15}"
            f"{'YES' if result['dictionary_correct'] else 'NO':<15}"
            f"{'YES' if result['chi_square_correct'] else 'NO':<15}"
        )


if __name__ == "__main__":
    main()