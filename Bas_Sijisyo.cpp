//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：製作指示書  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.06.12
//　 更 新 日：2002.10.21
//             2007.10.10 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <io.h>

#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用
#include "DataModule.h"

#include "Bas_Sijisyo.h"

#define  EXCEL_SHEETNAME    "製作指示書"          // EXCELシート名(製作指示書)

#include <math.h>       //2004.08.16 切り上げ

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 製作指示書　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Sijisyo(HeadsData* pcHeadsData)
{
   // 2021.06.18 FireDACへの移行 MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDACへの移行 MOD_E

   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_SQL;             // SQL文
   AnsiString s_Text;            // 汎用文字列
   //2004.07.15 変数追加
   AnsiString s_Text2;           // 汎用文字列２
   //*******************
   //2008.07.24 CX-10追加
   AnsiString P_Model;
   //********************
   AnsiString s_Bunbo;
   AnsiString s_Bunsi;
   AnsiString d_RevNo;           // 製作指示書のある直近のRevNo

   AnsiString s_Side_B;          // ｽﾀｰﾄﾌﾟﾚｰﾄ設定用 Ｂ側
   AnsiString s_Side_A;          //                 Ａ側
   AnsiString s_Syubetu;         // ｱﾝｶｰﾎﾞﾙﾄ長参照用 種別ﾌﾗｸﾞ

   AnsiString s_S1TankanNM;      // S1端管名称
   AnsiString s_S2TankanNM;      // S2端管名称
   AnsiString s_S3TankanNM;      // S3端管名称
   AnsiString s_S4TankanNM;      // S4端管名称

   //2009.07.31 変数追加
   AnsiString s_S1TugiteNM;      // S1継手名称
   AnsiString s_S2TugiteNM;      // S2継手名称
   AnsiString s_S3TugiteNM;      // S3継手名称
   AnsiString s_S4TugiteNM;      // S4継手名称
   //*******************

   // 2012.01.20 中国材質
   AnsiString s_ChinaM;
   // *******************

   // 2012.11.27 エポキシ塗装注記
   int i_1365Flg;                // ﾌﾚｰﾑ仕上注記ﾌﾗｸﾞ
   // ***************************

   int i_RecNo;                  // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   int i_ErrFlg;
   int i_Value_1;
   int i_Value_2;
   int i;
   int i_Siage;

   //2018.02.09 標準フランジ材質追加_S
   int i_CODE;
   //2018.02.09 標準フランジ材質追加_E

   //double d_wkValue;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   AnsiString s_SpecCode;    // 仕様ﾀｲﾌﾟ("":元仕様/"A":ﾐﾗｰA仕様/"B":ﾐﾗｰB仕様)
   bool       bFind;
   bool       bFlag;

   if(pcHeadsData->GetMirrorFlag()){
        // ﾐﾗｰ仕様の場合、A仕様、B仕様以外は処理不要(正常終了)
        if(pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_NONE) return(true);
   }

   // AnsiString型HEADSﾃﾞｰﾀを取得
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   // 仕様ﾀｲﾌﾟ文字列を取得(ﾌｧｲﾙ名作成で使用)
   s_SpecCode = AnsiString(pcHeadsData->GetSpecCode());

   s_Tmplate_Pass = G_Template_Dir + "製作指示書.xls";                 // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
//Mod_Str 2019/02/19 Ri
   //s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + G_SAKUBAN + s_SpecCode + "OLD.xls";
   s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "OLD.xls";
//Mod_End 2019/02/19 Ri
   // 2019.05.22 旧製作指示書名称変更_S
   if (G_OLD_FLG == 1) {
       //旧製作指示書作成
	   //2023/4/21 製造番号桁数追加対応 Mod
	   //s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
	   s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
   }
   // 2019.05.22 旧製作指示書名称変更_E
/*   s_MakeExcel_Pass = G_xls_Output_Dir + "MAN";
   s_MakeExcel_Pass = s_MakeExcel_Pass + G_SAKUBAN;
   s_MakeExcel_Pass = s_MakeExcel_Pass + s_SpecCode;
   s_MakeExcel_Pass = s_MakeExcel_Pass + ".xls";           // ｺﾋﾟｰ先ﾌｧｲﾙ
*/
   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『製作指示書』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
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

   //***************************************************************************
   //***                                                                     ***
   //***                       製作指示書　データセット                      ***
   //***                                                                     ***
   //***************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SHEETNAME);
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)EXCEL_SHEETNAME);

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ａ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //************************
   //***  HEADS VER
   //************************
   // 『A001』
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
   //s_Text = "HEADS VER." + s_Text;
   Excel_Cell_DataSet(1, 21, s_Text);
   G_Log_Text = "HEADS VER            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2018 09.18 バーコード削除_S
   //// 2017.12.01 バーコード追加_S
   ////***************************
   ////***  バーコード
   ////***************************
   //s_Text = G_SAKUBAN;
   //s_Text = "*" + s_Text + "*";
   //Excel_Cell_DataSet(2, 21, s_Text);
   //G_Log_Text = "バーコード           『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //// 2017.12.01 バーコード追加_E
   // 2018 09.18 バーコード削除_E

   //***************************
   //***  図面訂正
   //***************************
   // 『A002』
   i_RecNo = 319;
//   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
//      s_Text = "";
//   } else {
//      s_Text = "図面訂正";
//   }
   s_Text = "Rev." + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(3, 21, s_Text);
   G_Log_Text = "図面訂正              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  作成日
   //***************************
   // 『A003』
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(4, 21, s_Text);
   G_Log_Text = "作成日                RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  管理作番ﾏｰｸ
   //***************************
   // 『A004』
   i_RecNo = 417;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0"){
      s_Text = "";
      Excel_Cell_DataSet(1, 27, "");
   } else {
      s_Text = "K";
      Excel_Cell_DataSet(1, 27, "K");
   }
//   Excel_Cell_DataSet(2, 29, s_Text);
   G_Log_Text = "管理作番ﾏｰｸ          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  営業担当者
   //***************************
   // 『A005』
   i_RecNo = 3;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(4, 6, s_Text);
   G_Log_Text = "営業担当者            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   // 『A006』
   //-------------------------------
   // 2007.10.15 仕様ﾀｲﾌﾟ文字列追加
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E   
   s_Text = s_Text + pcHeadsData->GetSpecCode();
   // 2007.10.15
   //-------------------------------

   Excel_Cell_DataSet(5, 5, s_Text);
   G_Log_Text = "製造番号             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑﾊﾞｰｼﾞｮﾝ注記  03.06.27追加
   //***************************
   // 『A006』
   s_Text = s_HEADS_DATA[1380].TrimRight();
   Excel_Cell_DataSet(5, 10, s_Text);
   G_Log_Text = "ﾌﾚｰﾑﾊﾞｰｼﾞｮﾝ注記             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  型　式
   //*********************
   // 『A008』
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(6, 5, s_Text);
   G_Log_Text = "型式                  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  納　期
   //***************************
   // 『A009』
   //i_RecNo = 12;
   i_RecNo = 411;
   s_Text = s_HEADS_DATA[i_RecNo];
   s_Text = s_Text.SubString(1,4) + "年" +
            s_Text.SubString(6,2) + "月" +
            s_Text.SubString(9,2) + "日";
   Excel_Cell_DataSet(5, 23, s_Text);
   G_Log_Text = "作成日                RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  台  数
   //*******************
   // 『A010』
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(6, 23, s_Text);
      G_Log_Text = "台数                  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "台数      RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  注 文 先
   //***************************
   // 『A011』
   i_RecNo = 1276;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 5, s_Text);
   G_Log_Text = "注文先                RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  MIX記号１，２ 取得
   //***************************
   AnsiString s_MIX1,s_MIX2;
   // MIX記号１
   if (s_HEADS_DATA[1062].TrimRight() == "0"){
      s_MIX1 = "";
   } else if (s_HEADS_DATA[1062].TrimRight() == "1"){
      s_MIX1 = "H";
   } else if (s_HEADS_DATA[1062].TrimRight() == "2"){
      s_MIX1 = "M";
   } else if (s_HEADS_DATA[1062].TrimRight() == "3"){
      s_MIX1 = "M";
   } else {
      s_MIX1 = "";
   }

   // MIX記号２
   if (s_HEADS_DATA[1062].TrimRight() == "0"){
      s_MIX2 = "";
   } else if (s_HEADS_DATA[1062].TrimRight() == "1"){
      s_MIX2 = "M";
   } else if (s_HEADS_DATA[1062].TrimRight() == "2"){
      s_MIX2 = "M";
   } else if (s_HEADS_DATA[1062].TrimRight() == "3"){
      s_MIX2 = "L";
   } else {
      s_MIX2 = "";
   }

   //**************************
   //***  スタートプレート
   //**************************
   Get_Start_Plate(s_Side_B, s_Side_A, pcHeadsData);
   Excel_Cell_DataSet(8, 6, s_Side_B);
   Excel_Cell_DataSet(8, 14, s_Side_A);

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｂ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  B側ﾌﾟﾚｰﾄ編成 1
   //***************************
   // 『B001』
   s_Text = s_HEADS_DATA[837].TrimRight() + "×" + s_HEADS_DATA[838].TrimRight();
   s_Text = s_Text + s_MIX1;
   Excel_Cell_DataSet(9, 7, s_Text);
   G_Log_Text = "B側ﾌﾟﾚｰﾄ編成 1       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側ﾌﾟﾚｰﾄ編成 2
   //***************************
   // 『B002』
   i_Value_1 = s_HEADS_DATA[839].ToIntDef(0);   // B編成21
   i_Value_2 = s_HEADS_DATA[840].ToIntDef(0);   // B編成22

   if (i_Value_1 * i_Value_2 == 0){
      s_Text = "";
   } else {
      s_Text = AnsiString(i_Value_1) + "×" + AnsiString(i_Value_2) + s_MIX2;
   }
   Excel_Cell_DataSet(9, 10, s_Text);
   G_Log_Text = "B側ﾌﾟﾚｰﾄ編成 2       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ﾌﾟﾚｰﾄ編成 1
   //***************************
   // 『B003』
   s_Text = s_HEADS_DATA[841].TrimRight() + "×" + s_HEADS_DATA[842].TrimRight();
   s_Text = s_Text + s_MIX1;
   Excel_Cell_DataSet(9, 15, s_Text);
   G_Log_Text = "A側ﾌﾟﾚｰﾄ編成 1       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ﾌﾟﾚｰﾄ編成 2
   //***************************
   // 『B004』
   i_Value_1 = s_HEADS_DATA[843].ToIntDef(0);   // B編成21
   i_Value_2 = s_HEADS_DATA[844].ToIntDef(0);   // B編成22

   if (i_Value_1 * i_Value_2 == 0){
      s_Text = "";
   } else {
      s_Text = AnsiString(i_Value_1) + "×" + AnsiString(i_Value_2) + s_MIX2;
   }
   Excel_Cell_DataSet(9, 20, s_Text);
   G_Log_Text = "A側ﾌﾟﾚｰﾄ編成 2       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾟﾚｰﾄ材質
   //***************************
   // 『B005』
   i_RecNo = 43;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "ﾌﾟﾚｰﾄの材料           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";
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

   //2009.04.27 ﾐﾙｼｰﾄ注記
   i_RecNo = 433;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      s_Text = s_Text + " ﾐﾙ";
   }
   //********************

   Excel_Cell_DataSet(10, 7, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B006』
   i_RecNo = 835;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "B側ｶﾞｽｹｯﾄ材質         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";

   s_Text = Get_Zaisitu_Name(s_Text);

   //2018.02.09 硫酸用途注記追加_S
   i_RecNo = 43;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text2 = Type_MAT(s_Text2);
   if( s_Text2 == "HAS" ) {              // プレート材質ハステロイ
      i_RecNo = 835;
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text2 = Type_MAT(s_Text2);
      if( s_Text2 == "FPM" ) {           // Ｂ側ガスケット材質バイトン
         s_Text = s_Text + "／両面テープ";
      }
   }
   //2018.02.09 硫酸用途注記追加_E

   Excel_Cell_DataSet(11, 7, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B007』
   i_RecNo = 836;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "A側ｶﾞｽｹｯﾄ材質         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";

   s_Text = Get_Zaisitu_Name(s_Text);

   //2018.02.09 硫酸用途注記追加_S
   i_RecNo = 43;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text2 = Type_MAT(s_Text2);
   if( s_Text2 == "HAS" ) {              // プレート材質ハステロイ
      i_RecNo = 836;
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text2 = Type_MAT(s_Text2);
      if( s_Text2 == "FPM" ) {           // Ａ側ガスケット材質バイトン
         s_Text = s_Text + "／両面テープ";
      }
   }
   //2018.02.09 硫酸用途注記追加_E

   Excel_Cell_DataSet(11, 15, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2011.03.11 工程写真要注記追加
   //*******************
   //*** 注記(工程写真要)
   //*******************
   i_RecNo = 1102;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(12, 18, s_Text);
   G_Log_Text = "注記(工程写真要)     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*****************************

   //***************************
   //***  B側ﾉｽﾞﾙ材質
   //***************************
   // 『B008』
   i_RecNo = 894;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2013.04.03 SUS316Lﾁｭｰﾌﾞﾌﾗﾝｼﾞ
   if ( s_HEADS_DATA[37].ToIntDef(0) > 300 && s_Text == "1003" ) {
       s_Text = "1004";
   }
   // ****************************

   G_Log_Text = "B側ﾉｽﾞﾙ材質           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";

   s_Text = Get_Zaisitu_Name(s_Text);

   //2009.04.27 ﾐﾙｼｰﾄ注記
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
       // 2019.12.23 ﾐﾙｼｰﾄ修正_S
       //i_RecNo = 894;
       //i_Value_1 = s_HEADS_DATA[i_RecNo].ToIntDef(0);
       //if ( i_Value_1 > 1000 && i_Value_1 < 2000 ){
       //    s_Text = s_Text + " ﾐﾙ";
       //}
       s_Text = s_Text + " ﾐﾙ";
       // 2019.12.23 ﾐﾙｼｰﾄ修正_E
   }
   //********************
   //2010.06.07 CX-10D追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
       s_Text = s_Text + " (S1,S4)";
   }
   //*********************

   Excel_Cell_DataSet(13, 7, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側ﾉｽﾞﾙ色
   //***************************
   // 『B009』
   i_RecNo = 894;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "B側ﾉｽﾞﾙ材質           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より、";
   // 2015.01.16 ノズル色変更
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   // ***********************
   //if (s_Text == "2019"){
   //   s_Text = "黄";
   //} else if ( s_Text == "2026"){
   //   s_Text = "緑";
   //} else if ( s_Text == "2030"){
   //   s_Text = "青";
   //} else {
   //   s_Text = "";
   //}
   s_Text2 = "";
   for (i = 0; i < G_BOOTCL_RecCnt; i++) {
      if (G_BOOTCL[i][0] == s_Text) {
         if (G_BOOTCL[i][1] == P_Model) {
            s_Text2 = G_BOOTCL[i][2];
         }
      }
   }
   s_Text = s_Text2;
// ***********************

   Excel_Cell_DataSet(13, 12, s_Text);
   G_Log_Text = G_Log_Text + "B側ﾉｽﾞﾙ色 『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ﾉｽﾞﾙ材質
   //***************************
   // 『B010』
   i_RecNo = 909;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2013.04.03 SUS316Lﾁｭｰﾌﾞﾌﾗﾝｼﾞ
   if ( s_HEADS_DATA[37].ToIntDef(0) > 300 && s_Text == "1003" ) {
       s_Text = "1004";
   }
   // ****************************

   G_Log_Text = "A側ﾉｽﾞﾙ材質           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";

   s_Text = Get_Zaisitu_Name(s_Text);

   //2009.04.27 ﾐﾙｼｰﾄ注記
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
       // 2019.12.23 ﾐﾙｼｰﾄ修正_S
       //i_RecNo = 909;
       //i_Value_1 = s_HEADS_DATA[i_RecNo].ToIntDef(0);
       //if ( i_Value_1 > 1000 && i_Value_1 < 2000 ){
       //    s_Text = s_Text + " ﾐﾙ";
       //}
       s_Text = s_Text + " ﾐﾙ";
       // 2019.12.23 ﾐﾙｼｰﾄ修正_E
   }
   //********************
   //2010.06.07 CX-10D追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
       s_Text = s_Text + " (S2,S3)";
   }
   //*********************

   Excel_Cell_DataSet(13, 15, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ﾉｽﾞﾙ色
   //***************************
   // 『B011』
   i_RecNo = 909;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "A側ﾉｽﾞﾙ材質           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より、";
   // 2015.01.16 ノズル色変更
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   // ***********************
   //if (s_Text == "2019"){
   //   s_Text = "黄";
   //} else if ( s_Text == "2026"){
   //   s_Text = "緑";
   //} else if ( s_Text == "2030"){
   //   s_Text = "青";
   //} else {
   //   s_Text = "";
   //}
   s_Text2 = "";
   for (i = 0; i < G_BOOTCL_RecCnt; i++) {
      if (G_BOOTCL[i][0] == s_Text) {
         if (G_BOOTCL[i][1] == P_Model) {
            s_Text2 = G_BOOTCL[i][2];
         }
      }
   }
   s_Text = s_Text2;
// ***********************

   Excel_Cell_DataSet(13, 25, s_Text);
   G_Log_Text = G_Log_Text + "A側ﾉｽﾞﾙ色 『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S1継手・口径
   //***************************
   // 『B012』
   //2009.07.31 ASMEﾉｽﾞﾙ名称変更
   //s_S1TankanNM = "";   // 温度計座の材質に使う
   //
   //i_RecNo = 1136;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   ////2008.07.24 CX-10追加
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4)); //B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //       s_Text = "リング端管１　" + s_HEADS_DATA[1152].TrimRight();
   //   } else  {
   //       s_Text = "リング端管２　" + s_HEADS_DATA[1152].TrimRight();
   //   }
   //
   //} else if (s_Text == "1" || s_Text == "3"){
   ////if (s_Text == "1" || s_Text == "3"){
   ////*******************
   //   s_S1TankanNM = Get_Tankan_Data("S1", pcHeadsData);  // S1端管種類
   //   s_Text = s_S1TankanNM + "　" + s_HEADS_DATA[1184].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S1TankanNM = Get_Tankan_Data("S1", pcHeadsData);  // S1端管種類
   //   s_Text = s_S1TankanNM + "　" + s_HEADS_DATA[1184].TrimRight() + "ＷＦ";
   //
   //} else {
   //   if (s_HEADS_DATA[893].SubString(2,1) == "4") {  // フランジ付きノズル
   //      s_Text = s_HEADS_DATA[1152].TrimRight() + "　" + Get_Flange_Data(s_HEADS_DATA[893].TrimRight());
   //      s_S1TankanNM = s_HEADS_DATA[894].TrimRight();  // ノズル材質を使う
   //   } else {
   //      s_Text = s_HEADS_DATA[1152].TrimRight();
   //      s_S1TankanNM = s_HEADS_DATA[43].TrimRight(); // 端管が付かないブーツノズルの場合、プレート材質を使う
   //   }
   //}

   s_S1TankanNM = "";  // S1端菅種類
   s_S1TugiteNM = "";  // S1継手名称

   i_RecNo = 1136;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ) {
      s_S1TugiteNM = s_HEADS_DATA[1184].TrimRight();  // S1端菅継手名称
   } else {
      s_S1TugiteNM = s_HEADS_DATA[1152].TrimRight();  // S1ﾉｽﾞﾙ継手名称
   }
   if ( s_S1TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ１５０－ＮＰＳ" ) {
      s_S1TugiteNM = "ＡＳＭＥ ＣＬＳ１５０－" + s_S1TugiteNM.Delete(1,34);
   } else if ( s_S1TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ３００－ＮＰＳ" ) {
      s_S1TugiteNM = "ＡＳＭＥ ＣＬＳ３００－" + s_S1TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ﾉｽﾞﾙ規格追加
   if ( s_S1TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ４００－ＮＰＳ" ) {
      s_S1TugiteNM = "ＡＳＭＥ ＣＬＳ４００－" + s_S1TugiteNM.Delete(1,34);
   }
   // ***********************
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if ( P_Model == "CX-10" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S1TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S1TugiteNM;
      }
   } else if ( P_Model == "CX-10D" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S1TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S1TugiteNM;
      }
   // 2016.04.22 CX-00追加
   } else if ( P_Model == "CX-00" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S1TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S1TugiteNM;
      }
   } else if ( P_Model == "CXW-00" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S1TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S1TugiteNM;
      }
   // ********************
   // 2013.04.03 SX-30追加
   } else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
         s_Text = "リング端管 " + s_S1TugiteNM;
   // ********************
   // 2016.07.22 SX-30S追加
   } else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
         s_Text = "リング端管 " + s_S1TugiteNM;
   // *********************
   } else if ( s_Text == "1" || s_Text == "3" ) {
      s_S1TankanNM = Get_Tankan_Data("S1", pcHeadsData);  // S1端管種類
      s_Text = s_S1TankanNM + " " + s_S1TugiteNM;

   } else if ( s_Text == "2" ) {
      s_S1TankanNM = Get_Tankan_Data("S1", pcHeadsData);  // S1端管種類
      s_Text = s_S1TankanNM + " " + s_S1TugiteNM + "ＷＦ";

   } else {
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if (s_HEADS_DATA[893].SubString(2,1) == "4") {  // フランジ付きノズル
      if (s_HEADS_DATA[893].SubString(3,1) == "4") {  // フランジ付きノズル
      // ***********************
         s_Text = s_S1TugiteNM + " " + Get_Flange_Data(s_HEADS_DATA[893].TrimRight());
      } else {
         s_Text = s_S1TugiteNM;
      }

   }
   //***************************

   //2014.05.28 継手規格表記変更
   if ( s_S1TugiteNM.SubString(1,6) == "ＪＩＳ" ) {
   } else {
       s_Text = "注" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(14, 8, s_Text);
   G_Log_Text = "S1継手・口径         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S2継手・口径
   //***************************
   // 『B013』
   //2009.07.31 ASMEﾉｽﾞﾙ名称変更
   //s_S2TankanNM = "";   // 温度計座の材質に使う
   //
   //i_RecNo = 1137;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text == "1" || s_Text == "3"){
   //   s_S2TankanNM = Get_Tankan_Data("S2", pcHeadsData);
   //   s_Text = s_S2TankanNM + "　" + s_HEADS_DATA[1188].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S2TankanNM = Get_Tankan_Data("S2", pcHeadsData);
   //   s_Text = s_S2TankanNM + "　" + s_HEADS_DATA[1188].TrimRight() + "ＷＦ";
   //
   //} else {
   //   if (s_HEADS_DATA[908].SubString(2,1) == "4") {    // フランジ付きノズル
   //      s_Text = s_HEADS_DATA[1156].TrimRight() + "　" + Get_Flange_Data(s_HEADS_DATA[908].TrimRight());
   //      s_S1TankanNM = s_HEADS_DATA[909].TrimRight(); // ノズル材質を使う
   //   } else {
   //      s_Text = s_HEADS_DATA[1156].TrimRight();
   //      s_S1TankanNM = s_HEADS_DATA[43].TrimRight(); // 端管が付かないブーツノズルの場合、プレート材質を使う
   //   }
   //}

   s_S2TankanNM = "";  // S2端菅種類
   s_S2TugiteNM = "";  // S2継手名称

   i_RecNo = 1137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ){
      s_S2TugiteNM = s_HEADS_DATA[1188].TrimRight();  // S2端菅継手名称
   } else {
      s_S2TugiteNM = s_HEADS_DATA[1156].TrimRight();  // S2ﾉｽﾞﾙ継手名称
   }
   if ( s_S2TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ１５０－ＮＰＳ" ) {
      s_S2TugiteNM = "ＡＳＭＥ ＣＬＳ１５０－" + s_S2TugiteNM.Delete(1,34);
   } else if ( s_S2TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ３００－ＮＰＳ" ) {
      s_S2TugiteNM = "ＡＳＭＥ ＣＬＳ３００－" + s_S2TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ﾉｽﾞﾙ規格追加
   if ( s_S2TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ４００－ＮＰＳ" ) {
      s_S2TugiteNM = "ＡＳＭＥ ＣＬＳ４００－" + s_S2TugiteNM.Delete(1,34);
   }
   // ***********************

   if ( s_Text == "1" || s_Text == "3" ) {
      s_S2TankanNM = Get_Tankan_Data("S2", pcHeadsData);  // S2端菅
      s_Text = s_S2TankanNM + " " + s_S2TugiteNM;

   } else if ( s_Text == "2" ) {
      s_S2TankanNM = Get_Tankan_Data("S2", pcHeadsData);  // S2端菅
      s_Text = s_S2TankanNM + " " + s_S2TugiteNM + "ＷＦ";

   } else {
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if ( s_HEADS_DATA[908].SubString(2,1) == "4" ) {  // フランジ付きノズル
      if ( s_HEADS_DATA[908].SubString(3,1) == "4" ) {  // フランジ付きノズル
      // ***********************
         s_Text = s_S2TugiteNM + " " + Get_Flange_Data(s_HEADS_DATA[908].TrimRight());
      } else {
         s_Text = s_S2TugiteNM;
      }

   }

   //2014.05.28 継手規格表記変更
   if ( s_S2TugiteNM.SubString(1,6) == "ＪＩＳ" ) {
   } else {
       s_Text = "注" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(14, 16, s_Text);
   G_Log_Text = "S2継手・口径         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S1温度計
   //***  B側S1温度計座材質
   //***************************
   // 『B014』,『B015』
   i_RecNo = 898;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S1温度計種類

   if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
      // 『B014』
      s_Text = AnsiString(s_HEADS_DATA[899].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[900].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1357].TrimRight()));


      Excel_Cell_DataSet(15, 7, s_Text);
      G_Log_Text = "S1温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B015』
      if (s_HEADS_DATA[846].TrimRight() == "1"){
         s_Text = "ｽﾁｰﾑ用";
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(15, 11, s_Text);
      G_Log_Text = "B側S1温度計座材質    『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      // 『B014』
      s_Text = "";
      Excel_Cell_DataSet(15, 7, s_Text);
      G_Log_Text = "S1温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B015』
      s_Text = "";
      Excel_Cell_DataSet(15, 11, s_Text);
      G_Log_Text = "B側S1温度計座材質    『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S2温度計
   //***  A側S2温度計座材質
   //***************************
   i_RecNo = 913;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S2温度計種類

   if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
      // 『B016』
      s_Text = AnsiString(s_HEADS_DATA[914].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[915].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1358].TrimRight()));


      Excel_Cell_DataSet(15, 15, s_Text);
      G_Log_Text = "S2温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B017』
      if (s_HEADS_DATA[847].TrimRight() == "1"){
         s_Text = "ｽﾁｰﾑ用";
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(15, 24, s_Text);
      G_Log_Text = "A側S2温度計座材質    『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      // 『B016』
      s_Text = "";
      Excel_Cell_DataSet(15, 15, s_Text);
      G_Log_Text = "S2温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B017』
      s_Text = "";
      Excel_Cell_DataSet(15, 24, s_Text);
      G_Log_Text = "A側S2温度計座材質    『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S1圧力計
   //***************************
   // 『B018』
   s_Text = s_HEADS_DATA[901].TrimRight();   // S1圧力計種類

   //2004.05.19 圧力計条件追加
   //if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //*************************
      // S1圧力計目盛 1
      i_RecNo = 902;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S1圧力計目盛 2
         i_RecNo = 903;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S1圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[901].TrimRight()) + " ";

            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S1圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }

      //2018.04.12 目盛追加_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S1圧力計目盛 2
         i_RecNo = 903;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S1圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[901].TrimRight()) + " ";

            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S1圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }
      //2018.04.12 目盛追加_E

      } else {
         G_ErrLog_Text = "S1圧力計目盛1   RecNo:" + AnsiString(i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(16, 7, s_Text);
   G_Log_Text = "S1圧力計             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S2圧力計
   //***************************
   // 『B019』
   s_Text = s_HEADS_DATA[916].TrimRight();   // S2圧力計種類

   //2004.05.19 圧力計条件追加
   //if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //*************************
      // S2圧力計目盛 1
      i_RecNo = 917;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S2圧力計目盛 2
         i_RecNo = 918;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S2圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[916].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S2圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }


      //2018.04.12 目盛追加_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S2圧力計目盛 2
         i_RecNo = 918;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S2圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[916].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S2圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }

      //2018.04.12 目盛追加_E

      } else {
         G_ErrLog_Text = "S2圧力計目盛1   RecNo:" + AnsiString(i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(16, 15, s_Text);
   G_Log_Text = "S2圧力計             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S1ｴｱ抜継手名和
   //***************************
   // 『B020』
   s_Text = ZenToHan(s_HEADS_DATA[1216].TrimRight()) + ZenToHan(s_HEADS_DATA[1217].TrimRight());
   Excel_Cell_DataSet(17, 7, s_Text);
   G_Log_Text = "S1ｴｱ抜継手名和       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S2ｴｱ抜継手名和
   //***************************
   // 『B021』
   s_Text = ZenToHan(s_HEADS_DATA[1220].TrimRight()) + ZenToHan(s_HEADS_DATA[1221].TrimRight());
   Excel_Cell_DataSet(17, 15, s_Text);
   G_Log_Text = "S2ｴｱ抜継手名和       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S3継手・口径
   //***************************
   // 『B022』
   //2009.07.31 ASMEﾉｽﾞﾙ名称変更
   //s_S3TankanNM = "";   // S3端管種類『B024』,『B025』で使用
   //
   //i_RecNo = 1138;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text == "1" || s_Text == "3"){
   //   s_S3TankanNM = Get_Tankan_Data("S3", pcHeadsData);
   //   s_Text = s_S3TankanNM + "　" + s_HEADS_DATA[1192].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S3TankanNM = Get_Tankan_Data("S3", pcHeadsData);
   //   s_Text = s_S3TankanNM + "　" + s_HEADS_DATA[1192].TrimRight() + "ＷＦ";
   //
   //} else {
   //   if (s_HEADS_DATA[923].SubString(2,1) == "4") {
   //      s_Text = s_HEADS_DATA[1160].TrimRight() + "　" + Get_Flange_Data(s_HEADS_DATA[923].TrimRight());
   //   } else {
   //      s_Text = s_HEADS_DATA[1160].TrimRight();
   //   }
   //}
   //
   //Excel_Cell_DataSet(18, 8, s_Text);

   s_S3TankanNM = "";  // S3端菅種類
   s_S3TugiteNM = "";  // S3継手名称

   i_RecNo = 1138;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ) {
      s_S3TugiteNM = s_HEADS_DATA[1192].TrimRight();  // S3端菅継手名称
   } else {
      s_S3TugiteNM = s_HEADS_DATA[1160].TrimRight();  // S3ﾉｽﾞﾙ継手名称
   }
   if ( s_S3TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ１５０－ＮＰＳ" ) {
      s_S3TugiteNM = "ＡＳＭＥ ＣＬＳ１５０－" + s_S3TugiteNM.Delete(1,34);
   } else if ( s_S3TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ３００－ＮＰＳ" ) {
      s_S3TugiteNM = "ＡＳＭＥ ＣＬＳ３００－" + s_S3TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ﾉｽﾞﾙ規格追加
   if ( s_S3TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ４００－ＮＰＳ" ) {
      s_S3TugiteNM = "ＡＳＭＥ ＣＬＳ４００－" + s_S3TugiteNM.Delete(1,34);
   }
   // ***********************
   // 2016.04.22 CX-00追加
   if ( P_Model == "CX-00" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S1TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S1TugiteNM;
      }
   } else if ( P_Model == "CXW-00" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S1TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S1TugiteNM;
      }
   // 2013.04.03 SX-30追加
   //if ( P_Model == "SX-30" && s_S3TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
   } else if ( P_Model == "SX-30" && s_S3TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
   // ********************
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      s_Text = "リング端管 " + s_S3TugiteNM;
   // 2016.07.22 SX-30S追加
   } else if ( P_Model == "SX-30S" && s_S3TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      s_Text = "リング端管 " + s_S3TugiteNM;
    // ********************
   //if ( s_Text == "1" || s_Text == "3" ) {
   } else if ( s_Text == "1" || s_Text == "3" ) {
   // ********************
      s_S3TankanNM = Get_Tankan_Data("S3", pcHeadsData);  // S3端管種類
      s_Text = s_S3TankanNM + " " + s_S3TugiteNM;

   } else if ( s_Text == "2" ) {
      s_S3TankanNM = Get_Tankan_Data("S3", pcHeadsData);  // S3端管種類
      s_Text = s_S3TankanNM + " " + s_S3TugiteNM + "ＷＦ";

   } else {
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if ( s_HEADS_DATA[923].SubString(2,1) == "4" ) {  // フランジ付きノズル
      if ( s_HEADS_DATA[923].SubString(3,1) == "4" ) {  // フランジ付きノズル
      // ***********************
         s_Text = s_S3TugiteNM + " " + Get_Flange_Data(s_HEADS_DATA[923].TrimRight());
      } else {
         s_Text = s_S3TugiteNM;
      }

   }

   //2014.05.28 継手規格表記変更
   if ( s_S3TugiteNM.SubString(1,6) == "ＪＩＳ" ) {
   } else {
       s_Text = "注" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(18, 8, s_Text);
   G_Log_Text = "S3継手・口径         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //***************************

   //***************************
   //***  S4継手・口径
   //***************************
   // 『B023』
   //2009.07.31 ASMEﾉｽﾞﾙ名称変更
   //s_S4TankanNM = "";   // S4端管種類『B026』,『B027』で使用
   //
   //i_RecNo = 1139;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   ////2008.07.24 CX-10追加
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4)); //A側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //       s_Text = "リング端管１　" + s_HEADS_DATA[1152].TrimRight();
   //   } else  {
   //       s_Text = "リング端管２　" + s_HEADS_DATA[1152].TrimRight();
   //   }
   //} else if (s_Text == "1" || s_Text == "3"){
   ////if (s_Text == "1" || s_Text == "3"){
   ////*******************
   //   s_S4TankanNM = Get_Tankan_Data("S4", pcHeadsData);
   //   s_Text = s_S4TankanNM + "　" + s_HEADS_DATA[1196].TrimRight();
   //
   //} else if (s_Text == "2"){
   //   s_S4TankanNM = Get_Tankan_Data("S4", pcHeadsData);
   //   s_Text = s_S4TankanNM + "　" + s_HEADS_DATA[1196].TrimRight() + "ＷＦ";
   //
   //} else {
   //   if (s_HEADS_DATA[938].SubString(2,1) == "4") {
   //      s_Text = s_HEADS_DATA[1164].TrimRight() + "　" + Get_Flange_Data(s_HEADS_DATA[938].TrimRight());
   //   } else {
   //      s_Text = s_HEADS_DATA[1164].TrimRight();
   //   }
   //}
   //
   //Excel_Cell_DataSet(18, 16, s_Text);

   s_S4TankanNM = "";  // S4端菅種類
   s_S4TugiteNM = "";  // S4継手名称

   i_RecNo = 1139;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   if ( s_Text == "1" || s_Text == "2" || s_Text == "3" ){
      s_S4TugiteNM = s_HEADS_DATA[1196].TrimRight();  // S4端菅継手名称
   } else {
      s_S4TugiteNM = s_HEADS_DATA[1164].TrimRight();  // S4ﾉｽﾞﾙ継手名称
   }
   if ( s_S4TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ１５０－ＮＰＳ" ) {
      s_S4TugiteNM = "ＡＳＭＥ ＣＬＳ１５０－" + s_S4TugiteNM.Delete(1,34);
   } else if ( s_S4TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ３００－ＮＰＳ" ) {
      s_S4TugiteNM = "ＡＳＭＥ ＣＬＳ３００－" + s_S4TugiteNM.Delete(1,34);
   }
   // 2013.04.03 ﾉｽﾞﾙ規格追加
   if ( s_S4TugiteNM.SubString(1,34) == "ＡＳＭＥ　ＣＬＡＳＳ４００－ＮＰＳ" ) {
      s_S4TugiteNM = "ＡＳＭＥ ＣＬＳ４００－" + s_S4TugiteNM.Delete(1,34);
   }
   // ***********************

   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));  // A側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S4TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S4TugiteNM;
      }

   } else if ( P_Model == "CX-10D" ) {
      s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
      if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
         s_Text = "リング端管１ " + s_S4TugiteNM;
      } else  {
         s_Text = "リング端管２ " + s_S4TugiteNM;
      }

   } else if ( s_Text == "1" || s_Text == "3" ) {
      s_S4TankanNM = Get_Tankan_Data("S4", pcHeadsData);  // S4端管種類
      s_Text = s_S4TankanNM + " " + s_S4TugiteNM;

   } else if ( s_Text == "2" ) {
      s_S4TankanNM = Get_Tankan_Data("S4", pcHeadsData);  // S4端管種類
      s_Text = s_S4TankanNM + " " + s_S4TugiteNM + "ＷＦ";

   } else {
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if ( s_HEADS_DATA[938].SubString(2,1) == "4" ) {  // フランジ付きノズル
      if ( s_HEADS_DATA[938].SubString(3,1) == "4" ) {  // フランジ付きノズル
      // ***********************
         s_Text = s_S4TugiteNM + " " + Get_Flange_Data(s_HEADS_DATA[938].TrimRight());
      } else {
         s_Text = s_S4TugiteNM;
      }

   }

   //2014.05.28 継手規格表記変更
   if ( s_S4TugiteNM.SubString(1,6) == "ＪＩＳ" ) {
   } else {
       s_Text = "注" + s_Text;
   }
   //***************************

   Excel_Cell_DataSet(18, 16, s_Text);
   G_Log_Text = "S4継手・口径         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //***************************

   //***************************
   //***  S3温度計
   //***  S3ｽﾁｰﾑ区分
   //***************************
   s_Text = s_HEADS_DATA[928].TrimRight();      // S3温度計種類
   if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
      // 『B024』
      s_Text = AnsiString(s_HEADS_DATA[929].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[930].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1357].TrimRight()));


      Excel_Cell_DataSet(19, 7, s_Text);
      G_Log_Text = "S3温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B025』
      if (s_HEADS_DATA[846].TrimRight() == "1"){    // Bｽﾁｰﾑ区分
         s_Text = "ｽﾁｰﾑ用";
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(19, 11, s_Text);
      G_Log_Text = "S3ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      s_Text = "";
      // 『B024』
      Excel_Cell_DataSet(19, 7, s_Text);
      G_Log_Text = "S3温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B025』
      Excel_Cell_DataSet(19, 11, s_Text);
      G_Log_Text = "S3ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S4温度計
   //***  S4ｽﾁｰﾑ区分
   //***************************
   s_Text = s_HEADS_DATA[943].TrimRight();      // S4温度計種類
   if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
      // 『B026』
      s_Text = AnsiString(s_HEADS_DATA[944].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[945].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1358].TrimRight()));

      Excel_Cell_DataSet(19, 15, s_Text);
      G_Log_Text = "S4温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B027』
      if (s_HEADS_DATA[847].TrimRight() == "1"){    // Aｽﾁｰﾑ区分
         s_Text = "ｽﾁｰﾑ用";
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(19, 24, s_Text);
      G_Log_Text = "S4ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      s_Text = "";
      // 『B026』
      Excel_Cell_DataSet(19, 15, s_Text);
      G_Log_Text = "S4温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B027』
      Excel_Cell_DataSet(19, 24, s_Text);
      G_Log_Text = "S4ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //*********************************
   //***  S3圧力計 2002.10.19 S.Y
   //*********************************
   // 『B028』
   s_Text = s_HEADS_DATA[931].TrimRight();   // S3圧力計種類

   //2004.05.19 圧力計条件追加
   //if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //*************************
      // S3圧力計目盛 1
      i_RecNo = 932;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S3圧力計目盛 2
         i_RecNo = 933;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S3圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[931].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S3圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }

      //2018.04.12 目盛追加_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S3圧力計目盛 2
         i_RecNo = 933;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S3圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[931].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S3圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }
      //2018.04.12 目盛追加_E

      } else {
         G_ErrLog_Text = "S3圧力計目盛1   RecNo:" + AnsiString(i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(20, 7, s_Text);
   G_Log_Text = "S3圧力計             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //***************************
   //***  S4圧力計
   //***************************
   // 『B029』
   // S4圧力計目盛 1
   // i_RecNo = 947;
   // s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // if (AllFloatChk(s_Text.c_str(),0) == 0){
   //   s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

   // } else {
   //    G_ErrLog_Text = "S4圧力計目盛1   RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
   //    Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
   //    Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   //
   //    i_ErrFlg = 1;
   // }

   // S4圧力計目盛 2
   // i_RecNo = 948;
   // s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // if (AllFloatChk(s_Text.c_str(),0) == 0){
   //   s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
   // } else {
   //    G_ErrLog_Text = "S4圧力計目盛2   RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
   //    Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
   //    Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

   //    i_ErrFlg = 1;
   //  }

   // s_Text = s_HEADS_DATA[946].TrimRight();
   // if (s_Text == "01" || s_Text == "02" || s_Text == "03" || s_Text == "04" || s_Text == "09"){
   //    s_Text = Get_AturyokuKei(s_HEADS_DATA[946].TrimRight()) + " ";
   //    s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
   // } else {
   //    s_Text = "";
   // }

   // if (i_ErrFlg == 0){
   //    Excel_Cell_DataSet(20, 15, s_Text);
   //    G_Log_Text = "S4圧力計             『" + s_Text + "』をセット。";
   //    Write_Log(G_Log_Text);
   // } else {
   //    s_Text = "";
   //    Excel_Cell_DataSet(20, 15, s_Text);
   // }

   //***************************
   //***  S4圧力計 2002.10.19 S.Y
   //***************************
   // 『B029』
   s_Text = s_HEADS_DATA[946].TrimRight();   // S4圧力計種類

   //2004.05.19 圧力計条件追加
   //if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //*************************
      // S4圧力計目盛 1
      i_RecNo = 947;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S4圧力計目盛 2
         i_RecNo = 948;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S4圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[946].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S4圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }

      //2018.04.12 目盛追加_S
      } else if (s_Text == "-0.10") {
         s_Bunsi = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

         // S4圧力計目盛 2
         i_RecNo = 948;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            s_Bunbo = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

            // S4圧力計
            s_Text = Get_AturyokuKei(s_HEADS_DATA[946].TrimRight()) + " ";
            s_Text = s_Text + s_Bunsi + "/" + s_Bunbo;
         } else {
            G_ErrLog_Text = "S4圧力計目盛2   RecNo:" + AnsiString(i_RecNo)
                            + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

            s_Text = "";
         }
      //2018.04.12 目盛追加_E

      } else {
         G_ErrLog_Text = "S4圧力計目盛1   RecNo:" + AnsiString(i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "";
      }

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(20, 15, s_Text);
   G_Log_Text = "S4圧力計             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S3液体継手名和
   //***************************
   // 『B030』
   s_Text = ZenToHan(s_HEADS_DATA[1224].TrimRight()) + ZenToHan(s_HEADS_DATA[1225].TrimRight());
   Excel_Cell_DataSet(21, 7, s_Text);
   G_Log_Text = "S3液体継手名和       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  S4液体継手名和
   //***************************
   // 『B031』
   s_Text = ZenToHan(s_HEADS_DATA[1228].TrimRight()) + ZenToHan(s_HEADS_DATA[1229].TrimRight());
   Excel_Cell_DataSet(21, 15, s_Text);
   G_Log_Text = "S4液体継手名和       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側端管材質
   //***************************
   // 『B032』
   s_Text = s_HEADS_DATA[1136].TrimRight();  // S1端管

   switch (s_Text.ToIntDef(0)) {
      case 1: case 2: case 3:
         // 和文名称取得
         s_Text = Get_Zaisitu_Name(s_HEADS_DATA[1060].TrimRight());
         break;
      default:
         s_Text = "";
         break;
   }

   //2009.04.27 ﾐﾙｼｰﾄ注記
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ﾐﾙ";
      } else if (s_HEADS_DATA[1331].TrimRight() == "ヤンマー向リング端管　E333263"){
         s_Text = "ﾐﾙ";
      }
   }
   //********************
   //2010.06.07 CX-10D追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //*********************

   Excel_Cell_DataSet(22, 7, s_Text);
   G_Log_Text = "B側端管材質          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側端管材質
   //***************************
   // 『B033』
   s_Text = s_HEADS_DATA[1137].TrimRight();  // S2端管

   switch (s_Text.ToIntDef(0)) {
      case 1: case 2: case 3:
         // 和文名称取得
         s_Text = Get_Zaisitu_Name(s_HEADS_DATA[1061].TrimRight());
         break;
      default:
         s_Text = "";
         break;
   }

   //2008.07.24 CX-10追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      s_Text = Get_Zaisitu_Name(s_HEADS_DATA[1061].TrimRight());
   }
   //*******************

   //2009.04.27 ﾐﾙｼｰﾄ注記
   if (s_HEADS_DATA[436].TrimRight() == "0" && s_HEADS_DATA[437].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ﾐﾙ";
      } else if (s_HEADS_DATA[1331].TrimRight() == "ヤンマー向リング端管　E333263"){
         s_Text = "ﾐﾙ";
      }
   }
   //********************
   //2010.06.07 CX-10D追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      if (s_Text != ""){
         s_Text = s_Text + " (S2,S3)";
      }
   }
   //*********************

   Excel_Cell_DataSet(22, 15, s_Text);
   G_Log_Text = "A側端管材質          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B034』
   i_RecNo = 1136;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();     // S1端管
   if (s_Text == "1" || s_Text == "2" || s_Text == "3"){
      i_RecNo = 893;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S1ﾉｽﾞﾙ継手
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if (s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
      // ***********************
         //2017.10.06 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
         //// BｶﾞｽｹｯﾄCODEで検索
         //i_RecNo = 835;
         //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         //s_Text = Chg_Zaisitu_Code(s_Text,"1");
         //s_Text = Get_Zaisitu_Name(s_Text);
         s_Text = Get_SheetGasketMat("B", pcHeadsData);  // B側ｼｰﾄｶﾞｽｹｯﾄ材質
         //2017.10.06 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E

      } else {
         s_Text = "";
      }
   } else {
      s_Text = "";
   }
   //2010.06.07 CX-10D追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //*********************

   Excel_Cell_DataSet(23, 7, s_Text);
   G_Log_Text = "B側ｶﾞｽｹｯﾄ材質        『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B035』
   i_RecNo = 1137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();     // S2端管
   if (s_Text == "1" || s_Text == "2" || s_Text == "3"){
      i_RecNo = 908;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S2ﾉｽﾞﾙ継手
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if (s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
      // ***********************
         //2017.10.06 ｼｰﾄｶﾞｽｹｯﾄ材質変更_S
         // AｶﾞｽｹｯﾄCODEで検索
         //i_RecNo = 836;
         //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         //s_Text = Chg_Zaisitu_Code(s_Text,"1");
         //s_Text = Get_Zaisitu_Name(s_Text);
         s_Text = Get_SheetGasketMat("A", pcHeadsData);  // A側ｼｰﾄｶﾞｽｹｯﾄ材質
         //2017.10.06 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E

      } else {
         s_Text = "";
      }
   } else {
      s_Text = "";
   }

   //2008.07.24 CX-10追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      i_RecNo = 938;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S4ﾉｽﾞﾙ継手
      if (s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
         // AｶﾞｽｹｯﾄCODEで検索
         i_RecNo = 836;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
         s_Text = Chg_Zaisitu_Code(s_Text,"1");
         s_Text = Get_Zaisitu_Name(s_Text);

      } else {
         s_Text = "";
      }
   //2008.08.07 CX-10以外でも出力する
   //} else {
   //   s_Text = "";
   //********************************
   }
   //*******************
   //2010.06.07 CX-10D追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //*********************

   Excel_Cell_DataSet(23, 15, s_Text);
   G_Log_Text = "A側ｶﾞｽｹｯﾄ材質        『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側ﾌﾗﾝｼﾞ材質
   //***************************
   // 『B036』
   i_RecNo = 1050;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2018.02.09 標準フランジ材質追加_S
   i_RecNo = 37;
   i_CODE = s_HEADS_DATA[i_RecNo].ToIntDef(0); //フレームコード
   //2018.02.09 標準フランジ材質追加_E
   if (s_Text != "0000"){
      // SPFLMBで検索
      s_Text = Get_Zaisitu_Name(s_Text);

   } else {
      i_RecNo = 1136;   // S1端管
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      //2008.07.24 CX-10追加
      P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
      if (P_Model == "CX-10" ) {
         s_Text = "";
      //2010.06.07 CX-10D追加
      } else if ( P_Model == "CX-10D" ) {
         s_Text = "";
      //*********************
      //2016.04.22 CX-00追加
      } else if ( P_Model == "CX-00" ) {
         s_Text = "";
      } else if ( P_Model == "CXW-00" ) {
         s_Text = "";
      //********************
      // 2013.04.03 SX-30追加
      } else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
         s_Text = "";
      // ********************
      // 2016.07.22 SX-30S追加
      } else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
         s_Text = "";
      // *********************
      } else if (s_Text == "3"){
      //if (s_Text == "3"){
      //*******************
         s_Text = "相ﾌﾗﾝｼﾞ  SS40  W.F";
      //2018.02.09 標準フランジ材質追加_S
      } else if (s_Text == "1"){
         // フリーフランジ端管
         s_Text = "C.S.";
      } else if ( i_CODE == 11 || i_CODE == 12 || i_CODE == 13 || i_CODE == 61 ||
                  i_CODE == 21 || i_CODE == 22 || i_CODE == 26 || i_CODE == 62 ||
                  i_CODE == 16 || i_CODE == 14 || i_CODE == 15 || i_CODE == 63 ||
                  i_CODE == 29 || i_CODE == 27 || i_CODE == 28 || i_CODE == 64 ) {
         // パイプノズル
         s_Text = "C.S.";
      //2018.02.09 標準フランジ材質追加_S
      } else {
         s_Text = "";
      }
   }

   //2009.04.27 ﾐﾙｼｰﾄ注記
   i_RecNo = 437;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ﾐﾙ";
      } else if (s_HEADS_DATA[1136].ToIntDef(0) == 1 ||
                 s_HEADS_DATA[1136].ToIntDef(0) == 2 ) {  // 端管フランジ付
         s_Text = "C.S. ﾐﾙ";
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //} else if (s_HEADS_DATA[893].SubString(2,1) == "4") {  // フランジ付きノズル
      } else if (s_HEADS_DATA[893].SubString(3,1) == "4") {  // フランジ付きノズル
      // ***********************
         s_Text = "C.S. ﾐﾙ";
      }
   }
   //********************

   Excel_Cell_DataSet(24, 7, s_Text);
   G_Log_Text = "B側ﾌﾗﾝｼﾞ材質         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //***************************
   //***  A側ﾌﾗﾝｼﾞ材質
   //***************************
   // 『B037』
   i_RecNo = 1051;   // SPFLMA
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2018.02.09 標準フランジ材質追加_S
   i_RecNo = 37;
   i_CODE = s_HEADS_DATA[i_RecNo].ToIntDef(0); //フレームコード
   //2018.02.09 標準フランジ材質追加_E
   if (s_Text != "0000"){
      // SPFLMBで検索
      s_Text = Get_Zaisitu_Name(s_Text);

   } else {
      i_RecNo = 1137;   // S2端管
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      //2008.07.24 CX-10追加
      P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
      if (P_Model == "CX-10" ) {
         s_Text = "";
      //2010.06.07 CX-10D追加
      } else if ( P_Model == "CX-10D" ) {
         s_Text = "";
      //*********************
      } else if (s_Text == "3"){
      //if (s_Text == "3"){
      //*******************
         s_Text = "相ﾌﾗﾝｼﾞ  SS40  W.F";
      //2018.02.09 標準フランジ材質追加_S
      } else if (s_Text == "1"){
         // フリーフランジ端管
         s_Text = "C.S.";
      } else if ( i_CODE == 11 || i_CODE == 12 || i_CODE == 13 || i_CODE == 61 ||
                  i_CODE == 21 || i_CODE == 22 || i_CODE == 26 || i_CODE == 62 ||
                  i_CODE == 16 || i_CODE == 14 || i_CODE == 15 || i_CODE == 63 ||
                  i_CODE == 29 || i_CODE == 27 || i_CODE == 28 || i_CODE == 64 ) {
         // パイプノズル
         s_Text = "C.S.";
      //2018.02.09 標準フランジ材質追加_S
      } else {
         s_Text = "";
      }
   }

   //2009.04.27 ﾐﾙｼｰﾄ注記
   i_RecNo = 437;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      if (s_Text != ""){
         s_Text = s_Text + " ﾐﾙ";
      } else if (s_HEADS_DATA[1137].ToIntDef(0) == 1 ||
                 s_HEADS_DATA[1137].ToIntDef(0) == 2 ) {  // 端管フランジ付
         s_Text = "C.S. ﾐﾙ";
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //} else if (s_HEADS_DATA[908].SubString(2,1) == "4") {  // フランジ付きノズル
      } else if (s_HEADS_DATA[908].SubString(3,1) == "4") {  // フランジ付きノズル
      // ***********************
         s_Text = "C.S. ﾐﾙ";
      }
   }
   //********************

   Excel_Cell_DataSet(24, 15, s_Text);
   G_Log_Text = "A側ﾌﾗﾝｼﾞ材質         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  仕上タイトル
   //***  B側仕上名称
   //***************************
   // 『B038』,『B039』
   i_RecNo = 1136;   // S1端管
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2008.07.24 CX-10追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      ////2010.06.07 CX-10変更
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      ////********************
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // ＳＴＰＧ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色";
      //} else {
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   //2010.06.07 CX-10D追加
   } else if ( P_Model == "CX-10D" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // ＳＴＰＧ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色 (S1,S4)";
      //} else {
      //  s_Text = "内外  酸洗 (S1,S4)";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色 (S1,S4)";
      } else {
        s_Text = "内外  酸洗 (S1,S4)";
      }
      // ***********************
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   //*********************

   //2016.05.19 CX-00追加
   } else if ( P_Model == "CX-00" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // ＳＴＰＧ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色";
      //} else {
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      // ***********************
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else if ( P_Model == "CXW-00" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // ＳＴＰＧ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色";
      //} else {
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   //********************

   // 2013.04.03 SX-30追加
   } else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "STKM13A" ) {       // ＳＴＫＭ１３Ａ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色";
      //} else {
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // ********************

    // 2016.07.22 SX-30S加
   } else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "STKM13A" ) {       // ＳＴＫＭ１３Ａ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色";
      //} else {
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // ********************

   } else if (s_Text == 1 || s_Text == 2){
   //if (s_Text == 1 || s_Text == 2){
   //********************
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if(  s_Text.SubString(1,2) == "TP") {     // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      ////2004.01.28 ASME
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      ////***************
      //} else {
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else if (s_Text == 3){
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      s_Text = "外面のみﾌﾚｰﾑと同一色";
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      // 『B038』
      s_Text = "ノズル仕上";
      Excel_Cell_DataSet(25, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      i_RecNo = 904;   // S1ﾉｽﾞﾙ仕上
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text = Get_SiageMeisyou(s_Text);
      Excel_Cell_DataSet(25, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  A側仕上名称
   //***************************
   // 『B040』
   i_RecNo = 1137;   // S2端管
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2008.07.24 CX-10追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      // 『B040』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      ////2010.06.07 CX-10変更
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      ////********************
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // ＳＴＰＧ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色";
      //} else {
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   //2010.06.07 CX-10D追加
   } else if ( P_Model == "CX-10D" ) {
      // 『B040』
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if( s_Text.SubString(1,2) == "TP" ) {     // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "CAC703" ) {        // ＣＡＣ７０３
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "" ) {              // なし
      //  s_Text = "――――――";
      //} else if ( s_Text.TrimRight() == "STPG" ) {          // ＳＴＰＧ
      //  s_Text = "外面のみﾌﾚｰﾑと同一色 (S2,S3)";
      //} else {
      //  s_Text = "内外  酸洗 (S2,S3)";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色 (S2,S3)";
      } else {
        s_Text = "内外  酸洗 (S2,S3)";
      }
      // ***********************
      Excel_Cell_DataSet(25, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   //*********************

   } else if (s_Text == 1 || s_Text == 2){
   //if (s_Text == 1 || s_Text == 2){
   //********************
      // 2016.08.26 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      //s_Text = ZenToHan(s_Text);
      //if(  s_Text.SubString(1,2) == "TP") {     // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      ////2004.01.28 ASME
      //} else if ( s_Text.Pos( "B-265" ) > 0 ) { // チタンの場合は仕上５
      //  // 2010.06.07 線種変更
      //  //s_Text = "－－－－－";
      //  s_Text = "――――――";
      //  //********************
      ////***************
      //} else {
      //  // 『B040』
      //  s_Text = "内外  酸洗";
      //}
      // 2017.05.12 材質ｺｰﾄﾞ変更
      //s_Text = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      // ***********************
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(25, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else if (s_Text == 3){
      // 『B040』
      s_Text = "外面のみﾌﾚｰﾑと同一色";
      Excel_Cell_DataSet(25, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      // 『B040』
      i_RecNo = 919;   // S2ﾉｽﾞﾙ仕上
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text = Get_SiageMeisyou(s_Text);
      Excel_Cell_DataSet(25, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｃ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  締付ﾎﾞﾙﾄ材質
   //***************************
   // 『C001』
   i_RecNo = 1041;   // 材質指定
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   // 2017.06.09 海外製締付ﾎﾞﾙﾄ
   if ( s_Text == "1186" ) {
      // SNB7相当材
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "2.000" || s_Text2 == "36.000" || s_Text2 == "52.000"){
         // ﾎﾞﾙﾄ径2”、M36、M52
         i_RecNo = 438;   // 業界ｺｰﾄﾞ
         s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
         if (s_Text2 == "0019"){
            // 業界ｺｰﾄﾞ19
            s_Text = "SA-193-GrB7/194海外製";
            i_RecNo = 29;   // 規格ｺｰﾄﾞ
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 6 ) {
                // 規格ｺｰﾄﾞが06:ＡＳＭＥ
                s_Text = "SA-193-GrB7/194";
            }
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 31 ) {
                // 規格ｺｰﾄﾞが31:原子力
                s_Text = "SA-193-GrB7/194";
            }
         } else {
            s_Text  = Search_HD_MATERIAL(s_Text);
         }
      } else {
         s_Text  = Search_HD_MATERIAL(s_Text);
      }
   } else if ( s_Text == "32C2" ) {
      // SA-193-GrB7
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "2.000" || s_Text2 == "36.000" || s_Text2 == "52.000"){
         // ﾎﾞﾙﾄ径2”、M36、M52
         i_RecNo = 438;   // 業界ｺｰﾄﾞ
         s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
         if (s_Text2 == "0059" || s_Text2 == "0060"){
            // 業界ｺｰﾄﾞ59,60
            s_Text = "SA-193-GrB7/194海外製";
            i_RecNo = 29;   // 規格ｺｰﾄﾞ
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 6 ) {
                // 規格ｺｰﾄﾞが06:ＡＳＭＥ
                s_Text = "SA-193-GrB7/194";
            }
            if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 31 ) {
                // 規格ｺｰﾄﾞが31:原子力
                s_Text = "SA-193-GrB7/194";
            }
         } else if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) == 13 ) {
             // 規格ｺｰﾄﾞが13:ＡＢＳ
             s_Text = "SA-193-GrB7/194海外製";
         } else {
            s_Text  = Search_HD_MATERIAL(s_Text);
         }
      } else {
         s_Text  = Search_HD_MATERIAL(s_Text);
      }
      // *************************
   //if (( s_Text != "" ) && ( s_Text != "0000" ) ){
   } else if (( s_Text != "" ) && ( s_Text != "0000" ) ){
      s_Text  = Search_HD_MATERIAL(s_Text);
   } else {
      i_RecNo = 1021;   // BOL22
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }

   // 2012.01.20 中国材質
   i_RecNo = 1107;
   s_ChinaM = s_HEADS_DATA[i_RecNo].TrimRight();

   s_Text2 = G_KS_Syurui + "－" + G_KS_Model;
   if ( s_Text2 == "ＵＸ－１０" ) {                            // 中国材質
   } else if ( s_Text2 == "ＬＸ－００" ) {                     // 中国材質
   } else if ( s_Text2 == "ＬＸ－１０" ) {                     // 中国材質
   } else if ( s_Text2 == "ＬＸ－３０" ) {                     // 中国材質
   } else if ( s_Text2 == "ＬＸ－５０" ) {                     // 中国材質
   } else if ( s_Text2 == "ＳＸ－２０" ) {                     // 中国材質
   } else if ( s_Text2 == "ＳＸ－４１" ) {                     // 中国材質
   } else if ( s_Text2 == "ＳＸ－４３" ) {                     // 中国材質
   } else if ( s_Text2 == "ＲＸ－１１" ) {                     // 中国材質
   } else if ( s_Text2 == "ＲＸ－１３" ) {                     // 中国材質
   } else if ( s_Text2 == "ＲＸ－３０" ) {                     // 中国材質
   } else if ( s_Text2 == "ＲＸ－５０" ) {                     // 中国材質
   } else if ( s_Text2 == "ＲＸ－７０" ) {                     // 中国材質
   } else {
       s_ChinaM = "";
   }

   if ( s_ChinaM == "CHINA" ) {                                // 中国材質
       if ( s_Text == "SS400" ) {
           s_Text = "Q235B";
       }
   }
   // *******************

   //2009.04.27 ﾐﾙｼｰﾄ注記
   i_RecNo = 435;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      s_Text = s_Text + " ﾐﾙ";
   }
   //********************

   Excel_Cell_DataSet(26, 9, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄ 材質         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付ﾎﾞﾙﾄ 径
   //***************************
   // ﾎﾞﾙﾄ本数が0なら出力しない
   // 『C002』
   i_RecNo = 1286;   // ﾎﾞﾙﾄ本数L
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) != 0){
      i_RecNo = 1019;   // BDI
      s_Text = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
      if (s_Text == "42"){
         //2004.01.22
         //s_Text = "1 5/8-8UN";
         s_Text = "1 5/8""-8UN";
         //**********
      } else if (s_Text == "52"){
         //2004.01.22
         //s_Text = "2-8UN";
         s_Text = "2""-8UN";
         //**********
      } else {
         s_Text = "M" + s_Text;
      }

      // 2017.05.29 締付ﾎﾞﾙﾄ径処理修正
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "1.630" ){
         // 1 5/8-8UN
         s_Text = "1 5/8""-8UN";
      } else if (s_Text2 == "2.000" ){
         // 2-8UN
         s_Text = "2""-8UN";
      }
      // *****************************

      s_Text = s_Text + "×" + s_HEADS_DATA[1376].TrimRight() + "L";   //参照データ変更 02/10/24 Miya
   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(26, 12, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄ 径          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付ﾎﾞﾙﾄ 本数L
   //***************************
   // ﾎﾞﾙﾄ本数が0なら出力しない
   // 『C003』
   i_RecNo = 1286;   // ﾎﾞﾙﾄ本数L
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) == 0){
      s_Text = "";
   }
   Excel_Cell_DataSet(26, 24, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄ 本数Ｌ      『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付ﾎﾞﾙﾄ ﾎﾞﾙﾄ仕上
   //***************************
   // 『C004』
   i_RecNo = 1042;   // SPBLF
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "01") {
      s_Text = "有色ｸﾛﾒｰﾄ";
   } else if (s_Text == "02"){
      s_Text = "MFZn-Ⅱ";
   } else if (s_Text == "03"){
      s_Text = "ﾍﾔｰﾗｲﾝ";
   //2009.06.03 三価ｸﾛﾒｰﾄﾒｯｷ追加
   } else if (s_Text == "04"){
      s_Text = "三価ｸﾛﾒｰﾄ";
   //***************************
   //2013.11.08 ﾀｹｺｰﾄ追加
   } else if (s_Text == "05"){
      s_Text = "タケコート";
   //********************
   //2018.08.20 C3604 ｸﾛﾑﾒｯｷﾅｯﾄ追加_S
   } else if (s_Text == "06"){
      s_Text = "C3604 ｸﾛﾑﾒｯｷﾅｯﾄ";
   //2018.08.20 C3604 ｸﾛﾑﾒｯｷﾅｯﾄ追加_E
   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(27, 7, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄ ﾎﾞﾙﾄ仕上     RecNo:" + FormatFloat("0000",i_RecNo) + "より『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付ﾎﾞﾙﾄ BLS
   //***************************
   // ﾎﾞﾙﾄ本数が0なら出力しない
   // 『C005』
   i_RecNo = 1287;   // ﾎﾞﾙﾄ本数S
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) != 0){
      i_RecNo = 1019;   // BDI
      s_Text = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
      if (s_Text == "42"){
         //2004.01.24
         //s_Text = "1 5/8-8UN";
         s_Text = "1 5/8""-8UN";
         //**********
      } else if (s_Text == "52"){
         //2004.01.24
         //s_Text = "2-8UN";
         s_Text = "2""-8UN";
         //**********
      } else {
         s_Text = "M" + s_Text;
      }

      // 2017.05.29 締付ﾎﾞﾙﾄ径処理修正
      i_RecNo = 1019;   // BDI
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2 == "1.630" ){
         // 1 5/8-8UN
         s_Text = "1 5/8""-8UN";
      } else if (s_Text2 == "2.000" ){
         // 2-8UN
         s_Text = "2""-8UN";
      }
      // *****************************

      s_Text = s_Text + "×" + s_HEADS_DATA[1377].TrimRight() + "L";   //参照データ変更 02/10/24 Miya
   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(27, 12, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄ ﾎﾞﾙﾄ仕上    『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付ﾎﾞﾙﾄ 本数S
   //***************************
   // 『C006』
   i_RecNo = 1287;   // Sﾎﾞﾙﾄ本数
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) == 0){
      s_Text = "";
   }
   Excel_Cell_DataSet(27, 24, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄ 本数Ｓ       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2014.05.28 継手規格表記変更
   //***************************
   //***  Sﾌﾚｰﾑ孔明
   //***************************

   AnsiString s_S1Nzl_Syurui;                         // S1ﾉｽﾞﾙ種類
   AnsiString s_S1Nzl_Tugite;                         // S1ﾉｽﾞﾙ継手
   AnsiString s_S2Nzl_Syurui;                         // S2ﾉｽﾞﾙ種類
   AnsiString s_S2Nzl_Tugite;                         // S2ﾉｽﾞﾙ継手
   AnsiString s_S3Nzl_Syurui;                         // S3ﾉｽﾞﾙ種類
   AnsiString s_S3Nzl_Tugite;                         // S3ﾉｽﾞﾙ継手
   AnsiString s_S4Nzl_Syurui;                         // S4ﾉｽﾞﾙ種類
   AnsiString s_S4Nzl_Tugite;                         // S4ﾉｽﾞﾙ継手

   AnsiString s_S[5];
   int i_OgataFlg;               // 0:小型中型,1:大型,2:Eﾒﾀﾙ

   s_S1Nzl_Syurui = s_HEADS_DATA[891].TrimRight();    // S1ﾉｽﾞﾙ種類
   s_S1Nzl_Tugite = s_HEADS_DATA[893].TrimRight();    // S1ﾉｽﾞﾙ継手
   s_S2Nzl_Syurui = s_HEADS_DATA[906].TrimRight();    // S2ﾉｽﾞﾙ種類
   s_S2Nzl_Tugite = s_HEADS_DATA[908].TrimRight();    // S2ﾉｽﾞﾙ継手
   s_S3Nzl_Syurui = s_HEADS_DATA[921].TrimRight();    // S3ﾉｽﾞﾙ種類
   s_S3Nzl_Tugite = s_HEADS_DATA[923].TrimRight();    // S3ﾉｽﾞﾙ継手
   s_S4Nzl_Syurui = s_HEADS_DATA[936].TrimRight();    // S4ﾉｽﾞﾙ種類
   s_S4Nzl_Tugite = s_HEADS_DATA[938].TrimRight();    // S4ﾉｽﾞﾙ継手

   s_Text = G_KS_Syurui + "－" + G_KS_Model;

   if (s_Text == "ＵＸ－８０" || s_Text == "ＵＸ－９０" || s_Text == "ＵＸ－１００" ||
       s_Text == "ＵＸ－１００Ｒ" || s_Text == "ＵＸ－１３０" || s_Text == "ＳＸ－７０" ||
       s_Text == "ＳＸ－８０Ｓ" || s_Text == "ＳＸ－９０" || s_Text == "ＳＸ－９０Ｍ" ||
	   s_Text == "ＳＸ－９０Ｌ" || s_Text == "ＲＸ－７０" || s_Text == "ＲＸ－９０" ||
       s_Text == "ＲＸ－１００" || s_Text == "ＷＸ－９０" || s_Text == "ＣＸ－１０" ||
       s_Text == "ＣＸ－１０Ｄ" || s_Text == "ＮＸ－９０"){
           i_OgataFlg = 1;
   } else if (s_Text == "ＵＸ－１０" || s_Text == "ＵＸ－２０" ||
       s_Text == "ＵＸ－３０" || s_Text == "ＬＸ－２０" ||
       s_Text == "ＳＸ－４０" || s_Text == "ＳＸ－４１" ||
       s_Text == "ＳＸ－４３" || s_Text == "ＥＸ－１５"){
           i_OgataFlg = 2;
   } else {
           i_OgataFlg = 0;
   }

   //*************************
   //***  S1　部分
   //*************************
   if (s_S1Nzl_Syurui == "0"){
       s_S[1] = "0";
   } else {
       s_Text = s_S1Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[1] = "N";
           } else {
               s_S[1] = "K";
           }
       } else if (s_Text == "2"){
           s_S[1] = "T";
       } else if (s_Text == "3"){
           s_S[1] = "K";
       } else if (s_Text == "4"){
           s_S[1] = "1";
       } else {
           G_Log_Text = "S1ﾉｽﾞﾙ継手『" + s_S1Nzl_Tugite + "』により、S1孔明ﾃﾞｰﾀが異常です。";
           Write_Error_Log(G_Log_Text);
           s_S[1] = "*";
       }
   }

   //*************************
   //***  S2　部分
   //*************************
   if (s_S2Nzl_Syurui == "0"){
       s_S[2] = "0";
   } else {
       s_Text = s_S2Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[2] = "N";
           } else {
               s_S[2] = "K";
           }
       } else if (s_Text == "2"){
           s_S[2] = "T";
       } else if (s_Text == "3"){
           s_S[2] = "K";
       } else if (s_Text == "4"){
           s_S[2] = "2";
       } else {
           G_Log_Text = "S2ﾉｽﾞﾙ継手『" + s_S2Nzl_Tugite + "』により、S2孔明ﾃﾞｰﾀが異常です。";
           Write_Error_Log(G_Log_Text);
           s_S[2] = "*";
       }
   }

   //*************************
   //***  S3　部分
   //*************************
   if (s_S3Nzl_Syurui == "0"){
       s_S[3] = "0";
   } else {
       s_Text = s_S3Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[3] = "N";
           } else {
               s_S[3] = "K";
           }
       } else if (s_Text == "2"){
           s_S[3] = "T";
       } else if (s_Text == "3"){
           s_S[3] = "K";
       } else if (s_Text == "4"){
           s_S[3] = "3";
       } else {
           G_Log_Text = "S3ﾉｽﾞﾙ継手『" + s_S3Nzl_Tugite + "』により、S3孔明ﾃﾞｰﾀが異常です。";
           Write_Error_Log(G_Log_Text);
           s_S[3] = "*";
       }
   }

   //*************************
   //***  S4　部分
   //*************************
   if (s_S4Nzl_Syurui == "0"){
       s_S[4] = "0";
   } else {
       s_Text = s_S4Nzl_Tugite.SubString(3,1);

       if (s_Text == "1"){
           if (i_OgataFlg == 1){
               s_S[4] = "N";
           } else {
               s_S[4] = "K";
           }
       } else if (s_Text == "2"){
           s_S[4] = "T";
       } else if (s_Text == "3"){
           s_S[4] = "K";
       } else if (s_Text == "4"){
           s_S[4] = "4";
       } else {
           G_Log_Text = "S4ﾉｽﾞﾙ継手『" + s_S4Nzl_Tugite + "』により、S4孔明ﾃﾞｰﾀが異常です。";
           Write_Error_Log(G_Log_Text);
           s_S[4] = "*";
       }
   }

   // 各取得項目結合
   s_Text = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

   Excel_Cell_DataSet(29, 1, s_Text);
   G_Log_Text = "Sﾌﾚｰﾑ孔明             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //***************************

   //***************************
   //***  ﾌﾚｰﾑ材質 材質
   //***************************
   // 『C007』 02.09.20変更 N.UCHIDA     Get_Zaisitu_Name

   switch (s_HEADS_DATA[37].ToIntDef(0)) {
      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
         s_Text = s_HEADS_DATA[34].TrimRight();
         //2008.07.24 RX-10B追加
         //if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX"){
         if (s_Text.SubString(1,2) == "UX" || s_Text.SubString(1,2) == "LX" || s_Text.SubString(1,2) == "RX"){
         //*********************
            s_Text = "ＳＵＳ３０４";

         } else {
            s_Text = "ＳＳ４００";
         }

         break;
      default :
         i_RecNo = 1044;   // SPFRM
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

         if (s_Text == "0000" || s_Text == "1040"){
            s_Text = "ＳＳ４００";
         } else {
            i_RecNo = 1044;   // SPFRM
            s_Text = Get_Zaisitu_Name(s_HEADS_DATA[i_RecNo].TrimRight());
         }
         break;
   }

   // 2012.01.20 中国材質
   if ( s_ChinaM == "CHINA" ) {                                // 中国材質
       if ( s_Text == "ＳＳ４００" ) {
           s_Text = "Ｑ２３５Ｂ";
       }
   }
   // *******************

   //2009.04.27 ﾐﾙｼｰﾄ注記
   i_RecNo = 434;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      s_Text = s_Text + " ﾐﾙ";
   }
   //********************

   Excel_Cell_DataSet(28, 9, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ材質 材質         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ材質 厚さ
   //***************************
   // 『C008』
   i_RecNo = 1284;   // ﾌﾚｰﾑ厚
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(28, 15, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ材質 厚さ         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ材質 幅
   //***************************
   // 『C009』
   i_RecNo = 1285;   // ﾌﾚｰﾑ幅
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(28, 22, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ材質 幅           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2013/07/30 GB表記訂正
   ////***************************
   ////***  上部LU1
   ////***************************
   //// 『C010』
   //i_RecNo = 1022;   // RLU1
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(30, 10, s_Text);
   //G_Log_Text = "上部ＬＵ１            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //
   ////***************************
   ////***  上部LU2
   ////***************************
   //// 『C011』
   //i_RecNo = 1023;   // RLU2
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(30, 12, s_Text);
   //G_Log_Text = "上部ＬＵ２            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //
   ////***************************
   ////***  上部LU5
   ////***************************
   //// 『C011』
   //i_RecNo = 1378;   // RLU5
   //if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) != 0 ) {
   //  s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //  Excel_Cell_DataSet(30, 12, s_Text);
   //  Excel_Cell_DataSet(30, 11, "LU3 =");
   //  G_Log_Text = "上部ＬＵ３            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』を上書セット。";
   //  Write_Log(G_Log_Text);
   //}

   //***************************
   //***  上部LU
   //***************************
   // 『C010』
   i_RecNo = 1022;   // RLU1
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   i_RecNo = 1378;   // RLU5
   if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) != 0 ) {
     i_RecNo = 1378;   // RLU5
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(30, 10, s_Text);
     Excel_Cell_DataSet(30, 7, "LU1/3 =");
     G_Log_Text = "上部ＬＵ              RecNo:1022/1378『" + s_Text + "』を上書セット。";
     Write_Log(G_Log_Text);
   } else {
     i_RecNo = 1023;   // RLU2
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(30, 10, s_Text);
     G_Log_Text = "上部ＬＵ              RecNo:1022/1023『" + s_Text + "』を上書セット。";
     Write_Log(G_Log_Text);
   }

   //***************************
   //***  下部LU
   //***************************
   // 『C011』
   i_RecNo = 1024;   // RLL1
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   i_RecNo = 1379;   // RLL5
   if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) != 0 ) {
     i_RecNo = 1379;   // RLL5
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(30, 12, s_Text);
     Excel_Cell_DataSet(30, 11, "LL1/3 =");
     G_Log_Text = "下部ＬＬ              RecNo:1024/1379『" + s_Text + "』を上書セット。";
     Write_Log(G_Log_Text);
   } else {
     i_RecNo = 1025;   // RLL2
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(30, 12, s_Text);
     G_Log_Text = "下部ＬＵ              RecNo:1024/1025『" + s_Text + "』を上書セット。";
     Write_Log(G_Log_Text);
   }
   // *********************

   //***************************
   //***  ｶﾞｲﾄﾞﾊﾞｰ種類
   //***************************
   // 『C012』
   i_RecNo = 1290;   // ｶﾞｲﾄﾞﾊﾞｰ種類
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(30, 16, s_Text);
   G_Log_Text = "ｶﾞｲﾄﾞﾊﾞｰ種類          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2013/07/30 GB表記訂正
   ////***************************
   ////***  下部LU1
   ////***************************
   //// 『C013』
   //i_RecNo = 1024;   // RLL1
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(31, 10, s_Text);
   //G_Log_Text = "下部ＬＵ１            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //
   ////***************************
   ////***  下部LU2
   ////***************************
   //// 『C014』
   //i_RecNo = 1025;   // RLL2
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(31, 12, s_Text);
   //G_Log_Text = "下部ＬＵ２            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //
   ////***************************
   ////***  下部LU5
   ////***************************
   //// 『C014』
   //i_RecNo = 1379;   // RLL5
   //if ( s_HEADS_DATA[i_RecNo].ToIntDef(0) != 0 ) {
   //  s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //  Excel_Cell_DataSet(31, 12, s_Text);
   //  Excel_Cell_DataSet(31, 11, "LL3 =");
   //  G_Log_Text = "下部ＬＵ３            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』を上書セット。";
   //  Write_Log(G_Log_Text);
   //}

   //***************************
   //***  レールセンター
   //***************************
   s_Text2 = G_KS_Syurui + "－" + G_KS_Model;
   if ( s_Text2 == "ＬＸ－３０" ) {                            // ＬＸ－３０
       s_Text = "２ｍｍ以内";
   } else {                                                    // その他型式
       s_Text = "３ｍｍ以内";
   }
   Excel_Cell_DataSet(31, 7, s_Text);
   G_Log_Text = "レールセンター        『" + s_Text + "』を上書セット。";
   Write_Log(G_Log_Text);
   // *********************

   //***************************
   //***  Ｃ
   //***************************
   // 『C015』
   i_RecNo = 1026;   // CMAX
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(31, 18, s_Text);
   G_Log_Text = "Ｃ                    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  Ｐ
   //***************************
   // 『C016』
   i_RecNo = 1028;   // P
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(31, 24, s_Text);
   G_Log_Text = "Ｐ                    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //****************************
   //***  塗装色
   //****************************
   // 『C017』
   i_RecNo = 31;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.ToIntDef(0) == 1){
      // RecNo:539 塗装色をｾｯﾄ
      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   } else if (s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 5) {
      // 塗装色ｺｰﾄﾞﾃｰﾌﾞﾙ内和名称 + RecNo:539 塗装色をｾｯﾄ
//      s_Text = Search_TOSOU_CODE(s_Text, "J");

      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   } else {
      // 塗装色ｺｰﾄﾞﾃｰﾌﾞﾙ内和名称をｾｯﾄ
      s_Text = Search_TOSOU_CODE(s_Text, "J");
   }

   // 2012.12.06 エポキシ塗装注記修正
   //// 2012.11.27 エポキシ塗装注記
   //i_1365Flg = 0;                                       // ﾌﾚｰﾑ仕上注記無
   //// ***************************
   //
   ////2012.06.28 EP用ｶﾞｲﾄﾞﾊﾞｰｻﾎﾟｰﾄ及びSﾌﾚｰﾑﾍﾞｰｽﾒｯｷ
   //s_Text2 = G_KS_Syurui + "－" + G_KS_Model;
   //if ( s_Text2 == "ＬＸ－００" ||
   //       s_Text2 == "ＬＸ－１０" ||
   //       s_Text2 == "ＲＸ－１１" ||
   //       s_Text2 == "ＲＸ－１３" ) {
   //     i_RecNo = 37;
   //     switch (s_HEADS_DATA[i_RecNo].ToIntDef(0)) {      // フレームコード
   //        case 71: case 72: case 81: case 91: case 381: case 391:  // ＥＰシリーズ
   //           i_RecNo = 31;
   //           s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   //           if ( s_Text2.ToIntDef(0) != 8 ) {  // 日阪標準色以外
   //              // 2012.11.27 エポキシ塗装注記
   //              i_1365Flg = 1;                           // ﾌﾚｰﾑ仕上注記有
   //              //i_RecNo = 1365;   // 注記(フレーム仕上)
   //              //s_Text = s_Text + "  " + s_HEADS_DATA[i_RecNo].TrimRight();
   //              // ***************************
   //           }
   //           break;
   //        default :
   //           break;
   //     }
   //
   //}
   ////********************************************
   //
   //// 2012.11.27 エポキシ塗装注記
   //i_RecNo = 20;
   //switch (s_HEADS_DATA[i_RecNo].ToIntDef(0)) {         // 業界コード
   //     case 19:
   //        i_1365Flg = 1;                                 // ﾌﾚｰﾑ仕上注記有
   //        break;
   //     case 21: case 59: case 60:
   //        i_RecNo = 24;
   //        s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();   // 用途２
   //        if ( s_Text2 == "J" ) {                        // 空調用途
   //           i_1365Flg = 1;                              // ﾌﾚｰﾑ仕上注記有
   //        }
   //        break;
   //     default :
   //        break;
   //}

   i_RecNo = 1365;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   if( s_Text2.Length() != 0 ) {
      i_1365Flg = 1;
   }

   i_RecNo = 31;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text2.ToIntDef(0) == 6 ) {  // ヘヤーライン
      i_1365Flg = 0;
   } else if ( s_Text2.ToIntDef(0) == 7 ) {  // ビーズ
      i_1365Flg = 0;
   }
   // *******************************

   if ( i_1365Flg == 1 ) {                              // ﾌﾚｰﾑ仕上注記有
      i_RecNo = 1365;                                   // 注記(フレーム仕上)
      s_Text = s_Text + "  " + s_HEADS_DATA[i_RecNo].TrimRight();
   }
   // ***************************

   Excel_Cell_DataSet(32, 6, s_Text);
   G_Log_Text = "塗装色               『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ボルトカバー
   //***************************
   // 『C018』  2003/06/04 文言はHEADSテキストの内容を無条件で出力
//   i_RecNo = 1092;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if (s_Text != "0"){
//      i_RecNo = 1096;
//      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//      if (s_Text != "0"){
        i_RecNo = 1367;
        s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//         i_RecNo = 1019;   // BDI
//         s_Text = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
//         if ( s_Text == "24" || s_Text == "30" ) {
//           s_Text = "ボルトカバー";
//         } else {
//           s_Text = "カバー＋グリス";
//         }
//      } else {
//         s_Text = "";
//      }
//   } else {
//      s_Text = "";
//   }

   Excel_Cell_DataSet(36, 1, s_Text);
   G_Log_Text = "ボルトカバー         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //**********************************************************
   //***  ﾎﾞﾙﾄ本数L,S　数値ﾁｪｯｸ
   //***    『C019』,『C020』,『C021』の為の処理前ﾁｪｯｸ
   //**********************************************************
   i_ErrFlg = 0;

   // ﾎﾞﾙﾄ本数L
   i_RecNo = 1286;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      i_Value_1 = s_Text.ToIntDef(0);   // ﾎﾞﾙﾄ本数L
   } else {
      G_ErrLog_Text = "ﾎﾞﾙﾄ本数Ｌ      RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      i_ErrFlg = 1;
   }

   // ﾎﾞﾙﾄ本数S
   i_RecNo = 1287;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      i_Value_2 = s_Text.ToIntDef(0);   // ﾎﾞﾙﾄ本数S
   } else {
      G_ErrLog_Text = "ﾎﾞﾙﾄ本数Ｓ      RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      i_ErrFlg = 1;
   }

   //***************************
   //***  BLCP1
   //***************************
   // 『C019』,『C020』
   if (i_ErrFlg == 0){
      i_RecNo = 1092;      // BLCP1
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         // BLCP1 『C019』
         s_Text = s_Text + "L ";
         Excel_Cell_DataSet(37, 1, s_Text);
         G_Log_Text = "BLCP1                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCP1本数 『C020』
         s_Text = AnsiString(i_Value_1 + i_Value_2) +"本";
         Excel_Cell_DataSet(37, 3, s_Text);
         G_Log_Text = "BLCP1 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(37, 1, s_Text);
         Excel_Cell_DataSet(37, 3, s_Text);
         G_Log_Text = "BLCP1                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCP1 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(37, 1, s_Text);
      Excel_Cell_DataSet(37, 3, s_Text);
      G_Log_Text = "BLCP1                『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
      G_Log_Text = "BLCP1 本数           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  BLCEL
   //***************************
   // 『C021』,『C022』
   if (i_ErrFlg == 0){
      i_RecNo = 1096;      // BLCEL
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         // BLCEL 『C021』
         s_Text = s_Text + "L+C";
         Excel_Cell_DataSet(38, 1, s_Text);
         G_Log_Text = "BLCEL                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCEL本数 『C022』
         s_Text = AnsiString(i_Value_1) +"本";
         Excel_Cell_DataSet(38, 3, s_Text);
         G_Log_Text = "BLCEL 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(38, 1, s_Text);
         Excel_Cell_DataSet(38, 3, s_Text);
         G_Log_Text = "BLCEL                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCEL 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(38, 1, s_Text);
      Excel_Cell_DataSet(38, 3, s_Text);
      G_Log_Text = "BLCEL                『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
      G_Log_Text = "BLCEL 本数           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  BLCES
   //***************************
   // 『C023』,『C024』
   if (i_ErrFlg == 0){
      i_RecNo = 1097;      // BLCES
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      // 2016.08.30 ボルトカバー出力条件修正
      //if (s_Text.ToIntDef(0) != 0 ){
      if (s_Text.ToIntDef(0) != 0 && i_Value_2 != 0){
      // ***********************************
         // BLCEL 『C023』
         s_Text = s_Text + "L+C";
         Excel_Cell_DataSet(39, 1, s_Text);
         G_Log_Text = "BLCES                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCEL本数 『C024』
         s_Text = AnsiString(i_Value_2) +"本";
         Excel_Cell_DataSet(39, 3, s_Text);
         G_Log_Text = "BLCES 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(39, 1, s_Text);
         Excel_Cell_DataSet(39, 3, s_Text);
         G_Log_Text = "BLCES                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCES 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(39, 1, s_Text);
      Excel_Cell_DataSet(39, 3, s_Text);
      G_Log_Text = "BLCES                『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
      G_Log_Text = "BLCES 本数           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  ｱﾝｶｰﾎﾞﾙﾄ　種類
   //***************************
   // 『C025』 02.09.20変更 N.UCHIDA
   i_RecNo = 1045;   // SPANC
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2014.09.22 ｱﾝｶｰｺｰﾄﾞ変更
   //if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07"){
   if (s_Text == "00" || s_Text == "01"){
   // ***********************
      s_Text = "アンカーボルト・ナット";
   } else if (s_Text == "02") {
      s_Text = "ホールインアンカー";
   } else if (s_Text == "03") {
      s_Text = "ケミカル・アンカー";
   } else if (s_Text == "04") {
      s_Text = "フシ付きボルト・ナット";
   } else if (s_Text == "05") {
      s_Text = "セットボルト・ナット";
   // 2014.09.22 ｱﾝｶｰｺｰﾄﾞ変更
   } else if (s_Text == "06") {
      s_Text = "アンカーボルト・ナット（ＪＡ型）";
   } else if (s_Text == "07") {
      s_Text = "アンカーボルト・ナット（Ｊ型）";
   // ***********************
   } else if (s_Text == "09") {
      s_Text = "アンカーボルト不要";
   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(33, 8, s_Text);
   G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄｺｰﾄﾞ          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if ( s_Text == "アンカーボルト不要" || s_Text == "" ) {
      Excel_Cell_DataSet(34, 8, "");
      Excel_Cell_DataSet(34, 15, "");
      Excel_Cell_DataSet(34, 23, "");
   } else {
      //***************************
      //***  ｱﾝｶｰﾎﾞﾙﾄ　長さ
      //***************************
      // 『C026』
      // 種別ｺｰﾄﾞの取得
      s_Text = s_HEADS_DATA[1045].TrimRight();  // SPANC
      if (s_Text == "00" || s_Text == "01" || s_Text == "06" || s_Text == "07"){
         s_Text = s_HEADS_DATA[264].TrimRight();  // ﾄﾞﾚﾝﾊﾟﾝ有無
         if (s_Text == "1.00"){
            s_Syubetu = "1";
         } else {
            s_Syubetu = "0";
         }
      //2003.12.24 ﾎｰﾙｲﾝのSUSによる区分の削除
      //} else if (s_Text == "02") {
      //   s_Text = s_HEADS_DATA[37].TrimRight();   // ﾌﾚｰﾑ型式ｺｰﾄﾞ
      //   switch (s_Text.ToIntDef(0)){
      //      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
      //         s_Syubetu = "1";
      //         break;
      //      default:
      //         s_Text = s_HEADS_DATA[1048].TrimRight();  // SPANM
      //         if (s_Text == "1001") {
      //            s_Syubetu = "1";
      //         } else {
      //            s_Syubetu = "0";
      //         }
      //   }
      //*************************************
      } else {
         s_Syubetu = "0";
      }

      // ｱﾝｶｰﾎﾞﾙﾄ長ﾏｽﾀ 検索
      G_Log_Text = "";
      s_SQL = "";
      s_SQL = s_SQL + "SELECT * FROM HD_ANCHOR_SIZE";
      // 2012.06.08 ｱﾝｶｰﾎﾞﾙﾄ検索条件変更(RX-10-EP)
      //s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model) + "'";
      switch (s_HEADS_DATA[37].ToIntDef(0)) {      // フレームコード
         case 71: case 72: case 81: case 91: case 381: case 391:  // ＥＰシリーズ
            s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
            s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "E'";
            break;
         //2018.08.08 アンカー変更_S
         case 34: case 35: case 36: case 37: case 38:
         case 44: case 45: case 46: case 47: case 48:
         case 334: case 335: case 336: case 337: case 338:
         case 344: case 345: case 346: case 347: case 348:  // Ｚフレーム
              s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
              s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "Z'";
              break;
         //2018.08.08 アンカー変更_E
         default :
            s_SQL = s_SQL + "  WHERE PLATE_CODE  = '" + ZenToHan(G_KS_Syurui);
            s_SQL = s_SQL + "-" + ZenToHan(G_KS_Model) + "'";
            break;
      }
      // *****************************************
      s_SQL = s_SQL + "    AND ANCHOR_STAT = " + s_Syubetu + "";
      s_SQL = s_SQL + "    AND ABCHOR_TYPE = " + s_HEADS_DATA[1045].TrimRight() + ";";

      wkQuery->Close();
      // 2021.06.18 FireDACへの移行 MOD_S
      //wkQuery->DatabaseName = ODBC_DSN_DNO;
      wkQuery->ConnectionName = ODBC_DSN_DNO;
      // 2021.06.18 FireDACへの移行 MOD_E
      wkQuery->SQL->Clear();
      wkQuery->SQL->Add(s_SQL);
      wkQuery->Open();
      wkQuery->First();

      if(wkQuery->Eof){
         s_Text = "";
      } else {
         s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                + "×" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight() + "Ｌ "
                + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
      }

      //2003.12.08 長さ指定変更
      //if (s_HEADS_DATA[1045].TrimRight() != "02" && s_HEADS_DATA[1045].TrimRight() != "03" &&
      //    s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {  // SPANM
      if (s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {  // SPANM
      //***********************
         s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
      }

      //2003.12.24 ﾎｰﾙｲﾝ長さ指定変更
      if (s_HEADS_DATA[1045].TrimRight() == "02" ) {
         //2004.03.15 ﾎｰﾙｲﾝ長さ追加
         if (s_HEADS_DATA[1047].ToIntDef(0) == 60 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "60";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 70 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "70";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 80 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "80";
         //2004.04.14 ﾎｰﾙｲﾝ長さ追加
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 90 ) {   // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "90";
         //************************
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 100 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "10";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 120 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "12";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 130 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "13";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 150 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "15";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 190 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "19";
         } else if (s_HEADS_DATA[1047].ToIntDef(0) == 230 ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight()
                  + "23";
         } else if (s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {  // SPANM
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                  + "×" + FormatFloat("0", s_HEADS_DATA[1047].ToDouble()) + "Ｌ "
                  + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
         } else {
            s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight();
            if ( s_Text == "M12" ) {
                s_Text = s_Text + "×" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight()
                  + "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() + "80";
            } else {
                s_Text = s_Text + "×" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight()
                  + "Ｌ " + wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() + "13";
            }
         }
         //************************
      }
      //***********************

      //2015.11.19 ケミカルアンカー全面変更
      ////2006.02.06 ｹﾐｶﾙｱﾝｶｰのBOLT_TYPEの長さによる変更の追加
      //if ( s_HEADS_DATA[1045].TrimRight() == "03" ) {
      //   if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-20" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-20";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 180 ) {   // SPANM
      //         s_Text = "AP-20S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 250 ) {   // SPANM
      //         s_Text = "AP-20";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 300 ) {   // SPANM
      //         s_Text = "AP-20L";
      //      } else {
      //         s_Text = "";
      //      }
      //   } else if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-24" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-24";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 270 ) {   // SPANM
      //         s_Text = "AP-24S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 375 ) {   // SPANM
      //         s_Text = "AP-24";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 420 ) {   // SPANM
      //         s_Text = "AP-24L";
      //      } else {
      //         s_Text = "";
      //      }
      //   } else if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-30" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-30";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 310 ) {   // SPANM
      //         s_Text = "AP-30S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 430 ) {   // SPANM
      //         s_Text = "AP-30";
      //      } else {
      //         s_Text = "";
      //      }
      //   // 2012.07.03 AP-12追加
      //   } else if ( wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight() == "AP-12" ) {
      //      if ( s_HEADS_DATA[1047].ToIntDef(0) == 0 ) {   // SPANM
      //         s_Text = "AP-12";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 90 ) {    // SPANM
      //         s_Text = "AP-12S";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 130 ) {   // SPANM
      //         s_Text = "AP-12";
      //      } else if ( s_HEADS_DATA[1047].ToIntDef(0) <= 250 ) {   // SPANM
      //         s_Text = "AP-12L";
      //      } else {
      //         s_Text = "";
      //      }
      //   // ********************
      //   } else {
      //      s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
      //   }
      //   if ( s_HEADS_DATA[1047].TrimRight() != "0" && s_HEADS_DATA[1047].TrimRight() != "" ) {
      //      if ( s_Text == "AP-20L"
      //        && s_HEADS_DATA[1047].ToIntDef(0) > 290
      //        && s_HEADS_DATA[1047].ToIntDef(0) <= 295 ) {
      //           s_Text2 = "295";
      //      } else if ( s_Text == "AP-24"
      //        && s_HEADS_DATA[1047].ToIntDef(0) > 370
      //        && s_HEADS_DATA[1047].ToIntDef(0) <= 375 ) {
      //           s_Text2 = "375";
      //      } else {
      //           s_Text2 = FormatFloat("0",ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
      //      }
      //      s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
      //             + "×" + s_Text2 + "Ｌ "
      //             + s_Text;
      //   } else {
      //      s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
      //             + "×" + wkQuery->FieldByName("BOLT_LENGTH")->AsString.TrimRight() + "Ｌ "
      //             + s_Text;
      //   }
      //}
      ////****************************************************
      if ( s_HEADS_DATA[1045].TrimRight() == "03" ) {
         s_Text = wkQuery->FieldByName("BOLT_TYPE")->AsString.TrimRight();
         if (s_Text == "HP-12") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 71 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 250) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-20") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 141 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 350) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-24") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 371 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 375) {
               s_Text2 = "375";
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 450) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         } else if (s_Text == "HP-30") {
            if (s_HEADS_DATA[1047].ToIntDef(0) == 0 ||
                s_HEADS_DATA[1047].TrimRight() == "") {
               s_Text2 =
                  wkQuery->FieldByName("BOLT_LENGTH")
                  ->AsString.TrimRight();
            } else if (s_HEADS_DATA[1047].ToIntDef(0) >= 241 &&
                       s_HEADS_DATA[1047].ToIntDef(0) <= 500) {
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            } else {
               s_Text = "";
               s_Text2 =
                  FormatFloat("0",
                  ceil(s_HEADS_DATA[1047].ToDouble() / 10) * 10);
            }
         }

         s_Text = wkQuery->FieldByName("BOLT_SIZE")->AsString.TrimRight()
                + "×" + s_Text2 + "Ｌ "
                + s_Text;
      }
      // **********************************
      //2018.08.08 アンカー変更_S
      s_Text2 = wkQuery->FieldByName("NUM")->AsString.TrimRight();
      s_Text = s_Text + " ／ " + s_Text2 + "本";
      //2018.08.08 アンカー変更_E

      //2018.08.08 アンカー変更_S
      //Excel_Cell_DataSet(34, 8, s_Text);
      Excel_Cell_DataSet(34, 10, s_Text);
      //2018.08.08 アンカー変更_E
      G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ長さ          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //***************************
      //***  ｱﾝｶｰﾎﾞﾙﾄ　材質
      //***************************
      // 『C027』 02.09.20変更 N.UCHIDA

      s_Text = s_HEADS_DATA[1048].TrimRight();  // SPANM
      if ( s_Text != "" && s_Text != "0000") {
        s_Text = Search_HD_MATERIAL(s_Text);
      } else {
        i_RecNo = 1045;   // SPANC
        s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

        if (s_Text == "00" || s_Text == "01" || s_Text == "03" || s_Text == "05" || s_Text == "06" || s_Text == "07"){
           switch (s_HEADS_DATA[37].ToIntDef(0)) {
              case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
                 s_Text = "ＳＵＳ３０４";
                 break;
              default :
                 if (s_HEADS_DATA[1048].TrimRight() == "1001"){
                    s_Text = "ＳＵＳ３０４";
                 // 2016/09/14 材質コード変更
                 } else if (s_HEADS_DATA[1048].TrimRight() == "30C9"){
                    s_Text = "ＳＳ３０４";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "31C9"){
                    s_Text = "ＳＵＳ３０４";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "32C9"){
                    s_Text = "ＳＡ－１９３－Ｇｒ．Ｂ８";
                 // *************************
                 } else {
                    s_Text = "ＳＳ４００";
                 }
                 break;
           }
        } else if (s_Text == "02") {
           switch (s_HEADS_DATA[37].ToIntDef(0)) {
              case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
                 s_Text = "ＳＵＳ３０４";
                 break;
              default :
                 if (s_HEADS_DATA[1048].TrimRight() == "1001"){
                    s_Text = "ＳＵＳ３０４";
                 } else {
                    //2004.01.07 材質変更
                    //s_Text = "Ｃ．Ｓ．";
                    s_Text = "ＳＳ４００";
                    //*******************
                 }
                 break;
           }
        } else if (s_Text == "04") {
           //2004.01.06 材質変更
           //s_Text = "ＳＤ２４／ＳＳ４００";
           //2004.01.19 材質変更
           //s_Text = "ＳＤ２４";
           //2004.02.02 材質変更
           //s_Text = "ＳＲ２３５";
           s_Text = "ＳＤ３４５";
           //*******************
           //*******************
           //*******************
        } else if (s_Text == "09") {
           s_Text = "";
        } else {
           s_Text = "";
        }
      }

      // 2012.01.20 中国材質
      if ( s_ChinaM == "CHINA" ) {                                // 中国材質
          if ( s_Text == "ＳＳ４００" ) {
              s_Text = "Ｑ２３５Ｂ";
          }
      }
      // *******************

      Excel_Cell_DataSet(34, 15, s_Text);
      G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ材質          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      /*
      i_RecNo = 168;   // ｱﾝｶｰﾎﾞﾙﾄ材質
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(34, 15, s_Text);
      G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ材質          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
      */

      //***************************
      //***  ｱﾝｶｰﾎﾞﾙﾄ　仕上
      //***************************
      // ｱﾝｶｰﾎﾞﾙﾄ材質が｢ＳＵＳ３０４｣の場合出力しない
      // 『C028』
      if (s_Text != "ＳＵＳ３０４"){
         i_RecNo = 1049;   // ｱﾝｶｰﾎﾞﾙﾄ仕上
         i_Siage  =s_HEADS_DATA[i_RecNo].ToIntDef(0);
         if (i_Siage == 0 ) {
           //2004.01.07 仕上変更
           //s_Text = "標準";
           i_RecNo = 1045;   // SPANC
           s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
           if (s_Text == "02" || s_Text == "03"){
             s_Text = "ｸﾛﾒｰﾄ";
           } else {
             s_Text = "標準";
           }
           //*******************
         } else if ( i_Siage == 1 ) {
           s_Text = "Znﾒｯｷ";
         } else if ( i_Siage == 2 ) {
           s_Text = "ﾕﾆｸﾛ";
         } else {
           s_Text = "";
         }
      } else {
         s_Text = "";
      }
      Excel_Cell_DataSet(34, 23, s_Text);
      G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ仕上          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   }
   //***************************
   //***  指定項目欄
   //***************************
   for (i=0; i<=7; i++){
      s_Text = "";
      Excel_Cell_DataSet(35 + i, 6, s_Text);
   }

   //2011.03.11 指定項目欄変更
   for (i=0; i<=3; i++){
      s_Text = "";
      Excel_Cell_DataSet(39 + i, 13, s_Text);
   }
   //*************************

   i = 0;

   //2011.03.11 指定項目欄変更
   ////2003.11.19 指定項目追加
   //i_RecNo = 1335;   // 注記(ラチェットスパナ)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   ////***********************
   //
   //i_RecNo = 1302;   // 注記(禁油１)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1304;   // 注記(電解研磨１)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1306;   // 注記(ﾌﾟﾚｰﾄ枚数１)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1325;   // 注記(ﾉｽﾞﾙ１)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1326;   // 注記(ﾉｽﾞﾙ２)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1312;   // 注記(ﾎﾞﾙﾄ枚数１)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1318;   // 注記(ｶﾞｲﾄﾞﾊﾞｰ枚数１)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1329;   // 注記(相ﾌﾗﾝｼﾞ)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1330;   // 注記(ﾄﾞﾚﾝﾊﾟﾝ)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   //i_RecNo = 1331;   // 注記(客先仕様)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   ////2005.03.22 注記(客先仕様２)追加
   //i_RecNo = 1055;   // 注記(客先仕様２)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   ////*******************************
   //
   //i_RecNo = 1336;   // 注記(予備品)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //}
   //
   //i_RecNo = 1365;   // 注記(フレーム仕上)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   //
   ////2008.03.21 注記(ガスケット)追加
   //i_RecNo = 1054;   // 注記(ガスケット)
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text.Trim() != ""){
   //   Excel_Cell_DataSet(35 + i, 6, s_Text);
   //   i++;
   //}
   ////*******************************

   i_RecNo = 1302;   // 注記(禁油１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   }

   i_RecNo = 1325;   // 注記(ﾉｽﾞﾙ１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   }

   i_RecNo = 1326;   // 注記(ﾉｽﾞﾙ２)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   }

   i_RecNo = 1331;   // 注記(客先仕様)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   }

   i_RecNo = 1055;   // 注記(客先仕様２)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   }

   i_RecNo = 1054;   // 注記(ガスケット)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   }

   i_RecNo = 1312;   // 注記(ﾎﾞﾙﾄ枚数１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1318;   // 注記(ｶﾞｲﾄﾞﾊﾞｰ枚数１)
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != "" && s_Text2.Trim() != ""){
      s_Text = s_Text + "、" + s_Text2;
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   } else if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   } else if (s_Text2.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text2);
      i++;
   }

   i_RecNo = 1330;   // 注記(ﾄﾞﾚﾝﾊﾟﾝ)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      i++;
   }

   i_RecNo = 1335;   // 注記(ラチェットスパナ)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      if (i >= 8) {
         Excel_Cell_DataSet(31 + i, 13, s_Text);
      } else {
         Excel_Cell_DataSet(35 + i, 6, s_Text);
      }
      i++;
   }

   i_RecNo = 1304;   // 注記(電解研磨１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1306;   // 注記(ﾌﾟﾚｰﾄ枚数１)
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != "" && s_Text2.Trim() != ""){
      s_Text = s_Text + "、" + s_Text2;
      if (i >= 8) {
         Excel_Cell_DataSet(31 + i, 13, s_Text);
      } else {
         Excel_Cell_DataSet(35 + i, 6, s_Text);
      }
      i++;
   } else if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(35 + i, 6, s_Text);
      if (i >= 8) {
         Excel_Cell_DataSet(31 + i, 13, s_Text);
      } else {
         Excel_Cell_DataSet(35 + i, 6, s_Text);
      }
      i++;
   } else if (s_Text2.Trim() != ""){
      if (i >= 8) {
         Excel_Cell_DataSet(31 + i, 13, s_Text2);
      } else {
         Excel_Cell_DataSet(35 + i, 6, s_Text2);
      }
      i++;
   }

   i_RecNo = 1329;   // 注記(相ﾌﾗﾝｼﾞ)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      if (i >= 8) {
         Excel_Cell_DataSet(31 + i, 13, s_Text);
      } else {
         Excel_Cell_DataSet(35 + i, 6, s_Text);
      }
      i++;
   }

   i_RecNo = 1336;   // 注記(予備品)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      if (i >= 8) {
         Excel_Cell_DataSet(31 + i, 13, s_Text);
      } else {
         Excel_Cell_DataSet(35 + i, 6, s_Text);
      }
      i++;
   }
   //*************************

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｄ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  装置番号
   //***************************
   // 『D001』
   i_RecNo = 28;    // 工事番号
   s_Text = ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   Excel_Cell_DataSet(43, 9, s_Text);
   G_Log_Text = "工事番号              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器番号
   //***************************
   // 『D002』
   i_RecNo = 27;  // 機器番号
   //2004.10.04
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   int w_count = s_Text.AnsiPos("～");

   if (w_count){
      s_Text = ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "～" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
   }else{
      s_Text = ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   }
   //**********
   Excel_Cell_DataSet(43, 18, s_Text);
   G_Log_Text = "機器番号              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側設計圧力
   //***************************
   // 『D003』
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.06.09 負圧追加
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if (s_Text.ToDouble() < 900){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "大気開放";
      }
   } else {
      G_ErrLog_Text = "B側設計圧力     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(45, 6, s_Text);
   G_Log_Text = "B側設計圧力           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側設計温度
   //***************************
   // 『D004』
   i_RecNo = 829;    // B側設計温度
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "B側設計温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }
   //2004.07.15 国交省銘板追加
   if (s_HEADS_DATA[268].TrimRight() == "1" && s_HEADS_DATA[269].TrimRight() == "11") { // 国交省銘板Ｈ１１
      i_RecNo = 854;    // B側出口温度
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "B側出口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text2 + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "***.**/" + s_Text;
      }
      i_RecNo = 852;    // B側入口温度
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "B側入口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text2 + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "***.**/" + s_Text;
      }
   }
   //*************************

   Excel_Cell_DataSet(46, 6, s_Text);
   G_Log_Text = "B側設計温度           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側試験圧力 空圧
   //***************************
   // 『D005』
   i_RecNo = 826;    // B側試験圧力
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "B側試験圧力 空圧   RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(47, 6, s_Text);
   G_Log_Text = "B試験圧力 空圧        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側試験圧力 水圧
   //***************************
   // 『D006』
   i_RecNo = 827;    // B側試験水圧
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "B側試験圧力 水圧   RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(48, 6, s_Text);
   G_Log_Text = "B試験圧力 水圧        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.07.15 国交省銘板追加
   //***************************
   //***  B側流量
   //***************************
   if (s_HEADS_DATA[268].TrimRight() == "1") { // 国交省銘板
      s_Text = "流量";

      Excel_Cell_DataSet(49, 4, s_Text);

      i_RecNo = 107;    // 交換熱量
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("########0",s_Text.ToDouble()) + " kW/";
      } else {
         G_ErrLog_Text = "交換熱量        RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "*********kW/";
      }
      i_RecNo = 856;    // B側流量
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         //2004.08.16 切り上げ
         if (s_HEADS_DATA[846].TrimRight() == "1") { // Ｂ側スチーム
            s_Text = s_Text + FormatFloat("#####0.00",ceil(s_Text2.ToDouble() / 60 * 100) / 100);
         } else {                                    // Ｂ側液
            s_Text = s_Text + FormatFloat("#####0.00",ceil(s_Text2.ToDouble() * 1000 / 60 * 100) / 100);
         }
         //if (s_HEADS_DATA[846].TrimRight() == "1") { // Ｂ側スチーム
         //   s_Text = s_Text + FormatFloat("#####0.00",s_Text2.ToDouble() / 60);
         //} else {                                    // Ｂ側液
         //   s_Text = s_Text + FormatFloat("#####0.00",s_Text2.ToDouble() * 1000 / 60);
         //}
         //*******************
      } else {
         G_ErrLog_Text = "B側流量         RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text2 + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = s_Text + "******.**";
      }

   Excel_Cell_DataSet(49, 6, s_Text);
   G_Log_Text = "B側流量               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[846].TrimRight() == "1") { // Ｂ側スチーム
      s_Text = "kg/min";
   } else {                                    // Ｂ側液
      s_Text = "lit/min";
   }

   Excel_Cell_DataSet(49, 11, s_Text);
   }
   //*************************

   //***************************
   //***  B側内容量
   //***************************
   // 『D007』
   i_RecNo = 1282;    // Bﾎｰﾙﾄﾞ量
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "B側内容量       RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(50, 6, s_Text);
   G_Log_Text = "B側内容量             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  伝熱面積
   //***************************
   // 『D008』
   i_RecNo = 1067;    // 伝熱面積
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
//      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      s_Text = s_Text.TrimRight();
   } else {
      G_ErrLog_Text = "伝熱面積        RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(51, 6, s_Text);
   G_Log_Text = "伝熱面積              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側設計圧力
   //***************************
   // 『D009』
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.06.09 負圧追加
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if (s_Text.ToDouble() < 900){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "大気開放";
      }
   } else {
      G_ErrLog_Text = "A側設計圧力     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(45, 14, s_Text);
   G_Log_Text = "A側設計圧力           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側設計温度
   //***************************
   // 『D010』
   i_RecNo = 834;    // A側設計温度
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "A側設計温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }
   //2004.07.15 国交省銘板追加
   if (s_HEADS_DATA[268].TrimRight() == "1" && s_HEADS_DATA[269].TrimRight() == "11") { // 国交省銘板Ｈ１１
      i_RecNo = 855;    // A側出口温度
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "A側出口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text2 + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "***.**/" + s_Text;
      }
      i_RecNo = 853;    // A側入口温度
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text2.c_str(),1) == 0){
         s_Text = FormatFloat("##0.00",s_Text2.ToDouble()) + "/" + s_Text;
      } else {
         G_ErrLog_Text = "A側入口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text2 + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = "***.**/" + s_Text;
      }
   }
   //*************************

   Excel_Cell_DataSet(46, 14, s_Text);
   G_Log_Text = "A側設計温度           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側試験圧力 空圧
   //***************************
   // 『D011』
   i_RecNo = 831;    // A側試験圧力
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "A側試験圧力 空圧   RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(47, 14, s_Text);
   G_Log_Text = "A試験圧力 空圧        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側試験圧力 水圧
   //***************************
   // 『D012』
   i_RecNo = 832;    // A側試験水圧
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() != 0){
         s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
      } else {
         s_Text = "-----";
      }
   } else {
      G_ErrLog_Text = "A側試験圧力 水圧   RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(48, 14, s_Text);
   G_Log_Text = "A試験圧力 水圧        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.07.15 国交省銘板追加
   //***************************
   //***  A側流量
   //***************************
   if (s_HEADS_DATA[268].TrimRight() == "1") { // 国交省銘板
      i_RecNo = 857;    // A側流量
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         //2004.08.16 切り上げ
         if (s_HEADS_DATA[847].TrimRight() == "1") { // Ａ側スチーム
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() / 60 * 100) / 100);
         } else {                                    // Ａ側液
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
         }
         //if (s_HEADS_DATA[847].TrimRight() == "1") { // Ａ側スチーム
         //   s_Text = FormatFloat("#####0.00",s_Text.ToDouble() / 60);
         //} else {                                    // Ａ側液
         //   s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
         //}
         //*******************
      } else {
         G_ErrLog_Text = "A側流量         RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text2 + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

         s_Text = s_Text + "******.**";
      }

   Excel_Cell_DataSet(49, 14, s_Text);
   G_Log_Text = "A側流量               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[847].TrimRight() == "1") { // Ａ側スチーム
      s_Text = "kg/min";
   } else {                                    // Ａ側液
      s_Text = "lit/min";
   }

   Excel_Cell_DataSet(49, 24, s_Text);
   }
   //*************************

   //***************************
   //***  A側内容量
   //***************************
   // 『D013』
   i_RecNo = 1283;    // Aﾎｰﾙﾄﾞ量
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "A側内容量       RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(50, 14, s_Text);
   G_Log_Text = "A側内容量             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器質量
   //***************************
   // 『D014』
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(51, 17, s_Text);
   G_Log_Text = "機器質量              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  適用規格
   //************************
   // 『D015』
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //G_Log_Text = "適用規格              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』により、";
   //2013.11.08 適用規格(銘板)表記訂正
   //s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,true, s_HEADS_DATA[1].TrimRight());
   if (s_HEADS_DATA[1].TrimRight()==0) {
       // 和文
       s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,false, s_HEADS_DATA[1].TrimRight());
   } else {
       // 英文
       s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),false,false, s_HEADS_DATA[1].TrimRight());
   }
   //*********************************
   // 2011.05.30 常熟銘板
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   //2013.11.08 適用規格(銘板)表記訂正
   //if ( ( s_Text2.ToIntDef(0) == 0 ) && ( s_HEADS_DATA[1].TrimRight() == "0" ) ) {
   if ( s_Text2.ToIntDef(0) == 0 ) {
   //*********************************
       // 2012.01.20 常熟フラグ
       //if ( s_HEADS_DATA[5].TrimRight() == "42842" ) {
       //    //常熟向け(42842)(和文)銘板
       //    s_Text = "常熟銘板(和文)";
       //} else if ( s_HEADS_DATA[5].TrimRight() == "43935" ) {
       //    //常熟向け(43935)(和文)銘板
       //    s_Text = "常熟銘板(和文)";
       //} else if ( s_HEADS_DATA[401].TrimRight() == "42842" ) {
       //    //常熟向け(42842)(和文)銘板
       //    s_Text = "常熟銘板(和文)";
       //} else if ( s_HEADS_DATA[401].TrimRight() == "43935" ) {
       //    //常熟向け(43935)(和文)銘板
       //    s_Text = "常熟銘板(和文)";
       //}
       if ( s_HEADS_DATA[1107].TrimRight() == "CHINA" ) {
          //常熟向け(中文)銘板
          s_Text = "常熟銘板(中文)";
       }
       // *********************
   }
   // *******************
   //2004.07.15 国交省銘版追加
   i_RecNo = 1372;
   s_Text = s_Text + "  " + s_HEADS_DATA[i_RecNo].TrimRight();
   //*************************
   Excel_Cell_DataSet(52, 6, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付寸法　Max
   //***************************
   // 『D016』
//   i_RecNo = 1014;    // RMAX
   i_RecNo = 1089;    // BMAX に変更　2002.11.07 Miya
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "RMAX            RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(53, 9, s_Text);
   G_Log_Text = "締付寸法 Max          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付寸法　Min
   //***************************
   // 『D017』
   i_RecNo = 1015;    // RMAX
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0",s_Text.ToDouble());
   } else {
      G_ErrLog_Text = "RMIN            RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      s_Text = "";
   }

   Excel_Cell_DataSet(53, 15, s_Text);
   G_Log_Text = "締付寸法 Min          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2016/09/14 ステッカー追加
   //***************************
   //***  ステッカー
   //***************************
   // 『D018』
   i_RecNo = 1122;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "国名コード            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より、";
   s_Text2 = "";

   if( s_Text.Length() == 5 ) {

      for (i = 0; i < G_STACKER_RecCnt; i++) {
         if (G_STACKER[i][0] == s_Text) {
            s_Text2 = G_STACKER[i][1];
         }
      }

      if( s_Text2 == "" ) {
         s_Text2 = "英文";
      }

   } else {
      s_Text2 = "英文";
   }

   s_Text = s_Text2;

   if (s_HEADS_DATA[1].TrimRight()==0) {
      // 和文
      Excel_Cell_DataSet(54, 18, "");
      G_Log_Text = "ステッカー            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』を消去。";
   Write_Log(G_Log_Text);
   } else {
      // 英文
      Excel_Cell_DataSet(54, 24, s_Text);
      G_Log_Text = "ステッカー            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   }
   // **************************

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                        旧 製作指示書　比較処理                      ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //2004.12.15 REV00の場合の訂正印変更
   ////2004.11.16 出力フォルダ及び訂正印変更
   ////// 直近の製作指示書を作成している、ﾘﾋﾞｼﾞｮﾝ番号の取得
   ////d_RevNo = Sheet_Exist_Check(G_xls_Output_Dir);
   ////if (d_RevNo.Trim() != ""){
   ////   // 前回ﾌｧｲﾙﾊﾟｽ
   ////   s_Text = G_xls_Output_Dir + d_RevNo + "\\MAN" + G_SAKUBAN + ".xls";
   ////   Sheet_Compare_Syori(s_Text);
   ////}
   //// 直近の製作指示書を作成している、ﾘﾋﾞｼﾞｮﾝ番号の取得
   //d_RevNo=Sheet_Exist_Check(G_xls_Output_Dir_old.SubString(1,G_xls_Output_Dir_old.LastDelimiter("\\")));
   //if ((d_RevNo.Trim() != "-01") && (d_RevNo.Trim() != "")){
   //   // 前回ﾌｧｲﾙﾊﾟｽ
   //   s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + ".xls";
   //   Write_Log("!?!" +  s_Text);
   //   Sheet_Compare_Syori(s_Text);
   //}
   //else if (d_RevNo.Trim() == "-01"){
   //   s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + ".xls";
   //   Sheet_Compare_Syori(s_Text);
   //}
   ////*************************************
   if (s_HEADS_DATA[319].TrimRight() == "0" || s_HEADS_DATA[319].TrimRight() == "00") {
   } else {
	   d_RevNo=Sheet_Exist_Check(G_xls_Output_Dir_old.SubString(1,G_xls_Output_Dir_old.LastDelimiter("\\")), pcHeadsData);
       if ((d_RevNo.Trim() != "-01") && (d_RevNo.Trim() != "")){
          // 前回ﾌｧｲﾙﾊﾟｽ
//Mod_Str 2019/02/19 Ri
		  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
		  //2023/4/21 製造番号桁数追加対応 Mod
		  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + "OLD.xls";
		  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + "OLD.xls";
          // 2019.06.10 旧製作指示書名称変更_S
          if ( G_OLD_FLG == 1 || G_OLD_FLG == 2 ) {
              //旧製作指示書作成
			  //2023/4/21 製造番号桁数追加対応 Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
		  }
          // 2019.06.10 旧製作指示書名称変更_E
//Mod_End 2019/02/19 Ri
          if (!FileExists(s_Text.c_str()))
          {
//Mod_Str 2019/02/19 Ri
              //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
			  //2023/4/21 製造番号桁数追加対応 Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + "OLD.xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "OLD.xls";
              // 2019.06.10 旧製作指示書名称変更_S
              if ( G_OLD_FLG == 1 || G_OLD_FLG == 2 ) {
                  //旧製作指示書作成
				  //2023/4/21 製造番号桁数追加対応 Mod
				  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
				  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
              }
              // 2019.06.10 旧製作指示書名称変更_E
//Mod_End 2019/02/19 Ri
          }
          Write_Log("!?!" +  s_Text);
          Sheet_Compare_Syori(s_Text);
       }
       else if (d_RevNo.Trim() == "-01"){
          d_RevNo = FormatFloat("00", G_HEADS_DATA_Rev - 1);
//Mod_Str 2019/02/19 Ri
          //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
		  //2023/4/21 製造番号桁数追加対応 Mod
		  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + "OLD.xls";
		  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + "OLD.xls";
          // 2019.06.10 旧製作指示書名称変更_S
          if ( G_OLD_FLG == 1 || G_OLD_FLG == 2 ) {
              //旧製作指示書作成
			  //2023/4/21 製造番号桁数追加対応 Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
          }
          // 2019.06.10 旧製作指示書名称変更_E
//Mod_End 2019/02/19 Ri
          bFlag = false;
          if (!FileExists(s_Text.c_str())){
//Mod_Str 2019/02/19 Ri
              //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
			  //2023/4/21 製造番号桁数追加対応 Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + "OLD.xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "OLD.xls";
              // 2019.06.10 旧製作指示書名称変更_S
              if ( G_OLD_FLG == 1 || G_OLD_FLG == 2 ) {
                  //旧製作指示書作成
				  //2023/4/21 製造番号桁数追加対応 Mod
				  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
				  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
              }
              // 2019.06.10 旧製作指示書名称変更_E
//Mod_End 2019/02/19 Ri
              if (FileExists(s_Text.c_str())){
                  bFlag = true;
              }
          } else {
              bFlag = true;
          }

          bFind = false;
          if (!bFlag){
              while(d_RevNo.ToIntDef(0) >= 0 && bFind == false) {
                  // ﾌｧｲﾙ存在ﾁｪｯｸ
//Mod_Str 2019/02/19 Ri
                  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
				  //2023/4/21 製造番号桁数追加対応 Mod
				  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + "OLD.xls";
				  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + "OLD.xls";
                  // 2019.06.10 旧製作指示書名称変更_S
                  if ( G_OLD_FLG == 1 || G_OLD_FLG == 2 ) {
                      //旧製作指示書作成
					  //2023/4/21 製造番号桁数追加対応 Mod
					  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
					  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
                  }
                  // 2019.06.10 旧製作指示書名称変更_E
//Mod_End 2019/02/19 Ri
                  if (!FileExists(s_Text.c_str())){
//Mod_Str 2019/02/19 Ri
                      //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
					  //2023/4/21 製造番号桁数追加対応 Mod
					  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + "OLD.xls";
					  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "OLD.xls";
                      // 2019.06.10 旧製作指示書名称変更_S
                      if ( G_OLD_FLG == 1 || G_OLD_FLG == 2 ) {
                          //旧製作指示書作成
						  //2023/4/21 製造番号桁数追加対応 Mod
						  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
						  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
                      }
                      // 2019.06.10 旧製作指示書名称変更_E
//Mod_End 2019/02/19 Ri
                      if (FileExists(s_Text.c_str())){
                          bFlag = true;
                      }
                  } else {
                      bFlag = true;
                  }

                  if (bFlag){
                      G_Log_Text = "s_Text?『 " + s_Text + " 』";
                      Write_Log(G_Log_Text);
                      bFind = true;
                  } else {
                      d_RevNo = FormatFloat("00",d_RevNo.ToIntDef(0) - 1);
                  }
              }
          } else {
              G_Log_Text = "s_Text?『 " + s_Text + " 』";
              Write_Log(G_Log_Text);
              bFind = true;
          }
          if (bFind) {
              Sheet_Compare_Syori(s_Text);
          }
       }
   }
   //**********************************

   //Windows 2008Server 移植対応
   wkQuery->Close();
   delete wkQuery;
   //******************************
   //***  Excel Book Close
   //******************************
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
   G_Log_Text = "----------    『製作指示書』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   //Windows 2008Server 移植対応
   //delete wkQuery;


   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 最新製作指示書、直近製作指示書比較処理
// 概  要      ：
// 引  数      ：
// 戻り値      ：
// 備  考      ：
//---------------------------------------------------------------------------
void __fastcall Sheet_Compare_Syori(AnsiString Excel_OldPass){
   AnsiString s_MakeExcel_Pass;

   s_MakeExcel_Pass = G_xls_Output_Dir + "HIKAKU.xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ

   //******************************
   //***  テンプレート　コピー
   //******************************
   if (!Template_Copy(Excel_OldPass,s_MakeExcel_Pass)){
      // ｴﾗｰﾛｸﾞ追加
      G_ErrLog_Text = "ファイルコピーを失敗しました。『" + Excel_OldPass +
                      "』→『" + s_MakeExcel_Pass + "』";
      Write_Error_Log(G_ErrLog_Text);

      return;
   } else {
      G_Log_Text = "ファイルコピー成功。 『" + Excel_OldPass +
                      "』→『" + s_MakeExcel_Pass + "』";
      Write_Log(G_Log_Text);
   }

   // ﾌｧｲﾙｵｰﾌﾟﾝ
   //exWorkbook_old  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
   //Windows 2008Server 移植対応
   exWorkbook_old  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
   // ｼｰﾄｺﾚｸｼｮﾝ取得
   //exWorksheets_old = exWorkbook_old.Exec(PropertyGet("WorkSheets"));
   //Windows 2008Server 移植対応
   exWorksheets_old = exWorkbook_old.OlePropertyGet("WorkSheets");
   // ｼｰﾄの指定
   //exWorksheet_old = exWorksheets_old.Exec(PropertyGet("Item") << EXCEL_SHEETNAME);
   //Windows 2008Server 移植対応
   exWorksheet_old = exWorksheets_old.OlePropertyGet("item",OleVariant(EXCEL_SHEETNAME));
   //***************************************************************************
   //***                                                                     ***
   //***                       製作指示書　比較処理                          ***
   //***                                                                     ***
   //***************************************************************************

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ａ欄　　　比較                             ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //*********************
   //***  型　式
   //*********************
   // 『A008』
   if(!Sheet_Cell_Compare(6,5)){
      Excel_Cell_DataSet(6, 19, "*");
   }

   //***************************
   //***  納　期
   //***************************
   // 『A009』
   if(!Sheet_Cell_Compare(5,23)){
      Excel_Cell_DataSet(5, 28, "*");
   }

   //*******************
   //***  台  数
   //*******************
   // 『A010』
   if(!Sheet_Cell_Compare(6, 23)){
      Excel_Cell_DataSet(6, 28, "*");
   }

   //***************************
   //***  注 文 先
   //***************************
   // 『A011』
   if(!Sheet_Cell_Compare(7, 5)){
      Excel_Cell_DataSet(7, 28, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｂ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //*******************************************
   //***  B側ﾌﾟﾚｰﾄ編成 1 ＆ B側ﾌﾟﾚｰﾄ編成 2
   //*******************************************
   // 『B001』，『B002』
   if(!Sheet_Cell_Compare(9, 7) || !Sheet_Cell_Compare(9, 10)){
      Excel_Cell_DataSet(9, 13, "*");
   }

   //*******************************************
   //***  A側ﾌﾟﾚｰﾄ編成 1 ＆ A側ﾌﾟﾚｰﾄ編成 2
   //*******************************************
   // 『B003』，『B004』
   if(!Sheet_Cell_Compare(9, 15) || !Sheet_Cell_Compare(9, 20)){
      Excel_Cell_DataSet(9, 26, "*");
   }

   //***************************
   //***  ﾌﾟﾚｰﾄ材質
   //***************************
   // 『B005』
   if(!Sheet_Cell_Compare(10, 7)){
      Excel_Cell_DataSet(10, 26, "*");
   }

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B006』
   if(!Sheet_Cell_Compare(11, 7)){
      Excel_Cell_DataSet(11, 13, "*");
   }

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B007』
   if(!Sheet_Cell_Compare(11, 15)){
      Excel_Cell_DataSet(11, 26, "*");
   }

   //2011.03.11 工程写真要注記追加
   //***************************
   //***  工程写真要
   //***************************
   if(!Sheet_Cell_Compare(12, 18)){
      Excel_Cell_DataSet(12, 26, "*");
   }
   //*****************************

   //***********************************
   //***  B側ﾉｽﾞﾙ材質 ＆ B側ﾉｽﾞﾙ色
   //***********************************
   // 『B008』,『B009』
   if(!Sheet_Cell_Compare(13, 7) || !Sheet_Cell_Compare(13, 12)){
      Excel_Cell_DataSet(13, 13, "*");
   }

   //***********************************
   //***  A側ﾉｽﾞﾙ材質 ＆ A側ﾉｽﾞﾙ色
   //***********************************
   // 『B010』,『B011』
   if(!Sheet_Cell_Compare(13, 15) || !Sheet_Cell_Compare(13, 25)){
      Excel_Cell_DataSet(13, 26, "*");
   }

   //***************************
   //***  S1継手・口径
   //***************************
   // 『B012』
   if(!Sheet_Cell_Compare(14, 8)){
      Excel_Cell_DataSet(14, 13, "*");
   }

   //***************************
   //***  S2継手・口径
   //***************************
   // 『B013』
   if(!Sheet_Cell_Compare(14, 16)){
      Excel_Cell_DataSet(14, 26, "*");
   }

   //***************************
   //***  S1温度計
   //***  B側S1温度計座材質
   //***************************
   // 『B014』,『B015』
   if(!Sheet_Cell_Compare(15, 7) || !Sheet_Cell_Compare(15, 11)){
      Excel_Cell_DataSet(15, 13, "*");
   }

   //***************************
   //***  S2温度計
   //***  A側S2温度計座材質
   //***************************
   // 『B016』,『B017』
   if(!Sheet_Cell_Compare(15, 15) || !Sheet_Cell_Compare(15, 24)){
      Excel_Cell_DataSet(15, 26, "*");
   }

   //***************************
   //***  S1圧力計
   //***************************
   // 『B018』
   if(!Sheet_Cell_Compare(16, 7)){
      Excel_Cell_DataSet(16, 13, "*");
   }

   //***************************
   //***  S2圧力計
   //***************************
   // 『B019』
   if(!Sheet_Cell_Compare(16, 15)){
      Excel_Cell_DataSet(16, 26, "*");
   }

   //***************************
   //***  S1ｴｱ抜継手名和
   //***************************
   // 『B020』
   if(!Sheet_Cell_Compare(17, 7)){
      Excel_Cell_DataSet(17, 13, "*");
   }

   //***************************
   //***  S2ｴｱ抜継手名和
   //***************************
   // 『B021』
   if(!Sheet_Cell_Compare(17, 15)){
      Excel_Cell_DataSet(17, 26, "*");
   }

   //***************************
   //***  S3継手・口径
   //***************************
   // 『B022』
   if(!Sheet_Cell_Compare(18, 8)){
      Excel_Cell_DataSet(18, 13, "*");
   }

   //***************************
   //***  S4継手・口径
   //***************************
   // 『B023』
   if(!Sheet_Cell_Compare(18, 16)){
      Excel_Cell_DataSet(18, 26, "*");
   }

   //***************************
   //***  S3温度計
   //***  S3ｽﾁｰﾑ区分
   //***************************
   // 『B024』
   // 『B025』
   if(!Sheet_Cell_Compare(19, 7) || !Sheet_Cell_Compare(19, 11)){
      Excel_Cell_DataSet(19, 13, "*");
   }

   //***************************
   //***  S4温度計
   //***  S4ｽﾁｰﾑ区分
   //***************************
   // 『B026』
   // 『B027』
   if(!Sheet_Cell_Compare(19, 15) || !Sheet_Cell_Compare(19, 24)){
      Excel_Cell_DataSet(19, 26, "*");
   }

   //***************************
   //***  S3圧力計
   //***************************
   // 『B028』
   if(!Sheet_Cell_Compare(20, 7)){
      Excel_Cell_DataSet(20, 13, "*");
   }

   //***************************
   //***  S4圧力計
   //***************************
   // 『B029』
   if(!Sheet_Cell_Compare(20, 15)){
      Excel_Cell_DataSet(20, 26, "*");
   }

   //***************************
   //***  S3液体継手名和
   //***************************
   // 『B030』
   if(!Sheet_Cell_Compare(21, 7)){
      Excel_Cell_DataSet(21, 13, "*");
   }

   //***************************
   //***  S4液体継手名和
   //***************************
   // 『B031』
   if(!Sheet_Cell_Compare(21, 15)){
      Excel_Cell_DataSet(21, 26, "*");
   }

   //***************************
   //***  B側端管材質
   //***************************
   // 『B032』
   if(!Sheet_Cell_Compare(22, 7)){
      Excel_Cell_DataSet(22, 13, "*");
   }

   //***************************
   //***  A側端管材質
   //***************************
   // 『B033』
   if(!Sheet_Cell_Compare(22, 15)){
      Excel_Cell_DataSet(22, 26, "*");
   }

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B034』
   if(!Sheet_Cell_Compare(23, 7)){
      Excel_Cell_DataSet(23, 13, "*");
   }

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B035』
   if(!Sheet_Cell_Compare(23, 15)){
      Excel_Cell_DataSet(23, 26, "*");
   }

   //***************************
   //***  B側ﾌﾗﾝｼﾞ材質
   //***************************
   // 『B036』
   if(!Sheet_Cell_Compare(24, 7)){
      Excel_Cell_DataSet(24, 13, "*");
   }

   //***************************
   //***  A側ﾌﾗﾝｼﾞ材質
   //***************************
   // 『B037』
   if(!Sheet_Cell_Compare(24, 15)){
      Excel_Cell_DataSet(24, 26, "*");
   }

   //***************************
   //***  B側仕上名称
   //***************************
   // 『B039』
   if(!Sheet_Cell_Compare(25, 7)){
      Excel_Cell_DataSet(25, 13, "*");
   }

   //***************************
   //***  A側仕上名称
   //***************************
   // 『B040』
   if(!Sheet_Cell_Compare(25, 15)){
      Excel_Cell_DataSet(25, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｃ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //**********************************************************************
   //***  締付ﾎﾞﾙﾄ材質, 締付ﾎﾞﾙﾄ 径, 締付ﾎﾞﾙﾄ 本数L, 締付ﾎﾞﾙﾄ ﾎﾞﾙﾄ仕上
   //***  締付ﾎﾞﾙﾄ BLS, 締付ﾎﾞﾙﾄ 本数S
   //**********************************************************************
   // 『C001』,『C002』,『C003』,『C004』,『C005』,『C006』
   if(!Sheet_Cell_Compare(26, 9) || !Sheet_Cell_Compare(26, 12) || !Sheet_Cell_Compare(26, 24) ||
      !Sheet_Cell_Compare(27, 7) || !Sheet_Cell_Compare(27, 12) || !Sheet_Cell_Compare(27, 24)){
      Excel_Cell_DataSet(26, 26, "*");
   }

   //*******************************
   //***  ﾌﾚｰﾑ材質 材質,厚さ,幅
   //*******************************
   // 『C007』,『C008』,『C009』
   if(!Sheet_Cell_Compare(28, 9) || !Sheet_Cell_Compare(28, 15) || !Sheet_Cell_Compare(28, 22)){
      Excel_Cell_DataSet(28, 26, "*");
   }

   //***************************
   //***  上部LU1,上部LU2
   //***************************
   // 『C010』,『C011』,『C012』
   if(!Sheet_Cell_Compare(30, 10) || !Sheet_Cell_Compare(30, 12) || !Sheet_Cell_Compare(30, 16)){
      Excel_Cell_DataSet(30, 26, "*");
   }

   //**********************************
   //***  下部LU1, 下部LU2, Ｃ, Ｐ
   //**********************************
   // 『C013』,『C014』,『C015』,『C016』
   if(!Sheet_Cell_Compare(31, 10) || !Sheet_Cell_Compare(31, 12) || !Sheet_Cell_Compare(31, 18)
                                  || !Sheet_Cell_Compare(31, 24)){
      Excel_Cell_DataSet(31, 26, "*");
   }

   //****************************
   //***  塗装色
   //****************************
   // 『C017』
   if(!Sheet_Cell_Compare(32, 6)){
      Excel_Cell_DataSet(32, 26, "*");
   }


   //****************************
   //***  アンカーボルト
   //****************************
   if(!Sheet_Cell_Compare(33, 8)){      //種類
      Excel_Cell_DataSet(33, 26, "*");
   }
   if(!Sheet_Cell_Compare(34, 8)){      //長さ
      Excel_Cell_DataSet(34, 26, "*");
   }
   if(!Sheet_Cell_Compare(34, 15)){     //材質
      Excel_Cell_DataSet(34, 26, "*");
   }
   if(!Sheet_Cell_Compare(34, 23)){     //仕上
      Excel_Cell_DataSet(34, 26, "*");
   }

   //****************************
   //***  指定項目
   //****************************
   if(!Sheet_Cell_Compare(35, 6)){
      Excel_Cell_DataSet(35, 26, "*");
   }

   if(!Sheet_Cell_Compare(36, 6)){
      Excel_Cell_DataSet(36, 26, "*");
   }
   if(!Sheet_Cell_Compare(36, 1)){
      Excel_Cell_DataSet(36, 26, "*");
   }

   if(!Sheet_Cell_Compare(37, 6)){
      Excel_Cell_DataSet(37, 26, "*");
   }
   if(!Sheet_Cell_Compare(37, 1)){
      Excel_Cell_DataSet(37, 26, "*");
   }
   if(!Sheet_Cell_Compare(37, 3)){
      Excel_Cell_DataSet(37, 26, "*");
   }

   if(!Sheet_Cell_Compare(38, 6)){
      Excel_Cell_DataSet(38, 26, "*");
   }
   if(!Sheet_Cell_Compare(38, 1)){
      Excel_Cell_DataSet(38, 26, "*");
   }
   if(!Sheet_Cell_Compare(38, 3)){
      Excel_Cell_DataSet(38, 26, "*");
   }

   if(!Sheet_Cell_Compare(39, 6)){
      Excel_Cell_DataSet(39, 26, "*");
   }
   if(!Sheet_Cell_Compare(39, 1)){
      Excel_Cell_DataSet(39, 26, "*");
   }
   if(!Sheet_Cell_Compare(39, 3)){
      Excel_Cell_DataSet(39, 26, "*");
   }

   if(!Sheet_Cell_Compare(40, 6)){
      Excel_Cell_DataSet(40, 26, "*");
   }

   if(!Sheet_Cell_Compare(41, 6)){
      Excel_Cell_DataSet(41, 26, "*");
   }

   if(!Sheet_Cell_Compare(42, 6)){
      Excel_Cell_DataSet(42, 26, "*");
   }


   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｄ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  装置番号
   //***************************
   // 『D001』
   if(!Sheet_Cell_Compare(43, 9)){
      Excel_Cell_DataSet(43, 13, "*");
   }

   //***************************
   //***  機器番号
   //***************************
   // 『D002』
   if(!Sheet_Cell_Compare(43, 18)){
      Excel_Cell_DataSet(43, 26, "*");
   }

   //***************************
   //***  B側設計圧力
   //***************************
   // 『D003』
   if(!Sheet_Cell_Compare(45, 6)){
      Excel_Cell_DataSet(45, 13, "*");
   }

   //***************************
   //***  B側設計温度
   //***************************
   // 『D004』
   if(!Sheet_Cell_Compare(46, 6)){
      Excel_Cell_DataSet(46, 13, "*");
   }

   //***************************
   //***  B側試験圧力 空圧
   //***************************
   // 『D005』
   if(!Sheet_Cell_Compare(47, 6)){
      Excel_Cell_DataSet(47, 13, "*");
   }

   //***************************
   //***  B側試験圧力 水圧
   //***************************
   // 『D006』
   if(!Sheet_Cell_Compare(48, 6)){
      Excel_Cell_DataSet(48, 13, "*");
   }

   //2004.09.24 B側流量追加
   //***************************
   //***  B側流量
   //***************************
   if(!Sheet_Cell_Compare(49, 6)){
      Excel_Cell_DataSet(49, 13, "*");
   }
   //**********************

   //***************************
   //***  B側内容量
   //***************************
   // 『D007』
   if(!Sheet_Cell_Compare(50, 6)){
      Excel_Cell_DataSet(50, 13, "*");
   }

   //***************************
   //***  伝熱面積
   //***************************
   // 『D008』
   if(!Sheet_Cell_Compare(51, 6)){
      Excel_Cell_DataSet(51, 13, "*");
   }

   //***************************
   //***  A側設計圧力
   //***************************
   // 『D009』
   if(!Sheet_Cell_Compare(45, 14)){
      Excel_Cell_DataSet(45, 26, "*");
   }

   //***************************
   //***  A側設計温度
   //***************************
   // 『D010』
   if(!Sheet_Cell_Compare(46, 14)){
      Excel_Cell_DataSet(46, 26, "*");
   }

   //***************************
   //***  A側試験圧力 空圧
   //***************************
   // 『D011』
   if(!Sheet_Cell_Compare(47, 14)){
      Excel_Cell_DataSet(47, 26, "*");
   }

   //***************************
   //***  A側試験圧力 水圧
   //***************************
   // 『D012』
   if(!Sheet_Cell_Compare(48, 14)){
      Excel_Cell_DataSet(48, 26, "*");
   }

   //2004.09.24 A側流量追加
   //***************************
   //***  A側流量
   //***************************
   if(!Sheet_Cell_Compare(49, 14)){
      Excel_Cell_DataSet(49, 26, "*");
   }
   //**********************

   //***************************
   //***  A側内容量
   //***************************
   // 『D013』
   if(!Sheet_Cell_Compare(50, 14)){
      Excel_Cell_DataSet(50, 26, "*");
   }

   //***************************
   //***  機器質量
   //***************************
   // 『D014』
   if(!Sheet_Cell_Compare(51, 17)){
      Excel_Cell_DataSet(51, 26, "*");
   }

   //************************
   //***  適用規格
   //************************
   // 『D015』
   if(!Sheet_Cell_Compare(52, 6)){
      Excel_Cell_DataSet(52, 26, "*");
   }

   //***************************
   //***  締付寸法　Max,Min
   //***************************
   // 『D016』,『D017』
   if(!Sheet_Cell_Compare(53, 9) || !Sheet_Cell_Compare(53, 15)){
      Excel_Cell_DataSet(53, 26, "*");
   }

   // 2016/09/14 ステッカー追加
   //***************************
   //***  ステッカー
   //***************************
   // 『D018』
   if(!Sheet_Cell_Compare(54, 18) || !Sheet_Cell_Compare(54, 24)){
      Excel_Cell_DataSet(54, 26, "*");
   }
   // **************************

   //******************************
   //***  Excel Book Close
   //******************************
   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server 移植対応
   exApplication.OlePropertySet("DisplayAlerts", false);
   exWorkbook_old.OleFunction("Close");

   exWorkCell_old.Clear();
   exWorkbook_old.Clear();
   exWorksheet_old.Clear();
   exWorksheets_old.Clear();

   DeleteFile(s_MakeExcel_Pass);
}

//---------------------------------------------------------------------------
// 日本語関数名：
// 概  要      ：
// 引  数      ：
// 戻り値      ：
// 備  考      ：
//---------------------------------------------------------------------------
bool __fastcall Sheet_Cell_Compare(int Row, int Col){
   AnsiString Newsht_Value;
   AnsiString Oldsht_Value;
   int        OldLayout;

   OldLayout = 0;   // オフセット値を初期化
   //***************************
   //***  最新の値　取得
   //***************************
   //exWorkCell   = exWorksheet.Exec(PropertyGet("Cells") << Row << Col);
   //Newsht_Value = exWorkCell.Exec(PropertyGet("Value"));
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells",Row,Col);
   Newsht_Value = exWorkCell.OlePropertyGet("Value");

   // 書式変更したため、前回帳票が旧書式の場合、比較位置をずらす
   if ( Row >= 9 && Row <= 11 ) {        // プレート及びガスケット仕様部分（書式変更による影響範囲）
      //exWorkCell_old = exWorksheet_old.Exec(PropertyGet("Cells") << 8 << 13);
      //Oldsht_Value = exWorkCell_old.Exec(PropertyGet("Value"));
      //Windows 2008Server 移植対応
      exWorkCell_old = exWorksheet_old.OlePropertyGet("Cells",8,13);
      Oldsht_Value = exWorkCell_old.OlePropertyGet("Value");
      if ( Oldsht_Value != "" ) {   //８行目の１３桁目に”（□はチェック項目です）”文字あり ＝　旧書式
         OldLayout = 1;         // オフセット値を設定
      }
   }

   //***************************
   //***  前回の値　取得
   //***************************
   //exWorkCell_old = exWorksheet_old.Exec(PropertyGet("Cells") << (Row + OldLayout) << Col);
   //Oldsht_Value = exWorkCell_old.Exec(PropertyGet("Value"));
   //Windows 2008Server 移植対応
   exWorkCell_old = exWorksheet_old.OlePropertyGet("Cells",(Row + OldLayout),Col);
   Oldsht_Value = exWorkCell_old.OlePropertyGet("Value");
   // ログ作成
   G_Log_Text = "セル番号  [" + FormatFloat("00",Row) + "," + FormatFloat("00",Col)
              + "]  前回値『" + Oldsht_Value + "』 今回値『" + Newsht_Value + "』";


   Write_Log(G_Log_Text);

   if (Newsht_Value.TrimRight() != Oldsht_Value.TrimRight()){
      return false;
   }

   return true;
}

//-------------------------------------------------------------------------------------
// 日本語関数名： 過去Excelｼｰﾄ　存在ﾁｪｯｸ
// 概  要      ： Rev*のﾃﾞｨﾚｸﾄﾘ名を検索し、指示書の存在する最大のﾘﾋﾞｼﾞｮﾝ番号を取得
// 引  数      ：
// 戻り値      ： 指示書の存在する直近のﾘﾋﾞｼﾞｮﾝ番号
// 備  考      ：
//-------------------------------------------------------------------------------------
AnsiString __fastcall Sheet_Exist_Check(AnsiString path, HeadsData* pcHeadsData){
   AnsiString RevNo;
   AnsiString NowRev;
   AnsiString SheetPass;
   bool       bFind;
   bool       bFlag;
   AnsiString sSpecCode;

   // 2007.10.10 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟｺｰﾄﾞ
   sSpecCode = AnsiString(pcHeadsData->GetSpecCode());

   //2004.11.16 出力フォルダ及び訂正印変更
   //NowRev = s_HEADS_DATA[319].TrimRight();
   NowRev = IntToStr(G_HEADS_DATA_Rev);
   //*************************************
   if ( NowRev.ToIntDef(0) == 0 ) {
      RevNo = "";
      return RevNo;
   //2004.11.16 出力フォルダ及び訂正印変更
   //}
   //RevNo = FormatFloat("00",NowRev.ToIntDef(0) - 1);
   } else {
      RevNo = FormatFloat("00",NowRev.ToIntDef(0) - 1);
   }
   //*************************************
   bFind = false;
   //2004.11.16 出力フォルダ及び訂正印変更
   //while(RevNo.ToIntDef(0) >= 0 && bFind == false) {
   //   // ﾌｧｲﾙ存在ﾁｪｯｸ
   //   SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + ".xls";
   //   if (FileExists(SheetPass.c_str()) == true){
   //     bFind = true;
   //   } else {
   //     RevNo = FormatFloat("00",RevNo.ToIntDef(0) - 1);
   //   }
   //}
   while(RevNo.ToIntDef(0) >= 0 && bFind == false) {
      // ﾌｧｲﾙ存在ﾁｪｯｸ
	  bFlag = false;

//Mod_Str 2019/02/19 Ri
	  //SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + sSpecCode + "R" + RevNo + ".xls";
	  //2023/4/21 製造番号桁数追加対応 Mod
	  //SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + sSpecCode + "R" + RevNo + "OLD.xls";
	  SheetPass = path + RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + "R" + RevNo + "OLD.xls";
//Mod_End 2019/02/19 Ri
	  G_Log_Text = "SheetPass?『 " + SheetPass + " 』";
      Write_Log(G_Log_Text);
      if (FileExists(SheetPass.c_str()))
      {
          bFlag = true;
      }
	  if (!bFlag) {
//Mod_Str 2019/02/19 Ri
		  //2023/4/21 製造番号桁数追加対応 Mod
		  //SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + sSpecCode + "OLD.xls";
		  SheetPass = path + RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + "OLD.xls";
//Mod_End 2019/02/19 Ri
          if (FileExists(SheetPass.c_str()))
          {
              bFlag = true;
          }
      }

      if (bFlag){
        bFind = true;
        G_Log_Text = "true?『 true 』";
        Write_Log(G_Log_Text);
      } else {
        RevNo = FormatFloat("00",RevNo.ToIntDef(0) - 1);
      }
   }
   //*************************************
   return RevNo;
}
////-------------------------------------------------------------------------------------
//// 日本語関数名： 過去Excelｼｰﾄ　存在ﾁｪｯｸ
//// 概  要      ： Rev*のﾃﾞｨﾚｸﾄﾘ名を検索し、指示書の存在する最大のﾘﾋﾞｼﾞｮﾝ番号を取得
//// 引  数      ：
//// 戻り値      ： 指示書の存在する直近のﾘﾋﾞｼﾞｮﾝ番号
//// 備  考      ：
////-------------------------------------------------------------------------------------
//AnsiString __fastcall Sheet_Exist_Check(AnsiString path){
//   WIN32_FIND_DATA fd;
//   AnsiString RevNo;
//   AnsiString filenm;
//   AnsiString NowRev;
//   AnsiString SheetPass;
//
//   HANDLE hFind = FindFirstFile((path + "*").c_str(), &fd);
//   NowRev = s_HEADS_DATA[319].TrimRight();
//   if ( NowRev.ToIntDef(0) == 0 ) {
//      RevNo = "";
//      return RevNo;
//   }
//
//   if(hFind != INVALID_HANDLE_VALUE) {
//      do {
//         filenm = (AnsiString)fd.cFileName;
//         if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
//            // ﾌｧｲﾙ存在ﾁｪｯｸ
//            SheetPass = path + filenm + "\\MAN" + G_SAKUBAN + ".xls";
//            if (FileExists(SheetPass.c_str())){
//               if ( filenm.ToIntDef(0) != 0 ) {
//                 // ﾘﾋﾞｼﾞｮﾝ番号を比較する。
//                 if ( (RevNo < filenm) && (NowRev > filenm) ){
//                    RevNo = filenm;
//                 }
//               }
//            }
//         }
//      } while(FindNextFile(hFind, &fd));
//
//      FindClose(hFind);
//   }
//
//   return RevNo;
//}

//---------------------------------------------------------------------------
// 日本語関数名： 仕上名称　取得
// 概  要      ： 引数として渡されたｺｰﾄﾞで取得する。
// 引  数      ： Code：S1ﾉｽﾞﾙ仕上(RECORD NO：904) or S2ﾉｽﾞﾙ仕上(RECORD NO：919)
// 戻り値      ： 変換結果
// 備  考      ：
//---------------------------------------------------------------------------
AnsiString __fastcall Get_SiageMeisyou(AnsiString Code){
   AnsiString s_Name;

   if (Code == "1"){
      s_Name = "内外  酸洗";

   } else if (Code == "2"){
      s_Name = "内ﾊﾞﾌ#320外ﾍﾔ";

   } else if (Code == "3"){
      s_Name = "内ﾊﾞﾌ#250外ﾍﾔ";

   } else if (Code == "4"){
      s_Name = "内ﾊﾞﾌ#180外ﾍﾔ";

   } else if (Code == "5"){
      s_Name = "――――――";

   } else if (Code == "6"){
      s_Name = "内酸洗外ﾍﾔ";

   } else {
      s_Name = "";
   }

   return s_Name;
}

//---------------------------------------------------------------------------
// 日本語関数名： 材質ｺｰﾄﾞﾏｽﾀ　和文名称取得
// 概  要      ： 引数として渡されたｺｰﾄﾞで材質ｺｰﾄﾞﾏｽﾀを検索する。
// 引  数      ： Code：検索用ｺｰﾄﾞ
// 戻り値      ： 変換結果
// 備  考      ：
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Zaisitu_Name(AnsiString Code){
   // 2021.06.18 FireDACへの移行 MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDACへの移行 MOD_E
   AnsiString s_Name;
   AnsiString s_SQL;

   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + Code + "';";

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
      s_Name = "";
   } else {
      s_Name = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   delete wkQuery;

   return s_Name;
}

//---------------------------------------------------------------------------
// 日本語関数名： 圧力計種類　取得
// 概  要      ： 引数として渡された圧力計種類を検索する。
// 引  数      ： Code：圧力計種類
// 戻り値      ： 変換結果
// 備  考      ：
//---------------------------------------------------------------------------
AnsiString __fastcall Get_AturyokuKei(AnsiString s_Code){
   AnsiString s_Aturyokukei;   // 圧力計種類

   if (s_Code.ToIntDef(0) == 1){
      s_Aturyokukei = "BCﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 2){
      s_Aturyokukei = "SUSﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 3){
      s_Aturyokukei = "BCﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 4){
      s_Aturyokukei = "SUSﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 5){
      s_Aturyokukei = "BCﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 6){
      s_Aturyokukei = "SS/TFｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 7){
      s_Aturyokukei = "SUSｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 8){
      s_Aturyokukei = "SS/TFｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 9){
      s_Aturyokukei = "SUSｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 10){
      s_Aturyokukei = "SS/TFｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 11){
      s_Aturyokukei = "SUSｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 12){
      s_Aturyokukei = "SS/TFｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 13){
      s_Aturyokukei = "SUSｶｸﾏｸ";

   //2004.05.19 圧力計条件追加
   } else if (s_Code.ToIntDef(0) == 18){
      s_Aturyokukei = "SUSﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 19){
      s_Aturyokukei = "BCﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 20){
      s_Aturyokukei = "SUSﾌﾞﾙﾄﾞﾝ";
   //*************************

   } else {
      s_Aturyokukei = "";
   }

   return s_Aturyokukei;
}

//---------------------------------------------------------------------------
// 日本語関数名： 端管ﾃﾞｰﾀ作成
// 概  要      ：
// 引  数      ： AnsiString Syurui：端管種類(S1～E4)
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Tankan_Data(AnsiString Syurui, HeadsData* pcHeadsData)
{
   AnsiString s_Tkn_Syurui;   // 端管種類
   AnsiString s_HEADS_DATA[1500];

   // AnsiString型のHEADSﾃﾞｰﾀを取得
   pcHeadsData->SetAnsiStringData(s_HEADS_DATA);

// 6列目を各材質からエア抜き、液抜き継手に変更  2002.10.20 S.Y
   int HEADS[8][9] = {{ 1136,  901,  898,  895,  892,  905, 1144, 1216, 1217  },
                      { 1137,  916,  913,  910,  907,  920, 1145, 1220, 1221  },
                      { 1138,  931,  928,  925,  922,  935, 1146, 1224, 1225  },
                      { 1139,  946,  943,  940,  937,  950, 1147, 1228, 1229  },
                      { 1140,  961,  958,  955,  952,  965, 1148, 1232, 1233  },
                      { 1141,  976,  973,  970,  967,  980, 1149, 1236, 1237  },
                      { 1142,  991,  988,  985,  982,  995, 1150, 1240, 1241  },
                      { 1143, 1006, 1003, 1000,  997, 1010, 1151, 1244, 1245  }};


   int s_Prs_Syurui;          // 圧力計種類
   int s_Tmp_Syurui;          // 温度計種類
   int iWFflg;                // WF端管？
   double s_Ank_Kokei;        // ｴｱ抜き口径
   double s_Nzl_Kokei;        // ﾉｽﾞﾙ口径
   AnsiString s_Nzl_Tugite;   // ﾉｽﾞﾙ継手
   AnsiString s_Nzl_Syurui;   // ﾉｽﾞﾙ種類
   AnsiString s_PGtyp;        // 圧力計種類（検索用）
   AnsiString s_TPtyp;        // 温度計種類（検索用）
   AnsiString s_DRtyp;        // 液抜き種類（検索用）
   AnsiString s_ARtyp;        // エア抜き種類（検索用）
   AnsiString P_Model;
   AnsiString P_Katasiki;

   int i_HeadsNo;

   s_Nzl_Syurui = "";
   if( Syurui == "S1" ) {
      i_HeadsNo = 0;
      s_Nzl_Syurui = s_HEADS_DATA[891].TrimRight();
   } else if( Syurui == "S2" ) {
      i_HeadsNo = 1;
      s_Nzl_Syurui = s_HEADS_DATA[906].TrimRight();
   } else if( Syurui == "S3" ) {
      i_HeadsNo = 2;
      s_Nzl_Syurui = s_HEADS_DATA[921].TrimRight();
   } else if( Syurui == "S4" ) {
      i_HeadsNo = 3;
      s_Nzl_Syurui = s_HEADS_DATA[936].TrimRight();
   } else if( Syurui == "E1" ) {
      i_HeadsNo = 4;
      s_Nzl_Syurui = s_HEADS_DATA[951].TrimRight();
   } else if( Syurui == "E2" ) {
      i_HeadsNo = 5;
      s_Nzl_Syurui = s_HEADS_DATA[966].TrimRight();
   } else if( Syurui == "E3" ) {
      i_HeadsNo = 6;
      s_Nzl_Syurui = s_HEADS_DATA[981].TrimRight();
   } else if( Syurui == "E4" ) {
      i_HeadsNo = 7;
      s_Nzl_Syurui = s_HEADS_DATA[996].TrimRight();
   }

   // 圧力計種類
   s_Prs_Syurui  = s_HEADS_DATA[HEADS[i_HeadsNo][1]].TrimRight().ToIntDef(0);
   // 温度計種類
   s_Tmp_Syurui  = s_HEADS_DATA[HEADS[i_HeadsNo][2]].TrimRight().ToIntDef(0);
   // ｴｱ抜き口径
   s_Ank_Kokei   = StrToDblDef(s_HEADS_DATA[HEADS[i_HeadsNo][3]].TrimRight(),0);
   // ﾉｽﾞﾙ口径
   s_Nzl_Kokei   = StrToDblDef(s_HEADS_DATA[HEADS[i_HeadsNo][4]].TrimRight(),0);
   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //// ﾉｽﾞﾙ継手(先頭一桁)
   // ﾉｽﾞﾙ継手(先頭二桁)
   //s_Nzl_Tugite  = s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1,1);
   s_Nzl_Tugite  = s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(1,2);
   // ***********************

   //***********************************
   //***  端管ﾃﾞｰﾀ　作成
   //***********************************
   s_Tkn_Syurui = s_HEADS_DATA[HEADS[i_HeadsNo][0]].TrimRight();  // 端管種類
   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //if ( ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "1" ) ||
   //     ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(3,1) == "2" ) ) {
   if ( ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4,1) == "1" ) ||
        ( s_HEADS_DATA[HEADS[i_HeadsNo][6]].SubString(4,1) == "2" ) ) {
   // ***********************
     iWFflg=1;
   } else {
     iWFflg=0;
   }
   // (1)

   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //↓2002/10/24
   if(s_Tkn_Syurui == "0"){
      return "";
   }

   // 02.10.16 新規追加条件
   if (Syurui == "S3" || Syurui == "S4" || Syurui == "E3" || Syurui == "E4"){
      // ﾉｽﾞﾙ種類 = "3" かつ ﾉｽﾞﾙ継手の先頭二桁目 = "4"
      // 2011.06.20 ﾉｽﾞﾙ規格変更
      //if (s_Nzl_Syurui == "3" && s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(2,1) != "4" && s_Nzl_Kokei != 0 ){
      //   if (s_Nzl_Tugite == "7"){
      if ( s_Nzl_Syurui == "3"
        && s_HEADS_DATA[HEADS[i_HeadsNo][5]].SubString(3,1) != "4"
        && s_Nzl_Kokei != 0 ) {
         if (s_Nzl_Tugite == "07"){
      // ***********************
            return "端管Ｘ";
         } else {
            P_Katasiki = "端管Ｙ";
            return "端管Ｙ";
         }
      }
   }

   // 圧力計検索キー作成
   if (s_Prs_Syurui == 0 ) {
      s_PGtyp = "-";
   } else if ( (s_Prs_Syurui > 5 ) && (s_Prs_Syurui < 16) ){
      s_PGtyp = "K";
   } else if ( s_Prs_Syurui == 5 ){
      s_PGtyp = "S";
   //2004.05.19 圧力計条件追加
   } else if ( s_Prs_Syurui == 18 || s_Prs_Syurui == 19 || s_Prs_Syurui == 20 ){
      s_PGtyp = "S";
   //*************************
   } else {
      s_PGtyp = "B";
   }

   // 温度計検索キー作成
   if (s_Tmp_Syurui == 0 ) {
      s_TPtyp = "-";
   } else {
      s_TPtyp = "T";
   }
   if (Syurui == "S1" || Syurui == "S2" || Syurui == "E1" || Syurui == "E2"){
      // エア抜き有無
      s_DRtyp = "-";
      if ( s_Ank_Kokei > 0 ) {
         s_ARtyp = "A";
      } else {
         s_ARtyp = "-";
      }
   } else {
      // 液抜きサイズ
      s_ARtyp = "-";
      if ( s_Ank_Kokei > 0 ) {
         // 2011.06.20 ﾉｽﾞﾙ規格変更
         //if ( s_Nzl_Tugite != "7" ) {
         if ( s_Nzl_Tugite != "07" ) {
         // ***********************
           s_DRtyp = "L";
         }  else {
           s_DRtyp = "S";
         }
      } else {
         s_DRtyp = "-";
      }
   }

   P_Katasiki = Search_HD_FRNG_MST(P_Model, s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp,false);
   if ( P_Katasiki == "" ) {
//     P_Katasiki = Search_HD_FRNG_MST("-", s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp);
     P_Katasiki = "該当端管なし";
   }
   //2009.07.31 端菅名称変更
   if ( P_Katasiki == "端管１３（圧）" ) {
       P_Katasiki = "端管１３";
   }
   //***********************
   if ( P_Katasiki != "" ) {
     return P_Katasiki;
   }
   return "";

}

//---------------------------------------------------------------------------
// 日本語関数名： フランジ種類取得
// 概  要      ：
// 引  数      ： AnsiString Syurui：端管種類(S1～E4)
// 戻り値      ： フランジ種類]
// 備  考      ： なし
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Flange_Data(AnsiString Tugite)
{
   AnsiString s_Flange_Syurui;   // ﾌﾗﾝｼﾞ種類
   AnsiString s_Text;

   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //s_Text = Tugite.SubString(3,1);
   s_Text = Tugite.SubString(4,1);
   // ***********************

   if (s_Text == "0"){
      s_Flange_Syurui = "フリーＦ";

   } else if (s_Text == "1"){
      s_Flange_Syurui = "ＲＦ．ＷＦ";

   } else if (s_Text == "2"){
      s_Flange_Syurui = "ＷＦ";

   } else if (s_Text == "3"){
      s_Flange_Syurui = "ＬＪ";

   } else if (s_Text == "4"){
      s_Flange_Syurui = "ＬＪ　Ｆ";

   } else if (s_Text == "6"){
      s_Flange_Syurui = "ＬＪ";

   } else if (s_Text == "7"){
      s_Flange_Syurui = "サニタリネジ";

   } else if (s_Text == "8"){
      s_Flange_Syurui = "サニタリヘルール";

   } else if (s_Text == "9"){
      s_Flange_Syurui = "ネジＦ";

   } else {
      s_Flange_Syurui = "";
   }
   return s_Flange_Syurui;
}

//---------------------------------------------------------------------------
// 日本語関数名： 温度計座材質取得
// 概  要      ：
// 引  数      ： TanSyurui:端管種類(S1～S4)
//             ： TanName  :端管名称
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 部品展開ﾃﾞｰﾀ,HEARTS用ﾃﾞｰﾀに孔明ﾃﾞｰﾀを書込む
//---------------------------------------------------------------------------
AnsiString __fastcall Get_T_ZaZaisitu(AnsiString TanSyurui, AnsiString TanName, HeadsData* pcHeadsData)
{
   AnsiString s_T_ZaZaisitu;  // 温度計座の材質名称

   AnsiString s_NT_Zaisitu;   // ﾉｽﾞﾙ・端管材質ｺｰﾄﾞ
   AnsiString s_Code;         // 温度計座の材質ｺｰﾄﾞ
   AnsiString s_HEADS_DATA[1500];

   pcHeadsData->SetAnsiStringData(s_HEADS_DATA);

   s_T_ZaZaisitu = "";

   //**********************************
   //***  温度計座の材質ｺｰﾄﾞの決定
   //**********************************
   if (TanSyurui == "S1"){
      if (s_HEADS_DATA[898].ToIntDef(0) == 2) {   // 温度計種類が０２：スチーム用の場合は、ＳＵＳ４１０とする
        return "ＳＵＳ４１０";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1060].SubString(1,4);   // B端管材質
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[894].TrimRight();   // S1ﾉｽﾞﾙ材質
         }
      }
   } else if (TanSyurui == "S2"){
      if (s_HEADS_DATA[913].ToIntDef(0) == 2) {   // 温度計種類が０２：スチーム用の場合は、ＳＵＳ４１０とする
        return "ＳＵＳ４１０";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1061].SubString(1,4);   // A端管材質
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[909].TrimRight();   // S2ﾉｽﾞﾙ材質
         }
      }
   } else if (TanSyurui == "S3"){
      if (s_HEADS_DATA[928].ToIntDef(0) == 2) {   // 温度計種類が０２：スチーム用の場合は、ＳＵＳ４１０とする
        return "ＳＵＳ４１０";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1060].SubString(1,4);   // B端管材質
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[924].TrimRight();   // S3ﾉｽﾞﾙ材質
         }
      }
   } else if (TanSyurui == "S4"){
      if (s_HEADS_DATA[943].ToIntDef(0) == 2) {   // 温度計種類が０２：スチーム用の場合は、ＳＵＳ４１０とする
        return "ＳＵＳ４１０";
      } else {
         if (TanName.Trim() != "") {
            s_NT_Zaisitu = s_HEADS_DATA[1061].SubString(1,4);   // A端管材質
         } else {
            s_NT_Zaisitu = s_HEADS_DATA[939].TrimRight();   // S4ﾉｽﾞﾙ材質
         }
      }
   }

//   //**********************************
//   //***  材質ｺｰﾄﾞの変換
//   //**********************************
//   switch (s_NT_Zaisitu.ToIntDef(0)){
//      case 1001:
//         s_Code = "1003";
//         break;
//      case 1002:
//         s_Code = "1004";
//         break;
//      case 1003:
//         s_Code = "1003";
//         break;
//      case 1004:
//         s_Code = "1004";
//         break;
//      case 1005:
//         s_Code = "1005";
//         break;
//      case 1006:
//         s_Code = "1006";
//         break;
//      case 1012:
//         s_Code = "1045";
//         break;
//      case 1013:
//         s_Code = "1045";
//         break;
//      case 1014:
//         s_Code = "1032";
//         break;
//      case 1015:
//         s_Code = "1033";
//         break;
//      case 1016:
//         s_Code = "1045";
//         break;
//      case 1017:
//         s_Code = "1045";
//         break;
//      case 1018:
//         s_Code = "1045";
//         break;
//      case 1031:
//         s_Code = "1032";
//         break;
//      case 1045:
//         s_Code = "1045";
//         break;
//      case 1056:
//         s_Code = "1056";
//         break;
//      case 1064:
//         s_Code = "1045";
//         break;
//      default:
//         s_Code = "";
//
//         //return s_T_ZaZaisitu;
//         break;
//   }

   //**********************************
   //***  材質ｺｰﾄﾞﾏｽﾀの検索
   //**********************************
   s_Code = Chg_Zaisitu_Code( s_NT_Zaisitu, 1);
   s_T_ZaZaisitu = Get_Zaisitu_Name(s_Code);

   return s_T_ZaZaisitu;
}

//---------------------------------------------------------------------------
// 日本語関数名： スタートプレート取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 部品展開ﾃﾞｰﾀ,HEARTS用ﾃﾞｰﾀに孔明ﾃﾞｰﾀを書込む
//---------------------------------------------------------------------------
bool __fastcall Get_Start_Plate(AnsiString &Side_B, AnsiString &Side_A, HeadsData* pcHeadsData){

   FILE *wkfp;

   // CSV読込用
   char  buf[1024];                            /* 文字列バッファ   */
   char  *tok;                                 /* トークンポインタ */
   int   i;                                    /* ループカウンタ   */
   char  token[100][100];                      /* トークンバッファ */

   // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ用
   AnsiString s_Ana_Plate;    //
   AnsiString s_Ana_Kata;     // 型式
   AnsiString s_Ana_Kbn;      // 区分
   AnsiString s_Ana_ABKbn;    // AB区分
   AnsiString s_Ana_Anaake;   // 孔明
   AnsiString s_Ana_Suryo;    // 1台分数量
   AnsiString s_Ana_Zaisitu;  // ﾌﾟﾚｰﾄ材質
   AnsiString s_Ana_Siyou;    // 仕様

   AnsiString s_Text;

   AnsiString wk_FilePass;
   AnsiString hs_FilePass;

   // 2007.10.10 ﾐﾗｰ仕様対応 仕様ﾀｲﾌﾟ取得
   AnsiString s_SpecCode;
   s_SpecCode = AnsiString(pcHeadsData->GetSpecCode());

   // 初期データセット
   Side_B = "Ｂ側";
   Side_A = "Ａ側";

   // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀﾊﾟｽ
   //2023/4/21 製造番号桁数追加対応 Mod
   //wk_FilePass = G_AnaFilePass + "HOL" + G_SAKUBAN + s_SpecCode + ".csv";
   wk_FilePass = G_AnaFilePass + "HOL" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".csv";

   if ((wkfp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ 『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);

   } else {
      // 読込み可能の間
      memset(buf,0x00,sizeof(buf));
      while(fgets(buf,sizeof(buf),wkfp)!=NULL){
         i = 0;                                 // トークンカウンタ初期化
         memset(token,0x00,sizeof(token));      // トークンバッファ初期化
         tok = GetToken(buf,",");               // ","で項目を区切る
         while(tok){                            // トークンが存在する間
            sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
            tok = GetToken(NULL,",");           // 次のトークンを検索

            if (i>100) {
               break;
            }
         }

         s_Ana_Plate  = token[0];               // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 型式
         s_Ana_Kbn    = token[1];               // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 区分
         s_Ana_ABKbn  = token[2];               // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 AB区分
         s_Ana_Anaake = token[3];               // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 孔明
         s_Ana_Suryo  = token[4];               // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 1台分数量
         s_Ana_Zaisitu= token[5];               // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ内 ﾌﾟﾚｰﾄ型式

         // 有効ﾚｺｰﾄﾞﾁｪｯｸ
//         if (s_Ana_Plate.SubString(1,2) == G_K_Syurui && s_Ana_Plate.SubString(4,2) == G_K_Model1 && s_Ana_Kbn == "D"){
         if ( s_Ana_Kbn == "D"){
            if (s_Ana_ABKbn == "B"){
               Side_B = "Ｂ側（スタート）";
               Side_A = "Ａ側";
            } else {
               Side_B = "Ｂ側";
               Side_A = "Ａ側（スタート）";
            }
         }
      }
   }

   fclose(wkfp);  // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ 閉じる

   //delete wkfp;

   return true;
}


//---------------------------------------------------------------------------
// 日本語関数名： シートガスケット材質取得
// 概  要      ：
// 引  数      ： s_Side：流体種類(A:A側,B:B側)
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2017.10.06追加
//---------------------------------------------------------------------------
AnsiString __fastcall Get_SheetGasketMat(AnsiString s_Side, HeadsData* pcHeadsData)
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

   AnsiString s_HEADS_DATA[1500];

   // AnsiString型のHEADSﾃﾞｰﾀを取得
   pcHeadsData->SetAnsiStringData(s_HEADS_DATA);

   //***************************
   //***  型       式
   //***************************
   i_RecNo = 33;
   s_Katasiki = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  種       類
   //***************************
   i_RecNo = 34;
   s_Syurui = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Ｂスチーム区分(CASE1)
   //***************************
   i_RecNo = 846;
   s_Flow1 = s_HEADS_DATA[i_RecNo].TrimRight();

    //***************************
   //***  Bスチーム区分(CASE1)
   //***************************
   i_RecNo = 847;
   s_Flow2 = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Ｂスチーム区分(CASE2)
   //***************************
   i_RecNo = 869;
   s_Flow3 = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Bスチーム区分(CASE2)
   //***************************
   i_RecNo = 870;
   s_Flow4 = s_HEADS_DATA[i_RecNo].TrimRight();

   //***************************
   //***  Ｂ側設計温度
   //***************************
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   d_Bdt = s_Text.ToDouble();

   //***************************
   //***  A側設計温度
   //***************************
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
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

   //***************************
   //***  材 質 選 定
   //***************************
   s_GasMat="";
   s_Text="";
   if (i_Flow == 1) {
      // 蒸気１（ＥＰＤＭ）
      s_GasMat ="ＥＰＤＭ";
   } else if (i_Flow == 2) {
      // 蒸気２（バルカーＮｏ．ＧＦ３００）
      s_GasMat ="Ｖ／＃ＧＦ３００";
   } else {
      if(s_Side=="B") {
         //Ｂ側プレートガスケット
         i_RecNo = 835;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      } else if (s_Side=="A") {
         //Ａ側プレートガスケット
         i_RecNo = 836;
         s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      }
      s_Text = Chg_Zaisitu_Code(s_Text,"2");
      s_GasMat = Get_Zaisitu_Name(s_Text);
   }

   return s_GasMat;

}

