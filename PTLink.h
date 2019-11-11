#ifndef PTLink_H
#define PTLink_H

#include <iostream>
#include <fstream>
#include "Link.h"
#include "Config.h"

using  namespace std;

//nodeid and link id is set from 1

class Link;

class PTLink: public Link
{
public:
	

	PTLink(int id);
	virtual ~PTLink();
	
    virtual void disp();
	virtual float getTT(const int& currentMode);
	virtual float  getTT(const int& currentMode, const float& t_arr);
};

#endif