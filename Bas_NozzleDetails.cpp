// ----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：ﾉｽﾞﾙ部品展開処理  モジュール
// 作 成 者：N.Uchida
// 作 成 日：2002.06.24
// 更 新 日：2002.10.16
// 2007.10.03 ﾐﾗｰ仕様対応
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop

#include <io.h>
#include <Filectrl.hpp>

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_NozzleDetails.h"

#include "HeadsData.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ﾌﾟﾚｰﾄ部品展開処理 Class
// ---------------------------------------------------------------------------
class TNozzleDetails {
private:
    FILE *fp;

    AnsiString P_Sikibetu; // 識別子
    AnsiString P_Katasiki; // 部品型式
    AnsiString P_Zaisitu; // 材質名称
    AnsiString P_EZaisitu; // 材質名称
    AnsiString P_ZaisituCD; // 材質コード
    AnsiString P_Zuban; // 図番
    AnsiString P_Zuban1; // 図番1
    AnsiString P_Zuban2; // 図番2
    AnsiString P_Zuban3; // 図番3
    AnsiString P_Zuban4; // 図番4
    AnsiString P_SZuban; // 作図用図番
    AnsiString P_SZuban1; // 作図用図番1
    AnsiString P_SZuban2; // 作図用図番2
    AnsiString P_SZuban3; // 作図用図番3
    AnsiString P_SZuban4; // 作図用図番4
    int P_Yobisu; // 予備数
    AnsiString P_Siyou1; // 仕様１
    AnsiString P_Siyou2; // 仕様２
    AnsiString P_Siyou3; // 仕様３
    AnsiString P_Hosoku; // 端管用補足
    AnsiString P_BV; // 端管用補足
    int P_SUS; // SUS使用フラグ

    // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
    AnsiString P_Zuban5; // 図番5
    AnsiString P_SZuban5; // 作図用図番5
    // ***********************************

    AnsiString P_Yobihin; // 予備品名

    // ↓2002/10/24
    AnsiString P_Model; // 型式
    // ↑2002/10/24

    int P_WriteFlg; // ﾉｽﾞﾙﾃﾞｰﾀ出力ﾌﾗｸﾞ(0:出力,1:未出力)

    // ↓2007.10.4 ミラー仕様対応
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // ↑2007.10.4 ミラー仕様対応

    bool __fastcall Nozzle_Data_Sakusei(void); // ﾉｽﾞﾙ本体ﾃﾞｰﾀ 作成
    void __fastcall Tankan_Data_Sakusei(void); // 端管ﾃﾞｰﾀ作成
    // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
    // bool __fastcall Get_Nozzle_Data(AnsiString Syurui,AnsiString Tugite);     // 各ﾉｽﾞﾙﾃﾞｰﾀ取得
    bool __fastcall Get_Nozzle_Data(AnsiString Syurui, AnsiString Tugite,
        AnsiString Kokei, AnsiString Ondo); // 各ﾉｽﾞﾙﾃﾞｰﾀ取得
    // ***********************************
    bool __fastcall Get_Tankan_Data(AnsiString Syurui); // 各端管ﾃﾞｰﾀ取得
    bool __fastcall Get_LA_Nozzle_Data(AnsiString Syurui, AnsiString Tugite);
    // 各液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ取得
    bool __fastcall Get_LA_Tugite_Data(AnsiString Syurui, AnsiString Tugite,
        AnsiString NzlTgl); // 各液抜きエア抜き継手ﾃﾞｰﾀ取得

    void __fastcall Get_Siyou_Data(AnsiString Syurui, int Kubun,
        AnsiString nMatName); // 仕様ﾃﾞｰﾀ取得

    bool __fastcall Write_Nozzle_Data(void);
    AnsiString __fastcall Search_NOZZLE_CODE(AnsiString Code);
    // ﾉｽﾞﾙ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ検索

    // ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ 検索
    bool __fastcall Search_HD_FRM_NZ(AnsiString Key, AnsiString Type, int Code);
    bool __fastcall TNozzleDetails::Search_HD_FRNG_DNO(int Model, int Typ,
        AnsiString Key, int flgAnsi);
    // AnsiString __fastcall TNozzleDetails::Search_HD_FRNG_MST(AnsiString Key, int FSize, AnsiString SEDiv, AnsiString PG, AnsiString TMP, AnsiString Drn, AnsiString Air);

    // 2019.04.11 リング部品追加_S
    bool __fastcall Get_Ring_Data(AnsiString Dno, AnsiString Code,
        AnsiString Syurui);  // 各ﾘﾝｸﾞ部品ﾃﾞｰﾀ取得
    // 2019.04.11 リング部品追加_E

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // ﾉｽﾞﾙ部品展開処理 実行
    AnsiString __fastcall ChgAirDrnSize(double dSize);
    // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
    AnsiString __fastcall ChgNozzleSize(double dSize);
    // **************************
};

// ---------------------------------------------------------------------------
// 日本語関数名： ﾉｽﾞﾙ部品展開処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall NozzleDetails(HeadsData* pcHeadsData) {
    bool ret;

    TNozzleDetails nozzleDetails; // ﾉｽﾞﾙﾃﾞｰﾀ 作成Class自動生成,関数終了後破棄

    ret = nozzleDetails.Excute(pcHeadsData); // ﾉｽﾞﾙﾃﾞｰﾀ 作成実行

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾉｽﾞﾙ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADSﾃｷｽﾄﾃﾞｰﾀのﾎﾟｲﾝﾀをｾｯﾄ
    m_pcHeadsData = pcHeadsData;

    // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString型配列に取得
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********      『ﾉｽﾞﾙ部品展開処理』開始      **********");
    Write_Log("********************************************************");
    Write_Error_Log("********************************************************");
    Write_Error_Log("**********      『ﾉｽﾞﾙ部品展開処理』開始      **********");
    Write_Error_Log("********************************************************");

    // ***********************************
    // ***  ﾉｽﾞﾙﾃﾞｰﾀ作成
    // ***********************************
    Nozzle_Data_Sakusei();

    // ********************
    // ***  端管ﾃﾞｰﾀ作成
    // ********************
    Write_Log("********************************************");
    Write_Log("**********      『端管』開始      **********");
    Write_Log("********************************************");
    Write_Error_Log("********************************************");
    Write_Error_Log("**********      『端管』開始      **********");
    Write_Error_Log("********************************************");

    Tankan_Data_Sakusei();

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********      『ﾉｽﾞﾙ部品展開処理』終了      **********");
    Write_Log("********************************************************");
    Write_Error_Log("********************************************************");
    Write_Error_Log("**********      『ﾉｽﾞﾙ部品展開処理』終了      **********");
    Write_Error_Log("********************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾉｽﾞﾙﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Nozzle_Data_Sakusei(void) {
    AnsiString s_Nzl_Syurui; // ﾉｽﾞﾙ種類
    AnsiString s_Nzl_Tugite; // ﾉｽﾞﾙ継手
    // 2004.12.21 変数追加
    AnsiString s_Text;
    // *******************
    // 2005.02.08 変数追加
    AnsiString s_Nzl_Kokei; // ﾉｽﾞﾙ口径
    AnsiString s_Nzl_Ondo; // ﾉｽﾞﾙ温度計
    // *******************

    // **************************************************************************
    // ***                                                                    ***
    // ***                                                                    ***
    // ***                        ノズルデータ　作成                          ***
    // ***                                                                    ***
    // ***                                                                    ***
    // **************************************************************************

    // ***********************************
    // ***  S1ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    S1ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[894].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S1ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("S1",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[892].TrimRight(); // S1ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[898].TrimRight(); // S1ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("S1", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S1ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S2ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    S2ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[908].TrimRight(); // S2ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[909].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S2ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("S2",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[907].TrimRight(); // S2ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[913].TrimRight(); // S2ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("S2", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S2ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S3ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    S3ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[923].TrimRight(); // S3ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[924].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S3ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("S3",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[922].TrimRight(); // S3ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[928].TrimRight(); // S3ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("S3", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S3ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S4ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    S4ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[938].TrimRight(); // S4ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[939].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S4ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("S4",s_Nzl_Tugite))
        s_Nzl_Kokei = P_HEADS_DATA[937].TrimRight(); // S4ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[943].TrimRight(); // S4ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("S4", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S4ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E1ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E1ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[953].TrimRight(); // E1ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[954].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E1ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("E1",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[952].TrimRight(); // E1ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[958].TrimRight(); // E1ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("E1", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            // 2020.03.05 YX-80Eﾉｽﾞﾙ変更_S
            //// 2004.12.21 YX-80Eﾉｽﾞﾙ追加
            //if (s_Nzl_Syurui == "1") {
            //    s_Text = P_HEADS_DATA[34].TrimRight();
            //    if (s_Text.SubString(1, 2) == "YX") {
            //        if (P_HEADS_DATA[170].ToIntDef(0) < 150) {
            //            P_SZuban = "Ｅ３４４９３６＿１";
            //        }
            //        else {
            //            P_SZuban = "Ｅ３４４９３６＿２";
            //        }
            //    }
            //}
            //// *************************
            P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
            if (P_Model == "YX-80") {
                if (P_HEADS_DATA[170].ToIntDef(0) < 150) {
                    P_SZuban = "E344936_1";
                }
                else {
                    P_SZuban = "E344936_2";
                }
            }
            if (P_Model == "YX-83") {
                if (P_HEADS_DATA[1290].TrimRight() == "形鋼") {
                    P_SZuban = "E385512_1";
                }
                else {
                    P_SZuban = "E385512_2";
                }
            }
            // 2020.03.05 YX-80Eﾉｽﾞﾙ変更_E
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E1ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E2ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E2ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[968].TrimRight(); // E2ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[969].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E2ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("E2",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[967].TrimRight(); // E2ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[973].TrimRight(); // E2ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("E2", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E2ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E3ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E3ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[983].TrimRight(); // E3ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[984].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E3ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2015.03.20 OGI ノズル種類条件"8"の追加
    //else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
    //    (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
    //    || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || (s_Nzl_Syurui == "8")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("E3",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[982].TrimRight(); // E3ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[988].TrimRight(); // E3ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("E3", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E3ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E4ﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E4ﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[998].TrimRight(); // E4ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[999].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E4ﾉｽﾞﾙﾃﾞｰﾀ  ﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2004.03.16 ﾉｽﾞﾙ種類7(閉止ﾌﾗﾝｼﾞ止め)追加
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if((s_Nzl_Syurui == "1")||(s_Nzl_Syurui == "2")||(s_Nzl_Syurui == "5")||(s_Nzl_Syurui == "6")||
    // (s_Nzl_Syurui == "7")||(( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) != "4" ))) {
    // 2015.03.20 OGI ノズル種類条件"8"の追加
    //else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
    //  (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
    //  || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
    else if ((s_Nzl_Syurui == "1") || (s_Nzl_Syurui == "2") ||
        (s_Nzl_Syurui == "5") || (s_Nzl_Syurui == "6") || (s_Nzl_Syurui == "7")
        || (s_Nzl_Syurui == "8")
        || ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) != "4"))) {
        // ***********************
        // ***************************************
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        // if (Get_Nozzle_Data("E4",s_Nzl_Tugite)){
        s_Nzl_Kokei = P_HEADS_DATA[997].TrimRight(); // E4ﾉｽﾞﾙ口径
        s_Nzl_Ondo = P_HEADS_DATA[1003].TrimRight(); // E4ﾉｽﾞﾙ温度計
        if (Get_Nozzle_Data("E4", s_Nzl_Tugite, s_Nzl_Kokei, s_Nzl_Ondo)) {
            // ***********************************
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "-----    E4ﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // **************************************************************************
    // ***                                                                    ***
    // ***                                                                    ***
    // ***                 液抜きエア抜きノズルデータ　作成                   ***
    // ***                                                                    ***
    // ***                                                                    ***
    // **************************************************************************

    // 2008.03.21 ｴｱ抜ﾉｽﾞﾙ追加
    // ***************************************
    // ***  S1エア抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***************************************
    G_Log_Text = "------    S1液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[894].TrimRight();
    P_BV = P_HEADS_DATA[1216].TrimRight() + P_HEADS_DATA[1217].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S1ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S1", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S1液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***************************************
    // ***  S2エア抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***************************************
    G_Log_Text = "------    S2液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    -------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[908].TrimRight(); // S2ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[909].TrimRight();
    P_BV = P_HEADS_DATA[1220].TrimRight() + P_HEADS_DATA[1221].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S2ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S2", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S2液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);
    // ***********************

    // ***************************************
    // ***  S3液抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***************************************
    G_Log_Text = "------    S3液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[923].TrimRight(); // S3ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[924].TrimRight();
    P_BV = P_HEADS_DATA[1224].TrimRight() + P_HEADS_DATA[1225].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S3ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S3", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S3液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***************************************
    // ***  S4液抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***************************************
    G_Log_Text = "------    S4液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[938].TrimRight(); // S4ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[939].TrimRight();
    P_BV = P_HEADS_DATA[1228].TrimRight() + P_HEADS_DATA[1229].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "S4ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("S4", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S4液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // 2008.03.21 ｴｱ抜ﾉｽﾞﾙ追加
    // ***************************************
    // ***  E1エア抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***************************************
    G_Log_Text = "------    E1液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[953].TrimRight(); // E1ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[954].TrimRight();
    P_BV = P_HEADS_DATA[1232].TrimRight() + P_HEADS_DATA[1233].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E1ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E1", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    G_Log_Text = "------    E1液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***************************************
    // ***  E2エア抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***************************************
    G_Log_Text = "------    E2液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[968].TrimRight(); // E2ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[969].TrimRight();
    P_BV = P_HEADS_DATA[1236].TrimRight() + P_HEADS_DATA[1237].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E2ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E2", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E2液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);
    // ***********************

    // ***********************************
    // ***  E3液抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E3液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[983].TrimRight(); // E3ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[984].TrimRight();
    P_BV = P_HEADS_DATA[1240].TrimRight() + P_HEADS_DATA[1241].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E3ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E3", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E3液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E4液抜きﾉｽﾞﾙﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E4液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[998].TrimRight(); // E4ﾉｽﾞﾙ継手
    P_ZaisituCD = P_HEADS_DATA[999].TrimRight();
    P_BV = P_HEADS_DATA[1244].TrimRight() + P_HEADS_DATA[1245].TrimRight();

    if (s_Nzl_Syurui == "0") {
        G_Log_Text = "E4ﾉｽﾞﾙﾃﾞｰﾀ  液抜きエア抜きﾉｽﾞﾙ種類が｢0｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // else if( ( s_Nzl_Syurui == "3" ) && (s_Nzl_Tugite.SubString(2,1) =="4" ) )
    else if ((s_Nzl_Syurui == "3") && (s_Nzl_Tugite.SubString(3, 1) == "4"))
        // ***********************
    {
        if (Get_LA_Nozzle_Data("E4", s_Nzl_Tugite)) {
            Write_Nozzle_Data();
        }
    }
    else {
        G_Log_Text = "ﾉｽﾞﾙ種類が｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E4液抜きエア抜きﾉｽﾞﾙﾃﾞｰﾀ  作成終了    ------*";
    Write_Log(G_Log_Text);

    // **************************************************************************
    // ***                                                                    ***
    // ***                                                                    ***
    // ***              S1～E4液抜きエア抜き継手データ　作成                  ***
    // ***                                                                    ***
    // ***                                                                    ***
    // **************************************************************************

    // //***********************************
    // //***  S1ｴｱ抜き継手ﾃﾞｰﾀ　作成
    // //***********************************
    // G_Log_Text = "------    S1ｴｱ抜き継手ﾃﾞｰﾀ  作成開始    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight();   // S1ﾉｽﾞﾙ種類
    // s_Nzl_Tugite = P_HEADS_DATA[905].TrimRight();   // S1ｴｱ抜き継手
    // P_ZaisituCD  = P_HEADS_DATA[894].TrimRight();
    //
    // if(( P_HEADS_DATA[36].Pos("M") > 0 ) && (P_HEADS_DATA[1136].ToIntDef(0) != 0) )  {
    // P_ZaisituCD = Chg_Zaisitu_Code( P_ZaisituCD, 6);
    // }
    //
    // P_BV         = P_HEADS_DATA[1216].TrimRight() + P_HEADS_DATA[1217].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("S1",s_Nzl_Tugite, P_HEADS_DATA[893].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "S1ｴｱ抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "S1ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    S1ｴｱ抜き継手ﾃﾞｰﾀ  作成終了    ------";
    // Write_Log(G_Log_Text);
    //
    // //***********************************
    // //***  S2ｴｱ抜き継手ﾃﾞｰﾀ　作成
    // //***********************************
    // G_Log_Text = "------    S2ｴｱ抜き継手ﾃﾞｰﾀ  作成開始    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight();   // S2ﾉｽﾞﾙ種類
    // s_Nzl_Tugite = P_HEADS_DATA[920].TrimRight();   // S2ｴｱ抜き継手
    // P_ZaisituCD  = P_HEADS_DATA[909].TrimRight();
    //
    // if(( P_HEADS_DATA[36].Pos("M") > 0 ) && (P_HEADS_DATA[1137].ToIntDef(0) != 0) )  {
    // P_ZaisituCD = Chg_Zaisitu_Code( P_ZaisituCD, 6);
    // }
    //
    // P_BV         = P_HEADS_DATA[1220].TrimRight() + P_HEADS_DATA[1221].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("S2",s_Nzl_Tugite, P_HEADS_DATA[908].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "S2ｴｱ抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "S2ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    S2ｴｱ抜き継手ﾃﾞｰﾀ  作成終了    ------";
    // Write_Log(G_Log_Text);

    // ***********************************
    // ***  S3液抜き継手ﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    S3液抜き継手ﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[935].TrimRight(); // S3液抜き継手
    P_ZaisituCD = P_HEADS_DATA[924].TrimRight();

    if ((P_HEADS_DATA[36].Pos("M") > 0) && (P_HEADS_DATA[1138].ToIntDef(0)
        != 0)) {
        P_ZaisituCD = Chg_Zaisitu_Code(P_ZaisituCD, 6);
    }

    P_BV = P_HEADS_DATA[1224].TrimRight() + P_HEADS_DATA[1225].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("S3", s_Nzl_Tugite,
                P_HEADS_DATA[923].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "S3液抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "S3ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S3液抜き継手ﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  S4液抜き継手ﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    S4液抜き継手ﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[950].TrimRight(); // S4液抜き継手
    P_ZaisituCD = P_HEADS_DATA[939].TrimRight();

    if ((P_HEADS_DATA[36].Pos("M") > 0) && (P_HEADS_DATA[1139].ToIntDef(0)
        != 0)) {
        P_ZaisituCD = Chg_Zaisitu_Code(P_ZaisituCD, 6);
    }

    P_BV = P_HEADS_DATA[1228].TrimRight() + P_HEADS_DATA[1229].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("S4", s_Nzl_Tugite,
                P_HEADS_DATA[938].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "S4液抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "S4ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    S4液抜き継手ﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // //***********************************
    // //***  E1ｴｱ抜き継手ﾃﾞｰﾀ　作成
    // //***********************************
    // G_Log_Text = "------    E1ｴｱ抜き継手ﾃﾞｰﾀ  作成開始    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight();   // E1ﾉｽﾞﾙ種類
    // s_Nzl_Tugite = P_HEADS_DATA[965].TrimRight();   // E1ｴｱ抜き継手
    // P_ZaisituCD  = P_HEADS_DATA[954].TrimRight();
    // P_BV         = P_HEADS_DATA[1232].TrimRight() + P_HEADS_DATA[1233].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("E1",s_Nzl_Tugite,P_HEADS_DATA[953].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "E1ｴｱ抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "E1ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    E1ｴｱ抜き継手ﾃﾞｰﾀ  作成終了    ------";
    // Write_Log(G_Log_Text);
    //
    // //***********************************
    // //***  E2ｴｱ抜き継手ﾃﾞｰﾀ　作成
    // //***********************************
    // G_Log_Text = "------    E2ｴｱ抜き継手ﾃﾞｰﾀ  作成開始    ------";
    // Write_Log(G_Log_Text);
    //
    // s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight();   // E2ﾉｽﾞﾙ種類
    // s_Nzl_Tugite = P_HEADS_DATA[980].TrimRight();   // E2ｴｱ抜き継手
    // P_ZaisituCD  = P_HEADS_DATA[969].TrimRight();
    // P_BV         = P_HEADS_DATA[1236].TrimRight() + P_HEADS_DATA[1237].TrimRight();
    //
    // if( s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2" ){
    // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
    // if (Get_LA_Tugite_Data("E2",s_Nzl_Tugite,P_HEADS_DATA[968].TrimRight())){
    // Write_Nozzle_Data();
    // }
    // } else {
    // G_Log_Text = "E2ｴｱ抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // } else {
    // G_Log_Text = "E2ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    //
    // G_Log_Text = "------    E2ｴｱ抜き継手ﾃﾞｰﾀ  作成終了    ------";
    // Write_Log(G_Log_Text);

    // ***********************************
    // ***  E3液抜き継手ﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E3液抜き継手ﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[995].TrimRight(); // E3液抜き継手
    P_ZaisituCD = P_HEADS_DATA[984].TrimRight();
    P_BV = P_HEADS_DATA[1240].TrimRight() + P_HEADS_DATA[1241].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("E3", s_Nzl_Tugite,
                P_HEADS_DATA[983].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "E3液抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "E3ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E3液抜き継手ﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  E4液抜き継手ﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "------    E4液抜き継手ﾃﾞｰﾀ  作成開始    ------";
    Write_Log(G_Log_Text);

    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ﾉｽﾞﾙ種類
    s_Nzl_Tugite = P_HEADS_DATA[1010].TrimRight(); // E4液抜き継手
    P_ZaisituCD = P_HEADS_DATA[999].TrimRight();
    P_BV = P_HEADS_DATA[1244].TrimRight() + P_HEADS_DATA[1245].TrimRight();

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2") {
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (s_Nzl_Tugite != "000" && s_Nzl_Tugite != "999"){
        if (s_Nzl_Tugite != "0000" && s_Nzl_Tugite != "9999") {
            // ***********************
            if (Get_LA_Tugite_Data("E4", s_Nzl_Tugite,
                P_HEADS_DATA[998].TrimRight())) {
                Write_Nozzle_Data();
            }
        }
        else {
            G_Log_Text = "E4液抜き継手が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
    }
    else {
        G_Log_Text = "E4ﾉｽﾞﾙ種類が、｢" + s_Nzl_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "------    E4液抜き継手ﾃﾞｰﾀ  作成終了    ------";
    Write_Log(G_Log_Text);

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 液抜きエア抜き継手データ　作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 02.08.05追加
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_LA_Tugite_Data(AnsiString Syurui,
    AnsiString Tugite, AnsiString NzlTgl) {
    AnsiString s_Text;

    AnsiString s_Code; //
    AnsiString s_BuhinKbn; // 部品区分(Nozzle.csv検索時取得)
    AnsiString s_BootsFlg; // ﾌﾞｰﾂﾌﾗｸﾞ
    AnsiString s_FCode; // ﾌﾗﾝｼﾞｺｰﾄﾞ
    // 2012.07.12 ﾉｽﾞﾙ仕様変更
    AnsiString s_SPEC1; // SPEC1
    AnsiString s_SPEC2; // SPEC2
    // ***********************

    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;

    // ************************************
    // ***  ﾉｽﾞﾙ構成部品ｺｰﾄﾞﾃｰﾌﾞ ﾙ検索
    // ************************************
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( NzlTgl.SubString(1,1) == "6" && NzlTgl.SubString(3,1) == "7" ) ||
    // ( NzlTgl.SubString(1,1) == "6" && NzlTgl.SubString(3,1) == "8" ) ) {    // サニタリ
    // if ( Tugite.SubString(1,1) == "7" && Tugite.SubString(3,1) == "9" ) {             // 液抜管ＰＴ
    if ((NzlTgl.SubString(1, 2) == "06" && NzlTgl.SubString(4, 1) == "7") ||
        (NzlTgl.SubString(1, 2) == "06" && NzlTgl.SubString(4, 1) == "8"))
    { // サニタリ
        if (Tugite.SubString(1, 2) == "07" && Tugite.SubString(4, 1) == "9")
        { // 液抜管ＰＴ
            // ***********************
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "46";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "47";
            }
        }
    }
    else { // サニタリ以外
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if ( Tugite.SubString(1,1) == "7" && Tugite.SubString(3,1) == "9" ) {             // 液抜曲管ＰＴ
        if (Tugite.SubString(1, 2) == "07" && Tugite.SubString(4, 1) == "9")
        { // 液抜曲管ＰＴ
            // ***********************
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "30";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "31";
            }
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // } else if( Tugite.SubString(3,1) == "1" || Tugite.SubString(3,1) == "2" ) {       // 液抜曲管ＷＦ
        }
        else if (Tugite.SubString(4, 1) == "1" || Tugite.SubString(4, 1) == "2")
        { // 液抜曲管ＷＦ
            // ***********************
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "44";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "45";
            }
        }
        else { // 液抜曲管フリーＦ
            if (Syurui == "S3" || Syurui == "S4") {
                s_Code = "42";
            }
            else if (Syurui == "E3" || Syurui == "E4") {
                s_Code = "43";
            }
        }
    }
    if (s_Code == "") {
        G_ErrLog_Text = Syurui + "ﾉｽﾞﾙﾃﾞｰﾀ作成時、条件に一致しませんでした。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    s_BuhinKbn = Search_NOZZLE_CODE(s_Code);

    // *********************************************
    // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ) 検索
    // *********************************************
    // ↓2002/10/24
    // s_Text = P_HEADS_DATA[34].TrimRight() + "-"
    // + P_HEADS_DATA[35].SubString(1,2) + "-"
    // + P_HEADS_DATA[1267].TrimRight();

    int i_Mojisu;
    i_Mojisu = P_HEADS_DATA[35].TrimRight().Length();
    s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu, 1);
    if (s_Text.ToIntDef(999) != 999) {
        s_Text = P_HEADS_DATA[34].TrimRight() + "-" + P_HEADS_DATA[35].TrimRight
            ().SubString(1, i_Mojisu - 2) + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Text = P_HEADS_DATA[34].TrimRight() + "-" + P_HEADS_DATA[35].TrimRight
            ().SubString(1, i_Mojisu - 1) + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    P_Model = s_Text;
    // ↑2002/10/24
    s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀを検索ｷｰ ｢" + s_Text + "｣ ｢" +
        P_HEADS_DATA[36].TrimRight() + "｣ ｢" + s_BuhinKbn + "｣ ｢" +
        AnsiString(s_Code.ToIntDef(0)) + "｣ で検索します。";

    Write_Log(G_Log_Text);

    if (!Search_HD_FRM_NZ(s_Text, s_BuhinKbn, s_Code.ToIntDef(0))) {
        G_ErrLog_Text = Syurui + "ﾃﾞｰﾀ作成時　検索ｷｰ ｢" + s_Text + "｣ ｢" +
            P_HEADS_DATA[36].TrimRight() + "｣ ｢" + s_BuhinKbn + "｣ ｢" +
            AnsiString(s_Code.ToIntDef(0)) +
            "｣ は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しません。";

        Write_Log(G_ErrLog_Text);
        Write_Error_Log(G_ErrLog_Text);

        return false;
    }

    // *********************************
    // ***  材質 取得(02.10.26追加)
    // *********************************
    P_Zaisitu = Search_HD_MATERIAL(P_ZaisituCD);
    P_EZaisitu = Search_HD_E_MATERIAL(P_ZaisituCD);

    // 2016.11.07 材質ｺｰﾄﾞ変更
    //// 2003.12.25 ASME PLATE
    //// if ((P_SUS != 0 ) && (P_Zaisitu.SubString(1,2) != "TP" )) {  // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
    //if ((P_SUS != 0) && (P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
    //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
    //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
    //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
    //    (P_ZaisituCD != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11追加
    //    (P_ZaisituCD != "1076") &&
    //    // ***************************
    //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
    //    // ノズル材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
    //    // *********************
    if ( (P_SUS != 0) && (Type_MAT(P_ZaisituCD) != "Ti") ) {  // プレート材質がチタン系以外
    // ***********************
        // 図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban4;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban4;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban4;
        // ***********************
        else
            P_Zuban = P_Zuban3;

        // 作図用図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban4;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // *****************
                P_SZuban = P_SZuban4;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban4;
        // ***********************
        else
            P_SZuban = P_SZuban3;
    }
    else {
        // 図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban2;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban2;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban2;
        // ***********************
        else
            P_Zuban = P_Zuban1;

        // 作図用図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban2;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_SZuban = P_SZuban2;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban2;
        // ***********************
        else
            P_SZuban = P_SZuban1;
    }

    // **************************************
    // ***  予備品数 取得(02.08.05追加)
    // **************************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, P_ZaisituCD, m_pcHeadsData);

    // **********************************
    // ***  仕様１　取得
    // **********************************
    // ﾌﾞｰﾂﾌﾗｸﾞの取得
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if (Tugite.SubString(2,1) == "1" || Tugite.SubString(2,1) == "2" || Tugite.SubString(2,1) == "3"){
    if (Tugite.SubString(3, 1) == "1" || Tugite.SubString(3, 1) == "2" ||
        Tugite.SubString(3, 1) == "3") {
        // ***********************
        s_BootsFlg = "1";
    }
    else {
        s_BootsFlg = "0";
    }

    // ﾌﾗﾝｼﾞｺｰﾄﾞの取得
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // s_FCode = Tugite.SubString(1,1) + "0" + Tugite.SubString(3,1);
    if (Tugite.ToIntDef(0) >= 1000) {
        s_FCode = Tugite.SubString(1, 2) + "0" + Tugite.SubString(4, 1);
    }
    else {
        s_FCode = Tugite.SubString(2, 1) + "0" + Tugite.SubString(4, 1);
    }
    // ***********************

    // ﾌﾗﾝｼﾞ仕様変換表  検索
    G_Log_Text = "ﾌﾗﾝｼﾞ仕様変換表を、｢" + s_FCode + "｣と｢" + s_BootsFlg + "｣で検索。";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRNG_SPEC";
    s_SQL = s_SQL + "  WHERE FCODE = '" + s_FCode + "'";
    s_SQL = s_SQL + "    AND BOOTS = " + s_BootsFlg + "";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

//cccc ノズル仕様の取得か
    if (wkQuery->Eof) {
        P_Siyou1 = "";
    }
    else {
        // 2012.07.12 ﾉｽﾞﾙ仕様変更
        // P_Siyou1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight() + "-";
        P_Siyou1 = "";
        s_SPEC1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight(); // SPEC1
        s_SPEC2 = wkQuery->FieldByName("SPEC2")->AsString.TrimRight(); // SPEC2
        // ***********************

        if (Syurui == "S1") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[895].TrimRight(), 0.0));
        }
        else if (Syurui == "S2") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[910].TrimRight(), 0.0));
        }
        else if (Syurui == "S3") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[925].TrimRight(), 0.0));
        }
        else if (Syurui == "S4") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[940].TrimRight(), 0.0));
        }
        else if (Syurui == "E1") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[955].TrimRight(), 0.0));
        }
        else if (Syurui == "E2") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[970].TrimRight(), 0.0));
        }
        else if (Syurui == "E3") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[985].TrimRight(), 0.0));
        }
        else if (Syurui == "E4") {
            s_Text =
                ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[1000].TrimRight(), 0.0));
        }
        // if (Syurui == "S1"){
        // s_Text = atoi(P_HEADS_DATA[905].c_str());
        // } else if (Syurui == "S2"){
        // s_Text = atoi(P_HEADS_DATA[920].c_str());
        // } else if (Syurui == "S3"){
        // s_Text = atoi(P_HEADS_DATA[935].c_str());
        // } else if (Syurui == "S4"){
        // s_Text = atoi(P_HEADS_DATA[950].c_str());
        // } else if (Syurui == "E1"){
        // s_Text = atoi(P_HEADS_DATA[965].c_str());
        // } else if (Syurui == "E2"){
        // s_Text = atoi(P_HEADS_DATA[980].c_str());
        // } else if (Syurui == "E3"){
        // s_Text = atoi(P_HEADS_DATA[995].c_str());
        // } else if (Syurui == "E4"){
        // s_Text = atoi(P_HEADS_DATA[1010].c_str());
        // }

        // 2012.07.12 ﾉｽﾞﾙ仕様変更
        // P_Siyou1 = P_Siyou1 + s_Text;
        // P_Siyou1 = P_Siyou1 + " " + wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
        if (s_SPEC1.SubString(1, 2) == "GB") {
            P_Siyou1 = "GB DN" + s_Text + "-" + s_SPEC1.SubString(4, 4) + " " +
                s_SPEC2;
        }
        else if (s_SPEC1.SubString(1, 3) == "DIN") {
            P_Siyou1 = "DIN DN" + s_Text + "-" + s_SPEC1.SubString(5, 4) + " " +
                s_SPEC2;
        }
        else {
            P_Siyou1 = s_SPEC1 + "-" + s_Text + " " + s_SPEC2;
        }
        // ***********************

        G_Log_Text = "仕様１    ｢" + P_Siyou1 + "｣を取得。";
        Write_Log(G_Log_Text);
    }

    G_Log_Text = "ﾌﾗﾝｼﾞ仕様変換表を、検索終了。";
    Write_Log(G_Log_Text);

    // **********************
    // ***  仕様２　取得
    // **********************
    if (P_HEADS_DATA[36].Pos("B") != 0) { // Ｂシリーズ
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (ZenToHan(P_Zaisitu).SubString(1, 2) == "TP") { // チタンの場合は仕上５
        //    P_Siyou2 = "仕上５";
        //    // 2004.01.28 ASME
        //}
        //else if (ZenToHan(P_Zaisitu).Pos("B-265") > 0) { // チタンの場合は仕上５
        //    P_Siyou2 = "仕上５";
        //    // ***************
        //}
        //else { // チタン以外の場合は仕上１
        //    P_Siyou2 = "仕上６";
        //}
        if ( Type_MAT(P_ZaisituCD) == "Ti" ) {        // チタンの場合は仕上５
            P_Siyou2 = "仕上５";
        }
        else {                         // チタン以外の場合は仕上６
            P_Siyou2 = "仕上６";
        }
        // ***********************
    }
    else {
        if (Syurui == "S1") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[904].TrimRight());
        }
        else if (Syurui == "S2") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[919].TrimRight());
        }
        else if (Syurui == "S3") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[934].TrimRight());
        }
        else if (Syurui == "S4") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[949].TrimRight());
        }
        else if (Syurui == "E1") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[964].TrimRight());
        }
        else if (Syurui == "E2") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[979].TrimRight());
        }
        else if (Syurui == "E3") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[994].TrimRight());
        }
        else if (Syurui == "E4") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[1009].TrimRight());
        }
    }

    // **********************
    // ***  仕様３　取得
    // **********************
    if (Syurui == "S1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "S2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "S3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "S4") {
        P_Siyou3 = "N4";
    }
    else if (Syurui == "E1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "E2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "E3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "E4") {
        P_Siyou3 = "N4";
    }

    P_Katasiki = P_Katasiki + "  " + P_Siyou1;
    P_Siyou1 = P_BV;

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 液抜きエア抜きﾉｽﾞﾙ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 02.08.05追加
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_LA_Nozzle_Data(AnsiString Syurui,
    AnsiString Tugite) {
    AnsiString s_Text;

    AnsiString s_Code; //
    AnsiString s_BuhinKbn; // 部品区分(Nozzle.csv検索時取得)

    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // s_Text = Tugite.SubString(2,1);
    s_Text = Tugite.SubString(3, 1);
    // ***********************

    // ************************************
    // ***  ﾉｽﾞﾙ構成部品ｺｰﾄﾞﾃｰﾌﾞ ﾙ検索
    // ************************************

    // 2008.03.21 ｴｱ抜ﾉｽﾞﾙ 液抜ﾉｽﾞﾙﾍﾙｰﾙ追加
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( Tugite.SubString(1,1) == "7" && Tugite.SubString(3,1) == "9" ) {        // 液抜ノズル　ＰＴ
    if (Tugite.SubString(1, 2) == "07" && Tugite.SubString(4, 1) == "9")
    { // 液抜ノズル　ＰＴ
        // ***********************
        // if (Syurui == "S3" || Syurui == "S4"){
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") {
            s_Code = "27";
            // } else if (Syurui == "E3" || Syurui == "E4"){
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "11";
        }
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // } else if ( Tugite.SubString(3,1) == "1" || Tugite.SubString(3,1) == "2") {  // 液抜ノズル　ＷＦ
    }
    else if (Tugite.SubString(4, 1) == "1" || Tugite.SubString(4, 1) == "2")
    { // 液抜ノズル　ＷＦ
        // ***********************
        // if (Syurui == "S3" || Syurui == "S4"){
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") {
            s_Code = "40";
            // } else if (Syurui == "E3" || Syurui == "E4"){
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "41";
        }
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // } else if ( Tugite.SubString(3,1) == "6" || Tugite.SubString(3,1) == "8") {  // 液抜ノズル　ヘルール
    }
    else if (Tugite.SubString(1, 2) == "06" && Tugite.SubString(4, 1) == "8")
    { // 液抜ノズル　ヘルール
        // ***********************
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") {
            s_Code = "72";
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "73";
        }
    }
    else {
        // if (Syurui == "S3" || Syurui == "S4"){                                     // 液抜ノズル　フリーＦ
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4") { // 液抜ノズル　フリーＦ
            s_Code = "34";
            // } else if (Syurui == "E3" || Syurui == "E4"){
        }
        else if (Syurui == "E1" || Syurui == "E2" || Syurui == "E3" ||
            Syurui == "E4") {
            s_Code = "37";
        }
    }
    // ************************************
    if (s_Code == "") {
        G_ErrLog_Text = Syurui + "ﾉｽﾞﾙﾃﾞｰﾀ作成時、条件に一致しませんでした。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    s_BuhinKbn = Search_NOZZLE_CODE(s_Code);

    // *********************************************
    // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ) 検索
    // *********************************************
    s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀを検索ｷｰ ｢" + s_Text + "｣ ｢" +
        P_HEADS_DATA[36].TrimRight() + "｣ ｢" + s_BuhinKbn + "｣ ｢" +
        AnsiString(s_Code.ToIntDef(0)) + "｣ で検索します。";

    Write_Log(G_Log_Text);

    if (!Search_HD_FRM_NZ(s_Text, s_BuhinKbn, s_Code.ToIntDef(0))) {
        G_ErrLog_Text = Syurui + "ﾃﾞｰﾀ作成時　検索ｷｰ ｢" + s_Text + "｣ ｢" +
            P_HEADS_DATA[36].TrimRight() + "｣ ｢" + s_BuhinKbn + "｣ ｢" +
            AnsiString(s_Code.ToIntDef(0)) +
            "｣ は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しません。";

        Write_Error_Log(G_ErrLog_Text);

        return false;
    }
    // P_Katasiki = P_Katasiki + "  " + P_BV;

    // *********************************
    // ***  材質 取得(02.10.26追加)
    // *********************************
    P_Zaisitu = Search_HD_MATERIAL(P_ZaisituCD);
    P_EZaisitu = Search_HD_E_MATERIAL(P_ZaisituCD);

    // 2016.11.07 材質ｺｰﾄﾞ変更
    //// 2003.12.25 ASME PLATE
    //// if ((P_SUS != 0 ) && (P_Zaisitu.SubString(1,2) != "TP" )) {  // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
    //if ((P_SUS != 0) && (P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
    //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
    //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
    //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
    //    (P_ZaisituCD != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11追加
    //    (P_ZaisituCD != "1076") &&
    //    // ***************************
    //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
    //    // ノズル材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
    //    // *********************
    if ( (P_SUS != 0) && (Type_MAT(P_ZaisituCD) != "Ti") ) {  // プレート材質がチタン系以外
    // ***********************
        // 図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban4;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban4;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban4;
        // ***********************
        else
            P_Zuban = P_Zuban3;

        // 作図用図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban4;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_SZuban = P_SZuban4;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban4;
        // ***********************
        else
            P_SZuban = P_SZuban3;
    }
    else {
        // 図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_Zuban = P_Zuban2;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_Zuban = P_Zuban2;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_Zuban = P_Zuban2;
        // ***********************
        else
            P_Zuban = P_Zuban1;

        // 作図用図番
        // 2005.02.08 JPI追加
        // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
        // ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" ))
        if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05")
            || (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1,
            2) == "09") || (Tugite.SubString(1, 2) == "11") ||
            (Tugite.SubString(1, 2) == "12") || (Tugite.SubString(1, 2) == "13")
            || (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1,
            2) == "22") || (Tugite.SubString(1, 2) == "23"))
            // ***********************
            // ******************
                P_SZuban = P_SZuban2;
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
        else if ((Tugite.SubString(1, 2) == "06") && (Tugite.SubString(4,
            1) == "8"))
            // ***********************
                P_SZuban = P_SZuban2;
        // 2013.04.03 ﾉｽﾞﾙ規格追加
        else if ((Tugite.SubString(1, 2) == "14") || (Tugite.SubString(1,
            2) == "24") || (Tugite.SubString(1, 2) == "31") ||
            (Tugite.SubString(1, 2) == "32") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "44") || (Tugite.SubString(1, 2) == "48"))
            P_SZuban = P_SZuban2;
        // ***********************
        else
            P_SZuban = P_SZuban1;
    }

    // *********************************
    // ***  予備品数 取得(02.08.05追加)
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, P_ZaisituCD, m_pcHeadsData);

    Get_Siyou_Data(Syurui, 2, P_Zaisitu);

    P_Katasiki = P_Katasiki + "  " + P_Siyou1;
    P_Siyou1 = P_BV;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾉｽﾞﾙ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： Syurui:ﾉｽﾞﾙ種類(S1～E4)，Tugite:ﾉｽﾞﾙ継手(S1～E4)
//                Kokei:ﾉｽﾞﾙ口径(S1～E4), Ondo:温度計(S1～E4)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
// 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
// bool __fastcall TNozzleDetails::Get_Nozzle_Data(AnsiString Syurui,AnsiString Tugite)
// {
bool __fastcall TNozzleDetails::Get_Nozzle_Data(AnsiString Syurui,
    AnsiString Tugite, AnsiString Kokei, AnsiString Ondo) {
    // ***********************************

    AnsiString s_Text;

    AnsiString s_Code; //
    AnsiString s_BuhinKbn; // 部品区分(Nozzle.csv検索時取得)

    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // s_Text = Tugite.SubString(2,1);
    s_Text = Tugite.SubString(3, 1);
    // ***********************
    s_Code = "";

    // ************************************
    // ***  ﾉｽﾞﾙ構成部品ｺｰﾄﾞﾃｰﾌﾞ ﾙ検索
    // ************************************
    if (s_Text == "1") { // ラバーブーツ
        if (Syurui.SubString(1, 1) == "S") {
            s_Code = "02";
        }
        else {
            s_Code = "28";
        }
    }
    else if (s_Text == "2") { // チューブフランジ
        if (Syurui.SubString(1, 1) == "S") {
            s_Code = "03";
        }
        else {
            s_Code = "29";
        }
    }
    else if (s_Text == "3") { // メタルブーツ
        // 2004.04.16 RX-00ノズル用ボス追加
        // if (Syurui.SubString(1,1) == "S"){
        // s_Code = "05";
        // } else {
        // s_Code = "12";
        // }
        P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
        if (P_Model == "RX-00") {
            if (Syurui.SubString(1, 1) == "S") {
                s_Code = "50";
            }
            else {
                s_Code = "51";
            }
        }
        else {
            if (Syurui.SubString(1, 1) == "S") {
                s_Code = "05";
            }
            else {
                s_Code = "12";
            }
        }
        // ********************************
    }
    else {
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (Tugite.SubString(1,1) == "6"){     //サニタリ
        // if (Tugite.SubString(3,1) == "7"){               //ネジ
        // if (Syurui.SubString(1,1) == "S"){
        // s_Code = "32";
        // } else {
        // s_Code = "35";
        // }
        // } else if (Tugite.SubString(3,1) == "8"){        //ヘルール（クランプ）
        if (Tugite.SubString(1, 2) == "06") { // サニタリ
            if (Tugite.SubString(4, 1) == "7") { // ネジ
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "32";
                }
                else {
                    s_Code = "35";
                }
            }
            else if (Tugite.SubString(4, 1) == "8") { // ヘルール（クランプ）
                // ***********************
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "33";
                }
                else {
                    s_Code = "36";
                }
            }
            // 2008.03.21 RX-00 ﾈｼﾞ込みﾉｽﾞﾙ追加
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // } else if (Tugite.SubString(1,1) == "7"){     //ＰＴネジ込みノズル
        }
        else if (Tugite.SubString(1, 2) == "07") { // ＰＴネジ込みノズル
            // *******************************
            if (Syurui.SubString(1, 1) == "S") {
                s_Code = "70";
            }
            else {
                s_Code = "71";
            }
            // ********************************
        }
        else { // ノズル（フランジ付き）
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( (Tugite.SubString(3,1) == "1") || (Tugite.SubString(3,1) == "2") ){               //ＷＦ（溶接）
            if ((Tugite.SubString(4, 1) == "1") || (Tugite.SubString(4,
                1) == "2")) { // ＷＦ（溶接）
                // ***********************
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "38";
                }
                else {
                    s_Code = "39";
                }
            }
            else {
                if (Syurui.SubString(1, 1) == "S") {
                    s_Code = "07";
                }
                else {
                    s_Code = "10";
                }
            }
        }
    }
    if (s_Code == "") {
        G_ErrLog_Text = Syurui + "ﾉｽﾞﾙﾃﾞｰﾀ作成時、条件に一致しませんでした。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }

    // ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)検索用の部品区分の取得
    s_BuhinKbn = Search_NOZZLE_CODE(s_Code);

    // *********************************************
    // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ) 検索
    // *********************************************
    s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀを検索ｷｰ ｢" + s_Text + "｣ ｢" +
        P_HEADS_DATA[36].TrimRight() + "｣ ｢" + s_BuhinKbn + "｣ ｢" +
        AnsiString(s_Code.ToIntDef(0)) + "｣ で検索します。";

    Write_Log(G_Log_Text);

    if (!Search_HD_FRM_NZ(s_Text, s_BuhinKbn, s_Code.ToIntDef(0))) {
        G_ErrLog_Text = Syurui + "ﾃﾞｰﾀ作成時　検索ｷｰ ｢" + s_Text + "｣ ｢" +
            P_HEADS_DATA[36].TrimRight() + "｣ ｢" + s_BuhinKbn + "｣ ｢" +
            AnsiString(s_Code.ToIntDef(0)) +
            "｣ は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しません。";

        Write_Error_Log(G_ErrLog_Text);

        // return false;
    }

    // *********************************
    // ***  材質 取得(02.10.26追加)
    // *********************************
    // 2012.02.07 SUS316Lﾁｭｰﾌﾞﾌﾗﾝｼﾞ
    if (s_Code == "03" || s_Code == "29") {
        P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
        // 2012.08.23 LX-00以外はSUS316L
        // if ( P_Model == "UX-40" ||
        // P_Model == "LX-40" ||
        // P_Model == "LX-50" ||
        // P_Model == "SX-41" ||
        // P_Model == "SX-43" ||
        // P_Model == "SX-70" ||
        // P_Model == "SX-73" ||
        // P_Model == "RX-50" ||
        // P_Model == "RX-70" ) {
        // // 口径200A以上はSUSD316ﾁｭｰﾌﾞをSUSD316Lに変更
        // 2012.12.06 LX-00削除
        // if ( P_Model != "LX-00" ) {
        // // LX-00以外はSUSD316ﾁｭｰﾌﾞをSUSD316Lに変更
        //// *****************************
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_ZaisituCD == "1003") {
        //    P_ZaisituCD = "1004";
        //}
        if ((P_ZaisituCD == "1003")||(P_ZaisituCD == "1004")||(P_ZaisituCD == "3103")) {
            P_ZaisituCD = "3104";
        }
        if (P_ZaisituCD == "1014") {
            P_ZaisituCD = "3114";
        }
        // ***********************
        // }
        // ********************
    }
    // *****************************
    P_Zaisitu = Search_HD_MATERIAL(P_ZaisituCD);
    P_EZaisitu = Search_HD_E_MATERIAL(P_ZaisituCD);

    // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
    ////2003.12.25 ASME PLATE
    ////if ((P_SUS != 0 ) && (P_Zaisitu.SubString(1,2) != "TP" )) {  // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
    // if ((P_SUS != 0 ) && ( P_ZaisituCD != "1014" ) && ( P_ZaisituCD != "1015" ) &&
    // ( P_ZaisituCD != "1032" ) && ( P_ZaisituCD != "1033" ) &&
    // ( P_ZaisituCD != "1057" ) && ( P_ZaisituCD != "1058" ) &&
    // ( P_ZaisituCD != "1085" ) && ( P_ZaisituCD != "1095" )) {
    // // ノズル材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
    ////*********************
    // // 図番
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_Zuban = P_Zuban4;
    // else
    // P_Zuban = P_Zuban3;
    //
    // // 作図用図番
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_SZuban = P_SZuban4;
    // else
    // P_SZuban = P_SZuban3;
    // } else {
    // // 図番
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_Zuban = P_Zuban2;
    // // 02.10.16 ｺﾒﾝﾄ化
    // //else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
    // //   P_Zuban = P_Zuban2;
    // else
    // P_Zuban = P_Zuban1;
    //
    // // 作図用図番
    // if( ( Tugite.SubString(1,1) == "4" )||( Tugite.SubString(1,1) == "5" ) )
    // P_SZuban = P_SZuban2;
    // // 02.10.16 ｺﾒﾝﾄ化
    // //else if( ( Tugite.SubString(1,1) == "6" )&&( Tugite.SubString(3,1) == "8" ) )
    // //   P_SZuban = P_SZuban2;
    // else
    // P_SZuban = P_SZuban1;
    // }
    // 2013.04.03 ﾉｽﾞﾙ規格追加
    //// 2011.06.20 ﾉｽﾞﾙ規格変更
    ////ANSI,JPI
    ////if (( Tugite.SubString(1,1) == "4" ) || ( Tugite.SubString(1,1) == "5" ) ||
    ////    ( Tugite.SubString(1,1) == "8" ) || ( Tugite.SubString(1,1) == "9" )) {
    ////ANSI/JPI/DIN/GB
    // if (( Tugite.SubString(1,2) == "04" ) || ( Tugite.SubString(1,2) == "05" ) ||
    // ( Tugite.SubString(1,2) == "08" ) || ( Tugite.SubString(1,2) == "09" ) ||
    // ( Tugite.SubString(1,2) == "11" ) || ( Tugite.SubString(1,2) == "12" ) ||
    // ( Tugite.SubString(1,2) == "13" ) || ( Tugite.SubString(1,2) == "21" ) ||
    // ( Tugite.SubString(1,2) == "22" ) || ( Tugite.SubString(1,2) == "23" )) {
    if ((Tugite.SubString(1, 2) == "04") || (Tugite.SubString(1, 2) == "05") ||
        (Tugite.SubString(1, 2) == "08") || (Tugite.SubString(1, 2) == "09") ||
        (Tugite.SubString(1, 2) == "11") || (Tugite.SubString(1, 2) == "12") ||
        (Tugite.SubString(1, 2) == "13") || (Tugite.SubString(1, 2) == "14") ||
        (Tugite.SubString(1, 2) == "21") || (Tugite.SubString(1, 2) == "22") ||
        (Tugite.SubString(1, 2) == "23") || (Tugite.SubString(1, 2) == "24") ||
        (Tugite.SubString(1, 2) == "31") || (Tugite.SubString(1, 2) == "32") ||
        (Tugite.SubString(1, 2) == "33") || (Tugite.SubString(1, 2) == "34") ||
        (Tugite.SubString(1, 2) == "44") || (Tugite.SubString(1, 2) == "48")) {
        //// ***********************
        // ***********************
        if (P_SUS == 0) {
            P_Zuban = P_Zuban2; // 図番
            P_SZuban = P_SZuban2; // 作図用図番

            // 2008.08.21 WX-50ﾌﾞｰﾂ図番設定条件追加
            P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
            if (P_Model == "WX-53") {
                // WHC=0,REVE=0 Ｂ側溶接、Ａ側ガスケット
                if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban2; // ANSI溶接図番
                        P_SZuban = P_SZuban2; // ANSI溶接作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban4; // ANSIｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban4; // ANSIｶﾞｽｹｯﾄ作図用図番
                    }
                    // WHC=0,REVE=1 Ｂ側ガスケット、Ａ側溶接
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban4; // ANSIｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban4; // ANSIｶﾞｽｹｯﾄ作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban2; // ANSI溶接図番
                        P_SZuban = P_SZuban2; // ANSI溶接作図用図番
                    }
                    // WHC=1,REVE=0 Ｂ側ガスケット、Ａ側溶接
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban4; // ANSIｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban4; // ANSIｶﾞｽｹｯﾄ作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban2; // ANSI溶接図番
                        P_SZuban = P_SZuban2; // ANSI溶接作図用図番
                    }
                    // WHC=1,REVE=1 Ｂ側溶接、Ａ側ガスケット
                }
                else {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban2; // ANSI溶接図番
                        P_SZuban = P_SZuban2; // ANSI溶接作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban4; // ANSIｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban4; // ANSIｶﾞｽｹｯﾄ作図用図番
                    }
                }
            }
            // ************************************
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            else if ((P_Model == "LX-10") && ((Tugite.SubString(1,
                2) == "11") || (Tugite.SubString(1, 2) == "21"))) {
                P_Zuban = P_Zuban1; // GB PN1.0 DIN10図番
                P_SZuban = P_SZuban1; // GB PN1.0 DIN10作図用図番
            }
            // ************************
            // 2013.04.03 ﾉｽﾞﾙ規格追加
            else if ((P_Model == "LX-10") && (Tugite.SubString(1, 2) == "31")) {
                P_Zuban = P_Zuban1; // EN10図番
                P_SZuban = P_SZuban1; // EN10作図用図番
            }
            // ************************
            // 2019.11.06 CX-23追加_S
            else if ((P_Model == "CX-23") &&
                     ((Tugite.SubString(1, 2) == "11") ||
                      (Tugite.SubString(1, 2) == "12") ||
                      (Tugite.SubString(1, 2) == "21") ||
                      (Tugite.SubString(1, 2) == "22") ||
                      (Tugite.SubString(1, 2) == "31") ||
                      (Tugite.SubString(1, 2) == "32"))) {
                P_Zuban = P_Zuban1;   // GB PN10,16 DIN PN10,16 EN PN10,16 図番
                P_SZuban = P_SZuban1; // GB PN10,16 DIN PN10,16 EN PN10,16 作図用図番
            }
            // 2019.11.06 CX-23追加_E
            // 2022.04.27 CX-21追加_S
            else if ((P_Model == "CX-21") &&
                     ((Tugite.SubString(1, 2) == "11") ||
                      (Tugite.SubString(1, 2) == "12") ||
                      (Tugite.SubString(1, 2) == "21") ||
                      (Tugite.SubString(1, 2) == "22") ||
                      (Tugite.SubString(1, 2) == "31") ||
                      (Tugite.SubString(1, 2) == "32"))) {
                P_Zuban = P_Zuban1;   // GB PN10,16 DIN PN10,16 EN PN10,16 図番
                P_SZuban = P_SZuban1; // GB PN10,16 DIN PN10,16 EN PN10,16 作図用図番
            }
            // 2022.04.27 CX-21追加_E
        }
        else if ((P_SUS == 25) && (Kokei.ToDouble() == 1) &&
            (Ondo.ToIntDef(0) != 0)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
        }
        else if ((P_SUS == 50) && (Kokei.ToDouble() == 2)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
        }
        else if ((P_SUS == 100) && (Kokei.ToDouble() == 4)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
        }
        else if ((P_SUS == 150) && (Kokei.ToDouble() == 6)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
            //2020.03.05 YX-83修正_S
            if ((P_Model == "YX-83") && (Type_MAT(P_ZaisituCD) == "Ti")) {
                P_Zuban = P_Zuban4; // 図番
                P_SZuban = P_SZuban4; // 作図用図番
            }
            //2020.03.05 YX-83修正_E
        }
        else if ((P_SUS == 200) && (Kokei.ToDouble() == 8)) {
            //2020.03.05 YX-83修正_S
            //P_Zuban = P_Zuban6; // 図番
            //P_SZuban = P_SZuban6; // 作図用図番
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
            //2020.03.05 YX-83修正_E
        }
        else {
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if ((P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
            //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
            //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
            //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
            //    (P_ZaisituCD != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11追加
            //    (P_ZaisituCD != "1076") &&
            //    // ***************************
            //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
            //    // ノズル材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032]
            //    // TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // ノズル材質がチタン系以外
            // ***********************
                P_Zuban = P_Zuban4; // 図番
                P_SZuban = P_SZuban4; // 作図用図番
            }
            else {
                P_Zuban = P_Zuban2; // 図番
                P_SZuban = P_SZuban2; // 作図用図番
            }
        }
        // JIS他
    }
    else {
        if (P_SUS == 0) {
            P_Zuban = P_Zuban1; // 図番
            P_SZuban = P_SZuban1; // 作図用図番

            // 2008.08.21 WX-50ﾌﾞｰﾂ図番設定条件追加
            P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
            if (P_Model == "WX-53") {
                // WHC=0,REVE=0 Ｂ側溶接、Ａ側ガスケット
                if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban1; // JIS溶接図番
                        P_SZuban = P_SZuban1; // JIS溶接作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban3; // JISｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban3; // JISｶﾞｽｹｯﾄ作図用図番
                    }
                    // WHC=0,REVE=1 Ｂ側ガスケット、Ａ側溶接
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban3; // JISｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban3; // JISｶﾞｽｹｯﾄ作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban1; // JIS溶接図番
                        P_SZuban = P_SZuban1; // JIS溶接作図用図番
                    }
                    // WHC=1,REVE=0 Ｂ側ガスケット、Ａ側溶接
                }
                else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
                    (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban3; // JISｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban3; // JISｶﾞｽｹｯﾄ作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban1; // JIS溶接図番
                        P_SZuban = P_SZuban1; // JIS溶接作図用図番
                    }
                    // WHC=1,REVE=1 Ｂ側溶接、Ａ側ガスケット
                }
                else {
                    if ((Syurui.SubString(2, 1) == "1") || (Syurui.SubString(2,
                        1) == "3")) {
                        P_Zuban = P_Zuban1; // JIS溶接図番
                        P_SZuban = P_SZuban1; // JIS溶接作図用図番
                    }
                    else {
                        P_Zuban = P_Zuban3; // JISｶﾞｽｹｯﾄ図番
                        P_SZuban = P_SZuban3; // JISｶﾞｽｹｯﾄ作図用図番
                    }
                }
            }
            // ************************************
            // 2010.08.27 LX-10-NP,NHP JIS16K,20K追加
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // else if ( ( P_Model == "LX-10" ) &&
            // ( ( Tugite.SubString(1,1) == "2" ) || ( Tugite.SubString(1,1) == "3" ) ) ) {
            else if ((P_Model == "LX-10") && ((Tugite.SubString(1,
                2) == "02") || (Tugite.SubString(1, 2) == "03"))) {
                // ***********************
                P_Zuban = P_Zuban2; // 図番
                P_SZuban = P_SZuban2; // 作図用図番
            }
            // **************************************
            // 2013.05.22 RX-00ﾎﾞｽ変更
            else if ((s_Code == "50" || s_Code == "51") &&
                (P_ZaisituCD != "1047")) {
                P_Zuban = P_Zuban3; // 図番
                P_SZuban = P_SZuban3; // 作図用図番
            }
            // ************************
            // 2018.07.05 LX-30Z追加_S
            else if ((P_Model == "LX-30") && ( P_HEADS_DATA[36].Pos("Z") > 0) &&
                     ( Tugite.SubString(1, 2) == "02" )) {
                // ＬＸ－３０－Ｚ  ＪＩＳ１６Ｋ
                P_Zuban = P_Zuban3; // 図番
                P_SZuban = P_SZuban3; // 作図用図番
            }
            // 2018.06.19 LX-30Z追加_E
        }
        else if ((P_SUS == 25) && (Kokei.ToDouble() == 25) &&
            (Ondo.ToIntDef(0) != 0)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
        }
        else if ((P_SUS == 50) && (Kokei.ToDouble() == 50)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
        }
        else if ((P_SUS == 100) && (Kokei.ToDouble() == 100)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
        }
        else if ((P_SUS == 150) && (Kokei.ToDouble() == 150)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
            //2020.03.05 YX-83修正_S
            if ((P_Model == "YX-83") && (Type_MAT(P_ZaisituCD) == "Ti")) {
                P_Zuban = P_Zuban4; // 図番
                P_SZuban = P_SZuban4; // 作図用図番
            }
            //2020.03.05 YX-83修正_E
        }
        else if ((P_SUS == 200) && (Kokei.ToDouble() == 200)) {
            P_Zuban = P_Zuban5; // 図番
            P_SZuban = P_SZuban5; // 作図用図番
        }
        else {
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if ((P_ZaisituCD != "1014") && (P_ZaisituCD != "1015") &&
            //    (P_ZaisituCD != "1032") && (P_ZaisituCD != "1033") &&
            //    (P_ZaisituCD != "1057") && (P_ZaisituCD != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
            //    (P_ZaisituCD != "1075") && (P_ZaisituCD != "1089") &&
            //    (P_ZaisituCD != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11追加
            //    (P_ZaisituCD != "1076") &&
            //    // ***************************
            //    (P_ZaisituCD != "1085") && (P_ZaisituCD != "1095")) {
            //    // ノズル材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032]
            //    // TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // ノズル材質がチタン系以外
            // ***********************
                P_Zuban = P_Zuban3; // 図番
                P_SZuban = P_SZuban3; // 作図用図番
            }
            else {
                P_Zuban = P_Zuban1; // 図番
                P_SZuban = P_SZuban1; // 作図用図番
            }
        }
    }

    // 2009.02.16 RX-50ﾌﾞｰﾂの場合DIN,#150をJISとして扱う
    P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    if (P_Model == "RX-50") {
        // ANSI#300,JPI#300
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (( Tugite.SubString(1,1) == "5" ) || ( Tugite.SubString(1,1) == "9" )) {
        if ((Tugite.SubString(1, 2) == "05") || (Tugite.SubString(1,
        2) == "09")) {
            // ***********************
            if (P_SUS == 0) {
                P_Zuban = P_Zuban2; // 図番
                P_SZuban = P_SZuban2; // 作図用図番
            }
            // 2013.04.03 ﾉｽﾞﾙ規格追加
        }
        else if ((Tugite.SubString(1, 2) == "13") || (Tugite.SubString(1,
            2) == "14") || (Tugite.SubString(1, 2) == "23") ||
            (Tugite.SubString(1, 2) == "24") || (Tugite.SubString(1, 2) == "33")
            || (Tugite.SubString(1, 2) == "34") || (Tugite.SubString(1,
            2) == "41") || (Tugite.SubString(1, 2) == "44") ||
            (Tugite.SubString(1, 2) == "48")) {
            if (P_SUS == 0) {
                P_Zuban = P_Zuban2; // 図番
                P_SZuban = P_SZuban2; // 作図用図番
            }
            // ************************
            // JIS,DIN,ANSI#150,JPI#150他
        }
        else {
            if (P_SUS == 0) {
                P_Zuban = P_Zuban1; // 図番
                P_SZuban = P_SZuban1; // 作図用図番
            }
        }
    }
    // *************************************************

    // 2023.03.06 CX-00-Y Pﾉｽﾞﾙ追加_S
    if (((P_Model == "CX-01") || (P_Model == "CX-03") || (P_Model == "CX-01D") ||
         (P_Model == "CX-03D") || (P_Model == "CXW-01") || (P_Model == "CXW-03" )) &&
        ((s_Code == "07") || (s_Code == "38")) &&
        (((P_HEADS_DATA[37].ToIntDef(0)) == 1) ||
         ((P_HEADS_DATA[37].ToIntDef(0)) == 2) ||
         ((P_HEADS_DATA[37].ToIntDef(0)) == 3) ||
         ((P_HEADS_DATA[37].ToIntDef(0)) == 4))) {
        if (Syurui == "S1" || Syurui == "S3" || Syurui == "E1" || Syurui == "E4") {
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // ノズル材質がチタン系以外
                P_Zuban = P_Zuban3; // 図番
                P_SZuban = P_SZuban3; // 作図用図番
            }
            else {
                P_Zuban = P_Zuban1; // 図番
                P_SZuban = P_SZuban1; // 作図用図番
            }
        }
        else {
            if (Type_MAT(P_ZaisituCD) != "Ti") {  // ノズル材質がチタン系以外
                P_Zuban = P_Zuban4; // 図番
                P_SZuban = P_SZuban4; // 作図用図番
            }
            else {
                P_Zuban = P_Zuban2; // 図番
                P_SZuban = P_SZuban2; // 作図用図番
            }
        }
    }
    // 2023.03.06 CX-00-Y Pﾉｽﾞﾙ追加_E

    //2020.02.05 YX-83修正_S
    G_Log_Text = "図    番     ｢" + P_Zuban + "｣を取得。";
    //2020.02.05 YX-83修正_E

    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // ***********************************

    // *********************************
    // ***  予備品数 取得(02.08.05追加)
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, P_ZaisituCD, m_pcHeadsData);

    // *********************************
    // ***  仕様１・２・３　取得
    // *********************************
    Get_Siyou_Data(Syurui, 0, "");

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾉｽﾞﾙ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ　検索
// 概  要      ：
// 引  数      ： AnsiString Code  // 検索対象ｺｰﾄﾞ
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall TNozzleDetails::Search_NOZZLE_CODE(AnsiString Code) {
    int i;
    AnsiString s_Name;

    s_Name = "";

    P_Hosoku = "";
    for (i = 0; i < G_NOZZLECD_RecCnt; i++) {
        if (G_NOZZLECD[i][0] == Code) {
            s_Name = G_NOZZLECD[i][2]; // 部品区分

            P_Sikibetu = G_NOZZLECD[i][1]; // 識別子(CSVﾌｧｲﾙ作成用)
            P_Katasiki = G_NOZZLECD[i][3]; // 部品型式(CSVﾌｧｲﾙ作成用)

            P_Yobihin = G_NOZZLECD[i][4]; // 予備品
            break;
        }
    }

    return s_Name;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 仕様1,2,3 ﾃﾞｰﾀ取得処理
// 概  要      ：
// 引  数      ： AnsiString Syurui ：種類(S1～S4, E1～E4)
// ： int        Kubun  ：区分(0:ﾉｽﾞﾙ, 1:端管,2:液抜きｴｱ抜きﾉｽﾞﾙ)
// 戻り値      ： なし
// 備  考      ： なし
// ---------------------------------------------------------------------------
void __fastcall TNozzleDetails::Get_Siyou_Data(AnsiString Syurui, int Kubun,
    AnsiString nMatName) {
    AnsiString s_Text;
    AnsiString s_BootsFlg; // ﾌﾞｰﾂﾌﾗｸﾞ
    AnsiString s_FCode; // ﾌﾗﾝｼﾞｺｰﾄﾞ
    // 2012.07.12 ﾉｽﾞﾙ仕様変更
    AnsiString s_SPEC1; // SPEC1
    AnsiString s_SPEC2; // SPEC2
    // ***********************

    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;

    // **********************
    // ***  仕様１　取得
    // **********************
    if (Kubun == 1) {
        if (Syurui == "S1") {
            s_Text = P_HEADS_DATA[1186].TrimRight(); // S1端管継手英１
        }
        else if (Syurui == "S2") {
            s_Text = P_HEADS_DATA[1190].TrimRight(); // S2端管継手英１
        }
        else if (Syurui == "S3") {
            s_Text = P_HEADS_DATA[1194].TrimRight(); // S3端管継手英１
        }
        else if (Syurui == "S4") {
            s_Text = P_HEADS_DATA[1198].TrimRight(); // S4端管継手英１
        }
        else if (Syurui == "E1") {
            s_Text = P_HEADS_DATA[1202].TrimRight(); // E1端管継手英１
        }
        else if (Syurui == "E2") {
            s_Text = P_HEADS_DATA[1206].TrimRight(); // E2端管継手英１
        }
        else if (Syurui == "E3") {
            s_Text = P_HEADS_DATA[1210].TrimRight(); // E3端管継手英１
        }
        else if (Syurui == "E4") {
            s_Text = P_HEADS_DATA[1214].TrimRight(); // E4端管継手英１
        }

    }
    else {
        // ﾌﾞｰﾂﾌﾗｸﾞの取得
        if (Syurui == "S1") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[905].TrimRight(); // S1ｴｱ抜継手
            }
        }
        else if (Syurui == "S2") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[908].TrimRight(); // S2ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[920].TrimRight(); // S2ｴｱ抜継手
            }
        }
        else if (Syurui == "S3") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[923].TrimRight(); // S3ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[935].TrimRight(); // S3液抜継手
            }
        }
        else if (Syurui == "S4") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[938].TrimRight(); // S4ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[950].TrimRight(); // S4液抜継手
            }
        }
        else if (Syurui == "E1") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[953].TrimRight(); // E1ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[965].TrimRight(); // E1ｴｱ抜継手
            }
        }
        else if (Syurui == "E2") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[968].TrimRight(); // E2ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[980].TrimRight(); // E2ｴｱ抜継手
            }
        }
        else if (Syurui == "E3") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[983].TrimRight(); // E3ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[995].TrimRight(); // E3液抜継手
            }
        }
        else if (Syurui == "E4") {
            if (Kubun == 0) {
                s_Text = P_HEADS_DATA[998].TrimRight(); // E4ﾉｽﾞﾙ継手
            }
            else {
                s_Text = P_HEADS_DATA[1010].TrimRight(); // E4液抜継手
            }
        }

        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if (s_Text.SubString(2,1) == "1" || s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
        if (s_Text.SubString(3, 1) == "1" || s_Text.SubString(3, 1) == "2" ||
            s_Text.SubString(3, 1) == "3") {
            // ***********************
            s_BootsFlg = "1";
        }
        else {
            s_BootsFlg = "0";
        }

        // ﾌﾗﾝｼﾞｺｰﾄﾞの取得
        // 区分２(液抜きｴｱ抜きﾉｽﾞﾙ)作成時は継手を再取得
        if (Kubun == 2) {
            if (Syurui == "S1") {
                s_Text = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
            }
            else if (Syurui == "S2") {
                s_Text = P_HEADS_DATA[908].TrimRight(); // S2ﾉｽﾞﾙ継手
            }
            else if (Syurui == "S3") {
                s_Text = P_HEADS_DATA[923].TrimRight(); // S3ﾉｽﾞﾙ継手
            }
            else if (Syurui == "S4") {
                s_Text = P_HEADS_DATA[938].TrimRight(); // S4ﾉｽﾞﾙ継手
            }
            else if (Syurui == "E1") {
                s_Text = P_HEADS_DATA[953].TrimRight(); // E1ﾉｽﾞﾙ継手
            }
            else if (Syurui == "E2") {
                s_Text = P_HEADS_DATA[968].TrimRight(); // E2ﾉｽﾞﾙ継手
            }
            else if (Syurui == "E3") {
                s_Text = P_HEADS_DATA[983].TrimRight(); // E3ﾉｽﾞﾙ継手
            }
            else if (Syurui == "E4") {
                s_Text = P_HEADS_DATA[998].TrimRight(); // E4ﾉｽﾞﾙ継手
            }
        }
        // 2008.03.21 RX-00 ﾈｼﾞ込みﾉｽﾞﾙ追加
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // else if (s_Text.SubString(1,3) == "749"){     //ＰＴネジ込みノズル
        else if (s_Text.SubString(1, 4) == "0749") { // ＰＴネジ込みノズル
            // ***********************
            s_BootsFlg = "2";
        }
        // ********************************

        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // s_FCode = s_Text.SubString(1,1) + "0" + s_Text.SubString(3,1);
        if (s_Text.ToIntDef(0) >= 1000) {
            s_FCode = s_Text.SubString(1, 2) + "0" + s_Text.SubString(4, 1);
        }
        else {
            s_FCode = s_Text.SubString(2, 1) + "0" + s_Text.SubString(4, 1);
        }
        // ***********************

        // ﾌﾗﾝｼﾞ仕様変換表  検索
        G_Log_Text = "ﾌﾗﾝｼﾞ仕様変換表を、｢" + s_FCode + "｣と｢" + s_BootsFlg + "｣で検索。";
        Write_Log(G_Log_Text);

        s_SQL = "";
        s_SQL = s_SQL + "SELECT * FROM HD_FRNG_SPEC";
        s_SQL = s_SQL + "  WHERE FCODE = '" + s_FCode + "'";
        s_SQL = s_SQL + "    AND BOOTS = " + s_BootsFlg + "";

        wkQuery->Close();
        // 2021.06.18 FireDACへの移行 MOD_S
        //wkQuery->DatabaseName = ODBC_DSN_DNO;
        wkQuery->ConnectionName = ODBC_DSN_DNO;
        // 2021.06.18 FireDACへの移行 MOD_E
        wkQuery->SQL->Clear();
        wkQuery->SQL->Add(s_SQL);
        wkQuery->Open();
        wkQuery->First();

        if (wkQuery->Eof) {
            P_Siyou1 = "";
        }
        else {
            // 2012.07.12 ﾉｽﾞﾙ仕様変更
            // P_Siyou1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight() + "-";
            P_Siyou1 = "";
            s_SPEC1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight();
            // SPEC1
            s_SPEC2 = wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
            // SPEC2
            // ***********************

            if (Syurui == "S1") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[892].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[892].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[892].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[892].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "S2") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[907].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[907].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[907].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[907].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "S3") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[922].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[922].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[922].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[922].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "S4") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[937].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[937].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[937].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[937].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E1") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ************************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[952].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[952].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[952].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[952].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E2") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[967].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[967].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[967].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[967].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E3") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[982].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[982].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[982].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[982].TrimRight
                        (), 0.0));
                }
            }
            else if (Syurui == "E4") {
                if (Kubun == 0) {
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text.SubString(1,1) == "6" ) {         // サニタリ
                    if (s_Text.SubString(1, 2) == "06") { // サニタリ
                        // ***********************
                        s_Text =
                            ChgIDFSize(StrToDblDef(P_HEADS_DATA[997].TrimRight
                            (), 0.0));
                    }
                    else {
                        // 2008.06.26 ﾉｽﾞﾙ口径3/4追加
                        // s_Text = atoi(P_HEADS_DATA[997].c_str());
                        s_Text =
                            ChgNozzleSize(StrToDblDef
                            (P_HEADS_DATA[997].TrimRight(), 0.00));
                        // **************************
                    }
                }
                else {
                    s_Text =
                        ChgAirDrnSize(StrToDblDef(P_HEADS_DATA[997].TrimRight
                        (), 0.0));
                }
            }

            // 2012.07.12 ﾉｽﾞﾙ仕様変更
            // P_Siyou1 = P_Siyou1 + s_Text;
            // P_Siyou1 = P_Siyou1 + " " + wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
            //
            ////2008.03.06 RX-21 ﾈｼﾞ込みﾉｽﾞﾙ追加
            // if (s_BootsFlg == "2"){     //ＰＴネジ込みノズル
            // P_Siyou1 = wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
            // }
            ////********************************
            if (s_SPEC1.SubString(1, 2) == "GB") {
                P_Siyou1 = "GB DN" + s_Text + "-" + s_SPEC1.SubString(4, 4) +
                    " " + s_SPEC2;
            }
            else if (s_SPEC1.SubString(1, 3) == "DIN") {
                P_Siyou1 = "DIN DN" + s_Text + "-" + s_SPEC1.SubString(5, 4) +
                    " " + s_SPEC2;
            }
            else if (s_BootsFlg == "2") { // ＰＴネジ込みノズル
                P_Siyou1 = s_SPEC2;
            }
            else {
                P_Siyou1 = s_SPEC1 + "-" + s_Text + " " + s_SPEC2;
            }
            // ***********************

            G_Log_Text = "仕様１    ｢" + P_Siyou1 + "｣を取得。";
            Write_Log(G_Log_Text);

        }

        G_Log_Text = "ﾌﾗﾝｼﾞ仕様変換表を、検索終了。";
        Write_Log(G_Log_Text);
    }

    // **********************
    // ***  仕様２　取得
    // **********************
    switch (Kubun) {
    case 0:
        if (Syurui == "S1") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[904].TrimRight());
        }
        else if (Syurui == "S2") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[919].TrimRight());
        }
        else if (Syurui == "S3") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[934].TrimRight());
        }
        else if (Syurui == "S4") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[949].TrimRight());
        }
        else if (Syurui == "E1") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[964].TrimRight());
        }
        else if (Syurui == "E2") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[979].TrimRight());
        }
        else if (Syurui == "E3") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[994].TrimRight());
        }
        else if (Syurui == "E4") {
            P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[1009].TrimRight());
        }
        break;

    case 2:
        if (P_HEADS_DATA[36].Pos("B") != 0) { // Ｂシリーズ
            if (ZenToHan(nMatName).SubString(1, 2) == "TP") { // チタンの場合は仕上５
                P_Siyou2 = "仕上５";
                // 2004.01.28 ASME
            }
            else if (ZenToHan(nMatName).Pos("B-265") > 0) { // チタンの場合は仕上５
                P_Siyou2 = "仕上５";
                // ***************
            }
            else { // チタン以外の場合は仕上１
                P_Siyou2 = "仕上６";
            }
        }
        else {
            if (Syurui == "S1") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[904].TrimRight());
            }
            else if (Syurui == "S2") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[919].TrimRight());
            }
            else if (Syurui == "S3") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[934].TrimRight());
            }
            else if (Syurui == "S4") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[949].TrimRight());
            }
            else if (Syurui == "E1") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[964].TrimRight());
            }
            else if (Syurui == "E2") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[979].TrimRight());
            }
            else if (Syurui == "E3") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[994].TrimRight());
            }
            else if (Syurui == "E4") {
                P_Siyou2 = "仕上" + HanToZen(P_HEADS_DATA[1009].TrimRight());
            }
        }
        break;
    case 1:
        if ((Syurui == "S1") || (Syurui == "S3") || (Syurui == "E1") ||
            (Syurui == "E3")) {
            // 2016.08.26 材質ｺｰﾄﾞ変更
            //s_Text = Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
            //s_Text = ZenToHan(s_Text);
            //if (s_Text.SubString(1, 2) == "TP" || s_Text.SubString(1,
            //    4) == "STPG") { // チタンの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //    // 2004.01.28 ASME SGP
            //}
            //else if (s_Text.Pos("B-265") > 0) { // チタンの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("SGP") > 0) { // ＳＧＰの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //    // *******************
            //    // 2004.05.13 舶用端管材質追加
            //}
            //else if (s_Text.Pos("CAC703") > 0) { // ＣＡＣ７０３の場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("STKM13A") > 0) { // ＳＴＫＭ１３Ａの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //    // ***************************
            //}
            //else { // チタン以外の場合は仕上１
            //    P_Siyou2 = "仕上１";
            //}
            s_Text = P_HEADS_DATA[1060].SubString(1,4);
            s_Text = Type_MAT(s_Text);
            if ( s_Text == "Ti" ) {        // チタンの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "STPG" ) { // ＳＴＰＧの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "SGP" ) {  // ＳＧＰの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "CS" ) {   // ＣＳ(ＳＴＫＭ１３Ａ)の場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            //2021.03.10 NW2201追加_S
            else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            //2021.03.10 NW2201追加_E
            else {                         // その他の場合は仕上１
                 P_Siyou2 = "仕上１";
            }
            // ***********************
        }
        else if (((Syurui == "S4") || (Syurui == "E4")) &&
            (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ")) {
            // 2016.08.26 材質ｺｰﾄﾞ変更
            //s_Text = Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
            //s_Text = ZenToHan(s_Text);
            //if (s_Text.SubString(1, 2) == "TP" || s_Text.SubString(1,
            //    4) == "STPG") { // チタンの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("B-265") > 0) { // チタンの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("SGP") > 0) { // ＳＧＰの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("CAC703") > 0) { // ＣＡＣ７０３の場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("STKM13A") > 0) { // ＳＴＫＭ１３Ａの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else { // チタン以外の場合は仕上１
            //    P_Siyou2 = "仕上１";
            //}
            //// *********************
            s_Text = P_HEADS_DATA[1060].SubString(1,4);
            s_Text = Type_MAT(s_Text);
            if ( s_Text == "Ti" ) {        // チタンの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "STPG" ) { // ＳＴＰＧの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "SGP" ) {  // ＳＧＰの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "CS" ) {   // ＣＳ(ＳＴＫＭ１３Ａ)の場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            //2021.03.10 NW2201追加_S
            else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            //2021.03.10 NW2201追加_E
            else {                         // その他の場合は仕上１
                 P_Siyou2 = "仕上１";
            }
            // ***********************
        }
        else if ((Syurui == "S2") || (Syurui == "S4") || (Syurui == "E2") ||
            (Syurui == "E4")) {
            // 2016.08.26 材質ｺｰﾄﾞ変更
            //s_Text = Search_HD_MATERIAL(P_HEADS_DATA[1061].SubString(1, 4));
            //s_Text = ZenToHan(s_Text);
            //if (s_Text.SubString(1, 2) == "TP" || s_Text.SubString(1,
            //    4) == "STPG") { // チタンの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //    // 2004.01.28 ASME SGP
            //}
            //else if (s_Text.Pos("B-265") > 0) { // チタンの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("SGP") > 0) { // ＳＧＰの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //    // *******************
            //    // 2004.05.13 舶用端管材質追加
            //}
            //else if (s_Text.Pos("CAC703") > 0) { // ＣＡＣ７０３の場合は仕上５
            //    P_Siyou2 = "仕上５";
            //}
            //else if (s_Text.Pos("STKM13A") > 0) { // ＳＴＫＭ１３Ａの場合は仕上５
            //    P_Siyou2 = "仕上５";
            //    // ***************************
            //}
            //else { // チタン以外の場合は仕上１
            //    P_Siyou2 = "仕上１";
            //}
            s_Text = P_HEADS_DATA[1061].SubString(1,4);
            s_Text = Type_MAT(s_Text);
            if ( s_Text == "Ti" ) {        // チタンの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "STPG" ) { // ＳＴＰＧの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "SGP" ) {  // ＳＧＰの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            else if ( s_Text == "CS" ) {   // ＣＳ(ＳＴＫＭ１３Ａ)の場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            //2021.03.10 NW2201追加_S
            else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
                 P_Siyou2 = "仕上５";
            }
            //2021.03.10 NW2201追加_E
            else {                         // その他の場合は仕上１
                 P_Siyou2 = "仕上１";
            }
            // ***********************
        }
        break;
    }

    // **********************
    // ***  仕様３　取得
    // **********************
    if (Syurui == "S1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "S2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "S3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "S4") {
        P_Siyou3 = "N4";
    }
    else if (Syurui == "E1") {
        P_Siyou3 = "N1";
    }
    else if (Syurui == "E2") {
        P_Siyou3 = "N2";
    }
    else if (Syurui == "E3") {
        P_Siyou3 = "N3";
    }
    else if (Syurui == "E4") {
        P_Siyou3 = "N4";
    }

    delete wkQuery;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_FRM_NZﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ：
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Search_HD_FRM_NZ(AnsiString Key,
    AnsiString Type, int Code) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    int i_Length;
    AnsiString s_SQL;
    AnsiString s_Frame;

    s_Frame = P_HEADS_DATA[36].TrimRight(); // ﾌﾚｰﾑ
    i_Length = s_Frame.Length();
    s_Frame = s_Frame.SubString(i_Length, 1);
    if (s_Frame == "R") {
        s_Frame = P_HEADS_DATA[36].TrimRight();
        s_Frame = s_Frame.SetLength(i_Length - 1);
    }
    else {
        s_Frame = P_HEADS_DATA[36].TrimRight();
    }

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRM_NZ";
    s_SQL = s_SQL + "  WHERE PLATE_MODEL = '" + Key + "'";
    s_SQL = s_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'";
    s_SQL = s_SQL + "    AND PARTS_TYPE = '" + Type + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = " + Code + "";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        P_Zaisitu = "";
        P_Zuban1 = "";
        P_Zuban2 = "";
        P_Zuban3 = "";
        P_Zuban4 = "";
        P_SZuban1 = "";
        P_SZuban2 = "";
        P_SZuban3 = "";
        P_SZuban4 = "";
        // SUS使用フラグ
        P_SUS = 0;
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        P_Zuban5 = "";
        P_SZuban5 = "";
        // ***********************************

        delete wkQuery;
        return false;
    }
    else {
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight
            (); // 材質名称
        P_Zuban1 = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // 図番
        P_Zuban2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight(); // 図番
        P_Zuban3 = wkQuery->FieldByName("DNO3")->AsString.TrimRight(); // 図番
        P_Zuban4 = wkQuery->FieldByName("DNO4")->AsString.TrimRight(); // 図番
        P_SZuban1 = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        // 作図用図番
        P_SZuban2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_SZuban3 = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        // 作図用図番
        P_SZuban4 = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        // SUS使用フラグ
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        P_Zuban5 = wkQuery->FieldByName("DNO5")->AsString.TrimRight(); // 図番
        P_SZuban5 = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // 作図用図番
        // ***********************************

        G_Log_Text = "材質名称    ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図    番1    ｢" + P_Zuban1 + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図    番2    ｢" + P_Zuban2 + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図    番3    ｢" + P_Zuban3 + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図    番4    ｢" + P_Zuban4 + "｣を取得。";
        Write_Log(G_Log_Text);
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        G_Log_Text = "図    番5    ｢" + P_Zuban5 + "｣を取得。";
        Write_Log(G_Log_Text);
        // ***********************************
        G_Log_Text = "作図用図番1  ｢" + P_SZuban1 + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番2  ｢" + P_SZuban2 + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番3  ｢" + P_SZuban3 + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番4  ｢" + P_SZuban4 + "｣を取得。";
        Write_Log(G_Log_Text);
        // 2005.02.08 ﾉｽﾞﾙ口径、温度計有無追加
        G_Log_Text = "作図用図番5  ｢" + P_SZuban5 + "｣を取得。";
        Write_Log(G_Log_Text);
        // ***********************************
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 端管ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
void __fastcall TNozzleDetails::Tankan_Data_Sakusei(void) {
    int i;
    AnsiString TANKAN[8] = {"S1", "S2", "S3", "S4", "E1", "E2", "E3", "E4"};
    // 2016.08.26 材質ｺｰﾄﾞ変更
    AnsiString s_Text;
    // ***********************
    // 2019.04.11 リング部品追加_S
    AnsiString RINGDNO[8];
    int j;
    // 2019.04.11 リング部品追加_E

    for (i = 0; i < 8; i++) {
        // 端管ﾃﾞｰﾀ作成
        if (Get_Tankan_Data(TANKAN[i])) {
            // 2019.04.11 リング部品追加_S
            RINGDNO[i] = P_Zuban;
            // 2019.04.11 リング部品追加_E
            // 識別
            if (TANKAN[i] == "S1" || TANKAN[i] == "S2" || TANKAN[i] == "S3" ||
                TANKAN[i] == "S4") {
                P_Sikibetu = "FRNG-S";
            }
            else {
                P_Sikibetu = "FRNG-E";
            }

            // 材質
            if ((TANKAN[i].SubString(2, 1) == "1") || (TANKAN[i].SubString(2,
                1) == "3")) {
                P_Zaisitu =
                    Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                // 2003.12.22 SGP(SS400)
                // 2016.08.26 材質ｺｰﾄﾞ変更
                //if (P_HEADS_DATA[1060].SubString(1, 4) == "1060") {
                s_Text = P_HEADS_DATA[1060].SubString(1,4);
                s_Text = Type_MAT(s_Text);
                if ( s_Text == "SGP" ) { // ＳＧＰ
                // ***********************
                    P_Zaisitu = P_Zaisitu + "（ＳＳ４００）";
                    P_EZaisitu = P_EZaisitu + "（ＳＳ４００）";
                }
                // *********************
            }
            // 2008.07.24 CX-10D追加
            else if ((TANKAN[i].SubString(2, 1) == "4") &&
                (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ")) {
                P_Zaisitu =
                    Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
                // 2016.08.26 材質ｺｰﾄﾞ変更
                //if (P_HEADS_DATA[1060].SubString(1, 4) == "1060") {
                s_Text = P_HEADS_DATA[1060].SubString(1,4);
                s_Text = Type_MAT(s_Text);
                if ( s_Text == "SGP" ) { // ＳＧＰ
                // ***********************
                    P_Zaisitu = P_Zaisitu + "（ＳＳ４００）";
                    P_EZaisitu = P_EZaisitu + "（ＳＳ４００）";
                }
            }
            // *********************
            else {
                P_Zaisitu =
                    Search_HD_MATERIAL(P_HEADS_DATA[1061].SubString(1, 4));
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1061].SubString(1, 4));
                // 2003.12.22 SGP(SS400)
                 // 2016.08.26 材質ｺｰﾄﾞ変更
                //if (P_HEADS_DATA[1061].SubString(1, 4) == "1060") {
                s_Text = P_HEADS_DATA[1061].SubString(1,4);
                s_Text = Type_MAT(s_Text);
                if ( s_Text == "SGP" ) { // ＳＧＰ
                // ***********************
                    P_Zaisitu = P_Zaisitu + "（ＳＳ４００）";
                    P_EZaisitu = P_EZaisitu + "（ＳＳ４００）";
                }
                // *********************
            }

            if (P_Zaisitu == "") { // 端管材質に該当なしで、液抜き端管の場合
                if ((P_Katasiki.SubString(1, 6) == "端管Ｘ") ||
                    (P_Katasiki.SubString(1, 6) == "端管Ｙ")) {
                    // 2012.05.09 液抜き端管材質訂正
                    //// 液抜き端管の材質は、プレート材質から変換する
                    ////2004.02.05
                    ////P_Zaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[43].TrimRight(), 1));
                    ////P_EZaisitu = P_Zaisitu;
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[43].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[43].TrimRight(), 1));
                    ////**********

                    // 2013.02.14 材質取得変更
                    // if ( TANKAN[i] == "S1" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1248].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1248].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "S2" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1249].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1249].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "S3" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1250].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1250].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "S4" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1251].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1251].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E1" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1252].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1252].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E2" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1253].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1253].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E3" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1254].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1254].TrimRight(), 1));
                    // } else if ( TANKAN[i] == "E4" ) {
                    // P_Zaisitu =  Search_HD_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1255].TrimRight(), 1));
                    // P_EZaisitu =  Search_HD_E_MATERIAL(Chg_Zaisitu_Code( P_HEADS_DATA[1255].TrimRight(), 1));
                    // }
                    if (TANKAN[i] == "S1") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1248].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1248].TrimRight());
                    }
                    else if (TANKAN[i] == "S2") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1249].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1249].TrimRight());
                    }
                    else if (TANKAN[i] == "S3") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1250].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1250].TrimRight());
                    }
                    else if (TANKAN[i] == "S4") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1251].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1251].TrimRight());
                    }
                    else if (TANKAN[i] == "E1") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1252].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1252].TrimRight());
                    }
                    else if (TANKAN[i] == "E2") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1253].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1253].TrimRight());
                    }
                    else if (TANKAN[i] == "E3") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1254].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1254].TrimRight());
                    }
                    else if (TANKAN[i] == "E4") {
                        P_Zaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1255].TrimRight());
                        P_EZaisitu =
                            Search_HD_MATERIAL(P_HEADS_DATA[1255].TrimRight());
                    }
                    // *************************
                    // *****************************
                }
            }

            // 予備数
            P_Yobisu = 0;

            // if ( P_Zaisitu != "" ) {
            // 仕様1,2
            Get_Siyou_Data(TANKAN[i], 1, "");

            Write_Nozzle_Data();
            // }
        }
        else {
            // 2019.04.11 リング部品追加_S
            RINGDNO[i] = "";
            // 2019.04.11 リング部品追加_E
            G_Log_Text = TANKAN[i] + "端管ﾃﾞｰﾀ  端管種類が｢0｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
    }

    // 2019.04.11 リング部品追加_S
    for (j = 0; j < 8; j++) {
        if ( RINGDNO[j] == "") {
            // 端管図番無し
            G_Log_Text = TANKAN[j]
                + "ﾘﾝｸﾞ部品ﾃﾞｰﾀ  端管図無しの為作成しません。";
            Write_Log(G_Log_Text);
        } else {
            // 端管図番有り
            // ﾘﾝｸﾞ部品ﾃﾞｰﾀ作成
            if ( Get_Ring_Data(RINGDNO[j],"1",TANKAN[j])) {
                // 六角両ニップル有り
                Write_Nozzle_Data();
            } else {
                // 六角両ニップル無し
                G_Log_Text = TANKAN[j]
                    + "ﾘﾝｸﾞ部品ﾃﾞｰﾀ  六角ﾆｯﾌﾟﾙ無しの為作成しません。";
                Write_Log(G_Log_Text);
            }
            if ( Get_Ring_Data(RINGDNO[j],"2",TANKAN[j])) {
                // Ｔ－コネクタ有り
                Write_Nozzle_Data();
            } else {
                // Ｔ－コネクタ無し
                G_Log_Text = TANKAN[j]
                    + "ﾘﾝｸﾞ部品ﾃﾞｰﾀ  T-ｺﾈｸﾀ無しの為作成しません。";
                Write_Log(G_Log_Text);
            }
            // 2020.03.18 ストリートエルボ追加_S
            if (G_KS_Syurui == "ＲＸ" && G_KS_Model == "３０") {
                //RX-30型
                if ((P_HEADS_DATA[38].ToIntDef(0) >= 281) ||
                    (P_HEADS_DATA[163].ToIntDef(0) >= 281) ||
                    (P_HEADS_DATA[170].ToIntDef(0) >= 281)) {
                    if ( Get_Ring_Data(RINGDNO[j],"3",TANKAN[j])) {
                        // ｽﾄﾘｰﾄｴﾙﾎﾞ有り
                        Write_Nozzle_Data();
                    } else {
                        // ｽﾄﾘｰﾄｴﾙﾎﾞ無し
                        G_Log_Text = TANKAN[j]
                            + "ﾘﾝｸﾞ部品ﾃﾞｰﾀ  ｽﾄﾘｰﾄｴﾙﾎﾞ無しの為作成しません。";
                        Write_Log(G_Log_Text);
                    }
                }
            }
            // 2020.03.18 ストリートエルボ追加_E
        }

    }
    // 2019.04.11 リング部品追加_E

}

// ---------------------------------------------------------------------------
// 日本語関数名： 端管ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： AnsiString Syurui：端管種類(S1～E4)
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_Tankan_Data(AnsiString Syurui) {
    AnsiString s_Tkn_Syurui; // 端管種類

    /* 6列目を各材質から継手に変更
     int HEADS[8][6] = {{ 1136,  901,  898,  895,  892,  894 },
     { 1137,  916,  913,  910,  907,  909 },
     { 1138,  931,  928,  925,  922,  924 },
     { 1139,  946,  943,  940,  937,  939 },
     { 1140,  961,  958,  955,  952,  954 },
     { 1141,  976,  973,  970,  967,  969 },
     { 1142,  991,  988,  985,  982,  984 },
     { 1143, 1006, 1003, 1000,  997,  999 }};

     int HEADS[8][6] = {{ 1136,  901,  898,  895,  892,  893 },
     { 1137,  916,  913,  910,  907,  908 },
     { 1138,  931,  928,  925,  922,  923 },
     { 1139,  946,  943,  940,  937,  938 },
     { 1140,  961,  958,  955,  952,  953 },
     { 1141,  976,  973,  970,  967,  968 },
     { 1142,  991,  988,  985,  982,  983 },
     { 1143, 1006, 1003, 1000,  997,  998 }};
     */

    // 6列目を各材質からエア抜き、液抜き継手に変更  2002.10.20 S.Y
    int HEADS[8][11] = {
        {1136, 901, 898, 895, 892, 905, 1144, 1216, 1217, 893, 1184},
        {1137, 916, 913, 910, 907, 920, 1145, 1220, 1221, 908, 1188},
        {1138, 931, 928, 925, 922, 935, 1146, 1224, 1225, 923, 1192},
        {1139, 946, 943, 940, 937, 950, 1147, 1228, 1229, 938, 1196},
        {1140, 961, 958, 955, 952, 965, 1148, 1232, 1233, 953, 1200},
        {1141, 976, 973, 970, 967, 980, 1149, 1236, 1237, 968, 1204},
        {1142, 991, 988, 985, 982, 995, 1150, 1240, 1241, 983, 1208},
        {1143, 1006, 1003, 1000, 997, 1010, 1151, 1244, 1245, 998, 1212}};

    int s_Prs_Syurui; // 圧力計種類
    int s_Tmp_Syurui; // 温度計種類
    int iWFflg; // WF端管？
    double s_Ank_Kokei; // ｴｱ抜き口径
    double s_Nzl_Kokei; // ﾉｽﾞﾙ口径
    AnsiString s_Nzl_Tugite; // ﾉｽﾞﾙ継手
    AnsiString s_Nzl_Syurui; // ﾉｽﾞﾙ種類
    AnsiString s_NzCd; // ﾉｽﾞﾙ継手ｺｰﾄﾞ
    AnsiString s_PGtyp; // 圧力計種類（検索用）
    AnsiString s_TPtyp; // 温度計種類（検索用）
    AnsiString s_DRtyp; // 液抜き種類（検索用）
    AnsiString s_ARtyp; // エア抜き種類（検索用）
    AnsiString s_KeyMat; // 材質キー（リング端管のＳＴＰＧ）
    AnsiString s_MatCD; // 端管材質コード
    AnsiString s_ULdiv; // 上下区分

    // 2009.10.28 舶用端菅長追加
    AnsiString s_Text;
    AnsiString s_Mat; // 端菅材質
    AnsiString s_BootsFlg; // ﾌﾞｰﾂﾌﾗｸﾞ
    AnsiString s_FCode; // ﾌﾗﾝｼﾞｺｰﾄﾞ
    AnsiString s_ENzl_Syurui; // Eﾉｽﾞﾙ種類
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;
    // *************************
    // 2023.02.22 リング端管条件変更_S
    AnsiString s_Model;
    // 2023.02.22 リング端管条件変更_E

    int i_HeadsNo;

    s_Nzl_Syurui = "";

    P_Sikibetu = "";
    P_Katasiki = "";
    P_Hosoku = "";
    P_Zaisitu = "";
    P_Zuban = "";
    P_SZuban = "";
    P_Yobisu = 0;

    P_Siyou1 = "";
    P_Siyou2 = "";
    P_Siyou3 = "";

    if (Syurui == "S1") {
        i_HeadsNo = 0;
        s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "S2") {
        i_HeadsNo = 1;
        s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "S3") {
        i_HeadsNo = 2;
        s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "L";
    }
    else if (Syurui == "S4") {
        i_HeadsNo = 3;
        s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        // 2008.07.24 CX-10D追加
        // s_MatCD = P_HEADS_DATA[1061].SubString(1,4);
        if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
            s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else {
            s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        }
        // *********************
        s_ULdiv = "L";
    }
    else if (Syurui == "E1") {
        i_HeadsNo = 4;
        s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "E2") {
        i_HeadsNo = 5;
        s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        s_ULdiv = "U";
    }
    else if (Syurui == "E3") {
        i_HeadsNo = 6;
        s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        s_ULdiv = "L";
    }
    else if (Syurui == "E4") {
        i_HeadsNo = 7;
        s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight();
        P_Siyou1 = P_HEADS_DATA[HEADS[i_HeadsNo][10]].TrimRight();
        // 2008.07.24 CX-10D追加
        // s_MatCD = P_HEADS_DATA[1061].SubString(1,4);
        if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
            s_MatCD = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else {
            s_MatCD = P_HEADS_DATA[1061].SubString(1, 4);
        }
        // *********************
        s_ULdiv = "L";
    }

    // 2021.10.04 リング端管の場合 P_Siyou1をリセット_S
    if ( P_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight().ToIntDef(0) == 3 ) {
        P_Siyou1 = "";
    }
    // 2021.10.04 リング端管の場合 P_Siyou1をリセット_E

    // 2009.10.28 舶用端菅長追加
    if ((P_HEADS_DATA[36].Pos("M") != 0) && (Syurui == "S1" || Syurui == "S2" ||
        Syurui == "S3" || Syurui == "S4")) {
        s_Text = P_HEADS_DATA[HEADS[i_HeadsNo][9]].TrimRight();

        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // s_FCode = s_Text.SubString(1,1) + "0" + s_Text.SubString(3,1);
        if (s_Text.ToIntDef(0) >= 1000) {
            s_FCode = s_Text.SubString(1, 2) + "0" + s_Text.SubString(4, 1);
        }
        else {
            s_FCode = s_Text.SubString(2, 1) + "0" + s_Text.SubString(4, 1);
        }
        // ***********************

        s_BootsFlg = "1";

        // ﾌﾗﾝｼﾞ仕様変換表  検索
        G_Log_Text = "ﾌﾗﾝｼﾞ仕様変換表を、｢" + s_FCode + "｣と｢" + s_BootsFlg + "｣で検索。";
        Write_Log(G_Log_Text);

        s_SQL = "";
        s_SQL = s_SQL + "SELECT * FROM HD_FRNG_SPEC";
        s_SQL = s_SQL + "  WHERE FCODE = '" + s_FCode + "'";
        s_SQL = s_SQL + "    AND BOOTS = " + s_BootsFlg + "";

        wkQuery->Close();
        // 2021.06.18 FireDACへの移行 MOD_S
        //wkQuery->DatabaseName = ODBC_DSN_DNO;
        wkQuery->ConnectionName = ODBC_DSN_DNO;
        // 2021.06.18 FireDACへの移行 MOD_E
        wkQuery->SQL->Clear();
        wkQuery->SQL->Add(s_SQL);
        wkQuery->Open();
        wkQuery->First();

        if (wkQuery->Eof) {
        }
        else {

            // 2016.08.26 材質ｺｰﾄﾞ変更
            //s_Mat = Search_HD_MATERIAL(P_HEADS_DATA[1060].SubString(1, 4));
            //s_Mat = ZenToHan(s_Mat);
            //if ((s_Mat.TrimRight() == "CAC703") ||
            //    (s_Mat.TrimRight() == "STKM13A")) {
            //    // ALBC,CS
            // 2018.03.19 片側端管に対応_S
            //s_Mat = P_HEADS_DATA[1060].SubString(1,4);
            s_Mat = s_MatCD;
            // 2018.03.19 片側端管に対応_E
            s_Mat = Type_MAT(s_Mat);
            if ( s_Mat == "ALBC" || s_Mat == "CS" ) {
                // ＡＬＢＣ、ＣＳ(ＳＴＫＭ１３Ａ)
            // ***********************
                // 2014.12.12 ﾘﾝｸﾞ端菅注記変更
                // 圧力計種類
                s_Prs_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
                // Ｐ座有無 判別
                if (s_Prs_Syurui == 0) {
                    P_Siyou1 = "";
                } else {
                    P_Siyou1 = "Ｐ座付";
                }
                // ***************************
                if (Syurui == "S3") {
                    s_ENzl_Syurui = P_HEADS_DATA[981].TrimRight();
                }
                else if (Syurui == "S4") {
                    s_ENzl_Syurui = P_HEADS_DATA[996].TrimRight();
                }
                else {
                    s_ENzl_Syurui = "";
                }

                //2016.05.18 リング端管追加
                // ﾉｽﾞﾙ継手ｺｰﾄﾞ
                s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][9]].SubString(1, 2);
                if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" && s_NzCd == "41") {
                }
                // 2023.02.22 リング端管条件変更_S
                else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０Ｓ" && s_NzCd == "41") {
                }
                //else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "００") {
                //}
                 else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１" && P_HEADS_DATA[36].Pos("Y") == 0) {
                }
                else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ" && P_HEADS_DATA[36].Pos("Y") == 0) {
                }
                // 2023.02.22 リング端管条件変更_E
                else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０") {
                }
                else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
                }
                // 2023.02.22 リング端管条件変更_S
                //else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "００") {
                //}
                 else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "０１" && P_HEADS_DATA[36].Pos("Y") == 0) {
                }
                // 2023.02.22 リング端管条件変更_E
                //if (s_ENzl_Syurui == "5") {
                else if (s_ENzl_Syurui == "5") {
                //*************************　　
                    P_Siyou1 = P_Siyou1 + "Ｔ＝７５、";
                }
                else {
                    P_Siyou1 = P_Siyou1 + "Ｔ＝８０、";
                }

            }

            P_Siyou1 = P_Siyou1 + wkQuery->FieldByName("SPEC1")
                ->AsString.TrimRight();
            P_Siyou1 = P_Siyou1 + "-";

            s_Text =
                ChgNozzleSize(StrToDblDef
                (P_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(), 0.00));

            P_Siyou1 = P_Siyou1 + s_Text + "Ａ用";

        }
    }
    // *************************

    // 圧力計種類
    s_Prs_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
    // 温度計種類
    s_Tmp_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][2]].TrimRight().ToIntDef(0);
    // ｴｱ抜き口径
    s_Ank_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][3]].TrimRight(), 0);
    // ﾉｽﾞﾙ口径
    s_Nzl_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(), 0);
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // エア抜き液抜き継手(先頭一桁)
    // s_Nzl_Tugite  = P_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1,1);
    s_Nzl_Tugite = P_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1, 2);
    // ﾉｽﾞﾙ継手ｺｰﾄﾞ
    // s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][9]].SubString(1,1);
    s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][9]].SubString(1, 2);
    // ***********************

    // ***********************************
    // ***  端管ﾃﾞｰﾀ　作成
    // ***********************************
    s_Tkn_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight(); // 端管種類
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "1" ) ||
    // ( P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "2" ) ) {
    if ((P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4, 1) == "1") ||
        (P_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4, 1) == "2")) {
        // ***********************
        iWFflg = 1;
    }
    else {
        iWFflg = 0;
    }
    // (1)
    if (s_Tkn_Syurui == "0") {
        return false;
    }
    P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    // 2008.07.24 CX-10端管
    if (P_Model == "CX-10" || P_Model == "CX-10D") {
        // 材質判別
        // 2016.08.26 材質ｺｰﾄﾞ変更
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "ＣＡＣ７０３") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "ＳＴＰＧ") {
        //    s_KeyMat = "STPG";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // ＡＬＢＣ
        } else if ( s_KeyMat == "STPG") {
            // ＳＴＰＧ
        } else {
            // その他
            s_KeyMat = "-";
        }
        // ***********************
        s_ULdiv = "-"; // 位置固定
        s_PGtyp = "-"; // コックなし
        s_TPtyp = "-"; // 温度計圧力計座なし
        s_DRtyp = "-"; // 液抜なし
        s_ARtyp = "-"; // エア抜なし

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "該当端管なし";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // 2016.04.22 CX-00端管
    // 2023.02.22 リング端管条件変更_S
    //// 2021.01.06 CX(W)-03追加(CX(W)-00→CX(W)-01)_S
    ////else if (P_Model == "CX-00" || P_Model == "CXW-00") {
    //else if (P_Model == "CX-01" || P_Model == "CXW-01") {
    //// 2021.01.06 CX(W)-03追加(CX(W)-00→CX(W)-01)_E
    else if ((P_Model == "CX-01" || P_Model == "CXW-01") && P_HEADS_DATA[36].Pos("Y") == 0) {
    // 2023.02.22 リング端管条件変更_E
        // 材質判別
        // 2016.08.26 材質ｺｰﾄﾞ変更
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "ＣＡＣ７０３") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "ＳＴＰＧ") {
        //    s_KeyMat = "STPG";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // ＡＬＢＣ
        } else if ( s_KeyMat == "STPG") {
            // ＳＴＰＧ
        } else {
            // その他
            s_KeyMat = "-";
        }
        // ***********************
        s_ULdiv = "-"; // 位置固定
        s_PGtyp = "-"; // コックなし
        s_TPtyp = "-"; // 温度計圧力計座なし
        s_DRtyp = "-"; // 液抜なし
        s_ARtyp = "-"; // エア抜なし

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "該当端管なし";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // ********************
    // 2022.04.21 CX-01D追加_S
    // 2023.02.22 リング端管条件変更_S
    //else if (P_Model == "CX-01D" || P_Model == "CXW-01D") {
    else if ((P_Model == "CX-01D" || P_Model == "CXW-01D") && P_HEADS_DATA[36].Pos("Y") == 0) {
    // 2023.02.22 リング端管条件変更_E
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // ＡＬＢＣ
        } else if ( s_KeyMat == "STPG") {
            // ＳＴＰＧ
        } else {
            // その他
            s_KeyMat = "-";
        }
        s_ULdiv = "-"; // 位置固定
        s_PGtyp = "-"; // コックなし
        s_TPtyp = "-"; // 温度計圧力計座なし
        s_DRtyp = "-"; // 液抜なし
        s_ARtyp = "-"; // エア抜なし

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "該当端管なし";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // 2022.04.21 CX-01D追加_E
    // 2013.04.03 SX-30 JIS30K端管
    // 2016.07.25 SX-30S追加
    //else if (P_Model == "SX-30" && s_NzCd == "41") {
    else if ( (P_Model == "SX-30" || P_Model == "SX-30S") &&
               s_NzCd =="41" ) {
    // *********************　　
        // 材質判別
        // 2016.08.26 材質ｺｰﾄﾞ変更
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "ＣＡＣ７０３") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "ＳＴＫＭ１３Ａ") {
        //    s_KeyMat = "CS";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // ＡＬＢＣ
        } else if ( s_KeyMat == "CS") {
            // ＣＳ(ＳＴＫＭ１３Ａ)
        } else {
            // その他
            s_KeyMat = "-";
        }
        // ***********************
        s_ULdiv = "-"; // 位置固定
        s_PGtyp = "-"; // コックなし
        s_TPtyp = "-"; // 温度計圧力計座なし
        s_DRtyp = "-"; // 液抜なし
        s_ARtyp = "-"; // エア抜なし

        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);

        if (P_Katasiki == "") {
            P_Katasiki = "該当端管なし";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);
            return true;
        }
    }
    // ***************************
    // ↓2003/1/9
    else if ((P_HEADS_DATA[36].Pos("M") != 0) && (Syurui.SubString(1,
        1) == "S")) {
        // if ( ( P_HEADS_DATA[36].Pos("M") != 0 ) &&
        // ( Syurui.SubString(1,1) == "S" ) ) {
        // ********************
        // 2004.05.20 舶用端管材質追加
        // 材質判別
        // 2016.08.26 材質ｺｰﾄﾞ変更
        //s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        //if (s_KeyMat.TrimRight() == "ＣＡＣ７０３") {
        //    s_KeyMat = "ALBC";
        //}
        //else if (s_KeyMat.TrimRight() == "ＳＴＰＧ") {
        //    s_KeyMat = "STPG";
        //}
        //else if (s_KeyMat.TrimRight() == "ＳＴＫＭ１３Ａ") {
        //    s_KeyMat = "CS";
        //}
        //else {
        //    s_KeyMat = "-";
        //}
        s_KeyMat = Type_MAT(s_MatCD);
        if ( s_KeyMat == "ALBC" ) {
            // ＡＬＢＣ
        } else if ( s_KeyMat == "STPG") {
            // ＳＴＰＧ
        } else if ( s_KeyMat == "CS") {
            // ＣＳ(ＳＴＫＭ１３Ａ)
        // 2019.04.12 SGP端管追加_S
        } else if ( s_KeyMat == "SGP") {
            // ＳＧＰ
        // 2019.04.12 SGP端管追加_E
        } else {
            // その他
            s_KeyMat = "-";
        }
        // ***********************
        //// 材質（ＳＴＰＧ）判別
        // s_KeyMat = Search_HD_MATERIAL(s_MatCD);
        // if ( s_KeyMat.TrimRight() == "ＳＴＰＧ") {
        // s_KeyMat = "STPG";
        // } else {
        // s_KeyMat = "-";
        // }
        // ***************************
        // 舶用コック／玉形弁 判別
        // 2011.04.22 川崎重工舶用コック廃止
        // 大田指示による
        // if ( P_HEADS_DATA[1063].Pos("川崎重工") != 0 ){
        // s_PGtyp = "C";
        // } else {
        // s_PGtyp = "T";
        // }
        s_PGtyp = "T";
        // *********************************
        // Ｐ座有無 判別
        // if( P_Model == "RX-70" ) {
        if (s_Prs_Syurui == 0) {
            s_TPtyp = "-";
        }
        else {
            s_TPtyp = "P";
        }
        // } else {
        // s_TPtyp = "-";
        // }
        s_DRtyp = "-";
        s_ARtyp = "-";
        // 2003.09.24 ＲＸ－７０のＰ座判別をやめ型式判別をする
        // if( P_Model == "RX-70" ) {
        // P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, false);
        // } else {
        // P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);
        // }
        AnsiString s_Frame; // 型式種類
        AnsiString s_Ver; // バージョンＮｏ.
        int i_Length; // 文字数

        // 2004.05.13 舶用端管材質追加
        if (P_Model == "RX-70" && (s_KeyMat == "STPG" || s_KeyMat == "-")) {
            // if( P_Model == "RX-70" ){
            // ***************************
            s_Frame = P_HEADS_DATA[36].TrimRight(); // ﾌﾚｰﾑ
            i_Length = s_Frame.Length();
            s_Frame = s_Frame.SubString(i_Length, 1);
            s_Ver = P_HEADS_DATA[1267].TrimRight();
            if (s_Frame == "R") {
                s_Frame = P_HEADS_DATA[36].TrimRight();
                s_Frame = s_Frame.SetLength(i_Length - 1);
            }
            else {
                s_Frame = P_HEADS_DATA[36].TrimRight();
            }

            if (s_Frame == "NPM" || s_Frame == "KNPM" || s_Frame == "TNPM") {
                if (s_Ver == "200207") {
                    s_KeyMat = "HP";
                }
                else {
                    s_KeyMat = "P";
                }
            }
            else if (s_Frame == "NHPM" || s_Frame == "KNHPM" ||
                s_Frame == "TNHPM") {
                s_KeyMat = "HP";
            }
            else {
                s_KeyMat = "-";
            }
        }
        P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp,
            s_TPtyp, s_DRtyp, s_ARtyp, true);
        // ***************************************************
        if (P_Katasiki == "") {
            P_Katasiki = "該当端管なし";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(1, 0, P_Katasiki, 0);

            if (s_DRtyp == "S") {
                P_Katasiki = P_Katasiki + "  " +
                    P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() +
                    P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
            }
            return true;
        }

        // ↑2003/1/9
    }
    else {
        if (s_Tkn_Syurui == "0") {
            return false;
        }

        // 02.10.16 新規追加条件
        // 2006.04.14 エア抜端管追加
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "S3" ||
            Syurui == "S4" || Syurui == "E1" || Syurui == "E2" ||
            Syurui == "E3" || Syurui == "E4") {
            // if (Syurui == "S3" || Syurui == "S4" || Syurui == "E3" || Syurui == "E4"){
            // *************************
            // ﾉｽﾞﾙ種類 = "3" かつ ﾉｽﾞﾙ継手の先頭二桁目 = "4"
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if (s_Nzl_Syurui == "3" && P_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(2,1) != "4" && s_Nzl_Kokei != 0 ){
            if (s_Nzl_Syurui == "3" && P_HEADS_DATA[HEADS[i_HeadsNo][5]]
                .SubString(3, 1) != "4" && s_Nzl_Kokei != 0) {
                // ***********************
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // if (s_Nzl_Tugite == "7"){
                if (s_Nzl_Tugite == "07") {
                    // ***********************
                    P_Katasiki = "端管Ｘ";
                    P_Zuban = "ＥＩ１４４０６０";
                    P_SZuban = P_Zuban;
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( s_NzCd == "4" || s_NzCd == "5" ){  //ANSI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////2007.07.30 液抜エア抜端管変更
                    // } else if ( s_NzCd == "8" || s_NzCd == "9" ){  //JPI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////*****************************
                    if (s_NzCd == "04" || s_NzCd == "05") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "08" || s_NzCd == "09") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "11" || s_NzCd == "12" || s_NzCd == "13")
                    { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "21" || s_NzCd == "22" || s_NzCd == "23")
                    { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                        // 2013.04.03 ﾉｽﾞﾙ規格追加
                    }
                    else if (s_NzCd == "44") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "48") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "14") { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "24") { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "31" || s_NzCd == "32" ||
                        s_NzCd == "33" || s_NzCd == "34") { // EN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                    }
                    else {
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 0);
                    }
                    // ***********************
                    // 端管Xの場合は、部品名欄に液抜きバルブの記述を追加する
                    P_Katasiki = P_Katasiki + "  " +
                        P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() +
                        P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
                    return true;
                }
                else {
                    P_Katasiki = "端管Ｙ";
                    P_Zuban = "ＥＩ１４４０６１";
                    P_SZuban = P_Zuban;
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( s_NzCd == "4" || s_NzCd == "5" ){  //ANSI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////2007.07.30 液抜エア抜端管変更
                    // } else if ( s_NzCd == "8" || s_NzCd == "9" ){  //JPI
                    // Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    ////*****************************
                    if (s_NzCd == "04" || s_NzCd == "05") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "08" || s_NzCd == "09") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "11" || s_NzCd == "12" || s_NzCd == "13")
                    { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "21" || s_NzCd == "22" || s_NzCd == "23")
                    { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                        // 2013.04.03 ﾉｽﾞﾙ規格追加
                    }
                    else if (s_NzCd == "44") { // ANSI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "48") { // JPI
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 1);
                    }
                    else if (s_NzCd == "14") { // GB
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "24") { // DIN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                    }
                    else if (s_NzCd == "31" || s_NzCd == "32" ||
                        s_NzCd == "33" || s_NzCd == "34") { // EN
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 2);
                        // ***********************
                    }
                    else {
                        Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 0);
                    }
                    return true;
                }
            }
        }

        // 圧力計検索キー作成
        if (s_Prs_Syurui == 0) {
            s_PGtyp = "-";
        }
        else if ((s_Prs_Syurui > 5) && (s_Prs_Syurui < 16)) { // 隔膜
            s_PGtyp = "K";
        }
        else if (s_Prs_Syurui == 5) { // ｽﾁｰﾑ
            s_PGtyp = "S";
            // 2000.02.26 圧力計追加
        }
        else if ((s_Prs_Syurui > 17) && (s_Prs_Syurui < 21)) { // ｽﾁｰﾑ
            s_PGtyp = "S";
            // *********************
        }
        else {
            s_PGtyp = "B"; // ﾌﾞﾙﾄﾞﾝ
        }

        // 温度計検索キー作成
        if (s_Tmp_Syurui == 0) {
            s_TPtyp = "-";
        }
        else {
            s_TPtyp = "T";
        }
        if (Syurui == "S1" || Syurui == "S2" || Syurui == "E1" ||
            Syurui == "E2") {
            // エア抜き有無
            s_DRtyp = "-";
            if (s_Ank_Kokei > 0) {
                s_ARtyp = "A";
            }
            else {
                s_ARtyp = "-";
            }
        }
        else {
            // 液抜きサイズ
            s_ARtyp = "-";
            if (s_Ank_Kokei > 0) {
                // 2011.08.04 ﾉｽﾞﾙ規格変更(未修正)
                // if ( s_Nzl_Tugite != "7" ) {
                if (s_Nzl_Tugite != "07") {
                    // ***********************
                    s_DRtyp = "L";
                }
                else {
                    s_DRtyp = "S";
                }
            }
            else {
                s_DRtyp = "-";
            }
        }

        // 2023.02.22 リング端管条件変更_S
        //P_Katasiki = Search_HD_FRNG_MST(P_Model, FormatFloat(0, s_Nzl_Kokei),
        //    Syurui.SubString(1, 1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, false);

        s_Model = P_Model;

        if ((P_Model == "CX-01" || P_Model == "CXW-01" ||
             P_Model == "CX-01D" || P_Model == "CXW-01D") &&
            P_HEADS_DATA[36].Pos("Y") != 0) {

            s_Model = P_Model+ "Y";

        }

        P_Katasiki = Search_HD_FRNG_MST(s_Model, FormatFloat(0, s_Nzl_Kokei),
            Syurui.SubString(1, 1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, false);
        // 2023.02.22 リング端管条件変更_E
        if (P_Katasiki == "") {
            // P_Katasiki = Search_HD_FRNG_MST("-", s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp);
            P_Katasiki = "該当端管なし";
            P_Zaisitu = "";
            return true;
        }
        if (P_Katasiki != "") {
            Search_HD_FRNG_DNO(0, iWFflg, P_Katasiki, 0);

            if (s_DRtyp == "S") {
                // P_Hosoku = P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() + P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
                P_Katasiki = P_Katasiki + "  " +
                    P_HEADS_DATA[HEADS[i_HeadsNo][7]].TrimRight() +
                    P_HEADS_DATA[HEADS[i_HeadsNo][8]].TrimRight();
            }
            return true;
        }
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： リング部品ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： AnsiString Dno：ﾘﾝｸﾞ図番
//             ： AnsiString Code：部品ｺｰﾄﾞ
//             ： AnsiString Syurui：端管種類(S1～E4)
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2019.04.11追加
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Get_Ring_Data(AnsiString Dno, AnsiString Code,
    AnsiString Syurui) {

    int HEADS[8][11] = {
        {1136, 901, 898, 895, 892, 905, 1144},
        {1137, 916, 913, 910, 907, 920, 1145},
        {1138, 931, 928, 925, 922, 935, 1146},
        {1139, 946, 943, 940, 937, 950, 1147},
        {1140, 961, 958, 955, 952, 965, 1148},
        {1141, 976, 973, 970, 967, 980, 1149},
        {1142, 991, 988, 985, 982, 995, 1150},
        {1143, 1006, 1003, 1000, 997, 1010, 1151}};
        //1列目：端管種類
        //2列目：圧力計種類
        //3列目：温度計種類
        //4列目：液抜、ｴｱ抜口径
        //5列目：ﾉｽﾞﾙ口径
        //6列目：液抜、ｴｱ抜継手
        //7行目：ﾉｽﾞﾙ継手

    AnsiString s_Tkn_Syurui;  // 端管種類
    int i_Prs_Syurui;         // 圧力計種類
    int i_Tmp_Syurui;         // 温度計種類
    double d_Ank_Kokei;       // 液抜、ｴｱ抜口径
    double d_Nzl_Kokei;       // ﾉｽﾞﾙ口径
    AnsiString s_AnCd;        // 液抜、ｴｱ抜継手ｺｰﾄﾞ
    AnsiString s_NzCd;        // ﾉｽﾞﾙ継手ｺｰﾄﾞ
    AnsiString s_Ring_Size;   // リング口径（検索用）
    AnsiString s_PGtyp;       // 圧力計種類（検索用）
    AnsiString s_Text;
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;

    int i_HeadsNo;

    // i_HeadsNo
    if (Syurui == "S1") {
        i_HeadsNo = 0;
    }
    else if (Syurui == "S2") {
        i_HeadsNo = 1;
    }
    else if (Syurui == "S3") {
        i_HeadsNo = 2;
    }
    else if (Syurui == "S4") {
        i_HeadsNo = 3;
    }
    else if (Syurui == "E1") {
        i_HeadsNo = 4;
    }
    else if (Syurui == "E2") {
        i_HeadsNo = 5;
    }
    else if (Syurui == "E3") {
        i_HeadsNo = 6;
    }
    else if (Syurui == "E4") {
        i_HeadsNo = 7;
    }

    // 端管種類
    s_Tkn_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight();
    // 圧力計種類
    i_Prs_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
    // 温度計種類
    i_Tmp_Syurui = P_HEADS_DATA[HEADS[i_HeadsNo][2]].TrimRight().ToIntDef(0);
    // ｴｱ抜き口径
    d_Ank_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][3]].TrimRight(), 0);
    // ﾉｽﾞﾙ口径
    d_Nzl_Kokei = StrToDblDef(P_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(), 0);
    // ｴｱ抜液抜継手
    s_AnCd = P_HEADS_DATA[HEADS[i_HeadsNo][5]].TrimRight();
    // ﾉｽﾞﾙ継手ｺｰﾄﾞ
    s_NzCd = P_HEADS_DATA[HEADS[i_HeadsNo][6]].TrimRight();

    // リング口径（検索用）
    s_Ring_Size = FormatFloat(0, d_Nzl_Kokei);

    // 圧力計種類（検索用）
    if (i_Prs_Syurui==0) {
        s_PGtyp = "-";
    } else {
        s_PGtyp = "P";
    }

    // *********************************************
    // ***　リングマスタ(HD_RING) 検索
    // *********************************************
    G_Log_Text = "リングマスタを、｢" + Dno + "｣と｢" + Code + "｣と｢"
        + s_Ring_Size + "｣と｢" + s_PGtyp + "｣で検索。";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_RING";
    s_SQL = s_SQL + "  WHERE DNO = '" + Dno + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = '" + Code + "'";
    s_SQL = s_SQL + "    AND RING_SIZE = '" + s_Ring_Size + "'";
    s_SQL = s_SQL + "    AND RING_TYPE = '" + s_PGtyp + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ﾘﾝｸﾞ図面｢" + Dno +
            "｣ 部品ｺｰﾄﾞ｢" + s_Text + "｣ 口径｢" + s_Ring_Size +
            "｣ ﾘﾝｸﾞ種別｢" + s_PGtyp +
            "｣ は、ﾘﾝｸﾞﾏｽﾀ(HD_RING)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        // 部品表識別子
        P_Sikibetu = "FRNG-P";                      //識別子
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight();                 // 部品名称
        P_Hosoku = "";

        // 端管用補足
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")
            ->AsString.TrimRight();                 // 材質名称
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // 材質名称(英)
        P_Zuban = "";                               // 図番
        P_SZuban = "";                              // 作図用図番
        P_Siyou1 = wkQuery->FieldByName("SPEC1")
            ->AsString.TrimRight();                 // 仕様1　
        P_Siyou2 = wkQuery->FieldByName("SPEC2")
            ->AsString.TrimRight();                 // 仕様2
    }
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾉｽﾞﾙ
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Write_Nozzle_Data(void) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    AnsiString sSpecCode; // 仕様ﾀｲﾌﾟｺｰﾄﾞ
    // **********************************************
    // 仕様ﾀｲﾌﾟを取得
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03
    // ----------------------------------------------

    // **********************************************
    // ***  部品展開ﾃﾞｰﾀ　OPEN
    // ***    初回のOPENなので上書きモードで作成
    // **********************************************
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((wkfp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(wkfp);
        return false;
    }

    // 1  部品分類
    s_Text = "NOZZLE";
    fputs(s_Text.c_str(), wkfp);

    // 2  識別子
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), wkfp);

    // 3  部品型式
    s_Text = "," + P_Katasiki;
    fputs(s_Text.c_str(), wkfp);

    // 4  部品名称
    s_Text = "," + P_Hosoku;
    fputs(s_Text.c_str(), wkfp);

    // 5  材質名称
    s_Text = "," + P_Zaisitu;
    fputs(s_Text.c_str(), wkfp);
    // 6  図番
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), wkfp);
    // 7  作図用図番
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), wkfp);

    // 8  1台分数量
    s_Text = ",1";
    fputs(s_Text.c_str(), wkfp);

    // ,9  予備数
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), wkfp);

    // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
    i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
    i_Soukei = 1 * i_Daisu + P_Yobisu;
    s_Text = "," + AnsiString(i_Soukei);
    fputs(s_Text.c_str(), wkfp);

    // 11 仕様１
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), wkfp);
    // 12 仕様２
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);
    // 仕様３
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), wkfp);

    // 13 仕様３,４,５
    s_Text = ",,";
    fputs(s_Text.c_str(), wkfp);

    // 16  材質名称(英)
    s_Text = "," + P_EZaisitu;
    fputs(s_Text.c_str(), wkfp);

    // 17  仕様ﾀｲﾌﾟｺｰﾄﾞ
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), wkfp);

    // 改行文字
    s_Text = "\n";
    // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
    fputs(s_Text.c_str(), wkfp);

    fclose(wkfp); // 閉じる

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_FRNG_DNOﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ：
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TNozzleDetails::Search_HD_FRNG_DNO(int Model, int Typ,
    AnsiString Key, int flgAnsi) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_Frame;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_FRNG_DNO";
    s_SQL = s_SQL + "  WHERE MODEL_TYPE = " + Model + "";
    s_SQL = s_SQL + "    AND FRANGE_TYPE = " + Typ + "";
    s_SQL = s_SQL + "    AND FLG_ANSI = " + flgAnsi + "";
    s_SQL = s_SQL + "    AND FRANGE_NAME = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        P_Zuban = "";
        P_SZuban = "";

        delete wkQuery;
        return false;
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // 作図用図番

        G_Log_Text = "図    番1    ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番2  ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 液抜き口径表記変換
// 概  要      ：
// 引  数      ：
// 戻り値      ： 変換結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall TNozzleDetails::ChgAirDrnSize(double dSize) {
    AnsiString sSizeStr;

    sSizeStr = "";
    if (dSize == 10.0) {
        sSizeStr = "3/8";
    }
    else if (dSize == 15.0) {
        sSizeStr = "1/2";
    }
    else if (dSize == 20.0) {
        sSizeStr = "3/4";
    }
    else if (dSize == 25.0) {
        sSizeStr = "1";
    }
    else if (dSize == 32.0) {
        sSizeStr = "1 1/4";
    }
    else if (dSize == 40.0) {
        sSizeStr = "1 1/2";
    }
    else if (dSize == 50.0) {
        sSizeStr = "2";
    }
    else if (dSize == 65.0) {
        sSizeStr = "2 1/2";
    }
    else if (dSize == 80.0) {
        sSizeStr = "3";
    }
    else if (dSize == 90.0) {
        sSizeStr = "3 1/2";
    }
    else if (dSize == 100.0) {
        sSizeStr = "4";
    }
    else if (dSize == 125.0) {
        sSizeStr = "5";
    }
    else if (dSize == 150.0) {
        sSizeStr = "6";
    }
    // 2020.09.16 S口径追加_S
    else if (dSize == 0.37) {
        sSizeStr = "3/8";
    }
    else if (dSize == 0.5) {
        sSizeStr = "1/2";
    }
    else if (dSize == 0.75) {
        sSizeStr = "3/4";
    }
    else if (dSize == 1.0) {
        sSizeStr = "1";
    }
    else if (dSize == 1.5) {
        sSizeStr = "1 1/2";
    }
    else if (dSize == 2.0) {
        sSizeStr = "2";
    }

    else if (dSize == 2.5) {
        sSizeStr = "2 1/2";
    }
    else if (dSize == 3.0) {
        sSizeStr = "3";
    }
    else if (dSize == 4.0) {
        sSizeStr = "4";
    }
    // 2020.09.16 S口径追加_E
    return sSizeStr;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 液抜き口径表記変換
// 概  要      ：
// 引  数      ：
// 戻り値      ： 変換結果
// 備  考      ： 2008.06.26追加
// ---------------------------------------------------------------------------
AnsiString __fastcall TNozzleDetails::ChgNozzleSize(double dSize) {
    AnsiString sSizeStr;
    int i_Kokei;

    sSizeStr = "";
    if (dSize == 0.75) {
        sSizeStr = "3/4";
    }
    else {
        i_Kokei = (int)dSize;
        sSizeStr = i_Kokei;
    }
    return sSizeStr;
}
