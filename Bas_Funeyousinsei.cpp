//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：舶用申請書作成モジュール
//　 作 成 者：S.Ogita
//　 作 成 日：2007.03
//　 更 新 日：2007.10.10 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "SINSEICom.h"  // 申請書共通関数定義用

#include "Bas_ShomeiBV.h"
#include "Bas_ShomeiLR.h"
#include "Bas_ShomeiABS.h"
#include "Bas_ShomeiNK.h"
#include "Bas_ShinseiNK.h"
#include "Bas_ShinseiNonNK.h"
#include "Bas_ShinseiFaxNK.h"
#include "Bas_ShinseiFaxNonNK.h"

#include "Bas_Funeyousinsei.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書作成メイン
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Funeyousinsei(HeadsData* pcHeadsData)
{
   AnsiString strStdCode;
   int nRecNo;
   // 2011.02.09 中国語対応
   AnsiString s_Text;
   // *********************
   // 2016.08.02 検査成績諸舶用
   int i_PosM;                   // 舶用フラグ ("0":舶用以外、"1"以上:舶用)
   // *************************

   //------------------------------------
   // 2007.10.10 ﾐﾗｰ仕様対応
   // 元仕様以外は処理不要(正常終了)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   // 規格コードの取得
   nRecNo = 29;
   strStdCode = G_HEADS_DATA[nRecNo].Trim();

   // 2016.08.02 検査成績諸舶用
   i_PosM = 0;
   nRecNo = 36;
   i_PosM = G_HEADS_DATA[nRecNo].Pos("M");
   // *************************

   //******************************
   //***テンプレート/作成Excelファイルパスの生成
   //******************************
   // 2011.02.09 中国語対応
   //s_Tmplate_Pass = G_Template_Dir + "検査成績書.xls";                  // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   nRecNo = 851;
   s_Text = G_HEADS_DATA[nRecNo].TrimRight();
   if ( s_Text == "1" ) {
      // 2016.08.02 検査成績諸舶用
      //// 2016.06.08 検査成績諸舶用
      ////s_Tmplate_Pass = G_Template_Dir + "検査成績書C.xls";              // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      //s_Tmplate_Pass = G_Template_Dir + "検査成績書舶用C.xls";          // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      //// *************************
      if (i_PosM >= 1){
         s_Tmplate_Pass = G_Template_Dir + "検査成績書舶用C.xls";    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else {
         s_Tmplate_Pass = G_Template_Dir + "検査成績書C.xls";        // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      }
      // *************************
   } else {
      // 2016.08.02 検査成績諸舶用
      //// 2016.06.08 検査成績諸舶用
      ////s_Tmplate_Pass = G_Template_Dir + "検査成績書.xls";               // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      //s_Tmplate_Pass = G_Template_Dir + "検査成績書舶用.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      //// *************************
      if (i_PosM >= 1){
         s_Tmplate_Pass = G_Template_Dir + "検査成績書舶用.xls";    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else {
         s_Tmplate_Pass = G_Template_Dir + "検査成績書.xls";        // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      }
      // *************************
   }
   // *********************
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + G_SAKUBAN + ".xls";    // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";    // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "----------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "-----------    『舶用申請書』作成開始    -----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);


   //******************************
   //***  テンプレート　コピー
   //******************************
   if (!Template_Copy(s_Tmplate_Pass,s_MakeExcel_Pass)){
      // ｴﾗｰﾛｸﾞ追加
      G_ErrLog_Text = "ファイルコピーを失敗しました。『" + s_Tmplate_Pass +
                      "』→『" + s_MakeExcel_Pass + "』";
      Write_Error_Log(G_ErrLog_Text);

      return false;
   } else {
      G_Log_Text = "ファイルコピー成功。『" + s_Tmplate_Pass +
                      "』→『" + s_MakeExcel_Pass + "』";
      Write_Log(G_Log_Text);
   }

   G_Log_Text = "作成ファイル名：    『" + s_MakeExcel_Pass + "』";
   Write_Log(G_Log_Text);

   switch( strStdCode.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         FuneyousinseiDNV();           // 舶用申請書DNV作成
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         FuneyousinseiABS();        // 舶用申請書ABS作成
         break;
      case SEISEKISYO_STANDARDNO_LR:
         FuneyousinseiLR();         // 舶用申請書LR作成
         break;
      case SEISEKISYO_STANDARDNO_NK:
         FuneyousinseiNK();         // 舶用申請書NK作成
         break;
      case SEISEKISYO_STANDARDNO_GL:
         FuneyousinseiGL();         // 舶用申請書GL作成
         break;
      case SEISEKISYO_STANDARDNO_KR:
         FuneyousinseiKR();         // 舶用申請書KR作成
         break;
      case SEISEKISYO_STANDARDNO_BV:
         FuneyousinseiBV();         // 舶用申請書BV作成
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         FuneyousinseiCCS();        // 舶用申請書CCS作成
         break;
      default :
         FuneyousinseiABS();        // 舶用申請書ABS作成
         break;
   }

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『舶用申請書』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;

}


//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書DNV作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiDNV(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 DNV 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他-FAX-DNV.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他-FAX");

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他申");

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server 移植対応
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
	  G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  シート作成 (他FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  シート作成 (他申)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server 移植対応
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
	  exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書ABS作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiABS(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 ABS 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他-FAX-ABS.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他-FAX");
   G_Log_Text = "シートコピー (他FAX)";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他申");
   G_Log_Text = "シートコピー (他申)";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (ABS証)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "ABS証.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"ABS証");
   G_Log_Text = "シートコピー (ABS証)";
   Write_Log(G_Log_Text);

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
      //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
      //Windows 2008Server 移植対応
      exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      //Windows 2008Server 移植対応
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");

      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }


   //******************************
   //***  シート作成 (他FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  シート作成 (他申)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  シート作成 (ABS証)
   //******************************
   ShomeiABS();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
      //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
      //Windows 2008Server 移植対応
      exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書LR作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiLR(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 LR 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他-FAX-LR.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他-FAX");

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他申");

   //******************************
   //***  シートコピー (LR証)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "LR証.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"LR証");

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
      //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
      //Windows 2008Server 移植対応
      exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      //Windows 2008Server 移植対応
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  シート作成 (他FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  シート作成 (他申)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  シート作成 (LR証)
   //******************************
   ShomeiLR();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server 移植対応
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書NK作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiNK(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 NK 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "NK-FAX.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"NK-FAX");

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "NK申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"NK申");

   //******************************
   //***  シートコピー (LR証)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "NK証.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"NK証");

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server 移植対応
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
	  G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
	  G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  シート作成 (NK-FAX)
   //******************************
   ShinseiFaxNK();

   //******************************
   //***  シート作成 (NK申)
   //******************************
   ShinseiNK();

   //******************************
   //***  シート作成 (NK証)
   //******************************
   ShomeiNK();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server 移植対応
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書GL作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiGL(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 GL 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他-FAX-GL.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他-FAX");

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他申");   

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server 移植対応
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  シート作成 (他FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  シート作成 (他申)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server 移植対応
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}


//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書KR作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiKR(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 KR 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他-FAX-KR.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他-FAX");

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他申");

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server 移植対応
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  シート作成 (他FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  シート作成 (他申)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server 移植対応
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }


}


//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書BV作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiBV(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 BV 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他-FAX-BV.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他-FAX");

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他申");

   //******************************
   //***  シートコピー (BV証)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "BV証.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"BV証");

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server 移植対応
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
	  G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  シート作成 (他FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  シート作成 (他申)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  シート作成 (BV証)
   //******************************
   ShomeiBV();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server 移植対応
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// 日本語関数名： 舶用申請書CCS作成処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： 本項目のコピーの順番は"和文"シートに近い物からコピーしていく。
//             ： 理由："和文"シートの直前にコピーする為
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiCCS(void)
{
   //******************************
   //***  規格ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "申請書規格：    『 CCS 』";
   Write_Log(G_Log_Text);

   //******************************
   //***  シートコピー (他FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他-FAX-CCS.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他-FAX");

   //******************************
   //***  シートコピー (他申)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "他申.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"和文",s_Tmplate_Pass,"他申");

   //******************************
   //***  Excel Open処理
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server 移植対応
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "ファイル名『"+s_MakeExcel_Pass+"』Worksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  シート作成 (他FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  シート作成 (他申)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close処理
   //******************************
   // 警告ダイアログ非表示処理
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server 移植対応
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application DisplayAlerts = false』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // 名前を付けて保存処理
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Application 名前を付けて保存』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBookクローズ処理
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbook Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooksクローズ処理
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "『Excel Workbooks Close』に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
}
