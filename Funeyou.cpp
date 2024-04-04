//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：舶用要目表  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.05.30
//　 更 新 日：2002.05.30
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Funeyou.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 舶要目表　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Funeyou(void)
{
   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_Text;

   s_Tmplate_Pass = G_Template_Dir + "舶用要目表.xls";      // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "M" + G_SAKUBAN + ".xls";  // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "----------    『舶用要目表』作成開始    ----------";
   Write_Log(G_Log_Text);
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
   exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
   // ｼｰﾄｺﾚｸｼｮﾝ取得
   exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));

   //******************************
   //***  データ　セット（和文）
   //******************************
   Funeyou_Data_Set("J");


   //******************************
   //***  Excel Book Close
   //******************************
   exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ﾛｸﾞ作成
   G_Log_Text = "----------    『舶用要目表』作成終了    ----------";
   Write_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 舶要目表　Excelデータセット
// 概  要      ：
// 引  数      ： AnsiString Lang  // 言語(J:和文，E:英文)
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Funeyou_Data_Set(AnsiString Lang)
{
   AnsiString s_Text;

   // ｼｰﾄの指定
   if (Lang == "J"){
      exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "和文");
   } else {
      exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "英文");
   }

   //***  DOC.No.  ***
   s_Text = "M" + G_SAKUBAN;
   Excel_Cell_DataSet(1, 13, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：DOC.No.      『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：DOC.No.      『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  MFG.No.  ***
   //2022/12/15 桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 桁数追加対応 Mod_E
      
   Excel_Cell_DataSet(3, 1, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：MFG.No.      『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：MFG.No.      『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  型式  ***
   if (G_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = G_HEADS_DATA[540].TrimRight() + "（改）";
   } else {
      s_Text = G_HEADS_DATA[540].TrimRight();
   }
   Excel_Cell_DataSet(3, 3, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：型式         『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：型式         『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  台数  ***
   s_Text = G_HEADS_DATA[39];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(2, 11, s_Text);
      if (Lang == "J"){
         G_Log_Text = "和文：台数         『" + s_Text + "』をセット。";
      } else {
         G_Log_Text = "英文：台数         『" + s_Text + "』をセット。";
      }
      Write_Log(G_Log_Text);
   } else {
      if (Lang == "J"){
         G_ErrLog_Text = "和文：台数      不正な値『" + s_Text + "』 です。";
      } else {
         G_ErrLog_Text = "英文：台数      不正な値『" + s_Text + "』 です。";
      }
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***  適用企画？  ***
   s_Text = G_HEADS_DATA[29].TrimRight();
   Excel_Cell_DataSet(5, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：適用企画     『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：適用企画     『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  船番  ***
   s_Text = G_HEADS_DATA[777].TrimRight();
   Excel_Cell_DataSet(6, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：船番         『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：船番         『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  伝熱面積  ***
   s_Text = G_HEADS_DATA[1067].TrimRight();
   Excel_Cell_DataSet(7, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：伝熱面積     『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：伝熱面積     『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  機器質量 DRY  ***
   s_Text = G_HEADS_DATA[1029].TrimRight();
   Excel_Cell_DataSet(8, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：機器質量 DRY 『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：機器質量 DRY 『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  機器質量 DRY  ***
   s_Text = G_HEADS_DATA[1030].TrimRight();
   Excel_Cell_DataSet(9, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：機器質量 WET 『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：機器質量 WET 『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  B側液体名  ***
   s_Text = G_HEADS_DATA[552].TrimRight();
   Excel_Cell_DataSet(11, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：B側液体名    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：B側液体名    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  A側液体名  ***
   s_Text = G_HEADS_DATA[583].TrimRight();
   Excel_Cell_DataSet(11, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：A側液体名    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：A側液体名    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  B入口温度  ***
   s_Text = G_HEADS_DATA[852].TrimRight();
   Excel_Cell_DataSet(12, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：B入口温度    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：B入口温度    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  A入口温度  ***
   s_Text = G_HEADS_DATA[853].TrimRight();
   Excel_Cell_DataSet(12, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：A入口温度    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：A入口温度    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  B出口温度  ***
   s_Text = G_HEADS_DATA[854].TrimRight();
   Excel_Cell_DataSet(13, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：B出口温度    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：B出口温度    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  A出口温度  ***
   s_Text = G_HEADS_DATA[855].TrimRight();
   Excel_Cell_DataSet(13, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：A出口温度    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：A出口温度    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  B流量  ***
   s_Text = G_HEADS_DATA[856].TrimRight();
   Excel_Cell_DataSet(14, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：B流量        『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：B流量        『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  A流量  ***
   s_Text = G_HEADS_DATA[857].TrimRight();
   Excel_Cell_DataSet(14, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：A流量        『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：A流量        『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);


   //***  B圧力損失  ***
   s_Text = G_HEADS_DATA[858].TrimRight();
   Excel_Cell_DataSet(15, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：B圧力損失    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：B圧力損失    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  A圧力損失  ***
   s_Text = G_HEADS_DATA[859].TrimRight();
   Excel_Cell_DataSet(15, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：A圧力損失    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：A圧力損失    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  B設計圧力  ***
   s_Text = G_HEADS_DATA[825].TrimRight();
   Excel_Cell_DataSet(16, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：B設計圧力    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：B設計圧力    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  A設計圧力  ***
   s_Text = G_HEADS_DATA[830].TrimRight();
   Excel_Cell_DataSet(16, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：A設計圧力    『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：A設計圧力    『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  Bテスト圧力  ***
   s_Text = G_HEADS_DATA[827].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(17, 4, s_Text);
      if (Lang == "J"){
         G_Log_Text = "和文：Bテスト圧力  『" + s_Text + "』をセット。";
      } else {
         G_Log_Text = "英文：Bテスト圧力  『" + s_Text + "』をセット。";
      }
      Write_Log(G_Log_Text);

   } else {
      if (Lang == "J"){
         G_ErrLog_Text = "和文：Bテスト圧力  不正な値『" + s_Text + "』 です。";
      } else {
         G_ErrLog_Text = "英文：Bテスト圧力  不正な値『" + s_Text + "』 です。";
      }
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***  Aテスト圧力  ***
   s_Text = G_HEADS_DATA[832].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(17, 10, s_Text);
      if (Lang == "J"){
         G_Log_Text = "和文：Aテスト圧力  『" + s_Text + "』をセット。";
      } else {
         G_Log_Text = "英文：Aテスト圧力  『" + s_Text + "』をセット。";
      }
      Write_Log(G_Log_Text);

   } else {
      if (Lang == "J"){
         G_ErrLog_Text = "和文：Aテスト圧力  不正な値『" + s_Text + "』 です。";
      } else {
         G_ErrLog_Text = "英文：Aテスト圧力  不正な値『" + s_Text + "』 です。";
      }
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***  Bﾎｰﾙﾄﾞ量  ***
   s_Text = G_HEADS_DATA[1282].TrimRight();
   Excel_Cell_DataSet(18, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：Bﾎｰﾙﾄﾞ量     『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：Bﾎｰﾙﾄﾞ量     『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   //***  Aﾎｰﾙﾄﾞ量  ***
   s_Text = G_HEADS_DATA[1283].TrimRight();
   Excel_Cell_DataSet(18, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "和文：Aﾎｰﾙﾄﾞ量     『" + s_Text + "』をセット。";
   } else {
      G_Log_Text = "英文：Aﾎｰﾙﾄﾞ量     『" + s_Text + "』をセット。";
   }
   Write_Log(G_Log_Text);

   return true;
}
