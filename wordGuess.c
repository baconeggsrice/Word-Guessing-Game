#include <stdio.h>
#include <string.h>
#include <stdlib.h>
const char * const GREEN = "\x1b[32m";
const char * const YELLOW = "\x1b[33m";
const char * const WHITE = "\x1b[0m";
const char * current = WHITE;

void setColour(const char *colour) {
  if (current == colour) return;
  printf("%s", colour);
  current = colour;
}

//Get the word guess
char* guess() {
  char *buffer;
  size_t bufferSize = 13;
  size_t characters;
  printf("Enter guess: ");
  buffer = (char *)malloc(bufferSize * sizeof(char));
  characters = getline(&buffer,&bufferSize,stdin);
  if (characters == -1) {
    printf("Getline didn't work");
  }
  return buffer;
}

int *compare(char *word, char *guessW, const int wordlen) {
  //Create two arrays, one to compare with and one to hold the values.
  // vals[13] = {0}, enter 1 if green, 2 if yellow, 0 if white.
  // compare for green first, then compare for yellow.
  // comparing for yellow, only swtich to 2 if not in the position.
  // use a nested loop to iterate through the ans array twice.
  // for yellow, start at the first position, and check if the letter is in the word
  // then check the rest of the letters to see if its already been used.
  // make flags array only 0 and 1. 1 if used, 0 if not.
  int flags[13] = {0}; // Flag array, initialize values to zero
  int *vals = calloc(wordlen, sizeof(int)); // Clue arr, initialize values to zero.
  //Green Iteration
  // THIS WORKS FINE
  for (int i = 0; i < wordlen; i++) {
    if (guessW[i] == word[i]) {
      vals[i] = 1; //Set to 1 if green
      flags[i] = 1; //Used the letter, set flag to used (1)
    }
  }

  //Yellow iteration
  for (int i = 0; i < wordlen; i++) {
    if (vals[i] == 0) {
      for (int j = 0; j < wordlen; j++) {
        if (guessW[i] == word[j] && !flags[j]) { //Triggers if flag that is being used isnt used
          vals[i] = 2;
          flags[j] = 1;
          break;
        }
      }
    }
  }
  return vals;
}

void giveClue(int *vals, char*guessW, const int wordlen) {
  for (int i = 0; i < wordlen; i++) {
    if (vals[i] == 2) { //Set yellow color
      setColour(YELLOW);
    } else if (vals[i] == 1) { //Set green
      setColour(GREEN);
    } else {
      setColour(WHITE);
    }
    printf("%c", guessW[i]);
  }
  printf("\n");
  setColour(WHITE);
}

void wordl(const int wordlen, char *word) {
  int counter = 1;
  while(counter < 7) {
    char *guessW = guess();
    //printf("guess: %s", guessW);
    //int n = (strcmp(word, guessW));
    //printf("strcmp: %d", n);
    int *vals = compare(word, guessW, wordlen); //Only testing returning the func
    //Give clue
    giveClue(vals, guessW,wordlen);
    //Compare the input word and guess word
    if ((strcmp(word, guessW)) == 0) {
      printf("Finished in %d guesses\n", counter);
      break;
    }
    counter++;
    //Compare the inputs with the word.
    free(guessW);
    free(vals);
  }
  if (counter == 7) {
    printf("Failed to guess the word: %s", word);
  }
}

int main(int argc, const char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of command line arguments\n");
  } else {
    const int wordlen = strlen(argv[1]);
    if (wordlen>12) {
        fprintf(stderr, "Invalid Input");
        return 1;
    }
    char word[13];
    for (int i = 0; i < wordlen; ++i) {
      word[i] = argv[1][i];
    }
    word[wordlen+1] = '\0';
    word[wordlen] = '\n';
    wordl(wordlen, word);
  }
  return 0;
}
