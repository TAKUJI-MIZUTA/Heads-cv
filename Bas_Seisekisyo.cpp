//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：検査成績書  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.05.31
//　 更 新 日：2002.09.09 2007.03
//             2007.10.10 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include <utilcls.h>    // Excel起動用ﾍｯﾀ
#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Seisekisyo.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 検査成績書　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Seisekisyo(HeadsData* pcHeadsData)
{
   // 2021.06.18 FireDACへの移行 MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDACへの移行 MOD_E

   AnsiString s_Tmplate_Pass;       // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;     // 作成ﾌｧｲﾙ名
   AnsiString s_Text;               // 汎用文字列
   // 2020.06.18 常熟フラグ追加_S
   AnsiString s_Text2;               // 汎用文字列
   // 2020.06.18 常熟フラグ追加_E
   AnsiString s_S1Nossle;           // S1ﾉｽﾞﾙ種類
   AnsiString s_S2Nossle;           // S2ﾉｽﾞﾙ種類
   AnsiString s_S3Nossle;           // S3ﾉｽﾞﾙ種類
   AnsiString s_S4Nossle;           // S3ﾉｽﾞﾙ種類
   AnsiString s_E1Nossle;           // E1ﾉｽﾞﾙ種類
   AnsiString s_E2Nossle;           // E2ﾉｽﾞﾙ種類
   AnsiString s_E3Nossle;           // E3ﾉｽﾞﾙ種類
   AnsiString s_E4Nossle;           // E4ﾉｽﾞﾙ種類

   AnsiString s_Nossle_Nm1;         // ﾉｽﾞﾙ継手名和1(各 入口ﾉｽﾞﾙ作成用)
   AnsiString s_Nossle_Nm2;         // ﾉｽﾞﾙ継手名和2(各 入口ﾉｽﾞﾙ作成用)
   AnsiString s_Nsl_Tugite;         // ﾉｽﾞﾙ継手     (各 入口ﾉｽﾞﾙ作成用)

   int        i_RecNo;              // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   AnsiString s_SQL;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   AnsiString s_SpecCode;
   // 2011.02.09 中国語対応
   AnsiString s_China;           // 言語区分("0":日本語、"1":中国語)
   // *********************
   // 2016.06.08 検査成績諸舶用
   int i_PosM;                   // 舶用フラグ ("0":舶用以外、"1"以上:舶用)
   // *************************
   // 2017.06.28 斜行流区分追加_S
   bool bSYAKOU = false;
   // 2017.06.28 斜行流区分追加_E

   // 2022.04.22 耐圧保持時間指定追加_S
   int w_count1;
   int w_count2;
   int w_count3;
   int w_count4;
   int w_count5;
   int w_count6;
   int w_count7;
   // 2022.04.22 耐圧保持時間指定追加

   //-----------------------------------
   // 2007.10.10 ﾐﾗｰ仕様対応

   // ﾐﾗｰ仕様の場合、A仕様、B仕様以外は処理不要(正常終了)
   if(pcHeadsData->GetMirrorFlag()){
       if(pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_NONE) return(true);
   }

   // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString型配列に取得
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   // 仕様ﾀｲﾌﾟ文字列取得(ﾌｧｲﾙ名で使用)
   s_SpecCode = pcHeadsData->GetSpecCode();

   // 2011.02.09 中国語対応
   //s_Tmplate_Pass = G_Template_Dir + "検査成績書.xls";                  // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   i_RecNo = 851;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2016.06.08 検査成績諸舶用
   // 2016.08.02 条件修正
   i_PosM = 0;
   // *******************
   i_RecNo = 36;
   i_PosM = s_HEADS_DATA[i_RecNo].Pos("M");
   // *************************
   // 2020.06.18 常熟フラグ追加_S
   i_RecNo = 1107;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   //if ( s_Text == "1" ) {
   if ( s_Text == "1" || s_Text2 == "CHINA" ) {
   // 2020.06.18 常熟フラグ追加_E
      s_China = "1";                                                 // 中国語
      // 2016.06.08 検査成績諸舶用
      //s_Tmplate_Pass = G_Template_Dir + "検査成績書C.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      // 2016.08.02 条件修正
      //if (i_PosM != 0){
      if (i_PosM >= 1){
      // *******************
         s_Tmplate_Pass = G_Template_Dir + "検査成績書舶用C.xls";    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else {
         s_Tmplate_Pass = G_Template_Dir + "検査成績書C.xls";        // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      }
      // *************************
   } else {
      s_China = "0";                                                 // 日本語
      // 2016.06.08 検査成績諸舶用
      //s_Tmplate_Pass = G_Template_Dir + "検査成績書.xls";            // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      // 2016.08.02 条件修正
      //if (i_PosM != 0){
	  if (i_PosM >= 1){
      // *******************
         s_Tmplate_Pass = G_Template_Dir + "検査成績書舶用.xls";    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else {
         s_Tmplate_Pass = G_Template_Dir + "検査成績書.xls";        // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      }
      // *************************
   }
   // *********************
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + G_SAKUBAN + s_SpecCode + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ

   // 2007.10.10
   //-----------------------------------

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『検査成績書』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "作成ファイル名：    『" + s_MakeExcel_Pass + "』";
   Write_Log(G_Log_Text);

// --> 2007.03
   if( bCopySeisekishoFlag )
   {
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
   }
// <-- 2007.03
   //******************************
   //***  Excel Book Open
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
      //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
      //Windows 2008Server 移植対応
      exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      //Windows 2008Server 移植対応
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //***********************************************************************************************
   //***                                                                                         ***
   //***                          検査成績所(和文)　データセット                                 ***
   //***                                                                                         ***
   //***********************************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "和文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));

   //***********************
   //***  和文：御注文先
   //***********************
   // 『A001』
   i_RecNo = 1063;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(3, 4, s_Text);
   Excel_Cell_DataSet(3, 3, s_Text);
   //**********
   G_Log_Text = "和文：御注文先             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  和文：納入先
   //***********************
   // 『A002』
   i_RecNo = 1065;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(5, 4, s_Text);
   Excel_Cell_DataSet(5, 3, s_Text);
   //**********
   G_Log_Text = "和文：納入先               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  和文：型式
   //***********************
   // 『A003』
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      // 2011.02.09 中国語対応
      //s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
      if ( s_China == "0" ) {
          s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
      } else {
          s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      }
      //**********************
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   //2004.02.16
   //Excel_Cell_DataSet(7, 4, s_Text);
   Excel_Cell_DataSet(7, 3, s_Text);
   //**********
   G_Log_Text = "和文：型式                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  和文：機器番号
   //***********************
   // 『A004』
   i_RecNo = 27;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(3, 11, ZenToHan(s_Text));
   //Excel_Cell_DataSet(3, 12, ZenToHan(s_Text));
   //2004.10.01
   int w_count = s_Text.AnsiPos("～");

   if (w_count){
      s_Text = ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "～" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
   }else{
      s_Text = ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   }

   Excel_Cell_DataSet(3, 12, s_Text);
   //**********
   G_Log_Text = "和文：機器番号             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  和文：製造番号
   //***********************
   // 『A005』
   //------------------------------------
   // 2007.10.15 仕様ﾀｲﾌﾟ文字列追加
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E   
   s_Text = s_Text + pcHeadsData->GetSpecCode();
   // 2007.10.15
   //------------------------------------

   //2004.02.16
   //Excel_Cell_DataSet(5, 11, s_Text);
   Excel_Cell_DataSet(5, 12, s_Text);
   //**********
   G_Log_Text = "和文：製造番号            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  和文：台数
   //***********************
   // 『A006』
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      // 2016.06.08 検査成績諸舶用
      ////2004.02.16
      ////Excel_Cell_DataSet(7, 11, s_Text);
      //Excel_Cell_DataSet(7, 12, s_Text);
      ////**********
      // 2016.08.02 条件修正
      //if (i_PosM != 0){
      if (i_PosM >= 1){
      // *******************
         Excel_Cell_DataSet(7, 10, s_Text);
      } else {
         Excel_Cell_DataSet(7, 12, s_Text);
      }
      // *************************
      G_Log_Text = "和文：台数                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "和文：台数                RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   // 2016.06.08 検査成績諸舶用
   //***********************
   //***  和文：船番
   //***********************
   // 2016.08.02 条件修正
   //if (i_PosM != 0){
   if (i_PosM >= 1){
   // *******************
      i_RecNo = 270;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(7, 12, s_Text);
      G_Log_Text = "和文：船番                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   // *************************

   //2019/05/17 応用技術追加
   if (!bIsNotKatashiki) {
   // 2017.06.28 斜行流区分追加_S
   bSYAKOU = false;
   // 2022.04.21 CX-01D追加_S
   if ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "０１Ｄ" ) {
       bSYAKOU = true;
   }
   // 2022.04.21 CX-01D追加_E
   if ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) {
       bSYAKOU = true;
   }
   if ( G_KS_Syurui == "ＬＸ" && G_KS_Model == "９０Ｄ" ) {
       bSYAKOU = true;
   }
   if ( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) {
       bSYAKOU = true;
   }
   // 2022.04.21 YX-83追加_S
   if ( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８３" ) {
       bSYAKOU = true;
   }
   // 2022.04.21 YX-83追加_E
   // 2017.06.28 斜行流区分追加_E

   //***************************
   //***  和文：B側液体名
   //***************************
   // 『A007』
   i_RecNo = 137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
      i_RecNo = 76;     // Ａ流体名
   } else {
      i_RecNo = 45;     // Ｂ流体名
   }
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(11, 5, s_Text);
   G_Log_Text = "和文：B側液体名            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：B側設計圧力
   //***************************
   // 『A008』
   i_RecNo = 825;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        // 2011.02.09 中国語対応
        //s_Text = "大気開放";
        if ( s_China == "0" ) {
            s_Text = "大気開放";
        } else {
            s_Text = "大气/U+24320/U+放";
        }
        //**********************
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(12, 5, s_Text);
   G_Log_Text = "和文：B側設計圧力          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：B側入口ﾉｽﾞﾙ
   //*******************************
   // 『A009』,『A010』
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ﾉｽﾞﾙ種類
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   if (s_S1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1152].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1153].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1168].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1169].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   // 入口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(13, 5, s_Text);
   G_Log_Text = "和文：B側入口ﾉｽﾞﾙ上       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(14, 5, s_Text);
   G_Log_Text = "和文：B側入口ﾉｽﾞﾙ下       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：B側出口ﾉｽﾞﾙ
   //*******************************
   // 『A011』,『A012』
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ﾉｽﾞﾙ種類
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   if (s_S1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1152].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1153].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1168].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1169].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   // 出口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(15, 5, s_Text);
   G_Log_Text = "和文：B側出口ﾉｽﾞﾙ上       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(16, 5, s_Text);
   G_Log_Text = "和文：B側出口ﾉｽﾞﾙ下       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：B側ｶﾞｽｹｯﾄ材質
   //*******************************
   // 『A013』
   i_RecNo = 835;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDACへの移行 MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDACへの移行 MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX用ｶﾞｽｹｯﾄ材質追加
   if ( G_KS_Syurui == "ＷＸ" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // 溶接側は高温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＢ側
            // 2011.02.09 中国語対応
            //s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            if ( s_China == "0" ) {
                s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            } else {
                s_Text = "熔接＋" + s_Text + "(Ｏ－/U+29615/U+)";
            }
            //**********************
         } else {                                               // AB反転
            // 溶接側はＡ側
         }
      } else {                                               // 溶接側は低温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＡ側
         } else {                                               // AB反転
            // 溶接側はＢ側
            // 2011.02.09 中国語対応
            //s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            if ( s_China == "0" ) {
                s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            } else {
                s_Text = "熔接＋" + s_Text + "(Ｏ－/U+29615/U+)";
            }
            //**********************
         }
      }
   }

   Excel_Cell_DataSet(18, 5, s_Text);
   G_Log_Text = "和文：B側ｶﾞｽｹｯﾄ材質       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：A側液体名
   //*******************************
   // 『A014』
   i_RecNo = 137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
      i_RecNo = 45;     // Ｂ流体名
   } else {
      i_RecNo = 76;     // Ａ流体名
   }
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(11, 10, s_Text);
   G_Log_Text = "和文：A側液体名            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：A側液体圧力
   //*******************************
   // 『A015』
   i_RecNo = 830;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        // 2011.02.09 中国語対応
        //s_Text = "大気開放";
        if ( s_China == "0" ) {
            s_Text = "大気開放";
        } else {
            s_Text = "大气/U+24320/U+放";
        }
        //**********************
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(12, 10, s_Text);
   G_Log_Text = "和文：A側液体圧力          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：A側入口ﾉｽﾞﾙ
   //*******************************
   // 『A016』,『A017』
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ﾉｽﾞﾙ種類
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ﾉｽﾞﾙ種類
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ﾉｽﾞﾙ種類

   if (s_S2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1156].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1157].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1172].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1173].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   //入口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(13, 10, s_Text);
   G_Log_Text = "和文：A側入口ﾉｽﾞﾙ上       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(14, 10, s_Text);
   G_Log_Text = "和文：A側入口ﾉｽﾞﾙ下       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：A側出口ﾉｽﾞﾙ
   //*******************************
   // 『A018』,『A019』
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ﾉｽﾞﾙ種類
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ﾉｽﾞﾙ種類
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ﾉｽﾞﾙ種類

   if (s_S2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1156].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1157].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1172].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1173].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ﾉｽﾞﾙ継手名和1
      s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ﾉｽﾞﾙ継手名和2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ﾉｽﾞﾙ継手名和1
         s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ﾉｽﾞﾙ継手名和2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   // 出口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(15, 10, s_Text);
   G_Log_Text = "和文：A側出口ﾉｽﾞﾙ上       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(16, 10, s_Text);
   G_Log_Text = "和文：A側出口ﾉｽﾞﾙ下       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：A側ｶﾞｽｹｯﾄ材質
   //*******************************
   // 『A020』
   i_RecNo = 836;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDACへの移行 MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDACへの移行 MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX用ｶﾞｽｹｯﾄ材質追加
   if ( G_KS_Syurui == "ＷＸ" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // 溶接側は高温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＢ側
         } else {                                               // AB反転
            // 溶接側はＡ側
            // 2011.02.09 中国語対応
            //s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            if ( s_China == "0" ) {
                s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            } else {
                s_Text = "熔接＋" + s_Text + "(Ｏ－/U+29615/U+)";
            }
            //**********************
         }
      } else {                                               // 溶接側は低温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＡ側
            // 2011.02.09 中国語対応
            //s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            if ( s_China == "0" ) {
                s_Text = "溶接＋" + s_Text + "（Ｏ－リング）";
            } else {
                s_Text = "熔接＋" + s_Text + "(Ｏ－/U+29615/U+)";
            }
            //**********************
         } else {                                               // AB反転
            // 溶接側はＢ側
         }
      }
   }
   //*****************************

   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "和文：A側ｶﾞｽｹｯﾄ材質       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：ﾌﾟﾚｰﾄ材質
   //*******************************
   // 『A021』
   i_RecNo = 43;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDACへの移行 MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDACへの移行 MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   Excel_Cell_DataSet(17, 4, s_Text);
   G_Log_Text = "和文：ﾌﾟﾚｰﾄ材質           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  和文：試験圧力B側(水圧)
   //********************************
   // 『A022』
   i_RecNo = 827;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet(32, 7, s_Text);
      Excel_Cell_DataSet(36, 7, s_Text);
      // 2019.02.22 書式変更_E
      G_Log_Text = "和文：試験圧力B側(水圧)   RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "和文：試験圧力B側(水圧)   不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //********************************
   //***  和文：試験圧力B側(空圧)
   //********************************
   // 『A023』
   i_RecNo = 826;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet(33, 7, s_Text);
      Excel_Cell_DataSet(37, 7, s_Text);
      // 2019.02.22 書式変更_E
      G_Log_Text = "和文：試験圧力B側(空圧)         RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "和文：試験圧力B側(空圧)  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //********************************
   //***  和文：試験圧力A側(水圧)
   //********************************
   // 『A024』
   i_RecNo = 832;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet(35, 7, s_Text);
      Excel_Cell_DataSet(39, 7, s_Text);
      // 2019.02.22 書式変更_E
      G_Log_Text = "和文：試験圧力A側(水圧)         RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "和文：試験圧力A側(水圧)   不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //********************************
   //***  和文：試験圧力A側(空圧)
   //********************************
   // 『A025』
   i_RecNo = 831;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet(36, 7, s_Text);
      Excel_Cell_DataSet(40, 7, s_Text);
      // 2019.02.22 書式変更_E
      G_Log_Text = "和文：試験圧力A側(空圧)         RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "和文：試験圧力A側(空圧)  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //*******************************
   //***  禁油／脱脂処理
   //*******************************
   // 『A026』
//2003.09.09 禁油／脱脂処理
   if (s_HEADS_DATA[155].Trim().ToIntDef(0) == 1){
//      s_Text = "禁油処理";
      // 2019.02.22 書式変更_S
      //// 2011.02.09 中国語対応
      ////s_Text = "４．禁油処理品";
      //if ( s_China == "0" ) {
      //    s_Text = "４．禁油処理品";
      //} else {
      //    s_Text = "４．禁油/U+22788/U+理品";
      //}
      ////**********************
      if ( s_China == "0" ) {
          s_Text = "５．禁油処理品";
      } else {
          s_Text = "５．禁油/U+22788/U+理品";
      }
      // 2019.02.22 書式変更_E
   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) == 1) {
//      s_Text = "脱脂処理";
      // 2019.02.22 書式変更_S
      //// 2011.02.09 中国語対応
      ////s_Text = "４．脱脂処理品";
      //if ( s_China == "0" ) {
      //    s_Text = "４．脱脂処理品";
      //} else {
      //    s_Text = "４．脱脂/U+22788/U+理品";
      //}
      ////**********************
       if ( s_China == "0" ) {
          s_Text = "５．脱脂処理品";
      } else {
          s_Text = "５．脱脂/U+22788/U+理品";
      }
      // 2019.02.22 書式変更_E
   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) != 1
                                                        && s_HEADS_DATA[157].Trim().ToIntDef(0) == 1) {
//      s_Text = "準脱脂処理";
      // 2019.02.22 書式変更_S
      //// 2011.02.09 中国語対応
      ////s_Text = "４．準脱脂処理品";
      //if ( s_China == "0" ) {
      //    s_Text = "４．準脱脂処理品";
      //} else {
      //    s_Text = "４．准脱脂/U+22788/U+理品";
      //}
      ////**********************
      if ( s_China == "0" ) {
          s_Text = "５．準脱脂処理品";
      } else {
          s_Text = "５．准脱脂/U+22788/U+理品";
      }
      // 2019.02.22 書式変更_E
   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) != 1
                                                        && s_HEADS_DATA[157].Trim().ToIntDef(0) != 1) {
      s_Text = "";
//      Excel_Cell_DataSet(37, 12, "");
//      Excel_Cell_DataSet(37, 13, "");
      // 2019.02.22 書式変更_S
      ////2004.02.16
      ////Excel_Cell_DataSet(37, 12, "");
      ////Excel_Cell_DataSet(37, 13, "");
      //Excel_Cell_DataSet(37, 13, "");
      //Excel_Cell_DataSet(37, 14, "");
      ////**********
      Excel_Cell_DataSet(41, 13, "");
      Excel_Cell_DataSet(41, 14, "");
      // 2019.02.22 書式変更_E
   }
//   Excel_Cell_DataSet(37, 3, s_Text);
//   Excel_Cell_DataSet(37, 7, s_Text);
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet(37, 2, s_Text);
   Excel_Cell_DataSet(41, 2, s_Text);
   // 2019.02.22 書式変更_E
//*************************
   G_Log_Text = "和文：禁油／脱脂処理      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   }

   //*******************************
   //***  和文：営業担当者
   //*******************************
   // 『A027』
   i_RecNo = 3;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(45, 11, s_Text);
   Excel_Cell_DataSet(45, 12, s_Text);
   //**********
   G_Log_Text = "和文：営業担当者           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // 『A028』
   i_RecNo = 302;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "S";
   // 2017.10.06 短納期対応_S
   } else if (s_HEADS_DATA[i_RecNo].TrimRight() == "2") {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "T";
   // 2017.10.06 短納期対応_E
   } else {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   //2004.02.16
   //Excel_Cell_DataSet(51, 12, s_Text);
   Excel_Cell_DataSet(51, 13, s_Text);
   //**********
   G_Log_Text = "和文：HEADS VER           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  和文：耐圧保持時間
   //*******************************
   // 『A029』,『A030』
   if (Tokki_Code("2C", s_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間標準
      s_Text = "20分間保持";
      s_Text2 = "保持20分/U+38047/U+";
   }
   else if (Tokki_Code("2D", s_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間30分
      s_Text = "30分間保持";
      s_Text2 = "保持30分/U+38047/U+";
   }
   else if (Tokki_Code("2E", s_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間指定あり
      s_Text = Tokki_Zen("2E", s_HEADS_DATA, 1500);
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
      w_count7 = s_Text.AnsiPos("小");

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
      } else if (w_count7){
         // "小"が存在
         s_Text = "";
      } else if (s_Text == ""){
          // 特記が存在しない
      } else {
          // "分","m","M"が存在しない
      }

      if (s_Text == ""){
         // 特記が存在しない
         s_Text = "保持時間入力エラー";
         s_Text2 = "ERROR";
      } else {
         s_Text = s_Text.TrimRight();
         s_Text = ZenToHan(s_Text);
         s_Text2 = "保持" + s_Text +"分/U+38047/U+";
         s_Text = s_Text + "分間保持";
      }

   } else {
      // 耐圧保持時間指定なし
      s_Text = "20分間保持";
      s_Text2 = "保持20分/U+38047/U+";
   }

   if ( s_China == "0" ) {
      Excel_Cell_DataSet(36, 14, s_Text);
      Excel_Cell_DataSet(39, 14, s_Text);
      G_Log_Text = "和文：耐圧保持時間        『" + s_Text + "』をセット。";
   } else {
      Excel_Cell_DataSet(36, 14, s_Text2);
      Excel_Cell_DataSet(39, 14, s_Text2);
      G_Log_Text = "中文：耐圧保持時間        『" + s_Text2 + "』をセット。";
   }
   Write_Log(G_Log_Text);
   // 2022.04.12 耐圧保持時間指定追加_E

   //2019/05/17 応用技術追加
   if (!bIsNotKatashiki) {
   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));

   //// 2019.02.19 バーコード追加_S
   ////***************************
   ////***  和文：バーコード作番
   ////***************************
   //s_Text = G_SAKUBAN;
   //Excel_Cell_DataSet(10, 20, s_Text);
   //G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   // 2020.04.08 バーコード変更_E
   // 2023.05.30 バーコード変更_S
   s_Text = G_SAKUBAN;
   Excel_Cell_DataSet(4, 5, s_Text);
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2023.05.30 バーコード変更_E

   //***************************
   //***  和文：訂正
   //***************************
   i_RecNo = 319;
   s_Text = s_HEADS_DATA[i_RecNo];
   // 2020.04.08 バーコード変更_S
   //Excel_Cell_DataSet(14, 20, s_Text);
   Excel_Cell_DataSet(8, 2, s_Text);
   // 2020.04.08 バーコード変更_E
   G_Log_Text = "和文：訂正            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //// 2019.02.19 バーコード追加_E

   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   // 2020.04.08 バーコード変更_E
   }


   //***********************************************************************************************
   //***                                                                                         ***
   //***                          検査成績所(英文)　データセット                                 ***
   //***                                                                                         ***
   //***********************************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "英文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));

   //***********************
   //***  PURCHASER
   //***********************
   // 『A001』
   i_RecNo = 1261;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 5, s_Text);
   G_Log_Text = "英文：PURCHASER            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  Costomer
   //***********************
   // 『A002』
   i_RecNo = 1262;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(8, 5, s_Text);
   G_Log_Text = "英文：COSTOMER             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  Model
   //***********************
   // 『A003』
   i_RecNo = 33;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(12, 5, s_Text);
   G_Log_Text = "英文：MODEL                RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  ITEM NO
   //***********************
   // 『A004』
   i_RecNo = 27;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(10, 5, s_Text);
   G_Log_Text = "英文：ITEM NO              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  M.F.G No
   //***********************
   // 『A005』
   //------------------------------
   // 2007.10.15 仕様ﾀｲﾌﾟ文字列追加
   //2022/12/15 桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 桁数追加対応 Mod_E
   s_Text = s_Text + pcHeadsData->GetSpecCode();
   // 2007.10.15
   //------------------------------

   Excel_Cell_DataSet(12, 10, s_Text);
   G_Log_Text = "英文：M.F.G No            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***********************
   //***  QUANTITY
   //***********************
   // 『A006』
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(12, 13, s_Text);
      G_Log_Text = "英文：QUANTITY             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "英文：QUANTITY            RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   // 2016.06.08 検査成績諸舶用
   //************************
   //***  SHIP No.
   //************************
   // 2016.08.02 条件修正
   //if (i_PosM != 0){
   if (i_PosM >= 1){
   // *******************
      i_RecNo = 270;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(10, 10, s_Text);
      G_Log_Text = "英文：SHIP No.             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   // *************************

   //2019/05/17 応用技術追加
   if (!bIsNotKatashiki) {
   //***************************
   //***  B.Fluid
   //***************************
   // 『A007』
   //2003.10.31 データ位置変更
   //i_RecNo = 137;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
   //   i_RecNo = 1074;     // Ａ流体名
   //} else {
   //   i_RecNo = 1072;     // Ｂ流体名
   //}
   i_RecNo = 1072;     // Ｂ側流体名
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 5, s_Text);
   G_Log_Text = "英文：B.Fluid              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B.Design Pressure
   //***************************
   // 『A008』
   i_RecNo = 825;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        s_Text = "0.00";
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(17, 5, s_Text);
   G_Log_Text = "英文：B.Design Pressure    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  B.N&R inlet
   //*******************************
   // 『A009』,『A010』
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ﾉｽﾞﾙ種類
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   if (s_S1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1154].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1155].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1170].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1171].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E3ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   // 入口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(18, 5, s_Text);
   G_Log_Text = "英文：B.N&R inlet Top     『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(19, 5, s_Text);
   G_Log_Text = "英文：B.N&R inlet Bottom  『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  B.N&R outlet
   //*******************************
   // 『A011』,『A012』
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ﾉｽﾞﾙ種類
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   if (s_S1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1154].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1155].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1170].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1171].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E3ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   // 出口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(20, 5, s_Text);
   G_Log_Text = "英文：B.N&R outlet Top    『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(21, 5, s_Text);
   G_Log_Text = "英文：B.N&R outlet Bottom 『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  B.Gasket Material
   //*******************************
   // 『A013』
   i_RecNo = 835;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDACへの移行 MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDACへの移行 MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX用ｶﾞｽｹｯﾄ材質追加
   if ( G_KS_Syurui == "ＷＸ" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // 溶接側は高温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＢ側
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         } else {                                               // AB反転
            // 溶接側はＡ側
         }
      } else {                                               // 溶接側は低温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＡ側
         } else {                                               // AB反転
            // 溶接側はＢ側
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         }
      }
   }
   //*****************************

   Excel_Cell_DataSet(23, 5, ZenToHan(s_Text));
   G_Log_Text = "英文：B.Gasket Material   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.Fluid
   //*******************************
   // 『A014』
   //2003.10.31 データ位置変更
   //i_RecNo = 137;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
   //   i_RecNo = 1072;     // Ｂ流体名
   //} else {
   //   i_RecNo = 1074;     // Ａ流体名
   //}
   i_RecNo = 1074;     // Ａ側流体名
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 10, s_Text);
   G_Log_Text = "英文：A.Fluid              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.Design Pressure
   //*******************************
   // 『A015』
   i_RecNo = 830;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        s_Text = "0.00";
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(17, 10, s_Text);
   G_Log_Text = "英文：A.Design Pressure    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.N&R inlet
   //*******************************
   // 『A016』,『A017』
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ﾉｽﾞﾙ種類
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ﾉｽﾞﾙ種類
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ﾉｽﾞﾙ種類

   if (s_S2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1158].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1159].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1174].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1175].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   //入口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "英文：A.N&R inlet Top     『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(19, 10, s_Text);
   G_Log_Text = "英文：A.N&R inlet Bottom  『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.N&R outlet
   //*******************************
   // 『A018』,『A019』
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ﾉｽﾞﾙ種類
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ﾉｽﾞﾙ種類
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ﾉｽﾞﾙ種類
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ﾉｽﾞﾙ種類

   if (s_S2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1158].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1159].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1174].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1175].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ﾉｽﾞﾙ継手名英1
      s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ﾉｽﾞﾙ継手名英2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ﾉｽﾞﾙ継手

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D追加
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

   // 2017.06.28 斜行流区分追加_S
   //if(( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ) ||
   //   ( G_KS_Syurui == "ＣＸ" && G_KS_Model == "１０Ｄ" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 斜行流区分追加_E
      if (s_S3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ﾉｽﾞﾙ継手
      } else if (s_E3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ﾉｽﾞﾙ継手名英1
         s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ﾉｽﾞﾙ継手名英2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ﾉｽﾞﾙ継手
      }
   }
   //*********************

   // 出口ﾉｽﾞﾙ　セット値取得
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(20, 10, s_Text);
   G_Log_Text = "英文：A.N&R outlet Top    『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(21, 10, s_Text);
   G_Log_Text = "英文：A.N&R outlet Bottom 『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.Gasket Material
   //*******************************
   // 『A020』
   i_RecNo = 836;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDACへの移行 MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDACへの移行 MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX用ｶﾞｽｹｯﾄ材質追加
   if ( G_KS_Syurui == "ＷＸ" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // 溶接側は高温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＢ側
         } else {                                               // AB反転
            // 溶接側はＡ側
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         }
      } else {                                               // 溶接側は低温側
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB反転ではない
            // 溶接側はＡ側
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         } else {                                               // AB反転
            // 溶接側はＢ側
         }
      }
   }
   //*****************************

   Excel_Cell_DataSet(23, 10, ZenToHan(s_Text));
   G_Log_Text = "英文：A.Gasket Material   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  Plate Material
   //*******************************
   // 『A021』
   i_RecNo = 43;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDACへの移行 MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDACへの移行 MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
   }

   Excel_Cell_DataSet(22, 4, ZenToHan(s_Text));
   G_Log_Text = "英文：Plate Material      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  Test Pressure B.(W)
   //********************************
   // 『A022』
   i_RecNo = 827;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(37, 6, s_Text);
      G_Log_Text = "英文：Test Pressure B.(W)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "英文：Test Pressure B.(W)   不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //********************************
   //***  Test Pressure B.(A)
   //********************************
   // 『A023』
   i_RecNo = 826;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(38, 6, s_Text);
      G_Log_Text = "英文：Test Pressure B.(A)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "英文：Test Pressure B.(A)   不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //********************************
   //***  Test Pressure A.(W)
   //********************************
   // 『A024』
   i_RecNo = 832;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(40, 6, s_Text);
      G_Log_Text = "英文：Test Pressure A.(W)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "英文：Test Pressure A.(W)   不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //********************************
   //***  Test Pressure A.(A)
   //********************************
   // 『A025』
   i_RecNo = 831;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(41, 6, s_Text);
      G_Log_Text = "英文：Test Pressure A.(A)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "英文：Test Pressure A.(A)   不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************************
   //***  Oilless Treatment／Judgement
   //***************************************
   // 『A026』,『A027』
   if (s_HEADS_DATA[155].Trim().ToIntDef(0) == 1 || s_HEADS_DATA[156].Trim().ToIntDef(0) == 1
                                                 || s_HEADS_DATA[157].Trim().ToIntDef(0) == 1){

      // 2019.02.22 書式変更_S
      //s_Text = "4.Oilless Treatment";
      //Excel_Cell_DataSet(43, 2, s_Text);
      //G_Log_Text = "英文：4.Oilless Treatment 『" + s_Text + "』をセット。";
      s_Text = "5.Oilless Treatment";
      Excel_Cell_DataSet(47, 2, s_Text);
      G_Log_Text = "英文：5.Oilless Treatment 『" + s_Text + "』をセット。";
      // 2019.02.22 書式変更_E
      Write_Log(G_Log_Text);

//2003.09.09 禁油／脱脂処理
//      s_Text = "Judgement : GOOD";
//      s_Text = "Judgement : ";
      s_Text = "Judgement : GOOD";
//*************************
      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet(43, 10, s_Text);
      Excel_Cell_DataSet(47, 11, s_Text);
      // 2019.02.22 書式変更_E
      G_Log_Text = "英文：Judgement           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) != 1
                                                        && s_HEADS_DATA[157].Trim().ToIntDef(0) != 1){

      s_Text = "";
      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet(43, 2, s_Text);
      //G_Log_Text = "英文：4.Oilless Treatment 『" + s_Text + "』をセット。";
      Excel_Cell_DataSet(47, 2, s_Text);
      G_Log_Text = "英文：5.Oilless Treatment 『" + s_Text + "』をセット。";
      // 2019.02.22 書式変更_E
      Write_Log(G_Log_Text);

      s_Text = "";
      // 2019.02.22 書式変更_S
      //Excel_Cell_DataSet(43, 10, s_Text);
      Excel_Cell_DataSet(47, 11, s_Text);
      // 2019.02.22 書式変更_E
      G_Log_Text = "英文：Judgement           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   }

   //***************************
   //***  HEADS VER
   //***************************
   // 『A028』
   i_RecNo = 302;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "S";
   // 2017.10.06 短納期対応_S
   } else if (s_HEADS_DATA[i_RecNo].TrimRight() == "2") {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "T";
   // 2017.10.06 短納期対応_E
   } else {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   // 2019.02.22 書式変更_S
   //Excel_Cell_DataSet(50, 12, s_Text);
   Excel_Cell_DataSet(53, 12, s_Text);
   // 2019.02.22 書式変更_E
   G_Log_Text = "英文：HEADS VER           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2022.04.22 耐圧保持時間指定追加_S
   //***************************
   //***  Holding time
   //***************************
   if (Tokki_Code("2C", s_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間標準
      s_Text = "MPa G  (by water) Holding time   20   min.";
   }
   else if (Tokki_Code("2D", s_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間30分
      s_Text = "MPa G  (by water) Holding time   30   min.";
   }
   else if (Tokki_Code("2E", s_HEADS_DATA, 1500) == true) {
      // 耐圧保持時間指定あり
      s_Text = Tokki_Zen("2E", s_HEADS_DATA, 1500);
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
         s_Text = "MPa G  (by water) Holding time   " + s_Text + "   min.";
      }

   }
   else {
      // 耐圧保持時間指定なし
      s_Text = "MPa G  (by water) Holding time   20   min.";
   }

   Excel_Cell_DataSet(37, 8, s_Text);
   Excel_Cell_DataSet(40, 8, s_Text);
   G_Log_Text = "英文：Holding time        『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2022.04.22 耐圧保持時間指定追加_E

   //2019/05/17 応用技術追加
   if (!bIsNotKatashiki) {
   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));

   //// 2019.02.19 バーコード追加_S
   ////***************************
   ////***  英文：バーコード作番
   ////***************************
   //s_Text = G_SAKUBAN;
   //Excel_Cell_DataSet(10, 20, s_Text);
   //G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   // 2020.04.08 バーコード変更_E
   // 2023.05.30 バーコード変更_S
   s_Text = G_SAKUBAN;
   Excel_Cell_DataSet(17, 5, s_Text);
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2023.05.30 バーコード変更_E

   //***************************
   //***  英文：訂正
   //***************************
   i_RecNo = 319;
   s_Text = s_HEADS_DATA[i_RecNo];
   // 2020.04.08 バーコード変更_S
   //Excel_Cell_DataSet(14, 20, s_Text);
   Excel_Cell_DataSet(21, 2, s_Text);
   // 2020.04.08 バーコード変更_E
   G_Log_Text = "英文：訂正            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //// 2019.02.19 バーコード追加_E

   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));
   // 2020.04.08 バーコード変更_E
   }


   delete wkQuery;

   //******************************
   //***  Excel Book Close
   //******************************
   // 印刷しないほうのシートをＰＤＦ設定シートに記述する
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "ＰＤＦ設定");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("ＰＤＦ設定"));

   if (s_HEADS_DATA[1].TrimRight() == "0") {
     Excel_Cell_DataSet(4, 2, "英文");
   } else {
     Excel_Cell_DataSet(4, 2, "和文");
   }
   // --> 2007.03
   // 印刷しないシート追加
   Excel_Cell_DataSet( 5, 2, "NK-FAX");
   Excel_Cell_DataSet( 6, 2, "他-FAX");
   Excel_Cell_DataSet( 7, 2, "NK申");
   Excel_Cell_DataSet( 8, 2, "他申");
   Excel_Cell_DataSet( 9, 2, "ABS証");
   Excel_Cell_DataSet(10, 2, "LR証");
   Excel_Cell_DataSet(11, 2, "NK証");
   Excel_Cell_DataSet(12, 2, "BV証");
   // <-- 2007.03

   // 2020.04.08 バーコード変更_S
   // ｼｰﾄの指定
   if (s_HEADS_DATA[1].TrimRight() == "0") {
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   // 2021.10.19 保存処理追加_S
   } else if (s_HEADS_DATA[1].TrimRight() == "2") {
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
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『検査成績書』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名：
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： なし
//---------------------------------------------------------------------------
void __fastcall Get_Nossle_Value(AnsiString NossleNM1, AnsiString NossleNM2, AnsiString Tugite)
{
   int i_Len;

   P_Nossle_Top = "";
   P_Nossle_Bottom = "";

   // 和文：B側入口ﾉｽﾞﾙ 上部
//   if (NossleNM1.Trim() != ""){
//      i_Len = NossleNM1.Pos("-");
//      if (i_Len != 0){
//         P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
//      } else {
//         i_Len = NossleNM1.Pos("－");
//         if (i_Len != 0){
//            P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
//         } else {
//            P_Nossle_Top = "";
//         }
//      }
//   } else {
//      P_Nossle_Top = "";
//   }
//
//   i_Len = NossleNM1.Pos("-");
//   if (i_Len != 0){
//      P_Nossle_Bottom = NossleNM1.SubString(i_Len+1, 48);
//   } else {
//      i_Len = NossleNM1.Pos("－");
//      if (i_Len != 0){
//         P_Nossle_Bottom = NossleNM1.SubString(i_Len+2, 48);
//      } else {
//         P_Nossle_Bottom = "";
//      }
//   }


   // 和文：B側入口ﾉｽﾞﾙ 下部

   // 2011.11.29 ﾉｽﾞﾙ規格変更
   //if (NossleNM2.Trim() != ""){
   // 2013.04.03 ﾉｽﾞﾙ規格追加
   //if ( Tugite.SubString(1,2) == "11" || Tugite.SubString(1,2) == "12"
   //	 || Tugite.SubString(1,2) == "13" || Tugite.SubString(1,2) == "21"
   //	 || Tugite.SubString(1,2) == "22" || Tugite.SubString(1,2) == "23" ){
   if ( Tugite.SubString(1,2) == "11" || Tugite.SubString(1,2) == "12"
     || Tugite.SubString(1,2) == "13" || Tugite.SubString(1,2) == "14"
     || Tugite.SubString(1,2) == "21" || Tugite.SubString(1,2) == "22"
     || Tugite.SubString(1,2) == "23" || Tugite.SubString(1,2) == "24"
     || Tugite.SubString(1,2) == "31" || Tugite.SubString(1,2) == "32"
     || Tugite.SubString(1,2) == "33" || Tugite.SubString(1,2) == "34" ){
   // ***********************
      P_Nossle_Top = NossleNM1;
      if (Tugite.SubString(3,1) == "4"){
         // ﾉｽﾞﾙ継手名和2   "("より前部
         i_Len = NossleNM2.Pos("(");
         if (i_Len != 0){
            P_Nossle_Bottom = NossleNM2.SubString(1,i_Len-1);
         } else {
            i_Len = NossleNM2.Pos("（");
            if (i_Len != 0){
               P_Nossle_Bottom = NossleNM2.SubString(1,i_Len-1);
            } else {
               P_Nossle_Bottom = NossleNM2;
            }
         }

      }

   } else if (NossleNM2.Trim() != ""){
   // ***********************
      i_Len = NossleNM1.Pos("-");
      if (i_Len != 0){
         P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
      } else {
         i_Len = NossleNM1.Pos("－");
         if (i_Len != 0){
             P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
         } else {
             P_Nossle_Top = "";
         }
      }         // ﾉｽﾞﾙ継手名和1   "-"より後部
      i_Len = NossleNM1.Pos("-");
      if (i_Len != 0){
         P_Nossle_Bottom = NossleNM1.SubString(i_Len+1, 48);
      } else {
         i_Len = NossleNM1.Pos("－");
          if (i_Len != 0){
            P_Nossle_Bottom = NossleNM1.SubString(i_Len+2, 48);
          } else {
            P_Nossle_Bottom = "";
          }
      }

      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if (Tugite.SubString(2,1) == "4"){
      if (Tugite.SubString(3,1) == "4"){
      // ***********************
         // ﾉｽﾞﾙ継手名和2   "("より前部
         i_Len = NossleNM2.Pos("(");
         if (i_Len != 0){
            P_Nossle_Bottom = P_Nossle_Bottom + " " + NossleNM2.SubString(1,i_Len-1);
         } else {
            i_Len = NossleNM2.Pos("（");
            if (i_Len != 0){
               P_Nossle_Bottom = P_Nossle_Bottom + " " + NossleNM2.SubString(1,i_Len-1);
            } else {
               P_Nossle_Bottom = P_Nossle_Bottom + " " + NossleNM2;
            }
         }

      }
   //2008.03.21 RX-00 ﾈｼﾞ込みﾉｽﾞﾙ追加
   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //} else if (Tugite.SubString(1,1) == "7"){
   } else if (Tugite.SubString(1,2) == "07"){
   // ***********************
      // ﾉｽﾞﾙ 上部
      if (NossleNM1.Trim() != ""){
          P_Nossle_Top = NossleNM1.Trim();
      } else {
          P_Nossle_Top = "";
      }
      P_Nossle_Bottom = "";
   //********************************
   } else {
      P_Nossle_Bottom = "";
   }
}


//---------------------------------------------------------------------------
// 日本語関数名： 特記コード有無確認
// 概  要      ：
// 引  数      ： AnsiString Key   ： 特記ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2022.04.22追加
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
// 備  考      ： 2022.04.22追加
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

