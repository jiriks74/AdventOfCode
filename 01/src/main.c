#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct elf {
  int size;
  int *calories;
};

int get_num_of_elves(const char *fileName) {
  char line[300];
  int emptyLine = 0;
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    printf("Error: Could not open specified file!\n");
    return -1;
  } else {
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
    fclose(fp);
    return ++emptyLine;
  }
}

void load_data(int num_of_elves, char *filename, struct elf **arr) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: Could not open specified file!\n");
    return;
  }
  for (int i = 0; i < num_of_elves; i++) {
    // Count number of lines until blank line
    int num_of_lines = 0;
    char line[300];
    int char_count = 0;
    while (fgets(line, 300, fp)) {
      int i = 0;
      int len = strlen(line);
      char_count += len;
      for (i = 0; i < len; i++) {
        if (line[i] != '\n' && line[i] != '\t' && line[i] != ' ') {
          num_of_lines++;
          break;
        }
      }
      if (line[0] == '\n') {
        break;
      }
    }
    // Check for blank line at EOF
    if(line[0] == EOF) {
      break;
    }

    if (num_of_lines == 0) {
      i--;
      continue;
    }

    // Go back to start of current elf's data
    fseek(fp, -char_count, SEEK_CUR);

    struct elf *data = *arr;
    // Allocate memory for array
    data[i].size = num_of_lines;
    data[i].calories = malloc(num_of_lines * sizeof(int));

    // Load the data
    for (int j = 0; j < num_of_lines; j++) {
      int res = fscanf(fp, "%d", &data[i].calories[j]);
      if (res != 1){
        printf("Error, bad file format!");
        return;
      }
    }
  }
  fclose(fp);
}

void count_elf_calories(struct elf **arr, int num_of_elves){
  struct elf *data = *arr;
  for (int i = 0; i < num_of_elves; i++){
    int sum = 0;
    for (int j = 0; j < data[i].size; j++){
      sum += data[i].calories[j];
      data[i].calories[j] = 0;
    }
    data[i].calories[0] = sum;
  }
}

char *argv_parser(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return NULL;
  }
  return argv[1];
}

int main(int argc, char *argv[]) {
  struct elf *arr;
  char *filename = argv_parser(argc, argv);

  if (filename == NULL) {
    return 1;
  }

  int num_of_elves = get_num_of_elves(filename);
  if (num_of_elves == -1) {
    return 1;
  }

  arr = malloc(num_of_elves * sizeof(struct elf));

  load_data(num_of_elves, filename, &arr);

  count_elf_calories(&arr, num_of_elves);

  // Print 3 elfs carrying the most calories
  int first_elf_cal = 0;
  int first_elf_idx = 0;
  int second_elf_cal = 0;
  int second_elf_idx = 0;
  int third_elf_cal = 0;
  int third_elf_idx = 0;
  for (int i = 0; i < num_of_elves; i++){
    if (arr[i].calories[0] > first_elf_cal){
      third_elf_cal = second_elf_cal;
      third_elf_idx = second_elf_idx;
      second_elf_cal = first_elf_cal;
      second_elf_idx = first_elf_idx;
      first_elf_cal = arr[i].calories[0];
      first_elf_idx = i;
    } else if (arr[i].calories[0] > second_elf_cal){
      third_elf_cal = second_elf_cal;
      third_elf_idx = second_elf_idx;
      second_elf_cal = arr[i].calories[0];
      second_elf_idx = i;
    } else if (arr[i].calories[0] > third_elf_cal){
      third_elf_cal = arr[i].calories[0];
      third_elf_idx = i;
    }
  }
  printf("Elf %d carries the most calories at %d calories\n", first_elf_idx, first_elf_cal);
  printf("Elf %d carries the second most calories at %d calories\n", second_elf_idx, second_elf_cal);
  printf("Elf %d carries the third most calories at %d calories\n", third_elf_idx, third_elf_cal);

  // Print the top 3 elves with the most calories
  printf("The top 3 elves carry %d calories\n", first_elf_cal + second_elf_cal + third_elf_cal);

  // Free memory
  for (int i = 0; i < num_of_elves; i++){
    free(arr[i].calories);
  }
  free(arr);
  return 0;
}
