set CURDIR=%cd%
set QTBIN=D:\Qt\5.12.1\mingw73_64\bin
set BIN=D:\GitRepos\football\build-football-Qt_5_12_1_MinGW_64_bit_197311-Release
set APPNAME=Razdevalka.exe
set PROJ=D:\GitRepos\football\football
set PKG=C:\Users\Yan\Desktop\Package
set EXTRADLLS=D:\GitRepos\football\football\extraDLLs
set REMDLLS=(D3Dcompiler_47.dll libEGL.dll libGLESV2.dll opengl32sw.dll)
set REMDIRS=(bearer iconengines qmltooling QtQml styles translations)

if not exist "%PKG%" (md "%PKG%")

cd /D %PKG%
for /F "delims=" %%i in ('dir /b') do (rd "%%i" /S/Q || del "%%i" /S/Q)
cd /D %CURDIR%

xcopy /Y "%BIN%\%APPNAME%" "%PKG%\"
"%QTBIN%\windeployqt.exe" "%PKG%\%APPNAME%" --release --qmldir "%PROJ%\qml"

xcopy /Y "%EXTRADLLS%\*.dll" "%PKG%\"
xcopy /Y "%PROJ%\games" "%PKG%\"
xcopy /Y "%PROJ%\players" "%PKG%\"
xcopy /Y "%PROJ%\config" "%PKG%\"

for %%d in %REMDLLS% do del /Q "%PKG%\%%d"
for %%d in %REMDIRS% do rd /S/Q "%PKG%\%%d"