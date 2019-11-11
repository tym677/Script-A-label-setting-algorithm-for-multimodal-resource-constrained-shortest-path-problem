#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "Config.h"
using  namespace std;

//nodeid and link id is set from 1
class Link;
//node state enum


class Node
{

public:   

	float      x_;
	float      y_;
	int        NodeId;
	long int   NodeIdLong;
	int        maxDepindex_;
	float*     t_depVec;//dep time for PT stops, by second
	void       set_t_depVec(const float& first_dep, int PTmode);//by second only for PT
	
	Node(int  anodeid);
	~Node();
	int       state_;//node state
	void      setCoord(float x, float y);
	void      disp();	
	float     binary_search(const float& key, int low, int high);
	float     getWaitTime(const float& t_arr);
	float**   ResTable_PT;
 
    friend class Link;
};

#endif
