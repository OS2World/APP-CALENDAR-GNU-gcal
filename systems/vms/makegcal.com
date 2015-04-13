$ ! Makefile for VMS
$ ! DCL-Shell-language. Edit the symbols section at the end.
$ !
$ On Control_Y Then Goto The_Exit
$ On Error Then Goto The_Exit
$ define/user sys sys$library
$ cc gcal.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER=1) /define(USE_RC=1)
$ define/user sys sys$library
$ cc file-io.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc hd-data.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc hd-use.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc help.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER=1) /define(USE_RC=1)
$ define/user sys sys$library
$ cc print.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc rc-check.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc rc-insert.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc rc-use.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc rc-utils.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc regexp.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ define/user sys sys$library
$ cc tty.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER=1) /define(USE_RC=1)
$ define/user sys sys$library
$ cc utils.c /define=(VAXC) /define(USE_HLS=1) /define(USE_PAGER) /define(USE_RC=1)
$ linkobjs:=gcal.obj file-io.obj help.obj holiday.obj print.obj rc-check.obj rc-insert.obj rc-use.obj rc-utils.obj regexp.obj tty.obj utils.obj
$ Loop:
$ p = f$locate(" ",linkobjs)
$ if p .lt. f$length(linkobjs)
$ then linkobjs[p,1]:=","
$ goto Loop
$ endif
$ write sys$output "linking ''linkobjs'"
$ link /exec=gcal.exe 'linkobjs',sys$input/opt
SYS$LIBRARY:VAXCRTL/SHARE
$ !
$ define/user sys sys$library
$ cc tcal.c
$ linkobjs:=tcal.obj
$ write sys$output "linking ''linkobjs'"
$ link /exec=tcal.exe 'linkobjs',sys$input/opt
SYS$LIBRARY:VAXCRTL/SHARE
$ !
$ define/user sys sys$library
$ cc txt2gcal.c
$ linkobjs:=txt2gcal.obj
$ write sys$output "linking ''linkobjs'"
$ link /exec=txt2gcal.exe 'linkobjs',sys$input/opt
SYS$LIBRARY:VAXCRTL/SHARE
$ !
$ define/user sys sys$library
$ cc gcal2txt.c
$ linkobjs:=gcal2txt.obj
$ write sys$output "linking ''linkobjs'"
$ link /exec=gcal2txt.exe 'linkobjs',sys$input/opt
SYS$LIBRARY:VAXCRTL/SHARE
$ !
$ ! Set up symbols for the gcal, tcal, txt2gcal and gcal2txt executables.
$ ! Edit the example below, changing "disk:[directory]" as appropriate.
$ !
$ gcal == "$disk:[directory]gcal.exe"
$ tcal == "$disk:[directory]tcal.exe"
$ txt2gcal == "$disk:[directory]txt2gcal.exe"
$ gcal2txt == "$disk:[directory]gcal2txt.exe"
$ !
$ The_Exit:
$ Save_Status = $STATUS
$ exit Save_Status
