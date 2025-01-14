// ---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：ﾌﾟﾚｰﾄ部品展開処理  モジュール
// 作 成 者：N.Uchida
// 作 成 日：2002.06.18
// 更 新 日：2002.10.16
// ：2002.10.23 ガスケット処理追加
// 2007.10.03 ミラー仕様対応
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

#include "Bas_PlateDetails.h"

#include "HeadsData.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// ﾌﾟﾚｰﾄ部品展開処理 Class
// ---------------------------------------------------------------------------
class TPlateDetails {
private:
    FILE *fp;
    // FILE *wkfp;

    // ↓2002/10/23
    // BFRGﾃｰﾌﾞﾙそのまま
    long BP_Hp;
    AnsiString BBP_Name;
    AnsiString BBP_Label;
    long BBP_Code;
    long BBP_Num;
    AnsiString ABP_Name;
    AnsiString ABP_Label;
    long ABP_Code;
    long ABP_Num;
    AnsiString Note;
    // Dｶﾞｽｹｯﾄﾃｰﾌﾞﾙそのまま
    AnsiString DP_B1s1_Name;
    AnsiString DP_B1s1_Label;
    long DP_B1s1_Code;
    long DP_B1s1_Num;
    AnsiString DP_B0s1_Name;
    AnsiString DP_B0s1_Label;
    long DP_B0s1_Code;
    long DP_B0s1_Num;
    AnsiString DP_B1s2_Name;
    AnsiString DP_B1s2_Label;
    long DP_B1s2_Code;
    long DP_B1s2_Num;
    AnsiString DP_B0s2_Name;
    AnsiString DP_B0s2_Label;
    long DP_B0s2_Code;
    long DP_B0s2_Num;
    long DPU_Meq_Sub;
    AnsiString DP_B1s3_Name;
    AnsiString DP_B1s3_Label;
    long DP_B1s3_Code;
    long DP_B1s3_Num;
    AnsiString DP_B0s3_Name;
    AnsiString DP_B0s3_Label;
    long DP_B0s3_Code;
    long DP_B0s3_Num;
    AnsiString DP_B1s4_Name;
    AnsiString DP_B1s4_Label;
    long DP_B1s4_Code;
    long DP_B1s4_Num;
    AnsiString DP_B0s4_Name;
    AnsiString DP_B0s4_Label;
    long DP_B0s4_Code;
    long DP_B0s4_Num;
    long DPL_Meq_Sub;
    AnsiString DD_Name;
    AnsiString DD_Label;
    long DD_Code;
    long DD_Num;
    // Eｶﾞｽｹｯﾄﾃｰﾌﾞﾙそのまま
    AnsiString EP_B1s1_Name;
    AnsiString EP_B1s1_Label;
    long EP_B1s1_Code;
    long EP_B1s1_Num;
    AnsiString EP_B0s1_Name;
    AnsiString EP_B0s1_Label;
    long EP_B0s1_Code;
    long EP_B0s1_Num;
    AnsiString EP_B1s2_Name;
    AnsiString EP_B1s2_Label;
    long EP_B1s2_Code;
    long EP_B1s2_Num;
    AnsiString EP_B0s2_Name;
    AnsiString EP_B0s2_Label;
    long EP_B0s2_Code;
    long EP_B0s2_Num;
    long EPUH_Meq_Sub;
    long EPU_Meq_Sub;
    AnsiString EP_B1s3_Name;
    AnsiString EP_B1s3_Label;
    long EP_B1s3_Code;
    long EP_B1s3_Num;
    AnsiString EP_B0s3_Name;
    AnsiString EP_B0s3_Label;
    long EP_B0s3_Code;
    long EP_B0s3_Num;
    AnsiString EP_B1s4_Name;
    AnsiString EP_B1s4_Label;
    long EP_B1s4_Code;
    long EP_B1s4_Num;
    AnsiString EP_B0s4_Name;
    AnsiString EP_B0s4_Label;
    long EP_B0s4_Code;
    long EP_B0s4_Num;
    long EPLH_Meq_Sub;
    long ED_Hp;
    // 2019.08.01 Ｓ、Ｔディスタンスピース追加_S
    //AnsiString ED_Name;
    //AnsiString ED_Label;
    //long ED_Code;
    //long ED_Num;
    AnsiString ED_B1_Name;
    AnsiString ED_B1_Label;
    long ED_B1_Code;
    long ED_B1_Num;
    AnsiString ED_B0_Name;
    AnsiString ED_B0_Label;
    long ED_B0_Code;
    long ED_B0_Num;
    // 2019.08.01 Ｓ、Ｔディスタンスピース追加_E
    AnsiString DE_Kubun;
    // ↑2002/10/23

    // ↓02.08.20 Add
    AnsiString P_Sakuban_Mode; // 作番種類(S:製造番号，M:見積番号)
    AnsiString P_Mat_Code; // 材質ｺｰﾄﾞ(HEARTSﾃﾞｰﾀ書込専用)
    AnsiString P_Ana_Plate; // 孔明ﾃﾞｰﾀ1列目,ﾌﾟﾚｰﾄ型式
    AnsiString P_Unit; // 単位（式）
    // ↑

    AnsiString P_MixFlg; // ﾐｯｸｽﾌﾗｸﾞ
    AnsiString P_HeadsVer; // HEADS Version
    AnsiString P_SoSyoAscii; // 型式総称半角

    AnsiString P_Katasiki; // 部品型式
    AnsiString P_Zaisitu; // 材質名称
    AnsiString P_EZaisitu; // 材質名称
    AnsiString P_Zuban; // 図番
    AnsiString P_SZuban; // 作図用図番
    // 2003.11.26変数追加
    AnsiString P_Zuban2; // 図番２
    AnsiString P_SZuban2; // 作図用図番２
    // *******************
    long P_Suryo; // １台分数量
    long P_Yobisu; // 予備数
    long P_YobisuPltAll; // 予備数
    long P_PltGsktYobiA; // 予備品のプレートがガスケット付きの場合のAプレート枚数
    long P_PltGsktYobiB; // 予備品のプレートがガスケット付きの場合のBプレート枚数
    long P_PltGsktYobiD; // 予備品のプレートがガスケット付きの場合のDプレート枚数
    long P_PltGsktYobiE; // 予備品のプレートがガスケット付きの場合のEプレート枚数
    // 2004.01.19 変数追加
    long P_PltGsktYobiS; // 予備品のプレートに付く真空時目玉盲部補強ガスケット枚数
    // *******************
    // 2004.01.16 変数追加
    long P_PltYobiA; // 予備品のAプレート枚数
    long P_PltYobiB; // 予備品のBプレート枚数
    long P_PltYobiD; // 予備品のDプレート枚数
    long P_PltYobiE; // 予備品のEプレート枚数
    // *******************
    long P_Soukei; // 総計
    int P_RecNo; // ﾚｺｰﾄﾞｶｳﾝﾀ
    bool bMouAteFlg; // 盲当板を、フレーム材質とする場合のフラグ

    // ﾌﾟﾚｰﾄ部品 ﾃﾞｰﾀ作成用
    AnsiString P_PLATECD[5]; // ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ
    int P_JokenFlg1; // 読み飛ばし条件ﾌﾗｸﾞ１(0:読込む,1:読み飛ばす)
    int P_JokenFlg2; // 読み飛ばし条件ﾌﾗｸﾞ１(0:読込む,1:読み飛ばす)
    AnsiString P_SSyurui; // 総称種類
    AnsiString P_SModel; // 総称ﾓﾃﾞﾙ
    AnsiString P_MAT_CODE1; // 材質ｺｰﾄﾞ１
    AnsiString P_SideDiv; // A側・Ｂ側
    AnsiString P_YobiCD[50]; // 予備品ｺｰﾄﾞメモ
    AnsiString P_YobiMAT[50]; // 予備品材質ｺｰﾄﾞメモ
    int P_YobiMemoCnt; // 予備品メモのカウンタ

    // ↓2007.10.4 ミラー仕様対応
    HeadsData* m_pcHeadsData;
    AnsiString P_HEADS_DATA[1500];
    // ↑2007.10.4 ミラー仕様対応

    // 2017.08.17 YX-83追加_S
    int P_FL; // 斜行流ﾌﾗｸﾞ(0:台形流,1:斜行流)
    int P_YX836; // YX-836ﾌﾗｸﾞ
    // 2017.08.17 YX-83追加_E

    // ↓2002/10/24
    bool __stdcall Write_Ateita_Data_Sub(AnsiString, AnsiString, AnsiString,
        AnsiString, long, AnsiString, AnsiString);
    bool __stdcall Write_M_Gskt_Data_Sub(AnsiString, bool, AnsiString);
    bool __stdcall Write_D_Gskt_Data_Sub(AnsiString, AnsiString, AnsiString,
        AnsiString, long, AnsiString, AnsiString, AnsiString, long, AnsiString);
    bool __stdcall Write_D_Gskt_Data_Distance();
    bool __stdcall Write_E_Gskt_Data_Sub(AnsiString, AnsiString, AnsiString,
        AnsiString, long, AnsiString, AnsiString, AnsiString, long, AnsiString);
    bool __stdcall Write_E_Gskt_Data_Distance(void);
    bool __stdcall Search_HD_PLATE_3(AnsiString, AnsiString, AnsiString,
        AnsiString);
    bool __stdcall Search_HD_BFRG(AnsiString, AnsiString, AnsiString);
    bool __stdcall Search_HD_D_GASKET(AnsiString, AnsiString, AnsiString);
    bool __stdcall Search_HD_E_GASKET(AnsiString, AnsiString, AnsiString);
    // ↑2002/10/24

    bool __fastcall Hontai_Data_Sakusei(void); // ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ，孔明ﾃﾞｰﾀ 作成
    bool __fastcall Search_HD_PLATE(AnsiString Key); // ﾌﾟﾚｰﾄ部品ﾏｽﾀ 検索
    bool __fastcall Syukei_Anaake_Data(AnsiString Syurui, AnsiString Model);
    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ集計
    bool __fastcall Write_Hontai_Data(void); // ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ(ﾌﾟﾚｰﾄ本体) CSVﾌｧｲﾙ作成
    bool __fastcall Write_Anaake_Data(AnsiString Syurui, AnsiString Model);
    // ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ(ﾌﾟﾚｰﾄ孔明) CSVﾌｧｲﾙ作成
    bool __fastcall Syukei_Hontai_Yobisu(AnsiString Kata1, AnsiString Kata2,
        AnsiString Siyou); // ﾌﾟﾚｰﾄ本体予備数ﾃﾞｰﾀ集計
    // ↓2002/10/23
    bool __fastcall Write_Ateita_Data(void); // 当板 CSVﾌｧｲﾙ作成
    // ↑2002/10/23
    bool __fastcall Plate_Data_Sakusei(void); // ﾌﾟﾚｰﾄ部品　ﾃﾞｰﾀ作成
    bool __fastcall Search_HD_PLATE_2(AnsiString Key, AnsiString PType,
        AnsiString PCode);
    bool __fastcall Write_PlateBuhin_Data(bool ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    bool __fastcall Write_PlateBuhin_Data_GSKT(bool ret);
    // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成(ｶﾞｽｹｯﾄ用)
    bool __fastcall Write_HeartsBuhin_Data(AnsiString s_Mat_Code);
    // HEARTS用ﾃﾞｰﾀCSV作成
    // ↓2002/10/23
    bool __fastcall Get_Gskt_MastKey(int DEmode); // ガスケット検索キー取得
    bool __fastcall Write_M_Gskt_Data(void); // 中間プレートガスケット CSVﾌｧｲﾙ作成
    // 2006.01.10 Mﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(WX専用)ｻﾌﾞﾙｰﾁﾝ追加
    bool __fastcall Write_WXM_Gskt_Data(void); // 中間ﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(WX専用) CSVﾌｧｲﾙ作成
    // ******************************************
    bool __fastcall Write_D_Gskt_Data(void); // Ｄプレートガスケット CSVﾌｧｲﾙ作成
    bool __fastcall Write_E_Gskt_Data(void); // Ｅプレートガスケット CSVﾌｧｲﾙ作成
    // 2006.01.10 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(RX-00専用) CSVﾌｧｲﾙ作成ｻﾌﾞﾙｰﾁﾝ追加
    bool __fastcall Write_RX00D_Gskt_Data(void);
    // Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(RX-00専用) CSVﾌｧｲﾙ作成
    bool __fastcall Write_RX00E_Gskt_Data(void);
    // Eﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(RX-00専用) CSVﾌｧｲﾙ作成
    bool __fastcall Write_WX_CE_Gskt_Data(void);
    // Eﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(WXｶｾｯﾄ専用) CSVﾌｧｲﾙ作成
    // **********************************************************
    int __stdcall Search_GSKT_Yobi(AnsiString PartsCD, AnsiString MatCD);

    // ↑2002/10/23
public:
    bool __fastcall Excute(HeadsData* cHeadsData); // ﾌﾟﾚｰﾄ部品展開処理 実行
    // bool __fastcall Syukei_Buhin_Yobisu(AnsiString s_Katasiki, AnsiString a_Zaisitu);                // ﾌﾟﾚｰﾄ部品予備数ﾃﾞｰﾀ集計
    int __fastcall Get_Gskt_YobiSu(int i_TmpCd, AnsiString MatCD);

};

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ部品展開処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall PlateDetails(HeadsData* pcHeadsData) {
    bool ret;

    TPlateDetails plateDetails; // ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ，孔明ﾃﾞｰﾀ 作成Class自動生成,関数終了後破棄

    ret = plateDetails.Excute(pcHeadsData); // ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ，孔明ﾃﾞｰﾀ 作成実行

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ部品展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Excute(HeadsData* pcHeadsData) {
    bool bRet;

    // HEADSﾃｷｽﾄﾃﾞｰﾀをｾｯﾄ
    m_pcHeadsData = pcHeadsData;

    // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString型配列に取得
    bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
    if (!bRet)
        return (false);

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『ﾌﾟﾚｰﾄ部品展開処理』開始      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『ﾌﾟﾚｰﾄ部品展開処理』開始      *********");
    Write_Error_Log("*******************************************************");

    // ***********************************
    // ***  ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ，孔明ﾃﾞｰﾀ作成
    // ***********************************
    Hontai_Data_Sakusei();

    // ↓2002/10/24
    Write_Ateita_Data(); // 当板 CSVﾌｧｲﾙ作成
    // ↑2002/10/24

    // ***********************************
    // ***  ﾌﾟﾚｰﾄ部品　ﾃﾞｰﾀ作成
    // ***********************************
    Plate_Data_Sakusei();
    bMouAteFlg = false;

    // 2017.08.17 YX-83追加_S
    // ***********************************
    // ***  斜行流フラグ
    // ***********************************
    // 2022.04.21 CX-01D追加_S
    if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ") {
        P_FL = 1; // 斜行流
    }
    //if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
    else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ") {
    // 2022.04.21 CX-01D追加_E
        P_FL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") {
        P_FL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
        P_FL = 1; // 斜行流
    }
    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        P_FL = 1; // 斜行流
    }
    else {
        P_FL = 0; // 台形流
    }
    // 2017.08.17 YX-83追加_E

    // ↓2002/10/24
    // 2006.01.10 WXの場合Write_WXM_Gskt_Data()でCSVﾌｧｲﾙを作成する
    // Write_M_Gskt_Data();   // 中間プレートガスケット CSVﾌｧｲﾙ作成
    if (G_KS_Syurui == "ＷＸ") {
        Write_WXM_Gskt_Data(); // Mﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(WX専用) CSVﾌｧｲﾙ作成
    }
    // 2021.09.22 WH-30追加_S
    else if (G_KS_Syurui == "ＷＨ") {
        Write_WXM_Gskt_Data(); // Mﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(WX専用) CSVﾌｧｲﾙ作成
    }
    // 2021.09,22 WH-30追加_E
    else {
        Write_M_Gskt_Data(); // 中間プレートガスケット CSVﾌｧｲﾙ作成
    }
    // ***********************************************************
    Get_Gskt_MastKey(0); // ガスケット検索キー取得
    // 2006.01.10 RX-00の場合Write_RX00D_Gskt_Data()でCSVﾌｧｲﾙを作成する
    // Write_D_Gskt_Data();   // Ｄプレートガスケット CSVﾌｧｲﾙ作成
    if (G_KS_Syurui == "ＲＸ" && G_KS_Model == "００") {
        Write_RX00D_Gskt_Data(); // Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(RX-00専用) CSVﾌｧｲﾙ作成
    }
    // 2023.07.11 GX-50の場合Write_RX00D_Gskt_Data()でCSVﾌｧｲﾙを作成する_S
    else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "５０") {
        Write_RX00D_Gskt_Data(); // Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(RX-00専用) CSVﾌｧｲﾙ作成
    }
    // 2023.07.11 GX-50の場合Write_RX00D_Gskt_Data()でCSVﾌｧｲﾙを作成する_E
    else {
        Write_D_Gskt_Data(); // Ｄプレートガスケット CSVﾌｧｲﾙ作成
    }
    // ******************************************************************
    Get_Gskt_MastKey(1); // ガスケット検索キー取得
    // 2006.01.10 RX-00の場合Write_RX00E_Gskt_Data()でCSVﾌｧｲﾙを作成する
    // WXのｶｾｯﾄの場合Write_WX_CE_Gskt_Data()でCSVﾌｧｲﾙを作成する
    // Write_E_Gskt_Data();   // Ｅプレートガスケット CSVﾌｧｲﾙ作成
    int iSuryo;
    int iMod;
    if (G_KS_Syurui == "ＲＸ" && G_KS_Model == "００") {
        Write_RX00E_Gskt_Data(); // Eﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(RX-00専用) CSVﾌｧｲﾙ作成
    }
    // 2023.07.11 GX-50の場合Write_RX00E_Gskt_Data()でCSVﾌｧｲﾙを作成する_S
    else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "５０") {
        Write_RX00E_Gskt_Data(); // Eﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(RX-00専用) CSVﾌｧｲﾙ作成
    }
    // 2023.07.11 GX-50の場合Write_RX00E_Gskt_Data()でCSVﾌｧｲﾙを作成する_E
    else if (G_KS_Syurui == "ＷＸ") {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // 偶数 Ｅカセット
            Write_WX_CE_Gskt_Data(); // Eﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(WXｶｾｯﾄ専用) CSVﾌｧｲﾙ作成
        }
        else {
            // 奇数 Ｅプレート
            Write_E_Gskt_Data(); // Ｅプレートガスケット CSVﾌｧｲﾙ作成
        }
    }
    // 2021.09.22 WH-30追加_S
    else if (G_KS_Syurui == "ＷＨ") {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // 偶数 Ｅカセット
            Write_WX_CE_Gskt_Data(); // Eﾌﾟﾚｰﾄｶﾞｽｹｯﾄ(WXｶｾｯﾄ専用) CSVﾌｧｲﾙ作成
        }
        else {
            // 奇数 Ｅプレート
            Write_E_Gskt_Data(); // Ｅプレートガスケット CSVﾌｧｲﾙ作成
        }
    }
    // 2021.09.22 WH-30追加_E
    else {
        Write_E_Gskt_Data(); // Ｅプレートガスケット CSVﾌｧｲﾙ作成
    }
    // ******************************************************************
    // ↑2002/10/24

    // ﾛｸﾞ作成
    Write_Log("********************************************************");
    Write_Log("**********     『ﾌﾟﾚｰﾄ部品展開処理』終了      **********");
    Write_Log("********************************************************");
    Write_Error_Log("*******************************************************");
    Write_Error_Log("**********     『ﾌﾟﾚｰﾄ部品展開処理』終了      *********");
    Write_Error_Log("*******************************************************");

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ，孔明ﾃﾞｰﾀ　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Hontai_Data_Sakusei(void) {

    int i;
    int i_Mojisu;
    AnsiString s_Text;

    AnsiString s_Syurui; // HEADSﾃﾞｰﾀ 種類(RecNo 541)
    AnsiString s_Model; // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ(RecNo 542)
    AnsiString s_Key;

    // 2004.04.23 変数初期化位置変更
    P_PltYobiA = 0; // 予備品のAプレート枚数
    P_PltYobiB = 0; // 予備品のBプレート枚数
    P_PltYobiD = 0; // 予備品のDプレート枚数
    P_PltYobiE = 0; // 予備品のEプレート枚数
    // *****************************

    P_YobiMemoCnt = 0;
    // ****************************************************************************
    // ***                                                                      ***
    // ***                        プレート型式の決定                            ***
    // ***                                                                      ***
    // ****************************************************************************
    s_Syurui = P_HEADS_DATA[34].TrimRight(); // HEADSﾃﾞｰﾀ 種類
    P_HeadsVer = P_HEADS_DATA[1267].TrimRight(); // HEADSﾃﾞｰﾀ ヴァージョン
    // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // s_Model  = P_HEADS_DATA[35].TrimRight().SubString(1,2); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ

    // RecNo.542ﾓﾃﾞﾙの取得方法の変更
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // 2009.06.19 UX-100R追加
    // EXの場合
    // s_Model後端部に"C","P","R","A","B"がある場合
    // その文字を順にs_Modelから除いていく
    //
    // EX以外の場合
    // s_Model後端部に"C","P","A","B"がある場合
    // その文字を順にs_Modelから除いていく
    // 次に条件に合わせて以下の文字ををs_Modelから除く
    // 最後の文字が"D"の場合
    // ：最後から２文字目の文字が"C","P","A","B"の場合
    // ：その文字を順にs_Modelから除いていく
    // ：次に最後から２文字目の文字(数字)を除く
    // 最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
    // その他の場合                       ：最後の文字(数字)
    ////2006.02.06 SX-90M,L追加
    ////s_Model後端部に"C","P","R","A","B"がある場合
    ////その文字を順にs_Modelから除いていく
    ////次に条件に合わせて以下の文字ををs_Modelから除く
    ////EXの場合                           ：除かない
    ////2008.07.24 CX-10D追加
    ////最後の文字が"D"の場合
    ////　　：最後から２文字目の文字が"C","P","R","A","B"の場合
    ////　　：その文字を順にs_Modelから除いていく
    ////　　：次に最後から２文字目の文字(数字)を除く
    ////*********************
    ////最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
    ////その他の場合                       ：最後の文字(数字)
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // if (i_Mojisu == 3){
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,2);
    // } else if (i_Mojisu == 4){
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(4,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,3);
    // } else {
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,2);
    // }
    // } else if (i_Mojisu == 5){
    // s_Model = P_HEADS_DATA[35].TrimRight().SubString(1,3);
    // }
    if (s_Model.SubString(s_Model.Length(), 1) == "B") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    if (s_Model.SubString(s_Model.Length(), 1) == "A") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    // 2009.06.19 UX-100R追加
    // if ( s_Model.SubString(s_Model.Length(),1) == "R" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.SubString(s_Model.Length(),
        1) == "R") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    // **********************
    if (s_Model.SubString(s_Model.Length(), 1) == "P") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    if (s_Model.SubString(s_Model.Length(), 1) == "C") {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    if (s_Syurui.SubString(1, 2) == "EX") {
        // 2008.07.24 CX-10D追加
    }
    else if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "B") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "A") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        // 2009.06.19 UX-100R追加
        // if ( s_Model.SubString(s_Model.Length()-1,1) == "R" ) {
        // s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
        // }
        // **********************
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "P") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }
        if (s_Model.SubString(s_Model.Length() - 1, 1) == "C") {
            s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        }

        s_Model = s_Model.SubString(1, s_Model.Length() - 2) + "D";
        // *********************
    }
    else if (s_Model.SubString(s_Model.Length(), 1).ToIntDef(999) == 999) {
        s_Model = s_Model.SubString(1, s_Model.Length() - 2) + s_Model.SubString
            (s_Model.Length(), 1);
    }
    else {
        s_Model = s_Model.SubString(1, s_Model.Length() - 1);
    }
    // ***********************

    // ↓2002/10/23
    G_S_Model = s_Syurui + "-" + s_Model;
    // ↑2002/10/23

    G_K_Syurui = ""; // 型式種類
    G_K_Model1 = ""; // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式1)
    G_K_Model2 = ""; // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2)
    G_KS_Syurui = ""; // 総称種類
    G_KS_Model = ""; // 総称ﾓﾃﾞﾙ
    G_Model_Zen1 = ""; // 型式ﾓﾃﾞﾙ全角1
    G_Model_Zen2 = ""; // 型式ﾓﾃﾞﾙ全角2

    G_Log_Text = "ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙを、種類『" + s_Syurui + "』モデル『" + s_Model +
        "』で検索開始。";
    Write_Log(G_Log_Text);

    // 2004.06.07 PHENKAN.CSV検索変更
    int endflg = 1;
    // ******************************

    for (i = 0; i < G_PHENKAN_RecCnt; i++) {
        if (G_PHENKAN[i][0] == s_Syurui && G_PHENKAN[i][1] == s_Model) {

            // 2004.06.07 PHENKAN.CSV検索変更
            endflg = 0;
            // ******************************

            P_MixFlg = G_PHENKAN[i][4]; // ﾐｯｸｽﾌﾗｸﾞ

            G_K_Syurui = G_PHENKAN[i][0]; // 型式種類
            G_K_Model1 = G_PHENKAN[i][5]; // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式1)
            G_K_Model2 = G_PHENKAN[i][6]; // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2)
            G_KS_Syurui = G_PHENKAN[i][2]; // 総称種類
            G_KS_Model = G_PHENKAN[i][3]; // 総称ﾓﾃﾞﾙ
            G_Model_Zen1 = G_PHENKAN[i][7]; // 型式ﾓﾃﾞﾙ全角1
            G_Model_Zen2 = G_PHENKAN[i][8]; // 型式ﾓﾃﾞﾙ全角2

            // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀ作成処理用(Plate_Data_Sakusei())
            P_SSyurui = G_PHENKAN[i][2]; // 総称種類
            P_SModel = G_PHENKAN[i][3]; // 総称ﾓﾃﾞﾙ

            break;
        }
    }

    // 2004.06.07 PHENKAN.CSV検索変更
    if (endflg == 1) {
        return true;
    }
    // ******************************

    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // 2009.06.19 UX-100R追加
    // EXの場合
    // s_Model後端部に"C","P","R","A","B"がある場合
    // P_SoSyoAsciiのG_KS_Modelの後ろにその文字を追加する
    //
    // EX以外の場合
    // s_Model後端部に"C","P","A","B"がある場合
    // P_SoSyoAsciiのG_KS_Modelの後ろにその文字を追加する
    // 但しs_Modelの最後の文字が"D"の場合
    // P_SoSyoAsciiのG_KS_Modelの"D"の前の位置に"C","P","A","B"をを追加する
    ////2006.02.06 SX-90M,L追加
    ////P_HEADS_DATA[35]後端部に"C","P","R","A","B"がある場合
    ////P_SoSyoAsciiのZenToHan(G_KS_Model)の後ろにその文字を追加する
    ////2008.07.24 CX-10D追加
    ////但しP_HEADS_DATA[35]の最後の文字が"D"の場合
    ////ZenToHan(G_KS_Model)の"D"の前の位置に"C","P","R","A","B"をを追加する
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    // if (s_Text.ToIntDef(999) != 999){
    // P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "-" + P_HEADS_DATA[1267].TrimRight();;
    // } else {
    // P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();;;
    // }
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R追加
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D追加
    // P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = ZenToHan(G_KS_Model).SubString(1,
            ZenToHan(G_KS_Model).Length() - 1) + s_Text + "D";
        P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        P_SoSyoAscii = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) +
            s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    G_Log_Text = "型式種類            『" + G_K_Syurui + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式1) 『" + G_K_Model1 + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2) 『" + G_K_Model2 + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "総称種類            『" + G_KS_Syurui + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "総称ﾓﾃﾞﾙ            『" + G_KS_Model + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "型式ﾓﾃﾞﾙ全角1       『" + G_Model_Zen1 + "』を取得。";
    Write_Log(G_Log_Text);
    G_Log_Text = "型式ﾓﾃﾞﾙ全角2       『" + G_Model_Zen2 + "』を取得。";
    Write_Log(G_Log_Text);

    G_Log_Text = "ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙ  検索終了。";
    Write_Log(G_Log_Text);

    // 2017.08.17 YX-83追加_S
    // ***********************************
    // ***  ＹＸ−８３フラグ
    // ***********************************
    if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        if( P_HEADS_DATA[1013].TrimRight() != "08" ) {
            P_YX836 = 1; // YX-836(ﾐｯｸｽ型式1)
        }
        else {
            P_YX836 = 0; // YX-838
        }
    }
    else {
        P_YX836 = 0; // YX-83ではない
    }
    // 2017.08.17 YX-83追加_E

    // ****************************************************************************
    // ***                                                                      ***
    // ***                       ミックス型式１ 部分　作成                      ***
    // ***                                                                      ***
    // ****************************************************************************
    G_Log_Text = "ﾐｯｸｽﾌﾗｸﾞ            『" + P_MixFlg + "』を取得。";
    Write_Log(G_Log_Text);
    if (P_MixFlg != "0" && P_MixFlg != "1") {
        return true;
    }

    // **********************************
    // ***  ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ　検索
    // **********************************
    // 02.10.16 検索ｷｰ変更
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // 2009.06.19 UX-100R追加
    // EXの場合
    // s_Model後端部に"C","P","R","A","B"がある場合
    // s_KeyのG_K_Model1の後ろにその文字を追加する
    //
    // EX以外の場合
    // s_Model後端部に"C","P","A","B"がある場合
    // s_KeyのG_K_Model1の後ろにその文字を追加する
    // 但しs_Modelの最後の文字が"D"の場合
    // s_KeyのG_K_Model1の"D"の前の位置に"C","P","A","B"をを追加する
    ////2006.02.06 SX-90M,L追加
    ////P_HEADS_DATA[35]後端部に"C","P","R","A","B"がある場合
    ////s_KeyのG_K_Model1の後ろにその文字を追加する
    ////2008.07.24 CX-10D追加
    ////但しP_HEADS_DATA[35]の最後の文字が"D"の場合
    ////s_KeyのG_K_Model1の"D"の前の位置に"C","P","R","A","B"をを追加する
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    ////   P_SoSyoAscii = s_Key;
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R追加
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D追加
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model1.SubString(1, G_K_Model1.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    Search_HD_PLATE(s_Key);

    // **********************************
    // ***  ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀの検索，集計
    // **********************************
    // 2017.08.17 YX-83追加_S
    //Syukei_Anaake_Data(G_K_Syurui, G_K_Model1);
    //YX-836の場合はﾌﾟﾚｰﾄ板厚を加えて比較
    //YX-836 元板厚
    if (P_YX836 == 1) {
        Syukei_Anaake_Data(G_K_Syurui, "83" + P_HEADS_DATA[1013].SubString(2, 1));
    } else {
        Syukei_Anaake_Data(G_K_Syurui, G_K_Model1);
    }
    // 2017.08.17 YX-83追加_E

    // **********************************
    // ***  予備品ﾃﾞｰﾀの検索，集計
    // **********************************
    // P_Yobisu = 0;

    // *************************************************
    // ***  部品展開ﾃﾞｰﾀ(ﾌﾟﾚｰﾄ本体ﾚｺｰﾄﾞ部) への出力
    // *************************************************
    // 02.09.09 変更
    // P_Katasiki = G_KS_Syurui + "−" + G_Model_Zen1 + HanToZen(P_HEADS_DATA[1258].TrimRight());
    /* 02.09.11 ｺﾒﾝﾄ化
     P_Katasiki = HanToZen(P_HEADS_DATA[1256].TrimRight()) + "−"
     + HanToZen(P_HEADS_DATA[1257].TrimRight())
     + HanToZen(P_HEADS_DATA[1258].TrimRight());
     */
    Write_Hontai_Data();

    // P_Yobisu = 0;

    // *************************************************
    // ***  部品展開ﾃﾞｰﾀ(ﾌﾟﾚｰﾄ孔明ﾚｺｰﾄﾞ部) への出力
    // *************************************************
    // 2017.08.17 YX-83追加_S
    //Write_Anaake_Data(G_K_Syurui, G_K_Model1);
    //YX-836の場合はﾌﾟﾚｰﾄ板厚を加えて比較
    //YX-836 DEﾌﾟﾚｰﾄ板厚(t8固定)
    if (P_YX836 == 1) {
        Write_Anaake_Data(G_K_Syurui, "83" + P_HEADS_DATA[1013].SubString(2, 1));
    } else {
        Write_Anaake_Data(G_K_Syurui, G_K_Model1);
    }
    // 2017.08.17 YX-83追加_E

    // 2017.08.17 YX-83追加_S
    // ***********************************
    // ***  ＹＸ−８３フラグ
    // ***********************************
    if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
        if( P_HEADS_DATA[1013].TrimRight() != "08" ) {
            P_YX836 = 2; // YX-836(ﾐｯｸｽ型式2)
        }
        else {
            P_YX836 = 0; // YX-838
        }
    }
    else {
        P_YX836 = 0; // YX-83ではない
    }
    // 2017.08.17 YX-83追加_E

    // ****************************************************************************
    // ***                                                                      ***
    // ***                       ミックス型式２ 部分　作成                      ***
    // ***                                                                      ***
    // ****************************************************************************
    if (P_MixFlg != "1") {
        // 2017.08.17 YX-83追加_S
        //return true;
        if (P_YX836 == 2) {
        }
        else {
            return true;
        }
        // 2017.08.17 YX-83追加_E
    }

    // **********************************
    // ***  ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ　検索
    // **********************************
    // 02.10.16 検索ｷｰ変更
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // 2009.06.19 UX-100R追加
    // EXの場合
    // s_Model後端部に"C","P","R","A","B"がある場合
    // s_KeyのG_K_Model2の後ろにその文字を追加する
    //
    // EX以外の場合
    // s_Model後端部に"C","P","A","B"がある場合
    // s_KeyのG_K_Model2の後ろにその文字を追加する
    // 但しs_Modelの最後の文字が"D"の場合
    // s_KeyのG_K_Model2の"D"の前の位置に"C","P","A","B"をを追加する
    ////2006.02.06 SX-90M,L追加
    ////P_HEADS_DATA[35]後端部に"C","P","R","A","B"がある場合
    ////s_KeyのG_K_Model2の後ろにその文字を追加する
    ////2008.07.24 CX-10D追加
    ////但しP_HEADS_DATA[35]の最後の文字が"D"の場合
    ////s_KeyのG_K_Model2の"D"の前の位置に"C","P","R","A","B"をを追加する
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    ////2006.01.10 MIXﾌﾟﾚｰﾄ2はG_K_Model2(型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2))で検索する
    ////if (s_Text.ToIntDef(999) != 999){
    ////   s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    ////} else {
    ////   s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    ////}
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model2 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model2 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    ////*************************************************************
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R追加
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (s_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D追加
    // s_Key = G_K_Syurui + "-" + G_K_Model2 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model2.SubString(1, G_K_Model2.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model2 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    // 2017.08.17 YX-83追加_S
    if (P_YX836 == 2) {
        s_Key = "YX-83" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // 2017.08.17 YX-83追加_E

    Search_HD_PLATE(s_Key);

    // **********************************
    // ***  ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀの検索，集計
    // **********************************
    // 2017.08.17 YX-83追加_S
    //Syukei_Anaake_Data(G_K_Syurui, G_K_Model2);
    //YX-836の場合はﾌﾟﾚｰﾄ板厚を加えて比較
    //YX-836 DEﾌﾟﾚｰﾄ板厚(t8固定)
    if (P_YX836 == 2) {
        Syukei_Anaake_Data(G_K_Syurui, "838");
    } else {
        Syukei_Anaake_Data(G_K_Syurui, G_K_Model2);
    }
    // 2017.08.17 YX-83追加_E

    // **********************************
    // ***  予備品ﾃﾞｰﾀの検索，集計
    // **********************************
    // P_Yobisu = 0;

    // *************************************************
    // ***  部品展開ﾃﾞｰﾀ(ﾌﾟﾚｰﾄ本体ﾚｺｰﾄﾞ部) への出力
    // *************************************************
    // 02.09.09 変更
    // P_Katasiki = G_KS_Syurui + "−" + G_Model_Zen2 + HanToZen(P_HEADS_DATA[1258].TrimRight());
    /* 02.09.11 ｺﾒﾝﾄ化
     P_Katasiki = HanToZen(P_HEADS_DATA[1256].TrimRight()) + "−"
     + HanToZen(P_HEADS_DATA[1257].TrimRight())
     + HanToZen(P_HEADS_DATA[1258].TrimRight());
     */
    Write_Hontai_Data();
    // *************************************************
    // ***  部品展開ﾃﾞｰﾀ(ﾌﾟﾚｰﾄ孔明ﾚｺｰﾄﾞ部) への出力
    // *************************************************
    // 2017.08.17 YX-83追加_S
    //Write_Anaake_Data(G_K_Syurui, G_K_Model2);
    //YX-836の場合はﾌﾟﾚｰﾄ板厚を加えて比較
    //YX-836 DEﾌﾟﾚｰﾄ板厚(t8固定)
    if (P_YX836 == 2) {
        Write_Anaake_Data(G_K_Syurui, "838");
    } else {
        Write_Anaake_Data(G_K_Syurui, G_K_Model2);
    }
    // 2017.08.17 YX-83追加_E

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ　検索
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Search_HD_PLATE(AnsiString Key) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL; // SQL文

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PLATE";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + Key + "'";
    s_SQL = s_SQL + "    AND PLATE_TYPE = 'D'";
    s_SQL = s_SQL + "    AND PARTS_CODE = 3";

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
        P_Mat_Code = "";
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_Mat_Code = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
    }

    delete wkQuery;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ　集計
// 概  要      ：
// 引  数      ： Syurui :ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙ内 ﾌﾟﾚｰﾄ型式種類
// ： Model  :ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙ内 ﾐｯｸｽ型式
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： TYouryosyo 処理
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Syukei_Anaake_Data(AnsiString Syurui,
    AnsiString Model) {
    // CSV読込用
    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    char token[100][100]; /* トークンバッファ */

    AnsiString s_Ana_Text[5];
    AnsiString wk_FilePass;
    AnsiString s_Text;
    int i_Pos;
    int i_ZaisituFlg;
    // 2016.04.28 CXW-00追加
    AnsiString s_Syurui;
    AnsiString s_Model;
    // *********************

    P_Suryo = 0; // 1台分数量
    P_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)
    i_ZaisituFlg = 0; // 材質ﾌﾗｸﾞ
    P_YobisuPltAll = 0; // プレート本体用予備品数
    P_PltGsktYobiA = 0;
    P_PltGsktYobiB = 0;
    P_PltGsktYobiD = 0;
    P_PltGsktYobiE = 0;
    // 2004.01.19 変数追加
    P_PltGsktYobiS = 0; // 予備品のプレートに付く真空時目玉盲部補強ガスケット枚数
    // *******************

    // 2004.04.23 変数初期化位置変更
    // 2004.01.16 変数追加
    // P_PltYobiA = 0;      // 予備品のAプレート枚数
    // P_PltYobiB = 0;      // 予備品のBプレート枚数
    // P_PltYobiD = 0;      // 予備品のDプレート枚数
    // P_PltYobiE = 0;      // 予備品のEプレート枚数
    // *******************

    // *******************
    // 2007.10.03 ﾐﾗｰ仕様対応 ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽの仕様ﾀｲﾌﾟｺｰﾄﾞ追加
    AnsiString sMirrorCode;
    // *******************

    // *****************************
    // 2007.10.03 ﾐﾗｰ仕様対応 ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽを変更
    sMirrorCode = AnsiString(m_pcHeadsData->GetSpecCode());

    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽ
    // wk_FilePass = G_AnaFilePass + G_AnaakeFileNM;
	// wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + ".csv";
	//2023/4/21 製造番号桁数追加対応 Mod
	//wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + sMirrorCode + ".csv";
	wk_FilePass = G_AnaFilePass + "HOL" + GetFileSeizouBangou(G_SAKUBAN) + sMirrorCode + ".csv";
    // *****************************

    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // 読込み可能の間
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

            s_Ana_Text[0] = token[0]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式
            s_Ana_Text[1] = token[1]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式(?) 英数1文字
            s_Ana_Text[2] = token[2]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式 'A' or 'B'
            s_Ana_Text[3] = token[3]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 仕様(?)
            s_Ana_Text[4] = token[4]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 1台分数量

            // 2009.06.19 UX-100R追加
            // EXの場合
            // s_Ana_Text[0]後端部に"C","P","R","A","B"がある場合
            // その文字を順にs_Ana_Text[0]から除いていく
            //
            // EX以外の場合
            // s_Ana_Text[0]後端部に"C","P","A","B"がある場合
            // その文字を順にs_Ana_Text[0]から除いていく
            // 次に条件に合わせて以下の文字ををs_Ana_Text[0]から除く
            // 最後の文字が"D"の場合
            // ：最後から２文字目の文字が"C","P","A","B"の場合
            // ：その文字を順にs_Ana_Text[0]から除いていく
            // ：次に最後から２文字目の文字(数字)を除く
            // 最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
            // その他の場合                       ：最後の文字(数字)
            ////2006.02.06 SX-90M,L追加
            ////s_Ana_Text[0]後端部に"C","P","R","A","B"がある場合
            ////その文字を順にs_Ana_Text[0]から除いていく
            ////次に条件に合わせて以下の文字ををs_Ana_Text[0]から除く
            ////EXの場合                           ：除かない
            ////2008.07.24 CX-10D追加
            ////最後の文字が"D"の場合
            ////　　：最後から２文字目の文字が"C","P","R","A","B"の場合
            ////　　：その文字を順にs_Ana_Text[0]から除いていく
            ////　　：次に最後から２文字目の文字(数字)を除く
            ////*********************
            ////最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
            ////その他の場合                       ：最後の文字(数字)
            // **********************
            //
            ////↓2002/10/23
            ////D,Eﾌﾟﾚｰﾄの時のﾃﾞｰﾀを記憶する
            // if( s_Ana_Text[1] == "D" ){
            // if ( s_Ana_Text[0].SubString(s_Ana_Text[0].Length(),1).ToIntDef(999)==999 ){
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_D_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // } else {
            // G_D_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-2);
            // }
            // }else{
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_D_GsktType    = s_Ana_Text[0].TrimRight();
            // } else {
            // G_D_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // }
            // }
            // G_D_GsktABKubun = s_Ana_Text[2];
            // G_D_GsktAna     = s_Ana_Text[3];
            // }
            // if( s_Ana_Text[1] == "E" ){
            // if ( s_Ana_Text[0].SubString(s_Ana_Text[0].Length(),1).ToIntDef(999)==999 ){
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_E_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // } else {
            // G_E_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-2);
            // }
            // }else{
            // if (s_Ana_Text[0].SubString(1,2) == "EX" ) {
            // G_E_GsktType    = s_Ana_Text[0].TrimRight();
            // } else {
            // G_E_GsktType    = s_Ana_Text[0].SubString(1,s_Ana_Text[0].Length()-1);
            // }
            // }
            // G_E_GsktABKubun = s_Ana_Text[2];
            // G_E_GsktAna     = s_Ana_Text[3];
            // }
            ////↑2002/10/23
            //
            // D,Eﾌﾟﾚｰﾄの時のﾃﾞｰﾀを記憶する
            if (s_Ana_Text[1] == "D") {
                s_Text = s_Ana_Text[0].TrimRight();
                if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // 2009.06.19 UX-100R追加
                // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
                if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                    (s_Text.Length(), 1) == "R") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // **********************
                if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(1, 2) == "EX") {
                    G_D_GsktType = s_Text;
                    // 2008.07.24 CX-10D追加
                }
                else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    // 2009.06.19 UX-100R追加
                    // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                    // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                    // }
                    // **********************
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }

                    G_D_GsktType =
                        s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    // *********************
                }
                else if (s_Text.SubString(s_Text.Length(),
                    1).ToIntDef(999) == 999) {
                    G_D_GsktType = s_Text.SubString(1, s_Text.Length() - 2) +
                        s_Text.SubString(s_Text.Length(), 1);
                }
                else {
                    G_D_GsktType = s_Text.SubString(1, s_Text.Length() - 1);
                }
                G_D_GsktABKubun = s_Ana_Text[2];
                G_D_GsktAna = s_Ana_Text[3];
            }
            if (s_Ana_Text[1] == "E") {
                s_Text = s_Ana_Text[0].TrimRight();
                if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // 2009.06.19 UX-100R追加
                // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
                if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                    (s_Text.Length(), 1) == "R") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                // **********************
                if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 1);
                }
                if (s_Text.SubString(1, 2) == "EX") {
                    G_E_GsktType = s_Text;
                    // 2008.07.24 CX-10D追加
                }
                else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    // 2009.06.19 UX-100R追加
                    // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                    // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                    // }
                    // **********************
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }
                    if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                        s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    }

                    G_E_GsktType =
                        s_Text.SubString(1, s_Text.Length() - 2) + "D";
                    // *********************
                }
                else if (s_Text.SubString(s_Text.Length(),
                    1).ToIntDef(999) == 999) {
                    G_E_GsktType = s_Text.SubString(1, s_Text.Length() - 2) +
                        s_Text.SubString(s_Text.Length(), 1);
                }
                else {
                    G_E_GsktType = s_Text.SubString(1, s_Text.Length() - 1);
                }
                G_E_GsktABKubun = s_Ana_Text[2];
                G_E_GsktAna = s_Ana_Text[3];
            }
            // ***********************

            // 1台分数量の集計
            // 2009.06.19 UX-100R追加
            // EXの場合
            // s_Ana_Text[0]後端部に"C","P","R","A","B"がある場合
            // その文字を順にs_Ana_Text[0]から除いていく
            //
            // EX以外の場合
            // s_Ana_Text[0]後端部に"C","P","A","B"がある場合
            // その文字を順にs_Ana_Text[0]から除いていく
            // 次に条件に合わせて以下の文字ををs_Ana_Text[0]から除く
            // 最後の文字が"D"の場合
            // ：最後から２文字目の文字が"C","P","A","B"の場合
            // ：その文字を順にs_Ana_Text[0]から除いていく
            // ：次に最後から２文字目の文字(数字)を除く
            // 最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
            // その他の場合                       ：最後の文字(数字)
            ////2006.02.06 SX-90M,L追加
            ////s_Ana_Text[0]後端部に"C","P","R","A","B"がある場合
            ////その文字を順にs_Ana_Text[0]から除いていく
            ////次に条件に合わせて以下の文字ををs_Ana_Text[0]から除く
            ////EXの場合                           ：除かない
            ////2008.07.24 CX-10D追加
            ////最後の文字が"D"の場合
            ////　　：最後から２文字目の文字が"C","P","R","A","B"の場合
            ////　　：その文字を順にs_Ana_Text[0]から除いていく
            ////　　：次に最後から２文字目の文字(数字)を除く
            ////*********************
            ////最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
            ////その他の場合                       ：最後の文字(数字)
            // **********************
            //
            // if (s_Ana_Text[0].SubString(1,2) == Syurui && s_Ana_Text[0].SubString(4,Model.Length()) == Model){
            s_Text = s_Ana_Text[0].TrimRight();
            if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // 2009.06.19 UX-100R追加
            // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
            if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                (s_Text.Length(), 1) == "R") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // **********************
            if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(1, 2) == "EX") {
                // 2008.07.24 CX-10D追加
            }
            else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                // 2009.06.19 UX-100R追加
                // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                // }
                // **********************
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }

                s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                // *********************
            }
            else if (s_Text.SubString(s_Text.Length(), 1).ToIntDef(999) == 999)
            {
                s_Text = s_Text.SubString(1, s_Text.Length() - 2) +
                    s_Text.SubString(s_Text.Length(), 1);
            }
            else {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }

            // 2017.08.17 YX-83追加_S
            if (P_YX836 != 0) {
                // YX-836はs_Modelにﾌﾟﾚｰﾄ板厚追加
                // s_Ana_Text[0]をそのまま使用
                s_Text = s_Ana_Text[0].TrimRight();
            }
            // 2017.08.17 YX-83追加_E

            // 2016.04.28 CXW-00追加
            if (s_Text.SubString(3, 1) == "W") {
                s_Syurui = s_Text.SubString(1, 3);
                s_Model  = s_Text.SubString(5, Model.Length());
            } else {
                s_Syurui = s_Text.SubString(1, 2);
                s_Model  = s_Text.SubString(4, Model.Length());
            }

            //if (s_Text.SubString(1, 2) == Syurui && s_Text.SubString(4,
            //    Model.Length()) == Model) {
            if (s_Syurui == Syurui && s_Model == Model) {
            // *********************
                // ***********************
                P_Suryo = P_Suryo + s_Ana_Text[4].ToIntDef(0);

                // ***********************************
                // ***  予備数の集計 (02.08.03追加)
                // ***********************************
                P_Yobisu = 0;
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], s_Ana_Text[1], s_Ana_Text[2]+s_Ana_Text[3] );
                // 2004.01.16 変数追加
                // if ( s_Ana_Text[1].TrimRight() == "D" ) {
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], "D", s_Ana_Text[2]+s_Ana_Text[3] );
                // } else if ( s_Ana_Text[1].TrimRight() == "E" ) {
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], "E", s_Ana_Text[2]+s_Ana_Text[3] );
                // } else {
                // Syukei_Hontai_Yobisu(s_Ana_Text[0], "", s_Ana_Text[2]+s_Ana_Text[3] );
                // }
                if (s_Ana_Text[1].TrimRight() == "D") {
                    Syukei_Hontai_Yobisu(s_Ana_Text[0], "D",
                        s_Ana_Text[2] + s_Ana_Text[3]);
                    P_PltYobiD += P_Yobisu; // Dプレートの予備数をメモ
                }
                else if (s_Ana_Text[1].TrimRight() == "E") {
                    Syukei_Hontai_Yobisu(s_Ana_Text[0], "E",
                        s_Ana_Text[2] + s_Ana_Text[3]);
                    P_PltYobiE += P_Yobisu; // Eプレートの予備数をメモ
                    if (s_Ana_Text[2] == "A") { // Aプレート？
                        P_PltYobiA += P_Yobisu; // Aプレートの予備数をメモ
                    }
                    else {
                        P_PltYobiB += P_Yobisu; // Bプレートの予備数をメモ
                    }
                }
                else {
                    Syukei_Hontai_Yobisu(s_Ana_Text[0], "",
                        s_Ana_Text[2] + s_Ana_Text[3]);
                    if (s_Ana_Text[2] == "A") { // Aプレート？
                        P_PltYobiA += P_Yobisu; // Aプレートの予備数をメモ
                    }
                    else {
                        P_PltYobiB += P_Yobisu; // Bプレートの予備数をメモ
                    }
                }
                // *******************
                P_YobisuPltAll += P_Yobisu;

                if (i_ZaisituFlg == 0) {
                    i_ZaisituFlg = 1;

                    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式
                    i_Pos = s_Ana_Text[0].Pos("-");
                    if (i_Pos == 0) {
                        s_Text = s_Ana_Text[0];
                    }
                    else {
                        s_Text = s_Ana_Text[0].SubString(1, i_Pos - 1) + "-" +
                            s_Ana_Text[0].SubString(i_Pos + 1,
                            s_Ana_Text[0].Length() - i_Pos);

                    }
                    P_Katasiki = HanToZen(s_Text);

                    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 材質
                    P_Zaisitu = token[5];
                    P_EZaisitu = token[5];
                }

                P_RecNo++;
            }
            memset(buf, 0x00, sizeof(buf));
        }
    }

    // 該当予備品ﾃﾞｰﾀが存在しなかった場合
    if (P_RecNo == 0)
        P_Yobisu = 0;

    fclose(fp); // 閉じる
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 予備品ﾃﾞｰﾀ　集計処理(ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ,孔明ﾃﾞｰﾀ作成用)
// 概  要      ：
// 引  数      ： AnsiString Kata1 :ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式種類
// ： AnsiString Kata2 :ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式(?) 英数1文字
// ： AnsiString Siyou :ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 仕様(?)
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 02.08.03追加
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Syukei_Hontai_Yobisu(AnsiString Kata1,
    AnsiString Kata2, AnsiString Siyou) {
    // CSV読込用
    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    char token[100][100]; /* トークンバッファ */
    FILE *wkfp;
    bool withGskt;

    AnsiString s_Yobi_Text[7];
    AnsiString wk_FilePass;

    withGskt = false;
    P_RecNo = 0; // ﾚｺｰﾄﾞｶｳﾝﾀ(抽出条件一致件数)

    // -----------------------------------------------
    // 2007.10.03 追加
    // ﾐﾗｰB仕様の場合は、予備品ﾃﾞｰﾀなしとして処理
    if (m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_MRR_B) {
        P_Yobisu = 0;
        return withGskt;
    }
	// 2007.10.03
    // -----------------------------------------------

	// ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽ
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
            s_Yobi_Text[2] = token[5];
            // 予備品ﾃﾞｰﾀ 6列目 ( 'UX-216','UX-20','UX-226'... )
            s_Yobi_Text[3] = token[8]; // 予備品ﾃﾞｰﾀ 予備数( 10, 1, 5, 2, 8, ... )
            s_Yobi_Text[4] = token[18];
            // 予備品ﾃﾞｰﾀ 19列目( 'A1234', 'B1234', 'A0000', ... )
            s_Yobi_Text[5] = token[14]; // 予備品ﾃﾞｰﾀ 14列目( Bガスケット材質CD )
            s_Yobi_Text[6] = token[16]; // 予備品ﾃﾞｰﾀ 16列目( Aガスケット材質CD )

            // 2010.09.21 予備品ﾃﾞｰﾀ 6列目の変更(**-***B→**-***A)
            s_Yobi_Text[2] = s_Yobi_Text[2].Trim();
            if (s_Yobi_Text[2].SubString(s_Yobi_Text[2].Length(), 1) == "B") {
                s_Yobi_Text[2] =
                    s_Yobi_Text[2].SubString(1,
                s_Yobi_Text[2].Length() - 1) + "A";
            }
            // ***************************************************
            // 2020.10.28 予備品ﾃﾞｰﾀ 6列目の変更(**C-***→**-***)_S
            if (s_Yobi_Text[2].SubString(3, 1) == "C") {
                s_Yobi_Text[2] =
                    s_Yobi_Text[2].SubString(1, 2) +
                s_Yobi_Text[2].SubString(4, s_Yobi_Text[2].Length());
            }
            // 2020.10.28 予備品ﾃﾞｰﾀ 6列目の変更(**C-***→**-***)_E

            // 1台分数量の集計
            if (s_Yobi_Text[0].Trim() == "PLTE" && s_Yobi_Text[1].Trim()
                == Kata2 && s_Yobi_Text[2].Trim() == Kata1 && s_Yobi_Text[4]
                .Trim() == Siyou) {
                P_Yobisu = P_Yobisu + s_Yobi_Text[3].ToIntDef(0);

                // 2004.01.15 ガスケット材質判定追加
                // if ( s_Yobi_Text[1] != "D" ) {      // Dプレート以外
                // if ( s_Yobi_Text[5] != "" ) {    // ガスケット材質有り
                // withGskt = true;
                // if ( s_Yobi_Text[4].IsDelimiter("A",1) == true ) {    // Aプレート？
                // P_PltGsktYobiA += s_Yobi_Text[3].ToIntDef(0);      // Aプレートの予備数をメモ
                // } else {
                // P_PltGsktYobiB += s_Yobi_Text[3].ToIntDef(0);      // Bプレートの予備数をメモ
                // }
                // }
                // if ( s_Yobi_Text[1] == "E" ) {      // Eプレート
                // P_PltGsktYobiE += s_Yobi_Text[3].ToIntDef(0);      // Eプレートの予備数をメモ
                // }
                // }
                // if ( s_Yobi_Text[1] == "D" ) {      // Dプレート
                // P_PltGsktYobiD += s_Yobi_Text[3].ToIntDef(0);      // Dプレートの予備数をメモ
                // }
                if (s_Yobi_Text[1] != "D") { // Dプレート以外
                    if (s_Yobi_Text[5] != "") { // ガスケット材質有り
                        withGskt = true;
                        if (s_Yobi_Text[4].IsDelimiter("A", 1) == true)
                        { // Aプレート？
                            P_PltGsktYobiA += s_Yobi_Text[3].ToIntDef(0);
                            // Aプレート(ｶﾞｽｹｯﾄ付)の予備数をメモ
                        }
                        else {
                            P_PltGsktYobiB += s_Yobi_Text[3].ToIntDef(0);
                            // Bプレート(ｶﾞｽｹｯﾄ付)の予備数をメモ
                            // 2004.01.19 変数追加
                            if (s_Yobi_Text[4] == "B*23*") { // Aプレート？
                                P_PltGsktYobiS +=
                                    s_Yobi_Text[3].ToIntDef(0) * 1;
                                // 真空時目玉盲部補強ガスケットの予備数をメモ
                            }
                            else if (s_Yobi_Text[4] == "B123*") {
                                P_PltGsktYobiS +=
                                    s_Yobi_Text[3].ToIntDef(0) / 2;
                                // 真空時目玉盲部補強ガスケットの予備数をメモ
                            }
                            // *******************
                        }
                    }
                    if (s_Yobi_Text[1] == "E") { // Eプレート
                        if (s_Yobi_Text[5] != "") { // ガスケット材質有り
                            P_PltGsktYobiE += s_Yobi_Text[3].ToIntDef(0);
                            // Eプレート(ｶﾞｽｹｯﾄ付)の予備数をメモ
                        }
                    }
                }
                if (s_Yobi_Text[1] == "D") { // Dプレート
                    if (s_Yobi_Text[5] != "") { // ガスケット材質有り
                        withGskt = true;
                        P_PltGsktYobiD += s_Yobi_Text[3].ToIntDef(0);
                        // Dプレート(ｶﾞｽｹｯﾄ付)の予備数をメモ
                    }
                }
                // *********************************

                P_RecNo++;
            }
        }
    }

    // 該当予備品ﾃﾞｰﾀが存在しなかった場合
    if (P_RecNo == 0)
        P_Yobisu = 0;

    fclose(wkfp); // 閉じる
    return withGskt;

}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  CSVﾌｧｲﾙ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： TYouryosyo 処理
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_Hontai_Data(void) {
    AnsiString s_Text;
    AnsiString wk_FilePass;

    long l_Daisu;

    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());

    // 部品展開ﾃﾞｰﾀ　OPEN
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

    // 1  部品分類
    s_Text = "PLATE";
    fputs(s_Text.c_str(), fp);
    // 2  識別子
    s_Text = ",DPLT--";
    fputs(s_Text.c_str(), fp);
    // 3  部品型式
    s_Text = "," + P_Katasiki;
    fputs(s_Text.c_str(), fp);
    // 4  部品名称
    s_Text = ",プレート";
    fputs(s_Text.c_str(), fp);
    // 5  材質名称
    s_Text = "," + P_Zaisitu;
    fputs(s_Text.c_str(), fp);
    // 6  図番
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), fp);
    // 7  作図用図番
    s_Text = "," + P_SZuban;
    fputs(s_Text.c_str(), fp);
    // 8  1台分数量
    s_Text = "," + AnsiString(P_Suryo);
    fputs(s_Text.c_str(), fp);
    // 9  予備数
    s_Text = "," + AnsiString(P_YobisuPltAll);
    fputs(s_Text.c_str(), fp);
    // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
    l_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
    P_Soukei = P_Suryo * l_Daisu + P_YobisuPltAll;
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), fp);
    // 11 仕様１(孔明ﾃﾞｰﾀ件数)
    if (P_PltGsktYobiA > 0 || P_PltGsktYobiB > 0) { // ガスケット付き予備がある場合は１
        s_Text = ",1";
    }
    else {
        s_Text = ",0";
    }
    // s_Text = "," + AnsiString(P_RecNo);
    fputs(s_Text.c_str(), fp);
    // 12 仕様２
    s_Text = ",";
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
    // 16  材質名称
    s_Text = "," + P_EZaisitu;
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

    // -----  ログ作成　------
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  01 部品分類              『PLATE』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  02 識別子                『DPLT--』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  03 部品型式              『" + P_Katasiki + "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  04 部品名称              『プレート』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  05 材質名称              『" + P_Zaisitu + "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  06 図  番                『" + P_Zuban + "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  07 作図用図番            『" + P_SZuban + "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  08 １台分数量            『" + AnsiString(P_Suryo) +
        "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  09 予備数                『" +
        AnsiString(P_YobisuPltAll) + "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  10 総計                  『" + AnsiString(P_Soukei)
        + "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  11 仕様１(孔明ﾃﾞｰﾀ件数)  『" + AnsiString(P_RecNo) +
        "』をセット。";
    Write_Log(G_Log_Text);
    G_Log_Text = "ﾌﾟﾚｰﾄ本体ﾃﾞｰﾀ  17 仕様タイプコード      『" + AnsiString(sSpecCode) +
        "』をセット。";
    Write_Log(G_Log_Text);
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ  CSVﾌｧｲﾙ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 部品展開ﾃﾞｰﾀ,HEARTS用ﾃﾞｰﾀに孔明ﾃﾞｰﾀを書込む
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_Anaake_Data(AnsiString Syurui,
    AnsiString Model) {

    FILE *wkfp;
    FILE *hsfp;

    // CSV読込用
    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    char token[100][100]; /* トークンバッファ */

    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ用
    AnsiString s_Ana_Kata; // 型式
    AnsiString s_Ana_Kbn; // 区分
    AnsiString s_Ana_ABKbn; // AB区分
    AnsiString s_Ana_Anaake; // 孔明
    AnsiString s_Ana_Suryo; // 1台分数量
    AnsiString s_Ana_Zaisitu; // ﾌﾟﾚｰﾄ材質
    AnsiString s_Ana_Siyou; // 仕様

    AnsiString s_Text;

    int i_Daisu;

    AnsiString wk_FilePass;
    AnsiString hs_FilePass;

    // *******************
    // 2007.10.03 ﾐﾗｰ仕様対応 ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽの仕様ﾀｲﾌﾟｺｰﾄﾞ追加
    AnsiString sSpecCode;
    // *******************

    // 2016.04.28 CXW-00追加
    AnsiString s_Syurui;
    AnsiString s_Model;
    // *********************

    P_PltGsktYobiA = 0;
    P_PltGsktYobiB = 0;
    P_PltGsktYobiD = 0;
    P_PltGsktYobiE = 0;

    // --------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    // ****************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得
    // ****************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03
    // --------------------------------

    // ****************************
    // ***  部品展開ﾃﾞｰﾀ　OPEN
    // ****************************
    wk_FilePass = G_csv_Output_Dir + "HD_PARTS.csv";
    if ((fp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(fp);
        return false;
    }

	// ****************************
	// ***  HEARTS用ﾃﾞｰﾀﾊﾟｽ取得
	// ****************************
	if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
        hs_FilePass = G_xls_Output_Dir + G_OUT_ORDER_Dir + G_SAKUBAN + "\\" +
			G_SAKUBAN + ".csv";
    }
	else {
		hs_FilePass = G_xls_Output_Dir + G_OUT_ESTIM_Dir + G_SAKUBAN + "\\" +
			G_SAKUBAN + ".csv";
	}

    // ****************************
    // ***  HEARTS用ﾃﾞｰﾀ　OPEN
    // ****************************
    if ((hsfp = fopen(hs_FilePass.c_str(), "a+")) == NULL) {
        fclose(hsfp);
        return false;
    }

    // *****************************
    // 2007.10.03 ﾐﾗｰ仕様対応 ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽを変更
    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽ

    // wk_FilePass = G_AnaFilePass + G_AnaakeFileNM;
    // wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN  + ".csv";
    //2023/4/21 製造番号桁数追加対応 Mod
    //wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + sSpecCode + ".csv";
    wk_FilePass = G_AnaFilePass + "HOL" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + ".csv";
    // *****************************

    if ((wkfp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        // ｴﾗｰﾛｸﾞ作成
        G_ErrLog_Text = "ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
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

            P_Ana_Plate = token[0]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式
            s_Ana_Kbn = token[1]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 区分
            s_Ana_ABKbn = token[2]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 AB区分
            s_Ana_Anaake = token[3]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 孔明
            s_Ana_Suryo = token[4]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 1台分数量
            s_Ana_Zaisitu = token[5]; // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 ﾌﾟﾚｰﾄ型式

            // ***************************************************
            // *** 標準部品表 出力用に仕様の取得を変更(02.08.08)
            // ***************************************************
            s_Ana_Siyou = HanToZen(token[0]);
            s_Ana_Siyou = TxtEdit(s_Ana_Anaake, 20);
            s_Ana_Siyou += HanToZen(token[1]);
            s_Ana_Siyou += HanToZen(token[2]);
            s_Ana_Siyou += HanToZen(token[3]);

            P_Yobisu = 0;
            if (s_Ana_Kbn.TrimRight() == "D") {
                Syukei_Hontai_Yobisu(P_Ana_Plate, "D",
                    s_Ana_ABKbn + s_Ana_Anaake);
            }
            else if (s_Ana_Kbn.TrimRight() == "E") {
                Syukei_Hontai_Yobisu(P_Ana_Plate, "E",
                    s_Ana_ABKbn + s_Ana_Anaake);
            }
            else {
                Syukei_Hontai_Yobisu(P_Ana_Plate, "",
                s_Ana_ABKbn + s_Ana_Anaake);
            }

            // 有効ﾚｺｰﾄﾞﾁｪｯｸ
            // 2009.06.19 UX-100R追加
            // EXの場合
            // P_Ana_Plate後端部に"C","P","R","A","B"がある場合
            // その文字を順にP_Ana_Plateから除いていく
            //
            // EX以外の場合
            // P_Ana_Plate後端部に"C","P","A","B"がある場合
            // その文字を順にP_Ana_Plateから除いていく
            // 次に条件に合わせて以下の文字ををP_Ana_Plateから除く
            // 最後の文字が"D"の場合
            // ：最後から２文字目の文字が"C","P","A","B"の場合
            // ：その文字を順にP_Ana_Plateから除いていく
            // ：次に最後から２文字目の文字(数字)を除く
            // 最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
            // その他の場合                       ：最後の文字(数字)
            ////2006.02.06 SX-90M,L追加
            ////P_Ana_Plate後端部に"C","P","R","A","B"がある場合
            ////その文字を順にP_Ana_Plateから除いていく
            ////次に条件に合わせて以下の文字ををP_Ana_Plateから除く
            ////EXの場合                           ：除かない
            ////2008.07.24 CX-10D追加
            ////最後の文字が"D"の場合
            ////　　：最後から２文字目の文字が"C","P","R","A","B"の場合
            ////　　：その文字を順にP_Ana_Plateから除いていく
            ////　　：次に最後から２文字目の文字(数字)を除く
            ////*********************
            ////最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
            ////その他の場合                       ：最後の文字(数字)
            // **********************
            //
            // if (P_Ana_Plate.SubString(1,2) == Syurui && P_Ana_Plate.SubString(4,Model.Length()) == Model){
            s_Text = P_Ana_Plate.TrimRight();
            if (s_Text.SubString(s_Text.Length(), 1) == "B") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "A") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // 2009.06.19 UX-100R追加
            // if ( s_Text.SubString(s_Text.Length(),1) == "R" ) { s_Text = s_Text.SubString(1,s_Text.Length()-1); }
            if (s_Text.SubString(1, 2) == "EX" && s_Text.SubString
                (s_Text.Length(), 1) == "R") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            // **********************
            if (s_Text.SubString(s_Text.Length(), 1) == "P") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(s_Text.Length(), 1) == "C") {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }
            if (s_Text.SubString(1, 2) == "EX") {
                // 2008.07.24 CX-10D追加
            }
            else if (s_Text.SubString(s_Text.Length(), 1) == "D") {
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "B") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "A") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                // 2009.06.19 UX-100R追加
                // if ( s_Text.SubString(s_Text.Length()-1,1) == "R" ) {
                // s_Text = s_Text.SubString(1,s_Text.Length()-2) + "D";
                // }
                // **********************
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "P") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }
                if (s_Text.SubString(s_Text.Length() - 1, 1) == "C") {
                    s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                }

                s_Text = s_Text.SubString(1, s_Text.Length() - 2) + "D";
                // *********************
            }
            else if (s_Text.SubString(s_Text.Length(), 1).ToIntDef(999) == 999)
            {
                s_Text = s_Text.SubString(1, s_Text.Length() - 2) +
                    s_Text.SubString(s_Text.Length(), 1);
            }
            else {
                s_Text = s_Text.SubString(1, s_Text.Length() - 1);
            }

            // 2017.08.17 YX-83追加_S
            if (P_YX836 != 0) {
                // YX-836はs_Modelにﾌﾟﾚｰﾄ板厚追加
                // P_Ana_Plateをそのまま使用
                s_Text = P_Ana_Plate.TrimRight();
            }
            // 2017.08.17 YX-83追加_E

            // 2016.04.28 CXW-00追加
            if (s_Text.SubString(3, 1) == "W") {
                s_Syurui = s_Text.SubString(1, 3);
                s_Model  = s_Text.SubString(5, Model.Length());
            } else {
                s_Syurui = s_Text.SubString(1, 2);
                s_Model  = s_Text.SubString(4, Model.Length());
            }
            //if (s_Text.SubString(1, 2) == Syurui && s_Text.SubString(4,
            //    Model.Length()) == Model) {
            if (s_Syurui == Syurui && s_Model == Model) {
            // *********************
                // ***********************

                // **************************
                // ** 部品展開ﾃﾞｰﾀﾌｧｲﾙ出力
                // **************************
                // 1  部品分類
                s_Text = "PLATE";
                fputs(s_Text.c_str(), fp);

                // 2  識別子
                s_Text = ",PLHL--";
                fputs(s_Text.c_str(), fp);

                // 3  部品型式
                s_Text = ",";
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
                s_Text = "," + s_Ana_Suryo.TrimLeft();
                fputs(s_Text.c_str(), fp);

                // 9  予備数
                s_Text = "," + AnsiString(P_Yobisu);
                fputs(s_Text.c_str(), fp);

                // 10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
                i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
                P_Soukei = s_Ana_Suryo.ToIntDef(0) * i_Daisu + P_Yobisu;
                s_Text = "," + AnsiString(P_Soukei);
                fputs(s_Text.c_str(), fp);

                // 11 仕様１(ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式)
                s_Text = "," + P_Ana_Plate;
                fputs(s_Text.c_str(), fp);
                // 12 仕様２(ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 区分 + AB区分 + 孔明)
                s_Text = "," + s_Ana_Kbn + s_Ana_ABKbn + s_Ana_Anaake;
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
                // 16  材質名称
                s_Text = "," + P_EZaisitu;
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

                // **************************
                // ** HEARTSﾃﾞｰﾀﾌｧｲﾙ出力
                // **************************
                // 1 部品分類
                s_Text = "\"PLTE\"";
                fputs(s_Text.c_str(), hsfp);

                // 2 識別子
                s_Text = ",\"PLTE00\"";
                fputs(s_Text.c_str(), hsfp);

                // 3 ﾌﾟﾚｰﾄ型式
                s_Text = ",\"" + P_Ana_Plate + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 4 材質ｺｰﾄﾞ
                s_Text = ",\"" + P_Mat_Code + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 5 ﾌﾟﾚｰﾄ種別
                s_Text = ",\"" + s_Ana_Kbn + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 6 孔明
                s_Text = ",\"" + s_Ana_ABKbn + s_Ana_Anaake + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 7 1台分数量
                s_Text = ",\"" + s_Ana_Suryo + "\"";
                fputs(s_Text.c_str(), hsfp);

                // 8 新旧区分
                s_Text = ",\"0\"";
                fputs(s_Text.c_str(), hsfp);

                // 9 特殊ﾊﾝｶﾞｰ区分
                s_Text = ",\"0\"";
                fputs(s_Text.c_str(), hsfp);

                // 改行文字
                s_Text = "\n";
                // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
                fputs(s_Text.c_str(), hsfp);
            }
        }
    }

    fclose(hsfp); // HEARTS用ﾃﾞｰﾀ 閉じる
    fclose(fp); // 部品展開ﾃﾞｰﾀ 閉じる

    fclose(wkfp); // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ 閉じる

    // delete wkfp;

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Plate_Data_Sakusei(void) {
    // CSV読込用
    char buf[1024]; /* 文字列バッファ */
    char *tok; /* トークンポインタ */
    int i; /* ループカウンタ */
    char token[100][100]; /* トークンバッファ */

    AnsiString wk_FilePass;
    AnsiString s_Key;
    AnsiString s_Model;
    int i_Mojisu;
    AnsiString s_Text;

    bool ret;
    int i_SyoriFlg;
    int i_PateCDCode; //
    AnsiString sPltMat;

    P_JokenFlg1 = 0;
    P_JokenFlg2 = 0;

    // 2012.07.31 LX-10すべて補強パッチ追加
    ////2010.07.14 LX-10,RX-70舶用Ti0.5の場合補強パッチ追加
    // int   i_Pos;
    // AnsiString s_Pltt;
    ////***************************************************
    // ************************************

    // *************************************
    // ***  ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ　OPEN
    // *************************************
    wk_FilePass = G_Input_Csv_Dir + "PLATECD.CSV";
    if ((fp = fopen(wk_FilePass.c_str(), "r")) == NULL) {
        G_ErrLog_Text = "ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙ 『" + wk_FilePass +
            "』のＯＰＥＮに失敗しました！！";
        Write_Error_Log(G_ErrLog_Text);
    }
    else {
        // 読込み可能の間
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

            P_PLATECD[0] = token[0]; // ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ　ｺｰﾄﾞ
            P_PLATECD[1] = token[1]; // ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ  部品分類ｺｰﾄﾞ
            P_PLATECD[2] = token[2]; // ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ  部品区分
            P_PLATECD[3] = token[3]; // ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ  部品名称
            P_PLATECD[4] = token[4]; // ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ　予備品
            P_PLATECD[5] = token[5]; // ﾌﾟﾚｰﾄ構成部品ｺｰﾄﾞﾃｰﾌﾞﾙﾃﾞｰﾀ

            i_PateCDCode = P_PLATECD[0].ToIntDef(9999);

            if (i_PateCDCode == 3 || i_PateCDCode == 22 || i_PateCDCode == 41) {
                i_SyoriFlg = 1;

            }
            else if (i_PateCDCode == 1 || i_PateCDCode == 20 ||
                i_PateCDCode == 39) {
                // i_SyoriFlg = P_JokenFlg1;  // 上記条件のｺｰﾄﾞで既に処理済なら読み飛ばす

            }
            else if (i_PateCDCode == 30 || i_PateCDCode == 57) {
                // i_SyoriFlg = P_JokenFlg2;  // 上記条件のｺｰﾄﾞで既に処理済なら読み飛ばす
            }
            else {
                i_SyoriFlg = 0;
            }

            /* 数値の比較に変更
             if (P_PLATECD[0] == "03" || P_PLATECD[0] == "22" || P_PLATECD[0] == "41"){
             i_SyoriFlg = 1;
             } else if (P_PLATECD[0] == "01" || P_PLATECD[0] == "20" || P_PLATECD[0] == "39"){
             i_SyoriFlg = P_JokenFlg1;
             } else if (P_PLATECD[0] == "30" || P_PLATECD[0] == "57"){
             i_SyoriFlg = P_JokenFlg2;
             } else {
             i_SyoriFlg = 0;
             }
             */

            if (i_SyoriFlg == 0) {
                // *******************************
                // ***  ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ 検索
                // *******************************
                // ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ検索ｷｰ変更 02.09.12
                s_Model = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
                i_Mojisu = s_Model.Length();
                s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu, 1);
                if (s_Text.ToIntDef(999) != 999) {
                    s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" +
                        P_HEADS_DATA[1267].TrimRight();
                }
                else {
                    s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
                        P_HEADS_DATA[1267].TrimRight();
                }
                s_Key = P_SoSyoAscii;

                ret = Search_HD_PLATE_2(s_Key, P_PLATECD[2], P_PLATECD[0]);

                if (i_PateCDCode == 20 || i_PateCDCode == 21) { // ハンガー
                    P_Suryo = P_Suryo * (P_HEADS_DATA[38].ToIntDef(0));
                    // 2003.11.26 LX-40ハンガー
                    if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "４０" &&
                        P_HEADS_DATA[38].ToIntDef(0) >= 201) {
                        P_Zuban = P_Zuban2;
                        P_SZuban = P_SZuban2;
                        i_PateCDCode = 21;
                    }
                    // ************************
                    if (P_MAT_CODE1 == "") { // 材質ｺｰﾄﾞが無ければ、プレート材質から判断
                        // 2016.11.07 材質ｺｰﾄﾞ変更
                        //// 2003.12.05 ASME PLATE
                        //// if( ( P_HEADS_DATA[43].TrimRight() == "1014" ) || ( P_HEADS_DATA[43].TrimRight() == "1015" )
                        //// ||( P_HEADS_DATA[43].TrimRight() == "1032" ) || ( P_HEADS_DATA[43].TrimRight() == "1033" ) ) {
                        //if ((P_HEADS_DATA[43].TrimRight() == "1014") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1015") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1032") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1033") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1057") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1058")
                        //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
                        //    || (P_HEADS_DATA[43].TrimRight() == "1075") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1089") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1090")
                        //    // *************************************************
                        //    // 2010.12.02 SB-265-Gr.11追加
                        //    || (P_HEADS_DATA[43].TrimRight() == "1076")
                        //    // ***************************
                        //    || (P_HEADS_DATA[43].TrimRight() == "1085") ||
                        //    (P_HEADS_DATA[43].TrimRight() == "1095")) {
                        //    // *********************
                        //    P_MAT_CODE1 = "1014"; // TP270
                        //}
                        //else {
                        //    P_MAT_CODE1 = "1001"; // SUS304
                        //}
                        if ( (Type_MAT(P_HEADS_DATA[43].TrimRight()) == "Ti") ) {  // プレート材質がチタン系
                            P_MAT_CODE1 = "3114"; // TP270
                        } else {
                            P_MAT_CODE1 = "3101"; // SUS304
                        }
                        // ***********************
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                }
                if (i_PateCDCode == 11 || i_PateCDCode == 12 ||
                    i_PateCDCode == 48 || i_PateCDCode == 49 ||
                    i_PateCDCode == 50) { // リブの場合材質はメタル
                    sPltMat = P_HEADS_DATA[43].TrimRight();
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //// 2003.12.05 ASME PLATE
                    //// if (sPltMat == "1014" || sPltMat == "1015" || sPltMat == "1032" || sPltMat == "1033" ) {  // プレート材質がチタン系（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
                    //if (sPltMat == "1014" || sPltMat == "1015" ||
                    //    sPltMat == "1032" || sPltMat == "1033" ||
                    //    sPltMat == "1057" || sPltMat == "1058" ||
                    //    sPltMat == "1085" || sPltMat == "1095") {
                    //    // プレート材質がチタン系（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
                    //    // *********************
                    //    // P_MAT_CODE1 = "1014";  // TP270
                    //    P_MAT_CODE1 = "1032"; // TP340
                    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
                    //}
                    //else if (sPltMat == "1075" || sPltMat == "1089" ||
                    //    sPltMat == "1090") {
                    //    // プレート材質がチタン系（SB-265-Gr.7[1075] SB-265-Gr.2[1089] B-265-Gr.2[1090]
                    //    P_MAT_CODE1 = "1032"; // TP340
                    //    // *************************************************
                    //    // 2010.12.02 SB-265-Gr.11追加
                    //}
                    //else if (sPltMat == "1076") {
                    //    // プレート材質がチタン系（SB-265-Gr.11[1076]）
                    //    P_MAT_CODE1 = "1032"; // TP340
                    //    // ***************************
                    //}
                    //else {
                    //    P_MAT_CODE1 = "1003"; // SUS316
                    //}
                    if ( (Type_MAT(sPltMat) == "Ti") ) {  // プレート材質がチタン系
                        P_MAT_CODE1 = "3132"; // TP340
                    } else {
                        P_MAT_CODE1 = "3103"; // SUS316
                    }
                    // ***********************
                    P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                    P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                }

                // 2003.12.18 YX-80プレート部品追加
                // ＤＥガスケット
                if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
                    if (i_PateCDCode == 14 || i_PateCDCode == 15 ||
                        i_PateCDCode == 16 || i_PateCDCode == 17 ||
                        i_PateCDCode == 69 || i_PateCDCode == 70 ||
                        i_PateCDCode == 80 || i_PateCDCode == 90) {
                        // DEプレートのガスケットは数えない
                        P_Suryo = 0;
                        // 中間ガスケット
                    }
                    else if (i_PateCDCode == 32) { // ガスケットＢ−１（Ｎ／２−１枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 33) { // ガスケットＢ−４（Ｎ＋２枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo + 2;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 34) { // ガスケットＡ−２（Ｎ枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 35) { // ガスケットＡ−３（Ｎ／２枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = (double)P_Suryo * 0.5;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                        // 真空時目玉盲部補強
                    }
                    else if (i_PateCDCode == 38) { // E1ノズルで数量変化
                        if (P_HEADS_DATA[951].TrimRight() == "0") {
                            P_Suryo = 2;
                        }
                        else {
                            P_Suryo = 1;
                        }
                        // 2006.02.06 YX-80真空時目玉盲部補強の材質を
                        // B側ｶﾞｽｹｯﾄ材質がTF/NBR(2043),TF/EPDM(2071)
                        // の場合V#7010(2032)、
                        // その他の材質の場合B側ｶﾞｽｹｯﾄ材質と変更
                        ////2006.01.10 YX-80真空時目玉盲部補強の材質を
                        ////B側ｶﾞｽｹｯﾄ材質からV#7010(2032)固定に変更
                        ////P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight();  // B側
                        // P_MAT_CODE1 = "2032";  // V#7010
                        ////******************************************
                        if (P_HEADS_DATA[835].TrimRight() == "2043")
                        { // B側TF/NBR
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else if (P_HEADS_DATA[835].TrimRight() == "2071")
                        { // B側TF/EPDM
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else { // B側
                            P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                        }
                        // ******************************************
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                        P_Unit = "式";
                        // Ｅプレート補強板
                    }
                    else if (i_PateCDCode == 66) { // Eプレート補強板の場合材質はメタル
                        sPltMat = P_HEADS_DATA[43].TrimRight();
                        // 2016.11.07 材質ｺｰﾄﾞ変更
                        //if (sPltMat == "1014" || sPltMat == "1015" ||
                        //    sPltMat == "1032" || sPltMat == "1033" ||
                        //    sPltMat == "1057" || sPltMat == "1058" ||
                        //    sPltMat == "1085" || sPltMat == "1095") {
                        //    // プレート材質がチタン系（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
                        //    P_MAT_CODE1 = "1032"; // TP340
                        //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
                        //}
                        //else if (sPltMat == "1075" || sPltMat == "1089" ||
                        //    sPltMat == "1090") {
                        //    // プレート材質がチタン系（SB-265-Gr.7[1075] SB-265-Gr.2[1089] B-265-Gr.2[1090]
                        //    P_MAT_CODE1 = "1032"; // TP340
                        //    // *************************************************
                        //    // 2010.12.02 SB-265-Gr.11追加
                        //}
                        //else if (sPltMat == "1076") {
                        //    // プレート材質がチタン系（SB-265-Gr.11[1076]）
                        //    P_MAT_CODE1 = "1032"; // TP340
                        //    // ***************************
                        //}
                        //else {
                        //    P_MAT_CODE1 = "1001"; // SUS304
                        //}
                        if ( (Type_MAT(sPltMat) == "Ti") ) {  // プレート材質がチタン系
                            P_MAT_CODE1 = "3132"; // TP340
                        } else {
                            P_MAT_CODE1 = "3101"; // SUS304
                        }
                        // ***********************
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                }
                // ********************************

                // 2021.04.19 YX-83変更_S
                //// 2017.08.17 YX-83プレート部品追加_S
                //// ＹＸ−８３
                //if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
                //    if (i_PateCDCode == 32) { // ガスケットＢ１（Ｎ／２−１枚）
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                //        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A側
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 107) { // ガスケットＢ２（Ｎ／２−１枚）
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                //        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 108) { // ガスケットＢ３（Ｎ／２−１枚）
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                //        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A側
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 109 ) { // ガスケットＡ１（Ｎ／２枚）
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = (double)P_Suryo * 0.5;
                //        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 34) { // ガスケットＡ２（Ｎ／２枚）
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = (double)P_Suryo * 0.5;
                //        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A側
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                //    else if (i_PateCDCode == 35) { // ガスケットＡ３（Ｎ／２枚）
                //        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                //        P_Suryo = (double)P_Suryo * 0.5;
                //        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                //        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                //        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                //    }
                if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
                    if (i_PateCDCode == 32) { // ガスケットＢ−１（Ｎ／２−１枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = ((double)P_Suryo * 0.5) - 1;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 33 ) { // ガスケットＢ−４（Ｎ枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo;
                        P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 34) { // ガスケットＡ−２（Ｎ−２枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = P_Suryo - 2;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                    else if (i_PateCDCode == 35) { // ガスケットＡ−３（Ｎ／２枚）
                        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
                        P_Suryo = (double)P_Suryo * 0.5;
                        P_MAT_CODE1 = P_HEADS_DATA[836].TrimRight(); // A側
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                    }
                // 2021.04.19 YX-83変更_E
                    else if (i_PateCDCode == 38) { // 真空時目玉盲部補強
                        // E1ノズルで数量変化
                        if (P_HEADS_DATA[951].TrimRight() == "0") {
                            P_Suryo = 2;
                        }
                        else {
                            P_Suryo = 1;
                        }
                        // YX-83真空時目玉盲部補強の材質
                        // B側ｶﾞｽｹｯﾄ材質がTF/NBR(2043),TF/EPDM(2071)
                        // の場合V#7010(2032)、
                        // その他の材質の場合B側ｶﾞｽｹｯﾄ材質
                        if (P_HEADS_DATA[835].TrimRight() == "2043")
                        { // B側TF/NBR
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else if (P_HEADS_DATA[835].TrimRight() == "2071")
                        { // B側TF/EPDM
                            P_MAT_CODE1 = "2032"; // V#7010
                        }
                        else { // B側
                            P_MAT_CODE1 = P_HEADS_DATA[835].TrimRight(); // B側
                        }
                        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
                        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
                        P_Unit = "式";
                    }
                }
                // 2017.08.17 YX-83プレート部品追加_E

                // 2007.05.10 UX-90,100,130,SX-90M,Lプレート材質がチタン系以外でも三角堰パッチ追加
                ////2003.11.26 パッチ追加
                //// UX-90三角堰パッチ
                // if (i_PateCDCode == 80 || i_PateCDCode == 81 || i_PateCDCode == 82 || i_PateCDCode == 83 || i_PateCDCode == 84 || i_PateCDCode == 85 ) {
                // //2005.03.04 UX-100,130三角堰パッチ追加
                // //if ( G_KS_Syurui == "ＵＸ" && G_KS_Model == "９０" ) {
                // if ( G_KS_Syurui == "ＵＸ" && ( G_KS_Model == "９０" || G_KS_Model == "１００" || G_KS_Model == "１３０" ) ) {
                // //*************************************
                // sPltMat = P_HEADS_DATA[43].TrimRight();
                ////2003.12.05 ASME PLATE
                ////      if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" )) {  // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
                // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                // // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
                ////*********************
                // P_Suryo = 0;
                // }
                // }
                // }
                ////*********************
                //
                ////2006.02.06 SX-90M,L追加
                ////ﾌﾟﾚｰﾄ材質によるSX-90M,L三角堰ﾊﾟｯﾁの有無判断
                //// 三角堰パッチ
                // if (i_PateCDCode == 80 || i_PateCDCode == 81 || i_PateCDCode == 82 || i_PateCDCode == 83 || i_PateCDCode == 84 || i_PateCDCode == 85 ) {
                // if ( G_KS_Syurui == "ＳＸ" && ( G_KS_Model == "９０Ｍ" || G_KS_Model == "９０Ｌ" ) ) {
                // sPltMat = P_HEADS_DATA[43].TrimRight();
                // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                // // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
                // P_Suryo = 0;
                // }
                // }
                // }
                ////***********************
                // *******************************************************************************
                // 2012.07.31 LX-10すべて補強パッチ追加
                ////2010.07.14 LX-10,RX-70舶用Ti0.5の場合補強パッチ追加
                ////2010.09.08 RX-70すべて補強パッチ追加
                ////if ( ( G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０" )
                ////  || ( G_KS_Syurui == "ＲＸ" && G_KS_Model == "７０" ) ) {
                // if ( G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０" ) {
                ////************************************
                // i_Pos = P_HEADS_DATA[36].Pos("M");
                // if (i_PateCDCode == 87 || i_PateCDCode == 88 ) {  // 補強パッチ
                // if (i_Pos == 0) {  // 舶用以外
                // P_Suryo = 0;
                // }
                // sPltMat = P_HEADS_DATA[43].TrimRight();
                // if ( ( sPltMat != "1014" ) && ( sPltMat != "1015" )
                // && ( sPltMat != "1032" ) && ( sPltMat != "1033" )
                // && ( sPltMat != "1057" ) && ( sPltMat != "1058" )
                // //2010.10.29 SB-265-Gr.7追加
                // && ( sPltMat != "1075" )
                // //**************************
                // //2010.12.02 SB-265-Gr.11追加
                // && ( sPltMat != "1076" )
                // //***************************
                // && ( sPltMat != "1085" ) && ( sPltMat != "1089" )
                // && ( sPltMat != "1090" ) && ( sPltMat != "1095" )) {
                // // プレート材質がチタン系以外（TP270[1014] TP270PD[1015]
                // // TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058]
                // // SB-265-GR.1[1085] SB-265-GR.2[1089] B-265-GR.2[1090]
                // // B-265-GR.1[1095]）
                // P_Suryo = 0;
                // }
                // s_Pltt = P_HEADS_DATA[1270].TrimRight();
                // if ( s_Pltt != "0.5" ) {
                // P_Suryo = 0;
                // }
                // }
                // }
                // ***************************************************
                // ************************************

                // 読み飛ばしﾌﾗｸﾞｾｯﾄ
                if (i_PateCDCode == 1 || i_PateCDCode == 20 ||
                    i_PateCDCode == 39) {
                    // P_JokenFlg1 = 1;
                }
                else if (i_PateCDCode == 30 || i_PateCDCode == 57) {
                    // P_JokenFlg2 = 1;
                }

                // 部品型式
                // 2008.07.24 CX-10D追加
                // if (i_PateCDCode == 37 || i_PateCDCode == 57){
                // P_Katasiki = P_SSyurui + "−" + P_SModel + P_HEADS_DATA[1258].TrimRight();
                // } else {
                // P_Katasiki = P_SSyurui + "−" + P_SModel + P_HEADS_DATA[1258].TrimRight();;
                // }
                if (P_SModel.SubString(P_SModel.Length() - 1, 2) == "Ｄ") {
                    P_Katasiki = P_SSyurui + "−" + P_SModel.SubString(1,
                        P_SModel.Length() - 2) + P_HEADS_DATA[1258].TrimRight
                        () + "Ｄ";
                }
                else {
                    P_Katasiki = P_SSyurui + "−" + P_SModel +
                        P_HEADS_DATA[1258].TrimRight();
                }
                // *********************

                // 予備数の取得
                P_Yobisu = Syukei_Buhin_Yobisu(P_PLATECD[4], "0",
                    m_pcHeadsData);
                if (i_PateCDCode == 20 || i_PateCDCode == 21) { // ハンガー
                    // 2004.01.16 予備数の取得訂正
                    // P_Yobisu += (P_PltGsktYobiA + P_PltGsktYobiB + P_PltGsktYobiD) * 2;
                    if (G_KS_Syurui == "ＥＸ" && (G_KS_Model == "１１" ||
                        G_KS_Model == "１５")) {
                        // 2004.04.23 ハンガー予備数の取得訂正
                        P_Yobisu += P_PltYobiA + P_PltYobiB + P_PltYobiD;
                        // P_Yobisu += P_YobisuPltAll;
                        // ***********************************
                    }
                    else if (G_KS_Syurui == "ＵＸ" && (G_KS_Model == "２０" ||
                        G_KS_Model == "４０")) {
                        // 2004.04.23 ハンガー予備数の取得訂正
                        P_Yobisu += P_PltYobiA + P_PltYobiB + P_PltYobiD;
                        // P_Yobisu += P_YobisuPltAll;
                        // ***********************************
                    }
                    else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "４０") {
                        // 2004.04.23 ハンガー予備数の取得訂正
                        P_Yobisu += P_PltYobiA + P_PltYobiB + P_PltYobiD;
                        // P_Yobisu += P_YobisuPltAll;
                        // ***********************************
                    }
                    else {
                        // 2004.04.23 ハンガー予備数の取得訂正
                        P_Yobisu += (P_PltYobiA + P_PltYobiB + P_PltYobiD) * 2;
                        // P_Yobisu += P_YobisuPltAll * 2;
                        // ***********************************
                    }
                }
                else if (i_PateCDCode == 11) { // ＥＸ−１１，１５　Ｄプレートリブ
                    P_Yobisu += P_PltYobiD * 4;
                }
                else if (i_PateCDCode == 50) { // ＥＸ−１５　Ｅプレートリブ
                    P_Yobisu += P_PltYobiE * 2;
                }
                else if (i_PateCDCode == 80 || i_PateCDCode == 81 ||
                    i_PateCDCode == 82) {
                    // 2007.05.10 UX-90,100,130,SX-90M,Lプレート材質がチタン系以外でも三角堰パッチ追加
                    //// ＵＸ−９０　三角堰パッチ（Ｄ）
                    // sPltMat = P_HEADS_DATA[43].TrimRight();
                    // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                    // // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
                    // } else {
                    // P_Yobisu += P_PltGsktYobiD * 2;
                    // }
                    // ＵＸ−９０，１００，１３０，ＳＸ−９０Ｍ，Ｌ 三角堰パッチ（Ｄ）
                    P_Yobisu += P_PltGsktYobiD * 2;
                    // *******************************************************************************
                }
                else if (i_PateCDCode == 83 || i_PateCDCode == 84 ||
                    i_PateCDCode == 85) {
                    // 2007.05.10 UX-90,100,130,SX-90M,Lプレート材質がチタン系以外でも三角堰パッチ追加
                    //// ＵＸ−９０　三角堰パッチ（Ｅ）
                    // sPltMat = P_HEADS_DATA[43].TrimRight();
                    // if ((sPltMat != "1014" ) && ( sPltMat != "1015" ) && ( sPltMat != "1032" ) && ( sPltMat != "1033" ) && ( sPltMat != "1057" ) && ( sPltMat != "1058" ) && ( sPltMat != "1085" ) && ( sPltMat != "1095" )) {
                    // // プレート材質がチタン系以外（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
                    // } else {
                    // P_Yobisu +=  P_PltGsktYobiE * 2;
                    // }
                    // ＵＸ−９０，１００，１３０，ＳＸ−９０Ｍ，Ｌ 三角堰パッチ（Ｄ）
                    P_Yobisu += P_PltGsktYobiE * 2;
                    // *******************************************************************************
                }
                // 2019.08.01 Ｓ、Ｔディスタンスピース追加_S
                else if (i_PateCDCode == 116) {
                    // (S側)Ｔディスタンスピース
                    P_Yobisu += P_PltGsktYobiD * 2;
                }
                else if (i_PateCDCode == 117) {
                    // (E側)Ｔディスタンスピース
                    P_Yobisu += P_PltGsktYobiE * 2;
                }
                // 2019.08.01 Ｓ、Ｔディスタンスピース追加_E
                else if (i_PateCDCode == 87) {
                    // ＬＸ−３０　補強パッチ（Ｄ）
                    P_Yobisu += P_PltGsktYobiD * 2;
                }
                else if (i_PateCDCode == 88) {
                    // ＬＸ−３０　補強パッチ（Ｅ）
                    P_Yobisu += P_PltGsktYobiE * 2;
                    // 2009.02.16 UX-100ﾃﾞｨｽﾀﾝｽﾋﾟｰｽD追加
                }
                else if (i_PateCDCode == 103) {
                    // ＵＸ−１００　ディスタンスピースＤ（Ｄ）
                    P_Yobisu += P_PltGsktYobiD * 2;
                }
                else if (i_PateCDCode == 104) {
                    // ＵＸ−１００　ディスタンスピースＤ（Ｅ）
                    P_Yobisu += P_PltGsktYobiE * 2;
                    // *********************************
                    // ***************************
                    // 2004.01.19 YX-80プレート予備部品追加
                    // ＹＸ−８０中間ガスケット
                }
                else if (i_PateCDCode == 32) { // ガスケットＢ−１（Ｂ×１枚）
                    P_Yobisu += P_PltGsktYobiB;
                }
                else if (i_PateCDCode == 33) { // ガスケットＢ−４（Ｄ×２＋Ａ×２枚）
                    // 2021.04.19 YX-83変更_S
                    //P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiA * 2;
                    if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
                        //ガスケットＢ−４（Ｄ×２＋Ａ×２枚）
                        P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiA * 2;
                    }
                    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
                        //ガスケットＢ−４（Ａ×２枚）
                        P_Yobisu += P_PltGsktYobiA * 2;
                    }
                    // 2021.04.19 YX-83変更_E
                }
                else if (i_PateCDCode == 34) { // ガスケットＡ−２（Ｄ×２＋Ｂ×２枚）
                    // 2017.08.17 YX-83プレート部品追加_S
                    //P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiB * 2;
                    if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") {
                        // ガスケットＡ−２（Ｄ×２＋Ｂ×２枚）
                        P_Yobisu += P_PltGsktYobiD * 2 + P_PltGsktYobiB * 2;
                    }
                    else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３") {
                        // 2021.04.19 YX-83変更_S
                        //// ガスケットＡ２（Ａ×１枚）
                        //     P_Yobisu += P_PltGsktYobiA;
                        // ガスケットＡ−２（Ｂ×２枚）
                        P_Yobisu += P_PltGsktYobiB * 2;
                        // 2021.04.19 YX-83変更_E
                    }
                    // 2017.08.17 YX-83プレート部品追加_E
                }
                else if (i_PateCDCode == 35) { // ガスケットＡ−３（Ａ×１枚）
                    P_Yobisu += P_PltGsktYobiA;
                    // ＹＸ−８０真空時目玉盲部補強
                }
                else if (i_PateCDCode == 38) {
                    P_Yobisu += P_PltGsktYobiS;
                    // ＹＸ−８０Ｅプレート補強板
                }
                else if (i_PateCDCode == 66) { // Eプレート補強板（Ｅ×２枚）
                    P_Yobisu += P_PltYobiE * 2;
                    // ************************************
                // 2021.04.19 YX-83変更_S
                //// 2017.08.17 YX-83プレート部品追加_S
                //else if (i_PateCDCode == 107) { // ガスケットＢ２（Ｂ×１枚）
                //    P_Yobisu += P_PltGsktYobiB;
                //}
                //else if (i_PateCDCode == 108) { // ガスケットＢ３（Ｂ×１枚）
                //    P_Yobisu += P_PltGsktYobiB;
                //}
                //else if (i_PateCDCode == 109) { // ガスケットＡ１（Ａ×１枚）
                //    P_Yobisu += P_PltGsktYobiA;
                //// 2017.08.17 YX-83プレート部品追加_E
                // 2021.04.19 YX-83変更_E
                }
                // ***********************************
                // ***  ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ  CSVﾌｧｲﾙ作成
                // ***********************************
                Write_PlateBuhin_Data(ret);

                if ((P_PLATECD[4] == "PLTE05") || (P_PLATECD[4].SubString(1,
                    4) == "GSKT")) {
                    Write_HeartsBuhin_Data(P_PLATECD[4]);
                }
            }

        }
    }

    fclose(fp); // 閉じる
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ・材質ｺｰﾄﾞﾏｽﾀ検索（ﾌﾟﾚｰﾄ部品 ﾃﾞｰﾀ作成用）
// 概  要      ：
// 引  数      ： AnsiString Key   : ﾌﾟﾚｰﾄ型式
// ：            PType : ﾌﾟﾚｰﾄ種別
// ：            PCode : 部品ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Search_HD_PLATE_2(AnsiString Key,
    AnsiString PType, AnsiString PCode) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PLATE";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + Key + "'";
    s_SQL = s_SQL + "    AND PLATE_TYPE = '" + PType + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = " + PCode + "";

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
        P_Zaisitu = "";
        P_EZaisitu = "";
        P_Suryo = 0;
        // 2003.11.26 変数追加
        P_Zuban2 = "";
        P_SZuban2 = "";
        // *******************
        delete wkQuery;
        // return false;
        return true;
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_MAT_CODE1 = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_Suryo = wkQuery->FieldByName("NUMS")->AsString.ToIntDef(0);
        // 2003.11.26 変数追加
        P_Zuban2 = wkQuery->FieldByName("DNO2")->AsString.TrimRight();
        P_SZuban2 = wkQuery->FieldByName("WKDNO2")->AsString.TrimRight();
        // *******************

        P_Zaisitu = Search_HD_MATERIAL(P_MAT_CODE1);
        P_EZaisitu = Search_HD_E_MATERIAL(P_MAT_CODE1);
        // // 材質ｺｰﾄﾞﾏｽﾀ検索
        // s_SQL = "";
        // s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
        // s_SQL = s_SQL + "  WHERE MAT_CODE = '" + P_MAT_CODE1 + "'";
        //
        // wkQuery->Close();
        // wkQuery->DatabaseName = ODBC_DSN_MAT;
        // wkQuery->SQL->Clear();
        // wkQuery->SQL->Add(s_SQL);
        // wkQuery->Open();
        // wkQuery->First();
        //
        // if(wkQuery->Eof){
        // P_Zaisitu = "";
        // G_ErrLog_Text = "材質ｺｰﾄﾞ『" + P_MAT_CODE1 + "』は存在しません！！";
        // Write_Error_Log(G_ErrLog_Text);
        // } else {
        // if(P_HEADS_DATA[1].TrimRight() == "0" ){
        // // 和文名称の取得
        // P_Zaisitu = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // P_Zaisitu = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }
        // }
        //
        // delete wkQuery;

        return true;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ  CSVﾌｧｲﾙ作成
// 概  要      ：
// 引  数      ： bool ret [true:ﾌﾟﾚｰﾄ部品構成ﾏｽﾀに該当ﾚｺｰﾄﾞあり false:なし]
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： TYouryosyo 処理
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_PlateBuhin_Data(bool ret) {
    FILE *wkfp;

    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ用
    AnsiString s_Ana_Kata;
    AnsiString s_Ana_Suryo;
    AnsiString s_Ana_Anaake;
    AnsiString s_Text;

    int i_Daisu;

    AnsiString wk_FilePass;

    // ------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03
    // ------------------------------------------

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
    s_Text = "PLATE";
    fputs(s_Text.c_str(), wkfp);
    // 2  識別子
    s_Text = "," + P_PLATECD[1];
    fputs(s_Text.c_str(), wkfp);
    // 3  部品型式
    if (ret == true) {
        s_Text = "," + P_Katasiki;
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);
    // 4  部品名称
    if (ret == true) {
        s_Text = "," + P_PLATECD[3];
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);
    // 5  材質名称
    if (ret == true) {
        s_Text = "," + P_Zaisitu;
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);
    // 6  図番
    s_Text = "," + P_Zuban;
    fputs(s_Text.c_str(), wkfp);
    // 7  作図用図番
    if (ret == true) {
        s_Text = "," + P_SZuban;
    }
    else {
        s_Text = ",";
    }
    fputs(s_Text.c_str(), wkfp);

    // 8  1台分数量,9  予備数,10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
    if (ret == true) {
        s_Text = "," + AnsiString(P_Suryo);
        fputs(s_Text.c_str(), wkfp);

        s_Text = "," + AnsiString(P_Yobisu);
        fputs(s_Text.c_str(), wkfp);

        i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
        P_Soukei = P_Suryo * i_Daisu + P_Yobisu;
        s_Text = "," + AnsiString(P_Soukei);
        fputs(s_Text.c_str(), wkfp);
    }
    else {
        s_Text = ",0";
        fputs(s_Text.c_str(), wkfp);
        s_Text = ",0";
        fputs(s_Text.c_str(), wkfp);
        s_Text = ",0";
        fputs(s_Text.c_str(), wkfp);
    }

    // 11 仕様１(ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 孔明)
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 12 仕様２
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 13 仕様３
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 14 仕様４
    s_Text = ",";
    // 2017.08.17 YX-83プレート部品追加_S
    if (P_PLATECD[1] == "IVCR--") {
        s_Text = ",式";
    }
    // 2017.08.17 YX-83プレート部品追加_E
    fputs(s_Text.c_str(), wkfp);
    // 15 仕様５
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 16  材質名称(英)
    s_Text = "," + P_EZaisitu;
    fputs(s_Text.c_str(), wkfp);

    // ------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), wkfp);
    // 2007.10.03
    // ------------------------------

    // 改行文字
    s_Text = "\n";
    // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
    fputs(s_Text.c_str(), wkfp);

    fclose(wkfp); // 閉じる

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄｶﾞｽｹｯﾄﾃﾞｰﾀ  CSVﾌｧｲﾙ作成
// 概  要      ：
// 引  数      ： bool ret [true:ﾌﾟﾚｰﾄ部品構成ﾏｽﾀに該当ﾚｺｰﾄﾞあり false:なし]
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： TYouryosyo 処理
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_PlateBuhin_Data_GSKT(bool ret) {
    FILE *wkfp;

    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ用
    AnsiString s_Ana_Kata;
    AnsiString s_Ana_Suryo;
    AnsiString s_Ana_Anaake;
    AnsiString s_Text;

    int i_Daisu;

    AnsiString wk_FilePass;

    // ----------------------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    AnsiString sSpecCode;
    // **********************************************
    // 仕様ﾀｲﾌﾟｺｰﾄﾞ取得(""/"A"/"B")
    // **********************************************
    sSpecCode = AnsiString(m_pcHeadsData->GetSpecCode());
    // 2007.10.03 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
    // ----------------------------------------------

    // 2007.12.17 UX-90裏当板
    int i_BANA; // Ｂ側孔
    int i_AANA; // Ａ側孔
    // **********************

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
    if (bMouAteFlg == true) {
        s_Text = "MOATE"; // 盲当板用
    }
    else {
        s_Text = "PLATE";
    }
    fputs(s_Text.c_str(), wkfp);

    bMouAteFlg = false;
    // 2  識別子
    s_Text = "," + P_PLATECD[1];
    fputs(s_Text.c_str(), wkfp);
    // 3  部品型式
    s_Text = "," + P_Katasiki;
    // 2004.12.15 GX-20ｶﾞｽｹｯﾄ型式変更
    if (P_PLATECD[1] == "DDP-21" || P_PLATECD[1] == "EDPC21") {
        s_Text = ",ＧＸ−２１Ａ";
    }
    else if (P_PLATECD[1] == "DDP-22" || P_PLATECD[1] == "EDPC22") {
        s_Text = ",ＧＸ−２２Ａ";
    }
    // ******************************
    fputs(s_Text.c_str(), wkfp);
    // 4  部品名称
    s_Text = "," + P_PLATECD[3];
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

    // 8  1台分数量,9  予備数,10 総計(1台分数量×HEADSﾃﾞｰﾀ内台数+予備数)
    s_Text = "," + AnsiString(P_Suryo);
    fputs(s_Text.c_str(), wkfp);

    s_Text = "," + AnsiString(P_Yobisu);
    fputs(s_Text.c_str(), wkfp);

    i_Daisu = P_HEADS_DATA[39].ToIntDef(0); // 台数
    P_Soukei = P_Suryo * i_Daisu + P_Yobisu;
    s_Text = "," + AnsiString(P_Soukei);
    fputs(s_Text.c_str(), wkfp);

    // 11 仕様１(ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 孔明)
    s_Text = ",";
    // 2007.12.17 UX-90裏当板
    if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "９０" && P_PLATECD[1] == "EPGS-R") {
        i_BANA = 0; // Ｂ側孔
        i_AANA = 0; // Ａ側孔
        if (P_HEADS_DATA[951].TrimRight() == "0") {
        }
        else {
            i_BANA = i_BANA + 1; // Ｂ側孔
        }
        if (P_HEADS_DATA[966].TrimRight() == "0") {
        }
        else {
            i_AANA = i_AANA + 1; // Ａ側孔
        }
        if (P_HEADS_DATA[981].TrimRight() == "0") {
        }
        else {
            i_BANA = i_BANA + 1; // Ｂ側孔
        }
        if (P_HEADS_DATA[996].TrimRight() == "0") {
        }
        else {
            i_AANA = i_AANA + 1; // Ａ側孔
        }
        if (i_AANA == i_BANA) {
        }
        else {
            // 孔の数がＡ側Ｂ側で異なる
            if (sSpecCode == "A") {
                s_Text = ",Ａ号機";
            }
            else if (sSpecCode == "B") {
                s_Text = ",Ｂ号機";
            }
        }
    }
    // **********************
    fputs(s_Text.c_str(), wkfp);
    // 12 仕様２
    s_Text = ",";
    fputs(s_Text.c_str(), wkfp);
    // 13 仕様３
    s_Text = "," + DE_Kubun;
    fputs(s_Text.c_str(), wkfp);
    // 14 仕様４
    s_Text = "," + P_Unit;
    fputs(s_Text.c_str(), wkfp);
    // 15 仕様５
    s_Text = "," + P_SideDiv;
    fputs(s_Text.c_str(), wkfp);
    // 16  材質名称(英)
    s_Text = "," + P_EZaisitu;
    fputs(s_Text.c_str(), wkfp);

    // ------------------------------
    // 2007.10.03 ﾐﾗｰ仕様対応
    // 17 仕様ﾀｲﾌﾟｺｰﾄﾞ
    s_Text = "," + sSpecCode;
    fputs(s_Text.c_str(), wkfp);
    // 2007.10.03
    // ------------------------------

    // 改行文字
    s_Text = "\n";
    // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
    fputs(s_Text.c_str(), wkfp);

    fclose(wkfp); // 閉じる

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： HEARTS用ﾃﾞｰﾀ(部品)  CSVﾌｧｲﾙ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 各項目を“”で囲む
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_HeartsBuhin_Data(AnsiString s_Mat_Code) {
    FILE *wkfp;

    // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ用
    AnsiString s_Text;

    AnsiString wk_FilePass;

	// ****************************
	// ***  HEARTS用ﾃﾞｰﾀﾊﾟｽ取得
	// ****************************
	if (AllNumChk(G_SAKUBAN.c_str(), 0) == 0) {
		wk_FilePass = G_xls_Output_Dir + G_OUT_ORDER_Dir + "\\" + G_SAKUBAN +
			"\\" + G_SAKUBAN + ".csv";
	}
	else {
		wk_FilePass = G_xls_Output_Dir + G_OUT_ESTIM_Dir + "\\" + G_SAKUBAN +
			"\\" + G_SAKUBAN + ".csv";
	}

    // ****************************
    // ***  HEARTS用ﾃﾞｰﾀ　OPEN
    // ****************************
    if ((wkfp = fopen(wk_FilePass.c_str(), "a+")) == NULL) {
        fclose(wkfp);
        return false;
    }

    // ****************************
    // *** 先頭4桁="PLTE"の場合
    // ****************************
    if (s_Mat_Code.SubString(1, 4) == "PLTE") {
        // 1  部品分類
        s_Text = "\"PLTE\"";
        fputs(s_Text.c_str(), wkfp);

        // 2  識別子
        s_Text = ",\"" + s_Mat_Code + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 3  ﾌﾟﾚｰﾄ型式
        s_Text = ",\"" + ZenToHan(P_Katasiki) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 4  材質ｺｰﾄﾞ
        s_Text = ",\"" + P_MAT_CODE1 + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 5  ﾌﾟﾚｰﾄ種別
        s_Text = ",\"\"";
        fputs(s_Text.c_str(), wkfp);

        // 6  孔明
        s_Text = ",\"\"";
        fputs(s_Text.c_str(), wkfp);

        // 7  枚数
        // s_Text = ",\""+ IntToStr(P_Suryo) +"\"";
        s_Text = ",\"" + IntToStr(__int64(P_Suryo)) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 8  新旧区分
        s_Text = ",\"0\"";
        fputs(s_Text.c_str(), wkfp);

        // 9  特殊ﾊﾝｶﾞｰ区分
        if ((s_Mat_Code == "PLTE05") && (ZenToHan(P_Katasiki).SubString(1,
            4) == "LX-4") && (P_HEADS_DATA[38].ToIntDef(0) > 200))
            s_Text = ",\"1\"";
        else
            s_Text = ",\"0\"";
        fputs(s_Text.c_str(), wkfp);

        // 改行文字
        s_Text = "\n";
        // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
        fputs(s_Text.c_str(), wkfp);
    }
    else if (s_Mat_Code.SubString(1, 4) == "GSKT") {
        // 1  部品分類
        s_Text = "\"GSKT\"";
        fputs(s_Text.c_str(), wkfp);

        // 2  識別子
        s_Text = ",\"" + s_Mat_Code + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 3  ｼﾘｰｽﾞ型式
        s_Text = ",\"" + ZenToHan(P_Katasiki) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 3  ﾌﾚｰﾑ型式
        s_Text = ",\"" + ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) +
            "-" + P_HEADS_DATA[36].TrimRight() + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 4  材質ｺｰﾄﾞ
        s_Text = ",\"" + P_MAT_CODE1 + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 5  枚数
        // s_Text = ",\""+ IntToStr(P_Suryo) +"\"";
        // Windows Server2008移植
        s_Text = ",\"" + IntToStr(__int64(P_Suryo)) + "\"";
        fputs(s_Text.c_str(), wkfp);

        // 改行文字
        s_Text = "\n";
        // CSVﾃﾞｰﾀ　ﾌｧｲﾙ出力
        fputs(s_Text.c_str(), wkfp);
    }

    // --------------------
    // 2007.10.03 追加
    fclose(wkfp);
    // 2007.10.03
    // --------------------

    return true;
}

// ↓2002/10/23 以降

// ---------------------------------------------------------------------------
// 日本語関数名： 当板部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_Ateita_Data(void) {
    int ret;

    //
    // "D"
    //
    DE_Kubun = "D";
    Get_Gskt_MastKey(0); // ガスケット検索キー取得
    // ret = Search_HD_BFRG( G_D_GsktType, G_GsktType, G_FrmDiv );
    // S1
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "D", P_HEADS_DATA[836].TrimRight());
    // S2
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "D", P_HEADS_DATA[835].TrimRight());
    // S3
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "D", P_HEADS_DATA[836].TrimRight());
    // S4
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "D", P_HEADS_DATA[835].TrimRight());

    //
    // "E"
    //
    DE_Kubun = "E";
    // 2008.07.24 CX-10D追加
    // P_Katasiki = P_SSyurui + "−" + P_SModel + P_HEADS_DATA[1258].TrimRight();
    if (P_SModel.SubString(P_SModel.Length() - 1, 2) == "Ｄ") {
        P_Katasiki = P_SSyurui + "−" + P_SModel.SubString(1,
            P_SModel.Length() - 2) + P_HEADS_DATA[1258].TrimRight() + "Ｄ";
    }
    else {
        P_Katasiki = P_SSyurui + "−" + P_SModel +
            P_HEADS_DATA[1258].TrimRight();
    }
    // *********************
    Get_Gskt_MastKey(1); // ガスケット検索キー取得
    ret = Search_HD_BFRG(G_E_GsktType, G_GsktType, G_FrmDiv);

    // 2004.02.24 G_D_GsktABKubun条件追加
    ////S1
    // P_SideDiv ="B";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
    ////S2
    // P_SideDiv ="A";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
    // S3
    // P_SideDiv ="B";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3,1), BBP_Code, BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
    ////S4
    // P_SideDiv ="A";
    // ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4,1), ABP_Code, ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
    // Aスタート
    if (G_E_GsktABKubun == "A") {
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1, 1), BBP_Code,
            BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2, 1), ABP_Code,
            ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        // 2017.08.17 斜行流追加_S
        //// S3:B側の処理
        //P_SideDiv = "B";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), BBP_Code,
        //    BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        //// S4:A側の処理
        //P_SideDiv = "A";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), ABP_Code,
        //    ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        if (P_FL == 1) { // 斜行流
            // 斜行流S3:A側の処理
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
            // 斜行流S4:B側の処理
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        }
        else {
            // S3:B側の処理
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
            // S4:A側の処理
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        }
        // 2017.08.17 斜行流追加_E
    }
    // Bスタート
    else {
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(1, 1), ABP_Code,
            ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(2, 1), BBP_Code,
            BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        // 2017.08.17 斜行流追加_S
        //// S3:B側の処理
        //P_SideDiv = "B";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), ABP_Code,
        //    ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        //// S4:A側の処理
        //P_SideDiv = "A";
        //ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), BBP_Code,
        //    BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        if (P_FL == 1) { // 斜行流
            // 斜行流S3:A側の処理
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
            // 斜行流S4:B側の処理
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
        }
        else {
            // S3:B側の処理
            P_SideDiv = "B";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(3, 1), ABP_Code,
                ABP_Label, ABP_Name, ABP_Num, "E", P_HEADS_DATA[835].TrimRight());
            // S4:A側の処理
            P_SideDiv = "A";
            ret = Write_Ateita_Data_Sub(G_E_GsktAna.SubString(4, 1), BBP_Code,
                BBP_Label, BBP_Name, BBP_Num, "E", P_HEADS_DATA[836].TrimRight());
        }
        // 2017.08.17 斜行流追加_E
    }
    // **********************************

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 当板部品　ﾃﾞｰﾀ作成 サブ
// 概  要      ：
// ：　　　　　　s_Ana    : 孔明データのS1,S2・・のデータ
// ：            PCode    : 部品ｺｰﾄﾞ
// ：            PLabel   : 部品ﾗﾍﾞﾙ
// ：            PName    : 部品名
// ：            PNum     : 部品数量
// ：            PMode    : D/Eモード
// ：            PMatCode : 部品材質ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_Ateita_Data_Sub(AnsiString s_Ana,
    AnsiString PCode, AnsiString PLabel, AnsiString PName, long PNum,
    AnsiString PMode, AnsiString PMatCode) {
    int ret;
    AnsiString sPltMat;

    // 孔の場合
    if (s_Ana == "0") {
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, PMode, PCode, PMatCode);
        if (ret == true) {
            // 識別子と名称と数量を上書きする。
            P_PLATECD[1] = PLabel;
            P_PLATECD[3] = PName;
            P_Suryo = PNum;
            // 2003.11.26 PCode追加
            // if ( PCode == "65" ) { // 補強ガスケットは材質ＮＢＲに固定
            if (PCode == "65" || PCode == "78") { // 補強ガスケットは材質ＮＢＲに固定
                // ********************
                PMatCode = "2019";
                P_Zaisitu = "NBR";
                P_EZaisitu = "NBR";
                // 2004.01.16 予備数の取得訂正
                P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                P_Yobisu += P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                // ***************************
            }
            if (PCode == "61" || PCode == "62" || PCode == "63")
            { // 盲当板の場合材質はメタル
                if (P_SoSyoAscii.SubString(1, 5) == "EX-15" ||
                    P_SoSyoAscii.SubString(1, 5) == "LX-40") {
                    sPltMat = P_HEADS_DATA[43].TrimRight();
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //// 2003.12.05 ASME PLATE
                    //// if (sPltMat == "1014" || sPltMat == "1015" || sPltMat == "1032" || sPltMat == "1033" ) {  // プレート材質がチタン系（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033]）
                    //if (sPltMat == "1014" || sPltMat == "1015" ||
                    //    sPltMat == "1032" || sPltMat == "1033" ||
                    //    sPltMat == "1057" || sPltMat == "1058" ||
                    //    sPltMat == "1085" || sPltMat == "1095") {
                    //    // プレート材質がチタン系（TP270[1014] TP270PD[1015] TP340[1032] TP340PD[1033] TP480[1057] TP480PD[1058] SB-265-GR.1[1085] B-265-GR.1[1095]）
                    //    // *********************
                    //    PMatCode = "1014"; // TP270
                    //    // 2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
                    //}
                    //else if (sPltMat == "1075" || sPltMat == "1089" ||
                    //    sPltMat == "1090") {
                    //    // プレート材質がチタン系（SB-265-GR.7[1075] SB-265-GR.2[1089] B-265-GR.2[1090]）
                    //    PMatCode = "1014"; // TP270
                    //    // *************************************************
                    //    // 2010.12.02 SB-265-Gr.11追加
                    //}
                    //else if (sPltMat == "1076") {
                    //    // プレート材質がチタン系（SB-265-GR.11[1076]）
                    //    PMatCode = "1014"; // TP270
                    //    // ********************************************
                    //}
                    //else {
                    //    PMatCode = "1003"; // SUS316
                    //}
                    if ( (Type_MAT(sPltMat) == "Ti") ) {  // プレート材質がチタン系
                        PMatCode = "3114"; // TP270
                    } else {
                        PMatCode = "3103"; // SUS316
                    }
                    // ***********************
                    // 2004.01.16 予備数の取得訂正
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltYobiE; // プレートの予備品あり
                    // ***************************
                }
                else if (P_SoSyoAscii.SubString(1, 5) == "EX-11" ||
                    P_SoSyoAscii.SubString(1, 5) == "EX-16") {
                    PMatCode = "1040"; // SS400
                    bMouAteFlg = true; // フレーム部品として出力
                    // 2004.01.16 予備数の取得訂正
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    // ***************************
                    // 2003.12.17 YX-80
                }
                else if (P_SoSyoAscii.SubString(1, 5) == "YX-80") {
                    PMatCode = "2019"; // NBR
                    // 2017.08.17 YX-80部品変更_S
                    //P_Unit = "式";
                    if (PCode == "63" ) {
                        P_Unit = "式";
                    } else {
                        P_Unit = "";
                    }
                    // 2017.08.17 YX-80部品変更_E
                    // 2004.01.16 予備数の取得訂正
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    // ***************************
                    // ****************
                }
                // 2017.08.17 YX-83部品追加_S
                else if (P_SoSyoAscii.SubString(1, 5) == "YX-83") {
                    PMatCode = "2019"; // NBR
                    if (PCode == "63" ) {
                        P_Unit = "式";
                    } else {
                        P_Unit = "";
                    }
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                // 2017.08.17 YX-83部品追加_E
                else {
                    // 2016.11.07 材質ｺｰﾄﾞ変更
                    //PMatCode = "1003"; // SUS316
                    PMatCode = "3103"; // SUS316
                    // ***********************
                    // 2004.01.16 予備数の取得訂正
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltYobiE; // プレートの予備品あり
                    // ***************************
                }
                P_Zaisitu = Search_HD_MATERIAL(PMatCode);
                P_EZaisitu = Search_HD_E_MATERIAL(PMatCode);
            }
            // 2019.03.18 YX-80変更_S
            if (PCode == "112" || PCode == "113")
            { // ＢディスタンスピースＡＢの場合材質はＮＢＲ
                PMatCode = "2019"; // NBR
                if (P_SoSyoAscii.SubString(1, 5) == "YX-80") {
                    if (PCode == "113" ) {
                        P_Unit = "式";
                    } else {
                        P_Unit = "";
                    }
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                else if (P_SoSyoAscii.SubString(1, 5) == "YX-83") {
                    if (PCode == "113" ) {
                        P_Unit = "式";
                    } else {
                        P_Unit = "";
                    }
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                else {
                    P_Yobisu = Get_Gskt_YobiSu(PCode.ToIntDef(0), PMatCode);
                    P_Yobisu += P_PltYobiE; // プレートの予備品あり
                }
                P_Zaisitu = Search_HD_MATERIAL(PMatCode);
                P_EZaisitu = Search_HD_E_MATERIAL(PMatCode);
            }
            // 2019.03.18 YX-80変更_E

            // 出力する。
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(PMatCode); // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 中間プレートガスケット部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_M_Gskt_Data(void) {

    AnsiString s_Text, s_Model, s_Key;
    int i_Mojisu;

    // **********************************
    // ***  ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ　検索
    // **********************************
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // 2009.06.19 UX-100R追加
    // EXの場合
    // s_Model後端部に"C","P","R","A","B"がある場合
    // s_KeyのG_K_Model1の後ろにその文字を追加する
    //
    // EX以外の場合
    // s_Model後端部に"C","P","A","B"がある場合
    // s_KeyのG_K_Model1の後ろにその文字を追加する
    // 但しs_Modelの最後の文字が"D"の場合
    // s_KeyのG_K_Model1の"D"の前の位置に"C","P","A","B"をを追加する
    ////2006.02.06 SX-90M,L追加
    ////P_HEADS_DATA[35]後端部に"C","P","R","A","B"がある場合
    ////s_KeyのG_K_Model1の後ろにその文字を追加する
    ////2008.07.24 CX-10D追加
    ////但しP_HEADS_DATA[35]の最後の文字が"D"の場合
    ////s_KeyのG_K_Model1の"D"の前の位置に"C","P","R","A","B"をを追加する
    ////*********************
    // **********************
    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R追加
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (G_K_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D追加
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model1.SubString(1, G_K_Model1.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    // AガスケットとBガスケットの材質が同じ場合
    // if( P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836].TrimRight() ){
    // //Aｶﾞｽｹｯﾄを使用する。
    // Write_M_Gskt_Data_Sub( P_HEADS_DATA[835].TrimRight(), false );
    // }
    // else{
    // A,B一行ずつ出力します。
    Write_M_Gskt_Data_Sub(P_HEADS_DATA[835].TrimRight(), true, "B");
    Write_M_Gskt_Data_Sub(P_HEADS_DATA[836].TrimRight(), true, "A");
    // }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： 中間プレートガスケット部品　ﾃﾞｰﾀ出力サブ
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_M_Gskt_Data_Sub(AnsiString GsktCode,
    bool bOutMode, AnsiString sBADiv) {
    int ret;
    int iSuryo;
    int iMod;

    // マスタ検索関数
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "30", GsktCode);

    if (ret == true) {

        //2022/01/06 ガスケット図番変更_S
        if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "２０" && GsktCode == "2097") {
            // ＧＸ−２０ Ａ−ＥＰＤＭプレートガスケット図番はＥ１０１１５−Ｒ２
            P_Zuban = "Ｅ１０１１５−Ｒ２";    //図番
            P_SZuban = "Ｅ１０１１５";         //作図用図番
        }
        //2022/01/06 ガスケット図番変更_E

        // 数量補正 数量=数量／２＋０．５　小数部切り上げ
        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
        iSuryo = int(P_Suryo);
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if ((P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836].TrimRight()) &&
            (sBADiv == "B") && (iMod == 0)) {
            P_Suryo = ((double)(P_Suryo - 1) * 0.5) - 0.5;
        }
        else {
            P_Suryo = ((double)(P_Suryo - 1) * 0.5) + 0.5;
        }

        P_PLATECD[1] = "IPGS--";
        P_PLATECD[3] = "プレートガスケット";
        // 出力する。
        P_Yobisu = Syukei_Buhin_Yobisu("GSKT01", GsktCode, m_pcHeadsData);
        if (sBADiv == "A") {
            P_Yobisu = P_Yobisu + P_PltGsktYobiA;
        }
        else {
            P_Yobisu = P_Yobisu + P_PltGsktYobiB;
        }
        P_Unit = "";
        P_SideDiv = sBADiv;
        Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        Write_HeartsBuhin_Data(GsktCode); // HEARTS用ﾃﾞｰﾀCSV作成
    }
    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Ｄプレートガスケット部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_D_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    ret = Search_HD_D_GASKET(G_D_GsktType, G_GsktType, G_FrmDiv);

    DE_Kubun = "D";

    // 2004.06.09 LX-10 TCG用材質判定追加
    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
    if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０") { // LX-10
        if (s_B_GSKT_Mat == "2063" || s_A_GSKT_Mat == "2063") {
            // B側A側いずれかが2063(AT-TF/EPDM)の場合両側2063にする
            s_B_GSKT_Mat = "2063";
            s_A_GSKT_Mat = "2063";
        }
        //2017.10.10 AN-TF/EPDM追加_S
        else if (s_B_GSKT_Mat == "2064" || s_A_GSKT_Mat == "2064") {
            // B側A側いずれかが2064(AN-TF/EPDM)の場合両側2064にする
            s_B_GSKT_Mat = "2064";
            s_A_GSKT_Mat = "2064";
        }
        //2017.10.10 AN-TF/EPDM追加_E
    }

    // パターン分岐により必要なガスケットを選択
    // Aスタート
    if (G_D_GsktABKubun == "A") {
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s1_Code,
            DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code,
            DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_B_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s2_Code,
            DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code,
            DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_A_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // 型式とコードの比較
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B同材質の数量で出力
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // P_SideDiv ="";
            P_Suryo = DPU_Meq_Sub;
            P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2008.07.24 CX-10D追加
        ////S3:B側の処理
        // P_SideDiv ="B";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_B_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S4:A側の処理
        // P_SideDiv ="A";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_A_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 斜行流追加_S
        //if (G_KS_Model.Pos("Ｄ") > 0) { // 斜行流
        if (P_FL == 1) { // 斜行流
        // 2017.08.17 斜行流追加_E
            // 斜行流S3:A側の処理
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num,
                DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // 斜行流S4:B側の処理
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num,
                DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        else {
            // S3:B側の処理
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num,
                DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // S4:A側の処理
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num,
                DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        // *********************
        // 型式とコードの比較
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B同材質の数量で出力
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // P_SideDiv ="";
            P_Suryo = DPL_Meq_Sub;
            P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }
    // Bスタート
    else {
        // 2008.07.24 CX-10D追加
        ////S4:A側の処理
        // P_SideDiv ="A";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_A_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S3:B側の処理
        // P_SideDiv ="B";
        // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_B_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 斜行流追加_S
        //if (G_KS_Model.Pos("Ｄ") > 0) { // 斜行流
        if (P_FL == 1) { // 斜行流
        // 2017.08.17 斜行流追加_E
            // 斜行流S4:B側の処理
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num,
                DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // 斜行流S3:A側の処理
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num,
                DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        else {
            // S4:A側の処理
            P_SideDiv = "A";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1),
                DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num,
                DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // S3:B側の処理
            P_SideDiv = "B";
            ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1),
                DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num,
                DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        // *********************
        // 型式とコードの比較
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B同材質の数量で出力
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // P_SideDiv ="";
            P_Suryo = DPU_Meq_Sub;
            P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s3_Code,
            DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code,
            DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_A_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s4_Code,
            DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code,
            DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_B_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // 型式とコードの比較
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B同材質の数量で出力
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // P_SideDiv ="";
            P_Suryo = DPL_Meq_Sub;
            P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }

    ////パターン分岐により必要なガスケットを選択
    ////Aスタート
    // if( G_D_GsktABKubun == "A" ){
    // //S1:B側の処理
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(1,1), DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S2:A側の処理
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(2,1), DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //型式とコードの比較
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = DPU_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // //S3:B側の処理
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S4:A側の処理
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //型式とコードの比較
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = DPL_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // }
    ////Bスタート
    // else{
    // //S4:A側の処理
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(4,1), DP_B0s1_Code, DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code, DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S3:B側の処理
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(3,1), DP_B0s2_Code, DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code, DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //型式とコードの比較
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = DPU_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = DPU_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // //S2:A側の処理
    // P_SideDiv ="A";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(2,1), DP_B0s3_Code, DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code, DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S1:B側の処理
    // P_SideDiv ="B";
    // ret = Write_D_Gskt_Data_Sub( G_D_GsktAna.SubString(1,1), DP_B0s4_Code, DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code, DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //型式とコードの比較
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = DPL_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = DPL_Meq_Sub * P_PltGsktYobiD;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // }
    // **********************************
    // ディスタンスピース出力
    Write_D_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Ｄプレートガスケット部品　ﾃﾞｰﾀ作成 サブ
// 概  要      ：
// ：　　　　　　Ana 　 　：孔明データのS1,S2・・のデータ
// ：            PCode0   : 盲目部品ｺｰﾄﾞ
// ：            PLabel0  : 盲目部品ﾗﾍﾞﾙ
// ：            PName0   : 盲目部品名
// ：            PNum0    : 盲目部品数量
// ：            PCode1   : 孔明部品ｺｰﾄﾞ
// ：            PLabel1  : 盲目部品ﾗﾍﾞﾙ
// ：            PName1   : 盲目部品名
// ：            PNum1    : 盲目部品数量
// ：            PMatCode : 部品材質ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_D_Gskt_Data_Sub(AnsiString s_Ana,
    AnsiString PCode0, AnsiString PLabel0, AnsiString PName0, long PNum0,
    AnsiString PCode1, AnsiString PLabel1, AnsiString PName1, long PNum1,
    AnsiString PMatCode) {
    int ret;

    // 孔の場合
    if (s_Ana != "0") {
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "D", PCode1, PMatCode);
        if (ret == true) {
            // 識別子と名称と数量を上書きする。
            P_PLATECD[1] = PLabel1;
            P_PLATECD[3] = PName1;
            P_Suryo = PNum1;
            // 出力する。
            P_Unit = "";
            // 2003.12.09 UX-90,UX-100,GX-20以外のEﾌﾟﾚｰﾄｶﾞｽｹｯﾄはNBR
            // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode1),PMatCode);
            // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
            // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
            if (PCode1 == "60" || PCode1 == "55" || PCode1 == "56" ||
                PCode1 == "57") { // Ｅプレートガスケット（Ｅ）は材質をＮＢＲとする
                // 2013.11.06 UX-100材質変更
                // if ( G_KS_Syurui == "ＵＸ" && (G_KS_Model == "９０" || G_KS_Model == "１００") ){ // ＥプレートガスケットUX-90,100は材質変えない
                if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "９０")
                { // ＥプレートガスケットUX-90は材質変えない
                    // *************************
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                }
                else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "２０")
                { // ＥプレートガスケットGX-20は材質変えない
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                    // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                    ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                    // if ( DD_Label == "DDP-22" ) {
                    // P_Zuban = "Ｅ３４８１０７";     //図番
                    // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＳＸ" && (G_KS_Model == "９０Ｍ" ||
                    G_KS_Model == "９０Ｌ")) { // ＥプレートガスケットSX-90M,90Lは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "１００Ｒ")
                { // ＥプレートガスケットUX-100Rは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "ＲＸ" && G_KS_Model == "１００")
                { // ＥプレートガスケットRX-100は材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // Ｅプレートガスケット上記型式以外は材質をＮＢＲとする
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                }
                // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
            }
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_S
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０Ｌ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_E
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "２０" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // ＤプレートガスケットSX-20は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　　
            //else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // ＤプレートガスケットSX-30は材質変換しない
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************　　
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットSX-41は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４３" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "７０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00追加
            // 2021.01.06 CX-03追加(CX-00→CX-01)_S
            //else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // ＤプレートガスケットCX-00は材質変換しない
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットCX-01は材質変換しない
            // 2021.01.06 CX-03追加(CX-00→CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_S
            //else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // ＤプレートガスケットCXW-00は材質変換しない
             else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットCXW-01は材質変換しない
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.27 CX-01D追加_S
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットCX-01Dは材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D追加_E
            //2016.08.01 SX-30/30S Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　
            //ただし特殊な変更を行う
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０Ｓ" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************　
            // 2017.08.17 YX-83プレート部品追加_S
            else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３"  &&
                (PCode1 == "9" || PCode1 == "10")) {
                // ＤプレートガスケットYX-83は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2017.08.17 YX-83プレート部品追加_E
            // 2023.07.11 GX-50プレート部品追加_E
            else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "５０"  &&
                PCode1 == "6") {
                // ＤＣプレートガスケットGX-50は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50プレート部品追加_E
            else {
                // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                // ****************************************
                // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                // if ( DD_Label == "DDP-22" ) {
                // P_Zuban = "Ｅ３４８１０７";     //図番
                // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                // }
                ////******************************
                // ******************************
            }
            // ****************************************************
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(PMatCode); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 盲の場合
    }
    else {
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "D", PCode0, PMatCode);
        if (ret == true) {
            // 識別子と名称と数量を上書きする。
            P_PLATECD[1] = PLabel0;
            P_PLATECD[3] = PName0;
            P_Suryo = PNum0;
            // 出力する。
            P_Unit = "";
            // 2003.12.09 UX-90,UX-100,GX-20以外のEﾌﾟﾚｰﾄｶﾞｽｹｯﾄはNBR
            // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),PMatCode);
            // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
            // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
            if (PCode0 == "60" || PCode0 == "55" || PCode0 == "56" ||
                PCode0 == "57") { // Ｅプレートガスケット（Ｅ）は材質をＮＢＲとする
                // 2013.11.06 UX-100材質変更
                // if ( G_KS_Syurui == "ＵＸ" && (G_KS_Model == "９０" || G_KS_Model == "１００") ){ // ＥプレートガスケットUX-90,100は材質変えない
                if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "９０")
                { // ＥプレートガスケットUX-90は材質変えない
                    // *************************
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                }
                else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "２０")
                { // ＥプレートガスケットGX-20は材質変えない
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                    // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                    ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                    // if ( DD_Label == "DDP-22" ) {
                    // P_Zuban = "Ｅ３４８１０７";     //図番
                    // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＳＸ" && (G_KS_Model == "９０Ｍ" ||
                    G_KS_Model == "９０Ｌ")) { // ＥプレートガスケットSX-90M,90Lは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "１００Ｒ")
                { // ＥプレートガスケットUX-100Rは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "ＲＸ" && G_KS_Model == "１００")
                { // ＥプレートガスケットRX-100は材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // Ｅプレートガスケット上記型式以外は材質をＮＢＲとする
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                }
                // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
            }
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_S
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０Ｌ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_E
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "２０" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // ＤプレートガスケットSX-20は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　　
            //else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // ＤプレートガスケットSX-30は材質変換しない
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************　　
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットSX-41は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４３" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "７０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00追加
            // 2021.01.06 CX-03追加(CX-00→CX-01)_S
            //else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // ＤプレートガスケットCX-00は材質変換しない
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットCX-01は材質変換しない
            // 2021.01.06 CX-03追加(CX-00→CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_S
            //else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // ＤプレートガスケットCXW-00は材質変換しない
            else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットCXW-01は材質変換しない
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.27 CX-01D追加_S
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＤプレートガスケットCX-01Dは材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D追加_E
            //2016.08.01 SX-30/30S Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　
            //ただし特殊な変更を行う
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０Ｓ" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************　　
            // 2017.08.17 YX-83プレート部品追加_S
            else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３" &&
                (PCode1 == "9" || PCode1 == "10")) {
                // ＤプレートガスケットYX-83は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2017.08.17 YX-83プレート部品追加_E
            // 2023.07.11 GX-50プレート部品追加_S
            else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "５０"  &&
                PCode1 == "6") {
                // ＤＣプレートガスケットGX-50は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50プレート部品追加_E
            else {
                // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),PMatCode);
                // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                // ****************************************
                // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                // if ( DD_Label == "DDP-22" ) {
                // P_Zuban = "Ｅ３４８１０７";     //図番
                // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                // }
                ////******************************
                // ******************************
            }
            // ****************************************************
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(PMatCode); // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Ｄプレートガスケットディスタンスピース部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_D_Gskt_Data_Distance(void) {
    int ret;

    // マスタ検索関数(材質コードは固定だそうな・・・)
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "D", DD_Code, "2019");
    if (ret == true) {
        // 識別子と名称と数量を上書きする。
        P_PLATECD[1] = DD_Label;
        P_PLATECD[3] = DD_Name;
        P_Suryo = DD_Num;
        // 2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
        if (DD_Label == "DDP-22") {
            P_Zuban = "Ｅ３４８１０７"; // 図番
            P_SZuban = "Ｅ３４８１０７"; // 作図用図番
        }
        // ******************************
        // 出力する。
        P_Unit = "式";
        // P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(DD_Code),"2019");
        P_Yobisu = Get_Gskt_YobiSu(DD_Code, "2019");
        P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
        Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        Write_HeartsBuhin_Data(2019); // HEARTS用ﾃﾞｰﾀCSV作成
    }
    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Ｅプレートガスケット部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_E_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    // 2014.07.14 初期化
    ret = Search_HD_E_GASKET(G_E_GsktType, G_GsktType, G_FrmDiv);

    DE_Kubun = "E";

    // 2006.07.13 LX-10 TCG用材質判定追加及び、同材質の数量でも
    // HD_PARTS.CSVにDE_KubunとP_SideDivを記述する。
    // (ﾊﾟｰﾂﾘｽﾄで使用する)

    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
    if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０") { // LX-10
        if (s_B_GSKT_Mat == "2063" || s_A_GSKT_Mat == "2063") {
            // B側A側いずれかが2063(AT-TF/EPDM)の場合両側2063にする
            s_B_GSKT_Mat = "2063";
            s_A_GSKT_Mat = "2063";
        }
        //2017.10.10 AN-TF/EPDM追加_S
        else if (s_B_GSKT_Mat == "2064" || s_A_GSKT_Mat == "2064") {
            // B側A側いずれかが2064(AN-TF/EPDM)の場合両側2064にする
            s_B_GSKT_Mat = "2064";
            s_A_GSKT_Mat = "2064";
        }
        //2017.10.10 AN-TF/EPDM追加_E
    }

    // パターン分岐により必要なガスケットを選択
    // Aスタート
    if (G_E_GsktABKubun == "A") {
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, s_B_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, s_A_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // 型式とコードの比較
        // 孔による比較
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E1､E2孔&A,B同材質の数量で出力
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B同材質の数量で出力
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2008.07.24 CX-10D追加
        ////S3:B側の処理
        // P_SideDiv ="B";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, s_B_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S4:A側の処理
        // P_SideDiv ="A";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, s_A_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 斜行流追加_S
        //if (G_KS_Model.Pos("Ｄ") > 0) { // 斜行流
        if (P_FL == 1) { // 斜行流
        // 2017.08.17 斜行流追加_E
            // 斜行流S3:A側の処理
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num,
                EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // 斜行流S4:B側の処理
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num,
                EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        else {
            // S3:B側の処理
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num,
                EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // S4:A側の処理
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num,
                EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        // *********************
        // 型式とコードの比較
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E3､E4孔&A,B同材質の数量で出力
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        //2023/07/27 SX-10ガスケット展開修正_S
        if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "１０" ) {
            if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
                // A,B同材質の数量で出力
                P_Suryo = EPU_Meq_Sub;
                P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
                if (P_Suryo == 0) {
                    P_SideDiv = "";
                }
                else {
                    P_SideDiv = "A";
                }
                Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
                Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
            }
        }
        //2023/07/27 SX-10ガスケット展開修正_E
    }
    // Bスタート
    else {
        // 2008.07.24 CX-10D追加
        ////S4:A側の処理
        // P_SideDiv ="A";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, s_A_GSKT_Mat );
        // s_BLabel = P_PLATECD[1];
        ////S3:B側の処理
        // P_SideDiv ="B";
        // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, s_B_GSKT_Mat );
        // s_ALabel = P_PLATECD[1];
        // 2017.08.17 斜行流追加_S
        //if (G_KS_Model.Pos("Ｄ") > 0) { // 斜行流
        if (P_FL == 1) { // 斜行流
        // 2017.08.17 斜行流追加_E
            // 斜行流S4:B側の処理
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num,
                EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
            // 斜行流S3:A側の処理
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num,
                EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
        }
        else {
            // S4:A側の処理
            P_SideDiv = "A";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1),
                EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num,
                EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
                s_A_GSKT_Mat);
            s_ALabel = P_PLATECD[1];
            // S3:B側の処理
            P_SideDiv = "B";
            ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1),
                EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num,
                EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
                s_B_GSKT_Mat);
            s_BLabel = P_PLATECD[1];
        }
        // *********************
        // 型式とコードの比較
        // 孔による比較
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E3､E4孔&A,B同材質の数量で出力
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // A,B同材質の数量で出力
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, s_A_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, s_B_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && s_A_GSKT_Mat == s_B_GSKT_Mat) {
            // E1､E2孔&A,B同材質の数量で出力
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        //2023/07/27 SX-10ガスケット展開修正_S
        if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "１０" ) {
            if (s_ALabel == s_BLabel && s_A_GSKT_Mat == s_B_GSKT_Mat) {
                // A,B同材質の数量で出力
                P_Suryo = EPU_Meq_Sub;
                P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
                if (P_Suryo == 0) {
                    P_SideDiv = "";
                }
                else {
                    P_SideDiv = "B";
                }
                Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
                Write_HeartsBuhin_Data(s_B_GSKT_Mat); // HEARTS用ﾃﾞｰﾀCSV作成
            }
        }
        //2023/07/27 SX-10ガスケット展開修正_E
    }

    ////パターン分岐により必要なガスケットを選択
    ////Aスタート
    // if( G_E_GsktABKubun == "A" ){
    // //S1:B側の処理
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(1,1), EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S2:A側の処理
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(2,1), EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //型式とコードの比較
    // //孔による比較
    // if( s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,1) != "0" && G_E_GsktAna.SubString(2,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //E1､E2孔&A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = EPUH_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // P_SideDiv ="";
    // //A,B同材質の数量で出力
    // P_Suryo = EPU_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // //S3:B側の処理
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, P_HEADS_DATA[835].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S4:A側の処理
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, P_HEADS_DATA[836].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //型式とコードの比較
    // if( s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,1) != "0" && G_E_GsktAna.SubString(4,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight() ){
    // //A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = EPLH_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // }
    ////Bスタート
    // else{
    // //S4:A側の処理
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(4,1), EP_B0s1_Code, EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code, EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S3:B側の処理
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(3,1), EP_B0s2_Code, EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code, EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // //型式とコードの比較
    // //孔による比較
    // if( s_ALabel == s_BLabel &&  G_E_GsktAna.SubString(3,1) != "0" && G_E_GsktAna.SubString(4,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //E1､E2孔&A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = EPUH_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // if( s_ALabel == s_BLabel && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = EPU_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // //S2:A側の処理
    // P_SideDiv ="A";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(2,1), EP_B0s3_Code, EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code, EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num, P_HEADS_DATA[836].TrimRight() );
    // s_BLabel = P_PLATECD[1];
    // //S1:B側の処理
    // P_SideDiv ="B";
    // ret = Write_E_Gskt_Data_Sub( G_E_GsktAna.SubString(1,1), EP_B0s4_Code, EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code, EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num, P_HEADS_DATA[835].TrimRight() );
    // s_ALabel = P_PLATECD[1];
    // if( s_ALabel == s_BLabel &&  G_E_GsktAna.SubString(1,1) != "0" && G_E_GsktAna.SubString(2,1) != "0" && P_HEADS_DATA[836].TrimRight() == P_HEADS_DATA[835].TrimRight()){
    // //A,B同材質の数量で出力
    // P_SideDiv ="";
    // P_Suryo = EPLH_Meq_Sub;
    // //2004.01.20 予備数の取得訂正
    // P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
    // //***************************
    // Write_PlateBuhin_Data(ret);      // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); //HEARTS用ﾃﾞｰﾀCSV作成
    // }
    // }
    // **********************************

    // ディスタンスピース出力
    Write_E_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Ｅプレートガスケット部品　ﾃﾞｰﾀ作成 サブ
// 概  要      ：
// ：　　　　　　Ana 　 　：孔明データのS1,S2・・のデータ
// ：            PCode0   : 盲目部品ｺｰﾄﾞ
// ：            PLabel0  : 盲目部品ﾗﾍﾞﾙ
// ：            PName0   : 盲目部品名
// ：            PNum0    : 盲目部品数量
// ：            PCode1   : 孔明部品ｺｰﾄﾞ
// ：            PLabel1  : 盲目部品ﾗﾍﾞﾙ
// ：            PName1   : 盲目部品名
// ：            PNum1    : 盲目部品数量
// ：            PMatCode : 部品材質ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_E_Gskt_Data_Sub(AnsiString s_Ana,
    AnsiString PCode0, AnsiString PLabel0, AnsiString PName0, long PNum0,
    AnsiString PCode1, AnsiString PLabel1, AnsiString PName1, long PNum1,
    AnsiString PMatCode) {
    int ret;

    // 孔の場合
    if (s_Ana != "0") {
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "E", PCode1, PMatCode);
        if (ret == true) {
            // 識別子と名称と数量を上書きする。
            P_PLATECD[1] = PLabel1;
            P_PLATECD[3] = PName1;
            P_Suryo = PNum1;
            // 出力する。
            P_Unit = "";
            if (PCode1 == "79") { // Ｅノズルガスケットは材質を変換する
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 1);
                P_Yobisu = Syukei_Buhin_Yobisu("GSKT04", P_Zaisitu,
                    m_pcHeadsData);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // 2006.07.13 YX-80E通路ﾊﾟｯｷﾝｸﾞ追加
            }
            // 2019.03.18 YX-80変更_S
            //else if (PCode1 == "100") { // Ｅ通路パッキングは材質を変換する
            //    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 1);
            //    P_Yobisu = Syukei_Buhin_Yobisu("GSKT59", P_Zaisitu,
            //        m_pcHeadsData);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //    // ********************************
            //    // 2003.12.09 UX-90,UX-100,GX-20以外のEﾌﾟﾚｰﾄｶﾞｽｹｯﾄはNBR
            //    // } else if (PCode1 == "60" ) { // Ｅプレートガスケット（Ｅ）は材質をＮＢＲとする
            //    // P_Zaisitu = Search_HD_MATERIAL("2019");
            //    // P_EZaisitu = Search_HD_E_MATERIAL("2019");
            //    // //     P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode1),"2019");
            //    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),"2019");
            //    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
            //}
            // 2019.03.18 YX-80変更_E
            else if (PCode1 == "60" || PCode1 == "55" || PCode1 == "56" ||
                PCode1 == "57") { // Ｅプレートガスケット（Ｅ）は材質をＮＢＲとする
                // 2013.11.06 UX-100材質変更
                // if ( G_KS_Syurui == "ＵＸ" && (G_KS_Model == "９０" || G_KS_Model == "１００") ){ // ＥプレートガスケットUX-90,100は材質変えない
                if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "９０")
                { // ＥプレートガスケットUX-90は材質変えない
                    // *************************
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                }
                // 2022.04.12 LX-50 Eｶﾞｽｹｯﾄ材質G-NBRに変更_S
                else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "５０")
                { // ＥプレートガスケットLX-50は材質をＧ−ＮＢＲとする
                    P_Zaisitu = Search_HD_MATERIAL("2075");
                    P_EZaisitu = Search_HD_E_MATERIAL("2075");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2075");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                // 2022.04.12 LX-50 Eｶﾞｽｹｯﾄ材質G-NBRに変更_E
                else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "２０")
                { // ＥプレートガスケットGX-20は材質変えない
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                    // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                    ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                    // if ( ED_Label == "EDPC22" ) {
                    // P_Zuban = "Ｅ３４８１０７";     //図番
                    // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＳＸ" && (G_KS_Model == "９０Ｍ" ||
                    G_KS_Model == "９０Ｌ")) { // ＥプレートガスケットSX-90M,90Lは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "１００Ｒ")
                { // ＥプレートガスケットUX-100Rは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "ＲＸ" && G_KS_Model == "１００")
                { // ＥプレートガスケットRX-100は材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // Ｅプレートガスケット上記型式以外は材質をＮＢＲとする
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                // ****************************************************
                // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
            }
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_S
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０Ｌ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_E
            // 2017.07.18 SX-80Eガスケット追加
            else if (PCode1 == "108" || PCode1 == "109" ) {
                if (G_KS_Syurui == "ＳＸ" && (G_KS_Model == "８０Ｓ" ||
                    G_KS_Model == "８０Ｍ" || G_KS_Model == "８０Ｌ")) { // ＥプレートガスケットSX-80S,80M,80Lは材質GZ-NBR
                    P_Zaisitu = Search_HD_MATERIAL("2067");
                    P_EZaisitu = Search_HD_E_MATERIAL("2067");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2067");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                else { // Ｅプレートガスケット上記型式以外は材質をＮＢＲとする
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
            }
            // *******************************　　
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "２０" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // ＥプレートガスケットSX-20は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　　
            //else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // ＥプレートガスケットSX-30は材質変換しない
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットSX-41は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４３" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "７０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00追加
            // 2021.01.06 CX-03追加(CX-00→CX-01)_S
            //else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // ＥプレートガスケットCX-00は材質変換しない
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットCX-01は材質変換しない
            // 2021.01.06 CX-03追加(CX-00→CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_S
            //else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //      // ＥプレートガスケットCXW-00は材質変換しない
            else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットCXW-01は材質変換しない
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.08 CX-01D追加_S
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットCX-01Dは材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D追加_E
            //2016.08.01 SX-30/30S Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　
            //ただし特殊な変更を行う
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０Ｓ" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************　
            // 2017.08.23 YX-83プレート部品追加_S
            else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３" &&
                (PCode1 == "9" || PCode1 == "10" ||PCode1 == "110" || PCode1 == "111")) {
                // ＥプレートガスケットYX-83は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2017.08.23 YX-83プレート部品追加_E
            // 2023.07.11 GX-50プレート部品追加_S
            else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "５０" &&
                PCode1 == "6") {
                // ＤＣプレートガスケットGX-50は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50プレート部品追加_E
            else {
                // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode1),PMatCode);
                // P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
                // ****************************************
                // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                // if ( ED_Label == "EDPC22" ) {
                // P_Zuban = "Ｅ３４８１０７";     //図番
                // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                // }
                ////******************************
                // ******************************
            }
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(PMatCode); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 盲の場合
    }
    else {
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "E", PCode0, PMatCode);
        if (ret == true) {
            // 識別子と名称と数量を上書きする。
            P_PLATECD[1] = PLabel0;
            P_PLATECD[3] = PName0;
            P_Suryo = PNum0;
            if (PCode0 == "79") { // Ｅノズルガスケットは材質を変換する
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 1);
                P_Yobisu = Syukei_Buhin_Yobisu("GSKT04", P_Zaisitu,
                    m_pcHeadsData);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (PCode0 == "60" || PCode0 == "55" || PCode0 == "56" ||
                PCode0 == "57") { // Ｅプレートガスケット（Ｅ）は材質をＮＢＲとする
                // 2003.12.09 UX-90,UX-100,GX-20はNBRではない
                // P_Zaisitu = Search_HD_MATERIAL("2019");
                // P_EZaisitu = Search_HD_E_MATERIAL("2019");
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),"2019");
                // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),"2019");
                // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
                // 2013.11.06 UX-100材質変更
                // if ( G_KS_Syurui == "ＵＸ" && (G_KS_Model == "９０" || G_KS_Model == "１００") ){ // ＥプレートガスケットUX-90,100は材質変えない
                if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "９０")
                { // ＥプレートガスケットUX-90は材質変えない
                    // *************************
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                }
                // 2022.04.12 LX-50 Eｶﾞｽｹｯﾄ材質G-NBRに変更_S
                else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "５０")
                { // ＥプレートガスケットLX-50は材質をＧ−ＮＢＲとする
                    P_Zaisitu = Search_HD_MATERIAL("2075");
                    P_EZaisitu = Search_HD_E_MATERIAL("2075");
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), "2075");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                // 2022.04.12 LX-50 Eｶﾞｽｹｯﾄ材質G-NBRに変更_E
                else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "２０")
                { // ＥプレートガスケットGX-20は材質変えない
                    // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                    // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
                    // ****************************************
                    // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                    ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                    // if ( ED_Label == "EDPC22" ) {
                    // P_Zuban = "Ｅ３４８１０７";     //図番
                    // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                    // }
                    ////******************************
                    // ******************************
                    // 2006.05.31 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＳＸ" && (G_KS_Model == "９０Ｍ" ||
                    G_KS_Model == "９０Ｌ")) { // ＥプレートガスケットSX-90M,90Lは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                    // 2009.06.19 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                }
                else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "１００Ｒ")
                { // ＥプレートガスケットUX-100Rは材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                }
                else if (G_KS_Syurui == "ＲＸ" && G_KS_Model == "１００")
                { // ＥプレートガスケットRX-100は材質変えない
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                    // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                    // P_Yobisu += P_Suryo * P_PltGsktYobiD;    // ガスケット付きプレートの予備品あり
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                    // ****************************************
                    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                    // ****************************************
                }
                else { // Ｅプレートガスケット上記型式以外は材質をＮＢＲとする
                    P_Zaisitu = Search_HD_MATERIAL("2019");
                    P_EZaisitu = Search_HD_E_MATERIAL("2019");
                    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), "2019");
                    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                }
                // ******************************************
                // 2014.07.08 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
            }
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_S
            else if (G_KS_Syurui == "ＵＸ" && G_KS_Model == "３０Ｌ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットUX-30は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.08 UX-30L追加_E
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "２０" &&
                (PCode1 == "105" || PCode1 == "106")) {
                // ＥプレートガスケットSX-20は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //2016.07.25 SX-30Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　　
            //else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
            //    (PCode1 == "9" || PCode1 == "10")) {
            //    // ＥプレートガスケットSX-30は材質変換しない
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //*******************************
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットSX-41は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "４３" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "７０" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットSX-43は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // ****************************************
            }
            // 2016.04.28 CX-00追加
            // 2021.01.06 CX-03追加(CX-00→CX-01)_S
            //else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // ＥプレートガスケットCX-00は材質変換しない
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットCX-01は材質変換しない
            // 2021.01.06 CX-03追加(CX-00→CX-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_S
            //else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "００" &&
            //    (PCode1 == "7" || PCode1 == "8")) {
            //    // ＥプレートガスケットCXW-00は材質変換しない
            else if (G_KS_Syurui == "ＣＸＷ" && G_KS_Model == "０１" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットCXW-01は材質変換しない
            // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_E
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // ********************
            // 2022.04.27 CX-01D追加_S
            else if (G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ" &&
                (PCode1 == "7" || PCode1 == "8")) {
                // ＥプレートガスケットCX-01Dは材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2022.04.27 CX-01D追加_E
            //2016.08.01 SX-30/30S Dガスケット変更
            //中間ガスケットからの作成をやめ材質変更する　
            //ただし特殊な変更を行う
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "３０Ｓ" &&
                (PCode1 == "9" || PCode1 == "10")) {
                if ( PMatCode== "2019" || PMatCode== "2037" ||
                     PMatCode== "2052" || PMatCode== "2085" ) {
                    P_Zaisitu = "2037";  //N-NBR
                } else if ( PMatCode== "2026" || PMatCode== "2050" ||
                            PMatCode== "2051" || PMatCode== "2054" ||
                            PMatCode== "2081" ) {
                    P_Zaisitu = "2050";  //N-EPDM
                } else {
                    P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                }
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiD; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            //*******************************
            // 2020.03.04 YX-83修正_S
            //// 2017.08.23 YX-83プレート部品追加_S
            //else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３" &&
            //    (PCode1 == "9" || PCode1 == "10" || PCode1 == "110" || PCode1 == "111")) {
            //    // ＥプレートガスケットYX-83は材質変換しない
            //    P_Zaisitu = PMatCode;
            //    P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
            //    P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
            //    P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
            //    P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            //}
            //// 2017.08.23 YX-83プレート部品追加_E
            else if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３" &&
                (PCode1 == "9" || PCode1 == "110" || PCode1 == "111")) {
                // ＤＡプレートガスケットYX-83(盲)は材質をＮＢＲとする
                // ＥＢ１プレートガスケットYX-83(盲)は材質をＮＢＲとする
                // ＥＢ２プレートガスケットYX-83(盲)は材質をＮＢＲとする
                P_Zaisitu = Search_HD_MATERIAL("2019");
                P_EZaisitu = Search_HD_E_MATERIAL("2019");
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
            }
            // 2020.03.04 YX-83修正_E
            // 2023.07.11 GX-50プレート部品追加_S
            else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "５０" &&
                PCode1 == "6") {
                // ＤＣプレートガスケットGX-50は材質変換しない
                P_Zaisitu = PMatCode;
                P_Yobisu = Get_Gskt_YobiSu(PCode1.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
            }
            // 2023.07.11 GX-50プレート部品追加_E
            else {
                // 2004.06.09 DEﾌﾟﾚｰﾄｶﾞｽｹｯﾄ材質ｺｰﾄﾞ変換追加
                P_Zaisitu = Chg_Zaisitu_Code(PMatCode, 3);
                P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0), P_Zaisitu);
                P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
                P_EZaisitu = Search_HD_E_MATERIAL(P_Zaisitu);
                P_Zaisitu = Search_HD_MATERIAL(P_Zaisitu);
                // //  P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(PCode0),PMatCode);
                // P_Yobisu = Get_Gskt_YobiSu(PCode0.ToIntDef(0),PMatCode);
                // P_Yobisu += P_Suryo * P_PltGsktYobiE;    // ガスケット付きプレートの予備品あり
                // ****************************************
                // 2004.12.15 GX-22ｶﾞｽｹｯﾄ図番取消
                ////2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
                // if ( ED_Label == "EDPC22" ) {
                // P_Zuban = "Ｅ３４８１０７";     //図番
                // P_SZuban = "Ｅ３４８１０７";    //作図用図番
                // }
                ////******************************
                // ******************************
            }
            // 出力する。
            P_Unit = "";
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(PMatCode); // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： Ｅプレートガスケットディスタンスピース部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Write_E_Gskt_Data_Distance(void) {
    int ret;
    // 2019.08.01 Ｓ、Ｔディスタンスピース追加_S
    AnsiString ED_Name;
    AnsiString ED_Label;
    long ED_Code;
    long ED_Num;
    // 2019.08.01 Ｓ、Ｔディスタンスピース追加_E

    // 2019.08.01 Ｓ、Ｔディスタンスピース追加_S
    // Ｅプレート孔明判断
    if (G_E_GsktAna.SubString(1, 4) == "0000") {
        ED_Name = ED_B0_Name;
        ED_Label = ED_B0_Label;
        ED_Code = ED_B0_Code;
        ED_Num = ED_B0_Num;
    } else {
        ED_Name = ED_B1_Name;
        ED_Label = ED_B1_Label;
        ED_Code = ED_B1_Code;
        ED_Num = ED_B1_Num;
    }
    // 2019.08.01 Ｓ、Ｔディスタンスピース追加_E

    // マスタ検索関数(材質コードは固定だそうな・・・)
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "E", ED_Code, "2019");
    if (ret == true) {
        // 識別子と名称と数量を上書きする。
        P_PLATECD[1] = ED_Label;
        P_PLATECD[3] = ED_Name;
        P_Suryo = ED_Num;
        // 2004.11.22 GX-22ｶﾞｽｹｯﾄ図番変更
        if (ED_Label == "EDPC22") {
        // 2019.08.01 Ｓ、Ｔディスタンスピース追加_E
            P_Zuban = "Ｅ３４８１０７"; // 図番
            P_SZuban = "Ｅ３４８１０７"; // 作図用図番
        }
        // ******************************
        // 出力する。
        P_Unit = "式";
        // P_Yobisu = Syukei_Buhin_Yobisu(Get_Yobi_Code(ED_Code),"2019");
        P_Yobisu = Get_Gskt_YobiSu(ED_Code, "2019");
        P_Yobisu += P_Suryo * P_PltGsktYobiE; // ガスケット付きプレートの予備品あり
        Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        Write_HeartsBuhin_Data(2019); // HEARTS用ﾃﾞｰﾀCSV作成
    }

    return ret;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ガスケットマスタの検索キーを外部変数へ格納
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Get_Gskt_MastKey(int DEmode) {

    AnsiString s_Text;

    //
    // ﾌﾚｰﾑﾀｲﾌﾟ
    //
    G_FrmType = G_S_Model;

    //
    // ｶﾞｽｹｯﾄ種類
    //
    // 右端の一文字を取る
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 5) == "UX-31") ||
        (G_D_GsktType.SubString(1, 5) == "UX-32")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 5) == "UX-31") ||
        (G_E_GsktType.SubString(1, 5) == "UX-32"))) {
        s_Text = P_HEADS_DATA[35].TrimRight().SubString
            (P_HEADS_DATA[35].Length(), 1);
        G_GsktType = s_Text.SubString(s_Text.Length() - 1, 1);
        if (G_GsktType.IsDelimiter("A", 1) == false)
            G_GsktType = "N";
            // 2022.04.19 UX-30L追加_S
            if ((DEmode == 0 && (G_D_GsktType.SubString(1, 6) == "UX-31L") ||
                (G_D_GsktType.SubString(1, 6) == "UX-32L")) ||
                (DEmode == 1 && (G_E_GsktType.SubString(1, 6) == "UX-31L") ||
                (G_E_GsktType.SubString(1, 6) == "UX-32L"))) {
                G_GsktType = "-";
            }
            // 2022.04.19 UX-30L追加_E
        // 2006.07.13 LX-10 TCG用材質判定追加
    }
    else if ((DEmode == 0 && (G_D_GsktType.SubString(1, 5) == "LX-11") ||
        (G_D_GsktType.SubString(1, 5) == "LX-12")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 5) == "LX-11") ||
        (G_E_GsktType.SubString(1, 5) == "LX-12"))) {
        AnsiString s_B_GSKT_Mat;
        AnsiString s_A_GSKT_Mat;
        s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
        s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
        if (s_B_GSKT_Mat == "2063" || s_A_GSKT_Mat == "2063") {
            // B側A側いずれかが2063(AT-TF/EPDM)の場合
            G_GsktType = "T";
            // 2010.02.10 LX-10 2064(AN-TF/EPDM)追加
        }
        else if (s_B_GSKT_Mat == "2064" || s_A_GSKT_Mat == "2064") {
            // B側A側いずれかが2064(AN-TF/EPDM)の場合
            G_GsktType = "T";
            // *************************************
        }
        else {
            G_GsktType = "-";
        }
        // **********************************
    }
    // 2017.07.18 SX-80 GZ-NBR用材質判定追加
    else if (DEmode == 1 && (G_E_GsktType.SubString(1, 4) == "SX-8")) {
        AnsiString s_B_GSKT_Mat;
        AnsiString s_A_GSKT_Mat;
        s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
        s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
        if (s_B_GSKT_Mat == "2067" || s_B_GSKT_Mat == "2068") {
            // B側が2067(GZ-NBR)、2068(AGZ-NBR)の場合
            if (s_A_GSKT_Mat == "2067" || s_A_GSKT_Mat == "2068") {
                // A側B側が2067(GZ-NBR)、2068(AGZ-NBR)の場合
                G_GsktType = "GZ-NBR";
            }
            else {
                // B側のみが2067(GZ-NBR)、2068(AGZ-NBR)の場合
                G_GsktType = "GZ-NBRB";
            }
        }
        else  if (s_A_GSKT_Mat == "2067" || s_A_GSKT_Mat == "2068") {
            // A側のみが2067(GZ-NBR)、2068(AGZ-NBR)の場合
            G_GsktType = "GZ-NBRA";
        }
        else {
            G_GsktType = "-";
        }
    }
    // *************************************
    else {
        // コードの最後の文字が"A"以外なら不要
        if (G_GsktType.IsDelimiter("A", 1) == false)
            G_GsktType = "-";
    }

    //
    // ﾌﾚｰﾑ区分
    //
    // 36番目の先頭一桁
    s_Text = P_HEADS_DATA[36].TrimRight().SubString(1, 1);
    AnsiString s_FType;
    G_FrmDiv = "-";
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 5) == "LX-11") ||
        (G_D_GsktType.SubString(1, 5) == "LX-12")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 5) == "LX-11") ||
        (G_E_GsktType.SubString(1, 5) == "LX-12"))) {
        if (s_Text.SubString(1, 1).IsDelimiter("KT", 1) == true) {
            G_FrmDiv = "K";
        }
        else if (s_Text.SubString(1, 1) == 'N') {
            G_FrmDiv = "N";
            // 2005.02.23 LX-10B追加
        }
        else {
            G_FrmDiv = "K";
            // *********************
        }
    }

    // 2006.01.10 WX用ﾌﾟﾚｰﾄのﾌﾚｰﾑ区分の設定
    int iSuryo;
    int iMod;
    // 左端の２文字
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 2) == "WX")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 2) == "WX"))) {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // 偶数 Ｅカセット
            G_FrmDiv = "C";
        }
        else {
            // 奇数 Ｅプレート
            G_FrmDiv = "P";
        }
    }
    // *****************************
    // 2021.09.22 WH-30追加_S
    // 左端の２文字
    if ((DEmode == 0 && (G_D_GsktType.SubString(1, 2) == "WH")) ||
        (DEmode == 1 && (G_E_GsktType.SubString(1, 2) == "WH"))) {
        iSuryo = int(P_HEADS_DATA[38].ToIntDef(0));
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // 偶数 Ｅカセット
            G_FrmDiv = "C";
        }
        else {
            // 奇数 Ｅプレート
            G_FrmDiv = "P";
        }
    }
    // 2021.09.22 WH-30追加_E

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ・材質ｺｰﾄﾞﾏｽﾀ検索（ﾌﾟﾚｰﾄ部品 ﾃﾞｰﾀ作成用）
// 概  要      ：
// 引  数      ： AnsiString Key      : ﾌﾟﾚｰﾄ型式
// ：            PType    : ﾌﾟﾚｰﾄ種別
// ：            PCode    : 部品ｺｰﾄﾞ
// ：            PMatCode : 部品材質ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_PLATE_3(AnsiString Key,
    AnsiString PType, AnsiString PCode, AnsiString PMatCode) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_PLATE";
    s_SQL = s_SQL + "  WHERE MODEL_CODE = '" + Key + "'";
    // s_SQL = s_SQL + "    AND PLATE_TYPE = '" + PType + "'";
    s_SQL = s_SQL + "    AND PARTS_CODE = " + PCode + "";

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
        P_Zaisitu = "";
        P_EZaisitu = "";
        P_Suryo = 0;

        delete wkQuery;
        return false;
        // return true;
    }
    else {
        P_Zuban = wkQuery->FieldByName("DNO1")->AsString.TrimRight();
        P_SZuban = wkQuery->FieldByName("WKDNO1")->AsString.TrimRight();
        P_MAT_CODE1 = wkQuery->FieldByName("MAT_CODE1")->AsString.TrimRight();
        P_Suryo = wkQuery->FieldByName("NUMS")->AsString.ToIntDef(0);
        P_Zaisitu = Search_HD_MATERIAL(PMatCode);
        P_EZaisitu = Search_HD_E_MATERIAL(PMatCode);

        // // 材質ｺｰﾄﾞﾏｽﾀ検索
        // s_SQL = "";
        // s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
        // s_SQL = s_SQL + "  WHERE MAT_CODE = '" + PMatCode + "'";
        //
        // wkQuery->Close();
        // wkQuery->DatabaseName = ODBC_DSN_MAT;
        // wkQuery->SQL->Clear();
        // wkQuery->SQL->Add(s_SQL);
        // wkQuery->Open();
        // wkQuery->First();
        //
        // if(wkQuery->Eof){
        // P_Zaisitu = "";
        // G_ErrLog_Text = "材質ｺｰﾄﾞ『" + PMatCode + "』は存在しません！！";
        // Write_Error_Log(G_ErrLog_Text);
        // } else {
        // if(P_HEADS_DATA[1].TrimRight() == "0" ){
        // // 和文名称の取得
        // P_Zaisitu = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
        // } else {
        // P_Zaisitu = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
        // }
        // }
        // wkQuery->Close();
        //
        // delete wkQuery;

        // 予備数取得
        // P_Yobisu = Search_GSKT_Yobi(PCode, P_MAT_CODE1);

        return true;

    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： Dｶﾞｽｹｯﾄﾏｽﾀ検索
// 概  要      ：
// 引  数      ： AnsiString FRM_TYPE  : ﾌﾟﾚｰﾄ型式
// ：            GSKT_TYPE : ﾌﾟﾚｰﾄ種別
// ：            FRM_DIV   : 部品ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_D_GASKET(AnsiString FRM_TYPE,
    AnsiString GSKT_TYPE, AnsiString FRM_DIV) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_D_GASKET";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + FRM_TYPE + "'";
    s_SQL = s_SQL + "    AND GSKT_TYPE = '" + GSKT_TYPE + "'";
    s_SQL = s_SQL + "    AND FRM_DIV = '" + FRM_DIV + "'";

    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        DP_B1s1_Name = "";
        DP_B1s1_Label = "";
        DP_B1s1_Code = 0;
        DP_B1s1_Num = 0;
        DP_B0s1_Name = "";
        DP_B0s1_Label = "";
        DP_B0s1_Code = 0;
        DP_B0s1_Num = 0;
        DP_B1s2_Name = "";
        DP_B1s2_Label = "";
        DP_B1s2_Code = 0;
        DP_B1s2_Num = 0;
        DP_B0s2_Name = "";
        DP_B0s2_Label = "";
        DP_B0s2_Code = 0;
        DP_B0s2_Num = 0;
        DPU_Meq_Sub = 0;
        DP_B1s3_Name = "";
        DP_B1s3_Label = "";
        DP_B1s3_Code = 0;
        DP_B1s3_Num = 0;
        DP_B0s3_Name = "";
        DP_B0s3_Label = "";
        DP_B0s3_Code = 0;
        DP_B0s3_Num = 0;
        DP_B1s4_Name = "";
        DP_B1s4_Label = "";
        DP_B1s4_Code = 0;
        DP_B1s4_Num = 0;
        DP_B0s4_Name = "";
        DP_B0s4_Label = "";
        DP_B0s4_Code = 0;
        DP_B0s4_Num = 0;
        DPL_Meq_Sub = 0;
        DD_Name = "";
        DD_Label = "";
        DD_Code = 0;
        DD_Num = 0;

        delete wkQuery;
        // return false;
        return true;
    }
    else {

        DP_B1s1_Name = wkQuery->FieldByName("DP_B1S1_NAME")
            ->AsString.TrimRight();
        DP_B1s1_Label = wkQuery->FieldByName("DP_B1S1_LABEL")
            ->AsString.TrimRight();
        DP_B1s1_Code = wkQuery->FieldByName("DP_B1S1_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s1_Num = wkQuery->FieldByName("DP_B1S1_NUM")->AsString.ToIntDef(0);
        DP_B0s1_Name = wkQuery->FieldByName("DP_B0S1_NAME")
            ->AsString.TrimRight();
        DP_B0s1_Label = wkQuery->FieldByName("DP_B0S1_LABEL")
            ->AsString.TrimRight();
        DP_B0s1_Code = wkQuery->FieldByName("DP_B0S1_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s1_Num = wkQuery->FieldByName("DP_B0S1_NUM")->AsString.ToIntDef(0);
        DP_B1s2_Name = wkQuery->FieldByName("DP_B1S2_NAME")
            ->AsString.TrimRight();
        DP_B1s2_Label = wkQuery->FieldByName("DP_B1S2_LABEL")
            ->AsString.TrimRight();
        DP_B1s2_Code = wkQuery->FieldByName("DP_B1S2_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s2_Num = wkQuery->FieldByName("DP_B1S2_NUM")->AsString.ToIntDef(0);
        DP_B0s2_Name = wkQuery->FieldByName("DP_B0S2_NAME")
            ->AsString.TrimRight();
        DP_B0s2_Label = wkQuery->FieldByName("DP_B0S2_LABEL")
            ->AsString.TrimRight();
        DP_B0s2_Code = wkQuery->FieldByName("DP_B0S2_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s2_Num = wkQuery->FieldByName("DP_B0S2_NUM")->AsString.ToIntDef(0);
        DPU_Meq_Sub = wkQuery->FieldByName("DPU_MEQ_SUB")->AsString.ToIntDef(0);
        DP_B1s3_Name = wkQuery->FieldByName("DP_B1S3_NAME")
            ->AsString.TrimRight();
        DP_B1s3_Label = wkQuery->FieldByName("DP_B1S3_LABEL")
            ->AsString.TrimRight();
        DP_B1s3_Code = wkQuery->FieldByName("DP_B1S3_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s3_Num = wkQuery->FieldByName("DP_B1S3_NUM")->AsString.ToIntDef(0);
        DP_B0s3_Name = wkQuery->FieldByName("DP_B0S3_NAME")
            ->AsString.TrimRight();
        DP_B0s3_Label = wkQuery->FieldByName("DP_B0S3_LABEL")
            ->AsString.TrimRight();
        DP_B0s3_Code = wkQuery->FieldByName("DP_B0S3_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s3_Num = wkQuery->FieldByName("DP_B0S3_NUM")->AsString.ToIntDef(0);
        DP_B1s4_Name = wkQuery->FieldByName("DP_B1S4_NAME")
            ->AsString.TrimRight();
        DP_B1s4_Label = wkQuery->FieldByName("DP_B1S4_LABEL")
            ->AsString.TrimRight();
        DP_B1s4_Code = wkQuery->FieldByName("DP_B1S4_CODE")
            ->AsString.ToIntDef(0);
        DP_B1s4_Num = wkQuery->FieldByName("DP_B1S4_NUM")->AsString.ToIntDef(0);
        DP_B0s4_Name = wkQuery->FieldByName("DP_B0S4_NAME")
            ->AsString.TrimRight();
        DP_B0s4_Label = wkQuery->FieldByName("DP_B0S4_LABEL")
            ->AsString.TrimRight();
        DP_B0s4_Code = wkQuery->FieldByName("DP_B0S4_CODE")
            ->AsString.ToIntDef(0);
        DP_B0s4_Num = wkQuery->FieldByName("DP_B0S4_NUM")->AsString.ToIntDef(0);
        DPL_Meq_Sub = wkQuery->FieldByName("DPL_MEQ_SUB")->AsString.ToIntDef(0);
        DD_Name = wkQuery->FieldByName("DD_NAME")->AsString.TrimRight();
        DD_Label = wkQuery->FieldByName("DD_LABEL")->AsString.TrimRight();
        DD_Code = wkQuery->FieldByName("DD_CODE")->AsString.ToIntDef(0);
        DD_Num = wkQuery->FieldByName("DD_NUM")->AsString.ToIntDef(0);

        wkQuery->Close();

        delete wkQuery;
        return true;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： Eｶﾞｽｹｯﾄﾏｽﾀ検索
// 概  要      ：
// 引  数      ： AnsiString FRM_TYPE  : ﾌﾟﾚｰﾄ型式
// ：            GSKT_TYPE : ﾌﾟﾚｰﾄ種別
// ：            FRM_DIV   : 部品ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_E_GASKET(AnsiString FRM_TYPE,
    AnsiString GSKT_TYPE, AnsiString FRM_DIV) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_E_GASKET";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + FRM_TYPE + "'";
    s_SQL = s_SQL + "    AND GSKT_TYPE = '" + GSKT_TYPE + "'";
    s_SQL = s_SQL + "    AND FRM_DIV = '" + FRM_DIV + "'";

    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {
        EP_B1s1_Name = "";
        EP_B1s1_Label = "";
        EP_B1s1_Code = 0;
        EP_B1s1_Num = 0;
        EP_B0s1_Name = "";
        EP_B0s1_Label = "";
        EP_B0s1_Code = 0;
        EP_B0s1_Num = 0;
        EP_B1s2_Name = "";
        EP_B1s2_Label = "";
        EP_B1s2_Code = 0;
        EP_B1s2_Num = 0;
        EP_B0s2_Name = "";
        EP_B0s2_Label = "";
        EP_B0s2_Code = 0;
        EP_B0s2_Num = 0;
        EPUH_Meq_Sub = 0;
        EPU_Meq_Sub = 0;
        EP_B1s3_Name = "";
        EP_B1s3_Label = "";
        EP_B1s3_Code = 0;
        EP_B1s3_Num = 0;
        EP_B0s3_Name = "";
        EP_B0s3_Label = "";
        EP_B0s3_Code = 0;
        EP_B0s3_Num = 0;
        EP_B1s4_Name = "";
        EP_B1s4_Label = "";
        EP_B1s4_Code = 0;
        EP_B1s4_Num = 0;
        EP_B0s4_Name = "";
        EP_B0s4_Label = "";
        EP_B0s4_Code = 0;
        EP_B0s4_Num = 0;
        EPLH_Meq_Sub = 0;
        ED_Hp = 0;
        // 2019.08.01 Ｓ、Ｔディスタンスピース追加_S
        //ED_Name = "";
        //ED_Label = "";
        //ED_Code = 0;
        //ED_Num = 0;
        ED_B1_Name = "";
        ED_B1_Label = "";
        ED_B1_Code = 0;
        ED_B1_Num = 0;
        ED_B0_Name = "";
        ED_B0_Label = "";
        ED_B0_Code = 0;
        ED_B0_Num = 0;
        // 2019.08.01 Ｓ、Ｔディスタンスピース追加_E

        delete wkQuery;
        // return false;
        return true;
    }
    else {

        EP_B1s1_Name = wkQuery->FieldByName("EP_B1S1_NAME")
            ->AsString.TrimRight();
        EP_B1s1_Label = wkQuery->FieldByName("EP_B1S1_LABEL")
            ->AsString.TrimRight();
        EP_B1s1_Code = wkQuery->FieldByName("EP_B1S1_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s1_Num = wkQuery->FieldByName("EP_B1S1_NUM")->AsString.ToIntDef(0);
        EP_B0s1_Name = wkQuery->FieldByName("EP_B0S1_NAME")
            ->AsString.TrimRight();
        EP_B0s1_Label = wkQuery->FieldByName("EP_B0S1_LABEL")
            ->AsString.TrimRight();
        EP_B0s1_Code = wkQuery->FieldByName("EP_B0S1_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s1_Num = wkQuery->FieldByName("EP_B0S1_NUM")->AsString.ToIntDef(0);
        EP_B1s2_Name = wkQuery->FieldByName("EP_B1S2_NAME")
            ->AsString.TrimRight();
        EP_B1s2_Label = wkQuery->FieldByName("EP_B1S2_LABEL")
            ->AsString.TrimRight();
        EP_B1s2_Code = wkQuery->FieldByName("EP_B1S2_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s2_Num = wkQuery->FieldByName("EP_B1S2_NUM")->AsString.ToIntDef(0);
        EP_B0s2_Name = wkQuery->FieldByName("EP_B0S2_NAME")
            ->AsString.TrimRight();
        EP_B0s2_Label = wkQuery->FieldByName("EP_B0S2_LABEL")
            ->AsString.TrimRight();
        EP_B0s2_Code = wkQuery->FieldByName("EP_B0S2_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s2_Num = wkQuery->FieldByName("EP_B0S2_NUM")->AsString.ToIntDef(0);
        EPUH_Meq_Sub = wkQuery->FieldByName("EPUH_MEQ_SUB")
            ->AsString.ToIntDef(0);
        EPU_Meq_Sub = wkQuery->FieldByName("EPU_MEQ_SUB")->AsString.ToIntDef(0);
        EP_B1s3_Name = wkQuery->FieldByName("EP_B1S3_NAME")
            ->AsString.TrimRight();
        EP_B1s3_Label = wkQuery->FieldByName("EP_B1S3_LABEL")
            ->AsString.TrimRight();
        EP_B1s3_Code = wkQuery->FieldByName("EP_B1S3_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s3_Num = wkQuery->FieldByName("EP_B1S3_NUM")->AsString.ToIntDef(0);
        EP_B0s3_Name = wkQuery->FieldByName("EP_B0S3_NAME")
            ->AsString.TrimRight();
        EP_B0s3_Label = wkQuery->FieldByName("EP_B0S3_LABEL")
            ->AsString.TrimRight();
        EP_B0s3_Code = wkQuery->FieldByName("EP_B0S3_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s3_Num = wkQuery->FieldByName("EP_B0S3_NUM")->AsString.ToIntDef(0);
        EP_B1s4_Name = wkQuery->FieldByName("EP_B1S4_NAME")
            ->AsString.TrimRight();
        EP_B1s4_Label = wkQuery->FieldByName("EP_B1S4_LABEL")
            ->AsString.TrimRight();
        EP_B1s4_Code = wkQuery->FieldByName("EP_B1S4_CODE")
            ->AsString.ToIntDef(0);
        EP_B1s4_Num = wkQuery->FieldByName("EP_B1S4_NUM")->AsString.ToIntDef(0);
        EP_B0s4_Name = wkQuery->FieldByName("EP_B0S4_NAME")
            ->AsString.TrimRight();
        EP_B0s4_Label = wkQuery->FieldByName("EP_B0S4_LABEL")
            ->AsString.TrimRight();
        EP_B0s4_Code = wkQuery->FieldByName("EP_B0S4_CODE")
            ->AsString.ToIntDef(0);
        EP_B0s4_Num = wkQuery->FieldByName("EP_B0S4_NUM")->AsString.ToIntDef(0);
        EPLH_Meq_Sub = wkQuery->FieldByName("EPLH_MEQ_SUB")
            ->AsString.ToIntDef(0);
        // 2019.08.01 Ｓ、Ｔディスタンスピース追加_S
        //ED_Name = wkQuery->FieldByName("ED_NAME")->AsString.TrimRight();
        //ED_Label = wkQuery->FieldByName("ED_LABEL")->AsString.TrimRight();
        //ED_Code = wkQuery->FieldByName("ED_CODE")->AsString.ToIntDef(0);
        //ED_Num = wkQuery->FieldByName("ED_NUM")->AsString.ToIntDef(0);
        ED_B1_Name = wkQuery->FieldByName("ED_B1_NAME")->AsString.TrimRight();
        ED_B1_Label = wkQuery->FieldByName("ED_B1_LABEL")->AsString.TrimRight();
        ED_B1_Code = wkQuery->FieldByName("ED_B1_CODE")->AsString.ToIntDef(0);
        ED_B1_Num = wkQuery->FieldByName("ED_B1_NUM")->AsString.ToIntDef(0);
        ED_B0_Name = wkQuery->FieldByName("ED_B0_NAME")->AsString.TrimRight();
        ED_B0_Label = wkQuery->FieldByName("ED_B0_LABEL")->AsString.TrimRight();
        ED_B0_Code = wkQuery->FieldByName("ED_B0_CODE")->AsString.ToIntDef(0);
        ED_B0_Num = wkQuery->FieldByName("ED_B0_NUM")->AsString.ToIntDef(0);
        // 2019.08.01 Ｓ、Ｔディスタンスピース追加_E

        // 2003.10.17 ラバー、チューブの場合Ｅノズルガスケットはつかない
        // 2011.06.20 ﾉｽﾞﾙ規格変更
        // if( EP_B1s1_Code == 79 && (P_HEADS_DATA[953].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[953].TrimRight().SubString(2,1) == "2") ) EP_B1s1_Num = 0;
        // if( EP_B1s2_Code == 79 && (P_HEADS_DATA[968].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[968].TrimRight().SubString(2,1) == "2") ) EP_B1s2_Num = 0;
        // if( EP_B1s3_Code == 79 && (P_HEADS_DATA[983].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[983].TrimRight().SubString(2,1) == "2") ) EP_B1s3_Num = 0;
        // if( EP_B1s4_Code == 79 && (P_HEADS_DATA[998].TrimRight().SubString(2,1) == "1" || P_HEADS_DATA[998].TrimRight().SubString(2,1) == "2") ) EP_B1s4_Num = 0;
        if (EP_B1s1_Code == 79 && (P_HEADS_DATA[953].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[953].TrimRight().SubString(3, 1) == "2"))
            EP_B1s1_Num = 0;
        if (EP_B1s2_Code == 79 && (P_HEADS_DATA[968].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[968].TrimRight().SubString(3, 1) == "2"))
            EP_B1s2_Num = 0;
        if (EP_B1s3_Code == 79 && (P_HEADS_DATA[983].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[983].TrimRight().SubString(3, 1) == "2"))
            EP_B1s3_Num = 0;
        if (EP_B1s4_Code == 79 && (P_HEADS_DATA[998].TrimRight().SubString(3,
            1) == "1" || P_HEADS_DATA[998].TrimRight().SubString(3, 1) == "2"))
            EP_B1s4_Num = 0;
        // *************************************************************

        wkQuery->Close();

        delete wkQuery;
        return true;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： BFRGﾏｽﾀ検索
// 概  要      ：
// 引  数      ： AnsiString FRM_TYPE  : ﾌﾟﾚｰﾄ型式
// ：            GSKT_TYPE : ﾌﾟﾚｰﾄ種別
// ：            FRM_DIV   : 部品ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
bool __stdcall TPlateDetails::Search_HD_BFRG(AnsiString FRM_TYPE,
    AnsiString GSKT_TYPE, AnsiString FRM_DIV) {
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *wkQuery;
    //wkQuery = new TQuery(DataModule1);
    TFDQuery *wkQuery;
    wkQuery = new TFDQuery(DataModule1);
    // 2021.06.18 FireDACへの移行 MOD_E

    AnsiString s_SQL;

    s_SQL = "";
    s_SQL = s_SQL + "SELECT * FROM HD_BFRG";
    s_SQL = s_SQL + "  WHERE FRM_TYPE = '" + FRM_TYPE + "'";
    s_SQL = s_SQL + "    AND GSKT_TYPE = '" + GSKT_TYPE + "'";
    s_SQL = s_SQL + "    AND FRM_DIV = '" + FRM_DIV + "'";

    // 2021.06.18 FireDACへの移行 MOD_S
    //wkQuery->DatabaseName = ODBC_DSN_DNO;
    wkQuery->ConnectionName = ODBC_DSN_DNO;
    // 2021.06.18 FireDACへの移行 MOD_E
    wkQuery->SQL->Clear();
    wkQuery->SQL->Add(s_SQL);
    wkQuery->Open();
    wkQuery->First();

    if (wkQuery->Eof) {

        BP_Hp = 0;
        BBP_Name = "";
        BBP_Label = "";
        BBP_Code = 0;
        BBP_Num = 0;
        ABP_Name = "";
        ABP_Label = "";
        ABP_Code = 0;
        ABP_Num = 0;
        Note = "";

        delete wkQuery;
        // return false;
        return true;
    }
    else {

        BP_Hp = wkQuery->FieldByName("BP_HP")->AsString.ToIntDef(0);
        BBP_Name = wkQuery->FieldByName("BBP_NAME")->AsString.TrimRight();
        BBP_Label = wkQuery->FieldByName("BBP_LABEL")->AsString.TrimRight();
        BBP_Code = wkQuery->FieldByName("BBP_CODE")->AsString.ToIntDef(0);
        BBP_Num = wkQuery->FieldByName("BBP_NUM")->AsString.ToIntDef(0);
        ABP_Name = wkQuery->FieldByName("ABP_NAME")->AsString.TrimRight();
        ABP_Label = wkQuery->FieldByName("ABP_LABEL")->AsString.TrimRight();
        ABP_Code = wkQuery->FieldByName("ABP_CODE")->AsString.ToIntDef(0);
        ABP_Num = wkQuery->FieldByName("ABP_NUM")->AsString.ToIntDef(0);
        Note = wkQuery->FieldByName("NOTE")->AsString.TrimRight();

        wkQuery->Close();

        delete wkQuery;
        return true;
    }
}

// ---------------------------------------------------------------------------
// 日本語関数名： ガスケット予備品検索
// 概  要      ：
// 引  数      ： AnsiString PartsCD  : 部品番号
// ：            MatCD    : 材質ｺｰﾄﾞ
// 戻り値      ： 処理結果 [該当予備品数量]
// 備  考      ： なし
// ---------------------------------------------------------------------------
int __stdcall TPlateDetails::Search_GSKT_Yobi(AnsiString PartsCD,
    AnsiString MatCD) {
    int iLop;
    int iYobiNum;
    int iYobiNum2;
    int iYobiNum3;
    int iYobiNum4;
    AnsiString aYobiCD;
    AnsiString aYobiCD2;
    AnsiString aYobiCD3;
    AnsiString aYobiCD4;
    bool bYobiChk;

    iYobiNum = 0;
    iYobiNum2 = 0;
    iYobiNum3 = 0;
    iYobiNum4 = 0;
    bYobiChk = false;
    for (iLop = 0; iLop < P_YobiMemoCnt && iLop < 50; iLop++) {
        if ((P_YobiCD[iLop].TrimRight() == PartsCD.TrimRight()) && // 予備品ｺｰﾄﾞ一致
            (P_YobiMAT[iLop].TrimRight() == MatCD.TrimRight())) { // 予備品材質ｺｰﾄﾞ一致
            bYobiChk = true;
        }
    }
    if (bYobiChk == false) { // 予備数取得済みではない
        // 2004.02.02 予備数の取得訂正
        // switch( PartsCD.ToIntDef(0)) {
        // case 6:                     // Ｄプレートガスケット
        // aYobiCD = "GSKT02";
        // break;
        // case 7:                     // ＤプレートガスケットＡ
        // aYobiCD = "GSKT08";
        // break;
        // case 8:                     // ＤプレートガスケットＢ
        // aYobiCD = "GSKT09";
        // break;
        // //2004.01.20 予備数の取得訂正
        // //case 55:                    // EプレートガスケットＡ
        // //case 56:                    // ＥプレートガスケットＢ
        // //case 57:                    // Ｅプレートガスケット
        // case 9:                     // ＤプレートガスケットＤ−Ａ
        // aYobiCD = "GSKT08";
        // break;
        // case 10:                    // ＤプレートガスケットＤ−Ｂ
        // aYobiCD = "GSKT09";
        // break;
        // case 55:                    // ＥプレートガスケットＡ
        // aYobiCD = "GSKT03";
        // break;
        // case 56:                    // ＥプレートガスケットＢ
        // aYobiCD = "GSKT03";
        // break;
        // case 57:                    // Ｅプレートガスケット
        // aYobiCD = "GSKT03";
        // break;
        // case 58:                    // ＥプレートガスケットＤ−Ａ
        // aYobiCD = "GSKT03";
        // break;
        // case 59:                    // ＥプレートガスケットＤ−Ｂ
        // aYobiCD = "GSKT03";
        // break;
        // //***************************
        // case 60:                    // ＥプレートガスケットＥ
        // aYobiCD = "GSKT03";
        // break;
        // case 13:                    // Ｄディスタンスピース
        // //2004.01.20 予備数の取得訂正
        // aYobiCD = "GSKT06";
        // break;
        // //***************************
        // case 78:                    // ディスタンスピース
        // aYobiCD = "GSKT06";
        // break;
        // case 15:                    // ＤプレートガスケットＢ−４
        // aYobiCD = "GSKT13";
        // break;
        // case 16:                    // ＤプレートガスケットＡ−２
        // aYobiCD = "GSKT12";
        // break;
        // case 79:                    // Ｅノズルガスケット
        // aYobiCD = "GSKT04";
        // break;
        // case 64:                    // Ｅディスタンスピース
        // aYobiCD = "GSKT14";
        // break;
        // default :
        // aYobiCD = "";
        // break;
        // }
        aYobiCD2 = "";
        aYobiCD3 = "";
        aYobiCD4 = "";
        switch (PartsCD.ToIntDef(0)) {
        case 6: // Ｄプレートガスケット
            if (G_KS_Syurui == "ＦＸ" && G_KS_Model == "０３") { // ＤプレートガスケットＬ
                aYobiCD = "GSKT14";
            }
            // 2023.07.12 予備品変更_S
            else if (G_KS_Syurui == "ＧＸ" && G_KS_Model == "５０") { // ＤＣプレートガスケット
                aYobiCD = "GSKTA3";
            }
            // 2023.07.12 予備品変更_E
            else { // Ｄプレートガスケット
                aYobiCD = "GSKT27";
            }
            aYobiCD2 = "GSKT02"; // Ｄプレートガスケット（旧）
            aYobiCD3 = "GSKT08"; // ＤプレートガスケットＡ
            aYobiCD4 = "GSKT09"; // ＤプレートガスケットＢ
            break;
        case 7: // ＤプレートガスケットＡ
            if (G_KS_Syurui == "ＦＸ" && G_KS_Model == "０３") { // ＤプレートガスケットＲＡ
                aYobiCD = "GSKT15";
            }
            else { // ＤプレートガスケットＡ
                aYobiCD = "GSKT08";
            }
            break;
        case 8: // ＤプレートガスケットＢ
            if (G_KS_Syurui == "ＦＸ" && G_KS_Model == "０３") { // ＤプレートガスケットＲＢ
                aYobiCD = "GSKT16";
            }
            else { // ＤプレートガスケットＢ
                aYobiCD = "GSKT09";
            }
            break;
        case 9: // ＤプレートガスケットＤ−Ａ
            // 2023.07.27 予備品変更_S
            //// 2017.08.17 予備品変更_S
            ////aYobiCD = "GSKT08";
            //aYobiCD = "GSKT47";
            //// 2017.08.17 予備品変更_E
            if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "５０Ｓ") { // ＤＡガスケット
                aYobiCD = "GSKT08";
            }
            else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") { // ＤＡガスケット
                aYobiCD = "GSKT08";
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "１０") { // ＤＡガスケット
                aYobiCD = "GSKT08";
            }
            else { // ＤプレートガスケットＤ−Ａ
                aYobiCD = "GSKT47";
            }
            // 2023.07.27 予備品変更_E
            break;
        case 10: // ＤプレートガスケットＤ−Ｂ
            // 2023.07.27 予備品変更_S
            //// 2017.08.17 予備品変更_S
            ////aYobiCD = "GSKT09";
            //aYobiCD = "GSKT48";
            //// 2017.08.17 予備品変更_E
            if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "５０Ｓ") { // ＤＢガスケット
                aYobiCD = "GSKT09";
            }
            else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ") { // ＤＢガスケット
                aYobiCD = "GSKT09";
            }
            else if (G_KS_Syurui == "ＳＸ" && G_KS_Model == "１０") { // ＤＢガスケット
                aYobiCD = "GSKT09";
            }
            else { // ＤプレートガスケットＤ−Ｂ
                aYobiCD = "GSKT48";
            }
            // 2023.07.27 予備品変更_E
            break;
        case 13: // Ｄディスタンスピース
            aYobiCD = "GSKT06";
            break;
        // 2023.07.12 予備品変更_S
        case 14: // プレートガスケットＢ１
            aYobiCD = "GSKT11";
            break;
        case 15: // プレートガスケットＢ４
            aYobiCD = "GSKT13";
            break;
        case 16: // プレートガスケットＡ２
            aYobiCD = "GSKT12";
            break;
        case 17: // プレートガスケットＡ３
            aYobiCD = "GSKT10";
            break;
        // 2023.07.12 予備品変更_E
        case 32: // プレートガスケットＢ−１
            aYobiCD = "GSKT11";
            break;
        case 33: // プレートガスケットＢ−４
            aYobiCD = "GSKT13";
            break;
        case 34: // プレートガスケットＡ−２
            aYobiCD = "GSKT12";
            break;
        case 35: // プレートガスケットＡ−３
            aYobiCD = "GSKT10";
            break;
        case 38: // 真空時目玉盲部補強
            aYobiCD = "GSKT33";
            break;
        case 55: // ＥプレートガスケットＡ
            // 2017.08.17 予備品変更_S
            //aYobiCD = "GSKT29";
            aYobiCD = "GSKT42";
            // 2017.08.17 予備品変更_E
            break;
        case 56: // ＥプレートガスケットＢ
            // 2017.08.17 予備品変更_S
            //aYobiCD = "GSKT30";
            aYobiCD = "GSKT43";
            // 2017.08.17 予備品変更_E
            break;
        case 57: // Ｅプレートガスケット
            aYobiCD = "GSKT28";
            aYobiCD2 = "GSKT03"; // Ｅプレートガスケット（旧）
            // 2017.08.17 予備品変更_S
            //aYobiCD3 = "GSKT29"; // ＥプレートガスケットＡ
            aYobiCD3 = "GSKT42"; // ＥプレートガスケットＡ
            //aYobiCD4 = "GSKT30"; // ＥプレートガスケットＢ
            aYobiCD4 = "GSKT43"; // ＥプレートガスケットＢ
            // 2017.08.17 予備品変更_E
            break;
        // 2023.07.12 予備品変更_S
        //// 2017.08.17 予備品変更_S
        ////case 58: // ＥプレートガスケットＤ−Ａ
        ////    aYobiCD = "GSKT29";
        ////    break;
        //case 58: // Ｅプレートガスケット上Ｄ−Ａ
        //    aYobiCD = "GSKT47";
        //    break;
        ////case 59: // ＥプレートガスケットＤ−Ｂ
        ////    aYobiCD = "GSKT30";
        ////    break;
        //case 59: // Ｅプレートガスケット下Ｄ−Ｂ
        //    aYobiCD = "GSKT48";
        //    break;
        //// 2017.08.17 予備品変更_E
        case 58: // Ｅプレートガスケット上Ｄ−Ａ
            aYobiCD = "GSKT49";
            break;
        case 59: // Ｅプレートガスケット下Ｄ−Ｂ
            aYobiCD = "GSKT50";
            break;
        // 2023.07.12 予備品変更_E
        case 60: // ＥプレートガスケットＥ
            // 2017.08.17 予備品変更_S
            //aYobiCD = "GSKT28";
            aYobiCD = "GSKT51";
            aYobiCD2 = "GSKT03"; // Ｅプレートガスケット（旧）
            //aYobiCD3 = "GSKT29"; // ＥプレートガスケットＡ
             aYobiCD3 = "GSKT42"; // ＥプレートガスケットＡ
            //aYobiCD4 = "GSKT30"; // ＥプレートガスケットＢ
            aYobiCD = "GSKT43";
            // 2017.08.17 予備品変更_E
            break;
        case 62: // Ｅプレート目玉補強パッキングＡ
            if (G_KS_Syurui == "ＥＸ" && G_KS_Model == "１５") { // プレート付属
                aYobiCD = "";
            }
            else { // Ｅプレート目玉補強パッキングＡ
                aYobiCD = "GSKT31";
            }
            break;
        case 63: // Ｅプレート目玉補強パッキングＢ
            if (G_KS_Syurui == "ＥＸ" && G_KS_Model == "１５") { // プレート付属
                aYobiCD = "";
            }
            else { // Ｅプレート目玉補強パッキングＢ
                aYobiCD = "GSKT32";
            }
            break;
        case 64: // Ｅディスタンスピース
            aYobiCD = "GSKT19";
            break;
        case 65: // Ｅプレート補強ガスケット
            aYobiCD = "GSKT26";
            aYobiCD3 = "GSKT31"; // Ｅプレート目玉補強パッキングＡ
            aYobiCD4 = "GSKT32"; // Ｅプレート目玉補強パッキングＢ
            break;
        // 2023.07.12 予備品変更_S
        case 69: // プレートガスケットＢ１
            aYobiCD = "GSKT11";
            break;
        case 70: // プレートガスケットＢ４
            aYobiCD = "GSKT13";
            break;
        case 71: // プレートガスケットＡ２
            aYobiCD = "GSKT12";
            break;
        case 72: // プレートガスケットＡ３
            aYobiCD = "GSKT10";
            break;
        // 2023.07.12 予備品変更_E
        case 78: // ディスタンスピース
            aYobiCD = "GSKT06";
            break;
        case 79: // Ｅノズルガスケット
            // 2023.07.12 予備品変更_S
            //aYobiCD = "GSKT04";
            if (G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０") { // Ｅ側通路パッキング
                aYobiCD = "GSKT59";
            }
            else { // Ｅノズルガスケット
                aYobiCD = "GSKT04";
            }
            // 2023.07.12 予備品変更_S
            break;
        case 80: // (Ｄ)三角堰パッチＡ
            aYobiCD = "GSKT34";
            break;
        // 2017.08.17 予備品変更_S
        //case 81: // (Ｄ)三角堰パッチＢ
        //    aYobiCD = "GSKT35";
        //    break;
        //case 82: // (Ｄ)三角堰パッチＣ
        //    aYobiCD = "GSKT36";
        //    break;
        case 83: // (Ｅ)三角堰パッチＡ
        //    aYobiCD = "GSKT37";
            aYobiCD = "GSKT34";
            break;
        //case 84: // (Ｅ)三角堰パッチＢ
        //    aYobiCD = "GSKT38";
        //    break;
        //case 85: // (Ｅ)三角堰パッチＣ
        //    aYobiCD = "GSKT39";
        //    break;
        // 2017.08.17 予備品変更_E
        case 87: // (Ｄ)補強パッチ
            aYobiCD = "GSKT40";
            break;
        case 88: // (Ｅ)補強パッチ
            // 2017.08.17 予備品変更_S
            //aYobiCD = "GSKT41";
            aYobiCD = "GSKT40";
            // 2017.08.17 予備品変更_E
            break;
        // 2017.08.17 予備品変更_S
        case 89: // (Ｄ)ＤプレートガスケットＤＡ−Ｒ
            aYobiCD = "GSKT60";
            break;
        case 90: // (Ｄ)ＤプレートガスケットＤＡ−Ｌ
            aYobiCD = "GSKT61";
            break;
        case 91: // (Ｄ)ＤプレートガスケットＤＢ−Ｒ
            aYobiCD = "GSKT62";
            break;
        case 92: // (Ｄ)ＤプレートガスケットＤＢ−Ｌ
            aYobiCD = "GSKT63";
            break;
        case 93: // (Ｅ)ＤプレートガスケットＤＡ−Ｒ
            aYobiCD = "GSKT60";
            break;
        case 94: // (Ｅ)ＤプレートガスケットＤＡ−Ｌ
            aYobiCD = "GSKT61";
            break;
        case 95: // (Ｅ)ＤプレートガスケットＤＢ−Ｒ
            aYobiCD = "GSKT62";
            break;
        case 96: // (Ｅ)ＤプレートガスケットＤＢ−Ｌ
            aYobiCD = "GSKT63";
            break;
        // 2023.07.12 予備品変更_S
        case 97: // リングガスケット
            aYobiCD = "GSKT44";
            break;
        case 98: // リングガスケットＬ
            aYobiCD = "GSKT45";
            break;
        case 99: // リングガスケットＲ
            aYobiCD = "GSKT46";
            break;
        // 2023.07.12 予備品変更_E
        case 100: // Ｅ側通路パッキング
            aYobiCD = "GSKT59";
            break;
        case 101: // Ｄプレートガスケット上Ｄ−Ａ
            // 2023.07.12 予備品変更_S
            //aYobiCD = "GSKT49";
            if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "００") { // Ｄプレートガスケット上Ｄ−Ａ
                aYobiCD = "GSKT47";
            }
            else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０") { // Ｄプレートガスケット上Ｄ−Ａ
                aYobiCD = "GSKT47";
            }
            else { // Ｄ―Ａガスケット
                aYobiCD = "GSKT08";
            }
            // 2023.07.12 予備品変更_S
            break;
        case 102: // Ｄプレートガスケット下Ｄ−Ｂ
            // 2023.07.12 予備品変更_S
            //aYobiCD = "GSKT50";
            if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "００") { // Ｄプレートガスケット上Ｄ−Ｂ
                aYobiCD = "GSKT48";
            }
            else if (G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０") { // Ｄプレートガスケット上Ｄ−Ｂ
                aYobiCD = "GSKT48";
            }
            else { // Ｄ―Ｂガスケット
                aYobiCD = "GSKT09";
            }
            // 2023.07.12 予備品変更_S
            break;
        case 103: // (Ｄ)ディスタンスピースＤ
            aYobiCD = "GSKT92";
            break;
        case 104: // (Ｅ)ディスタンスピースＤ
            aYobiCD = "GSKT93";
            break;
        case 105: // Ｄプレートガスケット１
            aYobiCD = "GSKT35";
            break;
        case 106: // Ｄプレートガスケット２
            aYobiCD = "GSKT36";
            break;
        case 110: // ＥＢ１プレートガスケット
            aYobiCD = "GSKT92";
            break;
        case 111: // ＥＢ２プレートガスケット
            aYobiCD = "GSKT93";
            break;
        // 2017.08.17 予備品変更_E
        // 2023.07.12 予備品変更_S
        case 112: // ＢディスタンスピースＡ
            aYobiCD = "GSKT31";
            break;
        case 113: // ＢディスタンスピースＢ
            aYobiCD = "GSKT32";
            break;
        case 114: // Ｓディスタンスピース
            if (G_KS_Syurui == "ＷＨ" && G_KS_Model == "３０") { // Ｓディスタンス（Ｇ−５−２）
                aYobiCD = "GSKT37";
            }
            else { // Ｓディスタンスピース
                aYobiCD = "GSKT06";
            }
            break;
        case 115: // Ｅ側Ｓディスタンスピース
            aYobiCD = "GSKT19";
            break;
        case 116: // Ｔディスタンスピース
            aYobiCD = "GSKT40";
            break;
        case 117: // Ｔディスタンスピース
            aYobiCD = "GSKT40";
            break;
        // 2023.07.12 予備品変更_E
        default:
            aYobiCD = "";
            break;
        }
        // ***************************
        iYobiNum = Syukei_Buhin_Yobisu(aYobiCD, MatCD, m_pcHeadsData);
        // 2004.02.02 予備数の取得訂正
        if ((PartsCD.ToIntDef(0) == 6) || (PartsCD.ToIntDef(0) == 57) ||
            (PartsCD.ToIntDef(0) == 60) || (PartsCD.ToIntDef(0) == 65)) {
            if (iYobiNum == 0) {
                iYobiNum2 = Syukei_Buhin_Yobisu(aYobiCD2, MatCD, m_pcHeadsData);
                iYobiNum3 = Syukei_Buhin_Yobisu(aYobiCD3, MatCD, m_pcHeadsData);
                iYobiNum4 = Syukei_Buhin_Yobisu(aYobiCD4, MatCD, m_pcHeadsData);
                if (iYobiNum2 != 0) {
                    iYobiNum = iYobiNum2;
                }
                else {
                    iYobiNum = iYobiNum3 + iYobiNum4;
                }
            }
        }
        // ***************************
        P_YobiCD[P_YobiMemoCnt] = PartsCD;
        P_YobiMAT[P_YobiMemoCnt] = MatCD;
        P_YobiMemoCnt++;
    }
    return iYobiNum;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ガスケット予備品取得処理
// 概  要      ：
// 引  数      ： ガスケット部品コード　ガスケット材質コード
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
// ---------------------------------------------------------------------------
int __fastcall TPlateDetails::Get_Gskt_YobiSu(int i_TmpCd, AnsiString MatCD) {
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
    s_SQL = s_SQL + "SELECT * FROM HD_GSKT2YOBI";
    s_SQL = s_SQL + "  WHERE GSKTPCD = " + i_TmpCd;

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

    i_YobiSu = Syukei_Buhin_Yobisu(s_YobiName, MatCD, m_pcHeadsData);
    return i_YobiSu;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ＲＸ−００専用Ｄプレートガスケット部品　ﾃﾞｰﾀ作成
// 概  要      ： Ｄプレートのガスケットは一体物を切断して使用するので
// プレート１枚当りの数量は１枚以上にならない
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2006.01.10 サブルーチン追加
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_RX00D_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    AnsiString s_CLabel;
    AnsiString s_DLabel;

    ret = Search_HD_D_GASKET(G_D_GsktType, G_GsktType, G_FrmDiv);

    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();

    // 2013.04.03 材質比較追加
    AnsiString s_A_GMat;
    AnsiString s_B_GMat;
    AnsiString s_C_GMat;
    AnsiString s_D_GMat;
    // ***********************

    DE_Kubun = "D";
    // パターン分岐により必要なガスケットを選択
    // Aスタート
    if (G_D_GsktABKubun == "A") {
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s1_Code,
            DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code,
            DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_B_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(1, 1) != "0") {
            // （Ｄプレートガスケット）
            s_A_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_A_GMat = "2019";
        }
        // ***********************
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s2_Code,
            DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code,
            DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_A_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(2, 1) != "0") {
            // （Ｄプレートガスケット）
            s_B_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_B_GMat = "2019";
        }
        // 型式とコードの比較
        // if( s_BLabel == s_ALabel ){
        // //S2ｶﾞｽｹｯﾄとS1ｶﾞｽｹｯﾄが同一の場合の数量で出力（Ｅプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        // }
        if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // S2ｶﾞｽｹｯﾄとS1ｶﾞｽｹｯﾄが同一の場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // ***********************
        // S3:B側の処理
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1), DP_B0s3_Code,
            DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code,
            DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_B_GSKT_Mat);
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(3, 1) != "0") {
            // （Ｄプレートガスケット）
            s_C_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_C_GMat = "2019";
        }
        // 型式とコードの比較
        // 2013.04.03 材質比較追加
        // if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        // //S3ｶﾞｽｹｯﾄとS1ｶﾞｽｹｯﾄもしくはS3ｶﾞｽｹｯﾄとS2ｶﾞｽｹｯﾄが同一の場合の数量で出力
        // if( G_D_GsktAna.SubString(3,1) != "0" ){
        // //（Ｄプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        // }else{
        // //（Ｅプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        // }
        // }
        if (s_CLabel == s_ALabel ||
            (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // S3ｶﾞｽｹｯﾄとS1ｶﾞｽｹｯﾄもしくはS3ｶﾞｽｹｯﾄとS2ｶﾞｽｹｯﾄが同一の場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // ***********************
        // S4:A側の処理
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1), DP_B0s4_Code,
            DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code,
            DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_A_GSKT_Mat);
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(4, 1) != "0") {
            // （Ｄプレートガスケット）
            s_D_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_D_GMat = "2019";
        }
        // 型式とコードの比較
        // if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        // //S4ｶﾞｽｹｯﾄとS1ｶﾞｽｹｯﾄもしくはS4ｶﾞｽｹｯﾄとS2ｶﾞｽｹｯﾄもしくはS4ｶﾞｽｹｯﾄとS3ｶﾞｽｹｯﾄが同一の場合の数量で出力
        // if( G_D_GsktAna.SubString(4,1) != "0" ){
        // //（Ｄプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        // }else{
        // //（Ｅプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        // }
        // }
        if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
            s_DLabel == s_BLabel ||
            (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // S4ｶﾞｽｹｯﾄとS1ｶﾞｽｹｯﾄもしくはS4ｶﾞｽｹｯﾄとS2ｶﾞｽｹｯﾄもしくはS4ｶﾞｽｹｯﾄとS3ｶﾞｽｹｯﾄが同一の場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // ***********************
    }
    // Bスタート
    else {
        // S4:A側の処理
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(4, 1), DP_B0s1_Code,
            DP_B0s1_Label, DP_B0s1_Name, DP_B0s1_Num, DP_B1s1_Code,
            DP_B1s1_Label, DP_B1s1_Name, DP_B1s1_Num, s_A_GSKT_Mat);
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(4, 1) != "0") {
            // （Ｄプレートガスケット）
            s_A_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_A_GMat = "2019";
        }
        // ***********************
        // S3:B側の処理
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(3, 1), DP_B0s2_Code,
            DP_B0s2_Label, DP_B0s2_Name, DP_B0s2_Num, DP_B1s2_Code,
            DP_B1s2_Label, DP_B1s2_Name, DP_B1s2_Num, s_B_GSKT_Mat);
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(3, 1) != "0") {
            // （Ｄプレートガスケット）
            s_B_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_B_GMat = "2019";
        }
        // 型式とコードの比較
        // if( s_BLabel == s_ALabel ){
        // S3ｶﾞｽｹｯﾄとS4ｶﾞｽｹｯﾄが同一の場合の数量で出力（Ｅプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        // }
        if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // S3ｶﾞｽｹｯﾄとS4ｶﾞｽｹｯﾄが同一の場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // ***********************
        // S2:A側の処理
        P_SideDiv = "A";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(2, 1), DP_B0s3_Code,
            DP_B0s3_Label, DP_B0s3_Name, DP_B0s3_Num, DP_B1s3_Code,
            DP_B1s3_Label, DP_B1s3_Name, DP_B1s3_Num, s_A_GSKT_Mat);
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(2, 1) != "0") {
            // （Ｄプレートガスケット）
            s_C_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_C_GMat = "2019";
        }
        // 型式とコードの比較
        // if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        // //S2ｶﾞｽｹｯﾄとS4ｶﾞｽｹｯﾄもしくはS2ｶﾞｽｹｯﾄとS3ｶﾞｽｹｯﾄが同一の場合の数量で出力
        // if( G_D_GsktAna.SubString(2,1) != "0" ){
        // //（Ｄプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        // }else{
        // //（Ｅプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        // }
        // }
        if (s_CLabel == s_ALabel ||
            (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // S2ｶﾞｽｹｯﾄとS4ｶﾞｽｹｯﾄもしくはS2ｶﾞｽｹｯﾄとS3ｶﾞｽｹｯﾄが同一の場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // ***********************
        // S1:B側の処理
        P_SideDiv = "B";
        ret = Write_D_Gskt_Data_Sub(G_D_GsktAna.SubString(1, 1), DP_B0s4_Code,
            DP_B0s4_Label, DP_B0s4_Name, DP_B0s4_Num, DP_B1s4_Code,
            DP_B1s4_Label, DP_B1s4_Name, DP_B1s4_Num, s_B_GSKT_Mat);
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        if (G_D_GsktAna.SubString(1, 1) != "0") {
            // （Ｄプレートガスケット）
            s_D_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_D_GMat = "2019";
        }
        // 型式とコードの比較
        // if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        // //S1ｶﾞｽｹｯﾄとS4ｶﾞｽｹｯﾄもしくはS1ｶﾞｽｹｯﾄとS3ｶﾞｽｹｯﾄもしくはS1ｶﾞｽｹｯﾄとS2ｶﾞｽｹｯﾄが同一の場合の数量で出力
        // if( G_D_GsktAna.SubString(1,1) != "0" ){
        // //（Ｄプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        // }else{
        // //（Ｅプレートガスケット）
        // P_Suryo = -1;
        // P_Yobisu = -1 * P_PltGsktYobiD;
        // Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        // Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        // }
        // }
        if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
            s_DLabel == s_BLabel ||
            (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // S1ｶﾞｽｹｯﾄとS4ｶﾞｽｹｯﾄもしくはS1ｶﾞｽｹｯﾄとS3ｶﾞｽｹｯﾄもしくはS1ｶﾞｽｹｯﾄとS2ｶﾞｽｹｯﾄが同一の場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiD;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // ***********************
    }
    // ディスタンスピース出力
    Write_D_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ＲＸ−００専用Ｅプレートガスケット部品　ﾃﾞｰﾀ作成
// 概  要      ： Ｅプレートのガスケットは一体物を切断して使用するので
// プレート１枚当りの数量は１枚以上にならない
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2006.01.10 サブルーチン追加
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_RX00E_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    AnsiString s_CLabel;
    AnsiString s_DLabel;

    ret = Search_HD_E_GASKET(G_E_GsktType, G_GsktType, G_FrmDiv);

    // 2013.04.03 材質比較追加
    AnsiString s_B_GSKT_Mat;
    AnsiString s_A_GSKT_Mat;
    s_B_GSKT_Mat = P_HEADS_DATA[835].TrimRight();
    s_A_GSKT_Mat = P_HEADS_DATA[836].TrimRight();
    AnsiString s_A_GMat;
    AnsiString s_B_GMat;
    AnsiString s_C_GMat;
    AnsiString s_D_GMat;
    // ***********************

    DE_Kubun = "E";
    // パターン分岐により必要なガスケットを選択
    // Aスタート
    if (G_E_GsktABKubun == "A") {
        // E1:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[835].TrimRight());
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(1, 1) != "0") {
        if (G_E_GsktAna.SubString(1, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_A_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_A_GMat = "2019";
        }
        // ***********************
        // E2:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[836].TrimRight());
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(2, 1) != "0") {
        if (G_E_GsktAna.SubString(2, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_B_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_B_GMat = "2019";
        }
        // 型式とコードの比較
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //// if( s_BLabel == s_ALabel ){
        //// //E2ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄが同一の場合の数量で出力（Ｅプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        //// }
        //if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
        //    // E2ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄが同一の場合の数量で出力（Ｅプレートガスケット）
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //    Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        //}
        // ***********************
        if (G_E_GsktAna.SubString(1, 1) == "0" &&
            G_E_GsktAna.SubString(2, 1) == "0") {
            // E2ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // E2ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄが同一の場合の数量で出力(Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
        // E3:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[835].TrimRight());
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(3, 1) != "0") {
        if (G_E_GsktAna.SubString(3, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_C_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_C_GMat = "2019";
        }
        // 型式とコードの比較
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //// if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        //// //E3ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄもしくはE3ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //// if( G_E_GsktAna.SubString(3,1) != "0" ){
        //// //（Ｄプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        //// }else{
        //// //（Ｅプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        //// }
        //// }
        //if (s_CLabel == s_ALabel ||
        //    (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
        //    // E3ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄもしくはE3ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //    Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        //}
        // ***********************
        if (G_E_GsktAna.SubString(3, 1) == "0" &&
            G_E_GsktAna.SubString(1, 1) == "0") {
            // E3ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (G_E_GsktAna.SubString(3, 1) == "0" &&
                   G_E_GsktAna.SubString(2, 1) == "0") {
            // E3ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
         } else if (s_CLabel == s_ALabel ||
                   (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // E3ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄもしくはE3ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄが同一の場合の数量で出力
            // (Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
        // E4:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[836].TrimRight());
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(4, 1) != "0") {
        if (G_E_GsktAna.SubString(4, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_D_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_D_GMat = "2019";
        }
        // 型式とコードの比較
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //// if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        //// //E4ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄもしくはE4ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄもしくはE4ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //// if( G_E_GsktAna.SubString(4,1) != "0" ){
        //// //（Ｄプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        //// }else{
        //// //（Ｅプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        //// }
        //// }
        //if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
        //    s_DLabel == s_BLabel ||
        //    (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
        //    // E4ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄもしくはE4ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄもしくはE4ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //    Write_HeartsBuhin_Data(s_D_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(4, 1) == "0" &&
            G_E_GsktAna.SubString(1, 1) == "0") {
            // E4ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (G_E_GsktAna.SubString(4, 1) == "0" &&
                   G_E_GsktAna.SubString(2, 1) == "0") {
            // E4ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (G_E_GsktAna.SubString(4, 1) == "0" &&
                   G_E_GsktAna.SubString(3, 1) == "0") {
            // E4ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
                    s_DLabel == s_BLabel ||
                   (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // E4ｶﾞｽｹｯﾄとE1ｶﾞｽｹｯﾄもしくはE4ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄもしくはE4ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄが同一の場合の数量で出力
            // (Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
    }
    // Bスタート
    else {
        // E4:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[836].TrimRight());
        s_ALabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(4, 1) != "0") {
        if (G_E_GsktAna.SubString(4, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_A_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_A_GMat = "2019";
        }
        // ***********************
        // E3:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[835].TrimRight());
        s_BLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(3, 1) != "0") {
        if (G_E_GsktAna.SubString(3, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_B_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_B_GMat = "2019";
        }
        // 型式とコードの比較
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //// if( s_BLabel == s_ALabel ){
        //// //E3ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄが同一の場合の数量で出力（Ｅプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        //// }
        //if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
        //    // E3ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //    Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(3, 1) == "0" &&
            G_E_GsktAna.SubString(4, 1) == "0") {
            // E3ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (s_BLabel == s_ALabel && s_B_GMat == s_A_GMat) {
            // E3ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄが同一の場合の数量で出力(Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_B_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
        // E2:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[836].TrimRight());
        s_CLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(2, 1) != "0") {
        if (G_E_GsktAna.SubString(2, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_C_GMat = s_A_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_C_GMat = "2019";
        }
        // 型式とコードの比較
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //// if( s_CLabel == s_ALabel || s_CLabel == s_BLabel ){
        //// //E2ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄもしくはE2ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //// if( G_E_GsktAna.SubString(2,1) != "0" ){
        //// //（Ｄプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        //// }else{
        //// //（Ｅプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data("2019");  //HEARTS用ﾃﾞｰﾀCSV作成
        //// }
        //// }
        //if (s_CLabel == s_ALabel ||
        //    (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
        //    // E2ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄもしくはE2ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //    Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(2, 1) == "0" &&
            G_E_GsktAna.SubString(4, 1) == "0") {
            // E2ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (G_E_GsktAna.SubString(2, 1) == "0" &&
                   G_E_GsktAna.SubString(3, 1) == "0") {
            // E2ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (s_CLabel == s_ALabel ||
                  (s_CLabel == s_BLabel && s_C_GMat == s_B_GMat)) {
            // E2ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄもしくはE2ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄが同一の場合の数量で出力
            // (Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
        // E1:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[835].TrimRight());
        s_DLabel = P_PLATECD[1];
        // 2013.04.03 材質比較追加
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //if (G_D_GsktAna.SubString(1, 1) != "0") {
        if (G_E_GsktAna.SubString(1, 1) != "0") {
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
            // （Ｄプレートガスケット）
            s_D_GMat = s_B_GSKT_Mat;
        }
        else {
            // （Ｅプレートガスケット）
            s_D_GMat = "2019";
        }
        // 型式とコードの比較
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_S
        //// if( s_DLabel == s_ALabel || s_DLabel == s_BLabel || s_DLabel == s_CLabel ){
        //// //E1ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄもしくはE1ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄもしくはE1ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //// if( G_E_GsktAna.SubString(1,1) != "0" ){
        //// //（Ｄプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret);                       // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight()); // HEARTS用ﾃﾞｰﾀCSV作成
        //// }else{
        //// //（Ｅプレートガスケット）
        //// P_Suryo = -1;
        //// P_Yobisu = -1 * P_PltGsktYobiE;
        //// Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //// Write_HeartsBuhin_Data("2019");  // HEARTS用ﾃﾞｰﾀCSV作成
        //// }
        //// }
        //if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
        //    s_DLabel == s_BLabel ||
        //    (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
        //    // E1ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄもしくはE1ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄもしくはE1ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄが同一の場合の数量で出力
        //    P_Suryo = -1;
        //    P_Yobisu = -1 * P_PltGsktYobiE;
        //    Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        //    Write_HeartsBuhin_Data(s_D_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        //}
        //// ***********************
        if (G_E_GsktAna.SubString(1, 1) == "0" &&
            G_E_GsktAna.SubString(4, 1) == "0") {
            // E1ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (G_E_GsktAna.SubString(1, 1) == "0" &&
                   G_E_GsktAna.SubString(3, 1) == "0") {
            // E1ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if (G_E_GsktAna.SubString(1, 1) == "0" &&
                   G_E_GsktAna.SubString(2, 1) == "0") {
            // E1ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄがEﾌﾟﾚｰﾂｶﾞｽｹｯﾄの場合の数量で出力
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_C_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        } else if ((s_DLabel == s_ALabel && s_D_GMat == s_A_GMat) ||
                    s_DLabel == s_BLabel ||
                   (s_DLabel == s_CLabel && s_D_GMat == s_C_GMat)) {
            // E1ｶﾞｽｹｯﾄとE4ｶﾞｽｹｯﾄもしくはE1ｶﾞｽｹｯﾄとE3ｶﾞｽｹｯﾄもしくはE1ｶﾞｽｹｯﾄとE2ｶﾞｽｹｯﾄが同一の場合の数量で出力
            // (Dﾌﾟﾚｰﾄｶﾞｽｹｯﾄ)
            P_Suryo = -1;
            P_Yobisu = -1 * P_PltGsktYobiE;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_D_GMat); // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // 2019.08.27 Eｶﾞｽｹｯﾄ数量訂正_E
    }
    // ディスタンスピース出力
    Write_E_Gskt_Data_Distance();

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ＷＸ専用中間プレートガスケット部品　ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2006.01.10 サブルーチン追加
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_WXM_Gskt_Data(void) {

    AnsiString s_Text, s_Model, s_Key, s_Plate, s_Ring, s_PlateC, s_RingC;
    int i_Mojisu;
    int ret;
    int iSuryo;
    int iMod;

    // 2009.02.16 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_Kubunに"I"を記述するｻﾌﾞﾙｰﾁﾝに変える
    DE_Kubun = "I";
    // **********************************************************************

    // **********************************
    // ***  ﾌﾟﾚｰﾄ部品構成ﾏｽﾀ　検索
    // **********************************
    s_Model = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
    // 2009.06.19 UX-100R追加
    // EXの場合
    // s_Model後端部に"C","P","R","A","B"がある場合
    // s_KeyのG_K_Model1の後ろにその文字を追加する
    //
    // EX以外の場合
    // s_Model後端部に"C","P","A","B"がある場合
    // s_KeyのG_K_Model1の後ろにその文字を追加する
    // 但しs_Modelの最後の文字が"D"の場合
    // s_KeyのG_K_Model1の"D"の前の位置に"C","P","A","B"をを追加する
    ////2006.02.06 SX-90M,L追加
    ////P_HEADS_DATA[35]後端部に"C","P","R","A","B"がある場合
    ////s_KeyのG_K_Model1の後ろにその文字を追加する
    ////2008.07.24 CX-10D追加
    ////但しP_HEADS_DATA[35]の最後の文字が"D"の場合
    ////s_KeyのG_K_Model1の"D"の前の位置に"C","P","R","A","B"をを追加する
    ////*********************
    //
    ////2008.07.24 CX-10D追加
    ////s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    ////***********************
    // if ( s_Model.SubString(s_Model.Length(),1) == "D" ) {
    // s_Text = G_K_Model1.SubString(1,G_K_Model1.Length()-1) + s_Text + "D";
    // s_Key = G_K_Syurui + "-" + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    ////*********************
    // **********************

    //
    // i_Mojisu = s_Model.Length();
    // s_Text = P_HEADS_DATA[35].TrimRight().SubString(i_Mojisu,1);
    //
    // if (s_Text.ToIntDef(999) != 999){
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + "-" + P_HEADS_DATA[1267].TrimRight();
    // } else {
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // }
    s_Text = "";
    if (s_Model.Pos("C") > 0) {
        s_Text = s_Text + "C";
    }
    if (s_Model.Pos("P") > 0) {
        s_Text = s_Text + "P";
    }
    // 2009.06.19 UX-100R追加
    // if ( s_Model.Pos("R") > 0 ) { s_Text = s_Text + "R"; }
    if (G_K_Syurui.SubString(1, 2) == "EX" && s_Model.Pos("R") > 0) {
        s_Text = s_Text + "R";
    }
    // **********************
    if (s_Model.Pos("A") > 0) {
        s_Text = s_Text + "A";
    }
    if (s_Model.Pos("B") > 0) {
        s_Text = s_Text + "B";
    }

    // 2008.07.24 CX-10D追加
    // s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" + P_HEADS_DATA[1267].TrimRight();
    // ***********************
    if (s_Model.SubString(s_Model.Length(), 1) == "D") {
        s_Text = G_K_Model1.SubString(1, G_K_Model1.Length() - 1) +
            s_Text + "D";
        s_Key = G_K_Syurui + "-" + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    else {
        s_Key = G_K_Syurui + "-" + G_K_Model1 + s_Text + "-" +
            P_HEADS_DATA[1267].TrimRight();
    }
    // *********************

    // ｶﾞｽｹｯﾄの位置
    if (P_HEADS_DATA[243].ToIntDef(0) == 0) {
        if (P_HEADS_DATA[137].ToIntDef(0) == 0) {
            s_Plate = "A";
            s_Ring = "B";
            s_PlateC = P_HEADS_DATA[836].TrimRight();
            s_RingC = P_HEADS_DATA[835].TrimRight();
        }
        else {
            s_Plate = "B";
            s_Ring = "A";
            s_PlateC = P_HEADS_DATA[835].TrimRight();
            s_RingC = P_HEADS_DATA[836].TrimRight();
        }
    }
    else {
        if (P_HEADS_DATA[137].ToIntDef(0) == 0) {
            s_Plate = "B";
            s_Ring = "A";
            s_PlateC = P_HEADS_DATA[835].TrimRight();
            s_RingC = P_HEADS_DATA[836].TrimRight();
        }
        else {
            s_Plate = "A";
            s_Ring = "B";
            s_PlateC = P_HEADS_DATA[836].TrimRight();
            s_RingC = P_HEADS_DATA[835].TrimRight();
        }
    }

    // 溶接側のｶﾞｽｹｯﾄ(ﾘﾝｸﾞ)
    // 2006.06.30 数量変更、TCG追加
    ////マスタ検索関数
    // ret = Search_HD_PLATE_3(P_SoSyoAscii , "I", "97", s_RingC);
    //
    ////数量補正
    // if ( ret == true ) {
    // P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
    // iSuryo = int(P_Suryo);
    // iMod = iSuryo - int(iSuryo / 2) * 2;
    // if(iMod == 0){
    // //偶数 数量＝（数量−２）／２
    // P_Suryo = (double)(P_Suryo - 2) * 0.5;
    // } else {
    // //奇数 数量＝（数量−１）／２
    // P_Suryo = (double)(P_Suryo - 1) * 0.5;
    // }
    // P_PLATECD[1] = "IRING-";
    // if ( G_KS_Model == "１１" || G_KS_Model == "１３") {
    // P_PLATECD[3] = "リングガスケット";
    // } else {
    // P_PLATECD[3] = "プレートガスケットＢ";
    // }
    // //出力する。
    // P_Yobisu = Syukei_Buhin_Yobisu("GSKT44",s_RingC);
    // if (s_Ring == "A") {
    // P_Yobisu = P_Yobisu + P_PltGsktYobiA;
    // } else {
    // P_Yobisu = P_Yobisu + P_PltGsktYobiB;
    // }
    // P_Unit = "式";
    // P_SideDiv = s_Ring;
    // Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
    // Write_HeartsBuhin_Data(s_RingC); // HEARTS用ﾃﾞｰﾀCSV作成
    // }

    // 2009.12.01 WX-11 ﾘﾝｸﾞｶﾞｽｹｯﾄL,ﾘﾝｸﾞｶﾞｽｹｯﾄR再追加
    ////2006.12.04 WX-11 ﾘﾝｸﾞｶﾞｽｹｯﾄL,ﾘﾝｸﾞｶﾞｽｹｯﾄR削除
    ////if ( ( G_KS_Model == "１１" || G_KS_Model == "１３" ) &&
    // if ( ( G_KS_Model == "１３" ) &&
    // ( s_RingC == "2043" || s_RingC == "2060" || s_RingC == "2061" ||
    // s_RingC == "2062" || s_RingC == "2063" || s_RingC == "2064" ||
    // s_RingC == "2065" || s_RingC == "2071" ) ) {
    ////********************************************
    if (((G_KS_Model == "１３") && (s_RingC == "2043" || s_RingC == "2060" ||
        s_RingC == "2061" || s_RingC == "2062" || s_RingC == "2063" ||
        s_RingC == "2064" || s_RingC == "2065" || s_RingC == "2071")) ||
        (G_KS_Model == "１１")) {
        // **********************************************
        // リングガスケットＬ
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "98", s_RingC);

        // 数量補正
        if (ret == true) {
            P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
            iSuryo = int(P_Suryo);
            iMod = iSuryo -int(iSuryo / 2) * 2;
            if (iMod == 0) {
                // 偶数 数量＝（数量−２）／２
                P_Suryo = (double)(P_Suryo - 2) * 0.5;
            }
            else {
                // 奇数 数量＝（数量−１）／２
                P_Suryo = (double)(P_Suryo - 1) * 0.5;
            }
            P_PLATECD[1] = "IRINGL";
            P_PLATECD[3] = "リングガスケットＬ";
            // 出力する。
            P_Yobisu = Syukei_Buhin_Yobisu("GSKT45", s_RingC, m_pcHeadsData);
            if (s_Ring == "A") {
                P_Yobisu = P_Yobisu + P_PltGsktYobiB;
            }
            else {
                P_Yobisu = P_Yobisu + P_PltGsktYobiA;
            }
            P_Unit = "";
            P_SideDiv = s_Ring;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_RingC); // HEARTS用ﾃﾞｰﾀCSV作成
        }

        // リングガスケットＲ
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "99", s_RingC);

        // 数量補正
        if (ret == true) {
            P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
            iSuryo = int(P_Suryo);
            iMod = iSuryo -int(iSuryo / 2) * 2;
            if (iMod == 0) {
                // 偶数 数量＝（数量−２）／２
                P_Suryo = (double)(P_Suryo - 2) * 0.5;
            }
            else {
                // 奇数 数量＝（数量−１）／２
                P_Suryo = (double)(P_Suryo - 1) * 0.5;
            }
            P_PLATECD[1] = "IRINGR";
            P_PLATECD[3] = "リングガスケットＲ";
            // 出力する。
            P_Yobisu = Syukei_Buhin_Yobisu("GSKT46", s_RingC, m_pcHeadsData);
            if (s_Ring == "A") {
                P_Yobisu = P_Yobisu + P_PltGsktYobiB;
            }
            else {
                P_Yobisu = P_Yobisu + P_PltGsktYobiA;
            }
            P_Unit = "";
            P_SideDiv = s_Ring;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_RingC); // HEARTS用ﾃﾞｰﾀCSV作成
        }

    }
    else {

        // リングガスケット（プレートガスケットＢ）
        // マスタ検索関数
        ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "97", s_RingC);

        // 数量補正
        if (ret == true) {
            P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
            iSuryo = int(P_Suryo);
            iMod = iSuryo -int(iSuryo / 2) * 2;
            if (iMod == 0) {
                // 偶数 数量＝（数量−２）
                P_Suryo = (double)(P_Suryo - 2);
            }
            else {
                // 奇数 数量＝（数量−１）
                P_Suryo = (double)(P_Suryo - 1);
            }
            P_PLATECD[1] = "IRING-";
            if (G_KS_Model == "１１" || G_KS_Model == "１３") {
                P_PLATECD[3] = "リングガスケット";
            }
            // 2021.09.22 WH-30追加_S 　　
            else if (G_KS_Model == "３０") {
                P_PLATECD[3] = "リングガスケット";
            }
            // 2021.09.22 WH-30追加_E
            else {
                P_PLATECD[3] = "プレートガスケットＢ";
            }
            // 出力する。
            P_Yobisu = Syukei_Buhin_Yobisu("GSKT44", s_RingC, m_pcHeadsData);
            if (s_Ring == "A") {
                P_Yobisu = P_Yobisu + P_PltGsktYobiB * 2;
            }
            else {
                P_Yobisu = P_Yobisu + P_PltGsktYobiA * 2;
            }
            P_Unit = "";
            P_SideDiv = s_Ring;
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_HeartsBuhin_Data(s_RingC); // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }
    // ****************************

    // 溶接側のｶﾞｽｹｯﾄ(ﾌﾟﾚｰﾄ)
    // マスタ検索関数
    ret = Search_HD_PLATE_3(P_SoSyoAscii, "I", "30", s_PlateC);

    // 数量補正
    if (ret == true) {
        P_Suryo = P_HEADS_DATA[38].ToIntDef(0);
        iSuryo = int(P_Suryo);
        iMod = iSuryo -int(iSuryo / 2) * 2;
        if (iMod == 0) {
            // 偶数 数量＝（数量−２）／２
            P_Suryo = (double)(P_Suryo - 2) * 0.5;
        }
        else {
            // 奇数 数量＝（数量−１）／２
            P_Suryo = (double)(P_Suryo - 1) * 0.5;
        }
        P_PLATECD[1] = "IPGS--";
        if (G_KS_Model == "１１" || G_KS_Model == "１３") {
            P_PLATECD[3] = "プレートガスケット";
        }
        // 2021.10.13 WH-30追加_S 　　
        else if (G_KS_Model == "３０") {
            P_PLATECD[3] = "プレートガスケット";
        }
        // 2021.10.13 WH-30追加_E
        else {
            P_PLATECD[3] = "プレートガスケットＡ";
        }
        // 出力する。
        P_Yobisu = Syukei_Buhin_Yobisu("GSKT01", s_PlateC, m_pcHeadsData);
        if (s_Plate == "A") {
            P_Yobisu = P_Yobisu + P_PltGsktYobiA;
        }
        else {
            P_Yobisu = P_Yobisu + P_PltGsktYobiB;
        }
        P_Unit = "";
        P_SideDiv = s_Plate;
        Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
        Write_HeartsBuhin_Data(s_PlateC); // HEARTS用ﾃﾞｰﾀCSV作成
    }

    return true;
}

// ---------------------------------------------------------------------------
// 日本語関数名： ＷＸシリーズ専用Ｅカセットプレートガスケット部品　ﾃﾞｰﾀ作成
// 概  要      ： Ｅカセットのガスケットは材質がＡＢ逆になっている
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2006.01.10 サブルーチン追加
// ---------------------------------------------------------------------------
bool __fastcall TPlateDetails::Write_WX_CE_Gskt_Data(void) {
    int ret;

    AnsiString s_ALabel;
    AnsiString s_BLabel;
    ret = Search_HD_E_GASKET(G_E_GsktType, G_GsktType, G_FrmDiv);

    DE_Kubun = "E";
    // パターン分岐により必要なガスケットを選択
    // Aスタート
    if (G_E_GsktABKubun == "A") {
        // E1:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[836].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E2:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[835].TrimRight());
        s_ALabel = P_PLATECD[1];
        // 型式とコードの比較
        // 孔による比較
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // E1､E2孔&A,B同材質の数量で出力
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // P_SideDiv ="";
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS用ﾃﾞｰﾀCSV作成
        }
        if (s_ALabel == s_BLabel && P_HEADS_DATA[835].TrimRight()
            == P_HEADS_DATA[836].TrimRight()) {
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // A,B同材質の数量で出力
            // P_SideDiv ="";
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // E3:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[836].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E4:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[835].TrimRight());
        s_ALabel = P_PLATECD[1];
        // 型式とコードの比較
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // E3､E4孔&A,B同材質の数量で出力
            // P_SideDiv ="";
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "A";
            }
            // Write_PlateBuhin_Data(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[835].TrimRight());
            // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }
    // Bスタート
    else {
        // E4:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(4, 1), EP_B0s1_Code,
            EP_B0s1_Label, EP_B0s1_Name, EP_B0s1_Num, EP_B1s1_Code,
            EP_B1s1_Label, EP_B1s1_Name, EP_B1s1_Num,
            P_HEADS_DATA[835].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E3:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(3, 1), EP_B0s2_Code,
            EP_B0s2_Label, EP_B0s2_Name, EP_B0s2_Num, EP_B1s2_Code,
            EP_B1s2_Label, EP_B1s2_Name, EP_B1s2_Num,
            P_HEADS_DATA[836].TrimRight());
        s_ALabel = P_PLATECD[1];
        // 型式とコードの比較
        // 孔による比較
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(3,
            1) != "0" && G_E_GsktAna.SubString(4,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // E3､E4孔&A,B同材質の数量で出力
            // P_SideDiv ="";
            P_Suryo = EPUH_Meq_Sub;
            P_Yobisu = EPUH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS用ﾃﾞｰﾀCSV作成
        }
        if (s_ALabel == s_BLabel && P_HEADS_DATA[835].TrimRight()
            == P_HEADS_DATA[836].TrimRight()) {
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // A,B同材質の数量で出力
            // P_SideDiv ="";
            P_Suryo = EPU_Meq_Sub;
            P_Yobisu = EPU_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS用ﾃﾞｰﾀCSV作成
        }
        // E2:A側の処理
        P_SideDiv = "A";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(2, 1), EP_B0s3_Code,
            EP_B0s3_Label, EP_B0s3_Name, EP_B0s3_Num, EP_B1s3_Code,
            EP_B1s3_Label, EP_B1s3_Name, EP_B1s3_Num,
            P_HEADS_DATA[835].TrimRight());
        s_BLabel = P_PLATECD[1];
        // E1:B側の処理
        P_SideDiv = "B";
        ret = Write_E_Gskt_Data_Sub(G_E_GsktAna.SubString(1, 1), EP_B0s4_Code,
            EP_B0s4_Label, EP_B0s4_Name, EP_B0s4_Num, EP_B1s4_Code,
            EP_B1s4_Label, EP_B1s4_Name, EP_B1s4_Num,
            P_HEADS_DATA[836].TrimRight());
        s_ALabel = P_PLATECD[1];
        if (s_ALabel == s_BLabel && G_E_GsktAna.SubString(1,
            1) != "0" && G_E_GsktAna.SubString(2,
            1) != "0" && P_HEADS_DATA[835].TrimRight() == P_HEADS_DATA[836]
            .TrimRight()) {
            // 2006.07.13 ﾊﾟｰﾂﾘｽﾄで使用するのでDE_KubunとP_SideDivを記述するｻﾌﾞﾙｰﾁﾝに変える
            // E1､E2孔&A,B同材質の数量で出力
            // P_SideDiv ="";
            P_Suryo = EPLH_Meq_Sub;
            P_Yobisu = EPLH_Meq_Sub * P_PltGsktYobiE;
            if (P_Suryo == 0) {
                P_SideDiv = "";
            }
            else {
                P_SideDiv = "B";
            }
            // Write_PlateBuhin_Data(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            Write_PlateBuhin_Data_GSKT(ret); // ﾌﾟﾚｰﾄ部品ﾃﾞｰﾀCSV作成
            // ****************************************************************************
            Write_HeartsBuhin_Data(P_HEADS_DATA[836].TrimRight());
            // HEARTS用ﾃﾞｰﾀCSV作成
        }
    }
    // ディスタンスピース出力
    Write_E_Gskt_Data_Distance();

    return true;
}
