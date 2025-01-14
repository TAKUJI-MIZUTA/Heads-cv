// ----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：フレーム部品展開処理  モジュール
// 作 成 者：N.Uchida
// 作 成 日：2002.06.21
// 更 新 日：2002.10.08
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

#include "Bas_FrameDetails.h"

#include "HeadsData.h"

// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// フレーム部品展開処理 Class
// ---------------------------------------------------------------------------
class TFrameDetails {
private:
    FILE *fp;

    // 汎用
    AnsiString P_SQL; // SQL文用
    AnsiString P_Key; // 検索用ｷｰ

    // CSVﾌｧｲﾙ作成用
    AnsiString P_BBunrui; // 部品分類
    AnsiString P_Sikibetu; // 識別子
    AnsiString P_BKatasiki; // 部品型式
    AnsiString P_BMeisyou; // 部品名称
    AnsiString P_ZaisituNM; // 材質名称
    AnsiString P_EZaisituNM; // 材質名称
    AnsiString P_ZaisituCD; // 材質コード
    AnsiString P_Zuban; // 図番
    AnsiString P_SZuban; // 作図用図番
    int P_1DaiSuryo; // １台分数量
    int P_Yobisu; // 予備数
    int P_Soukei; // 総計
    int P_RecNo; // ﾚｺｰﾄﾞｶｳﾝﾀ

    AnsiString P_Siyou1; // 備考１
    AnsiString P_Siyou2; // 備考２
    AnsiString P_Siyou3; // 備考３
    AnsiString P_Siyou4; // 備考４
    AnsiString P_Siyou5; // 備考５

    // ﾌﾚｰﾑ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ(FRAMECD.CSV)用
    AnsiString P_FRAMECD[5];

    // ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)用
    AnsiString P_DNO[6]; // 図番
    AnsiString P_WKDNO[6]; // 作図用図番
    AnsiString P_MatNM[6]; // 材質名称
    AnsiString P_MatCD[6]; // 材質コード
    int P_PLATE_NUM[6]; // 枚数
    int P_PLATE_NUM_SUS[6]; // 枚数(SUS用)
    int P_SUS; // SUS使用フラグ

    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];

    bool __fastcall S_Frame_Hontai_Sakusei(void); // Sﾌﾚｰﾑﾃﾞｰﾀ　   作成
    bool __fastcall E_Frame_Hontai_Sakusei(void); // Eﾌﾚｰﾑﾃﾞｰﾀ　   作成
    // 2022.05.23 フレームガス切り追加_S
    bool __fastcall S_FrameG_Hontai_Sakusei(void); // Sﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ　   作成
    bool __fastcall E_FrameG_Hontai_Sakusei(void); // Eﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ　   作成
    // 2022.05.23 フレームガス切り追加_E
    bool __fastcall Frame_Buhin_Sakusei(void); // ﾌﾚｰﾑ部品ﾃﾞｰﾀ　作成

    bool __fastcall Search_HD_FRM_NZ(AnsiString Key, AnsiString Kbn);
    // ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ　検索
    bool __fastcall Search_HD_FRM_NZ_Buhin(AnsiString Key);
    // ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ　検索

    bool __fastcall Write_SE_Frame_Data(AnsiString Kbn, bool Seek_Result);
    // S,Eﾌﾚｰﾑﾃﾞｰﾀ　書き込み
    bool __fastcall Write_FrameBuhin_Data(bool Seek_Result); // ﾌﾚｰﾑ部品ﾃﾞｰﾀ 書き込み
    bool __fastcall Syukei_Frame_Yobisu(AnsiString a_Sekbn); // ﾌﾚｰﾑ予備数集計

    AnsiString __fastcall Get_AnaakeSiyou(AnsiString Kbn); // 孔明仕様 取得
    AnsiString __fastcall Get_BuhinKatasiki(AnsiString Code); // 部品型式　取得
    void __fastcall BLT_Cover_Out(void);
    // 2014/08/22 応用技術編集
    void __fastcall BLT_Cover_Out_Small(void);
    // 2022.01.14 スペーサ追加_S
    AnsiString __fastcall Get_AnaakeSiyou_S(AnsiString Kbn); // 孔明仕様 取得
    // 2022.01.14 スペーサ追加_E

public:
    bool __fastcall Excute(HeadsData* pcHeadsData); // ﾌﾚｰﾑ部品展開処理 実行
    int __fastcall Get_Blt_YobiSu(double d_BltSz, double d_BltLen,
        AnsiString MatCD, HeadsData* pcHeadsData);

};

// ---------------------------------------------------------------------------
// 日本語関数名： フレーム部品 展開処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall FrameDetails(HeadsData* pcHeadsData) {
    bool ret;

    TFrameDetails flameDetails; // ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ，孔明ﾃﾞｰﾀ 作成Class自動生成,関数終了後破棄

    ret = flameDetails.Excute(pcHeadsData); // ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ，孔明ﾃﾞｰﾀ 作成実行

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾚｰﾑ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    m_pcHeadsData = pcHeadsData;
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********      『ﾌﾚｰﾑ部品展開処理』開始      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********      『ﾌﾚｰﾑ部品展開処理』開始      *********");
    Write_Error_Log("*******************************************************");

    // 2022.05.23 フレームガス切り追加_S
    // ***********************************
    // ***  Sﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   『Sﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ』作成開始   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    S_FrameG_Hontai_Sakusei();

    G_Log_Text = "----------   『Sﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ』作成終了   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    // 2022.05.23 フレームガス切り追加_E

    // ***********************************
    // ***  Sﾌﾚｰﾑﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------     『Sﾌﾚｰﾑﾃﾞｰﾀ』作成開始      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    S_Frame_Hontai_Sakusei();

    G_Log_Text = "----------     『Sﾌﾚｰﾑﾃﾞｰﾀ』作成終了      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // 2022.05.23 フレームガス切り追加_S
    // ***********************************
    // ***  Eﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------   『Eﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ』作成開始   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    E_FrameG_Hontai_Sakusei();

    G_Log_Text = "----------   『Eﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ』作成終了   ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);
    // 2022.05.23 フレームガス切り追加_E

    // ***********************************
    // ***  Eﾌﾚｰﾑﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------     『Eﾌﾚｰﾑﾃﾞｰﾀ』作成開始      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    E_Frame_Hontai_Sakusei();

    G_Log_Text = "----------     『Eﾌﾚｰﾑﾃﾞｰﾀ』作成終了      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);


    // ***********************************
    // ***  ﾌﾚｰﾑ部品ﾃﾞｰﾀ　作成
    // ***********************************
    G_Log_Text = "----------     『ﾌﾚｰﾑ部品ﾃﾞｰﾀ』作成開始      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    Frame_Buhin_Sakusei();

    G_Log_Text = "----------     『ﾌﾚｰﾑ部品ﾃﾞｰﾀ』作成終了      ----------";
    Write_Log(G_Log_Text);
    Write_Error_Log(G_Log_Text);

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********      『ﾌﾚｰﾑ部品展開処理』終了      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********      『ﾌﾚｰﾑ部品展開処理』終了      *********");
    Write_Error_Log("*******************************************************");
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾚｰﾑ部品ﾃﾞｰﾀ作成処理 実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Frame_Buhin_Sakusei(void) {
    FILE *wkfp;

    bool seek_ret;
    AnsiString s_FilePass;
    AnsiString s_Katasiki;
    int i_FrameCode;

    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    char token[100][100]; /* トークンバッファ */

    // 2017.06.23 LX-90D-NUP追加
    AnsiString s_Text;
    // *************************

    // ***************************
    // **  ﾌﾚｰﾑ型式変換ﾃｰﾌﾞﾙ
    // ***************************
    s_FilePass = G_Input_Csv_Dir + "FRAMECD.csv";

    if ((wkfp = fopen(s_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "『" + s_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
        fclose(fp); // 閉じる
        return false;
    }

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

        // CSV読込み結果　セット
        P_FRAMECD[1] = token[0]; // FRAME.CSV：ｺｰﾄﾞ
        P_FRAMECD[2] = token[1]; // FRAME.CSV：部品分類ｺｰﾄﾞ
        P_FRAMECD[3] = token[2]; // FRAME.CSV：部品区分
        P_FRAMECD[4] = token[3]; // FRAME.CSV：部品名称

        s_Katasiki = token[4]; // 予備品数取得用

        // 2017.06.23 LX-90D-NUP追加
        // 2014/08/22 応用技術編集
        // コードが"75"(BNWRN-,締付BNW) かつ Zフレームでない場合、出力対象外
        //if(P_FRAMECD[1] == "75" &&
        //   P_HEADS_DATA[36].Pos("Z") == 0) {
        //
        //   continue;
        //
        //}
        s_Text = G_KS_Syurui + "−" + G_KS_Model;
        if( P_FRAMECD[1] == "75" ) {
            // 締付ﾎﾞﾙﾄ(細)
            if (( s_Text == "ＬＸ−９０Ｄ") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
                // LX-90D-KNUP  出力対象
            } else if (( s_Text == "ＬＸ−９０Ｄ") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
                // LX-90D-TNUP  出力対象
            } else if( P_HEADS_DATA[36].Pos("Z") == 0) {
                // Zﾌﾚｰﾑ  出力対象外
                continue;
            }
        }
        // *************************

        i_FrameCode = P_FRAMECD[1].ToIntDef(1);

        // 2022.05.23 フレームガス切り追加_S
        //if (i_FrameCode != 1 && i_FrameCode != 8) {
        if (i_FrameCode != 1 && i_FrameCode != 8 && i_FrameCode != 81 && i_FrameCode != 88) {
        // 2022.05.23 フレームガス切り追加_E

            // ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)検索ｷｰ
            P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
                P_HEADS_DATA[1267].TrimRight();

            // *********************************************
            // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)検索
            // *********************************************
            seek_ret = Search_HD_FRM_NZ_Buhin(P_Key);

            // **********************
            // ***  予備品ﾃﾞｰﾀ　取得
            // **********************
            P_Yobisu = 0;

            // 予備数の取得
            P_Yobisu = Syukei_Buhin_Yobisu(s_Katasiki, P_ZaisituCD,
                m_pcHeadsData);

            // ***************************
            // ***  ﾌﾚｰﾑ部品ﾃﾞｰﾀ　書込
            // ***************************
            Write_FrameBuhin_Data(seek_ret);

        }

    }

    fclose(wkfp); // 閉じる
    // delete wkfp;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 予備品ﾃﾞｰﾀ　集計処理
// 概  要      ： 予備品ﾃﾞｰﾀから予備品数を求める
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 02.08.03追加
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Syukei_Frame_Yobisu(AnsiString a_Sekbn) {
    // CSV読込用
    FILE *wkfp;

    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    char token[100][100]; /* トークンバッファ */

    AnsiString s_Yobi_Text[5];
    AnsiString a_Kbn;
    AnsiString wk_FilePass; // "D00001.csv"

    P_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)

    // -----------------------------------------------
    // 2007.10.03 追加
    // ﾐﾗｰB仕様の場合は、予備品ﾃﾞｰﾀなしとして処理
    if (m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B) {
        P_Yobisu = 0;
        return true;
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

            // 1台分数量の集計

            if (a_Sekbn == "S")
                a_Kbn = "2";
            else if (a_Sekbn == "E")
                a_Kbn = "3";

            if (s_Yobi_Text[0].Trim() == "FRME" && s_Yobi_Text[1].Trim()
                == a_Kbn) {
                P_Yobisu = s_Yobi_Text[2].ToIntDef(0);
                P_RecNo++;
            }
        }
    }

    // 該当予備品ﾃﾞｰﾀが存在しなかった場合
    if (P_RecNo == 0)
        P_Yobisu = 0;

    // 予備品ﾃﾞｰﾀ ﾌｧｲﾙｸﾛｰｽﾞ
    fclose(wkfp);
    return true;
}

// ---------------------------------------------------------------------------------------
// 日本語関数名： ﾌﾚｰﾑ部品ﾃﾞｰﾀ　書き込み
// 概  要      ：
// 引  数      ： Seek_Result:ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ検索結果(true:存在,false:存在しない)
// 戻り値      ： 処理結果 [true:ﾃﾞｰﾀ有り false:ﾃﾞｰﾀ無し]
// 備  考      ：
// ---------------------------------------------------------------------------------------
bool __fastcall TFrameDetails::Write_FrameBuhin_Data(bool Seek_Result) {
    AnsiString s_Text;
    // 2009.02.25 WX-50,CX-10Dベースプレート追加
    AnsiString s_Text2;
    // *****************************************
    // 2012.01.20 中国材質
    AnsiString s_ChinaM;
    int i_RecNo;
    // *******************
    AnsiString wk_FilePass;
    AnsiString sPltMat;
    AnsiString sS1NoMat;
    AnsiString sS2NoMat;
    AnsiString sE1NoMat;
    AnsiString sBltMat;
    // 2022.01.14 スペーサ追加_S
    AnsiString sAGasMat;
    AnsiString sBGasMat;
    // 2022.01.14 スペーサ追加_E
    bool bSUS;
    bool bS1SUS;
    bool bS2SUS;

    long l_Daisu;

    // ------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // ****************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    // ------------------------------------------

    // ****************************
    // ***  変数セット
    // ****************************
    // 部品分類
    P_BBunrui = "FRAME";
    // 識別子
    P_Sikibetu = P_FRAMECD[2];

    // 材質名称初期化
    P_ZaisituNM = "";
    P_EZaisituNM = "";

    // 2014/08/22 応用技術編集
    // 部品型式
    if (Seek_Result == true && P_FRAMECD[1] != "75") {
        P_BKatasiki = Get_BuhinKatasiki(P_FRAMECD[1]);
    }
    else {
        P_BKatasiki = "";
    }

    // 部品名称
    s_Text = P_FRAMECD[1];
    // 2014/08/22 応用技術編集
    if (s_Text == "18" || s_Text == "23" || s_Text == "24" || s_Text == "25" ||
        s_Text == "26" || s_Text == "27" || s_Text == "28" || s_Text == "75") {
        P_BMeisyou = "";
    }
    else {
        P_BMeisyou = P_FRAMECD[4];
    }

    // 図番，作図用図番
    // 2003.11.05 枚数区分関係式＜を＜＝に変更
    if (Seek_Result == true) {
        // 2006.01.10 枚数区分による図番の変更に下部ｶﾞｲﾄﾞﾊﾞｰ(15)を追加
        // if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" || P_FRAMECD[1] == "16"){
        if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" ||
            P_FRAMECD[1] == "15" || P_FRAMECD[1] == "16") {
            // ***********************************************************
            bSUS = false;
            // 2003.11.06　プレート材質データ位置変更
            // sPltMat = P_HEADS_DATA[160].TrimRight();
            sPltMat = P_HEADS_DATA[43].TrimRight();
            // **************************************
            // 2003.12.05 ASME PLATE
            // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
            // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
            //    (sPltMat != "1032") && (sPltMat != "1033") &&
            //    (sPltMat != "1057") && (sPltMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
            //    (sPltMat != "1075") && (sPltMat != "1089") &&
            //    (sPltMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11追加
            //    (sPltMat != "1076") &&
            //    // ***************************
            //    (sPltMat != "1085") && (sPltMat != "1095")) {
            //    // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
            //    // *********************
            //    bSUS = true;
            //}
            // 2019.05.09 t0.8ﾌﾟﾚｰﾄ判断追加_S
            //if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
            //    // プレート材質がチタン系以外
            //    bSUS = true;
            //}
            if ((P_SUS == 1) && (Type_MAT(sPltMat) != "Ti")) {
                // プレート材質がチタン系以外
                bSUS = true;
            }
            if ((P_SUS == 2) && (P_HEADS_DATA[1013].TrimRight() == "08")) {
                // プレート板厚がｔ０．８
                bSUS = true;
            }
            // 2019.05.09 t0.8ﾌﾟﾚｰﾄ判断追加_E
            // ***********************
            // 2022.04.08 UX-30L追加_S
            if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 1 )) {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            else if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 2 )) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
                if (P_MatNM[2] != "") {
                    P_ZaisituNM = P_MatNM[2];
                    P_EZaisituNM = P_MatNM[2];
                }
                else if (P_MatCD[2] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                   P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                   P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                    P_Siyou3 = "2";
            }
            else if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 3)) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
                if (P_MatNM[3] != "") {
                    P_ZaisituNM = P_MatNM[3];
                    P_EZaisituNM = P_MatNM[3];
                }
                else if (P_MatCD[3] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "3";
            }
            else if ((P_SUS == 3) &&  (P_HEADS_DATA[1381].ToIntDef(0) == 4)) {
                P_Zuban = P_DNO[4];
                P_SZuban = P_WKDNO[4];
                if (P_MatNM[4] != "") {
                    P_ZaisituNM = P_MatNM[4];
                    P_EZaisituNM = P_MatNM[4];
                }
                else if (P_MatCD[4] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "4";
            }
            else if ((P_SUS == 3) && (P_HEADS_DATA[1381].ToIntDef(0) == 5)) {
                P_Zuban = P_DNO[5];
                P_SZuban = P_WKDNO[5];
                if (P_MatNM[5] != "") {
                    P_ZaisituNM = P_MatNM[5];
                    P_EZaisituNM = P_MatNM[5];
                }
                else if (P_MatCD[5] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "5";
            }
            //if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[1])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[1]))) {
            // 2022.04.08 UX-30L追加_E
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[2])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[2]))) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
                if (P_MatNM[2] != "") {
                    P_ZaisituNM = P_MatNM[2];
                    P_EZaisituNM = P_MatNM[2];
                }
                else if (P_MatCD[2] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "2";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[3])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[3]))) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
                if (P_MatNM[3] != "") {
                    P_ZaisituNM = P_MatNM[3];
                    P_EZaisituNM = P_MatNM[3];
                }
                else if (P_MatCD[3] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "3";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[4])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[4]))) {
                P_Zuban = P_DNO[4];
                P_SZuban = P_WKDNO[4];
                if (P_MatNM[4] != "") {
                    P_ZaisituNM = P_MatNM[4];
                    P_EZaisituNM = P_MatNM[4];
                }
                else if (P_MatCD[4] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "4";
            }
            else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM[5])) ||
                ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
                P_PLATE_NUM_SUS[5]))) {
                P_Zuban = P_DNO[5];
                P_SZuban = P_WKDNO[5];
                if (P_MatNM[5] != "") {
                    P_ZaisituNM = P_MatNM[5];
                    P_EZaisituNM = P_MatNM[5];
                }
                else if (P_MatCD[5] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "5";
            }
            else {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            // 2003.12.12 ＹＸ−８０フレーム当板追加
        }
        else if (P_FRAMECD[1] == "06") { // Ｓフレーム当板
            bS1SUS = false;
            bS2SUS = false;
            sS1NoMat = P_HEADS_DATA[894].TrimRight();
            sS2NoMat = P_HEADS_DATA[909].TrimRight();
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if ((sS1NoMat != "1014") && (sS1NoMat != "1015") &&
            //    (sS1NoMat != "1032") && (sS1NoMat != "1033") &&
            //    (sS1NoMat != "1057") && (sS1NoMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
            //    (sS1NoMat != "1075") && (sS1NoMat != "1089") &&
            //    (sS1NoMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11追加
            //    (sS1NoMat != "1076") &&
            //    // ***************************
            //    (sS1NoMat != "1085") && (sS1NoMat != "1095")) {
            //    // Ｓ１ノズル材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
            //    bS1SUS = true;
            //}
            //if ((sS2NoMat != "1014") && (sS2NoMat != "1015") &&
            //    (sS2NoMat != "1032") && (sS2NoMat != "1033") &&
            //    (sS2NoMat != "1057") && (sS2NoMat != "1058") &&
            //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
            //    (sS2NoMat != "1075") && (sS2NoMat != "1089") &&
            //    (sS2NoMat != "1090") &&
            //    // *************************************************
            //    // 2010.12.02 SB-265-Gr.11追加
            //    (sS2NoMat != "1076") &&
            //    // ***************************
            //    (sS2NoMat != "1085") && (sS2NoMat != "1095")) {
            //    // Ｓ１ノズル材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
            //    bS2SUS = true;
            //}
            if (Type_MAT(sS1NoMat) != "Ti") {
                // Ｓ１ノズル材質がチタン系以外
                bS1SUS = true;
            }
            if (Type_MAT(sS2NoMat) != "Ti") {
                // Ｓ２ノズル材質がチタン系以外
                bS2SUS = true;
            }
            // ***********************
            if ((bS1SUS == true) && (bS2SUS == true)) {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
            }
            else if ((bS1SUS == false) && (bS2SUS == true)) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
            }
            else if ((bS1SUS == true) && (bS2SUS == false)) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
            }
            else {
                P_Zuban = P_DNO[4];
                P_SZuban = P_WKDNO[4];
            }
            P_ZaisituNM = Search_HD_MATERIAL(sS1NoMat) + "／" +
                Search_HD_MATERIAL(sS2NoMat);
            P_EZaisituNM = Search_HD_E_MATERIAL(sS1NoMat) + "／" +
                Search_HD_E_MATERIAL(sS2NoMat);
        }
        else if (P_FRAMECD[1] == "13") { // Ｅフレーム当板
            //2019.11.12 図番追加_S
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            //2019.11.12 図番追加_E
            sE1NoMat = P_HEADS_DATA[954].TrimRight();
            P_ZaisituNM = Search_HD_MATERIAL(sE1NoMat);
            P_EZaisituNM = Search_HD_E_MATERIAL(sE1NoMat);
        }
        //2015/03/19 アキュムレータの場合 ADD_S
        else if (P_FRAMECD[1] == "76") {
             if (P_HEADS_DATA[1116].ToIntDef(0) <= P_PLATE_NUM[1]) {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
            else if (P_HEADS_DATA[1116].ToIntDef(0) <= P_PLATE_NUM[2]) {
                P_Zuban = P_DNO[2];
                P_SZuban = P_WKDNO[2];
                if (P_MatNM[2] != "") {
                    P_ZaisituNM = P_MatNM[2];
                    P_EZaisituNM = P_MatNM[2];
                }
                else if (P_MatCD[2] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "2";
            }
            else if (P_HEADS_DATA[1116].ToIntDef(0) <= P_PLATE_NUM[3]) {
                P_Zuban = P_DNO[3];
                P_SZuban = P_WKDNO[3];
                if (P_MatNM[3] != "") {
                    P_ZaisituNM = P_MatNM[3];
                    P_EZaisituNM = P_MatNM[3];
                }
                else if (P_MatCD[3] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
                }
                else if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "3";
            }
            else {
                P_Zuban = P_DNO[1];
                P_SZuban = P_WKDNO[1];
                if (P_MatNM[1] != "") {
                    P_ZaisituNM = P_MatNM[1];
                    P_EZaisituNM = P_MatNM[1];
                }
                else if (P_MatCD[1] != "") {
                    P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                    P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
                }
                P_Siyou3 = "1";
            }
        }
        //2015/03/19 アキュムレータの場合 ADD_E
        //2018/10/28 ＳＵＳボルト図番追加_S
        else if (P_FRAMECD[1] == "18" || P_FRAMECD[1] == "23" ||
            P_FRAMECD[1] == "75") { // 締付ボルト

            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];

            sBltMat = P_HEADS_DATA[1041].TrimRight(); // 材質指定
            if (sBltMat == "1001" || sBltMat == "30C9" ||
                sBltMat == "31C9") { // 締付ボルト材質指定ＳＵＳ
                if (P_DNO[2] != "") {
                    P_Zuban = P_DNO[2];
                }
                if (P_WKDNO[2] != "") {
                    P_SZuban = P_WKDNO[2];
                }
            }
            else if (sBltMat == "" || sBltMat == "0000") { // 材質指定無し
                sBltMat = P_HEADS_DATA[1021].TrimRight(); // ＢＯＬ２２
                if (sBltMat == "SUS304" || sBltMat == "SS304") {
                    // 締付ボルトＢＯＬ２２ ＳＵＳ
                    if (P_DNO[2] != "") {
                        P_Zuban = P_DNO[2];
                    }
                    if (P_WKDNO[2] != "") {
                        P_SZuban = P_WKDNO[2];
                    }
                }
            }
        }
        //2019/10/28 ＳＵＳボルト図番追加_E
        else {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "";
        }
        // *************************************
    }
    else {
        P_Zuban = "";
        P_SZuban = "";
        P_Siyou3 = "";
    }
    // ***************************************

    // １台分数量
    if (Seek_Result == true) {
        if (P_FRAMECD[1] == "09" || P_FRAMECD[1] == "14" ||
            P_FRAMECD[1] == "15" || P_FRAMECD[1] == "16" ||
            P_FRAMECD[1] == "17" || P_FRAMECD[1] == "22") {
            P_1DaiSuryo = 1;
            // 2006.01.10 図番が"Ｅ００００００"もしくは"E000000"の場合数量を０にする
            if (P_Zuban == "Ｅ００００００" || P_Zuban == "E000000") {
                P_1DaiSuryo = 0;
            }
            // **********************************************************************
        }
        else if (P_FRAMECD[1] == "18") { // 締付ボルト
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0);
        }
        // 2014/08/22 応用技術編集
        else if (P_FRAMECD[1] == "75") { // 締付ボルト(小径)
            P_1DaiSuryo = P_HEADS_DATA[1113].ToIntDef(0);
        }
        else if (P_FRAMECD[1] == "19") { // 廻り止め
            // 2017.06.23 LX-90D追加
            //P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
            //    P_HEADS_DATA[1287].ToIntDef(0);
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                P_HEADS_DATA[1287].ToIntDef(0) +
                P_HEADS_DATA[1113].ToIntDef(0);
            // *********************
        }
        else if (P_FRAMECD[1] == "20") { // スラストワッシャ
            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            // 2011.02.22 ｽﾗｽﾄﾜｯｼｬ数量条件式変更(UX-90H21)
            // 2009.06.19 ｽﾗｽﾄﾜｯｼｬ数量条件式変更(UX-100R追加)
            // 2008.05.21 ｽﾗｽﾄﾜｯｼｬ数量条件式変更(RX-100追加)
            // 2006.02.06 ｽﾗｽﾄﾜｯｼｬ数量条件式変更(SX-90M,L追加)
            // 2006.01.10 ｽﾗｽﾄﾜｯｼｬ数量条件式変更(RX-90追加)
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＲＸ−７０"){
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０"){
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            // s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０"){
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            // s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０" ||
            // s_Text == "ＲＸ−１００"){
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            // s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０" ||
            // s_Text == "ＲＸ−１００" || s_Text == "ＵＸ−１００Ｒ"){
            if (s_Text == "ＵＸ−９０") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VERが200207(旧ﾌﾚｰﾑ)
                    P_1DaiSuryo = 4;
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VERが300207(旧ﾌﾚｰﾑ)
                    P_1DaiSuryo = 4;
                }
                else {
                    // H21
                    P_1DaiSuryo = 8;
                }
            }
            // 2016.07.25 ｽﾗｽﾄﾜｯｼｬ数量条件式変更
            //else if (s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            //    s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０" ||
            //    s_Text == "ＲＸ−１００" || s_Text == "ＵＸ−１００Ｒ") {
                // ********************************************
                // ***********************************************
                // *********************************************
                // **********************************************
                // *******************************************
            //    P_1DaiSuryo = 4;
            //}
            else if (s_Text == "ＵＸ−８０") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＵＸ−１００") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＵＸ−１００Ｒ") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＵＸ−１１０Ｒ") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＵＸ−１３０") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＵＸ−１３０Ｒ") {
                P_1DaiSuryo = 4;
            }
            // 2017.06.23 LX-90D追加
               else if (s_Text == "ＬＸ−９０Ｄ") {
                P_1DaiSuryo = 8;
            }
            // *********************
            else if (s_Text == "ＳＸ−９０Ｍ") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＳＸ−９０Ｌ") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＲＸ−９０") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＲＸ−１００") {
                P_1DaiSuryo = 4;
            }
            else if (s_Text == "ＷＸ−９０") {
                P_1DaiSuryo = 4;
            // *********************************
                // 2012.08.23 ｽﾗｽﾄﾜｯｼｬ数量条件式変更(SX-80S)
            }
            else if (s_Text == "ＳＸ−８０Ｓ") {
                P_1DaiSuryo = 8;
                // *****************************************
            }
            // 2016.07.25 ｽﾗｽﾄﾜｯｼｬ数量条件式変更
            else if (s_Text == "ＳＸ−８０Ｍ") {
                P_1DaiSuryo = 8;
            }
            else if (s_Text == "ＳＸ−８０Ｌ") {
                P_1DaiSuryo = 8;
            }
            // *********************************
            else {
                P_1DaiSuryo = P_HEADS_DATA[1020].ToIntDef(0) * 2;
            }
        }
        else if (P_FRAMECD[1] == "23") {
            P_1DaiSuryo = P_HEADS_DATA[1287].ToIntDef(0);
        }
        else if (P_FRAMECD[1] == "21") { // ショートボルトワッシャ
            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            // 2011.02.22 ｽﾗｽﾄﾜｯｼｬ数量条件式変更(UX-90H21)
            // 2009.06.19 ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ数量条件式変更(UX-100R追加)
            // 2008.05.21 ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ数量条件式変更(RX-100追加)
            // 2006.02.06 ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ数量条件式変更(SX-90M,L追加)
            // 2006.01.10 ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ数量条件式変更(RX-90追加)
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＲＸ−７０"){
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            // s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０"){
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            // s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０" ||
            // s_Text == "ＲＸ−１００"){
            // if (s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            // s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０" ||
            // s_Text == "ＲＸ−１００" || s_Text == "ＵＸ−１００Ｒ"){
            if (s_Text == "ＵＸ−９０") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VERが200207(旧ﾌﾚｰﾑ)
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 4;
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VERが300207(旧ﾌﾚｰﾑ)
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 4;
                }
                else {
                    // H21
                    // 2012.02.13 ﾜｯｼｬ数量条件式変更(UX-90H21)
                    // if ((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[966].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                    // (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                    // // Eﾉｽﾞﾙあり
                    // P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) + P_HEADS_DATA[1287].ToIntDef(0) - 4;
                    // } else {
                    // // Eﾉｽﾞﾙなし
                    // P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) + P_HEADS_DATA[1287].ToIntDef(0) - 8;
                    // }
                    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                        P_HEADS_DATA[1287].ToIntDef(0) - 8;
                    // ***************************************
                }
            }
            // 2016.07.25 ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ数量条件式変更
            //else if (s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−９０Ｍ" ||
            //    s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０" ||
            //    s_Text == "ＲＸ−１００" || s_Text == "ＵＸ−１００Ｒ") {
            //    // ************************************************
            //    // ***************************************************
            //    // *************************************************
            //    // **************************************************
            //    // *******************************************
            //    P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
            //        P_HEADS_DATA[1287].ToIntDef(0) - 4;
            else if (s_Text == "ＵＸ−８０") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＵＸ−１００") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＵＸ−１００Ｒ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＵＸ−１１０Ｒ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＵＸ−１３０") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＵＸ−１３０Ｒ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            // 2017.06.23 LX-90D追加
            else if (s_Text == "ＬＸ−９０Ｄ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) +
                    P_HEADS_DATA[1113].ToIntDef(0) - 8;
            }
            // *********************
            else if (s_Text == "ＳＸ−９０Ｍ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＳＸ−９０Ｌ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＲＸ−９０") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＲＸ−１００") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            }
            else if (s_Text == "ＷＸ−９０") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 4;
            // *************************************
                // 2012.08.23 ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ数量条件式変更(SX-80S)
            }
            else if (s_Text == "ＳＸ−８０Ｓ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
                // *********************************************
            }
            // 2016.07.25 ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ数量条件式変更
            else if (s_Text == "ＳＸ−８０Ｍ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
            }
            else if (s_Text == "ＳＸ−８０Ｌ") {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0) - 8;
            }
            // *************************************
            else {
                P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                    P_HEADS_DATA[1287].ToIntDef(0);
            }
            // 2009.02.16 ﾜｯｼｬ追加
        }
        else if (P_FRAMECD[1] == "52") { // 廻り止め
            P_1DaiSuryo = P_HEADS_DATA[1286].ToIntDef(0) +
                P_HEADS_DATA[1287].ToIntDef(0);
            // *******************
            // 2012.02.13 ﾜｯｼｬ数量条件式変更(UX-90H21)
        }
        else if (P_FRAMECD[1] == "53") { // 上下端部ボルト用ワッシャ
            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            if (s_Text == "ＵＸ−９０") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VERが200207(旧ﾌﾚｰﾑ)
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VERが300207(旧ﾌﾚｰﾑ)
                }
                else {
                    // H21
                    if ((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[966].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                        (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                        // Eﾉｽﾞﾙあり
                        P_1DaiSuryo = 4;
                    }
                    else {
                        // Eﾉｽﾞﾙなし
                        P_1DaiSuryo = 0;
                    }
                }
                // 2012.08.23 上下端部ﾎﾞﾙﾄ用ﾜｯｼｬ数量条件式変更(SX-80S)
            }
            else if (s_Text == "ＳＸ−８０Ｓ") {
                P_1DaiSuryo = 2;
                // ***************************************************
            }
            // ***************************************
            // 2017.06.23 LX-90D,SX-80LM追加
            else if (s_Text == "ＬＸ−９０Ｄ") {
                P_1DaiSuryo = 2;
            }
            else if (s_Text == "ＳＸ−８０Ｌ") {
                P_1DaiSuryo = 2;
            }
            else if (s_Text == "ＳＸ−８０Ｍ") {
                P_1DaiSuryo = 2;
            }
            // *****************************
        }
        else if (P_FRAMECD[1] == "48") {
            // 2003.12.24 ＹＸ−８０フレーム当板追加
        }
        else if (P_FRAMECD[1] == "06") { // Ｓフレーム当板
            P_1DaiSuryo = 1;
        }
        else if (P_FRAMECD[1] == "13") { // Ｅフレーム当板
            sE1NoMat = P_HEADS_DATA[954].TrimRight();
            // 2016.11.07 材質ｺｰﾄﾞ変更
            //if (sE1NoMat == "1001" || sE1NoMat == "1003")
            if (Type_MAT(sE1NoMat) == "SUS304" || Type_MAT(sE1NoMat) == "SUS316")
            // ***********************
            { // Ｅ１ノズル材質が SUS304[1001],SUS316[1003]）
                P_1DaiSuryo = 1;
            }
            else {
                P_1DaiSuryo = 0;
            }
            // *************************************
            // 2004.03.16 ＲＸ−００ベースプレート追加
        }
        else if (P_FRAMECD[1] == "49") { // ベースプレート
            P_1DaiSuryo = 1;
            // ***************************************
        }
        // 2021.10.25 ｽﾍﾟｰｻｰ追加_S
        else if (P_FRAMECD[1] == "79") { // スペーサー
            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            // 2022.01.24 スペーサ追加_S
            // Ｓフレームスペーサ
            sAGasMat = P_HEADS_DATA[836].TrimRight();
            sBGasMat = P_HEADS_DATA[835].TrimRight();
            P_1DaiSuryo = 0;
            //if (s_Text == "ＷＨ−３０") {
            //    if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[966].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
            //        (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
            //        // Eﾉｽﾞﾙ無い場合
            //        P_1DaiSuryo = 1;
            //    }
            //    else {
            //        P_1DaiSuryo = 2;
            //        // Eﾉｽﾞﾙ有る場合
            //    }
            //}
            //else {
            //    P_1DaiSuryo = 0;
            //}
            if (s_Text == "ＵＸ−９０") {
                // UX-90
                if (sAGasMat == "2067" || sBGasMat == "2067" ) {
                    // GZ-NBR
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "ＬＸ−１０") {
                // LX-10
                if (sAGasMat == "2063" || sBGasMat == "2063" ||
                    sAGasMat == "2064" || sBGasMat == "2064" ) {
                    // TCG
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "ＷＨ−３０") {
                // WH-30
                P_1DaiSuryo = 1;
            }
            //2023.03.20 CX-03-Y修正_S
            ////2023.03.09 CX-03-Y追加_S
            //else if (((s_Text == "ＣＸ−０３") || (s_Text == "ＣＸ−０３Ｄ") ||
            //          (s_Text == "ＣＸＷ−０３")) &&
            //          (P_HEADS_DATA[36].Pos("Y") != 0)) {
            //    P_1DaiSuryo = 2;
            //}
            ////2023.03.09 CX-03-Y追加_E
            //2023.03.20 CX-03-Y修正_E

            // *************************
            // ***  孔明仕様の作成
            // *************************
            if (P_1DaiSuryo == 1) {
                P_Siyou1 = Get_AnaakeSiyou_S("S");
                G_Log_Text = "Ｓフレームスペーサー孔明仕様  『" + P_Siyou1 + "』を取得。";
                Write_Log(G_Log_Text);
            }
            // 2022.01.24 スペーサ追加_E
        }
        else if (P_FRAMECD[1] == "80") { // シムプレートー
            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            if (s_Text == "ＷＨ−３０") {
                P_1DaiSuryo = 4;
            }
            else {
                P_1DaiSuryo = 0;
            }
        }
        // 2021.10.25 ｽﾍﾟｰｻｰ追加_E

        // 2022.01.24 スペーサ追加_S
        // 2022.xx.xx EスペーサーCODE変更_S
        //else if (P_FRAMECD[1] == "81") { // Ｅフレームスペーサー
        else if (P_FRAMECD[1] == "82") { // Ｅフレームスペーサー
        // 2022.xx.xx EスペーサーCODE変更_E
            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            sAGasMat = P_HEADS_DATA[836].TrimRight();
            sBGasMat = P_HEADS_DATA[835].TrimRight();
            P_1DaiSuryo = 0;
            if (s_Text == "ＵＸ−９０") {
                // UX-90
                if (sAGasMat == "2067" || sBGasMat == "2067" ) {
                    // GZ-NBR
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "ＬＸ−１０") {
                // LX-10
                if (sAGasMat == "2063" || sBGasMat == "2063" ||
                    sAGasMat == "2064" || sBGasMat == "2064" ) {
                    // TCG
                    P_1DaiSuryo = 1;
                }
            }
            else if (s_Text == "ＷＨ−３０") {
                // WH-30
                P_1DaiSuryo = 1;
            }

            // *************************
            // ***  孔明仕様の作成
            // *************************
            if (P_1DaiSuryo == 1) {
                P_Siyou1 = Get_AnaakeSiyou_S("E");
                G_Log_Text = "Ｅフレームスペーサー孔明仕様  『" + P_Siyou1 + "』を取得。";
                Write_Log(G_Log_Text);
            }
            if ( P_Siyou1  == "E0000") {
                // ノズルがない場合Ｅスペーサー不要
                P_1DaiSuryo = 0;
            }
        }
        // 2022.01.24 スペーサ追加_E
    }
    else {
        P_1DaiSuryo = 0;
    }

    // 総計
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

    // 仕様１
    // HanToZen(s_JPKbn);
    if (Seek_Result == true) {

        s_Text = P_FRAMECD[1];

        // 2014/08/22 応用技術編集
        if (s_Text == "18" || s_Text == "23" || s_Text == "75") { // 締付ボルト

            // 参照するデータインデックス
            int i_BoltRadiusNo = 1019;  // ボルト径
            int i_BoltLengthNo = 1376;  // ボルト全長

            if(s_Text == "75") {
                // BNWRN-,締付BNWの場合、参照データインデックスを変更
                i_BoltRadiusNo = 1108;
                i_BoltLengthNo = 1377;

                // 2014/09/04 応用技術編集
                if(StrToDblDef(P_HEADS_DATA[i_BoltLengthNo].TrimRight(), 0.0) <= 0.0) {
                    // Sボルト全長が0mm(未設定)の場合、Lボルト全長を採用する
                    i_BoltLengthNo = 1376;
                }
            }

            // 材質決定ロジック
            sBltMat = P_HEADS_DATA[1041].TrimRight(); // 材質指定
            if (sBltMat == "1001") {
                P_ZaisituNM = "SUS304/C3604";
                P_EZaisituNM = "SUS304/C3604";
                // 2011.05.30 ﾎﾞﾙﾄ材質指定追加
            }
            // 2016.09.01 材質ｺｰﾄﾞ変更
            else if (sBltMat == "30C9") {
                P_ZaisituNM = "SUS304/C3604";
                P_EZaisituNM = "SUS304/C36004";
            }
            else if (sBltMat == "31C9") {
                P_ZaisituNM = "SUS304/C3604";
                P_EZaisituNM = "SUS304/C3604";
            }
            else if (sBltMat == "1040") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            // 2016.11.07 材質ｺｰﾄﾞ変更
            else if (sBltMat == "1062") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            // ***********************
            else if (sBltMat == "30C1") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            else if (sBltMat == "31C1") {
                P_ZaisituNM = "SS400/SS400";
                P_EZaisituNM = "SS400/SS400";
            }
            else if (sBltMat == "1069") {
                P_ZaisituNM = "SNB7/S45C";
                P_EZaisituNM = "SNB7/S45C";
            }
            else if (sBltMat == "30C2") {
                P_ZaisituNM = "SNB7/S45C";
                P_EZaisituNM = "SNB7/S45C";
            }
            else if (sBltMat == "31C2") {
                P_ZaisituNM = "SNB7/S45C";
                P_EZaisituNM = "SNB7/S45C";
            }
            // **********************
            else if (sBltMat == "1086") {
                P_ZaisituNM = "SA193B7/SA194-2H";
                P_EZaisituNM = "SA193B7/SA194-2H";
                // ***************************
            }
            // 2016.09.01 材質ｺｰﾄﾞ変更
            else if (sBltMat == "32C2") {
                P_ZaisituNM = "SA193B7/SA194-2H";
                P_EZaisituNM = "SA193B7/SA194-2H";
            // **********************
            }
            // 2015.01.09 ﾎﾞﾙﾄ材質指定追加
            else if (sBltMat == "1169") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            }
            else if (sBltMat == "1171") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            }
            // 2016.09.01 材質ｺｰﾄﾞ変更
            else if (sBltMat == "33C2") {
                P_ZaisituNM = "A193B7/A194-2H";
                P_EZaisituNM = "A193-B7/A194-2H";
            // **********************
            }
            // ***************************
            // 2016.05.10 ﾎﾞﾙﾄ材質指定追加
            else if (sBltMat == "1186") {
                // 2016.11.07 材質ｺｰﾄﾞ変更
                //P_ZaisituNM = "SNB7/S45C eqv.";
                P_ZaisituNM = "SNB7/S45C相当";
                // ***********************
                P_EZaisituNM = "SNB7/S45C eqv.";
            }
            // ***************************
            else {
                sBltMat = P_HEADS_DATA[1021].TrimRight(); // BOL22
                if (sBltMat == "SUS304") {
                    P_ZaisituNM = "SUS304/C3604";
                    P_EZaisituNM = "SUS304/C3604";
                }
                else if (sBltMat == "SS400") {
                    P_ZaisituNM = "SS400/SS400";
                    P_EZaisituNM = "SS400/SS400";
                }
                else if (sBltMat == "S45C-D") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SCM435") {
                    P_ZaisituNM = "SCM435/S45C";
                    P_EZaisituNM = "SCM435/S45C";
                }
                else if (sBltMat == "SCM445") {
                    P_ZaisituNM = "SCM435/S45C";
                    P_EZaisituNM = "SCM435/S45C";
                }
                else if (sBltMat == "S45CH") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SNB 7") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SNB7") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                // 2014/08/26 応用技術編集
                else if (sBltMat == "45/35") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                else if (sBltMat == "35CrMo") {
                    P_ZaisituNM = "35CrMo/32CrMo";
                    P_EZaisituNM = "35CrMo/32CrMo";
                }
                // 2021.09.22 RX-70Z追加_S
                else if (sBltMat == "35CrMo2") {
                    P_ZaisituNM = "35CrMo/45";
                    P_EZaisituNM = "35CrMo/45";
                }
                // 2021.09.22 RX-70Z追加_E
                // 2015.01.09 ﾎﾞﾙﾄ材質指定追加
                else if (sBltMat == "SA193-B7") {
                    P_ZaisituNM = "SA193B7/SA194-2H";
                    P_EZaisituNM = "SA193B7/SA194-2H";
                }
                else if (sBltMat == "A193-B7") {
                    P_ZaisituNM = "A193-B7/A194-2H";
                    P_EZaisituNM = "A193-B7/A194-2H";
                }
                // ***************************
                // 2016.11.07 材質ｺｰﾄﾞ変更
                else if (sBltMat == "C.S.") {
                    P_ZaisituNM = "SS400/SS400";
                    P_EZaisituNM = "SS400/SS400";
                }
                else if (sBltMat == "SNB7 eqv.") {
                    P_ZaisituNM = "SNB7/S45C相当";
                    P_EZaisituNM = "SNB7/S45C eqv.";
                }
                else if (sBltMat == "B7") {
                    P_ZaisituNM = "SNB7/S45C";
                    P_EZaisituNM = "SNB7/S45C";
                }
                else if (sBltMat == "SS304") {
                    P_ZaisituNM = "SUS304/C3604";
                    P_EZaisituNM = "SUS304/C3604";
                }
                // 2017.06.23 LX-90D
                else if (sBltMat == "SA540-B23") {
                    P_ZaisituNM = "SA540B23/SA194-2H";
                    P_EZaisituNM = "SA540B23/SA194-2H";
                }
                // *****************
                // ***********************
                // 2018.06.19 LX-30Z追加_S
                else if (sBltMat == "Q235B") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
                else if (sBltMat == "Q235B2") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
                // 2018.06.19 LX-30Z追加_E
                // 2020.05.27 LX-50Z追加_S
                else if (sBltMat == "45") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                else if (sBltMat == "45_2") {
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
                // 2020.05.27 LX-50Z追加_E
                // 2021.09.22 RX-50Z,70Z追加_S
                else if (sBltMat == "42CrMo") {
                    P_ZaisituNM = "42CrMo/45";
                    P_EZaisituNM = "42CrMo/45";
                }
                else if (sBltMat == "42CrMo2") {
                    P_ZaisituNM = "42CrMo/45";
                    P_EZaisituNM = "42CrMo/45";
                }
                // 2021.09.22 RX-50Z,70Z追加_E
                else {
                    P_ZaisituNM = "";
                    P_EZaisituNM = "";
                    sBltMat = "0";
                }
            }

            // 2019.11.21 LX-90D-NUP材質変更_S
            //// 2017.06.23 LX-90D-NUP追加
            //s_Text = G_KS_Syurui + "−" + G_KS_Model;
            //if( P_FRAMECD[1] == "18" ) {
            //    if (( s_Text == "ＬＸ−９０Ｄ") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
            //        // LX-90D-KNUP
            //        P_ZaisituNM = "SA540B23/SA194-2H";
            //        P_EZaisituNM = "SA540B23/SA194-2H";
            //    } else if (( s_Text == "ＬＸ−９０") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
            //        // LX-90D-TNUP
            //        P_ZaisituNM = "SA540B23/SA194-2H";
            //        P_EZaisituNM = "SA540B23/SA194-2H";
            //    }
            //}
            //// *************************
            // 2019.11.21 LX-90D-NUP材質変更_E

            // 2020.05.27 LX-50Z追加_E
            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            // 2020.05.27 LX-50Z追加_E

            // 2018.06.19 LX-30Z追加_S
            if (sBltMat == "Q235B2") {
                if (( s_Text == "ＬＸ−３０" ) && ( P_FRAMECD[1] == "75" )){
                    // LX-30Z 細ﾎﾞﾙﾄ 45/35
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
            }
            // 2018.06.19 LX-30Z追加_E

            // 2020.05.27 LX-50Z追加_S
            if ( sBltMat == "45_2" ) {
                if (( s_Text == "ＬＸ−５０" ) && ( P_FRAMECD[1] == "75" )){
                    // LX-50Z 細ﾎﾞﾙﾄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // 2020.05.27 LX-50Z追加_E

            // 2021.09.22 RX-50Z,70Z追加_S
            if ( sBltMat == "45_2" ) {
                if (( s_Text == "ＲＸ−５０" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-50Z 細ﾎﾞﾙﾄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                } else if (( s_Text == "ＲＸ−７０" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-70Z 細ﾎﾞﾙﾄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }

            if ( sBltMat == "42CrMo2" ) {
                if (( s_Text == "ＲＸ−５０" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-50Z 細ﾎﾞﾙﾄ 45/35
                    P_ZaisituNM = "45/35";
                    P_EZaisituNM = "45/35";
                }
            }

            if ( sBltMat == "35CrMo2" ) {
                if (( s_Text == "ＲＸ−７０" ) && ( P_FRAMECD[1] == "75" )){
                    // RX-70Z 細ﾎﾞﾙﾄ Q235B
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // 2021.09.22 RX-50Z,70Z追加_E

            // 2012.01.20 中国材質
            i_RecNo = 1107;
            s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

            s_Text = G_KS_Syurui + "−" + G_KS_Model;
            if (s_Text == "ＵＸ−１０") { // 中国材質
            }
            else if (s_Text == "ＬＸ−００") { // 中国材質
            }
            else if (s_Text == "ＬＸ−１０") { // 中国材質
            }
            else if (s_Text == "ＬＸ−３０") { // 中国材質
            }
            else if (s_Text == "ＬＸ−５０") { // 中国材質
            }
            else if (s_Text == "ＳＸ−２０") { // 中国材質
            }
            else if (s_Text == "ＳＸ−４１") { // 中国材質
            }
            else if (s_Text == "ＳＸ−４３") { // 中国材質
            }
            else if (s_Text == "ＲＸ−１１") { // 中国材質
            }
            else if (s_Text == "ＲＸ−１３") { // 中国材質
            }
            else if (s_Text == "ＲＸ−３０") { // 中国材質
            }
            else if (s_Text == "ＲＸ−５０") { // 中国材質
            }
            else if (s_Text == "ＲＸ−７０") { // 中国材質
            }
            else {
                s_ChinaM = "";
            }

            if (s_ChinaM == "CHINA") { // 中国材質
                if (P_ZaisituNM == "SS400/SS400") {
                    P_ZaisituNM = "Q235B/Q235B";
                    P_EZaisituNM = "Q235B/Q235B";
                }
            }
            // *******************

            s_Text = P_HEADS_DATA[i_BoltRadiusNo].TrimRight();
            if (s_Text == "1.630") {
                //2020.10.02 ボルト寸法変更_S
                //P_Siyou1 =
                //    "１　５／８−８ＵＮ×" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                //    + "Ｌ×全Ｓ";
                P_Siyou1 =
                    "1 5/8-8UN×" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                    + "Ｌ×全Ｓ";
                //2020.10.02 ボルト寸法変更_E

            }
            else if (s_Text == "2.000") {
                P_Siyou1 = "２−８ＵＮ×" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight())
                    + "Ｌ×全Ｓ";

            }
            else {
                P_Siyou1 = "Ｍ" + HanToZen(P_HEADS_DATA[i_BoltRadiusNo].SubString(1, 2)) +
                    "×" + HanToZen(P_HEADS_DATA[i_BoltLengthNo].TrimRight()) + "Ｌ×全Ｓ";

            }
            P_Yobisu = Get_Blt_YobiSu(StrToDblDef(s_Text.TrimRight(), 0.0),
                StrToDblDef(P_HEADS_DATA[i_BoltLengthNo].TrimRight(), 0.0), sBltMat,
                m_pcHeadsData);
            P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
        }
        else if (s_Text == "26") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1092].TrimRight()) + "Ｌ";

        }
        else if (s_Text == "27") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1096].TrimRight()) + "Ｌ　キャップ付き";

        }
        else if (s_Text == "28") {
            P_Siyou1 = HanToZen(P_HEADS_DATA[1097].TrimRight()) + "Ｌ　キャップ付き";

            // 2012.02.13 ﾜｯｼｬ数量条件式変更(UX-90H21)
            //// 2010.04.19 UX-90ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ変更
            // } else if (s_Text == "21" ) {
        }
        else if (s_Text == "53") {
            // 上下端部ボルト用ワッシャ
            // ***************************************
            s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
            if (s_Text2 == "ＵＸ−９０") {
                if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                    // HEAD.VERが200207(旧ﾌﾚｰﾑ)
                    P_Siyou1 = "";
                    // 2010.12.14 UX-90落とし込みGB追加
                }
                else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
                    // HEAD.VERが300207(旧ﾌﾚｰﾑ落し込みGB)
                    // ********************************
                }
                else {
                    if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                        // S1継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[893].TrimRight();
                    }
                    else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                        // S2継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[908].TrimRight();
                    }
                    else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                        // S3継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[923].TrimRight();
                    }
                    else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                        // S4継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[938].TrimRight();
                    }
                    else if ((P_HEADS_DATA[951].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[951].ToIntDef(0) == 2)) {
                        // E1継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[953].TrimRight();
                    }
                    else if ((P_HEADS_DATA[966].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[966].ToIntDef(0) == 2)) {
                        // E2継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[968].TrimRight();
                    }
                    else if ((P_HEADS_DATA[981].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[981].ToIntDef(0) == 2)) {
                        // E3継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[983].TrimRight();
                    }
                    else if ((P_HEADS_DATA[996].ToIntDef(0) == 1) ||
                        (P_HEADS_DATA[996].ToIntDef(0) == 2)) {
                        // E4継手ｺｰﾄﾞ
                        s_Text2 = P_HEADS_DATA[998].TrimRight();
                    }
                    else {
                        // ﾉｽﾞﾙなし
                        s_Text2 = "999";
                    }

                    // 2011.06.20 ﾉｽﾞﾙ規格変更
                    // if (s_Text2 == "414" || s_Text2 == "424" || s_Text2 == "434" ||
                    // s_Text2 == "514" || s_Text2 == "524" || s_Text2 == "534" ||
                    // s_Text2 == "814" || s_Text2 == "824" || s_Text2 == "834" ||
                    // s_Text2 == "914" || s_Text2 == "924" || s_Text2 == "934") {
                    if (s_Text2 == "0414" || s_Text2 == "0424" ||
                        s_Text2 == "0434" || s_Text2 == "0514" ||
                        s_Text2 == "0524" || s_Text2 == "0534" ||
                        s_Text2 == "0816" || s_Text2 == "0826" ||
                        s_Text2 == "0836" || s_Text2 == "0916" ||
                        s_Text2 == "0926" || s_Text2 == "0936") {
                        // ***********************
                        // ANSI,JPI
                        P_Siyou1 = "φＡ＝７９※※手配注意のこと※※";
                    }
                    else if (s_Text2 == "999") {
                        // ﾉｽﾞﾙなし
                        P_Siyou1 = "展開不能";
                    }
                    else {
                        // JIS,DIN
                        P_Siyou1 = "φＡ＝８５";
                    }
                }
                // 2014.04.15 ﾜｯｼｬ数量条件式追加(UX-10-EP,LX-10,EP)
                //// 2012.06.08 ﾜｯｼｬ数量条件式変更(RX-10-EP)
                // } else if (s_Text2 == "ＲＸ−１１" || s_Text2 == "ＲＸ−１３") {
            }
            else if (s_Text2 == "ＵＸ−１０" || s_Text2 == "ＬＸ−１０" ||
                s_Text2 == "ＲＸ−１１" || s_Text2 == "ＲＸ−１３") {
                // ************************************************
                if ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                    (P_HEADS_DATA[996].ToIntDef(0) != 0)) {
                    // E3,E4ﾉｽﾞﾙ有りﾞ
                    P_1DaiSuryo = 2;
                    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
                    P_Siyou4 = "式";
                }
                else {
                    // ﾉｽﾞﾙなし
                    P_1DaiSuryo = 0;
                    P_Soukei = 0;
                    P_Siyou4 = "";
                }
                // ****************************************
            }
            else {
                P_Siyou1 = "";
            }
            // *********************************
            // 2011.03.11 RX-70ｻﾎﾟｰﾄ注記追加
        }
        else if (s_Text == "16") {
            s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
            if (s_Text2 == "ＲＸ−７０") {
                if (P_HEADS_DATA[1046].TrimRight() != "") {
                    P_Siyou1 = P_HEADS_DATA[1046].TrimRight() + "用";
                }
                else {
                    P_Siyou1 = "";
                }
            }
            else {
                P_Siyou1 = "";
            }
            // ************************
        }
        // 2022.01.14 スペーサ追加_S
        // 2022.08.05EスペーサーCODE変更_S
        //else if (s_Text == "79"|| s_Text == "81") {
        else if (s_Text == "79"|| s_Text == "82") {
        // 2022.08.05EスペーサーCODE変更_E
            // スペーサ
            P_Siyou1 = "孔明" + P_Siyou1;
            //2023.03.20 CX-03-Y修正_S
            ////2023.03.09 CX-03-Y追加_S
            //s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
            //if (((s_Text2 == "ＣＸ−０３") || (s_Text2 == "ＣＸ−０３Ｄ") ||
            //     (s_Text2 == "ＣＸＷ−０３")) &&
            //     (P_HEADS_DATA[36].Pos("Y") != 0)) {
            //    P_Siyou1 = "";
            //}
            ////2023.03.09 CX-03-Y追加_E
            //2023.03.20 CX-03-Y修正_E
        }
        // 2022.01.14 スペーサ追加_E
        else {
            P_Siyou1 = "";
        }

    }
    else {
        P_Siyou1 = "";
    }

    // 仕様２(S,Eﾌﾚｰﾑ種類)
    if (Seek_Result == true) {
        s_Text = P_FRAMECD[1];
        if (s_Text == "09" || s_Text == "16") {
            if (P_HEADS_DATA[1290].TrimRight() != "") {
                P_Siyou2 = P_HEADS_DATA[1290].TrimRight() + "用";
            }
            else {
                P_Siyou2 = "";
            }

        }
        else if (s_Text == "14") {
            if (P_HEADS_DATA[1290].TrimRight() != "") {
                P_Siyou2 = P_HEADS_DATA[1290].TrimRight();
            }
            else {
                P_Siyou2 = "";
            }

            // 2010.12.14 下GB注記追加
        }
        else if (s_Text == "15") {
            if (P_HEADS_DATA[1046].TrimRight() != "") {
                P_Siyou2 = P_HEADS_DATA[1046].TrimRight();
            }
            else {
                P_Siyou2 = "";
            }
            // ***********************

        }
        // 2014/08/22 応用技術編集
        else if (s_Text == "18" || s_Text == "23" || s_Text == "24" ||
                 s_Text == "25" || s_Text == "75") {
            P_Siyou2 = P_HEADS_DATA[1317].TrimRight();
        }
        else {
            P_Siyou2 = "";
        }

    }
    else {
        P_Siyou2 = "";
    }

    // 仕様４  ( 02.08.09 追加 )
    // 2014/08/22 応用技術編集
    if ((s_Text == "18") || (s_Text == "23") || (s_Text == "14") ||
        (s_Text == "15") || (s_Text == "75")) {
        P_Siyou4 = "式";
    }
    // 2009.02.25 WX-50,CX-10Dベースプレート追加
    else if (s_Text == "49") {
        s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
        // 2009.07.31 ベースプレート数量変更
        // RX-00    １
        // その他   １式
        // if (s_Text2 == "ＷＸ−５３") {
        // P_Siyou4 = "式";
        // } else if  (s_Text2 == "ＣＸ−１０Ｄ") {
        // P_Siyou4 = "式";
        // }
        if (s_Text2 == "ＲＸ−００") {
            P_Siyou4 = "";
        }
        else {
            P_Siyou4 = "式";
        }
        // *********************************
        // 2010.04.19 UX-90ｼｮｰﾄﾎﾞﾙﾄﾜｯｼｬ変更
    }
    else if (s_Text == "21") {
        s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
        if (s_Text2 == "ＵＸ−９０") {
            if (P_HEADS_DATA[1267].TrimRight() == "200207") {
                // HEAD.VERが200207(旧ﾌﾚｰﾑ)
                P_Siyou4 = "";
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                // Eﾉｽﾞﾙ無い場合
                P_Siyou4 = "";
            }
            else {
                // 新ﾌﾚｰﾑでEﾉｽﾞﾙ付き
                P_Siyou4 = "式";
            }
        }
        else {
            P_Siyou4 = "";
        }
        // *********************************
    }
    // *****************************************
    else {
        P_Siyou4 = "";
    }

    // 仕様５
    P_Siyou5 = "";

    // ****************************
    // ***  S,Eﾌﾚｰﾑﾃﾞｰﾀ  セット
    // ****************************
    // 部品展開ﾃﾞｰﾀ　OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  部品分類
    s_Text = P_BBunrui;
    fputs(s_Text.c_str(), fp);
    // 2  識別子
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), fp);
    // 3  部品型式
    s_Text = "," + P_BKatasiki;
    fputs(s_Text.c_str(), fp);
    // 4  部品名称
    s_Text = "," + P_BMeisyou;
    fputs(s_Text.c_str(), fp);
    // 5  材質名称
    s_Text = "," + P_ZaisituNM;
    fputs(s_Text.c_str(), fp);
    // 6  図番
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  作図用図番
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1台分数量
    s_Text = "," + AnsiString(P_1DaiSuryo);
    fputs(s_Text.c_str(), fp);
    // 9  予備数
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), fp);

    // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);

    // 11 仕様１
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), fp);
    // 12 仕様２
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), fp);
    // 13 仕様３
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), fp);
    // 14 仕様４
    s_Text = "," + P_Siyou4;
    fputs(s_Text.c_str(), fp);
    // 15 仕様５
    s_Text = "," + P_Siyou5;
    fputs(s_Text.c_str(), fp);
    // 16  材質名称(英)
    s_Text = "," + P_EZaisituNM;
    fputs(s_Text.c_str(), fp);

    // ------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), fp);
    // 2007.10.03
    // ------------------------------

    // 改行文字
    s_Text = "\n";
    // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
    fputs(s_Text.c_str(), fp);

    // ボルト長短有りの処理
    if (Seek_Result == true) {
        s_Text = P_FRAMECD[1];
        P_1DaiSuryo = P_HEADS_DATA[1287].ToIntDef(0);
        // 総計
        l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
        P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;
        if ((s_Text == "18" || s_Text == "23")) {
            if ((P_1DaiSuryo > 0)) {
                s_Text = P_HEADS_DATA[1019].TrimRight(); // BDI
                if (s_Text == "1.630") {
                    //2020.10.02 ボルト寸法変更_S
                    //P_Siyou1 =
                    //    "１　５／８−８ＵＮ×" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                    //    + "Ｌ×全Ｓ";
                    P_Siyou1 =
                        "1 5/8-8UN×" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                        + "Ｌ×全Ｓ";
                    //2020.10.02 ボルト寸法変更_E

                }
                else if (s_Text == "2.000") {
                    P_Siyou1 =
                        "２−８ＵＮ×" + HanToZen(P_HEADS_DATA[1377].TrimRight())
                        + "Ｌ×全Ｓ";

                }
                else {
                    P_Siyou1 =
                        "Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) +
                        "×" + HanToZen(P_HEADS_DATA[1377].TrimRight()) + "Ｌ×全Ｓ";

                }
                P_Yobisu = Get_Blt_YobiSu(StrToDblDef(s_Text.TrimRight(), 0.0),
                    StrToDblDef(P_HEADS_DATA[1377].TrimRight(), 0.0), sBltMat,
                    m_pcHeadsData);
                P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

                // 1  部品分類
                s_Text = P_BBunrui;
                fputs(s_Text.c_str(), fp);
                // 2  識別子
                s_Text = "," + P_Sikibetu;
                fputs(s_Text.c_str(), fp);
                // 3  部品型式
                s_Text = ",";
                fputs(s_Text.c_str(), fp);
                // 4  部品名称
                s_Text = ",";
                fputs(s_Text.c_str(), fp);
                // 5  材質名称
                s_Text = "," + P_ZaisituNM;
                fputs(s_Text.c_str(), fp);
                // 6  図番
                s_Text = "," + P_Zuban;
                fputs(s_Text.c_str(), fp);
                // 7  作図用図番
                s_Text = "," + P_SZuban;
                fputs(s_Text.c_str(), fp);
                // 8  1台分数量
                s_Text = "," + AnsiString(P_1DaiSuryo);
                fputs(s_Text.c_str(), fp);
                // 9  予備数
                s_Text = "," + AnsiString(P_Yobisu);
                fputs(s_Text.c_str(), fp);

                // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
                s_Text = "," + AnsiString(P_Soukei);
                fputs(s_Text.c_str(), fp);

                // 11 仕様１
                s_Text = "," + P_Siyou1;
                fputs(s_Text.c_str(), fp);
                // 12 仕様２
                s_Text = "," + P_Siyou2;
                fputs(s_Text.c_str(), fp);
                // 13 仕様３
                s_Text = "," + P_Siyou3;
                fputs(s_Text.c_str(), fp);
                // 14 仕様４
                s_Text = "," + P_Siyou4;
                fputs(s_Text.c_str(), fp);
                // 15 仕様５
                s_Text = "," + P_Siyou5;
                fputs(s_Text.c_str(), fp);
                // 16  材質名称(英)
                s_Text = "," + P_EZaisituNM;
                fputs(s_Text.c_str(), fp);

                // ------------------------------
                // 2007.10.03 ﾐﾗｰ仕様対応
                // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
                s_Text = "," + sSpecCode;
                fputs(s_Text.c_str(), fp);
                // 2007.10.03
                // ------------------------------

                // 改行文字
                s_Text = "\n";
                // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
                fputs(s_Text.c_str(), fp);
            }

            // 2017.06.23 LX-90D-NUP追加
            // 2014/08/22 応用技術編集
            //if(P_HEADS_DATA[36].Pos("Z") == 0) {
            //    // Zフレームで無い場合、このタイミングでボルトカバー出力へ
            //    BLT_Cover_Out();
            //}
            s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
            if (( s_Text2 == "ＬＸ−９０Ｄ") && ( P_HEADS_DATA[37].ToIntDef(0)) == 94 ){
                // LX-90D-KNUPの場合、別のタイミングでボルトカバー出力へ
            } else if (( s_Text2 == "ＬＸ−９０") && ( P_HEADS_DATA[37].ToIntDef(0)) == 394 ){
                // LX-90D-TNUPの場合、別のタイミングでボルトカバー出力へ
            } else if (P_HEADS_DATA[36].Pos("Z") == 0) {
                // Zフレームで無い場合、このタイミングでボルトカバー出力へ
                BLT_Cover_Out();
            }
            // *************************
        }
    }

    // 2017.06.23 LX-90D-NUP追加
    s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
    // *************************

    // 2014/08/22 応用技術編集
    if(P_FRAMECD[1] == "75" &&
       P_HEADS_DATA[36].Pos("Z") > 0) {
        // コードが"75"(BNWRN-,締付BNW) かつ Zフレームの場合、ボルトカバー出力へ
        BLT_Cover_Out();
        BLT_Cover_Out_Small();
    // 2017.06.23 LX-90D-NUP追加
    } else if ( P_FRAMECD[1] == "75" &&
                s_Text2 == "ＬＸ−９０Ｄ" &&
                P_HEADS_DATA[37].ToIntDef(0) == 94 ){
        // LX-90D-KNUPの場合、ボルトカバー出力へ
        BLT_Cover_Out();
    } else if ( P_FRAMECD[1] == "75" &&
                s_Text2 == "ＬＸ−９０" &&
                P_HEADS_DATA[37].ToIntDef(0) == 394 ){
        // LX-90D-TNUPの場合、ボルトカバー出力へ
        BLT_Cover_Out();
    // *************************
    }

    fclose(fp);

    return true;
}

// ---------------------------------------------------------------------------------------
// 日本語関数名： 部品型式　取得
// 概  要      ：
// 引  数      ： Code  :ﾌﾚｰﾑ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ(FRAMECD.csv)内ｺｰﾄﾞ
// 戻り値      ： 処理結果 取得した部品型式
// 備  考      ：
// ---------------------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_BuhinKatasiki(AnsiString Code) {
    AnsiString s_BKata;
    AnsiString s_JPKbn;
    int i_Pos;
    // 2019.11.22 CX-20型追加_S
     AnsiString s_Model;
    // 2019.11.22 CX-20型追加_E

    // 2003.09.16 部品型式変更
    // if (Code == "19" || Code == "20" || Code == "21"){
    // if ( P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),1) == "R" ) {
    // s_BKata = G_KS_Syurui + "−" + G_KS_Model  + "−" +  HanToZen(P_HEADS_DATA[36].SubString(1,P_HEADS_DATA[36].Length()-1));
    // } else {
    // s_BKata = G_KS_Syurui + "−" + G_KS_Model  + "−" +  HanToZen(P_HEADS_DATA[36].TrimRight());
    // }

    // } else if (Code == "14" || Code == "15" || Code == "16"){
    // i_Pos = P_HEADS_DATA[36].Pos("H");
    // if (i_Pos == 0){
    // i_Pos = P_HEADS_DATA[36].Pos("U");
    // if (i_Pos == 0){
    // i_Pos = P_HEADS_DATA[36].Pos("S");
    // }
    // }

    // if (i_Pos != 0){
    // s_JPKbn = P_HEADS_DATA[36].SubString(1, i_Pos-1)
    // + P_HEADS_DATA[36].SubString(i_Pos+1, P_HEADS_DATA[36].Length() );
    // } else {
    // s_JPKbn = P_HEADS_DATA[36].TrimRight();
    // }

    // s_BKata = G_KS_Syurui + "−" + G_KS_Model  + "−" +  HanToZen(s_JPKbn);

    // } else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" || Code == "26" || Code == "27" || Code == "28"){
    // s_BKata = P_FRAMECD[4];

    // } else {
    // s_BKata = G_KS_Syurui + "−" + G_KS_Model;
    // }

    // 2019.11.22 CX-20型追加_S
    //if (Code == "09" || Code == "14" || Code == "15" || Code == "16" ||
    //    Code == "17" || Code == "19" || Code == "20" || Code == "21") {
    //    if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
    //        s_BKata = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
    //    1) == "M") {
    //        s_BKata = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
    //        2) == "MR") {
    //        s_BKata = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 2));
    //    }
    //    else {
    //        s_BKata = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].TrimRight());
    //    }
    //}
    //// 2014/08/22 応用技術編集
    //else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" ||
    //         Code == "26" || Code == "27" || Code == "28" || Code == "75") {
    //    s_BKata = P_FRAMECD[4];
    //}
    //else {
    //    s_BKata = G_KS_Syurui + "−" + G_KS_Model;
    //}
    //// ***********************
    s_Model = G_KS_Model;

    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "２１") {
        s_Model ="２０";
    }
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "２３") {
        s_Model ="２０";
    }

    if (Code == "09" || Code == "14" || Code == "15" || Code == "16" ||
        Code == "17" || Code == "19" || Code == "20" || Code == "21") {
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            s_BKata = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
        1) == "M") {
            s_BKata = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
            2) == "MR") {
            s_BKata = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 2));
        }
        else {
            s_BKata = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].TrimRight());
        }
    }
    else if (Code == "18" || Code == "23" || Code == "24" || Code == "25" ||
             Code == "26" || Code == "27" || Code == "28" || Code == "75") {
        s_BKata = P_FRAMECD[4];
    }
    else {
        s_BKata = G_KS_Syurui + "−" + s_Model;
    }
    // 2019.11.22 CX-20型追加_E

    return s_BKata;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)検索  (ﾌﾚｰﾑ部品ﾃﾞｰﾀ作成用)
// 概  要      ：
// 引  数      ： Key ：検索ｷｰ
// 戻り値      ： 処理結果 [true:ﾃﾞｰﾀ有り false:ﾃﾞｰﾀ無し]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Search_HD_FRM_NZ_Buhin(AnsiString Key) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_Frame;
    AnsiString s_Parts_Code;
    AnsiString s_Text;
    bool b_Retry;

    int i_Length;

    // ﾌﾚｰﾑの右端文字ﾁｪｯｸ
    s_Frame = P_HEADS_DATA[36].TrimRight();

    i_Length = s_Frame.Length();
    s_Frame = s_Frame.SubString(i_Length, 1);
    if (s_Frame == "R") {
        s_Frame = P_HEADS_DATA[36].TrimRight();
        s_Frame = s_Frame.SetLength(i_Length - 1);
    }
    else {
        s_Frame = P_HEADS_DATA[36].TrimRight();
    }

    // 検索用部品ｺｰﾄﾞ取得
    if (P_FRAMECD[1].ToIntDef(0) != 9) {
        s_Parts_Code = AnsiString(P_FRAMECD[1].ToIntDef(0));
    }
    else {
        // E1ﾉｽﾞﾙ継手先頭一桁で分岐
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        // switch(s_Text.ToIntDef(0)) {
        // case 4: case 5: case 8: case 9:
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        switch (s_Text.ToIntDef(0)) {
        case 4:
        case 5:
        case 8:
        case 9:
        case 11:
        case 12:
        case 13:
        case 21:
        case 22:
        case 23:
            // ***********************
            s_Parts_Code = "109";
            break;
            // 2013/03/27 ﾉｽﾞﾙ規格追加
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48:
            s_Parts_Code = "109";
            break;
            // ***********************
        default:
            s_Parts_Code = "9";
            break;
        }
    }

    // ログ
    G_Log_Text = "ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀを検索ｷｰ ｢" + Key + "｣ ｢" + s_Frame + "｣ ｢" +
        P_FRAMECD[3] + "｣ ｢" + s_Parts_Code + "｣ で検索します。";
    Write_Log(G_Log_Text);

    P_SQL = "";
    P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
    P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'";
    P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'";
    P_SQL = P_SQL + "    AND PARTS_TYPE  = '" + P_FRAMECD[3] + "'";

    // 2006.12.21 ANSIの場合でも分岐させる
    // if ( s_Parts_Code == "9" ) {
    if ((s_Parts_Code == "9") || (s_Parts_Code == "109")) {
        // ***********************************
        b_Retry = true;
        // E1ﾉｽﾞﾙ継手先頭一桁で分岐
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // ***********************
        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // 孔が無い場合
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
                // 2013/03/27 ﾉｽﾞﾙ規格追加
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 9";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // 上(E1/E2のどちらか)に孔が有る場合
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
                // 2013/03/27 ﾉｽﾞﾙ規格追加
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 309";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 209";
                break;
            }
        }
        else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // 下(E3/E4のどちらか)に孔が有る場合
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
                // 2013/03/27 ﾉｽﾞﾙ規格追加
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 509";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 409";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0))
            && // 上下(E1/E2のどちらかと、E3/E4のどちらか)に孔が有る場合
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
                // 2013/03/27 ﾉｽﾞﾙ規格追加
            case 44:
            case 48: // ANSI/JPI
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
            case 11:
            case 12:
            case 13:
            case 14: // GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
            case 21:
            case 22:
            case 23:
            case 24: // DIN
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
            case 31:
            case 32:
            case 33:
            case 34: // EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 709";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 609";
                break;
            }
        }
    }
    else {
        b_Retry = false;
        P_SQL = P_SQL + "    AND PARTS_CODE = " + s_Parts_Code;
    }

    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(P_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        if (b_Retry == true) {
            G_ErrLog_Text =
                "検索ｷｰ『" + Key +
                "』は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しないので、孔無タイプで再検索します。";
            Write_Error_Log(G_ErrLog_Text);

            P_SQL = "";
            P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
            P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'";
            P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'";
            P_SQL = P_SQL + "    AND PARTS_TYPE  = '" + P_FRAMECD[3] + "'";
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
                P_SQL = P_SQL + "    AND PARTS_CODE = 109";
                break;
            default:
                P_SQL = P_SQL + "    AND PARTS_CODE = 9";
                break;
            }
            // 2021.06.18 FireDACへの移行 MOD_S
            //wkQuery->DatabaseName = ODBC_DSN_DNO;
            wkQuery->ConnectionName = ODBC_DSN_DNO;
            // 2021.06.18 FireDACへの移行 MOD_E
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(P_SQL);
            wkQuery->Open();
            wkQuery->First();
        }
    }
    if (wkQuery->Eof) {
        // ｴﾗｰﾛｸﾞ
        G_ErrLog_Text = "検索ｷｰ ｢" + Key + "｣ ｢" + s_Frame + "｣ ｢" +
            P_FRAMECD[3] + "｣ ｢" + s_Parts_Code +
            "｣ は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しません。";

        Write_Error_Log(G_ErrLog_Text);
        Write_Log(G_ErrLog_Text);

        // 図番
        P_DNO[1] = "";
        P_DNO[2] = "";
        P_DNO[3] = "";
        P_DNO[4] = "";
        P_DNO[5] = "";
        // 作図用図番
        P_WKDNO[1] = "";
        P_WKDNO[2] = "";
        P_WKDNO[3] = "";
        P_WKDNO[4] = "";
        P_WKDNO[5] = "";
        // 材質名称
        P_MatNM[1] = "";
        P_MatNM[2] = "";
        P_MatNM[3] = "";
        P_MatNM[4] = "";
        P_MatNM[5] = "";
        // 材質コード
        P_MatCD[1] = "";
        P_MatCD[2] = "";
        P_MatCD[3] = "";
        P_MatCD[4] = "";
        P_MatCD[5] = "";
        // 枚数
        P_PLATE_NUM[1] = 0;
        P_PLATE_NUM[2] = 0;
        P_PLATE_NUM[3] = 0;
        P_PLATE_NUM[4] = 0;
        P_PLATE_NUM[5] = 0;
        // 枚数
        P_PLATE_NUM_SUS[1] = 0;
        P_PLATE_NUM_SUS[2] = 0;
        P_PLATE_NUM_SUS[3] = 0;
        P_PLATE_NUM_SUS[4] = 0;
        P_PLATE_NUM_SUS[5] = 0;
        // SUS使用フラグ
        P_SUS = 0;

        delete wkQuery;
        return false;
    }
    else {
        // 材質名称
        P_MatNM[1] = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight();
        P_MatNM[2] = wkQuery->FieldByName("MAT_NAME2")->AsString.TrimRight();
        P_MatNM[3] = wkQuery->FieldByName("MAT_NAME3")->AsString.TrimRight();
        P_MatNM[4] = wkQuery->FieldByName("MAT_NAME4")->AsString.TrimRight();
        P_MatNM[5] = wkQuery->FieldByName("MAT_NAME5")->AsString.TrimRight();
        P_MatCD[1] = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_MatCD[2] = wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight();
        P_MatCD[3] = wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight();
        P_MatCD[4] = wkQuery->FieldByName("MAT_CODE4")->AsString.TrimRight();
        P_MatCD[5] = wkQuery->FieldByName("MAT_CODE5")->AsString.TrimRight();
        // 図番
        P_DNO[1] = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_DNO[2] = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_DNO[3] = wkQuery->FieldByName("DNO3")->AsString.TrimRight();
        P_DNO[4] = wkQuery->FieldByName("DNO4")->AsString.TrimRight();
        P_DNO[5] = wkQuery->FieldByName("DNO5")->AsString.TrimRight();
        // 作図用図番
        P_WKDNO[1] = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_WKDNO[2] = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_WKDNO[3] = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        P_WKDNO[4] = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        P_WKDNO[5] = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // 枚数
        P_PLATE_NUM[1] = wkQuery->FieldByName("PLATE_NUM1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[2] = wkQuery->FieldByName("PLATE_NUM2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[3] = wkQuery->FieldByName("PLATE_NUM3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[4] = wkQuery->FieldByName("PLATE_NUM4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[5] = wkQuery->FieldByName("PLATE_NUM5")
            ->AsString.ToIntDef(0);
        // 枚数(SUS用)
        P_PLATE_NUM_SUS[1] = wkQuery->FieldByName("HEIGHT1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[2] = wkQuery->FieldByName("HEIGHT2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[3] = wkQuery->FieldByName("HEIGHT3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[4] = wkQuery->FieldByName("HEIGHT4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[5] = wkQuery->FieldByName("HEIGHT5")
            ->AsString.ToIntDef(0);
        // SUS使用フラグ
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);

        G_Log_Text = "材質名称    『" + P_ZaisituNM + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番１      『" + P_DNO[1] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番２      『" + P_DNO[2] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番３      『" + P_DNO[3] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番４      『" + P_DNO[4] + "』を取得。";
        Write_Log(G_Log_Text);

        G_Log_Text = "作図用図番１『" + P_WKDNO[1] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番２『" + P_WKDNO[2] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番３『" + P_WKDNO[3] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番４『" + P_WKDNO[4] + "』を取得。";
        Write_Log(G_Log_Text);

        G_Log_Text = "枚数１      『" + AnsiString(P_PLATE_NUM[1]) + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "枚数２      『" + AnsiString(P_PLATE_NUM[2]) + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "枚数３      『" + AnsiString(P_PLATE_NUM[3]) + "』を取得。";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Eﾌﾚｰﾑﾃﾞｰﾀ作成処理 実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::E_Frame_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    // 2012.01.20 中国材質
    AnsiString s_ChinaM;
    // *******************

    // **********************************************
    // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)　検索
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索。";
    Write_Log(G_Log_Text);

    // seek_ret･･･ﾏｽﾀに存在したかしないか
    // Write_SE_Frame_Data()で使用する。
    seek_ret = Search_HD_FRM_NZ(P_Key, "E");

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索終了。";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  材質名称の検索(ASMEが含まれているか)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ASMEです。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("ＡＳＭＥ") != 0) {
            G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ＡＳＭＥです。";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  孔明仕様の作成
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("E");
    G_Log_Text = "孔明仕様  『" + P_Siyou1 + "』を取得。";
    Write_Log(G_Log_Text);

    // *************************
    // ***  材質の取得
    // *************************
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    // **********************************************
    // ***  図番・作図用図番の取得  02.09.03追加
    // **********************************************
    bSUS = false;
    // 2003.11.06　プレート材質データ位置変更
    // sPltMat = P_HEADS_DATA[160].TrimRight();
    sPltMat = P_HEADS_DATA[43].TrimRight();
    // **************************************
    // 2003.12.05 ASME PLATE
    // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
    // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
    // 2016.11.07 材質ｺｰﾄﾞ変更
    //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
    //    (sPltMat != "1032") && (sPltMat != "1033") && (sPltMat != "1057") &&
    //    (sPltMat != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
    //    (sPltMat != "1075") && (sPltMat != "1089") && (sPltMat != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11追加
    //    (sPltMat != "1076") &&
    //    // ***************************
    //    (sPltMat != "1085") && (sPltMat != "1095")) {
    //    // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
    //    // *********************
    //    bSUS = true;
    //}
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // プレート材質がチタン系以外
        bSUS = true;
    }
    // ***********************
    // 2003.11.05 枚数区分関係式＜を＜＝に変更
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        if (P_MatNM[2] != "") {
            P_ZaisituNM = P_MatNM[2];
            P_EZaisituNM = P_MatNM[2];
        }
        else if (P_MatCD[2] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        if (P_MatNM[3] != "") {
            P_ZaisituNM = P_MatNM[3];
            P_EZaisituNM = P_MatNM[3];
        }
        else if (P_MatCD[3] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        if (P_MatNM[4] != "") {
            P_ZaisituNM = P_MatNM[4];
            P_EZaisituNM = P_MatNM[4];
        }
        else if (P_MatCD[4] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "4";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        if (P_MatNM[5] != "") {
            P_ZaisituNM = P_MatNM[5];
            P_EZaisituNM = P_MatNM[5];
        }
        else if (P_MatCD[5] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "5";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }
    // ***************************************

    switch (P_HEADS_DATA[37].ToIntDef(0)) { // フレームコード
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // Ｂシリーズ
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
        // P_ZaisituNM = "ＳＵＳ３０４";
        // P_EZaisituNM = "ＳＵＳ３０４";
        //
        // } else {
        // P_ZaisituNM = "SS400/SUS304ｶﾊﾞｰﾘﾝｸﾞ";
        // P_EZaisituNM = "SS400/SUS304COVERING";
        // }

        break;
    // 2022.04.28 UX-30L追加_S
    // 2023.03.09 CX-00-Y追加_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y追加_E
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
    case 354: // Ｙシリーズ
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "Ｓ２７５ＪＲ";
            P_EZaisituNM = "Ｓ２７５ＪＲ";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    // 2022.04.28 UX-30L追加_E
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (s_Text == "0000" || s_Text == "1040") {
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
        // ***********************
            P_ZaisituNM = "ＳＳ４００";
            P_EZaisituNM = "ＳＳ４００";
        }
        else {
            // 2011.05.30 ﾌﾚｰﾑ材質指定変更
            // P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            // P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
            // ***************************
        }

        break;
    }

    // 2012.01.20 中国材質
    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text == "ＵＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−００") { // 中国材質
    }
    else if (s_Text == "ＬＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−２０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４１") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１１") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−７０") { // 中国材質
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // 中国材質
        // 2012.02.07 中国ﾌﾚｰﾑ材質
        // if ( P_ZaisituNM == "ＳＳ４００" ) {
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (s_Text == "1040") {
        if (Type_MAT(s_Text) == "SS400") {
        // ***********************
        }
        else if (P_ZaisituNM == "ＳＳ４００" || P_ZaisituNM == "Ｃ．Ｓ．") {
            // ***********************
            P_ZaisituNM = "Ｑ２３５Ｂ";
            P_EZaisituNM = "Ｑ２３５Ｂ";
        }
    }
    // *******************

    // 2008.08.21 WX-50ﾌﾚｰﾑ図番設定条件追加
    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text == "ＷＸ−５３") {
        // WHC=0,REVE=0 Ｂ側溶接
        if ((P_HEADS_DATA[243].ToIntDef(0) == 0) && (P_HEADS_DATA[137].ToIntDef
            (0) == 0)) {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            if (P_MatNM[2] != "") {
                P_ZaisituNM = P_MatNM[2];
                P_EZaisituNM = P_MatNM[2];
            }
            else if (P_MatCD[2] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
            }
            else if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "2";
            // WHC=0,REVE=1 Ａ側溶接
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "1";
            // WHC=1,REVE=0 Ａ側溶接
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "1";
            // WHC=1,REVE=1 Ｂ側溶接
        }
        else {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            if (P_MatNM[2] != "") {
                P_ZaisituNM = P_MatNM[2];
                P_EZaisituNM = P_MatNM[2];
            }
            else if (P_MatCD[2] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
            }
            else if (P_MatNM[1] != "") {
                P_ZaisituNM = P_MatNM[1];
                P_EZaisituNM = P_MatNM[1];
            }
            else if (P_MatCD[1] != "") {
                P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
                P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
            }
            P_Siyou3 = "2";
        }
    }
    // ************************************

    // *************************
    // ***  予備数の取得
    // *************************
    Syukei_Frame_Yobisu("E");

    // *************************
    // ***  Eﾌﾚｰﾑﾃﾞｰﾀの出力
    // *************************
    Write_SE_Frame_Data("E", seek_ret);

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Sﾌﾚｰﾑﾃﾞｰﾀ作成処理 実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::S_Frame_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    // 2012.01.20 中国材質
    AnsiString s_ChinaM;
    // *******************

    // **********************************************
    // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)　検索
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索。";
    Write_Log(G_Log_Text);

    // seek_ret･･･ﾏｽﾀに存在したかしないか
    // ※注 Write_SE_Frame_Data()で使用する。
    seek_ret = Search_HD_FRM_NZ(P_Key, "S");

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索終了。";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  材質名称の検索(ASMEが含まれているか)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ASMEです。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("ＡＳＭＥ") != 0) {
            G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ＡＳＭＥです。";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  孔明仕様の作成
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("S");
    G_Log_Text = "孔明仕様  『" + P_Siyou1 + "』を取得。";
    Write_Log(G_Log_Text);

    // *************************
    // ***  材質の取得
    // *************************
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    switch (P_HEADS_DATA[37].ToIntDef(0)) {
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // Ｂシリーズ
        // s_Text = P_HEADS_DATA[34].TrimRight();
        // if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
        // P_ZaisituNM = "ＳＵＳ３０４";
        // P_EZaisituNM = "ＳＵＳ３０４";
        //
        // } else {
        // P_ZaisituNM = "SS400/SUS304ｶﾊﾞｰﾘﾝｸﾞ";
        // P_EZaisituNM = "SS400/SUS304COVERING";
        // }
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2022.04.28 UX-30L追加_S
    // 2023.03.09 CX-00-Y追加_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y追加_E
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
    case 354: // Ｙシリーズ
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "Ｓ２７５ＪＲ";
            P_EZaisituNM = "Ｓ２７５ＪＲ";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    // 2022.04.28 UX-30L追加_E
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (s_Text == "0000" || s_Text == "1040") {
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
        // ***********************
            P_ZaisituNM = "ＳＳ４００";
            P_EZaisituNM = "ＳＳ４００";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    // 2012.01.20 中国材質
    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text == "ＵＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−００") { // 中国材質
    }
    else if (s_Text == "ＬＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−２０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４１") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１１") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−７０") { // 中国材質
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // 中国材質
        // 2012.02.07 中国ﾌﾚｰﾑ材質
        // if ( P_ZaisituNM == "ＳＳ４００" ) {
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        // 2016.11.07 材質ｺｰﾄﾞ変更
        //if (s_Text == "1040") {
        if (Type_MAT(s_Text) == "SS400") {
        // ***********************
        }
        else if (P_ZaisituNM == "ＳＳ４００" || P_ZaisituNM == "Ｃ．Ｓ．") {
            // ***********************
            P_ZaisituNM = "Ｑ２３５Ｂ";
            P_EZaisituNM = "Ｑ２３５Ｂ";
        }
    }
    // *******************

    // **********************************************
    // ***  図番・作図用図番の取得  02.09.03追加
    // **********************************************
    bSUS = false;
    // 2003.11.06　プレート材質データ位置変更
    // sPltMat = P_HEADS_DATA[160].TrimRight();
    sPltMat = P_HEADS_DATA[43].TrimRight();
    // **************************************
    // 2003.12.05 ASME PLATE
    // if ((P_SUS != 0 ) && (sPltMat != "1014" ) && ( sPltMat != "1015" ) &&
    // ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
    // 2016.11.07 材質ｺｰﾄﾞ変更
    //if ((P_SUS != 0) && (sPltMat != "1014") && (sPltMat != "1015") &&
    //    (sPltMat != "1032") && (sPltMat != "1033") && (sPltMat != "1057") &&
    //    (sPltMat != "1058") &&
    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
    //    (sPltMat != "1075") && (sPltMat != "1089") && (sPltMat != "1090") &&
    //    // *************************************************
    //    // 2010.12.02 SB-265-Gr.11追加
    //    (sPltMat != "1076") &&
    //    // ***************************
    //    (sPltMat != "1085") && (sPltMat != "1095")) {
    //    // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
    //    // *********************
    //    bSUS = true;
    //}
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // プレート材質がチタン系以外
        bSUS = true;
    }
    // ***********************
    // 2003.11.05 枚数区分関係式＜を＜＝に変更
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        P_Siyou3 = "3";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }
    // ***************************************

    // 2008.08.21 WX-50ﾌﾚｰﾑ図番設定条件追加
    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text == "ＷＸ−５３") {
        // WHC=0,REVE=0 Ｂ側溶接
        if ((P_HEADS_DATA[243].ToIntDef(0) == 0) && (P_HEADS_DATA[137].ToIntDef
            (0) == 0)) {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            P_Siyou3 = "2";
            // WHC=0,REVE=1 Ａ側溶接
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) != 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            P_Siyou3 = "1";
            // WHC=1,REVE=0 Ａ側溶接
        }
        else if ((P_HEADS_DATA[243].ToIntDef(0) != 0) &&
            (P_HEADS_DATA[137].ToIntDef(0) == 0)) {
            P_Zuban = P_DNO[1];
            P_SZuban = P_WKDNO[1];
            P_Siyou3 = "1";
            // WHC=1,REVE=1 Ｂ側溶接
        }
        else {
            P_Zuban = P_DNO[2];
            P_SZuban = P_WKDNO[2];
            P_Siyou3 = "2";
        }
    }
    // ************************************

    // *************************
    // ***  予備数の取得
    // *************************
    Syukei_Frame_Yobisu("S");

    // *************************
    // ***  Sﾌﾚｰﾑﾃﾞｰﾀの出力
    // *************************
    Write_SE_Frame_Data("S", seek_ret);

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------------------
// 日本語関数名： S,Eﾌﾚｰﾑﾃﾞｰﾀ　書き込み
// 概  要      ：
// 引  数      ： Key ：検索ｷｰ
// ： Kbn ：S･･･Sﾌﾚｰﾑ用処理，E･･･Eﾌﾚｰﾑ処理
// ： Seek_Result:ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ検索結果(true:存在,false:存在しない)
// 戻り値      ： 処理結果 [true:ﾃﾞｰﾀ有り false:ﾃﾞｰﾀ無し]
// 備  考      ：
// ---------------------------------------------------------------------------------------
bool __fastcall TFrameDetails::Write_SE_Frame_Data(AnsiString Kbn,
    bool Seek_Result) {
    AnsiString s_Text;
    AnsiString wk_FilePass;
    // 2019.11.22 CX-20型追加_S
    AnsiString s_Model;
    // 2019.11.22 CX-20型追加_E

    long l_Daisu;

    // ------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // ****************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // ------------------------------------------

    // ****************************
    // ***  変数セット
    // ****************************
    // 部品分類
    P_BBunrui = "FRAME";
    // 識別子
    if (Kbn == "S") {
        P_Sikibetu = "FRAM-S";
    }
    // 2022.05.23 フレームガス切り追加_S
    else if (Kbn == "SG") {
        P_Sikibetu = "FRAMSG";
    }
    else if (Kbn == "EG") {
        P_Sikibetu = "FRAMEG";
    }
    // 2022.05.23 フレームガス切り追加_E
    else {
        P_Sikibetu = "FRAM-E";
    }
    // 部品型式
    // 2019.11.22 CX-20型追加_S
    //if (Seek_Result == true) {
    //    // 2003.09.10 部品型式変更
    //    // if( P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) ==  "R" ) {
    //    // P_BKatasiki = G_KS_Syurui + "−" + G_KS_Model + "−" + HanToZen(P_HEADS_DATA[36].SubString(1,P_HEADS_DATA[36].Length()-1));
    //    // } else {
    //    // P_BKatasiki = G_KS_Syurui + "−" + G_KS_Model + "−" + HanToZen(P_HEADS_DATA[36].TrimRight());
    //    // }
    //    if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
    //        P_BKatasiki = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
    //    1) == "M") {
    //        P_BKatasiki = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 1));
    //    }
    //    else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
    //        2) == "MR") {
    //        P_BKatasiki = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].SubString(1,
    //            P_HEADS_DATA[36].Length() - 2));
    //    }
    //    else {
    //        P_BKatasiki = G_KS_Syurui + "−" + G_KS_Model + "−" +
    //            HanToZen(P_HEADS_DATA[36].TrimRight());
    //    }
    //    // ***********************
    //}
    //else {
    //    P_BKatasiki = "";
    //}
    s_Model = G_KS_Model;

    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "２１") {
        s_Model ="２０";
    }
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "２３") {
        s_Model ="２０";
    }

    if (Seek_Result == true) {
        if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(), 1) == "R") {
            P_BKatasiki = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length(),
        1) == "M") {
            P_BKatasiki = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 1));
        }
        else if (P_HEADS_DATA[36].SubString(P_HEADS_DATA[36].Length() - 1,
            2) == "MR") {
            P_BKatasiki = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].SubString(1,
                P_HEADS_DATA[36].Length() - 2));
        }
        else {
            P_BKatasiki = G_KS_Syurui + "−" + s_Model + "−" +
                HanToZen(P_HEADS_DATA[36].TrimRight());
        }
    }
    else {
        P_BKatasiki = "";
    }
    // 2019.11.22 CX-20型追加_E

    // 部品名称
    if (Kbn == "S") {
        P_BMeisyou = "Ｓ．フレーム";
    }
    // 2022.05.23 フレームガス切り追加_S
    else if (Kbn == "SG") {
        P_BMeisyou = "Ｓ．フレーム（ガス溶断）";
    }
    else if (Kbn == "EG") {
        P_BMeisyou = "Ｅ．フレーム（ガス溶断）";
    }
    // 2022.05.23 フレームガス切り追加_E
    else {
        P_BMeisyou = "Ｅ．フレーム";
    }
    // １台分数量
    if (Seek_Result == true) {
        P_1DaiSuryo = 1;
    }
    else {
        P_1DaiSuryo = 0;
    }

    // 総計
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
    P_Soukei = P_1DaiSuryo * l_Daisu + P_Yobisu;

    // 仕様１
    if (Seek_Result == true) {
        P_Siyou1 = "孔明" + P_Siyou1;
    }
    else {
        P_Siyou1 = "";
    }

    // 仕様２(S,Eﾌﾚｰﾑ種類)
    if (Seek_Result == true) {
        if (Kbn == "S") {
            P_Siyou2 = P_HEADS_DATA[1296].TrimRight(); // Sﾌﾚｰﾑ種類
        }
        else {
            P_Siyou2 = P_HEADS_DATA[1297].TrimRight(); // Eﾌﾚｰﾑ種類
        }
    }
    else {
        P_Siyou2 = "";
    }

    // 仕様４
    P_Siyou4 = "";
    // 仕様５
    P_Siyou5 = "";

    // ****************************
    // ***  S,Eﾌﾚｰﾑﾃﾞｰﾀ  セット
    // ****************************
    // 部品展開ﾃﾞｰﾀ　OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  部品分類
    s_Text = P_BBunrui;
    fputs(s_Text.c_str(), fp);
    // 2  識別子
    s_Text = "," + P_Sikibetu;
    fputs(s_Text.c_str(), fp);
    // 3  部品型式
    s_Text = "," + P_BKatasiki;
    fputs(s_Text.c_str(), fp);
    // 4  部品名称
    s_Text = "," + P_BMeisyou;
    fputs(s_Text.c_str(), fp);
    // 5  材質名称
    s_Text = "," + P_ZaisituNM;
    fputs(s_Text.c_str(), fp);
    // 6  図番
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  作図用図番
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1台分数量
    s_Text = "," + AnsiString(P_1DaiSuryo);
    fputs(s_Text.c_str(), fp);
    // 9  予備数
    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), fp);

    // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);

    // 11 仕様１
    s_Text = "," + P_Siyou1;
    fputs(s_Text.c_str(), fp);
    // 12 仕様２
    s_Text = "," + P_Siyou2;
    fputs(s_Text.c_str(), fp);
    // 13 仕様３
    s_Text = "," + P_Siyou3;
    fputs(s_Text.c_str(), fp);
    // 14 仕様４
    s_Text = "," + P_Siyou4;
    fputs(s_Text.c_str(), fp);
    // 15 仕様５
    s_Text = "," + P_Siyou5;
    fputs(s_Text.c_str(), fp);
    // 16  材質名称(英)
    s_Text = "," + P_EZaisituNM;
    fputs(s_Text.c_str(), fp);

    // ------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), fp);
    // 2007.10.03
    // ------------------------------

    // 改行文字
    s_Text = "\n";
    // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
    fputs(s_Text.c_str(), fp);

    fclose(fp);

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)　検索
// 概  要      ：
// 引  数      ： Key ：検索ｷｰ
// ： Kbn ：S･･･Sﾌﾚｰﾑ用処理，E･･･Eﾌﾚｰﾑ処理
// 戻り値      ： 処理結果 [true:ﾃﾞｰﾀ有り false:ﾃﾞｰﾀ無し]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::Search_HD_FRM_NZ(AnsiString Key, AnsiString Kbn)
{
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_Frame;
    AnsiString s_Text;
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    AnsiString s_Text2;
    // ***********************

    int i_Length;
    bool E1Hol, E2Hole, E3Hole, E4Hole;

    // ﾌﾚｰﾑの右端文字ﾁｪｯｸ
    s_Frame = P_HEADS_DATA[36].TrimRight();

    i_Length = s_Frame.Length();
    s_Frame = s_Frame.SubString(i_Length, 1);
    if (s_Frame == "R") {
        s_Frame = P_HEADS_DATA[36].TrimRight();
        s_Frame = s_Frame.SetLength(i_Length - 1);
    }
    else {
        s_Frame = P_HEADS_DATA[36].TrimRight();
    }

    P_SQL = "";
    P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
    P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ﾌﾟﾚｰﾄ型式
    P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ﾌﾚｰﾑ型式

    if (Kbn == "S") {
        P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

        // S1ﾉｽﾞﾙ継手先頭一桁で分岐
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // s_Text = P_HEADS_DATA[893].SubString(1,1);
        s_Text = P_HEADS_DATA[893].SubString(1, 2);
        // 2009.02.16 ﾉｽﾞﾙ継手条件変更
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[908].SubString(1,1);
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[923].SubString(1,1);
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[938].SubString(1,1);
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }

        switch (s_Text.ToIntDef(0)) {
            // case 4: case 5: case 8: case 9:
        case 4:
        case 5:
        case 8:
        case 9:
        case 11:
        case 12:
        case 13:
        case 21:
        case 22:
        case 23: // ANSI/JPI/DIN/GB
            P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            // 2013/03/27 ﾉｽﾞﾙ規格追加
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48: // ANSI/JPI/DIN/GB/EN
            P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            // ***********************
        default:
            P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
        }
        // ***************************

    }
    else if (Kbn == "E") {
        P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

        // E1ﾉｽﾞﾙ継手先頭一桁で分岐
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // s_Text = P_HEADS_DATA[953].SubString(1,1);
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // 2009.02.16 ﾉｽﾞﾙ継手条件変更
        // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[908].SubString(1,1);
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[923].SubString(1,1);
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ﾉｽﾞﾙ継手先頭一桁で分岐
            // s_Text = P_HEADS_DATA[938].SubString(1,1);
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }
        // ***************************

        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // 孔が無い場合
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // case 4: case 5: case 8: case 9:
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // 上(E1/E2のどちらか)に孔が有る場合
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // case 4: case 5: case 8: case 9:                     // ANSI/JPI
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 308";
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 308";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 208";
                break;
            }
        }
        else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // 下(E3/E4のどちらか)に孔が有る場合
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // case 4: case 5: case 8: case 9:                     // ANSI/JPI
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 508";
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 508";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 408";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0))
            && // 上下(E1/E2のどちらかと、E3/E4のどちらか)に孔が有る場合
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // case 4: case 5: case 8: case 9:                     // ANSI/JPI
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 708";
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 708";
                break;
                // ***********************
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 608";
                break;
            }
        }
        // switch(s_Text.ToIntDef(0)) {
        // case 4: case 5: case 8: case 9:
        // P_SQL = P_SQL + "    AND PARTS_CODE = 108";
        // break;
        // default:
        // P_SQL = P_SQL + "    AND PARTS_CODE = 8";
        // break;
        // }
    }
    // 2022.05.23 フレームガス切り追加_S
    else if (Kbn == "SG") {
        P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

        // S1ﾉｽﾞﾙ継手先頭一桁で分岐
        s_Text = P_HEADS_DATA[893].SubString(1, 2);
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }

        switch (s_Text.ToIntDef(0)) {
        case 4:
        case 5:
        case 8:
        case 9:
        case 11:
        case 12:
        case 13:
        case 21:
        case 22:
        case 23: // ANSI/JPI/DIN/GB
            P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
        case 14:
        case 24:
        case 31:
        case 32:
        case 33:
        case 34:
        case 44:
        case 48: // ANSI/JPI/DIN/GB/EN
            P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
        default:
            P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
        }

    }
    else if (Kbn == "EG") {
        P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

        // E1ﾉｽﾞﾙ継手先頭一桁で分岐
        s_Text = P_HEADS_DATA[953].SubString(1, 2);
        // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
        if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
            // S1ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
            // S2ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[908].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
            // S3ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[923].SubString(1, 2);
        }
        else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
            (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
            // S4ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[938].SubString(1, 2);
        }

        if ((P_HEADS_DATA[951].ToIntDef(0) == 0) && (P_HEADS_DATA[966].ToIntDef
            (0) == 0) && // 孔が無い場合
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 188";
                break;
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 188";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 88";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // 上(E1/E2のどちらか)に孔が有る場合
            (P_HEADS_DATA[981].ToIntDef(0) == 0) && (P_HEADS_DATA[996].ToIntDef
            (0) == 0)) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 388";
                break;
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 388";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 288";
                break;
            }
        }
        else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
            (P_HEADS_DATA[966].ToIntDef(0) == 0) && // 下(E3/E4のどちらか)に孔が有る場合
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ﾉｽﾞﾙ規格変更
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 588";
                break;
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 588";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 488";
                break;
            }
        }
        else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[966].ToIntDef(0) != 0))
            && // 上下(E1/E2のどちらかと、E3/E4のどちらか)に孔が有る場合
            ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
            (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                P_SQL = P_SQL + "    AND PARTS_CODE = 788";
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 788";
                break;
            default: // JIS
                P_SQL = P_SQL + "    AND PARTS_CODE = 688";
                break;
            }
        }
    }
    // 2022.05.23 フレームガス切り追加_E

    // 2009.02.16 RX-50の場合DIN,#150をJISとして扱う
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // s_Text = G_KS_Syurui + "−" + G_KS_Model;
    // if (s_Text == "ＲＸ−５０") {
    s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text2 == "ＲＸ−５０") {
        // ***********************

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ﾌﾟﾚｰﾄ型式
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ﾌﾚｰﾑ型式

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            // ***********************

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            // 2021.09.22 RX-50Z追加_S
            //case 13:
            // 2021.09.22 RX-50Z追加_E
            case 14:
            // 2021.09.22 RX-50Z追加_S
            //case 23:
            // 2021.09.22 RX-50Z追加_E
            case 24:
            // 2021.09.22 RX-50Z追加_S
            //case 33:
            // 2021.09.22 RX-50Z追加_E
            case 34:
            case 41:
            case 44:
            case 48:
                // 2021.09.22 RX-50Z追加_S
                //// JIS30K,PN25/40,#400(ﾌﾚｰﾑなし)
                // JIS30K,PN40,#400(ﾌﾚｰﾑなし)
                // 2021.09.22 RX-50Z追加_E
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // ***********************
            // 2021.09,22 RX-50Z追加_S
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Zﾌﾚｰﾑ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                } else {
                    // Zﾌﾚｰﾑ以外 PN25(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                }
                break;
            // 2021.09.22 RX-50Z追加_E
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            }

        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = P_HEADS_DATA[953].SubString(1,1);
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            // ***********************

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            // 2021.09.22 RX-50Z追加_S
            //case 13:
            // 2021.09.22 RX-50Z追加_E
            case 14:
            // 2021.09.22 RX-50Z追加_S
            //case 23:
            // 2021.09.22 RX-50Z追加_E
            case 24:
            // 2021.09.22 RX-50Z追加_S
            //case 33:
            // 2021.09.22 RX-50Z追加_E
            case 34:
            case 41:
            case 44:
            case 48:
                // 2021.09.22 RX-50Z追加_S
                //// JIS30K,PN25/40,#400(ﾌﾚｰﾑなし)
                // JIS30K,PN40,#400(ﾌﾚｰﾑなし)
                // 2021.09.22 RX-50Z追加_E
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // ***********************
            // 2021.09.22 RX-50Z追加_S
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Zﾌﾚｰﾑ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                } else {
                    // Zﾌﾚｰﾑ以外 PN25(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                }
                break;
            // 2021.09.22 RX-50Z追加_E
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            }
        }
        // 2022.05.23 フレームガス切り追加_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
            case 14:
            case 24:
            case 34:
            case 41:
            case 44:
            case 48:
                // JIS30K,PN40,#400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Zﾌﾚｰﾑ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                } else {
                    // Zﾌﾚｰﾑ以外 PN25(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                }
                break;
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            }

        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 5:
            case 9: // #300
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 14:
            case 24:
            case 34:
            case 41:
            case 44:
            case 48:
                // JIS30K,PN40,#400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 13:
            case 23:
            case 33:
                if(P_HEADS_DATA[36].Pos("Z") > 0) {
                    // Zﾌﾚｰﾑ PN25
                    P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                } else {
                    // Zﾌﾚｰﾑ以外 PN25(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                }
                break;
            default: // JIS,DIN,#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            }
        }
        // 2022.05.23 フレームガス切り追加_E
    }
    // *********************************************

    // 2010.08.27 LX-10-Sの場合JIS16K,20Kのﾌﾚｰﾑﾉｽﾞﾙ種別を追加する
    // 2011.06.20 ﾉｽﾞﾙ規格変更
    // s_Text = G_KS_Syurui + "−" + G_KS_Model;
    // if (s_Text == "ＬＸ−１０") {
    s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text2 == "ＬＸ−１０") {
        // ***********************

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ﾌﾟﾚｰﾄ型式
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ﾌﾚｰﾑ型式

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = P_HEADS_DATA[893].SubString(1,1);
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
                // case 2: case 3:                                    // JIS16K,20K
            case 2:
            case 3:
            case 12:
            case 22: // JIS16K,20K/GB1.6/DN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // 2013.04.03 ﾉｽﾞﾙ規格追加
                ////case 4: case 5: case 8: case 9:                    // ANSI/JPI/DIN
                // case 4: case 5: case 8: case 9:                    // ANSI/JPI
            case 4:
            case 8: // #150
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 32: // EN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 201"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34:
                // PN25/40(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // ***********************
                // default:                                           // JIS10K
            default: // JIS10K/GB1.0/DN10
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            }
            // ***********************

        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = P_HEADS_DATA[953].SubString(1,1);
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[893].SubString(1,1);
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[908].SubString(1,1);
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[923].SubString(1,1);
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                // s_Text = P_HEADS_DATA[938].SubString(1,1);
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // 孔が無い場合
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ﾉｽﾞﾙ規格追加
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // 上(E1/E2のどちらか)に孔が有る場合
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ﾉｽﾞﾙ規格追加
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 308";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 308"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 308"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 208";
                    break;
                }
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // 下(E3/E4のどちらか)に孔が有る場合
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ﾉｽﾞﾙ規格追加
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 508";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 508"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 508"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 408";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0))
                && // 上下(E1/E2のどちらかと、E3/E4のどちらか)に孔が有る場合
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                    // 2013.04.03 ﾉｽﾞﾙ規格追加
                    ////case 4: case 5: case 8: case 9:                     // ANSI/JPI/DIN
                    // case 4: case 5: case 8: case 9: case 11: case 12: case 21: case 22: // ANSI/JPI/DIN/GB
                    // P_SQL = P_SQL + "    AND PARTS_CODE = 708";
                    // break;
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 708"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 708"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                    // ***********************
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 608";
                    break;
                }
            }
            // ***********************
        }
        // 2022.05.23 フレームガス切り追加_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2:
            case 3:
            case 12:
            case 22: // JIS16K,20K/GB1.6/DN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 4:
            case 8: // #150
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 32: // EN16
                P_SQL = P_SQL + "    AND PARTS_CODE = 281"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34:
                // PN25/40(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K/GB1.0/DN10
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            }

        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }
            if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // 孔が無い場合
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 88";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0)) && // 上(E1/E2のどちらか)に孔が有る場合
                (P_HEADS_DATA[981].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[996].ToIntDef(0) == 0)) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 388"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 388"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 288";
                    break;
                }
            }
            else if ((P_HEADS_DATA[951].ToIntDef(0) == 0) &&
                (P_HEADS_DATA[966].ToIntDef(0) == 0) && // 下(E3/E4のどちらか)に孔が有る場合
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 588"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 588"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 488";
                    break;
                }
            }
            else if (((P_HEADS_DATA[951].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[966].ToIntDef(0) != 0))
                && // 上下(E1/E2のどちらかと、E3/E4のどちらか)に孔が有る場合
                ((P_HEADS_DATA[981].ToIntDef(0) != 0) ||
                (P_HEADS_DATA[996].ToIntDef(0) != 0))) {
                switch (s_Text.ToIntDef(0)) {
                case 2:
                case 3:
                case 4:
                case 8: // JIS16K,20K,#150
                    P_SQL = P_SQL + "    AND PARTS_CODE = 788"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 11:
                case 12:
                case 21:
                case 22:
                case 31:
                case 32:
                    // GB,DIN,EN10/16
                    P_SQL = P_SQL + "    AND PARTS_CODE = 788"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 5:
                case 9:
                case 41:
                case 44:
                case 48: // JIS30K,#300,400(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                case 13:
                case 14:
                case 23:
                case 24:
                case 33:
                case 34:
                    // PN25/40(ﾌﾚｰﾑなし)
                    P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                    break;
                default: // JIS
                    P_SQL = P_SQL + "    AND PARTS_CODE = 688";
                    break;
                }
            }
        }
        // 2022.05.23 フレームガス切り追加_E
    }
    // **********************************************************

    // 2018.06.19 LX-30Z追加_S
    // JIS10K
    //   Ｓフレーム："1"
    //   Ｅフレーム："8"
    //   Ｅローラー："9"　　　
    // ANSI#150/JPI#150/DIN/GB/DIN
    //   Ｓフレーム："1"
    //   Ｅフレーム："8"
    //   Ｅローラー："109"　　　
    // JIS16K
    //   Ｓフレーム："101"　　　
    //   Ｅフレーム："108"
    //   Ｅローラー："109"　
    // JIS20K,30K/ANSI#300,400/JPI#300,400(ﾌﾚｰﾑなし)　
    //   Ｓフレーム："999"　　　
    //   Ｅフレーム："999"
    //   Ｅローラー："9"or"109"　
    s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
    if ((s_Text2 == "ＬＸ−３０") && ( P_HEADS_DATA[36].Pos("Z") > 0)) {
        // ＬＸ−３０−Ｚ

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ﾌﾟﾚｰﾄ型式
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ﾌﾚｰﾑ型式

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        // 2022.05.23 フレームガス切り追加_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 2: // JIS16K
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 4:
            case 8:
            case 11:
            case 12:
            case 13:
            case 14:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34: // ANSI#150/JPI#150/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 3:
            case 5:
            case 9:
            case 41:
            case 44:
            case 48: // JIS20K,30K/ANSI#300,400/JPI#300,400(ﾌﾚｰﾑなし)
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        // 2022.05.23 フレームガス切り追加_E
    }
    // 2018.06.19 LX-30Z追加_E

    // 2019.11.08 CX-23追加_S
    // JIS10K,16K,20K/DIN10,16/GB10,16/EN10,16
    //   Ｓフレーム："1"
    //   Ｅフレーム："8"
    //   Ｅローラー："9"　　　
    // ANSI#150/JPI#150
    //   Ｓフレーム："101"
    //   Ｅフレーム："108"
    //   Ｅローラー："109"　　　
    // JIS30K/ANSI#300,400/JPI#300,400/DIN25,40/GB25,40/EN25,40 (ﾌﾚｰﾑなし)　
    //   Ｓフレーム："999"　　　
    //   Ｅフレーム："999"
    //   Ｅローラー："9"or"109"　
    s_Text2 = G_KS_Syurui + "−" + G_KS_Model;
    // 2022.04.27 CX-21追加_S
    //if (s_Text2 == "ＣＸ−２３") {
    //    // ＣＸ−２３
    if ((s_Text2 == "ＣＸ−２１") || (s_Text2 == "ＣＸ−２３")) {
        // ＣＸ−２１、ＣＸ−２３
    // 2022.04.27 CX-21追加_E

        P_SQL = "";
        P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
        P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ﾌﾟﾚｰﾄ型式
        P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ﾌﾚｰﾑ型式

        if (Kbn == "S") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 101"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40　
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 1"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        else if (Kbn == "E") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40　
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 8"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        // 2022.05.23 フレームガス切り追加_S
        else if (Kbn == "SG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE  = 'S'";

            // S1ﾉｽﾞﾙ継手先頭一桁で分岐(ﾃﾞｨﾌｫﾙﾄ)
            s_Text = P_HEADS_DATA[893].SubString(1, 2);
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 181"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40　
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 81"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        else if (Kbn == "EG") {
            P_SQL = P_SQL + "    AND PARTS_TYPE = 'E'";

            // E1ﾉｽﾞﾙ継手先頭一桁で分岐
            s_Text = P_HEADS_DATA[953].SubString(1, 2);
            // 継手種類はEﾌﾚｰﾑもSﾉｽﾞﾙで判断する
            if ((P_HEADS_DATA[891].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[891].ToIntDef(0) == 2)) {
                // S1ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[893].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[906].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[906].ToIntDef(0) == 2)) {
                // S2ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[908].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[921].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[921].ToIntDef(0) == 2)) {
                // S3ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[923].SubString(1, 2);
            }
            else if ((P_HEADS_DATA[936].ToIntDef(0) == 1) ||
                (P_HEADS_DATA[936].ToIntDef(0) == 2)) {
                // S4ﾉｽﾞﾙ継手先頭一桁で分岐
                s_Text = P_HEADS_DATA[938].SubString(1, 2);
            }

            switch (s_Text.ToIntDef(0)) {
            case 4:
            case 8: // ANSI#150/JPI#150
                P_SQL = P_SQL + "    AND PARTS_CODE = 188"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            case 41:
            case 5:
            case 44:
            case 9:
            case 48:
            case 13:
            case 14:
            case 23:
            case 24:
            case 33:
            case 34: // JIS30K/ANSI#300,400/JPI#300,400/GB25,40/DIN25,40/EN25,40　
                P_SQL = P_SQL + "    AND PARTS_CODE = 999"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
            default: // JIS10K,16K,20K/GB10,16/DIN10,16/EN10,16
                P_SQL = P_SQL + "    AND PARTS_CODE = 88"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
            break;
            }
        }
        // 2022.05.23 フレームガス切り追加_E
    }
    // 2019.11.08 CX-23追加_E

    // 2022.05.23 フレームガス切り追加_S
    G_Log_Text = "検索ｷｰ 『" + P_SQL + "』で検索します。";
    Write_Log(G_Log_Text);
    // 2022.05.23 フレームガス切り追加_E

    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(P_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        if (Kbn == "E") { // Eフレームの場合は、孔明条件で該当するレコードが無い場合、孔無しで再検索する
            G_ErrLog_Text =
                "検索ｷｰ『" + Key +
                "』は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しないので、孔無タイプで再検索します。";
            Write_Error_Log(G_ErrLog_Text);

            P_SQL = "";
            P_SQL = P_SQL + "SELECT * FROM HD_FRM_NZ";
            P_SQL = P_SQL + "  WHERE PLATE_MODEL = '" + Key + "'"; // ﾌﾟﾚｰﾄ型式
            P_SQL = P_SQL + "    AND FRAME_MODEL = '" + s_Frame + "'"; // ﾌﾚｰﾑ型式
            switch (s_Text.ToIntDef(0)) {
                // 2011.06.20 ﾉｽﾞﾙ規格変更
                // case 4: case 5: case 8: case 9:
            case 4:
            case 5:
            case 8:
            case 9:
            case 11:
            case 12:
            case 13:
            case 21:
            case 22:
            case 23: // ANSI/JPI/DIN/GB
                // ***********************
                P_SQL = P_SQL + "    AND PARTS_CODE = 108";
                break;
                // 2013/03/27 ﾉｽﾞﾙ規格追加
            case 14:
            case 24:
            case 31:
            case 32:
            case 33:
            case 34:
            case 44:
            case 48: // ANSI/JPI/DIN/GB/EN
                P_SQL = P_SQL + "    AND PARTS_CODE = 108"; // ﾌﾚｰﾑﾉｽﾞﾙ種別
                break;
                // ***********************
            default:
                P_SQL = P_SQL + "    AND PARTS_CODE = 8";
                break;
            }
            // 2021.06.18 FireDACへの移行 MOD_S
            //wkQuery->DatabaseName = ODBC_DSN_DNO;
            wkQuery->ConnectionName = ODBC_DSN_DNO;
            // 2021.06.18 FireDACへの移行 MOD_E
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(P_SQL);
            wkQuery->Open();
            wkQuery->First();
        }
        // 2022.05.23 フレームガス切り追加_S
        else if (Kbn == "SG" || Kbn == "EG") {
            // SEフレームガス切り図番なしの場合は、SEフレームと同じ図番（展開しない）
            // ｴﾗｰﾛｸﾞ
            G_ErrLog_Text =
                "検索ｷｰ『" + Key + "』は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しません。";
            Write_Error_Log(G_ErrLog_Text);

            // 材質名称
            P_MatNM[1] = "";
            P_MatNM[2] = "";
            P_MatNM[3] = "";
            P_MatNM[4] = "";
            P_MatNM[5] = "";
            // 図番
            P_DNO[1] = "NONE";
            P_DNO[2] = "";
            P_DNO[3] = "";
            P_DNO[4] = "";
            P_DNO[5] = "";
            // 作図用図番
            P_WKDNO[1] = "";
            P_WKDNO[2] = "";
            P_WKDNO[3] = "";
            P_WKDNO[4] = "";
            P_WKDNO[5] = "";
            // 枚数
            P_PLATE_NUM[1] = 0;
            P_PLATE_NUM[2] = 0;
            P_PLATE_NUM[3] = 0;
            P_PLATE_NUM[4] = 0;
            P_PLATE_NUM[5] = 0;
            // 枚数(SUS用)
            P_PLATE_NUM_SUS[1] = 0;
            P_PLATE_NUM_SUS[2] = 0;
            P_PLATE_NUM_SUS[3] = 0;
            P_PLATE_NUM_SUS[4] = 0;
            P_PLATE_NUM_SUS[5] = 0;
            // SUS使用フラグ
            P_SUS = 0;

            delete wkQuery;
            return false;
        }
        // 2022.05.23 フレームガス切り追加_E
        else {
            // ｴﾗｰﾛｸﾞ
            G_ErrLog_Text =
                "検索ｷｰ『" + Key + "』は、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)に存在しません。";
            Write_Error_Log(G_ErrLog_Text);

            // 材質名称
            P_MatNM[1] = "";
            P_MatNM[2] = "";
            P_MatNM[3] = "";
            P_MatNM[4] = "";
            P_MatNM[5] = "";
            // 図番
            P_DNO[1] = "";
            P_DNO[2] = "";
            P_DNO[3] = "";
            P_DNO[4] = "";
            P_DNO[5] = "";
            // 作図用図番
            P_WKDNO[1] = "";
            P_WKDNO[2] = "";
            P_WKDNO[3] = "";
            P_WKDNO[4] = "";
            P_WKDNO[5] = "";
            // 枚数
            P_PLATE_NUM[1] = 0;
            P_PLATE_NUM[2] = 0;
            P_PLATE_NUM[3] = 0;
            P_PLATE_NUM[4] = 0;
            P_PLATE_NUM[5] = 0;
            // 枚数(SUS用)
            P_PLATE_NUM_SUS[1] = 0;
            P_PLATE_NUM_SUS[2] = 0;
            P_PLATE_NUM_SUS[3] = 0;
            P_PLATE_NUM_SUS[4] = 0;
            P_PLATE_NUM_SUS[5] = 0;
            // SUS使用フラグ
            P_SUS = 0;

            delete wkQuery;
            return false;
        }
    }
    if (!wkQuery->Eof) {
        // 材質名称
        P_ZaisituNM = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight();
        P_EZaisituNM = P_ZaisituNM;
        // 材質名称
        P_MatNM[1] = wkQuery->FieldByName("MAT_NAME1")->AsString.TrimRight();
        P_MatNM[2] = wkQuery->FieldByName("MAT_NAME2")->AsString.TrimRight();
        P_MatNM[3] = wkQuery->FieldByName("MAT_NAME3")->AsString.TrimRight();
        P_MatNM[4] = wkQuery->FieldByName("MAT_NAME4")->AsString.TrimRight();
        P_MatNM[5] = wkQuery->FieldByName("MAT_NAME5")->AsString.TrimRight();
        P_MatCD[1] = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_MatCD[2] = wkQuery->FieldByName("MAT_CODE2")->AsString.TrimRight();
        P_MatCD[3] = wkQuery->FieldByName("MAT_CODE3")->AsString.TrimRight();
        P_MatCD[4] = wkQuery->FieldByName("MAT_CODE4")->AsString.TrimRight();
        P_MatCD[5] = wkQuery->FieldByName("MAT_CODE5")->AsString.TrimRight();
        // 図番
        P_DNO[1] = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_DNO[2] = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_DNO[3] = wkQuery->FieldByName("DNO3")->AsString.TrimRight();
        P_DNO[4] = wkQuery->FieldByName("DNO4")->AsString.TrimRight();
        P_DNO[5] = wkQuery->FieldByName("DNO5")->AsString.TrimRight();
        // 作図用図番
        P_WKDNO[1] = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_WKDNO[2] = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        P_WKDNO[3] = wkQuery->FieldByName("WKDNO3")->AsString.TrimRight();
        P_WKDNO[4] = wkQuery->FieldByName("WKDNO4")->AsString.TrimRight();
        P_WKDNO[5] = wkQuery->FieldByName("WKDNO5")->AsString.TrimRight();
        // 枚数
        P_PLATE_NUM[1] = wkQuery->FieldByName("PLATE_NUM1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[2] = wkQuery->FieldByName("PLATE_NUM2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[3] = wkQuery->FieldByName("PLATE_NUM3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[4] = wkQuery->FieldByName("PLATE_NUM4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM[5] = wkQuery->FieldByName("PLATE_NUM5")
            ->AsString.ToIntDef(0);
        // 枚数(SUS用)
        P_PLATE_NUM_SUS[1] = wkQuery->FieldByName("HEIGHT1")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[2] = wkQuery->FieldByName("HEIGHT2")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[3] = wkQuery->FieldByName("HEIGHT3")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[4] = wkQuery->FieldByName("HEIGHT4")
            ->AsString.ToIntDef(0);
        P_PLATE_NUM_SUS[5] = wkQuery->FieldByName("HEIGHT5")
            ->AsString.ToIntDef(0);
        // SUS使用フラグ
        P_SUS = wkQuery->FieldByName("LENGTH")->AsString.ToIntDef(0);

        G_Log_Text = "材質名称    『" + P_ZaisituNM + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番１      『" + P_DNO[1] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番２      『" + P_DNO[2] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番３      『" + P_DNO[3] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "図番４      『" + P_DNO[4] + "』を取得。";
        Write_Log(G_Log_Text);

        G_Log_Text = "作図用図番１『" + P_WKDNO[1] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番２『" + P_WKDNO[2] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番３『" + P_WKDNO[3] + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "作図用図番４『" + P_WKDNO[4] + "』を取得。";
        Write_Log(G_Log_Text);

        G_Log_Text = "枚数１      『" + AnsiString(P_PLATE_NUM[1]) + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "枚数２      『" + AnsiString(P_PLATE_NUM[2]) + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "枚数３      『" + AnsiString(P_PLATE_NUM[3]) + "』を取得。";
        Write_Log(G_Log_Text);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 孔明仕様 取得
// 概  要      ：
// 引  数      ： Kbn ：S･･･Sﾌﾚｰﾑ用処理，E･･･Eﾌﾚｰﾑ処理
// 戻り値      ： 処理結果
// 備  考      ：
// ---------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_AnaakeSiyou(AnsiString Kbn) {
    AnsiString s_Text;

    AnsiString s_S1Nzl_Syurui; // S1ﾉｽﾞﾙ種類
    AnsiString s_S1Nzl_Tugite; // S1ﾉｽﾞﾙ継手
    AnsiString s_S2Nzl_Syurui; // S2ﾉｽﾞﾙ種類
    AnsiString s_S2Nzl_Tugite; // S2ﾉｽﾞﾙ継手
    AnsiString s_S3Nzl_Syurui; // S3ﾉｽﾞﾙ種類
    AnsiString s_S3Nzl_Tugite; // S3ﾉｽﾞﾙ継手
    AnsiString s_S4Nzl_Syurui; // S4ﾉｽﾞﾙ種類
    AnsiString s_S4Nzl_Tugite; // S4ﾉｽﾞﾙ継手

    AnsiString s_E1Nzl_Syurui; // E1ﾉｽﾞﾙ種類
    AnsiString s_E1Nzl_Tugite; // E1ﾉｽﾞﾙ継手
    AnsiString s_E2Nzl_Syurui; // E2ﾉｽﾞﾙ種類
    AnsiString s_E2Nzl_Tugite; // E2ﾉｽﾞﾙ継手
    AnsiString s_E3Nzl_Syurui; // E3ﾉｽﾞﾙ種類
    AnsiString s_E3Nzl_Tugite; // E3ﾉｽﾞﾙ継手
    AnsiString s_E4Nzl_Syurui; // E4ﾉｽﾞﾙ種類
    AnsiString s_E4Nzl_Tugite; // E4ﾉｽﾞﾙ継手

    AnsiString s_AnaSiyou; // 孔明仕様
    AnsiString s_S1;
    AnsiString s_S2;
    AnsiString s_S3;
    AnsiString s_S4;
    AnsiString s_S[5];
    AnsiString s_E[5];

    int i_OgataFlg; // 0:小型中型,1:大型

    // *************************
    // ***  HEADSﾃﾞｰﾀ　ｾｯﾄ
    // *************************
    if (Kbn == "S") {
        s_S1Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ﾉｽﾞﾙ種類
        s_S1Nzl_Tugite = P_HEADS_DATA[893].TrimRight(); // S1ﾉｽﾞﾙ継手
        s_S2Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ﾉｽﾞﾙ種類
        s_S2Nzl_Tugite = P_HEADS_DATA[908].TrimRight(); // S2ﾉｽﾞﾙ継手
        s_S3Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ﾉｽﾞﾙ種類
        s_S3Nzl_Tugite = P_HEADS_DATA[923].TrimRight(); // S3ﾉｽﾞﾙ継手
        s_S4Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ﾉｽﾞﾙ種類
        s_S4Nzl_Tugite = P_HEADS_DATA[938].TrimRight(); // S4ﾉｽﾞﾙ継手
    }
    else {
        s_E1Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ﾉｽﾞﾙ種類
        s_E1Nzl_Tugite = P_HEADS_DATA[953].TrimRight(); // E1ﾉｽﾞﾙ継手
        s_E2Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ﾉｽﾞﾙ種類
        s_E2Nzl_Tugite = P_HEADS_DATA[968].TrimRight(); // E2ﾉｽﾞﾙ継手
        s_E3Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ﾉｽﾞﾙ種類
        s_E3Nzl_Tugite = P_HEADS_DATA[983].TrimRight(); // E3ﾉｽﾞﾙ継手
        s_E4Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ﾉｽﾞﾙ種類
        s_E4Nzl_Tugite = P_HEADS_DATA[998].TrimRight(); // E4ﾉｽﾞﾙ継手
    }

    // 2003.09.09 大型フレーム孔明
    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    // 2003.09.12 Ｅフレーム孔明
    // if (s_Text == "ＵＸ−８０" || s_Text == "ＵＸ−９０" ||
    // s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−７０" ||
    // s_Text == "ＳＸ−９０" || s_Text == "ＲＸ−７０"){
    // i_OgataFlg = 1;
    // } else {
    // i_OgataFlg = 0;
    // }
    // 2009.06.19 大型ﾌﾚｰﾑ条件式変更(UX-100R追加)
    ////2008.08.01 大型ﾌﾚｰﾑ条件式変更(CX-10,CX-10D追加)
    ////////2006.02.06 大型ﾌﾚｰﾑ条件式変更(RX-90,SX-90M,L追加)
    //////if (s_Text == "ＵＸ−８０" || s_Text == "ＵＸ−９０" ||
    //////    s_Text == "ＵＸ−１００" || s_Text == "ＳＸ−７０" ||
    //////    s_Text == "ＳＸ−９０" || s_Text == "ＲＸ−７０"){
    ////if (s_Text == "ＵＸ−８０" || s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" ||
    ////    s_Text == "ＳＸ−７０" || s_Text == "ＳＸ−９０" || s_Text == "ＳＸ−９０Ｍ" ||
    ////    s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０"){
    //////*************************************************
    // if (s_Text == "ＵＸ−８０" || s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" ||
    // s_Text == "ＵＸ−１３０" || s_Text == "ＳＸ−７０" || s_Text == "ＳＸ−９０" ||
    // s_Text == "ＳＸ−９０Ｍ" || s_Text == "ＳＸ−９０Ｌ" || s_Text == "ＲＸ−７０" ||
    // s_Text == "ＲＸ−９０" || s_Text == "ＲＸ−１００" || s_Text == "ＷＸ−９０" ||
    // s_Text == "ＣＸ−１０" || s_Text == "ＣＸ−１０Ｄ" || s_Text == "ＮＸ−９０"){
    ////***********************************************
    if (s_Text == "ＵＸ−８０" || s_Text == "ＵＸ−９０" || s_Text == "ＵＸ−１００" ||
        s_Text == "ＵＸ−１００Ｒ" || s_Text == "ＵＸ−１３０" || s_Text == "ＳＸ−７０" ||
        s_Text == "ＳＸ−９０" || s_Text == "ＳＸ−９０Ｍ" || s_Text == "ＳＸ−９０Ｌ" ||
        s_Text == "ＲＸ−７０" || s_Text == "ＲＸ−９０" || s_Text == "ＲＸ−１００" ||
        s_Text == "ＷＸ−９０" || s_Text == "ＣＸ−１０" || s_Text == "ＣＸ−１０Ｄ" ||
        s_Text == "ＮＸ−９０") {
        // ******************************************
        i_OgataFlg = 1;
        // 2012.08.23 大型ﾌﾚｰﾑ条件式変更(SX-80S追加)
    }
    else if (s_Text == "ＳＸ−８０Ｓ") {
        i_OgataFlg = 1;
        // *****************************************
    }
    // 2017.06.23 LX-90D,SX-80LM追加
    else if (s_Text == "ＬＸ−９０Ｄ" || s_Text == "ＳＸ−８０Ｌ" || s_Text == "ＳＸ−８０Ｍ" ) {
        i_OgataFlg = 1;
    }
    // *****************************
    // 2019.01.21 LX-50孔明変更_S
    else if (s_Text == "ＬＸ−５０" ) {
        i_OgataFlg = 1;
    }
    // 2019.01.21 LX-50孔明変更_E
    // 2019.01.21 LX-50S追加_S
    else if (s_Text == "ＬＸ−５０Ｓ" ) {
        i_OgataFlg = 1;
    }
    // 2019.01.21 LX-50S追加_E
    // 2021.03.11 RX-30変更_S
    else if (s_Text == "ＲＸ−３０" ) {
        i_OgataFlg = 1;
    }
    // 2021.03.11 RX-30変更_E
    // 2023.08.21 UX-10変更_S
    //else if (s_Text == "ＵＸ−１０" || s_Text == "ＵＸ−２０" || s_Text == "ＵＸ−３０" ||
    //    s_Text == "ＬＸ−２０" || s_Text == "ＳＸ−４０" || s_Text == "ＳＸ−４１" ||
    //    s_Text == "ＳＸ−４３" || s_Text == "ＥＸ−１５") {
    //    i_OgataFlg = 2;
    //}
    else if (s_Text == "ＵＸ−１０") {
        i_OgataFlg = 1;
    }
    else if (s_Text == "ＵＸ−２０" || s_Text == "ＵＸ−３０" ||
        s_Text == "ＬＸ−２０" || s_Text == "ＳＸ−４０" || s_Text == "ＳＸ−４１" ||
        s_Text == "ＳＸ−４３" || s_Text == "ＥＸ−１５") {
        i_OgataFlg = 2;
    }
    // 2023.08.21 UX-10変更_E
    else {
        i_OgataFlg = 0;
    }
    // *************************
    // ***************************

    if (Kbn == "S") {
        // *************************
        // ***  S1　部分
        // *************************
        if (s_S1Nzl_Syurui == "0") {
            s_S[1] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_S1Nzl_Tugite.SubString(2,1);
            s_Text = s_S1Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[1] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[1] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[1] = "N";
                }
                else {
                    s_S[1] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[1] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[1] = "1";
            }
            else {
                G_ErrLog_Text = "S1ﾉｽﾞﾙ継手『" + s_S1Nzl_Tugite +
                    "』により、S1孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S2　部分
        // *************************
        if (s_S2Nzl_Syurui == "0") {
            s_S[2] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_S2Nzl_Tugite.SubString(2,1);
            s_Text = s_S2Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[2] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[2] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[2] = "N";
                }
                else {
                    s_S[2] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[2] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[2] = "2";
            }
            else {
                G_ErrLog_Text = "S2ﾉｽﾞﾙ継手『" + s_S2Nzl_Tugite +
                    "』により、S2孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S3　部分
        // *************************
        if (s_S3Nzl_Syurui == "0") {
            s_S[3] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_S3Nzl_Tugite.SubString(2,1);
            s_Text = s_S3Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[3] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[3] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[3] = "N";
                }
                else {
                    s_S[3] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[3] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[3] = "3";
            }
            else {
                G_ErrLog_Text = "S3ﾉｽﾞﾙ継手『" + s_S3Nzl_Tugite +
                    "』により、S3孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  S4　部分
        // *************************
        if (s_S4Nzl_Syurui == "0") {
            s_S[4] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_S4Nzl_Tugite.SubString(2,1);
            s_Text = s_S4Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_S[4] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_S[4] = "K";
            }
            else if (s_Text == "1") {
                if (i_OgataFlg == 1) {
                    s_S[4] = "N";
                }
                else {
                    s_S[4] = "K";
                }
            }
            else if (s_Text == "3") {
                s_S[4] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_S[4] = "4";
            }
            else {
                G_ErrLog_Text = "S4ﾉｽﾞﾙ継手『" + s_S4Nzl_Tugite +
                    "』により、S4孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // 各取得項目結合
        s_AnaSiyou = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

    }
    else {
        // *************************
        // ***  E1　部分
        // *************************
        if (s_E1Nzl_Syurui == "0") {
            s_E[1] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_E1Nzl_Tugite.SubString(2,1);
            s_Text = s_E1Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[1] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[1] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 Ｅフレーム孔明
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[1] = "N";
                }
                else {
                    s_E[1] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[1] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[1] = "1";
            }
            else {
                G_ErrLog_Text = "E1ﾉｽﾞﾙ継手『" + s_E1Nzl_Tugite +
                    "』により、E1孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E2　部分
        // *************************
        if (s_E2Nzl_Syurui == "0") {
            s_E[2] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_E2Nzl_Tugite.SubString(2,1);
            s_Text = s_E2Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[2] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[2] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 Ｅフレーム孔明
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[2] = "N";
                }
                else {
                    s_E[2] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[2] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[2] = "2";
            }
            else {
                G_ErrLog_Text = "E2ﾉｽﾞﾙ継手『" + s_E2Nzl_Tugite +
                    "』により、E2孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E3　部分
        // *************************
        if (s_E3Nzl_Syurui == "0") {
            s_E[3] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_E3Nzl_Tugite.SubString(2,1);
            s_Text = s_E3Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[3] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[3] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 Ｅフレーム孔明
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[3] = "N";
                }
                else {
                    s_E[3] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[3] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[3] = "3";
            }
            else {
                G_ErrLog_Text = "E3ﾉｽﾞﾙ継手『" + s_E3Nzl_Tugite +
                    "』により、E3孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // *************************
        // ***  E4　部分
        // *************************
        if (s_E4Nzl_Syurui == "0") {
            s_E[4] = "0";
        }
        else {
            // 2011.06.20 ﾉｽﾞﾙ規格変更
            // s_Text = s_E4Nzl_Tugite.SubString(2,1);
            s_Text = s_E4Nzl_Tugite.SubString(3, 1);
            // ***********************

            if (s_Text == "2") {
                s_E[4] = "T";
                // 2003.09.09 大型フレーム孔明
                // } else if (s_Text == "1" || s_Text == "3"){
                // s_E[4] = "K";
            }
            else if (s_Text == "1") {
                // 2003.09.12 Ｅフレーム孔明
                // if (i_OgataFlg == 1){
                if (i_OgataFlg == 1 || i_OgataFlg == 2) {
                    // *************************
                    s_E[4] = "N";
                }
                else {
                    s_E[4] = "K";
                }
            }
            else if (s_Text == "3") {
                s_E[4] = "K";
                // ***************************
            }
            else if (s_Text == "4") {
                s_E[4] = "4";
            }
            else {
                G_ErrLog_Text = "E4ﾉｽﾞﾙ継手『" + s_E4Nzl_Tugite +
                    "』により、E4孔明ﾃﾞｰﾀが異常です。";
                Write_Error_Log(G_ErrLog_Text);

                s_AnaSiyou = "";
                return s_AnaSiyou;
            }
        }

        // 各取得項目結合
        s_AnaSiyou = "E" + s_E[1] + s_E[2] + s_E[3] + s_E[4];
    }

    return s_AnaSiyou;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ボルトカバー
// 概  要      ：
// 引  数      ： AnsiString *P_PARTS ：集計対象ﾚｺｰﾄﾞ
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
// ---------------------------------------------------------------------------
void __fastcall TFrameDetails::BLT_Cover_Out(void) {
    int i_RecNo;
    int i_ErrFlg;
    int i_Value_1;
    int i_Value_2;
    int i_Daisuu;
    AnsiString s_Text;
    // 2016.07.25 ボルトカバー寸法変更
    int i_Value_L; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    int i_Value_S; //Sﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    int i_Value_Z; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    // *******************************

    // -----------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // ***************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // ***************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // -----------------------------------------

    // ***************************
    // ***  ボルトカバー
    // ***************************
    // 『C018』
    // 2004.06.08 カバー出力条件変更
    i_RecNo = 1092;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (s_Text == "0") {
        i_RecNo = 1096;
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text == "0") {
            return;
        }
    }
    // i_RecNo = 1092;
    // s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    // if (s_Text != "0"){
    // i_RecNo = 1096;
    // s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    // if (s_Text == "0"){
    // return;
    // }
    // } else {
    // return;
    // }
    // *****************************

    G_Log_Text = "ボルトカバー         『" + s_Text + "』をセット。";
    Write_Log(G_Log_Text);

    // **********************************************************
    // ***  ﾎﾞﾙﾄ本数L,S　数値ﾁｪｯｸ
    // ***    『C019』,『C020』,『C021』の為の処理前ﾁｪｯｸ
    // **********************************************************
    i_ErrFlg = 0;
    s_Text = P_HEADS_DATA[39];
    i_Daisuu = s_Text.ToIntDef(0);

    // ﾎﾞﾙﾄ本数L
    i_RecNo = 1286;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value_1 = s_Text.ToIntDef(0); // ﾎﾞﾙﾄ本数L
    }
    else {
        G_ErrLog_Text = "ﾎﾞﾙﾄ本数Ｌ      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  不正な値『" + s_Text + "』 です。";
        Write_Error_Log(G_ErrLog_Text); // ﾃｷｽﾄｴﾗｰﾛｸﾞ
        Excel_Err_DataSet(G_ErrLog_Text); // ｴｸｾﾙｴﾗｰﾛｸﾞ

        i_ErrFlg = 1;
    }

    // ﾎﾞﾙﾄ本数S
    i_RecNo = 1287;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value_2 = s_Text.ToIntDef(0); // ﾎﾞﾙﾄ本数S
    }
    else {
        G_ErrLog_Text = "ﾎﾞﾙﾄ本数Ｓ      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  不正な値『" + s_Text + "』 です。";
        Write_Error_Log(G_ErrLog_Text); // ﾃｷｽﾄｴﾗｰﾛｸﾞ
        Excel_Err_DataSet(G_ErrLog_Text); // ｴｸｾﾙｴﾗｰﾛｸﾞ

        i_ErrFlg = 1;
    }

    // 2016.07.25 ボルトカバー寸法変更
    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text == "ＵＸ−９０") {
        if (P_HEADS_DATA[1267].TrimRight() == "200207") {
            // HEAD.VERが200207(旧ﾌﾚｰﾑ)
            i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
            i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        }
        else if (P_HEADS_DATA[1267].TrimRight() == "300207") {
            // HEAD.VERが300207(旧ﾌﾚｰﾑ)
            i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
            i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        }
        else {
            // H21
            i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
            i_Value_Z = 4; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        }
    }
    else if (s_Text == "ＵＸ−８０") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＵＸ−１００") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＵＸ−１００Ｒ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＵＸ−１１０Ｒ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＵＸ−１３０") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＵＸ−１３０Ｒ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    // 2017.06.23 LX-90D追加
    else if (s_Text == "ＬＸ−９０Ｄ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 2; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        if ( P_HEADS_DATA[37].ToIntDef(0) == 94 ){
            // LX-90D-KNUP
            i_Value_2 = 12; // ﾎﾞﾙﾄ本数S
        } else if ( P_HEADS_DATA[37].ToIntDef(0) == 394 ){
            // LX-90D-TNUP
            i_Value_2 = 12; // ﾎﾞﾙﾄ本数S
        }
    }
    // *********************
    else if (s_Text == "ＳＸ−９０Ｍ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＳＸ−９０Ｌ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＲＸ−９０") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＲＸ−１００") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＷＸ−９０") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else if (s_Text == "ＳＸ−８０Ｓ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        // 2017.06.23 数量変更
        //i_Value_Z = 4; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 2; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        // *******************　
    }
    else if (s_Text == "ＳＸ−８０Ｍ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        // 2017.06.23 数量変更
        //i_Value_Z = 4; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 2; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        // *******************　
    }
    else if (s_Text == "ＳＸ−８０Ｌ") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        // 2017.06.23 数量変更
        i_Value_Z = 4; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        //i_Value_Z = 2; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        // *******************　
    }
    else if (s_Text == "ＳＸ−７０") {
        i_Value_L = 4; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }
    else {
        i_Value_L = 0; //Lﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
        i_Value_Z = 0; //上下ﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量
    }

    //S及び上下Sﾎﾞﾙﾄ用ﾎﾞﾙﾄｶﾊﾞｰ数量が設定されている場合それを優先
    if (i_Value_L != 0) {
        i_Value_S = i_Value_1 + i_Value_2 - i_Value_L - i_Value_Z;
    }
    else {
        i_Value_L = i_Value_1;
        i_Value_S = i_Value_2;
    }
    // *******************************

    // ***************************
    // ***  BLCP1
    // ***************************
    // 『C019』,『C020』
    if (i_ErrFlg == 0) {
        i_RecNo = 1092; // BLCP1
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCP1 『C019』
            s_Text = s_Text + "L";
            P_Siyou1 = HanToZen(s_Text); // 仕様１
            G_Log_Text = "BLCP1                『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // BLCP1本数 『C020』
            // 2016.07.25 ボルトカバー寸法変更
            //P_1DaiSuryo = i_Value_1 + i_Value_2;
            P_1DaiSuryo = i_Value_L + i_Value_S + i_Value_Z;
            // *******************************
            P_Yobisu = 0; // 予備数
            P_Soukei = P_1DaiSuryo * i_Daisuu; // 総計
            G_Log_Text = "BLCP1 本数           『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // 1  部品分類
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  識別子
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  部品型式
            s_Text = ",締付ボルトカバー";
            fputs(s_Text.c_str(), fp);
            // 4  部品名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  作図用図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1台分数量
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  予備数
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 仕様１
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 応用技術編集
            // 12 仕様２
            // 2014.12.19 ﾎﾞﾙﾄ径訂正
            //s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "用";
             s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",１　５／８−８ＵＮ用";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",２−８ＵＮ用";
            } else {
                s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "用";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 仕様３
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 仕様４
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 仕様５
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 ﾐﾗｰ仕様対応
            // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // 改行文字
            s_Text = "\n";
            // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
            fputs(s_Text.c_str(), fp);
        }
    }
    // ***************************
    // ***  BLCEL
    // ***************************
    // 『C021』,『C022』
    if (i_ErrFlg == 0) {
        i_RecNo = 1096; // BLCEL
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCEL 『C021』
            s_Text = s_Text + "L キャップ付";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCEL                『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // BLCEL本数 『C022』
            // 2016.07.25 ボルトカバー寸法変更
            //P_1DaiSuryo = i_Value_1;
            P_1DaiSuryo = i_Value_L;
            // *******************************
            P_Yobisu = 0; // 予備数
            P_Soukei = P_1DaiSuryo * i_Daisuu; // 総計
            G_Log_Text = "BLCEL 本数           『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // 1  部品分類
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  識別子
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  部品型式
            s_Text = ",締付ボルトカバー";
            fputs(s_Text.c_str(), fp);
            // 4  部品名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  作図用図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1台分数量
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  予備数
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 仕様１
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 応用技術編集
            // 12 仕様２
            // 2014.12.19 ﾎﾞﾙﾄ径訂正
            //s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "用";
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",１　５／８−８ＵＮ用";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",２−８ＵＮ用";
            } else {
                s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "用";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 仕様３
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 仕様４
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 仕様５
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 ﾐﾗｰ仕様対応
            // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // 改行文字
            s_Text = "\n";
            // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
            fputs(s_Text.c_str(), fp);
        }
    }

    // ***************************
    // ***  BLCES
    // ***************************
    // 『C023』,『C024』
    if (i_ErrFlg == 0) {
        i_RecNo = 1097; // BLCES
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCEL 『C023』
            s_Text = s_Text + "L キャップ付";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCES                『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // BLCEL本数 『C024』
            // 2016.07.25 ボルトカバー寸法変更
            //P_1DaiSuryo = i_Value_2;
            P_1DaiSuryo = i_Value_S;
            // *******************************
            P_Yobisu = 0; // 予備数
            P_Soukei = P_1DaiSuryo * i_Daisuu; // 総計
            G_Log_Text = "BLCES 本数           『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // 1  部品分類
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  識別子
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  部品型式
            s_Text = ",締付ボルトカバー";
            fputs(s_Text.c_str(), fp);
            // 4  部品名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  作図用図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1台分数量
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  予備数
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 仕様１
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 2014/08/22 応用技術編集
            // 12 仕様２
            // 2014.12.19 ﾎﾞﾙﾄ径訂正
            //s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "用";
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",１　５／８−８ＵＮ用";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",２−８ＵＮ用";
            } else {
                s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "用";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 仕様３
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 仕様４
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 仕様５
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // ------------------------------
            // 2007.10.03 ﾐﾗｰ仕様対応
            // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);
            // 2007.10.03
            // ------------------------------

            // 改行文字
            s_Text = "\n";
            // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
            fputs(s_Text.c_str(), fp);
        }
    }

    // 2016.07.25 ボルトカバー寸法変更
    // ***************************
    // ***  BLCESS
    // ***************************
    if (i_ErrFlg == 0) {
        i_RecNo = 1112; // BLCESS
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if ((s_Text.ToIntDef(0) != 0)&& (i_Value_Z != 0)) {
            // BLCESS
            s_Text = s_Text + "L キャップ付";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCESS                『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // BLCESS本数
            P_1DaiSuryo = i_Value_Z;
            P_Yobisu = 0;                       // 予備数
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // 総計
            G_Log_Text = "BLCESS 本数           『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // 1  部品分類
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  識別子
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  部品型式
            s_Text = ",締付ボルトカバー";
            fputs(s_Text.c_str(), fp);
            // 4  部品名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  作図用図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1台分数量
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  予備数
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 仕様１
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            s_Text = P_HEADS_DATA[1019].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",１　５／８−８ＵＮ用";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",２−８ＵＮ用";
            } else {
                s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1019].SubString(1, 2)) + "用";
            }
            fputs(s_Text.c_str(), fp);
            // 13 仕様３
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 仕様４
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 仕様５
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // 改行文字
            s_Text = "\n";
            // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
            fputs(s_Text.c_str(), fp);

        }
    }
    // *******************************

}

// ---------------------------------------------------------------------------
// 日本語関数名： ボルトカバー出力(小径ボルト)
// 概  要      ： BLCP1SとBLCESSを出力する。
// 引  数      ：
// 戻り値      ： なし
// 備  考      ： BLT_Cover_Out(void)を踏襲して作成。
// ---------------------------------------------------------------------------
// 2014/08/22 応用技術編集
void __fastcall TFrameDetails::BLT_Cover_Out_Small(void) {
    int i_RecNo;
    int i_ErrFlg;
    int i_Value;
    int i_Daisuu;
    AnsiString s_Text;

    // ***************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // ***************************
    AnsiString sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());

    // ***************************
    // ***  ボルトカバー
    // ***************************
    i_RecNo = 1111;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (s_Text == "0") {
        i_RecNo = 1112;
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text == "0") {
            return;
        }
    }

    G_Log_Text = "小径ボルトカバー         『" + s_Text + "』をセット。";
    Write_Log(G_Log_Text);

    // **********************************************************
    // ***  ﾎﾞﾙﾄ本数数値ﾁｪｯｸ
    // **********************************************************
    i_ErrFlg = 0;
    s_Text = P_HEADS_DATA[39];
    i_Daisuu = s_Text.ToIntDef(0);

    // 小径ボルト本数S
    i_RecNo = 1113;
    s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
    if (AllFloatChk(s_Text.c_str(), 0) == 0) {
        i_Value = s_Text.ToIntDef(0); // 小径ボルト本数S
    }
    else {
        G_ErrLog_Text = "小径ﾎﾞﾙﾄ本数Ｓ      RecNo:" + FormatFloat("0000", i_RecNo) +
            "  不正な値『" + s_Text + "』 です。";
        Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
        Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

        i_ErrFlg = 1;
    }

    // ***************************
    // ***  BLCP1S
    // ***************************
    if (i_ErrFlg == 0) {
        i_RecNo = 1111; // BLCP1S
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCP1S
            s_Text = s_Text + "L";
            P_Siyou1 = HanToZen(s_Text); // 仕様１
            G_Log_Text = "BLCP1S               『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // BLCP1S本数
            P_1DaiSuryo = i_Value;
            P_Yobisu = 0;                       // 予備数
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // 総計
            G_Log_Text = "BLCP1S 本数           『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // 1  部品分類
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  識別子
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  部品型式
            s_Text = ",締付ボルトカバー";
            fputs(s_Text.c_str(), fp);
            // 4  部品名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  作図用図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1台分数量
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  予備数
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 仕様１
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 12 仕様２
            // 2014.12.19 ﾎﾞﾙﾄ径訂正
            //s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "用";
            s_Text = P_HEADS_DATA[1108].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",１　５／８−８ＵＮ用";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",２−８ＵＮ用";
            } else {
                s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "用";
            }
            //***********************
            fputs(s_Text.c_str(), fp);
            // 13 仕様３
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 仕様４
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 仕様５
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // 改行文字
            s_Text = "\n";
            // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
            fputs(s_Text.c_str(), fp);
        }
    }

    // ***************************
    // ***  BLCESS
    // ***************************
    if (i_ErrFlg == 0) {
        i_RecNo = 1112; // BLCESS
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (s_Text.ToIntDef(0) != 0) {
            // BLCESS
            s_Text = s_Text + "L キャップ付";
            P_Siyou1 = HanToZen(s_Text);
            G_Log_Text = "BLCESS                『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // BLCESS本数
            P_1DaiSuryo = i_Value;
            P_Yobisu = 0;                       // 予備数
            P_Soukei = P_1DaiSuryo * i_Daisuu;  // 総計
            G_Log_Text = "BLCESS 本数           『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            // 1  部品分類
            s_Text = "FRAME";
            fputs(s_Text.c_str(), fp);
            // 2  識別子
            s_Text = ",BLTCV1";
            fputs(s_Text.c_str(), fp);
            // 3  部品型式
            s_Text = ",締付ボルトカバー";
            fputs(s_Text.c_str(), fp);
            // 4  部品名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 5  材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 6  図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 7  作図用図番
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 8  1台分数量
            s_Text = "," + AnsiString(P_1DaiSuryo);
            fputs(s_Text.c_str(), fp);
            // 9  予備数
            s_Text = "," + AnsiString(P_Yobisu);
            fputs(s_Text.c_str(), fp);

            // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
            s_Text = "," + AnsiString(P_Soukei);
            fputs(s_Text.c_str(), fp);

            // 11 仕様１
            s_Text = "," + P_Siyou1;
            fputs(s_Text.c_str(), fp);
            // 12 仕様２
            // 2014.12.19 ﾎﾞﾙﾄ径訂正
            //s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "用";
            s_Text = P_HEADS_DATA[1108].TrimRight();
            if ( s_Text=="1.630" ) {
                s_Text = ",１　５／８−８ＵＮ用";
            } else if ( s_Text=="2.000" ) {
                s_Text = ",２−８ＵＮ用";
            } else {
                s_Text = ",Ｍ" + HanToZen(P_HEADS_DATA[1108].SubString(1, 2)) + "用";
            }
            //**********************
            fputs(s_Text.c_str(), fp);
            // 13 仕様３
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 14 仕様４
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 15 仕様５
            s_Text = ",";
            fputs(s_Text.c_str(), fp);
            // 16 材質名称
            s_Text = ",";
            fputs(s_Text.c_str(), fp);

            // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
            s_Text = "," + sSpecCode;
            fputs(s_Text.c_str(), fp);

            // 改行文字
            s_Text = "\n";
            // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
            fputs(s_Text.c_str(), fp);
        }
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： ボルト予備品取得処理
// 概  要      ：
// 引  数      ： ボルトサイズ　材質コード
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
int __fastcall TFrameDetails::Get_Blt_YobiSu(double d_BltSz, double d_BltLen,
    AnsiString MatCD, HeadsData* pcHeadsData) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;
    AnsiString s_YobiName;
    int i_Yobisu;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_BLT2YOBI";
    s_SQL = s_SQL + "  WHERE BLTSIZE = " + d_BltSz;

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

    // CSV読込用
    FILE *wkfp;

    char buf[1024]; // 文字列バッファ
    char *tok; // トークンポインタ
    int i; // ループカウンタ
    char token[100][100]; // トークンバッファ

    AnsiString s_Yobi_Text[5];
    AnsiString s_YBKey;
    AnsiString s_YBZaisitu;

    AnsiString wk_FilePass; // "D00001.csv"
    int i_RecNo;
    int i_lop;

    i_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)
    i_Yobisu = 0;

    // --------------------------------------------
    // 2007.10.04 ﾐﾗｰ仕様対応
    // ﾐﾗｰB仕様の場合、予備品は計上しない
    if (pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B)
        return 0;
    // 2007.10.04
    // --------------------------------------------

    // 一度計上した予備品は計上しない
    for (i_lop = 0; i_lop < G_YOBICHK_RecCnt; i_lop++) {
        if ((s_YobiName == G_YOBICHK_NAME[i_lop]) &&
            (MatCD == G_YOBICHK_MAT[i_lop])) {
            return 0;
        }
    }

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
            s_Yobi_Text[4] = token[24]; // 予備品ﾃﾞｰﾀ 全長

            // 検索ｷｰ生成
            if (AllNumChk(s_Yobi_Text[1].c_str(), 0) == 0) {
                int i_No;

                i_No = StrToInt(s_Yobi_Text[1]) + 100;
                s_YBKey = s_Yobi_Text[0] + IntToStr(i_No).SubString(2, 2);
                s_YBZaisitu = s_Yobi_Text[3].TrimRight();
            }
            else {
                // ********************************************
                // 予備品ﾃﾞｰﾀ 2列目が
                // ｱﾙﾌｧﾍﾞｯﾄもしくはNULLだった場合 検索しない?
                // ********************************************
                s_YBKey = "";
            }

            if (s_YBKey != "\0") {
                if (s_YBKey == s_YobiName) {
                    if ((((MatCD != "0") && (s_YBZaisitu == MatCD)) ||
                        (MatCD == "0")) &&
                        (d_BltLen == s_Yobi_Text[4].ToDouble())) {
                        i_Yobisu += s_Yobi_Text[2].ToIntDef(0);
                        i_RecNo++;
                        G_YOBICHK_NAME[G_YOBICHK_RecCnt] = s_YobiName;
                        G_YOBICHK_MAT[G_YOBICHK_RecCnt] = MatCD;
                        G_YOBICHK_RecCnt++;
                    }
                }
            }
        }
    }

    // 該当予備品ﾃﾞｰﾀが存在しなかった場合
    if (i_RecNo == 0)
        i_Yobisu = 0;

    // 予備品ﾃﾞｰﾀ ﾌｧｲﾙｸﾛｰｽﾞ
    fclose(wkfp);
    return i_Yobisu;
}

// ---------------------------------------------------------------------------
// 日本語関数名： スペーサー孔明仕様 取得
// 概  要      ：
// 引  数      ： Kbn ：S･･･Sﾌﾚｰﾑ用処理，E･･･Eﾌﾚｰﾑ処理
// 戻り値      ： 処理結果
// 備  考      ： 2022.01.14追加
// ---------------------------------------------------------------------------
AnsiString __fastcall TFrameDetails::Get_AnaakeSiyou_S(AnsiString Kbn) {
    AnsiString s_Text;

    AnsiString s_S1Nzl_Syurui; // S1ﾉｽﾞﾙ種類
    AnsiString s_S2Nzl_Syurui; // S2ﾉｽﾞﾙ種類
    AnsiString s_S3Nzl_Syurui; // S3ﾉｽﾞﾙ種類
    AnsiString s_S4Nzl_Syurui; // S4ﾉｽﾞﾙ種類

    AnsiString s_E1Nzl_Syurui; // E1ﾉｽﾞﾙ種類
    AnsiString s_E2Nzl_Syurui; // E2ﾉｽﾞﾙ種類
    AnsiString s_E3Nzl_Syurui; // E3ﾉｽﾞﾙ種類
    AnsiString s_E4Nzl_Syurui; // E4ﾉｽﾞﾙ種類

    AnsiString s_AnaSiyou; // 孔明仕様
    AnsiString s_S1;
    AnsiString s_S2;
    AnsiString s_S3;
    AnsiString s_S4;
    AnsiString s_S[5];
    AnsiString s_E[5];

    int i_OgataFlg; // 0:小型中型,1:大型

    // *************************
    // ***  HEADSﾃﾞｰﾀ　ｾｯﾄ
    // *************************
    if (Kbn == "S") {
        s_S1Nzl_Syurui = P_HEADS_DATA[891].TrimRight(); // S1ﾉｽﾞﾙ種類
        s_S2Nzl_Syurui = P_HEADS_DATA[906].TrimRight(); // S2ﾉｽﾞﾙ種類
        s_S3Nzl_Syurui = P_HEADS_DATA[921].TrimRight(); // S3ﾉｽﾞﾙ種類
        s_S4Nzl_Syurui = P_HEADS_DATA[936].TrimRight(); // S4ﾉｽﾞﾙ種類
    }
    else {
        s_E1Nzl_Syurui = P_HEADS_DATA[951].TrimRight(); // E1ﾉｽﾞﾙ種類
        s_E2Nzl_Syurui = P_HEADS_DATA[966].TrimRight(); // E2ﾉｽﾞﾙ種類
        s_E3Nzl_Syurui = P_HEADS_DATA[981].TrimRight(); // E3ﾉｽﾞﾙ種類
        s_E4Nzl_Syurui = P_HEADS_DATA[996].TrimRight(); // E4ﾉｽﾞﾙ種類
    }


    if (Kbn == "S") {
        // *************************
        // ***  S1　部分
        // *************************
        if (s_S1Nzl_Syurui == "0") {
            s_S[1] = "0";
        }
        else {
            s_S[1] = "1";
        }

        // *************************
        // ***  S2　部分
        // *************************
        if (s_S2Nzl_Syurui == "0") {
            s_S[2] = "0";
        }
        else {
            s_S[2] = "2";
        }

        // *************************
        // ***  S3　部分
        // *************************
        if (s_S3Nzl_Syurui == "0") {
            s_S[3] = "0";
        }
        else {
            s_S[3] = "3";
        }

        // *************************
        // ***  S4　部分
        // *************************
        if (s_S4Nzl_Syurui == "0") {
            s_S[4] = "0";
        }
        else {
            s_S[4] = "4";
        }

        // 各取得項目結合
        s_AnaSiyou = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

    }
    else {
        // *************************
        // ***  E1　部分
        // *************************
        if (s_E1Nzl_Syurui == "0") {
            s_E[1] = "0";
        }
        else {
            s_E[1] = "1";
        }

        // *************************
        // ***  E2　部分
        // *************************
        if (s_E2Nzl_Syurui == "0") {
            s_E[2] = "0";
        }
        else {
            s_E[2] = "2";
        }

        // *************************
        // ***  E3　部分
        // *************************
        if (s_E3Nzl_Syurui == "0") {
            s_E[3] = "0";
        }
        else {
            s_E[3] = "3";
        }

        // *************************
        // ***  E4　部分
        // *************************
        if (s_E4Nzl_Syurui == "0") {
            s_E[4] = "0";
        }
        else {
            s_E[4] = "4";
        }

        // 各取得項目結合
        s_AnaSiyou = "E" + s_E[1] + s_E[2] + s_E[3] + s_E[4];
    }

    return s_AnaSiyou;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Sﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ作成処理 実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2022.05.23追加
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::S_FrameG_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    AnsiString s_ChinaM;

    // **********************************************
    // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)　検索
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索。";
    Write_Log(G_Log_Text);

    // seek_ret･･･ﾏｽﾀに存在したかしないか
    // ※注 Write_SE_Frame_Data()で使用する。
    seek_ret = Search_HD_FRM_NZ(P_Key, "SG");

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索終了。";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  材質名称の検索(ASMEが含まれているか)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ASMEです。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("ＡＳＭＥ") != 0) {
            G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ＡＳＭＥです。";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  孔明仕様の作成
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("S");
    G_Log_Text = "孔明仕様  『" + P_Siyou1 + "』を取得。";
    Write_Log(G_Log_Text);

    // *************************
    // ***  材質の取得
    // *************************
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);

    switch (P_HEADS_DATA[37].ToIntDef(0)) {
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // Ｂシリーズ
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2023.03.09 CX-00-Y追加_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y追加_E
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
    case 354: // Ｙシリーズ
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "Ｓ２７５ＪＲ";
            P_EZaisituNM = "Ｓ２７５ＪＲ";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
            P_ZaisituNM = "ＳＳ４００";
            P_EZaisituNM = "ＳＳ４００";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text == "ＵＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−００") { // 中国材質
    }
    else if (s_Text == "ＬＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−２０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４１") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１１") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−７０") { // 中国材質
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // 中国材質
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "SS400") {
        }
        else if (P_ZaisituNM == "ＳＳ４００" || P_ZaisituNM == "Ｃ．Ｓ．") {
            P_ZaisituNM = "Ｑ２３５Ｂ";
            P_EZaisituNM = "Ｑ２３５Ｂ";
        }
    }

    // **********************************************
    // ***  図番・作図用図番の取得  02.09.03追加
    // **********************************************
    bSUS = false;
    sPltMat = P_HEADS_DATA[43].TrimRight();
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // プレート材質がチタン系以外
        bSUS = true;
    }
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        P_Siyou3 = "3";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        P_Siyou3 = "1";
    }

    // *************************
    // ***  予備数の取得
    // *************************
    Syukei_Frame_Yobisu("S");

    // *************************
    // ***  Sﾌﾚｰﾑﾃﾞｰﾀの出力
    // *************************
    if ( P_DNO[1] == "NONE" ) {
    }
    else {
        Write_SE_Frame_Data("SG", seek_ret);
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Eﾌﾚｰﾑｶﾞｽｷﾘﾃﾞｰﾀ作成処理 実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2022.05.23追加
// ---------------------------------------------------------------------------
bool __fastcall TFrameDetails::E_FrameG_Hontai_Sakusei(void) {
    bool seek_ret;
    AnsiString s_Text;
    int i_RecNo;
    AnsiString s_SQL;
    AnsiString sPltMat;
    bool bSUS;
    AnsiString s_ChinaM;

    // **********************************************
    // ***  ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)　検索
    // **********************************************
    P_Key = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" +
        P_HEADS_DATA[1267].TrimRight();

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索。";
    Write_Log(G_Log_Text);

    // seek_ret･･･ﾏｽﾀに存在したかしないか
    // Write_SE_Frame_Data()で使用する。
    seek_ret = Search_HD_FRM_NZ(P_Key, "EG");

    G_Log_Text = "検索ｷｰ『" + P_Key + "』で、ﾌﾚｰﾑﾉｽﾞﾙ部品構成ﾏｽﾀ(HD_FRM_NZ)を検索終了。";
    Write_Log(G_Log_Text);

    // **********************************************
    // ***  材質名称の検索(ASMEが含まれているか)
    // **********************************************
    if (P_ZaisituNM.Pos("ASME") != 0) {
        G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ASMEです。";
        Write_Error_Log(G_ErrLog_Text);
        return false;
    }
    else {
        if (P_ZaisituNM.Pos("ＡＳＭＥ") != 0) {
            G_ErrLog_Text = "材質『" + P_ZaisituNM + "』が、ＡＳＭＥです。";
            Write_Error_Log(G_ErrLog_Text);
            return false;
        }
    }

    // *************************
    // ***  孔明仕様の作成
    // *************************
    P_Siyou1 = Get_AnaakeSiyou("E");
    G_Log_Text = "孔明仕様  『" + P_Siyou1 + "』を取得。";
    Write_Log(G_Log_Text);

    // *************************
    // ***  材質の取得
    // *************************
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);

    // **********************************************
    // ***  図番・作図用図番の取得  02.09.03追加
    // **********************************************
    bSUS = false;
    sPltMat = P_HEADS_DATA[43].TrimRight();
    if ((P_SUS != 0) && (Type_MAT(sPltMat) != "Ti")) {
        // プレート材質がチタン系以外
        bSUS = true;
    }
    if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <= P_PLATE_NUM[1]))
        || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[1]))) {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[2])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[2]))) {
        P_Zuban = P_DNO[2];
        P_SZuban = P_WKDNO[2];
        if (P_MatNM[2] != "") {
            P_ZaisituNM = P_MatNM[2];
            P_EZaisituNM = P_MatNM[2];
        }
        else if (P_MatCD[2] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[2]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[2]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "2";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[3])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[3]))) {
        P_Zuban = P_DNO[3];
        P_SZuban = P_WKDNO[3];
        if (P_MatNM[3] != "") {
            P_ZaisituNM = P_MatNM[3];
            P_EZaisituNM = P_MatNM[3];
        }
        else if (P_MatCD[3] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[3]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[3]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "3";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[4])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[4]))) {
        P_Zuban = P_DNO[4];
        P_SZuban = P_WKDNO[4];
        if (P_MatNM[4] != "") {
            P_ZaisituNM = P_MatNM[4];
            P_EZaisituNM = P_MatNM[4];
        }
        else if (P_MatCD[4] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[4]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[4]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "4";
    }
    else if (((bSUS == false) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM[5])) || ((bSUS == true) && (P_HEADS_DATA[170].ToIntDef(0) <=
        P_PLATE_NUM_SUS[5]))) {
        P_Zuban = P_DNO[5];
        P_SZuban = P_WKDNO[5];
        if (P_MatNM[5] != "") {
            P_ZaisituNM = P_MatNM[5];
            P_EZaisituNM = P_MatNM[5];
        }
        else if (P_MatCD[5] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[5]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[5]);
        }
        else if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "5";
    }
    else {
        P_Zuban = P_DNO[1];
        P_SZuban = P_WKDNO[1];
        if (P_MatNM[1] != "") {
            P_ZaisituNM = P_MatNM[1];
            P_EZaisituNM = P_MatNM[1];
        }
        else if (P_MatCD[1] != "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        P_Siyou3 = "1";
    }

    switch (P_HEADS_DATA[37].ToIntDef(0)) { // フレームコード
    case 14:
    case 15:
    case 16:
    case 27:
    case 28:
    case 29:
    case 63:
    case 64: // Ｂシリーズ
        if (P_ZaisituNM == "") {
            P_ZaisituNM = Search_HD_MATERIAL(P_MatCD[1]);
            P_EZaisituNM = Search_HD_E_MATERIAL(P_MatCD[1]);
        }
        break;
    // 2023.03.09 CX-00-Y追加_S
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    // 2023.03.09 CX-00-Y追加_E
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
    case 354: // Ｙシリーズ
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "S275JR") {
            P_ZaisituNM = "Ｓ２７５ＪＲ";
            P_EZaisituNM = "Ｓ２７５ＪＲ";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }
        break;
    default:
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();

        if (Type_MAT(s_Text) == "-" ||
            Type_MAT(s_Text) == "SS400") {
            P_ZaisituNM = "ＳＳ４００";
            P_EZaisituNM = "ＳＳ４００";
        }
        else {
            P_ZaisituNM = Search_HD_MATERIAL(s_Text);
            P_EZaisituNM = Search_HD_E_MATERIAL(s_Text);
        }

        break;
    }

    i_RecNo = 1107;
    s_ChinaM = P_HEADS_DATA[i_RecNo].TrimRight();

    s_Text = G_KS_Syurui + "−" + G_KS_Model;
    if (s_Text == "ＵＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−００") { // 中国材質
    }
    else if (s_Text == "ＬＸ−１０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＬＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−２０") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４１") { // 中国材質
    }
    else if (s_Text == "ＳＸ−４３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１１") { // 中国材質
    }
    else if (s_Text == "ＲＸ−１３") { // 中国材質
    }
    else if (s_Text == "ＲＸ−３０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−５０") { // 中国材質
    }
    else if (s_Text == "ＲＸ−７０") { // 中国材質
    }
    else {
        s_ChinaM = "";
    }

    if (s_ChinaM == "CHINA") { // 中国材質
        i_RecNo = 1044; // SPFRM
        s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
        if (Type_MAT(s_Text) == "SS400") {
        }
        else if (P_ZaisituNM == "ＳＳ４００" || P_ZaisituNM == "Ｃ．Ｓ．") {
            P_ZaisituNM = "Ｑ２３５Ｂ";
            P_EZaisituNM = "Ｑ２３５Ｂ";
        }
    }

    // *************************
    // ***  予備数の取得
    // *************************
    Syukei_Frame_Yobisu("E");

    // *************************
    // ***  Eﾌﾚｰﾑﾃﾞｰﾀの出力
    // *************************
    if ( P_DNO[1] == "NONE" ) {
    }
    else {
        Write_SE_Frame_Data("EG", seek_ret);
    }

    delete wkQuery;

    return true;
}





