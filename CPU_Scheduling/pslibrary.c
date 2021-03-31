#define READY  0
#define RUNNING  1
#define WAITING  2
#define DONE  3

#include "pslibrary.h"

static char stateChars[] = {'r','R','w',0};

void schedulerInfo(char *s1, char *s2, char *type) {
    int i = 0;
    int rCount1 = 0;
    int rCount2 = 0;
    float rAvg = 0;
    int RCount1 = 0;
    int RCount2 = 0;
    int longest = 0;
    float CPU_Util = 0;
    printf(" \n");
    printf(type);
    printf("\n");
    printf(s1);
    printf("\n");
    printf(s2);
    printf("\n");

    while(s1[i] || s2[i]) {
        if(s1[i] == stateChars[0]) {
            rCount1++;
        } else if(s1[i] == stateChars[1]) {
            RCount1++;
        }
        
        if(s2[i] == stateChars[0]) {
            rCount2++;
        } else if(s2[i] == stateChars[1]) {
            RCount2++;
        }

        i++;
    }

    if(strlen(s1) > strlen(s2)) {
        longest = strlen(s1);
    } else if(strlen(s1) < strlen(s2)) {
        longest = strlen(s2);
    } else {
        longest = (strlen(s1) + strlen(s2)) / 2;
    }

    rAvg = ((float)rCount1 + (float)rCount2) / 2;
    CPU_Util = ((float)RCount1 + (float)RCount2) / (float)longest;
    printf("%d %d %f %f\n", rCount1, rCount2, rAvg, CPU_Util);
}

/* 1) handle state changes:
         running process completes CPU burst
         running process has quantum expire
         IO complete
   2) do context switch if necessary
         both ready
         one ready and CPU free
   3) append appropriate characters to character arrays
         avoid putting in multiple string terminators
*/

/* assume s1 and s2 point to buffers with enough space to hold the result */
/* assume that the int parameters are strictly greater than 0 */
void fcfs(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2) {
   int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* PROCESS1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* PROCESS2 next CPU burst remaining */
   int ioLeft1 = y1;        /* PROCESS1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* PROCESS2 next IO burst remaining, 0 if no more IO */

   //for loop is going while state1 or state2 are not done
   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) { //if process1 is running and the cpu burst is 0
         if (ioLeft1 == 0) {                       //if io is 0
            state1 = DONE;                         // change process state to done
            s1[i] = stateChars[state1];            /* terminate the string */ // state 1 is == done(has value of 3)
         }
         else
            state1 = WAITING;                      //else state of process1 should be changed to wait because it finished cpu burst and still has IO burst
      } else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0) {
            state2 = DONE;
            s2[i] = stateChars[state2];
         } else {
            state2 = WAITING;
         }
      }  
  
      /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
      }  
      /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
          state1 = RUNNING;
      }  
      /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;
      }  
      /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
      /* decrement counts, OK to decrement even if nothing running */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }  /* end of main for loop */
}
 
void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2) {
   int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
                                /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1];            /* terminate the string */
         }
         else
            state1 = WAITING;
      } else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0) {
            state2 = DONE;
            s2[i] = stateChars[state2];
         }
         else
            state2 = WAITING;
      } 
      /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;

      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
      }  
      /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
          if( cpuLeft1 > cpuLeft2 ) {
            state2 = RUNNING;
          } else if( cpuLeft1 < cpuLeft2 ) {
            state1 = RUNNING;
          } else {
            state1 = RUNNING;
          }
      }  
      /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;         
      }  
      /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
      /* decrement counts */
      /* OK to decrement even if nothing running */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }  /* end of main for loop */
}

void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2) {
   int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
      /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1]; /* terminate the string */
         } else 
            state1 = WAITING;
      }  
      else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0) {
            state2 = DONE;
            s2[i] = stateChars[state2];
         } else {
            state2 = WAITING;
         }
      }  
      /* handle IO complete */ //IMPLEMENTED PREEMPTIVE CHECK FOR PSJF
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
          if(cpuLeft1 < cpuLeft2 && state2 == RUNNING) {
            state2 = READY;
            state1 = RUNNING;
          }      
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
          if(cpuLeft1 > cpuLeft2 && state1 == RUNNING) {
            state1 = READY;
            state2 = RUNNING;
          }
      }  
      /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
        if( cpuLeft1 > cpuLeft2 ) {
            state2 = RUNNING;
        } else if ( cpuLeft1 < cpuLeft2 ) {
            state1 = RUNNING;
        } else {
            state1 = RUNNING;
        }    
      }  
      /* handle one ready and CPU available */
      else if ((state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;         
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;
      }  
      /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
      /* decrement counts */
      /* OK to decrement even if nothing running */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }  /* end of main for loop */
}

void rr(char *s1, char *s2, int quantum, int x1, int y1, int z1, int x2, int y2, int z2) {
   int i;                                   /* next string position (time) */
   int state1 = READY;                            /* start with both ready */
   int state2 = READY;
   int cpuLeft1 = x1;                       /* P1 next CPU burst remaining */
   int cpuLeft2 = x2;                       /* P2 next CPU burst remaining */
   int ioLeft1 = y1;        /* P1 next IO burst remaining, 0 if no more IO */
   int ioLeft2 = y2;        /* P2 next IO burst remaining, 0 if no more IO */
   int qleft;                                         /* quantum remaining */

   for (i=0; (state1 != DONE) || (state2 != DONE); i++) {
      /* running process completes its CPU burst */
      if ((state1 == RUNNING) && (cpuLeft1== 0)) {
         if (ioLeft1 == 0) {
            state1 = DONE;
            s1[i] = stateChars[state1]; /* terminate the string */
         }
         else
            state1 = WAITING;
      }  
      else if ((state2 == RUNNING) && (cpuLeft2 == 0) ) {
         if (ioLeft2 == 0) {
            state2 = DONE;
            s2[i] = stateChars[state2];
         } else {
            state2 = WAITING;
         }
      }  
      /* running process has quantum expire */
      if ((state1 == RUNNING) && (qleft == 0) ) {
         state1 = READY;
         qleft = quantum;//test
         /*if( state2 != DONE ) {
             state2 = RUNNING;
         } else {
            state1 = RUNNING;
         }*/
         if(state2 == READY) {
            state2 = RUNNING;
            qleft = quantum;
         }
      }  
      if ((state2 == RUNNING) && (qleft == 0) ) {
         state2 = READY;
         qleft = quantum;//test
         /*if( state1 != DONE ) {
            state1 = RUNNING;
         } else {
            state2 = RUNNING;
         }*/
         if(state1 == READY) {
            state1 = RUNNING;
            qleft = quantum;
         }
      }  

      /* handle IO complete */
      if ((state1 == WAITING) && (ioLeft1 == 0)) {
          state1 = READY;
          cpuLeft1 = z1;
          //state2 = RUNNING; //test
          //qleft = quantum;
      }  
      if ((state2 == WAITING) && (ioLeft2 == 0)) {
          state2 = READY;
          cpuLeft2 = z2;
          //state1 = RUNNING; //test
          //qleft = quantum;
      }  

      /* if both ready, depends on algorithm */
      if ( (state1 == READY) && (state2 == READY)) {
         state1 = RUNNING;
         qleft = quantum;
      }  
      /* handle one ready and CPU available */
      else if ( (state1 == READY) && (state2 != RUNNING)) {
         state1 = RUNNING;
         qleft = quantum;
      }  
      else if ( (state2 == READY) && (state1 != RUNNING)) {
         state2 = RUNNING;
         qleft = quantum;
      }  
      /* insert chars in string, but avoid putting in extra string terminators */
      if (state1 != DONE)
         s1[i] = stateChars[state1];
      if (state2 != DONE)
         s2[i] = stateChars[state2];
      /* decrement counts */
      qleft--;      /* OK to decrement even if nothing running */
      if (state1 == RUNNING)
         cpuLeft1--;
      if (state1 == WAITING)
         ioLeft1--;
      if (state2 == RUNNING)
         cpuLeft2--;
      if (state2 == WAITING)
         ioLeft2--;
   }    /* end of main for loop */
}
