//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：圧力容器強度計算書(ATU21)  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.06.06
//　 更 新 日：2002.09.20
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_ATU21.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 圧力容器強度計算書(ATU21)　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Atu21(void)
{
   TQuery *wkQuery;
   wkQuery = new TQuery(DataModule1);

   AnsiString s_Text;            // 汎用文字列
   AnsiString s_SheetName;       // ｼｰﾄ名称
   AnsiString s_SQL;

   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo

   //*******************
   //***  ｼｰﾄの指定
   //*******************
   s_SheetName = "強度計算書21";
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_SheetName);

   //********************************************
   //***  圧力容器強度計算書　データセット
   //********************************************
   G_Log_Text = "***************  " + s_SheetName + "作成開始  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //************************
   //***  角型隔板式
   //************************
   // 『A001』
   i_RecNo = 33;
   if (G_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
   } else {
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(3, 11, s_Text);
   G_Log_Text = "角型隔板式          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  使用圧力 熱源側
   //***************************
   // 『A002』
   i_RecNo = 1100;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( s_Text.ToDouble() >= 999) {
        s_Text = "大気開放";
      } else {
        s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(5, 8, s_Text);
      G_Log_Text = "使用圧力 熱源側     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "使用圧力 熱源側       不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  使用圧力 被熱源側
   //***************************
   // 『A003』
   i_RecNo = 1098;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( s_Text.ToDouble() >= 999) {
        s_Text = "大気開放";
      } else {
        s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(4, 8, s_Text);
      G_Log_Text = "使用圧力 被加熱側   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "使用圧力 被加熱側     不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  使用温度 熱源側
   //***************************
   // 『A004』
   i_RecNo = 1101;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(5, 35, s_Text);
      G_Log_Text = "使用温度 熱源側     RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "使用温度 熱源側       不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  使用温度 被熱源側
   //***************************
   // 『A005』
   i_RecNo = 1099;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(4, 35, s_Text);
      G_Log_Text = "使用温度 被加熱側   RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "使用温度 被加熱側     不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  フレーム材質
   //***************************
   // 『A006』
   switch (G_HEADS_DATA[37].ToIntDef(0)) {
      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
         s_Text = G_HEADS_DATA[34].TrimRight();
         //2005.03.22 RX-10B追加
         //if ((s_Text.SubString(1,2) == "UX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==0) ||       //UX-01
         //    (s_Text.SubString(1,2) == "LX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ) {     //LX-10
         if ((s_Text.SubString(1,2) == "UX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==0) ||       //UX-01
             (s_Text.SubString(1,2) == "LX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ||      //LX-10
             (s_Text.SubString(1,2) == "RX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ) {     //RX-10
         //*********************
            s_Text = "(ＳＵＳ３０４)";

         } else {
            s_Text = "(ＳＳ４００)";
         }

         break;
      default :
         i_RecNo = 1044;   // SPFRM
         s_Text = G_HEADS_DATA[i_RecNo].TrimRight();

         if (s_Text == "0000" || s_Text == "1040"){
            s_Text = "(ＳＳ４００)";
         } else {
            s_SQL = "";
            s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
            s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

            wkQuery->Close();
            wkQuery->DatabaseName = ODBC_DSN_MAT;
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(s_SQL);
            wkQuery->Open();
            wkQuery->First();

            if(wkQuery->Eof){
               s_Text = "";
            } else {
               s_Text = "(" + wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight() + ")";
            }
         }

         break;
   }

   Excel_Cell_DataSet(7, 1, s_Text);
   G_Log_Text = "フレーム材質     『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //*********************************
   //***  最高使用圧力 (A007,A015)
   //*********************************
   // 『A007』,『A015』
   i_RecNo = 1102;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(11, 32, s_Text);
      Excel_Cell_DataSet(28, 18, s_Text);
      G_Log_Text = "最高使用圧力        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "最高使用圧力          不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  ｽﾃｰの平均ﾋﾟｯﾁ
   //***************************
   // 『A008』,『A010』,『A013』
   i_RecNo = 1103;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(15, 32, s_Text);
   Excel_Cell_DataSet(17, 33, s_Text);
   Excel_Cell_DataSet(28, 10, s_Text);
   G_Log_Text = "ｽﾃｰの平均ﾋﾟｯﾁ       RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  最大円の径
   //***************************
   // 『A009』,『A011』
   i_RecNo = 1104;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(17, 20, s_Text);
   Excel_Cell_DataSet(20, 32, s_Text);
   G_Log_Text = "最大円の径          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  許容応力
   //***************************
   // 『A012』,『A014』
   i_RecNo = 1120;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(24, 33, s_Text);
   Excel_Cell_DataSet(29, 22, s_Text);
   G_Log_Text = "許容応力            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  必要板厚
   //***************************
   // 『A016』
   i_RecNo = 1105;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(31, 10, s_Text);
   G_Log_Text = "必要板厚            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾌﾚｰﾑ板厚
   //***************************
   // 『A017』
   i_RecNo = 1106;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(33, 24, s_Text);
   G_Log_Text = "ﾌﾚｰﾑ板厚            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾎﾞﾙﾄﾋﾟｯﾁ水平
   //***************************
   // 『A018』
   i_RecNo = 1115;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(22,  6, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄﾋﾟｯﾁ水平        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ﾎﾞﾙﾄﾋﾟｯﾁ垂直
   //***************************
   // 『A019』
   i_RecNo = 1113;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(23,  6, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄﾋﾟｯﾁ垂直        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  大基安第
   //***************************
   // 『B001』
   i_RecNo = 1109;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 10, s_Text);
   G_Log_Text = "大基安第            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  大基安号
   //***************************
   // 『B002』
   i_RecNo = 1110;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 13, s_Text);
   G_Log_Text = "大基安号            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  大期安()
   //***************************
   // 『B003』
   i_RecNo = 1111;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 18, s_Text);
   G_Log_Text = "大基安()            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  型　式
   //***************************
   // 『B004』
   s_Text = G_HEADS_DATA[1256].TrimRight() + "-" + G_HEADS_DATA[1257].TrimRight();
   Excel_Cell_DataSet(42, 3, s_Text);
   G_Log_Text = "型  式             『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //2007.07.09 出力内容変更
   ////***************************
   ////***  板　厚
   ////***************************
   //// 『B005』
   //i_RecNo = 1270;
   //s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(42, 16, s_Text);
   //G_Log_Text = "板  厚              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   //Write_Log(G_Log_Text);
   //***************************
   //***  寸法及び板厚
   //***************************
   // 『B005』
   i_RecNo = 1268;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1269;
   s_Text = s_Text + "×" + G_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1270;
   s_Text = s_Text + "×t" + G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(42, 16, s_Text);
   G_Log_Text = "寸法及び板厚       『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);
   //***********************

   //***************************
   //***  材　質
   //***************************
   // 『B006』
   i_RecNo = 43;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   Excel_Cell_DataSet(42, 29, s_Text);
   G_Log_Text = "材  質              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  最高使用圧力
   //***************************
   // 『B007』
   i_RecNo = 1107;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(44, 3, s_Text);
      G_Log_Text = "最高使用圧力        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "最高使用圧力          不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  最高使用温度
   //***************************
   // 『B008』
   i_RecNo = 1108;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(44, 16, s_Text);
      G_Log_Text = "最高使用温度        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "最高使用温度          不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  本器最高使用圧力
   //***************************
   // 『B009』
   i_RecNo = 1102;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(44, 29, s_Text);
      G_Log_Text = "本器最高使用圧力    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "本器最高使用圧力      不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  ﾌﾟﾚｰﾄ板厚
   //***************************
   // 『B010』
   i_RecNo = 1270;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(46, 20, s_Text);
   G_Log_Text = "板  厚              RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   // 『B011』
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E
   Excel_Cell_DataSet(50, 32, s_Text);
   G_Log_Text = "製造番号           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // 『B012』
   i_RecNo = 302;
   if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   } else {
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(50, 3, s_Text);
   G_Log_Text = "HEADS VER          『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //********************************************
   //***
   //***  ＰＤＦ出力用　設定内容書き込み
   //***
   //********************************************
   AnsiString s_Old_SheetName;
   AnsiString s_CellText;

   int i_ConfRow;

   // 現在のシート名称を保存
   s_Old_SheetName = exWorksheet.OlePropertyGet("Name");
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "ＰＤＦ設定");

   // 最終行にセット
   s_CellText = "Def";
   i_ConfRow = 1;
   while (s_CellText != ""){
      exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i_ConfRow << 2);
      s_CellText = exWorkCell.Exec(PropertyGet("Value"));

      i_ConfRow++;
   }

   // ＰＤＦ出力対象ｼｰﾄとして設定
   exWorkCell.Exec(PropertySet("Value") << s_SheetName);

   // 保存しておいたシートを再指定
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

   G_Log_Text = "***************  " + s_SheetName + "作成終了  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   delete wkQuery;
   
   return true;
}
