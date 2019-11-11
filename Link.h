#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <fstream>
#include "Config.h"

using  namespace std;

//nodeid and link id is set from 1
class Node;

class Link
{
public:  
	int     LinkId;
	int     toNode;
	float   dist_;
	float   tt;	           // travel time on PT links		
	bool    boardingArc;   //1 yes    
	Link*   nextLink;
	bool    Hide;
	int     LinkType;
	float   Resource[TOTALRESOURCE+1];

	Link(int id);

	float getDist();
	//virtual function
    virtual ~Link();
	virtual float  getTT(const int& currentMode, const float& t_arr);
	virtual float  getTT(const int& currentMode);
    virtual void   disp();

    friend class Node;
};

#endif