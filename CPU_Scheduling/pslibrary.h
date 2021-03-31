#ifndef PS_H_
#define PS_H_

void schedulerInfo(char *s1, char *s2, char *type);
void fcfs(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
void sjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
void psjf(char *s1, char *s2, int x1, int y1, int z1, int x2, int y2, int z2);
void rr(char *s1, char *s2, int quantum, int x1, int y1, int z1, int x2, int y2, int z2);

#endif
