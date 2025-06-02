#include <fstream>
using namespace std;


void setKeys(int* keys, int& kw_count) {
    ifstream fin_kw("keys.txt");
    int key = 0, i = 0;
    char c;
    while (fin_kw.get(c)) {
        if (c == ' ' || c == '\n' || c == char(32)) {
            if (key != 0) {
                keys[i] = key % 256;
                key = 0;
                i++;
                kw_count++;
            }
        }
        else {
            key += (int)c;
        }
    }
    if (key != 0) {
        keys[i] = key % 256;
        kw_count++;
    }
    fin_kw.close();
}

void encoding(int collisions[1000], int count[256][256], int keys[4000], int kw_count) {
    char c, encoded_c;
    int i = 0;
    int ind_col = 0;
    ifstream fin_encoding("input.txt");
    ofstream fout_encoding("encoded.txt");
    while (fin_encoding.get(c)) {
        encoded_c = char((int(c) + keys[i % kw_count] + 128) % 256 - 128);
        count[int(c) + 128][int(encoded_c) + 128]++;
        if (encoded_c != char(26)) {
            fout_encoding << encoded_c;
        }
        else {
            collisions[ind_col++] = i;
            fout_encoding << char(255);
        }
        i++;
    }
    fin_encoding.close();
    fout_encoding.close();
}


void print_table(int count[256][256]) {
    ofstream fout_table("table.txt");
    fout_table << "   ";
    for (int p = 0; p < 256; p++){
        fout_table << setw(6) << p;
    }
    fout_table << '\n';
    for (int p = 0; p < 256; p++){
        fout_table << setw(3) << p;
        for (int k = 0; k < 256; k++) {
            fout_table << setw(6) << count[p][k];
        }
        fout_table << '\n';
    }
    fout_table.close();
}


void decoding(int collisions[1000], int keys[4000], int kw_count) {
    ifstream fin_decoding("encoded.txt");
    ofstream fout_decoding("decoded.txt");
    int i = 0;
    int ind_col = 0;
    char c, encoded_c;
    while (fin_decoding.get(encoded_c)) {
        if (encoded_c == (char)255 && collisions[ind_col] == i) {
            ind_col++;
            c = char(26 - keys[i % kw_count]);
        }
        else {
            c = char((int(encoded_c) - keys[i % kw_count] + 128) % 256 - 128);
        }
        i++;
        fout_decoding << c;
    }
    fin_decoding.close();
    fout_decoding.close();
}


int main() {
    int kw_count = 0;
    int keys[4000];
    setKeys(keys, kw_count);
    int collisions[1000];
    int count[256][256] = {0};
    encoding(collisions, count, keys, kw_count);
    print_table(count);
    decoding(collisions, keys, kw_count);
    return 0;
}
