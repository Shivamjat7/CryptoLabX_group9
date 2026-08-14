# Password Manager

## Lab Assignment 3

**Group:** 9
**Application:** Password Manager
**Language:** Python
**SAST Tool:** Bandit

## Features

* Login
* Add credentials
* View credentials
* Search credentials
* Delete credentials

## Vulnerabilities

1. **Hardcoded Credentials** — Username and password are stored directly in source code.
2. **Missing Authentication** — Sensitive operations do not properly verify authentication.
3. **Information Leakage** — Passwords are exposed through error/debug messages.

## Project Structure

```text
secure_applications/
├── src/
├── reports/
├── screenshots/
├── sast/
├── outputs/
├── testcases/
└── README.md
```

## Run

```bash
source .venv/bin/activate
python src/password_manager.py
```

## Bandit Scan

```bash
bandit -r src
```

The Bandit results are stored in the `sast/` folder.
