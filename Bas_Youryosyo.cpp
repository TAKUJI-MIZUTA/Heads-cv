//---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：検査要領書  作成モジュール
//   作 成 者：H.Nishijima
//   作 成 日：2002.06.04
//   更 新 日：2002.09.18
//             2007.10.10 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop

#include <utilcls.h>    // Excel起動用ﾍｯﾀ
#include <Printers.hpp> // ﾌﾟﾘﾝﾀｵﾌﾞｼﾞｪｸﾄﾍｯﾀﾞ

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Youryosyo.h"

#include "HeadsData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
#define CYOURYOXLSFILENAME    "検査要領書.xls"  // 検査要領書 元ﾌｧｲﾙ名
//2004.02.16
#define CYOURYOXLSFILENAME2   "検査要領書立会無.xls"  // 検査要領書立会無 元ﾌｧｲﾙ名
//**********
// 2011.02.09 中国語対応
#define CYOURYOXLSFILENAMEC   "検査要領書C.xls"  // 検査要領書(中) 元ﾌｧｲﾙ名
#define CYOURYOXLSFILENAME2C  "検査要領書立会無C.xls"  // 検査要領書立会無(中) 元ﾌｧｲﾙ名
// *********************
#define CYOURYOPUTNAME        "D"               // 検査要領書 出力ﾌｧｲﾙ名先頭

//---------------------------------------------------------------------------
//const short  CDatMaxNum = 4;        // 検索ﾃﾞｰﾀ 最大件数
const short  CDatMaxNum = 5;        // 検索ﾃﾞｰﾀ 最大件数		//add 2004-06-12 障害0237対応
//const short  CDatAllNum = 5;        // 出力ﾃﾞｰﾀ 最大件数(CDatMaxNum+1)
const short  CDatAllNum = 6;        // 出力ﾃﾞｰﾀ 最大件数(CDatMaxNum+1)

const short CDatYJ1 = 13;    // ﾃﾞｰﾀ1 和文ｾﾙ行 原点
const short CDatYJ2 = 34;    // ﾃﾞｰﾀ2 和文ｾﾙ行 原点
const short CDatYJ3 = 21;    // ﾃﾞｰﾀ3 和文ｾﾙ行 原点
// 2019.02.22 書式変更_S
//const short CDatYE1 = 17;    // ﾃﾞｰﾀ1 英文ｾﾙ行 原点
const short CDatYE1 = 16;    // ﾃﾞｰﾀ1 英文ｾﾙ行 原点
//const short CDatYE2 = 81;    // ﾃﾞｰﾀ2 英文ｾﾙ行 原点
const short CDatYE2 = 82;    // ﾃﾞｰﾀ2 英文ｾﾙ行 原点
// 2019.02.22 書式変更_E
const short CDatXE3 = 2;     // ﾃﾞｰﾀ3 英文ｾﾙ列 原点 (英文はﾐﾙｼｰﾄが縦並び)
// 2019.02.22 書式変更_S
const short CDatYJJ1 = 13;   // ﾃﾞｰﾀ1 新和文ｾﾙ行 原点
const short CDatYJJ2 = 45;   // ﾃﾞｰﾀ2 新和文ｾﾙ行 原点
const short CDatYJJ3 = 21;   // ﾃﾞｰﾀ3 新和文ｾﾙ行 原点
// 2019.02.22 書式変更_E

#pragma option push -b-
enum DatXJ1T  { xJ1Model=1, xJ1Qanty=3, xJ1MFGNo=4, xJ1ItemNo=6, xJ1EleNo=8, xJ1ASSNo=11 }; // ﾃﾞｰﾀ1 和文ｾﾙ列
enum DatXJ2T  { xJ2Model=2, xJ2ItemNo=5, xJ2Prs1=7, xJ2Prs2=9, xJ2Media=11 };               // ﾃﾞｰﾀ2 和文ｾﾙ列
enum DatXJ3T  { xE1M1=1, xE1M2=3, xE1M3=5, xE1M4=7, xE1M5=10 };                             // ﾃﾞｰﾀ3 和文ｾﾙ列
enum DatXE1T  { xE1Model=1, xE1Qanty=3, xE1MFGNo=4, xE1ItemNo=6, xE1EleNo=8, xE1ASSNo=11 }; // ﾃﾞｰﾀ1 英文ｾﾙ列
enum DatXE2T  { xE2Model=2, xE2ItemNo=5, xE2Prs1=7, xE2Prs2=9, xE2Media=11 };               // ﾃﾞｰﾀ2 英文ｾﾙ列
//2004.02.16
//enum DatYE3T  { yE3M1=92, yE3M2, yE3M3, yE3M4, yE3M5 };                                     // ﾃﾞｰﾀ3 英文ｾﾙ行
enum DatYE3T  { yE3M1=93, yE3M2, yE3M3, yE3M4, yE3M5 };
//**********
#pragma option pop

//---------------------------------------------------------------------------
short __fastcall ExcelSheetPDFPrint(Variant&);  // Excel ｼｰﾄ PDF印刷処理s

//---------------------------------------------------------------------------
// 検査要領書 作成Class
//---------------------------------------------------------------------------
class TYouryosyo
{
private:
    // 2021.06.18 FireDACへの移行 MOD_S
    //TQuery *P_Query;
    TFDQuery *P_Query;
    // 2021.06.18 FireDACへの移行 MOD_E
    AnsiString P_Tmplate_Pass;      // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
    AnsiString P_MakeExcel_Pass;    // 作成ﾌｧｲﾙ名

    AnsiString P_Text;              // 汎用文字列

    AnsiString P_Seiban;            // 製造番号
    AnsiString P_OkyakuCode;        // お客様ｺｰﾄﾞ
    AnsiString P_NonyusakiCode;     // 納入先ｺｰﾄﾞ
    AnsiString P_Noki;              // 納期

    AnsiString P_OkyakuName;        // お客様名
    AnsiString P_NonyusakiName;     // 納入先名
    AnsiString P_NokiDate;          // 納期(編集文字列)

    struct YouryoDataT
    {
        AnsiString ModelCode;       // 型式
        AnsiString Kisuu;           // 基数
        AnsiString Seiban;          // 製造番号
        AnsiString ItemNo;          // ITEM NO.
        AnsiString ElementDno;      // ｴﾚﾒﾝﾄ構成図
        AnsiString AssembleDno;     // 組立図
        AnsiString Pres_p1;         // 圧力Ｐ１
        AnsiString Pres_p2;         // 圧力Ｐ２
        AnsiString TestMedia;       // 試験媒体
        AnsiString ReqDate;         // 出図要求日時
    } Pb;

   //↓2007.10.10 ミラー仕様対応
   HeadsData*  m_pcHeadsData;
   AnsiString P_HEADS_DATA[1500];
   //↑2007.10.10 ミラー仕様対応

    bool __fastcall Data_Chk(void);         // データ 重複ﾁｪｯｸ
    bool __fastcall Data_Set_J(void);       // データ セット（和文）
    bool __fastcall Data_Set_E(void);       // データ セット（英文）
    // 2011.02.09 中国語対応
    bool __fastcall Data_Set_C(void);       // データ セット（中文）
    // *********************
    bool __fastcall Data_Insert(void);      // 検査要領書管理ﾃｰﾌﾞﾙ 追加更新

public:
    bool __fastcall Excute(HeadsData* pcHeadsData);   // 検査要領書 作成実行

    __fastcall TYouryosyo();
    __fastcall ~TYouryosyo();
};

//---------------------------------------------------------------------------
// 日本語関数名： 検査要領書 作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： TYouryosyo 処理
//---------------------------------------------------------------------------
bool __fastcall Youryosyo(HeadsData* pcHeadsData)
{
    bool        ret;
    TYouryosyo  youryosyo;    // 検査要領書 作成Class自動生成,関数終了後破棄

    ret = youryosyo.Excute(pcHeadsData); // 検査要領書 作成実行

    return ret;
}

//---------------------------------------------------------------------------
// 日本語関数名：ｺﾝｽﾄﾗｸﾀ TYouryosyo
// 概  要      ：
// 引  数      ：
// 戻り値      ：
// 備  考      ：
//---------------------------------------------------------------------------
__fastcall TYouryosyo::TYouryosyo()
{
   // 2021.06.18 FireDACへの移行 MOD_S
   //P_Query = new TQuery(DataModule1);  // 検査要領書 ｸｴﾘｰ生成
   P_Query = new TFDQuery(DataModule1);  // 検査要領書 ｸｴﾘｰ生成
   // 2021.06.18 FireDACへの移行 MOD_E
   if( P_Query )
   {
      // 2021.06.18 FireDACへの移行 MOD_S
      //P_Query->DatabaseName = ODBC_DSN_INS;    // 検査要領書データベース(HD_INSPCTL.mdb)
      P_Query->ConnectionName = ODBC_DSN_INS;    // 検査要領書データベース(HD_INSPCTL.mdb)
      // 2021.06.18 FireDACへの移行 MOD_E
   }
}

//---------------------------------------------------------------------------
// 日本語関数名：ﾃﾞｽﾄﾗｸﾀ TYouryosyo
// 概  要      ：
// 引  数      ：
// 戻り値      ：
// 備  考      ：
//---------------------------------------------------------------------------
__fastcall TYouryosyo::~TYouryosyo()
{
    if( P_Query )
    {
        delete P_Query;
        P_Query = NULL;
    }
}

//---------------------------------------------------------------------------
// 日本語関数名： 検査要領書 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TYouryosyo::Excute(HeadsData* pcHeadsData)
{
   bool bRet;
   AnsiString sSpecCode;
   // 2011.02.09 中国語対応
   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   AnsiString s_Text;            // 汎用文字列
   AnsiString s_China;           // 言語区分("0":日本語、"1":中国語)
   // *********************

   // HEADSﾃｷｽﾄﾃﾞｰﾀをｾｯﾄ
   m_pcHeadsData = pcHeadsData;

   if(m_pcHeadsData->GetMirrorFlag()){
        // ﾐﾗｰ仕様の場合、A仕様、B仕様以外は処理不要(正常終了)
        if(m_pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_NONE) return(true);
   }

   // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString型配列に取得
   bRet = m_pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
   if(!bRet) return(false);

   // 仕様ﾀｲﾌﾟ文字列取得(ﾌｧｲﾙ名で使用)
   sSpecCode = m_pcHeadsData->GetSpecCode();

   // 2011.02.09 中国語対応
   ////2004.02.16 ﾃﾝﾌﾟﾚｰﾄ追加
   ////P_Tmplate_Pass = G_Template_Dir + CYOURYOXLSFILENAME;                         // 検査要領書ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //if (P_HEADS_DATA[431].TrimRight() == "0") {
   //   P_Tmplate_Pass = G_Template_Dir + CYOURYOXLSFILENAME2; //"立会検査無検査要領書ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ"
   //} else {
   //   P_Tmplate_Pass = G_Template_Dir + CYOURYOXLSFILENAME;  //"立会検査有検査要領書ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ"
   //}
   ////**********************
   i_RecNo = 851;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "1" ) {
      s_China = "1";                                             // 中国語
      if (P_HEADS_DATA[431].TrimRight() == "0") {
         P_Tmplate_Pass = G_Template_Dir + CYOURYOXLSFILENAME2C; //"立会検査無検査要領書ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ"
      } else {
         P_Tmplate_Pass = G_Template_Dir + CYOURYOXLSFILENAMEC;  //"立会検査有検査要領書ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ"
      }
   } else {
      s_China = "0";                                             // 日本語
      if (P_HEADS_DATA[431].TrimRight() == "0") {
         P_Tmplate_Pass = G_Template_Dir + CYOURYOXLSFILENAME2;  //"立会検査無検査要領書ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ"
      } else {
         P_Tmplate_Pass = G_Template_Dir + CYOURYOXLSFILENAME;   //"立会検査有検査要領書ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ"
      }
   }
   // *********************
   //2023/4/21 製造番号桁数追加対応 Mod
   //P_MakeExcel_Pass = G_xls_Output_Dir + CYOURYOPUTNAME + G_SAKUBAN + sSpecCode + ".xls";    // 検査要領書ｺﾋﾟｰ先ﾌｧｲﾙ
   P_MakeExcel_Pass = G_xls_Output_Dir + CYOURYOPUTNAME + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + ".xls";    // 検査要領書ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   Write_Log("--------------------------------------------------");
   Write_Log("----------    『検査要領書』作成開始    ----------");
   Write_Log("--------------------------------------------------");
   Write_Error_Log("--------------------------------------------------");
   Write_Error_Log("----------    『検査要領書』作成開始    ----------");
   Write_Error_Log("--------------------------------------------------");

   G_Log_Text = "作成ファイル名：    『" + P_MakeExcel_Pass + "』";
   Write_Log(G_Log_Text);

   //******************************
   //***  テンプレート　コピー
   //******************************
   if( !Template_Copy(P_Tmplate_Pass,P_MakeExcel_Pass) )
   {
      // ｴﾗｰﾛｸﾞ追加
      G_ErrLog_Text = "ファイルコピーを失敗しました。『" + P_Tmplate_Pass +
                      "』→『" + P_MakeExcel_Pass + "』";
      Write_Error_Log(G_ErrLog_Text);  // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      return false;
   }
   else
   {
      G_Log_Text = "ファイルコピー成功。『" + P_Tmplate_Pass +
                      "』→『" + P_MakeExcel_Pass + "』";
      Write_Log(G_Log_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
   }


#ifdef _DEBUG
   if( (GetAsyncKeyState(VK_MENU)&0xff00) )     // [Alt]押し？
   {
      exApplication.OlePropertySet( "Visible", true );
   }
#endif

   //******************************
   //***  Excel Book Open
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
      //Windows 2008Server 移植対応
      //exWorkbook  = exWorkbooks.Exec(Function("Open") << P_MakeExcel_Pass);
      exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)P_MakeExcel_Pass );

   }
   catch (...) {
      G_ErrLog_Text = "『" + P_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
      //Windows 2008Server 移植対応
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "『" + P_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   /*
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   exWorkbook  = exWorkbooks.Exec(Function("Open") << P_MakeExcel_Pass);
   // ｼｰﾄｺﾚｸｼｮﾝ取得
   exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
   */

   //******************************
   //***  データ セット（和文）
   //******************************
   // 2011.02.09 中国語対応
   //Data_Set_J();
   if ( s_China == "0" ) {
      Data_Set_J();
   }
   //******************************
   //***  データ セット（中文）
   //******************************
   if ( s_China == "1" ) {
      Data_Set_C();
   }
   //**********************
   //******************************
   //***  データ セット（英文）
   //******************************
   Data_Set_E();

   //******************************
   //***  データ 更新
   //***  (納期がﾌﾞﾗﾝｸの場合行わない 02.08.22Add )
   //******************************
   if( !P_Noki.IsEmpty() )
   {
      if( Data_Chk() )    // 登録ﾃﾞｰﾀ 重複ﾁｪｯｸ 正常？
      {
         Data_Insert();   // データ 追加更新
      }
   }

   //******************************
   //***  Excel Book Save
   //******************************
   // 印刷しないほうのシートをＰＤＦ設定シートに記述する
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "ＰＤＦ設定");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("Item",OleVariant("ＰＤＦ設定"));
   if (P_HEADS_DATA[1].TrimRight() == "0") {
     Excel_Cell_DataSet(4, 2, "英文");
   } else {
     Excel_Cell_DataSet(4, 2, "和文");
   }

   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   if (P_HEADS_DATA[1].TrimRight() == "0") {
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   // 2021.10.19 保存処理追加_S
   } else if (P_HEADS_DATA[1].TrimRight() == "2") {
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   // 2021.10.19 保存処理追加_E
   } else {
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));
   }
   // 2020.04.08 バーコード変更_E
   // 2021.10.19 保存処理追加_S
   exWorkbook.OleProcedure("Save");  //保存
   // 2021.10.19 保存処理追加_E

   // マクロ実行
   //PDFファイル出力取消
   //exApplication.Exec(Function("Run") << "Gsub_PDFOut");

   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server 移植対応
   exApplication.OlePropertySet("DisplayAlerts", false);
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << P_MakeExcel_Pass);
   //******************************
   //***  Excel Sheet Print
   //******************************
   if( P_HEADS_DATA[1].TrimRight() == "0" )   // 和文？
   {
       //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "和文");  // 和文ｼｰﾄの指定
       //Windows 2008Server 移植対応
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   }
   else
   {
       //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "英文");  // 英文ｼｰﾄの指定
       //Windows 2008Server 移植対応
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));
   }

   //ExcelSheetPDFPrint(exWorksheet);
   //******************************
   //***  Excel Book Close
   //******************************
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ﾛｸﾞ作成
   Write_Log("--------------------------------------------------");
   Write_Log("----------    『検査要領書』作成終了    ----------");
   Write_Log("--------------------------------------------------");
   Write_Error_Log("--------------------------------------------------");
   Write_Error_Log("----------    『検査要領書』作成終了    ----------");
   Write_Error_Log("--------------------------------------------------");

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 検査要領書 Excelデータセット(和文)
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TYouryosyo::Data_Set_J(void)
{
   bool       ret=true;             // 戻り値
   int        i_RecNo;              // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   AnsiString A_SQL;                // SQL文
   AnsiString A_NokiYear;           // 納期(年)
   AnsiString A_SubStr;             // 作業用文字列
   double     d_wrk;                // 数値変換用
   int        i_wrk;                // 数値変換用
   int        i_DatCnt;             // ﾃﾞｰﾀ 登録件数(MAX4)
   int        i_DatNum;             // ﾃﾞｰﾀ 位置(0～4:更新位置)
   int        i;                    // ｶｳﾝﾀ
   double     d_BpP;            // B試験圧力
   double     d_BpH;            // B試験水圧
   double     d_ApP;            // A試験圧力
   double     d_ApH;            // A試験水力
   AnsiString s_Text;           // ワーク文字列
   // 2019.02.22 書式変更_S
   int        i_Mil;            // ﾐﾙｼｰﾄ枚数
   // 2019.02.22 書式変更_E
   // 2022.04.12 耐圧保持時間指定追加_S
   int w_count1;
   int w_count2;
   int w_count3;
   int w_count4;
   int w_count5;
   int w_count6;
   // 2022.04.12 耐圧保持時間指定追加_E


   // ｼｰﾄの指定

   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "和文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));

   //***************************
   //***  見出し
   //***************************
   //--- 御注文主
   i_RecNo = 1063;
   // 2019.02.22 書式変更_S
   //P_Text = "御注文主　" +   P_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet( 3, 1, P_Text );
   P_Text = "　　　" +   P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( 3, 2, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：御注文主       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_OkyakuName = P_Text;     // お客様名
   // お客様ｺｰﾄﾞ
   P_OkyakuCode = P_HEADS_DATA[1064].TrimRight();

   //--- 納入先
   i_RecNo = 1065;
   // 2019.02.22 書式変更_S
   //P_Text = "納入先　　" + P_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet( 5, 1, P_Text );
   P_Text = "　　　" + P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( 5, 2, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：納入先         RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_NonyusakiName = P_Text;      // 納入先名
   // 納入先ｺｰﾄﾞ
   P_NonyusakiCode = P_HEADS_DATA[1066].TrimRight();

   // 2019.02.22 書式変更_S
   //***************************
   //***  和文：年月日
   //***************************
   i_RecNo = 1087;
   P_Text = P_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(3, 10, P_Text);
   G_Log_Text = "和文：年月日         RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2019.02.22 書式変更_E

   //--- 納期
   //i_RecNo = 12;
   i_RecNo = 411;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   //P_Noki = P_Text;             // 納期
   //P_Noki = P_Text.SubString(1,4) + "/" + P_Text.SubString(5,2) + "/" + P_Text.SubString(7,2);
   if( !P_Text.IsEmpty() )
      P_Noki = P_Text.SubString(1,4) + "/" + P_Text.SubString(6,2) + "/" + P_Text.SubString(9,2);
   else
      P_Noki = "\0";

   if( !P_Text.IsEmpty() )
   {
      A_NokiYear = P_Text.SubString(1,4);
      i_wrk = A_NokiYear.ToIntDef(0);
      if( i_wrk > 1988 )    // 平成以降？
      {
         ZeroSupress(IntToStr(i_wrk-1988),A_SubStr,2);  // 平成年２桁
         A_NokiYear = " 平成 " + A_SubStr;
      }
      P_Text = A_NokiYear + " 年 " +
               P_Text.SubString(6,2) + " 月 " +
               P_Text.SubString(9,2) + " 日 ";
   }
//   Excel_Cell_DataSet( 3, 9, P_Text );
//   G_Log_Text = "和文：納期           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_NokiDate = P_Text;          // 納期(編集文字列)

   // 2022.04.12 耐圧保持時間指定追加_S
   //***************************
   //***  耐圧保持時間
   //***************************
   if (Tokki_Code("2C", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間標準
      s_Text = "　　　※　保持時間は20分間とする。";
   }
   else if (Tokki_Code("2D", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間30分
      s_Text = "　　　※　保持時間は30分間とする。";
   }
   else if (Tokki_Code("2E", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間指定あり
      s_Text = Tokki_Zen("2E", P_HEADS_DATA, 1500);
      w_count1 = s_Text.AnsiPos("：");
      w_count2 = s_Text.AnsiPos(":");

      if (w_count1){
         // "："が存在　
         s_Text = s_Text.Delete(1,w_count1+1);
      } else if (w_count2){
         // ":"が存在
         s_Text = s_Text.Delete(1,w_count2);
      } else {
         // "：",":"が存在しない
         s_Text = "";
      }

      w_count1 = s_Text.AnsiPos("分");
      w_count2 = s_Text.AnsiPos("時");
      w_count3 = s_Text.AnsiPos("m");
      w_count4 = s_Text.AnsiPos("M");
      w_count5 = s_Text.AnsiPos("h");
      w_count6 = s_Text.AnsiPos("H");

      if (w_count1){
         // "分"が存在　
         s_Text = s_Text.SetLength(w_count1-1);
      } else if (w_count2){
         // "時"が存在
         s_Text = "";
      } else if (w_count3){
         // "m"が存在　
         s_Text = s_Text.SetLength(w_count3-1);
      } else if (w_count4){
         // "M"が存在
         s_Text = s_Text.SetLength(w_count4-1);
      } else if (w_count5){
         // "h"が存在
         s_Text = "";
      } else if (w_count6){
         // "H"が存在
         s_Text = "";
      } else if (s_Text == ""){
          // 特記が存在しない
      } else {
          // "分","m","M"が存在しない
      }

      if (s_Text == ""){
         // 特記が存在しない
         s_Text = "　　　※　保持時間入力エラー。";
      } else {
         s_Text = s_Text.TrimRight();
         s_Text = ZenToHan(s_Text);
         s_Text = "　　　※　保持時間は" + s_Text + "分間とする。";
      }

   } else {
      // 耐圧保持時間指定なし
      s_Text = "　　　※　保持時間は20分間とする。";
   }

   Excel_Cell_DataSet(43, 1, s_Text);
   // 2022.04.12 耐圧保持時間指定追加_E

   //***************************
   //***  該当ﾃﾞｰﾀ 最大４件まで読込み、ｾﾙに設定
   //***************************
   A_SQL = "";
   A_SQL = A_SQL + "SELECT * FROM HD_INSPCTL";
   A_SQL = A_SQL + " WHERE OKYAKUSAMA_CODE = '" + P_OkyakuCode + "'";
   A_SQL = A_SQL + " AND   NONYUSAKI_CODE = '" + P_NonyusakiCode + "'";

   if( !P_Noki.IsEmpty() )
      A_SQL = A_SQL + " AND   NOKI_DATE = CDATE('" + P_Noki + "')";

   A_SQL = A_SQL + ";";

   i_DatCnt = 0;   // ﾃﾞｰﾀ 件数(MAX4)
   i_DatNum = 0;   // ﾃﾞｰﾀ 位置(0～4:更新位置)

   try
   {
      P_Query->Close();
      P_Query->SQL->Clear();
      P_Query->SQL->Add(A_SQL);
      P_Query->Open();
      P_Query->First();
      while( !P_Query->Eof )   // ﾃﾞｰﾀが存在する間
      {
         i_DatCnt++;           // 数 更新
         if( i_DatCnt < CDatMaxNum )   // 数 範囲内？
         {
            // ﾚｺｰﾄﾞﾃﾞｰﾀ 取得
            Pb.ModelCode   = P_Query->FieldByName("MODEL_CODE")->AsString.TrimRight();     // 型式
            Pb.Kisuu       = P_Query->FieldByName("KISUU")->AsString.TrimRight();          // 基数
            Pb.Seiban      = P_Query->FieldByName("SEIBAN")->AsString.TrimRight();         // 製造番号
            Pb.ItemNo      = P_Query->FieldByName("ITEMNO")->AsString.TrimRight();         // ITEM NO.
            Pb.ElementDno  = P_Query->FieldByName("ELEMENT_DNO")->AsString.TrimRight();    // ｴﾚﾒﾝﾄ構成図
            Pb.AssembleDno = P_Query->FieldByName("ASSEMBLE_DNO")->AsString.TrimRight();   // 組立図
            Pb.Pres_p1     = P_Query->FieldByName("PRES_P1")->AsString.TrimRight();        // 圧力Ｐ１
            Pb.Pres_p2     = P_Query->FieldByName("PRES_P2")->AsString.TrimRight();        // 圧力Ｐ２
            Pb.TestMedia   = P_Query->FieldByName("TEST_MEDIA")->AsString.TrimRight();     // 試験媒体
            Pb.ReqDate     = P_Query->FieldByName("REQ_DATE")->AsString.TrimRight();       // 日時

            //2022/12/15 桁数追加対応 Mod_S
            //P_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
            P_Text = GetSeizouBangou(G_SAKUBAN);
            //2022/12/15 桁数追加対応 Mod_E
               
            //----------------------------
            // 2007.10.15 ﾐﾗｰ仕様対応
            //if ( Pb.Seiban != P_Text) {
            if ( Pb.Seiban.SubString(1, P_Text.Length()) != P_Text) {
            // 2007.10.15
            //----------------------------


               // 2019.02.22 書式変更_S
               /*
               //--- 型式
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Model, Pb.ModelCode );
               G_Log_Text = "和文：型式              No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);
               //--- 型式(下段)
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Model, Pb.ModelCode );
               G_Log_Text = "和文：型式(下)          No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 基数
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Qanty, Pb.Kisuu );
               G_Log_Text = "和文：基数              No:" + IntToStr(i_DatCnt) + " 『" + Pb.Kisuu + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 製造番号
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1MFGNo, Pb.Seiban );
               G_Log_Text = "和文：製造番号          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Seiban + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 機器番号(ITEM NO)
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ItemNo, Pb.ItemNo );
               G_Log_Text = "和文：機器番号          No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);
               //--- 機器番号(ITEM NO)(下段)
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2ItemNo, Pb.ItemNo );
               G_Log_Text = "和文：機器番号(下)      No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);

               //--- ｴﾚﾒﾝﾄ構成図
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1EleNo, Pb.ElementDno );
               G_Log_Text = "和文：ｴﾚﾒﾝﾄ構成図       No:" + IntToStr(i_DatCnt) + " 『" + Pb.ElementDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 組立図
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ASSNo, Pb.AssembleDno );
               G_Log_Text = "和文：組立図            No:" + IntToStr(i_DatCnt) + " 『" + Pb.AssembleDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ１
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, Pb.Pres_p1 );
               G_Log_Text = "和文：圧力Ｐ１          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p1 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ２
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, Pb.Pres_p2 );
               G_Log_Text = "和文：圧力Ｐ２          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p2 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 試験媒体
               if ( Pb.TestMedia == "H" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "水圧" );
               } else if ( Pb.TestMedia == "P" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "空圧" );
               } else if ( Pb.TestMedia == "P/H" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "空圧/水圧" );
               // 2011.02.09 中国語対応
               } else if ( Pb.TestMedia == "水/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "水圧" );
               } else if ( Pb.TestMedia == "气/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "空圧" );
               } else if ( Pb.TestMedia == "气/U+21387/U+/水/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "空圧/水圧" );
               // **********************
               } else {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, Pb.TestMedia );
               }
               G_Log_Text = "和文：試験媒体          No:" + IntToStr(i_DatCnt) + " 『" + Pb.TestMedia + "』をセット。";
               Write_Log(G_Log_Text);
               */
               //--- 型式
               Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1Model, Pb.ModelCode );
               G_Log_Text = "和文：型式              No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);
               //--- 型式(下段)
               Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Model, Pb.ModelCode );
               G_Log_Text = "和文：型式(下)          No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 基数
               Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1Qanty, Pb.Kisuu );
               G_Log_Text = "和文：基数              No:" + IntToStr(i_DatCnt) + " 『" + Pb.Kisuu + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 製造番号
               Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1MFGNo, Pb.Seiban );
               G_Log_Text = "和文：製造番号          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Seiban + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 機器番号(ITEM NO)
               Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1ItemNo, Pb.ItemNo );
               G_Log_Text = "和文：機器番号          No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);
               //--- 機器番号(ITEM NO)(下段)
               Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2ItemNo, Pb.ItemNo );
               G_Log_Text = "和文：機器番号(下)      No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);

               //--- ｴﾚﾒﾝﾄ構成図
               Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1EleNo, Pb.ElementDno );
               G_Log_Text = "和文：ｴﾚﾒﾝﾄ構成図       No:" + IntToStr(i_DatCnt) + " 『" + Pb.ElementDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 組立図
               Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1ASSNo, Pb.AssembleDno );
               G_Log_Text = "和文：組立図            No:" + IntToStr(i_DatCnt) + " 『" + Pb.AssembleDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ１
               Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs1, Pb.Pres_p1 );
               G_Log_Text = "和文：圧力Ｐ１          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p1 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ２
               Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs2, Pb.Pres_p2 );
               G_Log_Text = "和文：圧力Ｐ２          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p2 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 試験媒体
               if ( Pb.TestMedia == "H" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "水圧" );
               } else if ( Pb.TestMedia == "P" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "空圧" );
               } else if ( Pb.TestMedia == "P/H" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "空圧/水圧" );
               // 2011.02.09 中国語対応
               } else if ( Pb.TestMedia == "水/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "水圧" );
               } else if ( Pb.TestMedia == "气/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "空圧" );
               } else if ( Pb.TestMedia == "气/U+21387/U+/水/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "空圧/水圧" );
               // **********************
               } else {
                  Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, Pb.TestMedia );
               }
               G_Log_Text = "和文：試験媒体          No:" + IntToStr(i_DatCnt) + " 『" + Pb.TestMedia + "』をセット。";
               Write_Log(G_Log_Text);
               // 2019.02.22 書式変更_E


               i_DatNum++;       // 位置更新
            }
            P_Query->Next();  // 次へ
         }
         else
         {
            break; // ﾙｰﾌﾟ終了
         }
      }
      P_Query->Close();
   }
   catch(EDatabaseError &Error)
   {
      G_ErrLog_Text = "--検査要領書管理ﾃｰﾌﾞﾙの参照に失敗しました！！--" ;
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      ret = false;
      P_Query->Close();
   }

   if( i_DatCnt > CDatMaxNum )  // 最大数 超え？
   {
      // 最大数 超えerr
      G_ErrLog_Text = "和文：出力対象データが４件を超えました！！";
      Write_Error_Log(G_ErrLog_Text);       // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);     // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }


   //***************************
   //***  指定のﾃﾞｰﾀを ｾﾙに設定
   //***************************
   //--- 型式
   i_RecNo = 33;
   if( P_HEADS_DATA[1057].TrimRight() == "1" ) {    // 改造？
      P_Text = P_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   }
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Model, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1Model, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：型式           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 型式(下段)
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Model, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Model, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：型式(下)       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 基数
   i_RecNo = 39;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Qanty, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1Qanty, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：基数           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 製造番号
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //P_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   P_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E
      
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1MFGNo, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1MFGNo, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：製造番号      『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 機器番号(ITEM NO.)
   i_RecNo = 27;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ItemNo, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1ItemNo, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：機器番号       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 機器番号(ITEM NO.)(下段)
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2ItemNo, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2ItemNo, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：機器番号(下)   RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ｴﾚﾒﾝﾄ構成図
   //2023/4/21 製造番号桁数追加対応 Mod
   //P_Text = "A" + G_SAKUBAN;
   P_Text = "A" + GetFileSeizouBangou(G_SAKUBAN);
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1EleNo, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1EleNo, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：ｴﾚﾒﾝﾄ構成図   『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 組立図
   //2023/4/21 製造番号桁数追加対応 Mod
   //P_Text = "B" + G_SAKUBAN;
   P_Text = "B" + GetFileSeizouBangou(G_SAKUBAN);
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ASSNo, P_Text );
   Excel_Cell_DataSet( i_DatNum+CDatYJJ1, xJ1ASSNo, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：組立図        『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   d_BpP = 0.0;
   d_BpH = 0.0;
   d_ApP = 0.0;
   d_ApH = 0.0;
   s_Text = P_HEADS_DATA[62].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApP = s_Text.ToDouble();
      } else {
         d_BpP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[61].Trim();   // Ｂ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApH = s_Text.ToDouble();
      } else {
         d_BpH = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[93].Trim();   // Ａ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpP = s_Text.ToDouble();
      } else {
         d_ApP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[92].Trim();   // Ａ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpH = s_Text.ToDouble();
      } else {
         d_ApH = s_Text.ToDouble();
      }
   }

   if( d_BpP > 0.0 || d_ApP > 0.0) {         // Ｂ試験圧力＞０又はＡ試験圧力＞０
      if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
         // 両方の条件が成立する場合は両方出力する
         P_Text = FormatFloat("#,0.00",d_BpP) + "/" + FormatFloat("#,0.00",d_BpH);
         // 2019.02.22 書式変更_S
         //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, P_Text );
         Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs1, P_Text );
         // 2019.02.22 書式変更_E
         G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP) + "/" + FormatFloat("#,0.00",d_ApH);
         // 2019.02.22 書式変更_S
         //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, P_Text );
         Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs2, P_Text );
         // 2019.02.22 書式変更_E
         G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);

         // 2019.02.22 書式変更_S
         //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "空圧/水圧" );
         Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "空圧/水圧" );
         // 2019.02.22 書式変更_E
      } else {
         P_Text = FormatFloat("#,0.00",d_BpP);
         // 2019.02.22 書式変更_S
         //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, P_Text );
         Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs1, P_Text );
         // 2019.02.22 書式変更_E
         G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP);
         // 2019.02.22 書式変更_S
         //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, P_Text );
         Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs2, P_Text );
         // 2019.02.22 書式変更_E
         G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         // 2019.02.22 書式変更_S
         //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "空圧" );
         Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "空圧" );
         // 2019.02.22 書式変更_E
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
      P_Text = FormatFloat("#,0.00",d_BpH);
      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, P_Text );
      Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs1, P_Text );
      // 2019.02.22 書式変更_E
      G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      P_Text = FormatFloat("#,0.00",d_ApH);
      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, P_Text );
      Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs2, P_Text );
      // 2019.02.22 書式変更_E
      G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "水圧" );
      Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "水圧" );
      // 2019.02.22 書式変更_E
   } else {
      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, "-----" );
      //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, "-----" );
      //Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "" );
      Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs1, "-----" );
      Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Prs2, "-----" );
      Excel_Cell_DataSet( i_DatNum+CDatYJJ2, xJ2Media, "" );
      // 2019.02.22 書式変更_E
      G_ErrLog_Text = "和文：Bテスト圧力  該当する数値がありません。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }
/*
   //--- 圧力Ｐ１
   i_RecNo = 827;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( P_Text.IsEmpty() )
   {
      P_Text = "0";
   }
   if( AllFloatChk(P_Text.c_str(),0) == 0 )
   {
      d_wrk = StrToDblDef(P_Text);
      if( d_wrk == 0.0 ) {  // ０？
         P_Text = "-----";
      } else {
         P_Text = FormatFloat("#,0.00",d_wrk);
      }
      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, P_Text );
      G_Log_Text = "和文：圧力Ｐ１       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "和文：圧力Ｐ１      RecNo:" + IntToStr(i_RecNo) + "  不正な値『" + P_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //--- 圧力Ｐ２
   i_RecNo = 832;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( P_Text.IsEmpty() )
   {
      P_Text = "0";
   }
   if( AllFloatChk(P_Text.c_str(),0) == 0 )
   {
      d_wrk = StrToDblDef(P_Text);
      if( d_wrk == 0.0 ) {  // ０？
         P_Text = "-----";
      } else {
         P_Text = FormatFloat("#,0.00",d_wrk);
      }
      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, P_Text );
      G_Log_Text = "和文：圧力Ｐ２       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "和文：圧力Ｐ２      RecNo:" + IntToStr(i_RecNo) + "  不正な値『" + P_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //--- 試験媒体
   //i_RecNo = 9999;
   //P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( P_Text == "1" ) {
      P_Text = "水圧";
   } else {
      P_Text = "水圧";  // "気圧"？@@ 分岐方法が不明
   }
   Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, P_Text );
   G_Log_Text = "和文：試験媒体      『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

*/
   //***************************
   //***  ﾃﾞｰﾀ ５件分までの残りｾﾙを空白に
   //***************************
   for( i=i_DatNum+1 ; i<CDatAllNum ; i++ )
   {
      // 2019.02.22 書式変更_S
      ////--- 型式
      //Excel_Cell_DataSet( i+CDatYJ1, xJ1Model, "" );
      ////--- 基数
      //Excel_Cell_DataSet( i+CDatYJ1, xJ1Qanty, "" );
      ////--- 製造番号
      //Excel_Cell_DataSet( i+CDatYJ1, xJ1MFGNo, "" );
      ////--- ITEM NO
      //Excel_Cell_DataSet( i+CDatYJ1, xJ1ItemNo, "" );
      ////--- ｴﾚﾒﾝﾄ構成図
      //Excel_Cell_DataSet( i+CDatYJ1, xJ1EleNo, "" );
      ////--- 組立図
      //Excel_Cell_DataSet( i+CDatYJ1, xJ1ASSNo, "" );
      ////--- 型式(下段)
      //Excel_Cell_DataSet( i+CDatYJ2, xJ2Model, "" );
      ////--- ITEM NO(下段)
      //Excel_Cell_DataSet( i+CDatYJ2, xJ2ItemNo, "" );
      ////--- 圧力Ｐ１
      //Excel_Cell_DataSet( i+CDatYJ2, xJ2Prs1, "" );
      ////--- 圧力Ｐ２
      //Excel_Cell_DataSet( i+CDatYJ2, xJ2Prs2, "" );
      ////--- 試験媒体
      //Excel_Cell_DataSet( i+CDatYJ2, xJ2Media, "" );
      //--- 型式
      Excel_Cell_DataSet( i+CDatYJJ1, xJ1Model, "" );
      //--- 基数
      Excel_Cell_DataSet( i+CDatYJJ1, xJ1Qanty, "" );
      //--- 製造番号
      Excel_Cell_DataSet( i+CDatYJJ1, xJ1MFGNo, "" );
      //--- ITEM NO
      Excel_Cell_DataSet( i+CDatYJJ1, xJ1ItemNo, "" );
      //--- ｴﾚﾒﾝﾄ構成図
      Excel_Cell_DataSet( i+CDatYJJ1, xJ1EleNo, "" );
      //--- 組立図
      Excel_Cell_DataSet( i+CDatYJJ1, xJ1ASSNo, "" );
      //--- 型式(下段)
      Excel_Cell_DataSet( i+CDatYJJ2, xJ2Model, "" );
      //--- ITEM NO(下段)
      Excel_Cell_DataSet( i+CDatYJJ2, xJ2ItemNo, "" );
      //--- 圧力Ｐ１
      Excel_Cell_DataSet( i+CDatYJJ2, xJ2Prs1, "" );
      //--- 圧力Ｐ２
      Excel_Cell_DataSet( i+CDatYJJ2, xJ2Prs2, "" );
      //--- 試験媒体
      Excel_Cell_DataSet( i+CDatYJJ2, xJ2Media, "" );
      // 2019.02.22 書式変更_E
   }


   //***********************
   //***  材料検査欄
   //***********************
   //--- ﾌﾟﾚｰﾄ
   i_RecNo = 433;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   i_Mil = 0;
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "プレート" : "";
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( CDatYJ3, xE1M1, P_Text );
   Excel_Cell_DataSet( 23, 2, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：ﾌﾟﾚｰﾄ          RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾌﾚｰﾑ
   i_RecNo = 434;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "フレーム" : "";
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( CDatYJ3, xE1M2, P_Text );
   Excel_Cell_DataSet( 24, 2, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：ﾌﾚｰﾑ           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 締付ﾎﾞﾙﾄﾅｯﾄ
   i_RecNo = 435;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "締付ボルトナット" : "";
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( CDatYJ3, xE1M3, P_Text );
   Excel_Cell_DataSet( 25, 2, P_Text );
   // 2019.02.22 書式変更_E

   G_Log_Text = "和文：締付ﾎﾞﾙﾄﾅｯﾄ    RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)
   i_RecNo = 436;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "ノズル（パイプ）" : "";
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( CDatYJ3, xE1M4, P_Text );
   Excel_Cell_DataSet( 26, 2, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)    RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾉｽﾞﾙ一式
   i_RecNo = 437;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "ノズル（一式）" : "";
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( CDatYJ3, xE1M5, P_Text );
   Excel_Cell_DataSet( 27, 2, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "和文：ﾉｽﾞﾙ(一式)     RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2019.02.22 書式変更_S
   //--- ﾐﾙｼｰﾄ
   if( i_Mil > 0) {
      P_Text = "    対象部品は以下の通りとし、検査記録としてミルシートを提出します。";
      Excel_Cell_DataSet( 22, 1, P_Text );
      G_Log_Text = "和文：ﾐﾙｼｰﾄ          『" + P_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   // 2019.02.22 書式変更_E


   //***********************
   //***  ver.欄
   //***********************
   i_RecNo = 1267;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   //@@ 現,1291:STAN区分 無し
   if( P_HEADS_DATA[302].TrimRight() == "1" ) // STAN区分あり？
   {
       P_Text = P_Text + "S";
   }
   // 2017.10.06 短納期対応_S
   else if (P_HEADS_DATA[302].TrimRight() == "2") // 短納期対応
   {
       P_Text = P_Text + "T";
   }
   // 2017.10.06 短納期対応_E
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( 54, 12, P_Text );
   Excel_Cell_DataSet( 55, 11, P_Text );
   // 2019.02.22 書式変更_S
   G_Log_Text = "和文：HEADS VER      RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);


   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));
   // 2020.04.08 バーコード変更_E

   // 2019.02.19 バーコード追加_S
   //***************************
   //***  和文：バーコード作番
   //***************************
   P_Text = G_SAKUBAN;
   // 2020.04.08 バーコード変更_S
   //Excel_Cell_DataSet(10, 20, P_Text);
   Excel_Cell_DataSet(4, 2, P_Text);
   // 2020.04.08 バーコード変更_E
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：訂正
   //***************************
   i_RecNo = 319;
   P_Text = P_HEADS_DATA[i_RecNo];
   // 2020.04.08 バーコード変更_S
   //Excel_Cell_DataSet(14, 20, P_Text);
   Excel_Cell_DataSet(8, 2, P_Text);
   // 2020.04.08 バーコード変更_E
   G_Log_Text = "和文：訂正            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2019.02.31 バーコード追加_E

   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   // 2020.04.08 バーコード変更_E


   return ret;
}

//---------------------------------------------------------------------------
// 日本語関数名： 検査要領書 Excelデータセット(英文)
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TYouryosyo::Data_Set_E(void)
{
   bool       ret=true;             // 戻り値
   int        i_RecNo;              // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   AnsiString A_SQL;                // SQL文
   double     d_wrk;                // 数値変換用
   int        i_DatCnt;             // ﾃﾞｰﾀ 登録件数(MAX4)
   int        i_DatNum;             // ﾃﾞｰﾀ 位置(0～4:更新位置)
   int        i;                    // ｶｳﾝﾀ
   double     d_BpP;            // B試験圧力
   double     d_BpH;            // B試験水圧
   double     d_ApP;            // A試験圧力
   double     d_ApH;            // A試験水力
   AnsiString s_Text;           // ワーク文字列
   // 2019.02.22 書式変更_S
   int        i_Mil;            // ﾐﾙｼｰﾄ枚数
   // 2019.02.22 書式変更_E
   // 2022.04.12 耐圧保持時間指定追加_S
   int w_count1;
   int w_count2;
   int w_count3;
   int w_count4;
   int w_count5;
   int w_count6;
   // 2022.04.12 耐圧保持時間指定追加_E


   //--- ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "英文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));

   //***************************
   //***  見出し
   //***************************
   //--- 御注文主
   i_RecNo = 1261;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( 5, 3, P_Text );
   G_Log_Text = "英文：PURCHASER      RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_OkyakuName = P_Text;     // お客様ｺｰﾄﾞ
   // お客様ｺｰﾄﾞ
   P_OkyakuCode = P_HEADS_DATA[1064].TrimRight();

   //--- 納入先
   i_RecNo = 1262;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( 6, 3, P_Text );
   G_Log_Text = "英文：CUSTOMER       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_NonyusakiName = P_Text;      // 納入先ｺｰﾄﾞ
   // 納入先ｺｰﾄﾞ
   P_NonyusakiCode = P_HEADS_DATA[1066].TrimRight();

   //--- P.O.No.
   //i_RecNo = 1065;
   //P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   //P_Text = "";
   //Excel_Cell_DataSet( 7, 3, P_Text );
   // 2019.02.22 書式変更_E
   //G_Log_Text = "英文：P.O.No.        RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   //Write_Log(G_Log_Text);

   //--- MFG.NO.
   //2023/04/12 製造番号桁数追加対応_S
   //P_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   P_Text = GetSeizouBangou(G_SAKUBAN);
   //2023/04/12 製造番号桁数追加対応_E
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( 8, 3, P_Text );
   Excel_Cell_DataSet( 7, 3, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "英文：MFG.No.        RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 機器番号(ITEM NO.)
   i_RecNo = 27;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( 9, 3, P_Text );
   Excel_Cell_DataSet( 8, 3, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "英文：ITEM NO        RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 型式
   i_RecNo = 33;
// 英文の場合（改）は不要
//   if( P_HEADS_DATA[1057].TrimRight() == "1" ) {    // 改造？
//      P_Text = P_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
//   } else {
      P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
//   }
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( 10, 3, P_Text );
   Excel_Cell_DataSet( 9, 3, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "英文：MODEL          RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 台数
   i_RecNo = 39;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet( 11, 3, P_Text );
   Excel_Cell_DataSet( 10, 3, P_Text );
   // 2019.02.22 書式変更_E
   G_Log_Text = "英文：QUANTITY       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2019.02.22 書式変更_S
   //***************************
   //***  英文：年月日
   //***************************
   i_RecNo = 1056;
   P_Text = P_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(3, 10, P_Text);
   G_Log_Text = "英文：DATE           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2019.02.22 書式変更_E


   //--- 納期
   //i_RecNo = 12;
   i_RecNo = 411;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   //P_Noki = P_Text;             // 納期
   //P_Noki = P_Text.SubString(1,4) + P_Text.SubString(7,2) + P_Text.SubString(11,2);
   if( !P_Text.IsEmpty() )
      P_Noki = P_Text.SubString(1,4) + "/" + P_Text.SubString(6,2) + "/" + P_Text.SubString(9,2);
   else
      P_Noki = "\0";

   /*
   if( !P_Text.IsEmpty() )
   {
      P_Text = " DATE : " + P_Text.SubString(1,4) + " / " +
               P_Text.SubString(5,2) + " / " +
               P_Text.SubString(7,2);
   }
   */
//   Excel_Cell_DataSet( 3, 1, P_Text );
//   G_Log_Text = "英文：納期           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_NokiDate = P_Text;         // 納期(編集文字列)

   // 2022.04.11 耐圧保持時間指定追加_S
   //***************************
   //***  耐圧保持時間
   //***************************
   if (Tokki_Code("2C", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間標準
      s_Text = "Note) Equipment is left standing 20 min.";
   }
   else if (Tokki_Code("2D", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間30分
      s_Text = "Note) Equipment is left standing 30 min.";
   }
   else if (Tokki_Code("2E", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間指定あり
      s_Text = Tokki_Zen("2E", P_HEADS_DATA, 1500);
      w_count1 = s_Text.AnsiPos("：");
      w_count2 = s_Text.AnsiPos(":");

      if (w_count1){
         // "："が存在　
         s_Text = s_Text.Delete(1,w_count1+1);
      } else if (w_count2){
         // ":"が存在
         s_Text = s_Text.Delete(1,w_count2);
      } else {
         // "：",":"が存在しない
         s_Text = "";
      }

      w_count1 = s_Text.AnsiPos("分");
      w_count2 = s_Text.AnsiPos("時");
      w_count3 = s_Text.AnsiPos("m");
      w_count4 = s_Text.AnsiPos("M");
      w_count5 = s_Text.AnsiPos("h");
      w_count6 = s_Text.AnsiPos("H");

      if (w_count1){
         // "分"が存在　
         s_Text = s_Text.SetLength(w_count1-1);
      } else if (w_count2){
         // "時"が存在
         s_Text = "";
      } else if (w_count3){
         // "m"が存在　
         s_Text = s_Text.SetLength(w_count3-1);
      } else if (w_count4){
         // "M"が存在
         s_Text = s_Text.SetLength(w_count4-1);
      } else if (w_count5){
         // "h"が存在
         s_Text = "";
      } else if (w_count6){
         // "H"が存在
         s_Text = "";
      } else if (s_Text == ""){
          // 特記が存在しない
      } else {
          // "分","m","M"が存在しない
      }

      if (s_Text == ""){
         // 特記が存在しない
         s_Text = "※　保持時間入力エラー。";
      } else {
         s_Text = s_Text.TrimRight();
         s_Text = ZenToHan(s_Text);
         s_Text = "Note) Equipment is left standing " + s_Text + " min.";
      }

   }
   else {
      // 耐圧保持時間指定なし
      s_Text = "Note) Equipment is left standing 20 min.";
   }

   Excel_Cell_DataSet(79, 2, s_Text);
   // 2022.04.12 耐圧保持時間指定追加_E

   //***************************
   //***  該当ﾃﾞｰﾀ 最大４件まで読込み、ｾﾙに設定
   //***************************
   A_SQL = "";
   A_SQL = A_SQL + "SELECT * FROM HD_INSPCTL";
   A_SQL = A_SQL + " WHERE OKYAKUSAMA_CODE = '" + P_OkyakuCode + "'";
   A_SQL = A_SQL + " AND   NONYUSAKI_CODE = '" + P_NonyusakiCode + "'";
   if( !P_Noki.IsEmpty() )
      A_SQL = A_SQL + " AND   NOKI_DATE = CDATE('" + P_Noki + "') ";
   A_SQL = A_SQL + ";";

   i_DatCnt = 0;   // ﾃﾞｰﾀ 件数(MAX4)
   i_DatNum = 0;   // ﾃﾞｰﾀ 位置(0～4:更新位置)

   try
   {
      P_Query->Close();
      P_Query->SQL->Clear();
      P_Query->SQL->Add(A_SQL);
      P_Query->Open();
      P_Query->First();
      while( !P_Query->Eof )   // ﾃﾞｰﾀが存在する間
      {
         i_DatCnt++;           // 数 更新
         if( i_DatCnt < CDatMaxNum )   // 数 範囲内？
         {
            // ﾚｺｰﾄﾞﾃﾞｰﾀ 取得
            Pb.ModelCode   = P_Query->FieldByName("MODEL_CODE")->AsString.TrimRight();     // 型式
            Pb.Kisuu       = P_Query->FieldByName("KISUU")->AsString.TrimRight();          // 基数
            Pb.Seiban      = P_Query->FieldByName("SEIBAN")->AsString.TrimRight();         // 製造番号
            Pb.ItemNo      = P_Query->FieldByName("ITEMNO")->AsString.TrimRight();         // ITEM NO.
            Pb.ElementDno  = P_Query->FieldByName("ELEMENT_DNO")->AsString.TrimRight();    // ｴﾚﾒﾝﾄ構成図
            Pb.AssembleDno = P_Query->FieldByName("ASSEMBLE_DNO")->AsString.TrimRight();   // 組立図
            Pb.Pres_p1     = P_Query->FieldByName("PRES_P1")->AsString.TrimRight();        // 圧力Ｐ１
            Pb.Pres_p2     = P_Query->FieldByName("PRES_P2")->AsString.TrimRight();        // 圧力Ｐ２
            Pb.TestMedia   = P_Query->FieldByName("TEST_MEDIA")->AsString.TrimRight();     // 試験媒体

            //2022/12/15 桁数追加対応 Mod_S
            //P_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
            P_Text = GetSeizouBangou(G_SAKUBAN);
            //2022/12/15 桁数追加対応 Mod_E            
            //----------------------------
            // 2007.10.15 ﾐﾗｰ仕様対応
            //if ( Pb.Seiban != P_Text) {
            if ( Pb.Seiban.SubString(1, P_Text.Length()) != P_Text) {
            // 2007.10.15
            //----------------------------
               //--- 型式
               if ( Pb.ModelCode.Pos("改") != 0 ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1Model, Pb.ModelCode.SubString(1,Pb.ModelCode.Length()-5) );
               } else {
                  Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1Model, Pb.ModelCode );
               }
               G_Log_Text = "英文：MODEL             No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);
               //--- 型式(下段)
               if ( Pb.ModelCode.Pos("改") != 0 ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Model, Pb.ModelCode.SubString(1,Pb.ModelCode.Length()-5) );
               } else {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Model, Pb.ModelCode );
               }
               G_Log_Text = "英文：MODEL(NEXT)       No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 基数
               Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1Qanty, Pb.Kisuu );
               G_Log_Text = "英文：QANT'Y            No:" + IntToStr(i_DatCnt) + " 『" + Pb.Kisuu + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 製造番号
               Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1MFGNo, Pb.Seiban );
               G_Log_Text = "英文：MFG.NO.           No:" + IntToStr(i_DatCnt) + " 『" + Pb.Seiban + "』をセット。";
               Write_Log(G_Log_Text);

               //--- ITEM NO
               Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1ItemNo, Pb.ItemNo );
               G_Log_Text = "英文：ITEM NO           No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);
               //--- ITEM NO(下段)
               Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2ItemNo, Pb.ItemNo );
               G_Log_Text = "英文：ITEM NO(NEXT)     No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);

               //--- ｴﾚﾒﾝﾄ構成図
               Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1EleNo, Pb.ElementDno );
               G_Log_Text = "英文：ARRANG.DWG.NO     No:" + IntToStr(i_DatCnt) + " 『" + Pb.ElementDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 組立図
               Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1ASSNo, Pb.AssembleDno );
               G_Log_Text = "英文：ASSY DWG No       No:" + IntToStr(i_DatCnt) + " 『" + Pb.AssembleDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ１
               Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs1, Pb.Pres_p1 );
               G_Log_Text = "英文：P1                No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p1 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ２
               Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs2, Pb.Pres_p2 );
               G_Log_Text = "英文：P2                No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p2 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 試験媒体
               if ( Pb.TestMedia == "水圧" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "H" );
               } else if ( Pb.TestMedia == "空圧" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "P" );
               } else if ( Pb.TestMedia == "空圧/水圧" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "P/H" );
               // 2011.02.09 中国語対応
               } else if ( Pb.TestMedia == "水/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "H" );
               } else if ( Pb.TestMedia == "气/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "P" );
               } else if ( Pb.TestMedia == "气/U+21387/U+/水/U+21387/U+" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "P/H" );
               // **********************
               } else {
                  Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, Pb.TestMedia );
               }
               G_Log_Text = "英文：Test Medium       No:" + IntToStr(i_DatCnt) + " 『" + Pb.TestMedia + "』をセット。";
               Write_Log(G_Log_Text);


               i_DatNum++;       // 位置更新
            }

            P_Query->Next();  // 次へ
         }
         else
         {
            break; // ﾙｰﾌﾟ終了
         }
      }
      P_Query->Close();
   }
   catch(EDatabaseError &Error)
   {
      G_ErrLog_Text = "--検査要領書管理ﾃｰﾌﾞﾙの参照に失敗しました！！--" ;
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      ret = false;
      P_Query->Close();
   }

   if( i_DatCnt > CDatMaxNum )  // 最大数 超え？
   {
      // 最大数 超えerr
      G_ErrLog_Text = "英文：出力対象データが４件を超えました！！";
      Write_Error_Log(G_ErrLog_Text);       // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);     // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }


   //***************************
   //***  指定のﾃﾞｰﾀを ｾﾙに設定
   //***************************
   //--- 型式
   i_RecNo = 33;
   if( P_HEADS_DATA[1057].TrimRight() == "1" ) {    // 改造？
      P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   } else {
      P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1Model, P_Text );
   G_Log_Text = "英文：MODEL          RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //--- 型式(下段)
   Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Model, P_Text );
   G_Log_Text = "英文：MODEL(NEXT)    RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 基数
   i_RecNo = 39;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1Qanty, P_Text );
   G_Log_Text = "英文：QANT'Y         RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 製造番号
   //2022/12/15 桁数追加対応 Mod_S
   //P_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   P_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 桁数追加対応 Mod_E
      
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------
   Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1MFGNo, P_Text );
   G_Log_Text = "英文：MFG.No.       『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 機器番号(ITEM NO.)
   i_RecNo = 27;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1ItemNo, P_Text );
   G_Log_Text = "英文：ITEM NO        RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //--- 機器番号(ITEM NO.)(下段)
   Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2ItemNo, P_Text );
   G_Log_Text = "英文：ITEM NO(NEXT)  RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ｴﾚﾒﾝﾄ構成図
   //2023/4/21 製造番号桁数追加対応 Mod
   //P_Text = "A" + G_SAKUBAN;
   P_Text = "A" + GetFileSeizouBangou(G_SAKUBAN);
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------
   Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1EleNo, P_Text );
   G_Log_Text = "英文：ARRANG.DWG.NO 『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 組立図
   //2023/4/21 製造番号桁数追加対応 Mod
   //P_Text = "B" + G_SAKUBAN;
   P_Text = "B" + GetFileSeizouBangou(G_SAKUBAN);
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------
   Excel_Cell_DataSet( i_DatNum+CDatYE1, xE1ASSNo, P_Text );
   G_Log_Text = "英文：ASSY DWG No   『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   d_BpP = 0.0;
   d_BpH = 0.0;
   d_ApP = 0.0;
   d_ApH = 0.0;
   s_Text = P_HEADS_DATA[62].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApP = s_Text.ToDouble();
      } else {
         d_BpP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[61].Trim();   // Ｂ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApH = s_Text.ToDouble();
      } else {
         d_BpH = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[93].Trim();   // Ａ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpP = s_Text.ToDouble();
      } else {
         d_ApP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[92].Trim();   // Ａ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpH = s_Text.ToDouble();
      } else {
         d_ApH = s_Text.ToDouble();
      }
   }

   if( d_BpP > 0.0 || d_ApP > 0.0) {         // Ｂ試験圧力＞０又はＡ試験圧力＞０
      if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
         // 両方の条件が成立する場合は両方出力する
         P_Text = FormatFloat("#,0.00",d_BpP) + "/" + FormatFloat("#,0.00",d_BpH);
         Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs1, P_Text );
         G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP) + "/" + FormatFloat("#,0.00",d_ApH);
         Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs2, P_Text );
         G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);

         Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "P/H" );
      } else {
         P_Text = FormatFloat("#,0.00",d_BpP);
         Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs1, P_Text );
         G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP);
         Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs2, P_Text );
         G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "P" );
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
      P_Text = FormatFloat("#,0.00",d_BpH);
      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs1, P_Text );
      G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      P_Text = FormatFloat("#,0.00",d_ApH);
      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs2, P_Text );
      G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "H" );
   } else {
      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs1, "-----" );
      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs2, "-----" );
      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, "" );
      G_ErrLog_Text = "英文：Bテスト圧力  該当する数値がありません。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }
/*
   //--- 圧力Ｐ１
   i_RecNo = 827;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( AllFloatChk(P_Text.c_str(),0) == 0 )
   {
      d_wrk = StrToDblDef(P_Text);
      if( d_wrk == 0.0 ) {  // ０？
         P_Text = "-----";
      } else {
         P_Text = FormatFloat("#,0.00",d_wrk);
      }
      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs1, P_Text );
      G_Log_Text = "英文：P1             RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "英文：P1             RecNo:" + IntToStr(i_RecNo) + "  不正な値『" + P_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //--- 圧力Ｐ２
   i_RecNo = 832;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( AllFloatChk(P_Text.c_str(),0) == 0 )
   {
      d_wrk = StrToDblDef(P_Text);
      if( d_wrk == 0.0 ) {  // ０？
         P_Text = "-----";
      } else {
         P_Text = FormatFloat("#,0.00",d_wrk);
      }
      Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Prs2, P_Text );
      G_Log_Text = "英文：P2             RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "英文：P2             RecNo:" + IntToStr(i_RecNo) + "  不正な値『" + P_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //--- 試験媒体
   //i_RecNo = 9999;
   //P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( P_Text == "1" ) {
      P_Text = "水圧";
   } else {
      P_Text = "水圧";  // "気圧"？@@ 分岐方法が不明
   }
   Excel_Cell_DataSet( i_DatNum+CDatYE2, xE2Media, P_Text );
   G_Log_Text = "英文：Test Medium   『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
*/

   //***************************
   //***  ﾃﾞｰﾀ ５件分までの残りｾﾙを空白に
   //***************************
   for( i=i_DatNum+1 ; i<CDatAllNum ; i++ )
   {
      //--- 型式
      Excel_Cell_DataSet( i+CDatYE1, xE1Model, "" );
      //--- 基数
      Excel_Cell_DataSet( i+CDatYE1, xE1Qanty, "" );
      //--- 製造番号
      Excel_Cell_DataSet( i+CDatYE1, xE1MFGNo, "" );
      //--- ITEM NO
      Excel_Cell_DataSet( i+CDatYE1, xE1ItemNo, "" );
      //--- ｴﾚﾒﾝﾄ構成図
      Excel_Cell_DataSet( i+CDatYE1, xE1EleNo, "" );
      //--- 組立図
      Excel_Cell_DataSet( i+CDatYE1, xE1ASSNo, "" );
      //--- 型式(下段)
      Excel_Cell_DataSet( i+CDatYE2, xE2Model, "" );
      //--- ITEM NO(下段)
      Excel_Cell_DataSet( i+CDatYE2, xE2ItemNo, "" );
      //--- 圧力Ｐ１
      Excel_Cell_DataSet( i+CDatYE2, xE2Prs1, "" );
      //--- 圧力Ｐ２
      Excel_Cell_DataSet( i+CDatYE2, xE2Prs2, "" );
      //--- 試験媒体
      Excel_Cell_DataSet( i+CDatYE2, xE2Media, "" );
   }


   //***********************
   //***  材料検査欄
   //***********************
   //--- ﾌﾟﾚｰﾄ
   i_RecNo = 433;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   i_Mil = 0;
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "PLATE" : "";
   Excel_Cell_DataSet( yE3M1, CDatXE3, P_Text );
   G_Log_Text = "英文：ﾌﾟﾚｰﾄ          RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾌﾚｰﾑ
   i_RecNo = 434;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "FRAME" : "";
   Excel_Cell_DataSet( yE3M2, CDatXE3, P_Text );
   G_Log_Text = "英文：ﾌﾚｰﾑ           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 締付ﾎﾞﾙﾄﾅｯﾄ
   i_RecNo = 435;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   P_Text = ( P_Text == "1" ) ? "TIGHTENING BOLT&NUT" : "";
   Excel_Cell_DataSet( yE3M3, CDatXE3, P_Text );
   G_Log_Text = "英文：締付ﾎﾞﾙﾄﾅｯﾄ    RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)
   i_RecNo = 436;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "NOZZLE(PIPE)" : "";
   Excel_Cell_DataSet( yE3M4, CDatXE3, P_Text );
   G_Log_Text = "英文：ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)    RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾉｽﾞﾙ一式
   i_RecNo = 437;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   // 2019.02.22 書式変更_S
   if( P_Text == "1") {
      i_Mil++;
   }
   // 2019.02.22 書式変更_E
   P_Text = ( P_Text == "1" ) ? "NOZZLE(1SET)" : "";
   Excel_Cell_DataSet( yE3M5, CDatXE3, P_Text );
   G_Log_Text = "英文：ﾉｽﾞﾙ(一式)     RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2019.02.22 書式変更_S
   //--- ﾐﾙｼｰﾄ
   if( i_Mil > 0) {
      P_Text = "Mill Certificate Review:";
      Excel_Cell_DataSet( 92, 2, P_Text );
      G_Log_Text = "英文：ﾐﾙｼｰﾄ          『" + P_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   // 2019.02.22 書式変更_E


   //***********************
   //***  ver.欄
   //***********************
   i_RecNo = 1267;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   //@@ 現,1291:STAN区分 無し
   if( P_HEADS_DATA[302].TrimRight() == "1" ) // STAN区分あり？
   {
       P_Text = P_Text + "S";
   }
   // 2017.10.06 短納期対応_S
   else if (P_HEADS_DATA[302].TrimRight() == "2") // 短納期対応
   {
       P_Text = P_Text + "T";
   }
   // 2017.10.06 短納期対応_E
   // 2019.02.22 バーコード追加_S
   //Excel_Cell_DataSet( 49, 12, P_Text );
   //Excel_Cell_DataSet( 106, 12, P_Text );
   Excel_Cell_DataSet( 47, 11, P_Text );
   Excel_Cell_DataSet( 107, 11, P_Text );
    // 2019.02.22 バーコード追加_E

   G_Log_Text = "英文：HEADS VER      RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));
   // 2020.04.08 バーコード変更_E

   // 2019.02.22 バーコード追加_S
   //***************************
   //***  英文：バーコード作番
   //***************************
   P_Text = G_SAKUBAN;
   // 2020.04.08 バーコード変更_S
   //Excel_Cell_DataSet(10, 20, P_Text);
   Excel_Cell_DataSet(17, 2, P_Text);
   // 2020.04.08 バーコード変更_E
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：訂正
   //***************************
   i_RecNo = 319;
   P_Text = P_HEADS_DATA[i_RecNo];
   // 2020.04.08 バーコード変更_S
   //Excel_Cell_DataSet(14, 20, P_Text);
   Excel_Cell_DataSet(21, 2, P_Text);
   // 2020.04.08 バーコード変更_E
   G_Log_Text = "英文：訂正            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2019.02.22 バーコード追加_E

   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));
   // 2020.04.08 バーコード変更_E


   return ret;
}

//---------------------------------------------------------------------------
// 日本語関数名： 検査要領書管理ﾃｰﾌﾞﾙ 追加更新
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 追加結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TYouryosyo::Data_Insert(void)
{
   bool       ret=true;             // 戻り値
   int        i_RecNo;              // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   double     d_wrk;                // 数値変換用
   AnsiString A_SQL;                // SQL文
   double     d_BpP;            // B試験圧力
   double     d_BpH;            // B試験水圧
   double     d_ApP;            // A試験圧力
   double     d_ApH;            // A試験水力
   AnsiString s_Text;           // ワーク文字列

   //***************************
   //***  指定のﾃﾞｰﾀを設定
   //***************************
   //--- 型式
   i_RecNo = 33;
   if( P_HEADS_DATA[1057].TrimRight() == "1" ) {    // 改造？
      Pb.ModelCode = P_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      Pb.ModelCode = P_HEADS_DATA[i_RecNo].TrimRight();
   }

   //--- 基数
   i_RecNo = 39;
   Pb.Kisuu = P_HEADS_DATA[i_RecNo].TrimRight();

   //--- 納期
   //i_RecNo = 12;
   i_RecNo = 411;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   //P_Noki = P_Text;             // 納期
   P_Noki = P_Text.SubString(1,4) + "/" + P_Text.SubString(6,2) + "/" + P_Text.SubString(9,2);

   //--- 製造番号
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //Pb.Seiban = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   Pb.Seiban = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   Pb.Seiban = Pb.Seiban + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------

   //--- 機器番号(ITEM NO.)
   i_RecNo = 27;
   if (P_HEADS_DATA[i_RecNo].TrimRight() == ""){
      Pb.ItemNo = " ";
   } else {
      Pb.ItemNo = P_HEADS_DATA[i_RecNo].TrimRight();
   }

   //--- ｴﾚﾒﾝﾄ構成図
   //2023/4/21 製造番号桁数追加対応 Mod
   //Pb.ElementDno  = "A" + G_SAKUBAN;
   Pb.ElementDno  = "A" + GetFileSeizouBangou(G_SAKUBAN);
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   Pb.ElementDno = Pb.ElementDno + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------

   //--- 組立図
   //2023/4/21 製造番号桁数追加対応 Mod
   //Pb.AssembleDno = "B" + G_SAKUBAN;
   Pb.AssembleDno = "B" + GetFileSeizouBangou(G_SAKUBAN);
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   Pb.AssembleDno = Pb.AssembleDno + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------

   d_BpP = 0.0;
   d_BpH = 0.0;
   d_ApP = 0.0;
   d_ApH = 0.0;
   s_Text = P_HEADS_DATA[62].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApP = s_Text.ToDouble();
      } else {
         d_BpP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[61].Trim();   // Ｂ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApH = s_Text.ToDouble();
      } else {
         d_BpH = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[93].Trim();   // Ａ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpP = s_Text.ToDouble();
      } else {
         d_ApP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[92].Trim();   // Ａ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpH = s_Text.ToDouble();
      } else {
         d_ApH = s_Text.ToDouble();
      }
   }

   if( d_BpP > 0.0 || d_ApP > 0.0) {         // Ｂ試験圧力＞０又はＡ試験圧力＞０
      if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
         // 両方の条件が成立する場合は両方出力する
         P_Text = FormatFloat("#,0.00",d_BpP) + "/" + FormatFloat("#,0.00",d_BpH);
         Pb.Pres_p1     = P_Text;
         G_Log_Text = "Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP) + "/" + FormatFloat("#,0.00",d_ApH);
         Pb.Pres_p2     = P_Text;
         G_Log_Text = "Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);
         if (P_HEADS_DATA[1].TrimRight() == "0") {      // 和文
            Pb.TestMedia   = "空圧/水圧";
         } else {                                       // 英文
            Pb.TestMedia   = "P/H";
         }
      } else {
         P_Text = FormatFloat("#,0.00",d_BpP);
         Pb.Pres_p1     = P_Text;
         G_Log_Text = "Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP);
         Pb.Pres_p2     = P_Text;
         G_Log_Text = "Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         if (P_HEADS_DATA[1].TrimRight() == "0") {      // 和文
            Pb.TestMedia   = "空圧";
         } else {                                       // 英文
            Pb.TestMedia   = "P";
         }
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
      P_Text = FormatFloat("#,0.00",d_BpH);
      Pb.Pres_p1     = P_Text;
      G_Log_Text = "Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      P_Text = FormatFloat("#,0.00",d_ApH);
      Pb.Pres_p2     = P_Text;
      G_Log_Text = "Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      if (P_HEADS_DATA[1].TrimRight() == "0") {      // 和文
         Pb.TestMedia   = "水圧";
      } else {                                       // 英文
         Pb.TestMedia   = "H";
      }
   } else {
      Pb.Pres_p1     = "-----";
      Pb.Pres_p2     = "-----";
      Pb.TestMedia   = "";
      G_ErrLog_Text = "Bテスト圧力  該当する数値がありません。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }
/*
   //--- 圧力Ｐ１
   i_RecNo = 827;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( P_Text.IsEmpty() )
   {
      P_Text = "0";
   }
   if( AllFloatChk(P_Text.c_str(),0) == 0 )
   {
      d_wrk = StrToDblDef(P_Text);
      if( d_wrk == 0.0 ) {  // ０？
         P_Text = "-----";
      } else {
         P_Text = FormatFloat("#,0.00",d_wrk);
      }
   } else {
      G_ErrLog_Text = "圧力Ｐ１      RecNo:" + IntToStr(i_RecNo) + "  不正な値『" + P_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }
   Pb.Pres_p1     = P_Text;

   //--- 圧力Ｐ２
   i_RecNo = 832;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( P_Text.IsEmpty() )
   {
      P_Text = "0";
   }
   if( AllFloatChk(P_Text.c_str(),0) == 0 )
   {
      d_wrk = StrToDblDef(P_Text);
      if( d_wrk == 0.0 ) {  // ０？
         P_Text = "-----";
      } else {
         P_Text = FormatFloat("#,0.00",d_wrk);
      }
   } else {
      G_ErrLog_Text = "圧力Ｐ２      RecNo:" + IntToStr(i_RecNo) + "  不正な値『" + P_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }
   Pb.Pres_p2     = P_Text;

   //--- 試験媒体
   //i_RecNo = 9999;
   //P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if( P_Text == "1" ) {
      P_Text = "水圧";
   } else {
      P_Text = "水圧";  // "気圧"？@@ 分岐方法が不明
   }
   Pb.TestMedia   = P_Text;
*/
   //--- 出図要求日時
   //i_RecNo = 12;
   //P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   //Pb.ReqDate     = P_Noki;
   Pb.ReqDate     = Now().FormatString("yyyy/mm/dd hh:nn:ss"); // 現在日時

   //******************************************
   // 主ｷｰがNULLの場合更新しない(02.08.10追加)
   //******************************************
   if( ( P_OkyakuCode != "\0" )&&( P_NonyusakiCode != "\0" )&&( P_Noki != "\0" ) )
   {
      //***************************
      //***  ﾃﾞｰﾀﾍﾞｰｽに追加
      //***************************
      A_SQL = "";
      A_SQL = A_SQL + "INSERT INTO HD_INSPCTL";
      A_SQL = A_SQL + "(";
      A_SQL = A_SQL + "OKYAKUSAMA_CODE, NONYUSAKI_CODE, NOKI_DATE, ";
      A_SQL = A_SQL + "MODEL_CODE, KISUU, SEIBAN, ITEMNO, ELEMENT_DNO, ASSEMBLE_DNO, ";
      A_SQL = A_SQL + "PRES_P1, PRES_P2, TEST_MEDIA, REQ_DATE";
      A_SQL = A_SQL + ")";
      A_SQL = A_SQL + " VALUES(";
      A_SQL = A_SQL + "'" + P_OkyakuCode    + "', ";
      A_SQL = A_SQL + "'" + P_NonyusakiCode + "', ";
      A_SQL = A_SQL + "'" + P_Noki          + "', ";
      A_SQL = A_SQL + "'" + Pb.ModelCode    + "', ";
      A_SQL = A_SQL + "'" + Pb.Kisuu        + "', ";
      A_SQL = A_SQL + "'" + Pb.Seiban       + "', ";
      A_SQL = A_SQL + "'" + Pb.ItemNo       + "', ";
      A_SQL = A_SQL + "'" + Pb.ElementDno   + "', ";
      A_SQL = A_SQL + "'" + Pb.AssembleDno  + "', ";
      A_SQL = A_SQL + "'" + Pb.Pres_p1      + "', ";
      A_SQL = A_SQL + "'" + Pb.Pres_p2      + "', ";
      A_SQL = A_SQL + "'" + Pb.TestMedia    + "', ";
      A_SQL = A_SQL + "'" + Pb.ReqDate      + "')";
      A_SQL = A_SQL + ";";

      try
      {
         if( DataModule1->Database_MAT->InTransaction )
         {
            DataModule1->Database_MAT->Commit();
         }
         DataModule1->Database_MAT->StartTransaction();

         P_Query->Close();
         P_Query->SQL->Clear();
         P_Query->SQL->Add(A_SQL);
         if( !P_Query->Prepared )
         {
            P_Query->Prepare();
         }
         P_Query->ExecSQL();

         DataModule1->Database_MAT->Commit();

         G_Log_Text = "検査要領書管理に     『" + A_SQL + "』をセット。";
         Write_Log(G_Log_Text);
      }
      catch(EDatabaseError &Error)
      {
         DataModule1->Database_MAT->Rollback();
         P_Query->Close();

         G_ErrLog_Text = "--検査要領書管理ﾃｰﾌﾞﾙの追加に失敗しました！！--" ;
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
         return false;
      }
   }
   else
      ret = false;

   return ret;
}

//---------------------------------------------------------------------------
// 日本語関数名： 検査要領書 データ 重複ﾁｪｯｸ
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TYouryosyo::Data_Chk(void)
{
   bool       ret;                  // 戻り値
   AnsiString A_SQL;                // SQL文

   //--- 製造番号
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //P_Seiban = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   P_Seiban = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E
      
   //---------------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ文字列追加
   P_Seiban = P_Seiban + m_pcHeadsData->GetSpecCode();
   // 2007.10.15
   //---------------------------------------------

   // お客様ｺｰﾄﾞ
   P_OkyakuCode = P_HEADS_DATA[1064].TrimRight();

   // 納入先ｺｰﾄﾞ
   P_NonyusakiCode = P_HEADS_DATA[1066].TrimRight();

   //--- 納期
   P_Text = P_HEADS_DATA[411].TrimRight();
   P_Noki = P_Text.SubString(1,4) + "/" + P_Text.SubString(6,2) + "/" + P_Text.SubString(9,2);
   try
   {
      StrToDate(P_Noki);
   }
   catch(EConvertError &E)
   {
      G_ErrLog_Text = "納期             RecNo:" + IntToStr(411) + "  不正な値『" + P_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      return false;
   }


   //***************************
   //***  登録ﾃﾞｰﾀ 存在ﾁｪｯｸ
   //***************************
   A_SQL = "";
   A_SQL = A_SQL + "SELECT * FROM HD_INSPCTL";
   A_SQL = A_SQL + " WHERE OKYAKUSAMA_CODE = '" + P_OkyakuCode + "'";
   A_SQL = A_SQL + " AND   NONYUSAKI_CODE = '" + P_NonyusakiCode + "'";
   A_SQL = A_SQL + " AND   CDATE(NOKI_DATE) = CDATE('" + P_Noki + "')";
   A_SQL = A_SQL + " AND   SEIBAN = '" + P_Seiban + "'";
   A_SQL = A_SQL + ";";

   G_Log_Text = "検査要領書管理ﾃｰﾌﾞﾙの重複ﾁｪｯｸ中です。";
   Write_Log(G_Log_Text);

   try
   {
      P_Query->Close();
      P_Query->SQL->Clear();
      P_Query->SQL->Add(A_SQL);
      P_Query->Open();
      P_Query->First();
      if( P_Query->Eof )    // 該当ﾃﾞｰﾀが存在しない？
      {
         ret = true;            // 正常:登録可能
      }
      else
      {
         A_SQL = "";
         A_SQL = A_SQL + "DELETE FROM HD_INSPCTL";
         A_SQL = A_SQL + " WHERE OKYAKUSAMA_CODE = '" + P_OkyakuCode + "'";
         A_SQL = A_SQL + " AND   NONYUSAKI_CODE = '" + P_NonyusakiCode + "'";
         A_SQL = A_SQL + " AND   CDATE(NOKI_DATE) = CDATE('" + P_Noki + "')";
         A_SQL = A_SQL + " AND   SEIBAN = '" + P_Seiban + "'";
         A_SQL = A_SQL + ";";
      P_Query->Close();
      P_Query->SQL->Clear();
      P_Query->SQL->Add(A_SQL);
      P_Query->ExecSQL();
//      P_Query->First();
//         G_ErrLog_Text = "--検査要領書管理ﾃｰﾌﾞﾙに既に登録済みです--" ;
//         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
//         ret = false;

            //--------------------------------------
            // 2007.10.25 削除後に登録するよう修正
            ret = true;
            // 2007.10.25
            //--------------------------------------
      }
   }
   catch(EDatabaseError &Error)
   {
      G_ErrLog_Text = "--検査要領書管理ﾃｰﾌﾞﾙの参照に失敗しました！！--" ;
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      ret = false;
   }

   return ret;
}

//-----------------------------------------------------------------------------
// 日本語関数名：ｴｸｾﾙｼｰﾄﾌﾟﾘﾝﾀPDF印刷処理
// 概要        ：ｴｸｾﾙｼｰﾄのPDF印刷処理を行う
// パラメタ    ：Variant &vSheet ｴｸｾﾙｼｰﾄ
// 戻り値      ：-1:PDFﾌﾟﾘﾝﾀなし, 0:出力済み, 1:出力Excelｴﾗｰ
// 機能説明    ：PDFﾌﾟﾘﾝﾀをｴｸｾﾙに設定し、印刷する
// 備考        ：未完成・動作未確認 '02.06.07
//-----------------------------------------------------------------------------
short __fastcall ExcelSheetPDFPrint(Variant &vSheet)
{
    short   ret;
    int     i;
    int     strpos;
    char    buf[256];
    char    pr_name[128];
    char    pr_drvr[128];
    char    pr_port[128];
    AnsiString  pr_now, pr_str;
    Variant exPrinter=Unassigned;
    TPrinter    *printer=Printer();

    BYTE            *p;
    PRINTER_INFO_2  *pr_info2;
    // 2021.06.18 FireDACへの移行 MOD_S
    //HDC             handle;
    HANDLE             handle;
    // 2021.06.18 FireDACへの移行 MOD_E
    DWORD prlbl=2;
    DWORD prsize;
    DWORD prsizeneed;


    ret = -1;

    memset( pr_name, 0x00, sizeof(pr_name) );
    memset( pr_drvr, 0x00, sizeof(pr_drvr) );
    memset( pr_port, 0x00, sizeof(pr_port) );
    memset( buf, 0x00, sizeof(buf) );

    prsizeneed = sizeof(PRINTER_INFO_2);
    prsize = prsizeneed + 2048;
    p = new BYTE[prsize];
    pr_info2 = (PRINTER_INFO_2*)p;

    // PDF プリンタ 検索
    for( i=0 ; i<printer->Printers->Count ; i++ )
    {
        pr_str = printer->Printers->Strings[i];
        pr_now = pr_str.Trim().UpperCase();   // 大文字へ
        strpos = pr_now.Pos("PDF");
        if( strpos <= 0 )
        {
            strpos = pr_now.Pos("ACROBAT");
        }
        if( strpos > 0 )   // 対象ﾌﾟﾘﾝﾀ？
        {
            if( OpenPrinter(pr_str.c_str(),&handle,NULL) )  // ﾌﾟﾘﾝﾀHandle取得
            {
                memset( p, 0x00, prsize );
                prsizeneed = 0;
                strpos = GetPrinter(handle,prlbl,p,prsize,&prsizeneed); // ﾌﾟﾘﾝﾀ名等 取得
                if( !strpos )
                {
                    //if( GetLastError() == ERROR_INSUFFICIENT_BUFFER )   // 122:ﾊﾞｯﾌｧｻｲｽﾞ不足？
                    {
                        prsize = prsizeneed + 2;
                        if( p )
                        {
                            delete[] p;
                        }
                        p = new BYTE[prsize];
                        pr_info2 = (PRINTER_INFO_2*)p;
                        memset( p, 0x00, prsize );

                          //strpos = EnumPorts(NULL,1,p,prsize,&prsizeneed,(ULONG*)&i);

                          strpos = GetPrinter(handle,prlbl,p,prsize,&prsizeneed); // ﾌﾟﾘﾝﾀ名等 取得
                    }
                }
                if( strpos )
                {
                    strncpy( pr_name, pr_info2->pPrinterName, sizeof(pr_name) );
                    strncpy( pr_drvr, pr_info2->pDriverName, sizeof(pr_drvr) );
                    strncpy( pr_port, pr_info2->pPortName, sizeof(pr_port) );
                }
                ClosePrinter(handle);     // ﾌﾟﾘﾝﾀHandle閉じる
            }
            break;
        }
    }

    if( p )
    {
        delete[] p;
    }

    if( (strlen(pr_name)>0) )   // ﾌﾟﾘﾝﾀ名 有効？
    {
        // EXCELに渡すﾌﾟﾘﾝﾀ名
        if( (strlen(pr_port)<=0) )   // ﾎﾟｰﾄ名 無効？
        {
            strcpy( pr_port, "FILE" ); // FILEに
        }
        sprintf( buf, "%s on %s", pr_name, pr_port );
        if( exApplication.Type() == varDispatch )   // Excel起動中？
        {
            exPrinter = exApplication.OlePropertyGet( "ActivePrinter" );
            pr_now = exPrinter.bstrVal;

            if( strncmp(pr_name,pr_now.c_str(),strlen(pr_name)) )   // 別ﾌﾟﾘﾝﾀ？
            {
                try
                {
                    exApplication.OlePropertySet( "ActivePrinter", buf );
                    vSheet.OleFunction( "PrintOut" );   // 印刷
                    exApplication.OlePropertySet( "ActivePrinter", exPrinter );
                }
                catch(...)
                {
                    return 1;
                }
            }
            else
            {
                try
                {
                    vSheet.OleFunction( "PrintOut" );   // 印刷
                }
                catch(...)
                {
                    return 1;
                }
            }
            exPrinter.Clear();
            ret = 0;
        }
    }

    return ret;
}

//---------------------------------------------------------------------------
// 日本語関数名： 検査要領書 Excelデータセット(中文)
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2011.02.09 中国語対応のため追加
//---------------------------------------------------------------------------
bool __fastcall TYouryosyo::Data_Set_C(void)
{
   bool       ret=true;             // 戻り値
   int        i_RecNo;              // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   AnsiString A_SQL;                // SQL文
   AnsiString A_NokiYear;           // 納期(年)
   AnsiString A_SubStr;             // 作業用文字列
   double     d_wrk;                // 数値変換用
   int        i_wrk;                // 数値変換用
   int        i_DatCnt;             // ﾃﾞｰﾀ 登録件数(MAX4)
   int        i_DatNum;             // ﾃﾞｰﾀ 位置(0～4:更新位置)
   int        i;                    // ｶｳﾝﾀ
   double     d_BpP;            // B試験圧力
   double     d_BpH;            // B試験水圧
   double     d_ApP;            // A試験圧力
   double     d_ApH;            // A試験水力
   AnsiString s_Text;           // ワーク文字列
   // 2022.04.12 耐圧保持時間指定追加_S
   int w_count1;
   int w_count2;
   int w_count3;
   int w_count4;
   int w_count5;
   int w_count6;
   // 2022.04.12 耐圧保持時間指定追加_E


   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "和文");
       //Windows 2008Server 移植対応
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));

   //***************************
   //***  見出し
   //***************************
   //--- 御注文主
   i_RecNo = 1063;

   P_Text = "/U+20080/U+/U+23478/U+　　　" +   P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( 3, 1, P_Text );
   G_Log_Text = "中文：御注文主       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_OkyakuName = P_Text;         // お客様名
   // お客様ｺｰﾄﾞ
   P_OkyakuCode = P_HEADS_DATA[1064].TrimRight();

   //--- 納入先
   i_RecNo = 1065;
   P_Text = "/U+23458/U+/U+25143/U+　　　" + P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( 5, 1, P_Text );
   G_Log_Text = "中文：納入先         RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   P_NonyusakiName = P_Text;      // 納入先名
   // 納入先ｺｰﾄﾞ
   P_NonyusakiCode = P_HEADS_DATA[1066].TrimRight();

   //--- 納期
   i_RecNo = 411;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   P_Noki = P_Text.SubString(1,4) + "/" + P_Text.SubString(6,2) + "/" + P_Text.SubString(9,2);
   Write_Log(G_Log_Text);
   P_NokiDate = P_Text;           // 納期(編集文字列)

   // 2022.04.12 耐圧保持時間指定追加_S
   //***************************
   //***  耐圧保持時間
   //***************************
   if (Tokki_Code("2C", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間標準
      s_Text = "保持/U+26102/U+/U+38388/U+/U+20026/U+20分/U+38047/U+";
   }
   else if (Tokki_Code("2D", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間30分
      s_Text = "保持/U+26102/U+/U+38388/U+/U+20026/U+30分/U+38047/U+";
   }
   else if (Tokki_Code("2E", P_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間指定あり
      s_Text = Tokki_Zen("2E", P_HEADS_DATA, 1500);
      w_count1 = s_Text.AnsiPos("：");
      w_count2 = s_Text.AnsiPos(":");

      if (w_count1){
         // "："が存在　
         s_Text = s_Text.Delete(1,w_count1+1);
      } else if (w_count2){
         // ":"が存在
         s_Text = s_Text.Delete(1,w_count2);
      } else {
         // "：",":"が存在しない
         s_Text = "";
      }

      w_count1 = s_Text.AnsiPos("分");
      w_count2 = s_Text.AnsiPos("小");
      w_count3 = s_Text.AnsiPos("m");
      w_count4 = s_Text.AnsiPos("M");
      w_count5 = s_Text.AnsiPos("h");
      w_count6 = s_Text.AnsiPos("H");

      if (w_count1){
         // "分"が存在　
         s_Text = s_Text.SetLength(w_count1-1);
      } else if (w_count2){
         // "小"が存在
         s_Text = "";
      } else if (w_count3){
         // "m"が存在　
         s_Text = s_Text.SetLength(w_count3-1);
      } else if (w_count4){
         // "M"が存在
         s_Text = s_Text.SetLength(w_count4-1);
      } else if (w_count5){
         // "h"が存在
         s_Text = "";
      } else if (w_count6){
         // "H"が存在
         s_Text = "";
      } else if (s_Text == ""){
          // 特記が存在しない
      } else {
          // "分","m","M"が存在しない
      }

      if (s_Text == ""){
          // 特記が存在しない
         s_Text = "※　ERROR";
      } else {
         s_Text = s_Text.TrimRight();
         s_Text = ZenToHan(s_Text);
         s_Text = "保持/U+26102/U+/U+38388/U+/U+20026/U+" + s_Text + "分/U+38047/U+";
      }

   }
   else {
      s_Text = "保持/U+26102/U+/U+38388/U+/U+20026/U+20分/U+38047/U+";
   }

   Excel_Cell_DataSet(32, 1, s_Text);
   // 2022.04.12 耐圧保持時間指定追加_E

   //***************************
   //***  該当ﾃﾞｰﾀ 最大４件まで読込み、ｾﾙに設定
   //***************************
   A_SQL = "";
   A_SQL = A_SQL + "SELECT * FROM HD_INSPCTL";
   A_SQL = A_SQL + " WHERE OKYAKUSAMA_CODE = '" + P_OkyakuCode + "'";
   A_SQL = A_SQL + " AND   NONYUSAKI_CODE = '" + P_NonyusakiCode + "'";

   if( !P_Noki.IsEmpty() )
      A_SQL = A_SQL + " AND   NOKI_DATE = CDATE('" + P_Noki + "')";

   A_SQL = A_SQL + ";";

   i_DatCnt = 0;   // ﾃﾞｰﾀ 件数(MAX4)
   i_DatNum = 0;   // ﾃﾞｰﾀ 位置(0～4:更新位置)

   try
   {
      P_Query->Close();
      P_Query->SQL->Clear();
      P_Query->SQL->Add(A_SQL);
      P_Query->Open();
      P_Query->First();
      while( !P_Query->Eof )   // ﾃﾞｰﾀが存在する間
      {
         i_DatCnt++;           // 数 更新
         if( i_DatCnt < CDatMaxNum )   // 数 範囲内？
         {
            // ﾚｺｰﾄﾞﾃﾞｰﾀ 取得
            Pb.ModelCode   = P_Query->FieldByName("MODEL_CODE")->AsString.TrimRight();     // 型式
            Pb.Kisuu       = P_Query->FieldByName("KISUU")->AsString.TrimRight();          // 基数
            Pb.Seiban      = P_Query->FieldByName("SEIBAN")->AsString.TrimRight();         // 製造番号
            Pb.ItemNo      = P_Query->FieldByName("ITEMNO")->AsString.TrimRight();         // ITEM NO.
            Pb.ElementDno  = P_Query->FieldByName("ELEMENT_DNO")->AsString.TrimRight();    // ｴﾚﾒﾝﾄ構成図
            Pb.AssembleDno = P_Query->FieldByName("ASSEMBLE_DNO")->AsString.TrimRight();   // 組立図
            Pb.Pres_p1     = P_Query->FieldByName("PRES_P1")->AsString.TrimRight();        // 圧力Ｐ１
            Pb.Pres_p2     = P_Query->FieldByName("PRES_P2")->AsString.TrimRight();        // 圧力Ｐ２
            Pb.TestMedia   = P_Query->FieldByName("TEST_MEDIA")->AsString.TrimRight();     // 試験媒体
            Pb.ReqDate     = P_Query->FieldByName("REQ_DATE")->AsString.TrimRight();       // 日時

            //2022/12/15 桁数追加対応 Mod_S
            //P_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
            P_Text = GetSeizouBangou(G_SAKUBAN);
            //2022/12/15 桁数追加対応 Mod_E            
           if ( Pb.Seiban.SubString(1, P_Text.Length()) != P_Text) {
               //--- 型式
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Model, Pb.ModelCode );
               G_Log_Text = "中文：型式              No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);
               //--- 型式(下段)
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Model, Pb.ModelCode );
               G_Log_Text = "中文：型式(下)          No:" + IntToStr(i_DatCnt) + " 『" + Pb.ModelCode + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 基数
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Qanty, Pb.Kisuu );
               G_Log_Text = "中文：基数              No:" + IntToStr(i_DatCnt) + " 『" + Pb.Kisuu + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 製造番号
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1MFGNo, Pb.Seiban );
               G_Log_Text = "中文：製造番号          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Seiban + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 機器番号(ITEM NO)
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ItemNo, Pb.ItemNo );
               G_Log_Text = "中文：機器番号          No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);
               //--- 機器番号(ITEM NO)(下段)
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2ItemNo, Pb.ItemNo );
               G_Log_Text = "中文：機器番号(下)      No:" + IntToStr(i_DatCnt) + " 『" + Pb.ItemNo + "』をセット。";
               Write_Log(G_Log_Text);

               //--- ｴﾚﾒﾝﾄ構成図
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1EleNo, Pb.ElementDno );
               G_Log_Text = "中文：ｴﾚﾒﾝﾄ構成図       No:" + IntToStr(i_DatCnt) + " 『" + Pb.ElementDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 組立図
               Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ASSNo, Pb.AssembleDno );
               G_Log_Text = "中文：組立図            No:" + IntToStr(i_DatCnt) + " 『" + Pb.AssembleDno + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ１
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, Pb.Pres_p1 );
               G_Log_Text = "中文：圧力Ｐ１          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p1 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 圧力Ｐ２
               Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, Pb.Pres_p2 );
               G_Log_Text = "中文：圧力Ｐ２          No:" + IntToStr(i_DatCnt) + " 『" + Pb.Pres_p2 + "』をセット。";
               Write_Log(G_Log_Text);

               //--- 試験媒体
               if ( Pb.TestMedia == "水圧" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "水/U+21387/U+" );
               } else if ( Pb.TestMedia == "空圧" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "气/U+21387/U+" );
               } else if ( Pb.TestMedia == "空圧/水圧" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "气/U+21387/U+/水/U+21387/U+" );
               } else if  ( Pb.TestMedia == "H" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "水/U+21387/U+" );
               } else if ( Pb.TestMedia == "P" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "气/U+21387/U+" );
               } else if ( Pb.TestMedia == "P/H" ) {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "气/U+21387/U+/水/U+21387/U+" );
               } else {
                  Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, Pb.TestMedia );
               }
               G_Log_Text = "中文：試験媒体          No:" + IntToStr(i_DatCnt) + " 『" + Pb.TestMedia + "』をセット。";
               Write_Log(G_Log_Text);


               i_DatNum++;       // 位置更新
            }
            P_Query->Next();  // 次へ
         }
         else
         {
            break; // ﾙｰﾌﾟ終了
         }
      }
      P_Query->Close();
   }
   catch(EDatabaseError &Error)
   {
      G_ErrLog_Text = "--検査要領書管理ﾃｰﾌﾞﾙの参照に失敗しました！！--" ;
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      ret = false;
      P_Query->Close();
   }

   if( i_DatCnt > CDatMaxNum )  // 最大数 超え？
   {
      // 最大数 超えerr
      G_ErrLog_Text = "和文：出力対象データが４件を超えました！！";
      Write_Error_Log(G_ErrLog_Text);       // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);     // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }


   //***************************
   //***  指定のﾃﾞｰﾀを ｾﾙに設定
   //***************************
   //--- 型式
   i_RecNo = 33;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Model, P_Text );
   G_Log_Text = "中文：型式           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //--- 型式(下段)
   Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Model, P_Text );
   G_Log_Text = "中文：型式(下)       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 基数
   i_RecNo = 39;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1Qanty, P_Text );
   G_Log_Text = "中文：基数           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 製造番号
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //P_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   P_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E   
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1MFGNo, P_Text );
   G_Log_Text = "中文：製造番号      『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 機器番号(ITEM NO.)
   i_RecNo = 27;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ItemNo, P_Text );
   G_Log_Text = "中文：機器番号       RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //--- 機器番号(ITEM NO.)(下段)
   Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2ItemNo, P_Text );
   G_Log_Text = "中文：機器番号(下)   RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ｴﾚﾒﾝﾄ構成図
   //2023/4/21 製造番号桁数追加対応 Mod
   //P_Text = "A" + G_SAKUBAN;
   P_Text = "A" + GetFileSeizouBangou(G_SAKUBAN);
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1EleNo, P_Text );
   G_Log_Text = "中文：ｴﾚﾒﾝﾄ構成図   『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 組立図
   //2023/4/21 製造番号桁数追加対応 Mod
   //P_Text = "B" + G_SAKUBAN;
   P_Text = "B" + GetFileSeizouBangou(G_SAKUBAN);
   P_Text = P_Text + m_pcHeadsData->GetSpecCode();
   Excel_Cell_DataSet( i_DatNum+CDatYJ1, xJ1ASSNo, P_Text );
   G_Log_Text = "中文：組立図        『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   d_BpP = 0.0;
   d_BpH = 0.0;
   d_ApP = 0.0;
   d_ApH = 0.0;
   s_Text = P_HEADS_DATA[62].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApP = s_Text.ToDouble();
      } else {
         d_BpP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[61].Trim();   // Ｂ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_ApH = s_Text.ToDouble();
      } else {
         d_BpH = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[93].Trim();   // Ａ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpP = s_Text.ToDouble();
      } else {
         d_ApP = s_Text.ToDouble();
      }
   }
   s_Text = P_HEADS_DATA[92].Trim();   // Ａ試験水圧
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( P_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // ＡＢ逆
         d_BpH = s_Text.ToDouble();
      } else {
         d_ApH = s_Text.ToDouble();
      }
   }

   if( d_BpP > 0.0 || d_ApP > 0.0) {         // Ｂ試験圧力＞０又はＡ試験圧力＞０
      if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
         // 両方の条件が成立する場合は両方出力する
         P_Text = FormatFloat("#,0.00",d_BpP) + "/" + FormatFloat("#,0.00",d_BpH);
         Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, P_Text );
         G_Log_Text = "中文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP) + "/" + FormatFloat("#,0.00",d_ApH);
         Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, P_Text );
         G_Log_Text = "中文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧/水圧)をセット。";
         Write_Log(G_Log_Text);

         Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "气/U+21387/U+/水/U+21387/U+" );
      } else {
         P_Text = FormatFloat("#,0.00",d_BpP);
         Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, P_Text );
         G_Log_Text = "中文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         P_Text = FormatFloat("#,0.00",d_ApP);
         Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, P_Text );
         G_Log_Text = "中文：Bテスト圧力      RecNo:62『" + P_Text + "』(空圧)をセット。";
         Write_Log(G_Log_Text);

         Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "气/U+21387/U+" );
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
      P_Text = FormatFloat("#,0.00",d_BpH);
      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, P_Text );
      G_Log_Text = "中文：Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      P_Text = FormatFloat("#,0.00",d_ApH);
      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, P_Text );
      G_Log_Text = "中文：Bテスト圧力      RecNo:62『" + P_Text + "』(水圧)をセット。";
      Write_Log(G_Log_Text);

      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "水/U+21387/U+" );
   } else {
      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs1, "-----" );
      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Prs2, "-----" );
      Excel_Cell_DataSet( i_DatNum+CDatYJ2, xJ2Media, "" );
      G_ErrLog_Text = "中文：Bテスト圧力  該当する数値がありません。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  ﾃﾞｰﾀ ５件分までの残りｾﾙを空白に
   //***************************
   for( i=i_DatNum+1 ; i<CDatAllNum ; i++ )
   {
      //--- 型式
      Excel_Cell_DataSet( i+CDatYJ1, xJ1Model, "" );
      //--- 基数
      Excel_Cell_DataSet( i+CDatYJ1, xJ1Qanty, "" );
      //--- 製造番号
      Excel_Cell_DataSet( i+CDatYJ1, xJ1MFGNo, "" );
      //--- ITEM NO
      Excel_Cell_DataSet( i+CDatYJ1, xJ1ItemNo, "" );
      //--- ｴﾚﾒﾝﾄ構成図
      Excel_Cell_DataSet( i+CDatYJ1, xJ1EleNo, "" );
      //--- 組立図
      Excel_Cell_DataSet( i+CDatYJ1, xJ1ASSNo, "" );
      //--- 型式(下段)
      Excel_Cell_DataSet( i+CDatYJ2, xJ2Model, "" );
      //--- ITEM NO(下段)
      Excel_Cell_DataSet( i+CDatYJ2, xJ2ItemNo, "" );
      //--- 圧力Ｐ１
      Excel_Cell_DataSet( i+CDatYJ2, xJ2Prs1, "" );
      //--- 圧力Ｐ２
      Excel_Cell_DataSet( i+CDatYJ2, xJ2Prs2, "" );
      //--- 試験媒体
      Excel_Cell_DataSet( i+CDatYJ2, xJ2Media, "" );
   }


   //***********************
   //***  材料検査欄
   //***********************
   //--- ﾌﾟﾚｰﾄ
   i_RecNo = 433;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   P_Text = ( P_Text == "1" ) ? "/U+25442/U+/U+28909/U+板" : "";
   Excel_Cell_DataSet( CDatYJ3, xE1M1, P_Text );
   G_Log_Text = "中文：ﾌﾟﾚｰﾄ          RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾌﾚｰﾑ
   i_RecNo = 434;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   P_Text = ( P_Text == "1" ) ? "机架" : "";
   Excel_Cell_DataSet( CDatYJ3, xE1M2, P_Text );
   G_Log_Text = "中文：ﾌﾚｰﾑ           RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- 締付ﾎﾞﾙﾄﾅｯﾄ
   i_RecNo = 435;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   P_Text = ( P_Text == "1" ) ? "/U+32039/U+固螺栓/U+183/U+螺帽" : "";
   Excel_Cell_DataSet( CDatYJ3, xE1M3, P_Text );
   G_Log_Text = "中文：締付ﾎﾞﾙﾄﾅｯﾄ    RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)
   i_RecNo = 436;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   P_Text = ( P_Text == "1" ) ? "管嘴（管）" : "";
   Excel_Cell_DataSet( CDatYJ3, xE1M4, P_Text );
   G_Log_Text = "中文：ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)    RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--- ﾉｽﾞﾙ一式
   i_RecNo = 437;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   P_Text = ( P_Text == "1" ) ? "管嘴（一套）" : "";
   Excel_Cell_DataSet( CDatYJ3, xE1M5, P_Text );
   G_Log_Text = "中文：ﾉｽﾞﾙ(一式)     RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //***********************
   //***  ver.欄
   //***********************
   i_RecNo = 1267;
   P_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   //@@ 現,1291:STAN区分 無し
   if( P_HEADS_DATA[302].TrimRight() == "1" ) // STAN区分あり？
   {
       P_Text = P_Text + "S";
   }
   // 2017.10.06 短納期対応_S
   else if (P_HEADS_DATA[302].TrimRight() == "2") // 短納期対応
   {
       P_Text = P_Text + "T";
   }
   // 2017.10.06 短納期対応_E
   // 2019.02.22 バーコード追加_S
   //Excel_Cell_DataSet( 54, 12, P_Text );
   Excel_Cell_DataSet( 54, 11, P_Text );
   // 2019.02.22 バーコード追加_E
   G_Log_Text = "中文：HEADS VER      RecNo:" + IntToStr(i_RecNo) + "『" + P_Text + "』をセット。";
   Write_Log(G_Log_Text);


   // 2019.02.22 バーコード追加_S
   //***************************
   //***  中文：バーコード作番
   //***************************
   P_Text = G_SAKUBAN;
   Excel_Cell_DataSet(10, 20, P_Text);
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  中文：訂正
   //***************************
   i_RecNo = 319;
   P_Text = P_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(14, 20, P_Text);
   G_Log_Text = "中文：訂正            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2019.02.22 バーコード追加_E


   return ret;
}


//---------------------------------------------------------------------------
// 日本語関数名： 特記コード有無確認
// 概  要      ：
// 引  数      ： AnsiString Key   ： 特記ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2022.04.12追加
//---------------------------------------------------------------------------
bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu)
{

   bool bSearch;

   bSearch = false;

   if ( s_HEADS_DATA[451].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[454].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[457].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[460].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[463].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[466].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[469].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[472].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[475].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[478].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[481].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[484].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[487].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[490].TrimRight() == Key ) {
      bSearch = true;
   }

   if( bSearch ){
      return(true);
   } else {
      return(false);
   }

}


//---------------------------------------------------------------------------
// 日本語関数名： 特記項目（全角）取得
// 概  要      ：
// 引  数      ： AnsiString Key   ： 特記ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2022.04.12追加
//---------------------------------------------------------------------------
AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu)
{

   AnsiString s_RetText;

   s_RetText = "";

   if ( s_HEADS_DATA[451].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[453].TrimRight();
   }
   if ( s_HEADS_DATA[454].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[456].TrimRight();
   }
   if ( s_HEADS_DATA[457].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[459].TrimRight();
   }
   if ( s_HEADS_DATA[460].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[462].TrimRight();
   }
   if ( s_HEADS_DATA[463].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[465].TrimRight();
   }
   if ( s_HEADS_DATA[466].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[468].TrimRight();
   }
   if ( s_HEADS_DATA[469].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[471].TrimRight();
   }
   if ( s_HEADS_DATA[472].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[474].TrimRight();
   }
   if ( s_HEADS_DATA[475].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[477].TrimRight();
   }
   if ( s_HEADS_DATA[478].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[480].TrimRight();
   }
   if ( s_HEADS_DATA[481].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[483].TrimRight();
   }
   if ( s_HEADS_DATA[484].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[486].TrimRight();
   }
   if ( s_HEADS_DATA[487].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[489].TrimRight();
   }
   if ( s_HEADS_DATA[490].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[492].TrimRight();
   }

   return(s_RetText);

}

//---------------------------------------------------------------------------




