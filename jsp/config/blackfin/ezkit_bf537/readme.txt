Analog Devices製 EZ-KIT Lite BF537対応システム依存部。

sys_config.hとsys_defs.hは、#include_nextを使って、それぞれblackfin/_common_bf537/sys_config.h
とblackfin/_common_bf537/sys_defs.hを読み込んでいる。

Makefile.configは、インクルード・パスおよびソースファイルへのパスとしてblackfin/_common_bf537も
指定している。

- jsp_ezkit_bf537.dpj : VisualDSP++のためのプロジェクトファイル
- Makefile.config : gnu環境のためのシステム依存部定義ファイル
- readme.txt : このファイル
- sys_config.h : システム依存部のうち、アプリケーションから見えない宣言
- sys_defs.h : システム依存部のうち、アプリケーションから見える宣言