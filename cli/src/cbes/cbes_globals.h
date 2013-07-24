// Ŭnicode please 
#pragma once

class Object;
class World;
class BaseComponentList;
class BaseComponent;
struct SBaseMessage;

typedef int comp_id_type;
typedef int obj_id_type;

enum {
	kFamilyNone = -1,
	kFamilyHealth = 0,
	kFamilySimpleHP,
	kFamilyVisual,
	cNumFamily,
};

enum {
	kCompNone = -1,
	kCompHealth = 0,
	kCompVisualSphere,
	cNumComp,
};
