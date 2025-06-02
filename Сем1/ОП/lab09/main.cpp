#include "header.h"


int main() {
    Dictionary dict[100];
    int word_ind = 0;
    bool run = true;
    get_words_from_file(dict, word_ind);
        while (run){
        switch (menu()) {
            case 1:
                add_word(dict, word_ind);
                break;
            case 2:
                delete_word(dict, word_ind);
                break;
            case 3:
                translate_to_ru(dict, word_ind);
                break;
            case 4:
                translate_to_en(dict, word_ind);
                break;
            case 5:
                print_dict(dict, word_ind);
                break;
            case 6:
                output_dict_to_file(dict, word_ind);
                break;
            case 7:
                run = false;
                break;
            default:
                std::cout << "Введите число от 1 до 7\n";
                break;
        }
    }
    return 0;
}
