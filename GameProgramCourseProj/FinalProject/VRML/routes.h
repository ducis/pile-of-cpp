#ifndef _ROUTES_H_
#define _ROUTES_H_

#include "protos.h"
#include "vrAnimationNode.h"

class RouteParse{
public:
	RouteParse* next; // pointer to the next in the linked list

	DEFParse* from;
	DEFParse* to;

	VREventOutType eventFrom;
	VREventInType  eventTo;

	RouteParse(DEFParse* f,DEFParse* t,VREventOutType ef,VREventInType et);
	~RouteParse();

};

class RouteParseList{
public:
	RouteParse* head;

	RouteParseList();
	void add(RouteParse* what);
	void clean();
	~RouteParseList();
};

extern RouteParseList global_route_list;

RouteParse* create_route(DEFFrameParse* def_frame,
						 char* node_from,char* field_from,
						 char* node_to,char* field_to);
void rename_node(VRNode* n,int& def_idx);

#endif