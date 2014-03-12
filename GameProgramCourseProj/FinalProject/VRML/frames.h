#include <string.h>
#include "vrtreeval.h"
#include "filter.h"

#define VRML_ERROR_LINE_REPORT
//#define GENERATE_STANDARD_PROTOS
#ifndef GENERATE_STANDARD_PROTOS
	void init_std_protos();
#else
	void out_protos();
#endif

// #define PARSE_DEBUG

#define NAME_LEN 256

#define SFBOOL 1
#define SFCOLOR 2
#define SFFLOAT 3
#define SFIMAGE 4
#define SFINT32 5
#define SFNODE 6
#define SFROTATION 7
#define SFSTRING 8
#define SFTIME 9
#define SFVEC2F 10
#define SFVEC3F 11
#define MFCOLOR 13
#define MFFLOAT 14
#define MFINT32 15
#define MFNODE 16
#define MFROTATION 17
#define MFSTRING 18
#define MFVEC2F 19
#define MFVEC3F 20

#define START_FIELD_NUM 5
#define ADD_FIELD_NUM 5
class VRParseField;
class ProtoArg;

typedef enum {
	VRFIELD_EXPOSED, 
	VRFIELD_NOTEXPOSED, 
	VRFIELD_EVENTIN, 
	VRFIELD_EVENTOUT
} VRFieldScope;

class VRParseType{
public:
	char name[NAME_LEN];           // "SFBool" ... "Group" ,"Appearance" ,..."MyProto"
	int type; // SFBool ... SFNode
	int field_num;
	int size;
	VRParseField** fields;

	VRNode* body;

	VRParseType(char* nm);
	VRParseType(char* nm,int tp);
	~VRParseType();
	int add_field(VRParseField* fld);
	VRParseField* find_field(char* nm);
	int find_field_idx(char* nm);
};

extern VRParseType** all_types;
extern int type_num;

class VRParseField{
public:
	char name[NAME_LEN];
	int type_index; // index of the field type in "all_types"
	VRFieldScope scope;
	ProtoArg* field_update_info;
	VRParseField(char* nm,int tp,int is_it_basic_type=1);
	~VRParseField();
};

class DEFParse{
public:
	char name[NAME_LEN];
	VRMFNode trees;
	unsigned int repository_size; // size of trees.values - needed for dynamic reallocation
	DEFParse(char* nm);
	void add_tree(VRNode* t);
	~DEFParse();
};

class DEFFrameParse{
public:
	DEFParse** defs;
	int defs_num;
	int size;
	DEFFrameParse* prev; // pointer to including frame
	DEFFrameParse(DEFFrameParse* prv);
	~DEFFrameParse();
	int add(DEFParse* d);
	DEFParse* find(char* nm);
};

class DEFFrameStack{
public:
	DEFFrameParse* top;
	void push();
	void pop();
	DEFFrameStack();
	~DEFFrameStack();
};

class VRInline:public VRNode{
public:
	int path_found;
	char fname[256];
	VRInline();
	int get_file_path(VRMFString* possible_paths);
};

VRParseType* lookup_type(char* name); // finds a type in "all_types"
int add_type(VRParseType* tp);

VRParseField* lookup_field(char* name);

extern int ExpectedFloatNumbersInValue;
int get_vector_length(int);
VRNode* create_struct(VRParseType* type);

extern VRParseType* current_proto;
extern VRParseField* current_field;

#define NODE_STACK_MAX 1000
extern VRParseType* node_stack[NODE_STACK_MAX];
extern int node_stack_ptr;

extern VRNode* vr_struct_stack[NODE_STACK_MAX];
extern int vr_struct_stack_ptr;

extern DEFFrameStack def_stack;

void extract_path(char* fname,char* where_to_put);
void init_atomic_types();
void clean_type_table();

#ifdef GENERATE_STANDARD_PROTOS
extern char exec_dir[];
#endif

VRNode* Clone(VRNode* src);

int format_uncompatability_report(char* s);

#ifdef PARSE_DEBUG
extern int cur_line;
#define rdc(str) if( PARSE_DEBUG - 0   <=  cur_line ) printf("reducing %s\n",str)
#define usn(str) if( PARSE_DEBUG - 0   <=  cur_line ) printf("using %s\n",str)
#else
#define rdc(str) 
#define usn(str) 
#endif

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

void delete_mfstring(VRMFString* data);
void delete_mfnode(VRMFNode* data);
void delete_mfint32(VRMFInt32* data);
void delete_mffloat(VRMFFloat* data);
void delete_mfvec2f(VRMFVec2f* data);
void delete_mfvec3f(VRMFVec3f* data);
void delete_mfrotation(VRMFRotation* data);
void delete_mfcolor(VRMFColor* data);
void delete_sfnode(VRNode* data);
