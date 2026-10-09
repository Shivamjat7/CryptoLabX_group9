"""
AES-CBC Padding Oracle Attack Implementation
CryptoLabX Group 9 - Modern Cryptography / Attack Module

Demonstrates plaintext recovery from AES-CBC ciphertext using a padding oracle
without knowledge of the encryption key.
"""

import os
import sys

# --------------------------------------------------
# CRYPTOGRAPHIC LIBRARY FALLBACK COMPATIBILITY
# --------------------------------------------------
try:
    from Crypto.Cipher import AES
    BLOCK_SIZE = AES.block_size
except ImportError:
    try:
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        class AES:
            block_size = 16
            MODE_CBC = "CBC"
            @staticmethod
            def new(key, mode, iv):
                cipher = Cipher(algorithms.AES(key), modes.CBC(iv))
                class _AESCipher:
                    def encrypt(self, data):
                        encryptor = cipher.encryptor()
                        return encryptor.update(data) + encryptor.finalize()
                    def decrypt(self, data):
                        decryptor = cipher.decryptor()
                        return decryptor.update(data) + decryptor.finalize()
                return _AESCipher()
        BLOCK_SIZE = 16
    except ImportError:
        print("Error: Neither PyCryptodome (Crypto) nor cryptography package is available.")
        sys.exit(1)


# --------------------------------------------------
# PKCS#7 PADDING
# --------------------------------------------------

def pkcs7_pad(data: bytes) -> bytes:
    """
    Add PKCS#7 padding to data.
    """
    padding_length = BLOCK_SIZE - (len(data) % BLOCK_SIZE)
    return data + bytes([padding_length]) * padding_length


def pkcs7_unpad(data: bytes) -> bytes:
    """
    Remove PKCS#7 padding.
    Raises ValueError if padding is invalid.
    """
    if not data or len(data) % BLOCK_SIZE != 0:
        raise ValueError("Invalid padded data length")

    padding_length = data[-1]

    if not 1 <= padding_length <= BLOCK_SIZE:
        raise ValueError("Invalid padding length")

    expected_padding = bytes([padding_length]) * padding_length

    if data[-padding_length:] != expected_padding:
        raise ValueError("Invalid PKCS#7 padding")

    return data[:-padding_length]


# --------------------------------------------------
# AES-CBC ENCRYPTION
# Used only to prepare the demonstration.
# --------------------------------------------------

def encrypt_message(plaintext: bytes, key: bytes, iv: bytes) -> bytes:
    """
    Encrypts plaintext using AES-CBC with PKCS#7 padding.
    """
    cipher = AES.new(key, AES.MODE_CBC, iv)
    padded_plaintext = pkcs7_pad(plaintext)
    return cipher.encrypt(padded_plaintext)


# --------------------------------------------------
# PADDING ORACLE
# Returns only whether padding is valid.
# --------------------------------------------------

def create_padding_oracle(key: bytes):
    """
    Creates an oracle that internally knows the key.
    The attack function never receives the key.
    The oracle returns only True or False.
    """

    def padding_oracle(iv: bytes, ciphertext: bytes) -> bool:
        try:
            if len(iv) != BLOCK_SIZE:
                return False

            if not ciphertext or len(ciphertext) % BLOCK_SIZE != 0:
                return False

            cipher = AES.new(key, AES.MODE_CBC, iv)
            decrypted = cipher.decrypt(ciphertext)
            pkcs7_unpad(decrypted)
            return True

        except (ValueError, TypeError):
            return False

    return padding_oracle


# --------------------------------------------------
# PADDING ORACLE ATTACK
# --------------------------------------------------

def recover_block(previous_block: bytes, target_block: bytes, oracle) -> tuple[bytes, int]:
    """
    Recover one plaintext block using a padding oracle.

    previous_block:
        Original IV or preceding ciphertext block.

    target_block:
        Ciphertext block whose plaintext is being recovered.

    oracle:
        Function that returns whether padding is valid.

    Returns:
        (Recovered plaintext block, query count)
    """
    if len(previous_block) != BLOCK_SIZE:
        raise ValueError("Previous block must be 16 bytes")

    if len(target_block) != BLOCK_SIZE:
        raise ValueError("Target block must be 16 bytes")

    # Intermediate state I = AES_decrypt(target_block)
    intermediate = bytearray(BLOCK_SIZE)

    # The crafted block is modified to force valid padding.
    crafted = bytearray(previous_block)

    queries = 0

    # Recover the block from right to left (index 15 down to 0).
    for position in range(BLOCK_SIZE - 1, -1, -1):
        padding_value = BLOCK_SIZE - position

        # Force already recovered bytes to equal the desired padding value.
        for j in range(position + 1, BLOCK_SIZE):
            crafted[j] = intermediate[j] ^ padding_value

        found = False

        # Brute-force the current byte position (0 to 255).
        for guess in range(256):
            crafted[position] = guess
            queries += 1

            valid = oracle(bytes(crafted), target_block)

            if not valid:
                continue

            # When searching for 0x01 padding (last byte), verify that
            # this is not a false positive caused by multi-byte padding
            # already present in the decrypted plaintext.
            if position == BLOCK_SIZE - 1:
                confirmation = bytearray(crafted)
                # Changing the second-last byte should preserve genuine 0x01 padding.
                confirmation[position - 1] ^= 1
                queries += 1

                if not oracle(bytes(confirmation), target_block):
                    continue

            # The oracle confirmed the desired padding.
            intermediate[position] = guess ^ padding_value
            found = True
            break

        if not found:
            raise RuntimeError(f"Could not recover byte at position {position}")

    # Plaintext P = Intermediate state I XOR original previous ciphertext block
    plaintext_block = bytes(intermediate[i] ^ previous_block[i] for i in range(BLOCK_SIZE))

    return plaintext_block, queries


def padding_oracle_attack(iv: bytes, ciphertext: bytes, oracle) -> tuple[bytes, int]:
    """
    Recover complete plaintext without the AES key.

    Returns:
        (Recovered unpadded plaintext, total oracle queries)
    """
    if len(iv) != BLOCK_SIZE:
        raise ValueError("IV must be 16 bytes")

    if not ciphertext or len(ciphertext) % BLOCK_SIZE != 0:
        raise ValueError("Ciphertext must contain complete AES blocks")

    ciphertext_blocks = [
        ciphertext[i:i + BLOCK_SIZE]
        for i in range(0, len(ciphertext), BLOCK_SIZE)
    ]

    recovered_plaintext = bytearray()
    total_queries = 0

    previous_block = iv

    for block_number, target_block in enumerate(ciphertext_blocks, start=1):
        plaintext_block, queries = recover_block(
            previous_block,
            target_block,
            oracle
        )

        recovered_plaintext.extend(plaintext_block)
        total_queries += queries

        print(
            f"Recovered block {block_number}: "
            f"{plaintext_block!r} (Queries: {queries})"
        )

        previous_block = target_block

    # Remove the PKCS#7 padding after recovering all blocks.
    plaintext = pkcs7_unpad(bytes(recovered_plaintext))

    return plaintext, total_queries


# --------------------------------------------------
# DEMONSTRATION
# --------------------------------------------------

def main():
    # Original message.
    original_message = (
        b"Padding oracle attacks demonstrate why "
        b"CBC padding errors must not leak information."
    )

    # Key used only to create the test environment.
    # It is never passed to the attack function.
    secret_key = os.urandom(BLOCK_SIZE)

    # Random IV.
    iv = os.urandom(BLOCK_SIZE)

    # Encrypt the message.
    ciphertext = encrypt_message(
        original_message,
        secret_key,
        iv
    )

    # Create the oracle.
    # It returns only a Boolean padding result.
    oracle = create_padding_oracle(secret_key)

    print("=" * 65)
    print("AES-CBC PADDING ORACLE ATTACK - CRYPTOLABX GROUP 9")
    print("=" * 65)

    print("\nCiphertext (hex):")
    print(ciphertext.hex())

    print("\nIV (hex):")
    print(iv.hex())

    print(f"\nMessage length: {len(original_message)} bytes")
    print(f"Ciphertext length: {len(ciphertext)} bytes ({len(ciphertext) // BLOCK_SIZE} blocks)")

    print("\nRecovering plaintext byte-by-byte from right to left...\n")

    # The attack receives no key.
    recovered_plaintext, total_queries = padding_oracle_attack(
        iv,
        ciphertext,
        oracle
    )

    print("\n" + "=" * 65)
    print("ATTACK RESULTS")
    print("=" * 65)

    print("\nRecovered plaintext:")
    print(recovered_plaintext.decode("utf-8"))

    print("\nTotal oracle queries:")
    print(total_queries)

    print("\nVerification:")
    if recovered_plaintext == original_message:
        print("SUCCESS: Recovered plaintext perfectly matches original message!")
    else:
        print("FAILED: Recovered plaintext does not match original message.")


if __name__ == "__main__":
    main()
