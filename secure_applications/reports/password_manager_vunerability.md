# Vulnerability Report

## 1. Hardcoded Credentials

The application stores login credentials directly in the source code.

```python
ADMIN_USERNAME = "admin"
ADMIN_PASSWORD = "admin123"
```

**Impact:** Anyone with source-code access can obtain the credentials.

**Fix:** Use environment variables or a secure secrets manager.

## 2. Missing Authentication

Sensitive operations such as viewing and deleting credentials do not properly verify authentication.

**Impact:** Unauthorized users may access or modify credentials.

**Fix:** Implement authentication and authorization checks before sensitive operations.

## 3. Information Leakage

The application displays sensitive information in error/debug messages.

```python
print(f"Login failed. Password entered: {password}")
```

**Impact:** User passwords may be exposed.

**Fix:** Never display passwords or sensitive information in errors/logs.

## Conclusion

The application demonstrates three common security vulnerabilities and their potential impact. SAST analysis using Bandit helps identify security issues in the Python source code.
