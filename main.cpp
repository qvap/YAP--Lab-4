#include <iostream>
#include <fstream>
#include <string.h>

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
    cout << "Naive Search (searching for: " << findstr << "): " << endl;
    for (int i = 0; i < book.size(); i++) {
        if (book.substr(i, findstr.size()) == findstr) {
            cout << i << " ";
        }
    }
}

void find_search(const string &book, const string &findstr) {
    cout << "Find Search (searching for: " << findstr << "): " << endl;
    int i = 0, last_i = 0;
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
    cout << "Strstr Search (searching for: " << findstr << "): " << endl;
    const char* bookptr = book.c_str();
    const char* findptr = findstr.c_str();
    int offset = 0;
    while (true) {
        const char* pos = strstr(bookptr + offset, findptr);
        if (!pos) {
            break;
        }
        const int found_pos = pos - bookptr;
        cout << found_pos << " ";
        offset = found_pos + findstr.size();
    }
}

void prefix_search(const string &book, const string &findstr) {
    return;
}

int main() {
    const string text = readFile("../books/Harry Potter and the Chamber of Secrets.txt");
    const string strfind = "Harry";
    strstr_search(text, strfind);

    system("pause");

    return 0;
}