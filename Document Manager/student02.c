#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Exercise Description";
   int paragraph_number, line_number;

   init_document(&doc, doc_name);

   /* Adding paragraph */
   paragraph_number = 0;
   add_paragraph_after(&doc, paragraph_number);

   /* Adding lines to paragraph */
   paragraph_number = 1;

   /* First line */
   line_number = 0;
   add_line_after(&doc, paragraph_number, line_number, "First Paragraph, First line");

   /* Additional lines */
   add_line_after(&doc, paragraph_number, line_number + 1, "aaaaaFirst Paragraph, Second lineaaa");
   add_line_after(&doc, paragraph_number, line_number + 2, "First Paragraph, Third line");

   replace_text(&doc, "a", "b");

   print_document(&doc);

   remove_text(&doc, "First");
   highlight_text(&doc, "b");

   print_document(&doc);

   return 0;
}
