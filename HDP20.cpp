// ---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：出図要求受信サービス(部品展開処理)
// 作 成 者：N.Uchida
// 作 成 日：2002.06.18
// 更 新 日：2002.08.23
// 2007.10.04 ﾐﾗｰ仕様対応
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>

#include <dir.h>
#include <io.h>
#include <direct.h>

#pragma hdrstop

#include <sys\stat.h>

#include "HDPConst.h"         // 定数定義用
#include "HDPValue.h"         // 変数定義用
#include "HDPCom.h"           // 関数定義用

#include "DataModule.h"

#include "HDP20.h"
#include "Bas_PlateDetails.h"    // ﾌﾟﾚｰﾄ部品展開処理
#include "Bas_FrameDetails.h"    // ﾌﾚｰﾑ部品展開処理
#include "Bas_NozzleDetails.h"   // ﾉｽﾞﾙ部品展開処理
#include "Bas_TempDetails.h"     // 温度計部品展開処理
#include "Bas_PressGDetails.h"   // 圧力計部品展開処理
#include "Bas_AxsrsDetails.h"    // 付属品部品展開処理

#include "HeadsData.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmHDP020 *frmHDP020;

// ---------------------------------------------------------------------------
__fastcall TfrmHDP020::TfrmHDP020(TComponent* Owner) : TForm(Owner) {
}

// ---------------------------------------------------------------------------
// 日本語関数名： ウィンドウ生成時処理
// 概  要      ： 表示初期化をする
// 引  数      ： 初期値
// 戻り値      ： なし
// 備  考      ： ｼｽﾃﾑ設定読込み
// ---------------------------------------------------------------------------
void __fastcall TfrmHDP020::FormCreate(TObject *Sender) {
    // INI設定ファイルの読み込み
    IniFileRead();
    IniFileRead_Parts();

    // *************************
    // ***  ﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
    // *************************
    if (!Log_File_SizeCheck()) {
        return;
    }

    // ﾛｸﾞ作成
    G_Log_Text =
        "*****************************************************************************";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text =
        "*****                                                                   *****";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text = "*****                  部品展開処理システムを起動しました。             *****";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text =
        "*****                                                                   *****";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    G_Log_Text =
        "*****************************************************************************";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // ***************************
    // ***  部品展開処理　開始
    // ***************************
    Heads_Parts_Main();

    Screen->Cursor = crDefault;
//    PostMessage(Handle, WM_CLOSE, 0, 0);

//    Release(); // フォームを破棄し，関連付けられているメモリを解放します。

    Application->Terminate();
}

// ---------------------------------------------------------------------------
// 日本語関数名：
// 概  要      ：
// 引  数      ： 初期値
// 戻り値      ： なし
// 備  考      ： なし
// ---------------------------------------------------------------------------
void __fastcall TfrmHDP020::Heads_Parts_Main(void) {
    AnsiString s_FilePass;
    AnsiString hs_FilePass;
    bool bRet;
    HeadsData* pcHeadsDataNone;
    HeadsData* pcHeadsDataMrr;
    HeadsData* pcHeadsData;
    int i;
    int j;

    try {
        Refresh();
        Screen->Cursor = crHourGlass;

        // *************************
        // ***  作番取得
        // *************************
        if (!Get_Sakuban()) {
            return;
        }

        // **********************************
        // ***  規格ｺｰﾄﾞ,業界,塗装色  読込
        // **********************************
        if (!Read_CodeMasta_Data()) {
            return;
        }

        // *************************
        // ***  ﾃﾞｰﾀﾍﾞｰｽ接続
        // *************************
        if (!Connect_DataBase()) {
            return;
        }

        // ******************************
        // ***  CSVﾌｧｲﾙ出力先ﾌｫﾙﾀﾞﾁｪｯｸ
        // ******************************
        if (P_Sakuban_Mode == "S") {
            // ｵｰﾀﾞｰ
            G_csv_Output_Dir = G_csv_Output_Dir + G_OUT_ORDER_Dir;

            // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ格納先
            // マルチサーバ対応 2009/1/13 MOD START
            // G_AnaFilePass = IncludeTrailingBackslash(G_HD_Root)
            // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
            // + IncludeTrailingBackslash(G_DAT_ORDER_Dir);
            G_AnaFilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
                IncludeTrailingBackslash(G_DAT_ORDER_Dir);
            // マルチサーバ対応 2009/1/13 MOD END

            // 予備品ﾃﾞｰﾀ読込先
            // G_YobiFilePass = G_Input_Csv_Dir;
        }
        else {
            // 見積
            G_csv_Output_Dir = G_csv_Output_Dir + G_OUT_ESTIM_Dir;

            // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ格納先
            // マルチサーバ対応 2009/1/13 MOD START
            // G_AnaFilePass = IncludeTrailingBackslash(G_HD_Root)
            // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
            // + IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
            G_AnaFilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
                IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
            // マルチサーバ対応 2009/1/13 MOD END

            // 予備品ﾃﾞｰﾀ読込先
            // G_YobiFilePass = G_Input_Csv_Dir;
        }

        // 部品展開ﾃﾞｰﾀ出力先
        G_csv_Output_Dir = IncludeTrailingBackslash(G_csv_Output_Dir) +
            IncludeTrailingBackslash(G_SAKUBAN);

        // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ格納先
        G_AnaFilePass = G_AnaFilePass + IncludeTrailingBackslash(G_SAKUBAN);

        // 予備品ﾃﾞｰﾀ格納先
        // G_YobiFilePass =
        if (!OutPut_Folder_Check(G_csv_Output_Dir)) {
            return;
        }

        // *******************************************
        // ***  HEADSﾃﾞｰﾀ　読込
        // ***  ( 02.08.07 読込ﾌｧｲﾙ名変更 ) csv→txt
        // *******************************************
        // s_FilePass = G_Input_Csv_Dir + G_HEADS_ID;
        s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;

        // ------------------------------------------------------------------
        // 2007.10.02 ﾐﾗｰ仕様対応 最大3ﾌｧｲﾙ読込
        // 変数G_HEADS_DATAを使用せず、HeadsDataｸﾗｽ変数を使用する方式に変更
        /*
         if (!Read_HEADS_Data(s_FilePass)){
         G_ErrLog_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』のＯＰＥＮに失敗しました！！";
         Write_Error_Log(G_ErrLog_Text);
         return;
         } else {
         G_Log_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』を読み込みました。";
         Write_Log(G_Log_Text);
         }
         */

        // 元仕様用のHEADSテキストデータ
        pcHeadsDataNone = new HeadsData();

        // HEADSテキストデータ(元仕様)読込
        bRet = pcHeadsDataNone->ReadAllData(s_FilePass.c_str());

        if (!bRet) {
            G_ErrLog_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』(元仕様)のＯＰＥＮに失敗しました！！";
            Write_Error_Log(G_ErrLog_Text);
            return;
        }
        else {
            G_Log_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』(元仕様)を読み込みました。";
            Write_Log(G_Log_Text);
        }

        // ミラー仕様の場合は、A、Bファイルも読み込む
        if (pcHeadsDataNone->GetMirrorFlag()) {

            // ミラー仕様用のHEADSテキストデータ
            pcHeadsDataMrr = new HeadsData[2];

            // HEADSテキストデータ(ミラーA仕様)読込
            G_HEADS_ID = G_SAKUBAN + "A" + ".txt";
            s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;
            bRet = pcHeadsDataMrr[0].ReadAllData(s_FilePass.c_str());

            if (!bRet) {
                G_ErrLog_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass +
                    "』(ﾐﾗｰA仕様)のＯＰＥＮに失敗しました！！";
                Write_Error_Log(G_ErrLog_Text);
                return;
            }
            else {
                G_Log_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』(ﾐﾗｰA仕様)を読み込みました。";
                Write_Log(G_Log_Text);
            }

            // HEADSテキストデータ(ミラーB仕様)読込
            G_HEADS_ID = G_SAKUBAN + "B" + ".txt";
            s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;
            bRet = pcHeadsDataMrr[1].ReadAllData(s_FilePass.c_str());

            if (!bRet) {
                G_ErrLog_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass +
                    "』(ﾐﾗｰB仕様)のＯＰＥＮに失敗しました！！";
                Write_Error_Log(G_ErrLog_Text);
                return;
            }
            else {
                G_Log_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』(ﾐﾗｰB仕様)を読み込みました。";
                Write_Log(G_Log_Text);
            }
        }

        // 2007.10.02 ﾐﾗｰ仕様対応 最大3ﾌｧｲﾙ読込
        // ------------------------------------------------------------------

        // *************************
        // ***  HD_PARTSの削除
        // *************************
        s_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
        DeleteFile(s_FilePass);

        // ****************************
        // ***  HEARTS用ﾃﾞｰﾀ削除
        // ****************************
        if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
            hs_FilePass = IncludeTrailingBackslash
                (G_xls_Output_Dir + G_OUT_ORDER_Dir);
            hs_FilePass = IncludeTrailingBackslash(hs_FilePass + G_SAKUBAN);
            hs_FilePass = hs_FilePass + G_SAKUBAN + ".csv";

            // hs_FilePass = G_xls_Output_Dir + G_OUT_ORDER_Dir
            // + "\\" + G_SAKUBAN + "\\"
            // + G_SAKUBAN + ".csv";
        }
        else {
            hs_FilePass = IncludeTrailingBackslash
                (G_xls_Output_Dir + G_OUT_ESTIM_Dir);
            hs_FilePass = IncludeTrailingBackslash(hs_FilePass + G_SAKUBAN);
            hs_FilePass = hs_FilePass + G_SAKUBAN + ".csv";

            // hs_FilePass = G_xls_Output_Dir + G_OUT_ESTIM_Dir
            // + "\\" + G_SAKUBAN + "\\"
            // + G_SAKUBAN + ".csv";
        }
        DeleteFile(hs_FilePass);

        // ---------------------------------------------
        // 2007.10.02 ﾐﾗｰ仕様対応
        // HEADSﾃｷｽﾄﾌｧｲﾙ数分ﾙｰﾌﾟする

        for (i = 0; i < 3; i++) {

            // 予備品重複上防止用ｶｳﾝﾀ初期化
            G_YOBICHK_RecCnt = 0;

            // 2007.10.03 追加
            // 念のため予備品重複上防止用メモ配列も初期化
            for (j = 0; j < 100; j++) {
                G_YOBICHK_NAME[j] = "";
                G_YOBICHK_MAT[j] = "";
            }

            // --------------------------
            // 処理する仕様の変数をｾｯﾄ
            // --------------------------
            switch (i) {
            case 0:
                pcHeadsData = pcHeadsDataNone; // 元仕様
                break;
            case 1:
                pcHeadsData = &pcHeadsDataMrr[0]; // ﾐﾗｰA仕様
                break;
            case 2:
                pcHeadsData = &pcHeadsDataMrr[1]; // ﾐﾗｰB仕様
                break;
            }

            // 仕様ﾀｲﾌﾟ、仕様ﾀｲﾌﾟｺｰﾄﾞをｾｯﾄ
            pcHeadsData->SetSpecType(i);
            pcHeadsData->SetSpecCode();

            // ﾐﾗｰ仕様の場合、元仕様の処理は行わない
            if ((i == 0) && (pcHeadsDataNone->GetMirrorFlag()))
                continue;

            // *************************************
            // ***  ﾌﾟﾚｰﾄ部品　展開処理
            // *************************************
            if (!PlateDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  ﾌﾚｰﾑ部品　展開処理
            // *************************************
            if (!FrameDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  ﾉｽﾞﾙ部品　展開処理
            // *************************************
            if (!NozzleDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  温度計部品　展開処理
            // *************************************
            if (!TempDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  圧力計部品　展開処理
            // *************************************
            if (!PressGDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // *************************************
            // ***  付属品部品　展開処理
            // *************************************
            if (!AxsrsDetails(pcHeadsData)) {
                delete DataModule1;
                return;
            }

            // ﾐﾗｰ仕様でない場合、元仕様のみ処理して抜ける
            if (!pcHeadsDataNone->GetMirrorFlag())
                break;
        }

        if (pcHeadsDataNone->GetMirrorFlag())
            delete[]pcHeadsDataMrr;
        delete pcHeadsDataNone;

        // 2007.10.02 ﾐﾗｰ仕様対応
        //
        // ---------------------------------------------

        delete DataModule1;

        // ﾛｸﾞ作成
        G_Log_Text = "部品展開処理システムを終了しました。\n\n\n\n\n";
        Write_Log(G_Log_Text);
        Write_Error_Log(G_Log_Text);
    }
    __finally {
        Screen->Cursor = crDefault;
        Close();
        // PostMessage(Handle, WM_CLOSE, 0,0);
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： ログファイル　サイズチェック
// 概  要      ： ﾛｸﾞﾌｧｲﾙのｻｲｽﾞを取得し、規定値を超えていたら過去ﾌｫﾙﾀﾞに移動させる
// 引  数      ： なし
// 戻り値      ： 結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Log_File_SizeCheck(void) {
    AnsiString to_filename; // 作成過去ﾛｸﾞﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
    struct stat statbuf;
    double d_FileSize;

    // ***************************
    // ***  ﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
    // ***************************
    // 作成先ﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
    stat(G_LogFilePath.c_str(), &statbuf);
    d_FileSize = statbuf.st_size;

    // ﾛｸﾞﾌｧｲﾙのｻｲｽﾞが設定値以上なら『年月日時分秒』+ﾌｧｲﾙ名で過去ﾛｸﾞﾌｫﾙﾀﾞへｺﾋﾟｰ
    if (d_FileSize >= G_Log_Max_Size) {

        // 過去ﾛｸﾞ用ﾊﾟｽ作成
        to_filename = G_Rsrv_Log_Dir;
        to_filename = to_filename + FormatDateTime("yyyymmdd", Date());
        to_filename = to_filename + FormatDateTime("hhnnss", Time()) +
            CSYSLOGFILENAME_P;

        if (CopyFile(G_LogFilePath.c_str(), to_filename.c_str(), false)) {
            DeleteFile(G_LogFilePath.c_str());
        }
        else {
            return false;
        }
    }

    // ***************************
    // ***  ｴﾗｰﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
    // ***************************
    // 作成先ｴﾗｰﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
    stat(G_ErrLogFilePath.c_str(), &statbuf);
    d_FileSize = statbuf.st_size;

    // ｴﾗｰﾛｸﾞﾌｧｲﾙのｻｲｽﾞが設定値以上なら『年月日時分秒』+ﾌｧｲﾙ名で過去ｴﾗｰﾛｸﾞﾌｫﾙﾀﾞへｺﾋﾟｰ
    if (d_FileSize >= G_ErrLog_Max_Size) {

        // 過去ｴﾗｰﾛｸﾞ用ﾊﾟｽ作成
        to_filename = G_Rsrv_ErrLog_Dir;
        to_filename = to_filename + FormatDateTime("yyyymmdd", Date());
        to_filename = to_filename + FormatDateTime("hhnnss", Time()) +
            CSYSERRLOGFILENAME_P;

        if (CopyFile(G_ErrLogFilePath.c_str(), to_filename.c_str(), false)) {
            DeleteFile(G_ErrLogFilePath.c_str());
        }
        else {
            return false;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 規格ｺｰﾄﾞ，業界ｺｰﾄﾞ，塗装色　読込
// 概  要      ： 『KIKAKU.CSV』,『GYOKAI.CSV』,『TOSOU.CSV』を読み込む
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Read_CodeMasta_Data(void) {
    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    char token[100][100]; /* トークンバッファ */

    int RecNo;

    AnsiString wk_FilePass;
    FILE *fp;

    // ***********************
    // **  規格ｺｰﾄﾞ ｾｯﾄ
    // ***********************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "KIKAKU.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // トークンカウンタ初期化
        memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
        tok = GetToken(buf, ","); // ","で項目を区切る
        while (tok) { // トークンが存在する間
            sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
            tok = GetToken(NULL, ","); // 次のトークンを検索

            if (i > 70) {
                break;
            }
        }

        G_KIKAKU[RecNo][0] = token[0];
        G_KIKAKU[RecNo][1] = token[1];

        RecNo++;
    }

    fclose(fp); // 閉じる

    G_KIKAKU_RecCnt = RecNo;

    // ***********************
    // **  業界ｺｰﾄﾞ ｾｯﾄ
    // ***********************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "GYOKAI.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // トークンカウンタ初期化
        memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
        tok = GetToken(buf, ","); // ","で項目を区切る
        while (tok) { // トークンが存在する間
            sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
            tok = GetToken(NULL, ","); // 次のトークンを検索

            if (i > 70) {
                break;
            }
        }

        G_GYOKAI[RecNo][0] = token[0];
        G_GYOKAI[RecNo][1] = token[1];

        RecNo++;
    }

    fclose(fp); // 閉じる

    G_GYOKAI_RecCnt = RecNo;

    // ***********************
    // **  塗装色 ｾｯﾄ
    // ***********************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "TOSOU.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // トークンカウンタ初期化
        memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
        tok = GetToken(buf, ","); // ","で項目を区切る
        while (tok) { // トークンが存在する間
            sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
            tok = GetToken(NULL, ","); // 次のトークンを検索

            if (i > 70) {
                break;
            }
        }

        G_TOSOU[RecNo][0] = token[0];
        G_TOSOU[RecNo][1] = token[1];
        G_TOSOU[RecNo][2] = token[2];

        RecNo++;
    }

    fclose(fp); // 閉じる

    G_TOSOU_RecCnt = RecNo;

    // ***************************
    // **  ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙ
    // ***************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "PHENKAN.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // トークンカウンタ初期化
        memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
        tok = GetToken(buf, ","); // ","で項目を区切る
        while (tok) { // トークンが存在する間
            sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
            tok = GetToken(NULL, ","); // 次のトークンを検索

            if (i > 100) {
                break;
            }
        }

        G_PHENKAN[RecNo][0] = token[0];
        G_PHENKAN[RecNo][1] = token[1];
        G_PHENKAN[RecNo][2] = token[2];
        G_PHENKAN[RecNo][3] = token[3];
        G_PHENKAN[RecNo][4] = token[4];
        G_PHENKAN[RecNo][5] = token[5];
        G_PHENKAN[RecNo][6] = token[6];
        G_PHENKAN[RecNo][7] = token[7];
        G_PHENKAN[RecNo][8] = token[8];
        G_PHENKAN[RecNo][9] = token[9];

        RecNo++;
    }

    fclose(fp); // 閉じる

    G_PHENKAN_RecCnt = RecNo;

    // ******************************
    // **  ﾉｽﾞﾙ構成部品ｺｰﾄﾞ ﾃｰﾌﾞﾙ
    // ******************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "NOZZLECD.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // トークンカウンタ初期化
        memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
        tok = GetToken(buf, ","); // ","で項目を区切る
        while (tok) { // トークンが存在する間
            sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
            tok = GetToken(NULL, ","); // 次のトークンを検索

            if (i > 100) {
                break;
            }
        }

        G_NOZZLECD[RecNo][0] = token[0]; // ｺｰﾄﾞ
        G_NOZZLECD[RecNo][1] = token[1]; // 部品分類ｺｰﾄﾞ
        G_NOZZLECD[RecNo][2] = token[2]; // 備考(部品区分)
        G_NOZZLECD[RecNo][3] = token[3]; // 備考(部品名称)
        G_NOZZLECD[RecNo][4] = token[4]; // 予備品集計用(?)
        G_NOZZLECD[RecNo][5] = token[5]; // 連番(?)

        RecNo++;
    }

    fclose(fp); // 閉じる

    G_NOZZLECD_RecCnt = RecNo;

    // ******************************
    // **  温度計構成部品ｺｰﾄﾞ ﾃｰﾌﾞﾙ
    // ******************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "TEMPCD.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // トークンカウンタ初期化
        memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
        tok = GetToken(buf, ","); // ","で項目を区切る
        while (tok) { // トークンが存在する間
            sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
            tok = GetToken(NULL, ","); // 次のトークンを検索

            if (i > 100) {
                break;
            }
        }

        G_TEMPCD[RecNo][0] = token[0]; // ｺｰﾄﾞ
        G_TEMPCD[RecNo][1] = token[1]; // 部品分類ｺｰﾄﾞ
        G_TEMPCD[RecNo][2] = token[2]; // 備考(部品区分)
        G_TEMPCD[RecNo][3] = token[3]; // 備考(部品名称)
        G_TEMPCD[RecNo][4] = token[4]; // 予備品集計用(?)
        G_TEMPCD[RecNo][5] = token[5]; // 連番(?)

        RecNo++;
    }

    fclose(fp); // 閉じる

    G_TEMPCD_RecCnt = RecNo;

    // ******************************
    // **  圧力計構成部品ｺｰﾄﾞ ﾃｰﾌﾞﾙ
    // ******************************
    wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir) + "PRESSGCD.csv";

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

    // 読込み可能の間
    RecNo = 0;
    memset(buf, 0x00, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        i = 0; // トークンカウンタ初期化
        memset(token, 0x00, sizeof(token)); // トークンバッファ初期化
        tok = GetToken(buf, ","); // ","で項目を区切る
        while (tok) { // トークンが存在する間
            sprintf(token[i++], "%-.99s", tok); // トークンをバッファに転送
            tok = GetToken(NULL, ","); // 次のトークンを検索

            if (i > 100) {
                break;
            }
        }

        G_PRESSCD[RecNo][0] = token[0]; // ｺｰﾄﾞ
        G_PRESSCD[RecNo][1] = token[1]; // 識別子
        G_PRESSCD[RecNo][2] = token[2]; // 備考(?)
        G_PRESSCD[RecNo][3] = token[3]; // 部品名称
        G_PRESSCD[RecNo][4] = token[4]; // 予備品

        RecNo++;
    }

    fclose(fp); // 閉じる

    G_PRESSCD_RecCnt = RecNo;

    // delete fp;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： データベース　接続
// 概  要      ： mdbをODBC経由で接続する
// 引  数      ： なし
// 戻り値      ： 接続結果 [true:成功 false:失敗]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Connect_DataBase(void) {
    try {
        // AccessへのODBC接続
        DataModule1 = new TDataModule1(this);

        // 図番データベース(HD_DNO.mdb)
        G_ErrLog_Text = "図番データベース(HD_DNO.mdb)";

        // 2021.06.18 FireDACへの移行 MOD_S
        //DataModule1->Database_DNO->DatabaseName = ODBC_DSN_DNO;
        DataModule1->Database_DNO->ConnectionName = ODBC_DSN_DNO;
        // 2021.06.18 FireDACへの移行 MOD_E
        DataModule1->Database_DNO->Params->Clear();
        DataModule1->Database_DNO->LoginPrompt = false;
        DataModule1->Database_DNO->Connected = true;

        G_Log_Text = "図番データベース(HD_DNO.mdb)と接続しました。";
        Write_Log(G_Log_Text);

        // 検査要領書データベース(HD_INSPCTL.mdb)
        G_ErrLog_Text = "検査要領書データベース(HD_INSPCTL.mdb)";

        // 2021.06.18 FireDACへの移行 MOD_S
        //DataModule1->Database_MAT->DatabaseName = ODBC_DSN_INS;
        DataModule1->Database_MAT->ConnectionName = ODBC_DSN_INS;
        // 2021.06.18 FireDACへの移行 MOD_E
        DataModule1->Database_MAT->Params->Clear();
        DataModule1->Database_MAT->LoginPrompt = false;
        DataModule1->Database_MAT->Connected = true;

        G_Log_Text = "検査要領書データベース(HD_INSPCTL.mdb)と接続しました。";
        Write_Log(G_Log_Text);

        // 材質データベース(HD_MATERIAL.mdb)
        G_ErrLog_Text = "材質データベース(HD_MATERIAL.mdb)";

        // 2021.06.18 FireDACへの移行 MOD_S
        //DataModule1->Database_INS->DatabaseName = ODBC_DSN_MAT;
        DataModule1->Database_INS->ConnectionName = ODBC_DSN_MAT;
        // 2021.06.18 FireDACへの移行 MOD_E
        DataModule1->Database_INS->Params->Clear();
        DataModule1->Database_INS->LoginPrompt = false;
        DataModule1->Database_INS->Connected = true;

        G_Log_Text = "材質データベース(HD_MATERIAL.mdb)と接続しました。";
        Write_Log(G_Log_Text);
    }
    catch (EDatabaseError &Error) {
        Write_Error_Log(Error.Message);
        G_ErrLog_Text = G_ErrLog_Text + "への接続に失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 作番　取得
// 概  要      ： 起動時に取得するパラメータから、作番を取得する。
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::Get_Sakuban(void) {
    // *******************
    // ***  作番取得
    // *******************

    // 作番をHD_sakuban.txtから取得するよう変更　2003-02-05
    G_SAKUBAN = Get_Sakuban_File();
    if (G_SAKUBAN == "") {
        G_ErrLog_Text = "HD_sakuban.txtより作番が取得できませんでした！！";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        G_HEADS_ID = G_SAKUBAN + ".txt";
    }
    // ｺﾏﾝﾄﾞﾗｲﾝからﾌｧｲﾙIDを取得
    // G_HEADS_ID = ParamStr(1);     // HEADSﾃﾞｰﾀ ﾌｧｲﾙID
    // //G_HEADS_ID = "984010.txt";
    // //G_HEADS_ID = "083953.txt";

    // // ﾌｧｲﾙIDから作番を取得
    // int i_Pos;
    //
    // i_Pos = G_HEADS_ID.Pos(".");
    // if (i_Pos != 0){
    // G_SAKUBAN = G_HEADS_ID.SubString(1, i_Pos-1);
    // } else {
    // G_ErrLog_Text = "引数(" + G_HEADS_ID + ")により作番が取得できませんでした！！";
    // Write_Error_Log(G_ErrLog_Text);
    // return false;
    // }

    // ﾛｸﾞ作成
    G_Log_Text = "ファイルＩＤ『" + G_HEADS_ID + "』から、作番(" + G_SAKUBAN + ")を取得しました";
    Write_Log(G_Log_Text);

    // *************************
    // ***  作番種類　取得
    // *************************
    if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
        P_Sakuban_Mode = "S";
    }
    else {
        P_Sakuban_Mode = "M";
    }

    // **************************************************************
    // 02.08.07追加 HEADS入力ﾌｫﾙﾀﾞ･ﾌｧｲﾙ形式変更 csv→txt
    // 見積ﾃﾞｰﾀ "C:\HEADS_ROOT\HEADS_DATA\ESTIMATE\D00001\D00001.txt"
    // 発注ﾃﾞｰﾀ "C:\HEADS_ROOT\HEADS_DATA\ORDER\000001\000001.txt"
    // ***************************************************************
    if (P_Sakuban_Mode == "S") {
        // マルチサーバ対応 2009/1/13 MOD START
        // G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
        // + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
        // + IncludeTrailingBackslash(G_SAKUBAN);
        G_Input_Txt_Dir = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
            IncludeTrailingBackslash(G_DAT_ORDER_Dir) + IncludeTrailingBackslash
            (G_SAKUBAN);
        // マルチサーバ対応 2009/1/13 MOD END
    }
    else if (P_Sakuban_Mode == "M") {
        // マルチサーバ対応 2009/1/13 MOD START
        // G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
        // + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
        // + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
        // + IncludeTrailingBackslash(G_SAKUBAN);
        G_Input_Txt_Dir = IncludeTrailingBackslash(G_HEADS_DATA_Dir) +
            IncludeTrailingBackslash(G_DAT_ESTIM_Dir) + IncludeTrailingBackslash
            (G_SAKUBAN);
        // マルチサーバ対応 2009/1/13 MOD END
    }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 作成ファイル出力先フォルダ　チェック
// 概  要      ： 調査対象のﾌｫﾙﾀﾞを検索し、ﾌｫﾙﾀﾞが存在しなければ作成する。
// 引  数      ： AnsiString Check_Pass  // HEADS出力ﾌｫﾙﾀﾞ + 作番
// 戻り値      ： 検査結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TfrmHDP020::OutPut_Folder_Check(AnsiString Check_Pass) {
    if (!DirectoryExists(Check_Pass)) {
        if (!ForceDirectories(Check_Pass)) {
            // フォルダ作成　失敗
            G_ErrLog_Text = "フォルダ『 " + Check_Pass + "』が作成できませんでした！！";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
        else {
            // フォルダ作成　成功
            G_Log_Text = "フォルダ『 " + Check_Pass + "』を新規作成しました。"; ;
            Write_Log(G_Log_Text);
        }
    }

    return true;
}

// -----------------------------------------------------------------------------
// 日本語関数名： Excel起動処理
// 概要        ： Excelの起動を行う
// パラメタ    ： bool  showflg   起動表示フラグ
// 戻り値      ： 起動結果[0:正常起動,1:ｴﾗｰ]
// 機能説明    ：
// 備考        ：
// -----------------------------------------------------------------------------
short __fastcall TfrmHDP020::CallExcel(bool showflg) {
    try {
        if (exApplication.Type() != varDispatch) // Excel未起動？
        {
            // Excelｵﾌﾞｼﾞｪｸﾄを作成
            exApplication = Variant::CreateObject("Excel.Application");

            // Excelｵﾌﾞｼﾞｪｸﾄを非表示とする
            exApplication.Exec(PropertySet("Visible") << "false");
        }

        // ExcelｵﾌﾞｼﾞｪｸﾄのWorkBooksｺﾚｸｼｮﾝを取得
        exWorkbooks = exWorkCell.Exec(PropertyGet("Workbooks"));

        // Excelｵﾌﾞｼﾞｪｸﾄを表示/非表示とする
        exApplication.Exec(PropertySet("Visible") << "false");

        // 警告ﾒｯｾｰｼﾞを表示しないにする
        exApplication.Exec(PropertySet("DisplayAlerts") << "false");

        return 0; // 戻り値[0:正常起動]
    }
    catch (...) {
        // ｴﾗｰﾛｸﾞ追加
        G_ErrLog_Text = "Excelを起動できませんでした！！";
        Write_Error_Log(G_ErrLog_Text);

        QuitExcel();
        return (-1); // 戻り値[-1:起動異常]
    }
}

// -----------------------------------------------------------------------------
// 日本語関数名： Excel終了処理
// 概要        ： Excelの終了を行う
// パラメタ    ：
// 戻り値      ：
// 機能説明    ：
// 備考        ：
// -----------------------------------------------------------------------------
void __fastcall TfrmHDP020::QuitExcel(void) {
    // Variantの開放
    exWorkCell.Clear();
    exWorkbooks.Clear();
    exWorkbook.Clear();
    exWorksheets.Clear();
    exWorksheet.Clear();

    // Excel終了処理
    exApplication.Exec(Function("Quit"));

    // Variantの開放
    exApplication.Clear();

}

// ---------------- <<< End of File >>> ---------------- //
