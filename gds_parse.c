#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gds_parse.h"
#include "gds2pov.h"
#include "gds_types.h"
#include "process_cfg.h"
#include "gds_globals.h"

void GDStoPOV(FILE *infile, FILE *outfile, layers *all_layers, int layer_count)
{
	short recordlen;
	byte recordtype, datatype;

	while(!feof(infile)){
		recordlen = GetTwoByteSignedInt(infile, NULL);
		fread(&recordtype, 1, 1, infile);
		fread(&datatype, 1, 1, infile);
		if(!recordlen){
			printf("recordtype=%d, datatype=%d\n", recordtype, datatype);
		}
		recordlen -= 4;
		switch(recordtype){
			case rnHeader:
				ParseHeader(infile, outfile, recordlen);
				break;
			case rnBgnLib:
				ParseBgnLib(infile, outfile, recordlen);
				break;
			case rnLibName:
				ParseLibName(infile, outfile, recordlen);
				break;
			case rnUnits:
				ParseUnits(infile, outfile, recordlen);
				break;
			case rnEndLib:
				/* Empty, no need to parse */
				break;
			case rnBgnStr:
				debug_printf("BGNSTR");
				ParseBgnStr(infile, outfile, recordlen);
				break;
			case rnStrName:
				debug_printf("STRNAME");
				ParseStrName(infile, outfile, recordlen);
				break;
			case rnEndStr:
				debug_printf("ENDSTR");
				fprintf(outfile, "}\n");
				/* Empty, no need to parse */
				break;
			case rnBoundary:
				debug_printf("BOUNDARY");
				currentelement = elBoundary;
				break;
			case rnPath:
				debug_printf("PATH");
				currentelement = elPath;
				break;
			case rnSRef:
				debug_printf("SREF");
				currentelement = elSRef;
				break;
			case rnARef:
				debug_printf("AREF");
				currentelement = elARef;
				break;
			case rnText:
				debug_printf("TEXT");
				currentelement = elText;
				break;
			case rnLayer:
				currentlayer = GetTwoByteSignedInt(infile, &recordlen);
				//debug_printf("LAYER (%ld)\n", currentlayer);
				break;
			case rnDataType:
				ParseDataType(infile, outfile, recordlen);
				break;
			case rnWidth:
				debug_printf("WIDTH");
				currentwidth = (float)(GetFourByteSignedInt(infile, &recordlen)/2);
//				if(currentwidth > 0){
///					currentwidth *= units;
//				}
				// Scale to a half to make width correct when adding and
				// subtracting
				break;
			case rnXY:
				debug_printf("XY");
				ParseXY(infile, outfile, recordlen, all_layers, layer_count);
				break;
			case rnEndEl:
				debug_printf("ENDEL");
				/* Empty, no need to parse */
				break;
			case rnColRow:
				debug_printf("COLROW\n");
				arraycols = GetTwoByteSignedInt(infile, &recordlen);
				arrayrows = GetTwoByteSignedInt(infile, &recordlen);
				break;
			case rnSName:
				debug_printf("ENDEL");
				ParseSName(infile, outfile, recordlen);
				break;
			case rnPathType:
				printf("PATHTYPE\n");
				currentpathtype = GetTwoByteSignedInt(infile, &recordlen);
				break;
			case rnTextType:
				printf("TEXTTYPE\n");
				currenttexttype = GetTwoByteSignedInt(infile, &recordlen);
				break;
			case rnPresentation:
				printf("PRESENTATION\n");
				currentpresentation = GetTwoByteSignedInt(infile, &recordlen);
				break;
			case rnString:
				printf("STRING\n");
				//FIXME
				if(textstring){
					free(textstring);
					textstring = NULL;
				}
				textstring = GetAsciiString(infile, &recordlen);
				break;
			case rnSTrans:
				debug_printf("STRANS\n");
				//FIXME
				/*currentpresentation = */
				currentstrans = GetTwoByteSignedInt(infile, &recordlen);
				break;
			case rnMag:
				printf("MAG\n");
				//FIXME
				/*currentpresentation = */
				while(recordlen){
					GetEightByteReal(infile, &recordlen);
				}
				break;
			case rnAngle:
				currentangle = (float)GetEightByteReal(infile, &recordlen);
				printf("ANGLE (%.2f)\n", currentangle);
				//FIXME
				break;
			case rnAttrTable:
				printf("ATTRTABLE\n");
				break;
			case rnStypTable:
				printf("STYPTABLE\n");
				break;
			case rnStrType:
				printf("STRTYPE\n");
				break;
			case rnElFlags:
				printf("ELFLAGS\n");
				break;
			case rnElKey:
				printf("ELKEY\n");
				break;
			case rnLinkType:
				printf("LINKTYPE\n");
				break;
			case rnLinkKeys:
				printf("LINKKEYS\n");
				break;
			case rnNodeType:
				printf("NODETYPE\n");
				break;
			case rnPropAttr:
				printf("PROPATTR\n");
				break;
			case rnPropValue:
				printf("PROPVALUE\n");
				break;
			case rnBox:
				printf("BOX\n");
				break;
			case rnBoxType:
				printf("BOXTYPE\n");
				break;
			case rnPlex:
				printf("PLEX\n");
				break;
			case rnBgnExtn:
				printf("BGNEXTN\n");
				break;
			case rnEndExtn:
				printf("ENDEXTN\n");
				break;
			case rnTapeNum:
				printf("TAPENUM\n");
				break;
			case rnTapeCode:
				printf("TAPECODE\n");
				break;
			case rnStrClass:
				printf("STRCLASS\n");
				break;
			case rnReserved:
				printf("RESERVED\n");
				break;
			case rnFormat:
				printf("FORMAT\n");
				break;
			case rnMask:
				printf("MASK\n");
				break;
			case rnEndMasks:
				printf("ENDMASKS\n");
				break;
			case rnLibDirSize:
				printf("LIBDIRSIZE\n");
				break;
			case rnSrfName:
				printf("SRFNAME\n");
				break;
			case rnLibSecur:
				printf("LIBSECUR\n");
				break;
			case rnBorder:
				printf("BORDER\n");
				break;
			case rnSoftFence:
				printf("SOFTFENCE\n");
				break;
			case rnHardFence:
				printf("HARDFENCE\n");
				break;
			case rnSoftWire:
				printf("SOFTWIRE\n");
				break;
			case rnHardWire:
				printf("HARDWIRE\n");
				break;
			case rnPathPort:
				printf("PATHPORT\n");
				break;
			case rnNodePort:
				printf("NODEPORT\n");
				break;
			case rnUserConstraint:
				printf("USERCONSTRAINT\n");
				break;
			case rnSpacerError:
				printf("SPACERERROR\n");
				break;
			case rnContact:
				printf("CONTACT\n");
				break;
			default:
				printf("Unknown record type (%d) at position %ld.", recordtype, ftell(infile));

				return;
				break;
	
		}
	}	
}

void ParseHeader(FILE *infile, FILE *outfile, short recordlen)
{
	short version;
	version = GetTwoByteSignedInt(infile, NULL);
}

void ParseBgnLib(FILE *infile, FILE *outfile, short recordlen)
{
	short tmp;
	while(recordlen){
		tmp = GetTwoByteSignedInt(infile, &recordlen);
	}
}

void ParseLibName(FILE *infile, FILE *outfile, short recordlen)
{
	char *str;
	str = GetAsciiString(infile, &recordlen);
	if(libname){
		free(libname);
		libname = NULL;
	}
	libname = malloc(strlen(str)+1);
	if(libname){
		strcpy(libname, str);
	}else{
		printf("Unable to allocate memory for string (%d)\n", strlen(str)+1);
	}
	free(str);
}

void ParseSName(FILE *infile, FILE *outfile, short recordlen)
{
	char *str;
	str = GetAsciiString(infile, &recordlen);
	if(sname){
		free(sname);
		sname = NULL;
	}
	sname = malloc(strlen(str)+1);
	if(sname){
		strcpy(sname, str);
	}else{
		printf("Unable to allocate memory for string (%d)\n", strlen(str)+1);
	}
	free(str);
}

void ParseUnits(FILE *infile, FILE *outfile, short recordlen)
{
	//FIXME - must scale things as floats!
	double tmp;
	units = (float)GetEightByteReal(infile, &recordlen);
	tmp = GetEightByteReal(infile, &recordlen);

	units = 1; // ignore scaling for the mo
}

void ParseBgnStr(FILE *infile, FILE *outfile, short recordlen)
{
	short tmp;
	while(recordlen){
		tmp = GetTwoByteSignedInt(infile, &recordlen);
	}
}

void ParseStrName(FILE *infile, FILE *outfile, short recordlen)
{
	char *str;
	str = GetAsciiString(infile, &recordlen);
	fprintf(outfile, "#declare str_%s = union {\n", str);
	free(str);
}

void ParseDataType(FILE *infile, FILE *outfile, short recordlen)
{
	short result;
	while(recordlen){
		result = GetTwoByteSignedInt(infile, &recordlen);
	}
}

void ParseXY(FILE *infile, FILE *outfile, short recordlen, layers *all_layers, int layer_count)
{
	float X, Y, nextX=0.0, nextY=0.0, prevX=0.0, prevY=0.0;
	float firstX=0.0, firstY=0.0, secondX=0.0, secondY=0.0;
	float dx, dy;
	int points = recordlen/8;
	int i;
	int thislayer;

	thislayer = -1;
	for(i=0; i<layer_count; i++){
		if(all_layers[i].layer == currentlayer){
			thislayer = i;
			break;
		}
	}

	if(thislayer==-1){
		printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d.\n", currentlayer);
		return;
	}

	switch(currentelement){
		case elBoundary:
			if(all_layers[thislayer].height && all_layers[thislayer].show){
				fprintf(outfile, "prism { ");
				fprintf(outfile, "%d,%d,%d", all_layers[thislayer].height, all_layers[thislayer].height+all_layers[thislayer].thickness, points);
			}else{
				printf("Not adding elBoundary (Height = %d, Show = %d)\n", all_layers[thislayer].height, all_layers[thislayer].show);
			}

			for(i=0; i<points; i++){
				if(all_layers[thislayer].height && all_layers[thislayer].show){
					fprintf(outfile, ",");
				}
				X = units * (float)GetFourByteSignedInt(infile, &recordlen);
				Y = units * (float)GetFourByteSignedInt(infile, &recordlen);
				if(all_layers[thislayer].height && all_layers[thislayer].show){
					fprintf(outfile, "<%.2f, %.2f>", X, Y);
				}
			}
			if(all_layers[thislayer].height && all_layers[thislayer].show){
				if(all_layers[thislayer].transparent){
					fprintf(outfile, " texture { pigment { rgbf %s }", all_layers[thislayer].colour);
				}else{
					fprintf(outfile, " texture { pigment { rgb %s }", all_layers[thislayer].colour);
				}

				if(all_layers[thislayer].metal){
					fprintf(outfile, " finish { F_MetalC }");
				}
				fprintf(outfile, " } }\n");
			}
			break;
		case elPath:
			if(currentwidth){
				/* FIXME - need to check for -ve value and then not scale */
				if(all_layers[thislayer].height && all_layers[thislayer].show){
					fprintf(outfile, "prism { ");
					fprintf(outfile, " %d, %d, %d", all_layers[thislayer].height, all_layers[thislayer].height+all_layers[thislayer].thickness, points*2+1);
				}else{
					printf("Not adding elPath (Height = %d, Show = %d)\n", all_layers[thislayer].height, all_layers[thislayer].show);
				}
				X = units * (float)GetFourByteSignedInt(infile, &recordlen);
				Y = units * (float)GetFourByteSignedInt(infile, &recordlen);
				firstX = X;
				firstY = Y;
				for(i=1; i<points+1; i++){
					if(i<points){
						nextX = units * (float)GetFourByteSignedInt(infile, &recordlen);
						nextY = units * (float)GetFourByteSignedInt(infile, &recordlen);
					}
					if(i==1){
						secondX = nextX;
						secondY = nextY;
					}
					if(all_layers[thislayer].height && all_layers[thislayer].show){
						fprintf(outfile, ",");
 						if(i<points){
							fprintf(outfile, "<%.2f, ", X - (float)currentwidth * cos(atan2(X-nextX, nextY-Y)));
							fprintf(outfile, "%.2f>,", Y - (float)currentwidth * sin(atan2(X-nextX, nextY-Y)));
							fprintf(outfile, "<%.2f, ", X + (float)currentwidth * cos(atan2(X-nextX, nextY-Y)));
							fprintf(outfile, "%.2f>", Y + (float)currentwidth * sin(atan2(X-nextX, nextY-Y)));
						}else{
							fprintf(outfile, "<%.2f, ", X - (float)currentwidth * cos(atan2(X-firstX, firstY-Y)));
							fprintf(outfile, "%.2f>,", Y - (float)currentwidth * sin(atan2(X-firstX, firstY-Y)));
							fprintf(outfile, "<%.2f, ", X + (float)currentwidth * cos(atan2(X-firstX, firstY-Y)));
							fprintf(outfile, "%.2f>,", Y + (float)currentwidth * sin(atan2(X-firstX, firstY-Y)));

							fprintf(outfile, "<%.2f, ", X - (float)currentwidth * cos(atan2(firstX-secondX, secondY-firstY)));
							fprintf(outfile, "%.2f>", Y - (float)currentwidth * sin(atan2(firstX-secondX, secondY-firstY)));

						}
					}
					prevX = X;
					X = nextX;
					prevY = Y;
					Y = nextY;
				}
			}else{
				printf("!CurrentWidth\n");
			}
			if(all_layers[thislayer].height && all_layers[thislayer].show){
				fprintf(outfile, " texture { pigment { rgb %s }", all_layers[thislayer].colour);

				if(all_layers[thislayer].metal){
					fprintf(outfile, " finish { F_MetalC }");
				}else if(all_layers[thislayer].transparent){
				//	fprintf(outfile, "finish { F_MetalC }");
				}
				fprintf(outfile, " } }\n");
			}
			break;
		case elSRef:
			X = units * (float)GetFourByteSignedInt(infile, &recordlen);
			Y = units * (float)GetFourByteSignedInt(infile, &recordlen);
			if((unsigned short)(currentstrans & 0x8000) == (unsigned short)0x8000){
				printf("SREF objects not supported when flipped/mirrored.\n");
			}else{
			fprintf(outfile, "object { str_%s ", sname);
			fprintf(outfile, "translate <%.2f, 0, %.2f> ", X, Y);
			if(currentangle){
				fprintf(outfile, "Rotate_Around_Trans(<0, %.2f, 0>, <%0.2f, 0, %0.2f>)", -currentangle, X, Y);
			}
			fprintf(outfile, "}\n");
			}
			break;
		case elARef:
			//FIXME
			firstX = units * (float)GetFourByteSignedInt(infile, &recordlen);
			firstY = units * (float)GetFourByteSignedInt(infile, &recordlen);
			secondX = units * (float)GetFourByteSignedInt(infile, &recordlen);
			secondY = units * (float)GetFourByteSignedInt(infile, &recordlen);
			X = units * (float)GetFourByteSignedInt(infile, &recordlen);
			Y = units * (float)GetFourByteSignedInt(infile, &recordlen);

			if((unsigned short)(currentstrans & 0x8000) == (unsigned short)0x8000){
				printf("AREF (array/tiled/mosaic) objects not supported when flipped/mirrored.\n");
			}else{
				if(currentangle == 90.0 || currentangle == -90.0){
					if(arraycols && arrayrows && (X - firstX) && (secondY - firstY)){
						dx = (float)(X - firstX) / (float)arraycols;
						dy = (float)(secondY - firstY) / (float)arrayrows;
						for(X=0; X<arraycols; X++){
							for(Y=0; Y<arrayrows; Y++){
								fprintf(outfile, "object { str_%s ", sname);
								fprintf(outfile, "translate <%.2f, 0, %.2f>", firstX + dx*X, firstY + dy*Y);
								if(currentangle){
									fprintf(outfile, " Rotate_Around_Trans(<0, %.2f, 0>, <%0.2f, 0, %0.2f>)", -currentangle, firstX + dx*X, firstY + dy*Y);
								}
								fprintf(outfile, "}\n");
							}
						}
					}
				}else{
					if(arraycols && arrayrows && (secondX - firstX) && (Y - firstY)){
						dx = (float)(secondX - firstX) / (float)arraycols;
						dy = (float)(Y - firstY) / (float)arrayrows;
						for(X=0; X<arraycols; X++){
							for(Y=0; Y<arrayrows; Y++){
								fprintf(outfile, "object { str_%s ", sname);
								fprintf(outfile, "translate <%.2f, 0, %.2f>", firstX + dx*X, firstY + dy*Y);
								if(currentangle){
									fprintf(outfile, "Rotate_Around_Trans(<0, %.2f, 0>, <%0.2f, 0, %0.2f>)", -currentangle, firstX + dx*X, firstY + dy*Y);
								}
								fprintf(outfile, "}\n");
							}
						}
					}
				}
			}
			break;
		case elText:
			debug_printf("TEXT XY");
			printf("TEXT not yet supported.\n");
			X = units * (float)GetFourByteSignedInt(infile, &recordlen);
			Y = units * (float)GetFourByteSignedInt(infile, &recordlen);
			if(textstring){
				fprintf(outfile, "text { ttf \"arial.ttf\" \"%s\" 1,0.1*x ", textstring);
				fprintf(outfile, "texture { pigment { rgb %s } } ", all_layers[thislayer].colour);
				fprintf(outfile, "scale <1000,1000,10> rotate <90,0,0> translate <%.2f, %d, %.2f> ", X, all_layers[thislayer].height, Y);
				if(currentangle){
					fprintf(outfile, "Rotate_Around_Trans(<0, %.2f, 0>, <%0.2f, 0, %0.2f>)", -currentangle, X, Y);
				}
				fprintf(outfile, "}\n");
				free(textstring);
			}
			textstring = NULL;
			break;
	}
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
}

