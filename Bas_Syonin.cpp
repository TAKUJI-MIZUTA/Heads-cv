//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：承認図表紙  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.05.30
//　 更 新 日：2007.10.09
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <io.h>

#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Syonin.h"
#define  ZUMENNO　'aaaa'

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//2018.03.12 OGI ADD_S
//---------------------------------------------------------------------------
// 日本語関数名： 銘板図の図番を取得する
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [銘板図の図番]
// 備  考      ： なし
//---------------------------------------------------------------------------
AnsiString __fastcall GetMeibanZuban(void)
{
   AnsiString sRet = "";
   char  buf[1024];                            /* 文字列バッファ   */
   char  *tok;                                 /* トークンポインタ */
   int   i;                                    /* ループカウンタ   */
   char  token[100][100];                      /* トークンバッファ */

   AnsiString P_PARTS[17]; // 部品データ
   AnsiString wk_FilePass;
   FILE *fp;

   //***************************
   //**  部品展開ﾃﾞｰﾀ読込
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
   } else {
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
   }

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      fclose(fp);    // 閉じる
      return sRet;
   }

   //*****************
   // 読込み可能の間
   //*****************
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
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

      //*****************************************
      //***  部品データ(HD_PARTS.csv)の取得
      //***  (部品分類)
      //*****************************************
      P_PARTS[0] = token[0];     // 部品分類
      P_PARTS[1] = token[1];     // 識別子
      P_PARTS[2] = token[2];     // 部品型式
      P_PARTS[3] = token[3];     // 部品名称
      P_PARTS[4] = token[4];     // 材質型式
      P_PARTS[5] = token[5];     // 図番
      P_PARTS[6] = token[6];     // 作図用図番
      P_PARTS[7] = token[7];     // １台分数量
      P_PARTS[8] = token[8];     // 予備数
      P_PARTS[9] = token[9];     // 総計
      P_PARTS[10] = token[10];   // 仕様１
      P_PARTS[11] = token[11];   // 仕様２
      P_PARTS[12] = token[12];   // 仕様３
      P_PARTS[13] = token[13];   // 仕様４
      P_PARTS[14] = token[14];   // 仕様５
      P_PARTS[15] = token[15];   // 材質名称
      P_PARTS[16] = token[16];   // 仕様ﾀｲﾌﾟ(""/"A"/"B")

      //---------------------------------------------------
      //  銘板部品
      //---------------------------------------------------
      if( (P_PARTS[0] == "ACCS") && (P_PARTS[1] == "MEIBAN-") )
      {
        sRet = P_PARTS[5];
        break;
      }
   }

   fclose(fp);    // 閉じる
   return sRet;
}
//***************************************************************************
//2018/03/12 OGI ADD_E

//---------------------------------------------------------------------------
// 日本語関数名： 承認図表紙　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Syonin(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_UtiKbn;          // 打出し区分
   AnsiString s_Text;            // 汎用文字列
   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   //int        i_BuhinNo;         // 品番
   int        i_Pos;
   // 2010.08.19 中国語対応
   AnsiString s_China;           // 言語区分("0":日本語、"1":中国語)
   // *********************

   bool bRet;
   AnsiString s_HEADS_DATA[1500];

   // 元仕様でない場合は処理不要(正常終了)
   if(!pcHeadsData->GetSpecType()==DEF_SPEC_TYPE_NONE) return(true);

   // HEADSﾃﾞｰﾀをﾛｰｶﾙ変数にｾｯﾄ
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);
   // 2010.08.19 中国語対応
   //s_Tmplate_Pass = G_Template_Dir + "承認図表紙.xls";               // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   i_RecNo = 851;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "1" ) {
      s_China = "1";                                                 // 中国語
      // 2014.10.01 北京、広州支店追加
      //s_Tmplate_Pass = G_Template_Dir + "承認図表紙C.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      if (s_HEADS_DATA[4].TrimRight() == "C1") {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C1.xls";       // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else if (s_HEADS_DATA[4].TrimRight() == "C2") {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C2.xls";       // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else if (s_HEADS_DATA[4].TrimRight() == "C3") {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C3.xls";       // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C.xls";        // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      }
      // *****************************
   // 2014.04.07 名古屋支店追加
   } else if (s_HEADS_DATA[4].TrimRight() == "N") {
      //2114.05.30 言語修正
      //s_China = "N";                                                 // 日本語
      s_China = "0";                                                 // 日本語
      //*******************
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙N.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   // ***************************
   // 2019.04.08 北海道営業所追加_S
   } else if (s_HEADS_DATA[4].TrimRight() == "D") {
      s_China = "0";                                                 // 日本語
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙D.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   // 2019.04.08 北海道営業所追加_E
   // 2021.10.18 尾道営業所追加_S
   } else if (s_HEADS_DATA[4].TrimRight() == "S") {
      s_China = "0";                                                 // 日本語
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙S.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   // 2021.10.18 尾道営業所追加_E
   // 2022.04.12 千葉営業所追加_S
   } else if (s_HEADS_DATA[4].TrimRight() == "C") {
      s_China = "0";                                                 // 日本語
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙CH.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   // 2022.04.12 千葉営業所追加_E
   // 2014.10.01 北京、広州支店追加
   } else if (s_HEADS_DATA[4].TrimRight() == "C1") {
      s_China = "1";                                                 // 中国語
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙C1.xls";          // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   } else if (s_HEADS_DATA[4].TrimRight() == "C2") {
      s_China = "1";                                                 // 中国語
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙C2.xls";          // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   } else if (s_HEADS_DATA[4].TrimRight() == "C3") {
      s_China = "1";                                                 // 中国語
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙C3.xls";          // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   // *****************************
   } else {
      s_China = "0";                                                 // 日本語
      s_Tmplate_Pass = G_Template_Dir + "承認図表紙.xls";            // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   }
   // *********************
   // 2011.05.30 常熟用承認図
   // 2012.01.20 常熟フラグ
   //i_RecNo = 5;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // 中国語
   //   s_Tmplate_Pass = G_Template_Dir + "承認図表紙C.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //}
   //i_RecNo = 401;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // 中国語
   //   s_Tmplate_Pass = G_Template_Dir + "承認図表紙C.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //}
   i_RecNo = 1107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "CHINA" ) {
      s_China = "1";                                                 // 中国語
      // 2014.10.01 北京、広州支店追加
      //s_Tmplate_Pass = G_Template_Dir + "承認図表紙C.xls";           // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      if (s_HEADS_DATA[4].TrimRight() == "C1") {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C1.xls";       // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
	  } else if (s_HEADS_DATA[4].TrimRight() == "C2") {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C2.xls";       // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else if (s_HEADS_DATA[4].TrimRight() == "C3") {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C3.xls";       // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      } else {
         s_Tmplate_Pass = G_Template_Dir + "承認図表紙C.xls";        // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
      }
      // *****************************
   }
   // *********************
   // ***********************
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "AMP" + G_SAKUBAN + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "AMP" + GetFileSeizouBangou(G_SAKUBAN) + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『承認図表紙』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "作成ファイル名：     『" + s_MakeExcel_Pass + "』";
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
      G_Log_Text = "ファイルコピー成功。 『" + s_Tmplate_Pass +
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
      //Windows 2008Server 移植対応
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //***********************************************************************************
   //***                                                                             ***
   //***                    承認図表紙（和文）　データセット                         ***
   //***                                                                             ***
   //***********************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("item") << "和文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));

   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);

   //***************************
   //***  和文：タイトル
   //***************************
   // 『A001』
   // 2010.08.19 中国語対応
   //if (s_UtiKbn == "1"){
   //   s_Text = "見  積  図";
   //
   //} else if (s_UtiKbn == "2"){
   //   s_Text = "承  認  図";
   //
   //} else if (s_UtiKbn == "3"){
   //   s_Text = "確  定  図";
   //
   //} else if (s_UtiKbn == "4"){
   //   s_Text = "完  成  図";
   //
   //} else if (s_UtiKbn == "5"){
   //   s_Text = "納  入  仕  様  書";
   //
   //} else if (s_UtiKbn == "6"){
   //   s_Text = "工  事  図";
   // 2010.08.19 中国語対応
   if ( s_China == "0" && s_UtiKbn == "1" ) {
      s_Text = "見  積  図";
   } else if ( s_China == "0" && s_UtiKbn == "2" ) {
      s_Text = "承  認  図";
   } else if ( s_China == "0" && s_UtiKbn == "3" ) {
      s_Text = "確  定  図";
   } else if ( s_China == "0" && s_UtiKbn == "4" ) {
      s_Text = "完  成  図";
   } else if ( s_China == "0" && s_UtiKbn == "5" ) {
      s_Text = "納  入  仕  様  書";
   } else if ( s_China == "0" && s_UtiKbn == "6" ) {
      s_Text = "工  事  図";
   } else if ( s_China == "1" && s_UtiKbn == "1" ) {
      s_Text = "草  稿  /U+22270/U+";
   } else if ( s_China == "1" && s_UtiKbn == "2" ) {
      // 2011.05.30 常熟用承認図
      //s_Text = "核  准  /U+22270/U+";
      s_Text = "/U+35748/U+  可  /U+22270/U+";
      // ***********************
   } else if ( s_China == "1" && s_UtiKbn == "3" ) {
      s_Text = "/U+30830/U+  定  /U+22270/U+";
   } else if ( s_China == "1" && s_UtiKbn == "4" ) {
      s_Text = "完  成  /U+22270/U+";
   } else if ( s_China == "1" && s_UtiKbn == "5" ) {
      // 2011.05.30 常熟用承認図
      //s_Text = "交  /U+32435/U+  /U+35268/U+  格  /U+35828/U+  明  /U+20070/U+";
      s_Text = "/U+35774/U+  /U+22791/U+  /U+35268/U+  格  /U+20070/U+";
      // ***********************
   } else if ( s_China == "1" && s_UtiKbn == "6" ) {
      s_Text = "施  工  /U+22270/U+";
   // *********************
   } else {
      G_ErrLog_Text = "不正な打出し区分『" + s_UtiKbn + "』 承認図作成処理を終了します。";
      Write_Error_Log(G_ErrLog_Text);

      // ｵﾌﾞｼﾞｪｸﾄ終了
      //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
      //Windows 2008Server 移植対応
      exApplication.OlePropertySet("DisplayAlerts", false);
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
      exWorkbook.OleFunction("Close");
      exWorkbooks.OleFunction("Close");

      return false;
   }

   Excel_Cell_DataSet(1, 3, s_Text);
   G_Log_Text = "和文：タイトル       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：上段日付
   //***************************
   // 『A002』
// 日付は記入しない　2002/11/23
//   s_Text = FormatDateTime("yyyy",Date()) + "年"
//          + FormatDateTime("mm",Date()) + "月"
//          + FormatDateTime("dd",Date()) + "日";
//   Excel_Cell_DataSet(3, 14, s_Text);
//   G_Log_Text = "和文：上段日付       『" + s_Text + "』をセット。";
//   Write_Log(G_Log_Text);
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   // 2019.02.22 年月日追加_S　
   if ( s_China == "1" ) {
      //中国
   } else {
      if ( s_UtiKbn == "1" ) {
         //見  積  図
      } else if ( s_UtiKbn == "2" ) {
         //承  認  図
      } else if ( s_UtiKbn == "3" ) {
         //確  定  図
      } else if ( s_UtiKbn == "4" ) {
         //完  成  図
      } else if ( s_UtiKbn == "5" ) {
         //納  入  仕  様  書
         Excel_Cell_DataSet(3, 14, s_Text);
         G_Log_Text = "和文：年月日２        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else if ( s_UtiKbn == "6" ) {
         //工  事  図
      }
   }
   // 2019.02.22 年月日追加_E　

   //***************************
   //***  和文：注文先
   //***************************
   // 『A003』
   i_RecNo = 1063;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(5, 3, s_Text);
   G_Log_Text = "和文：注文先          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：納入先
   //***************************
   // 『A004』
   i_RecNo = 1065;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 3, s_Text);
   G_Log_Text = "和文：納入先          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：注文番号
   //***************************
   // 『A005』
   // 2017.10.06 注文番号削除_S
   //i_RecNo = 1260;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(7, 3, s_Text);
   //G_Log_Text = "和文：注文番号        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   // 2017.10.06 注文番号削除_E

   //***************************
   //***  和文：装置番号
   //***************************
   // 『A006』
   i_RecNo = 28;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(9, 3, s_Text);
   G_Log_Text = "和文：装置番号        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // 舶用の場合は、「装置番号」を「船番」に置き換える
      //Excel_Cell_DataSet(9, 2, "船番");
      // 2010.08.19 中国語対応
      if ( s_China == "0" ) {
          Excel_Cell_DataSet(9, 2, "船番");
      } else {
          Excel_Cell_DataSet(9, 2, "船号");
      }
      // *********************
   }

   //***************************
   //***  和文：機器名称
   //***************************
   // 『A007』
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(10, 3, s_Text);
   G_Log_Text = "和文：機器名称        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：機器番号
   //***************************
   // 『A008』
   i_RecNo = 27;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(11, 3, s_Text);
   G_Log_Text = "和文：機器番号        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：製造番号
   //***************************
   // 『A009』
   if(G_SAKUBAN.SubString(1,2).ToIntDef(999) != 999 ) {
     //2022/12/15 製造番号桁数追加対応 Mod_S
     //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
     s_Text = GetSeizouBangou(G_SAKUBAN);
     //2022/12/15 製造番号桁数追加対応 Mod_E     

     //----------------------------------------------
     // 2007.10.09 ﾐﾗｰ仕様の場合、末尾に"A/B"を追加
     if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
     }
     // 2007.10.09
     //----------------------------------------------

     Excel_Cell_DataSet(13, 3, s_Text);
     G_Log_Text = "和文：製造番号       『" + s_Text + "』をセット。";
     Write_Log(G_Log_Text);
   }

   //***************************
   //***  和文：型式
   //***************************
   // 『A010』
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(15, 3, s_Text);
   G_Log_Text = "和文：型式            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  和文：台数
   //***************************
   // 『A011』
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(16, 3, s_Text);
      G_Log_Text = "和文：台数            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "和文：台数           RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }
   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // 舶用の場合は、「台o」を「台／隻」に置き換える
      Excel_Cell_DataSet(16, 7, "台／隻");
   }
   //***************************
   //***  和文：交換熱量
   //***************************
   // 『A012』
   if (s_HEADS_DATA[20].TrimRight() == "19"){
      i_RecNo = 107;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(17, 3, s_Text);
      G_Log_Text = "和文：交換熱量        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
      // 単位を置き換える
      s_Text = s_HEADS_DATA[192];
      if ( s_Text.ToIntDef(0) >= 1 && s_Text.ToIntDef(0) <= 4 ) {
        Excel_Cell_DataSet(17, 7, "kJ");
      }

   } else {
      s_Text = "";
      Excel_Cell_DataSet(17, 2, s_Text);
      Excel_Cell_DataSet(17, 3, s_Text);
      Excel_Cell_DataSet(17, 7, s_Text);
   }

   //***************************
   //***  和文：営業担当(大阪)
   //***************************
   // 『A013』
   // 2010.08.19 中国語対応
   if ( s_China == "1" ) {
      G_Log_Text = "中国用なので営業担当はセットしない。";
      Write_Log(G_Log_Text);
   //if ((s_HEADS_DATA[4].TrimRight() == "O") || (s_HEADS_DATA[4].TrimRight() == "E")) {
   } else if ((s_HEADS_DATA[4].TrimRight() == "O") || (s_HEADS_DATA[4].TrimRight() == "E")) {
   // *********************
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(13, 15, s_Text);
      G_Log_Text = "和文：営業担当(大阪)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      i_RecNo = 4;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(13, 15, "");
      G_Log_Text = "和文：営業担当(大阪)  地区区分 RecNo:" + FormatFloat("0000",i_RecNo) +
                   "が『" + s_Text + "』なのでブランクをセット。";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  和文：営業担当(東京)
   //***************************
   // 『A014』
   // 2010.08.19 中国語対応
   if ( s_China == "1" ) {
      G_Log_Text = "中国用なので営業担当はセットしない。";
      Write_Log(G_Log_Text);
   //if (s_HEADS_DATA[4].TrimRight() == "T"){
   } else if (s_HEADS_DATA[4].TrimRight() == "T"){
   // *********************
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "和文：営業担当(東京)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // 2014.05.30 名古屋支店追加
   } else if (s_HEADS_DATA[4].TrimRight() == "N"){
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "和文：営業担当(名古屋)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // *************************
   // 2019.04.08 北海道営業所追加_S
   } else if (s_HEADS_DATA[4].TrimRight() == "D") {
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "和文：営業担当(北海道)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // 2019.04.08 北海道営業所追加_E
   // 2022.04.12 千葉営業所追加_S
   } else if (s_HEADS_DATA[4].TrimRight() == "S") {
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "和文：営業担当(尾道)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else if (s_HEADS_DATA[4].TrimRight() == "C") {
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "和文：営業担当(千葉)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   // 2022.04.12 千葉営業所追加_E
   } else {
      i_RecNo = 4;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, "");
      G_Log_Text = "和文：営業担当(東京)  地区区分 RecNo:" + FormatFloat("0000",i_RecNo) +
                   "が『" + s_Text + "』なのでブランクをセット。";
      Write_Log(G_Log_Text);
   }

   // 2010.02.10 エラー注記追加
   //***************************
   //*** 和文：注記(ｴﾗｰ)
   //***************************
   // 『A015』
   i_RecNo = 1021;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.SubString(1,2) == "ER" ) {
       s_Text = "エラー："+ s_Text;
       Excel_Cell_DataSet(2, 2, s_Text);
       G_Log_Text = "注記(ｴﾗｰ)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
       Write_Log(G_Log_Text);
   }
   // *************************

   // 2019.10.10 ﾊﾟｰﾂﾘｽﾄｴﾗｰ_S
   //***************************
   //*** 和文：注記(ﾊﾟｰﾂﾘｽﾄｴﾗｰ)
   //***************************
   // 『A016』
   i_RecNo = 1123;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text != "" ) {
       Excel_Cell_DataSet(3, 2, s_Text);
       G_Log_Text = "注記(ﾊﾟｰﾂﾘｽﾄｴﾗｰ)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
       Write_Log(G_Log_Text);
   }
   // 2019.10.10 ﾊﾟｰﾂﾘｽﾄｴﾗｰ_E

   // B欄
   //i_BuhinNo = 0;

   //********************************
   //***  Ｂ欄　出力図面セット
   //********************************
   s_UtiKbn = s_HEADS_DATA[1259].SubString(2,1);

   // 2018.07.26 打ち出し区分変更_S
   //if (s_UtiKbn == "0" || s_UtiKbn == "9"){
   //   //***************  打ち出し区分　０，９  **********************
   if (s_UtiKbn == "0"){
      //***************  打ち出し区分　０  **********************
   // 2018.07.26 打ち出し区分変更_E

      SyuturyokuZumen_Set('A', "J", pcHeadsData);     // 出力パターンＡ

   } else if (s_UtiKbn == "1"){
      //***************  打ち出し区分　１  **********************
      SyuturyokuZumen_Set('B', "J", pcHeadsData);     // 出力パターンＢ

   } else if (s_UtiKbn == "2"){
      //***************  打ち出し区分　２  **********************

      SyuturyokuZumen_Set('C', "J", pcHeadsData);     // 出力パターンＣ

   } else if (s_UtiKbn == "3"){
      //***************  打ち出し区分　３  **********************

      if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
         // ｽﾁｰﾑ
         SyuturyokuZumen_Set('E', "J", pcHeadsData);  // 出力パターンＥ
      } else {
         // 液
         SyuturyokuZumen_Set('D', "J", pcHeadsData);  // 出力パターンＤ
      }

   } else if (s_UtiKbn == "4"){
      //***************  打ち出し区分　４  **********************
      s_Text = s_HEADS_DATA[29].TrimRight();   // 圧力容器（規格ｺｰﾄﾞ）

      if (s_HEADS_DATA[327].TrimRight() == ""){
         // 改造無し
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // 舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('G', "J", pcHeadsData);  // 出力パターンＧ
               } else {
                  // 液
                  SyuturyokuZumen_Set('F', "J", pcHeadsData);  // 出力パターンＦ
               }
            } else {
               SyuturyokuZumen_Set('C', "J", pcHeadsData);     // 出力パターンＣ
            }
         } else {
            // 非舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('I', "J", pcHeadsData);  // 出力パターンＩ
               } else {
                  // 液
                  SyuturyokuZumen_Set('H', "J", pcHeadsData);  // 出力パターンＨ
               }
            } else {
               SyuturyokuZumen_Set('J', "J", pcHeadsData);     // 出力パターンＪ
            }
         }

      } else {
         // 改造有り
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // 舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('L', "J", pcHeadsData);  // 出力パターンＬ
               } else {
                  // 液
                  SyuturyokuZumen_Set('K', "J", pcHeadsData);  // 出力パターンＫ
               }
            } else {
               SyuturyokuZumen_Set('M', "J", pcHeadsData);     // 出力パターンＭ
            }
         } else {
            // 非舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('O', "J", pcHeadsData);  // 出力パターンＯ
               } else {
                  // 液
                  SyuturyokuZumen_Set('N', "J", pcHeadsData);  // 出力パターンＮ
               }
            } else {
               SyuturyokuZumen_Set('P', "J", pcHeadsData);     // 出力パターンＰ
            }
         }

      }

   } else if (s_UtiKbn == "5"){
      //***************  打ち出し区分　５  **********************

      SyuturyokuZumen_Set('J', "J", pcHeadsData);

   // 2018.07.26 打ち出し区分変更_S
   //} else if (s_UtiKbn == "6"){
   //   //***************  打ち出し区分　６  **********************
   } else if (s_UtiKbn == "6" || s_UtiKbn == "9"){
   //   //***************  打ち出し区分　６，９  **********************

   // 2018.07.26 打ち出し区分変更_E
      if (s_HEADS_DATA[327].TrimRight() == ""){           // 改造無し
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // 舶用
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('G', "J", pcHeadsData);  // 出力パターンＧ
            } else {
               // 液
               SyuturyokuZumen_Set('F', "J", pcHeadsData);  // 出力パターンＦ
            }
         } else  {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('R', "J", pcHeadsData);  // 出力パターンＲ
            } else {
               // 液
               SyuturyokuZumen_Set('Q', "J", pcHeadsData);  // 出力パターンＱ
            }
         }
      } else {                                            // 改造有り
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // 舶用
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('L', "J", pcHeadsData);  // 出力パターンＬ
            } else {
               // 液
               SyuturyokuZumen_Set('K', "J", pcHeadsData);  // 出力パターンＫ
            }
         } else {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('T', "J", pcHeadsData);  // 出力パターンＴ
            } else {
               // 液
               SyuturyokuZumen_Set('S', "J", pcHeadsData);  // 出力パターンＳ
            }
         }
      }

   } else if (s_UtiKbn == "7"){
      //***************  打ち出し区分　７  **********************

      SyuturyokuZumen_Set('U', "J", pcHeadsData);        // 出力パターンＵ

   } else if (s_UtiKbn == "8"){
      //***************  打ち出し区分　８  **********************

      SyuturyokuZumen_Set('V', "J", pcHeadsData);        // 出力パターンＶ

   }


   // C欄
   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);
   if (s_HEADS_DATA[319].TrimRight().ToIntDef(0) != 0){
//2004/10/25 add??
//   if (G_HEADS_DATA_Rev != 0){
//2004/10/25 add??
      //***************************
      //***  和文：訂正
      //***************************
      // 『C001』
      i_RecNo = 319;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 1, s_Text);
      G_Log_Text = "和文：訂正            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //***************************
      //***  和文：記事
      //***************************
      // 『C002』
      //i_RecNo = 1088;
      //s_Text = s_HEADS_DATA[i_RecNo];

      // 2010.08.19 中国語対応
      //s_Text = "設計変更";
      if ( s_China == "0" ) {
         s_Text = "設計変更";
      } else {
         s_Text = "/U+35774/U+/U+35745/U+/U+21464/U+更";
      }
      // *********************

      Excel_Cell_DataSet(41, 2, s_Text);
      G_Log_Text = "和文：記事           『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //***************************
      //***  和文：年月日
      //***************************
      // 『C003』
      i_RecNo = 1087;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 4, s_Text);
      G_Log_Text = "和文：年月日          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //***************************
      //***  和文：設計担当者
      //***************************
      // 『C004』
      i_RecNo = 1079;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 7, s_Text);
      G_Log_Text = "和文：設計担当者      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   // 2017.12.01 納期削除_S
   ////***************************
   ////***  和文：下段日付
   ////***************************
   //// 『C005』
   ////i_RecNo = 12;
   //i_RecNo = 411;
   //s_Text = s_HEADS_DATA[i_RecNo];
   //Excel_Cell_DataSet(44, 3, s_Text);
   //G_Log_Text = "和文：下段日付        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   // 2017.12.01 納期削除_E

   //***************************
   //***  和文：HEADS VER
   //***************************
   // 『C006』
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
   Excel_Cell_DataSet(45, 3, s_Text);
   G_Log_Text = "HEADS VER            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2021.11.02 納入仕様書返却文言削除_S
   //// 2021.10.18 納入仕様書返却文言追加_S
   ////*******************************
   ////***  和文：納入仕様書返却文言
   ////*******************************
   //// 業界コード【13：船舶】【19：空調】【21：プラント】を除く和文で文言を記述　　　
   //i_RecNo = 438;   // 業界ｺｰﾄﾞ
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text == "0013"){
   //    // 業界ｺｰﾄﾞ13
   //} else if (s_Text == "0019"){
   //    // 業界ｺｰﾄﾞ19
   //} else if   (s_Text == "0021"){
   //    // 業界ｺｰﾄﾞ21
   //} else {
   //    // 業界ｺｰﾄﾞ13,19,21以外
   //    // 返却文言を記述
   //    s_Text = "お　願　い";
   //    Excel_Cell_DataSet(37, 14, s_Text);
   //    G_Log_Text = "和文：返却文言１     『" + s_Text + "』をセット。";
   //    s_Text = "本図をご検討頂き、貴社受領印捺印の上、";
   //    Excel_Cell_DataSet(38, 11, s_Text);
   //    G_Log_Text = "和文：返却文言２     『" + s_Text + "』をセット。";
   //    s_Text = "弊社へご返却下さる様お願い申し上げます。";
   //    Excel_Cell_DataSet(39, 11, s_Text);
   //    G_Log_Text = "和文：返却文言３     『" + s_Text + "』をセット。";
   //    s_Text = "尚、 "
   //           + FormatDateTime("yyyy",Date() + 14) + "年"
   //           + FormatDateTime("mm",Date() + 14) + "月"
   //           + FormatDateTime("dd",Date() + 14) + "日"
   //           + " 迄にご返却頂けない";
   //    Excel_Cell_DataSet(40, 11, s_Text);
   //    G_Log_Text = "和文：返却文言４     『" + s_Text + "』をセット。";
   //    s_Text = "場合は、本図にて製作を進めさせて頂きます。";
   //    Excel_Cell_DataSet(41, 11, s_Text);
   //    G_Log_Text = "和文：返却文言５     『" + s_Text + "』をセット。";
   //}
   //// 2021.10.18 納入仕様書返却文言追加_E
   // 2021.11.02 納入仕様書返却文言削除_E

   // 2020.04.21 バーコード変更_S
   //// 2017.12.01 バーコード追加_S
   ////***************************
   ////***  和文：バーコード
   ////***************************
   //s_Text = G_SAKUBAN;
   //// 2018.10.22 バーコード変更_S
   ////s_Text = "*" + s_Text + "*";
   ////Excel_Cell_DataSet(45, 15, s_Text);
   //Excel_Cell_DataSet(10, 19, s_Text);
   //// 2018.10.22 バーコード変更_E
   //G_Log_Text = "バーコード           『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //// 2018.10.22 バーコード削除_E
   s_Text = G_SAKUBAN;
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));
   // 2023.05.30 バーコード変更_S
   //// 2023.04.10 バーコード変更_S
   ////Excel_Cell_DataSet(4, 2, s_Text);
   //if (AllNumChk(G_SAKUBAN.c_str(),0) == 0) {
   //    // オーダ
   //} else {
   //    // 新見積
   //    Excel_Cell_DataSet(3, 2, s_Text);
   //}
   // 2023.04.10 バーコード変更_E
   Excel_Cell_DataSet(4, 5, s_Text);
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2023.05.30 バーコード変更_E
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("和文"));
   // 2020.04.21 バーコード変更_E

   //***********************************************************************************
   //***                                                                             ***
   //***                    承認図表紙（英文）　データセット                         ***
   //***                                                                             ***
   //***********************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "英文");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("Item" , OleVariant("英文"));

   //i_BuhinNo = 0;

   //***************************
   //***  英文：タイトル
   //***************************
   // 『A001』
   s_Text = "FINAL SPECIFICATION & DRAWING";
   if (s_UtiKbn == "1"){
      s_Text = "ESTIMATE DWG.";

   } else if (s_UtiKbn == "2"){
      s_Text = "FINAL SPECIFICATION & DRAWING";

   } else if (s_UtiKbn == "3"){
      s_Text = "FINAL SPECIFICATION & DRAWING";

   } else if (s_UtiKbn == "4"){
      s_Text = "FINAL SPECIFICATION & DRAWING";

   } else if (s_UtiKbn == "5"){
      s_Text = "ADMISSION PROPOSAL DWG.";

   } else if (s_UtiKbn == "6"){
      //2014.11.28 工事図変更
      //s_Text = "CONSTRUCTION DWG.";
      s_Text = "WORKING DWG.";
      //*********************

   }
   Excel_Cell_DataSet(1, 3, s_Text);
   G_Log_Text = "英文：タイトル       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：上段日付
   //***************************
   // 『A002』
// 日付は記入しない　2002/11/23
//   s_Text = FormatDateTime("yyyy/mm/dd",Date());
//   Excel_Cell_DataSet(3, 11, s_Text);
//   G_Log_Text = "英文：上段日付       『" + s_Text + "』をセット。";
//   Write_Log(G_Log_Text);
   // 2019.02.22 年月日追加_S　
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   s_Text = JapDayToEngDay( s_Text );
   if ( s_China == "1" ) {
      //中国
   } else {
      if ( s_UtiKbn == "1" ) {
         //見  積  図
      } else if ( s_UtiKbn == "2" ) {
         //承  認  図
      } else if ( s_UtiKbn == "3" ) {
         //確  定  図
      } else if ( s_UtiKbn == "4" ) {
         //完  成  図
      } else if ( s_UtiKbn == "5" ) {
         //納  入  仕  様  書
         Excel_Cell_DataSet(3, 15, s_Text);
         G_Log_Text = "英文：年月日２        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else if ( s_UtiKbn == "6" ) {
         //工  事  図"
      }
   }
   // 2019.02.22 年月日追加_E　

   //***************************
   //***  英文：注文主
   //***************************
   // 『A003』
   i_RecNo = 1261;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(5, 4, s_Text);
   G_Log_Text = "英文：PURCHASER       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：納入先
   //***************************
   // 『A004』
   i_RecNo = 1262;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(6, 4, s_Text);
   G_Log_Text = "英文：COSTOMER        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：注文番号
   //***************************
   // 『A005』
   // 2017.10.06 注文番号削除_S
   //i_RecNo = 1260;
   //s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   //Excel_Cell_DataSet(7, 4, s_Text);
   //G_Log_Text = "英文：PURCHASE ORDER NO  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   // 2017.10.06 注文番号削除_E

   //***************************
   //***  英文：工事番号
   //***************************
   // 『A006』
   i_RecNo = 28;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(9, 3, s_Text);
   G_Log_Text = "英文：JOB NO          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // 舶用の場合は、「JOB No」を「SHIP No」に置き換える
      Excel_Cell_DataSet(9, 2, "SHIP NO.:");
   }
   //***************************
   // 英文：機器名称
   //***************************
   // 『A007』
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(10, 3, s_Text);
   G_Log_Text = "英文：EQPT.NAME:      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：機器番号
   //***************************
   // 『A008』
   i_RecNo = 27;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(11, 3, s_Text);
   G_Log_Text = "英文：ITEM NO.        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：製造番号
   //***************************
   // 『A009』
   if( G_SAKUBAN.SubString(1,2).ToIntDef(999) != 999 ) {
     //2022/12/15 製造番号桁数追加対応 Mod_S
     //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
     s_Text = GetSeizouBangou(G_SAKUBAN);
     //2022/12/15 製造番号桁数追加対応 Mod_E

     //----------------------------------------------
     // 2007.10.09 ﾐﾗｰ仕様の場合、末尾に"A/B"を追加
     if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
     }
     // 2007.10.09
     //----------------------------------------------

     Excel_Cell_DataSet(13, 3, s_Text);
     G_Log_Text = "英文：MFG NO.        『" + s_Text + "』をセット。";
     Write_Log(G_Log_Text);
   }

   //***************************
   //***  英文：型式
   //***************************
   // 『A010』
   i_RecNo = 33;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(15, 4, s_Text);
   G_Log_Text = "英文：MODEL           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  英文：台数
   //***************************
   // 『A011』
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(16, 3, s_Text);
      G_Log_Text = "英文：QUANTITY        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";

      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "英文：QUANTITY          RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);
   }
   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // 舶用の場合は、「UNIT(S)」を「UNIT(S)/SHIP」に置き換える
      Excel_Cell_DataSet(16, 7, "UNIT(S)/SHIP");
   }
   //***************************
   //***  英文：交換熱量
   //***************************
   // 『A012』
   if (s_HEADS_DATA[490].TrimRight() == "19"){
      i_RecNo = 107;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(17, 4, s_Text);
      G_Log_Text = "英文：HEAT EXCHANGED       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      s_Text = "";
      Excel_Cell_DataSet(17, 4, s_Text);
   }

   // 2010.02.10 エラー注記追加
   //***************************
   //*** 英文：注記(ｴﾗｰ)
   //***************************
   // 『A013』
   i_RecNo = 1021;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.SubString(1,2) == "ER" ) {
       s_Text = "エラー："+ s_Text;
       Excel_Cell_DataSet(2, 2, s_Text);
       G_Log_Text = "注記(ｴﾗｰ)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
       Write_Log(G_Log_Text);
   }
   // *************************

   // 2019.10.10 ﾊﾟｰﾂﾘｽﾄｴﾗｰ_S
   //***************************
   //*** 英文：注記(ﾊﾟｰﾂﾘｽﾄｴﾗｰ)
   //***************************
   // 『A016』
   i_RecNo = 1123;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text != "" ) {
       Excel_Cell_DataSet(3, 2, s_Text);
       G_Log_Text = "注記(ﾊﾟｰﾂﾘｽﾄｴﾗｰ)  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
       Write_Log(G_Log_Text);
   }
   // 2019.10.10 ﾊﾟｰﾂﾘｽﾄｴﾗｰ_E

   //*****************************************
   //***
   //***      英文Ｂ欄　出力図面セット
   //***
   //*****************************************
   s_UtiKbn = s_HEADS_DATA[1259].SubString(2,1);

   // 2019.04.09 打ち出し区分変更_S
   //if (s_UtiKbn == "0" || s_UtiKbn == "9"){
   //   //***************  打ち出し区分　０，９  **********************
   if (s_UtiKbn == "0"){
      //***************  打ち出し区分　０  **********************
   // 2019.04.09 打ち出し区分変更_E
      SyuturyokuZumen_Set('A', "E", pcHeadsData);     // 出力パターンＡ

   } else if (s_UtiKbn == "1"){
      //***************  打ち出し区分　１  **********************
      SyuturyokuZumen_Set('B', "E", pcHeadsData);     // 出力パターンＢ

   } else if (s_UtiKbn == "2"){
      //***************  打ち出し区分　２  **********************

      SyuturyokuZumen_Set('C', "E", pcHeadsData);     // 出力パターンＣ

   } else if (s_UtiKbn == "3"){
      //***************  打ち出し区分　３  **********************

      if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
         // ｽﾁｰﾑ
         SyuturyokuZumen_Set('E', "E", pcHeadsData);  // 出力パターンＥ
      } else {
         // 液
         SyuturyokuZumen_Set('D', "E", pcHeadsData);  // 出力パターンＤ
      }

   } else if (s_UtiKbn == "4"){
      //***************  打ち出し区分　４  **********************
      s_Text = s_HEADS_DATA[29].TrimRight();   // 圧力容器（規格ｺｰﾄﾞ）

      if (s_HEADS_DATA[327].TrimRight() == ""){
         // 改造無し
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // 舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('G', "E", pcHeadsData);  // 出力パターンＧ
               } else {
                  // 液
                  SyuturyokuZumen_Set('F', "E", pcHeadsData);  // 出力パターンＦ
               }
            } else {
                  SyuturyokuZumen_Set('C', "E", pcHeadsData);     // 出力パターンＣ
            }
         } else {
            // 非舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('I', "E", pcHeadsData);  // 出力パターンＩ
               } else {
                  // 液
                  SyuturyokuZumen_Set('H', "E", pcHeadsData);  // 出力パターンＨ
               }
            } else {
               SyuturyokuZumen_Set('J', "E", pcHeadsData);     // 出力パターンＪ
            }
         }

      } else {
         // 改造有り
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // 舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('L', "E", pcHeadsData);  // 出力パターンＬ
               } else {
                  // 液
                  SyuturyokuZumen_Set('K', "E", pcHeadsData);  // 出力パターンＫ
               }
            } else {
               SyuturyokuZumen_Set('M', "E", pcHeadsData);     // 出力パターンＭ
            }
         } else {
            // 非舶用
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ｽﾁｰﾑ
                  SyuturyokuZumen_Set('O', "E", pcHeadsData);  // 出力パターンＯ
               } else {
                  // 液
                  SyuturyokuZumen_Set('N', "E", pcHeadsData);  // 出力パターンＮ
               }
            } else {
               SyuturyokuZumen_Set('P', "E", pcHeadsData);     // 出力パターンＰ
            }
         }

      }

   } else if (s_UtiKbn == "5"){
      //***************  打ち出し区分　５  **********************

      SyuturyokuZumen_Set('J', "E", pcHeadsData);

   // 2019.04.09 打ち出し区分変更_S
   //} else if (s_UtiKbn == "6"){
   //   //***************  打ち出し区分　６  **********************
   } else if (s_UtiKbn == "6" || s_UtiKbn == "9"){
      //***************  打ち出し区分　６，９  **********************
   // 2019.04.09 打ち出し区分変更_E

      if (s_HEADS_DATA[327].TrimRight() == ""){         // 改造無し
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // 舶用
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('G', "E", pcHeadsData);  // 出力パターンＧ
            } else {
               // 液
               SyuturyokuZumen_Set('F', "E", pcHeadsData);  // 出力パターンＦ
            }
         } else {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('R', "E", pcHeadsData);  // 出力パターンＲ
            } else {
               // 液
               SyuturyokuZumen_Set('Q', "E", pcHeadsData);  // 出力パターンＱ
            }
         }
      } else {         // 改造有り
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // 舶用
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('L', "E", pcHeadsData);  // 出力パターンＬ
            } else {
               // 液
               SyuturyokuZumen_Set('K', "E", pcHeadsData);  // 出力パターンＫ
            }
         } else {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ｽﾁｰﾑ
               SyuturyokuZumen_Set('T', "E", pcHeadsData);  // 出力パターンＴ
            } else {
               // 液
               SyuturyokuZumen_Set('S', "E", pcHeadsData);  // 出力パターンＳ
            }
         }
      }

   } else if (s_UtiKbn == "7"){
      //***************  打ち出し区分　７  **********************

      SyuturyokuZumen_Set('U', "E", pcHeadsData);        // 出力パターンＵ

   } else if (s_UtiKbn == "8"){
      //***************  打ち出し区分　８  **********************

      SyuturyokuZumen_Set('V', "E", pcHeadsData);        // 出力パターンＶ

   }

   // C欄
   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);
   if (s_HEADS_DATA[319].TrimRight().ToIntDef(0) != 0) {
//2004/10/25 add???
//   if (G_HEADS_DATA_Rev != 0){
//2004/10/25 add???
      //***************************
      //***  英文：Rev
      //***************************
      // 『C001』
      i_RecNo = 319;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 1, s_Text);
      G_Log_Text = "英文：Rev             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //***************************
      //***  英文：DATE
      //***************************
      // 『C002』
      i_RecNo = 1087;
      s_Text = s_HEADS_DATA[i_RecNo];
      s_Text = JapDayToEngDay( s_Text );
      Excel_Cell_DataSet(41, 2, s_Text);
      G_Log_Text = "英文：年月日          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //***************************
      //***  英文：REVIEWED BY
      //***************************
      // 『C003』
      i_RecNo = 1080;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 4, s_Text);
      G_Log_Text = "英文：REVIEWED BY     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

   } else {

      s_Text = "";
      // 2024.04.08 バーコード変更_S
      //Excel_Cell_DataSet(41, 1, s_Text);  // 英文：Rev
      // 2020.04.08 バーコード変更_E
      Excel_Cell_DataSet(41, 2, s_Text);  // 英文：DATE
      Excel_Cell_DataSet(41, 4, s_Text);  // 英文：REVIEWED BY
   }

   //***************************
   //***  英文：HEADS VER
   //***************************
   // 『C006』
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
   Excel_Cell_DataSet(45, 3, s_Text);
   G_Log_Text = "HEADS VER            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2020.04.21 バーコード変更_S
   //// 2017.12.01 バーコード追加_S
   ////***************************
   ////***  英文：バーコード
   ////***************************
   //s_Text = G_SAKUBAN;
   //// 2018.10.22 バーコード変更_S
   ////s_Text = "*" + s_Text + "*";
   ////Excel_Cell_DataSet(45, 15, s_Text);
   //Excel_Cell_DataSet(10, 18, s_Text);
   //// 2018.10.22 バーコード変更_E
   //G_Log_Text = "バーコード           『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //// 2017.12.01 バーコード追加_E
   s_Text = G_SAKUBAN;
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));
   // 2023.05.30 バーコード変更_S
   //// 2023.04.10 バーコード変更_S
   ////Excel_Cell_DataSet(22, 2, s_Text);
   //if (AllNumChk(G_SAKUBAN.c_str(),0) == 0) {
   //    // オーダ
   //} else {
   //    // 新見積
   //    Excel_Cell_DataSet(21, 2, s_Text);
   //}
   // 2023.04.10 バーコード変更_E
   Excel_Cell_DataSet(22, 5, s_Text);
   G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2023.05.30 バーコード変更_E
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("英文"));
   // 2020.04.21 バーコード変更_E

   //******************************
   //***  Excel Book Close
   //******************************
   // 印刷しないほうのシートをＰＤＦ設定シートに記述する
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "ＰＤＦ設定");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("Item" , OleVariant("ＰＤＦ設定"));

   if (s_HEADS_DATA[1].TrimRight() == "0") {
     Excel_Cell_DataSet(4, 2, "英文");
   } else {
     Excel_Cell_DataSet(4, 2, "和文");
   }
   // 2020.05.28 バーコード変更_S
   Excel_Cell_DataSet(5, 2, "バーコード");
   // 2020.05.28 バーコード変更_E

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
   // 2012.11.07 中国PDF削除
   //// 2012.10.03 PDF出力再追加
   //////PDFファイル出力取消
   ////exApplication.Exec(Function("Run") << "Gsub_PDFOut");
   //exApplication.OleFunction("Run" , OleVariant("Gsub_PDFOut"));
   if ( s_China == "0" ) {
     exApplication.OleFunction("Run" , OleVariant("Gsub_PDFOut"));
   }
   // **********************
   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.OlePropertySet("DisplayAlerts" , OleVariant("false"));
   // ************************
   //Windows 2008Server 移植対応
   exApplication.OlePropertySet("DisplayAlerts", false);
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『承認図表紙』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 承認図表紙　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
AnsiString __fastcall JapDayToEngDay( AnsiString s_Text )
{
   AnsiString yyyy, mm, dd;
   yyyy = s_Text.SubString(1,4);
   mm = s_Text.SubString(6,2);
   dd = s_Text.SubString(9,2);

   try
   {
      switch( mm.ToIntDef(0) )
      {
         case 1 :
            mm = "JAN";
            break;

         case 2 :
            mm = "FEB";
            break;

         case 3 :
            mm = "MAR";
            break;

         case 4 :
            mm = "APR";
            break;

         case 5 :
            mm = "MAY";
            break;

         case 6 :
            mm = "JUN";
            break;

         case 7 :
            mm = "JUL";
            break;

         case 8 :
            mm = "AUG";
            break;

         case 9 :
            mm = "SEP";
            break;

         case 10 :
            mm = "OCT";
            break;

         case 11 :
            mm = "NOV";
            break;

         case 12 :
            mm = "DEC";
            break;

         default :
            break;
      }

      s_Text = mm +","+ dd +","+ yyyy;
   }
   catch(...)
   {
      return s_Text;
   }

   return s_Text;
}

//---------------------------------------------------------------------------
// 日本語関数名： 出力図面　セット
// 概  要      ：
// 引  数      ： Pattern [出力パターン]
// 戻り値      ： なし
// 備  考      ： なし
//---------------------------------------------------------------------------
void __fastcall SyuturyokuZumen_Set(char Pattern, AnsiString Lang, HeadsData* pcHeadsData)
{
   AnsiString s_Text;
   AnsiString s_MeibanZuban;
   AnsiString s_MeibanStr;
   int        i_RowCnt;
   int        i_MirrorLoop;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   // 2010.08.19 中国語対応
   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   AnsiString s_China;           // 言語区分("0":日本語、"1":中国語)
   // *********************

   // AnsiString型HEADSﾃﾞｰﾀをﾛｰｶﾙ変数にｾｯﾄ
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return;

   // 2010.08.19 中国語対応
   i_RecNo = 851;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "1" ) {
      s_China = "1";                                                 // 中国語
   } else {
      s_China = "0";                                                 // 日本語
   }
   // *********************
   // 2011.05.30 常熟用承認図
   // 2012.01.20 常熟フラグ
   //i_RecNo = 5;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // 中国語
   //}
   //i_RecNo = 401;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // 中国語
   //}
   i_RecNo = 1107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "CHINA" ) {
      s_China = "1";                                                 // 中国語
   }
   // ***********************

   //*******************
   //***  図面欄初期化
   //*******************
   for (i_RowCnt = 0; i_RowCnt < 10; i_RowCnt++){
      s_Text = "";
      Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);
      Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
      Excel_Cell_DataSet(23 + i_RowCnt, 9, s_Text);
   }

   i_RowCnt = 0;

   //*******************
   //***  要目表
   //*******************
   switch (Pattern) {
      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M':
         if (Lang == "J"){
            // 2010.08.19 中国語対応
            //s_Text = "要目表";
            if ( s_China == "0" ) {
                s_Text = "要目表";
            } else {
                // 2011.02.09 中国語対応
                //s_Text = "主要U+39033/U+目表";
                s_Text = "主要/U+39033/U+目表";
                // *********************
            }
            // *********************
         } else {
            s_Text = "PRINCIPAL ITEMS";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 製造番号桁数追加対応 Mod
		 //s_Text = "M" + G_SAKUBAN;
		 s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;
         break;

      default :
         break;
   }

    //-------------------------------------------------
    // 2007.11.02 出力順変更
    // 旧ｴﾚﾒﾝﾄ構成図、旧組立図をﾐﾗｰ仕様A/Bごとにまとめる

    i_MirrorLoop = 0;
    while( i_MirrorLoop <= 1 )
    {

        //***************************
        //***  エレメント構成図
        //***************************
        switch (Pattern) {
            case 'H':  case 'I': case 'J': case 'N': case 'O': case 'P':
            case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':

                //----------------------------------
                // 2007.10.09 ﾐﾗｰ仕様対応

                if (Lang == "J"){
                    // 2010.08.19 中国語対応
                    //s_Text = "エレメント構成図";
                    if ( s_China == "0" ) {
                        s_Text = "エレメント構成図";
                    } else {
                        // 2011.05.30 常熟用承認図
                        //s_Text = "要素/U+26500/U+成/U+22270/U+";
                        s_Text = "板片/U+26500/U+成/U+22270/U+";
                        // ***********************
                    }
                    // *********************
                } else {
                    s_Text = "DWG. OF PLATE ARRANGEMENT";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

				//2023/4/21 製造番号桁数追加対応 Mod
				//s_Text = "A" + G_SAKUBAN;       // A[作番]
				s_Text = "A" + GetFileSeizouBangou(G_SAKUBAN);       // A[作番]

                // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // B[作番]A
                    } else {
                        s_Text = s_Text + "B";   // B[作番]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

            default :
                break;
        }

        //*******************
        //***  組立図
        //*******************
        switch (Pattern) {
            case 'A': case 'U':
                break;

            default :

                //----------------------------------
                // 2007.10.09 ﾐﾗｰ仕様対応

                if (Lang == "J"){
                    // 2010.08.19 中国語対応
                    //s_Text = "組立図";
                    if ( s_China == "0" ) {
                        s_Text = "組立図";
                    } else {
                        // 2011.05.30 常熟用承認図
                        //s_Text = "装配/U+22270/U+";
                        s_Text = "外形/U+22270/U+";
                        // ***********************
                    }
                    // *********************
                } else {
                    s_Text = "ASSEMBLY DRAWING";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

				//2023/4/21 製造番号桁数追加対応 Mod
				//s_Text = "B" + G_SAKUBAN;       // B[作番]
				s_Text = "B" + GetFileSeizouBangou(G_SAKUBAN);       // B[作番]

                // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // B[作番]A
                    } else {
                        s_Text = s_Text + "B";   // B[作番]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

            // 2007.10.09 ﾐﾗｰ仕様対応
            //----------------------------------
        }

        i_MirrorLoop++;
        if( !pcHeadsData->GetMirrorFlag() ) break;
    }

    // 2007.11.02 出力順変更
    //-------------------------------------------------

   //2004.02.20 ﾄﾞﾚﾝﾊﾟﾝ追加
   //***************************
   //***  ﾄﾞﾚﾝﾊﾟﾝ図面
   //***************************
   //2020.11.17 断熱材追加_S
   ////2018.04.17 断熱材追加_S
   ////if (s_HEADS_DATA[264].TrimRight() != "1.00"){
   //if ((s_HEADS_DATA[264].TrimRight() != "1.00") && (s_HEADS_DATA[264].TrimRight() != "2.00")){
   ////2018.04.17 断熱材追加_E
   if ((s_HEADS_DATA[264].TrimRight() != "1.00") && (s_HEADS_DATA[264].TrimRight() != "2.00") && (s_HEADS_DATA[264].TrimRight() != "3.00")){
   //2020.11.17 断熱材追加_E
      //ﾄﾞﾚﾝﾊﾟﾝ がないため記述しません
   }
   else if (s_HEADS_DATA[1330].Trim() == ""){
      //図番がないため記述しません
   }
   else {
      switch (Pattern) {
         case 'H': case 'I': case 'J': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
            if (Lang == "J"){
               // 2010.08.19 中国語対応
               //s_Text = "ドレンパン";
               if ( s_China == "0" ) {
                   s_Text = "ドレンパン";
               } else {
                   s_Text = "排水/U+30424/U+";
               }
               // *********************
            } else {
               s_Text = "DRAINPAN";
            }
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

            s_Text = s_HEADS_DATA[1330].Trim();
            s_Text = s_Text.SubString(15,s_Text.Length()-14);
            s_Text = s_Text.SubString(1,s_Text.Pos("　L")-1);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;
            break;

         default :
            break;
      }
   }
   //**********************************

   //2018.03.12 銘板追加
   //***************************
   //***  銘板図面
   //***************************
   if (s_HEADS_DATA[272].TrimRight() != "1"){
      //銘板 がないため記述しません
   }
   else {
      s_MeibanZuban = GetMeibanZuban();
      if (s_MeibanZuban != "") {
        if (Lang == "J"){
           s_Text = "銘板図";
        } else {
           s_Text = "NAMEPLATE";
        }

        // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
        if( pcHeadsData->GetMirrorFlag() )
        {
            if (Lang == "J"){
               s_Text = "銘板図A";
            } else {
               s_Text = "NAMEPLATE A";
            }

            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);
            //2019.08.08 銘板図番変更_S
            //s_Text = ZenToHan(s_MeibanZuban);
			//s_Text = s_Text.SubString(0, 7);
			//2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "NP" + G_SAKUBAN;
			s_Text = "NP" + GetFileSeizouBangou(G_SAKUBAN);
            //2019.08.08 銘板図番変更_E
            s_Text = s_Text + "A";
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
            i_RowCnt++;

            if (Lang == "J"){
               s_Text = "銘板図B";
            } else {
               s_Text = "NAMEPLATE B";
            }

            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);
            //2019.08.08 銘板図番変更_S
            //s_Text = ZenToHan(s_MeibanZuban);
            //s_Text = s_Text.SubString(0, 7);
            //2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "NP" + G_SAKUBAN;
			s_Text = "NP" + GetFileSeizouBangou(G_SAKUBAN);
			//2019.08.08 銘板図番変更_E
            s_Text = s_Text + "B";
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
            i_RowCnt++;

        } else {
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2019.08.08 銘板図番変更_S
            //s_Text = ZenToHan(s_MeibanZuban);
            //s_Text = s_Text.SubString(0, 7);
			//2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "NP" + G_SAKUBAN;
			s_Text = "NP" + GetFileSeizouBangou(G_SAKUBAN);
			//2019.08.08 銘板図番変更_E
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
            i_RowCnt++;
        }
      }
   }
   //**********************************

   //***************************
   //***  リスト
   //***************************
   switch (Pattern) {
      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M': case 'Q': case 'R': case 'S': case 'T':
         if (Lang == "J"){
            // 2010.08.19 中国語対応
            //s_Text = "パーツリスト";
            if ( s_China == "0" ) {
                s_Text = "パーツリスト";
            } else {
                s_Text = "零件清/U+21333/U+";
            }
            // *********************
         } else {
            s_Text = "PARTS LIST";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 製造番号桁数追加対応 Mod
		 //s_Text = "PAR" + G_SAKUBAN;
		 s_Text = "PAR" + GetFileSeizouBangou(G_SAKUBAN);
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;

         if (Lang == "J"){
            // 2010.08.19 中国語対応
            //s_Text = "付属品リスト";
            if ( s_China == "0" ) {
                s_Text = "付属品リスト";
            } else {
                // 2011.02.09 中国語対応
                //s_Text = "附件清/U+21333/U+";
                s_Text = "附属品清/U+21333/U+";
                // *********************
            }
            // *********************
         } else {
            s_Text = "ACCESSORIES LIST";
         }
		 Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 製造番号桁数追加対応 Mod
		 //s_Text = "ACT" + G_SAKUBAN;
		 s_Text = "ACT" + GetFileSeizouBangou(G_SAKUBAN);
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;

         if (Lang == "J"){
            // 2010.08.19 中国語対応
            //s_Text = "工具リスト";
            if ( s_China == "0" ) {
                s_Text = "工具リスト";
            } else {
                s_Text = "工具清/U+21333/U+";
            }
            // *********************
         } else {
            s_Text = "TOOL LIST";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 製造番号桁数追加対応 Mod
		 //s_Text = "TOL" + G_SAKUBAN;
		 s_Text = "TOL" + GetFileSeizouBangou(G_SAKUBAN);
		 Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;
         break;

      default :
         break;
   }

   //***************************
   //***  圧力容器（液）
   //***************************
   switch (Pattern) {
//    case 'D': case 'F': case 'H': case 'K': case 'N': case 'Q': case 'S':
      case 'D':
         if (Lang == "J"){
            // 2010.08.19 中国語対応
            //s_Text = "強度計算書";
            if ( s_China == "0" ) {
                s_Text = "強度計算書";
            } else {
                s_Text = "/U+5F3A/U+度清/U+5355/U+";
            }
            // *********************
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "PVC" + G_SAKUBAN;
			s_Text = "PVC" + GetFileSeizouBangou(G_SAKUBAN);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;

           //----------------------------------
           // 2007.10.09 ﾐﾗｰ仕様対応

           i_MirrorLoop = 0;
           while( i_MirrorLoop <= 1 )
           {
              // 2010.08.19 中国語対応
              //s_Text = "フローシート";
              if ( s_China == "0" ) {
                  s_Text = "フローシート";
              } else {
                  s_Text = "FLOW SEAT";
              }
              // *********************
              Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			  //2023/4/21 製造番号桁数追加対応 Mod
			  //s_Text = "PVF" + G_SAKUBAN;       // PVF[作番]
			  s_Text = "PVF" + GetFileSeizouBangou(G_SAKUBAN);       // PVF[作番]

              // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
              if( pcHeadsData->GetMirrorFlag() )
              {
                 if( i_MirrorLoop == 0 ){
                    s_Text = s_Text + "A";   // PVF[作番]A
                 } else {
                    s_Text = s_Text + "B";   // PVF[作番]B
                 }
              }

              Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

              i_RowCnt++;
              i_MirrorLoop++;
              if( !pcHeadsData->GetMirrorFlag() ) break;
           }

           // 2007.10.09 ﾐﾗｰ仕様対応
           //----------------------------------
         }

         break;

      default :
         break;
   }

   //***************************
   //***  圧力容器（スチーム）
   //***************************
   switch (Pattern) {
//    case 'E': case 'G': case 'I': case 'L': case 'O': case 'R': case 'T':
      case 'E':
         if (Lang == "J"){
            // 2010.08.19 中国語対応
            //s_Text = "強度計算書";
            if ( s_China == "0" ) {
                s_Text = "強度計算書";
            } else {
                s_Text = "/U+5F3A/U+度清/U+5355/U+";
            }
            // *********************
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "PVC" + G_SAKUBAN;
			s_Text = "PVC" + GetFileSeizouBangou(G_SAKUBAN);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;

            //----------------------------------
            // 2007.10.09 ﾐﾗｰ仕様対応

            i_MirrorLoop = 0;
            while( i_MirrorLoop <= 1 )
            {
               // 2010.08.19 中国語対応
               //s_Text = "フローシート";
               if ( s_China == "0" ) {
                   s_Text = "フローシート";
               } else {
                   s_Text = "FLOW SEAT";
               }
               // *********************
               Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			   //2023/4/21 製造番号桁数追加対応 Mod
			   //s_Text = "PVF" + G_SAKUBAN;       // PVF[作番]
			   s_Text = "PVF" + GetFileSeizouBangou(G_SAKUBAN);       // PVF[作番]

               // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
               if( pcHeadsData->GetMirrorFlag() )
               {
                  if( i_MirrorLoop == 0 ){
                     s_Text = s_Text + "A";   // PVF[作番]A
                  } else {
                     s_Text = s_Text + "B";   // PVF[作番]B
                  }
               }

               Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

               i_RowCnt++;
               i_MirrorLoop++;
               if( !pcHeadsData->GetMirrorFlag() ) break;
            }

            // 2007.10.09 ﾐﾗｰ仕様対応
            //----------------------------------

            // 2010.08.19 中国語対応
            //s_Text = "スチーム送入量計算書";
            if ( s_China == "0" ) {
                s_Text = "スチーム送入量計算書";
            } else {
                s_Text = "蒸汽寄送入量清/U+5355/U+";
            }
            // *********************
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "PVC" + G_SAKUBAN;
			s_Text = "PVC" + GetFileSeizouBangou(G_SAKUBAN);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;
         }

         break;

      default :
         break;
   }

   //*******************
   //***  旧要目表
   //*******************
   switch (Pattern) {
      case 'K': case 'L': case 'M':
         if (Lang == "J"){
            // 2010.08.19 中国語対応
            //s_Text = "旧要目表";
            if ( s_China == "0" ) {
                s_Text = "旧要目表";
            } else {
                s_Text = "旧主要/U+39033/U+目表";
            }
            // *********************
         } else {
            s_Text = "OLD PRINCIPAL ITEMS";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 s_Text = "M" + s_HEADS_DATA[327].TrimRight();
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;
         break;

      default :
         break;
   }

    //-------------------------------------------------
    // 2007.11.02 出力順変更
    // 旧ｴﾚﾒﾝﾄ構成図、旧組立図をﾐﾗｰ仕様A/Bごとにまとめる

    i_MirrorLoop = 0;
    while( i_MirrorLoop <= 1 )
    {
        //***************************
        //***  旧エレメント構成図
        //***************************
        switch (Pattern) {
            case 'N': case 'O': case 'P': case 'S': case 'T':

                //----------------------------------
                // 2007.10.17 ﾐﾗｰ仕様対応

                if (Lang == "J"){
                   // 2010.08.19 中国語対応
                   //s_Text = "旧エレメント構成図";
                   if ( s_China == "0" ) {
                       s_Text = "旧エレメント構成図";
                   } else {
                       s_Text = "旧要素/U+26500/U+成/U+22270/U+";
                   }
                   // *********************
                } else {
                    s_Text = "OLD DWG. OF PLATE ARRANGEMENT";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

                s_Text = "A" + s_HEADS_DATA[327].TrimRight();

                // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // A[作番]A
                    } else {
                        s_Text = s_Text + "B";   // A[作番]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

                // 2007.10.17 ﾐﾗｰ仕様対応
                //----------------------------------

            default :
                break;
        }

        //*******************
        //***  旧組立図
        //*******************
        switch (Pattern) {
            case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'S': case 'T':

                //----------------------------------
                // 2007.10.09 ﾐﾗｰ仕様対応

                if (Lang == "J"){
                    // 2010.08.19 中国語対応
                    //s_Text = "旧組立図";
                    if ( s_China == "0" ) {
                        s_Text = s_Text = "旧組立図";
                    } else {
                        s_Text = "旧装配/U+22270/U+";
                    }
                    // *********************
                } else {
                    s_Text = "OLD ASSEMBLY DRAWING";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

                s_Text = "B" + s_HEADS_DATA[327].TrimRight();   // B[作番]

                // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // B[作番]A
                    } else {
                        s_Text = s_Text + "B";   // B[作番]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

                // 2007.10.09 ﾐﾗｰ仕様対応
                //----------------------------------

            default :
                break;
        }

        i_MirrorLoop++;
        if( !pcHeadsData->GetMirrorFlag() ) break;
    }

    // 2007.11.02 出力順変更
    //-------------------------------------------------

   //***************************
   //***  検査要領書
   //***************************
   switch (Pattern) {
      case 'V':
         //----------------------------------
         // 2007.10.17 ﾐﾗｰ仕様対応

         i_MirrorLoop = 0;
         while( i_MirrorLoop <= 1 )
         {
             if (Lang == "J"){
                // 2010.08.19 中国語対応
                //s_Text = "検査要領書";
                if ( s_China == "0" ) {
                    s_Text = "検査要領書";
                } else {
                    s_Text = "/U+26816/U+/U+26597/U+要/U+39046/U+/U+20070/U+";
                }
                // *********************
             } else {
                s_Text = "INSPECTION PROCEDURE";
             }
             Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			 //2023/4/21 製造番号桁数追加対応 Mod
			 //s_Text = "D" + G_SAKUBAN;
			 s_Text = "D" + GetFileSeizouBangou(G_SAKUBAN);

              // ﾐﾗｰ仕様の場合、"A"または"B"を追加する(2行出力)
             if( pcHeadsData->GetMirrorFlag() )
             {
                if( i_MirrorLoop == 0 ){
                    s_Text = s_Text + "A";   // D[作番]A
                } else {
                    s_Text = s_Text + "B";   // D[作番]B
                }
             }

             Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

             i_RowCnt++;
             i_MirrorLoop++;
             if( !pcHeadsData->GetMirrorFlag() ) break;
         }
         //i_RowCnt++;
         break;

         // 2007.10.17 ﾐﾗｰ仕様対応
         //----------------------------------

      default :
         break;
   }


    //-------------------------------------------------
    // OGI 2015.01.29 アキュームレータ部品対応
    // 形式にアキュームレータ部品が存在する場合に「アキュームレータ」の行を追加する

    //***************************
    //***  アキュームレータ
    //***************************
    AnsiString s_Nzl_SyuruiE3 = s_HEADS_DATA[981].TrimRight();
    AnsiString s_Nzl_SyuruiE4 = s_HEADS_DATA[996].TrimRight();
    if (s_Nzl_SyuruiE3 == "8" || s_Nzl_SyuruiE4 == "8"){
        switch (Pattern) {
          case 'C': case 'F': case 'G': case 'K': case 'L': case 'M': case 'Q': case 'R': case 'S': case 'T':
             if (Lang == "J"){
                if ( s_China == "0" ) {
                    s_Text = "アキュームレータ";
                } else {
                    s_Text = ""; // 中国語対応必要
                }
                // *********************
             } else {
                s_Text = "ACCUMULATOR DRAWING";
             }
             Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			 //2023/4/21 製造番号桁数追加対応 Mod
			 //s_Text = "TOL" + G_SAKUBAN;
			 s_Text = "TOL" + GetFileSeizouBangou(G_SAKUBAN);
             Excel_Cell_DataSet(23 + i_RowCnt, 6, "E430747");

             i_RowCnt++;
             break;

          default :
             break;
        }
    }
}

