KOBANZAME SDK Project Sample Code

Copyright (C), KOBANZAME SDK Project, All right reserved

このファイルは KOBANZAME SDK Project のサンプルコード説明です。
This file is explanation for KOBANZAME SDK Project Sample Code.

+++++++++++++++++++++++
 sample1_Hello
+++++++++++++++++++++++

main_tsk.c に定番のprintf("Hello World\n"); が記述されています。
これを実行するとシリアルに　Hello World と表示されプログラムが終了します。

+++++++++++++++++++++++
 sample2_helloCmd
+++++++++++++++++++++++

上記sample1のHello Worldを、コマンドにアサインする方法が示されています。main_tsk.cを参照してください。
コマンドラインで
hello と打つと　Hello World と表示されます

+++++++++++++++++++++++
 sample3_MultiTask
+++++++++++++++++++++++

マルチタスクを構成する方法が示されています。main_tsk.c と　MultiTask.cfg を参考にしてください。
config.batをダブルクリックすることでMultiTask.cfg から　kernel_cfg.c と kernel_id.h が生成されます。
優先順位の低いタスクでLow Priority Task Running　が表示されます。それを縫って優先順位の高いタスクが '*'を非同期に表示します。

+++++++++++++++++++++++
 sample4_Semaphore
+++++++++++++++++++++++

上記のsample3では非同期に'*' が表示されましたが、このサンプルでは同期的に表示します。
優先順位が低いタスクでも資源を保護するサンプルコードを示しています。
sample3と同様、main_tsk.c と MultiTask.cfg を参照して下さい。

+++++++++++++++++++++++
 sample5_AudioEchoback
+++++++++++++++++++++++

main_tsk.c にオーディオをエコーバックする方法を示しています。
Audio Input ChannelをそのままOutput Channelにコピーしています。

+++++++++++++++++++++++
 sample6_AudioCenterCancel
+++++++++++++++++++++++

main_tsk.c にオーディオの中心に定位している成分を除去する方法を示しています。
Audio Channel L から　Audio Cannel R を減算しています。

+++++++++++++++++++++++
 sample7_IIR
+++++++++++++++++++++++

main_tsk.c に IIR Filter を構成する例を示しています。
係数計算には以下のサイトを利用させていただいています。
http://momiji.i.ishikawa-nct.ac.jp/dfdesign/iir/i_lpf.shtml

+++++++++++++++++++++++
 sample8_FIR
+++++++++++++++++++++++

main_tsk.c に　FIR Fileter を構成する例を示しています。
係数計算には以下のサイトを利用させていただいています。
http://momiji.i.ishikawa-nct.ac.jp/dfdesign/fir/remez_b.shtml
