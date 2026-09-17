# Cryptanalysis Notebook & Justification Log

## Overview
- **Lab / Task**: Monoalphabetic Substitution Cipher & Cryptanalysis
- **Group Number**: 9
- **Selected Text Source**: Katz & Lindell *Introduction to Modern Cryptography*, Page 39 (`Group 9 + 30`)
- **Ciphertext Length**: 2,996 characters (428 words)
- **Alphabet**: 26 Standard English Letters (A–Z)

---

## 1. Letter Frequency Analysis Results

### Ciphertext Letter Counts & Percentages (Descending Order)

| Rank | Ciphertext Letter | Count | Frequency (%) | Standard English Ref Letter | Standard Eng Freq (%) | Notes / Observations |
|:----:|:-----------------:|:-----:|:-------------:|:---------------------------:|:---------------------:|:--------------------|
| 1    | **H**             | 300   | **12.63%**    | E                           | 12.70%                | Prime candidate for **e** |
| 2    | **D**             | 211   | **8.88%**     | T                           | 9.06%                 | Prime candidate for **t** |
| 3    | **X**             | 204   | **8.59%**     | A                           | 8.17%                 | High frequency, candidate for **a** |
| 4    | **U**             | 187   | **7.87%**     | O                           | 7.51%                 | Candidate for **n** / **o** |
| 5    | **Z**             | 167   | **7.03%**     | I                           | 6.97%                 | Candidate for **i** |
| 6    | **J**             | 149   | **6.27%**     | N                           | 6.75%                 | Candidate for **o** / **n** |
| 7    | **K**             | 148   | **6.23%**     | S                           | 6.33%                 | Candidate for **s** |
| 8    | **G**             | 121   | **5.09%**     | H                           | 6.09%                 | Candidate for **h** |
| 9    | **M**             | 119   | **5.01%**     | R                           | 5.99%                 | Candidate for **r** |
| 10   | **A**             | 108   | **4.55%**     | D                           | 4.25%                 | Candidate for **d** |
| 11   | **Q**             | 97    | **4.08%**     | L                           | 4.03%                 | Candidate for **l** |
| 12   | **Y**             | 88    | **3.71%**     | C                           | 2.78%                 | Candidate for **c** |
| 13   | **P**             | 83    | **3.49%**     | U                           | 2.76%                 | Candidate for **f** |
| 14   | **W**             | 63    | **2.65%**     | M                           | 2.41%                 | Candidate for **p** |
| 15   | **V**             | 59    | **2.48%**     | W                           | 2.36%                 | Candidate for **k** |
| 16   | **C**             | 54    | **2.27%**     | F                           | 2.23%                 | Candidate for **u** |
| 17   | **S**             | 41    | **1.73%**     | G                           | 2.02%                 | Candidate for **w** |
| 18   | **R**             | 39    | **1.64%**     | Y                           | 1.97%                 | Candidate for **m** |
| 19   | **O**             | 31    | **1.31%**     | P                           | 1.93%                 | Candidate for **g** |
| 20   | **I**             | 29    | **1.22%**     | B                           | 1.49%                 | Candidate for **v** |
| 21   | **F**             | 28    | **1.18%**     | V                           | 0.98%                 | Candidate for **y** |
| 22   | **E**             | 22    | **0.93%**     | K                           | 0.77%                 | Candidate for **q** |
| 23   | **T**             | 15    | **0.63%**     | J                           | 0.15%                 | Candidate for **x** |
| 24   | **N**             | 14    | **0.59%**     | X                           | 0.15%                 | Candidate for **b** |
| 25   | **L**             | 7     | **0.29%**     | Q                           | 0.10%                 | Candidate for **z** |
| 26   | **B**             | 1     | **0.04%**     | Z                           | 0.07%                 | Candidate for **j** |

---

## 2. Word Frequency & Pattern Analysis

### 1-Letter Words
- `X` (Count: 13) $\rightarrow$ Candidate for English one-letter word **'a'**
- `Z` (Count: 2) $\rightarrow$ Candidate for English one-letter word **'i'**

### 2-Letter Words
- `JP` (Count: 16) $\rightarrow$ Candidate for **'of'**
- `ZU` (Count: 11) $\rightarrow$ Candidate for **'in'**
- `DJ` (Count: 9) $\rightarrow$ Candidate for **'to'**
- `ZK` (Count: 8) $\rightarrow$ Candidate for **'is'**
- `XU` (Count: 4) $\rightarrow$ Candidate for **'an'**
- `NF` (Count: 3) $\rightarrow$ Candidate for **'by'**

### 3-Letter Words
- `DGH` (Count: 31) $\rightarrow$ Highest frequency 3-letter word $\rightarrow$ Candidate for **'the'**
- `XUA` (Count: 13) $\rightarrow$ Second most frequent 3-letter word starting with `X` ('a') and `U` ('n') $\rightarrow$ Candidate for **'and'**
- `VHF` (Count: 6) $\rightarrow$ Candidate for **'key'**

### Repeated Double Letter Patterns
- `DD` (Count: 14) $\rightarrow$ Candidate for **'tt'** (found in *letter*, *written*, etc.)
- `HH` (Count: 6) $\rightarrow$ Candidate for **'ee'** (found in *thirteen*, *nineteenth*, *freely*)
- `QQ` (Count: 5) $\rightarrow$ Candidate for **'ll'** (found in *collapses*, *classical*)
- `KK` (Count: 5) $\rightarrow$ Candidate for **'ss'** (found in *possibility*, *Kerckhoffs's*)

---

## 3. Cryptanalytic Decision & Justification Log Table

| Step | Ciphertext Letter(s) | Suspected Plaintext Letter(s) | Clue / Evidence / Pattern Used | Resulting Partial Plaintext Sample | Status |
|:----:|:-------------------:|:----------------------------:|:------------------------------|:----------------------------------|:------|
| 1    | `D`, `G`, `H`       | `t`, `h`, `e`                | `DGH` is most frequent 3-letter word (31 times); `H` is most frequent letter (12.63%). | `DGH` $\rightarrow$ `the`, `dgh` $\rightarrow$ `the` | **Accepted** |
| 2    | `X`, `U`, `A`       | `a`, `n`, `d`                | `XUA` is 2nd top 3-letter word (13 times); `X` is top 1-letter word. `X`='a', `U`='n', `A`='d'. | `XUA` $\rightarrow$ `and`, `X` $\rightarrow$ `a` | **Accepted** |
| 3    | `J`, `P`, `Z`, `K`  | `o`, `f`, `i`, `s`           | Top 2-letter word `JP` (16) $\rightarrow$ `of`; `ZU` (11) $\rightarrow$ `in`; `ZK` (8) $\rightarrow$ `is`. | `JP` $\rightarrow$ `of`, `ZU` $\rightarrow$ `in`, `ZK` $\rightarrow$ `is` | **Accepted** |
| 4    | `V`, `M`, `Y`, `O`  | `k`, `r`, `c`, `g`           | Word pattern `Vhmyvgjppk'k` matches structural pattern of `Kerckhoffs's`. | `Vhmyvgjppk'k` $\rightarrow$ `Kerckhoffs's` | **Accepted** |
| 5    | `F`, `W`, `E`, `C`  | `y`, `p`, `q`, `u`           | Contextual word `ymfwdjomxwgf` $\rightarrow$ `cryptography`; `huymfwdzju` $\rightarrow$ `encryption`. | `cryptography`, `encryption` | **Accepted** |
| 6    | `S`, `R`, `I`, `T`, `L`, `B`, `Q`, `N` | `w`, `m`, `v`, `x`, `z`, `j`, `l`, `b` | Contextual completion of `kfkdhr` $\rightarrow$ `system`, `xaihmkxmf` $\rightarrow$ `adversary`, `pxydjmzxq` $\rightarrow$ `factorial`. | Full readable English text recovered | **Accepted** |

---

## 4. Recovered Substitution Key & Validation

### Recovered Key Mapping

```
Standard Plaintext Alphabet : A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
Recovered Substitution Key  : X N Y A H P O G Z B V Q R U J W E M K D C I S T F L
```

### Key Verification
1. **Direct Key Comparison**: Comparing the recovered substitution key against `classical/key.txt` yields a 100% exact match.
2. **Re-encryption Validation**: Re-encrypting `classical/plaintext.txt` using the recovered key produces `classical/ciphertext.txt` with zero bit differences.
