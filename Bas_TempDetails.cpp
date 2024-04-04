// ---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：温度計部品展開処理  モジュール
// 作 成 者：T.Kawai
// 作 成 日：2002.08.05
// 更 新 日：2002.09.04
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

#include "Bas_TempDetails.h"

#include "HeadsData.h"
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ﾌﾟﾚｰﾄ部品展開処理 Class
// ---------------------------------------------------------------------------
class TTempDetails {
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
    AnsiString P_Siyou2; // 仕様

    // ↓2007.10.4 ミラー仕様対応
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // ↑2007.10.4 ミラー仕様対応

    bool __fastcall Temp_Data_Sakusei(void); // 温度計ﾃﾞｰﾀ 作成
    bool __fastcall Get_Temp_Data(AnsiString a_SE, AnsiString Syurui);
    // 温度計ﾃﾞｰﾀ取得
    bool __fastcall Search_TEMP_CODE(AnsiString Code); // 温度計
    AnsiString __fastcall Search_HD_TEMP(AnsiString a_SE,
        AnsiString Key); // 温度計

    bool __fastcall Write_Temp_Data(AnsiString a_SE);

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // 温度計部品展開処理 実行
    int __fastcall Get_Temp_YobiSu(int i_TmpCd); // 温度計予備品取得

};

// ---------------------------------------------------------------------------
// 日本語関数名： 温度計部品展開処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TempDetails(HeadsData* pcHeadsData) {
    bool ret;

    TTempDetails tempDetails; // 温度計ﾃﾞｰﾀ 作成Class自動生成,関数終了後破棄

    ret = tempDetails.Excute(pcHeadsData); // 温度計ﾃﾞｰﾀ 作成実行

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 温度計部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADSﾃｷｽﾄﾃﾞｰﾀのﾎﾟｲﾝﾀをｾｯﾄ
    m_pcHeadsData = pcHeadsData;

    // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString型配列に取得
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『温度計部品展開処理』開始     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『温度計部品展開処理』開始     *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  温度計ﾃﾞｰﾀ作成
    // ***********************************
    Temp_Data_Sakusei();

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『温度計部品展開処理』終了     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『温度計部品展開処理』終了     *********");
    Write_Error_Log("*******************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 温度計ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Temp_Data_Sakusei(void) {
    AnsiString s_Tmp_Syurui; // ﾉｽﾞﾙ種類
    // 2003.12.22
    AnsiString s_Text;
    // **********
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
    // 2022.04.21 CX-01D追加_E
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
    // ***               Ｂ側温度計データ　作成                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S1温度計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[898].TrimRight(); // S1温度計種類

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "S1温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("S1", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 追加
    ////***********************************
    ////***  S3温度計ﾃﾞｰﾀ　作成
    ////***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight();   // S3温度計種類
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "S3温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("S3", s_Tmp_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  S4温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight(); // S4温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S4温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S4", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S3温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight(); // S3温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S3温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S3", s_Tmp_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E1温度計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[958].TrimRight(); // E1温度計種類

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "E1温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("E1", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 追加
    ////***********************************
    ////***  E3温度計ﾃﾞｰﾀ　作成
    ////***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight();   // E3温度計種類
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "E3温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("E3", s_Tmp_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  E4温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight(); // E4温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E4温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E4", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E3温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight(); // E3温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E3温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E3", s_Tmp_Syurui);
        }
    }
    // *********************

    // ****************************************************************
    // ***                                                          ***
    // ***               Ａ側温度計データ　作成                     ***
    // ***                                                          ***
    // ****************************************************************

    // ***********************************
    // ***  S2温度計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[913].TrimRight(); // S2温度計種類

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "S2温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("S2", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 追加
    ////***********************************
    ////***  S4温度計ﾃﾞｰﾀ　作成
    ////***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight();   // S4温度計種類
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "S4温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("S4", s_Tmp_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  S3温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[928].TrimRight(); // S3温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S3温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S3", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  S4温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[943].TrimRight(); // S4温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "S4温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("S4", s_Tmp_Syurui);
        }
    }
    // *********************

    // ***********************************
    // ***  E2温度計ﾃﾞｰﾀ　作成
    // ***********************************
    s_Tmp_Syurui = P_HEADS_DATA[973].TrimRight(); // E2温度計種類

    if (s_Tmp_Syurui == "00") {
        G_Log_Text = "E2温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        Get_Temp_Data("E2", s_Tmp_Syurui);
    }

    // 2003.12.24 YX-80 追加
    // ***********************************
    // ***  E4温度計ﾃﾞｰﾀ　作成
    // ***********************************
    // s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight();  // E4温度計種類
    //
    // if (s_Tmp_Syurui == "00"){
    // G_Log_Text = "E4温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
    // Write_Log(G_Log_Text);
    // }
    // else
    // {
    // Get_Temp_Data("E4", s_Tmp_Syurui);
    // }
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
        // 斜行流
    // *********************
        // ***********************************
        // ***  E3温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[988].TrimRight(); // E3温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E3温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E3", s_Tmp_Syurui);
        }
    }
    else {
        // ***********************************
        // ***  E4温度計ﾃﾞｰﾀ　作成
        // ***********************************
        s_Tmp_Syurui = P_HEADS_DATA[1003].TrimRight(); // E4温度計種類

        if (s_Tmp_Syurui == "00") {
            G_Log_Text = "E4温度計ﾃﾞｰﾀ  温度計種類が｢00｣の為作成しません。";
            Write_Log(G_Log_Text);
        }
        else {
            Get_Temp_Data("E4", s_Tmp_Syurui);
        }
    }
    // *********************

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 温度計部品展開処理 作成実行
// 概  要      ：
// 引  数      ： AnsiString a_SE   ： 温度計SE区分(S1～S4,E1～E4)
// ： AnsiString Syurui ： 温度計種類
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Get_Temp_Data(AnsiString a_SE, AnsiString Syurui)
{
    AnsiString s_Code; // 部品ｺｰﾄﾞ

    // *********************************************
    // ***  温度計部品構成ﾏｽﾀ(HD_TEMP) 検索
    // *********************************************
    s_Code = Search_HD_TEMP(a_SE, Syurui);

    if (s_Code == "\0") {
        G_ErrLog_Text = Syurui + "ﾃﾞｰﾀ作成時　" + a_SE + "温度計種類 ｢" + Syurui +
            "｣ は、温度計部品構成ﾏｽﾀ(HD_TEMP)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);

        return false;
    }

    return true;

}

// ---------------------------------------------------------------------------
// 日本語関数名： HD_TEMPﾃｰﾌﾞﾙ検索処理
// 概  要      ：
// 引  数      ： AnsiString a_SE   ： 温度計SE区分(S1～S4,E1～E4)
// ： AnsiString Key    ： 温度計種類
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall TTempDetails::Search_HD_TEMP(AnsiString a_SE,
    AnsiString Key) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_Code;
    AnsiString s_FrngSize;
    AnsiString s_BaseMatM; // 金属系基準材質
    AnsiString s_BaseMatL; // 非金属系基準材質
    AnsiString s_Memori;
    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
    AnsiString s_PressGCode; // 圧力計種類コード
    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
    // 2003.12.24
    AnsiString s_Text;
    // **********
    bool bFlgANSI; // ＡＮＳＩ？
    int i_wYobi;
    int i_Kikaku;
    // 2017.07.11 斜行流追加
    int iFL;
    // *********************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_TEMP";
    s_SQL = s_SQL + "  WHERE TEMP_TYPE = '" + Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    // 各項目の初期化
    s_Code = "";
    P_Zuban = "";
    P_SZuban = "";
    P_Num = 0;
    P_Zaisitu = "";
    P_Siyou = "";
    P_Siyou2 = "";

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
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1136].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4); // 端管が付く場合はBTANMを使う
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1136].TrimRight()) != "-") {
            // 端管が付く場合はBTANMを使う
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[893].SubString(2,1) == "4") {         // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[893].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                // ***********************
                s_BaseMatM = P_HEADS_DATA[894].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[893].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[894].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[893].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[894].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[899].TrimRight() + "/" +
            P_HEADS_DATA[900].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[892].TrimRight();
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[893]);
        // if(P_HEADS_DATA[893].SubString(1,1) == "4" || P_HEADS_DATA[893].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 ﾘﾝｸﾞ端管変更_S
        s_PressGCode = P_HEADS_DATA[901].TrimRight(); // 圧力計種類コード
        // 2020.01.14 ﾘﾝｸﾞ端管変更_E
    }
    else if (a_SE == "S2") {
        //// 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1137].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4); // 端管が付く場合はATANMを使う
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1137].TrimRight()) != "-") {
            // 端管が付く場合はATANMを使う
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[908].SubString(2,1) == "4") {         // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[908].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                // ***********************
                s_BaseMatM = P_HEADS_DATA[909].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[908].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[909].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[908].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[909].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[914].TrimRight() + "/" +
            P_HEADS_DATA[915].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[907].TrimRight();
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[908]);
        // if(P_HEADS_DATA[908].SubString(1,1) == "4" || P_HEADS_DATA[908].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 ﾘﾝｸﾞ端管変更_S
        s_PressGCode = P_HEADS_DATA[916].TrimRight(); // 圧力計種類コード
        // 2020.01.14 ﾘﾝｸﾞ端管変更_E
    }
    else if (a_SE == "S3") {
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 追加
        //    // s_BaseMatM = P_HEADS_DATA[1060].SubString(1,4);       // 端管が付く場合はBTANMを使う
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // 端管が付く場合はATANMを使う
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // 端管が付く場合はBTANMを使う
        //    }
        //    // *********************
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1138].TrimRight()) != "-") {
            // 2017.07.11 斜行流追加
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
                // 斜行流
            // *********************
                // 端管が付く場合はATANMを使う
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
            else {
                // 端管が付く場合はBTANMを使う
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() == "1160") {
        // *********************
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() != "-") {
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // *********************
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2003.12.24
            // if(P_HEADS_DATA[993].SubString(2,1) == "4" ) {        // フランジ付きノズルの場合は、ノズル材質を使う
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[923].SubString(2,1) == "4" ) {        // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[923].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                // ***********************
                // **********
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[923].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[923].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[924].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[929].TrimRight() + "/" +
            P_HEADS_DATA[930].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[922].TrimRight();
        // 2003.12.24 YX-80 追加
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // i_Kikaku = GetNzKikaku( P_HEADS_DATA[993] );
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[923]);
        // *********************
        // if(P_HEADS_DATA[993].SubString(1,1) == "4" || P_HEADS_DATA[993].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 ﾘﾝｸﾞ端管変更_S
        if (iFL == 1){
            //S2 斜行流
            s_PressGCode = P_HEADS_DATA[916].TrimRight(); // 圧力計種類コード
        }
        else {
            //S1
            s_PressGCode = P_HEADS_DATA[901].TrimRight(); // 圧力計種類コード
        }
        // 2020.01.14 ﾘﾝｸﾞ端管変更_E
    }
    else if (a_SE == "S4") {
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 追加
        //    // s_BaseMatM = P_HEADS_DATA[1061].SubString(1,4);       // 端管が付く場合はATANMを使う
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // 端管が付く場合はBTANMを使う
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // 端管が付く場合はATANMを使う
        //    }
        //    // *********************
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1139].TrimRight()) != "-") {
            s_Text = P_HEADS_DATA[34].TrimRight();
            // 2017.07.11 斜行流追加
            if (iFL == 1){
            //if (s_Text.SubString(1, 2) == "YX") {
                // 斜行流
            // *********************
                // 端管が付く場合はBTANMを使う
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
            else {
                // 端管が付く場合はATANMを使う
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() == "1160") {
        // ***********************
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() != "-") {
        // ***********************
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[938].SubString(2,1) == "4" ) {        // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[938].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[938].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[938].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[939].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[944].TrimRight() + "/" +
            P_HEADS_DATA[945].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[937].TrimRight();
        // 2003.12.24 YX-80 追加
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[938]);
        // if(P_HEADS_DATA[938].SubString(1,1) == "4" || P_HEADS_DATA[938].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
        // 2020.01.14 ﾘﾝｸﾞ端管変更_S
        if (iFL == 1){
            //S1 斜行流
            s_PressGCode = P_HEADS_DATA[901].TrimRight(); // 圧力計種類コード
        }
        else {
            //S2
            s_PressGCode = P_HEADS_DATA[916].TrimRight(); // 圧力計種類コード
        }
        // 2020.01.14 ﾘﾝｸﾞ端管変更_E
    }
    else if (a_SE == "E1") {
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1140].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4); // 端管が付く場合はBTANMを使う
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1140].TrimRight()) != "-") {
            // 端管が付く場合はBTANMを使う
            s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[953].SubString(2,1) == "4" ) {        // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[953].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                // ***********************
                s_BaseMatM = P_HEADS_DATA[954].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[953].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[954].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[953].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[954].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[959].TrimRight() + "/" +
            P_HEADS_DATA[960].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[952].TrimRight();
        s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[953]);
        // if(P_HEADS_DATA[953].SubString(1,1) == "4" || P_HEADS_DATA[953].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    else if (a_SE == "E2") {
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1141].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4); // 端管が付く場合はATANMを使う
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1141].TrimRight()) != "-") {
            // 端管が付く場合はATANMを使う
            s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[968].SubString(2,1) == "4" ) {        // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[968].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                // ***********************
                s_BaseMatM = P_HEADS_DATA[969].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[968].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[969].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[968].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[969].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[974].TrimRight() + "/" +
            P_HEADS_DATA[975].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[967].TrimRight();
        s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[968]);
        // if(P_HEADS_DATA[968].SubString(1,1) == "4" || P_HEADS_DATA[968].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    else if (a_SE == "E3") {
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 追加
        //    // s_BaseMatM = P_HEADS_DATA[1060].SubString(1,4);       // 端管が付く場合はBTANMを使う
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // 端管が付く場合はATANMを使う
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // 端管が付く場合はBTANMを使う
        //    }
        //    // *********************
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1142].TrimRight()) != "-") {
            // 2017.07.11 斜行流追加
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
                // 斜行流
            // *********************
                // 端管が付く場合はATANMを使う
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
            else {
                // 端管が付く場合はBTANMを使う
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() == "1160") {
        // ***********************
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1358].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1358].TrimRight() != "-") {
        // ***********************
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1357].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1357].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[983].SubString(2,1) == "4" ) {        // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[983].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                // ***********************
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[983].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[983].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[984].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[989].TrimRight() + "/" +
            P_HEADS_DATA[990].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[982].TrimRight();
        // 2003.12.24 YX-80 追加
        // s_BaseMatL =  P_HEADS_DATA[835].TrimRight();
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        // *********************
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[983]);
        // if(P_HEADS_DATA[983].SubString(1,1) == "4" || P_HEADS_DATA[983].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    else if (a_SE == "E4") {
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {
        //    // 2003.12.24 YX-80 追加
        //    // s_BaseMatM = P_HEADS_DATA[1061].SubString(1,4);       // 端管が付く場合はATANMを使う
        //    s_Text = P_HEADS_DATA[34].TrimRight();
        //    if (s_Text.SubString(1, 2) == "YX") {
        //        s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
        //        // 端管が付く場合はBTANMを使う
        //    }
        //    else {
        //        s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
        //        // 端管が付く場合はATANMを使う
        //    }
        //    // *********************
        //}
        //// 2014.12.19 材質取得先追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].ToIntDef(0) != 0) {
        //    s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) == 1160) {
        //    // 温度計保護管がSUS403の場合はSUS316を使う
        //    s_BaseMatM = "1003";
        //}
        //else if (P_HEADS_DATA[1358].ToIntDef(0) != 0) {
        //   // 温度計保護管が付くの場合はその材質を使う
        //    s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        //}
        ////**************************
        if (Type_MAT(P_HEADS_DATA[1143].TrimRight()) != "-") {
            // 2017.07.11 斜行流追加
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
                // 斜行流
            // *********************
                // 端管が付く場合はBTANMを使う
                s_BaseMatM = P_HEADS_DATA[1060].SubString(1, 4);
            }
            else {
                // 端管が付く場合はATANMを使う
                s_BaseMatM = P_HEADS_DATA[1061].SubString(1, 4);
            }
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() == "1160") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() == "1160") {
        // ***********************
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        // 2017.07.11 斜行流追加
        //else if (P_HEADS_DATA[34].SubString(1, 2) == "YX" &&
        //         P_HEADS_DATA[1357].TrimRight() != "-") {
        else if (iFL == 1 && P_HEADS_DATA[1357].TrimRight() != "-") {
        // ***********************
            s_BaseMatM = P_HEADS_DATA[1357].SubString(1, 4);
        }
        else if (P_HEADS_DATA[1358].TrimRight() == "1160") {
            // 温度計保護管がSUS403の場合はSUS316を使う
            s_BaseMatM = "1003";
        }
        else if (Type_MAT(P_HEADS_DATA[1358].TrimRight()) != "-") {
           // 温度計保護管が付くの場合はその材質を使う
            s_BaseMatM = P_HEADS_DATA[1358].SubString(1, 4);
        }
        // ***********************
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if(P_HEADS_DATA[998].SubString(2,1) == "4" ) {        // フランジ付きノズルの場合は、ノズル材質を使う
            if (P_HEADS_DATA[998].SubString(3, 1) == "4")
            { // フランジ付きノズルの場合は、ノズル材質を使う
                // ***********************
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2014.06.24 ﾒﾀﾙﾌﾞｰﾂﾁｭｰﾌﾞﾌﾗﾝｼﾞ追加
            }
            else if (P_HEADS_DATA[998].SubString(3, 1) == "2")
            { // チューブフランジの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // 2018.03.26 温度計座材質変更_S
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
                // 2018.03.26 温度計座材質変更_E
            }
            else if (P_HEADS_DATA[998].SubString(3, 1) == "3")
            { // メタルブーツの場合は、ノズル材質を使う
                s_BaseMatM = P_HEADS_DATA[999].TrimRight();
                // ********************************
            }
            else { // ブーツノズルの場合は、プレート材質を使う
                s_BaseMatM = P_HEADS_DATA[43].TrimRight();
                // 2024.02.06 ALLOY材質変換_S
                s_BaseMatM = Alloy_MAT(s_BaseMatM); // ALLOY材→JIS材
                // 2024.02.06 ALLOY材質変換_E
            }
        }
        s_Memori = P_HEADS_DATA[1004].TrimRight() + "/" +
            P_HEADS_DATA[1005].TrimRight() + "℃";
        s_FrngSize = P_HEADS_DATA[997].TrimRight();
        // 2003.12.24 YX-80 追加
        // s_BaseMatL =  P_HEADS_DATA[836].TrimRight();
        // 2017.07.11 斜行流追加
        if (iFL == 1){
        //s_Text = P_HEADS_DATA[34].TrimRight();
        //if (s_Text.SubString(1, 2) == "YX") {
        // *********************
            s_BaseMatL = P_HEADS_DATA[835].TrimRight();
        }
        else {
            s_BaseMatL = P_HEADS_DATA[836].TrimRight();
        }
        // *********************
        i_Kikaku = GetNzKikaku(P_HEADS_DATA[998]);
        // if(P_HEADS_DATA[998].SubString(1,1) == "4" || P_HEADS_DATA[998].SubString(1,1) == "5") {
        // bFlgANSI = true;
        // } else {
        // bFlgANSI = false;
        // }
    }
    if (i_Kikaku != 2)
        s_FrngSize = FormatFloat("0", StrToDblDef(s_FrngSize.TrimRight(), 0.0));

    // i_wYobi = Get_Temp_YobiSu(Key.ToIntDef(0));
    i_wYobi = 0;

    while (!wkQuery->Eof) {
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // 部品ｺｰﾄﾞ
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // 作図用図番
        P_Num = wkQuery->FieldByName("NUM")->AsCurrency; // 数量
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // 材質名称
        P_EZaisitu = P_Zaisitu;
        P_Siyou = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight
            (); // 仕様

        // *********************************
        // ***  温度計構成部品ﾃｰﾌﾞﾙ検索
        // *********************************
        Search_TEMP_CODE(s_Code);

        // *********************************
        // ***  予備品数 取得(02.08.05追加)
        // *********************************
        // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0");

        P_Yobisu = 0;
        switch (s_Code.ToIntDef(0)) {
        case 1: // ソケット
            // if (bFlgANSI == true ) {
            if (i_Kikaku == 1) { // ANSI
                P_Siyou2 = FormatFloat("0", s_FrngSize.ToDouble() * 25) + "A";
            }
            else if (i_Kikaku == 2) { // ｻﾆﾀﾘ
                P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            else {
                P_Siyou2 = s_FrngSize + "A";
            }
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            break;
        case 2: // 保護管
            // 2009.02.16 温度計ｻｲｽﾞ追加
            // if ( Key == "02" ) {
            // P_Siyou2 = "25A";
            // P_Zaisitu = "ＳＵＳ４１０";
            // } else {
            ////            if (bFlgANSI == true ) {
            // if (i_Kikaku == 1 ) {         // ANSI
            // P_Siyou2 = FormatFloat("0",s_FrngSize.ToDouble() * 25) + "A";
            // } else if ( i_Kikaku == 2 ) { // ｻﾆﾀﾘ
            // P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            // } else {
            // P_Siyou2 = s_FrngSize + "A";
            // }
            ////            P_Zaisitu = Chg_Zaisitu_Code( s_BaseMatM, 1);
            // // 保護管の材質は、HEADSデータから取得するよう変更　2002.12.26
            // //2003.12.24 YX-80 追加
            // //if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
            // //   P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // //else
            // //   P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // s_Text = P_HEADS_DATA[34].TrimRight();
            // if (s_Text.SubString(1,2) == "YX"){
            // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "4" ) )
            // P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // else
            // P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // } else {
            // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
            // P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // else
            // P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // }
            // //*********************
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // }
            if (Key == "01" || Key == "05" || Key == "06") {
                if (i_Kikaku == 1) { // ANSI
                    if (s_FrngSize.ToDouble() <= 1) {
                        P_Siyou2 = "56L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 2) {
                        P_Siyou2 = "71L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 4) {
                        P_Siyou2 = "101L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 14) {
                        P_Siyou2 = "131L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 =
                        P_Siyou2 + FormatFloat("0",
                    s_FrngSize.ToDouble() * 25) + "A";
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                    if (s_FrngSize.ToDouble() == 2) {
                        // 2020.06.03 温度計変更_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 温度計変更_E
                            P_Siyou2 = "101L 100A";
                        }
                    }
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
                }
                else {
                    if (s_FrngSize.ToDouble() <= 25) {
                        P_Siyou2 = "56L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 50) {
                        P_Siyou2 = "71L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 100) {
                        P_Siyou2 = "101L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 350) {
                        P_Siyou2 = "131L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                    if (s_FrngSize.ToDouble() == 50) {
                        // 2020.06.03 温度計変更_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 温度計変更_E
                            P_Siyou2 = "101L 100A";
                        }
                    }
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
                }
                // 2012.01.20 舶用ｽﾁｰﾑ温度計追加
                // } else if ( Key == "02" ) {
            }
            else if (Key == "02" || Key == "10" || Key == "11") {
                // *****************************
                P_Siyou2 = "56L 25A";
            }
            else if (Key == "03") {
                if (s_FrngSize.ToDouble() <= 2.5) {
                    P_Siyou2 = "65L ";
                }
                else if (s_FrngSize.ToDouble() <= 4) {
                    P_Siyou2 = "105L ";
                }
                else {
                    P_Siyou2 = "";
                }
                P_Siyou2 = P_Siyou2 + ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            else if (Key == "04") {
                P_Siyou2 = "65L 1 1/2S";
            }
            else {
                if (i_Kikaku == 1) { // ANSI
                    P_Siyou2 =
                        FormatFloat("0", s_FrngSize.ToDouble() * 25) + "A";
                }
                else if (i_Kikaku == 2) { // ｻﾆﾀﾘ
                    P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
                }
                else {
                    P_Siyou2 = s_FrngSize + "A";
                }
            }
            // 2012.01.20 舶用ｽﾁｰﾑ温度計追加
            // if ( Key == "02" || Key == "04" ) {
            // 2014.06.24 保護管材質変更
            // if ( Key == "02" || Key == "04" || Key == "10" || Key == "11" ) {
            //// *****************************
            // P_Zaisitu = "ＳＵＳ４１０";
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // } else {
            // *************************
            // 保護管の材質は、HEADSデータから取得する
            // 2017.07.11 斜行流追加
            if (iFL == 1){
            //s_Text = P_HEADS_DATA[34].TrimRight();
            //if (s_Text.SubString(1, 2) == "YX") {
            // *********************
                if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2,
                    1) == "4"))
                    P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
                else
                    P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            }
            else {
                if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2,
                    1) == "3"))
                    P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
                else
                    P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            }
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // 2014.06.24 保護管材質変更
            // }
            // *************************
            P_Yobisu = i_wYobi;
            break;
        case 5: // 座
            // 2009.02.16 温度計ｻｲｽﾞ追加
            if (i_Kikaku == 1) { // ANSI
                P_Siyou2 = FormatFloat("0", s_FrngSize.ToDouble() * 25) + "A";
            }
            else if (i_Kikaku == 2) { // ｻﾆﾀﾘ
                P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            else {
                P_Siyou2 = s_FrngSize + "A";
            }
            // *************************
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatM, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 3: // 温度計(大)
            P_Siyou = s_Memori;
            // 2009.02.16 温度計ｻｲｽﾞ追加
            // if ( Key == "02" ) {
            // P_Siyou2 = "25A";
            // } else {
            ////            if (bFlgANSI == true ) {
            // if ( i_Kikaku == 1 ) {              // ANSI
            // P_Siyou2 = FormatFloat("0",s_FrngSize.ToDouble() * 25) + "A";
            // } else {
            // P_Siyou2 = s_FrngSize + "A";
            // }
            // }
            // 2013/06/28 温度計寸法訂正
            if (Key == "02") {
                // P_Siyou2 = "215L 25A";
                P_Siyou2 = "40L 25A";
            }
            else {
                if (i_Kikaku == 1) { // ANSI
                    if (s_FrngSize.ToDouble() <= 1) {
                        // P_Siyou2 = "215L ";
                        P_Siyou2 = "40L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 2) {
                        // P_Siyou2 = "235L ";
                        P_Siyou2 = "60L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 4) {
                        // P_Siyou2 = "260L ";
                        P_Siyou2 = "90L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 14) {
                        // P_Siyou2 = "295L ";
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 =
                        P_Siyou2 + FormatFloat("0",
                    s_FrngSize.ToDouble() * 25) + "A";
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                    if (s_FrngSize.ToDouble() == 2) {
                        // 2020.06.03 温度計変更_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 温度計変更_E
                            P_Siyou2 = "90L 100A";
                        }
                    }
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
                }
                else {
                    if (s_FrngSize.ToDouble() <= 25) {
                        // P_Siyou2 = "215L ";
                        P_Siyou2 = "40L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 50) {
                        // P_Siyou2 = "235L ";
                        P_Siyou2 = "60L ";
                        // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                        // 2020.06.03 温度計変更_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 温度計変更_E
                            P_Siyou2 = "90L 100A";
                        }
                        // 2020.01.14 ﾘﾝｸﾞ端管変更_E
                    }
                    else if (s_FrngSize.ToDouble() <= 100) {
                        // P_Siyou2 = "260L ";
                        P_Siyou2 = "90L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 350) {
                        // P_Siyou2 = "295L ";
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                    if (s_FrngSize.ToDouble() == 50) {
                        // 2020.06.03 温度計変更_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 温度計変更_E
                            P_Siyou2 = "90L 100A";
                        }
                    }
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
                }
            }
            // **************************
            P_Yobisu = i_wYobi;
            break;
        case 6: // 温度計(小)
            P_Siyou = s_Memori;
            // 2009.02.16 温度計ｻｲｽﾞ追加
            // P_Siyou2 = ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            if (Key == "04") {
                P_Siyou2 = "170L 1 1/2S";
            }
            else {
                if (s_FrngSize.ToDouble() <= 2.5) {
                    P_Siyou2 = "170L ";
                }
                else if (s_FrngSize.ToDouble() <= 4) {
                    P_Siyou2 = "210L ";
                }
                else {
                    P_Siyou2 = "";
                }
                P_Siyou2 = P_Siyou2 + ChgIDFSize(s_FrngSize.ToDouble()) + "S";
            }
            // *************************
            P_Yobisu = i_wYobi;
            break;
        case 12: // ダイヤル式温度計
        case 13: // 温度計
        case 14: // I型温度計
            P_Siyou = P_Siyou + s_Memori;
            P_Yobisu = i_wYobi;
            break;
        case 4: // ケース(大)
        case 7: // ケース(小)
            P_Siyou2 = "";
            P_Yobisu = i_wYobi;
        case 8: // 袋ナット
            break;
        case 9: // パッキング
            P_Zaisitu = Chg_Zaisitu_Code(s_BaseMatL, 1);
            P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            P_Yobisu = i_wYobi;
            break;
        case 10: // I型舶用温度計
        case 11: // L型舶用温度計
            P_Siyou = P_Siyou + s_Memori;
            // 2012.01.20 舶用ｽﾁｰﾑ温度計追加
            ////2009.02.16 温度計ｻｲｽﾞ追加
            // if (i_Kikaku == 1 ) {         // ANSI
            // if (s_FrngSize.ToDouble() <= 2) {
            // P_Siyou2 = "60L ";
            // } else if (s_FrngSize.ToDouble() <= 4) {
            // P_Siyou2 = "85L ";
            // } else if (s_FrngSize.ToDouble() <= 14) {
            // P_Siyou2 = "120L ";
            // } else {
            // P_Siyou2 = "";
            // }
            // P_Siyou2 = P_Siyou2 + FormatFloat("0",s_FrngSize.ToDouble() * 25) + "A";
            // } else {
            // if (s_FrngSize.ToDouble() <= 50) {
            // P_Siyou2 = "60L ";
            // } else if (s_FrngSize.ToDouble() <= 100) {
            // P_Siyou2 = "85L ";
            // } else if (s_FrngSize.ToDouble() <= 350) {
            // P_Siyou2 = "120L ";
            // } else {
            // P_Siyou2 = "";
            // }
            // P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
            // }
            // *************************
            if (Key == "10" || Key == "11") {
                P_Siyou2 = "40L 25A";
            }
            else {
                if (i_Kikaku == 1) { // ANSI
                    if (s_FrngSize.ToDouble() <= 2) {
                        P_Siyou2 = "60L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 4) {
                        P_Siyou2 = "85L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 14) {
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 =
                        P_Siyou2 + FormatFloat("0",
                    s_FrngSize.ToDouble() * 25) + "A";
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                    if (s_FrngSize.ToDouble() == 2) {
                        // 2020.06.03 温度計変更_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 温度計変更_E
                            P_Siyou2 = "85L 100A";
                        }
                    }
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
                }
                else {
                    if (s_FrngSize.ToDouble() <= 50) {
                        P_Siyou2 = "60L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 100) {
                        P_Siyou2 = "85L ";
                    }
                    else if (s_FrngSize.ToDouble() <= 350) {
                        P_Siyou2 = "120L ";
                    }
                    else {
                        P_Siyou2 = "";
                    }
                    P_Siyou2 = P_Siyou2 + s_FrngSize + "A";
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_S
                    if (s_FrngSize.ToDouble() == 50) {
                        // 2020.06.03 温度計変更_S
                        //if (s_PressGCode == "16" || s_PressGCode == "17") {
                        if ( s_PressGCode == "17") {
                        // 2020.06.03 温度計変更_E
                            P_Siyou2 = "85L 100A";
                        }
                    }
                    // 2020.01.14 ﾘﾝｸﾞ端管変更_E
                }
            }
            // *****************************
            // P_Zaisitu = Chg_Zaisitu_Code( s_BaseMatM, 2);
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // 2009.02.16 温度計材質削除
            // if( ( a_SE.SubString(2,1) == "1" )||( a_SE.SubString(2,1) == "3" ) )
            // P_Zaisitu = P_HEADS_DATA[1357].TrimRight();
            // else
            // P_Zaisitu = P_HEADS_DATA[1358].TrimRight();
            // P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            // P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            // *************************
            P_Yobisu = i_wYobi;
            break;
        default:
            break;
        }

        Write_Temp_Data(a_SE);
        wkQuery->Next();
    }

    delete wkQuery;
    return s_Code;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 温度計構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ　検索
// 概  要      ：
// 引  数      ： AnsiString Code  // 検索対象ｺｰﾄﾞ
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Search_TEMP_CODE(AnsiString Code) {
    int i;

    for (i = 0; i < G_TEMPCD_RecCnt; i++) {
        if (G_TEMPCD[i][0] == Code) {

            P_Sikibetu = G_TEMPCD[i][1]; // 識別子(CSVﾌｧｲﾙ作成用)
            P_Katasiki = G_TEMPCD[i][3]; // 部品名称(CSVﾌｧｲﾙ作成用)

            P_Yobihin = G_TEMPCD[i][4]; // 予備品
            break;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 温度計ﾃﾞｰﾀﾌｧｲﾙ書込み処理
// 概  要      ：
// 引  数      ： AnsiString a_SE   ： 温度計SE区分(S1～S4,E1～E4)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TTempDetails::Write_Temp_Data(AnsiString a_SE) {
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
    // s_Text = "B_TEMP";
    // else
    // s_Text = "A_TEMP";
    // 2017.07.11 斜行流追加
    if (iFL == 1){
    //s_Text = P_HEADS_DATA[34].TrimRight();
    //if (s_Text.SubString(1, 2) == "YX") {
    // *********************
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "4"))
            s_Text = "B_TEMP";
        else
            s_Text = "A_TEMP";
    }
    else {
        if ((a_SE.SubString(2, 1) == "1") || (a_SE.SubString(2, 1) == "3"))
            s_Text = "B_TEMP";
        else
            s_Text = "A_TEMP";
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
    s_Text = "," + P_Siyou;
    fputs(s_Text.c_str(), wkfp);
    // 12 仕様２
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);

    // 13 仕様３,４,５
    s_Text = ",,,";
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
// 日本語関数名： 温度計予備品取得処理
// 概  要      ：
// 引  数      ： 温度計区分(01～14)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
int __fastcall TTempDetails::Get_Temp_YobiSu(int i_TmpCd) {
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
    s_SQL = s_SQL + "SELECT * FROM HD_TMP2YOBI";
    s_SQL = s_SQL + "  WHERE TMPPCD = " + i_TmpCd;

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
