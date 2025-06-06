#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <chrono>

using namespace std;

string readFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return "";
    }
    string tempstring;
    string result;
    while (getline(file, tempstring)) {
        result += tempstring + "\n";
    }
    file.close();

    return result;
}

void naive_search(const string &book, const string &findstr) {
    for (int i = 0; i < book.size(); i++) {
        if (book.substr(i, findstr.size()) == findstr) {
            cout << i << " ";
        }
    }
}

void find_search(const string &book, const string &findstr) {
    unsigned long long i = 0, last_i = 0;
    while (i < book.size()) {
        last_i = i;
        i = book.find(findstr, i) + 1;
        if (last_i >= i) {
            break;
        }
        cout << i - 1 << " ";
    }
}

void strstr_search(const string &book, const string &findstr) {
    const char* bookptr = book.c_str();
    const char* findptr = findstr.c_str();
    unsigned long long offset = 0;
    while (true) {
        const char* pos = strstr(bookptr + offset, findptr);
        if (!pos) {
            break;
        }
        const long long found_pos = pos - bookptr;
        cout << found_pos << " ";
        offset = found_pos + findstr.size();
    }
}

void prefix_search(const string &book, const string &findstr) {
    const string new_str = findstr + "#" + book;
    vector<int> pi(new_str.length(), 0);
    for (int i = 1; i < new_str.length(); i++) {
        int j = pi[i - 1];
        while (j > 0 && new_str[i] != new_str[j]) {
            j = pi[j - 1];
        }
        if (new_str[i] == new_str[j]) {
            pi[i] = j + 1;
        } else {
            pi[i] = j;
        }
    }
    const unsigned long long t_len = findstr.length();
    for (int i = 0; i < book.length(); i++) {
        if (pi[t_len + 1 + i] == t_len) {
            cout << i - t_len + 1 << " ";
        }
    }
}

long long mod_pow(long long base, long long exponent, const long long mod) {
    long long result = 1;
    base %= mod;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        exponent >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

long long myhash(const string &s) {
    long long hash = 0;
    long long p_pow = 1;
    for (const char c : s) {
        hash = (hash + (static_cast<unsigned char>(c) + 1) * p_pow) % 1000000007;
        p_pow = (p_pow * 257) % 1000000007;
    }
    return hash;
}

void hash_search(const string &book, const string &sub) {
    const unsigned long long n = book.length();
    const unsigned long long m = sub.length();
    const long long hashW = myhash(sub);
    long long hashS = myhash(book.substr(0, m));

    vector<long long> p_pow(max(n, m));
    p_pow[0] = 1;
    for (int i = 1; i < p_pow.size(); ++i)
        p_pow[i] = (p_pow[i - 1] * 257) % 1000000007;

    const long long inv_p = mod_pow(257, 1000000007 - 2, 1000000007);

    for (int i = 0; i <= n - m; ++i) {
        if (hashS == hashW && book.substr(i, m) == sub) {
            cout << i << " ";
        }
        if (i < n - m) {
            const long long old_char = static_cast<unsigned char>(book[i]) + 1;
            const long long new_char = static_cast<unsigned char>(book[i + m]) + 1;

            hashS = (hashS - old_char + 1000000007) % 1000000007;
            hashS = (hashS * inv_p) % 1000000007;
            hashS = (hashS + new_char * p_pow[m - 1]) % 1000000007;
        }
    }
}

typedef void (*func) (const string&, const string&);
struct analyze{
    string name;
    func search;
};


int main() {
    const string text = readFile("../books/Harry Potter and the Chamber of Secrets.txt");

    analyze analyses[5] = {
        {"Naive search", naive_search},
        {"Find search", find_search},
        {"Strstr search", strstr_search},
        {"Prefix search", prefix_search},
        {"Hash search", hash_search}
    };
    string finds[5] = {
        "Harry",
        "I dunno",
        "What's up?",
        "Hermione and Ron",
        "said Professor McGonagall"
    };

    for(const string& findstr: finds){
        for(const auto& [name, search]: analyses){
            cout << name << " (searching for: " << findstr << "):" << endl;

            auto start = chrono::high_resolution_clock::now();
            search(text, findstr);
            auto finish= chrono::high_resolution_clock::now();

            const auto duration = chrono::duration_cast<chrono::microseconds>(finish - start).count();

            cout << endl << "Total time: " << duration << " ms" << endl;
        }
        cout << endl;
    }

    system("pause");

    return 0;
}