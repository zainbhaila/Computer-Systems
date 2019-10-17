/*Zain Bhaila
115197541
zainb*/

#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"

#define COM_NAME_LEN 20
#define COM_LINE_LEN 1024

static void is_empty(const char string[], int *flag);
static void process_line(Document * doc, char com_line[]);
static int get_number(char **command, char *num_string, int *num);
static int get_quoted_text(char **command, char *line);
static int get_start(const char *com_line, const char *first_letter,
		     char **command, const char *command_name);

int main(int argc, char *argv[]) {
   FILE *file = NULL;
   Document main_doc = { "main_document", 0 }, *doc = &main_doc;
   char com_line[COM_LINE_LEN + 1] = "";

   if (argc == 1) {             /*run with user input */
      printf("> ");
      while (fgets(com_line, COM_LINE_LEN + 1, stdin) != NULL) {
         process_line(doc, com_line);

         printf("> ");
      }
   }
   else if (argc == 2) {        /*run with file input */
      file = fopen(argv[1], "r");
      if (file == NULL) {
         fprintf(stderr, "%s cannot be opened.\n", argv[1]);
         exit(EX_OSERR);
      }

      while (fgets(com_line, COM_LINE_LEN + 1, file) != NULL) {
         process_line(doc, com_line);
      }
   }
   else {                       /*improper input */
      fprintf(stderr,
              "Usage: user_interface\nUsage: user_interface <filename>\n");
      exit(EX_USAGE);
   }

   return 0;
}

/*process the command passed to the function*/
static void process_line(Document * doc, char com_line[]) {
   int para_num = -1, line_num = -1, flag = 1;
   char asterix = '\0', line[MAX_STR_SIZE + 1] = "",
      para_num_string[MAX_STR_SIZE + 1] = "",
      line_num_string[MAX_STR_SIZE + 1] = "",
      *command = NULL, replace[MAX_STR_SIZE + 1] = "",
      *first_letter = NULL;
   unsigned int i = 0;

   /*check for comment or blank line and get location
      of the first char */
   for (i = 0; i < strlen(com_line); i++) {
      if (!isspace(com_line[i])) {
         flag = 0;
         first_letter = &com_line[i];

         if (com_line[i] == '#') {
            flag = 1;
         }

         break;
      }
   }

   if (flag) {
      return;
   }

   /*add paragraph after */
   if (strstr(com_line, "add_paragraph_after") != NULL) {
      if (get_start(com_line, first_letter, &command, "add_paragraph_after")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get paragraph number */
      if (get_number(&command, para_num_string, &para_num) == -1) {
         printf("Invalid Command\n");
         return;
      }

      is_empty(command, &flag);

      if (para_num < 0 || !flag) {
         printf("Invalid Command\n");
      }
      else {
         if (add_paragraph_after(doc, para_num) == FAILURE) {
            printf("add_paragraph_after failed\n");
         }
      }
   }
   /*add line after */
   else if (strstr(com_line, "add_line_after") != NULL) {
      if (get_start(com_line, first_letter, &command, "add_line_after")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get paragraph number */
      if (get_number(&command, para_num_string, &para_num) == -1) {
         printf("Invalid Command\n");
         return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get line number */
      if (get_number(&command, line_num_string, &line_num) == -1) {
         printf("Invalid Command\n");
         return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*check asterix */
      sscanf(command, " %c", &asterix);
      if (asterix != '*') {
         printf("Invalid Command\n");
         return;
      }
      command = strstr(command, "*") + 1;

      /*get line */
      strcat(line, command);

      if (para_num <= 0 || line_num < 0) {
         printf("Invalid Command\n");
      }
      else {
         if (add_line_after(doc, para_num, line_num, line) == FAILURE) {
            printf("add_line_after failed\n");
         }
      }
   }
   /*print document */
   else if (strstr(com_line, "print_document") != NULL) {
      if (get_start(com_line, first_letter, &command, "print_document")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      is_empty(command, &flag);

      if (!flag) {
         printf("Invalid Command\n");
      }
      else {
         print_document(doc);
      }
   }
   /*quit and exit */
   else if (strstr(com_line, "quit") != NULL ||
            strstr(com_line, "exit") != NULL) {
      if (strstr(com_line, "quit") != NULL) {
         if (get_start(com_line, first_letter, &command, "quit")
	     == FAILURE) {
	    printf("Invalid Command\n");
	    return;
	 }
      }
      else {
         if (get_start(com_line, first_letter, &command, "exit")
	     == FAILURE) {
	    printf("Invalid Command\n");
	    return;
	 }
      }

      is_empty(command, &flag);

      if (!flag) {
         printf("Invalid Command\n");
      }
      else {
         exit(EXIT_SUCCESS);
      }
   }
   /*append line */
   else if (strstr(com_line, "append_line") != NULL) {
      if (get_start(com_line, first_letter, &command, "append_line")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get paragraph number */
      if (get_number(&command, para_num_string, &para_num) == -1) {
         printf("Invalid Command\n");
         return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*check asterix */
      sscanf(command, " %c", &asterix);
      if (asterix != '*') {
         printf("Invalid Command\n");
         return;
      }
      command = strstr(command, "*") + 1;

      /*get line */
      strcat(line, command);

      if (para_num <= 0) {
         printf("Invalid Command\n");
      }
      else {
         if (append_line(doc, para_num, line) == FAILURE) {
            printf("append_line failed\n");
         }
      }
   }
   /*remove line */
   else if (strstr(com_line, "remove_line") != NULL) {
      if (get_start(com_line, first_letter, &command, "remove_line")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get paragraph number */
      if (get_number(&command, para_num_string, &para_num) == -1) {
         printf("Invalid Command\n");
         return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get line number */
      if (get_number(&command, line_num_string, &line_num) == -1) {
         printf("Invalid Command\n");
         return;
      }

      is_empty(command, &flag);

      if (para_num <= 0 || line_num <= 0 || !flag) {
         printf("Invalid Command\n");
      }
      else {
         if (remove_line(doc, para_num, line_num) == FAILURE) {
            printf("remove_line failed\n");
         }
      }
   }
   /*load file */
   else if (strstr(com_line, "load_file") != NULL) {
      if (get_start(com_line, first_letter, &command, "load_file")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get file name */
      sscanf(command, " %s", line);
      command = strstr(command, line) + strlen(line);

      is_empty(command, &flag);

      if (line[0] == '\0' || !flag) {
         printf("Invalid Command\n");
      }
      else {
         if (load_file(doc, line) == FAILURE) {
            printf("load_file failed\n");
         }
      }
   }
   /*replace text */
   else if (strstr(com_line, "replace_text") != NULL) {
      if (get_start(com_line, first_letter, &command, "replace_text")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get target */
      if (get_quoted_text(&command, line) == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }
      
      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get replacement */
      if (get_quoted_text(&command, replace) == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (line[0] == '\0') {
         printf("Invalid Command\n");
      }
      else {
         if (replace_text(doc, line, replace) == FAILURE) {
            printf("replace_text failed\n");
         }
      }
   }
   /*highlight text */
   else if (strstr(com_line, "highlight_text") != NULL) {
      if (get_start(com_line, first_letter, &command, "highlight_text")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get target */
      if (get_quoted_text(&command, line) == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (line[0] == '\0') {
         printf("Invalid Command\n");
      }
      else {
         highlight_text(doc, line);
      }
   }
   /*remove text */
   else if (strstr(com_line, "remove_text") != NULL) {
      if (get_start(com_line, first_letter, &command, "remove_text")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get target */
      if (get_quoted_text(&command, line) == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (line[0] == '\0') {
         printf("Invalid Command\n");
      }
      else {
         remove_text(doc, line);
      }
   }
   /*save document */
   else if (strstr(com_line, "save_document") != NULL) {
      if (get_start(com_line, first_letter, &command, "save_document")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      if (!isspace(*command)) {
         printf("Invalid Command\n");
         return;
      }
      else {
         command += 1;
      }

      /*get file name */
      sscanf(command, " %s", line);
      command = strstr(command, line) + strlen(line);

      is_empty(command, &flag);

      if (line[0] == '\0' || !flag) {
         printf("Invalid Command\n");
      }
      else {
         if (save_document(doc, line) == FAILURE) {
            printf("save_document failed\n");
         }
      }
   }
   /*reset document */
   else if (strstr(com_line, "reset_document") != NULL) {
      if (get_start(com_line, first_letter, &command, "reset_document")
	  == FAILURE) {
	 printf("Invalid Command\n");
	 return;
      }

      is_empty(command, &flag);

      if (!flag) {
         printf("Invalid Command\n");
      }
      else {
         reset_document(doc);
      }
   }
   /*invalid command */
   else {
      printf("Invalid Command\n");
   }
}

/*gets a number from a string*/
static int get_number(char **command, char *num_string, int *num) {
   int i = 0;

   sscanf(*command, " %s", num_string);
   *command = strstr(*command, num_string)
      + strlen(num_string);

   for (i = 0; i < strlen(num_string); i++) {
      if (!isdigit(num_string[i])) {
         return -1;
      }
   }

   *num = atoi(num_string);
   return *num;
}

 /*checking for blank lines or comments */
static void is_empty(const char string[], int *const flag) {
   unsigned int i = 0;

   *flag = 1;

   for (i = 0; i < strlen(string); i++) {
      if (!isspace(string[i])) {
         *flag = 0;

         if (string[i] == '#') {
            *flag = 1;
         }

         break;
      }
   }
}

/*get text within quotes*/
static int get_quoted_text(char **command, char *line) {
   char quote = '\0', *end_quote = NULL;
   
   sscanf(*command, " %c", &quote);
   *command = strstr(*command, "\"") + 1;

   if (quote != '"') {
      return FAILURE;
   }

   end_quote = strstr(*command, "\"");
   if (end_quote == NULL) {
      return FAILURE;
   }
   strncpy(line, *command, end_quote - *command);

   *command = end_quote + 1;

   return SUCCESS;
}

/*gets the start of the command*/
static int get_start(const char *com_line, const char *first_letter,
		     char **command, const char *command_name){
   *command = strstr(com_line, command_name);
   if (*command != first_letter) {
      return FAILURE;
   }
   *command += strlen(command_name);

   return SUCCESS;
}
