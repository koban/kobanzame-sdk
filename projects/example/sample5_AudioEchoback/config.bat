rem このbatはsuikan氏のjsp for blackfinから複製し改変したものである
rem APPNAME	アプリケーション名
rem CFGPATH 	cfg.exeが置いてあるディレクトリ
rem VDSPPATH 	VisualDSP++のインストールディレクトリ
rem JSPPATH  	TOPPERS/JSPのディレクトリ
rem CPATH	Blackfin CPU依存部のディレクトリ
rem SPATH 		システム依存部のディレクトリ
rem KZPATH		KOBANZAMEのlibディレクトリ

set APPNAME=sdkproject
set CFGPATH=..\..\..\maketool\win
set VDSPPATH=c:\Program Files\Analog Devices\VisualDSP 5.0
set JSPPATH=..\..\..\jsp
set CPATH=%JSPPATH%\config\blackfin
set SPATH=%CPATH%\ezkit_bf533;%CPATH%\_common_bf533
set KZPATH=..\..\..\lib

"%VDSPPATH%\pp.exe" %APPNAME%.cfg -D__ECC__ -D__ADSPLPBLACKFIN__ -I"%CPATH%;%SPATH%;%JSPPATH%\systask;%JSPPATH%\include;%KZPATH%" > %APPNAME%_pp.cfg
"%CFGPATH%\cfg.exe" %APPNAME%_pp.cfg
del %APPNAME%_pp.cfg
