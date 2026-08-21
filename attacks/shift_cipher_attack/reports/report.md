# Assignment 4: Cryptanalysis of Shift Cipher

## 1. Objective

To perform cryptanalysis of a Shift Cipher using:

- Brute Force
- Dictionary Scoring
- Chi-Square Analysis

---

## 2. Shift Cipher

A Shift Cipher shifts each alphabetic character by a fixed key.

### Encryption

\[
C = (P + K) \mod 26
\]

### Decryption

\[
P = (C - K) \mod 26
\]

Example:

```text
Plaintext  : HELLO
Key        : 3
Ciphertext : KHOOR
```

---

## 3. Cryptanalysis Algorithms

### 3.1 Brute Force

Try all 26 possible keys and decrypt the ciphertext.

```text
For key = 0 to 25:
    Decrypt ciphertext
    Display plaintext
```

Since there are only 26 keys, brute-force attack is very fast.

### 3.2 Dictionary Scoring

For every possible key:

1. Decrypt the ciphertext.
2. Split it into words.
3. Check the words against an English dictionary.
4. Count matching words.
5. Select the key with the highest score.

### 3.3 Chi-Square Analysis

Letter frequencies of the decrypted text are compared with standard English frequencies.

\[
\chi^2 = \sum \frac{(O_i-E_i)^2}{E_i}
\]

The key with the **lowest Chi-Square value** is selected.

---

## 4. Experimental Results

| Test | Actual Key | Dictionary Key | Chi-Square Key | Dictionary Correct | Chi-Square Correct |
|---:|---:|---:|---:|:---:|:---:|
| 1 | 3 | 3 | 3 | YES | YES |
| 2 | 7 | 7 | 7 | YES | YES |
| 3 | 13 | 13 | 13 | YES | YES |
| 4 | 19 | 19 | 19 | YES | YES |
| 5 | 5 | 5 | 5 | YES | YES |
| 6 | 11 | 11 | 11 | YES | YES |
| 7 | 21 | 21 | 21 | YES | YES |
| 8 | 9 | 0 | 9 | NO | YES |
| 9 | 15 | 15 | 15 | YES | YES |
| 10 | 2 | 2 | 2 | YES | YES |

### Accuracy

| Method | Correct | Accuracy |
|---|---:|---:|
| Dictionary Scoring | 9/10 | 90% |
| Chi-Square | 10/10 | 100% |

---

## 5. Comparison

Dictionary scoring correctly predicted **9 out of 10 keys**.

Chi-Square analysis correctly predicted **all 10 keys**.

The only failure occurred in Test Case 8:

```text
Actual Key       : 9
Dictionary Key   : 0
Chi-Square Key   : 9
```

Therefore, Chi-Square performed better in this experiment.

---

## 6. Failure Analysis

Dictionary scoring failed in Test Case 8.

Possible reasons:

- Some correct plaintext words may not be present in the dictionary.
- The dictionary may be too small.
- An incorrect plaintext may contain more dictionary words.
- Dictionary scoring only considers exact word matches.

### Improvement

The attack can be improved by:

- Using a larger dictionary.
- Using word-frequency scoring.
- Using n-gram scoring.
- Combining dictionary and Chi-Square scores.

Chi-Square can also fail for very short ciphertexts because there may not be enough letters for reliable frequency analysis.

---

## 7. Observations

- Shift Cipher has only 26 possible keys.
- Brute-force cryptanalysis is therefore practical.
- Dictionary scoring achieved 90% accuracy.
- Chi-Square achieved 100% accuracy in our experiment.
- Dictionary scoring depends on the quality of the word list.
- Chi-Square works using statistical English letter frequencies.
- Longer ciphertexts generally provide better frequency analysis results.
- Combining different attacks can improve confidence.

---

## 8. Conclusion

The experiment demonstrated that the Shift Cipher is vulnerable to simple cryptanalysis techniques.

Brute force can test all possible keys efficiently. Dictionary scoring identifies likely English plaintext using known words, while Chi-Square analysis uses English letter-frequency statistics.

In our experiment, Dictionary Scoring achieved **90% accuracy**, while Chi-Square achieved **100% accuracy**.

Thus, Chi-Square analysis performed better on the given test cases, while dictionary scoring showed a failure due to its dependence on the dictionary contents.

---

## 9. Repository Structure

```text
CryptoLabX/
└── attacks/
    └── shift_cipher_attack/
        ├── src/
        │   ├── shift_cipher.py
        │   ├── brute_force_dictionary.py
        │   ├── chi_square_attack.py
        │   └── main.py
        ├── dictionary/
        │   └── english_words.txt
        ├── testcases/
        │   └── testcases.txt
        ├── outputs/
        ├── screenshots/
        ├── reports/
        │   └── Assignment_4_Report.pdf
        └── README.md
```

---


