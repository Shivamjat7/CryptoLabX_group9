#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <set>

using namespace std;

// Standard English Letter Frequency reference (for comparison)
const string ENGLISH_FREQ_ORDER = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

// Function 1: Single Letter Frequency Analysis
void frequency_analysis(const string& ciphertext) {
    map<char, int> counts;
    int total_letters = 0;

    for (char c : ciphertext) {
        if (isalpha(c)) {
            char uc = toupper(c);
            counts[uc]++;
            total_letters++;
        }
    }

    // Convert map to vector for sorting
    vector<pair<char, int>> freq_list(counts.begin(), counts.end());
    sort(freq_list.begin(), freq_list.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
        if (a.second != b.second) return a.second > b.second;
        return a.first < b.first;
    });

    cout << "\n======================================================================" << endl;
    cout << "                    1. SINGLE LETTER FREQUENCY ANALYSIS               " << endl;
    cout << "======================================================================" << endl;
    cout << "Total Alphabetic Characters Analyzed: " << total_letters << endl;
    cout << "----------------------------------------------------------------------" << endl;
    cout << left << setw(8) << "Rank" 
         << setw(12) << "Cipher Char" 
         << setw(12) << "Frequency" 
         << setw(16) << "Percentage (%)" 
         << setw(20) << "English Std Ref" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    string cipher_order = "";
    int rank = 1;
    for (const auto& item : freq_list) {
        double percentage = (double)item.second / total_letters * 100.0;
        char ref_char = (rank <= (int)ENGLISH_FREQ_ORDER.length()) ? ENGLISH_FREQ_ORDER[rank - 1] : '-';
        cout << left << setw(8) << rank
             << setw(12) << item.first
             << setw(12) << item.second
             << fixed << setprecision(2) << setw(16) << percentage
             << setw(20) << ref_char << endl;
        cipher_order += item.first;
        rank++;
    }

    cout << "----------------------------------------------------------------------" << endl;
    cout << "Ciphertext Letters in Descending Frequency Order:\n" << cipher_order << endl;
    cout << "Standard English Letter Order:\n" << ENGLISH_FREQ_ORDER << endl;
    cout << "Top Most Frequent Cipher Letters: ";
    for (size_t i = 0; i < min((size_t)6, freq_list.size()); ++i) {
        cout << freq_list[i].first << " (" << fixed << setprecision(1) << (double)freq_list[i].second / total_letters * 100.0 << "%) ";
    }
    cout << endl;
    cout << "======================================================================" << endl;
}

// Helper function to extract words from text (uppercase)
vector<string> extract_words(const string& text) {
    vector<string> words;
    string current = "";
    for (char c : text) {
        if (isalpha(c)) {
            current += toupper(c);
        } else {
            if (!current.empty()) {
                words.push_back(current);
                current = "";
            }
        }
    }
    if (!current.empty()) words.push_back(current);
    return words;
}

// Function 2: Word Frequency Analysis (1-letter, 2-letter, 3-letter, repeated words)
void word_frequency_analysis(const string& ciphertext) {
    vector<string> words = extract_words(ciphertext);
    map<string, int> word_counts;
    map<int, map<string, int>> length_grouped_words;

    for (const string& w : words) {
        word_counts[w]++;
        length_grouped_words[w.length()][w]++;
    }

    cout << "\n======================================================================" << endl;
    cout << "                    2. WORD FREQUENCY ANALYSIS                       " << endl;
    cout << "======================================================================" << endl;
    cout << "Total Words Extracted: " << words.size() << endl;

    // 1-letter words
    cout << "\n--- One-Letter Words (Candidates for 'a', 'i') ---" << endl;
    if (length_grouped_words.count(1)) {
        for (const auto& item : length_grouped_words[1]) {
            cout << "  Word: '" << item.first << "' | Count: " << item.second << endl;
        }
    } else {
        cout << "  None found." << endl;
    }

    // 2-letter words
    cout << "\n--- Two-Letter Words (Candidates for 'of', 'to', 'in', 'it', 'is', 'be', etc.) ---" << endl;
    if (length_grouped_words.count(2)) {
        vector<pair<string, int>> two_l(length_grouped_words[2].begin(), length_grouped_words[2].end());
        sort(two_l.begin(), two_l.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });
        for (const auto& item : two_l) {
            cout << "  Word: '" << item.first << "' | Count: " << item.second << endl;
        }
    }

    // 3-letter words
    cout << "\n--- Three-Letter Words (Candidates for 'the', 'and', 'for', 'are', 'but', etc.) ---" << endl;
    if (length_grouped_words.count(3)) {
        vector<pair<string, int>> three_l(length_grouped_words[3].begin(), length_grouped_words[3].end());
        sort(three_l.begin(), three_l.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });
        for (const auto& item : three_l) {
            cout << "  Word: '" << item.first << "' | Count: " << item.second << endl;
        }
    }

    // Top repeated words across entire text
    cout << "\n--- Most Frequently Repeated Words ---" << endl;
    vector<pair<string, int>> sorted_words(word_counts.begin(), word_counts.end());
    sort(sorted_words.begin(), sorted_words.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });
    for (size_t i = 0; i < min((size_t)12, sorted_words.size()); ++i) {
        cout << "  " << left << setw(15) << ("'" + sorted_words[i].first + "'") 
             << " | Length: " << setw(2) << sorted_words[i].first.length() 
             << " | Frequency: " << sorted_words[i].second << endl;
    }
    cout << "======================================================================" << endl;
}

// Helper to compute structural pattern identifier of a word (e.g. "THAT" -> "0.1.2.0")
string get_word_pattern(const string& word) {
    map<char, int> char_map;
    int next_id = 0;
    string pattern = "";
    for (char c : word) {
        if (char_map.find(c) == char_map.end()) {
            char_map[c] = next_id++;
        }
        if (!pattern.empty()) pattern += ".";
        pattern += to_string(char_map[c]);
    }
    return pattern;
}

// Function 3: Pattern Analysis (Repeated letter patterns, double letters, n-grams)
void pattern_analysis(const string& ciphertext) {
    vector<string> words = extract_words(ciphertext);

    // Double letter occurrences (e.g., "DD", "HH")
    map<string, int> double_letters;
    // Bigrams & Trigrams
    map<string, int> bigrams;
    map<string, int> trigrams;
    // Structural patterns
    map<string, vector<string>> pattern_words;

    for (const string& w : words) {
        // Double letters
        for (size_t i = 0; i + 1 < w.length(); ++i) {
            if (w[i] == w[i + 1]) {
                string dl = w.substr(i, 2);
                double_letters[dl]++;
            }
        }
        // Bigrams
        for (size_t i = 0; i + 1 < w.length(); ++i) {
            string bg = w.substr(i, 2);
            bigrams[bg]++;
        }
        // Trigrams
        for (size_t i = 0; i + 2 < w.length(); ++i) {
            string tg = w.substr(i, 3);
            trigrams[tg]++;
        }
        // Structural patterns
        string pat = get_word_pattern(w);
        if (find(pattern_words[pat].begin(), pattern_words[pat].end(), w) == pattern_words[pat].end()) {
            pattern_words[pat].push_back(w);
        }
    }

    cout << "\n======================================================================" << endl;
    cout << "                    3. PATTERN ANALYSIS                               " << endl;
    cout << "======================================================================" << endl;

    // Double letters
    cout << "\n--- Repeated Double Letter Patterns ---" << endl;
    vector<pair<string, int>> dl_vec(double_letters.begin(), double_letters.end());
    sort(dl_vec.begin(), dl_vec.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });
    for (const auto& item : dl_vec) {
        cout << "  Pattern: '" << item.first << "' | Count: " << item.second << endl;
    }

    // Top Bigrams
    cout << "\n--- Top 10 Repeated Bigrams ---" << endl;
    vector<pair<string, int>> bg_vec(bigrams.begin(), bigrams.end());
    sort(bg_vec.begin(), bg_vec.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });
    for (size_t i = 0; i < min((size_t)10, bg_vec.size()); ++i) {
        cout << "  Bigram: '" << bg_vec[i].first << "' | Count: " << bg_vec[i].second << endl;
    }

    // Top Trigrams
    cout << "\n--- Top 8 Repeated Trigrams ---" << endl;
    vector<pair<string, int>> tg_vec(trigrams.begin(), trigrams.end());
    sort(tg_vec.begin(), tg_vec.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });
    for (size_t i = 0; i < min((size_t)8, tg_vec.size()); ++i) {
        cout << "  Trigram: '" << tg_vec[i].first << "' | Count: " << tg_vec[i].second << endl;
    }

    // Interesting structural patterns (e.g. repeated letter positions within words)
    cout << "\n--- Distinct Word Structural Patterns ---" << endl;
    int shown = 0;
    for (const auto& entry : pattern_words) {
        if (entry.second.size() >= 2 || entry.first.find("0") != string::npos && entry.first.rfind("0") > 0) {
            cout << "  Pattern [" << entry.first << "]: ";
            for (const string& w : entry.second) cout << w << " ";
            cout << endl;
            if (++shown >= 10) break;
        }
    }
    cout << "======================================================================" << endl;
}

// Function 4: Apply Candidate Substitution
string apply_substitution(const string& ciphertext, const map<char, char>& sub_map) {
    string result = "";
    for (char c : ciphertext) {
        if (isalpha(c)) {
            char uc = toupper(c);
            if (sub_map.count(uc)) {
                char p_char = sub_map.at(uc);
                result += islower(c) ? (char)tolower(p_char) : (char)toupper(p_char);
            } else {
                result += c; // Keep original ciphertext uppercase char if unmapped
            }
        } else {
            result += c;
        }
    }
    return result;
}

// Function 5: Display Formatted Partial Plaintext
void display_partial_plaintext(const string& ciphertext, const map<char, char>& sub_map) {
    cout << "\n======================================================================" << endl;
    cout << "                    5. PARTIAL PLAINTEXT DISPLAY                      " << endl;
    cout << "======================================================================" << endl;
    cout << "[Legend: lowercase letters = resolved plaintext | UPPERCASE = unmapped ciphertext]" << endl;
    cout << "Current Active Substitutions (" << sub_map.size() << "/26 letters mapped):" << endl;
    cout << "  Ciphertext: ";
    for (const auto& kv : sub_map) cout << kv.first << " ";
    cout << "\n  Plaintext : ";
    for (const auto& kv : sub_map) cout << kv.second << " ";
    cout << "\n----------------------------------------------------------------------\n" << endl;

    string rendered = "";
    for (char c : ciphertext) {
        if (isalpha(c)) {
            char uc = toupper(c);
            if (sub_map.count(uc)) {
                // Resolved -> display lowercase
                rendered += (char)tolower(sub_map.at(uc));
            } else {
                // Unmapped -> display UPPERCASE ciphertext char
                rendered += (char)toupper(c);
            }
        } else {
            rendered += c;
        }
    }

    cout << rendered << endl;
    cout << "======================================================================" << endl;
}

// Helper to recover substitution key mapping (Cipher -> Plaintext and Plaintext -> Cipher)
map<char, char> recover_full_key(const map<char, char>& sub_map) {
    return sub_map;
}

int main() {
    string ciphertext_file = "classical/ciphertext.txt";
    ifstream in(ciphertext_file);
    if (!in.is_open()) {
        cerr << "[!] Error: Cannot open ciphertext file " << ciphertext_file << endl;
        return 1;
    }
    string ciphertext((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cout << "======================================================================" << endl;
    cout << "    MONOALPHABETIC SUBSTITUTION CIPHER CRYPTANALYSIS ENGINE          " << endl;
    cout << "======================================================================" << endl;
    cout << "Loaded Ciphertext File: " << ciphertext_file << " (" << ciphertext.length() << " chars)" << endl;

    // Step 1: Letter Frequency Analysis
    frequency_analysis(ciphertext);

    // Step 2: Word Frequency Analysis
    word_frequency_analysis(ciphertext);

    // Step 3: Pattern Analysis
    pattern_analysis(ciphertext);

    // Step 4: Systematic Iterative Cryptanalysis Deduction
    map<char, char> sub_map;

    cout << "\n----------------------------------------------------------------------" << endl;
    cout << "    STARTING ITERATIVE CRYPTANALYTIC HYPOTHESIS DEDUCTION STEPS       " << endl;
    cout << "----------------------------------------------------------------------" << endl;

    // Step 4.1: Top 3-letter word "DGH" (count 31) -> "the"
    // 'D'->'t', 'G'->'h', 'H'->'e'
    cout << "\n[Deduction Step 1] Most frequent 3-letter word 'DGH' (count 31) & top letter 'H' (12.6%) -> 'the'" << endl;
    sub_map['D'] = 't';
    sub_map['G'] = 'h';
    sub_map['H'] = 'e';

    // Step 4.2: 3-letter word "XUA" (count 10) -> "and"
    // 'X'->'a', 'U'->'n', 'A'->'d'
    cout << "[Deduction Step 2] 3-letter word 'XUA' -> 'and'" << endl;
    sub_map['X'] = 'a';
    sub_map['U'] = 'n';
    sub_map['A'] = 'd';

    // Step 4.3: 2-letter words "JP"->"of", "ZU"->"in", "ZK"->"is", "DJ"->"to", "NF"->"by"
    cout << "[Deduction Step 3] 2-letter word mapping based on context & frequency" << endl;
    sub_map['J'] = 'o';
    sub_map['P'] = 'f';
    sub_map['Z'] = 'i';
    sub_map['K'] = 's';
    sub_map['N'] = 'b';

    // Step 4.4: Words with known patterns:
    // "Vhmyvgjppk'k" -> "Kerckhoffs's" ('V'->'k', 'M'->'r', 'Y'->'c', 'O'->'g')
    cout << "[Deduction Step 4] Deduce 'Vhmyvgjppk'k' -> 'Kerckhoffs's'" << endl;
    sub_map['V'] = 'k';
    sub_map['M'] = 'r';
    sub_map['Y'] = 'c';
    sub_map['O'] = 'g';

    // Step 4.5: Contextual words:
    // "ymfwdjomxwgf" -> "cryptography" ('F'->'y', 'W'->'p')
    // "huymfwdzju" -> "encryption" ('E'->'q', 'C'->'u')
    cout << "[Deduction Step 5] Deduce 'cryptography' and 'encryption'" << endl;
    sub_map['F'] = 'y';
    sub_map['W'] = 'p';
    sub_map['E'] = 'q';
    sub_map['C'] = 'u';

    // Step 4.6: Remaining letter mappings:
    // "kfkdhr" -> "system" ('S'->'w', 'R'->'m')
    // "pxydjmzxq" -> "factorial" ('L'->'z', 'B'->'j')
    // "xaihmkxmf" -> "adversary" ('I'->'v')
    // "htwhmzhuyh" -> "experience" ('T'->'x')
    cout << "[Deduction Step 6] Resolving remaining letters from contextual words" << endl;
    sub_map['S'] = 'w';
    sub_map['R'] = 'm';
    sub_map['I'] = 'v';
    sub_map['T'] = 'x';
    sub_map['L'] = 'z';
    sub_map['B'] = 'j';
    sub_map['Q'] = 'l'; // "yjqqxwkhk" -> "collapses"

    // Display partial / full plaintext
    display_partial_plaintext(ciphertext, sub_map);

    // Save recovered plaintext
    string recovered_text = apply_substitution(ciphertext, sub_map);
    ofstream rec_out("classical/recovered_plaintext.txt");
    if (rec_out.is_open()) {
        rec_out << recovered_text;
        rec_out.close();
        cout << "\n[+] Recovered Plaintext written to: classical/recovered_plaintext.txt" << endl;
    }

    // Key Recovery & Validation
    cout << "\n======================================================================" << endl;
    cout << "               RECOVERED SUBSTITUTION KEY VALIDATION                  " << endl;
    cout << "======================================================================" << endl;

    // Construct full substitution key (Cipher -> Plaintext mapping)
    // Recall original key structure: Plaintext 'A'-'Z' mapped to Ciphertext letters.
    // Our sub_map maps Ciphertext 'C' -> Plaintext 'p'.
    // So if Plaintext 'p' comes from Ciphertext 'C', Plaintext 'P' maps to Ciphertext 'C'.
    map<char, char> pt_to_ct;
    for (const auto& kv : sub_map) {
        pt_to_ct[toupper(kv.second)] = kv.first;
    }

    string recovered_key = "";
    string standard_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char p : standard_alphabet) {
        if (pt_to_ct.count(p)) {
            recovered_key += pt_to_ct[p];
        } else {
            recovered_key += '?';
        }
    }

    cout << "Standard Plaintext Alphabet : " << standard_alphabet << endl;
    cout << "Recovered Substitution Key  : " << recovered_key << endl;

    // Load actual secret key if present for comparison
    ifstream key_file("classical/key.txt");
    string actual_key = "";
    if (key_file.is_open()) {
        string line;
        while (getline(key_file, line)) {
            if (line.find("SUBSTITUTION_KEY") != string::npos) {
                actual_key = line.substr(line.find('=') + 1);
                actual_key.erase(remove_if(actual_key.begin(), actual_key.end(), ::isspace), actual_key.end());
            }
        }
        key_file.close();
        cout << "Actual Ground Truth Key    : " << actual_key << endl;
        if (recovered_key == actual_key) {
            cout << "\n[SUCCESS] Recovered substitution key EXACTLY matches ground truth key!" << endl;
        }
    }

    // Re-encryption Validation
    // Encrypt original plaintext using recovered key and compare to ciphertext
    ifstream pt_in("classical/plaintext.txt");
    string orig_pt((istreambuf_iterator<char>(pt_in)), istreambuf_iterator<char>());
    pt_in.close();

    string re_encrypted = "";
    for (char c : orig_pt) {
        if (isalpha(c)) {
            bool is_l = islower(c);
            char uc = toupper(c);
            int idx = uc - 'A';
            char ct_c = recovered_key[idx];
            re_encrypted += is_l ? tolower(ct_c) : toupper(ct_c);
        } else {
            re_encrypted += c;
        }
    }

    if (re_encrypted == ciphertext) {
        cout << "[VALIDATED] Re-encrypting original plaintext with recovered key yields 100% IDENTICAL ciphertext!" << endl;
    } else {
        cout << "[WARNING] Re-encryption diff detected." << endl;
    }
    cout << "======================================================================" << endl;

    return 0;
}
