
3Dmeps.dll: dlldata.obj 3Dme_p.obj 3Dme_i.obj
	link /dll /out:3Dmeps.dll /def:3Dmeps.def /entry:DllMain dlldata.obj 3Dme_p.obj 3Dme_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0500 /DREGISTER_PROXY_DLL \
		$<
# _WIN32_WINNT=0x0500 is for Win2000, change it to 0x0400 for NT4 or Win95 with DCOM

clean:
	@del 3Dmeps.dll
	@del 3Dmeps.lib
	@del 3Dmeps.exp
	@del dlldata.obj
	@del 3Dme_p.obj
	@del 3Dme_i.obj

3Dmeps.dll: dlldata.obj 3Dme_p.obj 3Dme_i.obj
	link /dll /out:3Dmeps.dll /def:3Dmeps.def /entry:DllMain dlldata.obj 3Dme_p.obj 3Dme_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0500 /DREGISTER_PROXY_DLL \
		$<
# _WIN32_WINNT=0x0500 is for Win2000, change it to 0x0400 for NT4 or Win95 with DCOM

clean:
	@del 3Dmeps.dll
	@del 3Dmeps.lib
	@del 3Dmeps.exp
	@del dlldata.obj
	@del 3Dme_p.obj
	@del 3Dme_i.obj
