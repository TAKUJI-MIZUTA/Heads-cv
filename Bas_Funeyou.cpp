	//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：舶用要目表  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.06.04
//　 更 新 日：2002.08.23
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Funeyou.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 舶要目表　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Funeyou(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_Tosou_CD;        // 塗装色ｺｰﾄﾞ
   AnsiString s_Text;            //
   AnsiString s_WkText1;
   AnsiString s_WkText2;

   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   int i_Value;
   double     d_BpP;            // B試験圧力
   double     d_BpH;            // B試験水圧
   double     d_ApP;            // A試験圧力
   double     d_ApH;            // A試験水力

   bool bRet;
   AnsiString s_HEADS_DATA[1500];

   // 元仕様でない場合は処理不要(正常終了)
   if(!pcHeadsData->GetSpecType()==DEF_SPEC_TYPE_NONE) return(true);

   // HEADSﾃﾞｰﾀをﾛｰｶﾙ変数にｾｯﾄ
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   s_Tmplate_Pass = G_Template_Dir + "舶用要目表.xls";               // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "M" + G_SAKUBAN + ".xls";   // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "M" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";   // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『舶用要目表』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "作成ファイル名：      『" + s_MakeExcel_Pass + "』";
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
      G_Log_Text = "ファイルコピー成功。  『" + s_Tmplate_Pass +
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

   //**********************************************************************************
   //***                                                                            ***
   //***                    舶用要目表(和文)　データセット                          ***
   //***                                                                            ***
   //**********************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "和文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   //*********************
   //***  和文：DOC.No.
   //*********************
   // 『A001』
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_Text = "M" + G_SAKUBAN;
   s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
   Excel_Cell_DataSet(1, 13, s_Text);
   G_Log_Text = "和文：DOC.No.         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  和文：MFG.No.
   //*********************
   // 『A002』
   //2022/12/15 桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 桁数追加対応 Mod_E

   //-------------------------------------
   // 2007.10.09 ﾐﾗｰ仕様対応 "A/B"を付加
   if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
   }
   // 2007.10.09
   //-------------------------------------

   Excel_Cell_DataSet(3, 1, s_Text);
   G_Log_Text = "和文：MFG.No.         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  和文：型式
   //*********************
   // 『A003』
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(2, 4, s_Text);
   //Excel_Cell_DataSet(3, 3, s_Text);
   //*********************
   G_Log_Text = "和文：型式             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //*********************
   //***  和文：EQPT.NAME
   //*********************
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(3, 4, s_Text);
   G_Log_Text = "和文：EQPT.NAME        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*********************

   //*********************
   //***  和文：台数
   //*********************
   // 『A004』
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(2, 13, s_Text);
      G_Log_Text = "和文：台数             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "和文：台数      不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  和文：適用規格
   //************************
   // 『A005』
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "和文：適用規格         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』により、";
   if ( s_Text.ToIntDef(0) == 0 ) {
      s_Text = "";
   } else {
      s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,true, s_HEADS_DATA[1].TrimRight());
   }
   Excel_Cell_DataSet(5, 4, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  和文：船番
   //************************
   // 『A006』
   i_RecNo = 270;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 4, s_Text);
   G_Log_Text = "和文：船番             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  和文：伝熱面積
   //************************
   // 『A007』
   i_RecNo = 1067;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 4, s_Text);
   G_Log_Text = "和文：伝熱面積         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //************************
   //***  和文：交換熱量
   //************************
   i_RecNo = 107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(8, 4, s_Text);
   G_Log_Text = "和文：交換熱量         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  和文：機器質量 DRY
   //****************************
   // 『A008』
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(9, 4, s_Text);
   //Excel_Cell_DataSet(8, 4, s_Text);
   //*********************
   G_Log_Text = "和文：機器質量 DRY     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：機器質量 WET
   //****************************
   // 『A009』
   i_RecNo = 1030;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(10, 4, s_Text);
   //Excel_Cell_DataSet(9, 4, s_Text);
   //*********************
   G_Log_Text = "和文：機器質量 WET     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   ///****************************
   //***  和文：B側液体名
   //****************************
   // 『A010』
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
//      i_RecNo = 76;     // Ａ流体名
//   } else {
//      i_RecNo = 45;     // Ｂ流体名
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1071].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(12, 4, s_Text);
   //Excel_Cell_DataSet(11, 4, s_Text);
   //*********************
   G_Log_Text = "和文：B側液体名        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：A側液体名
   //****************************
   // 『A011』
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
//      i_RecNo = 45;     // Ｂ流体名
//   } else {
//      i_RecNo = 76;     // Ａ流体名
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1073].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(12, 10, s_Text);
   //Excel_Cell_DataSet(11, 10, s_Text);
   //*********************
   G_Log_Text = "和文：A側液体名        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：B入口温度
   //****************************
   // 『A012』
   i_RecNo = 852;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(13, 4, s_Text);
   //Excel_Cell_DataSet(12, 4, s_Text);
   //*********************
   G_Log_Text = "和文：B入口温度        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：A入口温度
   //****************************
   // 『A013』
   i_RecNo = 853;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(13, 10, s_Text);
   //Excel_Cell_DataSet(12, 10, s_Text);
   //*********************
   G_Log_Text = "和文：A入口温度        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：B出口温度
   //****************************
   i_RecNo = 854;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(14, 4, s_Text);
   //Excel_Cell_DataSet(13, 4, s_Text);
   //*********************
   G_Log_Text = "和文：B出口温度        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：A出口温度
   //****************************
   i_RecNo = 855;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(14, 10, s_Text);
   //Excel_Cell_DataSet(13, 10, s_Text);
   //*********************
   G_Log_Text = "和文：A出口温度        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.20 プラン設定による可変文字列対応
   s_WkText1 = s_HEADS_DATA[846].TrimRight();  // Bスチーム区分(CASE1)
   s_WkText2 = s_HEADS_DATA[847].TrimRight();  // Aスチーム区分(CASE1)
   if( s_WkText1 == "0" && s_WkText2 == "0" ){
      // 液-液：[m3/h]
      Excel_Cell_DataSet(15, 1,"流量[m3/h]");
   }else if( s_WkText1 == "1" && s_WkText2 == "0" ){
      // 蒸気-液：[kg/h m3/h]
      Excel_Cell_DataSet(15, 1,"流量[kg/h m3/h]");
   }else if( s_WkText1 == "0" && s_WkText2 == "1" ){
      // 液-蒸気：[m3/h kg/h]
      Excel_Cell_DataSet(15, 1,"流量[m3/h kg/h]");
   }
   //-------------------------------------------------


   //*********************
   //***  和文：B流量
   //*********************
   i_RecNo = 856;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(15, 4, s_Text);
   //Excel_Cell_DataSet(14, 4, s_Text);
   //*********************
   G_Log_Text = "和文：B流量            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  和文：A流量
   //*********************
   i_RecNo = 857;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(15, 10, s_Text);
   //Excel_Cell_DataSet(14, 10, s_Text);
   //*********************
   G_Log_Text = "和文：A流量            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：B圧力損失
   //****************************
   i_RecNo = 858;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 桁数変更
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(16, 4, s_Text);
   //Excel_Cell_DataSet(15, 4, s_Text);
   //*********************
   G_Log_Text = "和文：B圧力損失        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：A圧力損失
   //****************************
   i_RecNo = 859;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 桁数変更
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(16, 10, s_Text);
   //Excel_Cell_DataSet(15, 10, s_Text);
   //*********************
   G_Log_Text = "和文：A圧力損失        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：B設計圧力
   //****************************
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "大気開放";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(17, 4, s_Text);
   //Excel_Cell_DataSet(16, 4, s_Text);
   //*********************
   G_Log_Text = "和文：B設計圧力        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：A設計圧力
   //****************************
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "大気開放";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(17, 10, s_Text);
   //Excel_Cell_DataSet(16, 10, s_Text);
   //*********************
   G_Log_Text = "和文：A設計圧力        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //****************************
   //***  和文：B設計温度
   //****************************
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 4, s_Text);
   G_Log_Text = "和文：B設計温度        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：A設計温度
   //****************************
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "和文：A設計温度        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  和文：Bテスト圧力
   //****************************
   //****************************
   //***  和文：Aテスト圧力
   //****************************
   s_Text = s_HEADS_DATA[62].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpP = s_Text.ToDouble();
   } else {
      d_BpP = 0.0;
   }
   s_Text = s_HEADS_DATA[61].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpH = s_Text.ToDouble();
   } else {
      d_BpH = 0.0;
   }
   s_Text = s_HEADS_DATA[93].Trim();   // Ａ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApP = s_Text.ToDouble();
   } else {
      d_ApP = 0.0;
   }
   s_Text = s_HEADS_DATA[92].Trim();   // Ａ試験水力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApH = s_Text.ToDouble();
   } else {
      d_ApH = 0.0;
   }
   if( d_BpP > 0.0 || d_ApP > 0.0) {         // Ｂ試験圧力＞０又はＡ試験圧力＞０
      if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
         // 両方の条件が成立する場合はテスト圧力項目を空欄とする
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 1, "");
         Excel_Cell_DataSet(19, 4, "");
         Excel_Cell_DataSet(19, 10, "");
         //Excel_Cell_DataSet(17, 1, "");
         //Excel_Cell_DataSet(17, 4, "");
         //Excel_Cell_DataSet(17, 10, "");
         //*********************
         G_ErrLog_Text = "和文：Bテスト圧力  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      } else {
         //2004.05.21 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 1, "ﾃｽﾄ圧力（空圧）[MPaG]");
         //Excel_Cell_DataSet(17, 1, "ﾃｽﾄ圧力（空圧）[MPaG]");
         //*********************
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB逆ではない
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "和文：Aテスト圧力      RecNo:61『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);
         } else {
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "和文：Bテスト圧力      RecNo:62『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "和文：Aテスト圧力      RecNo:61『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);
         }
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
      //2004.05.21 ﾌｫｰﾏｯﾄ訂正
      Excel_Cell_DataSet(19, 1, "ﾃｽﾄ圧力（水圧）[MPaG]");
      //Excel_Cell_DataSet(17, 1, "ﾃｽﾄ圧力（水圧）[MPaG]");
      //*********************
      if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB逆ではない
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "和文：Bテスト圧力      RecNo:93『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "和文：Bテスト圧力      RecNo:92『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);
      } else {
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "和文：Bテスト圧力      RecNo:93『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "和文：Bテスト圧力      RecNo:92『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      //2004.04.26 ﾌｫｰﾏｯﾄ訂正
      Excel_Cell_DataSet(19, 1, "");
      Excel_Cell_DataSet(19, 4, "");
      Excel_Cell_DataSet(19, 10, "");
      //Excel_Cell_DataSet(17, 1, "");
      //Excel_Cell_DataSet(17, 4, "");
      //Excel_Cell_DataSet(17, 10, "");
      //*********************
      G_ErrLog_Text = "和文：Bテスト圧力  該当する数値がありません。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }


   //****************************
   //***  和文：Bﾎｰﾙﾄﾞ量
   //****************************
   i_RecNo = 1282;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(20, 4, s_Text);
   //Excel_Cell_DataSet(18, 4, s_Text);
   //*********************
   G_Log_Text = "和文：Bﾎｰﾙﾄﾞ量         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：Aﾎｰﾙﾄﾞ量
   //****************************
   i_RecNo = 1283;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(20, 10, s_Text);
   //Excel_Cell_DataSet(18, 10, s_Text);
   //*********************
   G_Log_Text = "和文：Aﾎｰﾙﾄﾞ量         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // Ｊ型はチェーン（移動止め）不要
   if(s_HEADS_DATA[36].Pos("J") != 0) {
      //2004.04.26 ﾌｫｰﾏｯﾄ訂正
      Excel_Cell_DataSet(26, 4, "----");
      //Excel_Cell_DataSet(24, 4, "----");
      //*********************
   }


   //-------------------------------------------------
   // OGI 2015.02.19 アキュームレータ部品対応
   // 窒素ガス充填工具、窒素ガス充填圧力[MPa]、動作圧力【MPa】、アキュームレータ の追加対応

   //***************************
   //*** 和文：窒素ガス充填工具
   //***************************
   s_Text = s_HEADS_DATA[1117].TrimRight();
   if (s_Text == "1") {
      Excel_Cell_DataSet(27, 4, "1");
   } else {
      Excel_Cell_DataSet(27, 4, "----");
   }

   //***************************
   //*** 和文：窒素ガス充填圧力[MPa]
   //***************************
   Excel_Cell_DataSet(29, 4, "----");

   //***************************
   //*** 和文：動作圧力【MPa】
   //***************************
   Excel_Cell_DataSet(30, 4, "----");

   //***************************
   //***  和文：アキュームレータ
   //***************************
   AnsiString s_Nzl_SyuruiE3 = s_HEADS_DATA[981].TrimRight();
   AnsiString s_Nzl_SyuruiE4 = s_HEADS_DATA[996].TrimRight();
   if (s_Nzl_SyuruiE3 == "8" || s_Nzl_SyuruiE4 == "8"){
      Excel_Cell_DataSet(30, 12, "1");
   } else {
      Excel_Cell_DataSet(30, 12, "----");
   }


   //****************************
   //***  和文：ｴｱｰ抜き弁
   //****************************
   i_Value = 0;
   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //if( (s_HEADS_DATA[905].SubString(1,1) == "7")&&(s_HEADS_DATA[905].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[920].SubString(1,1) == "7")&&(s_HEADS_DATA[920].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[965].SubString(1,1) == "7")&&(s_HEADS_DATA[965].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[980].SubString(1,1) == "7")&&(s_HEADS_DATA[980].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[905].SubString(1,2) == "07")
    && (s_HEADS_DATA[905].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[920].SubString(1,2) == "07")
    && (s_HEADS_DATA[920].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[965].SubString(1,2) == "07")
    && (s_HEADS_DATA[965].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[980].SubString(1,2) == "07")
    && (s_HEADS_DATA[980].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(23, 12, s_Text);
   //Excel_Cell_DataSet(21, 12, s_Text);
   //*********************
   G_Log_Text = "和文：ｴｱｰ抜き弁       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：ﾄﾞﾚﾝ抜き弁
   //****************************
   i_Value = 0;
   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //if( (s_HEADS_DATA[935].SubString(1,1) == "7")&&(s_HEADS_DATA[935].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[950].SubString(1,1) == "7")&&(s_HEADS_DATA[950].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[995].SubString(1,1) == "7")&&(s_HEADS_DATA[995].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[1010].SubString(1,1) == "7")&&(s_HEADS_DATA[1010].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[935].SubString(1,2) == "07")
    && (s_HEADS_DATA[935].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[950].SubString(1,2) == "07")
    && (s_HEADS_DATA[950].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[995].SubString(1,2) == "07")
    && (s_HEADS_DATA[995].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[1010].SubString(1,2) == "07")
    && (s_HEADS_DATA[1010].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(24, 12, s_Text);
   //Excel_Cell_DataSet(22, 12, s_Text);
   //*********************
   G_Log_Text = "和文：ﾄﾞﾚﾝ抜き弁      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：銘板
   //****************************
   s_Text = "1";
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(25, 12, s_Text);
   //Excel_Cell_DataSet(23, 12, s_Text);
   //*********************
   G_Log_Text = "和文：銘版            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.03.14 Ｌ型温度計の判定基準に”11”を追加
   //****************************
   //***  和文：温度計 L型
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() == "06" || s_HEADS_DATA[898].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() == "06" || s_HEADS_DATA[913].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() == "06" || s_HEADS_DATA[928].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() == "06" || s_HEADS_DATA[943].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() == "06" || s_HEADS_DATA[958].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() == "06" || s_HEADS_DATA[973].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() == "06" || s_HEADS_DATA[988].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() == "06" || s_HEADS_DATA[1003].TrimRight() == "11"){
      i_Value++;
   }
   //-------------------------------------------------

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(26, 13, s_Text);
   //Excel_Cell_DataSet(24, 13, s_Text);
   //*********************
   G_Log_Text = "和文：温度計 L型      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：温度計 I型
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() != "06" && s_HEADS_DATA[898].TrimRight() != "07" && s_HEADS_DATA[898].TrimRight() != "11" && s_HEADS_DATA[898].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() != "06" && s_HEADS_DATA[913].TrimRight() != "07" && s_HEADS_DATA[913].TrimRight() != "11" && s_HEADS_DATA[913].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() != "06" && s_HEADS_DATA[928].TrimRight() != "07" && s_HEADS_DATA[928].TrimRight() != "11" && s_HEADS_DATA[928].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() != "06" && s_HEADS_DATA[943].TrimRight() != "07" && s_HEADS_DATA[943].TrimRight() != "11" && s_HEADS_DATA[943].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() != "06" && s_HEADS_DATA[958].TrimRight() != "07" && s_HEADS_DATA[958].TrimRight() != "11" && s_HEADS_DATA[958].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() != "06" && s_HEADS_DATA[973].TrimRight() != "07" && s_HEADS_DATA[973].TrimRight() != "11" && s_HEADS_DATA[973].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() != "06" && s_HEADS_DATA[988].TrimRight() != "07" && s_HEADS_DATA[988].TrimRight() != "11" && s_HEADS_DATA[988].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() != "06" && s_HEADS_DATA[1003].TrimRight() != "07" && s_HEADS_DATA[1003].TrimRight() != "11" && s_HEADS_DATA[1003].ToIntDef(0) > 0){
      i_Value++;
   }


   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(27, 13, s_Text);
   //Excel_Cell_DataSet(25, 13, s_Text);
   //*********************
   G_Log_Text = "和文：温度計 I型      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：ｽﾄﾚｰﾅ
   //****************************
   if (s_HEADS_DATA[891].ToIntDef(0) != 5 && s_HEADS_DATA[906].ToIntDef(0) != 5
                                          && s_HEADS_DATA[921].ToIntDef(0) != 5
                                          && s_HEADS_DATA[936].ToIntDef(0) != 5
                                          && s_HEADS_DATA[951].ToIntDef(0) != 5
                                          && s_HEADS_DATA[966].ToIntDef(0) != 5
                                          && s_HEADS_DATA[981].ToIntDef(0) != 5
                                          && s_HEADS_DATA[996].ToIntDef(0) != 5){

      s_Text = "----";
   } else {
      s_Text = "1";
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(28, 12, s_Text);
   //Excel_Cell_DataSet(26, 12, s_Text);
   //*********************
   G_Log_Text = "和文：ｽﾄﾚｰﾅ           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：二針式圧力計
   //****************************
   i_Value=0;
   //2015.04.28 数量変更
   //if (s_HEADS_DATA[901].ToIntDef(0) == 16 || s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[916].ToIntDef(0) == 16 || s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[961].ToIntDef(0) == 16 || s_HEADS_DATA[961].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[976].ToIntDef(0) == 16 || s_HEADS_DATA[976].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[901].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[931].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[946].ToIntDef(0) == 17 )  i_Value++;
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_S
   if (s_HEADS_DATA[901].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_E
   i_Value = i_Value / 2;
   //*******************
   if (i_Value == 0 ) {
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(29, 12, s_Text);
   //Excel_Cell_DataSet(27, 12, s_Text);
   //*********************
   G_Log_Text = "和文：二針式圧力計           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.20 プラン設定による可変文字列対応
   s_WkText1 = s_HEADS_DATA[901].TrimRight();  // S1圧力計種類
   s_WkText2 = s_HEADS_DATA[916].TrimRight();  // S2圧力計種類
   if( s_WkText1 == "16" || s_WkText2 == "16" ){
      Excel_Cell_DataSet(29, 7, "一針式圧力計");
      //2015.04.28 数量変更
      //Excel_Cell_DataSet(29, 12, "2");
      //*******************
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_S
   } else if ( s_WkText1 == "21" || s_WkText2 == "21" ){
      Excel_Cell_DataSet(29, 7, "圧力計");
   } else if ( s_WkText1 == "22" || s_WkText2 == "22" ){
      Excel_Cell_DataSet(29, 7, "圧力計");
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_E
   } else {
      Excel_Cell_DataSet(29, 7, "二針式圧力計");
   }
   //-------------------------------------------------


   //****************************
   //***  和文：塗装色
   //****************************
   i_RecNo = 31;
   s_Tosou_CD = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Tosou_CD.ToIntDef(0) == 1){
      // RecNo:539 塗装色をｾｯﾄ
      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   } else if (s_Tosou_CD.ToIntDef(0) == 3 || s_Tosou_CD.ToIntDef(0) == 5) {
      // 塗装色ｺｰﾄﾞﾃｰﾌﾞﾙ内和名称 + RecNo:539 塗装色をｾｯﾄ
//      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "J");

      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   } else {
      // 塗装色ｺｰﾄﾞﾃｰﾌﾞﾙ内和名称をｾｯﾄ
      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "J");
   }
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(32, 1, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(31, 1, s_Text);
   //Excel_Cell_DataSet(29, 1, s_Text);
   //*********************
   G_Log_Text = "和文：塗装色          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：検査責任者
   //****************************
   i_RecNo = 1085;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(34, 3, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(33, 3, s_Text);
   //Excel_Cell_DataSet(31, 3, s_Text);
   //*********************
   G_Log_Text = "和文：検査責任者       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：検査担当者
   //****************************
   i_RecNo = 1083;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(35, 3, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(34, 3, s_Text);
   //Excel_Cell_DataSet(32, 3, s_Text);
   //*********************
   G_Log_Text = "和文：検査担当者       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  和文：設計担当者
   //****************************
   //2017.04.17 舶用担当者変更
   //i_RecNo = 1079;
   i_RecNo = 1271;
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(36, 3, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(35, 3, s_Text);
   //Excel_Cell_DataSet(33, 3, s_Text);
   //*********************
   G_Log_Text = "和文：設計担当者       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：日付1
   //***************************
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(34, 5, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(33, 5, s_Text);
   //Excel_Cell_DataSet(31, 5, s_Text);
   //*********************
   G_Log_Text = "和文：日付１           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：日付2
   //***************************
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(35, 5, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(34, 5, s_Text);
   //Excel_Cell_DataSet(32, 5, s_Text);
   //*********************
   G_Log_Text = "和文：日付２           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：日付3
   //***************************
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(36, 5, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(35, 5, s_Text);
   //Excel_Cell_DataSet(33, 5, s_Text);
   //*********************
   G_Log_Text = "和文：日付３           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：HEADS VER
   //***************************
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(39, 1, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(38, 1, s_Text);
   //Excel_Cell_DataSet(36, 1, s_Text);
   //*********************
   G_Log_Text = "和文：HEADS VER        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //-----------------------------------
   //2004.11.15 [分類番号]出力追加

   //***************************
   //***  和文：分類番号
   //***************************
   if(pcHeadsData->GetMirrorFlag()){
      s_Text = "分類６";     // ミラー仕様
   }else{
      s_Text = "分類４";     // 通常仕様
   }
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(36, 15, s_Text);
   //Excel_Cell_DataSet(35, 15, s_Text);
   G_Log_Text = "和文：分類番号         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2007.11.15
   //-----------------------------------

   //2008.04.14 RX-00M[ﾗﾁｪｯﾄ･洗浄用ﾌﾞﾗｼ･ｹﾞｰｼﾞﾌﾟﾚｰﾄ不要]出力追加


   //-------------------------------------------------
   // OGI 2015.02.20 プラン設定による可変文字列対応
   //***************************
   //***  和文：ラチェット数量
   //***************************
//   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
//    s_Text = "----";
//    Excel_Cell_DataSet(23, 4, s_Text);
//   }

   // ラチェットスパナの有無より出力文言を変更する
   // s_HEADS_DATA[262]
   //   0   ：スパナ有、舶用用具有
   //   1   ：スパナ無、舶用用具無
   //   2   ：スパナ有、舶用用具有
   //   3   ：スパナ無、舶用用具無
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
        // RX-00の場合
        s_Text = "----";
        Excel_Cell_DataSet(23, 4, s_Text);
   }else{
        // ラチェット有り無し判定
        if (s_HEADS_DATA[262].TrimRight() == "0" || s_HEADS_DATA[262].TrimRight() == "2") {
          Excel_Cell_DataSet(23, 4, "1");
        } else {
          Excel_Cell_DataSet(23, 4, "----");
        }
   }

   G_Log_Text = "和文：ラチェット数量   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //-------------------------------------------------


   //***************************
   //***  和文：洗浄用ブラシ数量
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
   }
   G_Log_Text = "和文：洗浄用ブラシ数量   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：ゲージプレート数量
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(25, 4, s_Text);
   }
   G_Log_Text = "和文：ゲージプレート数量   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.03.04 プラン設定による可変文字列対応
   //***************************
   //***  和文：舶用用具無の場合は、洗浄用ブラシ、ゲージプレート、チェーンの数量を0にする
   //***************************
   if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
      Excel_Cell_DataSet(25, 4, s_Text);
      Excel_Cell_DataSet(26, 4, s_Text);
   }
   //-------------------------------------------------


   //**********************************************************


   //**********************************************************************************
   //***                                                                            ***
   //***                    舶用要目表(英文)　データセット                          ***
   //***                                                                            ***
   //**********************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "英文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));

   //*********************
   //***  英文：DOC.No.
   //*********************
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_Text = "M" + G_SAKUBAN;
   s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
   Excel_Cell_DataSet(1, 13, s_Text);
   G_Log_Text = "英文：DOC.No.         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  英文：MFG.No.
   //*********************
   //2022/12/15 桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 桁数追加対応 Mod_E   

   //-------------------------------------
   // 2007.10.09 ﾐﾗｰ仕様対応 "A/B"を付加
   if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
   }
   // 2007.10.09
   //-------------------------------------

   Excel_Cell_DataSet(3, 1, s_Text);
   G_Log_Text = "英文：MFG.No.         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  英文：MODEL
   //*********************
   i_RecNo = 33;
// 英文の場合（改）は不要
//   if (s_HEADS_DATA[1057].TrimRight() == "1"){
//      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
//   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(2, 4, s_Text);
   //Excel_Cell_DataSet(3, 3, s_Text);
   //*********************
   G_Log_Text = "英文：MODEL            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //*********************
   //***  英文：EQPT.NAME
   //*********************
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(3, 4, s_Text);
   G_Log_Text = "英文：EQPT.NAME        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*********************

   //*********************
   //***  英文：QUANTITY
   //*********************
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(2, 13, s_Text);
      G_Log_Text = "英文：QUANTITY         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "英文：QUANTITY  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //********************************
   //***  英文：APPLICABLE CODE
   //********************************
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "英文：APPLICABLE CODE  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』により、";
   s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),false,true, s_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(5, 4, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  英文：SHIP No.
   //************************
   i_RecNo = 270;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 4, s_Text);
   G_Log_Text = "英文：SHIP No.         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************************
   //***  英文：HEAT TRANSFER
   //*******************************
   i_RecNo = 1067;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 4, s_Text);
   G_Log_Text = "英文：HEAT TRANSFER    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //*******************************
   //***  英文：HEAT EXCHANGED
   //*******************************
   i_RecNo = 107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(8, 4, s_Text);
   G_Log_Text = "英文：HEAT EXCHANGED   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  英文：MASS[kg] DRY
   //****************************
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(9, 4, s_Text);
   //Excel_Cell_DataSet(8, 4, s_Text);
   //*********************
   G_Log_Text = "英文：MASS[kg] DRY     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：MASS[kg] WET
   //****************************
   i_RecNo = 1030;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(10, 4, s_Text);
   //Excel_Cell_DataSet(9, 4, s_Text);
   //*********************
   G_Log_Text = "英文：MASS[kg] WET     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   ///****************************
   //***  英文：B FLUID NAME
   //****************************
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
//      i_RecNo = 76;     // Ａ流体名
//   } else {
//      i_RecNo = 45;     // Ｂ流体名
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1072].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(12, 4, s_Text);
   //Excel_Cell_DataSet(11, 4, s_Text);
   //*********************
   G_Log_Text = "英文：B FLUID NAME     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：A FLUID NAME
   //****************************
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // ＡＢ逆
//      i_RecNo = 45;     // Ｂ流体名
//   } else {
//      i_RecNo = 76;     // Ａ流体名
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1074].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(12, 10, s_Text);
   //Excel_Cell_DataSet(11, 10, s_Text);
   //*********************
   G_Log_Text = "英文：A FLUID NAME     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  英文：B INLET TEMP[℃]
   //********************************
   i_RecNo = 852;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(13, 4, s_Text);
   //Excel_Cell_DataSet(12, 4, s_Text);
   //*********************
   G_Log_Text = "英文：B INLET TEMP     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  英文：A INLET TEMP[℃]
   //********************************
   i_RecNo = 853;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(13, 10, s_Text);
   //Excel_Cell_DataSet(12, 10, s_Text);
   //*********************
   G_Log_Text = "英文：A INLET TEMP     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  英文：B OUTLET TEMP[℃]
   //********************************
   i_RecNo = 854;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(14, 4, s_Text);
   //Excel_Cell_DataSet(13, 4, s_Text);
   //*********************
   G_Log_Text = "英文：B OUTLET TEMP    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  英文：A OUTLET TEMP[℃]
   //********************************
   i_RecNo = 855;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(14, 10, s_Text);
   //Excel_Cell_DataSet(13, 10, s_Text);
   //*********************
   G_Log_Text = "英文：A OUTLET TEMP    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.23 プラン設定による可変文字列対応（英文）
   s_WkText1 = s_HEADS_DATA[846].TrimRight();  // Bスチーム区分(CASE1)
   s_WkText2 = s_HEADS_DATA[847].TrimRight();  // Aスチーム区分(CASE1)
   if( s_WkText1 == "0" && s_WkText2 == "0" ){
      // 液-液：[m3/h]
      Excel_Cell_DataSet(15, 1,"FLOW RATE[m3/h]");
   }else if( s_WkText1 == "1" && s_WkText2 == "0" ){
      // 蒸気-液：[kg/h m3/h]
      Excel_Cell_DataSet(15, 1,"FLOW RATE[kg/h m3/h]");
   }else if( s_WkText1 == "0" && s_WkText2 == "1" ){
      // 液-蒸気：[m3/h kg/h]
      Excel_Cell_DataSet(15, 1,"FLOW RATE[m3/h kg/h]");
   }
   //-------------------------------------------------


   //********************************
   //***  英文：B FLOW RATE[m3/h]
   //********************************
   i_RecNo = 856;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(15, 4, s_Text);
   //Excel_Cell_DataSet(14, 4, s_Text);
   //*********************
   G_Log_Text = "英文：B FLOW RATE      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  英文：A FLOW RATE[m3/h]
   //********************************
   i_RecNo = 857;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(15, 10, s_Text);
   //Excel_Cell_DataSet(14, 10, s_Text);
   //*********************
   G_Log_Text = "英文：A FLOW RATE      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  英文：B PRESSURE DROP[MPa]
   //*********************************
   i_RecNo = 858;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 桁数変更
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(16, 4, s_Text);
   //Excel_Cell_DataSet(15, 4, s_Text);
   //*********************
   G_Log_Text = "英文：B PRESSURE DROP  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  英文：A PRESSURE DROP[MPa]
   //*********************************
   i_RecNo = 859;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 桁数変更
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(16, 10, s_Text);
   //Excel_Cell_DataSet(15, 10, s_Text);
   //*********************
   G_Log_Text = "英文：A PRESSURE DROP  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  英文：B DESIGN PRESS[MPaG]
   //*********************************
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "0.00";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(17, 4, s_Text);
   //Excel_Cell_DataSet(16, 4, s_Text);
   //*********************
   G_Log_Text = "英文：B DESIGN PRESS   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  英文：A DESIGN PRESS[MPaG]
   //*********************************
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "0.00";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(17, 10, s_Text);
   //Excel_Cell_DataSet(16, 10, s_Text);
   //*********************
   G_Log_Text = "英文：A DESIGN PRESS   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //********************************
   //***  英文：B DESIGN TEMP.[℃]
   //********************************
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 4, s_Text);
   G_Log_Text = "英文：B DESIGN TEMP.   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************
   //***  英文：A DESIGN TEMP.[℃]
   //********************************
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "英文：A DESIGN TEMP.   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  英文：Bテスト圧力
   //****************************
   //****************************
   //***  英文：Aテスト圧力
   //****************************
   s_Text = s_HEADS_DATA[62].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpP = s_Text.ToDouble();
   } else {
      d_BpP = 0.0;
   }
   s_Text = s_HEADS_DATA[61].Trim();   // Ｂ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpH = s_Text.ToDouble();
   } else {
      d_BpH = 0.0;
   }
   s_Text = s_HEADS_DATA[93].Trim();   // Ａ試験圧力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApP = s_Text.ToDouble();
   } else {
      d_ApP = 0.0;
   }
   s_Text = s_HEADS_DATA[92].Trim();   // Ａ試験水力
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApH = s_Text.ToDouble();
   } else {
      d_ApH = 0.0;
   }

   if( d_BpP > 0.0 || d_ApP > 0.0) {         // Ｂ試験圧力＞０又はＡ試験圧力＞０
      if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
         // 両方の条件が成立する場合はテスト圧力項目を空欄とする
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 1, "");
         Excel_Cell_DataSet(19, 4, "");
         Excel_Cell_DataSet(19, 10, "");
         //Excel_Cell_DataSet(17, 1, "");
         //Excel_Cell_DataSet(17, 4, "");
         //Excel_Cell_DataSet(17, 10, "");
         //*********************
         G_ErrLog_Text = "英文：Bテスト圧力  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      } else {
         //2003.10.08 試験空圧
         //Excel_Cell_DataSet(17, 1, "PNEU. PRESS[MPaG]");
         //2004.05.21 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 1, "TEST PRESS(PNEU.)[MPaG]");
         //Excel_Cell_DataSet(17, 1, "TEST PRESS(PNEU.)[MPaG]");
         //*********************
         //*******************
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB逆ではない
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "英文：Aテスト圧力      RecNo:61『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);
         } else {
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "英文：Bテスト圧力      RecNo:62『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ﾌｫｰﾏｯﾄ訂正
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "英文：Aテスト圧力      RecNo:61『" + s_Text + "』(空圧)をセット。";
            Write_Log(G_Log_Text);
         }
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // Ｂ試験水圧＞０又はＡ試験水圧＞０
      //2003.10.08 試験水圧
      //Excel_Cell_DataSet(17, 1, "HYDRO. PRESS[MPaG]");
      //2004.05.21 ﾌｫｰﾏｯﾄ訂正
      Excel_Cell_DataSet(19, 1, "TEST PRESS(HYDR.)[MPaG]");
      //Excel_Cell_DataSet(17, 1, "TEST PRESS(HYDR.)[MPaG]");
      //*********************
      //*******************
      if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB逆ではない
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "英文：Bテスト圧力      RecNo:93『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "英文：Bテスト圧力      RecNo:92『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);
      } else {
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "英文：Bテスト圧力      RecNo:93『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ﾌｫｰﾏｯﾄ訂正
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "英文：Bテスト圧力      RecNo:92『" + s_Text + "』(水圧)をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      //2004.04.26 ﾌｫｰﾏｯﾄ訂正
      Excel_Cell_DataSet(19, 1, "");
      Excel_Cell_DataSet(19, 4, "");
      Excel_Cell_DataSet(19, 10, "");
      //Excel_Cell_DataSet(17, 1, "");
      //Excel_Cell_DataSet(17, 4, "");
      //Excel_Cell_DataSet(17, 10, "");
      //*********************
      G_ErrLog_Text = "英文：Bテスト圧力  該当する数値がありません。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }


   //*********************************
   //***  英文：B HOLDING VOLUME[lit]
   //*********************************
   i_RecNo = 1282;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(20, 4, s_Text);
   //Excel_Cell_DataSet(18, 4, s_Text);
   //*********************
   G_Log_Text = "英文：B HOLDING VOLUME RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  英文：A HOLDING VOLUME[lit]
   //*********************************
   i_RecNo = 1283;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(20, 10, s_Text);
   //Excel_Cell_DataSet(18, 10, s_Text);
   //*********************
   G_Log_Text = "英文：A HOLDING VOLUME RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

    // Ｊ型はチェーン（移動止め）不要
   if(s_HEADS_DATA[36].Pos("J") != 0) {
      //2004.04.26 ﾌｫｰﾏｯﾄ訂正
      Excel_Cell_DataSet(26, 4, "----");
      //Excel_Cell_DataSet(24, 4, "----");
      //*********************
   }


   //-------------------------------------------------
   // OGI 2015.02.19 アキュームレータ部品対応
   // GAS CHARGING TOOLS, N2 GAS CHARGING PRESS[MPa], WORKING PRESS[MPa], ACCUMULATOR の追加対応

   //***************************
   //*** 英文：GAS CHARGING TOOLS
   //***************************
   s_Text = s_HEADS_DATA[1117].TrimRight();
   if (s_Text == "1") {
      Excel_Cell_DataSet(27, 4, "1");
   } else {
      Excel_Cell_DataSet(27, 4, "----");
   }

   //***************************
   //*** 英文：N2 GAS CHARGING PRESS[MPa]
   //***************************
   Excel_Cell_DataSet(29, 4, "----");

   //***************************
   //*** 英文：WORKING PRESS[MPa]
   //***************************
   Excel_Cell_DataSet(30, 4, "----");

   //***************************
   //***  英文：ACCUMULATOR
   //***************************
   if (s_Nzl_SyuruiE3 == "8" || s_Nzl_SyuruiE4 == "8"){
      Excel_Cell_DataSet(30, 12, "1");
   } else {
      Excel_Cell_DataSet(30, 12, "----");
   }


   //*********************************
   //***  英文：AIR VENT
   //*********************************
   i_Value = 0;
   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //if( (s_HEADS_DATA[905].SubString(1,1) == "7")&&(s_HEADS_DATA[905].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[920].SubString(1,1) == "7")&&(s_HEADS_DATA[920].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[965].SubString(1,1) == "7")&&(s_HEADS_DATA[965].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[980].SubString(1,1) == "7")&&(s_HEADS_DATA[980].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[905].SubString(1,2) == "07")
    && (s_HEADS_DATA[905].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[920].SubString(1,2) == "07")
    && (s_HEADS_DATA[920].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[965].SubString(1,2) == "07")
    && (s_HEADS_DATA[965].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[980].SubString(1,2) == "07")
    && (s_HEADS_DATA[980].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(23, 12, s_Text);
   //Excel_Cell_DataSet(21, 12, s_Text);
   //*********************
   G_Log_Text = "英文：AIR VENT        『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：DRAIN BROW
   //****************************
   i_Value = 0;
   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //if( (s_HEADS_DATA[935].SubString(1,1) == "7")&&(s_HEADS_DATA[935].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[950].SubString(1,1) == "7")&&(s_HEADS_DATA[950].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[995].SubString(1,1) == "7")&&(s_HEADS_DATA[995].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[1010].SubString(1,1) == "7")&&(s_HEADS_DATA[1010].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[935].SubString(1,2) == "07")
    && (s_HEADS_DATA[935].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[950].SubString(1,2) == "07")
    && (s_HEADS_DATA[950].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[995].SubString(1,2) == "07")
    && (s_HEADS_DATA[995].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[1010].SubString(1,2) == "07")
    && (s_HEADS_DATA[1010].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(24, 12, s_Text);
   //Excel_Cell_DataSet(22, 12, s_Text);
   //*********************
   G_Log_Text = "英文：DRAIN BLOW      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：NAME PLATE
   //****************************
   s_Text = "1";
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(25, 12, s_Text);
   //Excel_Cell_DataSet(23, 12, s_Text);
   //*********************
   G_Log_Text = "英文：NAME PLATE      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //-------------------------------------------------
   // OGI 2015.03.14 Ｌ型温度計の判定基準に”11”を追加
   //****************************
   //***  英文：THERMOMETER L
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() == "06" || s_HEADS_DATA[898].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() == "06" || s_HEADS_DATA[913].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() == "06" || s_HEADS_DATA[928].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() == "06" || s_HEADS_DATA[943].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() == "06" || s_HEADS_DATA[958].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() == "06" || s_HEADS_DATA[973].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() == "06" || s_HEADS_DATA[988].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() == "06" || s_HEADS_DATA[1003].TrimRight() == "11"){
      i_Value++;
   }
   //-------------------------------------------------

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(26, 13, s_Text);
   //Excel_Cell_DataSet(24, 13, s_Text);
   //*********************
   G_Log_Text = "英文：THERMOMETER L   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：THERMOMETER I
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() != "06" && s_HEADS_DATA[898].TrimRight() != "07" && s_HEADS_DATA[898].TrimRight() != "11" && s_HEADS_DATA[898].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() != "06" && s_HEADS_DATA[913].TrimRight() != "07" && s_HEADS_DATA[913].TrimRight() != "11" && s_HEADS_DATA[913].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() != "06" && s_HEADS_DATA[928].TrimRight() != "07" && s_HEADS_DATA[928].TrimRight() != "11" && s_HEADS_DATA[928].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() != "06" && s_HEADS_DATA[943].TrimRight() != "07" && s_HEADS_DATA[943].TrimRight() != "11" && s_HEADS_DATA[943].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() != "06" && s_HEADS_DATA[958].TrimRight() != "07" && s_HEADS_DATA[958].TrimRight() != "11" && s_HEADS_DATA[958].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() != "06" && s_HEADS_DATA[973].TrimRight() != "07" && s_HEADS_DATA[973].TrimRight() != "11" && s_HEADS_DATA[973].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() != "06" && s_HEADS_DATA[988].TrimRight() != "07" && s_HEADS_DATA[988].TrimRight() != "11" && s_HEADS_DATA[988].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() != "06" && s_HEADS_DATA[1003].TrimRight() != "07" && s_HEADS_DATA[1003].TrimRight() != "11" && s_HEADS_DATA[1003].ToIntDef(0) > 0){
      i_Value++;
   }

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(27, 13, s_Text);
   //Excel_Cell_DataSet(25, 13, s_Text);
   //*********************
   G_Log_Text = "英文：THERMOMETER I   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：STRAINER
   //****************************
   if (s_HEADS_DATA[891].ToIntDef(0) != 5 && s_HEADS_DATA[906].ToIntDef(0) != 5
                                          && s_HEADS_DATA[921].ToIntDef(0) != 5
                                          && s_HEADS_DATA[936].ToIntDef(0) != 5
                                          && s_HEADS_DATA[951].ToIntDef(0) != 5
                                          && s_HEADS_DATA[966].ToIntDef(0) != 5
                                          && s_HEADS_DATA[981].ToIntDef(0) != 5
                                          && s_HEADS_DATA[996].ToIntDef(0) != 5){

      s_Text = "----";
   } else {
      s_Text = "1";
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(28, 12, s_Text);
   //Excel_Cell_DataSet(26, 12, s_Text);
   //*********************
   G_Log_Text = "英文：STRAINER        『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：DUPLEX GAUGE
   //****************************
   i_Value=0;
   //2015.04.28 数量変更
   //if (s_HEADS_DATA[901].ToIntDef(0) == 16 || s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[916].ToIntDef(0) == 16 || s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[961].ToIntDef(0) == 16 || s_HEADS_DATA[961].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[976].ToIntDef(0) == 16 || s_HEADS_DATA[976].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[901].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[931].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[946].ToIntDef(0) == 17 )  i_Value++;
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_S
   if (s_HEADS_DATA[901].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_E
   i_Value = i_Value / 2;
   //*******************
   if (i_Value == 0 ) {
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(29, 12, s_Text);
   //Excel_Cell_DataSet(27, 12, s_Text);
   //*********************
   G_Log_Text = "英文：DUPLEX GAUGE           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.23 プラン設定による可変文字列対応（英文）
   s_WkText1 = s_HEADS_DATA[901].TrimRight();  // S1圧力計種類
   s_WkText2 = s_HEADS_DATA[916].TrimRight();  // S2圧力計種類
   if( s_WkText1 == "16" || s_WkText2 == "16" ){
      Excel_Cell_DataSet(29, 7, "PRESSURE GAUGE");
      //2015.04.28 数量変更
      //Excel_Cell_DataSet(29, 12, "2");
      //*********************
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_S
   } else if ( s_WkText1 == "21" || s_WkText2 == "21" ){
      Excel_Cell_DataSet(29, 7, "PRESSURE GAUGE");
   } else if ( s_WkText1 == "22" || s_WkText2 == "22" ){
      Excel_Cell_DataSet(29, 7, "PRESSURE GAUGE");
   //2020.01.08 SGPﾘﾝｸﾞ端管用圧力計追加変更_E
   } else {
      Excel_Cell_DataSet(29, 7, "DUPLEX GAUGE");
   }
   //-------------------------------------------------


   //****************************
   //***  英文：MUNSELL No.
   //****************************
   i_RecNo = 31;
   s_Tosou_CD = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Tosou_CD.ToIntDef(0) == 1){
      // RecNo:539 塗装色をｾｯﾄ
   //2003.09.24 英文塗装色
   //   i_RecNo = 32;
      i_RecNo = 539;
   //*********************
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   } else if (s_Tosou_CD.ToIntDef(0) == 3 || s_Tosou_CD.ToIntDef(0) == 5) {
      // 塗装色ｺｰﾄﾞﾃｰﾌﾞﾙ内和名称 + RecNo:539 塗装色をｾｯﾄ
//      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "J");
   //2003.09.24 英文塗装色
   //   i_RecNo = 32;
      i_RecNo = 539;
   //*********************
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   } else {
      // 塗装色ｺｰﾄﾞﾃｰﾌﾞﾙ内和名称をｾｯﾄ
      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "E");
   }
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(32, 1, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(31, 1, s_Text);
   //Excel_Cell_DataSet(29, 1, s_Text);
   //*********************
   G_Log_Text = "英文：MUNSELL No.     『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：APPROVED BY
   //****************************
   i_RecNo = 1086;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(34, 3, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(33, 3, s_Text);
   //Excel_Cell_DataSet(31, 3, s_Text);
   //*********************
   G_Log_Text = "英文：APPROVED BY      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：REVIEWED BY
   //****************************
   i_RecNo = 1084;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(35, 3, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(34, 3, s_Text);
   //Excel_Cell_DataSet(32, 3, s_Text);
   //*********************
   G_Log_Text = "英文：REVIEWED BY      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  英文：PREPARED BY
   //****************************
   //2017.04.17 舶用担当者変更
   //i_RecNo = 1080;
   i_RecNo = 1272;
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(36, 3, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(35, 3, s_Text);
   //Excel_Cell_DataSet(33, 3, s_Text);
   //*********************
   G_Log_Text = "英文：PREPARED BY      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：日付1
   //***************************
   //2005.02.23 英文日付修正
   //i_RecNo = 1087;
   i_RecNo = 1056;
   //***********************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(34, 5, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(33, 5, s_Text);
   //Excel_Cell_DataSet(31, 5, s_Text);
   //*********************
   G_Log_Text = "英文：日付１           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：日付2
   //***************************
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(35, 5, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(34, 5, s_Text);
   //Excel_Cell_DataSet(32, 5, s_Text);
   //*********************
   G_Log_Text = "英文：日付２           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：日付3
   //***************************
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(36, 5, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(35, 5, s_Text);
   //Excel_Cell_DataSet(33, 5, s_Text);
   //*********************
   G_Log_Text = "英文：日付３           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：HEADS VER
   //***************************
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   Excel_Cell_DataSet(39, 1, s_Text);
   //2004.04.26 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(38, 1, s_Text);
   //Excel_Cell_DataSet(36, 1, s_Text);
   //*********************
   G_Log_Text = "英文：HEADS VER        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //-----------------------------------
   //2004.11.15 [分類番号]出力追加

   //***************************
   //***  英文：分類番号
   //***************************
   if(pcHeadsData->GetMirrorFlag()){
      s_Text = "SHEET No.6";     // ミラー仕様
   }else{
      s_Text = "SHEET No.4";     // 通常仕様
   }
   //2015.02.19 ﾌｫｰﾏｯﾄ訂正
   //Excel_Cell_DataSet(35, 15, s_Text);
   Excel_Cell_DataSet(36, 15, s_Text);
   G_Log_Text = "英文：SHEET NO         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2007.11.15
   //-----------------------------------

   //2008.04.14 RX-00M[ﾗﾁｪｯﾄ･洗浄用ﾌﾞﾗｼ･ｹﾞｰｼﾞﾌﾟﾚｰﾄ不要]出力追加


   //-------------------------------------------------
   // OGI 2015.02.23 プラン設定による可変文字列対応（英文）
   //***************************
   //***  英文：ラチェット数量
   //***************************
//   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
//    s_Text = "----";
//    Excel_Cell_DataSet(23, 4, s_Text);
//   }

   // ラチェットスパナの有無より出力文言を変更する
   // s_HEADS_DATA[262]
   //   0   ：スパナ有、舶用用具有
   //   1   ：スパナ無、舶用用具無
   //   2   ：スパナ有、舶用用具有
   //   3   ：スパナ無、舶用用具無
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
        // RX-00の場合
        s_Text = "----";
        Excel_Cell_DataSet(23, 4, s_Text);
   }else{
        // ラチェット有り無し判定
        if (s_HEADS_DATA[262].TrimRight() == "0" || s_HEADS_DATA[262].TrimRight() == "2") {
          Excel_Cell_DataSet(23, 4, "1");
        } else {
          Excel_Cell_DataSet(23, 4, "----");
        }
   }

   G_Log_Text = "英文：ラチェット数量   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //-------------------------------------------------


   //***************************
   //***  英文：洗浄用ブラシ数量
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
   }
   G_Log_Text = "英文：洗浄用ブラシ数量   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：ゲージプレート数量
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(25, 4, s_Text);
   }
   G_Log_Text = "英文：ゲージプレート数量   『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.03.04 プラン設定による可変文字列対応
   //***************************
   //***  英文：舶用用具無の場合は、洗浄用ブラシ、ゲージプレート、チェーンの数量を0にする
   //***************************
   if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
      Excel_Cell_DataSet(25, 4, s_Text);
      Excel_Cell_DataSet(26, 4, s_Text);
   }
   //-------------------------------------------------


   //**********************************************************

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
   // マクロ実行
   // 2012.11.27 PDF再追加
   //PDFファイル出力取消
   //exApplication.Exec(Function("Run") << "Gsub_PDFOut");

   AnsiString s_China;           // 言語区分("0":日本語、"1":中国語)
   i_RecNo = 1107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "CHINA" ) {
      s_China = "1";                                                 // 中国語
   // 2013.04.03 巨元追加
      // 中国巨元
   } else if ( s_Text == "KYOGEN") {
      s_China = "1";                                                 // 巨元
   // *******************
   } else {
      s_China = "0";                                                 // 日本語
   }

   if ( s_China == "0" ) {
     exApplication.OleFunction("Run" , OleVariant("Gsub_PDFOut"));
   }

   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.OlePropertySet("DisplayAlerts" , OleVariant("false"));
   // ******************
   //Windows 2008Server 移植対応
   exApplication.OlePropertySet("DisplayAlerts", false);
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『舶用要目表』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}


