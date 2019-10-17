/*Zain bhaila
zainb
115197541*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calendar.h"
#include "event.h"

/*check if event already exists in calendar*/
static Event *exists(Calendar *calendar, const char *name,
		     int start_date, int end_date) {
   int i = 0;
   Event *cur_event = NULL;
   
   for (i = start_date; i <= end_date; i++){
      cur_event = calendar->events[i - 1];
      
      while (cur_event) {
	 if (!strcmp(cur_event->name, name)) {
	    return cur_event;
	 }
	 
	 cur_event = cur_event->next;
      }
   }

   return NULL;
}

/*initialize and allocate memory for a calendar*/
int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar) {
   if (!calendar || !name || days < 1) {
      return FAILURE;
   }

   /*allocate calendar memory*/
   *calendar = malloc(sizeof(Calendar));
   if (!(*calendar)) {
      return FAILURE;
   }

   /*allocate name memory*/
   (*calendar)->name = malloc(strlen(name) + 1);
   if (!((*calendar)->name)) {
      free(*calendar);
      *calendar = NULL;
      return FAILURE;
   }
   strcpy((*calendar)->name, name);

   /*allocate events memory*/
   (*calendar)->events = calloc((size_t) days, sizeof(Event *));
   if (!((*calendar)->events)) {
      free((*calendar)->name);
      (*calendar)->name = NULL;
      free(*calendar);
      *calendar = NULL;
      return FAILURE;
   }

   /*assign functions*/
   (*calendar)->comp_func = comp_func;
   (*calendar)->free_info_func = free_info_func;

   /*assign ints*/
   (*calendar)->total_events = 0;
   (*calendar)->days = days;

   return SUCCESS;
}

/*output events and calendar info*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
   int i = 0;
   Event *cur_event = NULL;
   
   if (!calendar || !output_stream) {
      return FAILURE;
   }

   /*print calendar info*/
   if (print_all) {
      fprintf(output_stream, "Calendar's Name: \"%s\"\n",
	      calendar->name);
      fprintf(output_stream, "Days: %d\nTotal Events: %d\n\n",
	      calendar->days, calendar->total_events);
   }

   /*print events*/
   fprintf(output_stream, "****Events****\n");
   for (i = 0; i < calendar->days; i++) {
      cur_event = calendar->events[i];

      if (calendar->total_events) {
	 fprintf(output_stream, "Day %d\n", i + 1);
      }
      
      while (cur_event) {
	 fprintf(output_stream,
		 "Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
		 cur_event->name, cur_event->start_time,
		 cur_event->duration_minutes);

	 cur_event = cur_event->next;
      }
   }

   return SUCCESS;
}

/*add event to calendar*/
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
   Event *cur_event = NULL, *added_event = NULL;
   
   if (!calendar || !name || start_time > 2400 || start_time < 0
       || duration_minutes <= 0 || day > calendar->days || day < 1) {
      return FAILURE;
   }

   /*check if event already exists*/
   if (exists(calendar, name, 1, calendar->days)) {
      return FAILURE;
   }

   /*assign values and allocate memory for the event*/
   added_event = malloc(sizeof(Event));
   if (!added_event) {
      return FAILURE;
   }
      
   added_event->name = malloc(strlen(name) + 1);
   if (!added_event->name) {
      free(added_event);
      added_event = NULL;
      return FAILURE;
   }
   strcpy(added_event->name, name);
      
   added_event->info = info;
   added_event->start_time = start_time;
   added_event->duration_minutes = duration_minutes;

   /*adding event*/
   cur_event = calendar->events[day - 1];
   if (!cur_event) {
      added_event->next = NULL;
      calendar->events[day - 1] = added_event;
   }
   else if (calendar->comp_func(added_event, cur_event) < 0) {
      added_event->next = cur_event;
      calendar->events[day - 1] = added_event;
   }
   else {
      while (cur_event->next != NULL &&
	     calendar->comp_func(added_event, cur_event->next) > 0) {
	 cur_event = cur_event->next;
      }

      added_event->next = cur_event->next;
      cur_event->next = added_event;
   }

   calendar->total_events += 1;
   
   return SUCCESS;
}

/*find an event in calendar*/
int find_event(Calendar *calendar, const char *name, Event **event) {
   Event *found = NULL;
   
   if (!calendar || !name) {
      return FAILURE;
   }

   /*search for event and get address*/
   found = exists(calendar, name, 1, calendar->days);

   if (found) {
      /*return event to param*/
      if (event) {
	 *event = found;
      }

      return SUCCESS;
   }

   return FAILURE;
}

/*find an event on a specific day*/
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
   Event *found = NULL;
   
   if (!calendar || !name || day < 1 || day > calendar->days) {
      return FAILURE;
   }

   /*search for event on day and get address*/
   found = exists(calendar, name, day, day);

   if (found) {
      /*return event to param*/
      if (event) {
	 *event = found;
      }

      return SUCCESS;
   }

   return FAILURE;
}

/*remove event from calendar*/
int remove_event(Calendar *calendar, const char *name) {
   int i = 0;
   Event *cur_event = NULL, *prev_event = NULL;
   
   if (!calendar || !name) {
      return FAILURE;
   }

   for (i = 0; i < calendar->days; i++) {
      cur_event = calendar->events[i];
      prev_event = NULL;
   
      while (cur_event) {
	 if (!strcmp(cur_event->name, name)) {
	    if (cur_event->info && calendar->free_info_func) {
	       calendar->free_info_func(cur_event->info);
	       cur_event->info = NULL;
	    }
	 
	    free(cur_event->name);
	    cur_event->name = NULL;

	    if (!prev_event) {
	       calendar->events[i] = cur_event->next;
	    }
	    else {
	       prev_event->next = cur_event->next;
	    }
	    

	    free(cur_event);
	    cur_event = NULL;
	    
	    calendar->total_events -= 1;

	    return SUCCESS;
	 }

	 prev_event = cur_event;
	 cur_event = cur_event->next;
      }
   }

   return FAILURE;
}

/*get event info from calendar*/
void *get_event_info(Calendar *calendar, const char *name) {
   Event *found = NULL;
   
   found = exists(calendar, name, 1, calendar->days);

   if (found) {
      return found->info;
   }

   return NULL;
}

/*remove all events from a calendar*/
int clear_calendar(Calendar *calendar) {
   int i = 0;
   
   if (!calendar) {
      return FAILURE;
   }
   
   for (i = 0; i < calendar->days; i++){
      clear_day(calendar, i + 1);
   }

   return SUCCESS;
}

/*remove all allocated memory for a day*/
int clear_day(Calendar *calendar, int day) {
   Event *cur_event = NULL, *prev_event = NULL;
   
   if (!calendar || day < 1 || day > calendar->days) {
      return FAILURE;
   }

   /*free the day*/
   cur_event = calendar->events[day - 1];
   
   while (cur_event) {
      if (cur_event->info && calendar->free_info_func) {
	 calendar->free_info_func(cur_event->info);
	 cur_event->info = NULL;
      }
	 
      free(cur_event->name);
      cur_event->name = NULL;

      prev_event = cur_event;
      cur_event = cur_event->next;
      
      free(prev_event);
      prev_event = NULL;
      
      calendar->total_events -= 1;
   }

   calendar->events[day -1] = NULL;
   
   return SUCCESS;
}

/*remove all allocated memory for a calendar*/
int destroy_calendar(Calendar *calendar) {
   if (!calendar) {
      return FAILURE;
   }

   clear_calendar(calendar);

   free(calendar->name);
   calendar->name = NULL;
   free(calendar->events);
   calendar->events = NULL;
   free(calendar);
   calendar = NULL;

   return SUCCESS;
}

