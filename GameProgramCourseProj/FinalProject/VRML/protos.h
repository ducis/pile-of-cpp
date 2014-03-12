#ifndef _PROTOS_H_
#define _PROTOS_H_

#include "frames.h"

void clean_parser_value(WRLSTYPE* v);

void delete_parser_value(int type,WRLSTYPE* v);
void copy_parser_value(int type,WRLSTYPE* to,WRLSTYPE* from);
void update_field_value(VRParseType* node_type,
						VRParseField* field,
						VRNode* current_vr_struct,
						WRLSTYPE* field_val);

class VRProtoCall:public VRNode{
public:
	WRLSTYPE** actual_args;
	int actual_arg_num;
	VRParseType* proto_info;
	VRProtoCall();
	VRProtoCall(VRParseType* protoheader);
	void Copy(VRProtoCall*);
	void release();
	~VRProtoCall();
	void set_actual_arg(int field_idx,int type,WRLSTYPE* actual_val);
};


class UpdateNodeData {
public:
	UpdateNodeData* next; // next in the linked list
	int definition_index;
	int use_index;
	void substitute_actual_argument(VRParseType* node_type,VRNode* where,VRProtoCall* call_arguments);
	// definition_index chooses WRLSTYPE in call_arguments
	// use_index chooses WRLSTYPE in where which will be replaced
	UpdateNodeData(int d_idx,int u_idx);
}; 


class UpdateList{
public:
	UpdateNodeData* head;
	VRParseType* type;

	UpdateList(VRParseType* tp);
	UpdateList(UpdateList*);
	void add_record(int d_idx,int u_idx);
	~UpdateList();
};

class ProtoArg{
public:
	int type;
	WRLSTYPE default_val;

	ProtoArg();
	ProtoArg(int tp);
	~ProtoArg();
	void set_default_param(WRLSTYPE* what);
};

void SubstituteActualArgs(VRProtoCall* the_call,VRNode* &root,int evaluate_shapes);
#endif