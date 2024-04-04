//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：小型圧力容器 個別検定申請書  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.06.03
//　 更 新 日：2002.09.20
//             2007.10.10 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include <utilcls.h>    // Excel起動用ﾍｯﾀ
#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Kobetusinsei.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 小型圧力容器 個別検定申請書　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Kobetusinsei(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_Text;            // 汎用文字列
   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo        

   //------------------------------------
   // 2007.10.10 ﾐﾗｰ仕様対応
   // 元仕様以外は処理不要(正常終了)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   s_Tmplate_Pass = G_Template_Dir + "小型圧力容器個別検定申請書.xls";     // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "PVD" + G_SAKUBAN + ".xls";       // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "PVD" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";       // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『小型圧力容器  個別検定申請書』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "作成ファイル名：    『" + s_MakeExcel_Pass + "』";
   Write_Log(G_Log_Text);

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
   //***                          小型圧力容器　個別検定申請書　データセット                     ***
   //***                                                                                         ***
   //***********************************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "個別検定申請書");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("個別検定申請書"));

   //*******************
   //***  製造番号
   //*******************
   // 『A001』
   //2004.12.21 注記変更
   //s_Text = "No." + G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   //Excel_Cell_DataSet(4, 1, s_Text);
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E   
   Excel_Cell_DataSet(3, 1, s_Text);
   //*******************
   G_Log_Text = "製造番号            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  台  数
   //*******************
   // 『A002』
   i_RecNo = 39;
   s_Text = G_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(6, 12, s_Text);
      G_Log_Text = "台  数               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "台数      RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  使用圧力　被加熱側
   //***************************
   // 『A003』
   i_RecNo = 1098;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if ( G_HEADS_DATA[i_RecNo].ToDouble() >= 999.0 ) {
       s_Text = "大気開放";
     } else {
       s_Text = FormatFloat("0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(7, 5, s_Text);
   G_Log_Text = "使用圧力 非加熱側    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  使用圧力　熱源側
   //***************************
   // 『A004』
   i_RecNo = 1100;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if ( G_HEADS_DATA[i_RecNo].ToDouble() >= 999.0 ) {
       s_Text = "大気開放";
     } else {
       s_Text = FormatFloat("0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   }else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(8, 5, s_Text);
   G_Log_Text = "使用圧力 熱源側      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  内容積　非加熱側,加熱側
   //*********************************
   // 『A005』,『A006』
   double d_HOLD_Value;

   if (StrToDblDef(G_HEADS_DATA[852].TrimRight(),0) >= StrToDblDef(G_HEADS_DATA[853].TrimRight(),0)) {
      //******************************************
      //***  (B入口温度) >= (A入口温度)のとき
      //******************************************

      // 内容積 被加熱側
      i_RecNo = 1283;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(7, 11, s_Text);
         G_Log_Text = "内容積 被加熱側      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "内容積 被加熱側  RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }

      // 内容積 熱源側
      i_RecNo = 1282;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(8, 11, s_Text);
         G_Log_Text = "内容積 熱源側        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "内容積 熱源側    RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }

   } else {
      //******************************************
      //***  (B入口温度) < (A入口温度)のとき
      //******************************************

      // 内容積 被加熱側
      i_RecNo = 1282;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(7, 11, s_Text);
         G_Log_Text = "内容積 被加熱側      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "内容積 被加熱側  RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }

      // 内容積 熱源側
      i_RecNo = 1283;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(8, 11, s_Text);
         G_Log_Text = "内容積 熱源側        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "内容積 熱源側    RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }
   }

   //***************************
   //***  ﾌﾚｰﾑの厚さ
   //***************************
   // 『A07』
//   i_RecNo = 1284;
   i_RecNo = 1106;      // 強度計算書のフレーム厚とあわせる
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2004.12.21 桁数変更
   double d_Text_Value;
   d_Text_Value = s_Text.ToDouble();
   d_Text_Value = Hasu_Round(0, d_Text_Value , 0);
   s_Text = FormatFloat("#,###,##0",d_Text_Value);
   //*******************
   Excel_Cell_DataSet(9, 2, s_Text);
   G_Log_Text = "ﾌﾚｰﾑの厚さ           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾟﾚｰﾄの大きさ
   //***************************
   // 『A008』
   i_RecNo = 1268;
   s_Text  = G_HEADS_DATA[i_RecNo].TrimRight() + "×";
   i_RecNo = 1269;
   s_Text  = s_Text + G_HEADS_DATA[i_RecNo].TrimRight() + "×";
   i_RecNo = 1270;
   s_Text  = s_Text + G_HEADS_DATA[i_RecNo].TrimRight();

   Excel_Cell_DataSet(9, 10, s_Text);
   G_Log_Text = "ﾌﾟﾚｰﾄの大きさ        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //******************************
   //***  Excel Book Close
   //******************************
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
   G_Log_Text = "--------------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『小型圧力容器  個別検定申請書』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}