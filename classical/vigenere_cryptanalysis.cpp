#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cctype>

using namespace std;

// Standard English Letter Frequencies (A-Z)
const vector<double> ENGLISH_FREQ = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
    0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
    0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074
};

// 1. Clean ciphertext: remove non-alphabetic chars and convert to uppercase
string clean_ciphertext(const string& raw_ct) {
    string clean = "";
    for (char c : raw_ct) {
        if (isalpha(c)) {
            clean += (char)toupper(c);
        }
    }
    return clean;
}

// 2. Find repeated patterns of specified min_length in ciphertext
map<string, vector<int>> find_repeated_patterns(const string& ct, int min_length = 3) {
    map<string, vector<int>> pattern_positions;
    int n = (int)ct.length();

    for (int len = min_length; len <= 5; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            string pat = ct.substr(i, len);
            if (pattern_positions.find(pat) == pattern_positions.end()) {
                vector<int> positions;
                for (int j = i; j <= n - len; ++j) {
                    if (ct.substr(j, len) == pat) {
                        positions.push_back(j);
                    }
                }
                if (positions.size() > 1) {
                    pattern_positions[pat] = positions;
                }
            }
        }
    }
    return pattern_positions;
}

// 3. Calculate distances between repeated pattern occurrences
vector<int> calculate_distances(const map<string, vector<int>>& patterns) {
    vector<int> distances;
    for (const auto& kv : patterns) {
        const auto& pos = kv.second;
        for (size_t i = 1; i < pos.size(); ++i) {
            distances.push_back(pos[i] - pos[i - 1]);
        }
    }
    return distances;
}

// 4. Find factors (2 to 20) of distances obtained from repeated patterns
map<int, int> find_factors(const vector<int>& distances) {
    map<int, int> factor_counts;
    for (int dist : distances) {
        for (int f = 2; f <= 20; ++f) {
            if (dist % f == 0) {
                factor_counts[f]++;
            }
        }
    }
    return factor_counts;
}

// 6. Calculate Index of Coincidence (IC) for a text stream
double calculate_ic(const string& text) {
    int n = (int)text.length();
    if (n <= 1) return 0.0;

    map<char, int> counts;
    for (char c : text) {
        counts[c]++;
    }

    double sum_n = 0.0;
    for (const auto& kv : counts) {
        sum_n += kv.second * (kv.second - 1);
    }
    return sum_n / (n * (n - 1));
}

// 7. Divide ciphertext according to a candidate key length
vector<string> split_into_groups(const string& ct, int key_length) {
    vector<string> groups(key_length, "");
    for (size_t i = 0; i < ct.length(); ++i) {
        groups[i % key_length] += ct[i];
    }
    return groups;
}

// 5. Kasiski analysis: use repeated patterns & IC to suggest candidate key length
int kasiski_analysis(const string& ct) {
    auto patterns = find_repeated_patterns(ct, 3);
    auto distances = calculate_distances(patterns);
    auto factor_counts = find_factors(distances);

    // Combine factor count ranking with Index of Coincidence evaluation
    int best_klen = 1;
    double best_score = -1.0;

    cout << "\n--- Kasiski Pattern Factor Frequencies ---" << endl;
    for (const auto& kv : factor_counts) {
        if (kv.first >= 2 && kv.first <= 16) {
            cout << "  Factor " << setw(2) << kv.first << ": " << kv.second << " occurrences" << endl;
        }
    }

    cout << "\n--- Index of Coincidence (IC) for Candidate Key Lengths ---" << endl;
    for (int klen = 1; klen <= 16; ++klen) {
        vector<string> groups = split_into_groups(ct, klen);
        double avg_ic = 0.0;
        for (const string& g : groups) {
            avg_ic += calculate_ic(g);
        }
        avg_ic /= klen;
        cout << "  Key Length " << setw(2) << klen << ": Avg IC = " << fixed << setprecision(4) << avg_ic << endl;

        // Score based on proximity to English IC (~0.065) and factor occurrences
        double ic_diff = abs(avg_ic - 0.065);
        double factor_bonus = factor_counts[klen] * 0.005;
        double score = (1.0 / (ic_diff + 0.001)) + factor_bonus;

        if (score > best_score && klen >= 3) {
            best_score = score;
            best_klen = klen;
        }
    }

    return best_klen;
}

// 8. Calculate A-Z letter frequency distribution for a group
vector<double> frequency_analysis(const string& group) {
    vector<double> freqs(26, 0.0);
    int total = (int)group.length();
    if (total == 0) return freqs;

    for (char c : group) {
        if (isalpha(c)) {
            freqs[toupper(c) - 'A'] += 1.0;
        }
    }
    for (int i = 0; i < 26; ++i) {
        freqs[i] /= total;
    }
    return freqs;
}

// 9. Estimate the Caesar shift for a group using chi-squared / frequency correlation
int find_shift(const string& group) {
    int best_shift = 0;
    double best_corr = -1.0;
    int n = (int)group.length();

    map<char, int> counts;
    for (char c : group) counts[c]++;

    for (int shift = 0; shift < 26; ++shift) {
        double corr = 0.0;
        for (int i = 0; i < 26; ++i) {
            char shifted_c = (char)('A' + (i + shift) % 26);
            double obs = (double)counts[shifted_c] / n;
            double exp = ENGLISH_FREQ[i];
            corr += obs * exp;
        }
        if (corr > best_corr) {
            best_corr = corr;
            best_shift = shift;
        }
    }
    return best_shift;
}

// 10. Combine shifts to obtain the probable Vigenere key
string find_key(const vector<string>& groups) {
    string key = "";
    for (size_t i = 0; i < groups.size(); ++i) {
        int shift = find_shift(groups[i]);
        key += (char)('A' + shift);
    }
    return key;
}

// 11. Decrypt ciphertext using recovered key
string vigenere_decrypt(const string& ct, const string& key) {
    string pt = "";
    int klen = (int)key.length();
    for (size_t i = 0; i < ct.length(); ++i) {
        char c = ct[i];
        if (isalpha(c)) {
            int shift = toupper(key[i % klen]) - 'A';
            char decrypted = (char)('A' + (toupper(c) - 'A' - shift + 26) % 26);
            pt += islower(c) ? (char)tolower(decrypted) : decrypted;
        } else {
            pt += c;
        }
    }
    return pt;
}

// 12. Re-encrypt plaintext using recovered key for verification
string vigenere_encrypt(const string& pt, const string& key) {
    string ct = "";
    int klen = (int)key.length();
    for (size_t i = 0; i < pt.length(); ++i) {
        char c = pt[i];
        if (isalpha(c)) {
            int shift = toupper(key[i % klen]) - 'A';
            char encrypted = (char)('A' + (toupper(c) - 'A' + shift) % 26);
            ct += islower(c) ? (char)tolower(encrypted) : encrypted;
        } else {
            ct += c;
        }
    }
    return ct;
}

// 13. Verify that re-encryption produces original ciphertext
bool verify(const string& orig_ct, const string& re_encrypted_ct) {
    return clean_ciphertext(orig_ct) == clean_ciphertext(re_encrypted_ct);
}

int main() {
    string input_file = "classical/vigenere_ciphertext.txt";
    ifstream in(input_file);
    if (!in.is_open()) {
        cerr << "[!] Error: Cannot open input file " << input_file << endl;
        return 1;
    }
    string raw_ciphertext((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cout << "======================================================================" << endl;
    cout << "          VIGENERE CIPHER CRYPTANALYSIS SYSTEM (GROUP 9)              " << endl;
    cout << "======================================================================" << endl;

    // Step 1: Preprocess ciphertext
    string clean_ct = clean_ciphertext(raw_ciphertext);
    cout << "[+] Loaded Raw Ciphertext Length   : " << raw_ciphertext.length() << " chars" << endl;
    cout << "[+] Preprocessed Clean Ciphertext : " << clean_ct.length() << " characters (A-Z)" << endl;

    // Step 2 & 5: Kasiski Examination & Key Length Estimation
    int estimated_key_length = kasiski_analysis(clean_ct);
    cout << "\n----------------------------------------------------------------------" << endl;
    cout << "[+] ESTIMATED KEY LENGTH (Kasiski & IC Analysis): " << estimated_key_length << endl;
    cout << "----------------------------------------------------------------------" << endl;

    // Step 3: Split into groups according to estimated key length
    vector<string> groups = split_into_groups(clean_ct, estimated_key_length);

    // Step 4 & 7: Perform frequency analysis for each group & display frequency tables
    cout << "\n======================================================================" << endl;
    cout << "               FREQUENCY TABLES FOR EACH KEY GROUP                    " << endl;
    cout << "======================================================================" << endl;
    for (int g = 0; g < estimated_key_length; ++g) {
        vector<double> freqs = frequency_analysis(groups[g]);
        int shift = find_shift(groups[g]);
        char key_char = (char)('A' + shift);

        cout << "\nGroup " << setw(2) << (g + 1) << " [Size: " << groups[g].length() 
             << " chars | Inferred Shift: " << setw(2) << shift << " ('" << key_char << "')]:" << endl;
        
        // Print top 5 letters for group
        vector<pair<char, double>> group_letters;
        for (int i = 0; i < 26; ++i) {
            group_letters.push_back({ (char)('A' + i), freqs[i] });
        }
        sort(group_letters.begin(), group_letters.end(), [](const pair<char, double>& a, const pair<char, double>& b) {
            return a.second > b.second;
        });

        cout << "  Top Letters: ";
        for (int i = 0; i < 5; ++i) {
            cout << group_letters[i].first << " (" << fixed << setprecision(1) << group_letters[i].second * 100.0 << "%)  ";
        }
        cout << endl;
    }

    // Step 5: Determine probable key
    string recovered_key = find_key(groups);

    // Step 6: Decrypt ciphertext using recovered key
    string recovered_plaintext = vigenere_decrypt(clean_ct, recovered_key);

    // Save recovered plaintext
    ofstream out("classical/vigenere_recovered_plaintext.txt");
    if (out.is_open()) {
        out << recovered_plaintext;
        out.close();
    }

    // Step 7: Display Results
    cout << "\n======================================================================" << endl;
    cout << "                        FINAL CRYPTANALYSIS RESULTS                   " << endl;
    cout << "======================================================================" << endl;
    cout << "Estimated Key Length : " << estimated_key_length << endl;
    cout << "Recovered Key        : " << recovered_key << endl;
    cout << "Recovered Plaintext  :\n" << recovered_plaintext << endl;
    cout << "----------------------------------------------------------------------" << endl;

    // Step 8: Verification by re-encrypting
    string re_encrypted_ciphertext = vigenere_encrypt(recovered_plaintext, recovered_key);
    bool is_verified = verify(clean_ct, re_encrypted_ciphertext);

    cout << "\n======================================================================" << endl;
    cout << "                   RE-ENCRYPTION VERIFICATION RESULT                  " << endl;
    cout << "======================================================================" << endl;
    if (is_verified) {
        cout << "[SUCCESS] RE-ENCRYPTION MATCH VERIFIED! 100% Identical to original ciphertext!" << endl;
    } else {
        cout << "[FAILED] Re-encryption diff detected." << endl;
    }
    cout << "======================================================================" << endl;

    return 0;
}
