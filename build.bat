setlocal
set src= src\bmfnt.c src\plugin_startup.cxx
set cl_flags=/EHsc /LD /FeAGSBMFontRenderer.dll
set link_flags=/link /DLL user32.lib

cl %cl_flags% %src% %link_flags%

del *.exp
del *.lib
del *.obj

endlocal

