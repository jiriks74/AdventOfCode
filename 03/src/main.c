#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Error codes:
 * 0 - no error
 * 1 - invalid arguments
 * 2 - memory allocation error
 * 3 - invalid file
 * 4 - invalid file format
 */

/*
 * Finds duplicate character in a given rucksack
 * It finds the duplicates between the first and second compartments
 * (first compartment is the first half of the rucksack, second compartment
 * is the second half of the rucksack)
 *
 * Parameters:
 * - char *str
 *   - The input string
 *
 * Returns:
 * - char
 *   - The duplicate character
 */
char find_dup_char(char *str) {
  int i, j;
  int len = strlen(str);
  char *first_compartment = (char *)malloc(len / 2);
  char *second_compartment = (char *)malloc(len / 2);
  char dup_char;

  // Copy the first half of the rucksack to the first compartment
  // Copy the second half of the rucksack to the second compartment
  for (i = 0; i < len; i++) {
    if (i < len / 2) {
      first_compartment[i] = str[i];
    } else {
      second_compartment[i - len / 2] = str[i];
    }
  }

  // Find the duplicate character
  for (i = 0; i < len / 2; i++) {
    for (j = 0; j < len / 2; j++) {
      if (first_compartment[i] == second_compartment[j]) {
        dup_char = first_compartment[i];

        // Free the memory
        free(first_compartment);
        free(second_compartment);

        return dup_char;
      }
    }
  }

  return '\0';
}

// Returns the score for a given character
int score_char(char c) {
  // If the character is a capital letter
  if (64 < c && c < 91) {
    return c - 38;
  }
  // If the character is a lower case letter
  else if (96 < c && c < 123) {
    return c - 96;
  } else {
    return -1;
  }
}

int score_file(char **filename, int *score) {
  FILE *fp = fopen(*filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file `%s!\n", *filename);
    return 3;
  }

  // Count the number of rucksacks
  // One rucksack per line
  // Ignore the empty lines
  int num_rucksacks = 0;

  // Get the number of rucksacks
  // Number of lines in the file (without last empty line)
  char line[300];

  while (fgets(line, 100, fp) != NULL) {
    if (strcmp(line, "\n") != 0)
      num_rucksacks++;
  }
  rewind(fp);

  // Alocate memory for the temporary rucksack
  char *rucksack = malloc(100 * sizeof(char));
  if (rucksack == NULL) {
    fprintf(stderr, "Error allocating memory for rucksack\n");
    return 2;
  }

  for (int i = 0; i < num_rucksacks; i++) {
    // Get the rucksack
    int res = fscanf(fp, "%s", rucksack);

    if (res == EOF) {
      break;
    }
    // Check for blank line at the end of the file
    else if (res == 1 && strcmp(rucksack, "\n") == 0) {
      i--;
      continue;
    } else if (res != 1) {
      printf("Error: Bad file format.\n");
      // Free the memory
      free(rucksack);
      fclose(fp);
      return 4;
    }

    // Find the duplicate character and score it
    *score += score_char(find_dup_char(rucksack));
  }
  // Free the memory
  fclose(fp);
  free(rucksack);

  return 0;
}

/*
 * NOTE: Part 2
 * ------------------------------------------------------
 */

/* Find badge character
 *
 * Parameters:
 * - char *rucksack 1
 * - char *rucksack 2
 * - char *rucksack 3
 * Returns:
 * - char
 *  - The badge character
 */
char find_badge(char *rucksack1, char *rucksack2, char *rucksack3) {
  for (size_t i = 0; i < strlen(rucksack1); i++) {
    for (size_t j = 0; j < strlen(rucksack2); j++) {
      if (rucksack1[i] == rucksack2[j]) {
        for (size_t k = 0; k < strlen(rucksack3); k++) {
          if (rucksack1[i] == rucksack3[k]) {
            return rucksack1[i];
          }
        }
      }
    }
  }
  return '\0';
}

/* Score badges in a file
 *
 * Parameters:
 * - char **filename
 *   - The name of the file
 *   - The file must have one rucksack per line
 *   - 3 rucksacks are separated by a blank line
 * - int *score
 *   - The score of the badges in the file
 * Returns:
 * - int
 *   - Error code
 */
int score_badges_file(char **filename, int *score) {
  FILE *fp = fopen(*filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file `%s!\n", *filename);
    return 3;
  }

  // Allocate memory for the rucksacks
  char *rucksack1 = malloc(100 * sizeof(char));
  char *rucksack2 = malloc(100 * sizeof(char));
  char *rucksack3 = malloc(100 * sizeof(char));

  // Iterate over the file
  int res = fscanf(fp, "%s\n%s\n%s\n\n", rucksack1, rucksack2, rucksack3);

  while (res != EOF) {
    if (res != 3) {
      fprintf(stderr, "Error: Bad file format.\n");
      // Free the memory
      free(rucksack1);
      free(rucksack2);
      free(rucksack3);
      fclose(fp);
      return 4;
    }

    // Find the badge character
    char badge = find_badge(rucksack1, rucksack2, rucksack3);

    // Score the badge
    *score += score_char(badge);

    res = fscanf(fp, "%s\n%s\n%s\n\n", rucksack1, rucksack2, rucksack3);
  }

  // Free the memory
  free(rucksack1);
  free(rucksack2);
  free(rucksack3);
  fclose(fp);

  return 0;
}

int arg_parser(int argc, char *argv[], char **filename, int *part) {
  if (argc != 3) {
    printf("Usage: %s <file> <part (1,2)>\n", argv[0]);
    return 1;
  }

  // Check if the file exists
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "File %s does not exist\n", argv[1]);
    return 3;
  }
  fclose(fp);

  *filename = argv[1];
  // Check if the part is valid
  if (strcmp(argv[2], "1") != 0 && strcmp(argv[2], "2") != 0) {
    fprintf(stderr, "Invalid part number: %s\n", argv[2]);
    return 1;
  }
  *part = atoi(argv[2]);
  return 0;
}

int main(int argc, char *argv[]) {
  char *filename;
  int part;
  int score = 0;

  int res = arg_parser(argc, argv, &filename, &part);
  if (res != 0) {
    return res;
  }

  if (part == 1) {
    res = score_file(&filename, &score);
    if (res != 0) {
      return res;
    }
  } else {
    res = score_badges_file(&filename, &score);
    if (res != 0) {
      return res;
    }
  }

  printf("Score: %d\n", score);

  return 0;
}
