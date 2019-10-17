/*
Zain Bhaila
115197541
zainb
*/

#include <stdio.h>
#include <string.h>
#include <math.h>

/*swaps two integer values*/
static void swap(int *a, int *b) {
   int tmp = *a;

   *a = *b;
   *b = tmp;
}

/*bubble sort for assignment info*/
static void bubble(int assignment[], int score[],
                   int weight[], int late[], int num_assign) {
   int i;

   for (i = 0; i < num_assign - 1; i++) {
      int j;

      for (j = 0; j < num_assign - 1 - i; j++) {
         if (assignment[j] > assignment[j + 1] || assignment[j] == 0) {
            /*swap values of all arrays */
            swap(&assignment[j], &assignment[j + 1]);
            swap(&score[j], &score[j + 1]);
            swap(&weight[j], &weight[j + 1]);
            swap(&late[j], &late[j + 1]);

            if (assignment[j] == 0) {
               j--;
            }
         }
      }
   }
}

int main() {
#define MAX_ASSIGN 50
#define CHAR_PER_LINE 18

   int i;

   /*user input */
   int penalty = 0, dropped = 0, num_assign = 0;
   char statistics = 'n';

   /*store assignment info */
   int assignment[MAX_ASSIGN] = { 0 }, score[MAX_ASSIGN] = {
   0}, weight[MAX_ASSIGN] = {
   0}, late[MAX_ASSIGN] = {
   0};
   int assignment_removed[MAX_ASSIGN] = { 0 }, score_removed[MAX_ASSIGN] = {
   0}, weight_removed[MAX_ASSIGN] = {
   0}, late_removed[MAX_ASSIGN] = {
   0};

   /*used to store a string of assignment info */
   char orig_values[MAX_ASSIGN * CHAR_PER_LINE + 1] = "";

   /*for removing dropped grades */
   int assign_temp = 0;
   int total_weight = 0, minimum = 0;

   /*results */
   double numeric_score = 0, mean = 0, std_dev = 0;
   double numerator = 0;

   scanf(" %d", &penalty);
   scanf(" %d", &dropped);
   scanf(" %c", &statistics);
   scanf(" %d", &num_assign);

   for (i = 0; i < num_assign; i++) {
      scanf(" %d, %d, %d, %d",
            &assignment[i], &score[i], &weight[i], &late[i]);
   }

   /*ensure weights add to 100 */
   for (i = 0; i < num_assign; i++) {
      total_weight += weight[i];
   }
   if (total_weight != 100) {
      printf("ERROR: Invalid values provided\n");
      return 0;
   }

   /*create a string that has all the input values */
   bubble(assignment, score, weight, late, num_assign);
   for (i = 0; i < num_assign; i++) {
      char new_values[CHAR_PER_LINE + 1] = "";

      sprintf(new_values, "%d, %d, %d, %d\n",
              assignment[i], score[i], weight[i], late[i]);

      strcat(orig_values, new_values);
   }

   /*create a copy array to remove values from for calculations */
   for (i = 0; i < num_assign; i++) {
      assignment_removed[i] = assignment[i];
      score_removed[i] = score[i];
      weight_removed[i] = weight[i];
      late_removed[i] = late[i];
   }

   /*eliminate lowest values */
   assign_temp = num_assign;
   for (i = 0; i < dropped; i++) {
      int j;
      minimum = 0;

      for (j = 0; j < assign_temp; j++) {
         if (score_removed[j] * weight_removed[j]
             < score_removed[minimum] * weight_removed[minimum]) {
            minimum = j;
         }
      }
      total_weight -= weight_removed[minimum];

      /*shift values down in array */
      for (j = minimum; j < num_assign; j++) {
         assignment_removed[j] = assignment_removed[j + 1];
         score_removed[j] = score_removed[j + 1];
         weight_removed[j] = weight_removed[j + 1];
         late_removed[j] = late_removed[j + 1];
      }

      assign_temp--;
   }

   /*calculate numeric score */
   for (i = 0; i < assign_temp; i++) {
      double zero_check = (double) score_removed[i]
         - (late_removed[i] * penalty);
      if (zero_check < 0) {
         zero_check = 0;
      }

      numerator += zero_check * weight_removed[i];
   }
   numeric_score = numerator / ((double) total_weight);

   printf("Numeric Score: %5.4f\n", numeric_score);
   printf("Points Penalty Per Day Late: %d\n", penalty);
   printf("Number of Assignments Dropped: %d\n", dropped);
   printf("Values Provided:\n");
   printf("Assignment, Score, Weight, Days Late\n%s", orig_values);

   /*if statistics are to be calculated */
   if (statistics == 'Y' || statistics == 'y') {
      double std_dev_sum = 0;

      /*calculate mean */
      for (i = 0; i < num_assign; i++) {
         double zero_check = (double) score[i] - (late[i] * penalty);
         if (zero_check < 0) {
            zero_check = 0;
         }

         mean += zero_check;
      }
      mean = mean / num_assign;

      /*calculate standard deviation */
      for (i = 0; i < num_assign; i++) {
         double zero_check = (double) score[i] - (late[i] * penalty);
         if (zero_check < 0) {
            zero_check = 0;
         }

         std_dev_sum += pow(zero_check - mean, 2);
      }
      std_dev = sqrt(std_dev_sum / ((double) num_assign));

      printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, std_dev);
   }

   return 0;
}
