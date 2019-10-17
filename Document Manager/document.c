/*Zain Bhaila
115197541
zainb*/

#include "document.h"
#include <string.h>
#include <stdio.h>

/*create a blank document*/
int init_document(Document * doc, const char *name) {
   if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
      return FAILURE;
   }

   strcpy(doc->name, name);
   doc->number_of_paragraphs = 0;

   return SUCCESS;
}

/*clear a document*/
int reset_document(Document * doc) {
   if (doc == NULL) {
      return FAILURE;
   }

   doc->number_of_paragraphs = 0;

   return SUCCESS;
}

/*print out a documents info and paragraphs*/
int print_document(Document * doc) {
   int i = 0, j = 0;

   if (doc == NULL) {
      return FAILURE;
   }

   printf("Document name: \"%s\"\n", doc->name);
   printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

   for (i = 0; i < doc->number_of_paragraphs; i++) {
      for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
         printf("%s\n", doc->paragraphs[i].lines[j]);
      }

      printf("\n");
   }

   return SUCCESS;
}

/*add a new paragraph to a document*/
int add_paragraph_after(Document * doc, int paragraph_number) {
   Paragraph new_paragraph;
   int i = 0;

   if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS ||
       paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   }

   new_paragraph.number_of_lines = 0;

   for (i = doc->number_of_paragraphs; i > paragraph_number; i++) {
      doc[i] = doc[i - 1];
   }

   doc->paragraphs[paragraph_number] = new_paragraph;
   doc->number_of_paragraphs++;

   return SUCCESS;
}

/*adds a new line to a paragraph*/
int add_line_after(Document * doc, int paragraph_number,
                   int line_number, const char *new_line) {
   int i = 0;

   if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
       doc->paragraphs[paragraph_number - 1].number_of_lines
       == MAX_PARAGRAPH_LINES ||
       line_number > doc->paragraphs[paragraph_number - 1].number_of_lines
       || new_line == NULL) {
      return FAILURE;
   }

   for (i = doc->paragraphs[paragraph_number - 1].number_of_lines;
        i > line_number; i++) {
      strcpy(doc->paragraphs[paragraph_number - 1].lines[i],
             doc->paragraphs[paragraph_number - 1].lines[i - 1]);
   }

   strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
   doc->paragraphs[paragraph_number - 1].number_of_lines++;

   return SUCCESS;
}

/*return the lines in a paragraph*/
int get_number_lines_paragraph(Document * doc, int paragraph_number,
                               int *number_of_lines) {
   if (doc == NULL || number_of_lines == NULL ||
       paragraph_number > doc->number_of_paragraphs) {
      return FAILURE;
   }

   *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

   return SUCCESS;
}

/*adds a line to the end of the paragraph*/
int append_line(Document * doc, int paragraph_number, const char *new_line) {
   if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
       doc->paragraphs[paragraph_number - 1].number_of_lines
       == MAX_PARAGRAPH_LINES || new_line == NULL) {
      return FAILURE;
   }

   strcpy(doc->paragraphs[paragraph_number - 1]
          .lines[doc->paragraphs[paragraph_number - 1].number_of_lines],
          new_line);
   doc->paragraphs[paragraph_number - 1].number_of_lines++;

   return SUCCESS;
}

/*remove a line from a paragraph*/
int remove_line(Document * doc, int paragraph_number, int line_number) {
   int i = 0;

   if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
       line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
      return FAILURE;
   }

   strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number - 1], "");

   for (i = line_number;
        i < doc->paragraphs[paragraph_number - 1].number_of_lines; i++) {
      strcpy(doc->paragraphs[paragraph_number - 1].lines[i - 1],
             doc->paragraphs[paragraph_number - 1].lines[i]);
   }

   doc->paragraphs[paragraph_number - 1].number_of_lines--;

   return SUCCESS;
}

/*add data lines the document*/
int load_document(Document * doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
   int i = 0;
   int paragraphs_added = 1;

   if (doc == NULL || data == NULL || data_lines == 0) {
      return FAILURE;
   }

   add_paragraph_after(doc, 0);

   for (i = 0; i < data_lines; i++) {
      if (strlen(data[i]) == 0) {
         add_paragraph_after(doc, paragraphs_added);
         paragraphs_added++;
      } else {
         append_line(doc, paragraphs_added, data[i]);
      }
   }

   return SUCCESS;
}

/*search and replace all target values*/
int replace_text(Document * doc, const char *target, const char *replacement) {
   char *found = NULL, copy[MAX_STR_SIZE + 1] = "";
   int loc = 0;
   int i = 0, j = 0;

   if (doc == NULL || target == NULL || replacement == NULL) {
      return FAILURE;
   }

   for (i = 0; i < doc->number_of_paragraphs; i++) {
      for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
         found = strstr(doc->paragraphs[i].lines[j], target);

         while (found != NULL) {
            loc = found - doc->paragraphs[i].lines[j];

            strncat(&copy[0], doc->paragraphs[i].lines[j], loc);
            strcat(&copy[0], replacement);
            strcat(&copy[0],
                   doc->paragraphs[i].lines[j] + strlen(target) + loc);

            strcpy(doc->paragraphs[i].lines[j], &copy[0]);

            strcpy(&copy[0], "");

            found = strstr(doc->paragraphs[i].lines[j]
                           + loc + strlen(replacement), target);
         }
      }
   }

   return SUCCESS;
}

/*highlight all target values*/
int highlight_text(Document * doc, const char *target) {
   char replacement[MAX_STR_SIZE + 1] = HIGHLIGHT_START_STR;

   strcat(&replacement[0], target);
   strcat(&replacement[0], HIGHLIGHT_END_STR);

   return replace_text(doc, target, replacement);
}

/*remove all target values*/
int remove_text(Document * doc, const char *target) {
   return replace_text(doc, target, "");
}
