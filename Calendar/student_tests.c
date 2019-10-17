#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

typedef struct task_info {
  double cost;
  char *prog_language;
} Task_info;


static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes
      - ((Event *)ptr2)->duration_minutes;
}

static Task_info *create_task_info(double cost,
				   const char *prog_language) {
   Task_info *task_info = malloc(sizeof(Task_info));

   if (task_info) {
      task_info->prog_language = malloc(strlen(prog_language) + 1);
      if (task_info->prog_language) {
         task_info->cost = cost;
         strcpy(task_info->prog_language, prog_language);
         return task_info;
      }
   }

   return NULL;
}


static void free_task_info(void *ptr) {
   Task_info *task_info = (Task_info *)ptr;

   free(task_info->prog_language);
   free(task_info);
}


/*This test checks init print and destroy  */
static int test1() {
   int days = 7;
   Calendar *calendar;

   printf("\n\nTest1\n");
   if (init_calendar("Spr", days,
		     comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/*basic test add and remove methods*/
static int test2() {
   int days = 4;
   Calendar *calendar = NULL;
   Task_info *info = create_task_info(1000, "C");
   Task_info *info2 = create_task_info(1050, "Java");
   Task_info *info3 = create_task_info(100, "JavaScript");

   printf("\n\nTest2\n");
   if (init_calendar("Test", days, comp_minutes,
		     free_task_info, &calendar) == SUCCESS) {
      if (add_event(calendar, "Event1", 1200, 100, info3, 1) == SUCCESS
	  && add_event(calendar, "Event2", 200, 50, info, 1) == SUCCESS
	  && add_event(calendar, "Event3", 500, 56, info2, 1) == SUCCESS
	  && add_event(calendar, "Event4", 1300, 122, NULL, 1) == SUCCESS) {
	 if (add_event(calendar, "Event1", 1, 1, NULL, 1) ==  FAILURE &&
	     add_event(calendar, "Event1", 1, 1, NULL, 2) ==  FAILURE) {
	    if (remove_event(calendar, "Event4") == SUCCESS
		&& remove_event(calendar, "Event3") == SUCCESS
		&& remove_event(calendar, "Event2") == SUCCESS) {
	       if (print_calendar(calendar, stdout, 0) == SUCCESS) {
		  if (destroy_calendar(calendar) == SUCCESS) {
		     return SUCCESS;
		  }
	       }
	    }
	 }
      }
   }
   
   return FAILURE;
}

/*basic test find functions + add event testing*/
static int test3() {
   int days = 2;
   Calendar *calendar = NULL;

   printf("\n\nTest3\n");
   if (init_calendar("Test", days, comp_minutes,
		     free_task_info, &calendar) == SUCCESS) {
      if (add_event(calendar, "Event1", 1200, 100, NULL, 0) == FAILURE
	  && add_event(calendar, "Event1", 1200, 100, NULL, 1) == SUCCESS
	  && add_event(calendar, "Event2", 200, 50, NULL, 1) == SUCCESS
	  && add_event(calendar, "Event3", 500, 56, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event3", 500, 56, NULL, 2) == FAILURE) {
	 if (find_event(calendar, "Event1", NULL) == SUCCESS &&
	     find_event(calendar, "Event2", NULL) == SUCCESS &&
	     find_event(calendar, "Event3", NULL) == SUCCESS &&
	     find_event(calendar, "Event4", NULL) == FAILURE) {
	    if (find_event_in_day(calendar,"Event1", 2, NULL) == FAILURE &&
		find_event_in_day(calendar,"Event1", 1, NULL) == SUCCESS &&
		find_event_in_day(calendar,"Event2", 2, NULL) == FAILURE &&
		find_event_in_day(calendar,"Event2", 1, NULL) == SUCCESS &&
		find_event_in_day(calendar,"Event3", 1, NULL) == FAILURE &&
		find_event_in_day(calendar,"Event3", 2, NULL) == SUCCESS) {
	       if (print_calendar(calendar, stdout, 0) == SUCCESS) {
		  if (destroy_calendar(calendar) == SUCCESS) {
		     return SUCCESS;
		  }
	       }
	    }
	 }
      }
   }

   return FAILURE;
}

/*test get event info*/
static int test4() {
   int days = 7;
   Calendar *calendar = NULL;
   Task_info *info = create_task_info(1000, "C");
   Task_info *info2 = create_task_info(1050, "Java");

   printf("\n\nTest4\n");
   if (init_calendar("Test", days, comp_minutes,
		     free_task_info, &calendar) == SUCCESS) {
      if (add_event(calendar, "Event1", 1200, 100, NULL, -1) == FAILURE
	  && add_event(calendar, "Event1", 1200, 100, info, 7) == SUCCESS
	  && add_event(calendar, "Event2", 200, 50, info2, 5) == SUCCESS
	  && add_event(calendar, "Event3", 500, 56, NULL, 2) == SUCCESS) {
	 if (get_event_info(calendar, "Event1") == info &&
	     get_event_info(calendar, "Event2") == info2 &&
	     get_event_info(calendar, "Event3") == NULL &&
	     get_event_info(calendar, "Event4") == NULL) {
	    if (print_calendar(calendar, stdout, 0) == SUCCESS) {
	       if (destroy_calendar(calendar) == SUCCESS) {
		  return SUCCESS;
	       }
	    }
	 }
      }
   }

   return FAILURE;
}

/*test clear day*/
static int test5() {
   int days = 8;
   Calendar *calendar = NULL;
   Task_info *info = create_task_info(1000, "C");
   Task_info *info2 = create_task_info(1050, "Java");

   printf("\n\nTest5\n");
   if (init_calendar("Test", days, comp_minutes,
		     free_task_info, &calendar) == SUCCESS) {
      if (clear_day(NULL, 1) == FAILURE &&
	  clear_day(calendar, 0) == FAILURE &&
	  clear_day(calendar, 9) == FAILURE) {
	 if (add_event(calendar, "Event1", 1200, 100, info, 7) == SUCCESS
	     && add_event(calendar, "Event2", 200, 50, info2, 5) == SUCCESS
	     && add_event(calendar, "Event3", 500, 19, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event4", 500, 56, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event5", 500, 1, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event6", 500, 20, NULL, 4) == SUCCESS
	     && add_event(calendar, "Event7", 500, 56, NULL, 1) == SUCCESS
	     && add_event(calendar, "Event8", 500, 56, NULL, 8) == SUCCESS
	     && add_event(calendar, "Event9", 500, 70, NULL, 8) == SUCCESS) {
	    if (print_calendar(calendar, stdout, 1) == SUCCESS) {
	       if (clear_day(calendar, 2) == SUCCESS &&
		   clear_day(calendar, 8) == SUCCESS &&
		   clear_day(calendar, 1) == SUCCESS &&
		   clear_day(calendar, 7) == SUCCESS) {
		  if (print_calendar(calendar, stdout, 1) == SUCCESS) {
		     if (destroy_calendar(NULL) == FAILURE
			 && destroy_calendar(calendar) == SUCCESS) {
			return SUCCESS;
		     }
		  }
	       }
	    }
	 }
      }
   }

   return FAILURE;
}

/*test clear calendar*/
static int test6() {
   int days = 8;
   Calendar *calendar = NULL;
   Task_info *info = create_task_info(1000, "C");
   Task_info *info2 = create_task_info(1050, "Java");

   printf("\n\nTest6\n");
   if (init_calendar("Test", days, comp_minutes,
		     free_task_info, &calendar) == SUCCESS) {
      if (clear_day(NULL, 1) == FAILURE &&
	  clear_day(calendar, 0) == FAILURE &&
	  clear_day(calendar, 9) == FAILURE) {
	 if (add_event(calendar, "Event1", 1200, 100, info, 7) == SUCCESS
	     && add_event(calendar, "Event2", 200, 50, info2, 5) == SUCCESS
	     && add_event(calendar, "Event3", 500, 1, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event4", 500, 56, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event5", 500, 19, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event6", 500, 56, NULL, 4) == SUCCESS
	     && add_event(calendar, "Event7", 500, 56, NULL, 1) == SUCCESS
	     && add_event(calendar, "Event8", 500, 56, NULL, 8) == SUCCESS
	     && add_event(calendar, "Event9", 500, 45, NULL, 8) == SUCCESS) {
	    if (clear_calendar(calendar) == SUCCESS) {
	       if (print_calendar(calendar, stdout, 1) == SUCCESS) {
		  if (destroy_calendar(calendar) == SUCCESS) {
		     return SUCCESS;
		  }
	       }   
	    }
	 }
      }
   }

   return FAILURE;
}

/*test init calendar*/
static int test7() {
   Calendar *calendar = NULL;

   printf("\n\nTest7\n");
   if (init_calendar(NULL, 1, comp_minutes, NULL, &calendar) == FAILURE &&
       init_calendar("Test", 1, NULL, NULL, NULL) == FAILURE &&
       init_calendar("Test", 0, NULL, NULL, &calendar) == FAILURE) {
      if (init_calendar("test", 1, NULL, NULL, &calendar) == SUCCESS) {
	 if (destroy_calendar(calendar) == SUCCESS) {
	    return SUCCESS;
	 }
      }
   }
   
   return FAILURE;
}

/*test print calendar*/
static int test8() {
   Calendar *calendar = NULL;

   printf("\n\nTest8\n");
   if (init_calendar("test", 1, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(NULL, stdout, 1) == FAILURE &&
	  print_calendar(calendar, NULL, 0) == FAILURE) {
	 if (print_calendar(calendar, stdout, 0) == SUCCESS &&
	     print_calendar(calendar, stdout, 1) == SUCCESS) {
	    if (destroy_calendar(calendar) == SUCCESS) {
	       return SUCCESS;
	    }
	 }
      }
   }

   return FAILURE;
}

/*test destroy calendar + some add event testing*/
static int test9() {
   Calendar *calendar = NULL;
   Task_info *info = create_task_info(1000, "C");
   Task_info *info2 = create_task_info(1050, "Java");

   printf("\n\nTest9\n");
   if (init_calendar("test", 8, comp_minutes,
		     free_task_info, &calendar) == SUCCESS) {
      if (add_event(calendar, "Event1", 1200, 100, info, 7) == SUCCESS
	  && add_event(calendar, "Event2", 200, 50, info2, 5) == SUCCESS
	  && add_event(calendar, "Event3", 500, 1, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event4", 500, 56, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event5", 500, 19, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event6", 500, 56, NULL, 4) == SUCCESS
	  && add_event(calendar, "Event7", 500, 56, NULL, 1) == SUCCESS
	  && add_event(calendar, "Event8", 500, 56, NULL, 8) == SUCCESS
	  && add_event(calendar, "Event9", 500, 9, NULL, 8) == SUCCESS
	  && add_event(calendar, "Event10", 500, 200, NULL, 3) == SUCCESS
	  && add_event(calendar, "Event11", 500, 45, NULL, 8) == SUCCESS
	  && add_event(calendar, "Event12", 500, 15, NULL, 4) == SUCCESS
	  && add_event(calendar, "Event13", 500, 100, NULL, 8) == SUCCESS
	  && add_event(calendar, "Event12", 50, 123, NULL, 7) == FAILURE
	  && add_event(calendar, "Event13", 100, 10, NULL, 1) == FAILURE) {
	 if (destroy_calendar(calendar) == SUCCESS
	     && destroy_calendar(NULL) == FAILURE) {
	    return SUCCESS;
	 }
      }
   }

   return FAILURE;
}

/*test find event*/
static int test10() {
   Calendar *calendar = NULL;
   Event *found = NULL;

   printf("\n\nTest10\n");
   if (init_calendar("test", 7, comp_minutes,
		     free_task_info, &calendar) == SUCCESS) {
      if (add_event(calendar, "Event1", 1200, 100, NULL, 7) == SUCCESS
	  && add_event(calendar, "Event2", 200, 50, NULL, 5) == SUCCESS
	  && add_event(calendar, "Event3", 500, 1, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event4", 500, 56, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event5", 500, 19, NULL, 2) == SUCCESS) {
	 if (find_event(calendar, NULL, NULL) == FAILURE &&
	     find_event(NULL, "Event1", NULL) == FAILURE) {
	    if (find_event(calendar, "Event1", &found) == SUCCESS
		&& find_event(calendar, "Event9", &found) == FAILURE
		&& find_event(calendar, "Event2", NULL) == SUCCESS
		&& find_event(calendar, "Event4", NULL) == SUCCESS) {
	       if (found) {
		  if (destroy_calendar(calendar) == SUCCESS) {
		     return SUCCESS;
		  }
	       }
	    }
	 }
      }
   }
   
   return FAILURE;
}

/*test find event in day*/
static int test11() {
   Calendar *calendar = NULL;
   Event *found = NULL;

   printf("\n\nTest11\n");
   if (init_calendar("test", 7, comp_minutes,
		     NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "Event1", 1200, 100, NULL, 7) == SUCCESS
	  && add_event(calendar, "Event2", 200, 50, NULL, 5) == SUCCESS
	  && add_event(calendar, "Event3", 500, 1, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event4", 500, 56, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event5", 500, 19, NULL, 2) == SUCCESS) {
	 if (find_event_in_day(calendar, NULL, 1, NULL) == FAILURE &&
	     find_event_in_day(NULL, "Event1", 1, NULL) == FAILURE &&
	     find_event_in_day(calendar, "Event1", 0, NULL) == FAILURE &&
	     find_event_in_day(calendar, "Event1",
			       calendar->days + 1, NULL) == FAILURE) {
	    if (find_event_in_day(calendar, "Event3", 2, &found) == SUCCESS
		&& find_event_in_day(calendar,
				     "Event9", 2, &found) == FAILURE
		&& find_event_in_day(calendar,
				     "Event2", 5, NULL) == SUCCESS
		&& find_event_in_day(calendar,
				     "Event4", 2, NULL) == SUCCESS) {
	       if (found) {
		  if (print_calendar(calendar, stdout, 1) == SUCCESS) {
		     if (destroy_calendar(calendar) == SUCCESS) {
			return SUCCESS;
		     }
		  }
	       }
	    }
	 }
      }
   }
   
   return FAILURE;
}

/*extra add event testing*/
static int test12() {
   Calendar *calendar = NULL;

   printf("\n\nTest12\n");
   if (init_calendar("test", 7, comp_minutes,
		     NULL, &calendar) == SUCCESS) {
      if (add_event(NULL, "Event1", 1, 1, NULL, 1) == FAILURE &&
	  add_event(calendar, NULL, 1, 1, NULL, 1) == FAILURE &&
	  add_event(calendar, "Event1", -1, 1, NULL, 1) == FAILURE &&
	  add_event(calendar, "Event1", 1, 0, NULL, 1) == FAILURE &&
	  add_event(calendar, "Event1", 1, 1, NULL, 0) == FAILURE &&
	  add_event(calendar, "Event1", 2401, 1, NULL, 1) == FAILURE &&
	  add_event(calendar, "Event1",
		    1, 1, NULL, calendar->days + 1) == FAILURE) {
	 if (add_event(calendar, "Event1", 1200, 1000, NULL, 7) == SUCCESS
	     && add_event(calendar, "Event2", 200, 50, NULL, 5) == SUCCESS
	     && add_event(calendar, "Event3", 1400, 1, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event4", 400, 16, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event5", 100, 190, NULL, 2) == SUCCESS
	     && add_event(calendar, "Event6", 500, 560, NULL, 4) == SUCCESS
	     && add_event(calendar, "Event7", 10, 56, NULL, 1) == SUCCESS
	     && add_event(calendar, "Event8", 20, 96, NULL, 6) == SUCCESS
	     && add_event(calendar, "Event9", 1000, 9, NULL, 7) == SUCCESS
	     && add_event(calendar, "Event9", 507, 91, NULL, 1) == FAILURE
	     && add_event(calendar, "Event4", 1560, 92, NULL, 2) == FAILURE
	     && add_event(calendar, "Event4", 550, 1, NULL, 7) == FAILURE) {
	    if (print_calendar(calendar, stdout, 0) == SUCCESS) {
	       if (destroy_calendar(calendar) == SUCCESS) {
		  return SUCCESS;
	       }
	    }
	 }
      }
   }

   return FAILURE;
}

/*extra remove event testing*/
static int test13() {
   Calendar *calendar = NULL;

   printf("\n\nTest13\n");
   if (init_calendar("test", 7, comp_minutes,
		     NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "Event1", 1200, 1000, NULL, 7) == SUCCESS
	  && add_event(calendar, "Event2", 200, 50, NULL, 5) == SUCCESS
	  && add_event(calendar, "Event3", 1400, 1, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event4", 400, 16, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event5", 100, 10, NULL, 2) == SUCCESS
	  && add_event(calendar, "Event6", 500, 560, NULL, 4) == SUCCESS
	  && add_event(calendar, "Event7", 10, 56, NULL, 1) == SUCCESS
	  && add_event(calendar, "Event8", 20, 96, NULL, 6) == SUCCESS
	  && add_event(calendar, "Event9", 1000, 9, NULL, 7) == SUCCESS) {
	 if (remove_event(NULL, "Event1") == FAILURE &&
	     remove_event(calendar, NULL) == FAILURE &&
	     remove_event(calendar, "Event10") == FAILURE) {
	    if (print_calendar(calendar, stdout, 1) == SUCCESS) {
	       if (remove_event(calendar, "Event1") == SUCCESS &&
		   remove_event(calendar, "Event2") == SUCCESS &&
		   remove_event(calendar, "Event3") == SUCCESS &&
		   remove_event(calendar, "Event4") == SUCCESS &&
		   remove_event(calendar, "Event5") == SUCCESS) {
		  if (print_calendar(calendar, stdout, 1) == SUCCESS) {
		     if (destroy_calendar(calendar) == SUCCESS) {
			return SUCCESS;
		     }
		  }
	       }
	    }
	 }
      }
   }

   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;
   if (test6() == FAILURE) result = FAILURE;
   if (test7() == FAILURE) result = FAILURE;
   if (test8() == FAILURE) result = FAILURE;
   if (test9() == FAILURE) result = FAILURE;
   if (test10() == FAILURE) result = FAILURE;
   if (test11() == FAILURE) result = FAILURE;
   if (test12() == FAILURE) result = FAILURE;
   if (test13() == FAILURE) result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
      printf("FAILURE\n\n\n");
   }

   return EXIT_SUCCESS;
}
