
#ifndef MYSET_UNION
#define MYSET_UNION

#include "Config.h"
#include <iostream>

using namespace std;


void myset_union_init(myset_union *s, int n)
{
	int i;				/* counter */

	for (i=1; i<=n; i++) {
		s->p[i] = i;
		s->size[i] = 1;
	}
	
        s->n = n;
}

int find(myset_union *s, int x)
{
	if (s->p[x] == x)
		return(x);
	else
		return( find(s,s->p[x]) );
}

void myunion_sets(myset_union *s, int s1, int s2)
{
	int r1, r2;			/* roots of sets */

	r1 = find(s,s1);
	r2 = find(s,s2);

	if (r1 == r2) return;		/* already in same set */

	if (s->size[r1] >= s->size[r2]) {
		s->size[r1] = s->size[r1] + s->size[r2];
		s->p[ r2 ] = r1;
	}
	else {
		s->size[r2] = s->size[r1] + s->size[r2];
                s->p[ r1 ] = r2;
	}
}


bool same_component(myset_union *s, int s1, int s2)
{
	return ( find(s,s1) == find(s,s2) );
}



void print_set_union(myset_union *s)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
    int i,elem=0;   /* counter */
	for (i=1; i<=s->n; i++)
		OutToFile<<i<<" "<<s->p[i]<<"\n";
}

#endif