#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "gdsobject.h"
#include "gdsobjects.h"
#include "process_cfg.h"

GDSObjects::GDSObjects()
{
	FirstObject = NULL;
	LastObject = NULL;
	Count = 0;
	Boundary = NULL;
}

GDSObjects::~GDSObjects()
{
	struct ObjectList *object;

	object = FirstObject;

	if(object){
		while(object->Next){
			object = object->Next;
			delete object->Prev->Object;
			delete object->Prev;
		}
		delete object->Object;
		delete object;
	}
	if(Boundary){
		delete Boundary;
	}
}

class GDSObject *GDSObjects::AddObject(char *Name)
{
	struct ObjectList *object = new struct ObjectList;
	object->Object = new class GDSObject(Name);
	if(FirstObject){
		LastObject->Next = object;
		object->Prev = LastObject;
		LastObject = object;
		LastObject->Next = NULL;
	}else{
		FirstObject = object;
		LastObject = object;
		object->Next = NULL;
		object->Prev = NULL;
	}
	Count++;
	return object->Object;
}

class GDSObject *GDSObjects::GetObject(int Index)
{
	struct ObjectList *object = FirstObject;
	for(int i=0; i<Index && object; i++){
		object = object->Next;
	}
	return object->Object;
}

int GDSObjects::GetCount()
{
	return Count;
}

struct _Boundary *GDSObjects::GetBoundary()
{
	if(!Boundary){
		Boundary = new struct _Boundary;
	}

	Boundary->XMax = Boundary->YMax = -10000.0;
	Boundary->XMin = Boundary->YMin =  10000.0;

	if(FirstObject){
		struct ObjectList *object = FirstObject;
		struct _Boundary *object_bound;

		while(object->Next){
			object_bound = object->Object->GetBoundary(FirstObject);

			if(object_bound->XMax > Boundary->XMax){
				Boundary->XMax = object_bound->XMax;
			}
			if(object_bound->XMin < Boundary->XMin){
				Boundary->XMin = object_bound->XMin;
			}
			if(object_bound->YMax > Boundary->YMax){
				Boundary->YMax = object_bound->YMax;
			}
			if(object_bound->YMin < Boundary->YMin){
				Boundary->YMin = object_bound->YMin;
			}

			object = object->Next;
		}
		object_bound = object->Object->GetBoundary(FirstObject);

		if(object_bound->XMax > Boundary->XMax){
			Boundary->XMax = object_bound->XMax;
		}
		if(object_bound->XMin < Boundary->XMin){
			Boundary->XMin = object_bound->XMin;
		}
		if(object_bound->YMax > Boundary->YMax){
			Boundary->YMax = object_bound->YMax;
		}
		if(object_bound->YMin < Boundary->YMin){
			Boundary->YMin = object_bound->YMin;
		}
	}
	return Boundary;
}
