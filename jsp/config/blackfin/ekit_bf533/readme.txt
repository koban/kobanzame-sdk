Device Drivers製 E!Kit-BF533対応システム依存部。

sys_config.hとsys_defs.hは、#include_nextを使って、それぞれblackfin/_common_bf533/sys_config.h
とblackfin/_common_bf533/sys_defs.hを読み込んでいる。

Makefile.configは、インクルード・パスおよびソースファイルへのパスとしてblackfin/_common_bf533も
指定している。

- Makefile.config : gnu環境のためのシステム依存部定義ファイル
- readme.txt : このファイル
- sys_config.h : システム依存部のうち、アプリケーションから見えない宣言
- sys_defs.h : システム依存部のうち、アプリケーションから見える宣言

E!Kit-BF533は、外部Flash ROMからNo BootでU-BOOTを起動する。このため、boot_for_debug()
ルーチンを呼ぶと、U-BOOTもろとも再起動してしまう。これを防ぐために、
config/blackfin/_common_bf533/chip_debugboot.c をリンク対象から外してある。