//---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：標準部品表(Buhinhyou)  作成モジュール
//　 作 成 者：T.Kawai
//　 作 成 日：2002.08.07
//　 更 新 日：2002.09.27
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <dir.h>
#include <io.h>
#include <direct.h>

#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Buhinhyou.h"

#include "HeadsData.h"

#define  EXCEL_SHEETNAME    "標準部品表"          // EXCELシート名(標準部品表)

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ﾌﾟﾚｰﾄ部品展開処理 Class
//---------------------------------------------------------------------------
class TBuhinhyou
{
private:
   FILE *wkfp;
//   AnsiString P_PREV_PARTS[500][16];
//   AnsiString P_WK_PARTS[500][15];
//   AnsiString P_WK_PARTS3[500][15];
   AnsiString P_PREV_PARTS[1000][18];
   AnsiString P_WK_PARTS[1000][17];
   AnsiString P_WK_PARTS3[1000][17];

   int P_PrevRec;   // 前回ﾃﾞｰﾀ数

   int P_compflg;   // 比較ﾌﾗｸﾞ(前回ﾌｧｲﾙ存在で1)
   int P_WkRec;     // 集計用ﾜｰｸﾃﾞｰﾀ格納ﾚｺｰﾄﾞ数
   int P_WkRec3;    // 集計用ﾜｰｸﾃﾞｰﾀ格納ﾚｺｰﾄﾞ数
   int P_Row;       // ﾃﾞｰﾀｾｯﾄ行数
   int P_PageNum;   // ページ数（新Ａ３横レイアウト用）
   int iBLTCV;
   //2003.11.14 変数追加
   int P_SHIP_Num;  // 舶用部品行数
   //*******************
   //2019.10.23 付属品ﾃﾞｰﾀ出力行数変更_S
   int P_ACCS_Num;  // 付属品部品行数
   //2019.10.23 付属品ﾃﾞｰﾀ出力行数変更_E

   HeadsData* m_pcHeadsData;
   AnsiString P_HEADS_DATA[1500];
   AnsiString P_HEADS_DATA_A[1500];
   AnsiString P_HEADS_DATA_B[1500];

   bool __fastcall Buhinhyou_Data_Sakusei(void);
   bool __fastcall Buhinhyou(void);              //
   //2003.11.14 関数追加
   bool __fastcall Read_Buhin_Data(void);        // 舶用部品行数読込
   //*******************

   //2019.10.23 付属品ﾃﾞｰﾀ出力行数変更_S
   bool __fastcall Read_ACCS_Data(void);         // 付属品行数読込
   //2019.10.23 付属品ﾃﾞｰﾀ出力行数変更_E

   bool __fastcall Write_Buhin_Detail(void);     //
   bool __fastcall Compare_Data( AnsiString* );

   bool __fastcall Prev_Data_Get(void);
   //2023.02.08 製造番号桁数追加_S
   bool __fastcall Prev_Old_Data_Get(void);
   //2023.02.08 製造番号桁数追加_E
   void __fastcall Output_WK_PARTS(bool);
   void __fastcall Output_WK_PARTS3(void);
   void __fastcall Buhin_Syukei( AnsiString* );
   void __fastcall Buhin_Syukei2( AnsiString* );
   void __fastcall Buhin_Syukei3( AnsiString* );
   //2003.11.14 関数追加
   void __fastcall Buhin_Syukei4( AnsiString* ); // ノズル用
   //*******************
   void __fastcall Cyuki_Out( void );
   void __fastcall BLT_Cover_Out( void );
   void __fastcall BLT_Cover_Out_Small( void );
   AnsiString __fastcall Csv_Exist_Check(AnsiString);
   bool __fastcall Compare_PrevData( AnsiString *P_PARTS );
   bool __fastcall Compare_PrevData2( AnsiString *P_PARTS );
   void __fastcall Write_BorderLine(void);

   // 2020.07.17 保護カバー注記追加_S
   bool __fastcall Tokki_Code(AnsiString);
   // 2020.07.17 保護カバー注記追加_E

   //--------------------------------
   // 2007.10.12 ﾐﾗｰ仕様対応 関数追加
   bool __fastcall Read_Buhin_Data_Sorted(AnsiString filepass, AnsiString s_DATA[][17]);
   bool __fastcall Sort_HEADS_Data(AnsiString s_DATA[][17], AnsiString s_SortDATA[][17]);
   // 2007.11.08
   bool __fastcall TBuhinhyou::Sort_HEADS_PlData(AnsiString s_wkTmpPlData[][18], AnsiString s_wkTmpHlData[][18], AnsiString s_SortPlDATA[][17]);
   // 2007.11.08
   // 2007.10.12
   //--------------------------------

   //--------------------------------
   // 2008.03.19 ﾐﾗｰ仕様対応 関数追加
   AnsiString __fastcall TBuhinhyou::ConvertRyutaiName(AnsiString s_SideCd, bool bMirror);
   // 2008.03.19
   //--------------------------------

public:
   bool __fastcall Excute(HeadsData* pcHeadsData, HeadsData* apcHeadsDataAll);    //  圧力計部品展開処理 実行

};
//---------------------------------------------------------------------------
// 日本語関数名： 圧力計部品展開処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ：
//---------------------------------------------------------------------------
bool __fastcall Buhinhyou(HeadsData* pcHeadsData, HeadsData* apcHeadsDataAll){
   bool        ret;

   TBuhinhyou  buhinhyou;    // 圧力計ﾃﾞｰﾀ 作成Class自動生成,関数終了後破棄

   ret = buhinhyou.Excute(pcHeadsData, apcHeadsDataAll); // 圧力計ﾃﾞｰﾀ 作成実行

   return ret;
}

//---------------------------------------------------------------------------
// 日本語関数名： 標準部品表展開処理 作成実行
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Excute(HeadsData* pcHeadsData, HeadsData* apcHeadsDataAll)
{
   // 2007.10.11 ﾐﾗｰ仕様対応
   // 元仕様以外は処理不要(正常終了)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);

   // HeadsDataｸﾗｽ変数
   m_pcHeadsData = pcHeadsData;

   // HEADSﾃﾞｰﾀを変数にｾｯﾄ、ﾐﾗｰ仕様の場合、ﾐﾗｰのﾃﾞｰﾀもｾｯﾄ
   if(!apcHeadsDataAll[0].SetAnsiStringData(P_HEADS_DATA)) return(false);
   if(apcHeadsDataAll[0].GetMirrorFlag()){
      // ﾐﾗｰA仕様、ﾐﾗｰB仕様のﾃﾞｰﾀ
      if(!apcHeadsDataAll[1].SetAnsiStringData(P_HEADS_DATA_A)) return(false);
      if(!apcHeadsDataAll[2].SetAnsiStringData(P_HEADS_DATA_B)) return(false);
   }

   // ﾛｸﾞ作成
   Write_Log("--------------------------------------------------------");
   Write_Log("----------      『標準部品表作成処理』開始      ----------");
   Write_Log("--------------------------------------------------------");
   Write_Error_Log("--------------------------------------------------------");
   Write_Error_Log("----------      『標準部品表作成処理』開始      ----------");
   Write_Error_Log("--------------------------------------------------------");

   //***********************************
   //***  圧力計ﾃﾞｰﾀ作成
   //***********************************
   Buhinhyou_Data_Sakusei();

   // ﾛｸﾞ作成
   Write_Log("--------------------------------------------------------");
   Write_Log("----------      『標準部品表作成処理』終了      ----------");
   Write_Log("--------------------------------------------------------");
   Write_Error_Log("--------------------------------------------------------");
   Write_Error_Log("----------      『標準部品表作成処理』終了      ----------");
   Write_Error_Log("--------------------------------------------------------");

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 標準部品表(Buhinhyou)　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Buhinhyou_Data_Sakusei(void)
{
   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_SQL;             // SQL文
   AnsiString s_Text;            // 汎用文字列
   AnsiString s_Bunbo;
   AnsiString s_Bunsi;
   AnsiString d_RevNo;           // 製作指示書のある直近のRevNo

   int i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   int i_RevNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   //int i_ErrFlg;
   //int i_Value_1;
   //int i_Value_2;

   //double d_wkValue;

//   s_Tmplate_Pass = G_Template_Dir + "標準部品表.xls";             // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   s_Tmplate_Pass = G_Template_Dir + "標準部品表A3L.xls";             // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ

   //2019/05/17 応用技術追加
   bHasOldDataFlag = false;
   if (bIsNotKatashiki) {
       AnsiString ToFolder, ToFolder1, Rev;
       AnsiString Work, TooFolder, TooFolder1;
       int Sakuban, Number;
       AnsiString sRev, sRev1;
       AnsiString sFirstChar;
       int i_PosIdx;
       AnsiString sNumberList;
       AnsiString sFileName;

       // 移動先フォルダの作成
      //2022/12/15 桁数追加対応 Mod_S
      //Work = G_SAKUBAN.SubString(1, 4);
      if (G_SAKUBAN.Length() > 7)
      {
    	  Work = G_SAKUBAN.SubString(1, 6);
      }
      else
      {
    	  Work = G_SAKUBAN.SubString(1, 4);
      }
      //2022/12/15 桁数追加対応 Mod_E       
       if( gP_Sakuban_Mode == "S" )
       {
          // 奇数の判断
          Sakuban = Work.ToIntDef(0);
		  if( Sakuban % 2 == 1 ){
             Sakuban --;
          }
       	  //2022/12/15 桁数追加対応 Mod_S
    	  //sprintf(Work.c_str(),"%04d",Sakuban);
	     if (Sakuban > 9999)
         {
			 Work.sprintf("%06d",Sakuban);
		 }
		 else
		 {
			 Work.sprintf("%04d",Sakuban);
	     }
	     //2022/12/15 桁数追加対応 Mod_E
          ToFolder1 = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
       } else {
           ToFolder1 = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
       }
       bool bHasData = false;
       AnsiString sAB = "A";
       if (bMirrorFlag) {
           sAB = "B";
           bMirrorFlag = false;
       }
       sRev.sprintf("R%02d", 0 );
       sRev1.sprintf("%02d", 0 );
       sFirstChar = G_SAKUBAN.SubString(1, 1);
       sNumberList = "0123456789";
       i_PosIdx = sNumberList.Pos(sFirstChar);
       if (i_PosIdx > 0) {
           if (G_HEADS_DATA_Rev > 0) {
               sRev.sprintf("R%02d", G_HEADS_DATA_Rev);
               sRev1.sprintf("%02d", G_HEADS_DATA_Rev);
           }
       }

	   ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
             + IncludeTrailingBackslash(ToFolder1)
             + IncludeTrailingBackslash(Work)
             + IncludeTrailingBackslash(G_SAKUBAN);

	   //2023/4/21 製造番号桁数追加対応 Mod
	   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sAB + sRev + ".xls";
	   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sAB + sRev + ".xls";
       if (FileExists(sFileName)) {
           s_Tmplate_Pass = sFileName;
           bHasData = true;
           bHasOldDataFlag = true;
       } else {
		   //2023/4/21 製造番号桁数追加対応 Mod
		   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sRev + ".xls";
		   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sRev + ".xls";
           if (FileExists(sFileName)) {
			   s_Tmplate_Pass = sFileName;
               bHasData = true;
               bHasOldDataFlag = true;
           }
       }

       if (!bHasData) {
           ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
                 + IncludeTrailingBackslash(ToFolder1)
                 + IncludeTrailingBackslash(Work)
                 + IncludeTrailingBackslash(G_SAKUBAN)
                 + IncludeTrailingBackslash(sRev1);

		   //2023/4/21 製造番号桁数追加対応 Mod
		   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sAB + sRev + ".xls";
		   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sAB + sRev + ".xls";
           if (FileExists(sFileName)) {
               s_Tmplate_Pass = sFileName;
               bHasData = true;
               bHasOldDataFlag = true;
           } else {
			   //2023/4/21 製造番号桁数追加対応 Mod
			   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sRev + ".xls";
			   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sRev + ".xls";
               if (FileExists(sFileName)) {
                   s_Tmplate_Pass = sFileName;
                   bHasData = true;
                   bHasOldDataFlag = true;
               }
           }
       }

       if (!bHasData) {
           sRev.sprintf("R%02d", 0 );
           sRev1.sprintf("%02d", 0 );
           sFirstChar = G_SAKUBAN.SubString(1, 1);
           sNumberList = "0123456789";
           i_PosIdx = sNumberList.Pos(sFirstChar);
           if (i_PosIdx > 0) {
               if (G_HEADS_DATA_Rev > 1) {
                   sRev.sprintf("R%02d", G_HEADS_DATA_Rev-1);
                   sRev1.sprintf("%02d", G_HEADS_DATA_Rev-1);
               }
           }

           ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
                 + IncludeTrailingBackslash(ToFolder1)
                 + IncludeTrailingBackslash(Work)
                 + IncludeTrailingBackslash(G_SAKUBAN)
                 + IncludeTrailingBackslash(sRev1);

		   //2023/4/21 製造番号桁数追加対応 Mod
		   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sAB + sRev + ".xls";
		   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sAB + sRev + ".xls";
           if (FileExists(sFileName)) {
               s_Tmplate_Pass = sFileName;
               bHasOldDataFlag = true;
           } else {
			   //2023/4/21 製造番号桁数追加対応 Mod
			   //sFileName = ToFolder1 + "C" + G_SAKUBAN + sRev + ".xls";
			   sFileName = ToFolder1 + "C" + GetFileSeizouBangou(G_SAKUBAN) + sRev + ".xls";
               if (FileExists(sFileName)) {
                   s_Tmplate_Pass = sFileName;
                   bHasOldDataFlag = true;
               }
           }
       }
   }

   //2023/4/21 製造番号桁数追加対応 Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "C" + G_SAKUBAN + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "C" + GetFileSeizouBangou(G_SAKUBAN) + ".xls"; // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『標準部品表』作成開始    ----------";
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
   //***                       標準部品表　データセット                      ***
   //***                                                                     ***
   //***************************************************************************
   // ｼｰﾄの指定
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SHEETNAME);
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)EXCEL_SHEETNAME);

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                    ヘッダー部　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------

   // 2014.04.07 ﾍｯﾀﾞｰ削除解除
   //// 2012.11.07 中国ﾍｯﾀﾞｰ削除
   //////----------------------------
   ////// 2007.11.15 ミラー仕様対応
   //////if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet();
   ////if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet(m_pcHeadsData->GetMirrorFlag());
   //if (P_HEADS_DATA[1107].TrimRight() == "CHINA"){
   //   // 中国
   //// 2013.04.03 巨元追加
   //   // 中国巨元
   //} else if (P_HEADS_DATA[1107].TrimRight() == "KYOGEN"){
   //// *******************　　
   //} else {
   //   // 中国以外
   //   if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet(m_pcHeadsData->GetMirrorFlag());                                                 // 日本語
   //}
   ////// 2007.11.15
   //////----------------------------
   //// ************************
   if ( P_HEADS_DATA[36].Pos("M") != 0 ) Excel_HeaderSet(m_pcHeadsData->GetMirrorFlag());
   // ************************

   //*********************
   //***  PHE型式
   //*********************
   // 『A001』
   i_RecNo = 33;
   if (P_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(2, 2, s_Text);
   G_Log_Text = "型式                  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************
   //*** 担当者
   //*********************
   // 『A002』
   i_RecNo = 1079;
   s_Text = P_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(3, 23, s_Text);
   G_Log_Text = "担当者                RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  作図日
   //***************************
   // 『A003』
   i_RecNo = 1087;
   s_Text = "'"+ HanToZen( P_HEADS_DATA[i_RecNo] );
   Excel_Cell_DataSet(3, 31, s_Text);
   G_Log_Text = "作図日                RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  図番
   //***************************
   // 『A004』
   //2023/4/21 製造番号桁数追加対応 Mod
   //s_Text = HanToZen( "C" + G_SAKUBAN );
   s_Text = HanToZen( "C" + GetFileSeizouBangou(G_SAKUBAN) );
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   Excel_Cell_DataSet(2, 61, s_Text);
   //2003.09.08 図番出力追加
   Excel_Cell_DataSet(135, 61, s_Text);
   Excel_Cell_DataSet(204, 61, s_Text);
   Excel_Cell_DataSet(273, 61, s_Text);
   Excel_Cell_DataSet(342, 61, s_Text);
   //***********************
   G_Log_Text = "図番                 『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  REV
   //***************************
   // 『A005』
   i_RecNo = 319;
   i_RevNo = P_HEADS_DATA[i_RecNo].ToIntDef(0);
   d_RevNo = FormatFloat("0",i_RevNo);
   s_Text = "ＲＥＶ." + HanToZen( d_RevNo );
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   Excel_Cell_DataSet(2, 73, s_Text);
   //2003.09.08 ＲＥＶ出力追加
   //2003.09.09 ＲＥＶ出力訂正
   //Excel_Cell_DataSet(135, 73, s_Text);
   //Excel_Cell_DataSet(204, 73, s_Text);
   //Excel_Cell_DataSet(273, 73, s_Text);
   //Excel_Cell_DataSet(342, 73, s_Text);
   Excel_Cell_DataSet(135, 68, s_Text);
   Excel_Cell_DataSet(204, 68, s_Text);
   Excel_Cell_DataSet(273, 68, s_Text);
   Excel_Cell_DataSet(342, 68, s_Text);
   //*************************
   G_Log_Text = "ＲＥＶ               『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2003.09.09 ＶＥＲ出力追加
   //***************************
   //***  VER
   //***************************
   i_RecNo = 1267;
   s_Text = "ＶＥＲ." + P_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(66, 75, s_Text);
   Excel_Cell_DataSet(135, 75, s_Text);
   Excel_Cell_DataSet(204, 75, s_Text);
   Excel_Cell_DataSet(273, 75, s_Text);
   Excel_Cell_DataSet(342, 75, s_Text);
   G_Log_Text = "ＶＥＲ               『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //*************************

   //***************************
   //***  製造番号
   //***************************
   // 『A006』
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = HanToZen( G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4) );
   s_Text = HanToZen( GetSeizouBangou(G_SAKUBAN) );
   //2022/12/15 製造番号桁数追加対応 Mod_E   

   //------------------------------------
   // 2007.10.11 ﾐﾗｰ仕様対応
   if(m_pcHeadsData->GetMirrorFlag()){
      s_Text = s_Text + HanToZen( "A/B" );
   }
   // 2007.10.11
   //------------------------------------

   Excel_Cell_DataSet(6, 3, s_Text);
   G_Log_Text = "製造番号             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  納入先
   //***************************
   // 『A007』
   i_RecNo = 1065;
   s_Text = P_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(6, 14, s_Text);
   G_Log_Text = "納入先               『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*******************
   //***  台  数
   //*******************
   // 『A008』
   i_RecNo = 39;
   s_Text = P_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = "'"+ HanToZen(s_Text);
      Excel_Cell_DataSet(6, 36, s_Text);
      G_Log_Text = "台数                  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "台数      RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      //Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //************************
   //***  適用規格
   //************************
   // 『A009』
   i_RecNo = 29;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "適用規格              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』により、";

   ZeroSupress( s_Text, s_Text, 2 );

   s_Text = Search_KIKAKU_CODE(s_Text,true,true, P_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(6, 38, s_Text);
   G_Log_Text = G_Log_Text + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);


   //2019/05/17 応用技術追加
   if (!bIsNotKatashiki) {

   //---------------------------------------------------------------------------
   //---                                                                     ---
   //---                        明細部　　データセット                       ---
   //---                                                                     ---
   //---------------------------------------------------------------------------
   // 2007.10.11 集計用変数初期化
   P_WkRec = 0;
   P_WkRec3 = 0;

   //2003.11.14 関数追加
   Read_Buhin_Data();        // 舶用部品行数読込
   //*******************

   //2019.10.23 付属品ﾃﾞｰﾀ出力行数変更_S
   Read_ACCS_Data();         // 付属品行数読込
   //2019.10.23 付属品ﾃﾞｰﾀ出力行数変更_E

   Write_Buhin_Detail();

   Cyuki_Out();

   // ページ数を追加したが、データを１行も追加していない場合は１ページ減算する。
   if(P_Row <= ( DefExcelPageLine + (P_PageNum-1) * DefExcelPageLine ) + 3) {
        P_PageNum--;
   }

   // 新・標準部品表用　不要ページ削除処理
   Excel_Cell_Delete(P_PageNum+1);

   //2019/05/17 応用技術追加
   } else {
       if (!bHasOldDataFlag) {
           // 新・標準部品表用　不要ページ削除処理
           Excel_Cell_Delete(1);
       }
   }

   //******************************
   //***  Excel Book Close
   //******************************
   // 2014/08/07 応用技術追加
   // 言語識別文字列がEN(英語)の場合、日英変換マクロを実行
   if(G_Language == "EN") {
        exApplication.OleFunction("Run" , OleVariant("TranslateJPtoEN"));
   }

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
   G_Log_Text = "----------    『標準部品表』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//2003.11.14 舶用部品行数****************************************************
//---------------------------------------------------------------------------
// 日本語関数名： 舶用部品行数読込
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Read_Buhin_Data(void)
{
   char  buf[1024];                            /* 文字列バッファ   */
   char  *tok;                                 /* トークンポインタ */
   int   i;                                    /* ループカウンタ   */
   char  token[100][100];                      /* トークンバッファ */

//   AnsiString P_PARTS[16]; // 部品データ
   AnsiString P_PARTS[17]; // 部品データ
   AnsiString wk_FilePass;
   FILE *fp;

   // 2007.10.11 ﾐﾗｰ仕様対応 追加
   AnsiString wk_PARTS[1000][17];
   int i_wkNum;
   int j;

   iBLTCV = 0;
   P_SHIP_Num = 0;
   i_wkNum = 0;

   //***************************
   //**  部品展開ﾃﾞｰﾀ読込
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      //マルチサーバ対応 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //マルチサーバ対応 2009/1/13 MOD END
   } else {
      //マルチサーバ対応 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //マルチサーバ対応 2009/1/13 MOD END
   }

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      fclose(fp);    // 閉じる
      return false;
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
      //  舶用部品行数
      //---------------------------------------------------
      if( P_PARTS[0] == "SHIP" )
      {
        //---------------------------------
        // 2007.10.11 ﾐﾗｰ仕様対応
        // A仕様とB仕様で同ﾃﾞｰﾀが存在する場合があるので、ﾏｰｼﾞする

         if( i_wkNum > 0 ){
            for( j = 0; j < i_wkNum; j++){
                if( (wk_PARTS[j][1] == P_PARTS[1])
                 && (wk_PARTS[j][3] == P_PARTS[3])
                 && (wk_PARTS[j][4] == P_PARTS[4])
                 && (wk_PARTS[j][5] == P_PARTS[5])
                 && (wk_PARTS[j][10] == P_PARTS[10])
                 && (wk_PARTS[j][11] == P_PARTS[11])
                 && (wk_PARTS[j][12] == P_PARTS[12])
                 && (wk_PARTS[j][13] == P_PARTS[13]) )
                {
                    break;
                }
            }
         }

         if(( i_wkNum == 0 ) || ( j == i_wkNum )){
            // まだ取得していなければ、作業配列に格納
            wk_PARTS[i_wkNum][0] = P_PARTS[0];     // 部品分類
            wk_PARTS[i_wkNum][1] = P_PARTS[1];     // 識別子
            wk_PARTS[i_wkNum][2] = P_PARTS[2];     // 部品型式
            wk_PARTS[i_wkNum][3] = P_PARTS[3];     // 部品名称
            wk_PARTS[i_wkNum][4] = P_PARTS[4];     // 材質型式
            wk_PARTS[i_wkNum][5] = P_PARTS[5];     // 図番
            wk_PARTS[i_wkNum][6] = P_PARTS[6];     // 作図用図番
            wk_PARTS[i_wkNum][7] = P_PARTS[7];     // １台分数量
            wk_PARTS[i_wkNum][8] = P_PARTS[8];     // 予備数
            wk_PARTS[i_wkNum][9] = P_PARTS[9];     // 総計
            wk_PARTS[i_wkNum][10] = P_PARTS[10];   // 仕様１
            wk_PARTS[i_wkNum][11] = P_PARTS[11];   // 仕様２
            wk_PARTS[i_wkNum][12] = P_PARTS[12];   // 仕様３
            wk_PARTS[i_wkNum][13] = P_PARTS[13];   // 仕様４
            wk_PARTS[i_wkNum][14] = P_PARTS[14];   // 仕様５
            wk_PARTS[i_wkNum][15] = P_PARTS[15];   // 材質名称
            wk_PARTS[i_wkNum][16] = P_PARTS[16];   // 仕様ﾀｲﾌﾟ(""/"A"/"B")

            // SHIP行ｶｳﾝﾄ
            i_wkNum++;

            P_SHIP_Num++;
         }

        // 2007.10.11 ﾐﾗｰ仕様対応
        //---------------------------------

      //2019.10.10 付属品ﾃﾞｰﾀ出力行数変更_S
      }

   }

   fclose(fp);    // 閉じる
   return true;
}
//***************************************************************************

//2019.10.23 付属品部品行数_S************************************************
//---------------------------------------------------------------------------
// 日本語関数名： 付属品部品行数読込
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Read_ACCS_Data(void)
{
   char  buf[1024];                            /* 文字列バッファ   */
   char  *tok;                                 /* トークンポインタ */
   int   i;                                    /* ループカウンタ   */
   char  token[100][100];                      /* トークンバッファ */

   AnsiString P_PARTS[17]; // 部品データ
   AnsiString wk_FilePass;
   FILE *fp;

   AnsiString wk_PARTS[1000][17];
   int i_wkNum;
   int j;

   iBLTCV = 0;
   P_ACCS_Num = 0;  // 付属品部品行数
   i_wkNum = 0;

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
      return false;
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
      //  付属品部品行数
      //---------------------------------------------------
      if( P_PARTS[0] == "ACCS" )
      {
        //---------------------------------
        // 2007.10.11 ﾐﾗｰ仕様対応
        // A仕様とB仕様で同ﾃﾞｰﾀが存在する場合があるので、ﾏｰｼﾞする

         if( i_wkNum > 0 ){
            for( j = 0; j < i_wkNum; j++){
                if( (wk_PARTS[j][1] == P_PARTS[1])
                 && (wk_PARTS[j][3] == P_PARTS[3])
                 && (wk_PARTS[j][4] == P_PARTS[4])
                 && (wk_PARTS[j][5] == P_PARTS[5])
                 && (wk_PARTS[j][10] == P_PARTS[10])
                 && (wk_PARTS[j][11] == P_PARTS[11])
                 && (wk_PARTS[j][12] == P_PARTS[12])
                 && (wk_PARTS[j][13] == P_PARTS[13]) )
                {
                    break;
                }
            }
         }

         if(( i_wkNum == 0 ) || ( j == i_wkNum )){
            // まだ取得していなければ、作業配列に格納
            wk_PARTS[i_wkNum][0] = P_PARTS[0];     // 部品分類
            wk_PARTS[i_wkNum][1] = P_PARTS[1];     // 識別子
            wk_PARTS[i_wkNum][2] = P_PARTS[2];     // 部品型式
            wk_PARTS[i_wkNum][3] = P_PARTS[3];     // 部品名称
            wk_PARTS[i_wkNum][4] = P_PARTS[4];     // 材質型式
            wk_PARTS[i_wkNum][5] = P_PARTS[5];     // 図番
            wk_PARTS[i_wkNum][6] = P_PARTS[6];     // 作図用図番
            wk_PARTS[i_wkNum][7] = P_PARTS[7];     // １台分数量
            wk_PARTS[i_wkNum][8] = P_PARTS[8];     // 予備数
            wk_PARTS[i_wkNum][9] = P_PARTS[9];     // 総計
            wk_PARTS[i_wkNum][10] = P_PARTS[10];   // 仕様１
            wk_PARTS[i_wkNum][11] = P_PARTS[11];   // 仕様２
            wk_PARTS[i_wkNum][12] = P_PARTS[12];   // 仕様３
            wk_PARTS[i_wkNum][13] = P_PARTS[13];   // 仕様４
            wk_PARTS[i_wkNum][14] = P_PARTS[14];   // 仕様５
            wk_PARTS[i_wkNum][15] = P_PARTS[15];   // 材質名称
            wk_PARTS[i_wkNum][16] = P_PARTS[16];   // 仕様ﾀｲﾌﾟ(""/"A"/"B")

            // SHIP行ｶｳﾝﾄ
            i_wkNum++;

            P_ACCS_Num++;
         }


      }

   }

   fclose(fp);    // 閉じる
   return true;
}
//2019.10.23 付属品部品行数_E************************************************

//---------------------------------------------------------------------------
// 日本語関数名： 標準部品表(Buhinhyou)　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Write_Buhin_Detail(void)
{
   char  buf[1024];                            /* 文字列バッファ   */
   char  *tok;                                 /* トークンポインタ */
   int   i;                                    /* ループカウンタ   */
   char  token[100][100];                      /* トークンバッファ */

   int   RecNo;

//@@@@
   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名

   int   i_plateflg = 0;   // プレート部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_frameflg = 0;   // フレーム部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_nozzlflg = 0;   // ノズル部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_Atempflg = 0;   // A側温度計部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_Btempflg = 0;   // B側温度計部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_Apresflg = 0;   // A側圧力計部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_Bpresflg = 0;   // B側圧力計部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_accsflg  = 0;   // 付属品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_shipflg  = 0;   // 舶用部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_errflg   = 0;   // エラー部品ﾀｲﾄﾙ表示ﾌﾗｸﾞ

   int   i_anaakeflg = 0;  // 孔明ﾀｲﾄﾙ表示ﾌﾗｸﾞ
   int   i_tyukiflg = 0;
   int   i_RecNo;

   bool  b_YobiSumMode;

   AnsiString s_Text;

//   AnsiString P_PARTS[15]; // 部品データ
   AnsiString P_PARTS[17]; // 部品データ

   int   i_ErrItemCnt;          // "ERROR-"で出力された項目のカウンタ
   int   i_Base_Y;              // "ERROR-"の項目の書き出し位置(Y軸)

   bool bRet;
   AnsiString s_wkData[1000][17];
   int j;

//@@@@

   AnsiString wk_FilePass;
   FILE *fp;

   //2005-10-07
   i_ErrItemCnt = 0;
   i_Base_Y = 112;

   iBLTCV = 0;
   //***************************
   //**  部品展開ﾃﾞｰﾀ読込
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      //マルチサーバ対応 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //マルチサーバ対応 2009/1/13 MOD END

   } else {
      //マルチサーバ対応 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN)
      //            + "HD_PARTS.csv";
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
      //マルチサーバ対応 2009/1/13 MOD END
   }
   //***************************
   //**  前回部品展開ﾃﾞｰﾀ読込
   //***************************
   if( Prev_Data_Get() )
      P_compflg = 1;
   else
      P_compflg = 0;

   //2023.02.08 製造番号桁数追加_S
   if( (P_compflg == 0) && (G_SAKUBAN.Length() > 7) && (G_SAKUBAN.SubString(1, 2) == "10") ) {
      //前回部品展開ﾃﾞｰﾀ読込に失敗かつ８桁で入力された６桁時代前回部品展開ﾃﾞｰﾀ読込
      if( Prev_Old_Data_Get() ) {
         P_compflg = 1;
      } else {
         P_compflg = 0;
      }
   }
   //2023.02.08 製造番号桁数追加_E

   //2004.12.15 REV00の場合の訂正印変更
   if (P_HEADS_DATA[319].TrimRight() == "0" || P_HEADS_DATA[319].TrimRight() == "00") {
      P_compflg = 0;
   }
   //**********************************

    //-----------------------------------------------------------------------------
    // 2007.10.12 ﾐﾗｰ仕様対応
    // 部品展開ﾃﾞｰﾀの読込みは別関数で行うよう変更
    // 別関数で、ﾐﾗｰ仕様のﾃﾞｰﾀをﾏｰｼﾞ,ｿｰﾄされて配列に格納される。
    // ﾏｰｼﾞ,ｿｰﾄされた配列ﾃﾞｰﾀを処理する。

/*   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   //*****************
   // 読込み可能の間
   //*****************
   P_Row = 8;        // 初期出力行設定
   RecNo = 0;
   P_PageNum = 0;
   b_YobiSumMode = false;

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
      //***  (型式、識別子以外全角)
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
*/

   //*********************************************
   // 部品ﾃﾞｰﾀを配列に読み込む(ﾏｰｼﾞ,ｿｰﾄ機能付)
   //*********************************************
   bRet = Read_Buhin_Data_Sorted(wk_FilePass, s_wkData);
   if(!bRet) return(false);

   //*****************
   // 読込み可能の間
   //*****************
   P_Row = 8;        // 初期出力行設定
   RecNo = 0;
   P_PageNum = 0;
   b_YobiSumMode = false;

   for(j=0;j<1000;j++){
     if( s_wkData[j][0].Length() == 0 ) break;

      //*****************************************
      //***  部品データ(HD_PARTS.csv)の取得
      //***  (型式、識別子以外全角)
      //*****************************************
      P_PARTS[0] = s_wkData[j][0];     // 部品分類
      P_PARTS[1] = s_wkData[j][1];     // 識別子
      P_PARTS[2] = s_wkData[j][2];     // 部品型式
      P_PARTS[3] = s_wkData[j][3];     // 部品名称
      P_PARTS[4] = s_wkData[j][4];     // 材質型式
      P_PARTS[5] = s_wkData[j][5];     // 図番
      P_PARTS[6] = s_wkData[j][6];     // 作図用図番
      P_PARTS[7] = s_wkData[j][7];     // １台分数量
      P_PARTS[8] = s_wkData[j][8];     // 予備数
      P_PARTS[9] = s_wkData[j][9];     // 総計
      P_PARTS[10] = s_wkData[j][10];   // 仕様１
      P_PARTS[11] = s_wkData[j][11];   // 仕様２
      P_PARTS[12] = s_wkData[j][12];   // 仕様３
      P_PARTS[13] = s_wkData[j][13];   // 仕様４
      P_PARTS[14] = s_wkData[j][14];   // 仕様５
      P_PARTS[15] = s_wkData[j][15];   // 材質名称
      P_PARTS[16] = s_wkData[j][16];   // ﾐﾗｰ仕様(""/"A"/"B")

    // 2007.10.12 ﾐﾗｰ仕様対応
    // 部品展開ﾃﾞｰﾀの読込みは別関数で行うよう変更
    //-----------------------------------------------------------------------------

      // 数量0の場合はブランクとする
      //2009/04/01 製造番号22-7621の標準部品表でエラー対応 MOD_S
      //if (P_PARTS[7] == "0" ) {
      if ((P_PARTS[7] == "0" ) || (P_PARTS[7] == "０" )) {
         P_PARTS[7] = "";
      }
      //if (P_PARTS[8] == "0" ) {
      if ((P_PARTS[8] == "0" ) || (P_PARTS[8] == "０" )) {
         P_PARTS[8] = "";
      }
      //2009/04/01 製造番号22-7621の標準部品表でエラー対応 MOD_E

      //----------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応
      //for( i=0; i<15; i++ ) {
      for( i=0; i<17; i++ ) {
      // 2007.10.12
      //----------------------------
         if( i > 1 ){
//            if ( P_PARTS[i].Pos("液抜ノズル") != 0 ) {
//               P_PARTS[i] = HanToZen(P_PARTS[i].SubString(1,11)) + P_PARTS[i].SubString(12,P_PARTS[i].Length());
//            } else {
               P_PARTS[i] = HanToZen(P_PARTS[i]);
//            }
         }
      }

      //****************************************************************************
      //***                                                                      ***
      //***                         プレート部品出力                             ***
      //***                                                                      ***
      //****************************************************************************
      if( P_PARTS[0] == "PLATE" ){
         //*****************************
         //***  タイトル　セット
         //*****************************
         //↓2002/10/24 集計処理を追加
         if( i_plateflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            // 改行
            P_Row++;

            Excel_Cell_DataSet(P_Row, 3, "プレート部品");
            P_Row++;
            i_plateflg++;
            P_WkRec = 0;
         }
         //↑2002/10/24

         //↓2014/07/17 総計が0である場合、出力しないようにする。
         if ((P_PARTS[9] == "0" ) || (P_PARTS[9] == "０" )) {
             continue;
         }
         //↑2014/07/17

         if ( P_PARTS[1] == "DPLT--" ) {
            i_anaakeflg = 0;
            i_tyukiflg = 0;

            //*******************************
            //***  プレート本体データ出力
            //*******************************
            // 部品データ
            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // 部品１(部品型式)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // 部品２(部品名称)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // 材質名称
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // 図番
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // 総計
            if ( P_compflg == 1 ) {
               if( !Compare_Data( P_PARTS )){
                  Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
               }
            }
            P_Row++;

         } else if( P_PARTS[1] == "PLHL--" ) {
            //*********************************************
            //*** プレート孔明データ出力 識別子｢PLHL--｣
            //*********************************************
            if( i_anaakeflg == 0 ){
               //******************
               //*** 注記(禁油1)
               //******************
               i_RecNo = 1302;
               s_Text = P_HEADS_DATA[i_RecNo];
               Excel_Cell_DataSet(P_Row, 5, s_Text);
               Excel_Cell_DataSet(P_Row, 47, "孔明仕様");
               P_Row++;

               i_anaakeflg = 1;
            }

            if( i_tyukiflg == 0 ){
               //*****************
               // 注記(電解研磨1)
               //*****************
               i_RecNo = 1304;
               s_Text = P_HEADS_DATA[i_RecNo];
               Excel_Cell_DataSet(P_Row, 5, s_Text);
               i_tyukiflg = 1;
            }

            //****************
            // 孔明部品データ
            //****************
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]); // 仕様１
            Excel_Cell_DataSet(P_Row, 62, P_PARTS[11]); // 仕様２
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // 総計
            Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // 単位（式）
            if( P_compflg == 1 ){
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");   // 比較
            }

            //----------------------------------------
            // 2007.10.15 ﾐﾗｰ仕様対応 号機名称出力
            if ( P_PARTS[16] != "" ) {
                Excel_Cell_DataSet(P_Row, 66, P_PARTS[16] + "号機");  // 号機名称
            }
            // 2007.10.15
            //----------------------------------------

            P_Row++;

         } else {
            //**********************************************
            // プレート部品データ (数量=0の場合読み飛ばす)
            //**********************************************

            //---------------------------------------------------
            // 2007.10.25 ﾐﾗｰ仕様対応
            // 以下の部品集計方法を変更(集計条件を厳しくする)
            //  [DﾌﾟﾚｰﾄｶﾞｽｹｯﾄA][DﾌﾟﾚｰﾄｶﾞｽｹｯﾄB]
            //  [DﾌﾟﾚｰﾄｶﾞｽｹｯﾄDA][DﾌﾟﾚｰﾄｶﾞｽｹｯﾄDB]
            //  [EﾌﾟﾚｰﾄｶﾞｽｹｯﾄDA][EﾌﾟﾚｰﾄｶﾞｽｹｯﾄDB]
            // 2007.11.02 追加
            //  [Eﾌﾟﾚｰﾄ裏当て板]

            //Buhin_Syukei2( P_PARTS );

            if( ( P_PARTS[1] == "DPLG-A" )
             || ( P_PARTS[1] == "DPLG-B" )
             || ( P_PARTS[1] == "DPLGDA" )
             || ( P_PARTS[1] == "DPLGDB" )
             || ( P_PARTS[1] == "EPLGDA" )
             || ( P_PARTS[1] == "EPLGDB" )
             || ( P_PARTS[1] == "EPGS-R" ) ){
                // 部品集計
                Buhin_Syukei( P_PARTS );
            } else {
                // 部品集計
                Buhin_Syukei2( P_PARTS );
            }

            // 2007.10.25
            //---------------------------------------------------

            b_YobiSumMode = true;
            // 改行
//            if( ZenToHan( P_PARTS[7] )!= "0" ) {
//               Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // 部品１(部品型式)
//               Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // 部品２(部品名称)
//               Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // 材質名称
//               Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // 図番
//               Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1台分数量
//               Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // 予備数
//               Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // 総計
//               if( P_compflg == 1 ) {
//                  if( !Compare_Data( P_PARTS ) )
//                     Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
//               }
//               P_Row++;
//            }
         }
      }


      //****************************************************************************
      //***                                                                      ***
      //***                         フレーム部品出力                             ***
      //***                                                                      ***
      //****************************************************************************
      else if( P_PARTS[0] == "MOATE" ){
         if( P_PARTS[1] == "EPBP-A" || P_PARTS[1] == "EPBP-B" )
         {
            Buhin_Syukei3( P_PARTS );
            b_YobiSumMode = true;
         }
      }
      else if( P_PARTS[0] == "FRAME" ){
         if( i_frameflg == 0 ) {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "フレーム部品");
            P_Row++;
            i_frameflg++;
         }

         // 2022.05.23 フレームガス切り追加_S
         //*****************************
         // フレームSガス切りデータ出力
         //*****************************
         if( P_PARTS[1] == "FRAMSG" )
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);   // 部品１(部品型式)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);   // 部品２(部品名称)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);   // 材質名称
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);   // 図番
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]);  // 仕様１

            // ･通常仕様 28ﾊﾞｲﾄを超えるか
            // ･ﾐﾗｰ仕様  22ﾊﾞｲﾄを超えるか
            if( P_PARTS[11].Length() > 28 ) {
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]);  // 仕様２
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]);  // 仕様２
            }
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);   // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);   // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);   // 総計
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
            }

            P_Row++;

         }
         // 2022.05.23 フレームガス切り追加_E

         //*****************************
         //*** フレームS本体データ出力
         //*****************************
         // 2022.05.23 フレームガス切り追加_S
         //if( P_PARTS[1] == "FRAM-S" )
         else if( P_PARTS[1] == "FRAM-S" )
         // 2022.05.23 フレームガス切り追加_E
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);   // 部品１(部品型式)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);   // 部品２(部品名称)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);   // 材質名称
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);   // 図番
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]);  // 仕様１
//            Excel_Cell_DataSet(P_Row, 62, P_PARTS[11]);  // 仕様２
            if( P_PARTS[11].Length() > 28 ) {
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]);  // 仕様２
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]);  // 仕様２
            }
            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);   // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);   // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);   // 総計
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
            }
            P_Row++;

            //*************
            // 注記(ﾉｽﾞﾙ1)
            //*************
            i_RecNo = 1325;
            s_Text = P_HEADS_DATA[i_RecNo];
            Excel_Cell_DataSet(P_Row, 47, s_Text);

            P_Row++;

            //*************
            // 注記(ﾉｽﾞﾙ2)
            //*************
            i_RecNo = 1326;
            s_Text = P_HEADS_DATA[i_RecNo];
            Excel_Cell_DataSet(P_Row, 47, s_Text);

            P_Row++;
         }

         // 2022.05.23 フレームガス切り追加_S
         //*****************************
         // フレームEガス切りデータ出力
         //*****************************
         else if( P_PARTS[1] == "FRAMEG" )


         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);  // 部品１(部品型式)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);  // 部品２(部品名称)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);  // 材質名称
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);  // 図番
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]); // 仕様１

            // ･通常仕様 28ﾊﾞｲﾄを超えるか
            // ･ﾐﾗｰ仕様  22ﾊﾞｲﾄを超えるか
            if( P_PARTS[11].Length() > (( m_pcHeadsData->GetMirrorFlag())? 22: 28 ) ){
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]); // 仕様２
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // 仕様２
            }

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // 総計
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
            }

            if (P_PARTS[16] != "" ) {
                Excel_Cell_DataSet(P_Row, 66, P_PARTS[16] + "号機");  // 号機名称
            }

            P_Row++;

         }
         // 2022.05.23 フレームガス切り追加_E

         //*****************************
         // フレームE本体データ出力
         //*****************************
         else if( P_PARTS[1] == "FRAM-E" )
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);  // 部品１(部品型式)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);  // 部品２(部品名称)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);  // 材質名称
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);  // 図番
            Excel_Cell_DataSet(P_Row, 48, P_PARTS[10]); // 仕様１
//            Excel_Cell_DataSet(P_Row, 62, P_PARTS[11]); // 仕様２

            //-------------------------------------
            // 2007.11.14 ﾐﾗｰ仕様対応
            // ･通常仕様 28ﾊﾞｲﾄを超えるか
            // ･ﾐﾗｰ仕様  22ﾊﾞｲﾄを超えるか

            //if( P_PARTS[11].Length() > 28 ) {
            if( P_PARTS[11].Length() > (( m_pcHeadsData->GetMirrorFlag())? 22: 28 ) ){
               Excel_Cell_DataSet(P_Row, 55, P_PARTS[11]); // 仕様２
            } else {
               Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // 仕様２
            }
            // 2007.11.14
            //-------------------------------------

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // 総計
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
            }

            //---------------------------------------
            // 2007.10.12 ﾐﾗｰ仕様対応 号機名称追加
            if (P_PARTS[16] != "" ) {
                Excel_Cell_DataSet(P_Row, 66, P_PARTS[16] + "号機");  // 号機名称
            }
            // 2007.10.12
            //---------------------------------------

            P_Row++;

            //---------------------------------------
            // 2007.10.17 ﾐﾗｰ仕様対応
            // ﾐﾗｰA仕様の直後には出力しない(if文追加)

            // 元仕様/ﾐﾗｰB仕様の場合
            if ( ZenToHan( P_PARTS[16] ) != "A"){
                //*************
                // 注記(ﾉｽﾞﾙ3)
                //*************
                i_RecNo = 1327;
                s_Text = P_HEADS_DATA[i_RecNo];
                Excel_Cell_DataSet(P_Row, 47, s_Text);

                P_Row++;

                //*************
                // 注記(ﾉｽﾞﾙ4)
                //*************
                i_RecNo = 1328;
                s_Text = P_HEADS_DATA[i_RecNo];
                Excel_Cell_DataSet(P_Row, 47, s_Text);

                P_Row++;

                // ﾜｰｸ格納ﾃﾞｰﾀあり?
                if( P_WkRec3 > 0 )
                    Output_WK_PARTS3();

                // 改行
                P_Row++;
            }

            // 2007.10.17
            //---------------------------------------
         }

         //********************************
         //*** 上部ガイドバーデータ出力
         //********************************
         else if( P_PARTS[1] == "GDBR-U" )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(true);

            // 改行
            P_Row++;

            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // 部品１(部品型式)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // 部品２(部品名称)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // 材質名称
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // 図番

            i_RecNo = 1022;
            s_Text = "ＬＵ１＝" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // 仕様(ＬＵ１)
            Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // 仕様２

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // 総計
            Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // 単位（式）
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
            }
            P_Row++;

            // 仕様(2行目)
            i_RecNo = 1023;
            s_Text = "ＬＵ２＝" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // 仕様(ＬＵ２)

            i_RecNo = 1288;
            if( P_HEADS_DATA[i_RecNo].ToIntDef(0) !=  0 )
            {
               s_Text = "ＬＵ３＝" + HanToZen( P_HEADS_DATA[i_RecNo] );
               Excel_Cell_DataSet(P_Row, 58, s_Text);  // 仕様(ＬＵ３)
            }
            P_Row++;
         }

         //*********************************
         //***  下部ガイドバーデータ出力
         //*********************************
         else if( P_PARTS[1] == "GDBR-L" )
         {
            i_frameflg++;

            Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]); // 部品１(部品型式)
            Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]); // 部品２(部品名称)
            Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]); // 材質名称
            Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]); // 図番

            i_RecNo = 1024;
            s_Text = "ＬＬ１＝" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // 仕様(ＬＬ１)
            Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // 仕様２

            Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]); // 1台分数量
            Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]); // 予備数
            Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]); // 総計
            Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // 単位（式）
            if( P_compflg == 1 )
            {
               if( !Compare_Data( P_PARTS ) )
                  Excel_Cell_DataSet(P_Row, 80, "*");  // 比較
            }
            P_Row++;

            // 仕様(2行目)
            i_RecNo = 1025;
            s_Text = "ＬＬ２＝" + HanToZen( P_HEADS_DATA[i_RecNo] );
            Excel_Cell_DataSet(P_Row, 47, s_Text);     // 仕様(ＬＬ２)

            i_RecNo = 1289;
            if( P_HEADS_DATA[i_RecNo].ToIntDef(0) !=  0 )
            {
               s_Text = "ＬＬ３＝" + HanToZen( P_HEADS_DATA[i_RecNo] );
               Excel_Cell_DataSet(P_Row, 58, s_Text);  // 仕様(ＬＬ３)
            }
            P_Row++;
         }

         else
         {
            //**********************************************
            // フレーム部品データ (数量=0の場合読み飛ばす)
            //**********************************************
            if( ( ZenToHan( P_PARTS[7] )!= "" )&&
                ( P_PARTS[4].SubString(1,5) != "GDBR-" ) )
            {
               if( ( P_PARTS[1] != "BNWR--" )&&( P_PARTS[1] != "BOLTNR" ) ) {
                 if ( iBLTCV == 1 ) {
                   iBLTCV=2;
//                   BLT_Cover_Out();
                 }
               }
               Excel_Cell_DataSet(P_Row, 4,  P_PARTS[2]);  // 部品１(部品型式)
               Excel_Cell_DataSet(P_Row, 13, P_PARTS[3]);  // 部品２(部品名称)
               if( ( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) ) && (P_HEADS_DATA[36].Pos("M")!=0) ) {
                  Excel_Cell_DataSet(P_Row, 13, "ナットはＷナットのこと");  // 舶用の場合、ボルトナットに注記を追加
               }
               Excel_Cell_DataSet(P_Row, 24, P_PARTS[4]);  // 材質名称
               //2014.12.11 ﾎﾞﾙﾄ材質変更
               if( ( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) )
                && ( P_HEADS_DATA[36].Pos("B")!=0) ) {
                   if( P_PARTS[4] == "ＳＵＳ３０４／Ｃ３６０４" ) {
                       //BﾌﾚｰﾑSUS304/C3604の場合Crﾒｯｷ追加
                       Excel_Cell_DataSet(P_Row, 24, "ＳＵＳ３０４／Ｃ３６０４ Crﾒｯｷ");
                   } else if( P_PARTS[4] == "ＳＵＳ３１６／Ｃ３６０４" ) {
                       //BﾌﾚｰﾑSUS316/C3604の場合Crﾒｯｷ追加
                       Excel_Cell_DataSet(P_Row, 24, "ＳＵＳ３１６／Ｃ３６０４ Crﾒｯｷ");
                   }
               }
               //***********************
               Excel_Cell_DataSet(P_Row, 36, P_PARTS[5]);  // 図番

               Excel_Cell_DataSet(P_Row, 47, P_PARTS[10]); // 仕様1
               if ( P_PARTS[11].Length() > 0 ) {
                 Excel_Cell_DataSet(P_Row, 58, P_PARTS[11]); // 仕様2
               }
               //2014.12.11 ﾎﾞﾙﾄ材質変更
               if( ( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) )
                  && ( P_PARTS[11] == "ヘヤーライン" ) ) {
                  //ﾍﾔｰﾗｲﾝの場合仕様2変更
                  Excel_Cell_DataSet(P_Row, 58, "締付Ｂ頭部はヘヤーライン");
               }
               //***********************
               Excel_Cell_DataSet(P_Row, 71, P_PARTS[7]);  // 1台分数量
               Excel_Cell_DataSet(P_Row, 74, P_PARTS[8]);  // 予備数
               Excel_Cell_DataSet(P_Row, 78, P_PARTS[9]);  // 総計

//               if( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) )
                  Excel_Cell_DataSet(P_Row, 79, P_PARTS[13]); // 単位（式）

               if( P_compflg == 1 )
               {
                  if( !Compare_Data( P_PARTS ) )
                     Excel_Cell_DataSet(P_Row, 80, "*");      // 比較
               }
               P_Row++;
               if( ( P_PARTS[1] == "BNWR--" )||( P_PARTS[1] == "BOLTNR" ) ) {
                   iBLTCV=1;
               }
            }

         }
      }

      //****************************************************************************
      //***                                                                      ***
      //***                           ノズル部品出力                             ***
      //***                                                                      ***
      //****************************************************************************
      else if( P_PARTS[0] == "NOZZLE" )
      {
         if ( iBLTCV == 1 ) {
            iBLTCV=2;
            BLT_Cover_Out();
            // 2014/08/25 応用技術編集
            if(P_HEADS_DATA[36].Pos("Z") > 0) {
                BLT_Cover_Out_Small();
            }
         }
         if( i_nozzlflg == 0 )
         {
            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "ノズル部品");

            P_Row++;
            i_nozzlflg++;
            P_WkRec = 0;
         }

         //2003.12.22
         //Buhin_Syukei2( P_PARTS );
         Buhin_Syukei4( P_PARTS );
         //**********
         b_YobiSumMode = true;
      }

      //---------------------------------------------------
      //  B側温度計ﾃﾞｰﾀ部出力
      //---------------------------------------------------
      else if( P_PARTS[0] == "B_TEMP" )
      {
         if( i_Btempflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 ミラー仕様の場合、B側液名を出力
            //Excel_Cell_DataSet(P_Row, 3, "Ｂ側温度計");
            s_Text = ConvertRyutaiName("B", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "側温度計");
            //-----------------------------------------------

            P_Row++;
            i_Btempflg++;
            P_WkRec = 0;
         }

         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  A側温度計ﾃﾞｰﾀ部出力
      //---------------------------------------------------
      else if( P_PARTS[0] == "A_TEMP" )
      {
         if( i_Atempflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 ミラー仕様の場合、A側液名を出力
            //Excel_Cell_DataSet(P_Row, 3, "Ａ側温度計");
            s_Text = ConvertRyutaiName("A", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "側温度計");
            //-----------------------------------------------

            P_Row++;
            i_Atempflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;

      }

      //---------------------------------------------------
      //  B側圧力計ﾃﾞｰﾀ部出力
      //---------------------------------------------------
      else if( P_PARTS[0] == "B_PRSG" )
      {
         if( i_Bpresflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 ミラー仕様の場合、B側液名を出力
            //Excel_Cell_DataSet(P_Row, 3, "Ｂ側圧力計");
            s_Text = ConvertRyutaiName("B", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "側圧力計");
            //-----------------------------------------------

            P_Row++;
            i_Bpresflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  A側圧力計ﾃﾞｰﾀ部出力
      //---------------------------------------------------
      else if( P_PARTS[0] == "A_PRSG" )
      {
         if( i_Apresflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }

            //-----------------------------------------------
            // 2008.03.19 ミラー仕様の場合、A側液名を出力
            //Excel_Cell_DataSet(P_Row, 3, "Ａ側圧力計");
            s_Text = ConvertRyutaiName("A", m_pcHeadsData->GetMirrorFlag());
            Excel_Cell_DataSet(P_Row, 3, s_Text + "側圧力計");
            //-----------------------------------------------

            P_Row++;
            i_Apresflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  付属品ﾃﾞｰﾀ部出力
      //---------------------------------------------------
      else if( P_PARTS[0] == "ACCS" )
      {
         if( i_accsflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            //2019.10.10 付属品ﾃﾞｰﾀ出力行数変更_S
            //if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {
            //   // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - ( P_ACCS_Num + 3 ) ) ) {
               // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行
               // ( P_ACCS_Num + 3 ):予想される出力行数
            //2019.10.10 付属品ﾃﾞｰﾀ出力行数変更_E
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "付属品");
            //G_Log_Text = "型式                  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
            //Write_Log(G_Log_Text);
            P_Row++;
            i_accsflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }

      //---------------------------------------------------
      //  舶用部品ﾃﾞｰﾀ部出力
      //---------------------------------------------------
      else if( P_PARTS[0] == "SHIP" )
      {
         if( i_shipflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            //2003.11.13 改ページ条件変更
            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - ( P_SHIP_Num + 3 ) ) ) {
              // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　
              // ( P_SHIP_Num + 3 ):予想される出力行数
            //if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {  // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
            //***************************
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }
            Excel_Cell_DataSet(P_Row, 3, "舶用部品");
            P_Row++;
            i_shipflg++;
            P_WkRec = 0;
         }
         Buhin_Syukei( P_PARTS );
         b_YobiSumMode = false;
      }
      //---------------------------------------------------
      //  エラー出力
      //---------------------------------------------------
      else if( P_PARTS[0] == "ERROR" )
      {
        //----------------------------------
        // 2007.11.14 コメントアウト
        /*
         //2005-10-07 展開できなかった予備品名を出力できるように変更
         //Excel_Cell_DataSet(112, 2,  P_PARTS[2]);  // 部品１(部品型式)
         int iTmp;
         iTmp = i_Base_Y + i_ErrItemCnt;
         Excel_Cell_DataSet(iTmp, 2,  P_PARTS[2]);  // 部品１(部品型式)
         Excel_Cell_DataSet(iTmp, 74,  P_PARTS[8]);  // 予備品数
         i_ErrItemCnt++;
        */
        // 2007.11.14 コメントアウト
        //----------------------------------

        //----------------------------------
        // 2007.11.14 追加
         if( i_errflg == 0 )
         {
            // ﾜｰｸ格納ﾃﾞｰﾀあり?
            if( P_WkRec > 0 )
               Output_WK_PARTS(b_YobiSumMode);

            if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {  // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
               P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
               P_PageNum++;
            } else {
            // 改行
               P_Row++;
               Write_BorderLine();
            }

            i_errflg++;
            P_WkRec = 0;
         }

         Excel_Cell_DataSet(P_Row, 2,  P_PARTS[2]);  // 部品１(部品型式)
         Excel_Cell_DataSet(P_Row, 74,  P_PARTS[8]);  // 予備品数
         P_Row++;
        // 2007.11.14 追加
        //----------------------------------

      }

      RecNo++;
   }

   // ﾜｰｸ格納ﾃﾞｰﾀあり?
   if ( P_WkRec > 0 ){
      Output_WK_PARTS(b_YobiSumMode);
   }

   fclose(fp);    // 閉じる
   return true;
}
//---------------------------------------------------------------------------
// 日本語関数名： 前回部品ﾃﾞｰﾀ取得処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 前回部品展開ﾃﾞｰﾀを確保する
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Prev_Data_Get( void )
{
   char  buf[1024];                            /* 文字列バッファ   */
   char  *tok;                                 /* トークンポインタ */
   int   i, j;                                 /* ループカウンタ   */
   char  token[100][100];                      /* トークンバッファ */

   AnsiString wk_FilePass;  //
   AnsiString wk_RevPass;   // 前回部品展開ﾃﾞｰﾀﾊﾟｽ
   //AnsiString P_PREV_PARTS[15]; // 前回部品ﾃﾞｰﾀ

//   struct ffblk ffblk;
//   int done;
   int i_rev = -1;
   AnsiString sRevNo;

   //2023.03.02 製造番号追加_S
   AnsiString  Work,Rev;
   AnsiString xls_Output_Dir_old;  // 出力ディレクトリ
   int i_Sakuban;
   //2023.03.02 製造番号追加_E

   //------------------------------
   // 2007.10.15
   bool bRet;
   AnsiString s_wkPARTS[1000][17];
   // 2007.10.15
   //------------------------------

   //***************************
   //**  前回部品展開ﾃﾞｰﾀ読込
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      //マルチサーバ対応 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN);
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN);
      //マルチサーバ対応 2009/1/13 MOD END
   } else {
      //マルチサーバ対応 2009/1/13 MOD START
      //wk_FilePass = IncludeTrailingBackslash(G_HD_Root)
      //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //            + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //            + IncludeTrailingBackslash(G_SAKUBAN);
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN);
      //マルチサーバ対応 2009/1/13 MOD END
   }
   //2023.03.02 製造番号追加_S
   //2004.11.16 出力フォルダ及び訂正印変更
   ////sRevNo = Csv_Exist_Check(wk_FilePass);
   //sRevNo = Csv_Exist_Check(G_xls_Output_Dir_old);
   ////*************************************

    //********************************
    //**  出力ディレクトリ
    //********************************
    if (G_SAKUBAN.Length() > 7) {
        Work = G_SAKUBAN.SubString(1, 6);
    } else {
        Work = G_SAKUBAN.SubString(1, 4);
    }
    i_Sakuban = Work.ToIntDef(0);
    if( i_Sakuban % 2 == 1 ){
        i_Sakuban --;
    }
    if (i_Sakuban > 9999) {
        Work.sprintf("%06d",i_Sakuban);
    } else {
        Work.sprintf("%04d",i_Sakuban);
    }

    if (G_HEADS_DATA_Rev - 1 <= 0 ) {
        Rev.sprintf("%02d", 0);
    }else{
        Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
    }

	//2023/4/21 製造番号桁数追加対応 Mod
	//xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_DATA_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\";
	xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_DATA_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + GetFileSeizouBangou(G_SAKUBAN) + "\\";

    sRevNo = Csv_Exist_Check(xls_Output_Dir_old);
   //2023.03.02 製造番号追加_E
//   wk_RevPass = wk_FilePass + "*";

//   //****************************
//   //*** 前回部品展開データ検索
//   //****************************
//   done = findfirst( wk_RevPass.c_str(), &ffblk, FA_DIREC);
//   while (!done)
//   {
//      AnsiString wkFileName;
//
//      wkFileName = ffblk.ff_name;
//      if( wkFileName.SubString( 4, 2 ).ToIntDef(0) > i_rev )
//         i_rev = wkFileName.SubString( 4, 2 ).ToIntDef(0);
//
//      done = findnext(&ffblk);
//   }


   //------------------------------------
   // 2007.10.15 ﾐﾗｰ仕様対応 共通関数化

/*   //****************************
   //*** 前回部品展開データ読込
   //****************************
//   if( i_rev != -1 )
   if ( sRevNo != "" )
   {
//      wk_FilePass = wk_FilePass + FormatFloat( "0#", i_rev )
      wk_FilePass = wk_FilePass + sRevNo + "\\HD_PARTS.csv";

      if ((wkfp = fopen(wk_FilePass.c_str(),"r")) == NULL){
         // ｴﾗｰﾛｸﾞ作成
         G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
         Write_Error_Log(G_ErrLog_Text);
         fclose(wkfp);    // 閉じる
         return false;
      }

      memset(buf,0x00,sizeof(buf));
      P_PrevRec = 0;
      while(fgets(buf,sizeof(buf),wkfp)!=NULL)
      {
         i = 0;                                 // トークンカウンタ初期化
         memset(token,0x00,sizeof(token));      // トークンバッファ初期化
         tok = GetToken(buf,",");               // ","で項目を区切る
         while(tok)
         {                            // トークンが存在する間
            sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
            tok = GetToken(NULL,",");           // 次のトークンを検索

            if (i>100) {
               break;
            }
         }

         //*****************************
         // 前回部品データの取得
         // (型式、識別子以外全角)
         //*****************************
         for( j=0; j<16; j++ )
         {
            P_PREV_PARTS[P_PrevRec][j] = token[j];
            if( j > 1 )
               P_PREV_PARTS[P_PrevRec][j] = HanToZen(P_PREV_PARTS[P_PrevRec][j]);

            //******************************************
            // 配列の最後に読込みﾌﾗｸﾞ用ﾃﾞｰﾀをセットする
            //******************************************
            if( j == 15 )
               P_PREV_PARTS[P_PrevRec][j] = "0";
         }
         P_PrevRec++;
      }
   }
   else
   {
      return false;
   }

   return true;
*/

    if ( sRevNo != "" ){

        wk_FilePass = wk_FilePass + sRevNo + "\\HD_PARTS.csv";

        //****************************
        //*** 前回部品展開データ読込
        //****************************
        bRet = Read_Buhin_Data_Sorted(wk_FilePass, s_wkPARTS);
        if(!bRet) return(false);

        P_PrevRec = 0;

        for( i = 0; i < 1000; i++ ){
            if( s_wkPARTS[i][0] != "" ){
                for( j = 0; j < 17; j++ ){
                    // 識別子以外は全角変換
                    if( j > 1 ){
                        P_PREV_PARTS[i][j] = HanToZen(s_wkPARTS[i][j]);
                    }else{
                        P_PREV_PARTS[i][j] = s_wkPARTS[i][j];
                    }
                }

                //******************************************
                // 配列の最後に読込みﾌﾗｸﾞ用ﾃﾞｰﾀをセットする
                //******************************************
                P_PREV_PARTS[i][17] = "0";

                // ﾃﾞｰﾀ行ｶｳﾝﾄ
                P_PrevRec++;
            }else{
                break;
            }
        }
    }else{
        return(false);
    }

    return(true);

   // 2007.10.15 ﾐﾗｰ仕様対応
   //------------------------------------
}

//---------------------------------------------------------------------------
// 日本語関数名： 旧(６桁)前回部品ﾃﾞｰﾀ取得処理
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： ８桁で入力された６桁時代の作番について
//                ６桁作番の前回部品展開ﾃﾞｰﾀを確保する
//                2023.02.08 作番桁数追加対応 (新規作成)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Prev_Old_Data_Get( void )
{
    char  buf[1024];                            /* 文字列バッファ   */
    char  *tok;                                 /* トークンポインタ */
    int   i, j;                                 /* ループカウンタ   */
    char  token[100][100];                      /* トークンバッファ */

    AnsiString wk_FilePass;  //
    AnsiString wk_RevPass;   // 前回部品展開ﾃﾞｰﾀﾊﾟｽ

    int i_rev = -1;
    AnsiString sRevNo;

    bool bRet;
    AnsiString s_wkPARTS[1000][17];

    AnsiString OLD_SAKUBAN;  // ６桁時代作番
    AnsiString  Work,Rev;
    AnsiString xls_Output_Dir_old;  // ６桁時代出力ディレクトリ
    int i_Sakuban;

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

    xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_DATA_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + OLD_SAKUBAN + "\\";

    //***************************
    //**  前回部品展開ﾃﾞｰﾀ読込
    //***************************
    wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                + IncludeTrailingBackslash(G_SAKUBAN);

    sRevNo = Csv_Exist_Check(xls_Output_Dir_old);

    if ( sRevNo != "" ){

        wk_FilePass = wk_FilePass + sRevNo + "\\HD_PARTS.csv";

        //****************************
        //*** 前回部品展開データ読込
        //****************************
        bRet = Read_Buhin_Data_Sorted(wk_FilePass, s_wkPARTS);
        if(!bRet) return(false);

        P_PrevRec = 0;

        for( i = 0; i < 1000; i++ ){
            if( s_wkPARTS[i][0] != "" ){
                for( j = 0; j < 17; j++ ){
                    // 識別子以外は全角変換
                    if( j > 1 ){
                        P_PREV_PARTS[i][j] = HanToZen(s_wkPARTS[i][j]);
                    }else{
                        P_PREV_PARTS[i][j] = s_wkPARTS[i][j];
                    }
                }

                //******************************************
                // 配列の最後に読込みﾌﾗｸﾞ用ﾃﾞｰﾀをセットする
                //******************************************
                P_PREV_PARTS[i][17] = "0";

                // ﾃﾞｰﾀ行ｶｳﾝﾄ
                P_PrevRec++;
            }else{
                break;
            }
        }
    }else{
        return(false);
    }

    return(true);


}

//---------------------------------------------------------------------------
// 日本語関数名： 部品比較処理
// 概  要      ：
// 引  数      ： AnsiString P_PARTS ： 比較対象データ
// 戻り値      ： 比較結果 [true:相違なし false:相違あり]
// 備  考      ： 前回部品展開データと今回のデータを比較する
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Compare_Data( AnsiString *P_PARTS )
{
   int i;   // ﾙｰﾌﾟｶｳﾝﾀ
   bool bFound;

   bFound = false;
   for( i=0; i<P_PrevRec; i++ )
   {
      //*****************************************************************
      // 前回部品ﾃﾞｰﾀと今回部品ﾃﾞｰﾀの部品分類[1列目]+識別子[2列目]一致?
      //*****************************************************************
      //-----------------------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応
      //  ﾐﾗｰ仕様ｺｰﾄﾞ[17列目]も比較対象に追加

//      if( ( P_PARTS[0] == P_PREV_PARTS[i][0] )&&( P_PARTS[1] == P_PREV_PARTS[i][1] )
//        &&( P_PREV_PARTS[i][15] == "0" ) )
      if( ( P_PARTS[0] == P_PREV_PARTS[i][0] )&&( P_PARTS[1] == P_PREV_PARTS[i][1] )
        &&( P_PREV_PARTS[i][17] == "0" )&&( P_PARTS[16] == P_PREV_PARTS[i][16] ) )

      // 2007.10.12
      //-----------------------------------------
      {
         bFound = true;
         //**************************
         // 該当ﾚｺｰﾄﾞを読込済みにｾｯﾄ
         //**************************
         //----------------------------
         // 2007.10.15
         //P_PREV_PARTS[i][15] = "1" ;
         P_PREV_PARTS[i][17] = "1" ;
         // 2007.10.15
         //----------------------------

         if ( P_PREV_PARTS[i][7] == "０" ) P_PREV_PARTS[i][7] = "";
         if( P_PARTS[2]  != P_PREV_PARTS[i][2] ) return false;        // 部品型式
         else if( P_PARTS[3]  != P_PREV_PARTS[i][3] ) return false;   // 部品名称
         else if( P_PARTS[4]  != P_PREV_PARTS[i][4] ) return false;   // 材質名称
         else if( P_PARTS[5]  != P_PREV_PARTS[i][5] ) return false;   // 図番
         else if( P_PARTS[6]  != P_PREV_PARTS[i][6] ) return false;   // 作図用図番
         else if( P_PARTS[7]  != P_PREV_PARTS[i][7] ) return false;   // 1台分数量
         else if( P_PARTS[10] != P_PREV_PARTS[i][10] ) return false;  // 仕様1
         else if( P_PARTS[11] != P_PREV_PARTS[i][11] ) return false;  // 仕様2
         else return true;
      }
   }
   if ( bFound == false ) return false;

   return true;
}


//---------------------------------------------------------------------------
// 日本語関数名： ﾜｰｸ部品ﾃﾞｰﾀ出力処理
// 概  要      ： ﾜｰｸ部品配列に格納されたﾃﾞｰﾀを出力する
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Output_WK_PARTS(bool bDataTyp)
{
   int i, j;
   AnsiString a_Bunrui, a_Zaisitu;
   AnsiString s_Text;
   bool b_GGPL;

   for( i=0; i<P_WkRec; i++)
   {
       b_GGPL = false;
      if( P_WK_PARTS[i][0] != "\0" )
      {
         a_Bunrui = P_WK_PARTS[i][0];
         a_Zaisitu = P_WK_PARTS[i][1];

         Excel_Cell_DataSet(P_Row, 4,  P_WK_PARTS[i][2]);  // 部品１(部品型式)
         if ( P_WK_PARTS[i][3].TrimRight() != "" )
           Excel_Cell_DataSet(P_Row, 13, P_WK_PARTS[i][3]);  // 部品２(部品名称)
         Excel_Cell_DataSet(P_Row, 24, P_WK_PARTS[i][4]);  // 材質名称
         Excel_Cell_DataSet(P_Row, 36, P_WK_PARTS[i][5]);  // 図番

         Excel_Cell_DataSet(P_Row, 47, P_WK_PARTS[i][10]); // 仕様1
         if ( P_WK_PARTS[i][11].Length() > 0 ) {
           if( P_WK_PARTS[i][1] == "GGPL--" ) {
             b_GGPL = true;
           } else {
             Excel_Cell_DataSet(P_Row, 68, P_WK_PARTS[i][11]); // 仕様2
           }
         }

         if ( P_WK_PARTS[i][7] == "0" || P_WK_PARTS[i][7] == "０" ) {
            Excel_Cell_DataSet(P_Row, 71, "");  // 1台分数量
         } else {
            Excel_Cell_DataSet(P_Row, 71, P_WK_PARTS[i][7]);  // 1台分数量
         }
         if ( P_WK_PARTS[i][8] == "0" || P_WK_PARTS[i][8] == "０" ) {
            Excel_Cell_DataSet(P_Row, 74, "");  // 予備数
         } else {
            Excel_Cell_DataSet(P_Row, 74, P_WK_PARTS[i][8]);  // 予備数
         }
         Excel_Cell_DataSet(P_Row, 78, P_WK_PARTS[i][9]);  // 総計
         Excel_Cell_DataSet(P_Row, 79, P_WK_PARTS[i][13]); // 単位（式）
         if( P_compflg == 1 )
         {
            if( bDataTyp == false ) {
               if( !Compare_PrevData( P_WK_PARTS[i] ) )
                  Excel_Cell_DataSet(P_Row, 80, "*"); // 前回ﾃﾞｰﾀ比較

            } else {
               if( !Compare_PrevData2( P_WK_PARTS[i] ) )
                  Excel_Cell_DataSet(P_Row, 80, "*"); // 前回ﾃﾞｰﾀ比較
            }
         }

         //----------------------------------------
         // 2007.10.12 ﾐﾗｰ仕様対応 号機名称出力
         // 2007.11.02 追加仕様 "EPGS-R"(Eﾌﾟﾚｰﾄ裏当て板)を追加
         if ( P_WK_PARTS[i][16] != "" ) {
            if( ( P_WK_PARTS[i][1] == "PLHL--" )
             || ( P_WK_PARTS[i][1] == "DPLG-A" )
             || ( P_WK_PARTS[i][1] == "DPLG-B" )
             || ( P_WK_PARTS[i][1] == "DPLGDA" )
             || ( P_WK_PARTS[i][1] == "DPLGDB" )
             || ( P_WK_PARTS[i][1] == "EPLGDA" )
             || ( P_WK_PARTS[i][1] == "EPLGDB" )
             || ( P_WK_PARTS[i][1] == "EPGS-R" )
             || ( P_WK_PARTS[i][1] == "FRAM-E" ) ){
                Excel_Cell_DataSet(P_Row, 66, P_WK_PARTS[i][16] + "号機");  // 号機名称
            }
         }
         // 2007.10.12
         //----------------------------------------

         P_Row++;
         if ( b_GGPL == true ) {  // ゲージプレート「テスト後打刻」を下段に印字
           Excel_Cell_DataSet(P_Row, 68, P_WK_PARTS[i][11]); // 仕様2
           P_Row++;
         }
      }

      //*************************
      // 格納後ﾜｰｸﾃﾞｰﾀをｸﾘｱする
      //*************************
      //----------------------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応
      //for ( j=0; j<15; j++ )
      for ( j=0; j<17; j++ )
      // 2007.10.12
      //----------------------------------------
         P_WK_PARTS[i][j]="";
   }

   //if( ( a_Bunrui == "NOZZLE" )&&( a_Zaisitu == "TANKAN" ) )   02.09.27変更
   if( ( a_Bunrui == "NOZZLE" )&&( a_Zaisitu == "FRNG-" ) )
   {
      // 注記(端管位置)
      s_Text = HanToZen( P_HEADS_DATA[1335] );
      Excel_Cell_DataSet(P_Row, 47, s_Text);
      P_Row++;
   }

   P_WkRec = 0;
}

//---------------------------------------------------------------------------
// 日本語関数名： ﾜｰｸ部品ﾃﾞｰﾀ出力処理
// 概  要      ： ﾜｰｸ部品配列に格納されたﾃﾞｰﾀを出力する
// 引  数      ： なし
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Output_WK_PARTS3(void)
{
   int i, j;
   AnsiString a_Bunrui, a_Zaisitu;
   AnsiString s_Text;

   for( i=0; i<P_WkRec3; i++)
   {
      if( P_WK_PARTS3[i][0] != "\0" )
      {
         a_Bunrui = P_WK_PARTS3[i][0];
         a_Zaisitu = P_WK_PARTS3[i][1];

         Excel_Cell_DataSet(P_Row, 4,  P_WK_PARTS3[i][2]);  // 部品１(部品型式)
         if ( P_WK_PARTS3[i][3].TrimRight() != "" )
           Excel_Cell_DataSet(P_Row, 13, P_WK_PARTS3[i][3]);  // 部品２(部品名称)
         Excel_Cell_DataSet(P_Row, 24, P_WK_PARTS3[i][4]);  // 材質名称
         Excel_Cell_DataSet(P_Row, 36, P_WK_PARTS3[i][5]);  // 図番

         Excel_Cell_DataSet(P_Row, 47, P_WK_PARTS3[i][10]); // 仕様1
         if ( P_WK_PARTS3[i][11].Length() > 0 ) {
           Excel_Cell_DataSet(P_Row, 68, P_WK_PARTS3[i][11]); // 仕様2
         }

         if (P_WK_PARTS3[i][7] == "0" || P_WK_PARTS3[i][7] == "０" ) {
            Excel_Cell_DataSet(P_Row, 71, "");  // 1台分数量
         } else {
            Excel_Cell_DataSet(P_Row, 71, P_WK_PARTS3[i][7]);  // 1台分数量
         }
         if (P_WK_PARTS3[i][8] == "0" || P_WK_PARTS3[i][8] == "０" ) {
            Excel_Cell_DataSet(P_Row, 74, "");  // 予備数
         } else {
            Excel_Cell_DataSet(P_Row, 74, P_WK_PARTS3[i][8]);  // 予備数
         }
         Excel_Cell_DataSet(P_Row, 78, P_WK_PARTS3[i][9]);  // 総計
         Excel_Cell_DataSet(P_Row, 79, P_WK_PARTS3[i][13]); // 単位（式）
         if( P_compflg == 1 )
         {
           if( !Compare_PrevData2( P_WK_PARTS3[i] ) )
              Excel_Cell_DataSet(P_Row, 80, "*"); // 前回ﾃﾞｰﾀ比較
         }

         //------------------------------------------
         // 2007.10.12 ﾐﾗｰ仕様対応 号機名称追加
         // 2007.11.02 追加仕様 "EPGS-R"(Eﾌﾟﾚｰﾄ裏当て板)追加
         if (P_WK_PARTS3[i][16] != "" ) {
            if( ( P_WK_PARTS3[i][1] == "PLHL--" )
             || ( P_WK_PARTS3[i][1] == "DPLG-A" )
             || ( P_WK_PARTS3[i][1] == "DPLG-B" )
             || ( P_WK_PARTS3[i][1] == "DPLGDA" )
             || ( P_WK_PARTS3[i][1] == "DPLGDB" )
             || ( P_WK_PARTS3[i][1] == "EPLGDA" )
             || ( P_WK_PARTS3[i][1] == "EPLGDB" )
             || ( P_WK_PARTS3[i][1] == "EPGS-R" )
             || ( P_WK_PARTS3[i][1] == "FRAM-E" ) ){
                Excel_Cell_DataSet(P_Row, 66, P_WK_PARTS3[i][16] + "号機");  // 号機名称
            }
         }
         // 2007.10.12
         //------------------------------------------

         P_Row++;
      }

      //*************************
      // 格納後ﾜｰｸﾃﾞｰﾀをｸﾘｱする
      //*************************
      //--------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応
      //for ( j=0; j<15; j++ )
      for ( j=0; j<17; j++ )
      // 2007.10.12
      //--------------------------
         P_WK_PARTS3[i][j]="";
   }

   P_WkRec3 = 0;
}

//---------------------------------------------------------------------------
// 日本語関数名： 部品ﾃﾞｰﾀ集計処理
// 概  要      ： 同一部品の数量、予備数、総計を集計する
// 引  数      ： AnsiString *P_PARTS ：集計対象ﾚｺｰﾄﾞ
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   //****************************************
   // 既に格納されている部品ﾃﾞｰﾀと比較
   //****************************************
   for( i=0; i<P_WkRec; i++ )
   {
      //03.12.22 比較条件変更
      //if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[3] == P_WK_PARTS[i][3] )
      //  &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
      //  &&( P_PARTS[12] == P_WK_PARTS[i][12] )&&( P_PARTS[13] == P_WK_PARTS[i][13] ) )
      //{
      //---------------------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応 ﾐﾗｰ仕様ｺｰﾄﾞ[16]も比較
      //if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[3] == P_WK_PARTS[i][3] )
      //  &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
      //  &&( P_PARTS[10] == P_WK_PARTS[i][10] )&&( P_PARTS[11] == P_WK_PARTS[i][11] )
      //  &&( P_PARTS[12] == P_WK_PARTS[i][12] )&&( P_PARTS[13] == P_WK_PARTS[i][13] ) )
      //{
      if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[3] == P_WK_PARTS[i][3] )
        &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
        &&( P_PARTS[10] == P_WK_PARTS[i][10] )&&( P_PARTS[11] == P_WK_PARTS[i][11] )
        &&( P_PARTS[12] == P_WK_PARTS[i][12] )&&( P_PARTS[13] == P_WK_PARTS[i][13] )
        &&( P_PARTS[16] == P_WK_PARTS[i][16] ) )
      {
      // 2007.10.12
      //---------------------------------------

      //*********************
         // １台分数量 集計
         P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
                                              + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
         // 予備数 集計
         P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
                                              + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
         // 総計
         P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
                                              + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
         i_matchflg = 1;

      }
   }

   //***************************************
   // 該当検索結果なしの場合ﾜｰｸ部品ﾃﾞｰﾀに格納
   //***************************************
   if( i_matchflg == 0 )
   {
      //----------------------------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応 ﾐﾗｰ仕様ｺｰﾄﾞ[16]も格納
      //for( j=0; j<15; j++ )
      for( j=0; j<17; j++ )
      // 2007.10.12
      //----------------------------------------------
         P_WK_PARTS[P_WkRec][j] = P_PARTS[j];
   }

   //i_matchflg = 0;
   P_WkRec++;
}

//↓2002/10/24
//---------------------------------------------------------------------------
// 日本語関数名： 部品ﾃﾞｰﾀ集計処理2
// 概  要      ： 同一部品の数量、予備数、総計を集計する
// 引  数      ： AnsiString *P_PARTS ：集計対象ﾚｺｰﾄﾞ
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei2( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   if( ZenToHan( P_PARTS[7] )!= "" ) {
     //****************************************
     // 既に格納されている部品ﾃﾞｰﾀと比較
     //****************************************
     for( i=0; i<P_WkRec; i++ )
     {
     //-------------------------------------------------
     // 2007.10.12 ﾐﾗｰ仕様対応 ﾐﾗｰ仕様ｺｰﾄﾞ[16]も比較
       //if( ( P_PARTS[1] == P_WK_PARTS[i][1] ) // &&( P_PARTS[2] == P_WK_PARTS[i][2] )
       //  &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] ) )
       if( ( P_PARTS[1] == P_WK_PARTS[i][1] ) // &&( P_PARTS[2] == P_WK_PARTS[i][2] )
         &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
         &&( P_PARTS[16] == P_WK_PARTS[i][16] ))
     // 2007.10.12
     //-------------------------------------------------
       {
          // １台分数量 集計
          P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
          // 予備数 集計
          P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
          // 総計
          P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
          P_WK_PARTS[i][13] = P_PARTS[13];
          i_matchflg = 1;

       }
     }
     //***************************************
     // 該当検索結果なしの場合ﾜｰｸ部品ﾃﾞｰﾀに格納
     //***************************************
     if( i_matchflg == 0 )
     {
        //----------------------------------------------
        // 2007.10.12 ﾐﾗｰ仕様対応 ﾐﾗｰ仕様ｺｰﾄﾞ[16]も格納
        //for( j=0; j<15; j++ )
        for( j=0; j<17; j++ )
        // 2007.10.12
        //----------------------------------------------
           P_WK_PARTS[P_WkRec][j] = P_PARTS[j];
     }

     //i_matchflg = 0;
     P_WkRec++;
   }
}

//---------------------------------------------------------------------------
// 日本語関数名： 部品ﾃﾞｰﾀ集計処理3
// 概  要      ： 同一部品の数量、予備数、総計を集計する
// 引  数      ： AnsiString *P_PARTS ：集計対象ﾚｺｰﾄﾞ
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei3( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   if( ZenToHan( P_PARTS[7] )!= "" ) {
     //****************************************
     // 既に格納されている部品ﾃﾞｰﾀと比較
     //****************************************
     for( i=0; i<P_WkRec3; i++ )
     {
       if( ( P_PARTS[1] == P_WK_PARTS3[i][1] ) // &&( P_PARTS[2] == P_WK_PARTS[i][2] )
         &&( P_PARTS[4] == P_WK_PARTS3[i][4] )&&( P_PARTS[5] == P_WK_PARTS3[i][5] ) )
       {
          // １台分数量 集計
          P_WK_PARTS3[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS3[i][7]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
          // 予備数 集計
          P_WK_PARTS3[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS3[i][8]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
          // 総計
          P_WK_PARTS3[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS3[i][9]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
          P_WK_PARTS3[i][13] = P_PARTS[13];
          i_matchflg = 1;

       }
     }
     //***************************************
     // 該当検索結果なしの場合ﾜｰｸ部品ﾃﾞｰﾀに格納
     //***************************************
     if( i_matchflg == 0 )
     {
        //---------------------------
        // 2007.10.15 ﾐﾗｰ仕様対応
        //for( j=0; j<15; j++ )
        for( j=0; j<17; j++ )
        // 2007.10.15
        //---------------------------
           P_WK_PARTS3[P_WkRec3][j] = P_PARTS[j];
     }

     //i_matchflg = 0;
     P_WkRec3++;
   }
}

//2003.12.22 関数追加
//---------------------------------------------------------------------------
// 日本語関数名： 部品ﾃﾞｰﾀ集計処理4
// 概  要      ： 同一部品の数量、予備数、総計を集計する
// 引  数      ： AnsiString *P_PARTS ：集計対象ﾚｺｰﾄﾞ
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Buhin_Syukei4( AnsiString *P_PARTS )
{
   int i, j;
   int i_matchflg =0;

   if( ZenToHan( P_PARTS[7] )!= "" ) {
     //****************************************
     // 既に格納されている部品ﾃﾞｰﾀと比較
     //****************************************
     for( i=0; i<P_WkRec; i++ )
     {
       // 2017.01.12 端管集計変更_S
       //if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[4] == P_WK_PARTS[i][4] )
       //  &&( P_PARTS[5] == P_WK_PARTS[i][5] )&&( P_PARTS[10] == P_WK_PARTS[i][10] )
       //  &&( P_PARTS[11] == P_WK_PARTS[i][11] ) )
       //{
       //   // １台分数量 集計
       //   P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
       //                                        + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
       //   // 予備数 集計
       //   P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
       //                                        + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
       //   // 総計
       //   P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
       //                                        + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
       //   P_WK_PARTS[i][13] = P_PARTS[13];
       //   i_matchflg = 1;
       //
       //}
       if( ( P_PARTS[1] == P_WK_PARTS[i][1] )&&( P_PARTS[2] == P_WK_PARTS[i][2] )
         &&( P_PARTS[4] == P_WK_PARTS[i][4] )&&( P_PARTS[5] == P_WK_PARTS[i][5] )
         &&( P_PARTS[10] == P_WK_PARTS[i][10] )&&( P_PARTS[11] == P_WK_PARTS[i][11] ) )
       {
          // １台分数量 集計
          P_WK_PARTS[i][7] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][7]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[7]).ToIntDef(0) ) );
          // 予備数 集計
          P_WK_PARTS[i][8] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][8]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[8]).ToIntDef(0) ) );
          // 総計
          P_WK_PARTS[i][9] = HanToZen( IntToStr( ZenToHan(P_WK_PARTS[i][9]).ToIntDef(0)
                                               + ZenToHan(P_PARTS[9]).ToIntDef(0) ) );
          P_WK_PARTS[i][13] = P_PARTS[13];
          i_matchflg = 1;

       }
       // 2017.01.12 端管集計変更_E
     }
     //***************************************
     // 該当検索結果なしの場合ﾜｰｸ部品ﾃﾞｰﾀに格納
     //***************************************
     if( i_matchflg == 0 )
     {
        //---------------------------
        // 2007.10.15 ﾐﾗｰ仕様対応
        //for( j=0; j<15; j++ )
        for( j=0; j<17; j++ )
        // 2007.10.15
        //---------------------------
           P_WK_PARTS[P_WkRec][j] = P_PARTS[j];
     }

     //i_matchflg = 0;
     P_WkRec++;
   }
}
//*******************

//---------------------------------------------------------------------------
// 日本語関数名： 部品比較処理
// 概  要      ：
// 引  数      ： AnsiString P_PARTS ： 比較対象データ
// 戻り値      ： 比較結果 [true:相違なし false:相違あり]
// 備  考      ： 前回部品展開データと今回のデータを比較する  集計タイプデータ用
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Compare_PrevData( AnsiString *P_PARTS )
{
   int i, j;
   //AnsiString P_WKPre[16];
   AnsiString P_WKPre[18];

   //****************************************
   // 既に格納されている部品ﾃﾞｰﾀと比較
   //****************************************
   for( i=0; i<P_PrevRec; i++ )
   {
      //-----------------------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応
      //  ﾐﾗｰ仕様ｺｰﾄﾞ[17列目]も比較対象に追加

      //if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )&&( P_PARTS[3]  == P_PREV_PARTS[i][3]  )
      //  &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  )
      //  &&( P_PARTS[12] == P_PREV_PARTS[i][12] )&&( P_PARTS[13] == P_PREV_PARTS[i][13] ) )

      if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )&&( P_PARTS[3]  == P_PREV_PARTS[i][3]  )
        &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  )
        &&( P_PARTS[12] == P_PREV_PARTS[i][12] )&&( P_PARTS[13] == P_PREV_PARTS[i][13] )&&( P_PARTS[16] == P_PREV_PARTS[i][16] ))
      // 2007.10.12
      //-----------------------------------------
      {
         //--------------------------
         // 2007.10.15
         //for( j=0; j<15; j++ ) {
         for( j=0; j<18; j++ ) {
         // 2007.10.15
         //--------------------------

           if ( j < 7 || j > 9 )
             P_WKPre[j] = P_PREV_PARTS[i][j];
         }
         // １台分数量 集計
         P_WKPre[7] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][7]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[7]).ToIntDef(0) ) );
         // 予備数 集計
         P_WKPre[8] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][8]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[8]).ToIntDef(0) ) );
         // 総計
         P_WKPre[9] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][9]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[9]).ToIntDef(0) ) );
      }
   }
   if (P_WKPre[7] == "０" ) P_WKPre[7] = "";
   if (P_WKPre[8] == "０" ) P_WKPre[8] = "";
   if (P_PARTS[7] == "０" ) P_PARTS[7] = "";
   if (P_PARTS[8] == "０" ) P_PARTS[8] = "";

   //*****************************************************************
   // 前回部品ﾃﾞｰﾀと今回部品ﾃﾞｰﾀの部品分類[1列目]+識別子[2列目]一致?
   //*****************************************************************
   if(      P_PARTS[2]  != P_WKPre[2]  ) return false;  // 部品型式
   else if( P_PARTS[3]  != P_WKPre[3]  ) return false;  // 部品名称
   else if( P_PARTS[4]  != P_WKPre[4]  ) return false;  // 材質名称
   else if( P_PARTS[5]  != P_WKPre[5]  ) return false;  // 図番
   else if( P_PARTS[6]  != P_WKPre[6]  ) return false;  // 作図用図番
   else if( P_PARTS[7]  != P_WKPre[7]  ) return false;  // 1台分数量
   else if( P_PARTS[8]  != P_WKPre[8]  ) return false;  //
   else if( P_PARTS[9]  != P_WKPre[9]  ) return false;  //
   else if( P_PARTS[10]  != P_WKPre[10]  ) return false;  //
   else if( P_PARTS[11]  != P_WKPre[11]  ) return false;  //
   else return true;

}
//---------------------------------------------------------------------------
// 日本語関数名： 部品比較処理
// 概  要      ：
// 引  数      ： AnsiString P_PARTS ： 比較対象データ
// 戻り値      ： 比較結果 [true:相違なし false:相違あり]
// 備  考      ： 前回部品展開データと今回のデータを比較する  集計タイプデータ用
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Compare_PrevData2( AnsiString *P_PARTS )
{
   int i, j;
   //AnsiString P_WKPre[16];
   AnsiString P_WKPre[18];

   //****************************************
   // 既に格納されている部品ﾃﾞｰﾀと比較
   //****************************************
   for( i=0; i<P_PrevRec; i++ )
   {
      //-----------------------------------------
      // 2007.10.12 ﾐﾗｰ仕様対応
      //  ﾐﾗｰ仕様ｺｰﾄﾞ[17列目]も比較対象に追加
      //if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )
      //  &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  ) )
      if( ( P_PARTS[0]  == P_PREV_PARTS[i][0]  )&&( P_PARTS[1]  == P_PREV_PARTS[i][1]  )
        &&( P_PARTS[4]  == P_PREV_PARTS[i][4]  )&&( P_PARTS[5]  == P_PREV_PARTS[i][5]  )
        &&( P_PARTS[16]  == P_PREV_PARTS[i][16]  ))
      // 2007.10.12
      //-----------------------------------------
      {
         //---------------------------
         // 2007.10.15
         //for( j=0; j<15; j++ ) {
         for( j=0; j<18; j++ ) {
         // 2007.10.15
         //---------------------------

           if ( j < 7 || j > 9 )
             P_WKPre[j] = P_PREV_PARTS[i][j];
         }
         // １台分数量 集計
         P_WKPre[7] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][7]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[7]).ToIntDef(0) ) );
         // 予備数 集計
         P_WKPre[8] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][8]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[8]).ToIntDef(0) ) );
         // 総計
         P_WKPre[9] = HanToZen( IntToStr( ZenToHan(P_PREV_PARTS[i][9]).ToIntDef(0)
                                              + ZenToHan(P_WKPre[9]).ToIntDef(0) ) );
      }
   }
   if (P_WKPre[7] == "０" ) P_WKPre[7] = "";
   if (P_WKPre[8] == "０" ) P_WKPre[8] = "";
   if (P_PARTS[7] == "０" ) P_PARTS[7] = "";
   if (P_PARTS[8] == "０" ) P_PARTS[8] = "";

   //*****************************************************************
   // 前回部品ﾃﾞｰﾀと今回部品ﾃﾞｰﾀの部品分類[1列目]+識別子[2列目]一致?
   //*****************************************************************
   if(      P_PARTS[2]  != P_WKPre[2]  ) return false;  // 部品型式
   else if( P_PARTS[3]  != P_WKPre[3]  ) return false;  // 部品名称
   else if( P_PARTS[4]  != P_WKPre[4]  ) return false;  // 材質名称
   else if( P_PARTS[5]  != P_WKPre[5]  ) return false;  // 図番
   else if( P_PARTS[6]  != P_WKPre[6]  ) return false;  // 作図用図番
   else if( P_PARTS[7]  != P_WKPre[7]  ) return false;  // 1台分数量
   else if( P_PARTS[8]  != P_WKPre[8]  ) return false;  //
   else if( P_PARTS[9]  != P_WKPre[9]  ) return false;  //
   else if( P_PARTS[10]  != P_WKPre[10]  ) return false;  //
   else if( P_PARTS[11]  != P_WKPre[11]  ) return false;  //
   else return true;

}

//↓2002/10/24

//---------------------------------------------------------------------------
// 日本語関数名： 注記出力
// 概  要      ： 最後に注記を出力する
// 引  数      ： AnsiString *P_PARTS ：集計対象ﾚｺｰﾄﾞ
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::Cyuki_Out( void )
{
   int  iOutCnt;
   // 2020.07.17 保護カバー注記追加_S
   AnsiString s_Text;            // 汎用文字列
   // 2020.07.17 保護カバー注記追加_E
   iOutCnt=0;

   if ( P_HEADS_DATA[1335].TrimRight() != "" ) iOutCnt++;  //　端管位置
   if ( P_HEADS_DATA[1302].TrimRight() != "" ) iOutCnt++;  //　禁油１
   if ( P_HEADS_DATA[1320].TrimRight() != "" ) iOutCnt++;  //　フレーム材質
   if ( P_HEADS_DATA[1365].TrimRight() != "" ) iOutCnt++;  //　フレーム仕上
   if ( P_HEADS_DATA[1304].TrimRight() != "" ) iOutCnt++;  //　電解研磨
   if ( P_HEADS_DATA[1325].TrimRight() != "" ) iOutCnt++;  //　ノズル１
   if ( P_HEADS_DATA[1326].TrimRight() != "" ) iOutCnt++;  //　ノズル２
   if ( P_HEADS_DATA[1314].TrimRight() != "" ) iOutCnt++;  //　ボルト材質
   if ( P_HEADS_DATA[1315].TrimRight() != "" ) iOutCnt++;  //　ボルト仕上１
   //2004.01.07 注記削除
   //if ( P_HEADS_DATA[1322].TrimRight() != "" ) iOutCnt++;  //　アンカー材質
   //*******************
   if ( P_HEADS_DATA[1323].TrimRight() != "" ) iOutCnt++;  //　アンカー仕上
   //2004.03.15 注記追加
   if ( P_HEADS_DATA[1018].TrimRight() != "" ) iOutCnt++;  //　ボルト抜け止め
   //*******************
   // 2020.07.17 保護カバー注記追加_S
   if (Tokki_Code("1E") == true)                 iOutCnt++;  //　保護カバー
   // 2020.07.17 保護カバー注記追加_E

   //2003.11.14 改ページ条件変更
   if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - ( iOutCnt + 3 ) )) {
     // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行
     // ( iOutCnt + 3 ):予想される出力行数
   //if ( P_Row > ( ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ) - 8 )) {  // ( DefExcelPageLine1 + P_PageNum * DefExcelPageLine ):ページの最終行　8:予想される出力行数
      P_Row = ( DefExcelPageLine + P_PageNum * DefExcelPageLine ) + 3;
      P_PageNum++;
   } else {
      P_Row++;
      P_Row++;
      P_Row++;
   //   P_Row++;
   //***************************
   }
   if ( P_HEADS_DATA[1335].TrimRight() != "" ) {  //　端管位置
      Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1335].TrimRight());
      P_Row++;
   }
   if ( P_HEADS_DATA[1302].TrimRight() != "" ) {  //　禁油１
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1302].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1320].TrimRight() != "" ) {  //　フレーム材質
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1320].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1365].TrimRight() != "" ) {  //　フレーム仕上
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1365].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1304].TrimRight() != "" ) {  //　電解研磨
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1304].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1325].TrimRight() != "" ) {  //　ノズル１
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1325].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1326].TrimRight() != "" ) {  //　ノズル２
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1326].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1314].TrimRight() != "" ) {  //　ボルト材質
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1314].TrimRight());
     P_Row++;
   }
   if ( P_HEADS_DATA[1315].TrimRight() != "" ) {  //　ボルト仕上１
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1315].TrimRight());
     P_Row++;
   }
   //2004.01.07 注記削除
   //if ( P_HEADS_DATA[1322].TrimRight() != "" ) {  //　アンカー材質
   //  Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1322].TrimRight());
   //  P_Row++;
   //}
   //*******************
   if ( P_HEADS_DATA[1323].TrimRight() != "" ) {  //　アンカー仕上
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1323].TrimRight());
     P_Row++;
   }
   //2004.03.15 注記追加
   if ( P_HEADS_DATA[1018].TrimRight() != "" ) {  //　ボルト抜け止め
     Excel_Cell_DataSet(P_Row, 3, P_HEADS_DATA[1018].TrimRight());
     P_Row++;
   }
   //*******************

   // 2020.07.31 保護カバー注記追加_S
   if (Tokki_Code("1E") == true) {
      if(P_HEADS_DATA[36].Pos("M") == 0)  {
         // 舶用でない場合
         s_Text = "保護カバー要";
      } else {
         // 舶用の場合
         s_Text = "建設用保護カバー要";
      }
      Excel_Cell_DataSet(P_Row, 3, s_Text );
      P_Row++;
   }
   // 2020.07.31 保護カバー注記追加_E

}


//---------------------------------------------------------------------------
// 日本語関数名： 注記出力
// 概  要      ： 最後に注記を出力する
// 引  数      ： AnsiString *P_PARTS ：集計対象ﾚｺｰﾄﾞ
// 戻り値      ： なし
// 備  考      ： ﾉｽﾞﾙ部品ﾃﾞｰﾀ以降で使用
//---------------------------------------------------------------------------
void __fastcall TBuhinhyou::BLT_Cover_Out( void )
{
   int          i_RecNo;
   int          i_ErrFlg;
   int          i_Value_1;
   int          i_Value_2;
   int          i_Daisuu;
   AnsiString   s_Text;
   //***************************
   //***  ボルトカバー
   //***************************
   // 『C018』
   i_RecNo = 1092;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text != "0"){
      i_RecNo = 1096;
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text != "0"){
         s_Text = "締付ボルトカバー";
         Excel_Cell_DataSet(P_Row, 5, "締付ボルトカバー");
      } else {
        return;
      }
   } else {
        return;
   }

   G_Log_Text = "ボルトカバー         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //**********************************************************
   //***  ﾎﾞﾙﾄ本数L,S　数値ﾁｪｯｸ
   //***    『C019』,『C020』,『C021』の為の処理前ﾁｪｯｸ
   //**********************************************************
   i_ErrFlg = 0;
   s_Text = P_HEADS_DATA[39];
   i_Daisuu = s_Text.ToIntDef(0);

   // ﾎﾞﾙﾄ本数L
   i_RecNo = 1286;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
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
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
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
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "締付ボルトカバー");
         // BLCP1 『C019』
         s_Text = s_Text + "L";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text)); // 仕様１
         G_Log_Text = "BLCP1                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // 2014/08/25 応用技術編集
         // BLCP1 仕様２
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCP1本数 『C020』
         s_Text = AnsiString(i_Value_1 + i_Value_2);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // 予備数
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // 総計
         G_Log_Text = "BLCP1 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      }
   }

   P_Row++;
   //***************************
   //***  BLCEL
   //***************************
   // 『C021』,『C022』
   if (i_ErrFlg == 0){
      i_RecNo = 1096;      // BLCEL
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "締付ボルトカバー");
         // BLCEL 『C021』
         s_Text = s_Text + "L キャップ付";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text));
         G_Log_Text = "BLCEL                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // 2014/08/25 応用技術編集
         // BLCEL 仕様２
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCEL本数 『C022』
         s_Text = AnsiString(i_Value_1);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // 予備数
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // 総計
         G_Log_Text = "BLCEL 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      }
   }
   P_Row++;

   //***************************
   //***  BLCES
   //***************************
   // 『C023』,『C024』
   if (i_ErrFlg == 0){
      i_RecNo = 1097;      // BLCES
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "締付ボルトカバー");
         // BLCES 『C023』
         s_Text = s_Text + "L キャップ付";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text));
         G_Log_Text = "BLCES                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // 2014/08/25 応用技術編集
         // BLCES 仕様２
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCES本数 『C024』
         s_Text = AnsiString(i_Value_2);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // 予備数
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // 総計
         G_Log_Text = "BLCES 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

      }
   }

   P_Row++;
}
//---------------------------------------------------------------------------
// 日本語関数名： 注記出力(小径ボルトカバー)
// 概  要      ： BLCP1SとBLCESSを出力する。
// 引  数      ：
// 戻り値      ： なし
// 備  考      ： BLT_Cover_Out(void)を踏襲して作成。
//---------------------------------------------------------------------------
// 2014/08/25 応用技術編集
void __fastcall TBuhinhyou::BLT_Cover_Out_Small( void )
{
   int          i_RecNo;
   int          i_ErrFlg;
   int          i_Value;
   int          i_Daisuu;
   AnsiString   s_Text;

   //***************************
   //***  ボルトカバー
   //***************************
   i_RecNo = 1111;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Text != "0"){
      i_RecNo = 1112;
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text != "0"){
         s_Text = "締付ボルトカバー";
         Excel_Cell_DataSet(P_Row, 5, "締付ボルトカバー");
      } else {
        return;
      }
   } else {
        return;
   }

   G_Log_Text = "小径ボルトカバー         『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //**********************************************************
   //***  ﾎﾞﾙﾄ本数数値ﾁｪｯｸ
   //**********************************************************
   i_ErrFlg = 0;
   s_Text = P_HEADS_DATA[39];
   i_Daisuu = s_Text.ToIntDef(0);

   // ﾎﾞﾙﾄ本数S
   i_RecNo = 1113;
   s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      i_Value = s_Text.ToIntDef(0);   // ﾎﾞﾙﾄ本数S
   } else {
      G_ErrLog_Text = "小径ﾎﾞﾙﾄ本数Ｓ      RecNo:" + FormatFloat("0000",i_RecNo) + "  不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ

      i_ErrFlg = 1;
   }

   //***************************
   //***  BLCP1S
   //***************************
   if (i_ErrFlg == 0){
      i_RecNo = 1111;      // BLCP1S
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "締付ボルトカバー");
         // BLCP1S
         s_Text = s_Text + "L";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text)); // 仕様１
         G_Log_Text = "BLCP1S                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCP1S 仕様２
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCP1S本数
         s_Text = AnsiString(i_Value);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // 予備数
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // 総計
         G_Log_Text = "BLCP1S 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   }

   P_Row++;
   //***************************
   //***  BLCESS
   //***************************
   if (i_ErrFlg == 0){
      i_RecNo = 1112;      // BLCESS
      s_Text = P_HEADS_DATA[i_RecNo].TrimRight();
      if (s_Text.ToIntDef(0) != 0 ){
         Excel_Cell_DataSet(P_Row, 5, "締付ボルトカバー");
         // BLCESS
         s_Text = s_Text + "L キャップ付";
         Excel_Cell_DataSet(P_Row, 48, HanToZen(s_Text));
         G_Log_Text = "BLCESS                『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);

         // BLCESS 仕様２
         Excel_Cell_DataSet(P_Row, 58, s_Text);

         // BLCESS本数
         s_Text = AnsiString(i_Value);
         Excel_Cell_DataSet(P_Row, 71, HanToZen(s_Text));
         Excel_Cell_DataSet(P_Row, 74, "");  // 予備数
         Excel_Cell_DataSet(P_Row, 78, HanToZen(s_Text.ToIntDef(0) * i_Daisuu));  // 総計
         G_Log_Text = "BLCESS 本数           『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      }
   }

   P_Row++;
}
//-------------------------------------------------------------------------------------
// 日本語関数名： 過去Excelｼｰﾄ　存在ﾁｪｯｸ
// 概  要      ： Rev*のﾃﾞｨﾚｸﾄﾘ名を検索し、指示書の存在する最大のﾘﾋﾞｼﾞｮﾝ番号を取得
// 引  数      ：
// 戻り値      ： 指示書の存在する直近のﾘﾋﾞｼﾞｮﾝ番号
// 備  考      ：
//-------------------------------------------------------------------------------------
AnsiString __fastcall TBuhinhyou::Csv_Exist_Check(AnsiString path){
   AnsiString RevNo;
   AnsiString NowRev;
   AnsiString SheetPass;
   bool       bFind;

   //2004.11.16 出力フォルダ及び訂正印変更
   //NowRev = P_HEADS_DATA[319].TrimRight();
   NowRev = G_HEADS_DATA_Rev;
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
   while(RevNo.ToIntDef(0) >= 0 && bFind == false) {
      // ﾌｧｲﾙ存在ﾁｪｯｸ
      SheetPass = path + RevNo + "\\HD_PARTS.csv";
      // 2023.02.08 ログ追加_S
      G_Log_Text = "SheetPass?『 " + SheetPass + " 』";
      Write_Log(G_Log_Text);
      // 2023.02.08 ログ追加_E
      if (FileExists(SheetPass.c_str()) == true){
        bFind = true;
      } else {
        RevNo = FormatFloat("00",RevNo.ToIntDef(0) - 1);
      }
   }
   if ( bFind == false ) {
     RevNo = "";
   }
   return RevNo;
}
//↑2002/10/24

//↓2003/01/21
//-------------------------------------------------------------------------------------
// 日本語関数名： ブロック毎にハイフンで擬似罫線を出力
// 概  要      ：
// 引  数      ：
// 戻り値      ：
// 備  考      ：
//-------------------------------------------------------------------------------------
void __fastcall TBuhinhyou::Write_BorderLine(void){
//   Excel_Cell_DataSet(P_Row,  3, "-------------------------------------------------");
//   Excel_Cell_DataSet(P_Row, 24, "----------------------------");
//   Excel_Cell_DataSet(P_Row, 36, "--------------------------");
//   Excel_Cell_DataSet(P_Row, 47, "----------------------------------------------------");
//   Excel_Cell_DataSet(P_Row, 71, "------");
//   Excel_Cell_DataSet(P_Row, 74, "------");
//   Excel_Cell_DataSet(P_Row, 78, "----------");
   Excel_Cell_DataSet(P_Row,  3, "-------------------------------------------------------------------------");
   Excel_Cell_DataSet(P_Row, 24, "--------------------------------------------");
   Excel_Cell_DataSet(P_Row, 36, "------------------------------------------");
   Excel_Cell_DataSet(P_Row, 47, "--------------------------------------------------------------------------");
   Excel_Cell_DataSet(P_Row, 71, "---------");
   Excel_Cell_DataSet(P_Row, 74, "----------");
   Excel_Cell_DataSet(P_Row, 78, "--------------");
   P_Row++;
}
//↑2003/01/21



//---------------------------------------------------------------------------
// 日本語関数名： 部品展開ﾃﾞｰﾀ読込(ｿｰﾄ,ﾏｰｼﾞ処理付)
// 概  要      ： 引数で渡された部品展開ﾃﾞｰﾀﾌｧｲﾙ(HD_PARTS.csv)を読み込み
//                ｿｰﾄ、ﾏｰｼﾞ後に、文字列配列にセットする。
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： ﾐﾗｰA仕様とﾐﾗｰB仕様で同ﾃﾞｰﾀがある場合にﾏｰｼﾞ,ｿｰﾄを行う
//                ※ﾌｧｲﾙの前提条件
//                  (1)ﾃﾞｰﾀは、以下のいずれかで構成されること
//                  　･元仕様のみのﾃﾞｰﾀ
//                  　･ﾐﾗｰA仕様とﾐﾗｰB仕様のﾃﾞｰﾀ
//                  (2)ﾐﾗｰA仕様のﾃﾞｰﾀ(全行)の後にﾐﾗｰB仕様のﾃﾞｰﾀ(全行)が出力されていること
//
//                2007.10.12 ﾐﾗｰ仕様対応 (新規作成)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Read_Buhin_Data_Sorted(AnsiString filepass, AnsiString s_DATA[][17])
{
    char  buf[1024];                            /* 文字列バッファ   */
    char  *tok;                                 /* トークンポインタ */
    int   i, j, k;                                 /* ループカウンタ   */
    char  token[100][100];                      /* トークンバッファ */
    int   iDataNum;
    int   iDataIdx;
    AnsiString s_wkData[1000][17];
    bool bSearch;
    AnsiString s_LineData[17];

    AnsiString wk_FilePass;
    FILE *fp;

    AnsiString s_sortDATA[1000][17];
    int iRow;

    // 2007.11.08 ﾌﾟﾚｰﾄﾃﾞｰﾀ、孔明ﾃﾞｰﾀ用
    AnsiString s_wkPlData[50][17];
    AnsiString s_wkTmpPlData[50][18];
    AnsiString s_wkTmpHlData[50][18];
    int iTmpPlDataNum;
    int iTmpHlDataNum;

    iDataNum = 0;
    iTmpPlDataNum = 0;
    iTmpHlDataNum = 0;

    memset(s_wkData,0x00,sizeof(s_wkData));
    memset(s_wkPlData,0x00,sizeof(s_wkPlData));
    memset(s_wkTmpPlData,0x00,sizeof(s_wkTmpPlData));
    memset(s_wkTmpHlData,0x00,sizeof(s_wkTmpHlData));

    //*******************************
    //***  HEADSデータ　読込
    //*******************************
    wk_FilePass = filepass;
    if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
        fclose(fp);    // 閉じる
        return false;
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

        // 1行ﾃﾞｰﾀ配列に格納
        for( j = 0; j < 17 ; j++ ){
            s_LineData[j] = token[j];
        }

        // 配列追加ﾌﾗｸﾞ初期化
        bSearch = false;

        //------------------------------------------------------
        // 2008.03.19 ミラーB仕様の場合、
        // 温度計、及び、圧力計データを、A側とB側で入れ替える
        // 識別子"A_TEMP"/"B_TEMP"/"A_PRSG"/"B_PRSG"
        //------------------------------------------------------
        if( s_LineData[16] == "B"){
            if( s_LineData[0] == "A_TEMP" ){
                s_LineData[0] = "B_TEMP";
            }else if( s_LineData[0] == "B_TEMP" ){
                s_LineData[0] = "A_TEMP";
            }else if( s_LineData[0] == "A_PRSG" ){
                s_LineData[0] = "B_PRSG";
            }else if( s_LineData[0] == "B_PRSG" ){
                s_LineData[0] = "A_PRSG";
            }
        }

        //2008.08.21 端管廻り止め修正
        //ミラーB仕様の場合、
        //端管廻り止めのデータを、A側とB側で入れ替える
        //識別子"RSTP-A"/"RSTP-B"
        if( s_LineData[16] == "B"){
            if( s_LineData[1] == "RSTP-A" ){
                s_LineData[1] = "RSTP-B";
            }else if( s_LineData[1] == "RSTP-B" ){
                s_LineData[1] = "RSTP-A";
            }
        }
        //***************************

        //============================
        // ﾐﾗｰ仕様のﾃﾞｰﾀ行の場合
        //============================
        if( s_LineData[16] == "A" || s_LineData[16] == "B" ){
            if(iDataNum > 0){
                for( j = 0; j < iDataNum ; j++ ){

                    //------------------------------------------------
                    // ﾐﾗｰA仕様とﾐﾗｰB仕様で、同じ部品は1つにまとめる
                    // [集計1]=[集計5]>[集計4]>[集計2]=[集計3]の順に集計条件が厳しい
                    //
                    // ※ﾐﾗｰ仕様の部品集計仕様及び、以下の既存関数をもとに作成
                    //   Buhin_Syukei,Buhin_Syukei2,Buhin_Syukei3,Buhin_Syukei4
                    //------------------------------------------------

                    //------------------------------------------------
                    // ■集計1
                    // ･ﾌﾟﾚｰﾄ/ﾌﾚｰﾑのうち、[1]識別子が
                    //  "DPLG-A"/"DPLG-B"/"DPLGDA"/
                    //  "DPLGDB"/"EPLGDA"/"EPLGDB"/"EPGS-R"/"FRAM-E"
                    // ･[1]識別子,[3]部品名称,[4]材質名称,[5]図番,
                    //  [10]仕様1 ～ [13]仕様4が全て一致するﾃﾞｰﾀ
                    // は1つにまとめる
                    //------------------------------------------------
                    if( ( s_LineData[0] == "PLATE" )
                     || ( s_LineData[0] == "FRAME" )){

                         if( ( s_LineData[1] == "DPLG-A" )
                          || ( s_LineData[1] == "DPLG-B" )
                          || ( s_LineData[1] == "DPLGDA" )
                          || ( s_LineData[1] == "DPLGDB" )
                          || ( s_LineData[1] == "EPLGDA" )
                          || ( s_LineData[1] == "EPLGDB" )
                          || ( s_LineData[1] == "EPGS-R" )
                          || ( s_LineData[1] == "FRAM-E" ) ){

                            if( ( s_wkData[j][1] == s_LineData[1] )
                             && ( s_wkData[j][3] == s_LineData[3] )
                             && ( s_wkData[j][4] == s_LineData[4] )
                             && ( s_wkData[j][5] == s_LineData[5] )
                             && ( s_wkData[j][10] == s_LineData[10] )
                             && ( s_wkData[j][11] == s_LineData[11] )
                             && ( s_wkData[j][12] == s_LineData[12] )
                             && ( s_wkData[j][13] == s_LineData[13] ) ){

                                bSearch = true;
                                break;
                            }
                        }
                    }

                    //------------------------------------------------
                    // ■集計2
                    // ･ﾌﾟﾚｰﾄのうち、[集計1]以外の識別子を持つﾃﾞｰﾀ
                    // ･[1]識別子,[4]材質名称,[5]図番が
                    //  全て一致するﾃﾞｰﾀは1つにまとめる
                    // ※ただし、"PLHL--","DPLT--"は処理しない
                    //------------------------------------------------
                    if( ( s_LineData[0] == "PLATE" ) ){

                        if( ( s_LineData[1] != "PLHL--" )
                         && ( s_LineData[1] != "DPLG-A" )
                         && ( s_LineData[1] != "DPLG-B" )
                         && ( s_LineData[1] != "DPLGDA" )
                         && ( s_LineData[1] != "DPLGDB" )
                         && ( s_LineData[1] != "EPLGDA" )
                         && ( s_LineData[1] != "EPLGDB" )
                         && ( s_LineData[1] != "EPGS-R" )
                         && ( s_LineData[1] != "DPLT--" ) ){

                            if( ( s_wkData[j][1] == s_LineData[1] )
                            && ( s_wkData[j][4] == s_LineData[4] )
                            && ( s_wkData[j][5] == s_LineData[5] ) ){

                                bSearch = true;
                                break;
                            }
                        }
                    }

                    //------------------------------------------------
                    // ■集計3
                    // ･部品名称=MOATE
                    // ･[1]識別子,[4]材質名称,[5]図番が
                    //  全て一致するﾃﾞｰﾀは1つにまとめる
                    //------------------------------------------------
                    if( ( s_LineData[0] == "MOATE" ) ){

                        if( ( s_LineData[1] == "EPBP-A" )
                         || ( s_LineData[1] == "EPBP-B" ) ){

                            if( ( s_wkData[j][1] == s_LineData[1] )
                             && ( s_wkData[j][4] == s_LineData[4] )
                             && ( s_wkData[j][5] == s_LineData[5] ) ){

                                bSearch = true;
                                break;
                            }
                        }
                    }

                    //------------------------------------------------
                    // ■集計4
                    // ･部品名称=ﾉｽﾞﾙ
                    // ･[1]識別子,[4]材質名称,[5]図番,[10]仕様1,[11]仕様2が
                    //  全て一致するﾃﾞｰﾀは1つにまとめる
                    //------------------------------------------------
                    if( ( s_LineData[0] == "NOZZLE" ) ){

                        if( ( s_wkData[j][1] == s_LineData[1] )
                         && ( s_wkData[j][4] == s_LineData[4] )
                         && ( s_wkData[j][5] == s_LineData[5] )
                         && ( s_wkData[j][10] == s_LineData[10] )
                         && ( s_wkData[j][11] == s_LineData[11] ) ){

                            bSearch = true;
                            break;
                        }
                    }

                    //------------------------------------------------
                    // ■集計5
                    // ･集計1～集計4を満たさない全ての部品
                    // ･[0]部品分類,[1]識別子,[3]部品名称,[4]材質名称,[5]図番,
                    //  [10]仕様1 ～ [13]仕様4が全て一致するﾃﾞｰﾀ
                    //  は1つにまとめる
                    // ※ERROR行はまとめない
                    // ※"PLHL--","DPLT--"は処理しない
                    // ※集計方法は、[集計1]と同じ
                    //------------------------------------------------
                    if( ( ! ( s_LineData[0] == "ERROR" ) )
                     && ( ! ( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "PLHL--" ) ) )
                     && ( ! ( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "DPLT--" ) ) ) ){

                        if( ( s_wkData[j][0] == s_LineData[0] )
                         && ( s_wkData[j][1] == s_LineData[1] )
                         && ( s_wkData[j][3] == s_LineData[3] )
                         && ( s_wkData[j][4] == s_LineData[4] )
                         && ( s_wkData[j][5] == s_LineData[5] )
                         && ( s_wkData[j][10] == s_LineData[10] )
                         && ( s_wkData[j][11] == s_LineData[11] )
                         && ( s_wkData[j][12] == s_LineData[12] )
                         && ( s_wkData[j][13] == s_LineData[13] ) ){

                            bSearch = true;
                            break;
                        }
                    }

                }
            }
        }

        if( bSearch ){
            // 「今回ﾃﾞｰﾀがﾐﾗｰA仕様」または「今回ﾃﾞｰﾀがﾐﾗｰB仕様で足し先ﾃﾞｰﾀがﾐﾗｰB仕様」の場合
            // ※「今回ﾃﾞｰﾀがﾐﾗｰA仕様、かつ、足し先ﾃﾞｰﾀがﾐﾗｰB仕様」のｹｰｽは存在しない
            if( ( ( s_LineData[16] == "A" ) )
             || ( ( s_LineData[16] == "B" ) && ( s_wkData[j][16] == "B") ) ){

                // 1台分数量に値を足す
                s_wkData[j][7] = HanToZen( IntToStr( ZenToHan(s_wkData[j][7]).ToIntDef(0)
                                                   + ZenToHan(s_LineData[7]).ToIntDef(0) ) );

                // 予備数に値を足す
                s_wkData[j][8] = HanToZen( IntToStr( ZenToHan(s_wkData[j][8]).ToIntDef(0)
                                                   + ZenToHan(s_LineData[8]).ToIntDef(0) ) );
            }

            // 総計にﾐﾗｰA仕様/ﾐﾗｰB仕様の総計を足す
            s_wkData[j][9] = HanToZen( IntToStr( ZenToHan(s_wkData[j][9]).ToIntDef(0)
                                               + ZenToHan(s_LineData[9]).ToIntDef(0) ) );

            // 今回ﾃﾞｰﾀがﾐﾗｰB仕様で、足し先ﾃﾞｰﾀがﾐﾗｰA仕様の場合
            if( ( s_LineData[16] == "B" ) && ( s_wkData[j][16] == "A" ) ){
                // ﾐﾗｰ仕様文字列を消去("A"→"")
                s_wkData[j][16] = "";
            }

        }else{
            // "PLATE"のﾌﾟﾚｰﾄﾃﾞｰﾀ行("DPLT--")
            if( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "DPLT--" ) ){
                for( j = 0; j < 17 ; j++ ){
                    s_wkTmpPlData[iTmpPlDataNum][j] = s_LineData[j];
                }
                s_wkTmpPlData[iTmpPlDataNum][17] = iTmpPlDataNum;
                iTmpPlDataNum++;

            // "PLATE"の孔明ﾃﾞｰﾀ行("PLHL--")
            }else if( ( s_LineData[0] == "PLATE" ) && ( s_LineData[1] == "PLHL--" ) ){
                for( j = 0; j < 17 ; j++ ){
                    s_wkTmpHlData[iTmpHlDataNum][j] = s_LineData[j];
                }
                s_wkTmpHlData[iTmpHlDataNum][17] = iTmpPlDataNum - 1;
                iTmpHlDataNum++;

            // 上記以外の部品で、一致ﾃﾞｰﾀがない場合
            }else{

                //*******************************
                // 一致ﾃﾞｰﾀがなければ配列に追加
                //*******************************
                for( j = 0; j < 17 ; j++ ){
                    s_wkData[iDataNum][j] = s_LineData[j];

                }
                // 配列Indexｲﾝｸﾘﾒﾝﾄ
                iDataNum++;
            }
        }
    }

    fclose(fp);    // 閉じる
    //delete fp;

    //********************************************
    // ﾌﾟﾚｰﾄ-孔明ﾃﾞｰﾀをﾏｰｼﾞ、ｿｰﾄして、配列に格納
    //********************************************
    Sort_HEADS_PlData(s_wkTmpPlData, s_wkTmpHlData, s_wkPlData);

    //********************************************
    // ﾃﾞｰﾀを部品分類、識別子でｿｰﾄして、配列に格納
    //  "PLATE"→"MOATE"→"FRAME"→"NOZZLE"→
    //  "B_TEMP"→"A_TEMP"→"B_PRSG"→"A_PRSG"→
    //  "ACCS"→"SHIP"→"ERROR"
    //********************************************
    Sort_HEADS_Data(s_wkData, s_sortDATA);

    //********************************************
    // ﾌﾟﾚｰﾄ-孔明ﾃﾞｰﾀとそれ以外のﾃﾞｰﾀを統合
    //********************************************
    iRow = 0;

    // ﾌﾟﾚｰﾄ－孔明ﾃﾞｰﾀを格納
    for( i = 0; i < 1000; i++){
        if( s_wkPlData[i][0] != "" ){
            for( j = 0; j < 17; j++){
                s_DATA[iRow][j] = s_wkPlData[i][j];
            }
            iRow++;
        }else{
            break;
        }
    }

    // それ以外のﾃﾞｰﾀ(ｿｰﾄ済)を格納
    for( i = 0; i< 1000; i++){
        if( s_sortDATA[i][0] != "" ){
            for( j = 0; j < 17; j++){
                s_DATA[iRow][j] = s_sortDATA[i][j];
            }
            iRow++;
        }else{
            break;
        }
    }

    return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 部品展開ﾃﾞｰﾀ配列ﾃﾞｰﾀをｿｰﾄする
// 概  要      ： 引数で渡された部品展開ﾃﾞｰﾀ配列をｿｰﾄする
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： ﾐﾗｰA仕様とﾐﾗｰB仕様で、識別子が同じﾃﾞｰﾀ順に並べ替える
//
//                2007.10.15 ﾐﾗｰ仕様対応 (新規作成)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Sort_HEADS_Data(AnsiString s_DATA[][17], AnsiString s_SortDATA[][17])
{
    // 部品分類配列
    AnsiString sPartsType[11] = {"PLATE", "MOATE", "FRAME", "NOZZLE",
                                 "B_TEMP", "A_TEMP", "B_PRSG", "A_PRSG",
                                 "ACCS", "SHIP", "ERROR"};
    // ｿｰﾄ前ﾃﾞｰﾀ行数
    int iDataNum;

    // 処理済識別子配列
    AnsiString sTmp[100];
    int iTmpCnt;

    // 現在の比較元
    int iIdRow;
    AnsiString sIdName;
    int iMatchRow;

    int i, j, k;

    // ｿｰﾄ後配列用
    int iSortDataNum;
    bool bDone;     // ｿｰﾄ配列に取得済かどうか

    //-----------------------------------
    // ｿｰﾄ後配列初期化
    //-----------------------------------
    memset(s_SortDATA, 0x00, sizeof(s_SortDATA));

    //-----------------------------------
    // ﾃﾞｰﾀ数ｶｳﾝﾄ
    //-----------------------------------
    iDataNum = 0;
    i = 0;
    // NULLになるまでｶｳﾝﾄ
    while( s_DATA[i][0] != "" ){
        iDataNum++;
        i++;
    }

    // ﾃﾞｰﾀ0件なら終了
    if( iDataNum==0 ) return(true);

    // ｿｰﾄﾃﾞｰﾀ格納ｶｳﾝﾀ初期化
    iSortDataNum = 0;
    iTmpCnt = 0;

    // 部品分類ごとに処理
    for( i = 0; i < 11; i++ ){

        //--------------------------------------------
        // 処理済識別子ｸﾘｱ
        //--------------------------------------------
        if( iTmpCnt > 0 ){
            for( j = 0; j < iTmpCnt; j++ ){
                sTmp[j] = "";
            }
        }
        iTmpCnt = 0;

        //--------------------------------------------
        // 初期値
        //--------------------------------------------
        iMatchRow = 0;  // 比較元行探索
        iIdRow  = 0;    // 比較元行
        sIdName = "";   // 比較元識別子

        while( iMatchRow < iDataNum ){

            //--------------------------------------------
            // 比較元識別子設定(初期値)
            //--------------------------------------------
            bDone = false;
            if( (s_DATA[iMatchRow][0] != sIdName) && (s_DATA[iMatchRow][0] == sPartsType[i])){
                for( j = 0; j < iTmpCnt; j++){
                    if( s_DATA[iMatchRow][1] == sTmp[j] ){
                        // 既に処理済の識別子
                        bDone = true;
                        break;
                    }
                }

                if(!bDone){
                    //--------------------------------------------
                    // 取得する識別子を設定
                    //--------------------------------------------
                    iIdRow = iMatchRow;
                    sIdName = s_DATA[iIdRow][1];

                    sTmp[iTmpCnt] = sIdName;
                    iTmpCnt++;

                    //--------------------------------------------
                    // 比較元識別子と一致するﾃﾞｰﾀをｿｰﾄ配列に格納
                    //--------------------------------------------
                    for( j = iMatchRow; j < iDataNum; j++ ){

                        if( (s_DATA[j][0] == sPartsType[i]) && (s_DATA[j][1] == sIdName) ){
                            for( k = 0; k < 17; k++){
                                s_SortDATA[iSortDataNum][k] = s_DATA[j][k];
                            }
                            iSortDataNum++;
                        }
                    }
                }
            }
            iMatchRow++;
        }
    }

    return(true);
}

//---------------------------------------------------------------------------
// 日本語関数名： 部品展開ﾃﾞｰﾀのﾌﾟﾚｰﾄﾃﾞｰﾀ、孔明ﾃﾞｰﾀをﾏｰｼﾞしてｿｰﾄする
// 概  要      ： 引数で渡されたﾌﾟﾚｰﾄﾃﾞｰﾀ配列、孔明ﾃﾞｰﾀをﾏｰｼﾞしてｿｰﾄする
// 引  数      ： s_PlData　ﾌﾟﾚｰﾄﾃﾞｰﾀ配列
//                s_HlData  孔明ﾃﾞｰﾀ配列
//                s_SortPlData ﾏｰｼﾞ、ｿｰﾄ後のﾌﾟﾚｰﾄﾃﾞｰﾀ、孔明ﾃﾞｰﾀ配列
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： ﾐﾗｰA仕様とﾐﾗｰB仕様で、同一ﾃﾞｰﾀは統合し、最後にｿｰﾄする
//
//                2007.11.08 ﾐﾗｰ仕様対応 (新規作成)
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Sort_HEADS_PlData(AnsiString s_wkTmpPlData[][18], AnsiString s_wkTmpHlData[][18], AnsiString s_SortPlDATA[][17])
{

    int i, j, k;
    int iPlDataNum;     // ﾏｰｼﾞ、ｿｰﾄ後のﾌﾟﾚｰﾄ、孔明ﾃﾞｰﾀ行数
    int iTmpPlDataNum;  // ﾌﾟﾚｰﾄﾃﾞｰﾀ行数
    int iTmpHlDataNum;  // 孔明ﾃﾞｰﾀ行数

    bool bMatch;
    int bMatchRow;

    //----------------------------------------------------
    // 初期化
    //----------------------------------------------------
    // 配列初期化
    memset(s_SortPlDATA,0x00,sizeof(s_SortPlDATA));

    // 配列ｶｳﾝﾀ初期化
    iPlDataNum = 0;
    iTmpPlDataNum = 0;
    iTmpHlDataNum = 0;

    //----------------------------------------------------
    // ﾃﾞｰﾀ数ｶｳﾝﾄ
    //----------------------------------------------------
    i = 0;
    // ﾌﾟﾚｰﾄﾃﾞｰﾀ数(NULLになるまでｶｳﾝﾄ)
    while( s_wkTmpPlData[i][0] != "" ){
        iTmpPlDataNum++;
        i++;
    }

    i = 0;
    // 孔明ﾃﾞｰﾀ数(NULLになるまでｶｳﾝﾄ)
    while( s_wkTmpHlData[i][0] != "" ){
        iTmpHlDataNum++;
        i++;
    }

        AnsiString iAplNo;
        AnsiString iBplNo;

    //----------------------------------------------------
    // ﾐﾗｰB仕様のﾌﾟﾚｰﾄﾃﾞｰﾀで、ﾐﾗｰA仕様に同一ﾃﾞｰﾀがあれば、
    // ･ﾐﾗｰA仕様のﾌﾟﾚｰﾄﾃﾞｰﾀの仕様ﾀｲﾌﾟｺｰﾄﾞを"A"→""に更新
    // ･ﾐﾗｰB仕様のﾌﾟﾚｰﾄﾃﾞｰﾀを無効にする
    // ･無効にしたﾌﾟﾚｰﾄﾃﾞｰﾀのﾌﾟﾚｰﾄNoと同じ値を持つ孔明ﾃﾞｰﾀがあれば、
    //  孔明ﾃﾞｰﾀ(ﾐﾗｰB仕様)のﾌﾟﾚｰﾄNoをﾐﾗｰA仕様の値で更新
    //----------------------------------------------------
    for( i = 0; i < iTmpPlDataNum ; i++ ){

        bMatch = false;

        // ﾐﾗｰB仕様のﾌﾟﾚｰﾄﾃﾞｰﾀの場合
        if( s_wkTmpPlData[i][16] == "B"){

            // ﾐﾗｰB仕様のﾌﾟﾚｰﾄNo
            iBplNo = s_wkTmpPlData[i][17].ToIntDef(-1);

            // A仕様に同じﾌﾟﾚｰﾄがないか探す
            for( j = 0; j < i ; j++ ){
                if( s_wkTmpPlData[j][16] == "A" ){
                    for( k = 0; k < 16; k++){
                        //2010/05/14 HEADSシステム障害連絡表-0004 → 仕様１の条件を外す(k != 10)
                        if( (k != 7) && (k != 8) && (k != 9) && (k != 10) ){   // 数量は比較しない
                            if( s_wkTmpPlData[i][k] != s_wkTmpPlData[j][k] ){
                                break;
                            }
                        }
                        // A仕様に同じﾌﾟﾚｰﾄあり
                        if( k == 15 ){
                            bMatch = true;
                            bMatchRow = j;
                            // ﾐﾗｰA仕様のﾌﾟﾚｰﾄNo
                            iAplNo = s_wkTmpPlData[bMatchRow][17].ToIntDef(-1);
                        }
                    }
                }
                if( bMatch ) break;
            }
        }

        if( bMatch ){

            // ﾌﾟﾚｰﾄ(A仕様)の仕様ﾀｲﾌﾟｺｰﾄﾞを消去("A"→"")
            s_wkTmpPlData[bMatchRow][16] = "";

            // ﾌﾟﾚｰﾄ(A仕様)の総数にﾌﾟﾚｰﾄ(B仕様)の総数を足す
            s_wkTmpPlData[bMatchRow][9] = HanToZen( IntToStr( ZenToHan(s_wkTmpPlData[bMatchRow][9]).ToIntDef(0)
                                        + ZenToHan(s_wkTmpPlData[i][9]).ToIntDef(0) ) );

            // ﾌﾟﾚｰﾄ(B仕様)のﾌﾟﾚｰﾄNoを無効にする
            s_wkTmpPlData[i][17] = -1;

            // 孔明(B仕様)のﾌﾟﾚｰﾄNoで、ﾌﾟﾚｰﾄ(B仕様)のﾌﾟﾚｰﾄNoと同じであれば、ﾌﾟﾚｰﾄ(A仕様)ﾌﾟﾚｰﾄNoで更新
            for( j = 0; j < iTmpHlDataNum; j++){
                if( s_wkTmpHlData[j][17].ToIntDef(-1) == iBplNo ){

                    s_wkTmpHlData[j][17] = iAplNo;
                }
            }
        }
    }

    //----------------------------------------------------
    // ﾐﾗｰB仕様の孔明ﾃﾞｰﾀで、ﾐﾗｰA仕様に同一ﾃﾞｰﾀがあれば、
    // ﾐﾗｰA仕様の孔明ﾃﾞｰﾀの仕様ﾀｲﾌﾟｺｰﾄﾞを"A"→""に更新し、
    // ﾐﾗｰB仕様の孔明ﾃﾞｰﾀを無効にする
    //----------------------------------------------------
    for( i = 0; i < iTmpHlDataNum ; i++ ){

        bMatch = false;

        // ﾐﾗｰB仕様の孔明ﾃﾞｰﾀの場合
        if( s_wkTmpHlData[i][16] == "B"){

            // A仕様に同じ孔明ﾃﾞｰﾀがないか探す
            for( j = 0; j < i ; j++ ){
                if( s_wkTmpHlData[j][16] == "A" ){
                    for( k = 0; k < 16; k++){
                        if( (k != 7) && (k != 8) && (k != 9) ){
                            if( s_wkTmpHlData[i][k] != s_wkTmpHlData[j][k] ){
                                break;
                            }
                            // A仕様に同じ孔明ﾃﾞｰﾀあり
                            if( k == 15 ){
                                bMatch = true;
                                bMatchRow = j;
                            }
                        }
                    }
                }
                if( bMatch ) break;
            }
        }

        if( bMatch ){
            // 孔明(A仕様)の仕様ﾀｲﾌﾟｺｰﾄﾞを消去("A"→"")
            s_wkTmpHlData[bMatchRow][16] = "";

            // 孔明(A仕様)の総計に孔明(B仕様)の総計を足す
            s_wkTmpHlData[bMatchRow][9] = HanToZen( IntToStr( ZenToHan(s_wkTmpHlData[bMatchRow][9]).ToIntDef(0)
                                        + ZenToHan(s_wkTmpHlData[i][9]).ToIntDef(0) ) );

            // 孔明(B仕様)のﾌﾟﾚｰﾄNoを無効にする
            s_wkTmpHlData[i][17] = -1;
        }
    }

    //----------------------------------------------------
    // ﾌﾟﾚｰﾄﾃﾞｰﾀ、孔明ﾃﾞｰﾀを、まとめて配列に格納
    //----------------------------------------------------
    for( i = 0; i < iTmpPlDataNum; i++ ){
        // 配列に格納が必要なﾌﾟﾚｰﾄﾃﾞｰﾀ行であるか
        if( s_wkTmpPlData[i][17].ToIntDef(-1) != -1 ){

            // ﾌﾟﾚｰﾄﾃﾞｰﾀ行を配列に格納
            for( j = 0; j < 17; j++ ){
                s_SortPlDATA[iPlDataNum][j] = s_wkTmpPlData[i][j];
            }
            iPlDataNum++;

            // ﾌﾟﾚｰﾄﾃﾞｰﾀ行に対応する孔明行を探す
            for( j = 0; j < iTmpHlDataNum; j++ ){
                // ﾌﾟﾚｰﾄﾃﾞｰﾀ行に対応する孔明ﾃﾞｰﾀ行であるか
                if(s_wkTmpPlData[i][17] == s_wkTmpHlData[j][17]){

                    // 孔明ﾃﾞｰﾀ行を配列に格納
                    for( k = 0; k < 17; k++ ){
                        s_SortPlDATA[iPlDataNum][k] = s_wkTmpHlData[j][k];
                    }
                    iPlDataNum++;
                }
            }
        }
    }

    return(true);
}

//---------------------------------------------------------------------------
// 日本語関数名： 流体名　取得処理
// 概  要      ： 温度計/圧力計に出力する[XX]側のXXの部分を取得する
// 引  数      ： s_SideCd  "B":B側/"A":A側
//                bMirror　True:ミラー仕様/False:通常仕様
//
// 戻り値      ： 取得文字列
// 備  考      ：
//                2008.03.19 ﾐﾗｰ仕様対応 (新規作成)
//---------------------------------------------------------------------------
AnsiString __fastcall TBuhinhyou::ConvertRyutaiName(AnsiString s_SideCd, bool bMirror)
{

    AnsiString s_RyutaiName;
    AnsiString s_Name;

    if( ! bMirror ){
        //===================================
        // 通常使用の場合
        //===================================
        // 指定側のコード("Ａ"/"Ｂ")
        s_RyutaiName = HanToZen( s_SideCd );

    }else{
        //===================================
        // ミラー仕様の場合
        //===================================
        //-----------------------------------
        // HEADSテキストデータから流体名を取得
        //-----------------------------------
        if( s_SideCd == "B" ){
            // B側流体名
            s_RyutaiName = P_HEADS_DATA[1071].Trim();
            if( s_RyutaiName == "" ) s_RyutaiName = "高温";
        }else{
            // A側流体名
            s_RyutaiName = P_HEADS_DATA[1073].Trim();
            if( s_RyutaiName == "" ) s_RyutaiName = "低温";
        }

        //--------------------------------------
        // 流体名が省略できる場合は省略形に変換
        //--------------------------------------
        s_Name = ZenToHan( s_RyutaiName ).UpperCase();
        if( s_Name == "SEA WATER")          s_RyutaiName = "S.W.";
        if( s_Name == "FRESH WATER")        s_RyutaiName = "F.W.";
        if( s_Name == "COOLING WATER")      s_RyutaiName = "C.W.";
        if( s_Name == "LUBRICATING OIL")    s_RyutaiName = "L.O.";

        //--------------------------------------
        // 流体名を全角に変換
        //--------------------------------------
        s_RyutaiName = HanToZen( s_RyutaiName );
    }

    //---------------------------------------------------
    // 変換後の流体名文字列を返す
    //---------------------------------------------------
    return( s_RyutaiName );

}

//---------------------------------------------------------------------------
// 日本語関数名： 特記コード有無確認
// 概  要      ：
// 引  数      ： AnsiString Key   ： 特記ｺｰﾄﾞ
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： 2020.07.17追加
//---------------------------------------------------------------------------
bool __fastcall TBuhinhyou::Tokki_Code(AnsiString Key)
{

   bool bSearch;

   bSearch = false;

   if ( P_HEADS_DATA[451].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[454].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[457].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[460].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[463].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[466].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[469].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[472].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[475].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[478].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[481].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[484].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[487].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( P_HEADS_DATA[490].TrimRight() == Key ) {
      bSearch = true;
   }

   if( bSearch ){
      return(true);
   } else {
      return(false);
   }

}

