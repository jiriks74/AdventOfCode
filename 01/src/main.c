#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct elf {
  int size;
  int *calories;
};

// Gets the number of elves in the input file.
int get_num_of_elves(const char *fileName) {
  // Max line length
  char line[300];
  // Stores the number of empty lines (elves)
  int emptyLine = 0;
  // Open the file
  FILE *fp = fopen(fileName, "r");

  // Check if the file opened successfully
  if (fp == NULL) {
    printf("Error: Could not open specified file!\n");
    return -1;
  }

  // Read the file and count blank lines
  while (fgets(line, 300, fp)) {
    int i = 0;
    int len = strlen(line);
    emptyLine++;
    for (i = 0; i < len; i++) {
      if (line[i] != '\n' && line[i] != '\t' && line[i] != ' ') {
        emptyLine--;
        break;
      }
    }
  }
  // Check if the last line in the file is empty
  if (line[0] == '\n') {
    emptyLine--;
  }
  
  // Close the file
  fclose(fp);

  // Return the number of elves (one elf won't be counted if there is a blank line at the end of the file)
  return ++emptyLine;
}

/*
 * Loads the elve's carried calories from the input file.
 * Params:
 * - int num_of_elves: The number of elves in the input file.
 * - const char *fileName: The name of the input file.
 * - struct elf *elves: The array of elves to store the data in.
 */
void load_data(int num_of_elves, char *filename, struct elf **arr) {
  // Open the file
  FILE *fp = fopen(filename, "r");
  
  // Check if the file opened successfully
  if (fp == NULL) {
    printf("Error: Could not open specified file!\n");
    return;
  }

  // Go through each elf
  for (int i = 0; i < num_of_elves; i++) {
    // Count number of lines until blank line
    int num_of_lines = 0;
    // Stores the current line (max length 300)
    char line[300];
    /* 
     * NOTE: The code below is used to get the number of items does the elf have
     * so that the memory can be allocated corretly. But then wee need to go back
     * to the start of the elve's data and we need the number of characters we read
     * to do that
     */
    // Counts the number of characters
    int char_count = 0;

    // Go through the elve's data
    while (fgets(line, 300, fp)) {
      // Length of the current line
      int len = strlen(line);
      // Update character count
      char_count += len;

      // Go through the line until a new line character is found
      for (int i = 0; i < len; i++) {
        if (line[i] != '\n' && line[i] != '\t' && line[i] != ' ') {
          num_of_lines++;
          break;
        }
      }
      // Check if the line is empty
      if (line[0] == '\n') {
        break;
      }
    }

    // Check for blank line at EOF (or you'll get an infinit loop if the last line is blank LOL)
    if (line[0] == EOF) {
      break;
    }

    // If the line is empty, go back to the start of the next elve
    if (num_of_lines == 0) {
      i--;
      continue;
    }

    // Go back to start of current elf's data
    fseek(fp, -char_count, SEEK_CUR);

    // Reference the array
    struct elf *data = *arr;
    // Allocate memory for array
    data[i].calories = malloc(num_of_lines * sizeof(int));
    // Remember how many calories are stored
    data[i].size = num_of_lines;

    // Load the data
    for (int j = 0; j < num_of_lines; j++) {
      // Read the line ans add the calories to the correct elf in the array
      int res = fscanf(fp, "%d", &data[i].calories[j]);
      // Check if the line has the correct formatting
      if (res != 1) {
        printf("Error, bad file format!");
        return;
      }
    }
  }

  // Close the file
  fclose(fp);
}

// Count all the calories each elf has
// Params:
// - int num_of_elves: The number of elves in the input file.
// - struct elf *elves: The array with the data.
void count_elf_calories(struct elf **arr, int num_of_elves) {
  // Reference the array
  struct elf *data = *arr;

  // Go through each elf
  for (int i = 0; i < num_of_elves; i++) {
    // Sum of the callories for the current elf
    int sum = 0;

    // Go through the elve's calories
    for (int j = 0; j < data[i].size; j++) {
      // Add the calories to the sum
      sum += data[i].calories[j];
      // Remove the callories and decrease the stored calories number
      data[i].calories[j] = 0;
    }
    // Set the sum to indes 0
    data[i].calories[0] = sum;
    data[i].size = 1;
  }
}

// Argument parser
char *argv_parser(int argc, char *argv[]) {
  // This program has only one argument
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return NULL;
  }

  // Return the argument as pos 1 (the filename)
  return argv[1];
}

int main(int argc, char *argv[]) {
  // The array of elves
  struct elf *arr;
  // Get the filename
  char *filename = argv_parser(argc, argv);

  // Get the number of elves in the file
  int num_of_elves = get_num_of_elves(filename);
  // Check if the operation was successful
  if (num_of_elves == -1) {
    return 1;
  }

  // Alocate the memory for the array
  arr = malloc(num_of_elves * sizeof(struct elf));

  // Load all the elve's calorias
  load_data(num_of_elves, filename, &arr);

  // Count all the calories each elf has
  count_elf_calories(&arr, num_of_elves);

  // Get 3 elves with the most calories
  int first_elf_cal = 0;
  int first_elf_idx = 0;
  int second_elf_cal = 0;
  int second_elf_idx = 0;
  int third_elf_cal = 0;
  int third_elf_idx = 0;
  for (int i = 0; i < num_of_elves; i++) {
    // If the elf has more calories than others, move the other elves down
    if (arr[i].calories[0] > first_elf_cal) {
      third_elf_cal = second_elf_cal;
      third_elf_idx = second_elf_idx;
      second_elf_cal = first_elf_cal;
      second_elf_idx = first_elf_idx;
      first_elf_cal = arr[i].calories[0];
      first_elf_idx = i;
    } else if (arr[i].calories[0] > second_elf_cal) {
      third_elf_cal = second_elf_cal;
      third_elf_idx = second_elf_idx;
      second_elf_cal = arr[i].calories[0];
      second_elf_idx = i;
    } else if (arr[i].calories[0] > third_elf_cal) {
      third_elf_cal = arr[i].calories[0];
      third_elf_idx = i;
    }
  }

  // Print out the 3 elves with the most calories
  printf("Elf %d carries the most calories at %d calories\n", first_elf_idx,
         first_elf_cal);
  printf("Elf %d carries the second most calories at %d calories\n",
         second_elf_idx, second_elf_cal);
  printf("Elf %d carries the third most calories at %d calories\n",
         third_elf_idx, third_elf_cal);

  // Print the top 3 elves with the most calories
  printf("The top 3 elves carry %d calories\n",
         first_elf_cal + second_elf_cal + third_elf_cal);

  // Free memory
  for (int i = 0; i < num_of_elves; i++) {
    free(arr[i].calories);
  }
  // Free the array
  free(arr);

  return 0;
}
