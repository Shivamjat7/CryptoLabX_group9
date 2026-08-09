import os
import hashlib
import subprocess

password = "admin123"  # Hardcoded password

user_input = input("Enter a command: ")

eval(user_input)  # Unsafe use of eval

os.system(user_input)  # Command injection

subprocess.call(user_input, shell=True)  # shell=True vulnerability

hash_value = hashlib.md5(password.encode()).hexdigest()  # Weak hash

print(hash_value)