#include <stdio.h>
#include <stdlib.h>

/* Error codes:
 * 0 - no error
 * 1 - invalid number of arguments
 * 2 - invalid file
 * 3 - invalid file format
 * 4 - invalid solution part
 */

/* NOTE: info to remember:
 * The aliases for rock, paper, scissors are:
 * - A = X = rock
 * - B = Y = paper
 * - C = Z = scissors
 * ------------------------------------------
 * Scores:
 * - Shape
 *   - Rock: 1
 *   - Paper: 2
 *   - Scissors: 3
 * - Outcome
 *   - Win: 6
 *   - Draw: 3
 *   - Loss: 0
 */

int arg_parser(int argc, char *argv[], char **file_name, int *solution_part) {
  if (argc != 3) {
    printf("Usage: %s <file> <solution_part (1/2)>\n", argv[0]);
    return 1;
  }
  // Check if the file exists
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Error: File does not exist.\n");
    return 2;
  }
  fclose(file);

  // Check if the solution part is a valid number
  if (atoi(argv[2]) != 1 && atoi(argv[2]) != 2) {
    printf("Error: Invalid solution part.\nValid values are [1, 2]\n");
    return 4;
  }

  *file_name = argv[1];
  *solution_part = atoi(argv[2]);

  return 0;
}

// Returns the score of the given shape
// 1 = rock, 2 = paper, 3 = scissors
int score_shapes(char shape) {
  switch (shape) {
  case 'A':
  case 'X':
    return 1;
  case 'B':
  case 'Y':
    return 2;
  case 'C':
  case 'Z':
    return 3;
  default:
    return -1;
  }
}

/* Returns my score for the round
 *
 * Parameters:
 * - opponents_shape
 *   - A = rock
 *   - B = paper
 *   - C = scissors
 * - my_shape
 *   - X = rock
 *   - Y = paper
 *   - Z = scissors
 */
int round_score(char opponents_shape, char my_shape) {
  // Checks if the shapes are valid and get their scores
  int opponents_score = score_shapes(opponents_shape);
  int shape_score = score_shapes(my_shape);
  if (opponents_score == -1 || shape_score == -1) {
    printf("Error: Invalid shape.\n");
    return -1;
  }

  // Draw
  if (opponents_shape == 'A' && my_shape == 'X') {
    return 3 + shape_score;
  } else if (opponents_shape == 'B' && my_shape == 'Y') {
    return 3 + shape_score;
  } else if (opponents_shape == 'C' && my_shape == 'Z') {
    return 3 + shape_score;
  }

  // Rock win
  else if (my_shape == 'X' && opponents_shape == 'C') {
    return 6 + shape_score;
  }
  // Paper win
  else if (my_shape == 'Y' && opponents_shape == 'A') {
    return 6 + shape_score;
  } // Scissors win
  else if (my_shape == 'Z' && opponents_shape == 'B') {
    return 6 + shape_score;
  } else {
    return shape_score;
  }
}

int score_file(char **file_name) {
  FILE *file = fopen(*file_name, "r");
  if (file == NULL) {
    printf("Error opening the file.\n");
    return -2;
  }

  int my_score = 0;

  // Read the file
  while (!feof(file)) {
    char opponent[1];
    char me[1];
    int res = fscanf(file, "%s %s", opponent, me);

    if (res == EOF) {
      break;
    }
    // Check for blank line at the end of the file
    else if (res == 1 && opponent[0] == '\n') {
      continue;
    } else if (res != 2) {
      printf("Error: Bad file format.\n");
      return -3;
    }

    my_score += round_score(*opponent, *me);
  }
  fclose(file);

  return my_score;
}

/* NOTE:
 * ------------------------------------------------------------
 * PART 2 OF THE SOLUTION
 * ------------------------------------------------------------
 */

char *get_win_shape_part2(char *opponents_shape) {
  switch (*opponents_shape) {
  case 'A':
    return "B";
  case 'B':
    return "C";
  case 'C':
    return "A";
  default:
    return "0";
  }
}

// Returns the shape that looses against the given shape
//
// Parameters:
// - opponents_shape
//  - A = rock
//  - B = paper
//  - C = scissors
char *get_loss_shape_part2(char *opponents_shape) {
  switch (*opponents_shape) {
  case 'A':
    return "C";
  case 'B':
    return "A";
  case 'C':
    return "B";
  default:
    return "0";
  }
}

/* Returns the score for a given round
 *
 * Parameters:
 * - opponents_shape
 *   - A = rock
 *   - B = paper
 *   - C = scissors
 * - round_result
 *   - X = loss
 *   - Y = draw
 *   - Z = win
 */
int score_round_part_2(char *opponents_shape, char *round_result) {
  if (*round_result == 'X') {
    char *loss_shape = get_loss_shape_part2(opponents_shape);
    return score_shapes(*loss_shape);
  } else if (*round_result == 'Y') {
    // I have the same shape as the opponent
    return 3 + score_shapes(*opponents_shape);
  } else if (*round_result == 'Z') {
    char *win_shape = get_win_shape_part2(opponents_shape);
    return 6 + score_shapes(*win_shape);
  } else {
    return -1;
  }
}

// score_part2
int score_file_part2(char **file_name, int *score) {
  FILE *file = fopen(*file_name, "r");
  if (file == NULL) {
    printf("Error opening the file.\n");
    return 2;
  }

  // int score = 0;

  // Read the file
  while (!feof(file)) {
    char opponent;
    char round_res;
    int res = fscanf(file, "%s %s", &opponent, &round_res);

    if (res == EOF) {
      break;
    }
    // Check for blank line at the end of the file
    else if (res == 1 && opponent == '\n') {
      continue;
    } else if (res != 2) {
      printf("Error: Bad file format.\n");
      return 3;
    }

    *score += score_round_part_2(&opponent, &round_res);
  }
  fclose(file);

  // return score;
  return 0;
}

int main(int argc, char *argv[]) {
  char *file_name;
  int solution_part;
  int score = 0;

  int res = arg_parser(argc, argv, &file_name, &solution_part);
  if (res != 0) {
    return res;
  }
  if (solution_part == 1) {
    printf("My score: %d\n", score_file(&file_name));
  } else {
    int res = score_file_part2(&file_name, &score);
    if (res != 0) {
      return res;
    }
    printf("My score: %d\n", score);
  }

  return 0;
}
