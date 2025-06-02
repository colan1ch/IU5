#pragma once
#include <iostream>
#include <fstream>

struct Dictionary{
    char *word_ru;
    char *word_en;
    void init(char new_word_ru[31], char new_word_en[31]);
};

bool is_equal(char *s1, char *s2);

bool is_bigger(char *s1, char *s2);

int menu();

void get_words_from_file(Dictionary *dict, int &n);

void add_word(Dictionary *dict, int &n);

void delete_word(Dictionary *dict, int &n);

void translate_to_ru(Dictionary *dict, const int &n);

void translate_to_en(Dictionary *dict, const int &n);

void print_dict(Dictionary *dict, const int &n);

void output_dict_to_file(Dictionary *dict, const int &n);
