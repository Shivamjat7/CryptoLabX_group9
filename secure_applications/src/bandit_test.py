from datetime import datetime

from collections import Counter

def write_log(option):
    with open("outputs/log.txt", "a") as file:
        current_time = datetime.now()
        file.write(f"{current_time} - {option}\n")

while True:
    print("\n========== CryptoLabX ==========")
    print("1. Encrypt")
    print("2. Decrypt")
    print("3. Attack")
    print("4. Analyze")
    print("5. Exit")

    choice = input("Enter your choice (1-5): ")

    if choice == "1":
        write_log("Encrypt")
        print("\nEncrypt - Coming Soon!")

    elif choice == "2":
        write_log("Decrypt")
        print("\nDecrypt - Coming Soon!")

    elif choice == "3":
        write_log("Attack")
        print("\nAttack - Coming Soon!")

    elif choice == "4":
        write_log("Analyze")
        filename = input("Enter the file name (example: sample1.txt): ")

        try:
            with open("datasets/" + filename, "r") as file:
                text = file.read()

            characters = len(text)
            words = len(text.split())
            lines = len(text.splitlines())
            unique_characters = len(set(text))

            print("\n----- File Analysis -----")
            print("Characters :", characters)
            print("Words      :", words)
            print("Lines      :", lines)
            print("Unique Characters :", unique_characters)

            letters = []

            for ch in text.lower():
                if ch.isalpha():
                    letters.append(ch)

            frequency = Counter(letters)

            print("\nLetter Frequency")
            for letter in sorted(frequency):
                print(letter, ":", frequency[letter])

        except FileNotFoundError:
            print("File not found!")

    elif choice == "5":
        write_log("Exit")
        print("Thank you for using CryptoLabX.")
        break

    else:
        print("Invalid choice!")