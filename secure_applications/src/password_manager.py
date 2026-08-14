import json
import os

# ============================================================
# VULNERABILITY 1: HARDCODED CREDENTIALS
# Credentials are directly stored in source code.
# ============================================================

ADMIN_USERNAME = "admin"
ADMIN_PASSWORD = "admin123"

DATA_FILE = "outputs/credentials.json"


def load_credentials():
    """Load credentials from the JSON file."""
    if not os.path.exists(DATA_FILE):
        return []

    try:
        with open(DATA_FILE, "r") as file:
            return json.load(file)
    except Exception as error:
        # ====================================================
        # VULNERABILITY 3: INFORMATION LEAKAGE
        # Internal error details are displayed to the user.
        # ====================================================
        print(f"ERROR: {error}")
        return []


def save_credentials(credentials):
    """Save credentials to the JSON file."""
    os.makedirs("outputs", exist_ok=True)

    with open(DATA_FILE, "w") as file:
        json.dump(credentials, file, indent=4)


def login():
    """Login to the password manager."""

    username = input("Username: ")
    password = input("Password: ")

    # VULNERABILITY 1:
    # Hardcoded credentials are used for authentication.
    if username == ADMIN_USERNAME and password == ADMIN_PASSWORD:
        print("\nLogin successful!")
        return True

    # ========================================================
    # VULNERABILITY 3: INFORMATION LEAKAGE
    # Sensitive password is printed during failed login.
    # ========================================================
    print(f"Login failed. Password entered: {password}")

    return False


def add_credential():
    """Add a new website credential."""

    website = input("Website: ")
    username = input("Username: ")
    password = input("Password: ")

    credentials = load_credentials()

    credentials.append({
        "website": website,
        "username": username,
        "password": password
    })

    save_credentials(credentials)

    print("\nCredential added successfully.")


def view_credentials():
    """Display all stored credentials."""

    credentials = load_credentials()

    if not credentials:
        print("\nNo credentials found.")
        return

    print("\n========== STORED CREDENTIALS ==========")

    for index, credential in enumerate(credentials, start=1):
        print(f"\nCredential {index}")
        print(f"Website  : {credential['website']}")
        print(f"Username : {credential['username']}")
        print(f"Password : {credential['password']}")


def search_credential():
    """Search credentials by website."""

    website = input("Enter website to search: ")

    credentials = load_credentials()

    found = False

    for credential in credentials:
        if website.lower() in credential["website"].lower():
            print("\nCredential Found")
            print(f"Website  : {credential['website']}")
            print(f"Username : {credential['username']}")
            print(f"Password : {credential['password']}")

            found = True

    if not found:
        print("\nNo credential found.")


def delete_credential():
    """Delete a credential."""

    website = input("Enter website to delete: ")

    credentials = load_credentials()

    new_credentials = [
        credential
        for credential in credentials
        if credential["website"].lower() != website.lower()
    ]

    save_credentials(new_credentials)

    print("\nCredential deleted successfully.")


def menu():
    """Display application menu."""

    while True:

        print("\n================================")
        print("        PASSWORD MANAGER")
        print("================================")
        print("1. Add Credential")
        print("2. View Credentials")
        print("3. Search Credential")
        print("4. Delete Credential")
        print("5. Exit")
        print("================================")

        choice = input("Enter choice: ")

        if choice == "1":
            add_credential()

        elif choice == "2":
            # =================================================
            # VULNERABILITY 2: MISSING AUTHENTICATION
            #
            # Sensitive credentials can be viewed without
            # checking whether the user is logged in.
            # =================================================
            view_credentials()

        elif choice == "3":
            search_credential()

        elif choice == "4":
            # =================================================
            # VULNERABILITY 2: MISSING AUTHENTICATION
            #
            # Anyone can delete stored credentials without
            # authentication.
            # =================================================
            delete_credential()

        elif choice == "5":
            print("\nExiting Password Manager...")
            break

        else:
            print("\nInvalid choice.")


def main():
    print("\n================================")
    print("       PASSWORD MANAGER")
    print("================================")

    authenticated = login()

    if authenticated:
        menu()
    else:
        print("\nAccess denied.")


if __name__ == "__main__":
    main()