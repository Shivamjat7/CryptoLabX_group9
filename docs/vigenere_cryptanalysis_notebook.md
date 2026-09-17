# Vigenère Cipher Cryptanalysis Notebook (Group 9 - Odd)

## 1. Executive Summary
- **Target Ciphertext**: Ciphertext 1 (Odd Group No. 9)
- **Preprocessed Length**: 395 alphabetic characters (`A-Z`)
- **Estimated Key Length**: 14 (Determined by Kasiski Pattern Distances & Index of Coincidence)
- **Recovered Key**: `AMBROISETHOMAS`
- **Re-encryption Verification**: **SUCCESS (100% Exact Match)**

---

## 2. Kasiski Pattern Examination & Distance Analysis

### Repeated Patterns and Distance Gaps

| Repeated Pattern | Length | Positions in Ciphertext | Distance Gap | Factors ($\le 16$) |
|:----------------|:------:|:-----------------------|:------------:|:-------------------|
| `YSZW`          | 4      | [17, 353]              | 336          | 2, 3, 4, 6, 7, 8, 12, 14, 16 |
| `SZWX`          | 4      | [18, 354]              | 336          | 2, 3, 4, 6, 7, 8, 12, 14, 16 |
| `ZWXA`          | 4      | [19, 355]              | 336          | 2, 3, 4, 6, 7, 8, 12, 14, 16 |
| `HQSV`          | 4      | [84, 294]              | 210          | 2, 3, 5, 6, 7, 10, 14, 15 |
| `MJEG`          | 4      | [103, 215]             | 112          | 2, 4, 7, 8, 14, 16 |
| `OSDE`          | 4      | [121, 303]             | 182          | 2, 7, 13, 14 |
| `AZE`           | 3      | [154, 168]             | 14           | 2, 7, 14 |
| `SZO`           | 3      | [264, 362]             | 98           | 2, 7, 14 |
| `ELH`           | 3      | [292, 306]             | 14           | 2, 7, 14 |

### Distance Factor Occurrence Frequency

| Factor | Occurrence Frequency | Candidate Multiple |
|:------:|:--------------------:|:-------------------|
| **2**  | 24 times             | $2 \times 7 = 14$  |
| **7**  | 22 times             | $7 \times 2 = 14$  |
| **14** | **22 times**         | **Key Length 14**  |
| **3**  | 16 times             | Factor of 6, 12    |
| **6**  | 15 times             | Sub-multiple       |
| **4**  | 12 times             | Sub-multiple       |

---

## 3. Index of Coincidence (IC) Analysis

Standard Index of Coincidence for English text is $\approx 0.0667$. Random ciphertext exhibits $IC \approx 0.0385$.

| Candidate Key Length ($K$) | Average Index of Coincidence ($IC$) | Significance / Evaluation |
|:--------------------------:|:----------------------------------:|:-------------------------|
| 1                          | 0.0426                             | Polyalphabetic noise     |
| 2                          | 0.0448                             | Low correlation          |
| 3                          | 0.0430                             | Low correlation          |
| 4                          | 0.0429                             | Low correlation          |
| 5                          | 0.0416                             | Low correlation          |
| 6                          | 0.0441                             | Low correlation          |
| 7                          | 0.0508                             | Sub-harmonic spike       |
| 8                          | 0.0435                             | Low correlation          |
| 9                          | 0.0410                             | Low correlation          |
| 10                         | 0.0443                             | Low correlation          |
| 11                         | 0.0427                             | Low correlation          |
| 12                         | 0.0412                             | Low correlation          |
| 13                         | 0.0403                             | Low correlation          |
| **14**                     | **0.0644**                         | **Strong Peak (Matches English IC ~0.065)** |
| 15                         | 0.0407                             | Low correlation          |

---

## 4. Group-by-Group Frequency Analysis & Shift Estimation ($K=14$)

| Group | Stream Size | Most Frequent Cipher Characters | Inferred Shift | Key Letter |
|:-----:|:-----------:|:-------------------------------|:--------------:|:----------:|
| 1     | 29          | T (13.8%), A (10.3%), L (10.3%) | 0              | **A**      |
| 2     | 29          | Q (24.1%), D (10.3%), A (10.3%) | 12             | **M**      |
| 3     | 29          | F (17.2%), S (10.3%), U (10.3%) | 1              | **B**      |
| 4     | 28          | V (17.9%), E (14.3%), J (10.7%) | 17             | **R**      |
| 5     | 28          | S (10.7%), O (10.7%), V (10.7%) | 14             | **O**      |
| 6     | 28          | M (25.0%), B (14.3%), Z (14.3%) | 8              | **I**      |
| 7     | 28          | W (17.9%), J (14.3%), F (7.1%)  | 18             | **S**      |
| 8     | 28          | I (14.3%), X (14.3%), S (10.7%) | 4              | **E**      |
| 9     | 28          | K (17.9%), E (14.3%), A (10.7%) | 19             | **T**      |
| 10    | 28          | L (17.9%), V (14.3%), Z (10.7%) | 7              | **H**      |
| 11    | 28          | C (14.3%), S (14.3%), W (10.7%) | 14             | **O**      |
| 12    | 28          | Q (17.9%), U (14.3%), D (10.7%) | 12             | **M**      |
| 13    | 28          | E (21.4%), T (14.3%), S (10.7%) | 0              | **A**      |
| 14    | 28          | W (14.3%), Z (10.7%), S (10.7%) | 18             | **S**      |

**Recovered Key**: `AMBROISETHOMAS`

---

## 5. Recovered Plaintext & Verification

### Decrypted Plaintext
> "DO YOU KNOW THE LAND WHERE THE ORANGE TREE BLOSSOMS, THE COUNTRY OF GOLDEN FRUITS AND MARVELOUS ROSES, WHERE THE BREEZE IS SOFTER AND BIRDS LIGHTER, WHERE BEES GATHER POLLEN IN EVERY SEASON AND WHERE SHINES AND SMILES LIKE A GIFT FROM GOD AN ETERNAL SPRINGTIME UNDER AN EVER BLUE SKY? ALAS, BUT I CANNOT FOLLOW YOU TO THAT HAPPY SHORE FROM WHICH FATE HAS EXILED ME. THERE IT IS, THERE THAT I SHOULD LIKE TO LIVE, TO LOVE, TO LOVE AND TO DIE! IT IS THERE THAT I SHOULD LIKE TO LIVE, IT IS THERE, YES, THERE!"

### Re-encryption Verification Test
- **Input Plaintext**: Recovered text string
- **Encryption Key**: `AMBROISETHOMAS`
- **Output Ciphertext**: Identical match to original input Ciphertext 1.
- **Verification Status**: **100% MATCH VERIFIED**
