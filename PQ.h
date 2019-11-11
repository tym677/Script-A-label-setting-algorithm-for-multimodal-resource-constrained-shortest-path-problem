#ifndef PQ_H
#define PQ_H

#include "Config.h"

#include <iostream>
#include <fstream>
using namespace std;

class PQ
{
protected:
	Label* q[PQ_SIZE+1];		/* body of queue */
    int n;

public:
	PQ();     
  	virtual ~PQ();
	int         size();
	int         pq_parent(int pos);
	int         pq_young_child(int pos);
	void        bubble_up(int pos);
	void        bubble_down(int pos);
	void        insert(Label* x);
	void        swap(int i, int j);
	Label*      extract_min();
	bool        is_empty();
	bool        decrease_key(int nodeid, float newkey);
	bool        decrease_key(const int& nodeid, const float& newkey, const float& t_arr);//t dep version
	void        print_pq();
	friend      PQ;
                                 /* number of queue elements */
};



#endif


