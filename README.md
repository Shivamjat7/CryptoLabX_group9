project overview: 
CryptoLabX is a modular cryptography toolkit developed to provide a structured framework for implementing and analyzing cryptographic techniques. The project is designed to support encryption, decryption, cryptanalysis, mathematical utilities, text analysis, and other cryptography-related operations. 


Objectives: 
1.Develop a well-organized cryptography toolkit.
2.Create a user-friendly command-line interface.
3.Perform text file analysis.
4.Maintain execution logs for user activities.
5.Use Git and GitHub for version control and collaborative development.
6.Design a reusable project structure for future cryptographic modules.


Team Members:
1. shweta choudhary
2. shivam


Bandit lab experiment lab -2
----------------------

Overview:
- Objective: Run Bandit (Python security scanner) on the repository to identify common security issues in Python code.
- Tool: Bandit (PyCQA) — a static analyzer for Python security issues.

How to reproduce:
1. Install Bandit:
	pip install bandit
2. Run Bandit recursively and save a text report:
	bandit -r . -lll -f txt -o secure_applications/outputs/sast_lab_log.txt

Findings:
- Full scan output and details are saved in secure_applications/outputs/sast_lab_log.txt.
- Typical findings to review: weak cryptographic usage, insecure use of subprocess/OS functions, hard-coded secrets, and insecure file handling.

Remediation (next steps):
- Review the items in secure_applications/outputs/sast_lab_log.txt and triage by severity.
- Apply fixes such as using secure library APIs, avoiding hard-coded secrets, validating inputs, and following secure coding patterns.
- Re-run Bandit after fixes to confirm issues are resolved.

Password Manager - Lab Assignment 3
----------------------

Overview:
- Objective: Develop and analyze a secure password manager application using Python and identify security vulnerabilities using Bandit.
- Tool: Bandit (PyCQA) — a static analyzer for Python security issues.
- Group: 9

How to reproduce:
1. Navigate to the secure_applications directory:
	cd secure_applications
2. Activate virtual environment:
	source .venv/bin/activate
3. Run the password manager:
	python src/password_manager.py
4. Run Bandit scan on the source code:
	bandit -r src

Features:
- Login functionality
- Add credentials
- View credentials
- Search credentials
- Delete credentials

Findings:
- Hardcoded Credentials — Username and password are stored directly in source code.
- Missing Authentication — Sensitive operations do not properly verify authentication.
- Information Leakage — Passwords are exposed through error/debug messages.
- Full scan results are saved in secure_applications/sast/ folder.

Remediation (next steps):
- Review vulnerabilities identified by Bandit in secure_applications/sast/bandit_report.json and bandit_report.txt.
- Implement secure credential storage using encryption.
- Add proper authentication and authorization checks.
- Remove hardcoded secrets and use environment variables instead.
- Implement secure error handling to prevent information leakage.
- Re-run Bandit after fixes to confirm issues are resolved.


Monoalphabetic Substitution Cipher & Cryptanalysis - Group 9
-----------------------------------------------------------

Overview:
- Objective: Implement Monoalphabetic substitution cipher and perform cryptanalysis using letter frequency, word pattern, and structural analysis on text from Katz & Lindell "Introduction to Modern Cryptography" (Page 39).
- Language: C++ (Standard C++17)
- Group: 9 (Page 9 + 30 = 39)

Files:
- `classical/plaintext.txt` - Katz & Lindell Page 39 text (> 1 page, 428 words).
- `classical/monoalphabetic_cipher.cpp` - C++ Cipher Engine (Encryption, Decryption, Key generation).
- `classical/ciphertext.txt` - Encrypted ciphertext.
- `classical/monoalphabetic_cryptanalysis.cpp` - C++ Cryptanalysis module containing required functions:
  1. `frequency_analysis()`
  2. `word_frequency_analysis()`
  3. `pattern_analysis()`
  4. `apply_substitution()`
  5. `display_partial_plaintext()`
- `classical/recovered_plaintext.txt` - Decrypted plaintext after cryptanalysis.
- `docs/cryptanalysis_notebook.md` - Cryptanalytic decision log table & key recovery verification report.

How to compile & run:
1. Build and run Monoalphabetic Cipher Engine:
   `g++ -std=c++17 -Wall classical/monoalphabetic_cipher.cpp -o classical/monoalphabetic_cipher`
   `./classical/monoalphabetic_cipher`

2. Build and run Cryptanalysis Engine:
   `g++ -std=c++17 -Wall classical/monoalphabetic_cryptanalysis.cpp -o classical/monoalphabetic_cryptanalysis`
   `./classical/monoalphabetic_cryptanalysis`


Vigenère Cipher Cryptanalysis (Kasiski & Frequency Analysis) - Group 9
---------------------------------------------------------------------

Overview:
- Objective: Cryptanalysis of Vigenère Cipher using Kasiski Examination, Index of Coincidence, and Chi-squared frequency correlation.
- Target Input: Ciphertext 1 (Odd Group No. 9).
- Language: C++ (Standard C++17)

Key Features & Required Functions:
- `clean_ciphertext()`: Preprocesses ciphertext string.
- `find_repeated_patterns()` & `calculate_distances()` & `find_factors()`: Kasiski examination.
- `kasiski_analysis()` & `calculate_ic()`: Estimates key length (Length 14).
- `split_into_groups()`, `frequency_analysis()`, `find_shift()`, `find_key()`: Recovers key `AMBROISETHOMAS`.
- `vigenere_decrypt()`, `vigenere_encrypt()`, `verify()`: Decrypts and verifies 100% re-encryption match.

Files:
- `classical/vigenere_ciphertext.txt` - Input ciphertext.
- `classical/vigenere_cryptanalysis.cpp` - C++ cryptanalysis engine.
- `classical/vigenere_recovered_plaintext.txt` - Decrypted plaintext output.
- `docs/vigenere_cryptanalysis_notebook.md` - Analysis notebook & distance factor tables.

How to compile & run:
1. Build and run Vigenère Cryptanalysis Engine:
   `g++ -std=c++17 -Wall classical/vigenere_cryptanalysis.cpp -o classical/vigenere_cryptanalysis`
   `./classical/vigenere_cryptanalysis`
