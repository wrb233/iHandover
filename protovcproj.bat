@echo off
echo Setting up project ...

call %CPS_ROOT%\CPS\compile\pre_pro2vcproj.bat

echo qmake vcxproj......
qmake -tp vc -o iHandover.vcxproj iHandover.pro
echo qmake for vcxproj finished!

call %CPS_ROOT%\CPS\compile\post_pro2vcproj.bat

pause
