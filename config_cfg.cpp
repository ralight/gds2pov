#include <stdio.h>
#include <string.h>

#include "config_cfg.h"

GDSConfig::GDSConfig()
{
	ProcessFile = NULL;
	Font = NULL;
	Ambient = 1.2;

	CameraPos.postype = ptCamera;
	CameraPos.boundarypos = bpCentre;
	CameraPos.XMod = 1.0;
	CameraPos.YMod = 1.0;
	CameraPos.ZMod = 1.0;

	LookAtPos.postype = ptLookAt;
	LookAtPos.boundarypos = bpCentre;
	LookAtPos.XMod = 1.0;
	LookAtPos.YMod = 1.0;
	LookAtPos.ZMod = 0.0;

	FirstLight = NULL;
	LastLight = NULL;
	LightCount = 0;
	Valid = 1;

}

GDSConfig::GDSConfig(char *configfile)
{
	ProcessFile = NULL;
	Font = NULL;
	Ambient = 1.2;

	int posstart_cnt = 0;
	int posend_cnt = 0;
	int globalstart_cnt = 0;
	int globalend_cnt = 0;
	char line[1024];
	int current_line = 0;

	PosType current_type;

	FILE *cptr = NULL;

	LightCount = 0;
	Valid = 1;

	bool in_global = false;
	bool got_ambient = false;
	bool got_processfile = false;
	bool got_font = false;

	bool in_position = false;
	bool got_type = false;
	bool got_position = false;
	bool got_xmod = false;
	bool got_ymod = false;
	bool got_zmod = false;

	int i;

	FirstLight = NULL;
	LastLight = NULL;

	cptr = fopen(configfile, "rt");
	
	if(!cptr){
		fprintf(stderr, "Error: Unable to open config file \"%s\"\n", configfile);
		Valid = 0;
		return;
	}

	while(!feof(cptr) && fgets(line, 1024, cptr)){
		if(line[0]!='#'){
			if(strstr(line, "PositionStart")){			
				posstart_cnt++;
			}else if(strstr(line, "PositionEnd")){
				posend_cnt++;
			}else if(strstr(line, "GlobalStart")){
				globalstart_cnt++;
			}else if(strstr(line, "GlobalEnd")){
				globalend_cnt++;
			}
		}
	}
	if(posstart_cnt!=posend_cnt){
		fprintf(stderr, "Invalid config file. ");
		fprintf(stderr, "There should be equal numbers of PositionStart and PositionEnd elements! ");
		fprintf(stderr, "(%d and %d found respectively)\n", posstart_cnt, posend_cnt);
		Valid = 0;
		return;
	}

	if(globalstart_cnt!=globalend_cnt || globalstart_cnt > 1 || globalend_cnt > 1){
		fprintf(stderr, "Invalid config file. ");
		fprintf(stderr, "There should be either 1 or 0 of both of GlobalStart and GlobalEnd elements! ");
		fprintf(stderr, "(%d and %d found respectively)\n", globalstart_cnt, globalend_cnt);
		Valid = 0;
		return;
	}

//	PosCount = posstart_cnt;

	fseek(cptr, 0, SEEK_SET);
	while(!feof(cptr) && fgets(line, 1024, cptr)){
		current_line++;
		if(line[0]!='#'){
			if(strstr(line, "GlobalStart")){
				if(in_position){
					fprintf(stderr, "Error: GlobalStart inside PositionStart on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				in_global = true;
				got_ambient = false;
				got_processfile = false;
			}else if(strstr(line, "Ambient:")){
				if(!in_global){
					fprintf(stderr, "Error: Ambient definition outside of GlobalStart and GlobalEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(got_ambient){
					fprintf(stderr, "Warning: Duplicate Ambient definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Ambient: %f", &Ambient);
				}
				got_ambient = true;
			}else if(strstr(line, "ProcessFile:")){
				if(!in_global){
					fprintf(stderr, "Error: ProcessFile definition outside of GlobalStart and GlobalEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(got_processfile){
					fprintf(stderr, "Warning: Duplicate ProcessFile definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					if(ProcessFile){
						delete ProcessFile;
						ProcessFile = NULL;
					}
					ProcessFile = new char[256];
				
					strncpy(ProcessFile, &line[13], 256);
					for(i=strlen(ProcessFile)-1; i>=0; i--){
						if(ProcessFile[i] == '\n'){
							ProcessFile[i] = '\0';
							break;
						}
					}
				}
				got_processfile = true;
			}else if(strstr(line, "Font:")){
				if(!in_global){
					fprintf(stderr, "Error: Font definition outside of GlobalStart and GlobalEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(got_font){
					fprintf(stderr, "Warning: Duplicate Font definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					if(Font){
						delete Font;
						Font = NULL;
					}
					Font = new char[256];
				
					strncpy(Font, &line[6], 256);
					for(i=strlen(Font)-1; i>=0; i--){
						if(Font[i] == '\n'){
							Font[i] = '\0';
							break;
						}
					}
				}
				got_font = true;
			}else if(strstr(line, "GlobalEnd")){
				in_global = false;
			}else if(strstr(line, "PositionStart")){
				if(in_position){
					fprintf(stderr, "Error: PositionStart without PositionEnd not allowed. PositionEnd should appear before line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}else if(in_global){
					fprintf(stderr, "Error: PositionStart inside GlobalStart on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				in_position = true;
				got_type = false;
				got_position = false;
				got_xmod = false;
				got_ymod = false;
				got_zmod = false;
				current_type = ptNone;
			}else if(strstr(line, "Type:")){
				if(!in_position){
					fprintf(stderr, "Error: Type definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(got_type){
					fprintf(stderr, "Warning: Duplicate Type definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					if(strstr(line, "Type: Camera")){
						current_type = ptCamera;
					}else if(strstr(line, "Type: LookAt")){
						current_type = ptLookAt;
					}else if(strstr(line, "Type: Light")){
						current_type = ptLight;
						if(LastLight){
							LastLight->Next = new Position;
							LastLight = LastLight->Next;
							LastLight->Next = NULL;
							LastLight->boundarypos = bpCentre;
							LastLight->XMod = 1.0;
							LastLight->YMod = 1.0;
							LastLight->ZMod = 1.0;
						}else{
							FirstLight = new Position;
							LastLight = FirstLight;
							LastLight->Next = NULL;
							LastLight->boundarypos = bpCentre;
							LastLight->XMod = 1.0;
							LastLight->YMod = 1.0;
							LastLight->ZMod = 1.0;
						}
					}else{
						fprintf(stderr, "Error: Unknown position type \"%s\" on line %d of config file.\n", line, current_line);
						Valid = 0;
						return;
					}
					got_type = true;
				}
			}else if(strstr(line, "Position:")){
				if(!in_position){
					fprintf(stderr, "Error: Position definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					Valid = 0;
					return;
				}
				if(got_position){
					fprintf(stderr, "Warning: Duplicate Position definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					BoundaryPos thispos;
					if(strstr(line, "Position: Centre")){
						thispos = bpCentre;
					}else if(strstr(line, "Position: TopLeft")){
						thispos = bpTopLeft;
					}else if(strstr(line, "Position: TopRight")){
						thispos = bpTopRight;
					}else if(strstr(line, "Position: BottomLeft")){
						thispos = bpBottomLeft;
					}else if(strstr(line, "Position: BottomRight")){
						thispos = bpBottomRight;
					}else{
						fprintf(stderr, "Error: Unknown Position \"%s\" on line %d of config file.\n", line, current_line);
						Valid = 0;
						return;
					}
					switch(current_type){
						case ptCamera:
							CameraPos.boundarypos = thispos;
							break;
						case ptLookAt:
							LookAtPos.boundarypos = thispos;
							break;
						case ptLight:
							if(LastLight){
								LastLight->boundarypos = thispos;
							}else{
								fprintf(stderr, "Error: Position found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								Valid = 0;
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_position = true;
				}
				
			}else if(strstr(line, "XMod:")){
				if(!in_position){
					fprintf(stderr, "Error: XMod definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					Valid = 0;
					return;
				}
				if(got_xmod){
					fprintf(stderr, "Error: Duplicate XMod definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					switch(current_type){
						case ptCamera:
							sscanf(line, "XMod: %f", &CameraPos.XMod);
							break;
						case ptLookAt:
							sscanf(line, "XMod: %f", &LookAtPos.XMod);
							break;
						case ptLight:
							if(LastLight){
								sscanf(line, "XMod: %f", &LastLight->XMod);
							}else{
								fprintf(stderr, "Error: XMod found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								Valid = 0;
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_xmod = true;
				}
			}else if(strstr(line, "YMod:")){
				if(!in_position){
					fprintf(stderr, "Error: YMod definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					Valid = 0;
					return;
				}
				if(got_ymod){
					fprintf(stderr, "Error: Duplicate YMod definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					switch(current_type){
						case ptCamera:
							sscanf(line, "YMod: %f", &CameraPos.YMod);
							break;
						case ptLookAt:
							sscanf(line, "YMod: %f", &LookAtPos.YMod);
							break;
						case ptLight:
							if(LastLight){
								sscanf(line, "YMod: %f", &LastLight->YMod);
							}else{
								fprintf(stderr, "Error: YMod found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								Valid = 0;
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_ymod = true;
				}
			}else if(strstr(line, "ZMod:")){
				if(!in_position){
					fprintf(stderr, "Error: ZMod definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					Valid = 0;
					return;
				}
				if(got_zmod){
					fprintf(stderr, "Error: Duplicate ZMod definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					switch(current_type){
						case ptCamera:
							sscanf(line, "ZMod: %f", &CameraPos.ZMod);
							break;
						case ptLookAt:
							sscanf(line, "ZMod: %f", &LookAtPos.ZMod);
							break;
						case ptLight:
							if(LastLight){
								sscanf(line, "ZMod: %f", &LastLight->ZMod);
							}else{
								fprintf(stderr, "Error: ZMod found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								Valid = 0;
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_zmod = true;
				}
			}else if(strstr(line, "PositionEnd")){
				if(!in_position){
					fprintf(stderr, "Error: PositionEnd without PositionStart on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}else if(!got_type){
					fprintf(stderr, "Error: PositionEnd without Type on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}else if(!got_position){
					fprintf(stderr, "Error: PositionEnd without Position on line %d of config file.\n", current_line);
					Valid = 0;
					return;
				}
				in_position = false;
			}
		}
	}
}

GDSConfig::~GDSConfig()
{
	if(ProcessFile){
		delete ProcessFile;
	}

	if(FirstLight){
		Position *pos1;
		Position *pos2;

		pos1 = FirstLight;

		while(pos1->Next){
			pos2 = pos1->Next;
			if(pos1){
				delete pos1;
			}
			pos1 = pos2;
		}
		if(pos1){
			delete pos1;
		}
	}
	if(Font){
		delete Font;
	}
}

int GDSConfig::IsValid()
{
	return Valid;
}

float GDSConfig::GetAmbient()
{
	return Ambient;
}

char *GDSConfig::GetProcessFile()
{
	return ProcessFile;
}

char *GDSConfig::GetFont()
{
	return Font;
}

Position *GDSConfig::GetLookAtPos()
{
	return &LookAtPos;
}

Position *GDSConfig::GetCameraPos()
{
	return &CameraPos;
}

Position *GDSConfig::GetLightPos()
{
	return FirstLight;
}

