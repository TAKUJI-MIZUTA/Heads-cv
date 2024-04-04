// ---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：圧力計部品展開処理  モジュール
// 作 成 者：T.Kawai
// 作 成 日：2002.08.06
// 更 新 日：2002.09.04
// 2007.10.03 ﾐﾗｰ仕様対応
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop

#include <io.h>                                       "
#include <Filectrl.hpp>

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_PressGDetails.h"

#include "HeadsData.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 圧力計部品展開処理 Class
// ---------------------------------------------------------------------------
class TPressGDetails {
private:
    FILE *fp;

    // CSVﾌｧｲﾙから取得
    AnsiString P_Sikibetu; // 識別子
    AnsiString P_Katasiki; // 部品型式
    AnsiString P_Yobihin; // 予備品名

    // ﾏｽﾀから取得
    AnsiString P_Zaisitu; // 材質名称
    AnsiString P_EZaisitu; // 材質名称
    AnsiString P_Zuban; // 図番
    AnsiString P_SZuban; // 作図用図番
    int P_Num; // 数量
    int P_Yobisu; // 予備数
    AnsiString P_Siyou; // 仕様
    AnsiString P_Unit; // 単位

    // ------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // 2007.10.03
    // ------------------------------

    bool __fastcall PressG_Data_Sakusei(void); // 圧力計ﾃﾞｰﾀ 作成
    bool __fastcall Get_PressG_Data(AnsiString a_SE, AnsiString Syurui);
    // 圧力計ﾃﾞｰﾀ取得
    bool __fastcall Search_PRESS_CODE(AnsiString Code); // 圧力計
    AnsiString __fastcall Search_HD_PGAGE(AnsiString a_SE,
        AnsiString Key); // 圧力計

    // bool __fastcall Write_Press_Data(AnsiString a_SE);
    bool __fastcall Write_PressG_Data(AnsiString a_SE);

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // 圧力計部品展開処理 実行
    int __fastcall Get_Press_YobiSu(int i_TmpCd); // 圧力計予備品取得

};

// ---------------------------------------------------------------------------
// 日本語関数名： 圧力計部品展開処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall PressGDetails(HeadsData* pcHeadsData) {
    bool ret;

    TPressGDetails pressGDetails; // 圧力計ﾃﾞｰﾀ 作成Class自動生成,関数終了後破棄

    ret = pressGDetails.Excute(pcHeadsData); // 圧力計ﾃﾞｰﾀ 作成実行

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 圧力計部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADSﾃｷｽﾄﾃﾞｰﾀのﾎﾟｲﾝﾀをｾｯﾄ
    m_pcHeadsData = pcHeadsData;

    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『圧力計部品展開処理』開始     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『圧力計部品展開処理』開始     *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  圧力計ﾃﾞｰﾀ作成
    // ***********************************
    PressG_Data_Sakusei();

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『圧力計部品展開処理』終了     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『圧力計部品展開処理』終了     *********");
    Write_Error_Log("*******************************************************");
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 圧力計ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::PressG_Data_Sakusei(void) {
    AnsiString s_Prs_Syurui; // 圧力計種類
    // 2003.12.24 YX-80 追加
    AnsiString s_Text;
    // *********************
    // 2017.07.11 斜行流追加
    int iFL;
    // *********************


    // 2017.07.11 斜行流追加
    // ****************************************************************
    // ***                                                          ***
    // ***                   斜      行      流                     ***
    // ***                                                          ***
    // ****************************************************************
   // 2022.04.21 CX-01D追加_S
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ") {
        iFL = 1; // 斜行流
    }
    //if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
    else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
    // 2022.04.21 CX-01D追加_S
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        iFL = 1; // 斜行流
    }
    else {
        iFL = 0; // 台形流
    }
    // *********************


    // ****************************************************************
    // ***                                                          ***
    // ***               Ｂ側圧力計データ　作成                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S1圧力計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[901].TrimRight(); // S1圧力計種類


    if (s_Prs_Syurui == "00") {
        G_Log_Text = "S1圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("S1", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 追加
    ////***********************************
    ////***  S3圧力計ﾃﾞｰﾀ　作成
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[931].TrimRight();   // S3圧力計種類
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "S3圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17"){
    // G_Log_Text = "S3圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("S3", s_Prs_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  S4圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[946].TrimRight(); // S4圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S4圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S4圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S4", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S3圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[931].TrimRight(); // S3圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S3圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S3圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S3", s_Prs_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E1圧力計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[961].TrimRight(); // E1圧力計種類

    if (s_Prs_Syurui == "00") {
        G_Log_Text = "E1圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("E1", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 追加
    ////***********************************
    ////***  E3圧力計ﾃﾞｰﾀ　作成
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[991].TrimRight();   // E3圧力計種類
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "E3圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17"){
    // G_Log_Text = "E3圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("E3", s_Prs_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  E4圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[1006].TrimRight(); // E4圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E4圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E4圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E4", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E3圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[991].TrimRight(); // E3圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E3圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E3圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E3", s_Prs_Syurui);
        }
    }
    // *********************

    // ****************************************************************
    // ***                                                          ***
    // ***               Ａ側圧力計データ　作成                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S2圧力計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[916].TrimRight(); // S2圧力計種類

    if (s_Prs_Syurui == "00") {
        G_Log_Text = "S2圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("S2", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 追加
    ////***********************************
    ////***  S4圧力計ﾃﾞｰﾀ　作成
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[946].TrimRight();   // S4圧力計種類
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "S4圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("S4", s_Prs_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  S3圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[931].TrimRight(); // S3圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S3圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S3圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S3", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S4圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[946].TrimRight(); // S4圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "S4圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "S4圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("S4", s_Prs_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E2圧力計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Prs_Syurui = P_HEADS_DATA[976].TrimRight(); // E2圧力計種類

    if (s_Prs_Syurui == "00") {
        G_Log_Text = "E2圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_PressG_Data("E2", s_Prs_Syurui);
    }

    // 2003.12.24 YX-80 追加
    ////***********************************
    ////***  E4圧力計ﾃﾞｰﾀ　作成
    ////***********************************
    // s_Prs_Syurui = P_HEADS_DATA[1006].TrimRight();  // E4圧力計種類
    //
    // if (s_Prs_Syurui == "00"){
    // G_Log_Text = "E4圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_PressG_Data("E4", s_Prs_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  E3圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[991].TrimRight(); // E3圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E3圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E3圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E3", s_Prs_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E4圧力計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Prs_Syurui = P_HEADS_DATA[1006].TrimRight(); // E4圧力計種類

        if (s_Prs_Syurui == "00") {
            G_Log_Text = "E4圧力計ﾃﾞｰﾀ  圧力計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") {
            G_Log_Text = "E4圧力計ﾃﾞｰﾀ  差圧計なので下側は作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_PressG_Data("E4", s_Prs_Syurui);
        }
    }
    // *********************

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 圧力計部品展開処理 作成実行
// 概  要      ：
// 引  数      ： AnsiString a_SE   ： 圧力計SE区分(S1～S4,E1～E4)
// ： AnsiString Syurui ： 圧力計種類
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Get_PressG_Data(AnsiString a_SE,
    AnsiString Syurui) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ

    // *********************************************
    // ***  圧力計部品構成ﾏｽﾀ(HD_TEMP) 検索
    // *********************************************
    s_Code = Search_HD_PGAGE(a_SE, Syurui);

    if (s_Code == "\0") {
        G_ErrLog_Text = Syurui + "ﾃﾞｰﾀ作成時　" + a_SE + "圧力計種類 ｢" + Syurui +
            "｣ は、圧力計部品構成ﾏｽﾀ(HD_TEMP)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);

        return false;
    }

    return true;

}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_PGAGEﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ： AnsiString a_SE   ： 圧力計SE区分(S1～S4,E1～E4)
// ： AnsiString Key    ： 圧力計種類
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall TPressGDetails::Search_HD_PGAGE(AnsiString a_SE,
    AnsiString Key) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_Code;
    AnsiString s_BaseMatM; // 金属系基準材質
    AnsiString s_BaseMatL; // 非金属系基準材質
    AnsiString s_Memori;
    AnsiString s_SaniSize; // サニタリノズル口径
    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
    AnsiString s_FrngSize; // ノズルフランジ口径
    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
    // 2003.12.24 YX-80 追加
    AnsiString s_Text;
    // *********************
    // 2012.03.15 直管曲管判断変更
    AnsiString s_Tmp_Syurui; // 温度計種類
    AnsiString s_BOOTS; // ﾌﾞｰﾂ種類(N:1,T:2,K:3,P:4)
    bool b_Tmp; // 温度計有無
    bool b_YX; // 圧力計配管曲管ﾉｽﾞﾙ
    bool b_STorCU; // 圧力計配管(直管:true,曲管:false)
    // ***************************
    bool b_NZorFR;
    int i_wYobi;
    // 2017.07.11 斜行流追加
    int iFL;
    // *********************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PGAGE";
    s_SQL = s_SQL + "  WHERE PRES_TYPE = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // 2008.03.21 圧力計取付台ﾐﾗｰ仕様対応
    AnsiString sSpecCode; // 仕様ﾀｲﾌﾟｺｰﾄﾞ
    // **********************************************
    // 仕様ﾀｲﾌﾟを取得
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // **********************************

    // 各項目の初期化
    s_Code = "";
    P_Zuban = "";
    P_SZuban = "";
    P_Num = 0;
    P_Zaisitu = "";
    P_Siyou = "";

    // 2017.07.11 斜行流追加
    // 斜行流
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        iFL = 1; // 斜行流
    }
    else {
        iFL = 0; // 台形流
    }
    // *********************

    if (a_SE == "S1") {
        if (P_HEADS_DATA[1136].ToIntDef(0) != 0) {
            // 2004.07.02 端管材質データ位置変更
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[894].TrimRight();
            // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
            // *************************************************
            // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
            ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[893].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
            // } else {
            // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 圧力計材質変更_S
            if (P_HEADS_DATA[893].SubString(3, 1) == "2") {
                // 端管無し、
                // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                // プレート材質を使う　　　
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
            }
            // 2018.03.26 圧力計材質変更_S
        }
        s_Memori = P_HEADS_DATA[902].TrimRight() + "/" +
            P_HEADS_DATA[903].TrimRight() + "ＭＰａ";
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
    }
    else if (a_SE == "S2") {
        if (P_HEADS_DATA[1137].ToIntDef(0) != 0) {
            // 2004.07.02 端管材質データ位置変更
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[909].TrimRight();
            // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
            // *************************************************
            // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
            ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[908].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
            // } else {
            // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 圧力計材質変更_S
            if (P_HEADS_DATA[908].SubString(3, 1) == "2") {
                // 端管無し、
                // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                // プレート材質を使う　　　
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
                }
            }
            // 2018.03.26 圧力計材質変更_S
        }
        s_Memori = P_HEADS_DATA[917].TrimRight() + "/" +
            P_HEADS_DATA[918].TrimRight() + "ＭＰａ";
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
    }
    else if (a_SE == "S3") {
        // 2003.12.24 YX-80 追加
        // if (P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[924].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[932].TrimRight() + "/" + P_HEADS_DATA[933].TrimRight() + "ＭＰａ";
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // 斜行流
        // *********************
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[923].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[923].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[932].TrimRight() + "/" +
                P_HEADS_DATA[933].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[923].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[923].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[932].TrimRight() + "/" +
                P_HEADS_DATA[933].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        // *********************
    }
    else if (a_SE == "S4") {
        // 2003.12.24 YX-80 追加
        // if (P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[939].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[947].TrimRight() + "/" + P_HEADS_DATA[948].TrimRight() + "ＭＰａ";
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // 斜行流
        // *********************
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[938].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[938].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[947].TrimRight() + "/" +
                P_HEADS_DATA[948].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[938].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[938].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[947].TrimRight() + "/" +
                P_HEADS_DATA[948].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
    }
    else if (a_SE == "E1") {
        if (P_HEADS_DATA[1140].ToIntDef(0) != 0) {
            // 2004.07.02 端管材質データ位置変更
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[954].TrimRight();
            // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
            // *************************************************
            // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
            ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[953].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
            // } else {
            // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 圧力計材質変更_S
            if (P_HEADS_DATA[953].SubString(3, 1) == "2") {
                // 端管無し、
                // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                // プレート材質を使う　　　
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
            }
            // 2018.03.26 圧力計材質変更_S
        }
        s_Memori = P_HEADS_DATA[962].TrimRight() + "/" +
            P_HEADS_DATA[963].TrimRight() + "ＭＰａ";
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
    }
    else if (a_SE == "E2") {
        if (P_HEADS_DATA[1141].ToIntDef(0) != 0) {
            // 2004.07.02 端管材質データ位置変更
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
            // *********************************
            b_NZorFR = true;
        }
        else {
            s_BaseMatM = P_HEADS_DATA[969].TrimRight();
            // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if (s_BaseMatM.ToIntDef(0) >= 2000) {
            if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                (s_BaseMatM.ToIntDef(0) < 3000)) {
            // ***********************
                s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
            // *************************************************
            // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
            ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
            b_NZorFR = false;
            // s_Text = P_HEADS_DATA[968].TrimRight();
            // if(s_Text.SubString(2,1) != "4" ) {
            // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
            // } else {
            // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
            // }
            ////**********************************************
            // **************************************************
            // 2018.03.26 圧力計材質変更_S
            if (P_HEADS_DATA[968].SubString(3, 1) == "2") {
                // 端管無し、
                // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                // プレート材質を使う　　　
                if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                    (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
                }
            }
            // 2018.03.26 圧力計材質変更_S
        }
        s_Memori = P_HEADS_DATA[977].TrimRight() + "/" +
            P_HEADS_DATA[978].TrimRight() + "ＭＰａ";
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
    }
    else if (a_SE == "E3") {
        // 2003.12.24 YX-80 追加
        // if (P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[984].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[992].TrimRight() + "/" + P_HEADS_DATA[993].TrimRight() + "ＭＰａ";
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // 斜行流
        // *********************
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[983].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[983].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[992].TrimRight() + "/" +
                P_HEADS_DATA[993].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[983].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[983].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[992].TrimRight() + "/" +
                P_HEADS_DATA[993].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        // *********************
    }
    else if (a_SE == "E4") {
        // 2003.12.24 YX-80 追加
        // if (P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {
        // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
        // b_NZorFR = true;
        // } else {
        // s_BaseMatM = P_HEADS_DATA[999].TrimRight();
        // b_NZorFR = false;
        // }
        // s_Memori = P_HEADS_DATA[1007].TrimRight() + "/" + P_HEADS_DATA[1008].TrimRight() + "ＭＰａ";
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
            // 斜行流
        // *********************
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[138].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[998].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[998].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[1007].TrimRight() + "/" +
                P_HEADS_DATA[1008].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {
                // 2004.07.02 端管材質データ位置変更
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
                // s_BaseMatM = P_HEADS_DATA[139].SubString(1,4);
                // *********************************
                b_NZorFR = true;
            }
            else {
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2006.02.06 ﾉｽﾞﾙ材質がｺﾞﾑの場合ﾌﾟﾚｰﾄ材質を使用する
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (s_BaseMatM.ToIntDef(0) >= 2000) {
                if ((s_BaseMatM.ToIntDef(0) >= 2000) &&
                    (s_BaseMatM.ToIntDef(0) < 3000)) {
                // ***********************
                    s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                    // 2024.02.06 ALLOY材質変換_S
                    s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                    // 2024.02.06 ALLOY材質変換_E
                }
                // *************************************************
                // 2007.12.17 客先配管(ﾌﾞｰﾂ)とﾊﾟｲﾌﾟﾉｽﾞﾙの区別をやめる
                ////2006.04.14 ﾊﾟｲﾌﾟﾉｽﾞﾙと客先配管(ﾌﾞｰﾂ)を区別する
                b_NZorFR = false;
                // s_Text = P_HEADS_DATA[998].TrimRight();
                // if(s_Text.SubString(2,1) != "4" ) {
                // b_NZorFR = true; //客先配管(ﾌﾞｰﾂ)
                // } else {
                // b_NZorFR = false; //ﾊﾟｲﾌﾟﾉｽﾞﾙ
                // }
                ////**********************************************
                // **************************************************
                // 2018.03.26 圧力計材質変更_S
                if (P_HEADS_DATA[998].SubString(3, 1) == "2") {
                    // 端管無し、
                    // ﾁｭｰﾌﾞﾌﾗﾝｼﾞ==SUS316L、
                    // ﾌﾟﾚｰﾄ!=SUS316Lの場合、
                    // プレート材質を使う　　　
                    if ((Type_MAT(s_BaseMatM) == "SUS316L") &&
                        (Type_MAT(P_HEADS_DATA[43].TrimRight()) != "SUS316L")) {
                        s_BaseMatM = P_HEADS_DATA[43].TrimRight(); // ﾌﾟﾚｰﾄ材質
                        // 2024.02.06 ALLOY材質変換_S
                        s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                        // 2024.02.06 ALLOY材質変換_E
                    }
                }
                // 2018.03.26 圧力計材質変更_S
            }
            s_Memori = P_HEADS_DATA[1007].TrimRight() + "/" +
                P_HEADS_DATA[1008].TrimRight() + "ＭＰａ";
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
    }
    // i_wYobi = Get_Press_YobiSu(Key.ToIntDef(0));
    i_wYobi = 0;

    if (a_SE == "S1") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[892].TrimRight(), 0.0));
    }
    else if (a_SE == "S2") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[907].TrimRight(), 0.0));
    }
    else if (a_SE == "S3") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[922].TrimRight(), 0.0));
    }
    else if (a_SE == "S4") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[937].TrimRight(), 0.0));
    }
    else if (a_SE == "E1") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[952].TrimRight(), 0.0));
    }
    else if (a_SE == "E2") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[967].TrimRight(), 0.0));
    }
    else if (a_SE == "E3") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[982].TrimRight(), 0.0));
    }
    else if (a_SE == "E4") {
        s_SaniSize =
            FormatFloat("#.#", StrToDblDef(P_HEADS_DATA[997].TrimRight(), 0.0));
    }

    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
    if (a_SE == "S1") {
        s_FrngSize = P_HEADS_DATA[892].TrimRight();
    }
    else if (a_SE == "S2") {
        s_FrngSize = P_HEADS_DATA[907].TrimRight();
    }
    else if (a_SE == "S3") {
        s_FrngSize = P_HEADS_DATA[922].TrimRight();
    }
    else if (a_SE == "S4") {
        s_FrngSize = P_HEADS_DATA[937].TrimRight();
    }
    else if (a_SE == "E1") {
        s_FrngSize = P_HEADS_DATA[952].TrimRight();
    }
    else if (a_SE == "E2") {
        s_FrngSize = P_HEADS_DATA[967].TrimRight();
    }
    else if (a_SE == "E3") {
        s_FrngSize = P_HEADS_DATA[982].TrimRight();
    }
    else if (a_SE == "E4") {
        s_FrngSize = P_HEADS_DATA[997].TrimRight();
    }
    // 2020.01.14 ﾘﾝｸﾞ端管変更_E

    // 2012.03.15 直管曲管判断変更
    s_Tmp_Syurui = "";
    if (a_SE == "S1") {
        s_BOOTS = P_HEADS_DATA[893].SubString(3, 1); // S1ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[898].TrimRight(); // S1温度計種類
    }
    else if (a_SE == "S2") {
        s_BOOTS = P_HEADS_DATA[908].SubString(3, 1); // S2ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[913].TrimRight(); // S2温度計種類
    }
    else if (a_SE == "S3") {
        s_BOOTS = P_HEADS_DATA[923].SubString(3, 1); // S3ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight(); // S3温度計種類
    }
    else if (a_SE == "S4") {
        s_BOOTS = P_HEADS_DATA[938].SubString(3, 1); // S4ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight(); // S4温度計種類
    }
    else if (a_SE == "E1") {
        s_BOOTS = P_HEADS_DATA[953].SubString(3, 1); // E1ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[958].TrimRight(); // E1温度計種類
    }
    else if (a_SE == "E2") {
        s_BOOTS = P_HEADS_DATA[968].SubString(3, 1); // E2ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[973].TrimRight(); // E2温度計種類
    }
    else if (a_SE == "E3") {
        s_BOOTS = P_HEADS_DATA[983].SubString(3, 1); // E3ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight(); // E3温度計種類
    }
    else if (a_SE == "E4") {
        s_BOOTS = P_HEADS_DATA[998].SubString(3, 1); // E4ﾌﾞｰﾂ種類
        s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight(); // E4温度計種類
    }
    if (s_Tmp_Syurui == "00") {
        b_Tmp = false;
    }
    else {
        b_Tmp = true;
    }
    if (P_HEADS_DATA[823].ToIntDef(0) == 111 && P_HEADS_DATA[1011].TrimRight()
        .ToIntDef(0) == 80) { // YX-80
        b_YX = true; // ﾊﾟｲﾌﾟﾉｽﾞﾙ圧力計配管：曲管
    }
    // 2017.07.11 YX-83追加
    else if (P_HEADS_DATA[823].ToIntDef(0) == 111 && P_HEADS_DATA[1011].TrimRight()
        .ToIntDef(0) == 83) { // YX-83
        b_YX = true; // ﾊﾟｲﾌﾟﾉｽﾞﾙ圧力計配管：曲管
    }
    // ********************
    else {
        b_YX = false; // ﾊﾟｲﾌﾟﾉｽﾞﾙ圧力計配管：直管
    }

    if (s_BOOTS == "4") {
        // ﾊﾟｲﾌﾟﾉｽﾞﾙ
        if (b_YX == true) {
            // ﾊﾟｲﾌﾟﾉｽﾞﾙ圧力計配管：曲管
            if (b_Tmp == true) {
                // 温度計有
                b_STorCU = false; // 曲管
            }
            else {
                // 温度計無
                b_STorCU = false; // 曲管
            }
        }
        else {
            // ﾊﾟｲﾌﾟﾉｽﾞﾙ圧力計配管：直管
            b_STorCU = true; // 直管
        }
    }
    else if (b_NZorFR == true) {
        // 端管
        if (a_SE.SubString(1, 1) == "S") {
            // Ｓ側端管
            if (b_Tmp == true) {
                // 温度計有
                b_STorCU = false; // 曲管
            }
            else {
                // 温度計無
                b_STorCU = true; // 直管
            }
        }
        else {
            // Ｅ側端管
            b_STorCU = true; // 直管
        }
    }
    else {
        // 客先配管
        b_STorCU = true; // 直管
    }
    // ***************************

    while (!wkQuery->Eof) {
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // 部品ｺｰﾄﾞ
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // 作図用図番
        P_Num = wkQuery->FieldByName("NUM")->AsCurrency; // 数量
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // 材質名称
        P_EZaisitu = P_Zaisitu; // 材質名称
        P_Siyou = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight
            (); // 仕様
        P_Unit = "";

        // 2004.01.27 差圧計２個追加
        AnsiString s_Prs_Syurui; // 圧力計種類
        // 2008.03.21 圧力計取付台ﾐﾗｰ仕様対応
        if (sSpecCode == "B") { // ﾐﾗｰB仕様
            if (a_SE == "S2" && (Key == "16" || Key == "17")) { // S2圧力計種類
                s_Prs_Syurui = P_HEADS_DATA[901].TrimRight();
                if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") { // S1圧力計種類
                    if (s_Code.ToIntDef(0) == 21) {
                        P_Zuban = "Ｅ４２５７８２－Ｒ２"; // 図番
                        P_SZuban = "Ｅ４２５７８２"; // 作図用図番
                    }
                }
            }
        }
        else { // ﾐﾗｰA仕様,ﾐﾗｰ以外
            if (a_SE == "S1" && (Key == "16" || Key == "17")) { // S1圧力計種類
                s_Prs_Syurui = P_HEADS_DATA[916].TrimRight();
                if (s_Prs_Syurui == "16" || s_Prs_Syurui == "17") { // S2圧力計種類
                    if (s_Code.ToIntDef(0) == 21) {
                        P_Zuban = "Ｅ４２５７８２－Ｒ２"; // 図番
                        P_SZuban = "Ｅ４２５７８２"; // 作図用図番
                    }
                }
            }
        }
        // if ( a_SE == "S1" && ( Key == "16" || Key == "17" ) ){  // S1圧力計種類
        // s_Prs_Syurui = P_HEADS_DATA[916].TrimRight();
        // if ( s_Prs_Syurui == "16" || s_Prs_Syurui == "17" ){ // S2圧力計種類
        // if ( s_Code.ToIntDef(0) == 21 ) {
        // //2006.06.28 図番変更
        // //P_Zuban   = "Ｅ４２５７８２";      // 図番
        // P_Zuban   = "Ｅ４２５７８２－Ｒ２";// 図番
        // //*******************
        // P_SZuban  = "Ｅ４２５７８２";      // 作図用図番
        // }
        // }
        // }
        // **********************************
        // *************************

        // *********************************
        // ***  圧力計構成部品ﾃｰﾌﾞﾙ検索
        // *********************************
        Search_PRESS_CODE(s_Code);

        // *********************************
        // ***  予備品数 取得(02.08.05追加)
        // *********************************
        // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0");
        P_Yobisu = 0;
        switch (s_Code.ToIntDef(0)) {
        case 11: // 隔膜式圧力計
            // 2004.02.04 サニタリ圧力計
            // P_Zaisitu = Search_HD_E_MATERIAL(s_BaseMatM);
            // if (P_Zaisitu.Pos("ＳＵＳ３０４") != 0 ) {
            // P_Zaisitu = "ＳＵＳ３０４";
            // P_EZaisitu = "ＳＵＳ３０４";
            // } else {
            // P_Zaisitu = "ＳＵＳ３１６";
            // P_EZaisitu = "ＳＵＳ３１６";
            // }
            if (Key == "14" || Key == "15") { // サニタリ圧力計
                P_Zaisitu = Search_HD_MATERIAL(s_BaseMatM);
                if (P_Zaisitu.Pos("ＳＵＳ３０４") != 0) {
                    P_Zaisitu = "ＳＵＳ３０４";
                    P_EZaisitu = "ＳＵＳ３０４";
                }
                else {
                    P_Zaisitu = "ＳＵＳ３１６";
                    P_EZaisitu = "ＳＵＳ３１６";
                }
            }
            // *************************
        case 1: // 圧力計本体
        case 20: // 二針式圧力計
            P_Siyou = P_Siyou + "  " + s_Memori;
            P_Yobisu = i_wYobi;
            break;
        case 2: // 曲管
        case 3: // 直管
            // 2006.04.14 ﾊﾟｲﾌﾟ,ｴﾙﾎﾞ追加
        case 25: // ﾊﾟｲﾌﾟ
        case 26: // ｴﾙﾎﾞ
            // *************************
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            // 2014.06.24 ｴﾙﾎﾞ材質変更
            if (P_Zaisitu == "1161") { // チタン２種
                P_Zaisitu = "1032"; // ＴＰ３４０に置き換える
            }
            // ***********************
            // 2014.07.08 ｴﾙﾎﾞ材質変更
            if (P_Zaisitu == "1162") { // チタン２種
                P_Zaisitu = "1033"; // ＴＰ３４０－Ｐｄに置き換える
            }
            // ***********************
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 4: // メートルコック
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 4);
            if (Key == "06" || Key == "07" || Key == "02")
            { // 圧力計６・７の場合、変換後のｺｰﾄﾞを修正
                if (P_Zaisitu == "1071") { // Ｃ３６００
                    P_Zaisitu = "1003"; // ＳＵＳ３１６に置き換える
                }
                // 2011.02.09 材質変更
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (P_Zaisitu == "1001") { // ＳＵＳ３０４
                if (Type_MAT(P_Zaisitu) == "SUS304")  { // ＳＵＳ３０４
                // ***********************
                    P_Zaisitu = "1003"; // ＳＵＳ３１６に置き換える
                }
                if (P_Zaisitu == "1005") { // ＳＵＳ３１６Ｊ１Ｌ
                    P_Zaisitu = "1003"; // ＳＵＳ３１６に置き換える
                }
                if (P_Zaisitu == "1030") { // ＳＵＳ３１５Ｊ１
                    P_Zaisitu = "1003"; // ＳＵＳ３１６に置き換える
                }
                // ********************
            // 2016.06.08 メートルコック材質変更
            }
            else if (Key == "01")
            {
               // 圧力計１の場合、Ｃ３６００
                // 2017.08.17 C3600廃止_S
                //P_Zaisitu = "1071"; // Ｃ３６００
                // 2017.08.17 C3600廃止_E　　
            // *********************************
            }
            // 2017.08.17 C3600廃止_S
            if (P_Zaisitu == "31CC") { // Ｃ３６００
                P_Zaisitu = "1003"; // ＳＵＳ３１６に置き換える
            }
            // 2017.08.17 C3600廃止_E
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 18: // チーズ
            // P_Zaisitu = Chg_Zaisitu_Code( s_BaseMatM, 3);
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // 2004.02.05
            // P_Zaisitu = Search_HD_E_MATERIAL(s_BaseMatM);
            P_Zaisitu = Search_HD_MATERIAL(s_BaseMatM);
            // **********
            if (P_Zaisitu.Pos("ＳＵＳ３０４") != 0) {
                P_Zaisitu = "ＳＵＳ３０４";
                P_EZaisitu = "ＳＵＳ３０４";
            }
            else {
                P_Zaisitu = "ＳＵＳ３１６";
                P_EZaisitu = "ＳＵＳ３１６";
            }
            P_Yobisu = i_wYobi;
            P_Siyou = s_SaniSize + P_Siyou; // チーズは「ノズル口径×圧力計取付部口径」で表記
            if (Key.ToIntDef(0) == 14) { // サニタリーネジ
                P_Sikibetu = "CHES-N";
            }
            else { // サニタリーヘルール
                P_Sikibetu = "CHES-H";
            }
            break;
        case 5: // シートパッキング
        case 14: // パッキング
        case 22: // パッキング
            // 2004.02.10 スチーム条件追加
            // if ( Key.ToIntDef(0) == 5 ) { // スチーム用はＥＰＤＭ固定
            if (Key.ToIntDef(0) == 5 || Key.ToIntDef(0) == 18 || Key.ToIntDef(0)
                == 19 || Key.ToIntDef(0) == 20) { // スチーム用はＥＰＤＭ固定
                // ***************************
                P_Zaisitu = Search_HD_MATERIAL("2026");
                P_EZaisitu = Search_HD_E_MATERIAL("2026");
            }
            else {
                P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatL, 1);
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            P_Yobisu = i_wYobi;
            if (s_Code.ToIntDef(0) == 22)
            { // ﾊﾟｯｷﾝｸﾞ２２はノズル側ﾊﾟｯｷﾝｸﾞなのでサニタリの口径で仕様作成
                P_Siyou = s_SaniSize + P_Siyou;
            }
            break;
        case 8: // ソケット
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            break;
        case 6: // バンブーＢＶ
        case 7: // ブッシュ
        case 9: // リターンパイプ
        case 10: // ゲージコック
        case 12: // フリーＦ
        case 15: // ダイヤフラム弁
        case 16: // スタッドＢＮ
        case 17: // グローブ弁
        case 19: // クランプ継手
        case 23: // クランプ継手
            P_Yobisu = i_wYobi;
            if (s_Code.ToIntDef(0) == 23)
            { // クランプ継手２３はノズル側クランプ継手なのでサニタリの口径で仕様作成
                P_Siyou = s_SaniSize + P_Siyou;
            }
            break;
        case 13: // Ｂ．Ｎ
            P_Yobisu = P_Num * i_wYobi;
            break;
        case 24: // 差圧計配管
            P_Unit = "式";
            if ((P_HEADS_DATA[823].ToIntDef(0) == 2 ||
                P_HEADS_DATA[823].ToIntDef(0) == 3 || P_HEADS_DATA[823].ToIntDef
                (0) == 8) && P_HEADS_DATA[1011].ToIntDef(0) == 10) { // 小型
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
                // 2007.02.15 LX-00追加
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 0) { // LX-00(小型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
                // ********************
                // 2008.04.14 中型・大型変更
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 2 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 20) { // UX-20(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 2 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 30) { // UX-30(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 2 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 40) { // UX-40(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 20) { // LX-20(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 30) { // LX-30(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 40) { // LX-40(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 3 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 50) { // LX-50(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 4 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 40) { // SX-40(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            else if (P_HEADS_DATA[823].ToIntDef(0) == 8 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 30) { // RX-30(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            // 2022.04.08 舶用型式追加_S
            else if (P_HEADS_DATA[823].ToIntDef(0) == 8 && P_HEADS_DATA[1011]
                .TrimRight().ToIntDef(0) == 50) { // RX-50(中型)
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "１．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
            }
            // 2022.04.08 舶用型式追加_E
            else { // 大型
                // P_Siyou = P_Siyou.SubString(1,P_Siyou.Pos("ｍ")-1) + "２" + P_Siyou.SubString(P_Siyou.Pos("ｍ"),P_Siyou.Length());
                P_Siyou = P_Siyou.SubString(1, P_Siyou.Pos("ｍ") - 1) + "２．５" +
                    P_Siyou.SubString(P_Siyou.Pos("ｍ"), P_Siyou.Length());
                // *************************
            }
            break;
        default:
            break;
        }
        if (s_Code.ToIntDef(0) == 1 || s_Code.ToIntDef(0) == 11 ||
            s_Code.ToIntDef(0) == 20) {
            if (Key.ToIntDef(0) >= 1 && Key.ToIntDef(0) <= 5) { // ﾌﾞﾙﾄﾞﾝ
                P_Sikibetu = "PRSG-B";
                // 2004.02.10 ブルドン条件追加
            }
            else if (Key.ToIntDef(0) >= 18 && Key.ToIntDef(0) <= 20) { // ﾌﾞﾙﾄﾞﾝ
                P_Sikibetu = "PRSG-B";
                // ***************************
            }
            else if (Key.ToIntDef(0) >= 6 && Key.ToIntDef(0) <= 13) { // 隔膜
                P_Sikibetu = "PRSG-D";
                // 2007.12.17 隔膜条件追加
            }
            //2020.01.14 SGPﾘﾝｸﾞ端管用圧力計追加変更_S
            //else if (Key.ToIntDef(0) >= 21 && Key.ToIntDef(0) <= 24) { // 隔膜
            //    P_Sikibetu = "PRSG-D";
            //    // ***********************
            //}
            //2020.01.14 SGPﾘﾝｸﾞ端管用圧力計追加変更_E
            else if (Key.ToIntDef(0) == 14) { // ｻﾆﾀﾘｰﾈｼﾞ
                P_Sikibetu = "PRSG-N";
            }
            else if (Key.ToIntDef(0) == 15) { // ｻﾆﾀﾘｰﾍﾙｰﾙ
                P_Sikibetu = "PRSG-H";
            }
            else if (Key.ToIntDef(0) == 16) { // 舶用
                P_Sikibetu = "PRSG-S";
            }
            else if (Key.ToIntDef(0) == 17) { // 舶用差圧
                P_Sikibetu = "PRSG-E";
            }
            //2020.01.14 SGPﾘﾝｸﾞ端管用圧力計追加変更_S
            else if (Key.ToIntDef(0) == 21 || Key.ToIntDef(0) == 22) { // ﾌﾞﾙﾄﾞﾝ
                P_Sikibetu = "PRSG-B";
            }
            //2020.01.14 SGPﾘﾝｸﾞ端管用圧力計追加変更_E
        }

        // 2015.03.23 OGI 圧力計種類による出力仕様変更対応
        AnsiString s1_Prs_Syurui = P_HEADS_DATA[901].TrimRight(); // S1圧力計種類
        AnsiString s2_Prs_Syurui = P_HEADS_DATA[916].TrimRight(); // S2圧力計種類
        AnsiString s3_Prs_Syurui = P_HEADS_DATA[931].TrimRight(); // S3圧力計種類
        AnsiString s4_Prs_Syurui = P_HEADS_DATA[946].TrimRight(); // S4圧力計種類
        if( s1_Prs_Syurui.ToIntDef(0) == 16 &&
            s2_Prs_Syurui.ToIntDef(0) == 16 &&
            s3_Prs_Syurui.ToIntDef(0) == 16 &&
            s4_Prs_Syurui.ToIntDef(0) == 16 ) {
            if( s_Code.ToIntDef(0) == 27 ) {
                // 一針式圧力計
                P_Num = 2;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // バンブーＢＶ
                P_Num = 2;
                // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 ﾘﾝｸﾞ端管変更_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // 圧力計取付台
                // AB両側に一針式圧力計が付く場合は、圧力計取付台の図番、作図用図番がDBと異なる
                P_Zuban  = "E358526";
                P_SZuban = "E358526" ;
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) == 16 &&
            s2_Prs_Syurui.ToIntDef(0) != 16 &&
            s3_Prs_Syurui.ToIntDef(0) == 16 &&
            s4_Prs_Syurui.ToIntDef(0) != 16 ) {
            if( s_Code.ToIntDef(0) == 27 ) {
                // 一針式圧力計
                P_Num = 2;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // バンブーＢＶ
                P_Num = 2;
                // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 ﾘﾝｸﾞ端管変更_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // 圧力計取付台
                // 2017.11.21 片側ﾘﾝｸﾞ端管_S
                 P_Zuban  = "E425782-R2";
                 P_SZuban = "E425782G" ;
                // 2017.11.21 片側ﾘﾝｸﾞ端管_E
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) != 16 &&
            s2_Prs_Syurui.ToIntDef(0) == 16 &&
            s3_Prs_Syurui.ToIntDef(0) != 16 &&
            s4_Prs_Syurui.ToIntDef(0) == 16 ) {
            if( s_Code.ToIntDef(0) == 27 ) {
                // 一針式圧力計
                P_Num = 2;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // バンブーＢＶ
                P_Num = 2;
                // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 ﾘﾝｸﾞ端管変更_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // 圧力計取付台
                // 2017.11.21 片側ﾘﾝｸﾞ端管_S
                 P_Zuban  = "E425782-R2";
                 P_SZuban = "E425782G" ;
                // 2017.11.21 片側ﾘﾝｸﾞ端管_E
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) == 17 &&
            s2_Prs_Syurui.ToIntDef(0) == 17 &&
            s3_Prs_Syurui.ToIntDef(0) == 17 &&
            s4_Prs_Syurui.ToIntDef(0) == 17 ) {
            if( s_Code.ToIntDef(0) == 20 ) {
                // 二針式圧力計
                P_Num = 1;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // バンブーＢＶ
                P_Num = 2;
                // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 ﾘﾝｸﾞ端管変更_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // 圧力計取付台
                // AB両側に一針式圧力計が付く場合は、圧力計取付台の図番、作図用図番がDBと異なる
                P_Zuban  = "E425782-R2";
                P_SZuban = "E425782" ;
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) == 17 &&
            s2_Prs_Syurui.ToIntDef(0) != 17 &&
            s3_Prs_Syurui.ToIntDef(0) == 17 &&
            s4_Prs_Syurui.ToIntDef(0) != 17 ) {
            if( s_Code.ToIntDef(0) == 20 ) {
                // 二針式圧力計
                P_Num = 1;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // バンブーＢＶ
                P_Num = 2;
                // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 ﾘﾝｸﾞ端管変更_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // 圧力計取付台
            }
        }
        else if( s1_Prs_Syurui.ToIntDef(0) != 17 &&
            s2_Prs_Syurui.ToIntDef(0) == 17 &&
            s3_Prs_Syurui.ToIntDef(0) != 17 &&
            s4_Prs_Syurui.ToIntDef(0) == 17 ) {
            if( s_Code.ToIntDef(0) == 20 ) {
                // 二針式圧力計
                P_Num = 1;
            }
            else if( s_Code.ToIntDef(0) == 6 ) {
                // バンブーＢＶ
                P_Num = 2;
                // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                if (s_FrngSize.ToDouble() == 2 || s_FrngSize.ToDouble() == 50 ) {
                    P_Siyou = "PT3/8";
                }
                // 2020.01.14 ﾘﾝｸﾞ端管変更_E
            }
            else if( s_Code.ToIntDef(0) == 21 ) {
                // 圧力計取付台
            }
        }


        // 2012.03.15 直管曲管判断変更
        ////2006.11.22 ＹＸ－８０（曲管）条件追加
        ////if(( b_NZorFR == true && s_Code.ToIntDef(0) == 3 ) ||  // 端管の時は曲管　それ以外は直管
        ////   ( b_NZorFR == false && s_Code.ToIntDef(0) == 2 ) ) {
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if(( s_Text.SubString(1,2) == "YX" && s_Code.ToIntDef(0) == 3 ) ||  // YX-80の時は曲管
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == true && s_Code.ToIntDef(0) == 3 ) ||  // 端管の時は曲管　それ以外は直管                    ){
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == false && s_Code.ToIntDef(0) == 2 ) ) {
        ////*************************************
        ////2006.04.14 ｽﾁ-ﾑ用圧力計変更
        ////端管の時はパイプ＋エルボ　それ以外はソケット
        ////2011.09.28 ＹＸ－８０（パイプ＋エルボ）条件追加
        ////} else if (( b_NZorFR == true && s_Code.ToIntDef(0) == 8 ) ||
        ////           ( b_NZorFR == false && s_Code.ToIntDef(0) == 25 ) ||
        ////           ( b_NZorFR == false && s_Code.ToIntDef(0) == 26 ) ) {
        // } else if (( s_Text.SubString(1,2) == "YX" && s_Code.ToIntDef(0) == 8 ) ||  // YX-80の時はパイプ＋エルボ
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == true && s_Code.ToIntDef(0) == 8 ) ||  //端管の時はパイプ＋エルボ
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == false && s_Code.ToIntDef(0) == 25 ) ||  //端管以外はソケット
        // ( s_Text.SubString(1,2) != "YX" && b_NZorFR == false && s_Code.ToIntDef(0) == 26 ) ) {
        ////***********************************************
        ////***************************
        if (b_STorCU == false && s_Code.ToIntDef(0) == 3) {
            // 曲管2使用(直管3不可)
        }
        else if (b_STorCU == true && s_Code.ToIntDef(0) == 2) {
            // 直管3使用(曲管2不可)
        }
        else if (b_STorCU == false && s_Code.ToIntDef(0) == 8) {
            // パイプ25＋エルボ26使用(ソケット8不可)
        }
        else if (b_STorCU == true && s_Code.ToIntDef(0) == 25) {
            // ソケット8使用(パイプ25＋(エルボ26)不可)
        }
        else if (b_STorCU == true && s_Code.ToIntDef(0) == 26) {
            // ソケット8使用((パイプ25)＋エルボ26不可)
            // ***************************
        }
        else {
            if (((s_Code.ToIntDef(0) == 20) || (s_Code.ToIntDef(0) == 21) ||
                (s_Code.ToIntDef(0) == 24)) && // 差圧計部品は上下で１セット
                ((a_SE == "S3") || (a_SE == "S4"))) {
                // 2008.03.21 圧力計取付台ﾐﾗｰ仕様対応
            }
            else if ((sSpecCode == "B") && (s_Code.ToIntDef(0) == 21) &&
                (a_SE == "S1" && (Key == "16" || Key == "17")) &&
                ((P_HEADS_DATA[916].TrimRight() == "16") ||
                (P_HEADS_DATA[916].TrimRight() == "17"))) {
                // ﾐﾗｰB仕様 ﾉｽﾞﾙ位置S1で圧力計取付台をS1,S2で使用する場合は圧力計取付台をはずす
            }
            else if ((sSpecCode != "B") && (s_Code.ToIntDef(0) == 21) &&
                (a_SE == "S2" && (Key == "16" || Key == "17")) &&
                ((P_HEADS_DATA[901].TrimRight() == "16") ||
                (P_HEADS_DATA[901].TrimRight() == "17"))) {
                // ﾐﾗｰA仕様,ﾐﾗｰ以外 ﾉｽﾞﾙ位置S2で圧力計取付台をS1,S2で使用する場合は圧力計取付台をはずす
                ////2004.01.27 差圧計２個追加
                // } else if ( ( s_Code.ToIntDef(0) == 21 ) && ( a_SE == "S2" && ( Key == "16" || Key == "17" ) ) &&
                // ( ( P_HEADS_DATA[901].TrimRight()  == "16" ) || ( P_HEADS_DATA[901].TrimRight() == "17" ) ) ) {
                ////*************************
                // **********************************
            }
            else {
                Write_PressG_Data(a_SE);
            }
        }
        wkQuery->Next();
    }


    delete wkQuery;
    return s_Code;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 圧力計構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ　検索
// 概  要      ：
// 引  数      ： AnsiString Code  // 検索対象ｺｰﾄﾞ
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Search_PRESS_CODE(AnsiString Code) {
    int i;

    for (i = 0; i < G_PRESSCD_RecCnt; i++) {
        if (G_PRESSCD[i][0] == Code) {

            P_Sikibetu = G_PRESSCD[i][1]; // 識別子(CSVﾌｧｲﾙ作成用)
            P_Katasiki = G_PRESSCD[i][3]; // 部品名称(CSVﾌｧｲﾙ作成用)

            P_Yobihin = G_PRESSCD[i][4]; // 予備品
            break;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 圧力計ﾃﾞｰﾀﾌｧｲﾙ書込み処理
// 概  要      ：
// 引  数      ： AnsiString a_SE   ： 圧力計SE区分(S1～S4,E1～E4)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPressGDetails::Write_PressG_Data(AnsiString a_SE) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;
    // 2017.07.11 斜行流追加
    int iFL;
    // *********************

    AnsiString wk_FilePass;

    // 2017.07.11 斜行流追加
    // 斜行流
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        iFL = 1; // 斜行流
    }
    else {
        iFL = 0; // 台形流
    }
    // *********************

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

    // 2003.12.11 YX-80追加
    // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
    // s_Text = "B_PRSG";
    // else
    // s_Text = "A_PRSG";
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "4"))
            s_Text = "B_PRSG";
        else
            s_Text = "A_PRSG";
    }
    else {
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "3"))
            s_Text = "B_PRSG";
        else
            s_Text = "A_PRSG";
    }
    // *********************

    fputs(s_Text.c_str(), wkfp);

    // 2  識別子
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), wkfp);

    // 3  部品型式
    s_Text = "," + P_Katasiki;
    fputs(s_Text.c_str(), wkfp);

    // 4  部品名称
    s_Text = ",";
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
    s_Text = "," + AnsiString(P_Num);
    fputs(s_Text.c_str(), wkfp);

    // ,9  予備数
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), wkfp);

    // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
    i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
    i_Soukei = P_Num * i_Daisu + P_Yobisu;
    s_Text = "," + AnsiString(i_Soukei);
    fputs(s_Text.c_str(), wkfp);

    // 11 仕様１
    s_Text = "," + P_Siyou;
    fputs(s_Text.c_str(), wkfp);
    // 12 仕様２
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 13 仕様３
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 仕様４
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);

    // 15 仕様５
    s_Text = ",";
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
// 日本語関数名： 圧力計予備品取得処理
// 概  要      ：
// 引  数      ： 圧力計区分(01～17)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
int __fastcall TPressGDetails::Get_Press_YobiSu(int i_TmpCd) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_YobiName;
    int i_YobiSu;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PRSG2YOBI";
    s_SQL = s_SQL + "  WHERE PRSPCD = " + i_TmpCd;

    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        s_YobiName = "";
    }
    else {
        s_YobiName = wkQuery->FieldByName("YOBINAME")->AsString.TrimRight();
        // 予備品識別子

        G_Log_Text = "予備品識別子    ｢" + s_YobiName + "｣を取得。";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    i_YobiSu = Syukei_Buhin_Yobisu(s_YobiName, "0", m_pcHeadsData);
    return i_YobiSu;
}
