# AES-CBC Padding Oracle Attack & Cryptanalysis Notebook
**Group 9 | CryptoLabX Modern Cryptography Module**

---

## 1. Executive Summary & Overview

This document presents a comprehensive analysis and implementation report for the **AES-CBC Padding Oracle Attack** (Vaudenay's Attack). A padding oracle vulnerability allows an adversary to decrypt arbitrary AES-CBC ciphertexts byte-by-byte without knowing the secret encryption key, relying solely on a side-channel signal indicating whether a submitted ciphertext yields valid **PKCS#7** padding upon decryption.

---

## 2. Fundamental Cryptographic Concepts

### 2.1 AES-CBC (Cipher Block Chaining) Mode
In AES-CBC mode, plaintext $P$ is divided into 128-bit (16-byte) blocks $P_1, P_2, \dots, P_N$.

- **Encryption**:
  $$C_i = E_K(P_i \oplus C_{i-1}) \quad \text{where } C_0 = \text{IV}$$
- **Decryption**:
  $$P_i = D_K(C_i) \oplus C_{i-1} \quad \text{where } C_0 = \text{IV}$$

Let $I_i = D_K(C_i)$ represent the **intermediate decryption state** after block decryption but prior to the XOR operation. Thus:
$$P_i = I_i \oplus C_{i-1}$$

```
CBC Decryption Flow:
  Ciphertext C_i ───► [ AES Decrypt (Key K) ] ───► Intermediate I_i
                                                          │
  Ciphertext C_{i-1} ────────────────────────────────────► XOR
                                                          │
                                                          ▼
                                                    Plaintext P_i
```

Key Observation: Modifying the previous ciphertext block $C_{i-1}$ directly alters the decrypted plaintext $P_i$ of the target block, byte-for-byte, while leaving $I_i$ unchanged.

### 2.2 PKCS#7 Padding
AES requires the plaintext length to be an exact multiple of the block size ($B = 16$ bytes). PKCS#7 appends $N$ bytes, each having the value $N$ (where $1 \le N \le B$):

| Unpadded Tail Length | Added Padding Bytes (Hex) | Padding Length |
|----------------------|--------------------------|----------------|
| 15 bytes             | `01`                     | 1 byte         |
| 14 bytes             | `02 02`                  | 2 bytes        |
| 13 bytes             | `03 03 03`               | 3 bytes        |
| 0 bytes (exact mult)| `10 10 ... 10` (16 times) | 16 bytes       |

### 2.3 Role of the Padding Oracle
A **Padding Oracle** is any application behavior or error response (e.g., HTTP 500 vs 403, error text, or response timing) that reveals whether a decrypted ciphertext contains valid PKCS#7 padding. The oracle acts as a boolean function:
$$\mathcal{O}(IV, C) \in \{\text{True}, \text{False}\}$$

---

## 3. Attack Mechanics & Algorithm

### 3.1 Mathematical Derivation
To recover plaintext block $P_i$, the attacker targets block $C_i$ by manipulating the preceding block $C_{i-1}'$ (or $IV'$ if $i=1$).

When the modified pair $(C_{i-1}', C_i)$ is decrypted by the oracle, the resulting modified plaintext is:
$$P_i' = I_i \oplus C_{i-1}'$$

To recover byte position $pos \in [15, 0]$ (right-to-left):
1. Target padding byte value: $V = 16 - pos$.
2. For all already-recovered positions $j > pos$, set:
   $$C_{i-1}'[j] = I_i[j] \oplus V$$
   This ensures that $P_i'[j] = V$.
3. Iterate guess $g \in [0, 255]$ for position $pos$:
   $$C_{i-1}'[pos] = g$$
4. Submit $(C_{i-1}', C_i)$ to oracle $\mathcal{O}$.
5. When $\mathcal{O}$ returns `True`, $P_i'[pos] == V$, revealing $I_i[pos]$:
   $$I_i[pos] = g \oplus V$$
6. Original plaintext byte is then:
   $$P_i[pos] = I_i[pos] \oplus C_{i-1}[pos]$$

```
Right-to-Left Byte Recovery Example (Targeting position 15, padding 0x01):
  Crafted C_{i-1}'[15] = guess  ──►  Decrypt C_i  ──►  I_i[15] ⊕ guess
  If Oracle == True  ==►  P_i'[15] == 0x01  ==►  I_i[15] = guess ⊕ 0x01
  Original P_i[15] = I_i[15] ⊕ C_{i-1}[15]
```

### 3.2 Mitigation of False Positives
When recovering byte 15 ($V = 0x01$), a guess $g$ might accidentally produce a multi-byte valid padding (e.g., `0x02 0x02`) if the preceding byte $P_i'[14]$ already happens to be `0x02`.
To eliminate false positives:
- Flip a bit in $C_{i-1}'[14]$: `confirmation[14] ^= 1`.
- If the oracle still returns `True`, the padding was genuine `0x01`. If it returns `False`, the match was a false positive due to multi-byte padding, and brute-forcing continues.

---

## 4. Empirical Results & Performance Analysis

### 4.1 Execution Log Summary
- **Original Plaintext**: `"Padding oracle attacks demonstrate why CBC padding errors must not leak information."`
- **Plaintext Size**: 84 bytes (padded to 96 bytes / 6 blocks)
- **Secret Key**: Random 128-bit key (hidden from attack function)
- **IV**: Random 128-bit IV

#### Block-by-Block Recovery Breakdown

| Block # | Plaintext Chunk | Bytes | Oracle Queries | Avg Queries/Byte |
|---------|-----------------|-------|----------------|------------------|
| 1       | `b'Padding oracle a'` | 16 | 1,983 | 123.9 |
| 2       | `b'ttacks demonstra'` | 16 | 1,961 | 122.6 |
| 3       | `b'te why CBC paddi'` | 16 | 2,019 | 126.2 |
| 4       | `b'ng errors must n'` | 16 | 1,640 | 102.5 |
| 5       | `b'ot leak informat'` | 16 | 2,230 | 139.4 |
| 6       | `b'ion.\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c'` | 16 | 2,415 | 150.9 |
| **Total** | **Full Recovered Text** | **96** | **12,248** | **127.6** |

### 4.2 Query Complexity Analysis
- Theoretical Worst Case per Byte: 256 queries.
- Theoretical Expected Case per Byte: $\frac{256}{2} = 128$ queries.
- Actual Measured Average: **127.6 queries/byte**.
- Total Queries for 96 bytes: **12,248 queries**.

---

## 5. Security Recommendations & Prevention

### 5.1 Primary Remediation: Authenticated Encryption (AEAD)
The most robust defense is migrating from unauthenticated AES-CBC to **Authenticated Encryption with Associated Data (AEAD)** modes:
- **AES-GCM (Galois/Counter Mode)**: Combines CTR encryption with GHASH authentication.
- **AES-CCM (Counter with CBC-MAC)**.

AEAD modes verify data integrity and authenticity *before* attempting decryption, making padding oracle attacks mathematically impossible.

### 5.2 Alternative Remediation: Encrypt-then-MAC (EtM)
If CBC mode must be retained, implement **Encrypt-then-MAC**:
1. Compute HMAC (e.g., HMAC-SHA256) over $(IV \parallel \text{Ciphertext})$.
2. On decryption, verify HMAC using constant-time comparison (`hmac.compare_digest`).
3. If MAC verification fails, immediately abort without executing decryption or padding unpad functions.

### 5.3 Application Security Guidelines
- **Unified Error Messages**: Do not differentiate between "Invalid MAC" and "Invalid Padding". Return generic response (e.g., "Decryption failed").
- **Constant-Time Error Handling**: Ensure execution time is identical whether padding fails or decryption succeeds.
