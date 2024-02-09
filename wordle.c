#define DICTIONARY_FILE "dictionary.txt"
#define WORD_LENGTH 5
#define MAX_TRIES 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void select_random_word(char* word, char* dictionary, int dictionary_words_count) {
  int word_num = rand() % dictionary_words_count;
  for (int i = 0; i < word_num; i++) {
    sscanf(dictionary, "%s", word);
    dictionary += WORD_LENGTH + 1;
  }
}

int check_in_dictionary(char* word, char* dictionary) {
  return strstr(dictionary, word) != NULL;
}

int find_matches(char* input_string, char* answer_string, char *output) {
  int matched_letters = 0;
  int match_class;    // 0 - answer does not contains letter; 
                      // 1 - answer contains letter but not in same place as in input;
                      // 2 - answer contains letter and it's in same place as in input
  for (int i = 0; i < WORD_LENGTH; i++) {
    match_class = 0;
    for (int j = 0; j < WORD_LENGTH; j++) {
      //printf("%c\t-\t%c\n", input_string[i], answer_string[j]);
      if (input_string[i] == answer_string[j]) {
        if (i == j) {
          match_class = 2;
          matched_letters++;
          break;
        } else {
          match_class = 1;
        }
      }
    }
    switch (match_class)
    {
    case 1:
      output[i] = '+';
      break;
    case 2:
      output[i] = '*';
      break;
    default:
      output[i] = '-';
      break;
    }
  }

  if (matched_letters == WORD_LENGTH) {
    return 1;
  } else {
    return 0;
  }
}


int main() {
  char input_word[WORD_LENGTH + 1];
  char answer[WORD_LENGTH + 1] = "test0\0";
  char output[WORD_LENGTH + 1];
  char* dictionary_buffer;
  int tries = 0;
  size_t dictionary_size;

  srand(time(NULL));

  printf("Reading dictionary from %s...\n", DICTIONARY_FILE);
  FILE* dictionary_file_ptr;
  dictionary_file_ptr = fopen(DICTIONARY_FILE, "r");
  if (dictionary_file_ptr == NULL) {
    printf("Unable to read dictionary!\n");
    return 1;
  }
  fseek(dictionary_file_ptr, 0L, SEEK_END);
  dictionary_size = ftell(dictionary_file_ptr);
  fseek(dictionary_file_ptr, 0L, SEEK_SET);

  dictionary_buffer = (char*) calloc(dictionary_size, sizeof(char));

  for (size_t i = 0; i < dictionary_size; i++) {
    dictionary_buffer[i] = fgetc(dictionary_file_ptr);
  }
  printf("Dictionary read succefully!\n");

  select_random_word(answer, dictionary_buffer, dictionary_size / (WORD_LENGTH + 1));


  while (tries < MAX_TRIES) {
    printf("Type word: ");
    scanf("%s", input_word, WORD_LENGTH + 1);
    getchar();
    if (!check_in_dictionary(input_word, dictionary_buffer)) {
      printf("Invalid word!\n");
      continue;
    }
    if (find_matches(input_word, answer, output)) {
      printf("=== You win! ===\n");
      free(dictionary_buffer);
      return 0;
    }
    printf("%s\n", output);
    tries++;
  }
  printf("Game over!\nCorrect answer is %s.\n", answer);
  free(dictionary_buffer);
  return 0;
}