# SAST Analysis

**Tool:** Bandit
**Language:** Python

## Command

```bash
bandit -r src
```

Save results:

```bash
bandit -r src -f txt -o sast/bandit_report.txt
```

## Analysis

Bandit scans Python source code for common security problems.

The scan was performed on:

```text
src/password_manager.py
```

The generated report is stored in:

```text
sast/bandit_report.txt
```

Some vulnerabilities, such as missing authentication, may require manual testing because they depend on application logic.

## Conclusion

Bandit provides automated security analysis, while manual testing was used to demonstrate application-level vulnerabilities.
