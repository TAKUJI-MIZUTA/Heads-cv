// ---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：付属品部品展開処理  モジュール
// 作 成 者：T.Kawai
// 作 成 日：2002.08.17
// 更 新 日：2002.09.10
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

#include "Bas_AxsrsDetails.h"

#include "HeadsData.h"

#include <math.h>       //2006.02.06 切り上げ

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// 付属品部品展開処理 Class
// ---------------------------------------------------------------------------
class TAxsrsDetails {
private:
    FILE *fp;

    // CSVﾌｧｲﾙから取得
    AnsiString P_Sikibetu; // 識別子
    AnsiString P_Katasiki; // 部品型式
    AnsiString P_Yobihin; // 予備品名
    AnsiString P_Hosoku; // 名称補足

    // ﾏｽﾀから取得
    AnsiString P_Zaisitu; // 材質名称
    AnsiString P_EZaisitu; // 材質名称
    AnsiString P_Zuban; // 図番
    AnsiString P_SZuban; // 作図用図番
    int P_Num; // 数量
    int P_Yobisu; // 予備数
    AnsiString P_Siyou; // 仕様
    AnsiString P_Siyou2; // 仕様2
    AnsiString P_Siyou5; // 仕様5
    AnsiString P_Unit; // 単位（式）

    // 2020.12.16 シートガスケット名称変更_S
    AnsiString P_GasNameB;  // Ｂシートガスケット名称
    AnsiString P_GasNameA;  // Ａシートガスケット名称
    // 2020.12.16 シートガスケット名称変更_E

    // ------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // 2007.10.03
    // ------------------------------

    bool __fastcall Axsrs_Data_Sakusei(void); // 付属品ﾃﾞｰﾀ 作成
    bool __fastcall Get_Drpn_Data(AnsiString); // ﾄﾞﾚﾝﾊﾟﾝﾃﾞｰﾀ取得
    bool __fastcall Get_Anch_Data(void); // ｱﾝｶｰﾃﾞｰﾀ取得
    bool __fastcall Get_Spana_Data(void); // ｽﾊﾟﾅﾃﾞｰﾀ取得
    // 2014/08/22 応用技術編集
    bool __fastcall Get_Spana_Data(AnsiString BoltSize); // ｽﾊﾟﾅﾃﾞｰﾀ取得 (ボルトサイズを指定)
    bool __fastcall Get_HMark_Data(void); // 日阪マーク取得
    bool __fastcall Get_Meiban_Data(void); // 銘板取得
    // 2012.10.15 ボルト落下注意ステッカー追加
    bool __fastcall Get_Rakka_Data(void); // ボルト落下注意ステッカー
    // ***************************************
    bool __fastcall Get_Fune_Data(bool); // 舶用付属品ﾃﾞｰﾀ取得
    // 2004.02.24
    bool __fastcall Get_BFRG_Data(void); // ﾌﾞﾗｲﾝﾄﾞﾌﾗﾝｼﾞﾃﾞｰﾀ取得
    // **********
    // 2020.04.23  舶用以外ｽﾄﾚｰﾅ_S　
    bool __fastcall Get_Strainer_Data(void); // ｽﾄﾚｰﾅﾃﾞｰﾀ取得
    // 2020.04.23  舶用以外ｽﾄﾚｰﾅ_E
    // 2014.12.22 出荷用閉止フランジ追加
    bool __fastcall Get_TFLG_Data(void); // 出荷用閉止ﾌﾗﾝｼﾞﾃﾞｰﾀ取得
    bool __fastcall TAxsrsDetails::Search_HD_TEMPORARY(AnsiString Tugite,
        AnsiString Kokei, AnsiString *s_DNO, AnsiString *s_SPEC1,
        AnsiString *s_SPEC2); // 出荷用閉止ﾌﾗﾝｼﾞをﾏｽﾀから取得
    int __fastcall TAxsrsDetails::Get_TMGASKET(AnsiString Tugite,
        AnsiString Syurui, int Fl); // 出荷用閉止ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ有無
    // *********************************
    // AnsiString __fastcall Search_HD_MATERIAL(AnsiString Key);            // 材質ｺｰﾄﾞ検索
    bool __fastcall Write_Funeyou_Data(AnsiString s_Code, AnsiString s_Spec);
    bool __fastcall Write_Axsrs_Data(AnsiString a_SE);
    bool __fastcall Get_IFrng_Data(void);
    // 2004.02.24
    bool __fastcall Get_STGkt_Data(void);
    // **********
    bool __fastcall EtcData_Write(void);
    bool __fastcall Write_Err_Data(void);
    bool __fastcall Get_IFrgBltSize(AnsiString sKikaku, AnsiString *s_retSize,
        int *iBltNum);
    bool __fastcall Get_SSTDBltSpec(AnsiString sKatasiki, int iFrgDiv,
        AnsiString *sSBLTSpec);

    // 2015.03.19 OGI 舶用部品アキュームレータ出力対応
    bool __fastcall Search_HD_FRM_NZ_Accum(AnsiString Key, AnsiString Type, int Code);

    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    AnsiString __fastcall Get_SheetGasketMat(AnsiString s_Side);
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E

    // 2020.12.16 シートガスケット名称変更_S
    bool __fastcall Get_SeatGas_Name(void); // シートガスケット名称取得
    // 2020.12.16 シートガスケット名称変更_E

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // 付属品部品展開処理 実行
};

// ---------------------------------------------------------------------------
// 日本語関数名： 付属品部品展開処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall AxsrsDetails(HeadsData* pcHeadsData) {
    bool ret;

    TAxsrsDetails axsrsDetails; // 付属品ﾃﾞｰﾀ 作成Class自動生成,関数終了後破棄

    ret = axsrsDetails.Excute(pcHeadsData); // 付属品ﾃﾞｰﾀ 作成実行

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 付属品部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADSﾃｷｽﾄﾃﾞｰﾀのﾎﾟｲﾝﾀをｾｯﾄ
    m_pcHeadsData = pcHeadsData;

    // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString配列に取得
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『付属品部品展開処理』開始     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『付属品部品展開処理』開始     *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  付属品ﾃﾞｰﾀ作成
    // ***********************************
    Axsrs_Data_Sakusei();

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『付属品部品展開処理』終了     **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『付属品部品展開処理』終了     *********");
    Write_Error_Log("*******************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 付属品ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Axsrs_Data_Sakusei(void) {
    bool ret;
    int i_Pos;
    AnsiString s_Drp_Syurui; // ﾄﾞﾚﾝﾊﾟﾝ種類
    AnsiString s_Ank_Syurui; // ﾄﾞﾚﾝﾊﾟﾝ種類
    ret = true;

    // ***********************************
    // ***  ﾄﾞﾚﾝﾊﾟﾝﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   ﾄﾞﾚﾝﾊﾟﾝﾃﾞｰﾀ  作成開始   ----------";
    Write_Log(G_Log_Text);

    // s_Drp_Syurui = P_HEADS_DATA[264].TrimRight();   // ﾄﾞﾚﾝﾊﾟﾝ種類

    // 2020.10.29 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_E
    //// 2018.04.04 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_S
    ////if (P_HEADS_DATA[264].TrimRight() != "1.00") {
    //if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
    //    (P_HEADS_DATA[264].TrimRight() != "2.00")) {
    //// 2018.04.04 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_E
    if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
        (P_HEADS_DATA[264].TrimRight() != "2.00") &&
        (P_HEADS_DATA[264].TrimRight() != "3.00")) {
    // 2020.10.29 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_E
        G_Log_Text = "ﾄﾞﾚﾝﾊﾟﾝﾃﾞｰﾀ  ﾄﾞﾚﾝﾊﾟﾝ有無が｢" + s_Drp_Syurui + "｣の為作成しません。";
        Write_Log(G_Log_Text);
    }
    else {
        // フレーム型式で検索して、該当無ければ「Ｊ」「Ｐ」で検索する
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            if (Get_Drpn_Data(P_HEADS_DATA[36].TrimRight()) == false) {
                if ((P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
                    1) == "J") ||
                    (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 2,
                    1) == "J")) {
                    Get_Drpn_Data("J");
                }
                else {
                    Get_Drpn_Data("P");
                }
            }
        }
        else {
            if (Get_Drpn_Data(P_HEADS_DATA[36].TrimRight()) == false) {
                if ((P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
                    1) == "J") ||
                    (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
                    1) == "J")) {
                    Get_Drpn_Data("J");
                }
                else {
                    Get_Drpn_Data("P");
                }
            }
        }
    }
    G_Log_Text = "----------   ﾄﾞﾚﾝﾊﾟﾝﾃﾞｰﾀ  作成終了   ----------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  ｱﾝｶｰﾎﾞﾙﾄﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   ｱﾝｶｰﾎﾞﾙﾄﾃﾞｰﾀ 作成開始   ----------";
    Write_Log(G_Log_Text);

    Get_Anch_Data();

    G_Log_Text = "----------   ｱﾝｶｰﾎﾞﾙﾄﾃﾞｰﾀ 作成終了   ----------";
    Write_Log(G_Log_Text);

    // 2015.03.23 OGI 付属品欄に「ラチェットスパナ」出力対応
    // 0：スパナ有、舶用用具有
    // 1：スパナ無、舶用用具無
    // 2：スパナ有、舶用用具無
    // 3：スパナ無、舶用用具有     //
    //if (P_HEADS_DATA[262].ToIntDef(0) == 0) {
     if ( (P_HEADS_DATA[262].ToIntDef(0) == 0) || (P_HEADS_DATA[262].ToIntDef(0) == 2) ) {
        // ***********************************
        // ***  ｽﾊﾟﾅﾃﾞｰﾀ　作成
        // ***********************************
        G_Log_Text = "----------   ｽﾊﾟﾅﾃﾞｰﾀ 作成開始   ----------";
        Write_Log(G_Log_Text);

        Get_Spana_Data();

        G_Log_Text = "----------   ｽﾊﾟﾅﾃﾞｰﾀ 作成終了   ----------";
        Write_Log(G_Log_Text);
    }
    // ***********************************
    // ***  日阪マークﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   日阪マーク 作成開始   ----------";
    Write_Log(G_Log_Text);

    Get_HMark_Data();

    G_Log_Text = "----------   日阪マーク 作成終了   ----------";
    Write_Log(G_Log_Text);

    // ***********************************
    // ***  銘板ﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   日阪マーク 作成開始   ----------";
    Write_Log(G_Log_Text);

    Get_Meiban_Data();

    G_Log_Text = "----------   日阪マーク 作成終了   ----------";
    Write_Log(G_Log_Text);

    // 2012.10.15 ボルト落下注意ステッカー追加
    // ***********************************
    // ***  ﾎﾞﾙﾄ落下注意ｽﾃｯｶｰﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "-------   ﾎﾞﾙﾄ落下注意ｽﾃｯｶｰ 作成開始   ------";
    Write_Log(G_Log_Text);

    Get_Rakka_Data();

    G_Log_Text = "-------   ﾎﾞﾙﾄ落下注意ｽﾃｯｶｰ 作成終了   ------";
    Write_Log(G_Log_Text);
    // ***************************************

    // 2020.12.16 シートガスケット名称変更_S
    // ***********************************
    // ***  シートガスケット名称　取得
    // ***********************************
    G_Log_Text = "-----   シートガスケット名称 取得開始   -----";

    Write_Log(G_Log_Text);

    Get_SeatGas_Name();

    G_Log_Text = "-----   シートガスケット名称 取得終了   -----";
    Write_Log(G_Log_Text);
    // 2020.12.16 シートガスケット名称変更_E

    // ***********************************
    // ***  相フランジﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   相フランジ 作成開始   ----------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // 舶用以外
        Get_IFrng_Data();
    }
    G_Log_Text = "----------   相フランジ 作成終了   ----------";
    Write_Log(G_Log_Text);

    // 2004.03.16 ﾌﾞﾗｲﾝﾄﾞﾌﾗﾝｼﾞ追加
    // ***********************************
    // ***  ﾌﾞﾗｲﾝﾄﾞﾌﾗﾝｼﾞﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "---------   ﾌﾞﾗｲﾝﾄﾞﾌﾗﾝｼﾞ 作成開始   ---------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // 舶用以外
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[265].TrimRight() != "0000")
        //{ // ﾌﾞﾗｲﾝﾄﾞﾌﾗﾝｼﾞ材質が００００以外の場合
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-") {
            // ﾌﾞﾗｲﾝﾄﾞﾌﾗﾝｼﾞ材質が００００以外の場合
        // ***********************
            Get_BFRG_Data();
        }
    }
    G_Log_Text = "---------   ﾌﾞﾗｲﾝﾄﾞﾌﾗﾝｼﾞ 作成終了   ---------";
    Write_Log(G_Log_Text);
    // *************************

    // 2004.02.04 ｼｰﾄﾊﾟｯｷﾝｸﾞ追加
    // ***********************************
    // ***  ｼｰﾄﾊﾟｯｷﾝｸﾞﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   ｼｰﾄﾊﾟｯｷﾝｸﾞ 作成開始   ----------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // 舶用以外
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1068].TrimRight() == "0000") { // 相フランジ材質が００００の場合
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "-") {
            // 相フランジ材質が００００の場合
        // ***********************
            Get_STGkt_Data();
        }
    }
    G_Log_Text = "----------   ｼｰﾄﾊﾟｯｷﾝｸﾞ 作成終了   ----------";
    Write_Log(G_Log_Text);
    // *************************

    // 2020.04.23 舶用以外ｽﾄﾚｰﾅ追加_S
    // ***********************************
    // ***  舶用以外ｽﾄﾚｰﾅ　作成
    // ***********************************
    G_Log_Text = "---------   舶用以外ｽﾄﾚｰﾅ 作成開始   ---------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos == 0) { // 舶用以外
        if (Type_MAT(P_HEADS_DATA[261].TrimRight()) != "-") {
            // ｽﾄﾚｰﾅ材質が００００以外の場合
            Get_Strainer_Data();
        }
    }
    G_Log_Text = "---------   舶用以外ｽﾄﾚｰﾅ 作成終了   ---------";
    Write_Log(G_Log_Text);
    // 2020.04.23 舶用以外ｽﾄﾚｰﾅ追加_E

    // ***********************************
    // ***  舶用部品ﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   船用部品ﾃﾞｰﾀ 作成開始   ----------";
    Write_Log(G_Log_Text);

    i_Pos = P_HEADS_DATA[36].Pos("M");
    if (i_Pos != 0) {
        Get_Fune_Data(true);
    } // else {
    // Get_Fune_Data(false);
    // G_Log_Text = "RecNo:543｢ﾌﾚｰﾑ｣に｢M｣が含まれないので、舶用部品ﾃﾞｰﾀは作成しません。";
    // Write_Log(G_Log_Text);
    // }

    G_Log_Text = "----------   船用部品ﾃﾞｰﾀ 作成終了   ----------";
    Write_Log(G_Log_Text);

    // 2014.12.22 出荷用閉止フランジ追加
    // ***********************************
    // ***  出荷用閉止ﾌﾗﾝｼﾞﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   出荷用閉止ﾌﾗﾝｼﾞﾃﾞｰﾀ 作成開始   ----------";
    Write_Log(G_Log_Text);
    Get_TFLG_Data();
    G_Log_Text = "----------   出荷用閉止ﾌﾗﾝｼﾞﾃﾞｰﾀ 作成終了   ----------";
    Write_Log(G_Log_Text);
    // *********************************

    EtcData_Write();
    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾄﾞﾚﾝﾊﾟﾝ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Drpn_Data(AnsiString aFrmType) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    AnsiString s_Key;
    AnsiString s_SQL;
    AnsiString s_Text;
    bool bRet;

    bRet = false;
    // 2009.09.04 ﾄﾞﾚﾝﾊﾟﾝﾏｽﾀではなくP_HEADS_DATAからﾃﾞｰﾀを求める
    // (製作指示書にあわせる)
    // TQuery *wkQuery;
    // wkQuery = new TQuery(DataModule1);
    //
    // // 検索キー変更 02.08.30
    // //s_Key = G_K_Syurui + "-" + G_K_Model1;
    // s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    //
    // //*********************************************
    // //***  ﾄﾞﾚﾝﾊﾟﾝﾏｽﾀ(HD_DR_PAN) 検索
    // //*********************************************
    // G_Log_Text = "ﾄﾞﾚﾝﾊﾟﾝﾏｽﾀ(HD_DR_PAN)を検索キー  『" + s_Key + "』で検索。";
    // Write_Log(G_Log_Text);
    //
    // s_SQL = "";
    // s_SQL = s_SQL + "SELECT * FROM HD_DR_PAN";
    // s_SQL = s_SQL + "  WHERE PLATE_MODEL = '" + s_Key + "'";
    // s_SQL = s_SQL + "    AND FRAME_TYPE = '" + aFrmType + "'";
    //
    // wkQuery->Close();
    // wkQuery->DatabaseName = ODBC_DSN_DNO;
    // wkQuery->SQL->Clear();
    // wkQuery->SQL->Add(s_SQL);
    // wkQuery->Open();
    // wkQuery->First();
    //
    // //if (!wkQuery->Eof) {
    // if (wkQuery->RecordCount == 0){
    // G_ErrLog_Text = "ﾃﾞｰﾀ作成時　 プレート型式｢" + s_Key +"｣+｢" + aFrmType + "｣ は、ﾄﾞﾚﾝﾊﾟﾝﾏｽﾀ(HD_DR_PAN)に存在しません。";
    // Write_Error_Log(G_ErrLog_Text);
    // }
    //
    // while(!wkQuery->Eof)
    // {
    // // ﾄﾞﾚﾝﾊﾟﾝ材質ｺｰﾄﾞの取得
    // // 部品分類
    // s_Code    = wkQuery->FieldByName("DR_PAN_MAT_CODE")->AsString.TrimRight();
    // P_Sikibetu = "DRPN--";                                            // 識別子
    // P_Katasiki = "ドレンパン";                                        // 部品型式
    // P_Zuban    = wkQuery->FieldByName("DNO")->AsString.TrimRight();   // 図番
    // P_SZuban   = wkQuery->FieldByName("DNO")->AsString.TrimRight();   // 作図用図番
    // P_Num      = 1;
    // P_Unit     = "";
    // P_Hosoku   = "";
    //
    //
    // //*********************************
    // //***  材質ｺｰﾄﾞﾏｽﾀ検索
    // //*********************************
    // P_Zaisitu = Search_HD_MATERIAL(s_Code);
    // P_EZaisitu =Search_HD_E_MATERIAL(s_Code);
    //
    // // 予備品数
    // P_Yobisu = 0;
    // // 仕様１
    // //2008.03.31 ﾄﾞﾚﾝﾊﾟﾝ仕様変更
    // ////2003.09.08 仕様変更
    // ////P_Siyou = "Ｌ１＝"+ HanToZen( P_HEADS_DATA[1293].TrimRight() )+
    // ////          " Ｌ２＝"+ HanToZen( P_HEADS_DATA[1294].TrimRight() )+
    // ////          " Ｌ３＝"+ HanToZen( P_HEADS_DATA[1295].TrimRight() );
    // //s_Text = P_HEADS_DATA[1294].TrimRight();  //Ｌ２寸法
    // //if (s_Text == "0") {
    // //   P_Siyou = "Ｌ１＝"+ HanToZen( P_HEADS_DATA[1293].TrimRight() );
    // //} else {
    // //   P_Siyou = "Ｌ１＝"+ HanToZen( P_HEADS_DATA[1293].TrimRight() )+
    // //             " Ｌ２＝"+ HanToZen( P_HEADS_DATA[1294].TrimRight() )+
    // //             " Ｌ５＝"+ HanToZen( P_HEADS_DATA[1295].TrimRight() );
    // //}
    // //Ｌ１寸法
    // P_Siyou = "Ｌ１＝"+ HanToZen( P_HEADS_DATA[1293].TrimRight() );
    // //Ｌ２寸法
    // s_Text = P_HEADS_DATA[1294].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " Ｌ２＝"+ HanToZen( P_HEADS_DATA[1294].TrimRight() );
    // }
    // //Ｌ３寸法
    // s_Text = P_HEADS_DATA[871].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " Ｌ３＝"+ HanToZen( P_HEADS_DATA[871].TrimRight() );
    // }
    // //Ｌ４寸法
    // s_Text = P_HEADS_DATA[872].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " Ｌ４＝"+ HanToZen( P_HEADS_DATA[872].TrimRight() );
    // }
    // //Ｌ５寸法
    // s_Text = P_HEADS_DATA[1295].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " Ｌ５＝"+ HanToZen( P_HEADS_DATA[1295].TrimRight() );
    // }
    // //Ｌ６寸法
    // s_Text = P_HEADS_DATA[873].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " Ｌ６＝"+ HanToZen( P_HEADS_DATA[873].TrimRight() );
    // }
    // //Ｌ７寸法
    // s_Text = P_HEADS_DATA[874].TrimRight();
    // if (s_Text != "0") {
    // P_Siyou = P_Siyou + " Ｌ７＝"+ HanToZen( P_HEADS_DATA[874].TrimRight() );
    // }
    // //*******************
    // //**************************
    //
    // P_Siyou5 = "";
    // // ログ
    // G_Log_Text = "ﾄﾞﾚﾝﾊﾟﾝ材質ｺｰﾄﾞ    ｢" + s_Code + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    // Write_Log(G_Log_Text);
    //
    // //*********************************
    // //***  付属品ﾃﾞｰﾀ(ﾄﾞﾚﾝﾊﾟﾝ)　作成
    // //*********************************
    // Write_Axsrs_Data(s_Code);
    //
    // wkQuery->Next();
    // bRet = true;
    // }
    //
    // delete wkQuery;

    // ***************************
    // ***  ﾄﾞﾚﾝﾊﾟﾝ図面
    // ***************************
    // 2020.10.28 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_S
    //// 2018.04.04 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_S
    ////if (P_HEADS_DATA[264].TrimRight() != "1.00") {
    //if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
    //    (P_HEADS_DATA[264].TrimRight() != "2.00")) {
    //// 2018.04.04 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_E
    if ((P_HEADS_DATA[264].TrimRight() != "1.00") &&
        (P_HEADS_DATA[264].TrimRight() != "2.00") &&
        (P_HEADS_DATA[264].TrimRight() != "3.00")) {
    // 2020.10.28 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_E
        // ﾄﾞﾚﾝﾊﾟﾝ がないため記述しません
    }
    else if (P_HEADS_DATA[1330].Trim() == "") {
        // 図番がないため記述しません
    }
    else {

        // ﾄﾞﾚﾝﾊﾟﾝ材質ｺｰﾄﾞの取得
        // 部品分類
        s_Code = "1001";
        P_Sikibetu = "DRPN--"; // 識別子
        P_Katasiki = "ドレンパン"; // 部品型式
        s_Text = P_HEADS_DATA[1330].Trim();
        s_Text = s_Text.SubString(15, s_Text.Length() - 14);
        s_Text = s_Text.SubString(1, s_Text.Pos("　L") - 1);
        P_Zuban = s_Text; // 図番
        P_SZuban = s_Text; // 作図用図番
        P_Num = 1;
        P_Unit = "";
        P_Hosoku = "";

        // *********************************
        // ***  材質ｺｰﾄﾞﾏｽﾀ検索
        // *********************************
        P_Zaisitu = Search_HD_MATERIAL(s_Code);
        P_EZaisitu = Search_HD_E_MATERIAL(s_Code);

        // 予備品数
        P_Yobisu = 0;
        // 仕様１
        // Ｌ１寸法
        P_Siyou = "Ｌ１＝" + HanToZen(P_HEADS_DATA[1293].TrimRight());
        // Ｌ２寸法
        s_Text = P_HEADS_DATA[874].TrimRight();
        if (s_Text == "0") {
            // ﾄﾞﾚﾝﾊﾟﾝ寸法は4つまで記述する
            // Ｌ７寸法が存在する場合、Ｌ２寸法は記述しない。
            s_Text = P_HEADS_DATA[1294].TrimRight();
            if (s_Text != "0") {
                P_Siyou = P_Siyou + " Ｌ２＝" +
                    HanToZen(P_HEADS_DATA[1294].TrimRight());
            }
        }
        // Ｌ３寸法(未記述)
        // s_Text = P_HEADS_DATA[871].TrimRight();
        // if (s_Text != "0") {
        // P_Siyou = P_Siyou + " Ｌ３＝"+ HanToZen( P_HEADS_DATA[871].TrimRight() );
        // }
        // Ｌ４寸法(未記述)
        // s_Text = P_HEADS_DATA[872].TrimRight();
        // if (s_Text != "0") {
        // P_Siyou = P_Siyou + " Ｌ４＝"+ HanToZen( P_HEADS_DATA[872].TrimRight() );
        // }
        // Ｌ５寸法
        s_Text = P_HEADS_DATA[1295].TrimRight();
        if (s_Text != "0") {
            P_Siyou = P_Siyou + " Ｌ５＝" +
                HanToZen(P_HEADS_DATA[1295].TrimRight());
        }
        // Ｌ６寸法
        s_Text = P_HEADS_DATA[873].TrimRight();
        if (s_Text != "0") {
            P_Siyou = P_Siyou + " Ｌ６＝" +
                HanToZen(P_HEADS_DATA[873].TrimRight());
        }
        // Ｌ７寸法
        s_Text = P_HEADS_DATA[874].TrimRight();
        if (s_Text != "0") {
            P_Siyou = P_Siyou + " Ｌ７＝" +
                HanToZen(P_HEADS_DATA[874].TrimRight());
        }
        P_Siyou5 = "";
        // 2020/10/22 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_S
        //// 2018.04.04 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_S
        //if (P_HEADS_DATA[264].TrimRight() == "2.00") {
        //// 断熱材付
        //    P_Katasiki = "ドレンパン(断熱材付)"; // 部品型式
        //}
        //// 2018.04.04 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_E
        if (P_HEADS_DATA[264].TrimRight() == "2.00") {
            // 断熱材付
            P_Katasiki = "ドレンパン(断熱材付(任意))"; // 部品型式
        } else if (P_HEADS_DATA[264].TrimRight() == "3.00") {
            // 断熱材付
            P_Katasiki = "ドレンパン(断熱材付(国交認定品))"; // 部品型式
        }
        // 2020/10/22 ﾄﾞﾚﾝﾊﾟﾝ断熱材追加_E
        // ログ
        G_Log_Text = "ﾄﾞﾚﾝﾊﾟﾝ材質ｺｰﾄﾞ    ｢" + s_Code + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  付属品ﾃﾞｰﾀ(ﾄﾞﾚﾝﾊﾟﾝ)　作成
        // *********************************
        Write_Axsrs_Data(s_Code);

        bRet = true;
    }

    // **********************************************************
    return bRet;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ｱﾝｶｰﾎﾞﾙﾄ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Anch_Data(void) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    AnsiString s_Key;
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;
    AnsiString s_Text;
    AnsiString s_Syubetu;
    // 2006.02.06 変数追加
    AnsiString s_Text2; // 汎用文字列２
    // *******************
    // 2012.01.20 中国材質
    AnsiString s_ChinaM;
    int i_RecNo;
    // *******************

    // ***************************
    // ***  ｱﾝｶｰﾎﾞﾙﾄ　長さ
    // ***************************
    // 『C026』
    // 種別ｺｰﾄﾞの取得
    s_Text = P_HEADS_DATA[1045].TrimRight(); // SPANC
    // 2022.09.21 ドレンパン用アンカーDB定義変更_S
    //if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07") {
    //    s_Text = P_HEADS_DATA[264].TrimRight(); // ﾄﾞﾚﾝﾊﾟﾝ有無
    //    if (s_Text == "1.00") {
    //        s_Syubetu = "1";
    //    }
    //    else {
    //        s_Syubetu = "0";
    //    }
    //    // 2003.12.22 ﾎｰﾙｲﾝのSUSによる区分の削除
    //    // } else if (s_Text == "02" ) {
    //    // s_Text = P_HEADS_DATA[37].TrimRight();   // ﾌﾚｰﾑ型式ｺｰﾄﾞ
    //    // switch (s_Text.ToIntDef(0)){
    //    // case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
    //    // s_Syubetu = "1";
    //    // break;
    //    // default:
    //    // s_Text = P_HEADS_DATA[1048].TrimRight();  // SPANM
    //    // if (s_Text == "1001") {
    //    // s_Syubetu = "1";
    //    // } else {
    //    // s_Syubetu = "0";
    //    // }
    //    // }
    //    // *************************************
    //}
    if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07" ||
        s_Text == "11") {
        s_Text = P_HEADS_DATA[264].TrimRight(); // ﾄﾞﾚﾝﾊﾟﾝ有無
        if (s_Text == "1.00" || s_Text == "2.00" || s_Text == "3.00" ) {
            s_Syubetu = "1";
        }
        else {
            s_Syubetu = "0";
        }
    }
    // 2022.09.21 ドレンパン用アンカーDB定義変更_E
    else {
        s_Syubetu = "0";
    }

    // ｱﾝｶｰﾎﾞﾙﾄ長ﾏｽﾀ 検索
    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ANCHOR_SIZE";
    // 2012.06.08 ｱﾝｶｰﾎﾞﾙﾄ検索条件変更(RX-10-EP)
    // s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "'";
    switch (P_HEADS_DATA[37].ToIntDef(0)) { // フレームコード
    case 71:
    case 72:
    case 81:
    case 91:
    case 381:
    case 391: // ＥＰシリーズ
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "E'";
        break;
    //2017.04.20 Zﾌﾚｰﾑ追加
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 334:
    case 335:
    case 336:
    case 337:
    case 338:
    case 344:
    case 345:
    case 346:
    case 347:
    case 348:  // Ｚフレーム
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "Z'";
        break;
    //********************
    //2023.04.05 Yﾌﾚｰﾑ追加_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 39:
    case 40:
    case 43:
    case 49:
    case 50:
    case 52:
    case 53:
    case 54:
    case 339:
    case 340:
    case 343:
    case 349:
    case 350:
    case 352:
    case 353:
    case 354:  // Ｙフレーム
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "Y'";
        break;
    //2023.04.05 Yﾌﾚｰﾑ追加_E
    default:
        s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
        s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "'";
        break;
    }
    // *****************************************
    s_SQL = s_SQL + "    AND ANCHOR_STAT = " + s_Syubetu + "";
    s_SQL = s_SQL + "    AND ABCHOR_TYPE = " + P_HEADS_DATA[1045].TrimRight
        () + ";";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    P_Siyou5 = "";
    if (wkQuery->Eof) {
        s_Text = "";
    }
    else {
        P_Sikibetu = "ACBL--"; // 部品分類
        // 2003.12.22 ﾎｰﾙｲﾝのBOLT_TYPEの長さによる変更の追加
        ////2003.12.05 ｹﾐｶﾙ､ﾎｰﾙｲﾝの長さを入力できるようにする
        ////if ( P_HEADS_DATA[1045].TrimRight() != "03" && P_HEADS_DATA[1045].TrimRight() != "02" &&
        ////     P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047].TrimRight() != "" ) {
        // if ( P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047].TrimRight() != "" ) {
        ////*************************************************
        if (P_HEADS_DATA[1045].TrimRight() == "02") {
            // 2004.03.15 ﾎｰﾙｲﾝ長さ追加
            if (P_HEADS_DATA[1047].ToIntDef(0) == 60) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "60";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 70) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "70";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 80) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "80";
                // 2004.04.27 ﾎｰﾙｲﾝ長さ追加
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 90) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "90";
                // ************************
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 100) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "10";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 120) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "12";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 130) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "13";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 150) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "15";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 190) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "19";
            }
            else if (P_HEADS_DATA[1047].ToIntDef(0) == 230) {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight
                    () + "23";
            }
            else if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
                .TrimRight() != "") {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight()) + "×" +
                    HanToZen(FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) +
                    "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")
                    ->AsString.TrimRight();
            }
            else {
                P_Siyou =
                    HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                    ->AsString.TrimRight());
                if (P_Siyou == "Ｍ１２") {
                    P_Siyou =
                        P_Siyou + "×" +
                        HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight()) + "Ｌ " + wkQuery->FieldByName
                        ("BOLT_TYPE")->AsString.TrimRight() + "80";
                }
                else {
                    P_Siyou =
                        P_Siyou + "×" +
                        HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight()) + "Ｌ " + wkQuery->FieldByName
                        ("BOLT_TYPE")->AsString.TrimRight() + "13";
                }
            }
            ////if ( P_HEADS_DATA[1047].ToIntDef(0) == 100 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "×" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "Ｌ "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "10";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 130 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "×" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "Ｌ "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "13";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 150 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "×" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "Ｌ "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "15";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 190 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "×" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "Ｌ "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "19";
            ////} else if ( P_HEADS_DATA[1047].ToIntDef(0) == 230 ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "×" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "Ｌ "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "23";
            ////} else if ( P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047].TrimRight() != "" ) {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "×" + HanToZen(FormatFloat("0",P_HEADS_DATA[1047].ToDouble())) + "Ｌ "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            ////} else {
            ////  P_Siyou = HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight())
            ////         + "×" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight()) + "Ｌ "
            ////         + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
            ////         + "13";
            ////}
            // ************************
            // 2006.02.06 ｹﾐｶﾙｱﾝｶｰのBOLT_TYPEの長さによる変更の追加
        }
        else if (P_HEADS_DATA[1045].TrimRight() == "03") {
            //2015.11.19 ケミカルアンカー全面変更
            //s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            //if (s_Text == "AP-20") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-20";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 180) {
            //        s_Text = "AP-20S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
            //        s_Text = "AP-20";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 300) {
            //        s_Text = "AP-20L";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //}
            //else if (s_Text == "AP-24") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-24";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 270) {
            //        s_Text = "AP-24S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 375) {
            //        s_Text = "AP-24";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 420) {
            //        s_Text = "AP-24L";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //}
            //else if (s_Text == "AP-30") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-30";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 310) {
            //        s_Text = "AP-30S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 430) {
            //        s_Text = "AP-30";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //    // 2012.07.03 AP-12追加
            //}
            //else if (s_Text == "AP-12") {
            //    if (P_HEADS_DATA[1047].ToIntDef(0) == 0) {
            //        s_Text = "AP-12";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 90) {
            //        s_Text = "AP-12S";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 130) {
            //        s_Text = "AP-12";
            //    }
            //    else if (P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
            //        s_Text = "AP-12L";
            //    }
            //    else {
            //        s_Text = "";
            //    }
            //    // ********************
            //}
            //if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
            //    .TrimRight() != "") {
            //    if (s_Text == "AP-20L" && P_HEADS_DATA[1047].ToIntDef(0) >
            //        290 && P_HEADS_DATA[1047].ToIntDef(0) <= 295) {
            //        s_Text2 = "295";
            //    }
            //    else if (s_Text == "AP-24" && P_HEADS_DATA[1047].ToIntDef(0) >
            //        370 && P_HEADS_DATA[1047].ToIntDef(0) <= 375) {
            //        s_Text2 = "375";
            //    }
            //    else {
            //        s_Text2 =
            //            FormatFloat("0",
            //            ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
            //    }
            //    P_Siyou =
            //        HanToZen(wkQuery->FieldByName("BOLT_SIZE")
            //        ->AsString.TrimRight()) + "×" + HanToZen(s_Text2) +
            //        "Ｌ " + s_Text;
            //}
            //else {
            //    P_Siyou =
            //        HanToZen(wkQuery->FieldByName("BOLT_SIZE")
            //        ->AsString.TrimRight()) + "×" +
            //        HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
            //        ->AsString.TrimRight()) + "Ｌ " + s_Text;
            //}
            //// ****************************************************
            s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            if (s_Text == "HP-12") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 71 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-20") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 141 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 350) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-24") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 371 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 375) {
                    s_Text2 =  "375";
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 450) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-30") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 500) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            }
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                ->AsString.TrimRight()) + "×" +
                HanToZen(s_Text2) + "Ｌ " + s_Text;
            // **********************************
        }
        // 2022.10.06 ダブルナット修正_S
        //else if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
        //    .TrimRight() != "") {
        //    // *************************************************
        //    P_Siyou =
        //        HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
        //        ) + "×" + HanToZen
        //        (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "Ｌ " +
        //        wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
        //}
        // 2022.10.06 ダブルナット修正_E
        // 2021.04.07 ダブルナット追加_S
        else if (P_HEADS_DATA[1045].TrimRight() == "11") {
            // Ｌ型アンカーボルト・ダブルナット
            if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
                .TrimRight() != "") {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "×" + HanToZen
                    (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "Ｌ " +
                    wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() +
                    "、ナット（１種×１個、３種×１個）";
            }
            else {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "×" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                   ->AsString.TrimRight()) + "Ｌ " + wkQuery->FieldByName
                    ("BOLT_TYPE")->AsString.TrimRight() +
                    "、ナット（１種×１個、３種×１個）";
            }
        }
        else if (P_HEADS_DATA[1045].TrimRight() == "13") {
            // ケミカルアンカー・ダブルナット
            s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
            if (s_Text == "HP-12") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 71 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 250) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-20") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 141 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 350) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-24") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 450) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            } else if (s_Text == "HP-30") {
                if (P_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                    P_HEADS_DATA[1047].TrimRight() == "") {
                    s_Text2 =
                        wkQuery->FieldByName("BOLT_LENGTH")
                        ->AsString.TrimRight();
                } else if (P_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                           P_HEADS_DATA[1047].ToIntDef(0) <= 500) {
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                } else {
                    s_Text = "";
                    s_Text2 =
                        FormatFloat("0",
                        ceil(P_HEADS_DATA[1047].ToDouble() / 10) * 10);
                }
            }
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")
                ->AsString.TrimRight()) + "×" +
                HanToZen(s_Text2) + "Ｌ " + s_Text +
                "、ナット（１種×１個、３種×１個）";
        }
        else if (P_HEADS_DATA[1045].TrimRight() == "15") {
            // セットボルト・ダブルナット
            if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
                .TrimRight() != "") {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "×" + HanToZen
                    (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "Ｌ " +
                    wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() +
                    "、ナット（１種×１個、３種×１個）";
            }
            else {
                P_Siyou =
                    HanToZen
                    (wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()) +
                    "×" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                   ->AsString.TrimRight()) + "Ｌ " + wkQuery->FieldByName
                    ("BOLT_TYPE")->AsString.TrimRight() +
                    "、ナット（１種×１個、３種×１個）";
            }
        }
        // 2021.04.07 ダブルナット追加_E
        // 2022.10.06 ダブルナット修正_S
        else if (P_HEADS_DATA[1047].TrimRight() != "0" && P_HEADS_DATA[1047]
            .TrimRight() != "") {
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                ) + "×" + HanToZen
                (FormatFloat("0", P_HEADS_DATA[1047].ToDouble())) + "Ｌ " +
                wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
        }
         // 2022.10.06 ダブルナット修正_E
        else {
            P_Siyou =
                HanToZen(wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                ) + "×" + HanToZen(wkQuery->FieldByName("BOLT_LENGTH")
                ->AsString.TrimRight()) + "Ｌ " + wkQuery->FieldByName
                ("BOLT_TYPE")->AsString.TrimRight();
        }

        P_Siyou = ZenToHan(P_Siyou);
        // ***************************
        // ***  ｱﾝｶｰﾎﾞﾙﾄ　材質
        // ***************************
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1048].TrimRight() != "" && P_HEADS_DATA[1048].TrimRight
        //    () != "0000") {
        if (Type_MAT(P_HEADS_DATA[1048].TrimRight()) != "-") {
        // ***********************
            P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1048].TrimRight());
            // 2004.02.05
            // P_EZaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1048].TrimRight());
            P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[1048].TrimRight());
            // **********
        }
        else {
            s_Text = P_HEADS_DATA[1045].TrimRight();
            // 2021.04.07 ダブルナット追加_S
            //if (s_Text == "00" || s_Text == "01" || s_Text == "03" ||
            //    s_Text == "05" || s_Text == "06" || s_Text == "07") {
            if (s_Text == "00" || s_Text == "01" || s_Text == "03" ||
                s_Text == "05" || s_Text == "06" || s_Text == "07" ||
                s_Text == "11" || s_Text == "13" || s_Text == "15" ) {
            // 2021.04.07 ダブルナット追加_E
                switch (P_HEADS_DATA[37].ToIntDef(0)) {
                case 14:
                case 15:
                case 16:
                case 27:
                case 28:
                case 29:
                case 63:
                case 64:
                    P_Zaisitu = "ＳＵＳ３０４";
                    P_EZaisitu = "ＳＵＳ３０４";
                    break;
                default:
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //if (P_HEADS_DATA[1048].TrimRight() == "1001") {
                    if (Type_MAT(P_HEADS_DATA[1048].TrimRight()) == "SUS304") {
                    // ***********************
                        P_Zaisitu = "ＳＵＳ３０４";
                        P_EZaisitu = "ＳＵＳ３０４";
                    }
                    else {
                        P_Zaisitu = "ＳＳ４００";
                        P_EZaisitu = "ＳＳ４００";
                    }
                    break;
                }
            }
            else if (s_Text == "02") {
                switch (P_HEADS_DATA[37].ToIntDef(0)) {
                case 14:
                case 15:
                case 16:
                case 27:
                case 28:
                case 29:
                case 63:
                case 64:
                    P_Zaisitu = "ＳＵＳ３０４";
                    P_EZaisitu = "ＳＵＳ３０４";
                    break;
                default:
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //if (P_HEADS_DATA[1048].TrimRight() == "1001") {
                    if (Type_MAT(P_HEADS_DATA[1048].TrimRight()) == "SUS304") {
                    // ***********************
                        P_Zaisitu = "ＳＵＳ３０４";
                        P_EZaisitu = "ＳＵＳ３０４";
                    }
                    else {
                        // 2004.01.07 材質変更
                        // P_Zaisitu = "Ｃ．Ｓ．";
                        // P_EZaisitu = "Ｃ．Ｓ．";
                        P_Zaisitu = "ＳＳ４００";
                        P_EZaisitu = "ＳＳ４００";
                        // *******************
                    }
                    break;
                }

            }
            else if (s_Text == "04") {
                // 2004.01.19 材質変更
                // P_Zaisitu = "ＳＤ２４／ＳＳ４００";
                // P_EZaisitu = "ＳＤ２４／ＳＳ４００";
                // 2004.02.02 材質変更
                // P_Zaisitu = "ＳＲ２３５／ＳＳ４００";
                // P_EZaisitu = "ＳＲ２３５／ＳＳ４００";
                P_Zaisitu = "ＳＤ３４５／ＳＳ４００";
                P_EZaisitu = "ＳＤ３４５／ＳＳ４００";
                // *******************
                // *******************
            }
            else if (s_Text == "09") {
                P_Zaisitu = "";
                P_EZaisitu = "";
            }
            else {
                P_Zaisitu = "";
                P_EZaisitu = "";
            }
        }
        if (P_Zaisitu == "ＳＵＳ３０４") {
            P_Zuban = wkQuery->FieldByName("DNO2")->AsString.TrimRight(); // 図番
            P_SZuban = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
            // 作図用図番
        }
        else {
            P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // 図番
            P_SZuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
            // 作図用図番
        }
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // 予備品識別子
        P_Num = wkQuery->FieldByName("NUM")->AsString.ToIntDef(1); // 予備品識別子
        P_Unit = "式";
        P_Yobisu = Syukei_Buhin_Yobisu(ZenToHan(P_Yobihin), "0",
            m_pcHeadsData); // 予備数

        // 2012.01.20 中国材質
        i_RecNo = 1107;
        s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

        s_Text = G_KS_Syurui + "－" + G_KS_Model;
        if (s_Text == "ＵＸ－１０") { // 中国材質
        }
        else if (s_Text == "ＬＸ－００") { // 中国材質
        }
        else if (s_Text == "ＬＸ－１０") { // 中国材質
        }
        else if (s_Text == "ＬＸ－３０") { // 中国材質
        }
        else if (s_Text == "ＬＸ－５０") { // 中国材質
        }
        else if (s_Text == "ＳＸ－２０") { // 中国材質
        }
        else if (s_Text == "ＳＸ－４１") { // 中国材質
        }
        else if (s_Text == "ＳＸ－４３") { // 中国材質
        }
        else if (s_Text == "ＲＸ－１１") { // 中国材質
        }
        else if (s_Text == "ＲＸ－１３") { // 中国材質
        }
        else if (s_Text == "ＲＸ－３０") { // 中国材質
        }
        else if (s_Text == "ＲＸ－５０") { // 中国材質
        }
        else if (s_Text == "ＲＸ－７０") { // 中国材質
        }
        else {
            s_ChinaM = "";
        }

        if (s_ChinaM == "CHINA") { // 中国材質
            if (P_Zaisitu == "ＳＳ４００") {
                P_Zaisitu = "Ｑ２３５Ｂ";
                P_EZaisitu = "Ｑ２３５Ｂ";
            }
        }
        // *******************

        // ***************************
        // ***  ｱﾝｶｰﾎﾞﾙﾄ　種類
        // ***************************
        s_Text = P_HEADS_DATA[1045].TrimRight();
        if (s_Text == "00" || s_Text == "01" || s_Text == "06" ||
            s_Text == "07") {
            P_Katasiki = "アンカーボルト・ナット";
            P_Sikibetu = "ACBL-L"; // 部品分類
        }
        else if (s_Text == "02") {
            P_Katasiki = "ホールインアンカー";
            P_Sikibetu = "ACBL-H"; // 部品分類
        }
        else if (s_Text == "03") {
            P_Katasiki = "ケミカル・アンカー";
            P_Sikibetu = "ACBL-C"; // 部品分類
        }
        else if (s_Text == "04") {
            P_Katasiki = "フシ付きボルト・ナット";
            P_Sikibetu = "ACBL-D"; // 部品分類
        }
        else if (s_Text == "05") {
            P_Katasiki = "セットボルト・ナット";
            P_Sikibetu = "ACBL-S"; // 部品分類
        }
        else if (s_Text == "09") {
            P_Katasiki = "";
        }
        // 2021.04.07 ダブルナット追加_S
        else if (s_Text == "11") {
            P_Katasiki = "アンカーボルト・ダブルナット";
            P_Sikibetu = "ACBLDL"; // 部品分類
        }
        else if (s_Text == "13") {
            P_Katasiki = "ケミカル・アンカー・ダブルナット";
            P_Sikibetu = "ACBLDC"; // 部品分類
        }
        else if (s_Text == "15") {
            P_Katasiki = "セットボルト・ダブルナット";
            P_Sikibetu = "ACBLDS"; // 部品分類
        }
        // 2021.04.07 ダブルナット追加_E
        else {
            P_Katasiki = "";
        }
        if (s_Text == "00" || s_Text == "01") {
            P_Siyou += "  Ｌ型";
        }
        else if (s_Text == "06") {
            // 2014.09.22 ｱﾝｶｰｺｰﾄﾞ変更
            //P_Siyou += "  Ｊ型";
            //P_Sikibetu = "ACBL-J"; // 部品分類
            P_Siyou += "  ＪＡ型";
            P_Sikibetu = "ACBL-I"; // 部品分類
            // ***********************
        }
        else if (s_Text == "07") {
            // 2014.09.22 ｱﾝｶｰｺｰﾄﾞ変更
            //P_Siyou += "  ＪＡ型";
            //P_Sikibetu = "ACBL-D"; // 部品分類
            P_Siyou += "  Ｊ型";
            P_Sikibetu = "ACBL-J"; // 部品分類
            // ***********************
        }
        else if (s_Text == "04") {
            P_Siyou += "  Ｉ型";
        // 2021.04.07 ダブルナット追加_S
        }
        else if (s_Text == "11") {
            P_Siyou += "  Ｌ型";
        // 2021.04.07 ダブルナット追加_E
        }
        P_Hosoku = "";

        // ログ
        G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ材質ｺｰﾄﾞ   ｢" + s_Code + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
        Write_Log(G_Log_Text);

        // 2012.09.20 ｱﾝｶｰﾎﾞﾙﾄ仕上追加
        // ***************************
        // ***  ｱﾝｶｰﾎﾞﾙﾄ　仕上
        // ***************************
        s_Text = P_HEADS_DATA[1324].TrimRight();
        P_Siyou2 = s_Text;
        // ***************************

        // *********************************
        // ***  付属品ﾃﾞｰﾀ(ｱﾝｶｰﾎﾞﾙﾄ)　作成
        // *********************************
        Write_Axsrs_Data(s_Code);

    }

    // //*********************************************
    // //***  ｱﾝｶｰﾎﾞﾙﾄﾏｽﾀ(HD_ANCHOR) 検索
    // //*********************************************
    // s_Key = P_HEADS_DATA[166].TrimRight();   // ｱﾝｶｰﾎﾞﾙﾄｺｰﾄﾞ
    //
    // G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄﾏｽﾀ(HD_ANCHOR)を検索キー  『" + s_Key + "』で検索。";
    // Write_Log(G_Log_Text);
    //
    // s_SQL = "";
    // s_SQL = s_SQL + "SELECT * FROM HD_ANCHOR";
    // s_SQL = s_SQL + "  WHERE ANCHOR_CODE = '" + s_Key + "'";
    //
    // wkQuery->Close();
    // wkQuery->DatabaseName = ODBC_DSN_DNO;
    // wkQuery->SQL->Clear();
    // wkQuery->SQL->Add(s_SQL);
    // wkQuery->Open();
    // wkQuery->First();
    //
    // if (wkQuery->RecordCount == 0){
    // G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ｱﾝｶｰﾎﾞﾙﾄ型式｢" + s_Key +"｣ は、ｱﾝｶｰﾎﾞﾙﾄﾏｽﾀ(HD_ANCHOR)に存在しません。";
    // Write_Error_Log(G_ErrLog_Text);
    // }
    //
    // while(!wkQuery->Eof)
    // {
    // // 材質ｺｰﾄﾞの取得
    // s_Code     = wkQuery->FieldByName("MAT_CODE")->AsString.TrimRight();    // ｱﾝｶｰﾎﾞﾙﾄ材質ｺｰﾄﾞ
    // P_Sikibetu = "ACBL--";                                                  // 部品分類
    // P_Katasiki = wkQuery->FieldByName("PARTS_NAME")->AsString.TrimRight();  // 部品型式
    // P_Zuban    = wkQuery->FieldByName("DNO")->AsString.TrimRight();         // 図番
    // P_SZuban   = wkQuery->FieldByName("DNO")->AsString.TrimRight();         // 作図用図番
    // P_Yobihin  = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();   // 予備品識別子
    //
    // //*********************************
    // //***  材質ｺｰﾄﾞﾏｽﾀ検索
    // //*********************************
    // P_Zaisitu = Search_HD_MATERIAL(s_Code);
    //
    // //*********************************
    // //***  予備品数 取得
    // //*********************************
    // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0");      // 予備数
    //
    // P_Siyou = P_HEADS_DATA[1324].TrimRight();       // 仕様１
    //
    // // ログ
    // G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ材質ｺｰﾄﾞ   ｢" + s_Code + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    // Write_Log(G_Log_Text);
    // G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
    // Write_Log(G_Log_Text);
    //
    // //*********************************
    // //***  付属品ﾃﾞｰﾀ(ｱﾝｶｰﾎﾞﾙﾄ)　作成
    // //*********************************
    // Write_Axsrs_Data(s_Code);
    // wkQuery->Next();
    // }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾗﾁｪｯﾄｽﾊﾟﾅ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
// 2014/08/22 応用技術編集
bool __fastcall TAxsrsDetails::Get_Spana_Data(void) {
    // 実行結果の保持
    bool Result_BDI  = true;
    bool Result_BDIS = true;

    // ボルトサイズを指定して作成(BDI)
    AnsiString BoltSizeL = P_HEADS_DATA[1019].TrimRight();
    Result_BDI = Get_Spana_Data(BoltSizeL);

    // Zフレームの場合、BDIS(小径ボルト)
    if(P_HEADS_DATA[36].Pos("Z") > 0) {
        // ボルトサイズを指定して作成(BDIS)
        AnsiString BoltSizeS = P_HEADS_DATA[1108].TrimRight();

        if(BoltSizeL != BoltSizeS) {
            // BDIと異なる場合のみ作成
            Result_BDIS = Get_Spana_Data(BoltSizeS);
        }
    }

    if(Result_BDI  == false ||
       Result_BDIS == false) {
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾗﾁｪｯﾄｽﾊﾟﾅ部品展開処理 ボルトサイズを指定して作成
// 概  要      ：
// 引  数      ： AnsiString BoltSize : ボルトサイズ
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
// 2014/08/22 応用技術編集
bool __fastcall TAxsrsDetails::Get_Spana_Data(AnsiString BoltSize) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    AnsiString s_Key;
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;

    // *********************************************
    // ***  ｽﾊﾟﾅﾏｽﾀ(HD_ANCHOR) 検索
    // *********************************************
    s_Key = BoltSize; // ﾎﾞﾙﾄｻｲｽﾞ
    if (s_Key == "") {
        return false;
    }
    if (int(s_Key.ToDouble() * 100) == 163) {
        s_Key = "1 5/8-UN";
    }
    else if (int(s_Key.ToDouble() * 100) == 200) {
        s_Key = "2-8UN";
    }
    else {
        s_Key = "M" + s_Key.SubString(1, 2);
    }

    G_Log_Text = "ｽﾊﾟﾅﾏｽﾀ(HD_SPANA)を検索キー  『" + s_Key + "』で検索。";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_SPANA";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ﾎﾞﾙﾄ径｢" + s_Key +
            "｣ は、ｽﾊﾟﾅﾏｽﾀ(HD_ANCHOR)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        // 材質ｺｰﾄﾞの取得
        P_Sikibetu = "RSPN--"; // 部品分類
        P_Katasiki = "ラチェットスパナ"; // 部品型式
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 作図用図番
        P_Yobihin = "SPNR01"; // 予備品識別子
        P_Siyou = wkQuery->FieldByName("SIZE")->AsString.TrimRight(); // 仕様
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Num = 1;
        P_Unit = "";
        P_Hosoku = "";

        // *********************************
        // ***  材質はｶｰﾎﾞﾝｽﾁｰﾙ固定
        // *********************************
        P_Zaisitu = "Ｃ．Ｓ";
        P_EZaisitu = "Ｃ．Ｓ";

        // *********************************
        // ***  予備品数 取得
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

        // ログ
        G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ材質ｺｰﾄﾞ   ｢" + s_Code + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  付属品ﾃﾞｰﾀ(ﾗﾁｪｯﾄｽﾊﾟﾅ)　作成
        // *********************************
        Write_Axsrs_Data(s_Code);
        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 銘板部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Meiban_Data(void) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    AnsiString s_Key;
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;
    AnsiString s_MeibanName;

    P_Siyou = "";
    P_Siyou5 = "";
    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_MODEL_PL";
    // 2004.07.15 国交省銘板に変更
    // if (P_HEADS_DATA[268].TrimRight() == "1" ) {         // 建設省仕様
    // if(P_HEADS_DATA[269].TrimRight() == "9"){          // 銘板年度指定９
    // s_SQL = s_SQL + "  WHERE MODEL_NO = '建設省仕様'";
    // s_SQL = s_SQL + "    AND B_YEAR = 9";
    // }else if(P_HEADS_DATA[269].TrimRight() == "11"){   // 銘板年度指定11
    // s_SQL = s_SQL + "  WHERE MODEL_NO = '建設省仕様'";
    // s_SQL = s_SQL + "    AND B_YEAR = 11";
    // }else{
    // if(P_HEADS_DATA[846].TrimRight() == "1") {      // Ｂがスチーム　Ａ－０５
    // s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-05'";
    // }else if(P_HEADS_DATA[847].TrimRight() == "1") {// Ａがスチーム　Ａ－０７
    // s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-07'";
    // }else {                                         // 上記以外　　　Ａ－０３
    // s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-03'";
    // }
    // }
    // P_Siyou2 = P_HEADS_DATA[1372].TrimRight();
    // s_MeibanName = "銘板(和文)";
    if (P_HEADS_DATA[268].TrimRight() == "1") { // 国交省仕様
        if (P_HEADS_DATA[269].TrimRight() == "11") { // 銘板年度指定11
            s_SQL = s_SQL + "  WHERE MODEL_NO = '国交省仕様'";
            s_SQL = s_SQL + "    AND B_YEAR = 11";
        }
        else {
            if (P_HEADS_DATA[846].TrimRight() == "1") { // Ｂがスチーム　Ａ－０５
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-05'";
            }
            else if (P_HEADS_DATA[847].TrimRight() == "1") { // Ａがスチーム　Ａ－０７
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-07'";
            }
            else { // 上記以外　　　Ａ－０３
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-03'";
            }
        }
        P_Siyou2 = P_HEADS_DATA[1372].TrimRight();
        s_MeibanName = "銘板(和文)" + P_Siyou2;
        // ***************************
        // 2008.06.26 神威銘板廃止
        // }else if(P_HEADS_DATA[1063].Pos("神威産業") != 0){
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '神威産業向'";
        // if(P_HEADS_DATA[1].TrimRight() == "0" ) {          // 和文なら神威Ｊ
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "銘板(和文)";
        // }else{                                             // 英文なら神威Ｅ
        // s_SQL = s_SQL + "    AND WAEI = 'E'";
        // s_MeibanName = "銘板(英文)";
        // }
        // ***********************
        // 2011.05.30 常熟銘板
        // 2012.01.20 常熟フラグ
        // }else if((P_HEADS_DATA[5].TrimRight() == "42842") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // 常熟向け(42842)(和文)銘板
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '常熟銘板和'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "常熟銘板(和文)";
        // }else if((P_HEADS_DATA[5].TrimRight() == "43935") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // 常熟向け(43935)(和文)銘板
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '常熟銘板和'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "常熟銘板(和文)";
        // }else if((P_HEADS_DATA[401].TrimRight() == "42842") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // 常熟向け(42842)(和文)銘板
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '常熟銘板和'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "常熟銘板(和文)";
        // }else if((P_HEADS_DATA[401].TrimRight() == "43935") && (P_HEADS_DATA[1].TrimRight() == "0" )) {
        // // 常熟向け(43935)(和文)銘板
        // s_SQL = s_SQL + "  WHERE MODEL_NO = '常熟銘板和'";
        // s_SQL = s_SQL + "    AND WAEI = 'J'";
        // s_MeibanName = "常熟銘板(和文)";
    }
    else if (P_HEADS_DATA[1107].TrimRight() == "CHINA") {
        // 常熟向け(中文)銘板
        s_SQL = s_SQL + "  WHERE MODEL_NO = '常熟銘板和'";
        s_SQL = s_SQL + "    AND WAEI = 'J'";
        s_MeibanName = "常熟銘板(中文)";
        // *********************
        // *******************
        // 2006.09.23 舶用銘板
    }
    else if (P_HEADS_DATA[36].Pos("M") != 0) { // 舶用
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文なら舶用銘板和
            s_SQL = s_SQL + "  WHERE MODEL_NO = '舶用銘板和'";
            s_SQL = s_SQL + "    AND WAEI = 'J'";
            s_MeibanName = "銘板(和文)";
        }
        else { // 英文なら舶用銘板英
            s_SQL = s_SQL + "  WHERE MODEL_NO = '舶用銘板英'";
            s_SQL = s_SQL + "    AND WAEI = 'E'";
            s_MeibanName = "銘板(英文)";
        }
        // *******************
    }
    else {
        // 2003.11.26 型式訂正
        // 2004.03.16 RX-00追加
        // if( G_KS_Syurui == "FX-01" ){
        // if( G_KS_Syurui == "ＦＸ" && G_KS_Model == "０１" ){
        if ((G_KS_Syurui == "ＦＸ" && G_KS_Model == "０１") ||
            (G_KS_Syurui == "ＲＸ" && G_KS_Model == "００")) {
            // ********************
            // *******************
            if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文ならＡ－１０
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-10'";
                s_MeibanName = "銘板(和文)";
            }
            else { // 英文ならＢ－１０
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'B-10'";
                s_MeibanName = "銘板(英文)";
            }
        }
        else {
            if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文ならＡ－１
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'A-01'";
                s_MeibanName = "銘板(和文)";
            }
            else { // 英文ならＢ－１
                s_SQL = s_SQL + "  WHERE MODEL_NO = 'B-01'";
                s_MeibanName = "銘板(英文)";
            }
        }
    }
    // *********************************************
    // ***  ﾏｽﾀ(HD_MODEL_PL) 検索
    // *********************************************

    G_Log_Text = "銘板ﾏｽﾀ(HD_MODEL_PL)を検索キー  『" + s_Key + "』で検索。";
    Write_Log(G_Log_Text);

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ｢" + s_Key +
            "｣ は、銘板ﾏｽﾀ(HD_MODEL_PL)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        // 材質ｺｰﾄﾞの取得
        P_Sikibetu = "MEIBAN-"; // 部品分類
        P_Katasiki = s_MeibanName; // 部品型式
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 作図用図番
        P_Yobihin = ""; // 予備品識別子
        P_Siyou = wkQuery->FieldByName("SIZE_W")->AsString.TrimRight(); // 仕様
        P_Siyou = P_Siyou + "×";
        P_Siyou = P_Siyou + wkQuery->FieldByName("SIZE_H")
            ->AsString.TrimRight(); // 仕様
        P_Siyou = P_Siyou + "×T0.8";
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Num = 1;
        P_Unit = "";
        if (P_HEADS_DATA[29].ToIntDef(0) != 0) {
            P_Hosoku = Search_KIKAKU_CODE(P_HEADS_DATA[29].TrimRight(), true,
                true, P_HEADS_DATA[1].TrimRight());
        }
        else {
            P_Hosoku = "";
        }

        // P_Katasiki = Search_KIKAKU_CODE(P_HEADS_DATA[29].TrimRight()); // 銘板の名称は、適用規格に変更

        // *********************************
        // ***  材質はＳＵＳ３０４固定
        // *********************************
        P_Zaisitu = "ＳＵＳ３０４";
        P_EZaisitu = "ＳＵＳ３０４";

        // *********************************
        // ***  予備品数 取得
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

        // ログ
        G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  付属品ﾃﾞｰﾀ(銘板)　作成
        // *********************************
        Write_Axsrs_Data(s_Code);
        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 日阪マーク部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_HMark_Data(void) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ

    // 2020.11.24 RX-00日阪マーク追加_S
    //// 2004.04.16 RX-00日阪マーク消去
    //AnsiString s_Key;
    //s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
    //if (s_Key == "RX-00") { // RX-00
    //    return true;
    //}
    // 2020.11.24 RX-00日阪マーク追加_E

    // 材質ｺｰﾄﾞの取得
    P_Sikibetu = "MARK--"; // 部品分類
    P_Katasiki = "日阪マーク"; // 部品型式
    P_Zuban = "Ｅ４２６０４６"; // 図版
    P_SZuban = "Ｅ４２６０４６"; // 作図用図番
    P_Yobihin = ""; // 予備品識別子
    P_Siyou = "200×50"; // 仕様
    P_Siyou2 = ""; // 仕様2
    P_Siyou5 = "";
    P_Num = 1;
    P_Unit = "";

    P_Zaisitu = "テトロン";
    P_EZaisitu = "テトロン";

    // *********************************
    // ***  予備品数 取得
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

    // ログ
    G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
    Write_Log(G_Log_Text);

    // *********************************
    // ***  付属品ﾃﾞｰﾀ(ﾗﾁｪｯﾄｽﾊﾟﾅ)　作成
    // *********************************
    Write_Axsrs_Data(s_Code);

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾎﾞﾙﾄ落下注意ｽﾃｯｶｰ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2012.10.15追加
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Rakka_Data(void) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    AnsiString s_Key;

    s_Key = P_HEADS_DATA[1277].TrimRight();

    if (s_Key == "3") { // ﾎﾞﾙﾄ円形配列ｽﾃｯｶｰ要
    }
    else {
        return true;
    }

    // 材質ｺｰﾄﾞの取得
    P_Sikibetu = "RAKKA-"; // 部品分類
    P_Katasiki = "ボルト落下注意ステッカー"; // 部品型式
    // 2014.03.06 ｽﾃｯｶｰ注記修正
    P_Hosoku = ""; // 部品名称
    // ************************
    P_Zuban = "Ｅ４２９５６４"; // 図版
    P_SZuban = "Ｅ４２９５６４"; // 作図用図番
    P_Yobihin = ""; // 予備品識別子
    P_Siyou = "90×90"; // 仕様
    P_Siyou2 = ""; // 仕様2
    P_Siyou5 = "";
    P_Num = 1;
    P_Unit = "";

    P_Zaisitu = "テトロン";
    P_EZaisitu = "テトロン";

    // *********************************
    // ***  予備品数 取得
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

    // ログ
    G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
    Write_Log(G_Log_Text);

    // *********************************
    // ***  付属品ﾃﾞｰﾀ(ﾗﾁｪｯﾄｽﾊﾟﾅ)　作成
    // *********************************
    Write_Axsrs_Data(s_Code);

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ブラインドフランジ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2004.03.16 追加
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_BFRG_Data() {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    AnsiString s_Key;
    AnsiString s_SQL;
    AnsiString s_Spec;
    AnsiString s_Dno2;
    AnsiString s_WKDno2;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    //AnsiString s_RtMat;
    AnsiString s_GasMat;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
    AnsiString s_SiyouA;
    AnsiString s_SiyouB;
    int iCntA;
    int iCntB;
    // 2010.07.14 斜行流追加
    int iFL;
    // *********************

    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    // *********************************************
    // ***　舶用付属品ﾏｽﾀ(HD_ACCESSORY) 検索
    // *********************************************
    s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);

    // 2015.03.20 OGI
    //   ノズル規格コードの追加に伴って、舶用付属品マスタ(HD_ACCESSORY) の
    //  「SERIES_MODEL」の登録内容は変わったのでBas_AxsrsDetails.cppの
    //   舶用部品展開処理の「CX-10-JIS16K」のハードコードは削除する
    //// 2011.10.28 CX-10-JIS16K追加
    //if (G_KS_Syurui == "ＣＸ" && (G_KS_Model == "１０Ｄ" || G_KS_Model == "１０")) {
    //    if (P_HEADS_DATA[893].TrimRight().SubString(1, 2) == "02") {
    //        // JIS16K
    //        s_Key = s_Key + "-JIS16K";
    //    }
    //}
    // ***************************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ACCESSORY";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ｼﾘｰｽﾞ型式｢" + s_Key +
            "｣ は、舶用付属品ﾏｽﾀ(HD_ACCESSORY)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        P_Hosoku = "";
        // ｺｰﾄﾞの取得
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // 部品ｺｰﾄﾞ
        P_Sikibetu = wkQuery->FieldByName("PL_SYMBOL")->AsString.TrimRight();
        // 部品表識別子
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight(); // 部品名称
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // 材質名称
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // 材質名称
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // 図番
        s_Dno2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        // ストレーナ用図番２
        s_WKDno2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // ストレーナ用図番２
        P_Num = StrToInt(wkQuery->FieldByName("NUMS")->AsString.TrimRight
            ()); // 数量
        P_Unit = "";
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // 予備品識別子

        // 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ、閉止ｼｰﾄｶﾞｽｹｯﾄ、全面座閉止ｼｰﾄｶﾞｽｹｯﾄ以外を除外する
        if (s_Code != "23" && s_Code != "25" && s_Code != "26") {
            wkQuery->Next();
            continue;
        }

        // 2015.06.23 舶用部品重複対応(ノズル規格の種類が追加される項目)
        // ノズルコードの取得
        AnsiString s_Nzl_Kind = "";
        AnsiString sTugite = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
        if (sTugite.ToIntDef(0) >= 1000) {
            s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
        }
        else {
            s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
        }
        AnsiString sNRLKind = wkQuery->FieldByName("NRL_KIND")->AsString.TrimRight();
        // PARTS_CODEが以下であればノズル規格比較を行う
        // 23 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
        // 25 閉止ｼｰﾄｶﾞｽｹｯﾄ
        // 26 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
        if( s_Nzl_Kind != sNRLKind ){
            wkQuery->Next();
            continue;
        }
        // *************************************************************

        iCntA = 0;
        iCntB = 0;
        // 2010.07.14 斜行流追加
        // 2022.04.21 CX-01D追加_S
        if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ") {
            iFL = 1; // 斜行流
        }
        //if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
        // 2022.04.21 CX-01D追加_E
        else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
            iFL = 1; // 斜行流
        }
        // 2017.07.17 斜行流追加
        else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
            iFL = 1; // 斜行流
        }
        // *********************
        else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
            iFL = 1; // 斜行流
        }
        // 2017.07.17 斜行流追加
        else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
            iFL = 1; // 斜行流
        }
        // *********************
        else {
            iFL = 0; // 台形流
        }
        // 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && (s_Code == "23")) {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "23") {
        // ***********************
            // S1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[891].TrimRight() == "6" ||
                P_HEADS_DATA[891].TrimRight() == "7") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num;
            }
            // S2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[906].TrimRight() == "6" ||
                P_HEADS_DATA[906].TrimRight() == "7") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num;
            }
            // S3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "6" ||
                P_HEADS_DATA[921].TrimRight() == "7") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "6" ||
                P_HEADS_DATA[936].TrimRight() == "7") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[951].TrimRight() == "6" ||
                P_HEADS_DATA[951].TrimRight() == "7") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num;
            }
            // E2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[966].TrimRight() == "6" ||
                P_HEADS_DATA[966].TrimRight() == "7") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num;
            }
            // E3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "6" ||
                P_HEADS_DATA[981].TrimRight() == "7") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "6" ||
                P_HEADS_DATA[996].TrimRight() == "7") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // if( (P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // *********************
        }
        // 閉止ｼｰﾄｶﾞｽｹｯﾄ、全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && (s_Code == "25" ||
        //    s_Code == "26")) {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            (s_Code == "25" || s_Code == "26")) {
        // ***********************
            // 2010.07.14 斜行流追加
            // S1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[891].TrimRight() == "6" ||
                P_HEADS_DATA[891].TrimRight() == "7") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    iCntB += 0;
                }
                else {
                    iCntB += P_Num;
                }
            }
            // S2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[906].TrimRight() == "6" ||
                P_HEADS_DATA[906].TrimRight() == "7") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    iCntA += 0;
                }
                else {
                    iCntA += P_Num;
                }
            }
            // S3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "6" ||
                P_HEADS_DATA[921].TrimRight() == "7") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
            }
            // S4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "6" ||
                P_HEADS_DATA[936].TrimRight() == "7") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
            }
            // E1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[951].TrimRight() == "6" ||
                P_HEADS_DATA[951].TrimRight() == "7") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    iCntB += 0;
                }
                else {
                    iCntB += P_Num;
                }
            }
            // E2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[966].TrimRight() == "6" ||
                 P_HEADS_DATA[966].TrimRight() == "7") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "7")) {
                if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    iCntA += 0;
                }
                else {
                    iCntA += P_Num;
                }
            }
            // E3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "6" ||
                P_HEADS_DATA[981].TrimRight() == "7") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
            }
            // E4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "6" ||
                P_HEADS_DATA[996].TrimRight() == "7") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  //ラバーブーツ
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        iCntA += P_Num;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  //ラバーブーツ
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        iCntB += P_Num;
                    }
                }
            }
            // if( (P_HEADS_DATA[891].TrimRight() == "6" || P_HEADS_DATA[891].TrimRight() == "7") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[906].TrimRight() == "6" || P_HEADS_DATA[906].TrimRight() == "7") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[921].TrimRight() == "6" || P_HEADS_DATA[921].TrimRight() == "7") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[936].TrimRight() == "6" || P_HEADS_DATA[936].TrimRight() == "7") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[951].TrimRight() == "6" || P_HEADS_DATA[951].TrimRight() == "7") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[966].TrimRight() == "6" || P_HEADS_DATA[966].TrimRight() == "7") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[981].TrimRight() == "6" || P_HEADS_DATA[981].TrimRight() == "7") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntB+=0;
            // }else{
            // iCntB+=P_Num;
            // }
            // }
            // if( (P_HEADS_DATA[996].TrimRight() == "6" || P_HEADS_DATA[996].TrimRight() == "7") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            // if(P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1") {  //ラバーブーツ
            // iCntA+=0;
            // }else{
            // iCntA+=P_Num;
            // }
            // }
            // *********************
            if (s_Code == "25") { // 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞSS400以外の場合閉止ｼｰﾄｶﾞｽｹｯﾄ
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (P_HEADS_DATA[265].ToIntDef(0) == 1040) {
                if (Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                // ***********************
                    iCntA = 0;
                    iCntB = 0;
                }
            }
            else if (s_Code == "26") { // 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞSS400の場合全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
                // 2018.02.09 材質判定修正_S
                //// 2016.11.07 材質ｺｰﾄﾞ変更
                ////if (P_HEADS_DATA[265].ToIntDef(0) != 1040) {
                //if (Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                //// ***********************
                if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                // 2018.02.09 材質判定修正_E
                    iCntA = 0;
                    iCntB = 0;
                }
            }
        }

        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_S　
        // E側ﾎﾞﾙﾄﾅｯﾄ
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "27") {
            // S1
            if ((P_HEADS_DATA[891].TrimRight() == "6" ||
                 P_HEADS_DATA[891].TrimRight() == "7") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += P_Num;
            }
            // S2
            if ((P_HEADS_DATA[906].TrimRight() == "6" ||
                 P_HEADS_DATA[906].TrimRight() == "7") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += P_Num;
            }
            // S3
            if ((P_HEADS_DATA[921].TrimRight() == "6" ||
                 P_HEADS_DATA[921].TrimRight() == "7") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            if ((P_HEADS_DATA[936].TrimRight() == "6" ||
                 P_HEADS_DATA[936].TrimRight() == "7") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            if ((P_HEADS_DATA[951].TrimRight() == "6" ||
                 P_HEADS_DATA[951].TrimRight() == "7") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += P_Num;
            }
            // E2
            if ((P_HEADS_DATA[966].TrimRight() == "6" ||
                 P_HEADS_DATA[966].TrimRight() == "7") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += P_Num;
            }
            // E3
            if ((P_HEADS_DATA[981].TrimRight() == "6" ||
                 P_HEADS_DATA[981].TrimRight() == "7") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if ((P_HEADS_DATA[996].TrimRight() == "6" ||
                 P_HEADS_DATA[996].TrimRight() == "7") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
        }
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_E　

        // ｺｰﾄﾞがない 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼ または、閉止ｼｰﾄｶﾞｽｹｯﾄ
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_S　
        //if (P_HEADS_DATA[265].ToIntDef(0) == 0 && (s_Code == "23" ||
        //    s_Code == "25" || s_Code == "26")) {
        if (P_HEADS_DATA[265].ToIntDef(0) == 0 && (s_Code == "23" ||
            s_Code == "25"|| s_Code == "26" || s_Code == "27")) {
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_E　
            wkQuery->Next();
            continue;
        }

        // *********************************
        // ***  予備品数 取得
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

        // ログ
        G_Log_Text = "部品ｺｰﾄﾞ           ｢" + s_Code + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
        Write_Log(G_Log_Text);

        if (s_Code == "23") { // 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
            P_Siyou = P_HEADS_DATA[1370].TrimRight();
            P_Siyou2 = "";
            P_Siyou5 = "A";
            P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
            P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";
            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // 2005.01.07 全面座閉止ｼｰﾄｶﾞｽｹｯﾄの場合材質変更
        // if( s_Code == "25" || s_Code == "26" ){ // 閉止ｼｰﾄｶﾞｽｹｯﾄ、全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
        if (s_Code == "25") { // 閉止ｼｰﾄｶﾞｽｹｯﾄ
            // ********************************************
            P_Siyou = P_HEADS_DATA[1370].TrimRight();
            P_Siyou2 = "";
            P_Siyou5 = "A";
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 2));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 2));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";
            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 2));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 2));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // 2005.01.07 全面座閉止ｼｰﾄｶﾞｽｹｯﾄの場合材質変更
        if (s_Code == "26") { // 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
            P_Siyou = P_HEADS_DATA[1370].TrimRight();
            P_Siyou2 = "";
            P_Siyou5 = "A";
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 4));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[836].TrimRight(), 4));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 4);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";

            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
            //P_Zaisitu =
            //    Search_HD_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 4));
            //P_EZaisitu =
            //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
            //    (P_HEADS_DATA[835].TrimRight(), 4));
            s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
            s_GasMat = Chg_Zaisitu_Code(s_GasMat, 4);
            P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
            P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
            //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // ********************************************
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_S　
        if (s_Code == "27") { // E側ﾎﾞﾙﾄﾅｯﾄ
            P_Siyou5 = "A";
            P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
            P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            // 2022.09.21 E側ボルトナットSUS指定_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 E側ボルトナットSUS指定_E
            P_Num = iCntA;
            P_Unit = "";
            P_Hosoku = "";
            if (iCntA > 0)
                Write_Axsrs_Data(s_Code);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Axsrs_Data(s_Code);
        }
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_E　

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 舶用部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_Fune_Data(bool bMflg) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    AnsiString s_Key;
    AnsiString s_SQL;
    AnsiString s_Spec;
    AnsiString s_Dno2;
    AnsiString s_WKDno2;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    //AnsiString s_RtMat;
    AnsiString s_GasMat;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
    int iCnt; // 2002/10/24
    int iCnt2; // 2004.05.31
    int iCntA; // 2004.06.30
    int iCntB; // 2004.06.30
    int iCnt2A; // 2008.01.18
    int iCnt2B; // 2008.01.18
    // 2010.07.14 斜行流追加
    int iFL;
    // *********************
    // 2016.08.30 材質ｺｰﾄﾞ変更
    AnsiString s_TextA;
    AnsiString s_TextB;
    // ***********************
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
    AnsiString s_GasSiyou;
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    // 2015.03.20 OGI 舶用部品重複対応(ノズル規格の種類が追加される項目)
    // ノズルコードの取得
    AnsiString s_Nzl_Kind = "";
    AnsiString sTugite = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
    if (sTugite.ToIntDef(0) >= 1000) {
        s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
    }
    else {
        s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
    }

    // *********************************************
    // ***　舶用付属品ﾏｽﾀ(HD_ACCESSORY) 検索
    // *********************************************
    // 検索キー変更 02.08.30
    // s_Key = G_K_Syurui + "-" + G_K_Model1;
    s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);

    // 2015.03.20 OGI
    //   ノズル規格コードの追加に伴って、舶用付属品マスタ(HD_ACCESSORY) の
    //  「SERIES_MODEL」の登録内容は変わったのでBas_AxsrsDetails.cppの
    //   舶用部品展開処理の「CX-10-JIS16K」のハードコードは削除する
    //// 2011.10.28 CX-10-JIS16K追加
    //if (G_KS_Syurui == "ＣＸ" && (G_KS_Model == "１０Ｄ" || G_KS_Model == "１０")) {
    //  if (P_HEADS_DATA[893].TrimRight().SubString(1, 2) == "02") {
    //      // JIS16K
    //      s_Key = s_Key + "-JIS16K";
    //  }
    //}
    // ***************************

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ACCESSORY";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ｼﾘｰｽﾞ型式｢" + s_Key +
            "｣ は、舶用付属品ﾏｽﾀ(HD_ACCESSORY)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        P_Hosoku = "";
        // ｺｰﾄﾞの取得
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // 部品ｺｰﾄﾞ
        s_Spec = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight(); // 仕様
        P_Sikibetu = wkQuery->FieldByName("PL_SYMBOL")->AsString.TrimRight();
        // 部品表識別子
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight(); // 部品名称
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // 材質名称
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // 材質名称
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // 図番
        s_Dno2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        // ストレーナ用図番２
        s_WKDno2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // ストレーナ用図番２
        P_Num = StrToInt(wkQuery->FieldByName("NUMS")->AsString.TrimRight
            ()); // 数量
        P_Unit = "";
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // 予備品識別子


        // 2015.03.20 OGI 舶用部品重複対応(ノズル規格の種類が追加される項目)
        AnsiString sNRLKind = wkQuery->FieldByName("NRL_KIND")->AsString.TrimRight();
        if( s_Code == "18" || s_Code == "19" || s_Code == "22" ||
            s_Code == "24" || s_Code == "27" || s_Code == "28" ) {
            // PARTS_CODEが以下であればノズル規格比較を行う
            // 18 S側ｽﾀｯﾄﾞBN
            // 19 端管廻り止め
            // 22 相ﾌﾗﾝｼﾞ
            // 24 ｼｰﾄｶﾞｽｹｯﾄ
            // 27 E側スタッドボルトナット
            // 28 ｽﾀｯﾄﾞﾎﾞﾙﾄ用ﾗｲﾅｰ
            if( s_Nzl_Kind != sNRLKind ){
                wkQuery->Next();
                continue;
            }
        }

        // 2015.06.23 舶用部品重複対応(ノズル規格の種類が追加される項目)
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_S　
        //if (s_Code == "23" || s_Code == "25" || s_Code == "26") {
        //    // PARTS_CODEが以下であればノズル規格比較を行う
        //    // 23 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
        //    // 25 閉止ｼｰﾄｶﾞｽｹｯﾄ
        //    // 26 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
        if (s_Code == "23" || s_Code == "25" || s_Code == "26" || s_Code ==
           "27") {
            // PARTS_CODEが以下であればノズル規格比較を行う
            // 23 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
            // 25 閉止ｼｰﾄｶﾞｽｹｯﾄ
            // 26 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
            // 27 E側ﾎﾞﾙﾄﾅｯﾄ
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_E
            if( s_Nzl_Kind != sNRLKind ){
                wkQuery->Next();
                continue;
            }
        }
        // *************************************************************

        // 2015.03.23 OGI 舶用部品欄に「移動止メ、洗浄ブラシ、ゲージプレート」が出力されないように対策
        if( s_Code == "14" || s_Code == "15" || s_Code == "16" ) {
            //14：移動止ﾒ
            //15：洗浄ﾌﾞﾗｼ
            //16：ｹﾞｰｼﾞﾌﾟﾚｰﾄ
            if ( (P_HEADS_DATA[262].ToIntDef(0) == 1) || (P_HEADS_DATA[262].ToIntDef(0) == 2) ) {
                wkQuery->Next();
                continue;
            }
        }

        // 舶用でない場合は、相フランジ関係、閉止フランジ関係以外を除外する
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_S　
        //if (bMflg == false &&
        //    (s_Code != "22" && s_Code != "23" && s_Code != "24" && s_Code !=
        //    "25")) {
        if (bMflg == false &&
            (s_Code != "22" && s_Code != "23" && s_Code != "24" && s_Code !=
            "25" && s_Code != "27")) {
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_E
            wkQuery->Next();
            continue;
        }
        // ↓2002/10/24
        // ｺｰﾄﾞがある 相フランジ または、ｼｰﾄｶﾞｽｹｯﾄ
        iCnt = 0;
        // 2017.11.22 片側ﾘﾝｸﾞ端管_S
        //// 2003.10.22 端管有無条件追加
        if( s_Code == "19" ) {
        //if (s_Code == "19" && P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
        // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            // 2016.08.30 材質ｺｰﾄﾞ変更
            //// ***************************
            //// 2004.07.07 端管材質データ位置変更
            //if (P_HEADS_DATA[1060].Pos("1051") > 0) { // 1051 STPG
            //    // if ( P_HEADS_DATA[138].Pos( "STPG" ) > 0 ) {
            //    // *********************************
            //    // 2004.05.13 舶用端管材質追加
            //    // 2004.07.07 端管材質データ位置変更
            //}
            //else if (P_HEADS_DATA[1060].Pos("1043") > 0) { // 1043 CAC703
            //    // } else if ( P_HEADS_DATA[138].Pos( "1043" ) > 0 ) {   //1043 CAC703
            //    // *********************************
            //    // 2004.06.21 舶用端管材質変更
            //    // 2004.07.07 端管材質データ位置変更
            //}
            //else if (P_HEADS_DATA[1060].Pos("1151") > 0) { // 1151 STKM13A
            //    // } else if ( P_HEADS_DATA[138].Pos( "1151" ) > 0 ) {   //1151 STKM13A
            //    // *********************************
            //    // } else if ( P_HEADS_DATA[138].Pos( "1052" ) > 0 ) {   //1052 STKM13A
            //    // ***************************
            //    // ***************************
            //}
            //else {
            //    // 2004.07.07 端管材質データ位置変更
            //    if (P_HEADS_DATA[1060].Pos("1014") > 0) { // 1014 TP270の場合はＴＰ３４０
            //        // if ( P_HEADS_DATA[138].Pos( "TP" ) > 0 ) {         //端管がチタン系材質の場合はＴＰ３４０
            //        // *********************************
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // 2004.07.07 端管材質データ位置変更
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1015") > 0)
            //    { // 1015 TP270PDの場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1032") > 0)
            //    { // 1032 TP340の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1033") > 0)
            //    { // 1033 TP340PDの場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1057") > 0)
            //    { // 1057 TP480の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1058") > 0)
            //    { // 1058 TP480PDの場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // *********************************
            //        // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1075") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．７の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1089") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．２の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1090") > 0)
            //    { // 端管がＢ－２６５－Ｇｒ．２の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // *************************************************
            //        // 2010.12.02 SB-265-Gr.11追加
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1076") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．１１の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // ***************************
            //        // 2004.01.28 ASME
            //        // 2004.07.07 端管材質データ位置変更
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1085") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // }else if ( P_HEADS_DATA[138].Pos( "1085" ) > 0 ) { //端管がＳＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // *********************************
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // 2004.07.07 端管材質データ位置変更
            //    }
            //    else if (P_HEADS_DATA[1060].Pos("1095") > 0)
            //    { // 端管がＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // }else if ( P_HEADS_DATA[138].Pos( "1095" ) > 0 ) { //端管がＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // *********************************
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // ***************
            //    }
            //    else {
            //        P_Zaisitu = "ＳＵＳ３０４";
            //        P_EZaisitu = "ＳＵＳ３０４";
            //    }
            //    P_Sikibetu = "RSTP-B";
            //    P_Num = 4;
            //    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
            //    Write_Funeyou_Data(s_Code, s_Spec);
            //}
            // 2004.07.07 端管材質データ位置変更
            //if (P_HEADS_DATA[1061].Pos("1051") > 0) { // 1051 STPG
            //    // if ( P_HEADS_DATA[139].Pos( "STPG" ) > 0 ) {
            //    // *********************************
            //    wkQuery->Next();
            //    continue;
            //    // 2004.05.13 舶用端管材質追加
            //    // 2004.07.07 端管材質データ位置変更
            //}
            //else if (P_HEADS_DATA[1061].Pos("1043") > 0) { // 1043 CAC703
            //    // } else if ( P_HEADS_DATA[139].Pos( "1043" ) > 0 ) {   //1043 CAC703
            //    // *********************************
            //    wkQuery->Next();
            //    continue;
            //    // 2004.06.21 舶用端管材質変更
            //    // 2004.07.07 端管材質データ位置変更
            //}
            //else if (P_HEADS_DATA[1061].Pos("1151") > 0) { // 1151 STKM13A
            //    // } else if ( P_HEADS_DATA[139].Pos( "1151" ) > 0 ) {   //1151 STKM13A
            //    // *********************************
            //    // } else if ( P_HEADS_DATA[139].Pos( "1052" ) > 0 ) {   //1052 STKM13A
            //    // ***************************
            //    wkQuery->Next();
            //    continue;
            //    // ***************************
            //}
            //else {
            //    // 2004.07.07 端管材質データ位置変更
            //    if (P_HEADS_DATA[1061].Pos("1014") > 0) { // 1014 TP270の場合はＴＰ３４０
            //        // if ( P_HEADS_DATA[139].Pos( "TP" ) > 0 ) {         //端管がチタン系材質の場合はＴＰ３４０
            //        // *********************************
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // 2004.07.07 端管材質データ位置変更
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1015") > 0)
            //    { // 1015 TP270PDの場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1032") > 0)
            //    { // 1032 TP340の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1033") > 0)
            //    { // 1033 TP340PDの場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1057") > 0)
            //    { // 1057 TP480の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1058") > 0)
            //    { // 1058 TP480PDの場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // *********************************
            //        // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1075") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．７の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1089") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．２の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1090") > 0)
            //    { // 端管がＢ－２６５－Ｇｒ．２の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // *************************************************
            //        // 2010.12.02 SB-265-Gr.11追加
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1076") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．１１の場合はＴＰ３４０
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // *************************************************
            //        // **************************
            //        // 2004.01.28 ASME
            //        // 2004.07.07 端管材質データ位置変更
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1085") > 0)
            //    { // 端管がＳＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // }else if ( P_HEADS_DATA[139].Pos( "1085" ) > 0 ) { //端管がＳＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // *********************************
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // 2004.07.07 端管材質データ位置変更
            //    }
            //    else if (P_HEADS_DATA[1061].Pos("1095") > 0)
            //    { // 端管がＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // }else if ( P_HEADS_DATA[139].Pos( "1095" ) > 0 ) { //端管がＢ－２６５－Ｇｒ．１の場合はＴＰ３４０
            //        // *********************************
            //        P_Zaisitu = "ＴＰ３４０";
            //        P_EZaisitu = "ＴＰ３４０";
            //        // ***************
            //    }
            //    else {
            //        P_Zaisitu = "ＳＵＳ３０４";
            //        P_EZaisitu = "ＳＵＳ３０４";
            //    }
            //    P_Num = 4;
            //    P_Sikibetu = "RSTP-A";
            //}
            s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            s_TextB = Type_MAT(s_TextB);
            if ( s_TextB == "STPG" ) {      // ＳＴＰＧ
                // 端管廻り止めなし
            }
            else if ( s_TextB == "ALBC" ) { // ＡＬＢＣ
                // 端管廻り止めなし
            }
            else if ( s_TextB == "CS" ) {   // ＣＳ(ＳＴＫＭ１３Ａ)
                // 端管廻り止めなし
            }
            // 2019.05.15 SGP端管追加_S
            else if ( s_TextB == "SGP") {   // ＳＧＰ
                // 端管廻り止めなし
            }
            // 2019.04.12 SGP端管追加_E
            // 2017.11.22 片側ﾘﾝｸﾞ端管_S
            else if ( s_TextB == "-" ) {    // 材質未指定
                // 端管なし
            }
            // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            else {
                // 2021.05.17 端管廻り止め廃止_S
                //if( s_TextB == "Ti" ) {     // チタン
                //    P_Zaisitu = "ＴＰ３４０";
                //    P_EZaisitu = "ＴＰ３４０";
                //} else {
                //    P_Zaisitu = "ＳＵＳ３０４";
                //    P_EZaisitu = "ＳＵＳ３０４";
                //}
                //P_Sikibetu = "RSTP-B";
                //P_Num = 4;
                //P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                //Write_Funeyou_Data(s_Code, s_Spec);
                // 2021.05.17 端管廻り止め廃止_E
            }

            s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            s_TextA = Type_MAT(s_TextA);
            if ( s_TextA == "STPG" ) {      // ＳＴＰＧ
                // 端管廻り止めなし
                wkQuery->Next();
                continue;
            }
            else if ( s_TextA == "ALBC" ) { // ＡＬＢＣ
                // 端管廻り止めなし
                wkQuery->Next();
                continue;
            }
            else if ( s_TextA == "CS" ) {   // ＣＳ(ＳＴＫＭ１３Ａ)
                // 端管廻り止めなし
                wkQuery->Next();
                continue;
            }
            // 2019.05.15 SGP端管追加_S
            else if ( s_TextA == "SGP") {   // ＳＧＰ
                // 端管廻り止めなし
                wkQuery->Next();
                continue;
            }
            // 2019.04.12 SGP端管追加_E
            // 2017.11.22 片側ﾘﾝｸﾞ端管_S
            else if ( s_TextA == "-" ) {    // 材質未指定
                // 端管なし
                wkQuery->Next();
                continue;
            }
            // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            else {
                // 2021.05.17 端管廻り止め廃止_S
                //if( s_TextA == "Ti" ) {     // チタン
                //    P_Zaisitu = "ＴＰ３４０";
                //    P_EZaisitu = "ＴＰ３４０";
                //} else {
                //    P_Zaisitu = "ＳＵＳ３０４";
                //    P_EZaisitu = "ＳＵＳ３０４";
                //}
                //P_Sikibetu = "RSTP-A";
                //P_Num = 4;
                wkQuery->Next();
                continue;
                // 2021.05.17 端管廻り止め廃止_E
            }
            // ***********************
        }
        // 2004.05.31 ｽﾀｯﾄﾞﾎﾞﾙﾄ用ﾗｲﾅｰ追加
        // 2017.11.22 片側ﾘﾝｸﾞ端管_S
        //if (s_Code == "28" && P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
        if (s_Code == "28") {
        // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            iCnt2 = 0;
            // 2016.08.30 材質ｺｰﾄﾞ変更
            //// 2004.06.21 舶用端管材質変更
            //// 2004.07.02 端管材質データ位置変更
            //if (P_HEADS_DATA[1060].Pos("1043") > 0 || P_HEADS_DATA[1060].Pos
            //    ("1151") > 0) { // 1043 CAC703  1151 STKM13A
            //    // if ( P_HEADS_DATA[138].Pos( "1043" ) > 0 || P_HEADS_DATA[138].Pos( "1151" ) > 0) {  //1043 CAC703  1151 STKM13A
            //    // *********************************
            //    iCnt2 += P_Num;
            //}
            //// 2004.07.02 端管材質データ位置変更
            //if (P_HEADS_DATA[1061].Pos("1043") > 0 || P_HEADS_DATA[1061].Pos
            //    ("1151") > 0) { // 1043 CAC703  1151 STKM13A
            //    // if ( P_HEADS_DATA[139].Pos( "1043" ) > 0 || P_HEADS_DATA[139].Pos( "1151" ) > 0) {  //1043 CAC703  1151 STKM13A
            //    // *********************************
            //    iCnt2 += P_Num;
            //}
            s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            s_TextB = Type_MAT(s_TextB);
            if ( s_TextB == "ALBC" ) {    // ＡＬＢＣ
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_S
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            }
            else if ( s_TextB == "CS" ) { // ＣＳ(ＳＴＫＭ１３Ａ)
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_S
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            }

            s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            s_TextA = Type_MAT(s_TextA);
            if ( s_TextA == "ALBC" ) {    // ＡＬＢＣ
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_S
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            }
            else if ( s_TextA == "CS" ) { // ＣＳ(ＳＴＫＭ１３Ａ)
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_S
                iCnt2 += P_Num;
                // 2017.11.22 片側ﾘﾝｸﾞ端管_E
            }
            // ***********************
            // if ( P_HEADS_DATA[138].Pos( "1043" ) > 0 || P_HEADS_DATA[138].Pos( "1052" ) > 0) {  //1043 CAC703  1052 STKM13A
            // iCnt2 += P_Num;
            // }
            // if ( P_HEADS_DATA[139].Pos( "1043" ) > 0 || P_HEADS_DATA[139].Pos( "1052" ) > 0) {  //1043 CAC703  1052 STKM13A
            // iCnt2 += P_Num;
            // }
            // ***************************
            if (iCnt2 == 0) {
                wkQuery->Next();
                continue;
            }
            P_Num = iCnt2;
        }
        // ******************************
        // 2010.07.14 斜行流追加
        if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
            iFL = 1; // 斜行流
        }
        else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
            iFL = 1; // 斜行流
        }
        // 2022.11.15 HERP修正_S
        else if  (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ") {
            iFL = 1; // 斜行流
        }
        else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
            iFL = 1; // 斜行流
        }
        else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
            iFL = 1; // 斜行流
        }
        // 2022.11.15 HERP修正_E
        else {
            iFL = 0; // 台形流
        }
        // *********************
        // 2004.06.29 相フランジAB区分追加
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1068].ToIntDef(0) != 0 && (s_Code == "22")) {
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) != "-" &&
            s_Code == "22") {
        // ***********************
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 斜行流追加
            // S1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[891].TrimRight() == "1" ||
                P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num;
            }
            // S2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[906].TrimRight() == "1" ||
                 P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num;
            }
            // S3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "1" ||
                 P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[951].TrimRight() == "1" ||
                 P_HEADS_DATA[951].TrimRight() == "2") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntB += P_Num; // E1
            }
            // E2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[966].TrimRight() == "1" ||
                 P_HEADS_DATA[966].TrimRight() == "2") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                iCntA += P_Num; // E2
            }
            // E3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "1" ||
                 P_HEADS_DATA[981].TrimRight() == "2") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "1" ||
                P_HEADS_DATA[996].TrimRight() == "2") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // *********************
            P_Siyou5 = "A";
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if (P_HEADS_DATA[1068].ToIntDef(0) > 0) {
            if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) != "-") {
            // ***********************
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
            }
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }
        // 2008.01.18 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
        if (s_Code == "24") {
            iCntA = 0;
            iCntB = 0;
            iCnt2A = 0;
            iCnt2B = 0;
        }
        // ***************************************
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1068].ToIntDef(0) != 0 && (s_Code == "24")) {
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) != "-" &&
            s_Code == "24") {
        // ***********************
            // 2010.07.14 CX-10ﾘﾝｸﾞ端管斜行流追加
            // 相フランジ用シートガスケット
            // S1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[891].TrimRight() == "1" ||
                P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
                else {
                    iCntB += 1;
                }
            }
            // S2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[906].TrimRight() == "1" ||
                P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // 端管が付く
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
                else {
                    iCntA += 1;
                }
            }
            // S3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
            }
            // S4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[936].TrimRight() == "1" ||
                P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
            }
            // E1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[951].TrimRight() == "1" ||
                P_HEADS_DATA[951].TrimRight() == "2") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // 端管が付く
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
                else {
                    iCntB += 1;
                }
            }
            // E2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {      // ラバーブーツ
            if ((P_HEADS_DATA[966].TrimRight() == "1" ||
                P_HEADS_DATA[966].TrimRight() == "2") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    // ***********************
                    if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // 端管が付く
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
                else {
                    iCntA += 1;
                }
            }
            // E3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[981].TrimRight() == "1" ||
                 P_HEADS_DATA[981].TrimRight() == "2") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
            }
            // E4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            if ((P_HEADS_DATA[996].TrimRight() == "1" ||
                 P_HEADS_DATA[996].TrimRight() == "2") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                    else {
                        iCntA += 1;
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {  // ラバーブーツ
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        // ***********************
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                    else {
                        iCntB += 1;
                    }
                }
            }
            ////2008.01.18 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
            // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=P_Num;
            // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=P_Num;
            ////    if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////    if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////if( P_HEADS_DATA[1068].ToIntDef(0) != 0 && (s_Code == "22" || s_Code == "24") ){
            //////    if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            //////    if ( s_Code == "22" ) {       //相フランジの材質指定有り
            //////        if ( P_HEADS_DATA[1068].ToIntDef(0) > 0 ) {
            //////           P_Zaisitu  = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
            //////           P_EZaisitu  = Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
            //////        }
            //////    }
            ////////2004.04.16 ストレーナＳ側抜止め用及び端管用シートガスケット変更
            //////////2003.10.17 ストレーナカバー用シートガスケット追加
            ////////    if ( s_Code == "24" ) {       //シートガスケット
            ////////        if ( s_Key == "UX-40" || s_Key == "LX-10" ||s_Key == "LX-40" ) {       // UX-10,LX-10,LX-40
            ////////            //Ｅ３ストレーナ＆Ｓ３端管
            ////////            if( (P_HEADS_DATA[981].TrimRight() == "5" ) && (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////////            //Ｅ４ストレーナ＆Ｓ４端管
            ////////            if( (P_HEADS_DATA[996].TrimRight() == "5" ) && (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=P_Num;
            ////////        }
            ////////    }
            //////////*************************************************
            //////////2004.02.24 端管用シートガスケット追加
            ////////    if( s_Code == "24"){ //　端管用シートガスケット
            ////////        if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[893].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[908].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[923].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[938].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////        if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            ////////            if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "3") iCnt+=P_Num;
            ////////        }
            ////////    }
            ////////***************************************************************
            //////*******************************
            ////    P_Num = iCnt;
            ////***************************************
            // **********************************
        }

        // 2004.04.16 ストレーナＳ側抜止め用及び端管用シートガスケット変更
        if (s_Code == "24") { // シートガスケット
            // ストレーナＳ側抜止め用シートガスケット
            // 2009.10.28 ストレーナＳ側抜止め用シートガスケット変更
            // すべての型式にＳ側抜止めが存在する
            ////2009.06.19 ストレーナＳ側抜止め用シートガスケット変更
            ////2007.02.13 ストレーナＳ側抜止め用シートガスケット変更
            ////if ( s_Key != "UX-10" && s_Key != "UX-90" && s_Key != "RX-70" ) {       // UX-10,UX-90,RX-70以外はＳ側抜止めが存在する
            ////if ( s_Key != "UX-90" && s_Key != "UX-100" && s_Key != "UX-130" &&
            ////     s_Key != "SX-70" && s_Key != "SX-90L" && s_Key != "SX-90M" &&
            ////     s_Key != "RX-70" && s_Key != "RX-90" ) { // UX-90,100,130,SX-70,90L,90M,RX-70,90以外はＳ側抜止めが存在する
            // if ( s_Key != "UX-90" && s_Key != "UX-100" && s_Key != "UX-100R" &&
            // s_Key != "UX-130" && s_Key != "SX-70" && s_Key != "SX-90L" &&
            // s_Key != "SX-90M" && s_Key != "RX-70" && s_Key != "RX-90" &&
            // s_Key != "RX-100" ) {
            // // UX-90,100,100R,130,SX-70,90L,90M,RX-70,90,100以外はＳ側抜止めが存在する
            // **************************************
            ////**************************************
            // 2017.11.07 E1E2ｽﾄﾚｰﾅ追加_S
            // Ｅ１ストレーナ＆Ｓ１ノズル
            if ((P_HEADS_DATA[951].TrimRight() == "5") &&
                (P_HEADS_DATA[891].TrimRight() == "1" ||
                 P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += 1; // 台形流
            }
            // Ｅ２ストレーナ＆Ｓ２ノズル
            if ((P_HEADS_DATA[966].TrimRight() == "5") &&
                (P_HEADS_DATA[906].TrimRight() == "1" ||
                 P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += 1; // 台形流
            }
            // 2017.11.07 E1E2ｽﾄﾚｰﾅ追加_E
            // 2010.07.14 斜行流追加
            // Ｅ３ストレーナ＆Ｓ３ノズル
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[981].TrimRight() == "5" )
            // && ( P_HEADS_DATA[921].TrimRight() == "1"
            // || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "5") &&
                (P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntB += 1; // 台形流
                }
                else {
                    iCntA += 1; // 斜行流
                }
            }
            // Ｅ４ストレーナ＆Ｓ４ノズル
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[996].TrimRight() == "5" )
            // && ( P_HEADS_DATA[936].TrimRight() == "1"
            // || P_HEADS_DATA[936].TrimRight() == "2")
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "5") &&
                (P_HEADS_DATA[936].TrimRight() == "1" ||
                P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) {
                    iCntA += 1; // 台形流
                }
                else {
                    iCntB += 1; // 斜行流
                }
            }
            ////2008.01.18 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
            ////Ｅ３ストレーナ＆Ｓ３ノズル
            // if( (P_HEADS_DATA[981].TrimRight() == "5" ) && (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
            ////Ｅ４ストレーナ＆Ｓ４ノズル
            // if( (P_HEADS_DATA[996].TrimRight() == "5" ) && (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
            //////Ｅ３ストレーナ＆Ｓ３ノズル
            ////if( (P_HEADS_DATA[981].TrimRight() == "5" ) && (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCnt+=1;
            //////Ｅ４ストレーナ＆Ｓ４ノズル
            ////if( (P_HEADS_DATA[996].TrimRight() == "5" ) && (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCnt+=1;
            ////***************************************
            // *********************

            // }
            // *****************************************************
            // 2009.10.28 ストレーナＥ側ツバ用シートガスケット廃止
            ////2004.06.01 ストレーナＥ側ツバ用シートガスケット変更
            ////ストレーナＥ側ツバ用シートガスケット
            ////2009.06.19 ストレーナＥ側抜止め用シートガスケット変更
            ////2007.02.13 ストレーナＥ側抜止め用シートガスケット変更
            ////if ( s_Key == "UX-90" || s_Key == "RX-70" ) {                           // UX-90,RX-70はＥ側ツバが存在する
            ////if ( s_Key == "UX-90" || s_Key == "UX-100" || s_Key == "UX-130" ||
            ////     s_Key == "SX-70" || s_Key == "SX-90L" || s_Key == "SX-90M" ||
            ////     s_Key == "RX-70" || s_Key == "RX-90" ) { // UX-90,100,130,SX-70,90L,90M,RX-70,90はＥ側ツバが存在する
            // if ( s_Key == "UX-90" || s_Key == "UX-100" || s_Key == "UX-100R" ||
            // s_Key == "UX-130" || s_Key == "SX-70" || s_Key == "SX-90L" ||
            // s_Key == "SX-90M" || s_Key == "RX-70" || s_Key == "RX-90" ||
            // s_Key == "RX-100" ) {
            // // UX-90,100,100R,130,SX-70,90L,90M,RX-70,90,100はＥ側ツバが存在する
            ////**************************************
            ////**************************************
            // //ガスケットはメタル、チューブ、パイプノズルにしかつかない
            // //2008.01.18 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
            // if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCntB+=1;
            // if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCntA+=1;
            // //if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt+=1;
            // //if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt+=1;
            // //***************************************
            // }else if ( s_Key == "LX-10" ) {
            if (s_Key == "LX-10") {
                // ***************************************************
                // 2008.01.18 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
                // iCnt2 = 0;
                // ガスケットはメタル、チューブ、パイプノズルにしかつかない
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt2B+=1;
                // if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt2A+=1;
                if (P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2B += 1;
                if (P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2A += 1;
                // ***********************

                // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                    iCnt2A = iCnt2A + iCnt2B;
                    iCnt2B = 0;
                }
                // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

                if (iCnt2B != 0) {
                    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                    //P_Zaisitu =
                    //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[835].TrimRight(), 2));
                    //P_EZaisitu =
                    //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[835].TrimRight(), 2));
                    s_GasMat = Get_SheetGasketMat("B");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                    P_Num = iCnt2B;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // 仕様
                    P_Siyou5 = "B";

                    // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    if (iCnt2A != 0) {
                        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(B側)
                        if (sSpecCode == "A") {
                            // ミラー仕様 Ａ号機
                            s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                        } else if (sSpecCode == "B") {
                            // ミラー仕様 Ｂ号機
                            s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                        } else {
                            s_Spec = s_Spec + "　Ｂ側用";                 // 仕様
                        }
                    }
                    // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

                    // 2020.12.16 シートガスケット名称変更_S
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                    // 2020.12.16 シートガスケット名称変更_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // 仕様
                }
                if (iCnt2A != 0) {
                    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                    //P_Zaisitu =
                    //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[836].TrimRight(), 2));
                    //P_EZaisitu =
                    //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                    //    (P_HEADS_DATA[836].TrimRight(), 2));
                    s_GasMat = Get_SheetGasketMat("A");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                    P_Num = iCnt2A;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // 仕様
                    P_Siyou5 = "A";

                    // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    if (iCnt2B != 0) {
                        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(A側)
                        if (sSpecCode == "A") {
                            // ミラー仕様 Ａ号機
                            s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                        } else if (sSpecCode == "B") {
                            // ミラー仕様 Ｂ号機
                            s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                        } else {
                            s_Spec = s_Spec + "　Ａ側用";                 // 仕様
                        }
                    }
                    // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

                    // 2020.12.16 シートガスケット名称変更_S
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                    // 2020.12.16 シートガスケット名称変更_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // 仕様
                }
                // if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt2+=1;
                // if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt2+=1;
                // if ( iCnt2 != 0) {
                // s_Spec = "JIS10K-80A";        // 仕様
                // P_Num = iCnt2;
                // P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin,"0", m_pcHeadsData);
                // Write_Funeyou_Data(s_Code, s_Spec);
                // s_Spec = "JIS10K-100A";        // 仕様
                // }
                // ***************************************
            }
            // ***************************************************
            // 端管用シートガスケット
            // 2010.07.14 CX-10ﾘﾝｸﾞ端管斜行流追加
            // S1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
            // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {            // ラバーブーツ
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {     // パイプノズル（フランジ付き）
            if ((P_HEADS_DATA[891].TrimRight() == "1" ||
                 P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    iCntB += 0;
                }
                else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
                { // パイプノズル（フランジ付き）
                    // ***********************
                    iCntB += 0;
                }
                else {
                    if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
            }
            // S2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
            // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {            // ラバーブーツ
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {     // パイプノズル（フランジ付き）
            if ((P_HEADS_DATA[906].TrimRight() == "1" ||
                P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    iCntA += 0;
                }
                else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
                { // パイプノズル（フランジ付き）
                    // ***********************
                    iCntA += 0;
                }
                else {
                    if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // 端管が付く
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
            }
            // S3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
            // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[923].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[923].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
            }
            // S4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
            // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[936].TrimRight() == "1" ||
                 P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[938].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[938].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
            }
            // E1
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
            // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {            // ラバーブーツ
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {     // パイプノズル（フランジ付き）
            if ((P_HEADS_DATA[951].TrimRight() == "1" ||
                 P_HEADS_DATA[951].TrimRight() == "2") &&
                (P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    iCntB += 0;
                }
                else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
                { // パイプノズル（フランジ付き）
                    // ***********************
                    iCntB += 0;
                }
                else {
                    if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // 端管が付く
                        iCntB += 1;
                    }
                    else {
                        iCntB += 0;
                    }
                }
            }
            // E2
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
            // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
            // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {            // ラバーブーツ
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4" ) {     // パイプノズル（フランジ付き）
            if ((P_HEADS_DATA[966].TrimRight() == "1" ||
                P_HEADS_DATA[966].TrimRight() == "2") &&
                (P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1,
                2) != "07")) {
                if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
                { // ラバーブーツ
                    iCntA += 0;
                }
                else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
                { // パイプノズル（フランジ付き）
                    // ***********************
                    iCntA += 0;
                }
                else {
                    if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // 端管が付く
                        iCntA += 1;
                    }
                    else {
                        iCntA += 0;
                    }
                }
            }
            // E3
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
            // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[981].TrimRight() == "1" ||
                P_HEADS_DATA[981].TrimRight() == "2") &&
                (P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[983].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[983].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
            }
            // E4
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
            // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
            // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
            if ((P_HEADS_DATA[996].TrimRight() == "1" ||
                 P_HEADS_DATA[996].TrimRight() == "2") &&
                (P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1,
                2) != "07")) {
                // ***********************
                if (iFL == 0) { // 台形流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntA+=0;
                    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntA += 0;
                    }
                    else if (P_HEADS_DATA[998].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntA += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                            iCntA += 1;
                        }
                        else {
                            iCntA += 0;
                        }
                    }
                }
                else { // 斜行流
                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {        // ラバーブーツ
                    // iCntB+=0;
                    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) { // パイプノズル（フランジ付き）
                    if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
                    { // ラバーブーツ
                        iCntB += 0;
                    }
                    else if (P_HEADS_DATA[998].TrimRight().SubString(3,
                        1) == "4") { // パイプノズル（フランジ付き）
                        // ***********************
                        iCntB += 0;
                    }
                    else {
                        if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                            iCntB += 1;
                        }
                        else {
                            iCntB += 0;
                        }
                    }
                }
            }
            ////2008.01.08 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
            // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[893].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[908].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[923].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[938].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "3") iCntB+=1;
            // }
            // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            // if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "3") iCntA+=1;
            // }
            ////if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[893].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[908].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[923].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[938].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
            ////    if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "3") iCnt+=1;
            ////}
            ////P_Num = iCnt;

            // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
            if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                iCntA = iCntA + iCntB;
                iCntB = 0;
            }
            s_GasSiyou = s_Spec;
            // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

            if (iCntB != 0) {
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("B");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";

                // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                if (iCntA != 0) {
                    // ｼｰﾄｶﾞｽｹｯﾄ貼分け(B側)
                    if (sSpecCode == "A") {
                        // ミラー仕様 Ａ号機
                        s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                    } else if (sSpecCode == "B") {
                        // ミラー仕様 Ｂ号機
                        s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                    } else {
                        s_Spec = s_Spec + "　Ｂ側用";                 // 仕様
                    }
                }
                // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

                // 2020.12.16 シートガスケット名称変更_S
                if ( P_GasNameB == "リングガスケット" ) {
                    // ﾘﾝｸﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                } else if ( P_GasNameB == "全面座フランジガスケット" ) {
                    // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTF";                   // 部品分類
                    P_Katasiki = "全面座フランジガスケット"; // 部品型式
                } else {
                    // その他
                }
                // 2020.12.16 シートガスケット名称変更_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
            s_Spec = s_GasSiyou;
            // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

            if (iCntA != 0) {
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("A");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";


                // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                if (iCntB != 0) {
                    // ｼｰﾄｶﾞｽｹｯﾄ貼分け(A側)
                    if (sSpecCode == "A") {
                        // ミラー仕様 Ａ号機
                        s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                    } else if (sSpecCode == "B") {
                        // ミラー仕様 Ｂ号機
                        s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                    } else {
                        s_Spec = s_Spec + "　Ａ側用";                 // 仕様
                    }
                }
                // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

                // 2020.12.16 シートガスケット名称変更_S
                if ( P_GasNameA == "リングガスケット" ) {
                    // ﾘﾝｸﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                } else if ( P_GasNameA == "全面座フランジガスケット" ) {
                    // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTF";                   // 部品分類
                    P_Katasiki = "全面座フランジガスケット"; // 部品型式
                } else {
                    // その他
                }
                // 2020.12.16 シートガスケット名称変更_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
            s_Spec = s_GasSiyou;
            // 2018.05.28 ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

            P_Num = 0;
            // ***************************************
            // **********************************
        }
        // ***************************************************************

        // 2004.04.15 ストレーナ用シートガスケット変更
        ////ｺｰﾄﾞがある　閉止ﾌﾗﾝｼﾞ　閉止ｼｰﾄｶﾞｽｹｯﾄ
        // if( P_HEADS_DATA[265].ToIntDef(0) != 0 && (s_Code == "23" || s_Code == "25") ){
        // if( P_HEADS_DATA[891].TrimRight() == "5" || P_HEADS_DATA[891].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[906].TrimRight() == "5" || P_HEADS_DATA[906].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[921].TrimRight() == "5" || P_HEADS_DATA[921].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[936].TrimRight() == "5" || P_HEADS_DATA[936].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[951].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[966].TrimRight() == "5" || P_HEADS_DATA[966].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[981].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight() == "6" ) iCnt+=P_Num;
        // if( P_HEADS_DATA[996].TrimRight() == "5" || P_HEADS_DATA[996].TrimRight() == "6" ) iCnt+=P_Num;
        // if ( s_Code == "23" ) {       //閉止フランジの材質指定有り
        // if ( P_HEADS_DATA[265].ToIntDef(0) > 0 ) {
        // P_Zaisitu  = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
        // P_EZaisitu  = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
        // }
        // }
        // P_Num = iCnt;
        // }
        ////↑2002/10/24
        // **************************************************************
        // ｺｰﾄﾞがある　閉止ﾌﾗﾝｼﾞ
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && s_Code == "23") {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" && s_Code == "23") {
        // ***********************
            // 2004.06.29 閉止フランジAB区分追加
            // if( P_HEADS_DATA[891].TrimRight() == "5" || P_HEADS_DATA[891].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "5" || P_HEADS_DATA[906].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "5" || P_HEADS_DATA[921].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "5" || P_HEADS_DATA[936].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[951].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "5" || P_HEADS_DATA[966].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight() == "6" ) iCnt+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "5" || P_HEADS_DATA[996].TrimRight() == "6" ) iCnt+=P_Num;
            ////閉止フランジの材質指定有り
            // if ( P_HEADS_DATA[265].ToIntDef(0) > 0 ) {
            // P_Zaisitu  = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
            // P_EZaisitu  = Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            // }
            // P_Num = iCnt;
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 斜行流追加
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5" ||
                P_HEADS_DATA[891].TrimRight() == "6") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5" ||
                P_HEADS_DATA[906].TrimRight() == "6") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5" ||
                P_HEADS_DATA[921].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5" ||
                P_HEADS_DATA[936].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5" ||
                P_HEADS_DATA[951].TrimRight() == "6") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5" ||
                P_HEADS_DATA[966].TrimRight() == "6") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5" ||
                P_HEADS_DATA[981].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5" ||
                P_HEADS_DATA[996].TrimRight() == "6") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // if( P_HEADS_DATA[891].TrimRight() == "5" || P_HEADS_DATA[891].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "5" || P_HEADS_DATA[906].TrimRight() == "6") iCntA+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "5" || P_HEADS_DATA[921].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "5" || P_HEADS_DATA[936].TrimRight() == "6") iCntA+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[951].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "5" || P_HEADS_DATA[966].TrimRight() == "6") iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight() == "6") iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "5" || P_HEADS_DATA[996].TrimRight() == "6") iCntA+=P_Num;
            // *********************
            P_Siyou5 = "A";
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if (P_HEADS_DATA[265].ToIntDef(0) > 0) {
            if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-") {
            // ***********************
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            }
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
            // *********************************
        }
        // ｺｰﾄﾞがある　閉止ｼｰﾄｶﾞｽｹｯﾄ
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && s_Code == "25") {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "25") {
        // ***********************
            // 2008.01.08 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 斜行流追加
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //// S1
            //if (P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntB += P_Num;
            //}
            //// S2
            //if (P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntA += P_Num;
            //}
            //// S3
            //if (P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // 台形流
            //    }
            //    else {
            //        iCntA += P_Num; // 斜行流
            //    }
            //}
            //// S4
            //if (P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // 台形流
            //    }
            //    else {
            //        iCntB += P_Num; // 斜行流
            //    }
            //}
            //// E1
            //if (P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntB += P_Num;
            //}
            //// E2
            //if (P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    iCntA += P_Num;
            //}
            //// E3
            //if (P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // 台形流
            //    }
            //    else {
            //        iCntA += P_Num; // 斜行流
            //    }
            //}
            //// E4
            //if (P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) != 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // 台形流
            //    }
            //    else {
            //        iCntB += P_Num; // 斜行流
            //    }
            //}
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) != "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // ***********************
            ////ｽﾄﾚｰﾅ以外で閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞSS400以外の場合閉止ｼｰﾄｶﾞｽｹｯﾄ
            // if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCntA+=P_Num;
            // *********************
            ////2004.06.25 閉止ｼｰﾄｶﾞｽｹｯﾄ変更
            //////2004.06.01 ストレーナＥ側ツバ用シートガスケット変更
            ////////ｽﾄﾚｰﾅはﾒﾀﾙ,ﾁｭｰﾌﾞ,ﾊﾟｲﾌﾟﾉｽﾞﾙにしかつかない
            //////if( P_HEADS_DATA[951].TrimRight() == "5" && P_HEADS_DATA[953].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////if( P_HEADS_DATA[966].TrimRight() == "5" && P_HEADS_DATA[968].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////if( P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////if( P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998].TrimRight().SubString(2,1) != "1") iCnt+=P_Num;
            //////***************************************************
            //////ｽﾄﾚｰﾅ以外はすべてつく
            ////if( P_HEADS_DATA[891].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[906].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[921].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[936].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[951].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[966].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[981].TrimRight() == "6" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[996].TrimRight() == "6" ) iCnt+=P_Num;
            ////ｽﾄﾚｰﾅ以外で閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞSS400以外の場合閉止ｼｰﾄｶﾞｽｹｯﾄ
            // if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) != 1040 ) iCnt+=P_Num;
            ////****************************
            // P_Num = iCnt;
            if (iCntB != 0) {
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("B");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            if (iCntA != 0) {
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("A");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            P_Num = 0;
            // ***************************************
        }
        // ｺｰﾄﾞがある　全面座閉止ｶﾞｽｹｯﾄ
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[265].ToIntDef(0) != 0 && s_Code == "26") {
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "26") {
        // ***********************
            // 2008.01.08 ｶﾞｽｹｯﾄ材質をAB両側で読み込む
            iCntA = 0;
            iCntB = 0;
            // 2010.07.14 斜行流追加
            // ｽﾄﾚｰﾅははすべてつく
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // ｽﾄﾚｰﾅ以外で閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞSS400の場合全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //// S1
            //if (P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntB += P_Num;
            //}
            //// S2
            //if (P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntA += P_Num;
            //}
            //// S3
            //if (P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // 台形流
            //    }
            //    else {
            //        iCntA += P_Num; // 斜行流
            //    }
            //}
            // S4
            //if (P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // 台形流
            //    }
            //    else {
            //        iCntB += P_Num; // 斜行流
            //    }
            //}
            //// E1
            //if (P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntB += P_Num;
            //}
            //// E2
            //if (P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    iCntA += P_Num;
            //}
            //// E3
            //if (P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntB += P_Num; // 台形流
            //    }
            //    else {
            //        iCntA += P_Num; // 斜行流
            //    }
            //}
            //// E4
            //if (P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265]
            //    .ToIntDef(0) == 1040) {
            //    if (iFL == 0) {
            //        iCntA += P_Num; // 台形流
            //    }
            //    else {
            //        iCntB += P_Num; // 斜行流
            //    }
            //}
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "6" &&
                Type_MAT(P_HEADS_DATA[265].TrimRight()) == "SS400") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // ***********************
            ////ｽﾄﾚｰﾅははすべてつく
            // if( P_HEADS_DATA[951].TrimRight() == "5" ) iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "5" ) iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "5" ) iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "5" ) iCntA+=P_Num;
            ////ｽﾄﾚｰﾅ以外で閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞSS400の場合全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
            // if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            // if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntB+=P_Num;
            // if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCntA+=P_Num;
            //////ｽﾄﾚｰﾅははすべてつく
            ////if( P_HEADS_DATA[951].TrimRight() == "5" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[966].TrimRight() == "5" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[981].TrimRight() == "5" ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[996].TrimRight() == "5" ) iCnt+=P_Num;
            //////2004.06.25 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ変更
            //////ｽﾄﾚｰﾅ以外で閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞSS400の場合全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
            ////if( P_HEADS_DATA[891].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[906].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[921].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[936].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[951].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[966].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[981].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            ////if( P_HEADS_DATA[996].TrimRight() == "6" && P_HEADS_DATA[265].ToIntDef(0) == 1040 ) iCnt+=P_Num;
            //////**********************************
            // *********************
            // P_Num = iCnt;
            if (iCntB != 0) {
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[835].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("B");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            if (iCntA != 0) {
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
                //P_Zaisitu =
                //    Search_HD_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                //P_EZaisitu =
                //    Search_HD_E_MATERIAL(Chg_Zaisitu_Code
                //    (P_HEADS_DATA[836].TrimRight(), 2));
                s_GasMat = Get_SheetGasketMat("A");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            P_Num = 0;
            // ***************************************
        }
        // *******************************************

        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_S　
        // E側ﾎﾞﾙﾄﾅｯﾄ
        if (s_Code == "27") {
            iCntA = 0;
            iCntB = 0;
            // 2022.09.21 E側ボルトナットSUS指定_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 E側ボルトナットSUS指定_E
            // 閉止ﾌﾗﾝｼﾞ
            if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" ) {
                // S1
                if (P_HEADS_DATA[891].TrimRight() == "6") {
                    iCntB += P_Num;
                }
                // S2
                if (P_HEADS_DATA[906].TrimRight() == "6") {
                    iCntA += P_Num;
                }
                // S3
                if (P_HEADS_DATA[921].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntB += P_Num; // 台形流
                    }
                    else {
                        iCntA += P_Num; // 斜行流
                    }
                }
                // S4
                if (P_HEADS_DATA[936].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntA += P_Num; // 台形流
                    }
                    else {
                        iCntB += P_Num; // 斜行流
                    }
                }
                // E1
                if (P_HEADS_DATA[951].TrimRight() == "6") {
                    iCntB += P_Num;
                }
                // E2
                if (P_HEADS_DATA[966].TrimRight() == "6") {
                    iCntA += P_Num;
                }
                // E3
                if (P_HEADS_DATA[981].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntB += P_Num; // 台形流
                    }
                    else {
                        iCntA += P_Num; // 斜行流
                    }
                }
                // E4
                if (P_HEADS_DATA[996].TrimRight() == "6") {
                    if (iFL == 0) {
                        iCntA += P_Num; // 台形流
                    }
                    else {
                        iCntB += P_Num; // 斜行流
                    }
                }
            }

            // ｽﾄﾚｰﾅ
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            P_Siyou5 = "A";
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }
        // 2019.08.26 E側ﾎﾞﾙﾄﾅｯﾄ追加_E　

        // *********************************
        // ***  予備品数 取得
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);

        // ログ
        G_Log_Text = "部品ｺｰﾄﾞ           ｢" + s_Code + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "仕様               ｢" + s_Spec + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "部品表識別         ｢" + P_Sikibetu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "部品名称           ｢" + P_Katasiki + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "数量               ｢" + AnsiString(P_Num) + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  舶用部品ﾃﾞｰﾀ　作成
        // *********************************
        // //↓2002/10/24
        // // Head Data の和／英の指示とｽﾃｯｶｰの和／英が違うときは出力せず続く
        // if( P_HEADS_DATA[1].TrimRight() == "0" && (s_Code == "8" || s_Code == "9" || s_Code == "10")){//和やのに英
        // wkQuery->Next();
        // continue;
        // }
        // if( P_HEADS_DATA[1].TrimRight() == "1" && (s_Code == "11" || s_Code == "12" || s_Code == "13")){//英やのに和
        // wkQuery->Next();
        // continue;
        // }
        // 2003/03/13 ステッカーは図番１つを取って、仕様欄の表記を液名で２パターン作成とする
        if (s_Code == "10" || s_Code == "11" || s_Code == "12" ||
            s_Code == "13") { // コード８と９で図番１つずつ取得し、後は無視
            wkQuery->Next();
            continue;
        }
        if (s_Code == "8") { // ステッカー
            if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
                s_Spec = P_HEADS_DATA[45].TrimRight() + "用 入口／出口"; // Ｂ流体名(和）で作成
            }
            else { // 英文
                s_Spec = P_HEADS_DATA[134].TrimRight() + " ＩＮ／ＯＵＴ";
                // Ｂ流体名(英）で作成
            }
        }
        if (s_Code == "9") { // ステッカー
            if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
                s_Spec = P_HEADS_DATA[76].TrimRight() + "用 入口／出口"; // Ａ流体名(和）で作成
            }
            else { // 英文
                s_Spec = P_HEADS_DATA[135].TrimRight() + " ＩＮ／ＯＵＴ";
                // Ａ流体名(英）で作成
            }
        }

        // 2017.11.22 片側ﾘﾝｸﾞ端管_S
        //// 端管が付かない場合
        //// 2003.10.17 端管有無条件変更
        //// if( P_HEADS_DATA[1136].TrimRight() == "0" ){
        //if (P_HEADS_DATA[1136].ToIntDef(0) == 0) {
        //    // ***************************
        //    // 2003.10.30 ｼｰﾄﾊﾟｯｷﾝｸﾞ追加
        //    // // 端管廻止、ｽﾀｯﾄﾞBN、玉形弁、ｴｱｰｺｯｸ、ﾄﾞﾚﾝｺｯｸ、ｴｱｰﾄﾞﾚﾝｺｯｸ、ｼｰﾄﾊﾟｯｷﾝｸﾞは付かない
        //    // if( s_Code == "19" || s_Code == "18" || s_Code == "1" || s_Code == "3" ||
        //    // s_Code == "5"  || s_Code == "7"  || s_Code == "4"){
        //    // wkQuery->Next();
        //    // continue;
        //    // }
        //    // 2004.05.20 ｽﾀｯﾄﾞﾎﾞﾙﾄ用ﾗｲﾅｰ追加
        //    // // 端管廻止、ｽﾀｯﾄﾞBN、玉形弁、ｴｱｰｺｯｸ、ﾄﾞﾚﾝｺｯｸ、ｴｱｰﾄﾞﾚﾝｺｯｸ、ｼｰﾄﾊﾟｯｷﾝｸﾞは付かない
        //    // if( s_Code == "19" || s_Code == "18" || s_Code == "1" || s_Code == "3" ||
        //    // s_Code == "5"  || s_Code == "7"  || s_Code == "4" || s_Code == "2" ||
        //    // s_Code == "6"){
        //    // wkQuery->Next();
        //    // continue;
        //    // }
        //    // 端管廻止、ｽﾀｯﾄﾞBN、玉形弁、ｴｱｰｺｯｸ、ﾄﾞﾚﾝｺｯｸ、ｴｱｰﾄﾞﾚﾝｺｯｸ、ｼｰﾄﾊﾟｯｷﾝｸﾞ、ｽﾀｯﾄﾞﾎﾞﾙﾄ用ﾗｲﾅｰは付かない
        //    if (s_Code == "19" || s_Code == "18" || s_Code == "1" ||
        //        s_Code == "3" || s_Code == "5" || s_Code == "7" ||
        //        s_Code == "4" || s_Code == "2" || s_Code == "6" ||
        //        s_Code == "28") {
        //        wkQuery->Next();
        //        continue;
        //    }
        //    // *************************
        //}
        // 端管の有無による数量変化
        s_TextB = P_HEADS_DATA[1060].SubString(1,4);
        s_TextB = Type_MAT(s_TextB);
        s_TextA = P_HEADS_DATA[1061].SubString(1,4);
        s_TextA = Type_MAT(s_TextA);
        // ｴｱｰｺｯｸ、ｼｰﾄﾊﾟｯｷﾝｸﾞ(ｴｱｰｺｯｸ)、ﾄﾞﾚﾝｺｯｸ、ｼｰﾄﾊﾟｯｷﾝｸﾞ(ﾄﾞﾚﾝｺｯｸ)、ｴｱｰﾄﾞﾚﾝｺｯｸ、
        // ｼｰﾄﾊﾟｯｷﾝｸﾞ(ｴｱｰﾄﾞﾚﾝｺｯｸ)、玉形弁
        if ( s_Code == "1" || s_Code == "2" || s_Code == "3" || s_Code == "4" ||
             s_Code == "5" || s_Code == "6" || s_Code == "7" ) {
            if ( s_TextA == "-" && s_TextB == "-" ) {
                // 端管なし
                wkQuery->Next();
                continue;
            } else if ( s_TextA != "-" && s_TextB != "-" ) {
                // 両側端管
            } else {
                // 片側端管
                P_Num = P_Num / 2;
            }
        }
        // 2017.11.22 片側ﾘﾝｸﾞ端管_E

        // 2004.04.15 相フランジ,シートガスケット出力変更
        ////ｺｰﾄﾞがない 相フランジ または、ｼｰﾄｶﾞｽｹｯﾄ
        // if( P_HEADS_DATA[1068].ToIntDef(0) == 0 && (s_Code == "22" || s_Code == "24") ){
        // wkQuery->Next();
        // continue;
        // }
        // ｺｰﾄﾞがない 相フランジ(ｼｰﾄｶﾞｽｹｯﾄはｺｰﾄﾞなしでもつく)
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[1068].ToIntDef(0) == 0 && s_Code == "22") {
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "-" &&
            s_Code == "22") {
        // ***********************
            wkQuery->Next();
            continue;
        }
        if ((s_Code == "22" || s_Code == "24") && P_Num == 0) {
            wkQuery->Next();
            continue;
        }
        // ***********************************************

        // ｺｰﾄﾞがない 閉止フランジ または、閉止ｼｰﾄｶﾞｽｹｯﾄ
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (P_HEADS_DATA[265].ToIntDef(0) == 0 && (s_Code == "23" ||
        //    s_Code == "25" || s_Code == "26" || s_Code == "27")) {
        if ((Type_MAT(P_HEADS_DATA[265].TrimRight()) == "-") &&
            (s_Code == "23" || s_Code == "25" || s_Code == "26" || s_Code == "27")) {
        // ***********************
            wkQuery->Next();
            continue;
        }

        // 2004.04.15 閉止フランジ,閉止ｼｰﾄｶﾞｽｹｯﾄ,全面座閉止ｶﾞｽｹｯﾄ出力変更
        if ((s_Code == "23" || s_Code == "25" || s_Code == "26") && P_Num == 0)
        {
            wkQuery->Next();
            continue;
        }
        // **************************************************************

        if (s_Code == "14") {
            if (P_HEADS_DATA[36].Pos("J") != 0) { // Ｊ型は移動止め（チェーン）不要
                wkQuery->Next();
                continue;
            }
        }
        if (s_Code == "21") {
            if (P_HEADS_DATA[36].Pos("J") != 0) { // Ｊ型はアイプレートのサポート用は不要
                wkQuery->Next();
                continue;
            }
        }
        if (s_Code.ToIntDef(0) >= 1 && s_Code.ToIntDef(0) <= 6) { // 舶用コック関係
            // 2010.10.18 川崎重工舶用コック廃止
            // 大田指示による
            // if ( P_HEADS_DATA[1063].Pos("川崎重工") == 0 ){        // 川崎重工以外のみ有効
            wkQuery->Next();
            continue;
            // }
            // *********************************
        }
        // 2010.10.18 川崎重工舶用コック廃止
        // 大田指示による
        // if ( s_Code == "7" ) {       // 玉形弁
        // if ( P_HEADS_DATA[1063].Pos("川崎重工") != 0 ){        // 川崎重工のみ有効
        // wkQuery->Next();
        // continue;
        // }
        // }
        // *********************************
        if (s_Code == "18") { // Ｓ側スタッド
            P_EZaisitu = "SS400  ZINC";
            // 2022.09.21 SUSスタッド追加_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 SUSスタッド追加_E
            // S側スタッドのサイズ表記（仕様欄）を、端管有無で打ち分ける　03.06.24
            // 2017.11.22 片側ﾘﾝｸﾞ端管_S
            //// 2003.10.17 端管有無条件変更
            //// if( P_HEADS_DATA[1136].TrimRight() == "0" ){          // 端管無し
            //if (P_HEADS_DATA[1136].ToIntDef(0) == 0) { // 端管無し
            //    // ***************************
            //    Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
            //        &s_Spec);
            //}
            //else { // 端管有り
            //    // 2004.11.26 ALBC端管用STUD追加
            //    // Get_SSTDBltSpec(s_Key + "-"+ P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //    if (s_Key == "UX-90" || s_Key == "RX-70") { // UX-90,RX-70
            //        // 2016.08.30 材質ｺｰﾄﾞ変更
            //        //if (P_HEADS_DATA[1060].Pos("1043") >
            //        //    0 || P_HEADS_DATA[1060].Pos("1151") > 0)
            //        //{ // 1043 CAC703  1151 STKM13A
            //        //    if (P_HEADS_DATA[1061].Pos("1043") >
            //        //        0 || P_HEADS_DATA[1061].Pos("1151") > 0)
            //        //    { // 1043 CAC703  1151 STKM13A
            //        //        // B側A側ともALBC端管
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //        //    }
            //        //    else {
            //        //        // B側ALBC端管
            //        //        P_Num = P_Num / 2;
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //        //        Write_Funeyou_Data(s_Code, s_Spec);
            //        //        // A側旧端管
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //        //    }
            //        //}
            //        //else {
            //        //    if (P_HEADS_DATA[1061].Pos("1043") >
            //        //        0 || P_HEADS_DATA[1061].Pos("1151") > 0)
            //        //    { // 1043 CAC703  1151 STKM13A
            //        //        // B側旧端管
            //        //        P_Num = P_Num / 2;
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //        //        Write_Funeyou_Data(s_Code, s_Spec);
            //        //        // A側ALBC端管
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //        //    }
            //        //    else {
            //        //        // B側A側とも旧端管
            //        //        Get_SSTDBltSpec(s_Key + "-" +
            //        //            P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //        //    }
            //        //}
            //        s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            //        s_TextB = Type_MAT(s_TextB);
            //        s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            //        s_TextA = Type_MAT(s_TextA);
            //        if ( s_TextB == "ALBC" || s_TextB == "CS" ) {
            //            if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
            //                // B側A側ともALBC端管
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //            }
            //            else {
            //                // B側ALBC端管
            //                P_Num = P_Num / 2;
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //                Write_Funeyou_Data(s_Code, s_Spec);
            //                // A側旧端管
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //            }
            //        }
            //        else {
            //            if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
            //                // B側旧端管
            //                P_Num = P_Num / 2;
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //                Write_Funeyou_Data(s_Code, s_Spec);
            //                // A側ALBC端管
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
            //            }
            //            else {
            //                // B側A側とも旧端管
            //                Get_SSTDBltSpec(s_Key + "-" +
            //                    P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
            //            }
            //        }
            //        // ***********************
            //    }
            //    else {
            //        Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight
            //            (), 1, &s_Spec);
            //    }
            //    // *****************************
            //}
            s_TextB = P_HEADS_DATA[1060].SubString(1,4);
            s_TextB = Type_MAT(s_TextB);
            s_TextA = P_HEADS_DATA[1061].SubString(1,4);
            s_TextA = Type_MAT(s_TextA);
            // 2019.05.15 SGP端管追加_S
            if ( s_TextB == "SGP") {   // ＳＧＰ
                // SGP端管の場合ｽﾀｯﾄﾞ展開廃止
                s_TextB == "-";
            }
            if ( s_TextA == "SGP") {   // ＳＧＰ
                // SGP端管の場合ｽﾀｯﾄﾞ展開廃止
                  s_TextA == "-";
            }
            // 2019.04.12 SGP端管追加_E
            if ( s_TextA == "-" && s_TextB == "-" ) { // 両側端管無し
                // 2018.03.20 両側ﾘﾝｸﾞ端管無しの場合ｽﾀｯﾄﾞ展開廃止_S
                //Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
                //    &s_Spec);
                wkQuery->Next();
                continue;
                // 2018.03.20 両側ﾘﾝｸﾞ端管無しの場合ｽﾀｯﾄﾞ展開廃止_E
            }
            else if ( s_TextB == "-" ) { // B側端管無し
                P_Num = P_Num / 2;
                // B側端管無し
                Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
                    &s_Spec);
                Write_Funeyou_Data(s_Code, s_Spec);
                if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
                    // A側ALBC端管
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                }
                else {
                    // 2021.09.30 ﾘﾝｸﾞ端管無しの場合ｽﾀｯﾄﾞ展開廃止_S
                    //Write_Funeyou_Data(s_Code, s_Spec);
                    // 2021.09.30 ﾘﾝｸﾞ端管無しの場合ｽﾀｯﾄﾞ展開廃止_E
                    // A側旧端管
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                }
            }
            else if ( s_TextA == "-" ) { // A側端管無し
                P_Num = P_Num / 2;
                // A側端管無し
                Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight(), 0,
                    &s_Spec);
                Write_Funeyou_Data(s_Code, s_Spec);
                if ( s_TextB == "ALBC" || s_TextB == "CS" ) {
                    // B側ALBC端管
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                }
                else {
                    // 2021.09.30 ﾘﾝｸﾞ端管無しの場合ｽﾀｯﾄﾞ展開廃止_S
                    //Write_Funeyou_Data(s_Code, s_Spec);
                    // 2021.09.30 ﾘﾝｸﾞ端管無しの場合ｽﾀｯﾄﾞ展開廃止_E
                    // B側旧端管
                    Get_SSTDBltSpec(s_Key + "-" +
                        P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                }
            }
            else { // 両側端管有り
                if (s_Key == "UX-90" ||s_Key == "SX-80S" || s_Key == "SX-80M"||
                    s_Key == "SX-80L" || s_Key == "RX-70" ) {
                    // UX-90,SX-80S,SX-80M,SX-80L,RX-70
                    if ( s_TextB == "ALBC" || s_TextB == "CS" ) {
                        if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
                            // B側A側ともALBC端管
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                        }
                        else {
                            // B側ALBC端管
                            P_Num = P_Num / 2;
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                            Write_Funeyou_Data(s_Code, s_Spec);
                            // A側旧端管
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                        }
                    }
                    else {
                        if ( s_TextA == "ALBC" || s_TextA == "CS" ) {
                            // B側旧端管
                            P_Num = P_Num / 2;
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                            Write_Funeyou_Data(s_Code, s_Spec);
                            // A側ALBC端管
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 1, &s_Spec);
                        }
                        else {
                            // B側A側とも旧端管
                            Get_SSTDBltSpec(s_Key + "-" +
                                P_HEADS_DATA[1267].TrimRight(), 2, &s_Spec);
                        }
                    }
                }
                else {
                    Get_SSTDBltSpec(s_Key + "-" + P_HEADS_DATA[1267].TrimRight
                        (), 1, &s_Spec);
                }
            }
            // 2017.11.22 片側ﾘﾝｸﾞ端管_E
        }
        // ｽﾄﾚｰﾅ
        // 2019.09.09 E側ﾎﾞﾙﾄﾅｯﾄ追加_S　
        //if (s_Code == "17" || s_Code == "27") {
        if (s_Code == "17") {
        // 2019.09.09 E側ﾎﾞﾙﾄﾅｯﾄ追加_E　
            // 各ノズル種類にｽﾄﾚｰﾅが設定されていない
            if ((P_HEADS_DATA[891].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[906].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[921].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[936].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[951].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[966].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[981].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[996].ToIntDef(0) != 5)) {
                wkQuery->Next();
                continue;
            }
            // 2009.10.28 ストレーナは条件によって図番が変わる場合がある
            //// ストレーナは条件によって図番が変わる場合がある 03-01-08
            //// 2003.10.22 型式追加 RX-11,RX-13
            ////if ( s_Key == "RX-10" ) {                    // RX-10
            // if ( s_Key == "RX-10" || s_Key == "RX-11" || s_Key == "RX-13" ) { // RX-10,RX-11,RX-13
            ////********************************
            // // 2004.04.15 RX-10-TNJM,TNPM,TNHJM,TNHPM追加
            // if (P_HEADS_DATA[36].TrimRight() == "NPM" || P_HEADS_DATA[36].TrimRight() == "NHPM" || P_HEADS_DATA[36].TrimRight() == "TNPM" || P_HEADS_DATA[36].TrimRight() == "TNHPM") {                                      // フレームがＮＰＭ、ＮＨＰＭ
            // // フレームがＮＰＭ、ＮＨＰＭ、ＴＮＰＭ、ＴＮＨＰＭ
            // } else if (P_HEADS_DATA[36].TrimRight() == "NJM" || P_HEADS_DATA[36].TrimRight() == "NHJM" || P_HEADS_DATA[36].TrimRight() == "TNJM" || P_HEADS_DATA[36].TrimRight() == "TNHJM") {
            // // フレームがＮＪＭ、ＮＨＪＭ、ＴＮＪＭ、ＴＮＨＪＭ
            // // 図番２を適用
            // P_Zuban    = s_Dno2;
            // P_SZuban   = s_WKDno2;
            // } else {
            // G_ErrLog_Text = "ストレーナ条件に該当しません。";
            // Write_Error_Log(G_ErrLog_Text);
            // wkQuery->Next();
            // continue;
            // }
            // //if (P_HEADS_DATA[36].TrimRight() == "NPM" || P_HEADS_DATA[36].TrimRight() == "NHPM") {                                      // フレームがＮＰＭ、ＮＨＰＭ
            // //} else if (P_HEADS_DATA[36].TrimRight() == "NJM" || P_HEADS_DATA[36].TrimRight() == "NHJM") {                               // フレームがＮＪＭ、ＮＨＪＭ
            // //   // 図番２を適用
            // //   P_Zuban    = s_Dno2;
            // //   P_SZuban   = s_WKDno2;
            // //} else {
            // //   G_ErrLog_Text = "ストレーナ条件に該当しません。";
            // //   Write_Error_Log(G_ErrLog_Text);
            // //   wkQuery->Next();
            // //   continue;
            // //}
            // //*******************************************
            // } else if ( s_Key == "UX-90" ) {             // UX-90
            // //2007.02.13 ﾃﾞｰﾀ位置変更
            // //if (P_HEADS_DATA[159].ToIntDef(0) > 350) {   // 懸垂枚数が３５１枚以上
            // if (P_HEADS_DATA[38].ToIntDef(0) > 350) {   // 懸垂枚数が３５１枚以上
            // //***********************
            // // 図番２を適用
            // P_Zuban    = s_Dno2;
            // P_SZuban   = s_WKDno2;
            // }
            // }
            if (s_Key == "UX-90" || s_Key == "SX-90M") {
                // UX-90,SX-90M
                if (P_HEADS_DATA[38].ToIntDef(0) > 350) {
                    // 懸垂枚数が351枚以上
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //if (P_HEADS_DATA[261].Pos("1003") > 0) {
                    //    // ｽﾄﾚｰﾅがSUS316の場合はE359301
                    //    // 2014.06.24 図番変更
                    //    // P_Zuban    = "Ｅ３５９３０１";
                    //    P_Zuban = "Ｅ３５９３０１－Ｒ１";
                    //    // *******************
                    //    P_SZuban = "Ｅ３５９３０１";
                    //}
                    //else if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    //    // ｽﾄﾚｰﾅがTP340の場合はE359302
                    //    // 2014.06.24 図番変更
                    //    // P_Zuban    = "Ｅ３５９３０２";
                    //    P_Zuban = "Ｅ３５９３０２－Ｒ２";
                    //    // *******************
                    //    P_SZuban = "Ｅ３５９３０２";
                    //}
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ｽﾄﾚｰﾅがSUS316の場合
                        P_Zuban = "Ｅ３５９３０１－Ｒ１";
                        P_SZuban = "Ｅ３５９３０１";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ｽﾄﾚｰﾅがTP340の場合
                        P_Zuban = "Ｅ３５９３０２－Ｒ２";
                        P_SZuban = "Ｅ３５９３０２";
                    }
                    // ***********************
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // 懸垂枚数が350枚以下
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                    // ***********************
                        // ｽﾄﾚｰﾅがTP340の場合は図番２
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
                // 2014.06.24 RX-90ｽﾄﾚｰﾅ追加
            }
            else if (s_Key == "RX-90") {
                // RX-90
                if (P_HEADS_DATA[38].ToIntDef(0) > 291) {
                    // 懸垂枚数が291枚以上
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //if (P_HEADS_DATA[261].Pos("1003") > 0) {
                    //    // ｽﾄﾚｰﾅがSUS316の場合はE359338
                    //    P_Zuban = "Ｅ３５９３３８－Ｒ２";
                    //    P_SZuban = "Ｅ３５９３３８";
                    //}
                    //else if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    //    // ｽﾄﾚｰﾅがTP340の場合はE359572
                    //    P_Zuban = "Ｅ３５９５７２－Ｒ３";
                    //    P_SZuban = "Ｅ３５９５７２";
                    //}
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ｽﾄﾚｰﾅがSUS316の場合
                        P_Zuban = "Ｅ３５９３３８－Ｒ２";
                        P_SZuban = "Ｅ３５９３３８";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ｽﾄﾚｰﾅがTP340の場合
                        P_Zuban = "Ｅ３５９５７２－Ｒ３";
                        P_SZuban = "Ｅ３５９５７２";
                    }
                    // ***********************
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // 懸垂枚数が290枚以下
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                    // ***********************
                        // ｽﾄﾚｰﾅがTP340の場合は図番２
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
                // *************************
            }
            else if (s_Key != "UX-01" && s_Key != "UX-10" && s_Key != "UX-20" &&
                s_Key != "UX-40" && s_Key != "LX-00" && s_Key != "LX-10" &&
                s_Key != "LX-20" && s_Key != "LX-40" && s_Key != "RX-90") {
                // UX-01,UX-10,UX-20,UX-40,LX-00,LX-10,LX-20,LX-40以外
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                    P_HEADS_DATA[261].Pos("3032") > 0 ||
                    P_HEADS_DATA[261].Pos("3132") > 0) {
                // ***********************
                    // ｽﾄﾚｰﾅがTP340の場合は図番２
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
            }
            // **********************************************************

            if (s_Code == "17") { // ｽﾄﾚｰﾅの場合はｽﾄﾚｰﾅ材質を出力
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[261].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[261].TrimRight());
            }
        }

        // ↑2002/10/24

        // 2010.04.19 UX-90 ｱｲﾌﾟﾚｰﾄ図番
        // UX-90の場合耐圧区分で図番を分ける
        // ｱｲﾌﾟﾚｰﾄ
        if (s_Code == "20" || s_Code == "21") {
            if (s_Key == "UX-90") {
                // UX-90
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VERが200207の場合は図番2
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
                // 2012.08.23 HEADS.VER追加
                if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VERが300207の場合は図番2
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
                // ************************
            }
        }
        // ****************************

        // 2022.09.30 Eﾌﾚ-ﾑﾍﾞｰｽﾌﾟﾚｰﾄ追加_S
        if (s_Code == "29") {
            if (s_Key == "UX-90") {
                // UX-90
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VERが200207の場合は図番無し
                    P_Zuban = "";
                    P_SZuban = "";
                }
                // 2012.08.23 HEADS.VER追加
                if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VERが300207の場合は図番無し
                    P_Zuban = "";
                    P_SZuban = "";
                }
            }

            if ( P_Zuban == "" ) { // 図番無し
                wkQuery->Next();
                continue;
            }

            if ( G_EBASE_FLG == 0 || P_HEADS_DATA[1382].ToIntDef(0) == 0 ) {
                // Eﾌﾚ-ﾑﾍﾞｰｽﾌﾟﾚｰﾄ出力不要
                wkQuery->Next();
                continue;
            }

        }
        // 2022.09.30 Eﾌﾚ-ﾑﾍﾞｰｽﾌﾟﾚｰﾄ追加_E


        Write_Funeyou_Data(s_Code, s_Spec);
        wkQuery->Next();
    }

    delete wkQuery;


    // 2015.03.19 OGI 舶用部品アキュームレータ出力対応
    if ((P_HEADS_DATA[981].ToIntDef(0) == 8 ) ||
        (P_HEADS_DATA[996].ToIntDef(0) == 8)) {

        AnsiString s_Text;
        s_Text = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
            P_HEADS_DATA[1267].TrimRight();

        P_Sikibetu = "ACCU-E"; // 部品分類

        P_Katasiki = "アキュムレータ"; // 部品型式
        Search_HD_FRM_NZ_Accum(s_Text, "E", 76);
        Write_Funeyou_Data("76", "");

        P_Katasiki = "アキュムレータサポート"; // 部品型式
        Search_HD_FRM_NZ_Accum(s_Text, "E", 77);
        Write_Funeyou_Data("77", "");

        // 2015.08.04 ｱｷｭﾑﾚｰﾀ取付部品追加
        P_Katasiki = "アキュムレータ取付部品"; // 部品型式
        Search_HD_FRM_NZ_Accum(s_Text, "E", 78);
        Write_Funeyou_Data("78", "");
        // ******************************
    }

    return true;
}

// 2015.03.19 OGI 舶用部品アキュームレータ出力対応
// ---------------------------------------------------------------------------
// 日本語関数名： HD_FRM_NZﾃｰﾌﾞﾙ検索処理（アキュームレータ専用）
// 概  要      ：
// 引  数      ：
// 戻り値      ： 検索結果
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Search_HD_FRM_NZ_Accum(AnsiString Key,
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
        P_EZaisitu = "";
        P_Zuban = "";
        P_SZuban = "";
        P_Num = 0;
        P_Yobisu = 0;
        P_Siyou = "";
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Unit = "";

        delete wkQuery;
        return false;
    }
    else {
        if( P_HEADS_DATA[1116].ToIntDef(0)==2 ) {  // 2L
            // 材質名称
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            // 図番
            P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // 図番
            // 作図用図番
            P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        }
        else if( P_HEADS_DATA[1116].ToIntDef(0)==3 ) {  // 3L
            // 材質名称
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight());
            // 図番
            P_Zuban = wkQuery->FieldByName("DNO2")->AsString.TrimRight(); // 図番
            // 作図用図番
            P_SZuban = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        }
        else if( P_HEADS_DATA[1116].ToIntDef(0)==5 ) {  // 5L
            // 材質名称
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight());
            // 図番
            P_Zuban = wkQuery->FieldByName("DNO3")->AsString.TrimRight(); // 図番
            // 作図用図番
            P_SZuban = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        }
        else
        {
            // 材質名称
            P_Zaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            P_EZaisitu = Search_HD_MATERIAL(wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight());
            // 図番
            P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight(); // 図番
            // 作図用図番
            P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        }

        // 数量
        P_Num = 1;
        // 予備数
        P_Yobisu = 0;
        // 仕様
        P_Yobisu = 0;
        P_Siyou = "";
        P_Siyou2 = "";
        P_Siyou5 = "";
        P_Unit = "";
    }

    delete wkQuery;
    return true;
}



// ---------------------------------------------------------------------------
// 日本語関数名： 付属品ﾃﾞｰﾀﾌｧｲﾙ書込み処理
// 概  要      ： ﾄﾞﾚﾝﾊﾟﾝ,ｱﾝｶｰﾎﾞﾙﾄ
// 引  数      ： AnsiString a_SE   ： 圧力計SE区分(S1～S4,E1～E4)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Write_Axsrs_Data(AnsiString a_SE) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
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
    s_Text = "ACCS";
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
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);

    // 13 仕様３
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 仕様４
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);

    // 15 仕様５
    s_Text = "," + P_Siyou5;
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
// 日本語関数名： 舶用付属品ﾃﾞｰﾀﾌｧｲﾙ書込み処理
// 概  要      ：
// 引  数      ： AnsiString s_Code   ： 舶用部品ｺｰﾄﾞ
// ： AnsiString s_Spec   ： 部品ｽﾍﾟｯｸ
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Write_Funeyou_Data(AnsiString s_Code,
    AnsiString s_Spec) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
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
    s_Text = "SHIP";
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
    if (s_Code == "16")
        // RecNo[1126] ｹﾞｰｼﾞの長さ
            s_Text = ",Ｌ＝" + HanToZen(P_HEADS_DATA[1126].Trim()) + " Ａｍａｘ＝" +
            HanToZen(P_HEADS_DATA[1089].Trim()) + " Ａｍｉｎ＝" +
            HanToZen(P_HEADS_DATA[1015].Trim());
    else if (s_Code == "17")
        // RecNo[1126] ｽﾄﾚｰﾅ長さ１
        // if ( ( P_HEADS_DATA[1124].TrimRight() != "" ) && (P_HEADS_DATA[1125].TrimRight() != "") ){
        // if ( P_HEADS_DATA[1124].ToDouble() > P_HEADS_DATA[1125].ToDouble() ) {
        // s_Text = ",Ｌ１＝" + HanToZen( P_HEADS_DATA[1124].Trim() );
        // } else {
        // s_Text = ",Ｌ１＝" + HanToZen( P_HEADS_DATA[1125].Trim() );
        // }
        // } else if (P_HEADS_DATA[1125].TrimRight() != "") {
        // s_Text = ",Ｌ１＝" + HanToZen( P_HEADS_DATA[1125].Trim() );
        // } else {
            s_Text = ",Ｌ１＝" + HanToZen(P_HEADS_DATA[1124].Trim());
    // }
    else
        s_Text = "," + s_Spec;
    fputs(s_Text.c_str(), wkfp);

    // 12 仕様２
    if (s_Code == "16")
        s_Text = ",テスト後打刻";
    else if (s_Code == "17")
        // RecNo[1126] ｽﾄﾚｰﾅ長さ１
        // 2009.12.01 L2寸法変更
        // s_Text = ",Ｌ２＝" + HanToZen( P_HEADS_DATA[1125].Trim() );
        if (P_HEADS_DATA[1125].Trim() != "0") {
            s_Text = ",Ｌ２＝" + HanToZen(P_HEADS_DATA[1125].Trim());
        }
        else {
            s_Text = ",";
        }
    // *********************
    else
        s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 13 仕様３
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 仕様４
    if (s_Code == "14")
        s_Text = ",式";
    else if (s_Code == "18" || s_Code == "27")
        s_Text = ",組";
    // 2010.04.19 UX-90 ｱｲﾌﾟﾚｰﾄ
    else if (s_Code == "20" || s_Code == "21") {
        // 1台分数量1個以下の場合"式"を付ける
        if (P_Num <= 1) {
            s_Text = ",式";
        }
    }

    // 2015.03.19 OGI 舶用部品アキュームレータ出力対応
    // 2015.08.04 ｱｷｭﾑﾚｰﾀ取付部品追加
    //else if (s_Code == "76" || s_Code == "77")
    else if (s_Code == "76" || s_Code == "77" || s_Code == "78")
    // ******************************
        s_Text = ",式";

    // ************************

    // 2022.09.30 Eﾌﾚ-ﾑﾍﾞｰｽﾌﾟﾚｰﾄ追加_S
    else if (s_Code == "29")
         s_Text = ",式";
    // 2022.09.30 Eﾌﾚ-ﾑﾍﾞｰｽﾌﾟﾚｰﾄ追加_E

    else
        s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 15 仕様５
    // 2003.11.25 ＡＢ変更
    // if( s_Code == "17" ){  //ｽﾄﾚｰﾅ
    // // RecNo[1071]Ｂ流体名
    // if( P_HEADS_DATA[1071].TrimRight() == "海水" || P_HEADS_DATA[1071].TrimRight() == "SEA WATER" ) {
    // s_Text = ",B";
    // }
    // else {
    // s_Text = ",A";
    // }
    // }else {
    // s_Text = ",";
    // }
    if (s_Code == "17") { // ｽﾄﾚｰﾅ
        // RecNo[951]Ｅ１ノズル種類
        // RecNo[981]Ｅ３ノズル種類
        if (P_HEADS_DATA[951].TrimRight() == "5" || P_HEADS_DATA[981].TrimRight
            () == "5") {
            s_Text = ",B";
        }
        else {
            s_Text = ",A";
        }
        // 2004.06.29 相ﾌﾗﾝｼﾞ､閉止ﾌﾗﾝｼﾞAB区分追加
        // 2008.01.08 ｼｰﾄｶﾞｽｹｯﾄをAB区分追加
        // }else if ( s_Code == "22" || s_Code == "23" ){
    }
    else if (s_Code == "22" || s_Code == "23" || s_Code == "24" ||
        s_Code == "25" || s_Code == "26") {
        // **************************************
        // ********************************
        s_Text = "," + P_Siyou5;
    }
    else {
        s_Text = ",";
    }
    // *******************
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
// 日本語関数名： 相フランジ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_IFrng_Data(void) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    int iCntA;
    int iCntB;
    int iBltNum;
    // 2010.07.14 斜行流追加
    int iFL;
    // *********************
    //2017.04.20 ASME,JPI材質変更
    int i_KikakuS1;
    int i_KikakuS3;
    int i_KikakuE1;
    int i_KikakuE3;
    //***************************
    //2018.11.08 相フランジ仕様変更_S
    int i_KikakuS2;
    int i_KikakuS4;
    int i_KikakuE2;
    int i_KikakuE4;
    //2018.11.08 相フランジ仕様変更_E
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    AnsiString s_GasMat;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E

    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

    // 2016.11.07 材質ｺｰﾄﾞ変更
    //if (P_HEADS_DATA[1068].TrimRight() == "0000") { // 相フランジ材質が００００の場合は無し
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "-") {
        // 相フランジ材質が００００の場合は無し
    // ***********************
        return false;
    }
    iCntA = 0;
    iCntB = 0;
    // 2010.07.14 斜行流追加
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        iFL = 1; // 斜行流
    }
    // 2022.11.15 HERP修正_S
    else if  (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        iFL = 1; // 斜行流
    }
    // 2022.11.15 HERP修正_E
    else {
        iFL = 0; // 台形流
    }
    // 数量算出（サニタリーには付かない）
    // S1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntB += 1;
    }
    // S2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntA += 1;
    }
    // S3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntB += 1; // 台形流
        }
        else {
            iCntA += 1; // 斜行流
        }
    }
    // S4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntA += 1; // 台形流
        }
        else {
            iCntB += 1; // 斜行流
        }
    }
    // E1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntB += 1;
    }
    // E2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        iCntA += 1;
    }
    // E3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntB += 1; // 台形流
        }
        else {
            iCntA += 1; // 斜行流
        }
    }
    // E4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) {
            iCntA += 1; // 台形流
        }
        else {
            iCntB += 1; // 斜行流
        }
    }
    ////数量算出（サニタリーには付かない）
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) iCntB+=1;
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) iCntA+=1;
    // *********************
    // 材質ｺｰﾄﾞの取得
    P_Sikibetu = "IFRG--"; // 部品分類
    P_Katasiki = "相フランジ"; // 部品型式
    P_Zuban = ""; // 図版（購入品なので図番は無し）
    P_SZuban = ""; // 作図用図番（購入品なので図番は無し）
    P_Yobihin = ""; // 予備品識別子
    //2018.11.08 相フランジ仕様変更_S
    //P_Siyou = P_HEADS_DATA[1368].TrimRight(); // 仕様
    P_Siyou = P_HEADS_DATA[1369].TrimRight(); // 仕様
    //2018.11.08 相フランジ仕様変更_E
    P_Siyou2 = ""; // 仕様2
    P_Siyou5 = "A";
    P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    //2017.04.20 ASME,JPI材質変更
    //2018.11.08 相フランジ仕様変更_S
    ////S1,S3,E1,E3規格がASME,JPIの場合S25C
    //i_KikakuS1 = 0;
    //i_KikakuS1 = GetNzKikaku(P_HEADS_DATA[893]);
    //i_KikakuS3 = 0;
    //i_KikakuS3 = GetNzKikaku(P_HEADS_DATA[923]);
    //i_KikakuE1 = 0;
    //i_KikakuE1 = GetNzKikaku(P_HEADS_DATA[953]);
    //i_KikakuE3 = 0;
    //i_KikakuE3 = GetNzKikaku(P_HEADS_DATA[983]);
    //if ( i_KikakuS1 == 1 || i_KikakuS3 == 1 || i_KikakuE1 == 1 || i_KikakuE3 == 1 ) {
    //S2,S4,E2,E4規格がASME,JPIの場合S25C
    i_KikakuS2 = 0;
    i_KikakuS2 = GetNzKikaku(P_HEADS_DATA[908]);
    i_KikakuS4 = 0;
    i_KikakuS4 = GetNzKikaku(P_HEADS_DATA[938]);
    i_KikakuE2 = 0;
    i_KikakuE2 = GetNzKikaku(P_HEADS_DATA[968]);
    i_KikakuE4 = 0;
    i_KikakuE4 = GetNzKikaku(P_HEADS_DATA[998]);
    if ( i_KikakuS2 == 1 || i_KikakuS4 == 1 || i_KikakuE2 == 1 || i_KikakuE4 == 1 ) {
    //2018.11.08 相フランジ仕様変更_E
        // ASME,JPI
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // 相フランジ材質がSS400の場合S25Cに変更
            P_Zaisitu="S25C";
            P_EZaisitu="S25C";
        }
    }
    //***************************
    P_Num = iCntA;
    P_Unit = "";
    P_Hosoku = "";

    // *********************************
    // ***  予備品数 取得
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

    // ログ
    G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
    Write_Log(G_Log_Text);

    Write_Axsrs_Data(s_Code);

    //2018.11.08 相フランジ仕様変更_S
    P_Siyou = P_HEADS_DATA[1368].TrimRight(); // 仕様
    P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    P_EZaisitu = Search_HD_E_MATERIAL(P_HEADS_DATA[1068].TrimRight());
    //S1,S3,E1,E3規格がASME,JPIの場合S25C
    i_KikakuS1 = 0;
    i_KikakuS1 = GetNzKikaku(P_HEADS_DATA[893]);
    i_KikakuS3 = 0;
    i_KikakuS3 = GetNzKikaku(P_HEADS_DATA[923]);
    i_KikakuE1 = 0;
    i_KikakuE1 = GetNzKikaku(P_HEADS_DATA[953]);
    i_KikakuE3 = 0;
    i_KikakuE3 = GetNzKikaku(P_HEADS_DATA[983]);
    if ( i_KikakuS1 == 1 || i_KikakuS3 == 1 || i_KikakuE1 == 1 || i_KikakuE3 == 1 ) {
        // ASME,JPI
        if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // 相フランジ材質がSS400の場合S25Cに変更
            P_Zaisitu="S25C";
            P_EZaisitu="S25C";
        }
    }
    //2018.11.08 相フランジ仕様変更_E
    P_Siyou5 = "B";
    P_Num = iCntB;
    Write_Axsrs_Data(s_Code);

    // *********************************
    // ***  付属品ﾃﾞｰﾀ(ｼｰﾄｶﾞｽｹｯﾄ)　作成
    // *********************************
    iCntA = 0;
    iCntB = 0;
    // 数量算出（サニタリーには付かない）
    // 2010.07.14 斜行流追加
    // S1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // if ( P_HEADS_DATA[1136].ToIntDef(0) != 0 ) {                         // 端管が付く
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2" || P_HEADS_DATA[891].TrimRight() == "3") &&
        (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[891].TrimRight() == "3") { // エア抜が付く
            if (P_HEADS_DATA[893].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[893].TrimRight().SubString(3,
                1) != "4") { // ラバーでもパイプでもない
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1")
        { // ラバーブーツ
            if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntB += 1;
        }
        else {
            if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 2;
            }
            else {
                iCntB += 1;
            }
        }
    }
    // S2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // if ( P_HEADS_DATA[1137].ToIntDef(0) != 0 ) {                         // 端管が付く
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2" || P_HEADS_DATA[906].TrimRight() == "3") &&
        (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[906].TrimRight() == "3") { // エア抜が付く
            if (P_HEADS_DATA[908].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[908].TrimRight().SubString(3,
                1) != "4") { // ラバーでもパイプでもない
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1")
        { // ラバーブーツ
            if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntA += 1;
        }
        else {
            if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 2;
            }
            else {
                iCntA += 1;
            }
        }
    }
    // S3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // 台形流
    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // } else {                                                                 // 斜行流
    // if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1138].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2" || P_HEADS_DATA[921].TrimRight() == "3") &&
        (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[921].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[921].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[923].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
    }
    // ***********************
    // S4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // 台形流
    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // } else {                                                                 // 斜行流
    // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1139].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2" || P_HEADS_DATA[936].TrimRight() == "3") &&
        (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[936].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[936].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[938].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
    }
    // ***********************
    // E1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // if ( P_HEADS_DATA[1140].ToIntDef(0) != 0 ) {                         // 端管が付く
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2" || P_HEADS_DATA[951].TrimRight() == "3") &&
        (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[951].TrimRight() == "3") { // エア抜が付く
            if (P_HEADS_DATA[953].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[953].TrimRight().SubString(3,
                1) != "4") { // ラバーでもパイプでもない
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1")
        { // ラバーブーツ
            if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
            }
            else {
                iCntB += 0;
            }
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntB += 1;
        }
        else {
            if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 2;
            }
            else {
                iCntB += 1;
            }
        }
    }
    // E2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // if ( P_HEADS_DATA[1141].ToIntDef(0) != 0 ) {                         // 端管が付く
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2" || P_HEADS_DATA[966].TrimRight() == "3") &&
        (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[966].TrimRight() == "3") { // エア抜が付く
            if (P_HEADS_DATA[968].TrimRight().SubString(3,
                1) != "1" && P_HEADS_DATA[968].TrimRight().SubString(3,
                1) != "4") { // ラバーでもパイプでもない
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1")
        { // ラバーブーツ
            if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
            }
            else {
                iCntA += 0;
            }
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntA += 1;
        }
        else {
            if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 2;
            }
            else {
                iCntA += 1;
            }
        }
    }
    // E3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // 台形流
    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // } else {                                                                 // 斜行流
    // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1142].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2" || P_HEADS_DATA[981].TrimRight() == "3") &&
        (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[981].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[981].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[983].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
    }
    // ***********************
    // E4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( iFL == 0 ) {                                                        // 台形流
    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=1;
    // } else {
    // iCntA+=0;
    // }
    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // } else {                                                                 // 斜行流
    // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=1;
    // } else {
    // iCntB+=0;
    // }
    // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if ( P_HEADS_DATA[1143].ToIntDef(0) != 0 ) {                     // 端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // }
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2" || P_HEADS_DATA[996].TrimRight() == "3") &&
        (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[996].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                }
                else {
                    iCntA += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntA += 1;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 2;
                }
                else {
                    iCntA += 1;
                }
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[996].TrimRight() == "3") { // 液抜が付く
                if (P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "1" && P_HEADS_DATA[998].TrimRight().SubString(3,
                    1) != "4") { // ラバーでもパイプでもない
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                }
                else {
                    iCntB += 0;
                }
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                iCntB += 1;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 2;
                }
                else {
                    iCntB += 1;
                }
            }
        }
    }
    // ***********************
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // }
    // } else if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // } else {
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=2;
    // } else {
    // iCntB+=1;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1") { //ラバーブーツ
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // }
    // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // } else {
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=2;
    // } else {
    // iCntA+=1;
    // }
    // }
    // }
    // *********************

    // 材質ｺｰﾄﾞの取得
    P_Sikibetu = "STGKT-"; // 部品分類
    P_Katasiki = "シートガスケット"; // 部品型式
    P_Zuban = ""; // 図版（購入品なので図番は無し）
    P_SZuban = ""; // 作図用図番（購入品なので図番は無し）
    P_Yobihin = ""; // 予備品識別子
    //2018.11.08 相フランジ仕様変更_S
    //P_Siyou = P_HEADS_DATA[1368].TrimRight(); // 仕様
    P_Siyou = P_HEADS_DATA[1369].TrimRight(); // 仕様
    //2018.11.08 相フランジ仕様変更_E
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S

    //2018.11.08 相フランジ仕様変更_S
    //if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
    //    iCntA = iCntA + iCntB;
    //    iCntB = 0;
    //}
    if ( (Get_SheetGasketMat("A") == Get_SheetGasketMat("B") ) &&
         (P_HEADS_DATA[1368].TrimRight() == P_HEADS_DATA[1369].TrimRight() ) ) {
        iCntA = iCntA + iCntB;
        iCntB = 0;
    }
    //2018.11.08 相フランジ仕様変更_E

    if ( iCntA > 0 && iCntB > 0 ) {
        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(A側)
        if (sSpecCode == "A") {
            // ミラー仕様 Ａ号機
            P_Siyou = P_Siyou + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
        } else if (sSpecCode == "B") {
            // ミラー仕様 Ｂ号機
            P_Siyou = P_Siyou + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
        } else {
            P_Siyou = P_Siyou + "　Ａ側用";                 // 仕様
        }
    }
    // 2018.05.29  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
    P_Siyou2 = ""; // 仕様2
    P_Siyou5 = "A";
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
    P_Num = iCntA;
    P_Unit = "";
    P_Hosoku = "";

    // 2020.12.16 シートガスケット名称変更_S
    if ( P_GasNameA == "リングガスケット" ) {
        // ﾘﾝｸﾞｶﾞｽｹｯﾄ
        P_Sikibetu = "STGKTR";                   // 部品分類
        P_Katasiki = "リングガスケット";         // 部品型式
    } else if ( P_GasNameA == "全面座フランジガスケット" ) {
        // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
        P_Sikibetu = "STGKTF";                   // 部品分類
        P_Katasiki = "全面座フランジガスケット"; // 部品型式
    } else {
        // その他
    }
    // 2020.12.16 シートガスケット名称変更_E

    // *********************************
    // ***  予備品数 取得
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

    // ログ
    G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
    Write_Log(G_Log_Text);

    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
    //if (iCntA > 0)
    //    Write_Axsrs_Data(s_Code);
    if (iCntA > 0) {
        Write_Axsrs_Data(s_Code);
    }

    //2018.11.08 相フランジ仕様変更_S
    P_Siyou = P_HEADS_DATA[1368].TrimRight(); // 仕様
    //2018.11.08 相フランジ仕様変更_E

    if ( iCntB > 0 ) {
        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(B側)
        if (sSpecCode == "A") {
            // ミラー仕様 Ａ号機
            P_Siyou = P_Siyou + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
        } else if (sSpecCode == "B") {
            // ミラー仕様 Ｂ号機
            P_Siyou = P_Siyou + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
        } else {
            P_Siyou = P_Siyou + "　Ｂ側用";                 // 仕様
        }
    }
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

    P_Siyou5 = "B";
    P_Num = iCntB;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
    // 2020.12.16 相ﾌﾗﾝｼﾞｼｰﾄｶﾞｽｹｯﾄ修正_S
    //if (iCntB > 0)
    if (iCntB > 0){
    // 2020.12.16 相ﾌﾗﾝｼﾞｼｰﾄｶﾞｽｹｯﾄ修正_E
        // 2020.12.16 シートガスケット名称変更_S
        if ( P_GasNameB == "リングガスケット" ) {
            // ﾘﾝｸﾞｶﾞｽｹｯﾄ
            P_Sikibetu = "STGKTR";                   // 部品分類
            P_Katasiki = "リングガスケット";         // 部品型式
        } else if ( P_GasNameB == "全面座フランジガスケット" ) {
            // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
            P_Sikibetu = "STGKTF";                   // 部品分類
            P_Katasiki = "全面座フランジガスケット"; // 部品型式
        } else {
            // その他
        }
        // 2020.12.16 シートガスケット名称変更_E

        Write_Axsrs_Data(s_Code);

    // 2020.12.16 相ﾌﾗﾝｼﾞｼｰﾄｶﾞｽｹｯﾄ修正_S
    }
    // 2020.12.16 相ﾌﾗﾝｼﾞｼｰﾄｶﾞｽｹｯﾄ修正_E

    // *********************************
    // ***  付属品ﾃﾞｰﾀ(ﾎﾞﾙﾄﾅｯﾄ)　作成
    // *********************************
    iCntA = 0;
    iCntB = 0;
    // 数量算出（サニタリーには付かない）
    // 2010.07.14 斜行流追加
    // S1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
            iCntB += 1;
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntB += 1;
        }
    }
    // S2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // 端管が付く
            iCntA += 1;
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntA += 1;
        }
    }
    // S3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 1;
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if ( P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 1;
            }
        }
    }
    // S4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 1;
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 1;
            }
        }
    }
    // E1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // 端管が付く
            iCntB += 1;
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntB += 1;
        }
    }
    // E2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // 端管が付く
            iCntA += 1;
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") {        // パイプノズル（フランジ付き）
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntA += 1;
        }
    }
    // E3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 1;
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 1;
            }
        }
    }
    // E4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {    // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 1;
            }
        }
        else { // 斜行流
            if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {    // パイプノズル（フランジ付き）
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 1;
            }
        }
    }
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // } else if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // } else if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // }
    // }
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // } else if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // }
    // }
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // } else if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // }
    // }
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { //　端管が付く
    // iCntB+=1;
    // } else if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntB+=1;
    // }
    // }
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { //　端管が付く
    // iCntA+=1;
    // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") {         //パイプノズル（フランジ付き）
    // iCntA+=1;
    // }
    // }
    // *********************

    //2018.11.08 相フランジ仕様変更_E
    //Get_IFrgBltSize(ZenToHan(P_HEADS_DATA[1368].TrimRight()), &P_Siyou,
    //    &iBltNum);
    Get_IFrgBltSize(ZenToHan(P_HEADS_DATA[1369].TrimRight()), &P_Siyou,
        &iBltNum);
    //2018.11.08 相フランジ仕様変更_E

    // 材質ｺｰﾄﾞの取得
    P_Sikibetu = "BFBLT-"; // 部品分類
    P_Katasiki = "ボルトナット"; // 部品型式
    P_Zuban = ""; // 図版（購入品なので図番は無し）
    P_SZuban = ""; // 作図用図番（購入品なので図番は無し）
    P_Yobihin = ""; // 予備品識別子
    P_Siyou2 = ""; // 仕様2
    P_Siyou5 = "A";
    P_Zaisitu = "SUS304";
    P_EZaisitu = "SUS304";
    // 2012.05.09 ﾎﾞﾙﾄﾅｯﾄ材質変更
    // 2016.11.07 材質ｺｰﾄﾞ変更
    //if ((P_HEADS_DATA[1068].TrimRight() == "1040") ||
    //    (P_HEADS_DATA[1068].TrimRight() == "1062")) {
    //    // 相フランジ材質がSS400,C.S.
    //    if ((P_HEADS_DATA[1051].TrimRight() == "0000") ||
    //        (P_HEADS_DATA[1051].TrimRight() == "1040") ||
    //        (P_HEADS_DATA[1051].TrimRight() == "1062")) {
    //        // Ａ側フランジ材質が指定なし,SS400,C.S.
    //        P_Zaisitu = "SS400";
    //        P_EZaisitu = "SS400";
    //    }
    //}
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // 相フランジ材質がSS400,C.S.
        if (Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "-" ||
            Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "SS400") {
            // Ａ側フランジ材質が指定なし,SS400,C.S.
            P_Zaisitu = "SS400";
            P_EZaisitu = "SS400";
        }
    }
    //2017.04.20 ASME,JPI材質変更
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "S25C") {
        // 相フランジ材質がS25C
        if (Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "-" ||
            Type_MAT(P_HEADS_DATA[1051].TrimRight()) == "SS400") {
            // Ａ側フランジ材質が指定なし,SS400,C.S.
            P_Zaisitu = "SS400";
            P_EZaisitu = "SS400";
        }
    }
    //***************************
    // ***********************
    // **************************
    P_Num = iCntA * iBltNum;
    P_Unit = "組";
    P_Hosoku = "";

    // *********************************
    // ***  予備品数 取得
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

    // ログ
    G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
    Write_Log(G_Log_Text);

    // *********************************
    // ***  付属品ﾃﾞｰﾀ(ﾗﾁｪｯﾄｽﾊﾟﾅ)　作成
    // *********************************
    if (iCntA > 0)
        Write_Axsrs_Data(s_Code);

    //2018.11.08 相フランジ仕様変更_S
    Get_IFrgBltSize(ZenToHan(P_HEADS_DATA[1368].TrimRight()), &P_Siyou,
        &iBltNum);
    //2018.11.08 相フランジ仕様変更_E

    P_Siyou5 = "B";
    // 2012.05.09 ﾎﾞﾙﾄﾅｯﾄ材質変更
    P_Zaisitu = "SUS304";
    P_EZaisitu = "SUS304";
    // 2016.11.07 材質ｺｰﾄﾞ変更
    //if ((P_HEADS_DATA[1068].TrimRight() == "1040") ||
    //    (P_HEADS_DATA[1068].TrimRight() == "1062")) {
    //    // 相フランジ材質がSS400,C.S.
    //    if ((P_HEADS_DATA[1050].TrimRight() == "0000") ||
    //        (P_HEADS_DATA[1050].TrimRight() == "1040") ||
    //        (P_HEADS_DATA[1050].TrimRight() == "1062")) {
    //        // Ｂ側フランジ材質が指定なし,SS400,C.S.
    //        P_Zaisitu = "SS400";
    //        P_EZaisitu = "SS400";
    //    }
    //}
    if (Type_MAT(P_HEADS_DATA[1068].TrimRight()) == "SS400") {
        // 相フランジ材質がSS400,C.S.
        if (Type_MAT(P_HEADS_DATA[1050].TrimRight()) == "-" ||
            Type_MAT(P_HEADS_DATA[1050].TrimRight()) == "SS400") {
            // Ｂ側フランジ材質が指定なし,SS400,C.S.
            P_Zaisitu = "SS400";
            P_EZaisitu = "SS400";
        }
    }
    // ***********************
    // **************************
    P_Num = iCntB * iBltNum;
    if (iCntB > 0)
        Write_Axsrs_Data(s_Code);
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： シートガスケット部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2004.02.24追加
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_STGkt_Data(void) {
    AnsiString s_Code; // 部品ｺｰﾄﾞ
    int iCntA;
    int iCntB;
    // 2010.07.14 斜行流追加
    int iFL;
    // *********************
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    AnsiString s_GasMat;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
    AnsiString s_GasSiyouA;
    AnsiString s_GasSiyouB;
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E

    // *********************************
    // ***  付属品ﾃﾞｰﾀ(ｼｰﾄｶﾞｽｹｯﾄ)　作成
    // *********************************
    iCntA = 0;
    iCntB = 0;
    // 2010.07.14 斜行流追加
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        iFL = 1; // 斜行流
    }
    // 2022.11.15 HERP修正_S
    else if  (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
        iFL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        iFL = 1; // 斜行流
    }
    // 2022.11.15 HERP修正_E
    else {
        iFL = 0; // 台形流
    }
    // 数量算出（サニタリーには付かない）
    // S1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[891].TrimRight() == "1"
    // || P_HEADS_DATA[891].TrimRight() == "2"
    // || P_HEADS_DATA[891].TrimRight() == "3" )
    // && ( P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // iCntB+=0;
    // } else if ( P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight()
        == "2" || P_HEADS_DATA[891].TrimRight() == "3") &&
        (P_HEADS_DATA[893].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[893].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "1") { // ラバーブーツ
            iCntB += 0;
        }
        else if (P_HEADS_DATA[893].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntB += 0;
        }
        else {
            if (P_HEADS_DATA[1136].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                //P_Siyou = P_HEADS_DATA[1184].TrimRight(); // 仕様
                s_GasSiyouB = P_HEADS_DATA[1184].TrimRight(); // Ｂ仕様
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
            }
            else {
                iCntB += 0;
            }
        }
    }
    // S2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[906].TrimRight() == "1"
    // || P_HEADS_DATA[906].TrimRight() == "2"
    // || P_HEADS_DATA[906].TrimRight() == "3" )
    // && ( P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // iCntA+=0;
    // } else if ( P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight()
        == "2" || P_HEADS_DATA[906].TrimRight() == "3") &&
        (P_HEADS_DATA[908].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[908].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "1") { // ラバーブーツ
            iCntA += 0;
        }
        else if (P_HEADS_DATA[908].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntA += 0;
        }
        else {
            if (P_HEADS_DATA[1137].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                //P_Siyou = P_HEADS_DATA[1188].TrimRight(); // 仕様
                s_GasSiyouA = P_HEADS_DATA[1188].TrimRight(); // Ａ仕様
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
            }
            else {
                iCntA += 0;
            }
        }
    }
    // S3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[921].TrimRight() == "1"
    // || P_HEADS_DATA[921].TrimRight() == "2"
    // || P_HEADS_DATA[921].TrimRight() == "3" )
    // && ( P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight()
        == "2" || P_HEADS_DATA[921].TrimRight() == "3") &&
        (P_HEADS_DATA[923].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {           // ラバーブーツ
            // iCntB+=0;
            // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {    // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntB += 0;
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1192].TrimRight(); // 仕様
                    s_GasSiyouB = P_HEADS_DATA[1192].TrimRight(); // Ｂ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
        else { // 斜行流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {           // ラバーブーツ
            // iCntA+=0;
            // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") {    // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntA += 0;
            }
            else if (P_HEADS_DATA[923].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1138].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1192].TrimRight(); // 仕様
                    s_GasSiyouA = P_HEADS_DATA[1192].TrimRight(); // Ａ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
    }
    // S4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[936].TrimRight() == "1"
    // || P_HEADS_DATA[936].TrimRight() == "2"
    // || P_HEADS_DATA[936].TrimRight() == "3" )
    // && ( P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight()
        == "2" || P_HEADS_DATA[936].TrimRight() == "3") &&
        (P_HEADS_DATA[938].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntA += 0;
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1196].TrimRight(); // 仕様
                    s_GasSiyouA = P_HEADS_DATA[1196].TrimRight(); // Ａ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
        else { // 斜行流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntB += 0;
            }
            else if (P_HEADS_DATA[938].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1139].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1196].TrimRight(); // 仕様
                    s_GasSiyouB = P_HEADS_DATA[1196].TrimRight(); // Ｂ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
    }
    // E1
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[951].TrimRight() == "1"
    // || P_HEADS_DATA[951].TrimRight() == "2"
    // || P_HEADS_DATA[951].TrimRight() == "3" )
    // && ( P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // iCntB+=0;
    // } else if ( P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight()
        == "2" || P_HEADS_DATA[951].TrimRight() == "3") &&
        (P_HEADS_DATA[953].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[953].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "1") { // ラバーブーツ
            iCntB += 0;
        }
        else if (P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntB += 0;
        }
        else {
            if (P_HEADS_DATA[1140].ToIntDef(0) != 0) { // 端管が付く
                iCntB += 1;
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                //P_Siyou = P_HEADS_DATA[1200].TrimRight(); // 仕様
                s_GasSiyouB = P_HEADS_DATA[1200].TrimRight(); // Ｂ仕様
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
            }
            else {
                iCntB += 0;
            }
        }
    }
    // E2
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[966].TrimRight() == "1"
    // || P_HEADS_DATA[966].TrimRight() == "2"
    // || P_HEADS_DATA[966].TrimRight() == "3" )
    // && ( P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7" ) ) {
    // if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" ) {             // ラバーブーツ
    // iCntA+=0;
    // } else if ( P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4" ) {      // パイプノズル（フランジ付き）
    if ((P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight()
        == "2" || P_HEADS_DATA[966].TrimRight() == "3") &&
        (P_HEADS_DATA[968].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[968].TrimRight().SubString(1, 2) != "07")) {
        if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "1") { // ラバーブーツ
            iCntA += 0;
        }
        else if (P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "4")
        { // パイプノズル（フランジ付き）
            // ***********************
            iCntA += 0;
        }
        else {
            if (P_HEADS_DATA[1141].ToIntDef(0) != 0) { // 端管が付く
                iCntA += 1;
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                //P_Siyou = P_HEADS_DATA[1204].TrimRight(); // 仕様
                s_GasSiyouA = P_HEADS_DATA[1204].TrimRight(); // Ａ仕様
                // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
            }
            else {
                iCntA += 0;
            }
        }
    }
    // E3
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[981].TrimRight() == "1"
    // || P_HEADS_DATA[981].TrimRight() == "2"
    // || P_HEADS_DATA[981].TrimRight() == "3" )
    // && ( P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight()
        == "2" || P_HEADS_DATA[981].TrimRight() == "3") &&
        (P_HEADS_DATA[983].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[983].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntB += 0;
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1208].TrimRight(); // 仕様
                    s_GasSiyouB = P_HEADS_DATA[1208].TrimRight(); // Ｂ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
        else { // 斜行流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntA += 0;
            }
            else if (P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1142].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1208].TrimRight(); // 仕様
                    s_GasSiyouA = P_HEADS_DATA[1208].TrimRight(); // Ａ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
    }
    // E4
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // if ( ( P_HEADS_DATA[996].TrimRight() == "1"
    // || P_HEADS_DATA[996].TrimRight() == "2"
    // || P_HEADS_DATA[996].TrimRight() == "3" )
    // && ( P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6"
    // && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7" ) ) {
    if ((P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight()
        == "2" || P_HEADS_DATA[996].TrimRight() == "3") &&
        (P_HEADS_DATA[998].TrimRight().SubString(1,
        2) != "06" && P_HEADS_DATA[998].TrimRight().SubString(1, 2) != "07")) {
        // ***********************
        if (iFL == 0) { // 台形流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
            // iCntA+=0;
            // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntA += 0;
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntA += 0;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                    iCntA += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1212].TrimRight(); // 仕様
                    s_GasSiyouA = P_HEADS_DATA[1212].TrimRight(); // Ａ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntA += 0;
                }
            }
        }
        else { // 斜行流
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" ) {         // ラバーブーツ
            // iCntB+=0;
            // } else if ( P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4" ) {  // パイプノズル（フランジ付き）
            if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "1")
            { // ラバーブーツ
                iCntB += 0;
            }
            else if (P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "4")
            { // パイプノズル（フランジ付き）
                // ***********************
                iCntB += 0;
            }
            else {
                if (P_HEADS_DATA[1143].ToIntDef(0) != 0) { // 端管が付く
                    iCntB += 1;
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
                    //P_Siyou = P_HEADS_DATA[1212].TrimRight(); // 仕様
                    s_GasSiyouB = P_HEADS_DATA[1212].TrimRight(); // Ｂ仕様
                    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
                }
                else {
                    iCntB += 0;
                }
            }
        }
    }
    // if( (P_HEADS_DATA[891].TrimRight() == "1" || P_HEADS_DATA[891].TrimRight() == "2" || P_HEADS_DATA[891].TrimRight() == "3") && (P_HEADS_DATA[893].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[893].TrimRight().SubString(1,1) != "7") ){
    // if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntB+=0;
    // } else if (P_HEADS_DATA[893].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1136].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1184].TrimRight();                    // 仕様
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[906].TrimRight() == "1" || P_HEADS_DATA[906].TrimRight() == "2" || P_HEADS_DATA[906].TrimRight() == "3") && (P_HEADS_DATA[908].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[908].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntA+=0;
    // } else if (P_HEADS_DATA[908].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1137].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1188].TrimRight();                    // 仕様
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[921].TrimRight() == "1" || P_HEADS_DATA[921].TrimRight() == "2" || P_HEADS_DATA[921].TrimRight() == "3") && (P_HEADS_DATA[923].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[923].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntB+=0;
    // } else if (P_HEADS_DATA[923].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1138].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1192].TrimRight();                    // 仕様
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[936].TrimRight() == "1" || P_HEADS_DATA[936].TrimRight() == "2" || P_HEADS_DATA[936].TrimRight() == "3") && (P_HEADS_DATA[938].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[938].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntA+=0;
    // } else if (P_HEADS_DATA[938].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1139].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1196].TrimRight();                    // 仕様
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[951].TrimRight() == "1" || P_HEADS_DATA[951].TrimRight() == "2" || P_HEADS_DATA[951].TrimRight() == "3") && (P_HEADS_DATA[953].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[953].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntB+=0;
    // } else if (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1140].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1200].TrimRight();                    // 仕様
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[966].TrimRight() == "1" || P_HEADS_DATA[966].TrimRight() == "2" || P_HEADS_DATA[966].TrimRight() == "3") && (P_HEADS_DATA[968].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[968].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntA+=0;
    // } else if (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1141].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1204].TrimRight();                    // 仕様
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[981].TrimRight() == "1" || P_HEADS_DATA[981].TrimRight() == "2" || P_HEADS_DATA[981].TrimRight() == "3") && (P_HEADS_DATA[983].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[983].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntB+=0;
    // } else if (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntB+=0;
    // } else {
    // if (P_HEADS_DATA[1142].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntB+=1;
    // P_Siyou = P_HEADS_DATA[1208].TrimRight();                    // 仕様
    // } else {
    // iCntB+=0;
    // }
    // }
    // }
    // if( (P_HEADS_DATA[996].TrimRight() == "1" || P_HEADS_DATA[996].TrimRight() == "2" || P_HEADS_DATA[996].TrimRight() == "3") && (P_HEADS_DATA[998].TrimRight().SubString(1,1) != "6" && P_HEADS_DATA[998].TrimRight().SubString(1,1) != "7") ) {
    // if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1") {        // ラバーブーツ
    // iCntA+=0;
    // } else if (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "4") { // パイプノズル（フランジ付き）
    // iCntA+=0;
    // } else {
    // if (P_HEADS_DATA[1143].ToIntDef(0) != 0) {                     // 端管が付く
    // iCntA+=1;
    // P_Siyou = P_HEADS_DATA[1212].TrimRight();                    // 仕様
    // } else {
    // iCntA+=0;
    // }
    // }
    // }
    // *********************

    // 材質ｺｰﾄﾞの取得
    P_Sikibetu = "STGKT-"; // 部品分類
    P_Katasiki = "シートガスケット"; // 部品型式
    P_Zuban = ""; // 図版（購入品なので図番は無し）
    P_SZuban = ""; // 作図用図番（購入品なので図番は無し）
    P_Yobihin = ""; // 予備品識別子

    // 2020.12.16 ｼｰﾄｶﾞｽｹｯﾄ修正_S
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
    //if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
    //    iCntA = iCntA + iCntB;
    //    iCntB = 0;
    //}
    if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B") &&
        s_GasSiyouA == s_GasSiyouB) {
        iCntA = iCntA + iCntB;
        iCntB = 0;
    }
    // 2020.12.16 ｼｰﾄｶﾞｽｹｯﾄ修正_E

    P_Siyou = s_GasSiyouA;
    if ( iCntA > 0 && iCntB > 0 ) {
        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(A側)
        if (sSpecCode == "A") {
            // ミラー仕様 Ａ号機
            P_Siyou = P_Siyou + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
        } else if (sSpecCode == "B") {
            // ミラー仕様 Ｂ号機
            P_Siyou = P_Siyou + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
        } else {
            P_Siyou = P_Siyou + "　Ａ側用";                 // 仕様
        }
    }
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_E
    P_Siyou2 = ""; // 仕様2
    P_Siyou5 = "A";
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[836].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
    P_Num = iCntA;
    P_Unit = "";
    P_Hosoku = "";

    // 2020.12.16 シートガスケット名称変更_S
    if ( P_GasNameA == "リングガスケット" ) {
        // ﾘﾝｸﾞｶﾞｽｹｯﾄ
        P_Sikibetu = "STGKTR";                   // 部品分類
        P_Katasiki = "リングガスケット";         // 部品型式
    } else if ( P_GasNameA == "全面座フランジガスケット" ) {
        // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
        P_Sikibetu = "STGKTF";                   // 部品分類
        P_Katasiki = "全面座フランジガスケット"; // 部品型式
    } else {
        // その他
    }
    // 2020.12.16 シートガスケット名称変更_E

    // *********************************
    // ***  予備品数 取得
    // *********************************
    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData); // 予備数

    // ログ
    G_Log_Text = "部品型式           ｢" + P_Katasiki + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
    Write_Log(G_Log_Text);

    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
    //if (iCntA > 0)
    //    Write_Axsrs_Data(s_Code);
    if (iCntA > 0) {
        Write_Axsrs_Data(s_Code);
    }

    P_Siyou = s_GasSiyouB; // 仕様
    if ( iCntB > 0 ) {
        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(B側)
        if (sSpecCode == "A") {
            // ミラー仕様 Ａ号機
            P_Siyou = P_Siyou + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
        } else if (sSpecCode == "B") {
            // ミラー仕様 Ｂ号機
            P_Siyou = P_Siyou + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
        } else {
            P_Siyou = P_Siyou + "　Ｂ側用";                 // 仕様
        }
    }
    // 2018.05.28  ｼｰﾄｶﾞｽｹｯﾄ貼分け区分_S
    P_Siyou5 = "B";
    P_Num = iCntB;
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
    //P_Zaisitu = Search_HD_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    //P_EZaisitu = Search_HD_E_MATERIAL
    //    (Chg_Zaisitu_Code(P_HEADS_DATA[835].TrimRight(), 2));
    s_GasMat = Get_SheetGasketMat(P_Siyou5);  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
    //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E

    // 2020.12.16 シートガスケット名称変更_S
    if ( P_GasNameB == "リングガスケット" ) {
        // ﾘﾝｸﾞｶﾞｽｹｯﾄ
        P_Sikibetu = "STGKTR";                   // 部品分類
        P_Katasiki = "リングガスケット";         // 部品型式
    } else if ( P_GasNameB == "全面座フランジガスケット" ) {
        // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
        P_Sikibetu = "STGKTF";                   // 部品分類
        P_Katasiki = "全面座フランジガスケット"; // 部品型式
    } else {
        // その他
    }
    // 2020.12.16 シートガスケット名称変更_E

    // 2020.12.16 ｼｰﾄｶﾞｽｹｯﾄ修正_S
    //if (iCntB > 0)
    if (iCntB > 0) {
    // 2020.12.16 ｼｰﾄｶﾞｽｹｯﾄ修正_E

        Write_Axsrs_Data(s_Code);

    // 2020.12.16 ｼｰﾄｶﾞｽｹｯﾄ修正_S
    }
    // 2020.12.16 ｼｰﾄｶﾞｽｹｯﾄ修正_E

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 舶用以外ストレーナ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2020.04.23追加
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails:: Get_Strainer_Data(void) {
    AnsiString s_Code;   // 部品ｺｰﾄﾞ
    AnsiString s_Key;    // 検索ｷｰ
    AnsiString s_SQL;
    AnsiString s_Spec;   // 仕様
    AnsiString s_Dno2;   // ｽﾄﾚｰﾅ用図番2
    AnsiString s_WKDno2; // ｽﾄﾚｰﾅ用作業図番2
    AnsiString s_GasMat; // ｶﾞｽｹｯﾄ材質CODE
    AnsiString sTugite;  // ﾉｽﾞﾙ継手
    int iCntA;
    int iCntB;
    int iCnt2A;
    int iCnt2B;
    int iFL;
    AnsiString s_TextA;
    AnsiString s_TextB;
    AnsiString s_GasSiyou;
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();

    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    // ノズルコードの取得
    AnsiString s_Nzl_Kind = "";
    if (P_HEADS_DATA[951].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[953].TrimRight(); // E1ﾉｽﾞﾙ継手
    } else if (P_HEADS_DATA[966].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[968].TrimRight(); // E2ﾉｽﾞﾙ継手
    } else if (P_HEADS_DATA[981].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[983].TrimRight(); // E3ﾉｽﾞﾙ継手
    } else if (P_HEADS_DATA[996].ToIntDef(0) == 5) {
        sTugite = P_HEADS_DATA[998].TrimRight(); // E3ﾉｽﾞﾙ継手
    }
    if (sTugite.ToIntDef(0) >= 1000) {
        s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
    }
    else {
        s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
    }

    // *********************************************
    // ***　舶用付属品ﾏｽﾀ(HD_ACCESSORY) 検索
    // *********************************************
    // 検索キー
    s_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_ACCESSORY";
    s_SQL = s_SQL + "  WHERE SERIES_MODEL = '" + s_Key + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ｼﾘｰｽﾞ型式｢" + s_Key +
            "｣ は、舶用付属品ﾏｽﾀ(HD_ACCESSORY)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        P_Hosoku = "";
        // ｺｰﾄﾞの取得
        s_Code = wkQuery->FieldByName("PARTS_CODE")->AsString.TrimRight();
        // 部品ｺｰﾄﾞ
        s_Spec = wkQuery->FieldByName("PARTS_SPEC")->AsString.TrimRight(); // 仕様
        P_Sikibetu = wkQuery->FieldByName("PL_SYMBOL")->AsString.TrimRight();
        // 部品表識別子
        P_Katasiki = wkQuery->FieldByName("PARTS_NAME")
            ->AsString.TrimRight(); // 部品名称
        P_Zaisitu = wkQuery->FieldByName("MAT_NAME")->AsString.TrimRight
            (); // 材質名称
        P_EZaisitu = Search_HD_MAT_JtoE(P_Zaisitu); // 材質名称
        P_Zuban = wkQuery->FieldByName("DNO")->AsString.TrimRight(); // 図番
        P_SZuban = wkQuery->FieldByName("WKDNO")->AsString.TrimRight(); // 作業図番
        s_Dno2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        // ストレーナ用図番２
        s_WKDno2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // ストレーナ用作業図番２
        P_Num = StrToInt(wkQuery->FieldByName("NUMS")->AsString.TrimRight
            ()); // 数量
        P_Unit = "";
        P_Yobihin = wkQuery->FieldByName("SP_SYMBOL")->AsString.TrimRight();
        // 予備品識別子


        // 舶用でないので、ストレーナ、ストレーナ用ガスケット関係以外を除外する
        if (s_Code != "17" && s_Code != "23" && s_Code != "24" &&
            s_Code != "26" && s_Code != "27") {
            // 17 ｽﾄﾚｰﾅ
            // 23 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
            // 24 ｼｰﾄｶﾞｽｹｯﾄ
            // 26 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
            // 27 ﾎﾞﾙﾄﾅｯﾄ
            wkQuery->Next();
            continue;
        }

        AnsiString sNRLKind = wkQuery->FieldByName("NRL_KIND")->AsString.TrimRight();
        // PARTS_CODEが以下であればノズル規格比較を行う
        // 23 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
        // 24 ｼｰﾄｶﾞｽｹｯﾄ
        // 25 閉止ｼｰﾄｶﾞｽｹｯﾄ
        // 26 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
        // 27 ﾎﾞﾙﾄﾅｯﾄ
        if (( s_Code == "23" || s_Code == "24" || s_Code == "25" ||
              s_Code == "26" || s_Code == "27" ) &&
            ( s_Nzl_Kind != sNRLKind )){
                wkQuery->Next();
                continue;
        }

        // ｺｰﾄﾞがある 相フランジ または、ｼｰﾄｶﾞｽｹｯﾄ
        if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
            iFL = 1; // 斜行流
        }
        else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
            iFL = 1; // 斜行流
        }
        // 2022.11.15 HERP修正_S
        else if  (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ") {
            iFL = 1; // 斜行流
        }
        else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
            iFL = 1; // 斜行流
        }
        else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
            iFL = 1; // 斜行流
        }
        // 2022.11.15 HERP修正_E
        else {
            iFL = 0; // 台形流
        }

        // 閉止(ブラインド)フランジ
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" && s_Code == "23") {
            iCntA = 0;
            iCntB = 0;

            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5" ) {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5" ) {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5" ) {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5" ) {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5" ) {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            P_Siyou5 = "A";
            if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-") {
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[265].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[265].TrimRight());
            }
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }

        // シートガスケット
        if (s_Code == "24") {
            iCntA = 0;
            iCntB = 0;
            iCnt2A = 0;
            iCnt2B = 0;

            // ストレーナＳ側抜止め用シートガスケット
            // すべての型式にＳ側抜止めが存在する
            // Ｅ１ストレーナ＆Ｓ１ノズル
            if ((P_HEADS_DATA[951].TrimRight() == "5") &&
                (P_HEADS_DATA[891].TrimRight() == "1" ||
                 P_HEADS_DATA[891].TrimRight() == "2") &&
                (P_HEADS_DATA[893].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                iCntB += 1; // 台形流
            }
            // Ｅ２ストレーナ＆Ｓ２ノズル
            if ((P_HEADS_DATA[966].TrimRight() == "5") &&
                (P_HEADS_DATA[906].TrimRight() == "1" ||
                 P_HEADS_DATA[906].TrimRight() == "2") &&
                (P_HEADS_DATA[908].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                iCntA += 1; // 台形流
            }
            // Ｅ３ストレーナ＆Ｓ３ノズル
            if ((P_HEADS_DATA[981].TrimRight() == "5") &&
                (P_HEADS_DATA[921].TrimRight() == "1" ||
                 P_HEADS_DATA[921].TrimRight() == "2") &&
                (P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[923].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntB += 1; // 台形流
                }
                else {
                    iCntA += 1; // 斜行流
                }
            }
            // Ｅ４ストレーナ＆Ｓ４ノズル
            if ((P_HEADS_DATA[996].TrimRight() == "5") &&
                (P_HEADS_DATA[936].TrimRight() == "1" ||
                P_HEADS_DATA[936].TrimRight() == "2") &&
                (P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "06" && P_HEADS_DATA[938].TrimRight().SubString(1,
                2) != "07")) {
                if (iFL == 0) {
                    iCntA += 1; // 台形流
                }
                else {
                    iCntB += 1; // 斜行流
                }
            }

            if (s_Key == "LX-10") {
                if (P_HEADS_DATA[981].TrimRight() == "5" && P_HEADS_DATA[983]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2B += 1;
                if (P_HEADS_DATA[996].TrimRight() == "5" && P_HEADS_DATA[998]
                    .TrimRight().SubString(3, 1) != "1")
                    iCnt2A += 1;

                if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                    iCnt2A = iCnt2A + iCnt2B;
                    iCnt2B = 0;
                }

                if (iCnt2B != 0) {
                    s_GasMat = Get_SheetGasketMat("B");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    P_Num = iCnt2B;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // 仕様
                    P_Siyou5 = "B";

                    if (iCnt2A != 0) {
                        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(B側)
                        if (sSpecCode == "A") {
                            // ミラー仕様 Ａ号機
                            s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                        } else if (sSpecCode == "B") {
                            // ミラー仕様 Ｂ号機
                            s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                        } else {
                            s_Spec = s_Spec + "　Ｂ側用";                 // 仕様
                        }
                    }

                    // 2020.12.16 シートガスケット名称変更_S
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                    // 2020.12.16 シートガスケット名称変更_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // 仕様
                }
                if (iCnt2A != 0) {
                    s_GasMat = Get_SheetGasketMat("A");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                    s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                    P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                    P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                    P_Num = iCnt2A;
                    P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0",
                        m_pcHeadsData);
                    s_Spec = "JIS10K-80A"; // 仕様
                    P_Siyou5 = "A";

                    if (iCnt2B != 0) {
                        // ｼｰﾄｶﾞｽｹｯﾄ貼分け(A側)
                        if (sSpecCode == "A") {
                            // ミラー仕様 Ａ号機
                            s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                        } else if (sSpecCode == "B") {
                            // ミラー仕様 Ｂ号機
                            s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                        } else {
                            s_Spec = s_Spec + "　Ａ側用";                 // 仕様
                        }
                    }

                    // 2020.12.16 シートガスケット名称変更_S
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                    // 2020.12.16 シートガスケット名称変更_E

                    Write_Funeyou_Data(s_Code, s_Spec);
                    s_Spec = "JIS10K-100A"; // 仕様
                }
            }

            if (Get_SheetGasketMat("A") == Get_SheetGasketMat("B")) {
                iCntA = iCntA + iCntB;
                iCntB = 0;
            }
            s_GasSiyou = s_Spec;

            if (iCntB != 0) {
                s_GasMat = Get_SheetGasketMat("B");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";

                if (iCntA != 0) {
                    // ｼｰﾄｶﾞｽｹｯﾄ貼分け(B側)
                    if (sSpecCode == "A") {
                        // ミラー仕様 Ａ号機
                        s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                    } else if (sSpecCode == "B") {
                        // ミラー仕様 Ｂ号機
                        s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                    } else {
                        s_Spec = s_Spec + "　Ｂ側用";                 // 仕様
                    }
                }

                // 2020.12.16 シートガスケット名称変更_S
                if ( P_GasNameB == "リングガスケット" ) {
                    // ﾘﾝｸﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                } else if ( P_GasNameB == "全面座フランジガスケット" ) {
                    // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTF";                   // 部品分類
                    P_Katasiki = "全面座フランジガスケット"; // 部品型式
                } else {
                    // その他
                }
                // 2020.12.16 シートガスケット名称変更_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            s_Spec = s_GasSiyou;

            if (iCntA != 0) {
                s_GasMat = Get_SheetGasketMat("A");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                if (iCntB != 0) {
                    // ｼｰﾄｶﾞｽｹｯﾄ貼分け(A側)
                    if (sSpecCode == "A") {
                        // ミラー仕様 Ａ号機
                        s_Spec = s_Spec + "　Ａ号機Ａ側／Ｂ号機Ｂ側"; // 仕様
                    } else if (sSpecCode == "B") {
                        // ミラー仕様 Ｂ号機
                        s_Spec = s_Spec + "　Ａ号機Ｂ側／Ｂ号機Ａ側"; // 仕様
                    } else {
                        s_Spec = s_Spec + "　Ａ側用";                 // 仕様
                    }
                }

                // 2020.12.16 シートガスケット名称変更_S
                if ( P_GasNameA == "リングガスケット" ) {
                    // ﾘﾝｸﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTR";                   // 部品分類
                    P_Katasiki = "リングガスケット";         // 部品型式
                } else if ( P_GasNameA == "全面座フランジガスケット" ) {
                    // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
                    P_Sikibetu = "STGKTF";                   // 部品分類
                    P_Katasiki = "全面座フランジガスケット"; // 部品型式
                } else {
                    // その他
                }
                // 2020.12.16 シートガスケット名称変更_E

                Write_Funeyou_Data(s_Code, s_Spec);
            }

            s_Spec = s_GasSiyou;

            P_Num = 0;
        }

        //全面座閉止ガスケット
        if (Type_MAT(P_HEADS_DATA[265].TrimRight()) != "-" &&
            s_Code == "26") {
            iCntA = 0;
            iCntB = 0;

            // ｽﾄﾚｰﾅははすべてつく
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            if (iCntB != 0) {
                s_GasMat = Get_SheetGasketMat("B");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                P_Num = iCntB;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "B";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            if (iCntA != 0) {
                s_GasMat = Get_SheetGasketMat("A");  // ｼｰﾄｶﾞｽｹｯﾄ材質CODE
                s_GasMat = Chg_Zaisitu_Code(s_GasMat, 2);
                P_Zaisitu = Search_HD_MATERIAL(s_GasMat);
                P_EZaisitu = Search_HD_E_MATERIAL(s_GasMat);
                //2017.10.10 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E
                P_Num = iCntA;
                P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);
                P_Siyou5 = "A";
                Write_Funeyou_Data(s_Code, s_Spec);
            }
            P_Num = 0;
        }

        // Ｅ側ボルトナット
        if (s_Code == "27") {
            iCntA = 0;
            iCntB = 0;

            // 2022.09.21 E側ボルトナットSUS指定_S
            if (P_HEADS_DATA[162].TrimRight() == "1.00") {
                P_Zaisitu = "SUS304";
                P_EZaisitu = "SUS304";
            }
            // 2022.09.21 E側ボルトナットSUS指定_E

            // ｽﾄﾚｰﾅ
            // S1
            if (P_HEADS_DATA[891].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // S2
            if (P_HEADS_DATA[906].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // S3
            if (P_HEADS_DATA[921].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // S4
            if (P_HEADS_DATA[936].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            // E1
            if (P_HEADS_DATA[951].TrimRight() == "5") {
                iCntB += P_Num;
            }
            // E2
            if (P_HEADS_DATA[966].TrimRight() == "5") {
                iCntA += P_Num;
            }
            // E3
            if (P_HEADS_DATA[981].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntB += P_Num; // 台形流
                }
                else {
                    iCntA += P_Num; // 斜行流
                }
            }
            // E4
            if (P_HEADS_DATA[996].TrimRight() == "5") {
                if (iFL == 0) {
                    iCntA += P_Num; // 台形流
                }
                else {
                    iCntB += P_Num; // 斜行流
                }
            }
            P_Siyou5 = "A";
            P_Num = iCntA;
            if (iCntA > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            P_Siyou5 = "B";
            P_Num = iCntB;
            if (iCntB > 0)
                Write_Funeyou_Data(s_Code, s_Spec);
            wkQuery->Next();
            continue;
        }

        // *********************************
        // ***  予備品数 取得
        // *********************************
        P_Yobisu = Syukei_Buhin_Yobisu(P_Yobihin, "0", m_pcHeadsData);

        // ログ
        G_Log_Text = "部品ｺｰﾄﾞ           ｢" + s_Code + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "仕様               ｢" + s_Spec + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "部品表識別         ｢" + P_Sikibetu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "部品名称           ｢" + P_Katasiki + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "材質名称           ｢" + P_Zaisitu + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番               ｢" + P_Zuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番         ｢" + P_SZuban + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "数量               ｢" + AnsiString(P_Num) + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "予備品識別子       ｢" + P_Yobihin + "｣を取得。";
        Write_Log(G_Log_Text);

        // *********************************
        // ***  ｽﾄﾚｰﾅﾃﾞｰﾀ　作成
        // *********************************

        // 閉止フランジ、ガスケット、Ｅ側ボルトナットを除外する
        if ((Type_MAT(P_HEADS_DATA[265].TrimRight()) == "-") &&
            (s_Code == "23" || s_Code == "24" || s_Code == "26" || s_Code == "27")) {
            // 23 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
            // 24 ｼｰﾄｶﾞｽｹｯﾄ
            // 26 全面座閉止ｼｰﾄｶﾞｽｹｯﾄ
            // 27 E側ﾎﾞﾙﾄﾅｯﾄ
            wkQuery->Next();
            continue;
        }

        // ストレーナ
        if (s_Code == "17") {
        // 2019.09.09 E側ﾎﾞﾙﾄﾅｯﾄ追加_E　
            // 各ノズル種類にｽﾄﾚｰﾅが設定されていない
            if ((P_HEADS_DATA[891].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[906].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[921].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[936].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[951].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[966].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[981].ToIntDef(0) != 5) &&
                (P_HEADS_DATA[996].ToIntDef(0) != 5)) {
                wkQuery->Next();
                continue;
            }
            if (s_Key == "UX-90" || s_Key == "SX-90M") {
                // UX-90,SX-90M
                if (P_HEADS_DATA[38].ToIntDef(0) > 350) {
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ｽﾄﾚｰﾅがSUS316の場合
                        P_Zuban = "Ｅ３５９３０１－Ｒ１";
                        P_SZuban = "Ｅ３５９３０１";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ｽﾄﾚｰﾅがTP340の場合
                        P_Zuban = "Ｅ３５９３０２－Ｒ２";
                        P_SZuban = "Ｅ３５９３０２";
                    }
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // 懸垂枚数が350枚以下
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ｽﾄﾚｰﾅがTP340の場合は図番２
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
            }
            else if (s_Key == "RX-90") {
                // RX-90
                if (P_HEADS_DATA[38].ToIntDef(0) > 291) {
                    // 懸垂枚数が291枚以上
                    if (Type_MAT(P_HEADS_DATA[261].TrimRight()) == "SUS316") {
                        // ｽﾄﾚｰﾅがSUS316の場合
                        P_Zuban = "Ｅ３５９３３８－Ｒ２";
                        P_SZuban = "Ｅ３５９３３８";
                    }
                    else if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                             P_HEADS_DATA[261].Pos("3032") > 0 ||
                             P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ｽﾄﾚｰﾅがTP340の場合
                        P_Zuban = "Ｅ３５９５７２－Ｒ３";
                        P_SZuban = "Ｅ３５９５７２";
                    }
                    else {
                        P_Zuban = "";
                        P_SZuban = "";
                    }
                }
                else {
                    // 懸垂枚数が290枚以下
                    //if (P_HEADS_DATA[261].Pos("1032") > 0) {
                    if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                        P_HEADS_DATA[261].Pos("3032") > 0 ||
                        P_HEADS_DATA[261].Pos("3132") > 0) {
                        // ｽﾄﾚｰﾅがTP340の場合は図番２
                        P_Zuban = s_Dno2;
                        P_SZuban = s_WKDno2;
                    }
                }
            }
            else if (s_Key != "UX-01" && s_Key != "UX-10" && s_Key != "UX-20" &&
                s_Key != "UX-40" && s_Key != "LX-00" && s_Key != "LX-10" &&
                s_Key != "LX-20" && s_Key != "LX-40" && s_Key != "RX-90") {
                // UX-01,UX-10,UX-20,UX-40,LX-00,LX-10,LX-20,LX-40以外
                if (P_HEADS_DATA[261].Pos("1032") > 0 ||
                    P_HEADS_DATA[261].Pos("3032") > 0 ||
                    P_HEADS_DATA[261].Pos("3132") > 0) {
                    // ｽﾄﾚｰﾅがTP340の場合は図番２
                    P_Zuban = s_Dno2;
                    P_SZuban = s_WKDno2;
                }
            }

       //        '''''''''''''''
            Write_Error_Log("s_Code3");
            Write_Error_Log(s_Code);
        // ''''''''''''''''



            // ｽﾄﾚｰﾅの場合はｽﾄﾚｰﾅ材質を出力
                P_Zaisitu = Search_HD_MATERIAL(P_HEADS_DATA[261].TrimRight());
                P_EZaisitu =
                    Search_HD_E_MATERIAL(P_HEADS_DATA[261].TrimRight());
        }

        Write_Funeyou_Data(s_Code, s_Spec);
        wkQuery->Next();
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 予備品のみの部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::EtcData_Write(void) {
    // CSV読込用
    FILE *wkfp;

    char buf[1024]; // 文字列バッファ
    char *tok; // トークンポインタ
    int i; // ループカウンタ
    char token[100][100]; // トークンバッファ
    int i_LineNum;

    AnsiString s_Yobi_Text[5];
    AnsiString s_YBKey;
    AnsiString s_YBZaisitu;

    AnsiString wk_FilePass; // "D00001.csv"
    int i_Yobisu;

    // 2005-10-07
    int n; // 予備品比較用ループカウンタ
    int i_Chk; // 重複予備品チェックカウンタ
    int i_ErrNum; // 展開できなかった予備品カウンタ
    AnsiString s_ErrName[100]; // 展開できなかった予備品名
    int i_ErrYobiNum[100]; // 展開できなかった予備品数量

    i_Yobisu = 0;
    i_LineNum = 0;
    i_ErrNum = 0; // 2005-10-07

    // -----------------------------------------------
    // 2007.10.03 追加
    // ﾐﾗｰB仕様の場合は、予備品ﾃﾞｰﾀなしとして処理
    if (m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B) {
        return i_Yobisu;
    }
    // 2007.10.03
    // -----------------------------------------------

    // ﾌﾟﾚｰﾄ予備品ﾃﾞｰﾀﾊﾟｽ
    wk_FilePass = G_AnaFilePass + G_SAKUBAN + ".csv";

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "予備品ﾃﾞｰﾀ 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // 読込み可能の間
        memset(buf, 0x00, sizeof(buf));
        while (fgets(buf, sizeof(buf), wkfp) != NULL) {
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
            // ﾃﾞｰﾀ例
            s_Yobi_Text[0] = token[0];
            // 予備品ﾃﾞｰﾀ 1列目 ( 'PLTE','GSKT','ADHS','THRM'... )
            s_Yobi_Text[1] = token[1];
            // 予備品ﾃﾞｰﾀ 2列目 ( ﾌﾞﾗﾝｸ,'D','E','1','2'... )
            s_Yobi_Text[2] = token[8]; // 予備品ﾃﾞｰﾀ 予備数( 10, 1, 5, 2, 8, ... )
            s_Yobi_Text[3] = token[11]; // 予備品ﾃﾞｰﾀ 材質コード( 1011, ... )
            s_Yobi_Text[4] = token[2]; // 予備品ﾃﾞｰﾀ 材質コード( 1011, ... )

            // 検索ｷｰ生成
            if (AllNumChk(s_Yobi_Text[1].c_str(), 0) == 0) {
                int i_No;

                i_No = StrToInt(s_Yobi_Text[1]) + 100;
                s_YBKey = s_Yobi_Text[0] + IntToStr(i_No).SubString(2, 2);
                s_YBZaisitu = s_Yobi_Text[3].TrimRight();
                i_LineNum++;
            }
            else {
                // ********************************************
                // 予備品ﾃﾞｰﾀ 2列目が
                // ｱﾙﾌｧﾍﾞｯﾄもしくはNULLだった場合 検索しない?
                // ********************************************
                s_YBKey = "";
            }

            if (s_YBKey != "\0") {
                if ((s_YBKey.SubString(1, 4) == "ADHS") || (s_YBKey.SubString(1,
                    4) == "OILR")) {
                    P_Sikibetu = "ETCP--"; // 部品分類
                    P_Katasiki = s_Yobi_Text[4]; // 部品型式
                    P_Zuban = ""; // 図版（購入品なので図番は無し）
                    P_SZuban = ""; // 作図用図番（購入品なので図番は無し）
                    P_Yobihin = ""; // 予備品識別子
                    P_Siyou = ""; // 仕様
                    P_Siyou2 = ""; // 仕様2
                    P_Siyou5 = "";
                    P_Zaisitu = "";
                    P_EZaisitu = "";
                    P_Num = 0;
                    P_Yobisu = s_Yobi_Text[2].ToIntDef(0);
                    P_Unit = "";
                    P_Hosoku = "";
                    Write_Axsrs_Data("");

                    G_YOBICHK_NAME[G_YOBICHK_RecCnt] =
                        s_Yobi_Text[0] + s_Yobi_Text[1];
                    G_YOBICHK_MAT[G_YOBICHK_RecCnt] = s_Yobi_Text[3];
                    G_YOBICHK_RecCnt++;
                }
                else { // 2005-10-07 展開できていない予備品チェック
                    i_Chk = 0;
                    for (n = 0; n < G_YOBICHK_RecCnt; n++) {
                        // 重複チェック
                        if ((s_Yobi_Text[0] + s_Yobi_Text[1])
                            == G_YOBICHK_NAME[n]) {
                            i_Chk++;
                        }

                    }
                    // 重複しない予備品を取得
                    if (i_Chk == 0) {
                        s_ErrName[i_ErrNum] = s_Yobi_Text[4];
                        i_ErrYobiNum[i_ErrNum] = s_Yobi_Text[2].ToIntDef(0);
                        i_ErrNum++;
                    }
                }
            }
        }
    }

    // 展開不良をチェック
    if (i_LineNum != G_YOBICHK_RecCnt) {
        P_Sikibetu = "ERROR-"; // 部品分類
        P_Katasiki = "※展開できなかった予備品が存在します。"; // 部品型式
        P_Zuban = ""; // 図版（購入品なので図番は無し）
        P_SZuban = ""; // 作図用図番（購入品なので図番は無し）
        P_Yobihin = ""; // 予備品識別子
        P_Siyou = ""; // 仕様
        P_Siyou2 = ""; // 仕様2
        P_Siyou5 = "";
        P_Zaisitu = "";
        P_EZaisitu = "";
        P_Num = 0;
        P_Yobisu = 0;
        P_Unit = "";
        P_Hosoku = "";
        Write_Err_Data();
    }

    // 2005-10-07 展開できなかった予備品を出力
    if (i_LineNum != G_YOBICHK_RecCnt) {
        for (n = 0; n < i_ErrNum; n++) {
            P_Sikibetu = "ERROR-"; // 部品分類
            P_Katasiki = s_ErrName[n]; // 部品型式
            P_Zuban = ""; // 図版（購入品なので図番は無し）
            P_SZuban = ""; // 作図用図番（購入品なので図番は無し）
            P_Yobihin = ""; // 予備品識別子
            P_Siyou = ""; // 仕様
            P_Siyou2 = ""; // 仕様2
            P_Siyou5 = "";
            P_Zaisitu = "";
            P_EZaisitu = "";
            P_Num = 0;
            P_Yobisu = i_ErrYobiNum[n];
            P_Unit = "";
            P_Hosoku = "";
            Write_Err_Data();
        }
    }

    // 予備品ﾃﾞｰﾀ ﾌｧｲﾙｸﾛｰｽﾞ
    fclose(wkfp);
    return i_Yobisu;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 付属品ﾃﾞｰﾀﾌｧｲﾙ書込み処理
// 概  要      ： ﾄﾞﾚﾝﾊﾟﾝ,ｱﾝｶｰﾎﾞﾙﾄ
// 引  数      ： AnsiString a_SE   ： 圧力計SE区分(S1～S4,E1～E4)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Write_Err_Data(void) {
    FILE *wkfp;

    AnsiString s_Text;
    int i_Daisu;
    int i_Soukei;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = m_pcHeadsData->GetSpecCode();
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
    s_Text = "ERROR";
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
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), wkfp);

    // 13 仕様３
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);

    // 14 仕様４
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);

    // 15 仕様５
    s_Text = "," + P_Siyou5;
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
// 日本語関数名： 相フランジ用ボルトサイズをマスタから取得
// 概  要      ：
// 引  数      ： I:sKikaku     規格文字列
// :  O:s_retSize   ボルトサイズ仕様
// :  O:iBltNum     ボルト本数
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_IFrgBltSize(AnsiString sKikaku,
    AnsiString *s_retSize, int *iBltNum) {
    AnsiString s_Key;
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;

    G_Log_Text = "相ﾌﾗﾝｼﾞﾎﾞﾙﾄｻｲｽﾞﾏｽﾀ(HD_IFRG_BLT)を検索キー  『" + sKikaku + "』で検索。";
    Write_Log(G_Log_Text);

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_IFRG_BLT";
    s_SQL = s_SQL + "  WHERE NZ_KIKAKUK = '" + sKikaku + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ﾉｽﾞﾙ規格｢" + sKikaku +
            "｣ は、相ﾌﾗﾝｼﾞﾎﾞﾙﾄｻｲｽﾞﾏｽﾀ(HD_IFRG_BLT)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
    //2021.10.19 サイズに仕上げ追加_S
    //    *s_retSize = wkQuery->FieldByName("BLT_SIZE")->AsString.TrimRight()
    //        + "×全Ｓ"; // サイズ
        *s_retSize = wkQuery->FieldByName("BLT_SIZE")->AsString.TrimRight()
            + "×全Ｓ  三価クロメート"; // サイズ
    //2021.10.19 サイズに仕上げ追加_E
        *iBltNum = wkQuery->FieldByName("BLT_NUM")->AsCurrency; // １箇所あたりの本数

        // ログ
        G_Log_Text = "相ﾌﾗﾝｼﾞﾎﾞﾙﾄｻｲｽﾞ   ｢" + *s_retSize + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "相ﾌﾗﾝｼﾞﾎﾞﾙﾄ本数   ｢" + AnsiString(*iBltNum) + "｣を取得。";
        Write_Log(G_Log_Text);

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Ｓ側スタッドボルトサイズ（仕様欄表記）をマスタから取得
// 概  要      ：
// 引  数      ： I:sKatasiki   フレーム型式
// :  I:iFrgDiv     端管有無
// :  O:sSBLTSpec   ボルト仕様
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_SSTDBltSpec(AnsiString sKatasiki,
    int iFrgDiv, AnsiString *sSBLTSpec) {
    AnsiString s_Key;
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;

    G_Log_Text = "S側ｽﾀｯﾄﾞﾎﾞﾙﾄ仕様ﾏｽﾀ(HD_SSTD_SPEC)を検索キー  『" + sKatasiki + "』で検索。";
    Write_Log(G_Log_Text);


    // 2015.03.20 OGI 舶用部品重複対応(ノズル規格の種類が追加される項目)
    //s_SQL = "";
    //s_SQL = s_SQL + "SELECT * FROM HD_SSTD_SPEC";
    //s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + sKatasiki + "'";
    //s_SQL = s_SQL + "    AND FRG_DIV = " + AnsiString(iFrgDiv);
    // ノズルコードの取得
    AnsiString s_Nzl_Kind = "";
    AnsiString sTugite = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
    if (sTugite.ToIntDef(0) >= 1000) {
        s_Nzl_Kind = sTugite.SubString(1, 2) + "0" + sTugite.SubString(4, 1);
    }
    else {
        s_Nzl_Kind = sTugite.SubString(2, 1) + "0" + sTugite.SubString(4, 1);
    }
    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_SSTD_SPEC";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + sKatasiki + "'";
    s_SQL = s_SQL + "    AND NRL_KIND = " + s_Nzl_Kind;
    s_SQL = s_SQL + "    AND FRG_DIV = " + AnsiString(iFrgDiv);


    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    フレーム型式｢" + sKatasiki +
            "｣ は、S側ｽﾀｯﾄﾞﾎﾞﾙﾄ仕様ﾏｽﾀ(HD_SSTD_SPEC)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        *sSBLTSpec = wkQuery->FieldByName("BLT_SPEC")->AsString.TrimRight
            (); // サイズ

        // ログ
        G_Log_Text = "S側ｽﾀｯﾄﾞﾎﾞﾙﾄ仕様   ｢" + *sSBLTSpec + "｣を取得。";
        Write_Log(G_Log_Text);

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}


// ---------------------------------------------------------------------------
// 日本語関数名： 出荷用閉止フランジ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2014.12.22 追加
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_TFLG_Data() {
    AnsiString s_TFLG;       // 出荷用閉止ﾌﾗﾝｼﾞの有無
    AnsiString s_SILICAGEL;  // ｼﾘｶｹﾞﾙの有無
    AnsiString s_Code;       // 部品ｺｰﾄﾞ
    AnsiString s_Nzl_Syurui; // ﾉｽﾞﾙ種類
    AnsiString s_Nzl_Kokei;  // ﾉｽﾞﾙ口径
    AnsiString s_Nzl_Tugite; // ﾉｽﾞﾙ継手
    AnsiString s_DNO[9];     // ﾌﾗﾝｼﾞ図番
    AnsiString s_SPEC1[9];   // ﾌﾗﾝｼﾞ仕様1
    AnsiString s_SPEC2[9];   // ﾌﾗﾝｼﾞ仕様2
    int i_INOUT[9];          // 出入口有無
    int i_SI[9];             // ｼﾘｶｹﾞﾙ有無
    int i_GAS[9];            // ｶﾞｽｹｯﾄ有無
    int i_FL;                // 斜行流ﾌﾗｸﾞ
    int i_WkRec;             // 集計ﾃﾞｰﾀ数
    bool bMatch;
    AnsiString s_WK_DNO[8];  // ﾌﾗﾝｼﾞ図番(集計)
    AnsiString s_WK_SPEC1[8];// ﾌﾗﾝｼﾞ仕様1(集計)
    AnsiString s_WK_SPEC2[8];// ﾌﾗﾝｼﾞ仕様2(集計)
    int i_WK_FL[8];          // ﾌﾗﾝｼﾞ数量(集計)
    int i_WK_SI[8];          // ｼﾘｶｹﾞﾙ有無(集計)
    int i_WK_GAS[8];         // ｶﾞｽｹｯﾄ数量(集計)
    int i;
    int j;
    AnsiString s_code;
    AnsiString s_Text;
    // 2020.04.15 乾燥剤量追加_S
    AnsiString s_AB[9];           // AB区分
    AnsiString s_Silicagel[9];    // シリカゲル量(kg)
    AnsiString s_WK_Silicagel[8]; // シリカゲル量(kg)(集計)
    // 2020.04.15 乾燥剤量追加_E

    s_TFLG = P_HEADS_DATA[1114].TrimRight();   // 出荷用閉止ﾌﾗﾝｼﾞｺｰﾄﾞ
    if ( s_TFLG == "01" ) {
        s_TFLG = "1";      // 出荷用閉止ﾌﾗﾝｼﾞ(SS400)
        s_SILICAGEL = "0"; // ｼﾘｶｹﾞﾙ無
    } else if ( s_TFLG == "02" ) {
        s_TFLG = "1";      // 出荷用閉止ﾌﾗﾝｼﾞ(SS400)
        s_SILICAGEL = "1"; // ｼﾘｶｹﾞﾙ有
    } else if ( s_TFLG == "03" ) {
        s_TFLG = "2";      // 出荷用閉止ﾌﾗﾝｼﾞ(SUS304)
        s_SILICAGEL = "0"; // ｼﾘｶｹﾞﾙ無
    } else if ( s_TFLG == "04" ) {
        s_TFLG = "2";      // 出荷用閉止ﾌﾗﾝｼﾞ(SUS304)
        s_SILICAGEL = "1"; // ｼﾘｶｹﾞﾙ有
    } else {
        G_Log_Text = "出荷用閉止ﾌﾗﾝｼﾞは不必要";
        Write_Log(G_Log_Text);
        return true;
    }

    //******************
    //***  斜行流判断
    //******************
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
        i_FL = 1; // 斜行流
    } else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        i_FL = 1; // 斜行流
    } else {
        i_FL = 0; // 台形流
    }

    //**************
    //***  S1ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[891].TrimRight();   // S1ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[892].TrimRight();   // S1ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[893].TrimRight();   // S1ﾉｽﾞﾙ継手
    s_DNO[1] = "";                                  // S1ﾌﾗﾝｼﾞ図番
    s_SPEC1[1] = "";                                // S1ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[1] = "";                                // S1ﾌﾗﾝｼﾞ仕様2
    i_INOUT[1] = 0;                                 // S1出入口有無
    i_GAS[1] = 0;                                   // S1ｶﾞｽｹｯﾄ有無
    i_SI[1] = 0;                                    // S1ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[1] = 1;   // S1出入口有無
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[1], &s_SPEC1[1], &s_SPEC2[1]);
        i_GAS[1] = Get_TMGASKET(s_Nzl_Tugite, "N1", i_FL);

    }

    //**************
    //***  S2ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[906].TrimRight();   // S2ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[907].TrimRight();   // S2ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[908].TrimRight();   // S2ﾉｽﾞﾙ継手
    s_DNO[2] = "";                                  // S2ﾌﾗﾝｼﾞ図番
    s_SPEC1[2] = "";                                // S2ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[2] = "";                                // S2ﾌﾗﾝｼﾞ仕様2
    i_INOUT[2] = 0;                                 // S2出入口有無
    i_GAS[2] = 0;                                   // S2ｶﾞｽｹｯﾄ有無
    i_SI[2] = 0;                                    // S2ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[2] = 1;   // S2出入口有無_
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[2], &s_SPEC1[2], &s_SPEC2[2]);
        i_GAS[2] = Get_TMGASKET(s_Nzl_Tugite, "N2", i_FL);

    }

    //**************
    //***  S3ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[921].TrimRight();   // S3ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[922].TrimRight();   // S3ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[923].TrimRight();   // S3ﾉｽﾞﾙ継手
    s_DNO[3] = "";                                  // S3ﾌﾗﾝｼﾞ図番
    s_SPEC1[3] = "";                                // S3ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[3] = "";                                // S3ﾌﾗﾝｼﾞ仕様2
    i_INOUT[3] = 0;                                 // S3出入口有無
    i_GAS[3] = 0;                                   // S3ｶﾞｽｹｯﾄ有無
    i_SI[3] = 0;                                    // S3ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[3] = 1;   // S3出入口有無_
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[3], &s_SPEC1[3], &s_SPEC2[3]);
        i_GAS[3] = Get_TMGASKET(s_Nzl_Tugite, "N3", i_FL);

    }

    //**************
    //***  S4ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[936].TrimRight();   // S4ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[937].TrimRight();   // S4ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[938].TrimRight();   // S4ﾉｽﾞﾙ継手
    s_DNO[4] = "";                                  // S4ﾌﾗﾝｼﾞ図番
    s_SPEC1[4] = "";                                // S4ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[4] = "";                                // S4ﾌﾗﾝｼﾞ仕様2
    i_INOUT[4] = 0;                                 // S4出入口有無
    i_GAS[4] = 0;                                   // S4ｶﾞｽｹｯﾄ有無
    i_SI[4] = 0;                                    // S4ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[4] = 1;   // S4出入口有無_
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[4], &s_SPEC1[4], &s_SPEC2[4]);
        i_GAS[4] = Get_TMGASKET(s_Nzl_Tugite, "N4", i_FL);

    }


    //**************
    //***  E1ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[951].TrimRight();   // E1ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[952].TrimRight();   // E1ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[953].TrimRight();   // E1ﾉｽﾞﾙ継手
    s_DNO[5] = "";                                  // E1ﾌﾗﾝｼﾞ図番
    s_SPEC1[5] = "";                                // E1ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[5] = "";                                // E1ﾌﾗﾝｼﾞ仕様2
    i_INOUT[5] = 0;                                 // E1出入口有無
    i_GAS[5] = 0;                                   // E1ｶﾞｽｹｯﾄ有無
    i_SI[5] = 0;                                    // E1ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[5] = 1;   // E1出入口有無
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[5], &s_SPEC1[5], &s_SPEC2[5]);
        i_GAS[5] = Get_TMGASKET(s_Nzl_Tugite, "N1", i_FL);

    }


    //**************
    //***  E2ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[966].TrimRight();   // E2ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[967].TrimRight();   // E2ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[968].TrimRight();   // E2ﾉｽﾞﾙ継手
    s_DNO[6] = "";                                  // E2ﾌﾗﾝｼﾞ図番
    s_SPEC1[6] = "";                                // E2ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[6] = "";                                // E2ﾌﾗﾝｼﾞ仕様2
    i_INOUT[6] = 0;                                 // E2出入口有無
    i_GAS[6] = 0;                                   // E2ｶﾞｽｹｯﾄ有無
    i_SI[6] = 0;                                    // E2ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[6] = 1;   // E2出入口有無
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[6], &s_SPEC1[6], &s_SPEC2[6]);
        i_GAS[6] = Get_TMGASKET(s_Nzl_Tugite, "N2", i_FL);

    }


    //**************
    //***  E3ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[981].TrimRight();   // E3ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[982].TrimRight();   // E3ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[983].TrimRight();   // E3ﾉｽﾞﾙ継手
    s_DNO[7] = "";                                  // E3ﾌﾗﾝｼﾞ図番
    s_SPEC1[7] = "";                                // E3ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[7] = "";                                // E3ﾌﾗﾝｼﾞ仕様2
    i_INOUT[7] = 0;                                 // E3出入口有無
    i_GAS[7] = 0;                                   // E3ｶﾞｽｹｯﾄ有無
    i_SI[7] = 0;                                    // E3ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[7] = 1;   // E3出入口有無
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[7], &s_SPEC1[7], &s_SPEC2[7]);
        i_GAS[7] = Get_TMGASKET(s_Nzl_Tugite, "N3", i_FL);

    }


    //**************
    //***  E4ﾉｽﾞﾙ
    //**************
    s_Nzl_Syurui = P_HEADS_DATA[996].TrimRight();   // E4ﾉｽﾞﾙ種類
    s_Nzl_Kokei  = P_HEADS_DATA[997].TrimRight();   // E4ﾉｽﾞﾙ口径
    s_Nzl_Tugite = P_HEADS_DATA[998].TrimRight();   // E4ﾉｽﾞﾙ継手
    s_DNO[8] = "";                                  // E4ﾌﾗﾝｼﾞ図番
    s_SPEC1[8] = "";                                // E4ﾌﾗﾝｼﾞ仕様1
    s_SPEC2[8] = "";                                // E4ﾌﾗﾝｼﾞ仕様2
    i_INOUT[8] = 0;                                 // E4出入口有無
    i_GAS[8] = 0;                                   // E2ｶﾞｽｹｯﾄ有無
    i_SI[8] = 0;                                    // E4ｼﾘｶｹﾞﾙ有無

    if (s_Nzl_Syurui == "1" || s_Nzl_Syurui == "2"){

        i_INOUT[8] = 1;   // E4出入口有無
        Search_HD_TEMPORARY(s_Nzl_Tugite, s_Nzl_Kokei,
            &s_DNO[8], &s_SPEC1[8], &s_SPEC2[8]);
        i_GAS[8] = Get_TMGASKET(s_Nzl_Tugite, "N4", i_FL);

    }


    //******************
    //***  乾燥剤位置
    //******************
    if (i_FL == 0) {
        // 台形流
        // Ｂ側
        if (i_INOUT[3] == 1 ) {
            //S3
            i_SI[3] = 1;
        } else if (i_INOUT[7] == 1) {
            //E3
            i_SI[7] = 1;
        } else if (i_INOUT[1] == 1) {
            //S1
            i_SI[1] = 1;
        } else if (i_INOUT[5] == 1) {
            //E1
            i_SI[5] = 1;
        }
        // Ａ側
        if (i_INOUT[4] == 1 ) {
            //S4
            i_SI[4] = 1;
        } else if (i_INOUT[8] == 1) {
            //E4
            i_SI[8] = 1;
        } else if (i_INOUT[2] == 1) {
            //S2
            i_SI[2] = 1;
        } else if (i_INOUT[6] == 1) {
            //E2
            i_SI[6] = 1;
        }
        // 2020.04.15 乾燥剤量追加_S
        s_AB[1] ="B";
        s_AB[2] ="A";
        s_AB[3] ="B";
        s_AB[4] ="A";
        s_AB[5] ="B";
        s_AB[6] ="A";
        s_AB[7] ="B";
        s_AB[8] ="A";
        // 2020.04.15 乾燥剤量追加_E
    } else {
        // 斜行流
        // Ｂ側
        if (i_INOUT[4] == 1 ) {
            //S4
            i_SI[4] = 1;
        } else if (i_INOUT[8] == 1) {
            //E4
            i_SI[8] = 1;
        } else if (i_INOUT[1] == 1) {
            //S1
            i_SI[1] = 1;
        } else if (i_INOUT[5] == 1) {
            //E1
            i_SI[5] = 1;
        }
        // Ａ側
        if (i_INOUT[3] == 1 ) {
            //S3
            i_SI[3] = 1;
        } else if (i_INOUT[7] == 1) {
            //E3
            i_SI[7] = 1;
        } else if (i_INOUT[2] == 1) {
            //S2
            i_SI[2] = 1;
        } else if (i_INOUT[6] == 1) {
            //E2
            i_SI[6] = 1;
        }
        // 2020.04.15 乾燥剤量追加_S
        s_AB[1] ="B";
        s_AB[2] ="A";
        s_AB[3] ="A";
        s_AB[4] ="B";
        s_AB[5] ="B";
        s_AB[6] ="A";
        s_AB[7] ="A";
        s_AB[8] ="B";
        // 2020.04.15 乾燥剤量追加_E
    }

    if (s_SILICAGEL == "0") {
        // ｼﾘｶｹﾞﾙ無
        i_SI[1] = 0;   // S1
        i_SI[2] = 0;   // S2
        i_SI[3] = 0;   // S3
        i_SI[4] = 0;   // S4
        i_SI[5] = 0;   // E1
        i_SI[6] = 0;   // E2
        i_SI[7] = 0;   // E3
        i_SI[8] = 0;   // E4
        // 2020.04.15 乾燥剤量追加_S
        s_Silicagel[1] = "  0.00";
        s_Silicagel[2] = "  0.00";
        s_Silicagel[3] = "  0.00";
        s_Silicagel[4] = "  0.00";
        s_Silicagel[5] = "  0.00";
        s_Silicagel[6] = "  0.00";
        s_Silicagel[7] = "  0.00";
        s_Silicagel[8] = "  0.00";
        // 2020.04.15 乾燥剤量追加_E
    }
    // 2020.04.15 乾燥剤量追加_S
    else {
        // ｼﾘｶｹﾞﾙ取得
        for ( i = 0; i < 8 ; i++ ) {

            if ( i_SI[i] == 0 ) {
                // ｼﾘｶｹﾞﾙ無
                s_Silicagel[i] = "  0.00";
            }
            else {
                // ｼﾘｶｹﾞﾙ有
                s_Silicagel[i] = Get_Silicagel(s_AB[i]);
            }

        }

    }
    // 2020.04.15 乾燥剤量追加_E

    //******************
    //***   部品集計
    //******************

    //------------------------------------------------
    // ■集計
    // ･[s_DNO]図番,[s_SPEC1]仕様1,[s_SPEC2]仕様2,
    //  [i_INOUT]出入口有無,[i_SI]ｼﾘｶｹﾞﾙ有無が
    //  全て一致するﾃﾞｰﾀは1つにまとめる
    //------------------------------------------------
    i_WkRec = 0;
    for ( i = 0; i < 8 ; i++ ) {

        s_WK_DNO[i] = "";
        s_WK_SPEC1[i] = "";
        s_WK_SPEC2[i] = "";
        i_WK_FL[i] = 0;
        i_WK_SI[i] = 0;
        i_WK_GAS[i] = 0;
        // 2020.04.15 乾燥剤量追加_S
        s_WK_Silicagel[1] = "  0.00";
        // 2020.04.15 乾燥剤量追加_E

    }

    for ( i = 1; i < 9 ; i++ ) {

        if ( i_INOUT[i] == 1 ) {
            //*****************************************
            // 既に格納されている部品ﾃﾞｰﾀと比較
            //*****************************************
            bMatch = false;
            for ( j = 0; j < i_WkRec ; j++ ) {

                // 2020.04.15 乾燥剤量追加_S
                //if( ( s_DNO[i] == s_WK_DNO[j] ) &&
                //    ( s_SPEC1[i] == s_WK_SPEC1[j] ) &&
                //    ( s_SPEC2[i] == s_WK_SPEC2[j] ) &&
                //    ( i_SI[i] == i_WK_SI[j] ) ) {
                //
                //    // 数量集計
                //    i_WK_FL[j] = i_WK_FL[j] + i_INOUT[i];
                //    i_WK_GAS[j] = i_WK_GAS[j] + i_GAS[i];
                //
                //    bMatch = true;
                //
                //}
                if( ( s_DNO[i] == s_WK_DNO[j] ) &&
                    ( s_SPEC1[i] == s_WK_SPEC1[j] ) &&
                    ( s_SPEC2[i] == s_WK_SPEC2[j] ) &&
                    ( i_SI[i] == i_WK_SI[j] ) &&
                    ( s_Silicagel[i] == s_WK_Silicagel[j] ) ) {

                    // 数量集計
                    i_WK_FL[j] = i_WK_FL[j] + i_INOUT[i];
                    i_WK_GAS[j] = i_WK_GAS[j] + i_GAS[i];

                    bMatch = true;

                }
                // 2020.04.15 乾燥剤量追加_E

            }
            //*****************************************
            // 該当検索結果なしの場合ﾜｰｸ部品ﾃﾞｰﾀに格納
            //*****************************************
            if ( bMatch ) {
            } else {
                s_WK_DNO[i_WkRec] = s_DNO[i];
                s_WK_SPEC1[i_WkRec] = s_SPEC1[i];
                s_WK_SPEC2[i_WkRec] = s_SPEC2[i];
                i_WK_FL[i_WkRec] = i_INOUT[i];
                i_WK_SI[i_WkRec] = i_SI[i];
                i_WK_GAS[i_WkRec] = i_GAS[i];
                // 2020.05.08 乾燥剤量追加_S
                s_WK_Silicagel[i_WkRec] = s_Silicagel[i];
                // 2020.05.08 乾燥剤量追加_E

                i_WkRec++;
            }

        }

    }

    //**********************
    //***  出荷用閉止ﾌﾗﾝｼﾞ
    //**********************
    for ( i = 0; i < i_WkRec ; i++ ) {
        P_Sikibetu = "TFRG--";                         // 識別子
        P_Katasiki = "出荷用閉止フランジ";             // 部品型式
        P_Hosoku = "";                                 // 部品名称
        if ( s_TFLG == "1" ) {
            // SS400,t3.2
            P_Zaisitu = Search_HD_MATERIAL("1040");    // 材質名称
            P_EZaisitu = Search_HD_E_MATERIAL("1040"); // 材質名称(英)
            P_Siyou = "　ｔ３．２";                    // 仕様
        } else if ( s_TFLG == "2" ) {
            // SUS304,t3
            P_Zaisitu = Search_HD_MATERIAL("1001");    // 材質名称
            P_EZaisitu = Search_HD_E_MATERIAL("1001"); // 材質名称(英)
            P_Siyou = "　ｔ３　　";                    // 仕様
        }
        P_Zuban = s_WK_DNO[i];                         // 図番
        P_SZuban = s_WK_DNO[i];                        // 作図用図番
        P_Num =  i_WK_FL[i];                           // 1台分数量
        P_Yobisu = 0;                                  // 予備数
        P_Siyou = s_WK_SPEC1[i] + P_Siyou;             // 仕様1
        if ( i_WK_SI[i]  == 1 ) {
            // 乾燥剤付
            P_Siyou = P_Siyou + "　乾燥剤付";          // 仕様1
            // 2020.04.15 乾燥剤量追加_S
            P_Siyou = P_Siyou + s_WK_Silicagel[i].SubString(2,5); // 仕様1
            P_Siyou = P_Siyou +" kg";         // 仕様1
            // 2020.04.15 乾燥剤量追加_E
        }
        P_Siyou2 = "";                                 // 仕様2
        P_Unit = "";                                   // 仕様4
        P_Siyou5 = "";                                 // 仕様5
        s_code = "23";                                 // 閉止(ﾌﾞﾗｲﾝﾄﾞ)ﾌﾗﾝｼﾞ
        Write_Axsrs_Data(s_code);
    }

    //**********************
    //***  ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
    //**********************
    for ( i = 0; i < i_WkRec ; i++ ) {
        if (i_WK_GAS[i] == 0) {
        } else {
            if ( s_WK_SPEC2[i] == "FR" ) {
                // ﾘﾝｸﾞｶﾞｽｹｯﾄ
                P_Sikibetu = "STGKTR";                 // 識別子
                P_Katasiki = "リングガスケット";       // 部品型式
                s_code = "25";                         // 閉止ｼｰﾄｶﾞｽｹｯﾄ
            } else if ( s_WK_SPEC2[i] == "FF" ) {
                // 2020.12.16 ｶﾞｽｹｯﾄ名変更_S
                //// 全面座閉止ｶﾞｽｹｯﾄ
                //P_Sikibetu = "STGKTF";                 // 識別子
                //P_Katasiki = "全面座ガスケット";       // 部品型式
                // 全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ
                P_Sikibetu = "STGKTF";                 // 識別子
                P_Katasiki = "全面座フランジガスケット"; // 部品型式
                // 2020.12.16 ｶﾞｽｹｯﾄ名変更_S
                s_code = "26";                         // 全面座閉止ｶﾞｽｹｯﾄ
            } else {
                // その他
                P_Sikibetu = "STGKT-";                 // 識別子
                P_Katasiki = "";                       // 部品型式
                s_code = "";
            }
            P_Hosoku = "";                             // 部品名称
            P_Zaisitu = Search_HD_MATERIAL("2019");    // 材質名称
            P_Zuban = "";                              // 図番
            P_SZuban = "";                             // 作図用図番
            P_Num =  i_WK_GAS[i];                      // 1台分数量
            P_Yobisu = 0;                              // 予備数
            P_Siyou = s_WK_SPEC1[i];                   // 仕様1
            P_Siyou2 = "";                             // 仕様2
            P_Unit = "";                               // 仕様4
            P_Siyou5 = "";                             // 仕様5
            Write_Axsrs_Data(s_code);
        }

    }

    return true;
}


// ---------------------------------------------------------------------------
// 日本語関数名： 出荷用閉止フランジをマスタから取得
// 概  要      ：
// 引  数      ： I:Tugite      ノズル継手コード
//             ： I:Kokei       ノズル口径
//             ： O:s_DNO       閉止フランジ図番
//             ： O:s_SPEC1     閉止フランジ規格名称
//             ： O:s_SPEC2     ガスケット種類
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2014.12.22 追加
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Search_HD_TEMPORARY( AnsiString Tugite,
    AnsiString Kokei, AnsiString *s_DNO, AnsiString *s_SPEC1,
    AnsiString *s_SPEC2) {

    AnsiString s_Text1;
    AnsiString s_Text2;
    AnsiString s_Key;

    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString s_SQL;

    // *********************************************
    // ***　出荷用閉止ﾌﾗﾝｼﾞﾏｽﾀ(HD_TEMPORARY) 検索
    // *********************************************

    G_Log_Text = "出荷用閉止ﾌﾗﾝｼﾞﾏｽﾀ(HD_TEMPORARY)を検索キー  『" + Tugite + "," + Kokei + "』で検索。";
    Write_Log(G_Log_Text);

    s_Text1 = Tugite.SubString(1,2);
    s_Text2 = Tugite.SubString(4,1);
    s_Key= s_Text1 + "0" +s_Text2;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_TEMPORARY WHERE ";
    s_SQL = s_SQL + "  FCODE = '" + s_Key + "'";
    s_SQL = s_SQL + "    AND FLANGE_SIZE = '" + Kokei + "'";

    wkQuery->Close();
    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->RecordCount == 0) {
        G_ErrLog_Text = "ﾃﾞｰﾀ作成時    ｢" + Tugite + "," + Kokei +
            "｣ は、出荷用閉止ﾌﾗﾝｼﾞﾏｽﾀ(HD_TEMPORARY)に存在しません。";
        Write_Error_Log(G_ErrLog_Text);
    }

    while (!wkQuery->Eof) {
        // 閉止フランジ図番
        *s_DNO = wkQuery->FieldByName("DNO")->AsString.TrimRight();
        // 閉止フランジ規格名称
        *s_SPEC1 = wkQuery->FieldByName("SPEC1")->AsString.TrimRight();
        // ガスケット種類
        *s_SPEC2  = wkQuery->FieldByName("SPEC2")->AsString.TrimRight();
        // 予備品識別子

        // ログ
        G_Log_Text = "閉止ﾌﾗﾝｼﾞ図番   ｢" + *s_DNO + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "閉止ﾌﾗﾝｼﾞ規格名称   ｢" + *s_SPEC1 + "｣を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "ｶﾞｽｹｯﾄ種類   ｢" + *s_SPEC2 + "｣を取得。";
        Write_Log(G_Log_Text);

        wkQuery->Next();
    }

    delete wkQuery;
    return true;
}


// ---------------------------------------------------------------------------
// 日本語関数名： 出荷用閉止フランジガスケット有無
// 概  要      ：
// 引  数      ： I:Tugite      ノズル継手コード
//             ： I:Syurui      ノズル位置
//             ： I:Fl          流れ方向 [0:台形流 1:斜行流]
// 戻り値      ： ガスケット有無 [1:有 0:無]
// 備  考      ： 2014.12.22 追加
// ---------------------------------------------------------------------------
int __fastcall TAxsrsDetails::Get_TMGASKET(AnsiString Tugite,
    AnsiString Syurui, int Fl) {

    int i_Tan_mat;
    AnsiString s_Boots;
       AnsiString s_Text;


    // 端菅材質
    if (Syurui=="N1") {
        i_Tan_mat = P_HEADS_DATA[1136].ToIntDef(0);
    } else if (Syurui=="N2") {
        i_Tan_mat = P_HEADS_DATA[1137].ToIntDef(0);
    } else if (Syurui=="N3") {
        if (Fl==0) {
            // 台形流
            i_Tan_mat = P_HEADS_DATA[1136].ToIntDef(0);
        } else {
            // 斜行流
            i_Tan_mat = P_HEADS_DATA[1137].ToIntDef(0);
        }
    } else if (Syurui=="N4") {
        if (Fl==0) {
             // 台形流
            i_Tan_mat = P_HEADS_DATA[1137].ToIntDef(0);
        } else {
            // 斜行流
            i_Tan_mat = P_HEADS_DATA[1136].ToIntDef(0);
        }
    }

    // ブーツ種類
    s_Boots = Tugite.SubString(3,1);

    if (s_Boots != "1") {
        // ラバーブーツ以外
        return(1);
    } else if (i_Tan_mat != 0) {
        // 端管が付く
        return(1);
    } else if (P_HEADS_DATA[1068].TrimRight() != "0000") {
        // 相フランジが付く
        return(1);
    } else {
        // ガスケット不要
        return(0);
    }

}


//---------------------------------------------------------------------------
// 日本語関数名： シートガスケット材質取得
// 概  要      ：
// 引  数      ： s_Side：流体種類(A:Ａ側,B:Ｂ側)
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2017.10.10追加
//---------------------------------------------------------------------------
AnsiString __fastcall TAxsrsDetails::Get_SheetGasketMat(AnsiString s_Side)
{
   int i_Flow;                // 流体種類
   int i_RecNo;
   double d_Bdt;              // Ｂ側設計温度
   double d_Adt;              // Ａ側設計温度

   AnsiString s_Katasiki;     // 型式
   AnsiString s_Syurui;       // 種類
   AnsiString s_Flow1;        // Ｂスチーム区分(CASE1)
   AnsiString s_Flow2;        // Ａスチーム区分(CASE1)
   AnsiString s_Flow3;        // Ｂスチーム区分(CASE2)
   AnsiString s_Flow4;        // Ａスチーム区分(CASE2)
   AnsiString s_GasMat;       // シートガスケット材質
   AnsiString s_Text;

   //***************************
   //***  型       式
   //***************************
   i_RecNo = 33;
   s_Katasiki = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  種       類
   //***************************
   i_RecNo = 34;
   s_Syurui = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Ｂスチーム区分(CASE1)
   //***************************
   i_RecNo = 846;
   s_Flow1 = P_HEADS_DATA[i_RecNo].TrimRight();

    //***************************
   //***  Ａスチーム区分(CASE1)
   //***************************
   i_RecNo = 847;
   s_Flow2 = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Ｂスチーム区分(CASE2)
   //***************************
   i_RecNo = 869;
   s_Flow3 = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Ａスチーム区分(CASE2)
   //***************************
   i_RecNo = 870;
   s_Flow4 = P_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Ｂ側設計温度
   //***************************
   i_RecNo = 829;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   d_Bdt = s_Text.ToDouble();

   //***************************
   //***  Ａ側設計温度
   //***************************
   i_RecNo = 834;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   d_Adt = s_Text.ToDouble();

   //***************************
   //***  流 体 種 類
   //***************************
   if (s_Katasiki.SubString(3,1) == "C") {
      //コンデンサ　
      i_Flow = 3;
   } else if (s_Katasiki.SubString(4,1) == "C") {
      //コンデンサ　
      i_Flow = 3;
   } else if (s_Syurui.SubString(1,2) == "YX") {
      //コンデンサ　
      i_Flow = 3;
   } else {
      // 液体(蒸気)
      i_Flow = 0;
   }

   if (i_Flow != 3) {
      if(s_Side == "B") {
         // Ｂ側
         if(s_Flow1 == "1" || s_Flow3 == "1") {
            if (d_Bdt >= 150) {
               // 蒸気２
               i_Flow = 2;
            } else {
               // 蒸気１
               i_Flow = 1;
            }
         }
      } else if(s_Side == "A") {
         // Ａ側
         if(s_Flow2 == "1" || s_Flow4 == "1") {
            if (d_Adt >= 150) {
               // 蒸気２
               i_Flow = 2;
            } else {
               // 蒸気１
               i_Flow = 1;
            }
         }
      }
   }

   if (i_Flow == 0) {
      G_Log_Text = "流体種類『液体』";
   } else if (i_Flow == 1) {
      G_Log_Text = "流体種類『蒸気１』";
   } else if (i_Flow == 2) {
      G_Log_Text = "流体種類『蒸気２』";
   } else if (i_Flow == 3) {
      G_Log_Text = "流体種類『コンデンサ』";
   } else {
      G_Log_Text = "流体種類『エラー』";
   }
   Write_Log(G_Log_Text);

   //***************************
   //***  材 質 選 定
   //***************************
   s_GasMat="";
   s_Text="";
   if (i_Flow == 1) {
      // 蒸気１（ＥＰＤＭ）
      s_GasMat ="2026";
   } else if (i_Flow == 2) {
      // 蒸気２（バルカーＮｏ．ＧＦ３００）
      s_GasMat ="1188";
   } else {
      if(s_Side=="B") {
         //Ｂ側プレートガスケット
         i_RecNo = 835;
         s_GasMat = P_HEADS_DATA[i_RecNo].TrimRight();
      } else if (s_Side=="A") {
         //Ａ側プレートガスケット
         i_RecNo = 836;
         s_GasMat = P_HEADS_DATA[i_RecNo].TrimRight();
      }
   }

   G_Log_Text = "シートガスケット材質コード『" + s_GasMat + "』";
   Write_Log(G_Log_Text);

   return s_GasMat;

}


// ---------------------------------------------------------------------------
// 日本語関数名： シートガスケット名称取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2020.12.16 追加
// ---------------------------------------------------------------------------
bool __fastcall TAxsrsDetails::Get_SeatGas_Name() {
    int i_FL;                // 斜行流ﾌﾗｸﾞ
    double d_BKou;      // Ｂシートガスケット口径
    double d_AKou;      // Ａシートガスケット口径
    AnsiString s_BKou;  // Ｂシートガスケット口径
    AnsiString s_AKou;  // Ａシートガスケット口径
    AnsiString s_BKika; // Ｂシートガスケットノズル規格
    AnsiString s_AKika; // Ａシートガスケットノズル規格


    //******************
    //***  斜行流判断
    //******************
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
        i_FL = 1; // 斜行流
    } else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        i_FL = 1; // 斜行流
    } else {
        i_FL = 0; // 台形流
    }


    //******************　
    //***  Ｂ側口径
    //******************
    s_BKou = "0";     // Ｂシートガスケット口径
    s_BKika = "0000"; // Ｂシートガスケットノズル規格

    // S1
    if (P_HEADS_DATA[891].TrimRight() == "1" ||
           P_HEADS_DATA[891].TrimRight() == "2") {
        s_BKou = P_HEADS_DATA[892].TrimRight();
        s_BKika = P_HEADS_DATA[893].TrimRight();
    }
    // S3
    else if (P_HEADS_DATA[921].TrimRight() == "1" ||
             P_HEADS_DATA[921].TrimRight() == "2") {
        if (i_FL == 0) { // 台形流
            s_BKou = P_HEADS_DATA[922].TrimRight();
            s_BKika = P_HEADS_DATA[923].TrimRight();
        }
    }
    // S4
    else if (P_HEADS_DATA[936].TrimRight() == "1" ||
             P_HEADS_DATA[936].TrimRight() == "2") {
        if (i_FL != 0) { // 斜行流
            s_BKou = P_HEADS_DATA[937].TrimRight();
            s_BKika = P_HEADS_DATA[938].TrimRight();
        }
    }
    // E1
    else if (P_HEADS_DATA[951].TrimRight() == "1" ||
             P_HEADS_DATA[951].TrimRight() == "2") {
        s_BKou = P_HEADS_DATA[952].TrimRight();
        s_BKika = P_HEADS_DATA[953].TrimRight();
    }
    // E3
    else if (P_HEADS_DATA[981].TrimRight() == "1" ||
             P_HEADS_DATA[981].TrimRight() == "2") {
        if (i_FL == 0) { // 台形流
            s_BKou = P_HEADS_DATA[982].TrimRight();
            s_BKika = P_HEADS_DATA[983].TrimRight();
        }
    }
    // E4
    else if (P_HEADS_DATA[996].TrimRight() == "1" ||
             P_HEADS_DATA[996].TrimRight() == "2") {
        if (i_FL == 0) { // 斜行流
            s_BKou = P_HEADS_DATA[997].TrimRight();
            s_BKika = P_HEADS_DATA[998].TrimRight();
        }
    }
    d_BKou = s_BKou.ToDouble(); // Ｂシートガスケット口径


    //******************　
    //***  Ａ側口径
    //******************
    s_AKou = "0";     // Ａシートガスケット口径
    s_AKika = "0000"; // Ａシートガスケットノズル規格

    // Ａ側口径
    // S2
    if (P_HEADS_DATA[906].TrimRight() == "1" ||
        P_HEADS_DATA[906].TrimRight() == "2") {
        s_AKou = P_HEADS_DATA[907].TrimRight();
        s_AKika = P_HEADS_DATA[908].TrimRight();
    }
    // S3
    else if (P_HEADS_DATA[921].TrimRight() == "1" ||
             P_HEADS_DATA[921].TrimRight() == "2") {
        if (i_FL != 0) { // 斜行流
            s_AKou = P_HEADS_DATA[922].TrimRight();
            s_AKika = P_HEADS_DATA[923].TrimRight();
        }
    }
    // S4
    else if (P_HEADS_DATA[936].TrimRight() == "1" ||
             P_HEADS_DATA[936].TrimRight() == "2") {
        if (i_FL == 0) { // 台形流
            s_AKou = P_HEADS_DATA[937].TrimRight();
            s_AKika = P_HEADS_DATA[938].TrimRight();
        }
    }
    // E2
    else if (P_HEADS_DATA[966].TrimRight() == "1" ||
             P_HEADS_DATA[966].TrimRight() == "2") {
        s_AKou = P_HEADS_DATA[967].TrimRight();
        s_AKika = P_HEADS_DATA[968].TrimRight();
    }
    // E3
    else if (P_HEADS_DATA[981].TrimRight() == "1" ||
             P_HEADS_DATA[981].TrimRight() == "2") {
        if (i_FL != 0) { // 斜行流
            s_AKou = P_HEADS_DATA[982].TrimRight();
            s_AKika = P_HEADS_DATA[983].TrimRight();
        }
    }
    // E4
    else if (P_HEADS_DATA[996].TrimRight() == "1" ||
             P_HEADS_DATA[996].TrimRight() == "2") {
        if (i_FL == 0) { // 台形流
            s_AKou = P_HEADS_DATA[997].TrimRight();
            s_AKika = P_HEADS_DATA[998].TrimRight();
        }
    }
    d_AKou = s_AKou.ToDouble(); // Ａシートガスケット口径


    //******************　
    //***  Ｂ側名称
    //******************
    if ( d_BKou >= 250 ) {
        P_GasNameB = "全面座フランジガスケット";
    } else if (s_BKika.SubString(1,2) == "04" ||
               s_BKika.SubString(1,2) == "05" ||
               s_BKika.SubString(1,2) == "44" ||
               s_BKika.SubString(1,2) == "08" ||
               s_BKika.SubString(1,2) == "09" ||
               s_BKika.SubString(1,2) == "48") {
        if ( d_BKou >= 10 ) {
            P_GasNameB = "全面座フランジガスケット";
        } else {
            P_GasNameB = "リングガスケット";
        }
    } else {
        P_GasNameB = "リングガスケット";
    }


    //******************　
    //***  Ａ側名称
    //******************
    if ( d_AKou >= 250 ) {
        P_GasNameA = "全面座フランジガスケット";
    } else if (s_AKika.SubString(1,2) == "04" ||
               s_AKika.SubString(1,2) == "05" ||
               s_AKika.SubString(1,2) == "44" ||
               s_AKika.SubString(1,2) == "08" ||
               s_AKika.SubString(1,2) == "09" ||
               s_AKika.SubString(1,2) == "48") {
        if ( d_AKou >= 10 ) {
            P_GasNameA = "全面座フランジガスケット";
        } else {
            P_GasNameA = "リングガスケット";
        }
    } else {
        P_GasNameA = "リングガスケット";
    }


    G_Log_Text = "Ａ側シートガスケット名称『" + P_GasNameA + "』";
    Write_Log(G_Log_Text);


    G_Log_Text = "s_BKou『" + s_BKou + "』";
    Write_Log(G_Log_Text);
    G_Log_Text = "s_BKika『" + s_BKika + "』";
    Write_Log(G_Log_Text);
    G_Log_Text = "s_AKou『" + s_AKou + "』";
    Write_Log(G_Log_Text);
    G_Log_Text = "s_AKika『" + s_AKika + "』";
    Write_Log(G_Log_Text);


    G_Log_Text = "Ｂ側シートガスケット名称『" + P_GasNameB + "』";
    Write_Log(G_Log_Text);

    return true;
}

