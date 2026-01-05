///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		RUBENS VOODOOMACHINEX
//
//		2020	-	2025
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string>
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "vxmisc.h"
#include "vxprocess.h"
#include "resource.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hi, HINSTANCE hp, LPSTR cl, int cs)
///////////////////////////////////////////////////////////////////////////////////////////////////
//	The main entry.
{
	LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
//	Adding a nice icon to the program.
	if (*cl)
//	Checking if there is an argument, essentially the same functionality as if(cl != NULL),
//	then, assuming this is a valid file directory, proceeding to grab the contents.
	{
		std::string x;
//		x to dump the directory in in.
		char * ptr = cl;
//		Making a copy of the pointer to safely read.
		while (*ptr)
		{
			char curr = *(ptr++); SC_MINIMIZE;
			if (curr != '\"' && curr != '\'')
				x.push_back(curr);
//			This is reading the directory char by char, getting rid of quotation marks, because Windows.
		}
		FILE * f = fopen(x.c_str(), "r");
		if (!f) return mess(ERR | FOPEN);
//		C style file opening. Return with an error message if this failed.
		x.clear();
//		Clearing the string for efficient reuse.
		char * t = (char*)calloc(1024, sizeof(char));
		
		while (fgets(t, 1024, f))
			x.append(t);
		//x.append(" try{ VoodooMain(); }catch(err){ vxErrorMessage(err); vxExit(-1); }");
//		Grabbing the file contents and storing in x.
		free(t);
//		Self explanatory releasing t;
		vxprocess p;
		p.pass_string("vxProgramDir", std::string(cl));
		p.run_direct("vxProgramDir = vxProgramDir.replace(/\"/g, '');");
		p.run_direct("vxProgramDir = vxProgramDir.replace(/\\\\/g, '//');");
		p.run_direct(x);
//		Creating an instance of the VoodooMachineX and putting code x through.
		fclose(f);
//		Closing the file.
	}
	else return MessageBox(NULL, "No input :(", "Oops!", MB_OK | MB_ICONERROR);
	/*{
		vxprocess p;
		p.run_direct(vxShell);
	}*/
//	In this case there is no argument given. Show a messagebox and return.
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
