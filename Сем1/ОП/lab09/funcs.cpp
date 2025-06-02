#include "header.h"

void Dictionary::init(char new_word_ru[31], char new_word_en[31]) {
    int len_ru = strlen(new_word_ru);
    word_ru = new char[len_ru];
    for (int i = 0; i < len_ru; i++) {
        word_ru[i] = new_word_ru[i];
    }
    int len_en = strlen(new_word_en);
    word_en = new char[len_en];
    for (int i = 0; i < len_en; i++) {
        word_en[i] = new_word_en[i];
    }
}

bool is_equal(char *s1, char *s2){
    if (strlen(s1) != strlen(s2))
        return false;
    for (int i = 0; i < strlen(s1); i++){
        if (s1[i] != s2[i])
            return false;
    }
    return true;
}

bool is_bigger(char *s1, char *s2){
    for (int i = 0; i < 31; i++){
        if (s1[i] > s2[i]){
            return true;
        }
        if (s1[i] < s2[i]){
            return false;
        }
    }
    return false;
}

int menu(){
    int cmd;
    std::cout << "Введите номер команды\n";
    std::cout << "1 - добавление слов в словарь\n";
    std::cout << "2 - удаление слов из словаря\n";
    std::cout << "3 - перевод слов с английского на русский\n";
    std::cout << "4 - перевод слов с русского на английский\n";
    std::cout << "5 - просмотр словаря\n";
    std::cout << "6 - вывод словаря в файл\n";
    std::cout << "7 - выход\n";
    std::cin >> cmd;
    return cmd;
}

void get_words_from_file(Dictionary *dict, int &n) {
    std::ifstream fin("dict.txt");
    char word_en[31], word_ru[31];
    while (fin >> word_ru) {
        fin >> word_en;
        dict[n++].init(word_ru, word_en);
    }
    fin.close();
}


void add_word(Dictionary *dict, int &n){
    char word_ru[31], word_en[31];
    std::cout << "Введите сначала слово на русском языке, затем на английском\n";
    std::cin >> word_ru >> word_en;
    if ((is_bigger(dict[0].word_en, word_en)) || (n == 0)) {
        for (int i = n - 1; i >= 0; i--) {
            dict[i + 1] = dict[i];
        }
        dict[0].init(word_ru, word_en);
    }
    else {
        for (int i = 1; i < n; i++) {
            if (is_bigger(dict[i].word_en, word_en)) {
                for (int j = n - 1; j >= i; j--) {
                    dict[j + 1] = dict[j];
                }
                dict[i].init(word_ru, word_en);
                break;
            }
        }
        if (is_bigger(word_en, dict[n - 1].word_en)) {
            dict[n].init(word_ru, word_en);
        }
    }
    n++;
}

void delete_word(Dictionary *dict, int &n){
    std::cout << "Введите слово на английском, которое хотите удалить\n";
    char del_word[31];
    std::cin >> del_word;
    int num = -1;
    for (int i = 0; i < n; i++) {
        if (is_equal(dict[i].word_en, del_word)) {
            num = i;
            break;
        }
    }
    if (num == -1) {
        std::cout << "Такого слова нет в словаре\n";
    }
    else {
        delete[] dict[num].word_en;
        delete[] dict[num].word_ru;
        for (int i = num; i < n; i++) {
            dict[i] = dict[i + 1];
        }
        n--;
    }
}

void translate_to_ru(Dictionary *dict, const int &n){
    int l = 0, r = n - 1;
    int mid = (l + r) / 2;
    char search_word_en[31];
    std::cout << "Введите искомое слово на английском\n";
    std::cin >> search_word_en;
    while ((!is_equal(dict[mid].word_en, search_word_en)) && (l < r)){
        if (is_bigger(dict[mid].word_en, search_word_en)){
            r = mid;
        }
        else{
            l = mid + 1;
        }
        mid = (l + r) / 2;
    }
    if (is_equal(dict[mid].word_en, search_word_en)) {
        std::cout << "Слово " << search_word_en << " переводится на русский как " << dict[mid].word_ru << '\n';
    }
    else{
        std::cout << "Такого слова нет в словаре\n";
    }
}


void translate_to_en(Dictionary *dict, const int &n){
    char search_word_ru[31];
    std::cout << "Введите искомое слово на русском\n";
    std::cin >> search_word_ru;
    bool is_exist = false;
    for (int i = 0; (i < n) && (!is_exist); i++){
        if (is_equal(dict[i].word_ru, search_word_ru)) {
            std::cout << "Слово " << search_word_ru << " переводится на английский как " << dict[i].word_en << '\n';
            is_exist = true;
        }
    }
    if (!is_exist) {
        std::cout << "Такого слова нет в словаре\n";
    }
}

void print_dict(Dictionary *dict, const int &n){
    for (int i = 0; i < n; i++){
        std::cout << dict[i].word_ru << ' ' << dict[i].word_en << '\n';
    }
}

void output_dict_to_file(Dictionary *dict, const int &n){
    std::ofstream fout("dict.txt");
    for (int i = 0; i < n; i++){
        fout << dict[i].word_ru << ' ' << dict[i].word_en << '\n';
    }
    fout.close();
}
