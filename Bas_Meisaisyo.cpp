//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：小型圧力容器 明細書 作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.06.04
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

#include "Bas_Meisaisyo.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 小型圧力容器 明細書　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Meisaisyo(HeadsData* pcHeadsData)
{
   // 2021.06.18 FireDACへの移行 MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDACへの移行 MOD_E

   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_Text;            // 汎用文字列
   AnsiString s_SQL;             // SQL文
   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo

   //------------------------------------
   // 2007.10.10 ﾐﾗｰ仕様対応
   // 元仕様以外は処理不要(正常終了)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   s_Tmplate_Pass = G_Template_Dir + "小型圧力容器明細書.xls";          // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "PVI" + G_SAKUBAN + ".xls";    // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "PVI" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";    // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『小型圧力容器  明細書』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------------------";
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

   //******************************************************************************
   //***                                                                        ***
   //***                          小型圧力容器　明細書                          ***
   //***                                                                        ***
   //******************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "明細書");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("明細書"));

   //*******************
   //***  製造番号
   //*******************
   // 『A001』
   //2004.12.21 注記変更
   //s_Text = "No." + G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   //Excel_Cell_DataSet(3, 1, s_Text);
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E   
   Excel_Cell_DataSet(2, 1, s_Text);
   //*******************
   G_Log_Text = "製造番号            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  使用圧力　被加熱側
   //***************************
   // 『A002』
   i_RecNo = 1098;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if (G_HEADS_DATA[i_RecNo].ToDouble() >= 999.0) {
       s_Text = "大気開放";
     } else {
       s_Text = FormatFloat( "0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   //2021.12.09 テンプレート変更_S
   //Excel_Cell_DataSet(10, 7, s_Text);
   Excel_Cell_DataSet(10, 10, s_Text);
   //2021.12.09 テンプレート変更_E
   G_Log_Text = "使用圧力 非加熱側    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  使用圧力　熱源側
   //***************************
   // 『A003』
   i_RecNo = 1100;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if (G_HEADS_DATA[i_RecNo].ToDouble() >= 999) {
       s_Text = "大気開放";
     } else {
       s_Text = FormatFloat( "0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   //2021.12.09 テンプレート変更_S
   //Excel_Cell_DataSet(11, 7, s_Text);
   Excel_Cell_DataSet(11, 10, s_Text);
   //2021.12.09 テンプレート変更_E
   G_Log_Text = "使用圧力 熱源側      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  内容積　非加熱側,加熱側
   //*********************************
   // 『A004』,『A005』
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
         Excel_Cell_DataSet(13, 10, s_Text);
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
         Excel_Cell_DataSet(14, 10, s_Text);
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
         Excel_Cell_DataSet(13, 10, s_Text);
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
         Excel_Cell_DataSet(14, 10, s_Text);
         G_Log_Text = "内容積 熱源側        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "内容積 熱源側    RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }
   }

   //***************************
   //***  ﾌﾟﾚｰﾄの材料
   //***************************
   // 『A006』
   i_RecNo = 43;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "ﾌﾟﾚｰﾄの材料          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";
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
   //2004.12.21 半角に変更
   s_Text = ZenToHan(s_Text);
   //*********************
   Excel_Cell_DataSet(16, 4, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾟﾚｰﾄ幅
   //***************************
   // 『A007』
   i_RecNo = 1268;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 11, s_Text);
   G_Log_Text = "ﾌﾟﾚｰﾄ幅              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾟﾚｰﾄ長
   //***************************
   // 『A008』
   i_RecNo = 1269;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 14, s_Text);
   G_Log_Text = "ﾌﾟﾚｰﾄ長              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾟﾚｰﾄ厚
   //***************************
   // 『A009』
   i_RecNo = 1270;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 17, s_Text);
   G_Log_Text = "ﾌﾟﾚｰﾄ厚              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑの材料
   //***************************
   // 『A010』
   switch (G_HEADS_DATA[37].ToIntDef(0)) {
      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:           //Ｂシリーズ
         s_Text = G_HEADS_DATA[34].TrimRight();
         if ((s_Text.SubString(1,2) == "UX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==0) ||       //UX-01
             (s_Text.SubString(1,2) == "LX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ) {     //LX-10
            //2004.12.21 半角に変更
            //s_Text = "ＳＵＳ３０４";
            s_Text = "SUS304";
            //*********************
         } else {
            //2004.12.21 半角に変更
            //s_Text = "ＳＳ４００";
            s_Text = "SS400";
            //*********************
         }

         break;
      default :
         i_RecNo = 1044;   // SPFRM
         s_Text = G_HEADS_DATA[i_RecNo].TrimRight();

         if (s_Text == "0000" || s_Text == "1040"){
            //2004.12.21 半角に変更
            //s_Text = "ＳＳ４００";
            s_Text = "SS400";
            //*********************
         } else {
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
               //2004.12.21 半角に変更
               s_Text = ZenToHan(s_Text);
               //*********************
            }
         }

         break;
   }

   Excel_Cell_DataSet(18, 4, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ材料              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ厚
   //***************************
   // 『A011』
//   i_RecNo = 1284;
   i_RecNo = 1106;            // 強度計算書のフレーム厚と合わせる
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2004.12.21 桁数変更
   double d_Text_Value;
   d_Text_Value = s_Text.ToDouble();
   d_Text_Value = Hasu_Round(0, d_Text_Value , 0);
   s_Text = FormatFloat("#,###,##0",d_Text_Value);
   //*******************
   Excel_Cell_DataSet(18, 14, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ厚               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ締付ﾎﾞﾙﾄ材料
   //***************************
   // 『A012』
//   i_RecNo = 306;
//   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1041;   // 材質指定
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if (( s_Text != "" ) && ( s_Text != "0000" ) ){
      s_Text  = Search_HD_MATERIAL(s_Text);
   } else {
      i_RecNo = 1021;   // BOL22
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(20, 4, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ締付ﾎﾞﾙﾄ材料     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ締付ﾎﾞﾙﾄ呼び径
   //***************************
   // 『A013』
   i_RecNo = 1019;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "ﾌﾚｰﾑ締付ﾎﾞﾙﾄ呼び径   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";
   if (s_Text == "42"){
      //2004.01.22
      //s_Text = "1 5/8-UN";
      s_Text = "1 5/8""-8UN";
      //**********
   } else if (s_Text == "52"){
      //2004.01.22
      //s_Text = "2-8UN";
      s_Text = "2""-8UN";
      //**********
   } else {
      s_Text = "M" + FormatFloat("0",s_Text.ToDouble());
   }

   Excel_Cell_DataSet(20, 11, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ締付ﾎﾞﾙﾄ数
   //***************************
   // 『A014』
   i_RecNo = 1020;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = s_Text.ToDouble() * 2;
      Excel_Cell_DataSet(20, 17, s_Text);
      G_Log_Text = "ﾌﾚｰﾑ締付ﾎﾞﾙﾄ数       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "ﾌﾚｰﾑ締付ﾎﾞﾙﾄ数   RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //*********************************
   //***  水圧試験 非加熱側,熱源側　
   //*********************************
   // 『A015』,『A016』
   if (StrToDblDef(G_HEADS_DATA[852].TrimRight(),0) >= StrToDblDef(G_HEADS_DATA[853].TrimRight(),0)) {
      //******************************************
      //***  (B入口温度) >= (A入口温度)のとき
      //******************************************

      // 水圧試験 被加熱側
      i_RecNo = 832;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(29, 6, s_Text);
         G_Log_Text = "水圧試験 被加熱側    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "水圧試験 被加熱側  RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }

      // 水圧試験 熱源側
      i_RecNo = 827;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(30, 6, s_Text);
         G_Log_Text = "水圧試験 熱源側      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "水圧試験 熱源側  RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }

   } else {
      //******************************************
      //***  (B入口温度) < (A入口温度)のとき
      //******************************************

      // 水圧試験 被加熱側
      i_RecNo = 827;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(29, 6, s_Text);
         G_Log_Text = "水圧試験 被加熱側    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "水圧試験 被加熱側  RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }

      // 水圧試験 熱源側
      i_RecNo = 832;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(30, 6, s_Text);
         G_Log_Text = "水圧試験 熱源側      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "水圧試験 熱源側  RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }
   }

   //************************
   //***  HEADS VER
   //************************
   // 『A017』
   //2004.12.21 HEADS VER削除
   //i_RecNo = 302;
   //if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
   //   i_RecNo = 1267;
   //   s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   //} else {
   //   i_RecNo = 1267;
   //   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //}
   //Excel_Cell_DataSet(40, 19, s_Text);
   //G_Log_Text = "HEADS VER           『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //************************

   //Windows 2008Server 移植対応
   wkQuery->Close();
   delete wkQuery;
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
   G_Log_Text = "------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『小型圧力容器  明細書』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //delete wkQuery;

   return true;
}