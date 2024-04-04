//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：圧力容器強度計算書(ATU52)  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.06.06
//　 更 新 日：2002.08.23
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_ATU52.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 圧力容器強度計算書(ATU52)　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Atu52(void)
{
   AnsiString s_Text;            // 汎用文字列
   AnsiString s_SheetName;       // ｼｰﾄ名称
   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo

   //*******************
   //***  ｼｰﾄの指定
   //*******************
   s_SheetName = "強度計算書52";
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_SheetName);

   //********************************************
   //***  圧力容器強度計算書　データセット
   //********************************************
   G_Log_Text = "***************  " + s_SheetName + "作成開始  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //***************************
   //***  ﾎﾞﾙﾄ材質
   //***************************
   // 『A001』
   i_RecNo = 1021;
   s_Text = "(" + G_HEADS_DATA[i_RecNo].TrimRight() + ")";
   Excel_Cell_DataSet(3, 1, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄ材質            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  ｶﾞｽｹｯﾄ周長
   //***************************
   // 『A002』
   i_RecNo = 1271;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(5, 14, s_Text);
   Excel_Cell_DataSet(9, 25, s_Text);
   G_Log_Text = "ｶﾞｽｹｯﾄ周長          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  締付重荷
   //***************************
   // 『A003』
   i_RecNo = 1273;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(5, 22, s_Text);
   Excel_Cell_DataSet(19, 9, s_Text);
   G_Log_Text = "締付重荷            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  受付面積
   //***************************
   // 『A004』
   i_RecNo = 1272;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

      Excel_Cell_DataSet(12, 9, s_Text);
      Excel_Cell_DataSet(15, 25, s_Text);

      G_Log_Text = "受付面積            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "受付面積              不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  最高使用圧力
   //***************************
   // 『A005』
   i_RecNo = 1102;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

      Excel_Cell_DataSet(12, 17, s_Text);
      Excel_Cell_DataSet(16, 25, s_Text);

      G_Log_Text = "最高使用圧力        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "最高使用圧力          不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //*** 内圧による荷重
   //***************************
   // 『A006』
   i_RecNo = 1274;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(12, 27, s_Text);
   Excel_Cell_DataSet(19, 19, s_Text);
   G_Log_Text = "内圧による荷重      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** 合計荷重
   //***************************
   // 『A007』
   i_RecNo = 1275;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(19, 29, s_Text);
   Excel_Cell_DataSet(26, 9, s_Text);
   G_Log_Text = "合計荷重            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** ﾎﾞﾙﾄ本数による定数
   //***************************
   // 『A008』
   i_RecNo = 1112;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(24, 8, s_Text);
   Excel_Cell_DataSet(24, 31, s_Text);
   Excel_Cell_DataSet(26, 18, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄ本数による定数  RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** ﾎﾞﾙﾄﾋﾟｯﾁ垂直
   //***************************
   // 『A009』
   i_RecNo = 1113;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(26, 27, s_Text);
   Excel_Cell_DataSet(31, 30, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄﾋﾟｯﾁ垂直        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** 総ﾎﾞﾙﾄﾋﾟｯﾁ垂直
   //***************************
   // 『A010』
   i_RecNo = 1114;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(27, 20, s_Text);
   Excel_Cell_DataSet(32, 30, s_Text);
   G_Log_Text = "総ﾎﾞﾙﾄﾋﾟｯﾁ垂直      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** ﾎﾞﾙﾄ一本の最大荷重
   //***************************
   // 『A011』
   i_RecNo = 1116;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(41, 8, s_Text);
   Excel_Cell_DataSet(29, 8, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄ一本最大荷重    RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** ﾎﾞﾙﾄ許容応力
   //***************************
   // 『B001』
   i_RecNo = 1118;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(42, 16, s_Text);
   Excel_Cell_DataSet(44, 28, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄ許容応力        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** ﾎﾞﾙﾄ必要径
   //***************************
   // 『B002』
   i_RecNo = 1117;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(41, 24, s_Text);
   G_Log_Text = "ﾎﾞﾙﾄ必要径          RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** ﾌﾚｰﾑ締付ﾎﾞﾙﾄ呼び径
   //***************************
   // 『B003』
   i_RecNo = 1019;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.10 UNﾈｼﾞ訂正
   //if (s_Text == "42") {
   if (s_Text == "1.630") {
   //   s_Text = "1 5/8-8UN";
      //2004.01.24
      //s_Text = "1 5/8-8UN（谷径";
      s_Text = "1 5/8""-8UN（谷径";
      //**********
   //} else if (s_Text == "52"){
   } else if (s_Text == "2.000"){
   //   s_Text = "2-8UN";
      //2004.01.24
      //s_Text = "2-8UN（谷径";
      s_Text = "2""-8UN（谷径";
      //**********
   } else {
   //   s_Text = "M" + FormatFloat("0",s_Text.ToDouble());
      s_Text = "M" + FormatFloat("0",s_Text.ToDouble()) + " （谷径";
   }
   //********************
   Excel_Cell_DataSet(46, 4, s_Text);
   G_Log_Text = "締付ﾎﾞﾙﾄ呼び径      RecNo:" + FormatFloat("0000",i_RecNo) + "『" + G_HEADS_DATA[i_RecNo].TrimRight()
              + "』より『" +  s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** ﾎﾞﾙﾄ谷径
   //***************************
   // 『B004』
   i_RecNo = 1119;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.10 UNﾈｼﾞ訂正
   //Excel_Cell_DataSet(46, 11, s_Text);
   Excel_Cell_DataSet(46, 14, s_Text);
   //********************
   G_Log_Text = "ﾎﾞﾙﾄ谷径            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //*** 片側ﾎﾞﾙﾄ本数
   //***************************
   // 『B005』
   i_RecNo = 1020;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = FormatFloat("0", s_Text.ToDouble() * 2);
   //2003.09.10 UNﾈｼﾞ訂正
   //Excel_Cell_DataSet(46, 23, s_Text);
   Excel_Cell_DataSet(46, 24, s_Text);
   //********************
   G_Log_Text = "片側ﾎﾞﾙﾄ本数        RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   // 『B006』
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E   
   Excel_Cell_DataSet(56, 26, s_Text);
   G_Log_Text = "製造番号           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // 『B007』
   i_RecNo = 302;
   if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   } else {
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(56, 3, s_Text);
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
      
   return true;
}