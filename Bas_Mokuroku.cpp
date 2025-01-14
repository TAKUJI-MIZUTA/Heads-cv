//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：図面目録  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.05.31
//　 更 新 日：2002.09.10
//             2007.10.09 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Mokuroku.h"

#include <math.h>       //2004.08.16 切り上げ
#include <StrUtils.hpp> //2007.11.16 機器名称抽出

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 図面目録　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Mokuroku(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_Text;            // 汎用文字列
   //2004.07.15 変数追加
   AnsiString s_Text2;           // 汎用文字列２
   int        i_Length;          // 文字数
   //*******************
   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   //2019.10.31 NK量産外注記_S
   AnsiString s_Text3;           // 汎用文字列３
   //2019.10.31 NK量産外注記_E
   // 2020.04.20 温水乾燥注記追加_S
   AnsiString s_Text4;           // 汎用文字列４
   AnsiString s_Text5;           // 汎用文字列５
   // 2020.04.20 温水乾燥注記追加_E

   AnsiString s_UtiKbn;
   int        i_Pos;

   // 2007.11.15 変数追加
   AnsiString s_TextA;  // 機器名称(ミラーA仕様)
   AnsiString s_TextB;  // 機器名称(ミラーB仕様)
   //*******************

   // 2022.01.06 ｽﾍﾟｰｻ注記追加_E
   AnsiString s_S1Nzl_Syurui; // S1ﾉｽﾞﾙ種類
   AnsiString s_S2Nzl_Syurui; // S2ﾉｽﾞﾙ種類
   AnsiString s_S3Nzl_Syurui; // S3ﾉｽﾞﾙ種類
   AnsiString s_S4Nzl_Syurui; // S4ﾉｽﾞﾙ種類
   AnsiString s_E1Nzl_Syurui; // E1ﾉｽﾞﾙ種類
   AnsiString s_E2Nzl_Syurui; // E2ﾉｽﾞﾙ種類
   AnsiString s_E3Nzl_Syurui; // E3ﾉｽﾞﾙ種類
   AnsiString s_E4Nzl_Syurui; // E4ﾉｽﾞﾙ種類
   // 2022.01.06 ｽﾍﾟｰｻ注記追加_E

   // 2022.04.11 特孔判定修正
   bool bTok_S;               // S側特孔ﾌﾗｸﾞ
   bool bTok_E;               // E側特孔ﾌﾗｸﾞ
   // 2022.04.11 特孔判定修正

   bool bRet;
   AnsiString s_HEADS_DATA[1500];

   // 元仕様でない場合は処理不要(正常終了)
   if(!pcHeadsData->GetSpecType()==DEF_SPEC_TYPE_NONE) return(true);

   // HEADSﾃﾞｰﾀをﾛｰｶﾙ変数にｾｯﾄ
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   s_Tmplate_Pass = G_Template_Dir + "図面目録.xls";                 // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "DLT" + G_SAKUBAN + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "DLT" + GetFileSeizouBangou(G_SAKUBAN) + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『図面目録』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------";
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
   //***                         図面目録　データセット                      ***
   //***                                                                     ***
   //***************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "図面目録");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("図面目録"));

   // 打ち出し区分
   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);

   // 2017.06.28 配付先追加_S
   //*******************
   //***  配  付  先
   //*******************
   i_RecNo = 438;   // 業界ｺｰﾄﾞ
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0009"){
       // 業界ｺｰﾄﾞ009(まるごとパック、出張工事)
       s_Text = "●　製造部｢5部｣(工務控え ・ｻｰﾋﾞｽ　・ﾌﾟﾚｰﾄ　・組立　・発送　・(ﾉｽﾞﾙ→組立orﾉｽﾞﾙ→前工程→組立or前工程→組立))　品質保証室｢1部｣";
   } else {
       s_Text = "●　製造部｢5部｣(工務控え ・ﾌﾟﾚｰﾄ　・組立　・発送　・(ﾉｽﾞﾙ→組立orﾉｽﾞﾙ→前工程→組立or前工程→組立))　品質保証室｢1部｣";
   }
   Excel_Cell_DataSet(1, 1, s_Text);
   G_Log_Text = "配付先               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   // 2017.06.28 配付先追加_E

   //*******************
   //***  上段日付
   //*******************
   // 『A001』
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(3, 14, s_Text);
   G_Log_Text = "上段日付             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2011.05.30 常熟製造番号追加
   //*******************
   //*** 常熟製造番号
   //*******************
   i_RecNo = 1103;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 1, s_Text);
   G_Log_Text = "常熟製造番号         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //****************************

   //*******************
   //***  製造番号
   //*******************
   // 『A002』
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

   Excel_Cell_DataSet(7, 1, s_Text);
   G_Log_Text = "製造番号            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  目録種類
   //*******************
   // 『A003』
//   s_Text = "H4標準版";
//   Excel_Cell_DataSet(6, 3, s_Text);
//   G_Log_Text = "目録種類            『" + s_Text + "』をセット。";
//   Write_Log(G_Log_Text);

   //*******************
   //***  型  式
   //*******************
   // 『A004』
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(7, 3, s_Text);
   G_Log_Text = "型式                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  台  数
   //*******************
   // 『A005』
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(7, 7, s_Text);
      G_Log_Text = "台数                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "台数          RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //*******************
   //***  納入先
   //*******************
   // 『A006』
   i_RecNo = 1065;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 9, s_Text);
   G_Log_Text = "納入先               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //********************************
   //***  Ｂ欄　出力図面セット
   //********************************
   i_Pos = s_HEADS_DATA[36].Pos("M");
   if (i_Pos != 0){
      SyuturyokuZumen_Set_S(s_UtiKbn, true, pcHeadsData);     // 舶用
   } else {
      SyuturyokuZumen_Set_S(s_UtiKbn, false, pcHeadsData);    // 標準
   }


//   if (s_UtiKbn == "0" || s_UtiKbn == "9"){
//      //***************  打ち出し区分　０，９  **********************
//
//      SyuturyokuZumen_Set_S('A', "J");     // 出力パターンＡ
//
//   } else if (s_UtiKbn == "1"){
//      //***************  打ち出し区分　１  **********************
//      SyuturyokuZumen_Set_S('B', "J");     // 出力パターンＢ
//
//   } else if (s_UtiKbn == "2"){
//      //***************  打ち出し区分　２  **********************
//
//      SyuturyokuZumen_Set_S('C', "J");     // 出力パターンＣ
//
//   } else if (s_UtiKbn == "3"){
//      //***************  打ち出し区分　３  **********************
//
//      if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//         // ｽﾁｰﾑ
//         SyuturyokuZumen_Set_S('E', "J");  // 出力パターンＥ
//      } else {
//         // 液
//         SyuturyokuZumen_Set_S('D', "J");  // 出力パターンＤ
//      }
//
//   } else if (s_UtiKbn == "4"){
//      //***************  打ち出し区分　４  **********************
//      s_Text = s_HEADS_DATA[29].TrimRight();   // 圧力容器（規格ｺｰﾄﾞ）
//
//      if (s_HEADS_DATA[327].TrimRight() == ""){
//         // 改造無し
//         i_Pos = s_HEADS_DATA[36].Pos("M");
//         if (i_Pos != 0){
//            // 舶用
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ｽﾁｰﾑ
//                  SyuturyokuZumen_Set_S('G', "J");  // 出力パターンＧ
//               } else {
//                  // 液
//                  SyuturyokuZumen_Set_S('F', "J");  // 出力パターンＦ
//               }
//            } else {
//               SyuturyokuZumen_Set_S('C', "J");     // 出力パターンＣ
//            }
//         } else {
//            // 非舶用
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ｽﾁｰﾑ
//                  SyuturyokuZumen_Set_S('I', "J");  // 出力パターンＩ
//               } else {
//                  // 液
//                  SyuturyokuZumen_Set_S('H', "J");  // 出力パターンＨ
//               }
//            } else {
//               SyuturyokuZumen_Set_S('J', "J");     // 出力パターンＪ
//            }
//         }
//
//      } else {
//         // 改造有り
//         i_Pos = s_HEADS_DATA[36].Pos("M");
//         if (i_Pos != 0){
//            // 舶用
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ｽﾁｰﾑ
//                  SyuturyokuZumen_Set_S('L', "J");  // 出力パターンＬ
//               } else {
//                  // 液
//                  SyuturyokuZumen_Set_S('K', "J");  // 出力パターンＫ
//               }
//            } else {
//               SyuturyokuZumen_Set_S('M', "J");     // 出力パターンＭ
//            }
//         } else {
//            // 非舶用
//            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
//               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//                  // ｽﾁｰﾑ
//                  SyuturyokuZumen_Set_S('O', "J");  // 出力パターンＯ
//               } else {
//                  // 液
//                  SyuturyokuZumen_Set_S('N', "J");  // 出力パターンＮ
//               }
//            } else {
//               SyuturyokuZumen_Set_S('P', "J");     // 出力パターンＰ
//            }
//         }
//
//      }
//
//   } else if (s_UtiKbn == "5"){
//      //***************  打ち出し区分　５  **********************
//
//      SyuturyokuZumen_Set_S('J', "J");
//
//   } else if (s_UtiKbn == "6"){
//      //***************  打ち出し区分　６  **********************
//
//      if (s_HEADS_DATA[327].TrimRight() == ""){
//         // 改造無し
//         if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//            // ｽﾁｰﾑ
//            SyuturyokuZumen_Set_S('R', "J");  // 出力パターンＲ
//         } else {
//            // 液
//            SyuturyokuZumen_Set_S('Q', "J");  // 出力パターンＱ
//         }
//      } else {
//         // 改造有り
//         if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
//            // ｽﾁｰﾑ
//            SyuturyokuZumen_Set_S('T', "J");  // 出力パターンＴ
//         } else {
//            // 液
//            SyuturyokuZumen_Set_S('S', "J");  // 出力パターンＳ
//         }
//      }
//
//   } else if (s_UtiKbn == "7"){
//      //***************  打ち出し区分　７  **********************
//
//      SyuturyokuZumen_Set_S('U', "J");        // 出力パターンＵ
//
//   } else if (s_UtiKbn == "8"){
//      //***************  打ち出し区分　８  **********************
//
//      SyuturyokuZumen_Set_S('V', "J");        // 出力パターンＶ
//
//   }

   //---------------------------------------
   // 2007.10.10 ﾐﾗｰ仕様対応
   // 列Bに出力する注記ﾃﾞｰﾀを2行下にずらす

   //2019/05/17 応用技術追加
   if (!bIsNotKatashiki) {
   //*******************
   //*** 注記(ﾌﾚｰﾑｴﾗｰ)
   //*******************
   // 『B014』
   i_RecNo = 1346;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(28, 2, s_Text);
   Excel_Cell_DataSet(30, 2, s_Text);
   G_Log_Text = "注記(ﾌﾚｰﾑｴﾗｰ)        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(圧力計ｴﾗｰ)
   //*******************
   // 『B015』
   i_RecNo = 1345;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(29, 2, s_Text);
   Excel_Cell_DataSet(31, 2, s_Text);
   G_Log_Text = "注記(圧力系ｴﾗｰ)      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(温度計ｴﾗｰ)
   //*******************
   // 『B016』
   i_RecNo = 1344;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(30, 2, s_Text);
   Excel_Cell_DataSet(32, 2, s_Text);
   G_Log_Text = "注記(温度計ｴﾗｰ)      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(端管ｴﾗｰ)
   //*******************
   // 『B017』
   i_RecNo = 1343;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(31, 2, s_Text);
   Excel_Cell_DataSet(33, 2, s_Text);
   G_Log_Text = "注記(端管ｴﾗｰ)        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾉｽﾞﾙｴﾗｰ)
   //*******************
   // 『B018』
   i_RecNo = 1342;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(32, 2, s_Text);
   Excel_Cell_DataSet(34, 2, s_Text);
   G_Log_Text = "注記(ﾉｽﾞﾙｴﾗｰ)        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(IERPVｴﾗｰ)
   //*******************
   // 『B019』
   i_RecNo = 1340;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(33, 2, s_Text);
   Excel_Cell_DataSet(35, 2, s_Text);
   G_Log_Text = "注記(IERPVｴﾗｰ)       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(特殊)
   //*******************
   // 『B021』
   i_RecNo = 1334;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(35, 2, s_Text);
   Excel_Cell_DataSet(37, 2, s_Text);
   G_Log_Text = "注記(特殊)           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2011.02.09 IERRｴﾗｰ追加
   //*******************
   //*** 注記(IERRｴﾗｰ)
   //*******************
   // 『B022』
   i_RecNo = 1341;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(38, 2, s_Text);
   G_Log_Text = "注記(IERRｴﾗｰ)        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //***********************

   //*******************
   //*** 注記(多ﾊﾟｽ)
   //*******************
   // 『B023』
   i_RecNo = 1333;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(37, 2, s_Text);
   Excel_Cell_DataSet(39, 2, s_Text);
   G_Log_Text = "注記(多ﾊﾟｽ)          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   // 2007.10.10 ﾐﾗｰ仕様対応
   //---------------------------------------

   //*******************
   //*** 継手規格
   //*******************
   // 『B150』
   //2009.06.23 ASMEﾉｽﾞﾙ名称変更
   //s_Text = "Ｂ側継手 " + s_HEADS_DATA[1154].TrimRight() + "  "+ "Ａ側継手 " + s_HEADS_DATA[1158].TrimRight();
   s_Text2 = s_HEADS_DATA[1154].TrimRight();
   if ( s_Text2.SubString(1,17) == "ASME CLASS150-NPS" ) {
       s_Text2 = "ASME CLS150-" + s_Text2.Delete(1,17);
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS300-NPS" ) {
       s_Text2 = "ASME CLS300-" + s_Text2.Delete(1,17);
   // 2013.04.03 ﾉｽﾞﾙ規格追加
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS400-NPS" ) {
       s_Text2 = "ASME CLS400-" + s_Text2.Delete(1,17);
   // ***********************
   // 2011.11.29 ﾉｽﾞﾙ規格変更
   //// 2011.06.20 ﾉｽﾞﾙ規格変更
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.0MPa-DN" ) {
   //    s_Text2 = "GB PN1.0-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.6MPa-DN" ) {
   //    s_Text2 = "GB PN1.6-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN2.5MPa-DN" ) {
   //    s_Text2 = "GB PN2.5-DN" + s_Text2.Delete(1,14);
   //// ***********************
   // ***********************
   }
   //2014.05.28 継手規格表記変更
   //s_Text = "Ｂ側継手 " + s_Text2;
   if ( s_Text2.SubString(1,3) == "JIS" ) {
       s_Text = "Ｂ側継手 " + s_Text2;
   } else {
       s_Text = "Ｂ側継手 注" + s_Text2;
   }
   //***************************
   s_Text2 = s_HEADS_DATA[1158].TrimRight();
   if ( s_Text2.SubString(1,17) == "ASME CLASS150-NPS" ) {
       s_Text2 = "ASME CLS150-" + s_Text2.Delete(1,17);
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS300-NPS" ) {
       s_Text2 = "ASME CLS300-" + s_Text2.Delete(1,17);
   // 2013.04.03 ﾉｽﾞﾙ規格追加
   } else if ( s_Text2.SubString(1,17) == "ASME CLASS400-NPS" ) {
       s_Text2 = "ASME CLS400-" + s_Text2.Delete(1,17);
   // ***********************
   // 2011.11.29 ﾉｽﾞﾙ規格変更
   //// 2011.06.20 ﾉｽﾞﾙ規格変更
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.0MPa-DN" ) {
   //    s_Text2 = "GB PN1.0-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN1.6MPa-DN" ) {
   //    s_Text2 = "GB PN1.6-DN" + s_Text2.Delete(1,14);
   //} else if ( s_Text2.SubString(1,14) == "GB PN2.5MPa-DN" ) {
   //    s_Text2 = "GB PN2.5-DN" + s_Text2.Delete(1,14);
   //// ***********************
   // ***********************
   }
   //2014.05.28 継手規格表記変更
   //s_Text = s_Text + "  Ａ側継手 " + s_Text2;
   if ( s_Text2.SubString(1,3) == "JIS" ) {
       s_Text = s_Text + "  Ａ側継手 " + s_Text2;
   } else {
       s_Text = s_Text + "  Ａ側継手 注" + s_Text2;
   }
   //***************************
   //2004.04.23 YX-80継手規格なし
   if ( G_KS_Syurui == "ＹＸ" && G_KS_Model == "８０" ){
      s_Text = "";
   }
   //****************************
   Excel_Cell_DataSet(10, 8, s_Text);
   G_Log_Text = "継手        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2004.07.15 国交省銘板追加
   //*******************
   //*** 国交省銘板
   //*******************
   // 『B151』
   if (s_HEADS_DATA[268].TrimRight() == "1") { // 国交省銘板
      s_Text = "国交省仕様銘板                 B側　　　             　A側";
      Excel_Cell_DataSet(11, 8, s_Text);
      G_Log_Text = "国交省銘板           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //*******************
   //*** 交換熱量
   //*******************
   // 『B152』
   if (s_HEADS_DATA[268].TrimRight() == "1") { // 国交省銘板
      i_RecNo = 107;    // 交換熱量
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("########0",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 1) {
            s_Text ="        " + s_Text;
         } else if (i_Length == 2) {
            s_Text ="       " + s_Text;
         } else if (i_Length == 3) {
            s_Text ="      " + s_Text;
         } else if (i_Length == 4) {
            s_Text ="     " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="    " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="   " + s_Text;
         } else if (i_Length == 7) {
            s_Text ="  " + s_Text;
         } else if (i_Length == 8) {
            s_Text =" " + s_Text;
         }
         s_Text = "交換熱量(kW)                           " + s_Text;
      } else {
         G_ErrLog_Text = "交換熱量        RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
         s_Text = "交換熱量(kW)                           *********";
      }
      Excel_Cell_DataSet(12, 8, s_Text);
      G_Log_Text = "交換熱量             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //*******************
   //*** 入口出口温度
   //*******************
   // 『B153』
   if (s_HEADS_DATA[268].TrimRight() == "1" && s_HEADS_DATA[269].TrimRight() == "11") { // 国交省銘板Ｈ１１
      i_RecNo = 852;    // B側入口温度
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="   " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="  " + s_Text;
         } else if (i_Length == 6) {
            s_Text =" " + s_Text;
         }
         s_Text2 = "入口/出口温度(℃)　　" + s_Text;
      } else {
         G_ErrLog_Text = "B側入口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
         s_Text2 = "入口/出口温度(℃)　　****.**";
      }
      i_RecNo = 854;    // B側出口温度
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="/   " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="/  " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="/ " + s_Text;
         } else {
            s_Text ="/" + s_Text;
         }
         s_Text2 = s_Text2 + s_Text;
      } else {
         G_ErrLog_Text = "B側出口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
         s_Text2 = s_Text2 + "/****.**";
      }
      i_RecNo = 853;    // A側入口温度
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="         " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="        " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="       " + s_Text;
         } else {
            s_Text ="      " + s_Text;
         }
         s_Text2 = s_Text2 + s_Text;
      } else {
         G_ErrLog_Text = "A側入口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
         s_Text2 = s_Text2 + "      ****.**";
      }
      i_RecNo = 855;    // A側出口温度
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),1) == 0){
         s_Text = FormatFloat("###0.00",s_Text.ToDouble());
         i_Length = s_Text.Length();
         if (i_Length == 4) {
            s_Text ="/   " + s_Text;
         } else if (i_Length == 5) {
            s_Text ="/  " + s_Text;
         } else if (i_Length == 6) {
            s_Text ="/ " + s_Text;
         } else {
            s_Text ="/" + s_Text;
         }
         s_Text2 = s_Text2 + s_Text;
      } else {
         G_ErrLog_Text = "A側出口温度     RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
         s_Text2 = s_Text2 + "/****.**";
      }
      s_Text = s_Text2;
      Excel_Cell_DataSet(13, 8, s_Text);
      G_Log_Text = "入口出口温度         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }

   //*******************
   //*** 流量
   //*******************
   // 『B154』
   if (s_HEADS_DATA[268].TrimRight() == "1") { // 国交省銘板
      i_RecNo = 856;    // B側流量
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_HEADS_DATA[846].TrimRight() == "1") { // Ｂ側スチーム
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 切り上げ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="     " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="    " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="   " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="  " + s_Text;
            } else if (i_Length == 8) {
               s_Text =" " + s_Text;
            }
            s_Text2 = "流量(kg/min:lit/min)          " + s_Text;
         } else {
            G_ErrLog_Text = "B側流量         RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
            s_Text2 = "流量(kg/min:lit/min)          ******.**";
         }
      } else if (s_HEADS_DATA[847].TrimRight() == "1") { // Ａ側スチーム
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 切り上げ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="     " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="    " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="   " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="  " + s_Text;
            } else if (i_Length == 8) {
               s_Text =" " + s_Text;
            }
            s_Text2 = "流量(lit/min:kg/min)          " + s_Text;
         } else {
            G_ErrLog_Text = "B側流量         RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
            s_Text2 = "流量(lit/min:kg/min)          ******.**";
         }
      } else {                                    // 両側液
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 切り上げ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="     " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="    " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="   " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="  " + s_Text;
            } else if (i_Length == 8) {
               s_Text =" " + s_Text;
            }
            s_Text2 = "流量(lit/min)                  " + s_Text;
         } else {
            G_ErrLog_Text = "B側流量         RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
            s_Text2 = "流量(lit/min)                   ******.**";
         }
      }
      i_RecNo = 857;    // A側流量
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      if (s_HEADS_DATA[847].TrimRight() == "1") { // Ａ側スチーム
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 切り上げ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="                   " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="                  " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="                 " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="                " + s_Text;
            } else if (i_Length == 8) {
               s_Text ="               " + s_Text;
            } else {
               s_Text ="              " + s_Text;
            }
            s_Text2 = s_Text2 + s_Text;
         } else {
            G_ErrLog_Text = "A側流量         RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
            s_Text2 = s_Text2 + "              ******.**";
         }
      } else {                                    // Ａ側液
         if (AllFloatChk(s_Text.c_str(),1) == 0){
            //2004.08.16 切り上げ
            s_Text = FormatFloat("#####0.00",ceil(s_Text.ToDouble() * 1000 / 60 * 100) / 100);
            //s_Text = FormatFloat("#####0.00",s_Text.ToDouble() * 1000 / 60);
            //*******************
            i_Length = s_Text.Length();
            if (i_Length == 4) {
               s_Text ="                   " + s_Text;
            } else if (i_Length == 5) {
               s_Text ="                  " + s_Text;
            } else if (i_Length == 6) {
               s_Text ="                 " + s_Text;
            } else if (i_Length == 7) {
               s_Text ="                " + s_Text;
            } else if (i_Length == 8) {
               s_Text ="               " + s_Text;
            } else {
               s_Text ="              " + s_Text;
            }
            s_Text2 = s_Text2 + s_Text;
         } else {
            G_ErrLog_Text = "A側流量         RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
            s_Text2 = s_Text2 + "              ******.**";
         }
      }
      s_Text = s_Text2;
      if (s_HEADS_DATA[269].TrimRight() == "11") { // 国交省銘板Ｈ１１
         Excel_Cell_DataSet(14, 8, s_Text);
      } else {
         Excel_Cell_DataSet(13, 8, s_Text);
      }
      G_Log_Text = "流量                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   ////2003.12.05 ミラー仕様注記追加
   //// 『B154』
   //i_RecNo = 1363;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(14, 8, s_Text);
   //G_Log_Text = "注記(ﾐﾗｰ仕様)        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //*****************************
   //*************************

   //*******************
   //*** 注記(相ﾌﾗﾝｼﾞ)
   //*******************
   // 『B155』
   i_RecNo = 1329;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(15, 8, s_Text);
   G_Log_Text = "注記(相ﾌﾗﾝｼﾞ)        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2003.11.19 ラチェット注記追加
   // 『B156』
   i_RecNo = 1335;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 8, s_Text);
   G_Log_Text = "注記(ﾗﾁｪｯﾄ)          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*****************************

   //*******************
   //*** 注記(伝面１)
   //*******************
   // 『B157』
   i_RecNo = 1298;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(17, 8, s_Text);
   G_Log_Text = "注記(伝面１)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(型式１)
   //*******************
   // 『B158』
   i_RecNo = 1300;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 8, s_Text);
   G_Log_Text = "注記(型式１)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(電解研磨１)
   //*******************
   // 『B159』
   i_RecNo = 1304;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(19, 8, s_Text);
   G_Log_Text = "注記(電解研磨１)     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾌﾟﾚｰﾄ枚数１)
   //*******************
   // 『B160』
   i_RecNo = 1306;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(20, 8, s_Text);
   G_Log_Text = "注記(ﾌﾟﾚｰﾄ枚数１)    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾌﾟﾚｰﾄ材質１)
   //*******************
   // 『B161』
   i_RecNo = 1308;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(21, 8, s_Text);
   G_Log_Text = "注記(ﾌﾟﾚｰﾄ材質１)    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ｶﾞｽｹｯﾄ材質１)
   //*******************
   // 『B162』
   i_RecNo = 1310;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(22, 8, s_Text);
   G_Log_Text = "注記(ｶﾞｽｹｯﾄ材質１)   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾉｽﾞﾙ１)
   //*******************
   // 『B163』
   i_RecNo = 1325;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(23, 8, s_Text);
   G_Log_Text = "注記(ﾉｽﾞﾙ１)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾉｽﾞﾙ２)
   //*******************
   // 『B164』
   i_RecNo = 1326;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(24, 8, s_Text);
   G_Log_Text = "注記(ﾉｽﾞﾙ２)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾌﾚｰﾑ材質)
   //*******************
   // 『B165』
   i_RecNo = 1320;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(25, 8, s_Text);
   G_Log_Text = "注記(ﾌﾚｰﾑ材質)       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾎﾞﾙﾄ材質)
   //*******************
   // 『B166』
   i_RecNo = 1314;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(26, 8, s_Text);
   G_Log_Text = "注記(ﾎﾞﾙﾄ材質)       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾎﾞﾙﾄ仕上１)
   //*******************
   // 『B167』
   i_RecNo = 1315;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(27, 8, s_Text);
   G_Log_Text = "注記(ﾎﾞﾙﾄ仕上１)     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ｱﾝｶｰ材質)
   //*******************
   // 『B168』
   i_RecNo = 1322;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(28, 8, s_Text);
   G_Log_Text = "注記(ｱﾝｶｰ材質)       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ｱﾝｶｰ長)
   //*******************
   // 『B169』
   i_RecNo = 1321;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(29, 8, s_Text);
   G_Log_Text = "注記(ｱﾝｶｰ長)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

//2004.01.07 注記削除
//   //*******************
//   //*** 注記(ｱﾝｶｰ仕上)
//   //*******************
//   // 『B170』
//   i_RecNo = 1323;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   Excel_Cell_DataSet(30, 8, s_Text);
//   G_Log_Text = "注記(ｱﾝｶｰ仕上)       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
//   Write_Log(G_Log_Text);
//*******************
   //2011.03.11 工程写真要注記追加
   //*******************
   //*** 注記(工程写真要)
   //*******************
   // 『B170』
   i_RecNo = 1102;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(30, 8, s_Text);
   G_Log_Text = "注記(工程写真要)     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*****************************

   //*******************
   //*** 注記(ｶﾞｲﾄﾞﾊﾞｰ枚数１)
   //*******************
   // 『B171』
   i_RecNo = 1318;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(31, 8, s_Text);
   G_Log_Text = "注記(ｶﾞｲﾄﾞﾊﾞｰ枚数１) RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ﾎﾞﾙﾄ枚数１)
   //*******************
   // 『B172』
   i_RecNo = 1312;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(32, 8, s_Text);
   G_Log_Text = "注記(ﾎﾞﾙﾄ枚数１)     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(禁油１)
   //*******************
   // 『B173』
   i_RecNo = 1302;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(33, 8, s_Text);
   G_Log_Text = "注記(禁油１)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(接着剤)
   //*******************
   // 『B174』
   i_RecNo = 1337;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(34, 8, s_Text);
   G_Log_Text = "注記(接着剤)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(フレーム仕上)
   //*******************
   // 『B175』
   i_RecNo = 1365;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 8, s_Text);
   G_Log_Text = "注記(フレーム仕上)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(船番)
   //*******************
   // 『B176』
   i_RecNo = 1364;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   //2019.10.31 NK量産外注記_S
   G_Log_Text = "注記(船番)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』を取得。";
   Write_Log(G_Log_Text);

   //NK量産外注記
   i_RecNo = 536;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();

   G_Log_Text = "NK規格             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text2 + "』を取得。";
   Write_Log(G_Log_Text);

   if (s_Text2 == "15") {
      //ＮＫ２種
      s_Text3 = "  フレームへNK刻印を打刻する事";
   } else if (s_Text2 == "29") {
      //ＮＫ３種
      s_Text3 = "  フレームへNK刻印を打刻する事";
   } else {
      s_Text3 =  "";
   }

   G_Log_Text = "NK注記             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text3 + "』を取得。";
   Write_Log(G_Log_Text);

   s_Text = s_Text + s_Text3;
   //2019.10.31 NK量産外注記_E

   Excel_Cell_DataSet(36, 8, s_Text);
   G_Log_Text = "注記(船番)         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(圧力容器)
   //*******************
   // 『B177』
   i_RecNo = 1332;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(37, 8, s_Text);
   G_Log_Text = "注記(圧力容器)       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2017.10.06 改造時銘板注記追加_S
   //*******************
   //*** 注記(改造時銘板)
   //*******************
   // 『B179』
   i_RecNo = 1057;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      s_Text = "銘板は裏面両面テープ（社内取付時は不要）、リベット付";
      Excel_Cell_DataSet(38, 8, s_Text);
      G_Log_Text = "注記(改造時銘板)     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //2017.10.06 改造時銘板注記追加_E

   //*******************
   //*** 注記(禁油(大))
   //*******************
   // 『B179』
   i_RecNo = 1338;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.10.06 改造時銘板注記追加_S
   //Excel_Cell_DataSet(38, 8, s_Text);
   Excel_Cell_DataSet(38, 5, s_Text);
   //2017.10.06 改造時銘板注記追加_E
   G_Log_Text = "注記(禁油(大))       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(圧力容器)
   //*******************
   // 『B181』
   i_RecNo = 1339;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2017.10.06 改造時銘板注記追加_S
   //Excel_Cell_DataSet(40, 8, s_Text);
   Excel_Cell_DataSet(40, 5, s_Text);
   //2017.10.06 改造時銘板注記追加_E
   G_Log_Text = "注記(管理作業(大))   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ノズル３)
   //*******************
   // 『B183』
   i_RecNo = 1327;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(42, 8, s_Text);
   G_Log_Text = "注記(管理作業(大))   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //*** 注記(ノズル４)
   //*******************
   // 『B184』
   i_RecNo = 1328;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(43, 8, s_Text);
   G_Log_Text = "注記(管理作業(大))   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //-----------------------------
   // 2007.10.10 ﾐﾗｰ仕様対応

   if(!pcHeadsData->GetMirrorFlag()){
        // ﾐﾗｰ仕様でない場合
        //2006.11.22 機器名称注記追加
        i_Pos = s_HEADS_DATA[36].Pos("M");
        if (i_Pos != 0){
            // 舶用

            //*******************
            //*** 機器名称１
            //*******************
            // 『B185』
            i_RecNo = 26;
            s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
            s_Text = "機器名称：" + s_Text;
            Excel_Cell_DataSet(44, 8, s_Text);
            G_Log_Text = "機器名称１           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            //*******************
            //*** 機器名称２
            //*******************
            // 『B186』

        }
   //***************************
   }else{
        //2008.01.08 ﾐﾗｰ仕様(舶用以外)修正
        //// 機器名称(元仕様)
        //i_RecNo = 26;
        //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
        //
        //// ミラーA仕様、ミラーB仕様の機器名称を作成
        //s_TextA = GetEquipmentName(s_Text, "A");
        //s_TextB = GetEquipmentName(s_Text, "B");
        //
        ////*******************
        ////*** 機器名称１
        ////*******************
        //Excel_Cell_DataSet(44, 2, s_TextA);
        //G_Log_Text = "機器名称１           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_TextA + "』をセット。";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "←A号機機器名称";
        //Excel_Cell_DataSet(44, 5, s_Text);
        //G_Log_Text = "機器名称１（図番）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "・左記機器名称を銘板へ刻印の事";
        //Excel_Cell_DataSet(44, 8, s_Text);
        //G_Log_Text = "機器名称１（備考）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
        //Write_Log(G_Log_Text);
        //
        ////*******************
        ////*** 機器名称２
        ////*******************
        //Excel_Cell_DataSet(45, 2, s_TextB);
        //G_Log_Text = "機器名称２           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_TextB + "』をセット。";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "←B号機機器名称";
        //Excel_Cell_DataSet(45, 5, s_Text);
        //G_Log_Text = "機器名称２（図番）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
        //Write_Log(G_Log_Text);
        //
        //s_Text = "・B号機の銘板刻印項目はA号機の左右勝手違いのこと";
        //Excel_Cell_DataSet(45, 8, s_Text);
        //G_Log_Text = "機器名称２（備考）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
        //Write_Log(G_Log_Text);
        i_Pos = s_HEADS_DATA[36].Pos("M");
        if (i_Pos != 0){
            // 舶用
            // 機器名称(元仕様)
            i_RecNo = 26;
            s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

            // ミラーA仕様、ミラーB仕様の機器名称を作成
            s_TextA = GetEquipmentName(s_Text, "A");
            s_TextB = GetEquipmentName(s_Text, "B");

            //*******************
            //*** 機器名称１
            //*******************
            Excel_Cell_DataSet(44, 2, s_TextA);
            G_Log_Text = "機器名称１           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_TextA + "』をセット。";
            Write_Log(G_Log_Text);

            s_Text = "←A号機機器名称";
            Excel_Cell_DataSet(44, 5, s_Text);
            G_Log_Text = "機器名称１（図番）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            s_Text = "・左記機器名称を銘板へ刻印の事";
            Excel_Cell_DataSet(44, 8, s_Text);
            G_Log_Text = "機器名称１（備考）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            //*******************
            //*** 機器名称２
            //*******************
            Excel_Cell_DataSet(45, 2, s_TextB);
            G_Log_Text = "機器名称２           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_TextB + "』をセット。";
            Write_Log(G_Log_Text);

            s_Text = "←B号機機器名称";
            Excel_Cell_DataSet(45, 5, s_Text);
            G_Log_Text = "機器名称２（図番）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);

            s_Text = "・B号機の銘板刻印項目はA号機の左右勝手違いのこと";
            Excel_Cell_DataSet(45, 8, s_Text);
            G_Log_Text = "機器名称２（備考）   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);
        } else {
            // 舶用以外

            //*******************
            //*** 刻印項目注記
            //*******************

            s_Text = "・B号機の銘板刻印項目はA号機の左右勝手違いのこと";
            Excel_Cell_DataSet(45, 8, s_Text);
            G_Log_Text = "刻印項目注記         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);
        }
        //********************************
   }

   // 2007.10.10
   //-----------------------------

   //*******************
   //***  立会検査
   //*******************
   // 『C001』
   i_RecNo = 431;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0") {
      s_Text = "無";
   } else {
      s_Text = "有" ;
   }
   Excel_Cell_DataSet(48, 3, s_Text);
   G_Log_Text = "立会検査            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  検査報告書
   //*******************
   // 『C002』
   i_RecNo = 441;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text == "0") {
      s_Text = "不要";
   } else {
      s_Text = "要" ;
   }
   Excel_Cell_DataSet(49, 3, s_Text);
   G_Log_Text = "検査報告書          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  ﾐﾙｼｰﾄ
   //*******************
   // 『C003』
   if (s_HEADS_DATA[433] != "0" || s_HEADS_DATA[434] != "0" || s_HEADS_DATA[435] != "0"
          || s_HEADS_DATA[436] != "0" || s_HEADS_DATA[437] != "0"){

      s_Text = "要";
   } else {
      s_Text = "不要";
   }
   Excel_Cell_DataSet(50, 3, s_Text);
   G_Log_Text = "ﾐﾙｼｰﾄ               『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  ﾌﾟﾚｰﾄ
   //*******************
   // 『C004』
   i_RecNo = 433;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "ﾌﾟﾚｰﾄ" ;
   }
   Excel_Cell_DataSet(51, 3, s_Text);
   G_Log_Text = "ﾌﾟﾚｰﾄ               『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  ﾌﾚｰﾑ
   //*******************
   // 『C005』
   i_RecNo = 434;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "ﾌﾚｰﾑ" ;
   }
   Excel_Cell_DataSet(52, 3, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ                『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  締付ﾎﾞﾙﾄﾅｯﾄ
   //*********************
   // 『C006』
   i_RecNo = 435;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "締付ﾎﾞﾙﾄﾅｯﾄ" ;
   }
   Excel_Cell_DataSet(53, 3, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄﾅｯﾄ         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)
   //*********************
   // 『C007』
   i_RecNo = 436;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)" ;
   }
   Excel_Cell_DataSet(51, 5, s_Text);
   G_Log_Text = "ﾉｽﾞﾙ(ﾊﾟｲﾌﾟ)         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  ﾉｽﾞﾙ一式
   //*********************
   // 『C008』
   i_RecNo = 437;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = " ";
   } else {
      s_Text = "ﾉｽﾞﾙ一式" ;
   }
   Excel_Cell_DataSet(52, 5, s_Text);
   G_Log_Text = "ﾉｽﾞﾙ一式            『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  予備品
   //*********************
   // 『C009』
   i_RecNo = 1265;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "0"){
      s_Text = "無";
   } else {
      s_Text = "有" ;
   }
   Excel_Cell_DataSet(54, 3, s_Text);
   G_Log_Text = "予備品              『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   }

   //*********************
   //***  納  期
   //*********************
   // 『C010』
   //i_RecNo = 12;
   i_RecNo = 411;
   s_Text = s_HEADS_DATA[i_RecNo];
   if(s_Text.Trim() != ""){
      s_Text = s_Text.SubString(1,4) + "年" +
               s_Text.SubString(6,2) + "月" +
               s_Text.SubString(9,2) + "日";
   } else {
      s_Text = "";
   }
   Excel_Cell_DataSet(46, 9, s_Text);
   G_Log_Text = "納期                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //--------------------------
   //2007.10.09 ﾐﾗｰ仕様対応

   //2019/05/17 応用技術追加
   if (!bIsNotKatashiki) {
   if(pcHeadsData->GetMirrorFlag()){
        //*********************
        //***  銘板刻印項目
        //*********************
        s_Text = "銘　　　板　　　刻　　　印　　　項　　　目(A号機)";
        Excel_Cell_DataSet(49, 7, s_Text);
        G_Log_Text = "納期刻印項目         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
   }
   //2007.10.09
   //--------------------------

   //*********************
   //***  装置番号
   //*********************
   // 『C011』
   i_RecNo = 28;
   s_Text = "装置番号  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   Excel_Cell_DataSet(50, 7, s_Text);
   G_Log_Text = "装置番号             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //***  機器番号
   //*********************
   // 『C012』
   i_RecNo = 27;
   //2004.10.01
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   int w_count = s_Text.AnsiPos("〜");

   if (w_count){
      s_Text = "機器番号  " + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "〜" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
   }else{
      s_Text = "機器番号  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   }

   // 2022.04.04 機器番号機器名称特記追加_S
   if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
      // 機器番号特記項目有
      // 機器番号欄外注記に記述
      s_Text = "※を参照";
   }
   // 2022.04.04 機器番号機器名称特記追加_E

   Excel_Cell_DataSet(50, 11, s_Text);
   G_Log_Text = "機器番号             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

//   //*********************
//   //***  装置番号/機器番号
//   //*********************
//   // 『C012』
//    s_Text = "装置番号 " + s_HEADS_DATA[28].TrimRight() + spc(20-s_HEADS_DATA[28].TrimRight().Length())
//           + "　機器番号 " + s_HEADS_DATA[27].TrimRight();
//   Excel_Cell_DataSet(50, 7, s_Text);
//   G_Log_Text = "機器番号             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
//   Write_Log(G_Log_Text);

   //*********************
   //***  B側設計圧力
   //*********************
   // 『C013』
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   //2017.06.09 負圧追加
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "大気開放";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
      Excel_Cell_DataSet(52, 9, s_Text);
      G_Log_Text = "B側設計圧力          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "B側設計圧力            不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //*********************
   //***  B側設計温度
   //*********************
   // 『C014』
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(53, 9, s_Text);
      G_Log_Text = "B側設計温度      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "B側設計温度        不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  B側ﾃｽﾄ圧力 水圧
   //************************
   // 『C015』
   i_RecNo = 827;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(54, 9, s_Text);
      G_Log_Text = "B側ﾃｽﾄ圧力 水圧      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "B側ﾃｽﾄ圧力 水圧        不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  B側ﾃｽﾄ圧力 空圧
   //************************
   // 『C016』
   i_RecNo = 826;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(55, 9, s_Text);
      G_Log_Text = "B側ﾃｽﾄ圧力 空圧      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "B側ﾃｽﾄ圧力 空圧        不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  A側設計圧力
   //************************
   // 『C017』
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   //2017.06.09 負圧追加
   //if (AllFloatChk(s_Text.c_str(),0) == 0){
   if (AllFloatChk(s_Text.c_str(),1) == 0){
   //*******************
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "大気開放";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
      Excel_Cell_DataSet(52, 12, s_Text);
      G_Log_Text = "A側設計圧力          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   } else {
      G_ErrLog_Text = "A側設計圧力            不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  A側設計温度
   //************************
   // 『C018』
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(53, 12, s_Text);
      G_Log_Text = "A側設計温度      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "A側設計温度        不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  A側ﾃｽﾄ圧力 水圧
   //************************
   // 『C019』
   i_RecNo = 832;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(54, 12, s_Text);
      G_Log_Text = "A側ﾃｽﾄ圧力 水圧      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "A側ﾃｽﾄ圧力 水圧        不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  A側ﾃｽﾄ圧力 空圧
   //************************
   // 『C020』
   i_RecNo = 831;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (StrToFloat(s_Text) == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(55, 12, s_Text);
      G_Log_Text = "A側ﾃｽﾄ圧力 空圧      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "A側ﾃｽﾄ圧力 空圧        不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  B側内容量
   //************************
   // 『C021』
   i_RecNo = 1282;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(56, 9, s_Text);
   G_Log_Text = "B側内容量            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  伝熱面積
   //************************
   // 『C022』
   i_RecNo = 1067;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(57, 9, s_Text);
   G_Log_Text = "伝熱面積             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  A側内容量
   //************************
   // 『C023』
   i_RecNo = 1283;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(56, 12, s_Text);
   G_Log_Text = "A側内容量            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  機器質量
   //************************
   // 『C024』
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(57, 12, s_Text);
   G_Log_Text = "機器質量             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  適用規格
   //************************
   // 『C025』
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "適用規格             RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』により、";
   if (s_HEADS_DATA[1].TrimRight() == "0") {    //和文
      s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,false, s_HEADS_DATA[1].TrimRight());
      // 2011.05.30 常熟銘板
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if ( s_Text2.ToIntDef(0) == 0 ) {
         // 2012.01.20 常熟フラグ
         //if ( s_HEADS_DATA[5].TrimRight() == "42842" ) {
         //   //常熟向け(42842)(和文)銘板
         //   s_Text = "常熟銘板(和文)";
         //} else if ( s_HEADS_DATA[5].TrimRight() == "43935" ) {
         //   //常熟向け(43935)(和文)銘板
         //   s_Text = "常熟銘板(和文)";
         //} else if ( s_HEADS_DATA[401].TrimRight() == "42842" ) {
         //   //常熟向け(42842)(和文)銘板
         //   s_Text = "常熟銘板(和文)";
         //} else if ( s_HEADS_DATA[401].TrimRight() == "43935" ) {
         //   //常熟向け(43935)(和文)銘板
         //   s_Text = "常熟銘板(和文)";
         //}
         if ( s_HEADS_DATA[1107].TrimRight() == "CHINA" ) {
            //常熟向け(中文)銘板
            s_Text = "常熟銘板(中文)";
         }
         // *********************
      }
      // *******************
   } else {
      s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),false,false, s_HEADS_DATA[1].TrimRight());
      // 2012.01.20 常熟フラグ
      s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
      if ( s_Text2.ToIntDef(0) == 0 ) {
         if ( s_HEADS_DATA[1107].TrimRight() == "CHINA" ) {
            //常熟向け(中文)銘板
            s_Text = "常熟銘板(中文)";
         }
      }
      // *******************
   }
   i_RecNo = 1372;
   s_Text = s_Text + "  " + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(58, 9, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  締付寸法 MAX
   //************************
   // 『C026』
   i_RecNo = 1089;
   s_Text = "MAX " + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(60, 9, s_Text);
   G_Log_Text = "締付寸法 MAX         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  締付寸法 MIN
   //************************
   // 『C027』
   i_RecNo = 1015;
   s_Text = "MIN " + s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(60, 12, s_Text);
   G_Log_Text = "締付寸法 MIN         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //************************
   //***  訂正情報
   //************************
   i_RecNo = 319;
   if (s_HEADS_DATA[i_RecNo].TrimRight().ToIntDef(0) != 0){
      //************************
      //***  訂  正
      //************************
      // 『C050』
      i_RecNo = 319;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(59, 1, s_Text);
      G_Log_Text = "訂正                 RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //************************
      //***  設定変更
      //************************
      // 『C051』
      s_Text = "設計変更";
      Excel_Cell_DataSet(59, 2, s_Text);
      G_Log_Text = "設定変更            『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //************************
      //***  年月日
      //************************
      // 『C052』
      i_RecNo = 1087;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(59, 4, s_Text);
      G_Log_Text = "年月日               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);

      //************************
      //***  担当者
      //************************
      // 『C053』
      i_RecNo = 1079;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(59, 6, s_Text);
      G_Log_Text = "担当者               RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   }
   }
    //************************
    //***  営業社員番号
    //************************
    // 『C054』
    i_RecNo = 2;
    s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
    Excel_Cell_DataSet(61, 1, s_Text);
    G_Log_Text = "営業社員番号         RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
    Write_Log(G_Log_Text);

    //************************
    //***  営業社員名
    //************************
    // 『C055』
    i_RecNo = 3;
    s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
    Excel_Cell_DataSet(61, 3, s_Text);
    G_Log_Text = "営業社員名           RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
    Write_Log(G_Log_Text);

    //***************************
    //***  HEADS VER
    //***************************
    // 『C056』
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
    Excel_Cell_DataSet(61, 14, s_Text);
    G_Log_Text = "HEADS VER           『" + s_Text + "』をセット。";
    Write_Log(G_Log_Text);

    //2019/05/17 応用技術追加
    if (!bIsNotKatashiki) {
    //--------------------------------
    // 2007.10.09 ﾐﾗｰ仕様対応
    // 2007.10.10 行=36に変更
    //***************************
    //***  ﾐﾗｰ仕様
    //***************************
    if(pcHeadsData->GetMirrorFlag()){
        i_RecNo = 1363;
        s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
        //Excel_Cell_DataSet(34, 2, s_Text);  // "ミラー仕様"
        Excel_Cell_DataSet(36, 2, s_Text);  // "ミラー仕様"
        G_Log_Text = "ﾐﾗｰ仕様             『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2007.10.09
    // 2007.10.10
    //--------------------------------
    }

    // 2020.05.28 温水乾燥注記追加_S
    //*************************************
    //***  温風乾燥指示１(欄外(1.16))
    //*************************************
    s_Text = "";
    s_Text5 = "";

    s_Text = Get_HotAirDrying();
    i_Length = s_Text.Length();
    if (i_Length < 21 ) {
        s_Text = "";
    }

    if ( s_Text != "" ) {
        if(pcHeadsData->GetMirrorFlag()){
           s_Text = "A号機:" + s_Text;
        }
        s_Text5 = s_Text + "  ※チェックシート台数分あり";
        Excel_Cell_DataSet(1, 16, s_Text5);
        G_Log_Text = "温風乾燥指示１      『" + s_Text5 + "』をセット。";
        Write_Log(G_Log_Text);
    }

    //*************************************
    //***  温風乾燥指示２(欄外(2.16))
    //*************************************
    if(pcHeadsData->GetMirrorFlag()){
        s_Text = "";
        s_Text2 = "";
        s_Text3 = "";
        s_Text4 = "";
        s_Text5 = "";

        s_Text = Get_HotAirDrying();
        i_Length = s_Text.Length();
        if (i_Length < 21 ) {
            s_Text = "";
        }

        if ( s_Text != "" ) {
            //前部分
            s_Text2 = "B号機:" + s_Text.SubString(1,21);
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
            if ( s_Text.Pos("A(1)") > 0 ) {
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
            // B号機注記： 前部分 + A時間 + ")/A(" + B時間 + ")★"
            s_Text = s_Text2 + s_Text4 + ")/A(" + s_Text3 + ")★";
            s_Text5 = s_Text + "  ※チェックシート台数分あり";

            Excel_Cell_DataSet(2, 16, s_Text5);
        G_Log_Text = "温風乾燥指示２      『" + s_Text5 + "』をセット。";
        Write_Log(G_Log_Text);
        }
    }
    // 2020.05.28 温水乾燥注記追加_E

    // 2020.07.31 注記削除_S
    //// 2020.05.28 SX-41舶用注記追加_S
    ////*************************************
    ////***  SX-41舶用注記(欄外(3.16))
    ////*************************************
    //if ( G_KS_Syurui == "ＳＸ" && G_KS_Model == "４１" ){
    //    //SX-41
    //    if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 99 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 396 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 397 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ||
    //         s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 399 ) {
    //        // NPM,NHPM,NUPM,NSPM
    //        s_Text = "ﾎﾞﾙﾄ材質はSNB7/S45C､締付寸法は(2.4+t)xNのことTS2019-146";
    //        Excel_Cell_DataSet(3, 16, s_Text);
    //        G_Log_Text = "SX-41舶用注記       『" + s_Text + "』をセット。";
    //        Write_Log(G_Log_Text);
    //    }
    //}
    //// 2020.05.28 SX-41舶用注記追加_E
    // 2020.07.31 注記削除_E
    // 2020.07.31 保護カバー注記追加_S
    //*************************************
    //***  保護カバー舶用注記(欄外(3.16))
    //*************************************
    if (Tokki_Code("1E", s_HEADS_DATA, 1500) == true) {
        if(s_HEADS_DATA[36].Pos("M") == 0)  {
             // 舶用でない場合
             s_Text = "保護カバー要";
        } else {
             // 舶用の場合
             s_Text = "建設用保護カバー要";
        }
        Excel_Cell_DataSet(3, 16, s_Text);
        G_Log_Text = "保護カバー注記  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2020.07.31 保護カバー注記追加_E

    // 2020.05.12 Eノズル付きオーダーの警告追加_S
    //*************************************
    //***  Eﾉｽﾞﾙ付きｵｰﾀﾞの警告(欄外(4.16))
    //*************************************
    // RX10NP/NHP：99〜106枚、143〜150枚、210枚
    // RX10NUP/NSP：97〜104枚、141〜148枚、208〜210枚
    // LX10：56〜62枚、87〜92枚、133〜140枚
    // 2022.01.26 注記修正_S
                     s_Text = "";
    // 2022.01.26 注記修正_E
    if ( G_KS_Syurui == "ＲＸ" && G_KS_Model == "１１" ){
            //RX-11
        if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 92 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 93 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 392 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 393 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 396 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 397 ) {
            // NP,NHP,NPM,NHPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 99) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 106)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 143) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 150)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if (s_HEADS_DATA[170].ToIntDef(0) == 210) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else {
                s_Text = "";
            }
        } else if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 94 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 95 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 99 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 394 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 395 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 399 ) {
            // NUP,NSP,NUPM,NSPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 97) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 104)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 141) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 148)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 208) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 210)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else {
                s_Text = "";
            }
        }
    } else if ( G_KS_Syurui == "ＲＸ" && G_KS_Model == "１３" ){
        //RX-13
        if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 92 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 93 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 392 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 393 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 396 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 397 ) {
            // NP,NHP,NPM,NHPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 99) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 106)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 143) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 150)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if (s_HEADS_DATA[170].ToIntDef(0) == 210) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else {
                s_Text = "";
            }
        } else if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 94 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 95 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 99 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 394 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 395 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ||
                    s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 399 ) {
            // NUP,NSP,NUPM,NSPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 97) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 104)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 141) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 148)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 208) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 210)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else {
                 s_Text = "";
            }
        }
    } else if ( G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０" ){
        //LX-10
        if ( s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 92 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 93 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 94 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 96 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 97 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 98 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 392 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 393 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 394 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 936 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 937 ||
             s_HEADS_DATA[37].TrimRight().ToIntDef(0) == 398 ) {
            // NP,NHP,NUP,NPM,NHPM,NUPM
            if ((s_HEADS_DATA[170].ToIntDef(0) >= 56) &&
                (s_HEADS_DATA[170].ToIntDef(0) <= 62)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 87) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 92)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else if ((s_HEADS_DATA[170].ToIntDef(0) >= 133) &&
                       (s_HEADS_DATA[170].ToIntDef(0) <= 140)) {
                s_Text = "新旧ガイドバー長さ異なるためCP寸法確認の事（設計へ連絡の事）";
            } else {
               s_Text = "";
            }
        }
    } else {
        s_Text = "";
    }

    // Ｅノズルがない場合注記は出さない
    if ( s_HEADS_DATA[951].ToIntDef(0) == 0 &&
         s_HEADS_DATA[966].ToIntDef(0) == 0 &&
         s_HEADS_DATA[981].ToIntDef(0) == 0 &&
         s_HEADS_DATA[996].ToIntDef(0) == 0 ){
        s_Text = "";
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(4, 16, s_Text);
        G_Log_Text = "Eﾉｽﾞﾙ付きｵｰﾀﾞｰ警告  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2020.05.12 Eノズル付きオーダーの警告追加_E

    // 2020.10.30 圧力容器UNS規格警告追加_S
    if ( s_HEADS_DATA[29].ToIntDef(0) == 1 ||
         s_HEADS_DATA[29].ToIntDef(0) == 2 ||
         s_HEADS_DATA[29].ToIntDef(0) == 3 ){
        // 圧力容器（規格ｺｰﾄﾞ）
        if ( s_HEADS_DATA[43].TrimRight() == "3472" ) {
            // ＵＮＳ　Ｎ１０２７６
            s_Text = "ＪＩＳ規格材質併記要";
        } else {
            s_Text = "";
        }
    }  else {
        s_Text = "";
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(5, 16, s_Text);
        G_Log_Text = "圧力容器UNS規格警告  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2020.10.30 圧力容器UNS規格警告追加_E

    // 2021.04.15 注記追加_S
    //*************************************
    //***  完全分解発送(欄外(5.16))
    //*************************************
    s_Text = "";

    i_RecNo = 1275;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "3"){
        // 完全分解発送
        s_Text = "完全分解発送";
        Excel_Cell_DataSet(5, 16, s_Text);
        G_Log_Text = "完全分解発送    『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }

    //***********************************************
    //***  SILVER(HEAT RESISTACE PAINT�@(欄外(6.16))
    //***********************************************
    s_Text = "";

    i_RecNo = 31;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "B"){
        // SILVER(HEAT RESISTACE PAINT
        s_Text = "再ショットブラスト要";
        Excel_Cell_DataSet(6, 16, s_Text);
        G_Log_Text = "SILVER(HEAT RESISTACE PAINT�@  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }

    //***********************************************
    //***  SILVER(HEAT RESISTACE PAINT�A(欄外(7.17))
    //***********************************************
    s_Text = "";

    i_RecNo = 31;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "B"){
        // SILVER(HEAT RESISTACE PAINT
        s_Text = "注）塗料は耐熱塗料の事、塗装色はシルバー";
        Excel_Cell_DataSet(7, 16, s_Text);
        G_Log_Text = "SILVER(HEAT RESISTACE PAINT�@  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }

    //*************************************
    //***  標準外塗料(欄外(8.16))
    //*************************************
    s_Text = "";

    // 2021.05.19 注記追加_S
    //if (Tokki_Code("1T", s_HEADS_DATA, 1500) == true) {
    if (Tokki_Code("1X", s_HEADS_DATA, 1500) == true) {
    // 2021.05.19 注記追加_E
        // 標準外塗料
        s_Text = "標準外塗料使用／工程注意のこと";
        Excel_Cell_DataSet(8, 16, s_Text);
        G_Log_Text = "標準外塗料注記  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2021.04.15 注記追加_E

    // 2021.05.17 注記追加_S
    //*************************************
    //***  ＷＸ注記(欄外(9.16)、(10.16))
    //*************************************
    s_Text = "";
    s_Text2 = "";
    s_Text3 = "";
    s_Text4 = "";
    s_Text5 = "";

    if ( G_KS_Syurui == "ＷＸ" ) {

        i_RecNo = 835;   // B側ｶﾞｽｹｯﾄ材質
        s_Text3 = s_HEADS_DATA[i_RecNo].TrimRight();
        s_Text3 = Type_MAT(s_Text3);

        i_RecNo = 836;   // A側ｶﾞｽｹｯﾄ材質
        s_Text4 = s_HEADS_DATA[i_RecNo].TrimRight();
        s_Text4 = Type_MAT(s_Text4);

        if ( s_Text3 == "TCG" || s_Text4 == "TCG" ) {
            s_Text5 = "TCG";
        }

        if ( G_KS_Model == "５３" && s_Text5 == "TCG" ) {
            //WX-53 TCG
            s_Text = "溶接プレート作業工程フローEX2005-046(最新版)出図要";
            s_Text2 = "溶接検査要領書EX2020-056(最新版)を添付のこと";
        } else if (G_KS_Model == "９０" && s_Text5 == "TCG" ) {
            //WX-90 TCG
            s_Text = "溶接プレート作業工程フローEX2005-046(最新版)出図要";
            s_Text2 = "溶接検査要領書EX2020-056(最新版)を添付のこと";
        } else {
            //WX
            s_Text = "溶接プレート作業工程フローEX2005-046(最新版)出図要";
            s_Text2 = "";
        }

        Excel_Cell_DataSet(9, 16, s_Text);
        G_Log_Text = "ＷＸ注記１  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);

        Excel_Cell_DataSet(10, 16, s_Text2);
        G_Log_Text = "ＷＸ注記２  『" + s_Text2 + "』をセット。";
        Write_Log(G_Log_Text);

    }
    // 2021.05.17 注記追加_E

    // 2021.10.19 注記追加_S
    //*************************************
    //***  ダイトク注記(欄外(11.16))
    //*************************************
    //--- 御注文主コード
    i_RecNo = 1064;
    if (s_HEADS_DATA[i_RecNo].TrimRight() == "04633"){
        // ダイトク向け塗装手塗り指示
        s_Text = "注）塗装は手塗りのこと。TS2021-041による";
        Excel_Cell_DataSet(11, 16, s_Text);
        G_Log_Text = "ダイトク向け塗装手塗り指示     『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2021.10.19 注記追加_E

    // 2022.01.26 ｽﾍﾟｰｻ注記追加_E
    //****************************************************
    //***  スペーサ注記(欄外(12.16)、(13.16) 、(14.16))
    //****************************************************
    s_Text = "";
    s_Text2 = "";
    s_Text3 = "";
    s_Text4 = "";
    s_Text5 = "";

    i_RecNo = 835;   // B側ｶﾞｽｹｯﾄ材質
    s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
    i_RecNo = 836;   // A側ｶﾞｽｹｯﾄ材質
    s_Text3 = s_HEADS_DATA[i_RecNo].TrimRight();
    s_S1Nzl_Syurui = s_HEADS_DATA[891].TrimRight(); // S1ﾉｽﾞﾙ種類
    s_S2Nzl_Syurui = s_HEADS_DATA[906].TrimRight(); // S2ﾉｽﾞﾙ種類
    s_S3Nzl_Syurui = s_HEADS_DATA[921].TrimRight(); // S3ﾉｽﾞﾙ種類
    s_S4Nzl_Syurui = s_HEADS_DATA[936].TrimRight(); // S4ﾉｽﾞﾙ種類
    s_E1Nzl_Syurui = s_HEADS_DATA[951].TrimRight(); // E1ﾉｽﾞﾙ種類
    s_E2Nzl_Syurui = s_HEADS_DATA[966].TrimRight(); // E2ﾉｽﾞﾙ種類
    s_E3Nzl_Syurui = s_HEADS_DATA[981].TrimRight(); // E3ﾉｽﾞﾙ種類
    s_E4Nzl_Syurui = s_HEADS_DATA[996].TrimRight(); // E4ﾉｽﾞﾙ種類

    if ( G_KS_Syurui == "ＵＸ" && G_KS_Model == "９０" ){
        // UX-90
        if ( s_Text2 == "2067" || s_Text3 == "2067" ) {
            // GZ-NBR
            s_Text4 = "ＧＳＫ材質が水添加ＮＢＲ";
        }
       s_Text5 = "別図出図(E366044-1)要。組立図(部品、重量、締付寸法)は対応済";
    }
    else if ( G_KS_Syurui == "ＬＸ" && G_KS_Model == "１０" ){
         // LX-10
         if ( s_Text2 == "2063" || s_Text3 == "2063" ||
              s_Text2 == "2064" || s_Text3 == "2064" ) {
            // TCG
            s_Text4 = "ＧＳＫ材質がＴＣＧ";
        }
        s_Text5 = "別図出図(E348550-1)要。組立図(部品、重量、締付寸法)は対応済";
    }
    else if ( G_KS_Syurui == "ＷＨ" && G_KS_Model == "３０" ){
        // WH-30
        s_Text4 = "型式がＷＨ−３０";
        s_Text5 = "別図出図(E383984-2)要。組立図(部品、重量、締付寸法)は対応済";
    }

    if(s_Text4 != ""){

        s_Text = s_Text4 + "のため、孔開きのフレームとプレートの間にスペーサ要。";

        Excel_Cell_DataSet(12, 16, s_Text);
        G_Log_Text = "スペーサ注記１  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);

        Excel_Cell_DataSet(13, 16, s_Text5);
        G_Log_Text = "スペーサ注記２  『" + s_Text5 + "』をセット。";
        Write_Log(G_Log_Text);

        // 2022.04.12 特孔判定修正_S
        //if( s_S1Nzl_Syurui == "0" || s_S2Nzl_Syurui == "0" || s_S3Nzl_Syurui == "0" || s_S4Nzl_Syurui == "0" ) {
        //    // S側特孔
        //    s_Text = s_Text4 + "＆特孔仕様のため、スペーサ特孔対応要。";
        //}
        //else if ( s_E1Nzl_Syurui == "0" && s_E2Nzl_Syurui == "0" && s_E3Nzl_Syurui == "0" && s_E4Nzl_Syurui == "0" ) {
        //    // E側孔無
        //    s_Text = "";
        //}
        //else if ( s_E1Nzl_Syurui != "0" && s_E2Nzl_Syurui != "0" && s_E3Nzl_Syurui != "0" && s_E4Nzl_Syurui != "0" ) {
        //    // E側四孔
        //    s_Text = "";
        //}
        //else {
        //    // E側特孔
        //    s_Text = s_Text4 + "＆特孔仕様のため、スペーサ特孔対応要。";
        //}

        bTok_S = true;
        bTok_E = true;
        // S側
        if( s_S1Nzl_Syurui != "0" && s_S2Nzl_Syurui != "0" && s_S3Nzl_Syurui != "0" && s_S4Nzl_Syurui != "0" ) {
            // S側四孔
            bTok_S = false;
        }
        else if ( s_S1Nzl_Syurui == "0" && s_S2Nzl_Syurui == "0" && s_S3Nzl_Syurui == "0" && s_S4Nzl_Syurui == "0" ) {
            // S側孔無
            bTok_S = false;
        }
        else {
            // S側特孔
            bTok_S = true;
        }

        // E側
        if ( s_E1Nzl_Syurui != "0" && s_E2Nzl_Syurui != "0" && s_E3Nzl_Syurui != "0" && s_E4Nzl_Syurui != "0" ) {
            // E側四孔
            bTok_E = false;
        }
         else if ( s_E1Nzl_Syurui == "0" && s_E2Nzl_Syurui == "0" && s_E3Nzl_Syurui == "0" && s_E4Nzl_Syurui == "0" ) {
            // E側孔無
            bTok_E = false;
        }
        else {
            // E側特孔
            bTok_E = true;
        }

        if ( bTok_S == true || bTok_E == true) {
            // 特孔有
            s_Text = s_Text4 + "＆特孔仕様のため、スペーサ特孔対応要。";
        }
        else {
            // 特孔無
            s_Text = "";
        }
        // 2022.04.12 特孔判定修正_E

        if(s_Text != ""){
            Excel_Cell_DataSet(14, 16, s_Text);
            G_Log_Text = "スペーサ注記３  『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);
        }

    }
    // 2022.01.26 ｽﾍﾟｰｻ注記追加_E

    // 2022.01.26 注記追加_S
    //*********************************
    //***  日新興業注記(欄外(15.16))
    //*********************************
    //--- 御注文主コード
    i_RecNo = 1064;
    if ( s_HEADS_DATA[i_RecNo].TrimRight() == "06346" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "06448" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "07194" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "10374" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "10383" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "11261" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "12063" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "12403" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "12404" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18033" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18253" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18254" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "18255" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "19823" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "20676" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "24120" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "27410" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "28934" ||
         s_HEADS_DATA[i_RecNo].TrimRight() == "43573" ) {
        // 日新興業注文
        s_Text = "TS2019-146と同仕様（高温側、低温側の運転圧力が同圧）でないことを確認すること。同仕様の場合、増し締め対応のこと。";
        Excel_Cell_DataSet(15, 16, s_Text);
        G_Log_Text = "日新興業注記  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }

    //*********************************
    //***  F3接着剤注記(欄外(16.16))
    //*********************************
    s_Text = "";
    s_Text2 = "";
    s_Text3 = "";

    i_RecNo = 835;   // B側ｶﾞｽｹｯﾄ材質
    s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
    i_RecNo = 836;   // A側ｶﾞｽｹｯﾄ材質
    s_Text3 = s_HEADS_DATA[i_RecNo].TrimRight();

    if ( s_Text2 == "2003" || s_Text3 == "2003" ||
         s_Text2 == "2005" || s_Text3 == "2005" ||
         s_Text2 == "2011" || s_Text3 == "2011" ||
         s_Text2 == "2021" || s_Text3 == "2021" ||
         s_Text2 == "2038" || s_Text3 == "2038" ||
         s_Text2 == "2041" || s_Text3 == "2041" ||
         s_Text2 == "2045" || s_Text3 == "2045" ||
         s_Text2 == "2079" || s_Text3 == "2079" ||
         s_Text2 == "2083" || s_Text3 == "2083" ||
         s_Text2 == "2098" || s_Text3 == "2098" ) {
        // 食品ガスケット　　　
        s_Text = "注意：Ｆ３ガスケットはアルコールへ溶解します。";
        Excel_Cell_DataSet(16, 16, s_Text);
        G_Log_Text = "F3接着剤注記  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2022.01.26 注記追加_E

    // 2022.04.04 機器番号機器名称特記追加_S
    //*************************************
    //***  機器番号欄外注記(欄外(17.16))
    //*************************************
    s_Text = "";
    if (Tokki_Code("39", s_HEADS_DATA, 1500) == true) {
        // 機器番号特記項目有
        i_RecNo = 27;
        int w_count = s_Text.AnsiPos("〜");

        if (w_count){
            s_Text = "※機器番号  " + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "〜" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
        } else {
            s_Text = "※機器番号  " + ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
        }

        Excel_Cell_DataSet(17, 16, s_Text);
        G_Log_Text = "機器番号欄外注記  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2022.04.04 機器番号機器名称特記追加_E

    // 2022.04.11 就航船名追加_S
    //*************************************
    //***  就航船名欄外注記(欄外(18.16))
    //*************************************
    s_Text = "";
    if (Tokki_Code("29", s_HEADS_DATA, 1500) == true) {
        // SHIP NAME特記記項目有
        s_Text = Tokki_Zen("29", s_HEADS_DATA, 1500);
        s_Text = s_Text.SubString(10, s_Text.Length() - 9);
        s_Text = "就航船名:" + s_Text;

        Excel_Cell_DataSet(18, 16, s_Text);
        G_Log_Text = "就航船名欄外注記  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2022.04.11 就航船名追加追加_E

    // 2022.04.11 耐圧保持時間指定追加_S
    //**************************************
    //***  耐圧保持時間注記(欄外(19.16))
    //**************************************
    s_Text = "";
    if (Tokki_Code("2C", s_HEADS_DATA, 1500) == true) {
        // 耐圧保持時間標準
        s_Text = "耐圧保持時間標準";
    }
    else if (Tokki_Code("2D", s_HEADS_DATA, 1500) == true) {
        // 耐圧保持時間30分
        s_Text = "耐圧保持時間30分";
    }
    else if (Tokki_Code("2E", s_HEADS_DATA, 1500) == true) {
        // 耐圧保持時間指定あり
        s_Text = "耐圧保持時間指定あり";
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(19, 16, s_Text);
        G_Log_Text = "耐圧保持時間注記  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2022.04.11 耐圧保持時間指定追加_E

    // 2023.09.26 RX-30型Eﾉｽﾞﾙ付警告_S
    //**************************************
    //***  RX-30型Eﾉｽﾞﾙ付警告(欄外(20.16))
    //**************************************
    s_Text = "";

    if ( G_KS_Syurui == "ＲＸ" && G_KS_Model == "３０" ){
        // RX-30
        if ( s_E1Nzl_Syurui != "0" || s_E2Nzl_Syurui != "0" || s_E3Nzl_Syurui != "0" || s_E4Nzl_Syurui != "0" ) {
            // E側ﾉｽﾞﾙ付
            s_Text = "標準Ｅフレーム使用不可";
        }
    }

    if ( s_Text != "" ) {
        Excel_Cell_DataSet(20, 16, s_Text);
        G_Log_Text = "RX-30型Eﾉｽﾞﾙ付警告  『" + s_Text + "』をセット。";
        Write_Log(G_Log_Text);
    }
    // 2023.09.26 RX-30型Eﾉｽﾞﾙ付警告_E

    // 2018 09.18 バーコード削除_S
    //// 2017.12.01 バーコード追加_S
    ////***************************
    ////***  バーコード
    ////***************************
    //s_Text = G_SAKUBAN;
    //s_Text = "*" + s_Text + "*";
    //Excel_Cell_DataSet(62, 10, s_Text);
    //G_Log_Text = "バーコード           『" + s_Text + "』をセット。";
    //Write_Log(G_Log_Text);
    //// 2017.12.01 バーコード追加_E
    //// 2018 09.18 バーコード削除_E
    // 2023.05.30 バーコード変更_S
    s_Text = G_SAKUBAN;
    exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("バーコード"));
    Excel_Cell_DataSet(4, 5, s_Text);
    G_Log_Text = "バーコード作番         『" + s_Text + "』をセット。";
    Write_Log(G_Log_Text);
    exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("図面目録"));
    // 2023.05.30 バーコード変更_E

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
   G_Log_Text = "------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『図面目録』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 出力図面　セット
// 概  要      ：
// 引  数      ： Pattern [出力パターン]
// 戻り値      ： なし
// 備  考      ： なし
//---------------------------------------------------------------------------
void __fastcall SyuturyokuZumen_Set_S(AnsiString s_UtiKbn, bool bMarine, HeadsData* pcHeadsData)
{
   AnsiString s_Text;
   int        i_RowCnt;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   int i;
   int iLoop;
   AnsiString sMirrorCode;

   // HEADSﾃﾞｰﾀをﾛｰｶﾙ変数にｾｯﾄ
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return;

   //*******************
   //***  図面欄初期化
   //*******************
   for (i_RowCnt = 0; i_RowCnt < 32; i_RowCnt++){
      s_Text = "";
      Excel_Cell_DataSet(10 + i_RowCnt, 2, s_Text);
      Excel_Cell_DataSet(10 + i_RowCnt, 5, s_Text);
      Excel_Cell_DataSet(10 + i_RowCnt, 7, s_Text);
      Excel_Cell_DataSet(10 + i_RowCnt, 8, s_Text);
   }

   i_RowCnt = 0;

   if(!pcHeadsData->GetMirrorFlag()){
        iLoop = 1;  // 元仕様
   }else{
        iLoop = 2;  // ﾐﾗｰA、ﾐﾗｰB
   }

   for(i=0; i<iLoop; i++){

        if(pcHeadsData->GetMirrorFlag()){
            // ﾐﾗｰ仕様である
            if(i==0){
                sMirrorCode = "A";
            }else{
                sMirrorCode = "B";
            }
        }else{
            // ﾐﾗｰ仕様でない
            sMirrorCode = "";
        }

        //2019/05/17 応用技術追加
        if (!bIsNotKatashiki) {
            //***************************
            //***  エレメント構成図
            //***************************
            s_Text = "エレメント構成図";
            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

			//2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "A" + G_SAKUBAN + sMirrorCode;
			s_Text = "A" + GetFileSeizouBangou(G_SAKUBAN) + sMirrorCode;
            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

            i_RowCnt = i_RowCnt + 2;

			//*******************
            //***  組立図
            //*******************
            s_Text = "組立図";
            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

			//2023/4/21 製造番号桁数追加対応 Mod
			//s_Text = "B" + G_SAKUBAN + sMirrorCode;
			s_Text = "B" + GetFileSeizouBangou(G_SAKUBAN) + sMirrorCode;
            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

            i_RowCnt = i_RowCnt + 2;
        }
   }

   //*******************
   //***  標準部品表
   //*******************
   s_Text = "標準部品表";
   Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

   //2023/4/21 製造番号桁数追加対応 Mod
   //s_Text = "C" + G_SAKUBAN;
   s_Text = "C" + GetFileSeizouBangou(G_SAKUBAN);
   Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

   i_RowCnt = i_RowCnt + 2;

   //*******************
   //***  要目表
   //*******************
   if( bMarine == true ) {
      //2019/05/17 応用技術追加
      if (!bIsNotKatashiki) {
          s_Text = "要目表";
          Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

		  //2023/4/21 製造番号桁数追加対応 Mod
		  //s_Text = "M" + G_SAKUBAN;
		  s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
          Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

          i_RowCnt = i_RowCnt + 2;
      }
   }

   if (s_HEADS_DATA[327].TrimRight() != ""){        // 改造前作番

        for(i=0; i<iLoop; i++){

            if(pcHeadsData->GetMirrorFlag()){
                // ﾐﾗｰ仕様である
                if(i==0){
                    sMirrorCode = "A";
                }else{
                    sMirrorCode = "B";
                }
            }else{
                // ﾐﾗｰ仕様でない
                sMirrorCode = "";
            }

            //2019/05/17 応用技術追加
            if (!bIsNotKatashiki) {
                //***************************
                //***  旧エレメント構成図
                //***************************
                s_Text = "旧エレメント構成図";
                Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

                s_Text = "A" + s_HEADS_DATA[327].TrimRight() + sMirrorCode;
                Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

                i_RowCnt = i_RowCnt + 2;

                //*******************
                //***  旧組立図
                //*******************
                s_Text = "旧組立図";
                Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);

                s_Text = "B" + s_HEADS_DATA[327].TrimRight() + sMirrorCode;
                Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);

                i_RowCnt = i_RowCnt + 2;
            }
        }
    }

}

//---------------------------------------------------------------------------
// 日本語関数名： 半角空白
// 概  要      ：
// 引  数      ： 数量
// 戻り値      ： 半角空白文字列
// 備  考      ： なし
//---------------------------------------------------------------------------
AnsiString __fastcall spc(int iNum)
{
   AnsiString retSpc;
   int ilp;

   retSpc="";
   for(ilp=0; ilp<iNum; ilp++) retSpc += "　";

   return retSpc;

}

//---------------------------------------------------------------------------
// 日本語関数名： ミラーA仕様、ミラーB仕様の機器名称を取得する
// 概  要      ：
// 引  数      ： s_NameOrg AnsiString(I)   元仕様の機器名称
//                s_NameA   AnsiString(O)   ミラーA仕様の機器名称
//                s_NameB   AnsiString(O)   ミラーB仕様の機器名称
// 戻り値      ： なし
//
// 備  考      ： P_CV00.exeの部分文字列作成処理と同期を取ること
//                (Module1.bas のFnc_Write_SIYO 関数)
//
// 履　歴      ：2007.11.15 新規作成
//---------------------------------------------------------------------------
AnsiString __fastcall GetEquipmentName(AnsiString s_NameOrg, AnsiString sSpecCode)
{
   AnsiString s_Text6L; // 1〜6文字目
   AnsiString s_Text6R; // 6文字目以降
   AnsiString s_Text9L; // 1〜9文字目
   AnsiString s_Text9R; // 9文字目以降
   AnsiString s_NoHan;     // No
   AnsiString s_NoZen;     // No
   AnsiString s_RetText;

   // 元仕様の機器名称の部分文字列
   s_Text6L = LeftStr(s_NameOrg, 6);
   s_Text6R = s_NameOrg.SubString(s_Text6L.Length() + 1, s_NameOrg.Length() - s_Text6L.Length());
   s_Text9L = LeftStr(s_NameOrg, 9);
   s_Text9R = s_NameOrg.SubString(s_Text9L.Length() + 1, s_NameOrg.Length() - s_Text9L.Length());

   if( sSpecCode == "A"){
      s_NoHan = "1";
      s_NoZen = "１";
   }else if( sSpecCode == "B" ){
      s_NoHan = "2";
      s_NoZen = "２";
   }

   // 元仕様の機器名称から、ミラーA仕様、ミラーB仕様の機器名称を作成
   if( s_NameOrg.Trim().Length() == 0 ){    // 空欄
      s_RetText = "";

   } else if( s_Text6L == "No.1/2" ){
      s_RetText = "No." + s_NoHan;
      s_RetText = s_RetText + s_Text6R;

   }else if( s_Text6L == "NO.1/2" ){
      s_RetText = "NO." + s_NoHan + s_Text6R;

   }else if( s_Text9L == "No.1/No.2" ){
      s_RetText = "No." + s_NoHan + s_Text9R;

   }else if( s_Text9L == "NO.1/NO.2" ){
      s_RetText = "NO." + s_NoHan + s_Text9R;

   }else if( s_Text9L == "NO.1/No.2" ){
      s_RetText = "NO." + s_NoHan + s_Text9R;

   }else if( s_Text9L == "No.1/NO.2" ){
      s_RetText = "No." + s_NoHan + s_Text9R;

   }else if( s_Text6L == "Ｎｏ．１／２" ){
      s_RetText = "Ｎｏ．" + s_NoZen + s_Text6R;

   }else if( s_Text6L == "ＮＯ．１／２" ){
      s_RetText = "ＮＯ．" + s_NoZen + s_Text6R;

   }else if( s_Text9L == "Ｎｏ．１／Ｎｏ．２" ){
      s_RetText = "Ｎｏ．" + s_NoZen + s_Text9R;

   }else if( s_Text9L == "ＮＯ．１／ＮＯ．２" ){
      s_RetText = "ＮＯ．" + s_NoZen + s_Text9R;

   }else if( s_Text9L == "ＮＯ．１／Ｎｏ．２" ){
      s_RetText = "ＮＯ．" + s_NoZen + s_Text9R;

   }else if( s_Text9L == "Ｎｏ．１／ＮＯ．２" ){
      s_RetText = "Ｎｏ．" + s_NoZen + s_Text9R;

   }else{
      s_RetText = s_NameOrg;
   }

   return(s_RetText);
}

//---------------------------------------------------------------------------
// 日本語関数名： 出力図面　セット
// 概  要      ：
// 引  数      ： Pattern [出力パターン]
// 戻り値      ： なし
// 備  考      ： なし
//---------------------------------------------------------------------------
//void __fastcall SyuturyokuZumen_Set_S_OLD(char Pattern, AnsiString Lang)
//{
//   AnsiString s_Text;
//   int        i_RowCnt;
//
//   //*******************
//   //***  図面欄初期化
//   //*******************
//   for (i_RowCnt = 0; i_RowCnt < 32; i_RowCnt++){
//      s_Text = "";
//      Excel_Cell_DataSet(10 + i_RowCnt, 2, s_Text);
//      Excel_Cell_DataSet(10 + i_RowCnt, 5, s_Text);
//      Excel_Cell_DataSet(10 + i_RowCnt, 7, s_Text);
//      Excel_Cell_DataSet(10 + i_RowCnt, 8, s_Text);
//   }
//
//   i_RowCnt = 0;
//
//   //*******************
//   //***  要目表
//   //*******************
//   switch (Pattern) {
//      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M':
//         if (Lang == "J"){
//            s_Text = "要目表";
//         } else {
//            s_Text = "PRINCIPAL ITEMS";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "M" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  エレメント構成図
//   //***************************
//   switch (Pattern) {
//      case 'H':  case 'I': case 'J': case 'N': case 'O': case 'P':
//      case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
//         if (Lang == "J"){
//            s_Text = "エレメント構成図";
//         } else {
//            s_Text = "DWG. OF ELEMENT ARRANGEMENT";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "A" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //*******************
//   //***  組立図
//   //*******************
//   switch (Pattern) {
//      case 'A': case 'U':
//         break;
//
//      default :
//         if (Lang == "J"){
//            s_Text = "組立図";
//         } else {
//            s_Text = "ASSEMBLY DRAWING";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "B" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//   }
//
//   //*******************
//   //***  標準部品表
//   //*******************
//   switch (Pattern) {
//      case 'A': case 'U':
//         break;
//
//      default :
//         if (Lang == "J"){
//            s_Text = "標準部品表";
//         } else {
//            s_Text = "ASSEMBLY DRAWING";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "B" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//   }
//
//   //***************************
//   //***  リスト
//   //***************************
//   switch (Pattern) {
//      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M': case 'Q': case 'R': case 'S': case 'T':
//         if (Lang == "J"){
//            s_Text = "パーツリスト";
//         } else {
//            s_Text = "PARTS LIST";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "PAR" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//
//         if (Lang == "J"){
//            s_Text = "付属品リスト";
//         } else {
//            s_Text = "ACCESSORIES LIST";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "ACT" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//
//         if (Lang == "J"){
//            s_Text = "工具リスト";
//         } else {
//            s_Text = "TOOL LIST";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "TOL" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  圧力容器（液）
//   //***************************
//   switch (Pattern) {
//      case 'D': case 'F': case 'H': case 'K': case 'N': case 'Q': case 'S':
//         if (Lang == "J"){
//            s_Text = "強度計算書";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVC" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//
//            s_Text = "フローシート";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVF" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//         }
//
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  圧力容器（スチーム）
//   //***************************
//   switch (Pattern) {
//      case 'E': case 'G': case 'I': case 'L': case 'O': case 'R': case 'T':
//         if (Lang == "J"){
//            s_Text = "強度計算書";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVC" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//
//            s_Text = "フローシート";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            s_Text = "PVF" + G_SAKUBAN;
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//
//            s_Text = "スチーム送入量計算書";
//            Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//            //s_Text = "PVC" + G_SAKUBAN;
//            s_Text = "";
//            Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//            i_RowCnt = i_RowCnt + 2;
//         }
//
//         break;
//
//      default :
//         break;
//   }
//
//   //*******************
//   //***  旧要目表
//   //*******************
//   switch (Pattern) {
//      case 'K': case 'L': case 'M':
//         if (Lang == "J"){
//            s_Text = "旧要目表";
//         } else {
//            s_Text = "OLD PRINCIPAL ITEMS";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "M" + s_HEADS_DATA[327].TrimRight();
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //*******************
//   //***  旧組立図
//   //*******************
//   switch (Pattern) {
//      case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'S': case 'T':
//         if (Lang == "J"){
//            s_Text = "旧組立図";
//         } else {
//            s_Text = "OLD ASSEMBLY DRAWING";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "B" + s_HEADS_DATA[327].TrimRight();
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  旧エレメント構成図
//   //***************************
//   switch (Pattern) {
//      case 'N': case 'O': case 'P': case 'S': case 'T':
//         if (Lang == "J"){
//            s_Text = "旧エレメント構成図";
//         } else {
//            s_Text = "OLD DWG. OF ELEMENT ARRANGEMENT";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "A" + s_HEADS_DATA[327].TrimRight();
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//
//   //***************************
//   //***  検査要領書
//   //***************************
//   switch (Pattern) {
//      case 'V':
//         if (Lang == "J"){
//            s_Text = "検査要領書";
//         } else {
//            s_Text = "INSPECTION PROCEDURE";
//         }
//         Excel_Cell_DataSet(11 + i_RowCnt, 2, s_Text);
//
//         s_Text = "D" + G_SAKUBAN;
//         Excel_Cell_DataSet(11 + i_RowCnt, 5, s_Text);
//
//         //i_RowCnt = i_RowCnt + 2;
//         break;
//
//      default :
//         break;
//   }
//}

//---------------------------------------------------------------------------
// 日本語関数名： 特記コード有無確認
// 概  要      ：
// 引  数      ： AnsiString Key   ： 特記ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2020.07.17追加
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

