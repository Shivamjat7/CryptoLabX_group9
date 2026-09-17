#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <cctype>
#include <random>

using namespace std;

// Standard alphabet
const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Default substitution key (Permutation of ALPHABET)
// Plaintext:  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
// Ciphertext: X N Y A H P O G Z B V Q R U J W E M K D C I S T F L
const string DEFAULT_KEY = "XNYAHPOGZBVQRUJWEMKDCISTFL";

// Function to generate a random key
string generate_random_key() {
    string key = ALPHABET;
    random_device rd;
    mt19937 g(rd());
    shuffle(key.begin(), key.end(), g);
    return key;
}

// Function to validate substitution key
bool is_valid_key(const string& key) {
    if (key.length() != 26) return false;
    string temp = key;
    transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
    sort(temp.begin(), temp.end());
    return temp == ALPHABET;
}

// Function to encrypt plaintext using key
string encrypt(const string& plaintext, const string& key) {
    string ciphertext = "";
    for (char c : plaintext) {
        if (isalpha(c)) {
            bool is_lower = islower(c);
            char upper_c = toupper(c);
            int index = upper_c - 'A';
            char encrypted_char = key[index];
            ciphertext += is_lower ? tolower(encrypted_char) : toupper(encrypted_char);
        } else {
            ciphertext += c;
        }
    }
    return ciphertext;
}

// Function to decrypt ciphertext using key
string decrypt(const string& ciphertext, const string& key) {
    // Build reverse mapping
    map<char, char> reverse_map;
    for (size_t i = 0; i < ALPHABET.length(); ++i) {
        reverse_map[toupper(key[i])] = ALPHABET[i];
    }

    string plaintext = "";
    for (char c : ciphertext) {
        if (isalpha(c)) {
            bool is_lower = islower(c);
            char upper_c = toupper(c);
            char decrypted_char = reverse_map[upper_c];
            plaintext += is_lower ? tolower(decrypted_char) : toupper(decrypted_char);
        } else {
            plaintext += c;
        }
    }
    return plaintext;
}

int main(int argc, char* argv[]) {
    string key = DEFAULT_KEY;
    string input_file = "classical/plaintext.txt";
    string output_file = "classical/ciphertext.txt";
    string key_file = "classical/key.txt";

    if (argc > 1) {
        string arg1 = argv[1];
        if (arg1 == "--generate-key") {
            key = generate_random_key();
            cout << "[+] Generated New Substitution Key: " << key << endl;
        } else if (arg1.length() == 26 && is_valid_key(arg1)) {
            key = arg1;
            transform(key.begin(), key.end(), key.begin(), ::toupper);
        }
    }

    cout << "====================================================" << endl;
    cout << "     MONOALPHABETIC SUBSTITUTION CIPHER ENGINE     " << endl;
    cout << "====================================================" << endl;
    cout << "Plaintext Alphabet : " << ALPHABET << endl;
    cout << "Ciphertext Key     : " << key << endl;
    cout << "----------------------------------------------------" << endl;

    // Read plaintext
    ifstream in(input_file);
    if (!in.is_open()) {
        cerr << "[!] Error: Cannot open input file " << input_file << endl;
        return 1;
    }
    string plaintext((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    // Encrypt plaintext
    string ciphertext = encrypt(plaintext, key);

    // Save ciphertext
    ofstream out(output_file);
    if (!out.is_open()) {
        cerr << "[!] Error: Cannot open output file " << output_file << endl;
        return 1;
    }
    out << ciphertext;
    out.close();

    // Save key
    ofstream key_out(key_file);
    if (key_out.is_open()) {
        key_out << "PLAINTEXT_ALPHABET=" << ALPHABET << "\n";
        key_out << "SUBSTITUTION_KEY  =" << key << "\n";
        key_out.close();
    }

    cout << "[+] Encrypted " << plaintext.length() << " characters." << endl;
    cout << "[+] Plaintext read from : " << input_file << endl;
    cout << "[+] Ciphertext saved to : " << output_file << endl;
    cout << "[+] Substitution key saved to : " << key_file << endl;

    // Verification check: Decrypt and compare
    string test_decrypted = decrypt(ciphertext, key);
    if (test_decrypted == plaintext) {
        cout << "[+] Verification Passed: Decrypted ciphertext perfectly matches original plaintext!" << endl;
    } else {
        cerr << "[!] Warning: Decryption mismatch detected!" << endl;
    }

    cout << "====================================================" << endl;
    return 0;
}
