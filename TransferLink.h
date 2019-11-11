#ifndef TRANSFERLINK_H
#define TRANSFERLINK_H

#include "Link.h"

using  namespace std;

//nodeid and link id is set from 1

class Link;

class TransferLink: public Link
{
public:

	TransferLink(int id);
	virtual ~TransferLink();
	
    virtual void disp();
	virtual float getTT(const int& currentMode);
	virtual float getTT(const int& currentMode, const float& t_arr);
	
};

#endif