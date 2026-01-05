///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		RUBENS VOODOOMACHINEX
//
//		2020	-	2024
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <Windows.h>
#include <CommCtrl.h>
#include <process.h>
#include <stdio.h>
#include <string>
#include <string.h>
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "vxmisc.h"
#include "duktape.h"
#include "vxprocess.h"
#include "resource.h";
///////////////////////////////////////////////////////////////////////////////////////////////////
void vx_thread_bootstrap(void * x)
{
	vxprocess * p = (vxprocess*)x;
	p -> on_run();
	p -> flag -= ACTIVE;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
duk_ret_t vx_bind_exit(duk_context * c)
{
	int stat = duk_to_int(c, 0);
	exit(stat);
	return 0;
}
duk_ret_t vx_bind_include(duk_context * c)
{
	char * d = (char*)duk_to_string(c, 0);
	FILE * f = fopen(d, "r");
	if (!f)
	{
		mess(ERR | FOPEN);
		return 0;
	}
	char * t = (char*)calloc(1024*128, sizeof(char));
	std::string code;
	while (fgets(t, 1024, f))
		code.append(t);
	duk_peval_string(c, code.c_str());
	free(t);
	fclose(f);
	return 1;
}
duk_ret_t vx_bind_put(duk_context * c)
{
	FILE * f = (FILE*)duk_to_uint(c, 1);
	char x = (char)duk_to_int(c, 0);
	fputc(x, f);
	return 0;
}
duk_ret_t vx_bind_get(duk_context * c)
{
	FILE * f = (FILE*)duk_to_uint(c, 0);
	char x = fgetc(f);
	duk_push_int(c, (int)x);
	return 1;
}

duk_ret_t vx_bind_print(duk_context * c)
{
	char * s = (char*)duk_to_string(c, 0);
	FILE * f = (FILE*)duk_to_uint(c, 1);
	fprintf(f, s, "%s");
	return 0;
}
duk_ret_t vx_bind_read(duk_context * c)
{
	FILE * f = (FILE*)duk_to_uint(c, 0);

	char * x = (char*)calloc(1024 * 256, sizeof(char));
	fgets(x, 1024 * 256, f);
	duk_push_string(c, x);
	free(x);
	return 1;
}
duk_ret_t vx_bind_fopen(duk_context * c)
{
	char * d = (char*)duk_to_string(c, 0);
	char * m = (char*)duk_to_string(c, 1);
	FILE * f = NULL;
	f = fopen(d, m);
	duk_push_uint(c, (unsigned int)f);
	return 1;
}
duk_ret_t vx_bind_fclose(duk_context * c)
{
	FILE * f = (FILE*)duk_to_uint(c, 0);
	fclose(f);
	return 0;
}
duk_ret_t vx_bind_sleep(duk_context * c)
{
	unsigned int ms = duk_to_uint(c, 0);
	Sleep(ms);
	return 0;
}
duk_ret_t vx_bind_conopen(duk_context * c)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	return 0;
}
duk_ret_t vx_bind_conclose(duk_context * c)
{
	FreeConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	return 0;
}
duk_ret_t vx_bind_inline(duk_context * c)
{
	char * x = (char*)duk_to_string(c, 0);
	duk_peval_string(c, x);
	return 1;
}
duk_ret_t vx_bind_process(duk_context * c)
{
	char * x = (char*)duk_to_string(c, 0);
	vxprocess * p = new vxprocess();
	p->loadup(std::string(x));
	p->run();
	duk_push_uint(c, (unsigned int)p);
	return 1;
}
duk_ret_t vx_bind_alloc(duk_context * c)
{
	unsigned int n = duk_to_uint(c, 0);
	vxbyte * x = (vxbyte*)calloc(n, sizeof(vxbyte));
	duk_push_uint(c, (unsigned int)x);
	return 1;
}
duk_ret_t vx_bind_realloc(duk_context * c)
{
	vxbyte * x = (vxbyte*)duk_to_uint(c, 0);
	unsigned int n = duk_to_uint(c, 1);
	x = (vxbyte*)realloc((void*)x, n);

	duk_push_uint(c, (unsigned int) x);
	return 1;
}
duk_ret_t vx_bind_free(duk_context * c)
{
	vxbyte * x = (vxbyte*)duk_to_uint(c, 0);
	free(x);
	return 0;
}
duk_ret_t vx_bind_cstring(duk_context * c)
{
	char * x = (char*)(unsigned int)duk_to_uint(c, 0);
	duk_push_string(c, x);
	return 1;
}
duk_ret_t vx_bind_memcpy(duk_context * c)
{
	unsigned int dest = (unsigned int)duk_to_uint(c, 0);
	unsigned int src = (unsigned int)duk_to_uint(c, 1);
	unsigned int n = (unsigned int)duk_to_uint(c, 2);
	memcpy((void*)dest, (void*)src, n);
	duk_push_uint(c, dest);
	return 1;
}
duk_ret_t vx_bind_putbyte(duk_context * c)
{
	vxbyte * x = (vxbyte*)duk_to_uint(c, 0);
	*x = (vxbyte)duk_to_uint(c, 1);
	duk_push_uint(c, *x);
	return 1;
}
duk_ret_t vx_bind_putword(duk_context * c)
{
	short * x = (short*)duk_to_uint(c, 0);
	*x = (short)duk_to_int(c, 1);
	duk_push_int(c, *x);
	return 1;
}
duk_ret_t vx_bind_putdword(duk_context * c)
{
	int * x = (int*)duk_to_uint(c, 0);
	*x = (int)duk_to_int(c, 1);
	duk_push_int(c, *x);
	return 1;
}
duk_ret_t vx_bind_putqword(duk_context * c)
{
	unsigned long long * x = (unsigned long long*)duk_to_uint(c, 0);
	*x = (unsigned long long)duk_to_uint(c, 1);
	duk_push_uint(c, *x);
	return 1;
}
duk_ret_t vx_bind_putint(duk_context * c)
{
	int * x = (int*)duk_to_uint(c, 0);
	*x = duk_to_int(c, 1);
	duk_push_int(c, *x);
	return 1;
}
duk_ret_t vx_bind_putdouble(duk_context * c)
{
	double * x = (double*)duk_to_uint(c, 0);
	*x = (double)duk_to_number(c, 1);
	duk_push_number(c, *x);
	return 1;
}
duk_ret_t vx_bind_putstring(duk_context * c)
{
	char * x = (char*)duk_to_uint(c, 0);
	char * y = (char*)duk_to_string(c, 1);
	strcpy(x, y);
	duk_push_string(c, x);
	return 1;
}
duk_ret_t vx_bind_getbyte(duk_context * c)
{
	vxbyte * x = (vxbyte*)duk_to_uint(c, 0);
	duk_push_uint(c, (unsigned int)*x);
	return 1;
}
duk_ret_t vx_bind_getword(duk_context * c)
{
	short * x = (short*)duk_to_int(c, 0);
	duk_push_int(c, (int)*x);
	return 1;
}
duk_ret_t vx_bind_getdword(duk_context * c)
{
	int * x = (int*)duk_to_int(c, 0);
	duk_push_int(c, (int)*x);
	return 1;
}
duk_ret_t vx_bind_getqword(duk_context * c)
{
	unsigned long long * x = (unsigned long long*)duk_to_uint(c, 0);
	duk_push_uint(c, (unsigned int)*x);
	return 1;
}
duk_ret_t vx_bind_getint(duk_context * c)
{
	int * x = (int*)duk_to_uint(c, 0);
	duk_push_int(c, *x);
	return 1;
}
duk_ret_t vx_bind_getdouble(duk_context * c)
{
	double * x = (double*)duk_to_uint(c, 0);
	duk_push_number(c, *x);
	return 1;
}
duk_ret_t vx_bind_getstring(duk_context * c)
{
	char * x = (char*)duk_to_uint(c, 0);
	duk_push_string(c, x);
	return 1;
}
duk_ret_t vx_bind_setstatic(duk_context * c)
{
	vxprocess::static_bytes = (vxbyte*)duk_to_uint(c, 0);
	duk_push_uint(c, (unsigned int)vxprocess::static_bytes);
	return 1;
}
duk_ret_t vx_bind_getstatic(duk_context * c)
{
	duk_push_uint(c, (unsigned int)vxprocess::static_bytes);
	return 1;
}
duk_ret_t vx_bind_error(duk_context * c)
{
	char * x = (char*)duk_to_string(c, 0);
	duk_push_int(c, MessageBox(NULL, x, "VoodooMachineX error!", MB_OK | MB_ICONERROR));
	return 1;
}
duk_ret_t vx_bind_message(duk_context * c)
{
	char * x = (char*)duk_to_string(c, 0);
	char * y = (char*)duk_to_string(c, 1);
	unsigned int z = duk_to_uint(c, 2);
	duk_push_int(c, MessageBox(NULL, y, x, z));
	return 1;
}
duk_ret_t vx_bind_brainfuck(duk_context* c)
{
	char* i = (char*)duk_to_string(c, 0);
	char* brainfuckheap = (char*)calloc(1024*128, sizeof(char));
	while (*i)
	{
		if (*i == '>')
			brainfuckheap++;
		else if (*i == '<')
			brainfuckheap--;
		else if (*i == '+')
			(*brainfuckheap)++;
		else if (*i == '-')
			(*brainfuckheap)--;
		else if (*i == '.')
			putc(*brainfuckheap, stdout);
		else if (*i == ',')
			(*brainfuckheap) = getc(stdin);
		else if (*i == '[')
		{
			if (!(*brainfuckheap))
			{
				int depth = 1;
				while (depth)
				{
					i++;
					if (*i == '[')
						depth++;
					else if (*i == ']')
						depth--;
				}
			}
		}
		else if (*i == ']')
		{
			if (*brainfuckheap)
			{
				int depth = 1;
				while (depth)
				{
					i--;
					if (*i == ']')
						depth++;
					else if (*i == '[')
						depth--;
				}
			}
		}
		i++;
	}
	free(brainfuckheap);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
duk_ret_t vxp_bind_create_process(duk_context * c)
{
	vxprocess * p = new vxprocess();
	duk_push_uint(c, (unsigned int)p);
	return 1;
}
duk_ret_t vxp_bind_destroy_process(duk_context * c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	delete p;
	return 0;
}
duk_ret_t vxp_bind_is_active_process(duk_context * c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	duk_push_boolean(c, (p -> flag) & ACTIVE);
	return 1;
}
duk_ret_t vxp_bind_loadup_process(duk_context * c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	char * code = (char*)duk_to_string(c, 1);
	std::string xcode = std::string(code);
	p->loadup(xcode);
	return 0;
}
duk_ret_t vxp_bind_run_process(duk_context * c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	p->run();
	return 0;
}
duk_ret_t vxp_bind_pass_byte(duk_context * c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	std::string l = std::string(duk_to_string(c, 1));
	unsigned char x = duk_to_uint(c, 2);
	p->pass_byte(l, x);
	return 0;
}
duk_ret_t vxp_bind_pass_uint(duk_context * c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	std::string l = std::string(duk_to_string(c, 1));
	unsigned int x = duk_to_uint(c, 2);
	p->pass_uint(l, x);
	return 0;
}
duk_ret_t vxp_bind_pass_int(duk_context * c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	std::string l = std::string(duk_to_string(c, 1));
	signed int x = duk_to_int(c, 2);
	p->pass_int(l, x);
	return 0;
}
duk_ret_t vxp_bind_pass_string(duk_context* c)
{
	vxprocess * p = (vxprocess*)duk_to_uint(c, 0);
	std::string l = std::string(duk_to_string(c, 1));
	std::string x = std::string(duk_to_string(c, 2));
	p->pass_string(l, x);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//TODO:
static duk_ret_t vxw_bind_create_directory(duk_context* c)
{
	char * path = (char*)duk_to_string(c, 0);
	CreateDirectory(path, NULL);
	return 0;
}
static duk_ret_t vxw_bind_delete_directory(duk_context* c)
{
	char * path = (char*)duk_to_string(c, 0);
	RemoveDirectory(path);
	return 0;
}
static duk_ret_t vxw_bind_delete_file(duk_context * c)
{
	char * file = (char*)duk_to_string(c, 0);
	DeleteFile(file);
	return 0;
}
static duk_ret_t vxw_bind_copy_file(duk_context * c)
{
	char * existing = (char*)duk_to_string(c, 0);
	char* copy = (char*)duk_to_string(c, 1);
	BOOL result = CopyFile(existing, copy, TRUE);
	return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
static duk_ret_t vxw_bind_move_cursor(duk_context * c)
{
	unsigned int x = (unsigned int)duk_to_uint(c, 0);
	unsigned int y = (unsigned int)duk_to_uint(c, 1);
	SetCursorPos(x, y);
	return 0;
}
static duk_ret_t vxw_bind_mouse_event(duk_context * c)
{
	unsigned int flags = (unsigned int)duk_to_uint(c, 0);
	signed int x = (signed int)duk_to_int(c, 1);
	signed int y = (signed int)duk_to_int(c, 2);

	mouse_event(flags, x, y, 0, 0);

	return 0;
}
static duk_ret_t vxw_bind_keyboard_event(duk_context * c)
{
	char key = (char)duk_to_int(c, 0);
	char scan = (char)duk_to_int(c, 1);
	char flag = (unsigned int)duk_to_int(c, 2);
	keybd_event(key, scan, flag, 0);
	return 0;
}
static duk_ret_t vxw_bind_sleep(duk_context * c)
{
	unsigned int i = (unsigned int)duk_to_uint(c, 0);
	Sleep(i);
	return 1;
}
static duk_ret_t vxw_bind_beep(duk_context * c)
{
	unsigned int f = (unsigned int)duk_to_uint(c, 0);
	unsigned int d = (unsigned int)duk_to_uint(c, 1);
	return Beep(f, d);
}
static duk_ret_t vxw_bind_system(duk_context * c)
{
	char * prompt = (char*)duk_to_string(c, 0);
	system(prompt);
	return 1;
}

duk_ret_t vxw_bind_openfiledialog(duk_context * c)
{
	char * filter = (char*)duk_to_string(c, 0);


	OPENFILENAME ofn;
	TCHAR szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		duk_push_string(c, (char*)ofn.lpstrFile);
	}
	else duk_push_null(c);
	return 1;
}
duk_ret_t vxw_bind_savefiledialog(duk_context * c)
{
	char * filter = (char*)duk_to_string(c, 0);


	OPENFILENAME ofn;
	TCHAR szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		duk_push_string(c, (char*)ofn.lpstrFile);
	}
	else duk_push_null(c);
	return 1;
}
duk_ret_t vxw_bind_send_message(duk_context * c)
{
	LRESULT l = SendMessage((HWND)duk_to_uint(c, 0),
					duk_to_uint(c, 1),
					duk_to_uint(c, 2),
					duk_to_uint(c, 3));
	duk_push_int(c, l);
	return 1;
}
duk_ret_t vxw_bind_create_wndclassex(duk_context * c)
{
	WNDCLASSEX * wc = (WNDCLASSEX*)calloc(1, sizeof(WNDCLASSEX));

	(*wc).hInstance = GetModuleHandle(NULL);
	(*wc).cbClsExtra = 0;
	(*wc).cbSize = sizeof(WNDCLASSEX);
	(*wc).cbWndExtra = 0;
	(*wc).hCursor = LoadCursor((*wc).hInstance, IDC_ARROW);
	(*wc).hIcon = LoadIcon((*wc).hInstance, IDI_APPLICATION);
	(*wc).hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	(*wc).lpszClassName = duk_to_string(c, 0);
	(*wc).style = CS_HREDRAW | CS_VREDRAW;
	(*wc).lpfnWndProc = (WNDPROC)vxw_std_wndproc;

	if (!RegisterClassEx(wc)) duk_push_uint(c, 0);
	else duk_push_uint(c, (unsigned int) wc);

	return 1;
}
duk_ret_t vxw_bind_destroy_wndclassex(duk_context * c)
{
	WNDCLASSEX * wc = (WNDCLASSEX*)duk_to_uint(c, 0);
	free(wc);
	return 0;
}
duk_ret_t vxw_bind_create_window(duk_context * c)
{
	HWND o = NULL;
	o = CreateWindow(duk_to_string(c, 0),
					duk_to_string(c, 1),
					duk_to_uint(c, 2),
					duk_to_uint(c, 3),
					duk_to_uint(c, 4),
					duk_to_uint(c, 5),
					duk_to_uint(c, 6),
			  (HWND)duk_to_uint(c, 7),
			 (HMENU)duk_to_uint(c, 8),
					GetModuleHandle(NULL),
			(LPVOID)duk_to_uint(c, 9));
	HICON x = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(o, WM_SETICON, 0, (LPARAM)x);
	SendMessage(o, WM_SETICON, 1, (LPARAM)x);
	duk_push_uint(c, (unsigned int) o);
	return 1;
}
duk_ret_t vxw_bind_destroy_window(duk_context * c)
{
	HWND x = (HWND)duk_to_uint(c, 0);
	DestroyWindow(x);
	return 0;
}
duk_ret_t vxw_bind_show_window(duk_context * c)
{
	HWND x = (HWND)duk_to_uint(c, 0);
	ShowWindow(x, SW_SHOW);
	return 0;
}
duk_ret_t vxw_bind_update_window(duk_context * c)
{
	HWND x = (HWND)duk_to_uint(c, 0);
	UpdateWindow(x);
	return 0;
}
duk_ret_t vxw_bind_create_message(duk_context * c)
{
	duk_push_uint(c, (unsigned int)calloc(1, sizeof(MSG)));
	return 1;
}
duk_ret_t vxw_bind_destroy_message(duk_context * c)
{
	MSG * x = (MSG*)duk_to_uint(c, 0);
	free(x);
	return 0;
}
duk_ret_t vxw_bind_get_message(duk_context * c)
{
	HWND y = (HWND)duk_to_uint(c, 1);
	MSG * x = (MSG*)duk_to_uint(c, 0);
	duk_push_uint(c, GetMessage(x, y, 0, 0));
	return 1;
}
duk_ret_t vxw_bind_translate_message(duk_context * c)
{
	MSG * x = (MSG*)duk_to_uint(c, 0);
	TranslateMessage(x);
	return 0;
}
duk_ret_t vxw_bind_dispatch_message(duk_context * c)
{
	MSG * x = (MSG*)duk_to_uint(c, 0);
	DispatchMessage(x);
	return 0;
}
duk_ret_t vxw_bind_hwnd(duk_context * c)
{
	duk_push_uint(c, (unsigned int) xhwnd);
	return 1;
}
duk_ret_t vxw_bind_msg(duk_context * c)
{
	//MSG * x = (MSG*)duk_to_uint(c, 0);
	duk_push_uint(c, xmsg);
	return 1;
}
duk_ret_t vxw_bind_wparam(duk_context * c)
{
	//MSG * x = (MSG*)duk_to_uint(c, 0);
	duk_push_uint(c, xwparam);
	return 1;
}
duk_ret_t vxw_bind_lparam(duk_context * c)
{
	//MSG * x = (MSG*)duk_to_uint(c, 0);
	duk_push_uint(c, xlparam);
	return 1;
}
duk_ret_t vxw_bind_get_dc(duk_context * c)
{
	HWND h = (HWND)duk_to_uint(c, 0);
	HDC x = GetDC(h);
	duk_push_uint(c, (unsigned int)x);
	return 1;
}
duk_ret_t vxw_bind_release_dc(duk_context * c)
{
	HWND h = (HWND)duk_to_uint(c, 0);
	HDC x = (HDC)duk_to_uint(c, 1);
	ReleaseDC(h, x);
	return 0;
}
duk_ret_t vxw_bind_move_to(duk_context * c)
{
	HDC h = (HDC)duk_to_uint(c, 0);
	unsigned int x = duk_to_uint(c, 1);
	unsigned int y = duk_to_uint(c, 2);

	MoveToEx(h, x, y, NULL);
	return 0;
}
duk_ret_t vxw_bind_line_to(duk_context * c)
{
	HDC h = (HDC)duk_to_uint(c, 0);
	unsigned int x = duk_to_uint(c, 1);
	unsigned int y = duk_to_uint(c, 2);

	LineTo(h, x, y);
	return 0;
}
duk_ret_t vxw_bind_select_object(duk_context * c)
{
	HDC h = (HDC)duk_to_uint(c, 0);
	HGDIOBJ x = (HGDIOBJ)duk_to_uint(c, 1);
	SelectObject(h, x);
	return 0;
}
duk_ret_t vxw_bind_delete_object(duk_context * c)
{
	HGDIOBJ x = (HGDIOBJ)duk_to_uint(c, 0);
	DeleteObject(x);
	return 0;
}
duk_ret_t vxw_bind_color(duk_context * c)
{
	COLORREF z = RGB
		(
			duk_to_uint(c, 0),
			duk_to_uint(c, 1),
			duk_to_uint(c, 2)
		);
	duk_push_uint(c, z);
	return 1;
}
duk_ret_t vxw_bind_create_pen(duk_context * c)
{
	unsigned int w = duk_to_uint(c, 0);
	COLORREF z = duk_to_uint(c, 1);
	HPEN p = CreatePen(PS_SOLID, w, z);
	duk_push_uint(c, (unsigned int)p);
	return 1;
}
duk_ret_t vxw_bind_create_font(duk_context * c)
{
	HFONT x = CreateFont(duk_to_uint(c, 0), duk_to_uint(c, 1), 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, duk_to_string(c, 2));
	duk_push_uint(c, (unsigned int)x);
	return 1;
}
duk_ret_t vxw_bind_mci_send_string_(duk_context * c)
{
	const char* str = (char*)duk_to_string(c, 0);
	mciSendString(str, NULL, 0, 0);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT vxw_std_wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		Since it would be difficult creating seperate window procs for each window,
//		this is a standard one given to all created windows, puts the necessary information
//		in the central places as specified in vxprocess.h. This works for most real world
//		uses, given that certain identification numbers are truly unique.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	switch (msg)
	{
	default:
		xhwnd = hwnd;
		xmsg = msg;
		xwparam = wp;
		xlparam = lp;
		return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
vxbyte * vxprocess::static_bytes = NULL;
///////////////////////////////////////////////////////////////////////////////////////////////////
vxprocess::vxprocess(void)
{
	vxprocess::flag = 0;
	vxprocess::duk = duk_create_heap_default();

	duk_push_uint(vxprocess::duk, (unsigned int)stdin);
	duk_put_global_string(vxprocess::duk, "_vx_stdin_");

	duk_push_uint(vxprocess::duk, (unsigned int)stdout);
	duk_put_global_string(vxprocess::duk, "_vx_stdout_");

	duk_push_uint(vxprocess::duk, (unsigned int)stderr);
	duk_put_global_string(vxprocess::duk, "_vx_stderr_");
	
	duk_push_uint(vxprocess::duk, sizeof(char));
	duk_put_global_string(vxprocess::duk, "_vx_byte_");
	
	duk_push_uint(vxprocess::duk, sizeof(unsigned short));
	duk_put_global_string(vxprocess::duk, "_vx_word_");
	
	duk_push_uint(vxprocess::duk, sizeof(unsigned int));
	duk_put_global_string(vxprocess::duk, "_vx_dword_");

	duk_push_uint(vxprocess::duk, 1024*256);
	duk_put_global_string(vxprocess::duk, "_vx_maxbuff_");
	//
	//
	//
	//duk_push_string(vxprocess::duk, TRACKBAR_CLASS);
	//duk_put_global_string(vxprocess::duk, "_vxw_trackbar_");

	//duk_push_uint(vxprocess::duk, TBM_SETPOS);
	//duk_put_global_string(vxprocess::duk, "_vxw_getpos_");
	//
	//
	//
	duk_push_c_function(vxprocess::duk, vx_bind_exit, 1);
	duk_put_global_string(vxprocess::duk, "_vx_exit_");
	
	duk_push_c_function(vxprocess::duk, vx_bind_include, 1);
	duk_put_global_string(vxprocess::duk, "_vx_include_");

	duk_push_c_function(vxprocess::duk, vx_bind_put, 2);
	duk_put_global_string(vxprocess::duk, "_vx_putc_");

	duk_push_c_function(vxprocess::duk, vx_bind_get, 1);
	duk_put_global_string(vxprocess::duk, "_vx_getc_");

	duk_push_c_function(vxprocess::duk, vx_bind_print, 2);
	duk_put_global_string(vxprocess::duk, "_vx_print_");

	duk_push_c_function(vxprocess::duk, vx_bind_read, 1);
	duk_put_global_string(vxprocess::duk, "_vx_read_");
	
	duk_push_c_function(vxprocess::duk, vx_bind_fopen, 2);
	duk_put_global_string(vxprocess::duk, "_vx_fopen_");
	
	duk_push_c_function(vxprocess::duk, vx_bind_fclose, 1);
	duk_put_global_string(vxprocess::duk, "_vx_fclose_");
	
	duk_push_c_function(vxprocess::duk, vx_bind_sleep, 1);
	duk_put_global_string(vxprocess::duk, "_vx_sleep_");

	duk_push_c_function(vxprocess::duk, vx_bind_conopen, 0);
	duk_put_global_string(vxprocess::duk, "_vx_conopen_");

	duk_push_c_function(vxprocess::duk, vx_bind_conclose, 0);
	duk_put_global_string(vxprocess::duk, "_vx_conclose_");

	duk_push_c_function(vxprocess::duk, vx_bind_inline, 1);
	duk_put_global_string(vxprocess::duk, "_vx_inline_");

	duk_push_c_function(vxprocess::duk, vx_bind_alloc, 1);
	duk_put_global_string(vxprocess::duk, "_vx_alloc_");

	duk_push_c_function(vxprocess::duk, vx_bind_realloc, 2);
	duk_put_global_string(vxprocess::duk, "_vx_realloc_");
	  
	duk_push_c_function(vxprocess::duk, vx_bind_free, 1);
	duk_put_global_string(vxprocess::duk, "_vx_free_");

	duk_push_c_function(vxprocess::duk, vx_bind_cstring, 1);
	duk_put_global_string(vxprocess::duk, "_vx_cstring_");

	duk_push_c_function(vxprocess::duk, vx_bind_memcpy, 3);
	duk_put_global_string(vxprocess::duk, "_vx_memcpy_");

	duk_push_c_function(vxprocess::duk, vx_bind_putbyte, 2);
	duk_put_global_string(vxprocess::duk, "_vx_put_byte_");

	duk_push_c_function(vxprocess::duk, vx_bind_putword, 2);
	duk_put_global_string(vxprocess::duk, "_vx_put_word_");

	duk_push_c_function(vxprocess::duk, vx_bind_putdword, 2);
	duk_put_global_string(vxprocess::duk, "_vx_put_dword_");

	duk_push_c_function(vxprocess::duk, vx_bind_putint, 2);
	duk_put_global_string(vxprocess::duk, "_vx_put_int_");

	duk_push_c_function(vxprocess::duk, vx_bind_putdouble, 2);
	duk_put_global_string(vxprocess::duk, "_vx_put_double_");

	duk_push_c_function(vxprocess::duk, vx_bind_putstring, 2);
	duk_put_global_string(vxprocess::duk, "_vx_put_string_");

	duk_push_c_function(vxprocess::duk, vx_bind_getbyte, 1);
	duk_put_global_string(vxprocess::duk, "_vx_get_byte_");

	duk_push_c_function(vxprocess::duk, vx_bind_getword, 1);
	duk_put_global_string(vxprocess::duk, "_vx_get_word_");

	duk_push_c_function(vxprocess::duk, vx_bind_getdword, 1);
	duk_put_global_string(vxprocess::duk, "_vx_get_dword_");

	duk_push_c_function(vxprocess::duk, vx_bind_getint, 1);
	duk_put_global_string(vxprocess::duk, "_vx_get_int_");

	duk_push_c_function(vxprocess::duk, vx_bind_getdouble, 1);
	duk_put_global_string(vxprocess::duk, "_vx_get_double_");

	duk_push_c_function(vxprocess::duk, vx_bind_getstring, 1);
	duk_put_global_string(vxprocess::duk, "_vx_get_string_");

	duk_push_c_function(vxprocess::duk, vx_bind_getstatic, 0);
	duk_put_global_string(vxprocess::duk, "_vx_get_static_");

	duk_push_c_function(vxprocess::duk, vx_bind_setstatic, 1);
	duk_put_global_string(vxprocess::duk, "_vx_set_static_");

	duk_push_c_function(vxprocess::duk, vx_bind_brainfuck, 1);
	duk_put_global_string(vxprocess::duk, "_vx_brainfuck_");
	//
	//
	duk_push_c_function(vxprocess::duk, vxp_bind_create_process, 0);
	duk_put_global_string(vxprocess::duk, "_vxp_create_");

	duk_push_c_function(vxprocess::duk, vxp_bind_destroy_process, 1);
	duk_put_global_string(vxprocess::duk, "_vxp_destroy_");

	duk_push_c_function(vxprocess::duk, vxp_bind_is_active_process, 1);
	duk_put_global_string(vxprocess::duk, "_vxp_is_active_");

	duk_push_c_function(vxprocess::duk, vxp_bind_loadup_process, 2);
	duk_put_global_string(vxprocess::duk, "_vxp_loadup_");

	duk_push_c_function(vxprocess::duk, vxp_bind_run_process, 1);
	duk_put_global_string(vxprocess::duk, "_vxp_run_");
	
	duk_push_c_function(vxprocess::duk, vxp_bind_pass_string, 3);
	duk_put_global_string(vxprocess::duk, "_vxp_pass_string_");

	duk_push_c_function(vxprocess::duk, vxp_bind_pass_uint, 3);
	duk_put_global_string(vxprocess::duk, "_vxp_pass_uint_");

	duk_push_c_function(vxprocess::duk, vxp_bind_pass_int, 3);
	duk_put_global_string(vxprocess::duk, "_vxp_pass_int_");

	duk_push_c_function(vxprocess::duk, vxp_bind_pass_byte, 3);
	duk_put_global_string(vxprocess::duk, "_vxp_pass_byte_");
	//
	//
	//TODO:
	duk_push_c_function(vxprocess::duk, vxw_bind_create_directory, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_create_directory_");

	duk_push_c_function(vxprocess::duk, vxw_bind_delete_directory, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_delete_directory_");

	duk_push_c_function(vxprocess::duk, vxw_bind_copy_file, 2);
	duk_put_global_string(vxprocess::duk, "_vxw_copy_file_");

	duk_push_c_function(vxprocess::duk, vxw_bind_delete_file, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_delete_file_");
	//
	//
	//
	duk_push_c_function(vxprocess::duk, vxw_bind_move_cursor, 2);
	duk_put_global_string(vxprocess::duk, "_vxw_move_cursor_");

	duk_push_c_function(vxprocess::duk, vxw_bind_mouse_event, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_mouse_event_");

	duk_push_c_function(vxprocess::duk, vxw_bind_keyboard_event, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_keyboard_event_");

	duk_push_c_function(vxprocess::duk, vx_bind_error, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_error_");

	duk_push_c_function(vxprocess::duk, vxw_bind_sleep, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_sleep_");

	duk_push_c_function(vxprocess::duk, vxw_bind_beep, 2);
	duk_put_global_string(vxprocess::duk, "_vxw_beep_");

	duk_push_c_function(vxprocess::duk, vxw_bind_system, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_system_");

	duk_push_c_function(vxprocess::duk, vx_bind_message, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_dialog_");

	duk_push_c_function(vxprocess::duk, vxw_bind_openfiledialog, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_openfiledialog_");

	duk_push_c_function(vxprocess::duk, vxw_bind_savefiledialog, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_savefiledialog_");

	duk_push_c_function(vxprocess::duk, vxw_bind_send_message, 4);
	duk_put_global_string(vxprocess::duk, "_vxw_send_message_");

	duk_push_c_function(vxprocess::duk, vxw_bind_create_wndclassex, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_create_wndclassex_");
	
	duk_push_c_function(vxprocess::duk, vxw_bind_destroy_wndclassex, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_destroy_wndclassex_");
	
	duk_push_c_function(vxprocess::duk, vxw_bind_create_window, 10);
	duk_put_global_string(vxprocess::duk, "_vxw_create_window_");
	
	duk_push_c_function(vxprocess::duk, vxw_bind_destroy_window, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_destroy_window_");

	duk_push_c_function(vxprocess::duk, vxw_bind_show_window, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_show_window_");

	duk_push_c_function(vxprocess::duk, vxw_bind_update_window, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_update_window_");

	duk_push_c_function(vxprocess::duk, vxw_bind_create_message, 0);
	duk_put_global_string(vxprocess::duk, "_vxw_create_message_");

	duk_push_c_function(vxprocess::duk, vxw_bind_destroy_message, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_destroy_message_");

	duk_push_c_function(vxprocess::duk, vxw_bind_get_message, 2);
	duk_put_global_string(vxprocess::duk, "_vxw_get_message_");

	duk_push_c_function(vxprocess::duk, vxw_bind_translate_message, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_translate_message_");

	duk_push_c_function(vxprocess::duk, vxw_bind_dispatch_message, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_dispatch_message_");

	duk_push_c_function(vxprocess::duk, vxw_bind_hwnd, 0);
	duk_put_global_string(vxprocess::duk, "_vxw_hwnd_");

	duk_push_c_function(vxprocess::duk, vxw_bind_msg, 0);
	duk_put_global_string(vxprocess::duk, "_vxw_msg_");

	duk_push_c_function(vxprocess::duk, vxw_bind_wparam, 0);
	duk_put_global_string(vxprocess::duk, "_vxw_wparam_");

	duk_push_c_function(vxprocess::duk, vxw_bind_lparam, 0);
	duk_put_global_string(vxprocess::duk, "_vxw_lparam_");

	duk_push_c_function(vxprocess::duk, vxw_bind_get_dc, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_get_dc_");

	duk_push_c_function(vxprocess::duk, vxw_bind_release_dc, 2);
	duk_put_global_string(vxprocess::duk, "_vxw_release_dc_");

	duk_push_c_function(vxprocess::duk, vxw_bind_move_to, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_move_to_");

	duk_push_c_function(vxprocess::duk, vxw_bind_line_to, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_line_to_");

	duk_push_c_function(vxprocess::duk, vxw_bind_select_object, 2);
	duk_put_global_string(vxprocess::duk, "_vxw_select_object_");

	duk_push_c_function(vxprocess::duk, vxw_bind_delete_object, 1);
	duk_put_global_string(vxprocess::duk, "_vxw_delete_object_");

	duk_push_c_function(vxprocess::duk, vxw_bind_color, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_color_");

	duk_push_c_function(vxprocess::duk, vxw_bind_create_pen, 2);
	duk_put_global_string(vxprocess::duk, "_vxw_create_pen_");

	duk_push_c_function(vxprocess::duk, vxw_bind_create_font, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_create_font_");

	duk_push_c_function(vxprocess::duk, vxw_bind_mci_send_string_, 3);
	duk_put_global_string(vxprocess::duk, "_vxw_mci_");

	//Adding aliases:
	duk_peval_string(vxprocess::duk, "\
	vxInclude = _vx_include_;\
	_vx_system_ = _vxw_system_;\
	_vx_javascript_ = _vx_inline_;\
	_vx_char_ = _vx_byte_;\
	_vx_ushort_ = _vx_word_;\
	_vx_uint_ = _vx_dword_;\
	_vx_put_char_ = _vx_put_byte_;\
	_vx_put_ushort_ = _vx_put_word_;\
	_vx_put_uint_ = _vx_put_dword_;\
	_vx_get_char_ = _vx_get_byte_;\
	_vx_get_ushort_ = _vx_get_word_;\
	_vx_get_uint_ = _vx_get_dword_;\
	vxErrorMessage = _vxw_error_;");

	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
vxprocess::~vxprocess(void)
{
	vxprocess::flag = 0;
	duk_destroy_heap(vxprocess::duk);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::run(void)
{
	vxprocess::flag |= ACTIVE;
	_beginthread(vx_thread_bootstrap, 0, (void*) this);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::run(std::string s)
{
	vxprocess::loadup(s);
	vxprocess::flag |= ACTIVE;
	vxprocess::run();
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::on_run(void)
{
	if (!(vxprocess::code.empty()))
	{
		duk_peval_string(vxprocess::duk, vxprocess::code.c_str());
		vxprocess::flag -= ACTIVE;
	}
	else mess(ERR);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::pass_byte(std::string l, unsigned char x)
{
	duk_push_uint(vxprocess::duk, x);
	duk_put_global_string(vxprocess::duk, l.c_str());
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::pass_uint(std::string l, unsigned int x)
{
	duk_push_uint(vxprocess::duk, x);
	duk_put_global_string(vxprocess::duk, l.c_str());
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::pass_int(std::string l, signed int x)
{
	duk_push_int(vxprocess::duk, x);
	duk_put_global_string(vxprocess::duk, l.c_str());
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::pass_string(std::string l, std::string x)
{
	duk_push_string(vxprocess::duk, x.c_str());
	duk_put_global_string(vxprocess::duk, l.c_str());
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::loadup(std::string s)
{
	vxprocess::code = s;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void vxprocess::run_direct(std::string s)
{
	vxprocess::flag |= ACTIVE;
	duk_peval_string(vxprocess::duk, s.c_str());
	vxprocess::flag -= ACTIVE;
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////