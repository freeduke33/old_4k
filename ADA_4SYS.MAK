.AUTODEPEND

#		*Translator Definitions*
CC = bcc +ADA_4SYS.CFG
TASM = TASM
TLINK = tlink


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
  ada_4sys.obj \
  wz_s.obj \
  stat_har.obj \
  out_tb_g.obj \
  dac_adc.obj \
  dsys.obj \
  gl_4k1.obj \
  poly_sin.obj \
  sys_har.obj \
  etalon.obj \
  reports.obj \
  vars.obj \
  p_garm.obj \
  inf_rab.obj \
  inf_form.obj \
  inf_file.obj \
  _grf.lib \
  _tcxlbcl.lib \
  tcxlbcl.lib

#		*Explicit Rules*
ada_4sys.exe: ada_4sys.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/LD:\BC\LIB @&&|
c0l.obj+
ada_4sys.obj+
wz_s.obj+
stat_har.obj+
out_tb_g.obj+
dac_adc.obj+
dsys.obj+
gl_4k1.obj+
poly_sin.obj+
sys_har.obj+
etalon.obj+
reports.obj+
vars.obj+
p_garm.obj+
inf_rab.obj+
inf_form.obj+
inf_file.obj
ada_4sys
		# no map file
_grf.lib+
_tcxlbcl.lib+
tcxlbcl.lib+
emu.lib+
mathl.lib+
cl.lib+
graphics.lib
|


#		*Individual File Dependencies*
ada_4sys.obj: ada_4sys.c 

wz_s.obj: wz_s.c 

stat_har.obj: stat_har.c 

out_tb_g.obj: out_tb_g.c 

dac_adc.obj: dac_adc.c 

dsys.obj: dsys.c 

gl_4k1.obj: gl_4k1.c 

poly_sin.obj: poly_sin.c 

sys_har.obj: sys_har.c 

etalon.obj: etalon.c 

reports.obj: reports.c 

vars.obj: vars.c 

p_garm.obj: p_garm.c 

inf_rab.obj: inf_rab.c 

inf_form.obj: inf_form.c 

inf_file.obj: inf_file.c 

#		*Compiler Configuration File*
ada_4sys.cfg: ada_4sys.mak
  copy &&|
-ml
-v
-ID:\BC\INCLUDE;D:\BC\INCLUDE\TCXL_IN
-LD:\BC\LIB
| ada_4sys.cfg


