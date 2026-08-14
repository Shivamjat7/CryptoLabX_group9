# Password Manager Test Cases

## TC-01: Valid Login

Input:
Username: admin
Password: admin123

Expected:
Login successful.

Result:
PASS


## TC-02: Invalid Login

Input:
Username: admin
Password: wrong123

Expected:
Access denied.

Result:
PASS


## TC-03: Add Credential

Input:
Website: github.com
Username: shivam
Password: github123

Expected:
Credential added successfully.

Result:
PASS


## TC-04: View Credentials

Input:
Select option 2.

Expected:
Stored website, username and password are displayed.

Result:
PASS


## TC-05: Search Credential

Input:
Website: github

Expected:
GitHub credential is displayed.

Result:
PASS


## TC-06: Delete Credential

Input:
Website: github.com

Expected:
Credential is deleted.

Result:
PASS


# Security Test Cases

## ST-01: Hardcoded Credentials

Observation:
ADMIN_USERNAME and ADMIN_PASSWORD are directly
present in the source code.

Vulnerability:
Hardcoded Credentials.


## ST-02: Missing Authentication

Observation:
Sensitive functions such as view_credentials()
and delete_credential() do not perform their own
authentication/authorization checks.

Vulnerability:
Missing Authentication.


## ST-03: Information Leakage

Input:
Username: test
Password: secret123

Observation:
The application prints the entered password
when login fails.

Vulnerability:
Information Leakage.