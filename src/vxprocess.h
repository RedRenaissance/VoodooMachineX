///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		RUBENS VOODOOMACHINEX
//
//		2020	-	2024
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include <string>
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "duktape.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTIVE 1
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef unsigned char vxbyte;
///////////////////////////////////////////////////////////////////////////////////////////////////
void vx_thread_bootstrap(void*);
///////////////////////////////////////////////////////////////////////////////////////////////////
static duk_ret_t vx_bind_exit(duk_context*);
static duk_ret_t vx_bind_include(duk_context*);
static duk_ret_t vx_bind_put(duk_context*);
static duk_ret_t vx_bind_get(duk_context*);

static duk_ret_t vx_bind_print(duk_context*);
static duk_ret_t vx_bind_read(duk_context*);

static duk_ret_t vx_bind_fopen(duk_context*);
static duk_ret_t vx_bind_fclose(duk_context*);
static duk_ret_t vx_bind_process(duk_context*);
static duk_ret_t vx_bind_sleep(duk_context*);
static duk_ret_t vx_bind_conopen(duk_context*);
static duk_ret_t vx_bind_conclose(duk_context*);
static duk_ret_t vx_bind_inline(duk_context*);
static duk_ret_t vx_bind_alloc(duk_context*);
static duk_ret_t vx_bind_realloc(duk_context*);
static duk_ret_t vx_bind_free(duk_context*);
static duk_ret_t vx_bind_memcpy(duk_context*);
static duk_ret_t vx_bind_putbyte(duk_context*);
static duk_ret_t vx_bind_putword(duk_context*);
static duk_ret_t vx_bind_putdword(duk_context*);
static duk_ret_t vx_bind_putqword(duk_context*);
static duk_ret_t vx_bind_putint(duk_context*);
static duk_ret_t vx_bind_putdouble(duk_context*);
static duk_ret_t vx_bind_putstring(duk_context*);
static duk_ret_t vx_bind_getbyte(duk_context*);
static duk_ret_t vx_bind_getword(duk_context*);
static duk_ret_t vx_bind_getdword(duk_context*);
static duk_ret_t vx_bind_getqword(duk_context*);
static duk_ret_t vx_bind_getint(duk_context*);
static duk_ret_t vx_bind_getdouble(duk_context*);
static duk_ret_t vx_bind_getstring(duk_context*);
static duk_ret_t vx_bind_setstatic(duk_context*);
static duk_ret_t vx_bind_getstatic(duk_context*);
static duk_ret_t vx_bind_error(duk_context*);
static duk_ret_t vx_bind_message(duk_context*);
static duk_ret_t vx_bind_brainfuck(duk_context*);
///////////////////////////////////////////////////////////////////////////////////////////////////
static duk_ret_t vxp_bind_create_process(duk_context*);
static duk_ret_t vxp_bind_destroy_process(duk_context*);
static duk_ret_t vxp_bind_is_active_process(duk_context*);
static duk_ret_t vxp_bind_loadup_process(duk_context*);
static duk_ret_t vxp_bind_run_process(duk_context*);
static duk_ret_t vxp_bind_pass_byte(duk_context*);
static duk_ret_t vxp_bind_pass_uint(duk_context*);
static duk_ret_t vxp_bind_pass_int(duk_context*);
static duk_ret_t vxp_bind_pass_string(duk_context*);
///////////////////////////////////////////////////////////////////////////////////////////////////
//TODO
static duk_ret_t vxw_bind_create_directory(duk_context*);
static duk_ret_t vxw_bind_delete_directory(duk_context*);
static duk_ret_t vxw_bind_delete_file(duk_context*);
static duk_ret_t vxw_bind_get_file_size(duk_context*);
static duk_ret_t vxw_bind_copy_file(duk_context*);
///////////////////////////////////////////////////////////////////////////////////////////////////

static duk_ret_t vxw_bind_move_cursor(duk_context*);
static duk_ret_t vxw_bind_mouse_event(duk_context*);
static duk_ret_t vxw_bind_keyboard_event(duk_context*);
static duk_ret_t vxw_bind_sleep(duk_context*);
static duk_ret_t vxw_bind_beep(duk_context*);
static duk_ret_t vxw_bind_system(duk_context*);
static duk_ret_t vxw_bind_openfiledialog(duk_context*);
static duk_ret_t vxw_bind_savefiledialog(duk_context*);
static duk_ret_t vxw_bind_send_message(duk_context*);
static duk_ret_t vxw_bind_load_icon(duk_context*);
static duk_ret_t vxw_bind_create_wndclassex(duk_context*);
static duk_ret_t vxw_bind_destroy_wndclassex(duk_context*);
static duk_ret_t vxw_bind_create_window(duk_context*);
static duk_ret_t vxw_bind_destroy_window(duk_context*);
static duk_ret_t vxw_bind_show_window(duk_context*);
static duk_ret_t vxw_bind_update_window(duk_context*);
static duk_ret_t vxw_bind_create_message(duk_context*);
static duk_ret_t vxw_bind_destroy_message(duk_context*);
static duk_ret_t vxw_bind_get_message(duk_context*);
static duk_ret_t vxw_bind_translate_message(duk_context*);
static duk_ret_t vxw_bind_dispatch_message(duk_context*);
static duk_ret_t vxw_bind_hwnd(duk_context*);
static duk_ret_t vxw_bind_msg(duk_context*);
static duk_ret_t vxw_bind_wparam(duk_context*);
static duk_ret_t vxw_bind_lparam(duk_context);
static duk_ret_t vxw_bind_get_dc(duk_context*);
static duk_ret_t vxw_bind_release_dc(duk_context*);
static duk_ret_t vxw_bind_move_to(duk_context*);
static duk_ret_t vxw_bind_line_to(duk_context*);
static duk_ret_t vxw_bind_select_object(duk_context*);
static duk_ret_t vxw_bind_delete_object(duk_context*);
static duk_ret_t vxw_bind_color(duk_context*);
static duk_ret_t vxw_bind_create_pen(duk_context*);
static duk_ret_t vxw_bind_create_font(duk_context*);
///////////////////////////////////////////////////////////////////////////////////////////////////
static LRESULT vxw_std_wndproc(HWND, UINT, WPARAM, LPARAM);
///////////////////////////////////////////////////////////////////////////////////////////////////
static HWND xhwnd = 0;
static UINT xmsg = 0;
static WPARAM xwparam = 0;
static LPARAM xlparam = 0;
//
//		To prevent all messages being dumped into oblivion,
//		all messages of all windows created in the VoodooMachineX are put here as
//		xhwnd,  xmsg, xwparam, and xlparam,
//		so all care must be taken that GUI elements all get UNIQUE identifiers.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class vxprocess
//This is the object that constitutes an instance of the machine, it runs in a seperate thread,
//and can create and run instances of itself to enable multithreading in javascript using the machine.
{
private:
	std::string code;
//	Place to store the javascript code.
	duk_context * duk;
//	The duktape javascript interpreter
public:
	unsigned short flag;
//	Place to store information about the state of this object.
	static vxbyte * static_bytes;
//	Really a workaround to make it easier to make threads talk to eachother but writing in the same memory.
	vxprocess(void);
	~vxprocess(void);
	void run(void);
	void run(std::string);
	void on_run(void);
	void pass_byte(std::string, unsigned char);
	void pass_uint(std::string, unsigned int);
	void pass_int(std::string, signed int);
	void pass_string(std::string, std::string);
	void loadup(std::string s);
	void run_direct(std::string);
};
///////////////////////////////////////////////////////////////////////////////////////////////////