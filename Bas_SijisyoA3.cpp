
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

#include "Bas_SijisyoA3.h"

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
bool __fastcall SijisyoA3(HeadsData* pcHeadsData)
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
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   AnsiString s_Text1;           // 汎用文字列１
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_E
   //2004.07.15 変数追加
   AnsiString s_Text2;           // 汎用文字列２
   //*******************
   //2019.10.10 変数追加_S
   AnsiString s_Text3;           // 汎用文字列３
   AnsiString s_Text4;           // 汎用文字列４
   //2019.10.10 変数追加_E
   // 2020.05.28 温水乾燥注記追加_S
   int        i_Length;          // 文字数
   // 2020.05.28 温水乾燥注記追加_E
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

   //2019.07.25 製作指示書書式変更_S
   int P_FL; //斜行流ﾌﾗｸﾞ(0:台形流,1:斜行流)
   //2019.07.25 製作指示書書式変更_E

//Add_Str 2019/02/22 Ri
   //*******************
   //ノズルオリエンテーション
   //*******************
   AnsiString s_S1NO_JP;         // S1液名和文
   AnsiString s_S2NO_JP;         // S2液名和文
   AnsiString s_S3NO_JP;         // S3液名和文
   AnsiString s_S4NO_JP;         // S4液名和文

   AnsiString s_S1NO_EN;         // S1液名英文
   AnsiString s_S2NO_EN;         // S2液名英文
   AnsiString s_S3NO_EN;         // S3液名英文
   AnsiString s_S4NO_EN;         // S4液名英文
   //*******************
//Add_End 2019/02/22 Ri

   // 2012.01.20 中国材質
   AnsiString s_ChinaM;
   // *******************

   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   AnsiString sBltMat;           // 締付ボルト材質
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_E

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

   // 2020.10.30 温度計保護管修正_S
   AnsiString s_S1OndoCode;      // S1温度計種類
   AnsiString s_S2OndoCode;      // S2温度計種類
   AnsiString s_S3OndoCode;      // S3温度計種類
   AnsiString s_S4OndoCode;      // S4温度計種類
   // 2020.10.30 温度計保護管修正_E

   //2020.11.24 ガスケット注記追加_S
   bool bGas_A;           // A側ｼｰﾄｶﾞｽｹｯﾄ有無
   bool bGas_B;           // B側ｼｰﾄｶﾞｽｹｯﾄ有無
   double d_Koukei;       // 口径
   AnsiString s_GasMatA;  // A側ｼｰﾄｶﾞｽｹｯﾄ材質
   AnsiString s_GasMatB;  // B側ｼｰﾄｶﾞｽｹｯﾄ材質
   AnsiString s_GasName;  // ガスケット名称　　
   //2020.11.24 ガスケット注記追加_E

   if(pcHeadsData->GetMirrorFlag()){
        // ﾐﾗｰ仕様の場合、A仕様、B仕様以外は処理不要(正常終了)
        if(pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_NONE) return(true);
   }

   // AnsiString型HEADSﾃﾞｰﾀを取得
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   AnsiString P_HEADS_DATA[1500];
   // HEADSﾃｷｽﾄﾃﾞｰﾀをAnsiString配列に取得
   bRet = pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
   if (!bRet) return (false);

   // 仕様ﾀｲﾌﾟ文字列を取得(ﾌｧｲﾙ名作成で使用)
   s_SpecCode = AnsiString(pcHeadsData->GetSpecCode());

   s_Tmplate_Pass = G_Template_Dir + "製作指示書A3.xls";                 // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
   s_MakeExcel_Pass = G_xls_Output_Dir + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
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

   //***************************
   //***  図面訂正
   //***************************
   // 『A002』
   i_RecNo = 319;
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
   // 『A007』
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
   //***  バーコード
   //***************************
   // 『A012』
   //2019.07.25 製作指示書書式変更_S
   ////"'*" + A006の(1～2文字目) + A006の(4～7文字目) + "%MD" + A003の文字 + "*"
   ////を作成A012とA013(バーコード)に出力します
   //s_Text = "'*" + G_SAKUBAN.SubString(1,2) + G_SAKUBAN.SubString(3,4) + "%MD" + s_HEADS_DATA[1087] + "*";
   //Excel_Cell_DataSet(1, 19, s_Text);
   //// 『A013』
   //Excel_Cell_DataSet(2, 25, s_Text);
   //2019.07.25 製作指示書書式変更_E
   // 2023.05.30 バーコード変更_S
   s_Text = G_SAKUBAN;
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));
   Excel_Cell_DataSet(4, 5, s_Text);
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("製作指示書"));
   // 2023.05.30 バーコード変更_E

   //****************************
   //***  塗装色
   //****************************
   // 『A014』
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

   //2019.12.27 製作指示書書式変更_S
   s_Text = HanToZen(s_Text);
   //2019.12.27 製作指示書書式変更_E

   Excel_Cell_DataSet(8, 6, s_Text);
   G_Log_Text = "塗装色               『" + s_Text + "』をセット。";
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

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｂ欄　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //**************************
   //***  スタートプレート
   //**************************
   Get_Start_PlateA3(s_Side_B, s_Side_A, pcHeadsData);
   // 『B001』
   Excel_Cell_DataSet(9, 6, s_Side_B);
   // 『B002』
   Excel_Cell_DataSet(9, 14, s_Side_A);

   //***************************
   //***  B側ﾌﾟﾚｰﾄ編成 1
   //***  B側ﾌﾟﾚｰﾄ編成 2
   //***************************
   // 『B003』 B側ﾌﾟﾚｰﾄ編成 1
   s_Text = s_HEADS_DATA[837].TrimRight() + "×" + s_HEADS_DATA[838].TrimRight();
   s_Text = s_Text + s_MIX1;
   G_Log_Text = "B側ﾌﾟﾚｰﾄ編成 1       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 『B003』 B側ﾌﾟﾚｰﾄ編成 2
   i_Value_1 = s_HEADS_DATA[839].ToIntDef(0);   // B編成21
   i_Value_2 = s_HEADS_DATA[840].ToIntDef(0);   // B編成22

   if (i_Value_1 * i_Value_2 == 0){
      s_Text2 = "";
   } else {
      s_Text2 = AnsiString(i_Value_1) + "×" + AnsiString(i_Value_2) + s_MIX2;
      s_Text2 = " ／ " + s_Text2;
   }
   G_Log_Text = "B側ﾌﾟﾚｰﾄ編成 2       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   Excel_Cell_DataSet(10, 7, s_Text + s_Text2);

   //***************************
   //***  A側ﾌﾟﾚｰﾄ編成 1
   //***  A側ﾌﾟﾚｰﾄ編成 2
   //***************************
   // 『B004』 A側ﾌﾟﾚｰﾄ編成 1
   s_Text = s_HEADS_DATA[841].TrimRight() + "×" + s_HEADS_DATA[842].TrimRight();
   s_Text = s_Text + s_MIX1;
   G_Log_Text = "A側ﾌﾟﾚｰﾄ編成 1       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 『B004』 A側ﾌﾟﾚｰﾄ編成 2
   i_Value_1 = s_HEADS_DATA[843].ToIntDef(0);   // B編成21
   i_Value_2 = s_HEADS_DATA[844].ToIntDef(0);   // B編成22

   if (i_Value_1 * i_Value_2 == 0){
      s_Text2 = "";
   } else {
      s_Text2 = AnsiString(i_Value_1) + "×" + AnsiString(i_Value_2) + s_MIX2;
      s_Text2 = " ／ " + s_Text2;
   }
   G_Log_Text = "A側ﾌﾟﾚｰﾄ編成 2       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   Excel_Cell_DataSet(10, 15, s_Text + s_Text2);

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

   Excel_Cell_DataSet(11, 7, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2019.07.25 製作指示書書式変更_S
   //***************************
   //***  斜行流フラグ
   //***************************
   // 2022.04.21 CX-01D追加_S
    if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 1) {
       //CX-01D
       P_FL = 1; // 斜行流
    }
    //if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
    else if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       // 2022.04.21 CX-01D追加_S
       //CX-10D
       P_FL = 1; // 斜行流
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 21 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 90) {
       //LX-90D
       P_FL = 1; // 斜行流
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 80) {
       //YX-80
       P_FL = 1; // 斜行流
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 83) {
       //YX-83
       P_FL = 1; // 斜行流
    }
    else {
       P_FL = 0; // 台形流
    }
   //2019.07.25 製作指示書書式変更_E

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B006』
   i_RecNo = 835;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "B側ｶﾞｽｹｯﾄ材質         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";

   s_Text = Get_Zaisitu_NameA3(s_Text);

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

   Excel_Cell_DataSet(12, 7, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B007』
   i_RecNo = 836;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "A側ｶﾞｽｹｯﾄ材質         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』より";

   s_Text = Get_Zaisitu_NameA3(s_Text);

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

   Excel_Cell_DataSet(12, 15, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

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

   s_Text = Get_Zaisitu_NameA3(s_Text);

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
   //2019.07.25 製作指示書書式変更_S
   ////********************
   ////2010.06.07 CX-10D追加
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10D" ) {
   //    s_Text = s_Text + " (S1,S4)";
   //}
   ////*********************
   if ( P_FL == 1 ) {
       // 斜行流
       s_Text = s_Text + " (S1,S4)";
   }
   //2019.07.25 製作指示書書式変更_E

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

   s_Text = Get_Zaisitu_NameA3(s_Text);

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

   //2019.07.25 製作指示書書式変更_S
   ////2010.06.07 CX-10D追加
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10D" ) {
   //    s_Text = s_Text + " (S2,S3)";
   //}
   ////*********************
   if ( P_FL == 1 ) {
       // 斜行流
       s_Text = s_Text + " (S2,S3)";
   }
   //2019.07.25 製作指示書書式変更_E

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
      //2019.07.25 製作指示書書式変更_S
      //s_S1TugiteNM = s_HEADS_DATA[1184].TrimRight();  // S1端菅継手名称
      s_S1TugiteNM = s_HEADS_DATA[1152].TrimRight();  // S1ﾉｽﾞﾙ継手名称
      //2019.07.25 製作指示書書式変更_E
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

   //2019.07.25 製作指示書書式変更_S
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if ( P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S1TugiteNM;
   //   }
   //} else if ( P_Model == "CX-10D" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S1TugiteNM;
   //   }
   //// 2016.04.22 CX-00追加
   //} else if ( P_Model == "CX-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S1TugiteNM;
   //   }
   //} else if ( P_Model == "CXW-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S1TugiteNM;
   //   }
   //// ********************
   //// 2013.04.03 SX-30追加
   //} else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //      s_Text = "リング端管 " + s_S1TugiteNM;
   //// ********************
   //// 2016.07.22 SX-30S追加
   //} else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //      s_Text = "リング端管 " + s_S1TugiteNM;
   //// *********************
   //} else if ( s_Text == "1" || s_Text == "3" ) {
   //   s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1端管種類
   //   s_Text = s_S1TankanNM + " " + s_S1TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1端管種類
   //   s_Text = s_S1TankanNM + " " + s_S1TugiteNM + "ＷＦ";
   //
   //} else {
   //   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //   //if (s_HEADS_DATA[893].SubString(2,1) == "4") {  // フランジ付きノズル
   //   if (s_HEADS_DATA[893].SubString(3,1) == "4") {  // フランジ付きノズル
   //   // ***********************
   //      s_Text = s_S1TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[893].TrimRight());
   //   } else {
   //      s_Text = s_S1TugiteNM;
   //   }
   //
   //}
   ////***************************
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1端管種類
      s_Text = s_S1TugiteNM + " " + s_S1TankanNM;

   } else if ( s_Text == "2" ) {
      s_S1TankanNM = Get_Tankan_DataA3("S1", pcHeadsData);  // S1端管種類
      s_Text = s_S1TugiteNM + " " + s_S1TankanNM + " ＷＦ";

   } else {
      if (s_HEADS_DATA[893].SubString(3,1) == "4") {  // フランジ付きノズル
         s_Text = s_S1TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[893].TrimRight());
      } else {
         s_Text = s_S1TugiteNM;
      }

   }
   //2019.07.25 製作指示書書式変更_E

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
      //2019.07.25 製作指示書書式変更_S
      //s_S2TugiteNM = s_HEADS_DATA[1188].TrimRight();  // S2端菅継手名称
      s_S2TugiteNM = s_HEADS_DATA[1156].TrimRight();  // S2ﾉｽﾞﾙ継手名称
      //2019.07.25 製作指示書書式変更_E
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

   //2019.07.25 製作指示書書式変更_S
   //if ( s_Text == "1" || s_Text == "3" ) {
   //   s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2端菅
   //   s_Text = s_S2TankanNM + " " + s_S2TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2端菅
   //   s_Text = s_S2TankanNM + " " + s_S2TugiteNM + "ＷＦ";
   //
   //} else {
   //   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //   //if ( s_HEADS_DATA[908].SubString(2,1) == "4" ) {  // フランジ付きノズル
   //   if ( s_HEADS_DATA[908].SubString(3,1) == "4" ) {  // フランジ付きノズル
   //   // ***********************
   //      s_Text = s_S2TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[908].TrimRight());
   //   } else {
   //      s_Text = s_S2TugiteNM;
   //   }
   //
   //}
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2端菅
      s_Text = s_S2TugiteNM + " " + s_S2TankanNM;

   } else if ( s_Text == "2" ) {
      s_S2TankanNM = Get_Tankan_DataA3("S2", pcHeadsData);  // S2端菅
      s_Text = s_S2TugiteNM + " " + s_S2TankanNM + " ＷＦ";

   } else {
      if ( s_HEADS_DATA[908].SubString(3,1) == "4" ) {  // フランジ付きノズル
         s_Text = s_S2TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[908].TrimRight());
      } else {
         s_Text = s_S2TugiteNM;
      }

   }
   //2019.07.25 製作指示書書式変更_E

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
   //***  B側S1ｽﾁｰﾑ区分
   //***************************
   // 『B014』,『B015』
   i_RecNo = 898;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S1温度計種類

   //2019.07.25 製作指示書書式変更_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I型";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L型";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 製作指示書書式変更_E
      // 『B014』
      s_Text = AnsiString(s_HEADS_DATA[899].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[900].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1357].TrimRight()));

      //2019.07.25 製作指示書書式変更_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 製作指示書書式変更_E

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
      //2019.07.25 製作指示書書式変更_S
      //G_Log_Text = "B側S1温度計座材質    『" + s_Text + "』をセット。";
      G_Log_Text = "S1ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      //2019.07.25 製作指示書書式変更_E
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
      //2019.07.25 製作指示書書式変更_S
      //G_Log_Text = "B側S1温度計座材質    『" + s_Text + "』をセット。";
      G_Log_Text = "S1ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      //2019.07.25 製作指示書書式変更_E

      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S2温度計
   //***  A側S2ｽﾁｰﾑ区分
   //***************************
   // 『B016』,『B017』
   i_RecNo = 913;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S2温度計種類

   //2019.07.25 製作指示書書式変更_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I型";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L型";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 製作指示書書式変更_E
      // 『B016』
      s_Text = AnsiString(s_HEADS_DATA[914].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[915].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1358].TrimRight()));

      //2019.07.25 製作指示書書式変更_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 製作指示書書式変更_E

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
      //2019.07.25 製作指示書書式変更_S
      //G_Log_Text = "A側S2温度計座材質    『" + s_Text + "』をセット。";
      G_Log_Text = "S2ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      //2019.07.25 製作指示書書式変更_E
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
      //2019.07.25 製作指示書書式変更_S
      //G_Log_Text = "A側S2温度計座材質    『" + s_Text + "』をセット。";
      G_Log_Text = "S2ｽﾁｰﾑ区分           『" + s_Text + "』をセット。";
      //2019.07.25 製作指示書書式変更_E
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  S1圧力計
   //***************************
   // 『B018』
   s_Text = s_HEADS_DATA[901].TrimRight();   // S1圧力計種類

   // 2020.07.17 圧力計変更_S
   ////2019.07.25 製作指示書書式変更_S
   //////2004.05.19 圧力計条件追加
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   //2019.07.25 製作指示書書式変更_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 圧力計変更_E
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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[901].TrimRight()) + " ";

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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[901].TrimRight()) + " ";

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

   // 2020.07.17 圧力計変更_S
   ////2019.07.25 製作指示書書式変更_S
   //////2004.05.19 圧力計条件追加
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   ////2019.07.25 製作指示書書式変更_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 圧力計変更_E
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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[916].TrimRight()) + " ";
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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[916].TrimRight()) + " ";
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
      //2019.07.25 製作指示書書式変更_S
      //s_S3TugiteNM = s_HEADS_DATA[1192].TrimRight();  // S3端菅継手名称
      s_S3TugiteNM = s_HEADS_DATA[1160].TrimRight();  // S3ﾉｽﾞﾙ継手名称
      //2019.07.25 製作指示書書式変更_E
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

   //2019.07.25 製作指示書書式変更_S
   //// 2016.04.22 CX-00追加
   //if ( P_Model == "CX-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S1TugiteNM;
   //   }
   //} else if ( P_Model == "CXW-00" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S1TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S1TugiteNM;
   //   }
   //// 2013.04.03 SX-30追加
   ////if ( P_Model == "SX-30" && s_S3TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
   //} else if ( P_Model == "SX-30" && s_S3TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
   //// ********************
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   s_Text = "リング端管 " + s_S3TugiteNM;
   //// 2016.07.22 SX-30S追加
   //} else if ( P_Model == "SX-30S" && s_S3TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   s_Text = "リング端管 " + s_S3TugiteNM;
   // // ********************
   ////if ( s_Text == "1" || s_Text == "3" ) {
   //} else if ( s_Text == "1" || s_Text == "3" ) {
   //// ********************
   //   s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3端管種類
   //   s_Text = s_S3TankanNM + " " + s_S3TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3端管種類
   //   s_Text = s_S3TankanNM + " " + s_S3TugiteNM + "ＷＦ";
   //
   //} else {
   //   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //   //if ( s_HEADS_DATA[923].SubString(2,1) == "4" ) {  // フランジ付きノズル
   //   if ( s_HEADS_DATA[923].SubString(3,1) == "4" ) {  // フランジ付きノズル
   //   // ***********************
   //      s_Text = s_S3TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[923].TrimRight());
   //   } else {
   //      s_Text = s_S3TugiteNM;
   //   }
   //
   //}
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3端管種類
      s_Text = s_S3TugiteNM + " " + s_S3TankanNM;

   } else if ( s_Text == "2" ) {
      s_S3TankanNM = Get_Tankan_DataA3("S3", pcHeadsData);  // S3端管種類
      s_Text = s_S3TugiteNM + " " + s_S3TankanNM + " ＷＦ";

   } else {
      if ( s_HEADS_DATA[923].SubString(3,1) == "4" ) {  // フランジ付きノズル
         s_Text = s_S3TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[923].TrimRight());
      } else {
         s_Text = s_S3TugiteNM;
      }

   }
   //2019.07.25 製作指示書書式変更_E

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
      //2019.07.25 製作指示書書式変更_S
      //s_S4TugiteNM = s_HEADS_DATA[1196].TrimRight();  // S4端菅継手名称
      s_S4TugiteNM = s_HEADS_DATA[1164].TrimRight();  // S4ﾉｽﾞﾙ継手名称
      //2019.07.25 製作指示書書式変更_E
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

   //2019.07.25 製作指示書書式変更_S
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1061].SubString(1,4));  // A側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S4TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S4TugiteNM;
   //   }
   //
   //} else if ( P_Model == "CX-10D" ) {
   //   s_Text2 = Search_HD_MATERIAL(s_HEADS_DATA[1060].SubString(1,4));  // B側端管材質
   //   if ( s_Text2.TrimRight() == "ＣＡＣ７０３" || s_Text2.TrimRight() == "ＳＴＰＧ" ) {
   //      s_Text = "リング端管１ " + s_S4TugiteNM;
   //   } else  {
   //      s_Text = "リング端管２ " + s_S4TugiteNM;
   //   }
   //
   //} else if ( s_Text == "1" || s_Text == "3" ) {
   //   s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4端管種類
   //   s_Text = s_S4TankanNM + " " + s_S4TugiteNM;
   //
   //} else if ( s_Text == "2" ) {
   //   s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4端管種類
   //   s_Text = s_S4TankanNM + " " + s_S4TugiteNM + "ＷＦ";
   //
   //} else {
   //   // 2011.06.20 ﾉｽﾞﾙ規格変更
   //   //if ( s_HEADS_DATA[938].SubString(2,1) == "4" ) {  // フランジ付きノズル
   //   if ( s_HEADS_DATA[938].SubString(3,1) == "4" ) {  // フランジ付きノズル
   //   // ***********************
   //      s_Text = s_S4TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[938].TrimRight());
   //   } else {
   //      s_Text = s_S4TugiteNM;
   //   }
   //
   //}
   if ( s_Text == "1" || s_Text == "3" ) {
      s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4端管種類
      s_Text = s_S4TugiteNM + " " + s_S4TankanNM;

   } else if ( s_Text == "2" ) {
      s_S4TankanNM = Get_Tankan_DataA3("S4", pcHeadsData);  // S4端管種類
      s_Text = s_S4TugiteNM + " " + s_S4TankanNM + " ＷＦ";

   } else {
      if ( s_HEADS_DATA[938].SubString(3,1) == "4" ) {  // フランジ付きノズル
         s_Text = s_S4TugiteNM + " " + Get_Flange_DataA3(s_HEADS_DATA[938].TrimRight());
      } else {
         s_Text = s_S4TugiteNM;
      }

   }
   //2019.07.25 製作指示書書式変更_E

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
   // 『B024』 『B025』
   s_Text = s_HEADS_DATA[928].TrimRight();      // S3温度計種類

   //2019.07.25 製作指示書書式変更_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I型";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L型";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 製作指示書書式変更_E
      // 『B024』
      s_Text = AnsiString(s_HEADS_DATA[929].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[930].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1357].TrimRight()));

      //2019.07.25 製作指示書書式変更_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 製作指示書書式変更_E

      Excel_Cell_DataSet(19, 7, s_Text);
      G_Log_Text = "S3温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B025』
      //2019.07.25 製作指示書書式変更_S
      //if (s_HEADS_DATA[846].TrimRight() == "1"){    // Bｽﾁｰﾑ区分
      //   s_Text = "ｽﾁｰﾑ用";
      //2020.02.04 P_FL修正_S
      //if ( P_FL = 0 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      if ( P_FL == 0 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      //2020.02.04 P_FL修正_E
         // 斜行流 Bｽﾁｰﾑ区分
         s_Text = "ｽﾁｰﾑ用";
      //2020.02.04 P_FL修正_S
      //} else if ( P_FL = 1 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      } else if ( P_FL == 1 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      //2020.02.04 P_FL修正_E
         // 斜行流 Aｽﾁｰﾑ区分
         s_Text = "ｽﾁｰﾑ用";
      //2019.07.25 製作指示書書式変更_E
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
   // 『B026』『B027』
   s_Text = s_HEADS_DATA[943].TrimRight();      // S4温度計種類

   //2019.07.25 製作指示書書式変更_S
   //if (s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2){
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 11) {
      if ( s_Text.ToIntDef(0) == 1 || s_Text.ToIntDef(0) == 2 ||
           s_Text.ToIntDef(0) == 3 || s_Text.ToIntDef(0) == 4 ||
           s_Text.ToIntDef(0) == 5 || s_Text.ToIntDef(0) == 8 ||
           s_Text.ToIntDef(0) == 9 || s_Text.ToIntDef(0) == 10  ) {
         s_Text2 = " I型";
      } else if ( s_Text.ToIntDef(0) == 6 || s_Text.ToIntDef(0) == 11 ) {
         s_Text2 = " L型";
      } else {
         s_Text2 = "";
      }
   //2019.07.25 製作指示書書式変更_E
      // 『B026』
      s_Text = AnsiString(s_HEADS_DATA[944].ToIntDef(0)) + "/"
             + AnsiString(s_HEADS_DATA[945].ToIntDef(0)) + " "
             + ZenToHan(Search_HD_MATERIAL( s_HEADS_DATA[1358].TrimRight()));

      //2019.07.25 製作指示書書式変更_S
      s_Text = s_Text + s_Text2;
      //2019.07.25 製作指示書書式変更_E

      Excel_Cell_DataSet(19, 15, s_Text);
      G_Log_Text = "S4温度計             『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B027』
      //2019.07.25 製作指示書書式変更_S
      //if (s_HEADS_DATA[847].TrimRight() == "1"){    // Aｽﾁｰﾑ区分
      //   s_Text = "ｽﾁｰﾑ用";
      //2020.02.04 P_FL修正_S
      //if ( P_FL = 0 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      if ( P_FL == 0 && s_HEADS_DATA[847].TrimRight() == "1" ) {
      //2020.02.04 P_FL修正_E
         // 台形流 Aｽﾁｰﾑ区分
         s_Text = "ｽﾁｰﾑ用";
      //2020.02.04 P_FL修正_S
      //} else if ( P_FL = 1 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      } else if ( P_FL == 1 && s_HEADS_DATA[846].TrimRight() == "1" ) {
      //2020.02.04 P_FL修正_E
         // 斜行流 Bｽﾁｰﾑ区分
         s_Text = "ｽﾁｰﾑ用";
      //2019.07.25 製作指示書書式変更_E
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

   // 2020.07.17 圧力計変更_S
   ////2019.07.25 製作指示書書式変更_S
   //////2004.05.19 圧力計条件追加
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   ////2019.07.25 製作指示書書式変更_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 圧力計変更_E
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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[931].TrimRight()) + " ";
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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[931].TrimRight()) + " ";
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

   // 2020.07.17 圧力計変更_S
   ////2019.07.25 製作指示書書式変更_S
   //////2004.05.19 圧力計条件追加
   //////if  (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13){
   ////if  ( (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 13) || (s_Text.ToIntDef(0) >= 18 && s_Text.ToIntDef(0) <= 20) ) {
   //////*************************
   //if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 20) {
   ////2019.07.25 製作指示書書式変更_E
   if (s_Text.ToIntDef(0) > 0 && s_Text.ToIntDef(0) <= 26) {
   // 2020.07.17 圧力計変更_E
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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[946].TrimRight()) + " ";
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
            s_Text = Get_AturyokuKeiA3(s_HEADS_DATA[946].TrimRight()) + " ";
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
         s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[1060].TrimRight());
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
   //2019.07.25 製作指示書書式変更_S
   // CX-10DではS1,S4(B側)にﾘﾝｸﾞ取付
   else if ( P_FL == 1 ) {
      // 斜行流ではS1,S4(B側)に端管取付
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //2019.07.25 製作指示書書式変更_E
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
         s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[1061].TrimRight());
         break;
      default:
         s_Text = "";
         break;
   }

   //2008.07.24 CX-10追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10" ) {
      s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[1061].TrimRight());
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
      //2019.07.25 製作指示書書式変更_S
      //if (s_Text != ""){
      //   s_Text = s_Text + " (S2,S3)";
      //}
      // CX-10DではS2,S3(A側)にﾘﾝｸﾞは付かない
      s_Text = "";
      //2019.07.25 製作指示書書式変更_E
   }
   //*********************
   //2019.07.25 製作指示書書式変更_S
   else if ( P_FL == 1 ) {
      // 斜行流ではS2,S3(A側)に端管取付
      if (s_Text != ""){
         s_Text = s_Text + " (S2,S3)";
      }
   }
   //2019.07.25 製作指示書書式変更_E

   Excel_Cell_DataSet(22, 15, s_Text);
   G_Log_Text = "A側端管材質          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B034』
   i_RecNo = 1136;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();     // S1端管
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_S
   i_RecNo = 1138;
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();    // S3端管
   i_RecNo = 1139;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();    // S4端管
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_E
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
         s_Text = Get_SheetGasketMatA3("B", pcHeadsData);  // B側ｼｰﾄｶﾞｽｹｯﾄ材質
         //2017.10.06 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E

      } else {
         s_Text = "";
      }
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_S
   } else if ( P_FL == 1 && (s_Text2 == "1" || s_Text2 == "2" || s_Text2 == "3") ) {
      // 斜行流(S4端管)
      i_RecNo = 938;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S4ﾉｽﾞﾙ継手
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("B", pcHeadsData);  // B側ｼｰﾄｶﾞｽｹｯﾄ材質
      } else {
         s_Text = "";
      }
   } else if ( P_FL != 1 && (s_Text1 == "1" || s_Text1 == "2" || s_Text1 == "3") ) {
      // 斜行流以外(S3端管)
      i_RecNo = 923;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S3ﾉｽﾞﾙ継手
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("B", pcHeadsData);  // B側ｼｰﾄｶﾞｽｹｯﾄ材質
      } else {
         s_Text = "";
      }
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_E
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
   // 2022.04.21 CX-01D追加_S
   // 2023.02.16 リング端管条件変更_S
   //else if ( P_Model == "CX-01D" ) {
   else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
   // 2023.02.16 リング端管条件変更_E
      if (s_Text != ""){
         s_Text = s_Text + " (S1)";
      }
   }
   // 2022.04.21 CX-01D追加_E
   //2019.07.25 製作指示書書式変更_S
   // CX-10DではS1,S4(B側)にﾘﾝｸﾞ取付
   else if ( P_FL == 1 ) {
      // 斜行流ではS1,S4(B側)に端管取付
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //2019.07.25 製作指示書書式変更_E
   Excel_Cell_DataSet(23, 7, s_Text);
   G_Log_Text = "B側ｶﾞｽｹｯﾄ材質        『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B035』
   i_RecNo = 1137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();     // S2端管
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_S
   i_RecNo = 1138;
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();    // S3端管
   i_RecNo = 1139;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();    // S4端管
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_E
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
         s_Text = Get_SheetGasketMatA3("A", pcHeadsData);  // A側ｼｰﾄｶﾞｽｹｯﾄ材質
         //2017.10.06 ｼｰﾄｶﾞｽｹｯﾄ材質変更_E

      } else {
         s_Text = "";
      }
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_S
   } else if ( P_FL == 1 && (s_Text1 == "1" || s_Text1 == "2" || s_Text1 == "3") ) {
      // 斜行流(S3端管)
      i_RecNo = 923;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S3ﾉｽﾞﾙ継手
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("A", pcHeadsData);  // A側ｼｰﾄｶﾞｽｹｯﾄ材質
      } else {
         s_Text = "";
      }
   } else if ( P_FL != 1 && (s_Text2 == "1" || s_Text2 == "2" || s_Text2 == "3") ) {
      // 斜行流以外(S4端管)
      i_RecNo = 938;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S4ﾉｽﾞﾙ継手
      if (s_Text.SubString(3,1) == "2" || s_Text.SubString(3,1) == "3"){
         s_Text = Get_SheetGasketMatA3("A", pcHeadsData);  // A側ｼｰﾄｶﾞｽｹｯﾄ材質
      } else {
         s_Text = "";
      }
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_E
   } else {
      s_Text = "";
   }

   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_S
   ////2008.07.24 CX-10追加
   //P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   //if (P_Model == "CX-10" ) {
   //   i_RecNo = 938;
   //   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // S4ﾉｽﾞﾙ継手
   //   if (s_Text.SubString(2,1) == "2" || s_Text.SubString(2,1) == "3"){
   //      // AｶﾞｽｹｯﾄCODEで検索
   //      i_RecNo = 836;
   //      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //      s_Text = Chg_Zaisitu_Code(s_Text,"1");
   //      s_Text = Get_Zaisitu_NameA3(s_Text);
   //
   //   } else {
   //      s_Text = "";
   //   }
   ////2008.08.07 CX-10以外でも出力する
   ////} else {
   ////   s_Text = "";
   ////********************************
   //}
   ////*******************
   // 2020.12.21 ｼ-ﾄｶﾞｽｹｯﾄ変更_E
   //2010.06.07 CX-10D追加
   P_Model = ZenToHan(G_KS_Syurui) + "-" + ZenToHan(G_KS_Model);
   if (P_Model == "CX-10D" ) {
      //2019.07.25 製作指示書書式変更_S
      //if (s_Text != ""){
      //   s_Text = s_Text + " (S1,S4)";
      //}
      // CX-10DではS2,S3(A側)にﾘﾝｸﾞは付かない
      s_Text = "";
      //2019.07.25 製作指示書書式変更_E
   }
   //*********************
   // 2022.04.21 CX-01D追加_S
   // 2023.02.16 リング端管条件変更_S
   //else if ( P_Model == "CX-01D" ) {
   else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
   // 2023.02.16 リング端管条件変更_E
      if (s_Text != ""){
         s_Text = s_Text + " (S3)";
      }
   }
   // 2022.04.21 CX-01D追加_E
   //2019.07.25 製作指示書書式変更_S
   else if ( P_FL == 1 ) {
      // 斜行流ではS2,S3(A側)に端管取付
      if (s_Text != ""){
         s_Text = s_Text + " (S2,S3)";
      }
   }
   //2019.07.25 製作指示書書式変更_E
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
      s_Text = Get_Zaisitu_NameA3(s_Text);

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
      // 2023.02.16 リング端管条件変更_S
      //// 2021.01.06 CX(W)-03追加(CX(W)-00→CX(W)-01)_S
      //////2016.04.22 CX-00追加
      ////} else if ( P_Model == "CX-00" ) {
      ////   s_Text = "";
      ////} else if ( P_Model == "CXW-00" ) {
      ////   s_Text = "";
      //////********************
      //} else if ( P_Model == "CX-01" ) {
      //   s_Text = "";
      //} else if ( P_Model == "CXW-01" ) {
      //   s_Text = "";
      //// 2021.01.06 CX(W)-03追加(CX(W)-00→CX(W)-01)_E
      //// 2022.04.21 CX-01D追加_S
      //} else if ( P_Model == "CX-01D" ) {
      //   s_Text = "";
      //// 2022.04.21 CX-01D追加_E
      } else if ( P_Model == "CX-01" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
         s_Text = "";
      } else if ( P_Model == "CXW-01" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
         s_Text = "";
      } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
         s_Text = "";
      // 2023.02.16 リング端管条件変更_E
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
         // 2020.09.01 舶用ﾘﾝｸﾞ変更_S
         ////2019.07.25 製作指示書書式変更_S
         ////s_Text = "相ﾌﾗﾝｼﾞ  SS40  W.F";
         //s_Text = "相ﾌﾗﾝｼﾞ  SS400  W.F";
         ////2019.07.25 製作指示書書式変更_E
         if (P_Model == "UX-10" && s_HEADS_DATA[1060].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         } else if ( P_Model == "LX-00" && s_HEADS_DATA[1060].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         }
         // 2020.09.01 舶用ﾘﾝｸﾞ変更_E
         // 2020.10.30 舶用ﾘﾝｸﾞ変更_S
         else {
             s_Text = "";
         }
         // 2020.10.30 舶用ﾘﾝｸﾞ変更_E
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

   //2019.07.25 製作指示書書式変更_S
   if ( P_FL == 1 ) {
      // 斜行流ではS1,S4(B側)にフランジ取付
      if (s_Text != ""){
         s_Text = s_Text + " (S1,S4)";
      }
   }
   //2019.07.25 製作指示書書式変更_E

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
      s_Text = Get_Zaisitu_NameA3(s_Text);

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
      // 2022.04.21 CX-01D追加_S
      // 2023.02.16 リング端管条件変更_S
      //} else if ( P_Model == "CX-01D" ) {
      } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0 ) {
      // 2023.02.16 リング端管条件変更_E
         s_Text = "";
      // 2022.04.21 CX-01D追加_E
      } else if (s_Text == "3"){
      //if (s_Text == "3"){
      //*******************
         // 2020.09.01 舶用ﾘﾝｸﾞ変更_S
         ////2019.07.25 製作指示書書式変更_S
         ////s_Text = "相ﾌﾗﾝｼﾞ  SS40  W.F";
         //s_Text = "相ﾌﾗﾝｼﾞ  SS400  W.F";
         ////2019.07.25 製作指示書書式変更_E
         if (P_Model == "UX-10" && s_HEADS_DATA[1061].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         } else if ( P_Model == "LX-00" && s_HEADS_DATA[1061].TrimRight() == "1060" ) {
             s_Text = "SS400  W.F";
         }
         // 2020.09.01 舶用ﾘﾝｸﾞ変更_E
         // 2020.10.30 舶用ﾘﾝｸﾞ変更_S
         else {
             s_Text = "";
         }
         // 2020.10.30 舶用ﾘﾝｸﾞ変更_E
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

   //2019.07.25 製作指示書書式変更_S
   if ( P_FL == 1 ) {
      // 斜行流ではS2,S3(A側)にフランジ取付
      if (s_Text != ""){
         s_Text = s_Text + " (S2,S3)";
      }
   }
   //2019.07.25 製作指示書書式変更_E

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
      Excel_Cell_DataSet(26, 2, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   //2010.06.07 CX-10D追加
   } else if ( P_Model == "CX-10D" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色 (S1,S4)";
      } else {
        s_Text = "内外  酸洗 (S1,S4)";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   //*********************

   // 2023.02.16 リング端管条件変更_S
   ////2016.05.19 CX-00追加
   //// 2021.01.06 CX-03追加(CX-00→CX-01)_S
   ////} else if ( P_Model == "CX-00" ) {
   //} else if ( P_Model == "CX-01" ) {
   //// 2021.01.06 CX-03追加(CX-00→CX-01)_E
   } else if ( P_Model == "CX-01" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 リング端管条件変更_E
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   // 2023.02.16 リング端管条件変更_S
   ////2021.01.06 CXW-03追加(CXW-00→CXW-01)_S
   ////} else if ( P_Model == "CXW-00" ) {
   //} else if ( P_Model == "CXW-01" ) {
   ////2021.01.06 CXW-03追加(CXW-00→CXW-01)_E
   } else if ( P_Model == "CXW-01" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 リング端管条件変更_E
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   //********************

   // 2023.02.16 リング端管条件変更_S
   // 2022.04.21 CX-01D追加_S
   //} else if ( P_Model == "CX-01D" ) {
   } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 リング端管条件変更_E
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色 (S1)";
      } else {
        s_Text = "内外  酸洗 (S1)";
      }
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // 2022.04.21 CX-01D追加_E

   // 2013.04.03 SX-30追加
   } else if ( P_Model == "SX-30" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // ********************

    // 2016.07.22 SX-30S加
   } else if ( P_Model == "SX-30S" && s_S1TugiteNM.SubString(1,12) == "ＪＩＳ３０Ｋ" ) {
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // ********************

   } else if (s_Text == 1 || s_Text == 2){
   //if (s_Text == 1 || s_Text == 2){
   //********************
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
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
      // 2021.03.23 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.23 ﾆｯｹﾙ追加_E
      } else {
        s_Text = "内外  酸洗";
        //2019.07.25 製作指示書書式変更_S
        if ( P_FL == 1 ) {
           // 斜行流ではS1,S4(B側)に端管取付
           s_Text = s_Text + " (S1,S4)";
        }
        //2019.07.25 製作指示書書式変更_E
      }
      // ***********************
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else if (s_Text == 3){
      // 『B038』
      s_Text = "端管仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      //2019.10.10 リング端管仕上変更_S
      //s_Text = "外面のみﾌﾚｰﾑと同一色";
      s_Text = s_HEADS_DATA[1060].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ALBCの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "STPG" ) { // STPGの場合は塗装
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else if ( s_Text == "SGP" ) {  // SGPの場合は塗装
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else if ( s_Text == "CS" ) {   // CSの場合は塗装
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {                         // その他の場合は仕上げ１
        s_Text = "内外  酸洗";
      }
      //2019.10.10 リング端管仕上変更_E
      //2019.07.25 製作指示書書式変更_S
      if ( P_FL == 1 ) {
         // 斜行流ではS1,S4(B側)に端管取付
         s_Text = s_Text + " (S1,S4)";
      }
      //2019.07.25 製作指示書書式変更_E
      Excel_Cell_DataSet(26, 7, s_Text);
      G_Log_Text = "B側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      // 『B038』
      s_Text = "ノズル仕上";
      Excel_Cell_DataSet(26, 2, s_Text);
      G_Log_Text = "仕上タイトル         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      // 『B039』
      i_RecNo = 904;   // S1ﾉｽﾞﾙ仕上
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text = Get_SiageMeisyouA3(s_Text);
      //2019.07.25 製作指示書書式変更_S
      if ( P_FL == 1 ) {
         // 斜行流ではS1,S4(B側)にノズル取付
         if (s_Text != "" && s_Text != "――――――"){
            s_Text = s_Text + " (S1,S4)";
         }
      }
      //2019.07.25 製作指示書書式変更_E
      Excel_Cell_DataSet(26, 7, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      // ***********************
      Excel_Cell_DataSet(26, 15, s_Text);
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
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色 (S2,S3)";
      } else {
        s_Text = "内外  酸洗 (S2,S3)";
      }
      // ***********************
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   //*********************

   // 2022.04.21 CX-01D追加_S
   // 2023.02.16 リング端管条件変更_S
   //} else if ( P_Model == "CX-01D" ) {
   } else if ( P_Model == "CX-01D" && s_HEADS_DATA[36].Pos("Y") == 0  ) {
   // 2023.02.16 リング端管条件変更_E
      // 『B040』
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色 (S3)";
      } else {
        s_Text = "内外  酸洗 (S3)";
      }
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // 2022.04.21 CX-01D追加_E

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
      // 2021.03.23 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.23 ﾆｯｹﾙ追加_E
      } else {
        s_Text = "内外  酸洗";
        //2019.07.25 製作指示書書式変更_S
        if ( P_FL == 1 ) {
           // 斜行流ではS2,S3(A側)に端管取付
           s_Text = s_Text + " (S2,S3)";
        }
        //2019.07.25 製作指示書書式変更_E
      }
      // ***********************
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else if (s_Text == 3){
      // 『B040』
      //2019.10.28 リング端管仕上変更_S
      //s_Text = "外面のみﾌﾚｰﾑと同一色";
      s_Text = s_HEADS_DATA[1061].SubString(1,4);
      s_Text = Type_MAT(s_Text);
      if( s_Text == "Ti" ) {           // チタンの場合は仕上５
        s_Text = "――――――";
      } else if ( s_Text == "ALBC" ) { // ＡＬＢＣの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_S
      } else if ( s_Text == "NI" ) {   // ニッケルの場合は仕上５
        s_Text = "――――――";
      // 2021.03.10 ﾆｯｹﾙ追加_E
      } else if ( s_Text == "-" ) {    // なし
        s_Text = "――――――";
      } else if ( s_Text == "STPG" ) { // ＳＴＰＧ
        s_Text = "外面のみﾌﾚｰﾑと同一色";
      } else {
        s_Text = "内外  酸洗";
      }
      //2019.10.10 リング端管仕上変更_E
      //2019.07.25 製作指示書書式変更_S
      if ( P_FL == 1 ) {
         // 斜行流ではS2,S3(A側)に端管取付
         s_Text = s_Text + " (S2,S3)";
      }
      //2019.07.25 製作指示書書式変更_E
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {
      // 『B040』
      i_RecNo = 919;   // S2ﾉｽﾞﾙ仕上
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      s_Text = Get_SiageMeisyouA3(s_Text);
      //2019.07.25 製作指示書書式変更_S
      if ( P_FL == 1 ) {
         // 斜行流ではS2,S3(A側)にノズル取付
         if (s_Text != "" && s_Text != "――――――"){
            s_Text = s_Text + " (S2,S3)";
         }
      }
      //2019.07.25 製作指示書書式変更_E
      Excel_Cell_DataSet(26, 15, s_Text);
      G_Log_Text = "A側仕上名称          『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  A側保護官材質
   //***************************
   // 『B041』
   //2019.07.25 製作指示書書式変更_S
   //i_RecNo = 1357;   // A側保護官材質
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //s_Text = Get_Zaisitu_NameA3(s_Text);
   //Excel_Cell_DataSet(25, 7, s_Text);
   //G_Log_Text = "A側保護官材質          『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   i_RecNo = 141;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // A温度計数量

   // 2020.10.30 温度計保護管修正_S
   s_S1OndoCode = s_HEADS_DATA[898].TrimRight();  // S1温度計種類
   s_S2OndoCode = s_HEADS_DATA[913].TrimRight();  // S2温度計種類
   s_S3OndoCode = s_HEADS_DATA[928].TrimRight();  // S3温度計種類
   s_S4OndoCode = s_HEADS_DATA[943].TrimRight();  // S4温度計種類

   if ( P_FL == 1 ) {
      // 斜行流ではS2,S3がA側
      if (s_S2OndoCode.ToIntDef(0) == 0 && s_S3OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // A側に温度計は付かない
      }
   } else {
      // 通常は S2,S4がA側
      if (s_S2OndoCode.ToIntDef(0) == 0 && s_S4OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // A側に温度計は付かない
      }
   }
   // 2020.10.30 温度計保護管修正_E

   if (s_Text.ToIntDef(0) != 0){
      if (Type_MAT(s_HEADS_DATA[1358].TrimRight()) != "-") {
         s_Text = s_HEADS_DATA[1358].SubString(1,4);
         s_Text = Get_Zaisitu_NameA3(s_Text);
         //2019.10.10 温度計保護管寸法追加_S
         //if ( P_FL == 1 ) {
         //   // 斜行流ではS2,S3(A側)に保護管取付
         //   s_Text = s_Text + " (S2,S3)";
         //}
         //2019.10.10 温度計保護管寸法追加_E
      } else {
         s_Text = "";
      }

      //2019.10.10 温度計保護管寸法追加_S
      s_Text2 = s_HEADS_DATA[913].TrimRight();  // S2温度計種類
      s_Text3 = s_HEADS_DATA[908].TrimRight();  // S2ノズル規格コード
      s_Text4 = s_HEADS_DATA[907].TrimRight();  // S2ノズル口径
      s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);

      if (s_Text2 == "") {
         if ( P_FL == 1 ) {
            // 斜行流
            s_Text2 = s_HEADS_DATA[928].TrimRight();  // S3温度計種類
            s_Text3 = s_HEADS_DATA[923].TrimRight();  // S3ノズル規格コード
            s_Text4 = s_HEADS_DATA[922].TrimRight();  // S3ノズル口径
            s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);
         } else {
            s_Text2 = s_HEADS_DATA[943].TrimRight();  // S4温度計種類
            s_Text3 = s_HEADS_DATA[938].TrimRight();  // S4ノズル規格コード
            s_Text4 = s_HEADS_DATA[937].TrimRight();  // S4ノズル口径
            s_Text2 = Get_Well_Temp(s_Text, s_Text3, s_Text4);
         }
      }

      s_Text = s_Text + s_Text2;

      if ( P_FL == 1 ) {
         // 斜行流ではS2,S3(A側)に保護管取付
         s_Text = s_Text + " (S2,S3)";
      }
      //2019.10.10 温度計保護管寸法追加_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(25, 15, s_Text);
   G_Log_Text = "A側温度計保護管材質  『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //2019.07.25 製作指示書書式変更_E

   //***************************
   //***  B側保護官材質
   //***************************
   // 『B042』
   //2019.07.25 製作指示書書式変更_S
   //i_RecNo = 1358;   // B側保護官材質
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //s_Text = Get_Zaisitu_NameA3(s_Text);
   //Excel_Cell_DataSet(25, 15, s_Text);
   //G_Log_Text = "B側保護官材質          『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   i_RecNo = 140;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();  // B温度計数量

   // 2020.10.30 温度計保護管修正_S
   if ( P_FL == 1 ) {
      // 斜行流ではS1,S4がB側
      if (s_S1OndoCode.ToIntDef(0) == 0 && s_S4OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // B側に温度計は付かない
      }
   } else {
      // 通常は S1,S3がB側
      if (s_S1OndoCode.ToIntDef(0) == 0 && s_S3OndoCode.ToIntDef(0) == 0){
         s_Text = "0";  // B側に温度計は付かない
      }
   }
   // 2020.10.30 温度計保護管修正_E

   if (s_Text.ToIntDef(0) != 0){
      if (Type_MAT(s_HEADS_DATA[1357].TrimRight()) != "-") {
         s_Text = s_HEADS_DATA[1357].SubString(1,4);
         s_Text = Get_Zaisitu_NameA3(s_Text);
         //2019.10.10 温度計保護管寸法追加_S
         //if ( P_FL == 1 ) {
         //   // 斜行流ではS1,S4(B側)にフランジ取付
         //   s_Text = s_Text + " (S1,S4)";
         //}
         //2019.10.10 温度計保護管寸法追加_E
      } else {
         s_Text = "";
      }

      //2019.10.10 温度計保護管寸法追加_S
      // B側保護官寸法
      s_Text2 = s_HEADS_DATA[898].TrimRight();  // S1温度計種類
      s_Text3 = s_HEADS_DATA[893].TrimRight();  // S1ノズル規格コード
      s_Text4 = s_HEADS_DATA[892].TrimRight();  // S1ノズル口径
      s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);
      if (s_Text2 == "") {
         if ( P_FL == 1 ) {
            // 斜行流
            s_Text2 = s_HEADS_DATA[943].TrimRight();      // S4温度計種類
            s_Text3 = s_HEADS_DATA[938].TrimRight();  // S4ノズル規格コード
            s_Text4 = s_HEADS_DATA[937].TrimRight();  // S4ノズル口径
            s_Text2 = Get_Well_Temp(s_Text, s_Text3, s_Text4);
         } else {
            // 台形流
            s_Text2 = s_HEADS_DATA[928].TrimRight();      // S3温度計種類
            s_Text3 = s_HEADS_DATA[923].TrimRight();  // S3ノズル規格コード
            s_Text4 = s_HEADS_DATA[922].TrimRight();  // S3ノズル口径
            s_Text2 = Get_Well_Temp(s_Text2, s_Text3, s_Text4);
         }
      }

      s_Text = s_Text + s_Text2;

      if ( P_FL == 1 ) {
         // 斜行流ではS1,S4(B側)にフランジ取付
         s_Text = s_Text + " (S1,S4)";
      }
      //2019.10.10 温度計保護管寸法追加_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(25, 7, s_Text);
   G_Log_Text = "B側温度計保護管材質  『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //2019.07.25 製作指示書書式変更_E

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

   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   s_Text1 = G_KS_Syurui + "－" + G_KS_Model;
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_E

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
      // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   } else if (( s_Text == "A157" ) && ( s_Text1 == "ＬＸ－３０")) {
      // Q235B2: S/L Q235B 小径 45/35 CODE A157
         s_Text = "Q235B";                         // 締付ボルト材質
   } else if (( s_Text == "A163" ) && ( s_Text1 == "ＬＸ－５０")) {
      // 45_2  : S/L 45/35 小径 Q235B CODE A163
         s_Text = "45/35";                            // 締付ボルト材質
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_E
   //if (( s_Text != "" ) && ( s_Text != "0000" ) ){
   } else if (( s_Text != "" ) && ( s_Text != "0000" ) ){
      s_Text  = Search_HD_MATERIAL(s_Text);
   } else {
      i_RecNo = 1021;   // BOL22
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }

   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   if (( s_Text == "Q235B2" ) && ( s_Text1 == "ＬＸ－３０")) {
      // Q235B2: S/L Q235B 小径 45/35 CODE A157
      s_Text = "Q235B";                            // 締付ボルト材質
   } else if (( s_Text == "45_2" ) && ( s_Text1 == "ＬＸ－５０")) {
      // 45_2  : S/L 45/35 小径 Q235B CODE A163
      s_Text = "45/35";                               // 締付ボルト材質
   }
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_E

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

   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   sBltMat = s_Text; // 締付ボルト材質
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_E

   //2009.04.27 ﾐﾙｼｰﾄ注記
   i_RecNo = 435;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
   } else {
      s_Text = s_Text + " ﾐﾙ";
   }
   //********************

   Excel_Cell_DataSet(27, 8, s_Text);
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

      //2019.07.25 製作指示書書式変更_S
      if (P_HEADS_DATA[36].Pos("M") != 0){
         s_Text =  s_Text + "  Ｗナット";
      }
      //2019.07.25 製作指示書書式変更_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(27, 12, s_Text);
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
   Excel_Cell_DataSet(27, 24, s_Text);
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
   //2020.10.02 ﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上追加_S
   } else if (s_Text == "07"){
      i_RecNo = 263;   // ﾎﾞﾙﾄｶﾊﾞｰ有無
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text == "1" || s_Text == "1.00") {
         s_Text = "ｸﾞﾘｽｱｯﾌﾟ(ﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上)";
      } else {
         s_Text = "締付ﾎﾞﾙﾄのﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上";
      }
   } else if (s_Text == "08"){
      i_RecNo = 263;   // ﾎﾞﾙﾄｶﾊﾞｰ有無
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text == "1" || s_Text == "1.00") {
         s_Text = "ｸﾞﾘｽｱｯﾌﾟ(ﾅｯﾄﾒｯｷ仕上)";
      } else {
         s_Text = "締付ﾎﾞﾙﾄのﾅｯﾄﾒｯｷ仕上";
      }
   //2020.10.02 ﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上追加_E
   } else {
      s_Text = "";
      //2020.10.02 ﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上追加_S
      i_RecNo = 263;   // ﾎﾞﾙﾄｶﾊﾞｰ有無
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text == "1" || s_Text == "1.00") {
         s_Text = "ｸﾞﾘｽｱｯﾌﾟ";
         //2022.11.15 SUSボルト修正_S
         i_RecNo = 1041;   // 材質指定
         s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
         if (s_Text2 == "31C9" || s_Text2 == "3101" || s_Text2 == "1101" ) {
            s_Text = "";
         }
         //2022.11.15 SUSボルト修正_E
      }
      //2020.10.02 ﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上追加_E
      //2020.10.16 ﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上修正_S
      else {
         s_Text = "";
      }
      //2020.10.16 ﾅｯﾄ,ﾜｯｼｬﾒｯｷ仕上修正_E
   }

   Excel_Cell_DataSet(28, 7, s_Text);
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

      //2019.07.25 製作指示書書式変更_S
      if (P_HEADS_DATA[36].Pos("M") != 0){
         s_Text =  s_Text + "  Ｗナット";
      }
      //2019.07.25 製作指示書書式変更_E

   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(28, 12, s_Text);
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
   Excel_Cell_DataSet(28, 24, s_Text);
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
   //2019.07.25 製作指示書書式変更_S
   } else if (s_Text == "ＬＸ－５０" || s_Text == "ＬＸ－５０Ｓ"){
           i_OgataFlg = 1;
   //2019.07.25 製作指示書書式変更_E
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

   //***************************
   //***  Sﾌﾚｰﾑ孔明
   //***************************
   // 『C029』
   // 各取得項目結合
   s_Text = "S" + s_S[1] + s_S[2] + s_S[3] + s_S[4];

   Excel_Cell_DataSet(33, 1, s_Text);
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
            s_Text = Get_Zaisitu_NameA3(s_HEADS_DATA[i_RecNo].TrimRight());
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

   Excel_Cell_DataSet(32, 8, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ材質 材質         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ材質 厚さ
   //***************************
   // 『C008』
   i_RecNo = 1284;   // ﾌﾚｰﾑ厚
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(32, 15, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ材質 厚さ         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ材質 幅
   //***************************
   // 『C009』
   i_RecNo = 1285;   // ﾌﾚｰﾑ幅
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(32, 22, s_Text);
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
     Excel_Cell_DataSet(34, 10, s_Text);
   // 『C013』
     Excel_Cell_DataSet(34, 7, "LU1/3 =");
     G_Log_Text = "上部ＬＵ              RecNo:1022/1378『" + s_Text + "』を上書セット。";
     Write_Log(G_Log_Text);
   } else {
     i_RecNo = 1023;   // RLU2
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(34, 10, s_Text);
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
     Excel_Cell_DataSet(34, 12, s_Text);
   // 『C014』
     Excel_Cell_DataSet(34, 11, "LL1/3 =");
     G_Log_Text = "下部ＬＬ              RecNo:1024/1379『" + s_Text + "』を上書セット。";
     Write_Log(G_Log_Text);
   } else {
     i_RecNo = 1025;   // RLL2
     s_Text = s_Text + "/" + s_HEADS_DATA[i_RecNo].TrimRight();
     Excel_Cell_DataSet(34, 12, s_Text);
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
   Excel_Cell_DataSet(34, 16, s_Text);
   G_Log_Text = "ｶﾞｲﾄﾞﾊﾞｰ種類          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  Ｃ
   //***************************
   // 『C015』
   i_RecNo = 1026;   // CMAX
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 18, s_Text);
   G_Log_Text = "Ｃ                    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  Ｐ
   //***************************
   // 『C016』
   i_RecNo = 1028;   // P
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 24, s_Text);
   G_Log_Text = "Ｐ                    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  レールセンター
   //***************************
   // 『C017』
   s_Text2 = G_KS_Syurui + "－" + G_KS_Model;
   if ( s_Text2 == "ＬＸ－３０" ) {                            // ＬＸ－３０
       s_Text = "２ｍｍ以内";
   } else {                                                    // その他型式
       s_Text = "３ｍｍ以内";
   }
   Excel_Cell_DataSet(35, 7, s_Text);
   G_Log_Text = "レールセンター        『" + s_Text + "』を上書セット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ボルトカバー
   //***************************
   // 『C018』  2003/06/04 文言はHEADSテキストの内容を無条件で出力
   i_RecNo = 1367;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(29, 7, s_Text);
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
         Excel_Cell_DataSet(29, 10, s_Text);
         G_Log_Text = "BLCP1                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCP1本数 『C020』
         s_Text = AnsiString(i_Value_1 + i_Value_2);
         Excel_Cell_DataSet(29, 24, s_Text);
         G_Log_Text = "BLCP1 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(29, 10, s_Text);
         Excel_Cell_DataSet(29, 24, s_Text);
         G_Log_Text = "BLCP1                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCP1 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(29, 10, s_Text);
      Excel_Cell_DataSet(29, 24, s_Text);
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
         Excel_Cell_DataSet(30, 10, s_Text);
         G_Log_Text = "BLCEL                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCEL本数 『C022』
         s_Text = AnsiString(i_Value_1);
         Excel_Cell_DataSet(30, 24, s_Text);
         G_Log_Text = "BLCEL 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(30, 10, s_Text);
         Excel_Cell_DataSet(30, 24, s_Text);
         G_Log_Text = "BLCEL                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCEL 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(30, 10, s_Text);
      Excel_Cell_DataSet(30, 24, s_Text);
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
         Excel_Cell_DataSet(31, 10, s_Text);
         G_Log_Text = "BLCES                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCEL本数 『C024』
         s_Text = AnsiString(i_Value_2);
         Excel_Cell_DataSet(31, 24, s_Text);
         G_Log_Text = "BLCES 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      } else {
         s_Text = "";
         Excel_Cell_DataSet(31, 10, s_Text);
         Excel_Cell_DataSet(31, 24, s_Text);
         G_Log_Text = "BLCES                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
         G_Log_Text = "BLCES 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   } else {
      s_Text = "";
      Excel_Cell_DataSet(31, 10, s_Text);
      Excel_Cell_DataSet(31, 24, s_Text);
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
   // 2021.04.07 ダブルナット追加_S
   } else if (s_Text == "11") {
      s_Text = "アンカーボルト・ダブルナット（１種×１個、３種×１個）";
   } else if (s_Text == "13") {
      s_Text = "ケミカル・アンカー・ダブルナット（１種×１個、３種×１個）";
   } else if (s_Text == "15") {
      s_Text = "セットボルト・ダブルナット（１種×１個、３種×１個）";
   // 2021.04.07 ダブルナット追加_E
   } else {
      s_Text = "";
   }

   Excel_Cell_DataSet(36, 8, s_Text);
   G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄｺｰﾄﾞ          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if ( s_Text == "アンカーボルト不要" || s_Text == "" ) {
      Excel_Cell_DataSet(37, 8, "");
      Excel_Cell_DataSet(37, 15, "");
      Excel_Cell_DataSet(37, 23, "");
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

      // 2021.04.07 ダブルナット追加_S
      if ( s_HEADS_DATA[1045].TrimRight() == "13" ) {
         // ケミカルアンカー・ダブルナット
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
      // 2021.04.07 ダブルナット追加_E

      //2018.08.08 アンカー変更_S
      s_Text2 = wkQuery->FieldByName("NUM")->AsString.TrimRight();
      s_Text = s_Text + " ／ " + s_Text2 + "本";
      //2018.08.08 アンカー変更_E

      //2018.08.08 アンカー変更_S
      //Excel_Cell_DataSet(34, 8, s_Text);
      Excel_Cell_DataSet(37, 8, s_Text);
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
        // 2022.06.10 ダブルナット追加_S
        } else if (s_Text == "11" || s_Text == "13" || s_Text == "15"){
            switch (s_HEADS_DATA[37].ToIntDef(0)) {
              case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
                 s_Text = "ＳＵＳ３０４";
                 break;
              default :
                 if (s_HEADS_DATA[1048].TrimRight() == "1001"){
                    s_Text = "ＳＵＳ３０４";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "30C9"){
                    s_Text = "ＳＳ３０４";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "31C9"){
                    s_Text = "ＳＵＳ３０４";
                 } else if (s_HEADS_DATA[1048].TrimRight() == "32C9"){
                    s_Text = "ＳＡ－１９３－Ｇｒ．Ｂ８";
                 } else {
                    s_Text = "ＳＳ４００";
                 }
                 break;
           }
        // 2022.06.10 ダブルナット追加_E
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

      Excel_Cell_DataSet(37, 15, s_Text);
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
      Excel_Cell_DataSet(37, 23, s_Text);
      G_Log_Text = "ｱﾝｶｰﾎﾞﾙﾄ仕上          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   }

   //2011.03.11 工程写真要注記追加
   //*******************
   //*** 注記(工程写真要)
   //*******************
   // 『C030』
   i_RecNo = 1102;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(40, 18, s_Text);
   G_Log_Text = "注記(工程写真要)     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*****************************

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
   // 2020.10.30 船番追加_S
   //i_RecNo = 28;    // 工事番号
   if(s_HEADS_DATA[36].Pos("M") != 0)  {
      // 舶用の場合
      i_RecNo = 270;   // 船番
   } else {
      // 舶用でない場合
      i_RecNo = 28;    // 工事番号
     Write_Log("工事番号");
   }
   // 2020.10.30 船番追加_E
   s_Text = ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   Excel_Cell_DataSet(42, 9, s_Text);
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

   // 2022.04.04 機器番号機器名称特記追加_S
   if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
      // 機器番号特記項目有
      // 機器番号欄外注記に記述
      s_Text = "※を参照";
   }
   // 2022.04.04 機器番号機器名称特記追加_E

   Excel_Cell_DataSet(42, 18, s_Text);
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

   Excel_Cell_DataSet(44, 6, s_Text);
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

   Excel_Cell_DataSet(45, 6, s_Text);
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

   Excel_Cell_DataSet(46, 6, s_Text);
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

   Excel_Cell_DataSet(47, 6, s_Text);
   G_Log_Text = "B試験圧力 水圧        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.07.15 国交省銘板追加
   //***************************
   //***  B側流量
   //***************************
   if (s_HEADS_DATA[268].TrimRight() == "1") { // 国交省銘板
      s_Text = "流量";
   // 『D018』
      Excel_Cell_DataSet(48, 4, s_Text);

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
   // 『D019』
   Excel_Cell_DataSet(48, 6, s_Text);
   G_Log_Text = "B側流量               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[846].TrimRight() == "1") { // Ｂ側スチーム
      s_Text = "kg/min";
   } else {                                    // Ｂ側液
      s_Text = "lit/min";
   }
   // 『D020』
   Excel_Cell_DataSet(48, 11, s_Text);
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

   Excel_Cell_DataSet(49, 6, s_Text);
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

   Excel_Cell_DataSet(50, 6, s_Text);
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

   Excel_Cell_DataSet(44, 14, s_Text);
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

   Excel_Cell_DataSet(45, 14, s_Text);
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

   Excel_Cell_DataSet(46, 14, s_Text);
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

   Excel_Cell_DataSet(47, 14, s_Text);
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
   // 『D021』
   Excel_Cell_DataSet(48, 14, s_Text);
   G_Log_Text = "A側流量               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[847].TrimRight() == "1") { // Ａ側スチーム
      s_Text = "kg/min";
   } else {                                    // Ａ側液
      s_Text = "lit/min";
   }
   // 『D022』
   Excel_Cell_DataSet(48, 24, s_Text);
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

   Excel_Cell_DataSet(49, 14, s_Text);
   G_Log_Text = "A側内容量             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器質量
   //***************************
   // 『D014』
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(50, 17, s_Text);
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
   Excel_Cell_DataSet(51, 6, s_Text);
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

   Excel_Cell_DataSet(52, 9, s_Text);
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

   Excel_Cell_DataSet(52, 15, s_Text);
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
   // 『D023』
      // 和文
       // 2020.10.30 ステッカー変更_S
      //Excel_Cell_DataSet(53, 18, "");
      //G_Log_Text = "ステッカー            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』を消去。";
      //Write_Log(G_Log_Text);
      if ( s_HEADS_DATA[36].Pos("M") != 0 ) {
         // 舶用の場合ステッカーは必ず付く
         Excel_Cell_DataSet(53, 24, "和文");
         G_Log_Text = "ステッカー            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         Excel_Cell_DataSet(53, 18, "");
         G_Log_Text = "ステッカー            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』を消去。";
         Write_Log(G_Log_Text);
      }
      // 2020.10.30 ステッカー変更_E
   } else {
   // 『D024』
      // 英文
      // 2020.10.30 ステッカー変更_S
      if ( s_HEADS_DATA[36].Pos("M") != 0 ) {
         // 舶用の場合ステッカーは必ず付く
         s_Text = "英文";
      }
      // 2020.10.30 ステッカー変更_E
      Excel_Cell_DataSet(53, 24, s_Text);
      G_Log_Text = "ステッカー            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  機械名称
   //***************************
   // 『D025』
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2019.12.27 製作指示書書式変更_S
   //      舶用でない場合
   if(s_HEADS_DATA[36].Pos("M") == 0)  {
       // 出力しない
       s_Text = "";
   }
   //2019.12.27 製作指示書書式変更_E

   if(s_Text != "")
   {
      Excel_Cell_DataSet(41, 9, s_Text);
   }

   // **************************

   //*************************

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          E欄　　データセット                        ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  アイプレート
   //***************************
   // 『E001』
   i = 0;

   i_Value_1 = s_HEADS_DATA[823].ToIntDef(0);
   i_Value_2 = s_HEADS_DATA[1011].ToIntDef(0);

   //①舶用でない場合(アイプレートなし)
   if(P_HEADS_DATA[36].Pos("M")==0){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   //②アイプレートが付かない型式の場合
   // 2021.01.06 CX-03追加(CX-00→CX-01)_S
   ////CX-00の場合
   //CX-01の場合
   // 2021.01.06 CX-03追加(CX-00→CX-01)_E
   else if (i_Value_1 == 14 && i_Value_2 == 0){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_S
   ////CXW-00の場合
   //CXW-01の場合
   // 2021.01.06 CXW-03追加(CXW-00→CXW-01)_S
   else if (i_Value_1 == 20 && i_Value_2 == 0){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   // 2021.01.06 CX-03追加_S
   //CX-03の場合
   else if (i_Value_1 == 14 && i_Value_2 == 3){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
   }
   //CXW-03の場合
   else if (i_Value_1 == 20 && i_Value_2 == 3){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
   }
   // 2021.01.06 CX-03追加_E
   //CX-10の場合
   else if (i_Value_1 == 14 && i_Value_2 == 10){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   //CX-10Dの場合
   else if (i_Value_1 == 15 && i_Value_2 == 10){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   //LX-00の場合
   else if (i_Value_1 == 3 && i_Value_2 == 0){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   //RX-00の場合
   else if (i_Value_1 == 8 && i_Value_2 == 0){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   //UX-01の場合
   else if (i_Value_1 == 2 && i_Value_2 == 0){
      //""を出力位置E001に出力する
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   else{
//③それ以外
    // 2020.07.37 アイプレート注記訂正_S
    // A寸法

    if (s_HEADS_DATA[823].ToIntDef(0) == 2 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       //UX-10
       s_Text = "  A=330";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 3 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       //LX-10
       s_Text = "  A=380";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 3 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 30) {
       //LX-30
       s_Text = "  A=500";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 3 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 50) {
       //LX-50
       s_Text = "  A=640";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 4 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 40) {
       //SX-40
       s_Text = "  A=700";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
       //RX-10
       s_Text = "  A=340";
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 30) {
       //RX-30
       if ((s_HEADS_DATA[170].ToIntDef(0) >= 3) &&
           (s_HEADS_DATA[170].ToIntDef(0) <= 280)) {
          s_Text = "  A=500";
       }
       else if ((s_HEADS_DATA[170].ToIntDef(0) >= 281) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 400)) {
          s_Text = "  A=300";
       }
    }
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 50) {
       //RX-50
       s_Text = "  A=700";
    }
    else {
       s_Text = "";
    }
    // 2020.07.37 アイプレート注記訂正_E
    //  （A)P型の場合
        if(s_HEADS_DATA[36].Pos("P") != 0) {
            //  "Sフレーム＆GBサポートアイプレート付"を出力位置E001に出力する
            // 2020.07.17 アイプレート注記訂正_S
            //Excel_Cell_DataSet(2 + i, 36, "Sフレーム＆GBサポートアイプレート付");
            s_Text = "Sフレーム＆GBサポートアイプレート付" + s_Text;
            Excel_Cell_DataSet(2 + i, 36, s_Text);
            // 2020.07.17 アイプレート注記訂正_E
            //  またF001に”□”を出力しE001とF001とL001の下に罫線を引く
            Excel_Cell_DataSet(2 + i, 35, "□");
            /*
            Excel_Cell_SetUnderline(2 + i, 36);    //E001
            Excel_Cell_SetUnderline(2 + i, 35);    //F001
            Excel_Cell_SetUnderline(2 + i, 56);    //L001
            */
            Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
            //2019.07.25 製作指示書書式変更_S
            //i++;
            //2019.07.25 製作指示書書式変更_E
        }
    //  （B)J型の場合
        else if(s_HEADS_DATA[36].Pos("J") != 0) {
            //  "Sフレームアイプレート付"を出力位置E001に出力する
            // 2020.07.17 アイプレート注記訂正_S
            //Excel_Cell_DataSet(2 + i, 36, "Sフレームアイプレート付");
            s_Text = "Sフレームアイプレート付" + s_Text;
            Excel_Cell_DataSet(2 + i, 36, s_Text);
            // 2020.07.17 アイプレート注記訂正_E
            //  またF001に”□”を出力しE001とF001とL001の下に罫線を引く
            Excel_Cell_DataSet(2 + i, 35, "□");
            /*
            Excel_Cell_SetUnderline(2 + i, 36);    //E001
            Excel_Cell_SetUnderline(2 + i, 35);    //F001
            Excel_Cell_SetUnderline(2 + i, 56);    //L001
            */
            Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
            //2019.07.25 製作指示書書式変更_S
            //i++;
            //2019.07.25 製作指示書書式変更_E
        }
   }

   //***************************
   //***  流体名銘板付
   //***************************
   // 『E002』
   //2019.07.25 製作指示書書式変更_S
   i = 1;
   //2019.07.25 製作指示書書式変更_E
   //① 用でない場合(アイプレートなし)
   if(P_HEADS_DATA[36].Pos("M")==0)  {
      //""を出力
      Excel_Cell_DataSet(2 + i, 36, "");
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   //② れ以外
   }else{
      //"流体名銘板付"を出力
      Excel_Cell_DataSet(2 + i, 36, "流体名銘板付");
      //またF002に”□”を出力しE002とF002とF002の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E002
      Excel_Cell_SetUnderline(2 + i, 35);    //F002
      Excel_Cell_SetUnderline(2 + i, 56);    //L002
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(禁油１)
   //***************************
   // 『E003』
   //2019.07.25 製作指示書書式変更_S
   i = 2;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1302;   // 注記(禁油１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF003に”□”を出力しE003とF003とL003の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E003
      Excel_Cell_SetUnderline(2 + i, 35);    //F003
      Excel_Cell_SetUnderline(2 + i, 56);    //L003
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(ﾉｽﾞﾙ１)
   //***************************
   // 『E004』
   //2019.07.25 製作指示書書式変更_S
   i = 3;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1325;   // 注記(ﾉｽﾞﾙ１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
        //またF004に”□”を出力しE004とF004とL004の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E004
      Excel_Cell_SetUnderline(2 + i, 35);    //F004
      Excel_Cell_SetUnderline(2 + i, 56);    //L004
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(ﾉｽﾞﾙ２)
   //***************************
   // 『E005』
   //2019.07.25 製作指示書書式変更_S
   i = 4;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1326;   // 注記(ﾉｽﾞﾙ２)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF005に”□”を出力しE005とF005とL005の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E005
      Excel_Cell_SetUnderline(2 + i, 35);    //F005
      Excel_Cell_SetUnderline(2 + i, 56);    //L005
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(客先仕様)
   //***************************
   // 『E006』
   //2019.07.25 製作指示書書式変更_S
   i = 5;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1331;   // 注記(客先仕様)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF006に”□”を出力しE006とF006とL006の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E006
      Excel_Cell_SetUnderline(2 + i, 35);    //F006
      Excel_Cell_SetUnderline(2 + i, 56);    //L006
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(客先仕様２)
   //***************************
   // 『E007』
   //2019.07.25 製作指示書書式変更_S
   i = 6;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1055;   // 注記(客先仕様２)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF007に”□”を出力しE007とF007とL007の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E007
      Excel_Cell_SetUnderline(2 + i, 35);    //F007
      Excel_Cell_SetUnderline(2 + i, 56);    //L007
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(ガスケット)
   //***************************
   // 『E008』
   //2019.07.25 製作指示書書式変更_S
   i = 7;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1054;   // 注記(ガスケット)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF008に”□”を出力しE008とF008とL008の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E008
      Excel_Cell_SetUnderline(2 + i, 35);    //F008
      Excel_Cell_SetUnderline(2 + i, 56);    //L008
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(ﾎﾞﾙﾄｶﾞｲﾄﾞﾊﾞｰ枚数１)
   //***************************
   // 『E009』
   //2019.07.25 製作指示書書式変更_S
   i = 8;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1312;   // 注記(ﾎﾞﾙﾄｶﾞｲﾄﾞﾊﾞｰ枚数１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF009に”□”を出力しE009とF009とL009の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E009
      Excel_Cell_SetUnderline(2 + i, 35);    //F009
      Excel_Cell_SetUnderline(2 + i, 56);    //L009
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***  注記(舶用工具)
   //***************************
   // 『E010』
   //2019.07.25 製作指示書書式変更_S
   i = 9;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1318;   // 注記(舶用工具)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF010に”□”を出力しE010とF010とL010の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E010
      Excel_Cell_SetUnderline(2 + i, 35);    //F010
      Excel_Cell_SetUnderline(2 + i, 56);    //L010
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   //2019.07.25 製作指示書書式変更_S
   }
   //2019.07.25 製作指示書書式変更_E

   //***************************
   //***   注記(ﾄﾞﾚﾝﾊﾟﾝ)
   //***************************
   // 『E011』
   //2019.07.25 製作指示書書式変更_S
   i = 10;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1330;   // 注記(ﾄﾞﾚﾝﾊﾟﾝ)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF011に”□”を出力しE011とF011とL011の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E011
      Excel_Cell_SetUnderline(2 + i, 35);    //F011
      Excel_Cell_SetUnderline(2 + i, 56);    //L011
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***   注記(ラチェットスパナ)
   //***************************
   // 『E012』
   //2019.07.25 製作指示書書式変更_S
   i = 11;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1335;   // 注記(ラチェットスパナ)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      //if (i >= 8) {
      //     Excel_Cell_DataSet(31 + i, 13, s_Text);
      //} else {
      //     Excel_Cell_DataSet(35 + i, 6, s_Text);
      //}
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF012に”□”を出力しE012とF012とL012の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E012
      Excel_Cell_SetUnderline(2 + i, 35);    //F012
      Excel_Cell_SetUnderline(2 + i, 56);    //L012
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   //2019.07.25 製作指示書書式変更_S
   //}
   //2019.07.25 製作指示書書式変更_E

   //***************************
   //***    注記(電解研磨１)
   //***************************
   // 『E013』
   //2019.07.25 製作指示書書式変更_S
   i = 12;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1304;   // 注記(電解研磨１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      // またF013に”□”を出力しE013とF013とL013の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E013
      Excel_Cell_SetUnderline(2 + i, 35);    //F013
      Excel_Cell_SetUnderline(2 + i, 56);    //L013
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***    注記(ﾌﾟﾚｰﾄ枚数１)
   //***************************
   // 『E014』
   //2019.07.25 製作指示書書式変更_S
   i = 13;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1306;   // 注記(ﾌﾟﾚｰﾄ枚数１)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF014に”□”を出力しE014とF014とL014の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E014
      Excel_Cell_SetUnderline(2 + i, 35);    //F014
      Excel_Cell_SetUnderline(2 + i, 56);    //L014
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   //2019.07.25 製作指示書書式変更_S
   }
   //2019.07.25 製作指示書書式変更_E

   //***************************
   //***    注記(相ﾌﾗﾝｼﾞ)
   //***************************
   // 『E015』
   //2019.07.25 製作指示書書式変更_S
   i = 14;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1329;   // 注記(相ﾌﾗﾝｼﾞ)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2020.11.24 ガスケット注記追加_S
   s_GasMatA = "";                                     // A側ｼｰﾄｶﾞｽｹｯﾄ材質
   s_GasMatB = "";                                     // B側ｼｰﾄｶﾞｽｹｯﾄ材質
   s_GasMatA = Get_SheetGasketMatA3("A", pcHeadsData); // A側ｼｰﾄｶﾞｽｹｯﾄ材質
   s_GasMatB = Get_SheetGasketMatA3("B", pcHeadsData); // B側ｼｰﾄｶﾞｽｹｯﾄ材質
   bGas_A = true;                                      // A側ｼｰﾄｶﾞｽｹｯﾄ有　
   bGas_B = true;                                      // B側ｼｰﾄｶﾞｽｹｯﾄ有
   if (s_Text.Trim() == ""){
      // 相ﾌﾗﾝｼﾞ注記無し
      bGas_A = false;                                  // A側ｼｰﾄｶﾞｽｹｯﾄ無　
      bGas_B = false;                                  // B側ｼｰﾄｶﾞｽｹｯﾄ無
   }
   if (s_HEADS_DATA[893].SubString(3,1)== "1") {
      // B側ﾌﾞｰﾂﾉｽﾞﾙ
      bGas_B = false;                                  // B側ｼｰﾄｶﾞｽｹｯﾄ無
   }
   if (s_HEADS_DATA[906].SubString(3,1)== "1") {
      // A側ﾌﾞｰﾂﾉｽﾞﾙ
      bGas_A = false;                                  // A側ｼｰﾄｶﾞｽｹｯﾄ無　
   }

   if (bGas_A == true || bGas_B == true ) {  // ガスケット要
      s_Text2 = s_HEADS_DATA[892].TrimRight(); // Ｓ１ノズル口径
      d_Koukei = s_Text2.ToDouble();
      s_Text3 = s_HEADS_DATA[893].TrimRight(); // Ｓ１ノズル継手
      if ( d_Koukei >= 250 ) {
         s_GasName = "　全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ";
      } else if (s_Text3.SubString(1,2) == "04" ||
                 s_Text3.SubString(1,2) == "05" ||
                 s_Text3.SubString(1,2) == "44" ||
                 s_Text3.SubString(1,2) == "08" ||
                 s_Text3.SubString(1,2) == "09" ||
                 s_Text3.SubString(1,2) == "48") {
         if ( d_Koukei >= 10 ) {
            s_GasName = "　全面座ﾌﾗﾝｼﾞｶﾞｽｹｯﾄ";
         } else {
            s_GasName = "　ﾘﾝｸﾞｶﾞｽｹｯﾄ";
         }
         s_GasName = "　ﾘﾝｸﾞｶﾞｽｹｯﾄ";
      } else {
         s_GasName = "　ﾘﾝｸﾞｶﾞｽｹｯﾄ";
      }
   } else {
      s_GasName = "";
   }

   if (bGas_A == false && bGas_B == true ) {
      // B側ｼｰﾄｶﾞｽｹｯﾄ有
      s_Text4 = s_GasName + "(" + s_GasMatB + ")";
   } else if (bGas_A == true && bGas_B == false ) {
      // A側ｼｰﾄｶﾞｽｹｯﾄ有
      s_Text4 == s_GasName + "(" + s_GasMatA + ")";
   } else if (bGas_A == true && bGas_B == true ) {
      // 両側ｼｰﾄｶﾞｽｹｯﾄ有
      if (s_GasMatA != s_GasMatB) {
         // ABｼｰﾄｶﾞｽｹｯﾄ異なる
         s_Text4 = s_GasName + "(B:" + s_GasMatB + "/A:" + s_GasMatA + ")";
      } else {
         s_Text4 = s_GasName + "(" + s_GasMatB + ")";
      }
   } else {
      // ｼｰﾄｶﾞｽｹｯﾄ無
      s_Text4 = "";
   }

   s_Text = s_Text + s_Text4;
   //2020.11.24 ガスケット注記追加_E
   if (s_Text.Trim() != ""){
      //if (i >= 8) {
      //     Excel_Cell_DataSet(31 + i, 13, s_Text);
      //} else {
      //     Excel_Cell_DataSet(35 + i, 6, s_Text);
      //}
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF015に”□”を出力しE015とF015とL015の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E015
      Excel_Cell_SetUnderline(2 + i, 35);    //F015
      Excel_Cell_SetUnderline(2 + i, 56);    //L015
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(2 + i, 30);
      Excel_Cell_SetUnderline(2 + i, 32);
      Excel_Cell_SetUnderline(2 + i, 56);
      Excel_Cell_SetUnderline(2 + i, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }

   //***************************
   //***    注記(予備品)
   //***************************
   // 『E016』
   //2019.07.25 製作指示書書式変更_S
   i = 15;
   //2019.07.25 製作指示書書式変更_E
   i_RecNo = 1336;   // 注記(予備品)
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text.Trim() != ""){
      //if (i >= 8) {
      //     Excel_Cell_DataSet(31 + i, 13, s_Text);
      //} else {
      //     Excel_Cell_DataSet(35 + i, 6, s_Text);
      //}
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF016に”□”を出力しE016とF016とL016の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      /*
      Excel_Cell_SetUnderline(2 + i, 36);    //E016
      Excel_Cell_SetUnderline(2 + i, 35);    //F016
      Excel_Cell_SetUnderline(2 + i, 56);    //L016
      */
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
      //2019.07.25 製作指示書書式変更_S
      //i++;
      //2019.07.25 製作指示書書式変更_E
   }
   //2019.07.25 製作指示書書式変更_S
   //}
   //2019.07.25 製作指示書書式変更_E

   //*************************

   // 2020.05.28 温水乾燥注記追加_S
   //***************************
   //***    注記(温水乾燥)
   //***************************
   // 『E017』
   i = 16;
   s_Text = "";
   s_Text2 = "";
   s_Text3 = "";
   s_Text4 = "";

   s_Text = Get_HotAirDrying();

   G_Log_Text = "温水乾燥取得          『" + s_Text + "』";
   Write_Log(G_Log_Text);

   G_Log_Text = "s_SpecCode            『" + s_SpecCode + "』";
   Write_Log(G_Log_Text);

   i_Length = s_Text.Length();
   if (i_Length < 21 ) {
       s_Text = "";
   }

   if(s_SpecCode == "B"){

      if ( s_Text != "" ) {
         //前部分
         s_Text2 = s_Text.SubString(1,21);
         //B側時間
         if ( s_Text.Pos("B(1)") > 0 ) {
            s_Text3 = "1";
         } else if ( s_Text.Pos("B(2)") > 0 ) {
            s_Text3 = "2";
         } else if ( s_Text.Pos("B(3)") > 0 ) {
            s_Text3 = "3";
         } else if ( s_Text.Pos("B(4)") > 0 ) {
            s_Text3 = "4";
         } else if ( s_Text.Pos("B(5)") > 0 ) {
            s_Text3 = "5";
         } else if ( s_Text.Pos("B(6)") > 0 ) {
            s_Text3 = "6";
         } else if ( s_Text.Pos("B(7)") > 0 ) {
            s_Text3 = "7";
         } else if ( s_Text.Pos("B(8)") > 0 ) {
            s_Text3 = "8";
         } else if ( s_Text.Pos("B(9)") > 0 ) {
            s_Text3 = "9";
         } else if ( s_Text.Pos("B(10)") > 0 ) {
            s_Text3 = "10";
         } else if ( s_Text.Pos("B(11)") > 0 ) {
            s_Text3 = "11";
         } else if ( s_Text.Pos("B(12)") > 0 ) {
            s_Text3 = "12";
         } else if ( s_Text.Pos("B(13)") > 0 ) {
            s_Text3 = "13";
         } else if ( s_Text.Pos("B(14)") > 0 ) {
            s_Text3 = "14";
         } else if ( s_Text.Pos("B(15)") > 0 ) {
            s_Text3 = "15";
         } else if ( s_Text.Pos("B(16)") > 0 ) {
            s_Text3 = "16";
         } else if ( s_Text.Pos("B(17)") > 0 ) {
            s_Text3 = "17";
         } else if ( s_Text.Pos("B(18)") > 0 ) {
            s_Text3 = "18";
         } else if ( s_Text.Pos("B(19)") > 0 ) {
            s_Text3 = "19";
         } else if ( s_Text.Pos("B(20)") > 0 ) {
            s_Text3 = "20";
         } else {
            s_Text3 = "0";
         }
         //A側時間
         if ( s_Text.Pos("A(1)") > 0) {
            s_Text4 = "1";
         } else if ( s_Text.Pos("A(2)") > 0 ) {
            s_Text4 = "2";
         } else if ( s_Text.Pos("A(3)") > 0 ) {
            s_Text4 = "3";
         } else if ( s_Text.Pos("A(4)") > 0 ) {
            s_Text4 = "4";
         } else if ( s_Text.Pos("A(5)") > 0 ) {
            s_Text4 = "5";
         } else if ( s_Text.Pos("A(6)") > 0 ) {
            s_Text4 = "6";
         } else if ( s_Text.Pos("A(7)") > 0 ) {
            s_Text4 = "7";
         } else if ( s_Text.Pos("A(8)") > 0 ) {
            s_Text4 = "8";
         } else if ( s_Text.Pos("A(9)") > 0 ) {
            s_Text4 = "9";
         } else if ( s_Text.Pos("A(10)") > 0 ) {
            s_Text4 = "10";
         } else if ( s_Text.Pos("A(11)") > 0 ) {
            s_Text4 = "11";
         } else if ( s_Text.Pos("A(12)") > 0 ) {
            s_Text4 = "12";
         } else if ( s_Text.Pos("A(13)") > 0 ) {
            s_Text4 = "13";
         } else if ( s_Text.Pos("A(14)") > 0 ) {
            s_Text4 = "14";
         } else if ( s_Text.Pos("A(15)") > 0 ) {
            s_Text4 = "15";
         } else if ( s_Text.Pos("A(16)") > 0 ) {
            s_Text4 = "16";
         } else if ( s_Text.Pos("A(17)") > 0 ) {
            s_Text4 = "17";
         } else if ( s_Text.Pos("A(18)") > 0 ) {
            s_Text4 = "18";
         } else if ( s_Text.Pos("A(19)") > 0 ) {
            s_Text4 = "19";
         } else if ( s_Text.Pos("A(20)") > 0 ) {
            s_Text4 = "20";
         } else {
            s_Text4 = "0";
         }
         // B号機注記：前部分 + A時間 + ")/A(" + B時間 + ")★"
         s_Text = s_Text2 + s_Text4 + ")/A(" + s_Text3 + ")★";

      }

   }

   if (s_Text.Trim() != ""){
      s_Text = s_Text + "  ※チェックシート台数分あり";
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF017に”□”を出力しE017とF017とL017の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }

   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   //************************
   //***  注記(小径締付ﾎﾞﾙﾄ)
   //************************
   // 『E018』
   i = 17;
   s_Text = "";
   s_Text1 = "";
   s_Text2 = "";
   s_Text3 = "";
   s_Text4 = "";

   i_RecNo = 1113;                              // 小径ﾎﾞﾙﾄ本数
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

   if (s_Text1.ToIntDef(0) != 0){
      // 小径ﾎﾞﾙﾄ有り
      s_Text1 = "";
      i_RecNo = 1108;   // BDIS(径)
      s_Text1 = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
      if (s_Text1 == "42"){
         // 1 5/8-8UN
         s_Text1 = "1 5/8""-8UN";
      } else if (s_Text1 == "52"){
         // 2-8UN
         s_Text1 = "2""-8UN";
      } else if (s_Text1 == "1.630" ){
         // 1 5/8-8UN
         s_Text1 = "1 5/8""-8UN";
      } else if (s_Text1 == "2.000" ){
         // 2-8UN
         s_Text1 = "2""-8UN";
      } else {
         s_Text1 = "M" + s_Text1;
      }

      i_RecNo = 1377;                              // 小径ﾎﾞﾙﾄ全長
      s_Text = "小径締付ボルト：" + s_Text1 + "×" + s_HEADS_DATA[i_RecNo].TrimRight() + "L";

      if (P_HEADS_DATA[36].Pos("M") != 0){
         s_Text =  s_Text + "  Ｗナット";          // 舶用Ｗナット
      }

      // 材質決定ロジック
      s_Text1 = "";
      i_RecNo = 1041;                              // 材質指定
      s_Text2 = P_HEADS_DATA[i_RecNo].TrimRight();
      i_RecNo = 1021;                              // BOL22
      s_Text3 = P_HEADS_DATA[i_RecNo].TrimRight();
      s_Text4 = G_KS_Syurui + "－" + G_KS_Model;

      if (( s_Text2 == "A157" ) && ( s_Text4 == "ＬＸ－３０")) {
         // Q235B2: S/L Q235B 小径 45/35 CODE A157
         s_Text1 = "45/35";                        // 締付ボルト材質
      } else if (( s_Text2 == "A163" ) && ( s_Text4 == "ＬＸ－５０")) {
         // 45_2  : S/L 45/35 小径 Q235B CODE A163
         s_Text1 = "Q235B";                        // 締付ボルト材質
      } else if (( s_Text2 != "" ) && ( s_Text2 != "0000" ) ){
         s_Text1 = sBltMat;                        // 締付ボルト材質
      } else {
         if (( s_Text3 == "Q235B2" ) && ( s_Text4 == "ＬＸ－３０")) {
            // Q235B2: S/L Q235B 小径 45/35 CODE A157
            s_Text1 = "45/35";                        // 締付ボルト材質
         } else if (( s_Text3 == "45_2" ) && ( s_Text4 == "ＬＸ－５０")) {
            // 45_2  : S/L 45/35 小径 Q235B CODE A163
            s_Text1 = "Q235B";                     // 締付ボルト材質
         } else {
            s_Text1 = sBltMat;                     // 締付ボルト材質
         }
      }

      i_RecNo = 435;                               // ﾐﾙｼｰﾄ
      if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      } else {
         s_Text1 = s_Text1 + " ﾐﾙ";
      }

      s_Text = s_Text + "  " + s_Text1;

      s_Text1 = "";
      i_RecNo = 1113;                              // 小径ﾎﾞﾙﾄ本数
      s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

      if (s_Text1.ToIntDef(0) == 0){
         s_Text = "";
      } else {
         s_Text = s_Text + "  " + s_Text1 + "本";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF018に”□”を出力しE018とF018とL018の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }

   //************************
   //***  注記(小径ﾎﾞﾙﾄｶﾊﾞｰBLCP1S)
   //************************
   // 『E019』
   i = 18;
   s_Text = "";
   s_Text1 = "";
   s_Text2 = "";

   i_RecNo = 1113;                                 // 小径ﾎﾞﾙﾄ本数
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

   if (s_Text1.ToIntDef(0) != 0){
      i_RecNo = 1111;      // BLCP1S
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2.ToIntDef(0) != 0 ){
         // 小径ﾎﾞﾙﾄ有り
         i_RecNo = 1108;   // BDIS(径)
         s_Text3 = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
         if (s_Text3 == "42"){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN用";
         } else if (s_Text3 == "52"){
            // 2-8UN
            s_Text3 = "2""-8UN用";
         } else if (s_Text3 == "1.630" ){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN用";
         } else if (s_Text3 == "2.000" ){
            // 2-8UN
            s_Text3 = "2""-8UN用";
         } else {
            s_Text3 = "M" + s_Text3+ "用";
         }

         s_Text = "小径締付ボルトカバー：" + s_Text3 + "×" + s_Text2 + "L";
         s_Text = s_Text + "  " + s_Text1 + "本";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF019に”□”を出力しE019とF019とL019の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }

   //************************
   //***  注記(小径ﾎﾞﾙﾄｶﾊﾞｰBLCESS)
   //************************
   // 『E020』
   i = 19;
   s_Text = "";
   s_Text1 = "";
   s_Text2 = "";

   i_RecNo = 1113;                                 // 小径ﾎﾞﾙﾄ本数
   s_Text1 = s_HEADS_DATA[i_RecNo].TrimRight();

   if (s_Text1.ToIntDef(0) != 0){
      i_RecNo = 1112;      // BLCESS
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text2.ToIntDef(0) != 0 ){
         // 小径ﾎﾞﾙﾄ有り
         i_RecNo = 1108;   // BDIS(径)
         s_Text3 = atoi(s_HEADS_DATA[i_RecNo].TrimRight().c_str());
         if (s_Text3 == "42"){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN用";
         } else if (s_Text3 == "52"){
            // 2-8UN
            s_Text3 = "2""-8UN用";
         } else if (s_Text3 == "1.630" ){
            // 1 5/8-8UN
            s_Text3 = "1 5/8""-8UN用";
         } else if (s_Text3 == "2.000" ){
            // 2-8UN
            s_Text3 = "2""-8UN用";
         } else {
            s_Text3 = "M" + s_Text3+ "用";
         }

         s_Text = "小径締付ボルトカバー：" + s_Text3 + "×" + s_Text2 + "L+C";
         s_Text = s_Text + "  " + s_Text1 + "本";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF020に”□”を出力しE020とF020とL020の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2020.06.19 小径締付ﾎﾞﾙﾄ追加_E

   // 2020.10.30 圧力容器UNS規格警告追加_S
   //************************
   //***  注記(圧力容器UNS規格警告)
   //************************
   // 『E021』
   i = 20;
   s_Text = "";

   if ( s_HEADS_DATA[29].ToIntDef(0) == 1 ||
        s_HEADS_DATA[29].ToIntDef(0) == 2 ||
        s_HEADS_DATA[29].ToIntDef(0) == 3 ){
      // 圧力容器（規格ｺｰﾄﾞ）
      if ( s_HEADS_DATA[43].TrimRight() == "3472" ) {
         // ＵＮＳ　Ｎ１０２７６
         s_Text = "ＪＩＳ規格材質併記要";
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF021に”□”を出力しE021とF021とL021の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2020.10.30 圧力容器UNS規格警告追加_E

   // 2021.10.19 注記追加_S
   //************************
   //***  ダイトク注記
   //************************
   // 『E022』
   i = 21;
   s_Text = "";

   //--- 御注文主コード
   i_RecNo = 1064;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "04633"){
       // ダイトク向け塗装手塗り指示
       s_Text = "注）塗装は手塗りのこと。TS2021-041による";
       Excel_Cell_DataSet(11, 16, s_Text);
       G_Log_Text = "ダイトク向け塗装手塗り指示     『" + s_Text + "』をセット。";
       Write_Log(G_Log_Text);
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF022に”□”を出力しE022とF022とL022の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2021.10.19 注記追加_E

   // 2022.04.04 機器番号機器名称特記追加_S
   //************************
   //***  機器番号
   //************************
   // 『E023』

   i = 21;
   s_Text = "";

   if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
      // 機器番号特記項目有
      // 機器番号欄外注記に記述

      i_RecNo = 27;  // 機器番号
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

      int w_count = s_Text.AnsiPos("～");

      if (w_count) {
         s_Text = "※機器番号  " + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "～" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
      } else {
         s_Text = "※機器番号  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
      }
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF023に”□”を出力しE023とF023とL023の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2022.04.04 機器番号機器名称特記追加_E

   // 2022.04.11 就航船名特記追加_S
   //************************
   //***  就航船名
   //************************
   // 『E024』

   i = 22;
   s_Text = "";

   if (Tokki_Code("29", s_HEADS_DATA, 1500) == true) {
      // SHIP NAME特記記項目有
      // 就航船名を記述
      s_Text = Tokki_Zen("29", s_HEADS_DATA, 1500);
      s_Text = s_Text.SubString(10, s_Text.Length() - 9);
      s_Text = "就航船名:" + s_Text;
   }

   if (s_Text.Trim() != ""){
      Excel_Cell_DataSet(2 + i, 36, s_Text);
      //またF024に”□”を出力しE024とF024とL024の下に罫線を引く
      Excel_Cell_DataSet(2 + i, 35, "□");
      Excel_Cell_SetUnderline(2 + i, 30, 2 + i, 57);
   }
   // 2022.04.11 就航船名特記追加_E


   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          G欄　　データセット                        ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //2019.10.31 スパナ変更_S
   //***************************
   //***    工具タイトル
   //***************************
   // 『G000』

   //2019.12.27 製作指示書書式変更_S
   ////      舶用でない場合
   // if(s_HEADS_DATA[36].Pos("M") == 0)  {
   //     // "工具"を出力
   //     Excel_Cell_DataSet(43, 30, "工具");
   // }
   //2019.12.27 製作指示書書式変更_E
   //2019.10.31 スパナ変更_E

   //***************************
   //***    スパナ数量
   //***************************
   // 『G001』
    //2019.10.31 スパナ変更_S
    //①    舶用でない場合
    if(s_HEADS_DATA[36].Pos("M") == 0)  {
    //2019.12.02 スパナ変更_S
        //// "----"を出力
        //Excel_Cell_DataSet(44, 44, "----");
        //Ａ    ラチェットなしの場合
        if (s_HEADS_DATA[262].TrimRight() != "0" && s_HEADS_DATA[262].TrimRight() != "2"){
            //"----"を出力
            Excel_Cell_DataSet(44, 44, "----");
        }
        //Ｂ    それ以外
        else{
            //2020/03/31 数量変更_S
            ////"1"を出力
            //Excel_Cell_DataSet(44, 44, "1");
            //"1/台"を出力
            Excel_Cell_DataSet(44, 44, "1/台");
            //2020/03/31 数量変更_E
        }
        Excel_Cell_DataSet(44, 36, "ラチェットスパナ");
    //2019.12.02 スパナ変更_E
    }
    //②    RX-00の場合
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"を出力
        Excel_Cell_DataSet(44, 44, "----");
    }
    //③    ラチェットなしの場合
    else if (s_HEADS_DATA[262].TrimRight() != "0" && s_HEADS_DATA[262].TrimRight() != "2"){
        //"----"を出力
        Excel_Cell_DataSet(44, 44, "----");
    }
    //④    それ以外
    else{
        //2020/03/31 数量変更_S
        ////"1"を出力
        //Excel_Cell_DataSet(44, 44, "1");
        //"1/台"を出力
        Excel_Cell_DataSet(44, 44, "1/台");
        //2020/03/31 数量変更_E
    }

   //***************************
   //***    洗浄用ブラシ数量
   //***************************
   // 『G002』

    //①    舶用でない場合
    if(P_HEADS_DATA[36].Pos("M")==0)  {
        // "----"を出力
        Excel_Cell_DataSet(45, 44, "----");
    }
    //②    RX-00の場合
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"を出力
        Excel_Cell_DataSet(45, 44, "----");
    }
    //2019.07.25 製作指示書書式変更_S
    ////③    ラチェットなしの場合
    //else if (s_HEADS_DATA[262].TrimRight() != "1" && s_HEADS_DATA[262].TrimRight() != "2"){
    //③    舶用工具なしの場合
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    //2019.07.25 製作指示書書式変更_E
        //"----"を出力
        Excel_Cell_DataSet(45, 44, "----");
    }
    //④    それ以外
    else{
        //2020/03/31 数量変更_S
        ////"1"を出力
        //Excel_Cell_DataSet(45, 44, "1");
        //"1/台"を出力
        Excel_Cell_DataSet(45, 44, "1/台");
        //2020/03/31 数量変更_E
    }

   //***************************
   //***    ゲージプレート　L
   //***************************
   // 『G003』

    //2019.07.25 製作指示書書式変更_S
    ////①    舶用でない場合
    //if(P_HEADS_DATA[36].Pos("M") == 0)  {
    //    // "----"を出力
    //    Excel_Cell_DataSet(46, 44, "----");
    //}
    ////②    RX-00の場合
    //else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
    //    //"----"を出力
    //    Excel_Cell_DataSet(46, 44, "----");
    //}
    ////③    ラチェットなしの場合
    //else if (s_HEADS_DATA[262].TrimRight() != "1" && s_HEADS_DATA[262].TrimRight() != "2"){
    //    //"----"を出力
    //    Excel_Cell_DataSet(46, 44, "----");
    //}
    ////④    それ以外
    //else{
    //    s_Text = HanToZen(P_HEADS_DATA[1126].Trim());
    //    Excel_Cell_DataSet(46, 44, s_Text);
    //}
    //①    舶用でない場合
    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"を出力
        s_Text = "----";
    }
    //②    RX-00の場合
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"を出力
        s_Text = "----";
    }
    //③    舶用工具なしの場合
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"を出力
        s_Text = "----";
    }
    //④    それ以外
    else{
        s_Text = HanToZen(P_HEADS_DATA[1126].Trim());
    }
    Excel_Cell_DataSet(46, 40, s_Text);
    //2019.07.25 製作指示書書式変更_E

   //***************************
   //***    ゲージプレート　Max
   //***************************
   // 『G004』

    //2019.07.25 製作指示書書式変更_S
    ////①    舶用でない場合
    //if(P_HEADS_DATA[36].Pos("M")==0)  {
    ////"----"を出力
    //    Excel_Cell_DataSet(47, 44, "----");
    //}
    ////②    RX-00の場合
    //if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
    ////"----"を出力
    //    Excel_Cell_DataSet(47, 44, "----");
    //}
    ////③    舶用工具なしの場合
    //if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    ////"----"を出力
    //    Excel_Cell_DataSet(47, 44, "----");
    //}
    ////④    それ以外
    //else{
    //    s_Text = HanToZen(P_HEADS_DATA[1089].Trim());
    //    Excel_Cell_DataSet(47, 44, s_Text);
    //}
    //①    舶用でない場合
    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"を出力
        s_Text = "----";
    }
    //②    RX-00の場合
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"を出力
        s_Text = "----";
    }
    //③    舶用工具なしの場合
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"を出力
        s_Text = "----";
    }
    //④    それ以外
    else{
        s_Text = HanToZen(P_HEADS_DATA[1089].Trim());
    }
    Excel_Cell_DataSet(47, 40, s_Text);
    //2019.07.25 製作指示書書式変更_E

   //***************************
   //***    ゲージプレート　Min
   //***************************
   // 『G005』

    //2019.07.25 製作指示書書式変更_S
    ////①    舶用でない場合
    //if(P_HEADS_DATA[36].Pos("M")==0)  {
    ////"----"を出力
    //    Excel_Cell_DataSet(48, 44, "----");
    //}
    ////②    RX-00の場合
    //if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
    ////"----"を出力
    //    Excel_Cell_DataSet(48, 44, "----");
    //}
    ////③    舶用工具なしの場合
    //if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    ////"----"を出力
    //    Excel_Cell_DataSet(48, 44, "----");
    //}
    ////④    それ以外
    //else{
    //    s_Text = HanToZen(P_HEADS_DATA[1015].Trim());
    //    Excel_Cell_DataSet(48, 44, s_Text);
    //}
    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"を出力
        s_Text = "----";
    }
    //②    RX-00の場合
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"を出力
        s_Text = "----";
    }
    //③    舶用工具なしの場合
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"を出力
        s_Text = "----";
    }
    //④    それ以外
    else{
        s_Text = HanToZen(P_HEADS_DATA[1015].Trim());
    }
    Excel_Cell_DataSet(48, 40, s_Text);
    //2019.07.25 製作指示書書式変更_E

   //2019.07.25 製作指示書書式変更_S
   //***************************
   //***    ゲージプレート
   //***************************

    if(P_HEADS_DATA[36].Pos("M") == 0)  {
        // "----"を出力
        s_Text = "----";
    }
    //②    RX-00の場合
    else if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0){
        //"----"を出力
        s_Text = "----";
    }
    //③    舶用工具なしの場合
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
        //"----"を出力
        s_Text = "----";
    }
    //④    それ以外
    else{
        //2020/03/31 数量変更_S
        //s_Text = "1";
        s_Text = "1/台";
        //2020/03/31 数量変更_E
    }
    Excel_Cell_DataSet(47, 44, s_Text);
   //2019.07.25 製作指示書書式変更_E

   //***************************
   //***    移動止め
   //***************************
   // 『G006』

    //①    舶用でない場合
    if(P_HEADS_DATA[36].Pos("M")==0)  {
    //"----"を出力
        Excel_Cell_DataSet(49, 44, "----");
    }
    //②    舶用工具なしの場合
    else if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
    //"----"を出力
        Excel_Cell_DataSet(49, 44, "----");
    }
    //③    J型の場合移動止め不要
    else if(s_HEADS_DATA[36].Pos("J") != 0) {
    //"----"を出力
        Excel_Cell_DataSet(49, 44, "----");
    }//④	それ以外
    else{
        //2020/03/31 数量変更_S
        ////"1"を出力
        //Excel_Cell_DataSet(49, 44, "1");
        //"1/台"を出力
        Excel_Cell_DataSet(49, 44, "1/台");
        //2020/03/31 数量変更_E
    }

   //***************************
   //***    ノズルオリエンテーション
   //***************************
   // 『S1』『S2』『S3』『S4』
   s_S1NO_JP = "";           // S1液名和文
   s_S2NO_JP = "";           // S2液名和文
   s_S3NO_JP = "";           // S3液名和文
   s_S4NO_JP = "";           // S4液名和文

   s_S1NO_EN = "";           // S1液名英文
   s_S2NO_EN = "";           // S2液名英文
   s_S3NO_EN = "";           // S3液名英文
   s_S4NO_EN = "";           // S4液名英文

    //①    斜行流の場合
    //2019.07.25 製作指示書書式変更_S
    ////CX-10D
    //if ( (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) ||
    ////LX-90D
    //     (s_HEADS_DATA[823].ToIntDef(0) == 21 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 90) ||
    ////YX-80
    //     (s_HEADS_DATA[823].ToIntDef(0) == 111&& s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 80)){
    if ( P_FL == 1 ) {
    //2019.07.25 製作指示書書式変更_E
        //(A)   AB反転の場合
        if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // AB逆
            //S1液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[76].TrimRight();
            //S2液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[45].TrimRight();
            //S3液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[45].TrimRight();
            //S4液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[76].TrimRight();
            //S1液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[135].TrimRight();
            //S2液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S3液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S4液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[135].TrimRight();
        }
        //(B)   それ以外
        else{
            //S1液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[45].TrimRight();
            //S2液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[76].TrimRight();
            //S3液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[76].TrimRight();
            //S4液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[45].TrimRight();
            //S1液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S2液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S2NO_EN = P_HEADS_DATA[135].TrimRight();
            //S3液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S3NO_EN = P_HEADS_DATA[135].TrimRight();
            //S4液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S4NO_EN = P_HEADS_DATA[134].TrimRight();
        }
    }
    //②    それ以外
    else{
        //(A)   AB反転の場合
        if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) == 1 ) {     // AB逆
            //S1液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[76].TrimRight();
            //S2液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[45].TrimRight();
            //S3液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[76].TrimRight();
            //S4液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[45].TrimRight();
            //S1液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[135].TrimRight();
            //S2液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S2NO_EN = P_HEADS_DATA[134].TrimRight();
            //S3液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S3NO_EN = P_HEADS_DATA[135].TrimRight();
            //S4液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S4NO_EN = P_HEADS_DATA[134].TrimRight();
        }
        //(B)   それ以外
        else{
            //S1液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S1NO_JP = P_HEADS_DATA[45].TrimRight();
            //S2液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S2NO_JP = P_HEADS_DATA[76].TrimRight();
            //S3液名和文　　P_HEADS_DATA[45].TrimRight()
            s_S3NO_JP = P_HEADS_DATA[45].TrimRight();
            //S4液名和文　　P_HEADS_DATA[76].TrimRight()
            s_S4NO_JP = P_HEADS_DATA[76].TrimRight();
            //S1液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S1NO_EN = P_HEADS_DATA[134].TrimRight();
            //S2液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S2NO_EN = P_HEADS_DATA[135].TrimRight();
            //S3液名英文　　P_HEADS_DATA[134].TrimRight()
            s_S3NO_EN = P_HEADS_DATA[134].TrimRight();
            //S4液名英文　　P_HEADS_DATA[135].TrimRight()
            s_S4NO_EN = P_HEADS_DATA[135].TrimRight();
        }
    }

   //***************************
   //***    S1ノズルオリエンテーション
   //***************************
   // 『G007』

    //①    S1ノズル入口の場合
    //2019.07.25 製作指示書書式変更_S
    //if(s_HEADS_DATA[1136].TrimRight() == "1"){
    if( s_HEADS_DATA[891].TrimRight() == "1" ) {
    //2019.07.25 製作指示書書式変更_E
        //和文の場合
        if(P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S1液名和文 + "用 入口"を出力位置G007に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S1NO_JP + "用 入口";
            s_Text = s_S1NO_JP + " 入口";
            // 2020.10.30 オリエンテーション変更_E
        }
        //その他の場合
        else{
            //S1液名英文 + " IN"を出力位置G007に出力する
            s_Text = s_S1NO_EN + " IN";
        }
    }
    //②    S1ノズル出口の場合
    //2019.07.25 製作指示書書式変更_S
    //else if( s_HEADS_DATA[1136].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[891].TrimRight() == "2" ) {
    //2019.07.25 製作指示書書式変更_E
        //和文の場合
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S1液名和文 + "用 出口"を出力位置G007に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S1NO_JP + "用 出口";
            s_Text = s_S1NO_JP + " 出口";
            // 2020.10.30 オリエンテーション変更_E
        }
        //その他の場合
        else{
            //S1液名英文 + " OUT"を出力位置G007に出力する
            s_Text = s_S1NO_EN + " OUT";
        }
    }
    //③    それ以外
    else{
        //""を出力位置G007に出力する
        s_Text = "";
    }
    Excel_Cell_DataSet(51, 37, s_Text);

   //***************************
   //***    S2ノズルオリエンテーション
   //***************************
   // 『G008』

    //①    S2ノズル入口の場合
    //2019.07.25 製作指示書書式変更_S
    //if ( s_HEADS_DATA[1137].TrimRight() == "1" ) {
    if( s_HEADS_DATA[906].TrimRight() == "1" ) {
    //2019.07.25 製作指示書書式変更_E
    //和文の場合
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S2液名和文 + "用 入口"を出力位置G008に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S2NO_JP + "用 入口";
            s_Text = s_S2NO_JP + " 入口";
            // 2020.10.30 オリエンテーション変更_E

        }
        //その他の場合
        else{
            //S2液名英文 + " IN"を出力位置G008に出力する
            s_Text = s_S2NO_EN + " IN";
        }
    }
    //②    S2ノズル出口の場合
    //2019.07.25 製作指示書書式変更_S
    //else if ( s_HEADS_DATA[1137].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[906].TrimRight() == "2" ) {
    //2019.07.25 製作指示書書式変更_E
    //和文の場合
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S2液名和文 + "用 出口"を出力位置G008に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S2NO_JP + "用 出口";
            s_Text = s_S2NO_JP + " 出口";
            // 2020.10.30 オリエンテーション変更_E
        }
        //その他の場合
        else{
            //S2液名英文 + " OUT"を出力位置G008に出力する
            s_Text = s_S2NO_EN + " OUT";
        }
    }
    //③    それ以外
    else{
        //""を出力位置G008に出力する
        s_Text = "";
    }
    Excel_Cell_DataSet(51, 45, s_Text);


   //***************************
   //***    S3ノズルオリエンテーション
   //***************************
   // 『G009』

    //①    S3ノズル入口の場合
    //2019.07.25 製作指示書書式変更_S
    //if ( s_HEADS_DATA[1138].TrimRight() == "1" ) {
    if( s_HEADS_DATA[921].TrimRight() == "1" ) {
    //2019.07.25 製作指示書書式変更_E
    //和文の場合
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S3液名和文 + "用 入口"を出力位置G009に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S3NO_JP + "用 入口";
            s_Text = s_S3NO_JP + " 入口";
            // 2020.10.30 オリエンテーション変更_E
        }
        //その他の場合
        else{
            //S3液名英文 + " IN"を出力位置G009に出力する
            s_Text = s_S3NO_EN + " IN";
        }
    }
    //②    S3ノズル出口の場合
    //2019.07.25 製作指示書書式変更_S
    //else if ( s_HEADS_DATA[1138].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[921].TrimRight() == "2" ) {
    //2019.07.25 製作指示書書式変更_E
    //和文の場合
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S3液名和文 + "用 出口"を出力位置G009に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S3NO_JP + "用 出口";
            s_Text = s_S3NO_JP + " 出口";
            // 2020.10.30 オリエンテーション変更_E
        }
        //その他の場合
        else{
            //S3液名英文 + " OUT"を出力位置G009に出力する
            s_Text = s_S3NO_EN + " OUT";
        }
    }
    //③    それ以外
    else{
        //""を出力位置G009に出力する
        s_Text = "";
    }
    Excel_Cell_DataSet(52, 37, s_Text);


   //***************************
   //***    S4ノズルオリエンテーション
   //***************************
   // 『G010』

    //①    S4ノズル入口の場合
    //2019.07.25 製作指示書書式変更_S
    //if ( s_HEADS_DATA[1139].TrimRight() == "1" ) {
    if( s_HEADS_DATA[936].TrimRight() == "1" ) {
    //2019.07.25 製作指示書書式変更_E
    //和文の場合
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S4液名和文 + "用 入口"を出力位置G010に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S4NO_JP + "用 入口";
            s_Text = s_S4NO_JP + " 入口";
            // 2020.10.30 オリエンテーション変更_E
        }
        //その他の場合
        else{
            //S4液名英文 + " IN"を出力位置G010に出力する
            s_Text = s_S4NO_EN + " IN";
        }
    }
    //②    S4ノズル出口の場合
    //2019.07.25 製作指示書書式変更_S
    //else if ( s_HEADS_DATA[1139].TrimRight() == "2" ) {
    else if( s_HEADS_DATA[936].TrimRight() == "2" ) {
    //2019.07.25 製作指示書書式変更_E
    //和文の場合
        if (P_HEADS_DATA[1].TrimRight() == "0") { // 和文
            //S4液名和文 + "用 出口"を出力位置G010に出力する
            // 2020.10.30 オリエンテーション変更_S
            //s_Text = s_S4NO_JP + "用 出口";
            s_Text = s_S4NO_JP + " 出口";
            // 2020.10.30 オリエンテーション変更_E
        }
        //その他の場合
        else{
            //S4液名英文 + " OUT"を出力位置G010に出力する
            s_Text = s_S4NO_EN + " OUT";
        }
    }
    //③    それ以外
    else{
        //""を出力位置G010に出力する
        s_Text = "";
    }
    Excel_Cell_DataSet(52, 45, s_Text);

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
   //2023.02.08 作番桁数追加_S
   AnsiString OLD_SAKUBAN;         // ６桁時代作番
   AnsiString  Work,Rev;
   AnsiString xls_Output_Dir_old;  // ６桁時代出力ディレクトリ
   int i_Sakuban;
   //2023.02.08 作番桁数追加_E

   if (s_HEADS_DATA[319].TrimRight() == "0" || s_HEADS_DATA[319].TrimRight() == "00") {
   } else {
       d_RevNo=Sheet_Exist_CheckA3(G_xls_Output_Dir_old.SubString(1,G_xls_Output_Dir_old.LastDelimiter("\\")), pcHeadsData);
       if ((d_RevNo.Trim() != "-01") && (d_RevNo.Trim() != "")){
          // 前回ﾌｧｲﾙﾊﾟｽ
		  //2023/4/21 製造番号桁数追加対応 Mod
		  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
		  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
          if (!FileExists(s_Text.c_str()))
          {
			  //2023/4/21 製造番号桁数追加対応 Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + d_RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
          }
          Write_Log("!?!" +  s_Text);
          //2023.02.08 作番桁数追加_S
          bFind = true;
          //2023.02.08 作番桁数追加_E
          Sheet_Compare_SyoriA3(s_Text);
       }
       else if (d_RevNo.Trim() == "-01"){
          d_RevNo = FormatFloat("00", G_HEADS_DATA_Rev - 1);
		  //2023/4/21 製造番号桁数追加対応 Mod
		  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
		  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
          bFlag = false;
          if (!FileExists(s_Text.c_str())){
			  //2023/4/21 製造番号桁数追加対応 Mod
			  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + G_SAKUBAN + s_SpecCode + ".xls";
			  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
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
				  //2023/4/21 製造番号桁数追加対応 Mod
				  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
				  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
                  if (!FileExists(s_Text.c_str())){
					  //2023/4/21 製造番号桁数追加対応 Mod
					  //s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + G_SAKUBAN + s_SpecCode + ".xls";
					  s_Text = G_xls_Output_Dir_old.SubString( 1 , G_xls_Output_Dir_old.LastDelimiter("\\")) + "\\000_登録\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls";
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
              Sheet_Compare_SyoriA3(s_Text);
          }
       }

       //2023.02.08 作番桁数追加_S
       if( (!bFind) && (G_SAKUBAN.Length() > 7) && (G_SAKUBAN.SubString(1, 2) == "10") ) {
          //前回部品展開ﾃﾞｰﾀ読込に失敗かつ８桁で入力された６桁時代前回部品展開ﾃﾞｰﾀ読込

          //********************************
          //**  ６桁時代出力ディレクトリ
          //********************************
          OLD_SAKUBAN = G_SAKUBAN.SubString(3, 6);

          Work = OLD_SAKUBAN.SubString(1, 4);
          i_Sakuban = Work.ToIntDef(0);
          if( i_Sakuban % 2 == 1 ){
             i_Sakuban --;
          }
          Work.sprintf("%04d",i_Sakuban);

          if (G_HEADS_DATA_Rev - 1 <= 0 ) {
             Rev.sprintf("%02d", 0);
          }else{
             Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
          }

          xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + OLD_SAKUBAN + "\\";

          d_RevNo=Sheet_Exist_CheckA3(xls_Output_Dir_old, pcHeadsData);
          if ((d_RevNo.Trim() != "-01") && (d_RevNo.Trim() != "")){
             // 前回ﾌｧｲﾙﾊﾟｽ
			 //2023/4/21 製造番号桁数追加対応 Mod
			 //s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + OLD_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
			 s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
             if (!FileExists(s_Text.c_str()))
             {
				//2023/4/21 製造番号桁数追加対応 Mod
				//s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + OLD_SAKUBAN + s_SpecCode + ".xls";
				s_Text = xls_Output_Dir_old + d_RevNo + "\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + ".xls";
             }
             Write_Log("!?!" +  s_Text);
             bFind = true;
             Sheet_Compare_SyoriA3(s_Text);
          }
          else if (d_RevNo.Trim() == "-01"){
             d_RevNo = FormatFloat("00", G_HEADS_DATA_Rev - 1);
			 //2023/4/21 製造番号桁数追加対応 Mod
			 //s_Text = xls_Output_Dir_old + "MAN" + OLD_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
			 s_Text = xls_Output_Dir_old + "MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
             bFlag = false;
             if (!FileExists(s_Text.c_str())){
				//2023/4/21 製造番号桁数追加対応 Mod
				//s_Text = xls_Output_Dir_old + "MAN" + OLD_SAKUBAN + s_SpecCode + ".xls";
				s_Text = xls_Output_Dir_old + "MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + ".xls";
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
				   //2023/4/21 製造番号桁数追加対応 Mod
				   //s_Text = xls_Output_Dir_old + "\\000_登録\\MAN" + OLD_SAKUBAN + s_SpecCode + "R" + d_RevNo + ".xls";
				   s_Text = xls_Output_Dir_old + "\\000_登録\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + "R" + d_RevNo + ".xls";
                   if (!FileExists(s_Text.c_str())){
					   //2023/4/21 製造番号桁数追加対応 Mod
					   //s_Text = xls_Output_Dir_old + "\\000_登録\\MAN" + OLD_SAKUBAN + s_SpecCode + ".xls";
					   s_Text = xls_Output_Dir_old + "\\000_登録\\MAN" + GetFileSeizouBangou(OLD_SAKUBAN) + s_SpecCode + ".xls";
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
                Sheet_Compare_SyoriA3(s_Text);
             }

          }

       }
       //2023.02.08 作番桁数追加_E
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
void __fastcall Sheet_Compare_SyoriA3(AnsiString Excel_OldPass){
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
   if(!Sheet_Cell_CompareA3(6,5)){
      Excel_Cell_DataSet(6, 19, "*");
      // 2020.05.28 温水乾燥注記追加_S
      if ( Excel_Cell_DataGet (18, 36) == "" ) {
      } else if ( Excel_Cell_DataGet (18, 36) == "'" ) {
      } else {
         Excel_Cell_DataSet(18, 55, "*");
      }
      // 2020.05.28 温水乾燥注記追加_E
   }

   //***************************
   //***  納　期
   //***************************
   // 『A009』
   if(!Sheet_Cell_CompareA3(5,23)){
      Excel_Cell_DataSet(5, 28, "*");
   }

   //*******************
   //***  台  数
   //*******************
   // 『A010』
   if(!Sheet_Cell_CompareA3(6, 23)){
      Excel_Cell_DataSet(6, 28, "*");
      // 2020.05.28 温水乾燥注記追加_S
      if ( Excel_Cell_DataGet (18, 36) == "" ) {
      } else if ( Excel_Cell_DataGet (18, 36) == "'" ) {
      } else {
         Excel_Cell_DataSet(18, 55, "*");
      }
      // 2020.05.28 温水乾燥注記追加_E
   }

   //***************************
   //***  注 文 先
   //***************************
   // 『A011』
   if(!Sheet_Cell_CompareA3(7, 5)){
      Excel_Cell_DataSet(7, 26, "*");
   }

   //***************************
   //***  塗 装 色
   //***************************
   // 『A014』
   if(!Sheet_Cell_CompareA3(8, 6)){
      Excel_Cell_DataSet(8, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｂ欄　　比較                               ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //*******************************************
   //***  B側ﾌﾟﾚｰﾄ編成 1 ＆ B側ﾌﾟﾚｰﾄ編成 2
   //*******************************************
   // 『B003』
   if(!Sheet_Cell_CompareA3(10, 7)){
      Excel_Cell_DataSet(10, 13, "*");
   }

   //*******************************************
   //***  A側ﾌﾟﾚｰﾄ編成 1 ＆ A側ﾌﾟﾚｰﾄ編成 2
   //*******************************************
   // 『B004』
   if(!Sheet_Cell_CompareA3(10, 15)){
      Excel_Cell_DataSet(10, 26, "*");
   }

   //***************************
   //***  ﾌﾟﾚｰﾄ材質
   //***************************
   // 『B005』
   if(!Sheet_Cell_CompareA3(11, 7)){
      Excel_Cell_DataSet(11, 26, "*");
   }

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B006』
   if(!Sheet_Cell_CompareA3(12, 7)){
      Excel_Cell_DataSet(12, 13, "*");
   }

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B007』
   if(!Sheet_Cell_CompareA3(12, 15)){
      Excel_Cell_DataSet(12, 26, "*");
   }

   ////2011.03.11 工程写真要注記追加
   ////***************************
   ////***  工程写真要
   ////***************************
   //if(!Sheet_Cell_Compare(12, 18)){
   //   Excel_Cell_DataSet(12, 26, "*");
   //}
   //*****************************

   //***********************************
   //***  B側ﾉｽﾞﾙ材質 ＆ B側ﾉｽﾞﾙ色
   //***********************************
   // 『B008』,『B009』
   if(!Sheet_Cell_CompareA3(13, 7) || !Sheet_Cell_CompareA3(13, 11)){
      Excel_Cell_DataSet(13, 13, "*");
   }

   //***********************************
   //***  A側ﾉｽﾞﾙ材質 ＆ A側ﾉｽﾞﾙ色
   //***********************************
   // 『B010』,『B011』
   if(!Sheet_Cell_CompareA3(13, 15) || !Sheet_Cell_CompareA3(13, 24)){
      Excel_Cell_DataSet(13, 26, "*");
   }

   //***************************
   //***  S1継手・口径
   //***************************
   // 『B012』
   if(!Sheet_Cell_CompareA3(14, 8)){
      Excel_Cell_DataSet(14, 13, "*");
   }

   //***************************
   //***  S2継手・口径
   //***************************
   // 『B013』
   if(!Sheet_Cell_CompareA3(14, 16)){
      Excel_Cell_DataSet(14, 26, "*");
   }

   //***************************
   //***  S1温度計
   //***  B側S1温度計座材質
   //***************************
   // 『B014』,『B015』
   if(!Sheet_Cell_CompareA3(15, 7) || !Sheet_Cell_CompareA3(15, 11)){
      Excel_Cell_DataSet(15, 13, "*");
   }

   //***************************
   //***  S2温度計
   //***  A側S2温度計座材質
   //***************************
   // 『B016』,『B017』
   if(!Sheet_Cell_CompareA3(15, 15) || !Sheet_Cell_CompareA3(15, 24)){
      Excel_Cell_DataSet(15, 26, "*");
   }

   //***************************
   //***  S1圧力計
   //***************************
   // 『B018』
   if(!Sheet_Cell_CompareA3(16, 7)){
      Excel_Cell_DataSet(16, 13, "*");
   }

   //***************************
   //***  S2圧力計
   //***************************
   // 『B019』
   if(!Sheet_Cell_CompareA3(16, 15)){
      Excel_Cell_DataSet(16, 26, "*");
   }

   //***************************
   //***  S1ｴｱ抜継手名和
   //***************************
   // 『B020』
   if(!Sheet_Cell_CompareA3(17, 7)){
      Excel_Cell_DataSet(17, 13, "*");
   }

   //***************************
   //***  S2ｴｱ抜継手名和
   //***************************
   // 『B021』
   if(!Sheet_Cell_CompareA3(17, 15)){
      Excel_Cell_DataSet(17, 26, "*");
   }

   //***************************
   //***  S3継手・口径
   //***************************
   // 『B022』
   if(!Sheet_Cell_CompareA3(18, 8)){
      Excel_Cell_DataSet(18, 13, "*");
   }

   //***************************
   //***  S4継手・口径
   //***************************
   // 『B023』
   if(!Sheet_Cell_CompareA3(18, 16)){
      Excel_Cell_DataSet(18, 26, "*");
   }

   //***************************
   //***  S3温度計
   //***  S3ｽﾁｰﾑ区分
   //***************************
   // 『B024』『B025』
   if(!Sheet_Cell_CompareA3(19, 7) || !Sheet_Cell_CompareA3(19, 11)){
      Excel_Cell_DataSet(19, 13, "*");
   }

   //***************************
   //***  S4温度計
   //***  S4ｽﾁｰﾑ区分
   //***************************
   // 『B026』『B027』
   if(!Sheet_Cell_CompareA3(19, 15) || !Sheet_Cell_CompareA3(19, 24)){
      Excel_Cell_DataSet(19, 26, "*");
   }

   //***************************
   //***  S3圧力計
   //***************************
   // 『B028』
   if(!Sheet_Cell_CompareA3(20, 7)){
      Excel_Cell_DataSet(20, 13, "*");
   }

   //***************************
   //***  S4圧力計
   //***************************
   // 『B029』
   if(!Sheet_Cell_CompareA3(20, 15)){
      Excel_Cell_DataSet(20, 26, "*");
   }

   //***************************
   //***  S3液体継手名和
   //***************************
   // 『B030』
   if(!Sheet_Cell_CompareA3(21, 7)){
      Excel_Cell_DataSet(21, 13, "*");
   }

   //***************************
   //***  S4液体継手名和
   //***************************
   // 『B031』
   if(!Sheet_Cell_CompareA3(21, 15)){
      Excel_Cell_DataSet(21, 26, "*");
   }

   //***************************
   //***  B側端管材質
   //***************************
   // 『B032』
   if(!Sheet_Cell_CompareA3(22, 7)){
      Excel_Cell_DataSet(22, 13, "*");
   }

   //***************************
   //***  A側端管材質
   //***************************
   // 『B033』
   if(!Sheet_Cell_CompareA3(22, 15)){
      Excel_Cell_DataSet(22, 26, "*");
   }

   //***************************
   //***  B側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B034』
   if(!Sheet_Cell_CompareA3(23, 7)){
      Excel_Cell_DataSet(23, 13, "*");
   }

   //***************************
   //***  A側ｶﾞｽｹｯﾄ材質
   //***************************
   // 『B035』
   if(!Sheet_Cell_CompareA3(23, 15)){
      Excel_Cell_DataSet(23, 26, "*");
   }

   //***************************
   //***  B側ﾌﾗﾝｼﾞ材質
   //***************************
   // 『B036』
   if(!Sheet_Cell_CompareA3(24, 7)){
      Excel_Cell_DataSet(24, 13, "*");
   }

   //***************************
   //***  A側ﾌﾗﾝｼﾞ材質
   //***************************
   // 『B037』
   if(!Sheet_Cell_CompareA3(24, 15)){
      Excel_Cell_DataSet(24, 26, "*");
   }

   //***************************
   //***  B側仕上名称
   //***************************
   // 『B039』
   if(!Sheet_Cell_CompareA3(26, 7)){
      Excel_Cell_DataSet(26, 13, "*");
   }

   //***************************
   //***  A側仕上名称
   //***************************
   // 『B040』
   if(!Sheet_Cell_CompareA3(26, 15)){
      Excel_Cell_DataSet(26, 26, "*");
   }

   //***************************
   //***  B側保護管材質
   //***************************
   // 『B041』
   if(!Sheet_Cell_CompareA3(25, 7)){
      Excel_Cell_DataSet(25, 13, "*");
   }

   //***************************
   //***  A側保護管材質
   //***************************
   // 『B042』
   if(!Sheet_Cell_CompareA3(25, 15)){
      Excel_Cell_DataSet(25, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｃ欄　　比較                               ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //**********************************************************************
   //***  締付ボルト材質
   //**********************************************************************
   // 『C001』,『C002』,『C003』,
   if(!Sheet_Cell_CompareA3(27, 8) || !Sheet_Cell_CompareA3(27, 12) || !Sheet_Cell_CompareA3(27, 24)){
      Excel_Cell_DataSet(27, 26, "*");
   }

   //**********************************************************************
   //***  締付ボルト仕上げ
   //**********************************************************************
   // 『C004』,『C005』,『C006』,
   if(!Sheet_Cell_CompareA3(28, 7) || !Sheet_Cell_CompareA3(28, 12) || !Sheet_Cell_CompareA3(28, 24)){
      Excel_Cell_DataSet(28, 26, "*");
   }

   //*******************************
   //***  ﾌﾚｰﾑ材質 材質,厚さ,幅
   //*******************************
   // 『C007』,『C008』,『C009』
   if(!Sheet_Cell_CompareA3(32, 8) || !Sheet_Cell_CompareA3(32, 15) || !Sheet_Cell_CompareA3(32, 22)){
      Excel_Cell_DataSet(32, 26, "*");
   }

   //*******************************
   //***  ガイドバー
   //*******************************
   // 『C010』,『C011』,『C012』,『C013』,『C014』
   if(!Sheet_Cell_CompareA3(34, 7) || !Sheet_Cell_CompareA3(34, 10) || !Sheet_Cell_CompareA3(34, 11) ||
      !Sheet_Cell_CompareA3(34, 12)|| !Sheet_Cell_CompareA3(34, 16)){
      Excel_Cell_DataSet(34, 26, "*");
   }

   //*******************************
   //***  ガイドバー
   //*******************************
   // 『C015』,『C016』,『C017』
   if(!Sheet_Cell_CompareA3(35, 7) || !Sheet_Cell_CompareA3(35, 18) || !Sheet_Cell_CompareA3(35, 24)){
      Excel_Cell_DataSet(35, 26, "*");
   }

   //*******************************
   //***  ボルトカバーBLCP1
   //*******************************
   // 『C018』,『C019』,『C020』
   if(!Sheet_Cell_CompareA3(29, 7) || !Sheet_Cell_CompareA3(29, 10) || !Sheet_Cell_CompareA3(29, 24)){
      Excel_Cell_DataSet(29, 26, "*");
   }

   //*******************************
   //***  ボルトカバーBLCEL
   //*******************************
   // 『C021』,『C022』
   if(!Sheet_Cell_CompareA3(30, 10) || !Sheet_Cell_CompareA3(30, 24)){
      Excel_Cell_DataSet(30, 26, "*");
   }

   //*******************************
   //***  ボルトカバーBLCES
   //*******************************
   // 『C023』,『C024』
   if(!Sheet_Cell_CompareA3(31, 10) || !Sheet_Cell_CompareA3(31, 24)){
      Excel_Cell_DataSet(31, 26, "*");
   }

   //*******************************
   //***  アンカーボルト種類
   //*******************************
   // 『C025』
   if(!Sheet_Cell_CompareA3(36, 8)){
      Excel_Cell_DataSet(36, 26, "*");
   }

   //*******************************
   //***  アンカーボルト長さ,員数,材質,仕上
   //*******************************
   // 『C026』,『C027』,『C028』
   if(!Sheet_Cell_CompareA3(37, 8) || !Sheet_Cell_CompareA3(37, 15) || !Sheet_Cell_CompareA3(37, 23)){
      Excel_Cell_DataSet(37, 26, "*");
   }

   //*******************************
   //***  Sﾌﾚｰﾑ孔明
   //*******************************
   // 『C029』
   if(!Sheet_Cell_CompareA3(33, 1)){
      Excel_Cell_DataSet(33, 26, "*");
   }

   //*******************************
   //***  工程写真要
   //*******************************
   // 『C030』
   if(!Sheet_Cell_CompareA3(40, 18)){
      Excel_Cell_DataSet(40, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          Ｄ欄　　比較                               ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //***************************
   //***  装置番号／船番
   //***************************
   // 『D001』
   if(!Sheet_Cell_CompareA3(42, 9)){
      Excel_Cell_DataSet(42, 13, "*");
   }

   //***************************
   //***  機器番号
   //***************************
   // 『D002』
   if(!Sheet_Cell_CompareA3(42, 18)){
      Excel_Cell_DataSet(42, 26, "*");
   }

   //***************************
   //***  B側設計圧力
   //***************************
   // 『D003』
   if(!Sheet_Cell_CompareA3(44, 6)){
      Excel_Cell_DataSet(44, 13, "*");
   }

   //***************************
   //***  B側設計温度
   //***************************
   // 『D004』
   if(!Sheet_Cell_CompareA3(45, 6)){
      Excel_Cell_DataSet(45, 13, "*");
   }

   //***************************
   //***  B側試験圧力 空圧
   //***************************
   // 『D005』
   if(!Sheet_Cell_CompareA3(46, 6)){
      Excel_Cell_DataSet(46, 13, "*");
   }

   //***************************
   //***  B側試験圧力 水圧
   //***************************
   // 『D006』
   if(!Sheet_Cell_CompareA3(47, 6)){
      Excel_Cell_DataSet(47, 13, "*");
   }

   //***************************
   //***  B側内容量
   //***************************
   // 『D007』
   if(!Sheet_Cell_CompareA3(49, 6)){
      Excel_Cell_DataSet(49, 13, "*");
   }

   //***************************
   //***  伝熱面積
   //***************************
   // 『D008』
   if(!Sheet_Cell_CompareA3(50, 6)){
      Excel_Cell_DataSet(50, 13, "*");
   }

   //***************************
   //***  A側設計圧力
   //***************************
   // 『D009』
   if(!Sheet_Cell_CompareA3(44, 14)){
      Excel_Cell_DataSet(44, 26, "*");
   }

   //***************************
   //***  A側設計温度
   //***************************
   // 『D010』
   if(!Sheet_Cell_CompareA3(45, 14)){
      Excel_Cell_DataSet(45, 26, "*");
   }

   //***************************
   //***  A側試験圧力 空圧
   //***************************
   // 『D011』
   if(!Sheet_Cell_CompareA3(46, 14)){
      Excel_Cell_DataSet(46, 26, "*");
   }

   //***************************
   //***  A側試験圧力 水圧
   //***************************
   // 『D012』
   if(!Sheet_Cell_CompareA3(47, 14)){
      Excel_Cell_DataSet(47, 26, "*");
   }

   //***************************
   //***  A側内容量
   //***************************
   // 『D013』
   if(!Sheet_Cell_CompareA3(49, 14)){
      Excel_Cell_DataSet(49, 26, "*");
   }

   //***************************
   //***  機器質量
   //***************************
   // 『D014』
   if(!Sheet_Cell_CompareA3(50, 17)){
      Excel_Cell_DataSet(50, 26, "*");
   }

   //************************
   //***  適用規格
   //************************
   // 『D015』
   if(!Sheet_Cell_CompareA3(51, 6)){
      Excel_Cell_DataSet(51, 26, "*");
   }

   //***************************
   //***  締付寸法　Max,Min
   //***************************
   // 『D016』,『D017』
   if(!Sheet_Cell_CompareA3(52, 9) || !Sheet_Cell_CompareA3(52, 15)){
      Excel_Cell_DataSet(52, 26, "*");
   }

   //***************************
   //***  B側流量
   //***************************
   // 『D018』,『D019』,『D020』
   if(!Sheet_Cell_CompareA3(48, 4) || !Sheet_Cell_CompareA3(48, 6) || !Sheet_Cell_CompareA3(48, 11)){
      Excel_Cell_DataSet(48, 13, "*");
   }

   //***************************
   //***  A側流量
   //***************************
   // 『D021』,『D022』
   if(!Sheet_Cell_CompareA3(48, 14) || !Sheet_Cell_CompareA3(48, 24)){
      Excel_Cell_DataSet(48, 26, "*");
   }

   //***************************
   //***  ステッカー
   //***************************
   // 『D023』,『D024』
   if(!Sheet_Cell_CompareA3(53, 18) || !Sheet_Cell_CompareA3(53, 24)){
      Excel_Cell_DataSet(53, 26, "*");
   }

   //************************
   //***  機器名称
   //************************
   // 『D025』
   if(!Sheet_Cell_CompareA3(41, 9)){
      Excel_Cell_DataSet(41, 26, "*");
   }

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          E欄　　比較                                ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //************************
   //***  Sフレームアイプレート付
   //************************
   // 『E001』
   if(!Sheet_Cell_CompareA3(2, 36)){
      Excel_Cell_DataSet(2, 55, "*");
      /*
      Excel_Cell_SetUnderline(2, 36);
      Excel_Cell_SetUnderline(2, 35);
      Excel_Cell_SetUnderline(2, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(2, 30);
      Excel_Cell_SetUnderline(2, 32);
      Excel_Cell_SetUnderline(2, 56);
      Excel_Cell_SetUnderline(2, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(2, 30, 2, 57);
   }

   //************************
   //***  流体名銘板付
   //************************
   // 『E002』
   if(!Sheet_Cell_CompareA3(3, 36)){
      Excel_Cell_DataSet(3, 55, "*");
      /*
      Excel_Cell_SetUnderline(3, 36);
      Excel_Cell_SetUnderline(3, 35);
      Excel_Cell_SetUnderline(3, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(3, 30);
      Excel_Cell_SetUnderline(3, 32);
      Excel_Cell_SetUnderline(3, 56);
      Excel_Cell_SetUnderline(3, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(3, 30, 3, 57);
   }

   //************************
   //***  注記(禁油１)
   //************************
   // 『E003』
   if(!Sheet_Cell_CompareA3(4, 36)){
      Excel_Cell_DataSet(4, 55, "*");
      /*Excel_Cell_SetUnderline(4, 36);
      Excel_Cell_SetUnderline(4, 35);
      Excel_Cell_SetUnderline(4, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(4, 30);
      Excel_Cell_SetUnderline(4, 32);
      Excel_Cell_SetUnderline(4, 56);
      Excel_Cell_SetUnderline(4, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(4, 30, 4, 57);
   }

   //************************
   //***  注記(ﾉｽﾞﾙ１)
   //************************
   // 『E004』
   if(!Sheet_Cell_CompareA3(5, 36)){
      Excel_Cell_DataSet(5, 55, "*");
      /*Excel_Cell_SetUnderline(5, 36);
      Excel_Cell_SetUnderline(5, 35);
      Excel_Cell_SetUnderline(5, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(5, 30);
      Excel_Cell_SetUnderline(5, 32);
      Excel_Cell_SetUnderline(5, 56);
      Excel_Cell_SetUnderline(5, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(5, 30, 5, 57);
   }

   //************************
   //***  注記(ﾉｽﾞﾙ２)
   //************************
   // 『E005』
   if(!Sheet_Cell_CompareA3(6, 36)){
      Excel_Cell_DataSet(6, 55, "*");
      /*
      Excel_Cell_SetUnderline(6, 36);
      Excel_Cell_SetUnderline(6, 35);
      Excel_Cell_SetUnderline(6, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(6, 30);
      Excel_Cell_SetUnderline(6, 32);
      Excel_Cell_SetUnderline(6, 56);
      Excel_Cell_SetUnderline(6, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(6, 30, 6, 57);
   }

   //************************
   //***  注記(客先仕様１)
   //************************
   // 『E006』
   if(!Sheet_Cell_CompareA3(7, 36)){
      Excel_Cell_DataSet(7, 55, "*");
      /*
      Excel_Cell_SetUnderline(7, 36);
      Excel_Cell_SetUnderline(7, 35);
      Excel_Cell_SetUnderline(7, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(7, 30);
      Excel_Cell_SetUnderline(7, 32);
      Excel_Cell_SetUnderline(7, 56);
      Excel_Cell_SetUnderline(7, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(7, 30, 7, 57);
   }

   //************************
   //***  注記(客先仕様２)
   //************************
   // 『E007』
   if(!Sheet_Cell_CompareA3(8, 36)){
      Excel_Cell_DataSet(8, 55, "*");
      /*
      Excel_Cell_SetUnderline(8, 36);
      Excel_Cell_SetUnderline(8, 35);
      Excel_Cell_SetUnderline(8, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(8, 30);
      Excel_Cell_SetUnderline(8, 32);
      Excel_Cell_SetUnderline(8, 56);
      Excel_Cell_SetUnderline(8, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(8, 30, 8, 57);
   }

   //************************
   //***  注記(ガスケット)
   //************************
   // 『E008』
   if(!Sheet_Cell_CompareA3(9, 36)){
      Excel_Cell_DataSet(9, 55, "*");
      /*
      Excel_Cell_SetUnderline(9, 36);
      Excel_Cell_SetUnderline(9, 35);
      Excel_Cell_SetUnderline(9, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(9, 30);
      Excel_Cell_SetUnderline(9, 32);
      Excel_Cell_SetUnderline(9, 56);
      Excel_Cell_SetUnderline(9, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(9, 30, 9, 57);
   }

   //************************
   //***  注記(ﾎﾞﾙﾄ枚数１)
   //************************
   // 『E009』
   if(!Sheet_Cell_CompareA3(10, 36)){
      Excel_Cell_DataSet(10, 55, "*");
      /*
      Excel_Cell_SetUnderline(10, 36);
      Excel_Cell_SetUnderline(10, 35);
      Excel_Cell_SetUnderline(10, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(10, 30);
      Excel_Cell_SetUnderline(10, 32);
      Excel_Cell_SetUnderline(10, 56);
      Excel_Cell_SetUnderline(10, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(10, 30, 10, 57);
   }

   //************************
   //***  注記(舶用工具)
   //************************
   // 『E010』
   if(!Sheet_Cell_CompareA3(11, 36)){
      Excel_Cell_DataSet(11, 55, "*");
      /*
      Excel_Cell_SetUnderline(11, 36);
      Excel_Cell_SetUnderline(11, 35);
      Excel_Cell_SetUnderline(11, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(11, 30);
      Excel_Cell_SetUnderline(11, 32);
      Excel_Cell_SetUnderline(11, 56);
      Excel_Cell_SetUnderline(11, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(11, 30, 11, 57);
   }

   //************************
   //***  注記(ﾄﾞﾚﾝﾊﾟﾝ)
   //************************
   // 『E011』
   if(!Sheet_Cell_CompareA3(12, 36)){
      Excel_Cell_DataSet(12, 55, "*");
      /*Excel_Cell_SetUnderline(12, 36);
      Excel_Cell_SetUnderline(12, 35);
      Excel_Cell_SetUnderline(12, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(12, 30);
      Excel_Cell_SetUnderline(12, 32);
      Excel_Cell_SetUnderline(12, 56);
      Excel_Cell_SetUnderline(12, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(12, 30, 12, 57);
   }

   //************************
   //***  注記(ラチェットスパナ)
   //************************
   // 『E012』
   if(!Sheet_Cell_CompareA3(13, 36)){
      Excel_Cell_DataSet(13, 55, "*");
      /*
      Excel_Cell_SetUnderline(13, 36);
      Excel_Cell_SetUnderline(13, 35);
      Excel_Cell_SetUnderline(13, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(13, 30);
      Excel_Cell_SetUnderline(13, 32);
      Excel_Cell_SetUnderline(13, 56);
      Excel_Cell_SetUnderline(13, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(13, 30, 13, 57);
   }

   //************************
   //***  注記(電解研磨１)
   //************************
   // 『E013』
   if(!Sheet_Cell_CompareA3(14, 36)){
      Excel_Cell_DataSet(14, 55, "*");
      /*
      Excel_Cell_SetUnderline(14, 36);
      Excel_Cell_SetUnderline(14, 35);
      Excel_Cell_SetUnderline(14, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(14, 30);
      Excel_Cell_SetUnderline(14, 32);
      Excel_Cell_SetUnderline(14, 56);
      Excel_Cell_SetUnderline(14, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(14, 30, 14, 57);
   }

   //************************
   //***  注記(ﾌﾟﾚｰﾄ枚数１)
   //************************
   // 『E014』
   if(!Sheet_Cell_CompareA3(15, 36)){
      Excel_Cell_DataSet(15, 55, "*");
      /*
      Excel_Cell_SetUnderline(15, 36);
      Excel_Cell_SetUnderline(15, 35);
      Excel_Cell_SetUnderline(15, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(15, 30);
      Excel_Cell_SetUnderline(15, 32);
      Excel_Cell_SetUnderline(15, 56);
      Excel_Cell_SetUnderline(15, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(15, 30, 15, 57);
   }

   //************************
   //***  注記(相ﾌﾗﾝｼﾞ)
   //************************
   // 『E015』
   if(!Sheet_Cell_CompareA3(16, 36)){
      Excel_Cell_DataSet(16, 55, "*");
      /*
      Excel_Cell_SetUnderline(16, 36);
      Excel_Cell_SetUnderline(16, 35);
      Excel_Cell_SetUnderline(16, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(16, 30);
      Excel_Cell_SetUnderline(16, 32);
      Excel_Cell_SetUnderline(16, 56);
      Excel_Cell_SetUnderline(16, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(16, 30, 16, 57);
   }

   //************************
   //***  注記(予備品)
   //************************
   // 『E016』
   if(!Sheet_Cell_CompareA3(17, 36)){
      Excel_Cell_DataSet(17, 55, "*");
      /*
      Excel_Cell_SetUnderline(17, 36);
      Excel_Cell_SetUnderline(17, 35);
      Excel_Cell_SetUnderline(17, 55);
      //2019.07.25 製作指示書書式変更_S
      Excel_Cell_SetUnderline(17, 30);
      Excel_Cell_SetUnderline(17, 32);
      Excel_Cell_SetUnderline(17, 56);
      Excel_Cell_SetUnderline(17, 58);
      //2019.07.25 製作指示書書式変更_E
      */
      Excel_Cell_SetUnderline(17, 30, 17, 57);
   }

   // 2020.06.19 温水乾燥注記追加_S
   //***************************
   //***    注記(温水乾燥)
   //***************************
   // 『E017』
   if(!Sheet_Cell_CompareA3(18, 36)){
      Excel_Cell_DataSet(18, 55, "*");
      Excel_Cell_SetUnderline(18, 30, 18, 57);
   }

   //2020.06.19 小径締付ﾎﾞﾙﾄ追加_S
   //************************
   //***  注記(小径締付ﾎﾞﾙﾄ)
   //************************
   // 『E018』
   if(!Sheet_Cell_CompareA3(19, 36)){
      Excel_Cell_DataSet(19, 55, "*");
      Excel_Cell_SetUnderline(19, 30, 19, 57);
   }

   //************************
   //***  注記(小径ﾎﾞﾙﾄｶﾊﾞｰBLCP1S)
   //************************
   // 『E019』
   if(!Sheet_Cell_CompareA3(20, 36)){
      Excel_Cell_DataSet(20, 55, "*");
      Excel_Cell_SetUnderline(20, 30, 20, 57);
   }

   //************************
   //***  注記(小径ﾎﾞﾙﾄｶﾊﾞｰBLCESS)
   //************************
   // 『E020』
   if(!Sheet_Cell_CompareA3(21, 36)){
      Excel_Cell_DataSet(21, 55, "*");
      Excel_Cell_SetUnderline(21, 30, 21, 57);
   }
   // 2020.06.19 温水乾燥注記追加_E

   // 2020.10.30 圧力容器UNS規格警告追加_S
   //************************
   //***  注記(圧力容器UNS規格警告)
   //************************
   // 『E021』
   if(!Sheet_Cell_CompareA3(22, 36)){
      Excel_Cell_DataSet(22, 55, "*");
      Excel_Cell_SetUnderline(22, 30, 22, 57);
   }
   // 2020.10.30 圧力容器UNS規格警告追加_E

   // 2021.10.19 注記追加_S
   //************************
   //***  ダイトク注記
   //************************
   // 『E022』
   if(!Sheet_Cell_CompareA3(23, 36)){
      Excel_Cell_DataSet(23, 55, "*");
      Excel_Cell_SetUnderline(23, 30, 23, 57);
   }
   // 2021.10.19 注記追加_E

   // 2022.04.04 機器番号機器名称特記追加_S
   //************************
   //***  機器番号
   //************************
   // 『E023』
   if(!Sheet_Cell_CompareA3(24, 36)){
      Excel_Cell_DataSet(24, 55, "*");
      Excel_Cell_SetUnderline(24, 30, 24, 57);
   }
   // 2022.04.04 機器番号機器名称特記追加_E

   // 2022.04.05 就航船名特記追加_S
   //************************
   //***  機器番号
   //************************
   // 『E024』
   if(!Sheet_Cell_CompareA3(25, 36)){
      Excel_Cell_DataSet(25, 55, "*");
      Excel_Cell_SetUnderline(25, 30, 25, 57);
   }
   // 2022.04.05 就航船名特記追加_E


   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                          G欄　　比較                                ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   //************************
   //***  スパナ数量
   //************************
   // 『G001』
   if(!Sheet_Cell_CompareA3(44, 44)){
      Excel_Cell_DataSet(44, 55, "*");
   }

   //************************
   //***  洗浄用ブラシ数量
   //************************
   // 『G002』
   if(!Sheet_Cell_CompareA3(45, 44)){
      Excel_Cell_DataSet(45, 55, "*");
   }

   //************************
   //***  ゲージプレート　L
   //************************
   // 『G003』
   //2019.07.25 製作指示書書式変更_S
   //if(!Sheet_Cell_CompareA3(46, 44)){
   if(!Sheet_Cell_CompareA3(46, 40)){
   //2019.07.25 製作指示書書式変更_E
      Excel_Cell_DataSet(46, 55, "*");
   }

   //************************
   //***  ゲージプレート　Max
   //************************
   // 『G004』
   //2019.07.25 製作指示書書式変更_S
   //if(!Sheet_Cell_CompareA3(47, 44)){
   if(!Sheet_Cell_CompareA3(47, 40) || !Sheet_Cell_CompareA3(47, 44)){
   //2019.07.25 製作指示書書式変更_E
      Excel_Cell_DataSet(47, 55, "*");
   }

   //************************
   //***  ゲージプレート　Min
   //************************
   // 『G005』
   //2019.07.25 製作指示書書式変更_S
   //if(!Sheet_Cell_CompareA3(48, 44)){
   if(!Sheet_Cell_CompareA3(48, 40)){
   //2019.07.25 製作指示書書式変更_E
      Excel_Cell_DataSet(48, 55, "*");
   }

   //************************
   //***  移動止め
   //************************
   // 『G006』
   if(!Sheet_Cell_CompareA3(49, 44)){
      Excel_Cell_DataSet(49, 55, "*");
   }

   //************************
   //***  S1ノズルオリエンテーション
   //************************
   // 『G007』
   if(!Sheet_Cell_CompareA3(51, 37)){
      Excel_Cell_DataSet(51, 42, "*");
   }

   //************************
   //***  S2ノズルオリエンテーション
   //************************
   // 『G008』
   if(!Sheet_Cell_CompareA3(51, 45)){
      Excel_Cell_DataSet(51, 55, "*");
   }

   //************************
   //***  S3ノズルオリエンテーション
   //************************
   // 『G009』
   if(!Sheet_Cell_CompareA3(52, 37)){
      Excel_Cell_DataSet(52, 42, "*");
   }

   //************************
   //***  S4ノズルオリエンテーション
   //************************
   // 『G010』
   if(!Sheet_Cell_CompareA3(52, 45)){
      Excel_Cell_DataSet(52, 55, "*");
   }

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
bool __fastcall Sheet_Cell_CompareA3(int Row, int Col){
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

//   // 書式変更したため、前回帳票が旧書式の場合、比較位置をずらす
//   if ( Row >= 9 && Row <= 11 ) {        // プレート及びガスケット仕様部分（書式変更による影響範囲）
//    //exWorkCell_old = exWorksheet_old.Exec(PropertyGet("Cells") << 8 << 13);
//    //Oldsht_Value = exWorkCell_old.Exec(PropertyGet("Value"));
//    //Windows 2008Server 移植対応
//    exWorkCell_old = exWorksheet_old.OlePropertyGet("Cells",8,13);
//    Oldsht_Value = exWorkCell_old.OlePropertyGet("Value");
//    if ( Oldsht_Value != "" ) {   //８行目の１３桁目に”（□はチェック項目です）”文字あり ＝　旧書式
//       OldLayout = 1;         // オフセット値を設定
//    }
//   }

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
AnsiString __fastcall Sheet_Exist_CheckA3(AnsiString path, HeadsData* pcHeadsData){
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
	  //2023/4/21 製造番号桁数追加対応 Mod
	  //SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + sSpecCode + "R" + RevNo + ".xls";
	  SheetPass = path + RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + "R" + RevNo + ".xls";
      G_Log_Text = "SheetPass?『 " + SheetPass + " 』";
      Write_Log(G_Log_Text);
      if (FileExists(SheetPass.c_str()))
      {
          bFlag = true;
      }
      if (!bFlag) {
		  //2023/4/21 製造番号桁数追加対応 Mod
		  //SheetPass = path + RevNo + "\\MAN" + G_SAKUBAN + sSpecCode + ".xls";
		  SheetPass = path + RevNo + "\\MAN" + GetFileSeizouBangou(G_SAKUBAN) + sSpecCode + ".xls";
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
AnsiString __fastcall Get_SiageMeisyouA3(AnsiString Code){
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
AnsiString __fastcall Get_Zaisitu_NameA3(AnsiString Code){
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
AnsiString __fastcall Get_AturyokuKeiA3(AnsiString s_Code){
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
      // 2020.07.17 圧力計変更_S
      //s_Aturyokukei = "SS/TFｶｸﾏｸ";
      s_Aturyokukei = "C.S./TFｶｸﾏｸ";
      // 2020.07.17 圧力計変更_E

   } else if (s_Code.ToIntDef(0) == 7){
      s_Aturyokukei = "SUSｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 8){
      // 2020.07.17 圧力計変更_S
      //s_Aturyokukei = "SS/TFｶｸﾏｸ";
      s_Aturyokukei = "C.S./TFｶｸﾏｸ";
      // 2020.07.17 圧力計変更_E

   } else if (s_Code.ToIntDef(0) == 9){
      s_Aturyokukei = "SUSｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 10){
      // 2020.07.17 圧力計変更_S
      //s_Aturyokukei = "SS/TFｶｸﾏｸ";
      s_Aturyokukei = "C.S./TFｶｸﾏｸ";
      // 2020.07.17 圧力計変更_E

   } else if (s_Code.ToIntDef(0) == 11){
      s_Aturyokukei = "SUSｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 12){
      // 2020.07.17 圧力計変更_S
      //s_Aturyokukei = "SS/TFｶｸﾏｸ";
      s_Aturyokukei = "C.S./TFｶｸﾏｸ";
      // 2020.07.17 圧力計変更_E

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

   // 2020.07.17 圧力計変更_S
   } else if (s_Code.ToIntDef(0) == 21){
      s_Aturyokukei = "BCﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 22){
      s_Aturyokukei = "SUSﾌﾞﾙﾄﾞﾝ";

   } else if (s_Code.ToIntDef(0) == 23){
      s_Aturyokukei = "SUS・C.S.ｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 24){
      s_Aturyokukei = "SUS・C.S.ｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 25){
      s_Aturyokukei = "SUS・C.S.ｶｸﾏｸ";

   } else if (s_Code.ToIntDef(0) == 26){
      s_Aturyokukei = "SUS・C.S.ｶｸﾏｸ";
   // 2020.07.17 圧力計変更_E

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
AnsiString __fastcall Get_Tankan_DataA3(AnsiString Syurui, HeadsData* pcHeadsData)
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

   //2019.07.25 製作指示書書式変更_S
   int HEADSM[8] = { 893, 908, 923, 938, 953, 968, 983, 998 };

   AnsiString s_NzCd;         // ﾉｽﾞﾙ継手ｺｰﾄﾞ
   AnsiString s_KeyMat;       // ﾘﾝｸﾞ材質種類（ﾘﾝｸﾞ端管のSTPG）
   AnsiString s_MatCD;        // ﾌﾞｰﾂﾌﾗｸﾞ
   AnsiString s_ULdiv;        // ﾌﾗﾝｼﾞｺｰﾄﾞ

   AnsiString s_Frame;        // 型式種類
   AnsiString s_Ver;          // ﾊﾞｰｼﾞｮﾝNo.
   int i_Length;              // 文字数
     AnsiString s_Text;            // 汎用文字列
//   AnsiString s_RText;         // ﾘﾝｸﾞ注記　
//   AnsiString s_Mat;          // 端管材質
//   AnsiString s_BootFlg;      // ﾌﾞｰﾂﾌﾗｸﾞ
//   AnsiString s_FCode;        // ﾌﾗﾝｼﾞｺｰﾄﾞ


   int i_FL; //斜行流ﾌﾗｸﾞ(0:台形流,1:斜行流)

   //2019.07.05 製作指示書書式変更_E

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

   //2019.07.25 製作指示書書式変更_S
   if (s_HEADS_DATA[823].ToIntDef(0) == 15 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 10) {
      //CX-10D
      i_FL = 1; // 斜行流
   }
   else if (s_HEADS_DATA[823].ToIntDef(0) == 21 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 90) {
      //LX-90D
      i_FL = 1; // 斜行流
   }
   else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 80) {
      //YX-80
      i_FL = 1; // 斜行流
   }
   else if (s_HEADS_DATA[823].ToIntDef(0) == 111 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 83) {
      //YX-83
      i_FL = 1; // 斜行流
   }
   else {
      i_FL = 0; // 台形流
   }

   if( Syurui == "S1" ) {
      s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "S2" ) {
      s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "S3" ) {
      if ( i_FL == 1 ) {
         // 斜行流
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      } else {
         // 台形流
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      }
      s_ULdiv = "L";
   } else if( Syurui == "S4" ) {
      if ( i_FL == 1 ) {
         // 斜行流
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      } else {
         // 台形流
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      }
      s_ULdiv = "L";
   } else if( Syurui == "E1" ) {
      s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "E2" ) {
      s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      s_ULdiv = "U";
   } else if( Syurui == "E3" ) {
      if ( i_FL == 1 ) {
         // 斜行流
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      } else {
         // 台形流
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      }
      s_ULdiv = "L";
   } else if( Syurui == "E4" ) {
      if ( i_FL == 1 ) {
         // 斜行流
         s_MatCD = s_HEADS_DATA[1060].SubString(1, 4);
      } else {
         // 台形流
         s_MatCD = s_HEADS_DATA[1061].SubString(1, 4);
      }
      s_ULdiv = "L";
   }

   // リング材質種類
   s_KeyMat = Type_MAT( s_MatCD );

   // ノズル継手コード
   s_NzCd =   s_HEADS_DATA[HEADSM[i_HeadsNo]].SubString(1, 2);
   //2019.07.25 製作指示書書式変更_E

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

   //2019.07.25 製作指示書書式変更_S
   // ＤＢ検索
   // 2023.02.16 リング端管条件変更_S
   // 2021.01.06 CX(W)-03追加(CX(W)-00→CX(W)-01)_S
   //if ( P_Model == "CX-00" || P_Model == "CXW-00" ) {
   //   //CX-00 ﾘﾝｸﾞ端管
   //if ( P_Model == "CX-01" || P_Model == "CXW-01" ) {
   if (( P_Model == "CX-01" || P_Model == "CX-01D" || P_Model == "CXW-01" ) && s_HEADS_DATA[36].Pos("Y")==0 ){
   // 2023.02.16 リング端管条件変更_E
      //CX-00 ﾘﾝｸﾞ端管
   // 2021.01.06 CX(W)-03追加(CX(W)-00→CX(W)-01)_E
      if ( s_KeyMat == "ALBC" ) {
          // ＡＬＢＣ
      } else if ( s_KeyMat == "STPG" ) {
          // ＳＴＰＧ
      } else {
          //その他
          // 2020.05.22.ﾘﾝｸﾞ材質修正_S
          //s_KeyMat == "-";
          s_KeyMat = "-";
          // 2020.05.22.ﾘﾝｸﾞ材質修正_E
      }
      s_ULdiv = "-"; // 位置固定
      s_PGtyp = "-"; // コックなし
      s_TPtyp = "-"; // 温度計圧力計座なし
      s_DRtyp = "-"; // 液抜なし
      s_ARtyp = "-"; // エア抜なし

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else if  ( P_Model == "CX-10" || P_Model == "CX-10D" ) {
      //CX-10 ﾘﾝｸﾞ端管
      if ( s_KeyMat == "ALBC" ) {
          // ＡＬＢＣ
      } else if ( s_KeyMat == "STPG" ) {
          // ＳＴＰＧ
      } else {
          //その他
          // 2020.05.22.ﾘﾝｸﾞ材質修正_S
          //s_KeyMat == "-";
          s_KeyMat = "-";
          // 2020.05.22.ﾘﾝｸﾞ材質修正_E
      }
      s_ULdiv = "-"; // 位置固定
      s_PGtyp = "-"; // コックなし
      s_TPtyp = "-"; // 温度計圧力計座なし
      s_DRtyp = "-"; // 液抜なし
      s_ARtyp = "-"; // エア抜なし

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else if ( ( P_Model == "SX-30" || P_Model == "SX-30S" ) && s_NzCd == "41" ) {
      //SX-30,30S JIS30Kﾘﾝｸﾞ端管
      if ( s_KeyMat == "ALBC" ) {
          // ＡＬＢＣ
      } else if ( s_KeyMat == "CS" ) {
          // ＣＳ（ＳＴＫＭ１３Ａ）
      } else {
          //その他
          // 2020.05.22.ﾘﾝｸﾞ材質修正_S
          //s_KeyMat == "-";
          s_KeyMat = "-";
          // 2020.05.22.ﾘﾝｸﾞ材質修正_E
      }
      s_ULdiv = "-"; // 位置固定
      s_PGtyp = "-"; // コックなし
      s_TPtyp = "-"; // 温度計圧力計座なし
      s_DRtyp = "-"; // 液抜なし
      s_ARtyp = "-"; // エア抜なし

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else if ( ( s_HEADS_DATA[36].Pos("M") != 0 ) &&
               ( Syurui == "S1" || Syurui == "S2" || Syurui == "S3" || Syurui == "S4" ) ) {
      //舶用ﾘﾝｸﾞ端管
      if ( s_KeyMat == "ALBC" ) {
         // ＡＬＢＣ
      } else if ( s_KeyMat == "STPG" ) {
         // ＳＴＰＧ
      } else if ( s_KeyMat == "CS" ) {
          // ＣＳ（ＳＴＫＭ１３Ａ）
      } else if ( s_KeyMat == "SGP" ) {
         // ＳＧＰ
      } else {
         //その他
         // 2020.05.22.ﾘﾝｸﾞ材質修正_S
         //s_KeyMat == "-";
         s_KeyMat = "-";
         // 2020.05.22.ﾘﾝｸﾞ材質修正_E
      }
      //s_ULdiv(設定済)
      s_PGtyp = "T"; // 玉型弁
      // 2020.09.01 舶用ﾘﾝｸﾞ変更_S
      //s_TPtyp = "-"; // 温度計圧力計座なし
      if (s_Prs_Syurui == 0) {
          s_TPtyp = "-"; // 圧力計座なし　
      }
      else {
          s_TPtyp = "P"; // 圧力計座あり
      }
      // 2020.09.01 舶用ﾘﾝｸﾞ変更_E
      s_DRtyp = "-"; // 液抜なし
      s_ARtyp = "-"; // エア抜なし

      if ( ( P_Model == "RX-70" ) &&
           ( s_KeyMat == "STPG" || s_KeyMat == "-" ) ) {
         //RX-70 旧材質 ﾘﾝｸﾞ端管
         s_Text = s_HEADS_DATA[36].TrimRight();
         i_Length = s_Text.Length();                    // 文字数
         s_Text = s_Text.SubString( i_Length, 1 );
         if ( s_Text == "R" ) {
            s_Text = s_HEADS_DATA[36].TrimRight();
            s_Frame = s_Text.SetLength( i_Length - 1 ); // 型式種類
         } else {
            s_Frame = s_HEADS_DATA[36].TrimRight();     // 型式種類
         }
         s_Ver = s_HEADS_DATA[1267].TrimRight();

         if ( s_Frame == "NPM" ||  s_Frame == "KNPM" || s_Frame == "TNPM" ) {
            if ( s_Ver == "200207" ) {
               s_KeyMat = "HP";
            } else {
               s_KeyMat = "P";
            }
         } else if ( s_Frame == "NHPM" ||  s_Frame == "KNHPM" || s_Frame == "TNHPM" ) {
            s_KeyMat = "HP";
         } else {
            s_KeyMat = "-";
         }
      }

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_KeyMat, s_ULdiv, s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp, true);

   } else {

      P_Katasiki = Search_HD_FRNG_MST(P_Model, s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp,false);

   }
   //2019.07.25 製作指示書書式変更_E

   //2019.07.25 製作指示書書式変更_S
   //P_Katasiki = Search_HD_FRNG_MST(P_Model, s_Nzl_Kokei, Syurui.SubString(1,1), s_PGtyp, s_TPtyp, s_DRtyp, s_ARtyp,false);
   //2019.07.25 製作指示書書式変更_E
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
AnsiString __fastcall Get_Flange_DataA3(AnsiString Tugite)
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
AnsiString __fastcall Get_T_ZaZaisituA3(AnsiString TanSyurui, AnsiString TanName, HeadsData* pcHeadsData)
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
   s_T_ZaZaisitu = Get_Zaisitu_NameA3(s_Code);

   return s_T_ZaZaisitu;
}

//---------------------------------------------------------------------------
// 日本語関数名： スタートプレート取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 部品展開ﾃﾞｰﾀ,HEARTS用ﾃﾞｰﾀに孔明ﾃﾞｰﾀを書込む
//---------------------------------------------------------------------------
bool __fastcall Get_Start_PlateA3(AnsiString &Side_B, AnsiString &Side_A, HeadsData* pcHeadsData){

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
AnsiString __fastcall Get_SheetGasketMatA3(AnsiString s_Side, HeadsData* pcHeadsData)
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
      s_GasMat = Get_Zaisitu_NameA3(s_Text);
   }

   return s_GasMat;

}


//---------------------------------------------------------------------------
// 日本語関数名： 温度計保護管寸法取得
// 概  要      ：
// 引  数      ： Key       ：温度計種類
//                s_Kikaku  ：ノズル規格コード
//                s_FrngSize：ノズル口径
// 戻り値      ： 保護管寸法
// 備  考      ： 2019.10.18追加
//---------------------------------------------------------------------------
AnsiString __fastcall Get_Well_Temp(AnsiString Key, AnsiString s_Kikaku,AnsiString s_FrngSize)
{
   int i_Kikaku;              // 規格種類
   AnsiString s_hogokan;      // 保護管


   //***************************
   //***  規       格
   //***************************
   if ( s_Kikaku.SubString(1,2)=="04" || s_Kikaku.SubString(1,2)=="05" ||
        s_Kikaku.SubString(1,2)=="08" || s_Kikaku.SubString(1,2)=="09" ||
        s_Kikaku.SubString(1,2)=="44" || s_Kikaku.SubString(1,2)=="48" ) {
      // ANSI,JPI
      i_Kikaku = 1;
   }
   else if ( s_Kikaku.SubString(1,2)=="06") {
      // サリタニ
      i_Kikaku = 2;
   }
   else {
      i_Kikaku = 0;
   }

   //***************************
   //***  保   護   管
   //***************************
   if ( Key == "01" || Key == "05" || Key == "06" ) {
      // Ｉ型、Ｉ型舶用、Ｌ型舶用
      if( i_Kikaku == 1 ) { // ANSI,JPI
         if ( s_FrngSize.ToDouble() <= 1 ) {
            s_hogokan = " / 56L";
         }
         else if ( s_FrngSize.ToDouble() <= 2 ) {
            s_hogokan = " / 71L";
         }
         else if ( s_FrngSize.ToDouble() <= 4 ) {
            s_hogokan = " / 101L";
         }
         else if ( s_FrngSize.ToDouble() <= 14 ) {
            s_hogokan = " / 131L";
         }
         else {
            s_hogokan = "";
         }
      }
      else {
         if ( s_FrngSize.ToDouble() <= 25 ) {
            s_hogokan = " / 56L";
         }
         else if ( s_FrngSize.ToDouble() <= 50 ) {
            s_hogokan = " / 71L";
         }
         else if ( s_FrngSize.ToDouble() <= 100 ) {
            s_hogokan = " / 101L";
         }
         else if ( s_FrngSize.ToDouble() <= 350 ) {
            s_hogokan = " / 131L";
         }
         else {
            s_hogokan = "";
         }
      }
   }
   else if ( Key == "02" || Key == "10" || Key == "11" ) {
      // Ｉ型、Ｉ型舶用、Ｌ型舶用（スチーム）
      s_hogokan = " / 56L";
   }
   else if ( Key == "03" ) {
      // サニタリ
      if ( s_FrngSize.ToDouble() <= 2.5 ) {
         s_hogokan = " / 65L";
      }
      else if ( s_FrngSize.ToDouble() <= 4 ) {
         s_hogokan = " / 105L";
      }
      else {
         s_hogokan = "";
      }
   }
   else if ( Key == "04" ) {
      // サニタリ（スチーム）
      s_hogokan = " / 65L";
   }
   else {
      s_hogokan = "";
   }

   return s_hogokan;

}


//---------------------------------------------------------------------------
// 日本語関数名： 特記コード有無確認
// 概  要      ：
// 引  数      ： AnsiString Key   ： 特記ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2022.04.04追加
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
// 備  考      ： 2022.04.05追加
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







