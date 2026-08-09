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








