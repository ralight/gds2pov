#ifndef __GDSPARSE_POV_H__
#define __GDSPARSE_POV_H__

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"

class GDSParse_pov : public GDSParse
{
private:
	bool _bounding_output;

public:
	GDSParse_pov (class GDSConfig *config, class GDSProcess *process, bool bounding_output);
	~GDSParse_pov ();

	//class GDSObject_pov *NewObject(char *Name);
	class GDSObject *NewObject(char *Name);
	void OutputHeader();
	void OutputFooter();
};

#endif // __GDSPARSE_POV_H__

