#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "gdsobject.h"
#include "process_cfg.h"

GDSObject::GDSObject (char *filename)
{
	FirstElement = NULL;
}

GDSObject::~GDSObject()
{
	struct Element *element1;
	struct Element *element2;

	element1 = FirstElement;

	while(element1->Next){
		element2 = element1->Next;
		if(element1->line){
			delete element1->line;
		}
		if(element1){
			delete element1;
		}
		element1 = element2;
	}
	if(element1->line){
		delete element1->line;
	}
	if(element1){
		delete element1;
	}
}

GDSObject::AddElement(char *line, int size)
{
	struct Element *newelement;
	struct Element *element;

	newelement = new struct Element;

	if(FirstElement){
		element = FirstElement;
		while(element->Next){
			element = element->Next
		}
		element->Next = newelement;
	}else{
		FirstElement = newelement;
	}

	newelement->line = new char[size + 1];
	strncpy(newelement->line, line, size+1);
}

GDSObject::OutputToFile(FILE *fptr)
{
	if(fptr && FirstElement){
		struct Element *element;

		element = FirstElement;
		
		while(element->Next){
			if(element->line){
				fprintf(fptr, "%s\n", element->line);
			}
			element = element->Next;
		}
		if(element->line){
			fprintf(fptr, "%s\n", element->line);
		}
	}
}
