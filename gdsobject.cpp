#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "gdsobject.h"
#include "gdsobjects.h"
#include "process_cfg.h"

GDSObject::GDSObject(char *NewName)
{
	FirstPrism = NULL;
	LastPrism = NULL;
	FirstPath = NULL;
	LastPath = NULL;
	FirstText = NULL;
	LastText = NULL;
	FirstSRef = NULL;
	LastSRef = NULL;
	FirstARef = NULL;
	LastARef = NULL;

	Name = new char[strlen(NewName)+1];
	strncpy(Name, NewName, strlen(NewName)+1);

	GotBoundary = 0;
	Boundary.XMax = Boundary.YMax = -10000.0;
	Boundary.XMin = Boundary.YMin = 10000.0;
}

GDSObject::~GDSObject()
{
	if(FirstPrism){
		Prism *prism1;
		Prism *prism2;

		prism1 = FirstPrism;
		while(prism1->Next){
			prism2 = prism1->Next;
			if(prism1->Coords){
				delete prism1->Coords;
			}
			delete prism1;
			prism1 = prism2;
		}
		if(prism1){
			if(prism1->Coords){
				delete prism1->Coords;
			}
			delete prism1;
		}
	}

	if(FirstPath){
		Prism *path1;
		Prism *path2;

		path1 = FirstPath;
		while(path1->Next){
			path2 = path1->Next;
			if(path1->Coords){
				delete path1->Coords;
			}
			delete path1;
			path1 = path2;
		}
		if(path1){
			if(path1->Coords){
				delete path1->Coords;
			}
			delete path1;
		}
	}

	if(FirstText){
		TextElement *text1;
		TextElement *text2;

		text1 = FirstText;
		while(text1->Next){
			text2 = text1->Next;
			if(text1->String){
				delete text1->String;
			}
			delete text1;
			text1 = text2;
		}
		if(text1){
			if(text1->String){
				delete text1->String;
			}
			delete text1;
		}
	}

	if(FirstSRef){
		SRefElement *sref1;
		SRefElement *sref2;

		sref1 = FirstSRef;
		while(sref1->Next){
			sref2 = sref1->Next;
			if(sref1->Name){
				delete sref1->Name;
			}
			delete sref1;
			sref1 = sref2;
		}
		if(sref1){
			if(sref1->Name){
				delete sref1->Name;
			}
			delete sref1;
		}
	}

	if(FirstARef){
		ARefElement *aref1;
		ARefElement *aref2;

		aref1 = FirstARef;
		while(aref1->Next){
			aref2 = aref1->Next;
			if(aref1->Name){
				delete aref1->Name;
			}
			delete aref1;
			aref1 = aref2;
		}
		if(aref1){
			if(aref1->Name){
				delete aref1->Name;
			}
			delete aref1;
		}
	}

	delete Name;
}

void GDSObject::OutputToFile(FILE *fptr)
{
	if(fptr){
		fprintf(fptr, "#declare str_%s = union {\n", Name);
		if(FirstPrism){
			Prism dummyprism;
			dummyprism.Next = FirstPrism;

			Prism *prism = &dummyprism;

			while(prism->Next){
				prism = prism->Next;
				fprintf(fptr, "prism{%.2f,%.2f,%d",prism->Height, prism->Height+prism->Thickness, prism->Points);
				for(int i=0; i<prism->Points; i++){
					fprintf(fptr, ",<%.2f,%.2f>", prism->Coords[i].X, prism->Coords[i].Y);
				}
				if(!prism->Colour.Metal){
					fprintf(fptr, " texture{ pigment{ rgbf <%.2f,%.2f,%.2f,%.2f>}}", prism->Colour.R, prism->Colour.G, prism->Colour.B, prism->Colour.F);
				}else{
					fprintf(fptr, " texture{ pigment{ rgbf <%.2f,%.2f,%.2f,%.2f>} finish{F_MetalA}}", prism->Colour.R, prism->Colour.G, prism->Colour.B, prism->Colour.F);
				}
				fprintf(fptr, "}\n");
			}
		}

		if(FirstPath){
			Prism dummypath;
			dummypath.Next = FirstPath;

			Prism *path = &dummypath;

			int i;
			while(path->Next){
				path = path->Next;

				fprintf(fptr, "mesh2 { vertex_vectors { %d", 8*(path->Points-1));
//				int dia=100;
				for(i=0; i<path->Points-1; i++){

					// 1
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i].X + path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + path->Thickness,
						path->Coords[i].Y + path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

					// 2
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i].X - path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + path->Thickness,
						path->Coords[i].Y - path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

					// 3
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i].X + path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + 0.0,
						path->Coords[i].Y + path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

					// 4
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i].X - path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + 0.0,
						path->Coords[i].Y - path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

					// 5
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i+1].X + path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + path->Thickness,
						path->Coords[i+1].Y + path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

					// 6
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i+1].X - path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + path->Thickness,
						path->Coords[i+1].Y - path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

					// 7
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i+1].X + path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + 0.0,
						path->Coords[i+1].Y + path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

					// 8
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
//					fprintf(fptr, "sphere {<%.2f,%.2f,%.2f>,%d ",
						path->Coords[i+1].X - path->Width * cos(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y)),
						path->Height + 0.0,
						path->Coords[i+1].Y - path->Width * sin(atan2(path->Coords[i].X - path->Coords[i+1].X, path->Coords[i+1].Y - path->Coords[i].Y))
//						,dia
						);
//					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } }\n", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);

				}
				fprintf(fptr, "} face_indices { %d", 12*(path->Points-1));
				for(i=0; i<path->Points-1; i++){
					// print ,faces now
					fprintf(fptr, ",<%d,%d,%d>", 0+8*i, 1+8*i, 2+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 1+8*i, 2+8*i, 3+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 4+8*i, 5+8*i, 6+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 5+8*i, 6+8*i, 7+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 0+8*i, 1+8*i, 5+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 0+8*i, 4+8*i, 5+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 2+8*i, 3+8*i, 6+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 3+8*i, 6+8*i, 7+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 1+8*i, 3+8*i, 7+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 1+8*i, 5+8*i, 7+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 0+8*i, 2+8*i, 4+8*i);
					fprintf(fptr, ",<%d,%d,%d>", 2+8*i, 4+8*i, 6+8*i);
				}
				fprintf(fptr, "} ");
				if(!path->Colour.Metal){
					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} ", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);
				}else{
					fprintf(fptr, "pigment{ rgbf <%.2f, %.2f, %.2f, %.2f>} finish { F_MetalA } ", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);
				}
				fprintf(fptr, "}\n");
			}
		}

		if(FirstSRef){
			SRefElement dummysref;
			dummysref.Next = FirstSRef;

			SRefElement *sref = &dummysref;

			while(sref->Next){
				sref = sref->Next;
				fprintf(fptr, "object{str_%s ", sref->Name);				
				if(sref->Flipped){
					fprintf(fptr, "scale <1,1,-1> ");
				}
				fprintf(fptr, "translate <%.2f,0,%.2f> ", sref->X, sref->Y);
				if(sref->Rotate.Y){
					fprintf(fptr, "Rotate_Around_Trans(<0,%.2f,0>,<%.2f,0,%.2f>)", sref->Rotate.Y, sref->X, sref->Y);
				}
				fprintf(fptr, "}\n");
			}
		}

		if(FirstText){
			TextElement dummytext;
			dummytext.Next = FirstText;

			TextElement *text = &dummytext;

			while(text->Next){
				text = text->Next;
				if(text->String){
					fprintf(fptr, "text{ttf \"arial.ttf\" \"%s\" 1,0.1*x ", text->String);
					fprintf(fptr, "texture{pigment{rgb <%.2f,%.2f,%.2f>}} ", text->Colour.R, text->Colour.G, text->Colour.B, text->Colour.F);
					if(text->Flipped){
						fprintf(fptr, "scale <1,1,-1> ");
//					}else{
//						fprintf(fptr, "scale <1,1,1> ");
					}
					fprintf(fptr, "rotate <90,0,0> translate <%.2f,%.2f,%.2f> ", text->X, text->Z, text->Y);
					if(text->Rotate.Y){
						fprintf(fptr, "Rotate_Around_Trans(<0,%.2f,0>,<%.2f,0,%.2f>)", text->Rotate.Y, text->X, text->Y);
					}
					fprintf(fptr, "}\n");
				}
			}
		}

		if(FirstARef){
			ARefElement dummyaref;
			dummyaref.Next = FirstARef;
			ARefElement *aref = &dummyaref;

			float dx, dy;
			float X, Y;

			while(aref->Next){
				aref = aref->Next;
				if(aref->Rotate.Y == 90.0 || aref->Rotate.Y == -90.0){
					if(aref->Columns && aref->Rows && (aref->X3 - aref->X1) && (aref->Y2 - aref->Y1)){
						dx = (float)(aref->X3 - aref->X1) / (float)aref->Columns;
						dy = (float)(aref->Y2 - aref->Y1) / (float)aref->Rows;

						for(X=0; X<aref->Columns; X++){
							for(Y=0; Y<aref->Rows; Y++){
								fprintf(fptr, "object{str_%s ", aref->Name);
								if(aref->Flipped){
									fprintf(fptr, "scale <1,1,-1> ");
								}
								fprintf(fptr, "translate <%.2f,0,%.2f>", aref->X1 + dx*X, aref->Y1 + dy*Y);
								if(aref->Rotate.Y){
									fprintf(fptr, " Rotate_Around_Trans(<0,%.2f,0>,<%.2f,0,%.2f>)", aref->Rotate.Y, aref->X1 + dx*X, aref->Y1 + dy*Y);
								}
								fprintf(fptr, "}\n");
							}
						}
					}
				}else{
					if(aref->Columns && aref->Rows && (aref->X2 - aref->X1) && (aref->Y3 - aref->Y1)){
						dx = (float)(aref->X2 - aref->X1) / (float)aref->Columns;
						dy = (float)(aref->Y3 - aref->Y1) / (float)aref->Rows;
						for(X=0; X<aref->Columns; X++){
							for(Y=0; Y<aref->Rows; Y++){
								fprintf(fptr, "object { str_%s ", aref->Name);
								if(aref->Flipped){
									fprintf(fptr, "scale <1,1,-1> ");
								}
								fprintf(fptr, "translate <%.2f,0,%.2f>", aref->X1 + dx*X, aref->Y1 + dy*Y);
								if(aref->Rotate.Y){
									fprintf(fptr, " Rotate_Around_Trans(<0,%.2f,0>,<%.2f,0,%.2f>)", aref->Rotate.Y, aref->X1 + dx*X, aref->Y1 + dy*Y);
								}
								fprintf(fptr, "}\n");
							}
						}
					}
				}
			}
		}
		fprintf(fptr, "}\n");
	}
}

char *GDSObject::GetName()
{
	return Name;
}

void GDSObject::AddPrism(float Height, float Thickness, int Points)
{
	Prism *NewPrism = new Prism;

	NewPrism->Next = NULL;

	if(LastPrism){
		LastPrism->Next = NewPrism;
		LastPrism = NewPrism;
	}else{
		FirstPrism = NewPrism;
		LastPrism = NewPrism;
	}

	NewPrism->Coords = new Point[Points];
	NewPrism->Height = Height;
	NewPrism->Thickness = Thickness;
	NewPrism->Points = Points;
}

void GDSObject::AddPrismPoint(int Index, float X, float Y)
{
	if(LastPrism){
		if(LastPrism->Points < Index){
		}else{
			LastPrism->Coords[Index].X = X;
			LastPrism->Coords[Index].Y = Y;
		}
	}
}


void GDSObject::SetPrismColour(float R, float G, float B, float F, int Metal)
{
	if(LastPrism){
		LastPrism->Colour.R = R;
		LastPrism->Colour.G = G;
		LastPrism->Colour.B = B;
		LastPrism->Colour.F = F;
		LastPrism->Colour.Metal = Metal;
	}
}

void GDSObject::SetPrismRotation(float X, float Y, float Z)
{
	if(LastPrism){
		LastPrism->Rotate.X = X;
		LastPrism->Rotate.Y = Y;
		LastPrism->Rotate.Z = Z;
	}
}

void GDSObject::AddText(float X, float Y, float Z, int Flipped)
{
	TextElement *NewText = new TextElement;

	NewText->Next = NULL;
	NewText->String = NULL;

	if(LastText){
		LastText->Next = NewText;
		LastText = NewText;
	}else{
		FirstText = NewText;
		LastText = NewText;
	}

	NewText->X = X;
	NewText->Y = Y;
	NewText->Z = Z;
	NewText->Rotate.X = 0.0;
	NewText->Rotate.Y = 0.0;
	NewText->Rotate.Z = 0.0;
	NewText->Flipped = Flipped;
}

void GDSObject::SetTextColour(float R, float G, float B, float F, int Metal)
{
	if(LastText){
		LastText->Colour.R = R;
		LastText->Colour.G = G;
		LastText->Colour.B = B;
		LastText->Colour.F = F;
		LastText->Colour.Metal = Metal;
	}
}

void GDSObject::SetTextString(char *String)
{
	if(LastText){
		if(LastText->String){
			delete LastText->String;
		}
		LastText->String = new char[strlen(String)+1];
		strncpy(LastText->String, String, strlen(String)+1);
	}
}

void GDSObject::SetTextRotation(float X, float Y, float Z)
{
	if(LastText){
		LastText->Rotate.X = X;
		LastText->Rotate.Y = Y;
		LastText->Rotate.Z = Z;
	}
}

void GDSObject::AddSRef(char *Name, float X, float Y, int Flipped)
{
	SRefElement *NewSRef = new SRefElement;

	NewSRef->Next = NULL;
	NewSRef->Name = NULL;

	if(LastSRef){
		LastSRef->Next = NewSRef;
		LastSRef = NewSRef;
	}else{
		FirstSRef = NewSRef;
		LastSRef = NewSRef;
	}

	NewSRef->Name = new char[strlen(Name)+1];
	strncpy(NewSRef->Name, Name, strlen(Name)+1);
	NewSRef->X = X;
	NewSRef->Y = Y;
	NewSRef->Rotate.X = 0.0;
	NewSRef->Rotate.Y = 0.0;
	NewSRef->Rotate.Z = 0.0;
	NewSRef->Flipped = Flipped;
}

void GDSObject::SetSRefRotation(float X, float Y, float Z)
{
	if(LastSRef){
		LastSRef->Rotate.X = X;
		LastSRef->Rotate.Y = Y;
		LastSRef->Rotate.Z = Z;
	}
}

void GDSObject::AddARef(char *Name, float X1, float Y1, float X2, float Y2, float X3, float Y3, int Columns, int Rows, int Flipped)
{
	ARefElement *NewARef = new ARefElement;

	NewARef->Next = NULL;
	NewARef->Name = NULL;

	if(LastARef){
		LastARef->Next = NewARef;
		LastARef = NewARef;
	}else{
		FirstARef = NewARef;
		LastARef = NewARef;
	}

	NewARef->Name = new char[strlen(Name)+1];
	strncpy(NewARef->Name, Name, strlen(Name)+1);
	NewARef->X1 = X1;
	NewARef->Y1 = Y1;
	NewARef->X2 = X2;
	NewARef->Y2 = Y2;
	NewARef->X3 = X3;
	NewARef->Y3 = Y3;
	NewARef->Columns = Columns;
	NewARef->Rows = Rows;
	NewARef->Rotate.X = 0.0;
	NewARef->Rotate.Y = 0.0;
	NewARef->Rotate.Z = 0.0;
	NewARef->Flipped = Flipped;
}

void GDSObject::SetARefRotation(float X, float Y, float Z)
{
	if(LastARef){
		LastARef->Rotate.X = X;
		LastARef->Rotate.Y = Y;
		LastARef->Rotate.Z = Z;
	}
}

struct _Boundary *GDSObject::GetBoundary(struct ObjectList *objectlist)
{
	if(GotBoundary){
		return &Boundary;
	}

	struct ObjectList dummyobject;

	if(FirstPrism){
		Prism dummyprism;
		dummyprism.Next = FirstPrism;

		Prism *prism = &dummyprism;

		while(prism->Next){
			prism = prism->Next;
			for(int i=0; i<prism->Points; i++){
				if(prism->Coords[i].X > Boundary.XMax){
					Boundary.XMax = prism->Coords[i].X;
				}
				if(prism->Coords[i].X < Boundary.XMin){
					Boundary.XMin = prism->Coords[i].X;
				}
				if(prism->Coords[i].Y > Boundary.YMax){
					Boundary.YMax = prism->Coords[i].Y;
				}
				if(prism->Coords[i].Y < Boundary.YMin){
					Boundary.YMin = prism->Coords[i].Y;
				}
			}
		}
	}

	if(FirstPath){ /* FIXME - need to take width into account? */
		Prism dummypath;
		dummypath.Next = FirstPath;

		Prism *path = &dummypath;

		while(path->Next){
			path = path->Next;
			for(int i=0; i<path->Points; i++){
				if(path->Coords[i].X > Boundary.XMax){
					Boundary.XMax = path->Coords[i].X;
				}
				if(path->Coords[i].X < Boundary.XMin){
					Boundary.XMin = path->Coords[i].X;
				}
				if(path->Coords[i].Y > Boundary.YMax){
					Boundary.YMax = path->Coords[i].Y;
				}
				if(path->Coords[i].Y < Boundary.YMin){
					Boundary.YMin = path->Coords[i].Y;
				}
			}
		}
	}

	if(FirstSRef){
		SRefElement dummysref;
		dummysref.Next=FirstSRef;
		SRefElement *sref = &dummysref;

		struct ObjectList *object;
		dummyobject.Next = objectlist;
		struct _Boundary *NewBound;
		while(sref->Next){
			sref = sref->Next;
			object = &dummyobject;
			while(object->Next){
				object = object->Next;
				if(strncmp(object->Object->GetName(), sref->Name, strlen(sref->Name))==0){
					NewBound = object->Object->GetBoundary(objectlist);
					if(sref->X + NewBound->XMax > Boundary.XMax){
						Boundary.XMax = sref->X + NewBound->XMax;
					}
					if(sref->X - NewBound->XMin < Boundary.XMin){
						Boundary.XMin = sref->X - NewBound->XMin;
					}
					if(sref->Y + NewBound->YMax > Boundary.YMax){
						Boundary.XMax = sref->Y + NewBound->YMax;
					}
					if(sref->Y - NewBound->YMin < Boundary.YMin){
						Boundary.XMin = sref->Y - NewBound->YMin;
					}
					break;
				}
			}
		}
	}

	if(FirstARef){
		ARefElement dummyaref;
		dummyaref.Next = FirstARef;

		ARefElement *aref = &dummyaref;

		struct ObjectList *object;
		dummyobject.Next = objectlist;
		
		struct _Boundary *NewBound;
		while(aref->Next){
			aref = aref->Next;
			object = &dummyobject;
			while(object->Next){
				object = object->Next;
				if(strncmp(object->Object->GetName(), aref->Name, strlen(aref->Name))==0){
					NewBound = object->Object->GetBoundary(objectlist);
					if(aref->X2 + NewBound->XMax > Boundary.XMax){
						Boundary.XMax = aref->X2 + NewBound->XMax;
					}
					if(aref->X1 - NewBound->XMin < Boundary.XMin){
						Boundary.XMin = aref->X1 - NewBound->XMin;
					}
					if(aref->Y3 + NewBound->YMax > Boundary.YMax){
						Boundary.YMax = aref->Y3 + NewBound->YMax;
					}
					if(aref->Y1 - NewBound->YMin < Boundary.YMin){
						Boundary.YMin = aref->Y1 - NewBound->YMin;
					}
					break;
				}
			}
		}
	}

	GotBoundary = 1;

	return &Boundary;
}

void GDSObject::AddPath(float Height, float Thickness, int Points, float Width)
{
	Prism *NewPath = new Prism;

	NewPath->Next = NULL;

	if(LastPath){
		LastPath->Next = NewPath;
		LastPath = NewPath;
	}else{
		FirstPath = NewPath;
		LastPath = NewPath;
	}

	NewPath->Coords = new Point[Points];
	NewPath->Height = Height;
	NewPath->Thickness = Thickness;
	NewPath->Points = Points;
	NewPath->Width = Width;
}

void GDSObject::AddPathPoint(int Index, float X, float Y)
{
	if(LastPath){
		if(LastPath->Points < Index){
		}else{
			LastPath->Coords[Index].X = X;
			LastPath->Coords[Index].Y = Y;
		}
	}
}


void GDSObject::SetPathColour(float R, float G, float B, float F, int Metal)
{
	if(LastPath){
		LastPath->Colour.R = R;
		LastPath->Colour.G = G;
		LastPath->Colour.B = B;
		LastPath->Colour.F = F;
		LastPath->Colour.Metal = Metal;
	}
}

void GDSObject::SetPathRotation(float X, float Y, float Z)
{
	if(LastPath){
		LastPath->Rotate.X = X;
		LastPath->Rotate.Y = Y;
		LastPath->Rotate.Z = Z;
	}
}

int GDSObject::HasASRef()
{
	return (LastARef || LastSRef);
}
