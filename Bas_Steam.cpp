//---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：ｽﾁｰﾑ送入量計算書(Steam)  作成モジュール
//　 作 成 者：T.Kawai
//　 作 成 日：2002.06.10
//　 更 新 日：2002.09.20
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_Steam.h"

#define MdevideOnF(a,b)  ( ( (a) != 0.0f && (b) != 0.0f )? ( (a)/(b) ) : (0.0f) )
#define EXCEL_SCP_SHEETNAME    "ｽﾁ-ﾑ比重"          // EXCELシート名(ｽﾁｰﾑ比重)
#define EXCEL_SCP_NAIKEI       "内径変換"          // EXCELシート名(内径変換)
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： ｽﾁｰﾑ送入量計算書(Steam)　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Steam(void)
{
   AnsiString s_Text;            // 汎用文字列
   AnsiString s_SheetName;       // ｼｰﾄ名称
   AnsiString s_S1Nossle;        // S1ﾉｽﾞﾙ種類
   AnsiString s_S2Nossle;        // S2ﾉｽﾞﾙ種類
   AnsiString s_S3Nossle;        // S3ﾉｽﾞﾙ種類
   AnsiString s_S4Nossle;        // S4ﾉｽﾞﾙ種類
   AnsiString s_E1Nossle;        // E1ﾉｽﾞﾙ種類
   AnsiString s_E2Nossle;        // E2ﾉｽﾞﾙ種類
   AnsiString s_E3Nossle;        // E3ﾉｽﾞﾙ種類
   AnsiString s_E4Nossle;        // E4ﾉｽﾞﾙ種類

   int        i_RecNo;           // HEADSﾃﾞｰﾀ ﾚｺｰﾄﾞNo
   int        i_Errflg;          // ｴﾗｰﾌﾗｸﾞ
   float      f_Hijyu;           // 比重量
   float      f_Temp;            // 使用温度
   float      f_Naikei;          // 配管の内径
   float      f_Sounyu;          // ｽﾁｰﾑ送入量
   int        i_Kikaku;          // 入口ノズルの規格　0:JIS 1:ANSI 2:サニタリ

   f_Hijyu = f_Naikei = f_Temp = f_Sounyu = 0;

   //*******************
   //***  ｼｰﾄの指定
   //*******************
   s_SheetName = "スチーム";
   // ｼｰﾄの指定
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_SheetName);

   //********************************************
   //***  ｽﾁｰﾑ送入量計算書　データセット
   //********************************************
   G_Log_Text = "***************  " + s_SheetName + "作成開始  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   i_Errflg = 0;

   //***************************
   //***  ｽﾁｰﾑ比重量
   //***************************
   i_RecNo = 1101;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();

   if (AllFloatChk(s_Text.c_str(),0) == 0){
      f_Temp = StrToFloat( s_Text );
      f_Temp = FourDownFiveUp( f_Temp, 1 );
      s_Text = FloatToStr( f_Temp );

      if( HijyuGet(&s_Text) == 0 )
      {
         if (AllFloatChk(s_Text.c_str(),0) == 0){
            f_Hijyu = StrToFloat( s_Text );
            if (f_Hijyu == 0){
               s_Text = "   -";
            } else {
               s_Text = FormatFloat("#,###,##0.0000",StrToFloat(s_Text));
            }
            Excel_Cell_DataSet(13, 21, s_Text);
            //2004.07.16 内径単位変更
            //Excel_Cell_DataSet(20, 15, s_Text);
            Excel_Cell_DataSet(20, 16, s_Text);
            //***********************
            G_Log_Text = "ｽﾁｰﾑ比重量         『" + s_Text + "』をセット。";
            Write_Log(G_Log_Text);
         } else {
            G_ErrLog_Text = "ｽﾁｰﾑ比重量            不正な値『" + s_Text + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
            i_Errflg =1;
         }
      } else {
         G_ErrLog_Text = "ｽﾁｰﾑ比重量       温度の値に対応する比重量がありません。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
         i_Errflg =1;
      }
   } else {
      G_ErrLog_Text = "ｽﾁｰﾑ比重量       温度の値に対応する比重量がありません。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      i_Errflg =1;
   }

   //***************************
   //***  使用温度
   //***************************
   i_RecNo = 1101;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      f_Temp = StrToFloat( s_Text );
      f_Temp = FourDownFiveUp( f_Temp, 1 );
      if (f_Temp == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("#,###,##0",f_Temp);
      }
      Excel_Cell_DataSet(13, 39, s_Text);
      G_Log_Text = "使用温度            RecNo:" + FormatFloat("0000",i_RecNo) + "『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "使用温度              不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
   }

   //***************************
   //***  配管の内径
   //***************************
   if (StrToDblDef(G_HEADS_DATA[852].TrimRight(),0) >= StrToDblDef(G_HEADS_DATA[853].TrimRight(),0)) {

      //******************************************
      //***  (B入口温度) >= (A入口温度)のとき
      //******************************************
      s_S1Nossle = G_HEADS_DATA[891].TrimRight();      // S1ﾉｽﾞﾙ種類
      s_S3Nossle = G_HEADS_DATA[921].TrimRight();      // S3ﾉｽﾞﾙ種類
      s_E1Nossle = G_HEADS_DATA[951].TrimRight();      // E1ﾉｽﾞﾙ種類
      s_E3Nossle = G_HEADS_DATA[981].TrimRight();      // E3ﾉｽﾞﾙ種類

      if (s_S1Nossle == "1"){
         s_Text = G_HEADS_DATA[892].TrimRight();       // S1ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[893] );

      } else if (s_S1Nossle != "1" && s_S3Nossle == "1"){
         s_Text = G_HEADS_DATA[922].TrimRight();       // S3ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[923] );

      } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle == "1"){
         s_Text = G_HEADS_DATA[952].TrimRight();       // E1ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[953] );

      } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle == "1"){
         s_Text = G_HEADS_DATA[982].TrimRight();       // E3ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[983] );

      } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle != "1"){
         s_Text = "";
      }

   } else {
      //******************************************
      //***  (B入口温度) < (A入口温度)のとき
      //******************************************
      s_S2Nossle = G_HEADS_DATA[906].TrimRight();      // S2ﾉｽﾞﾙ種類
      s_S4Nossle = G_HEADS_DATA[936].TrimRight();      // S4ﾉｽﾞﾙ種類
      s_E2Nossle = G_HEADS_DATA[966].TrimRight();      // E2ﾉｽﾞﾙ種類
      s_E4Nossle = G_HEADS_DATA[996].TrimRight();      // E4ﾉｽﾞﾙ種類

      if (s_S2Nossle == "1"){
         s_Text = G_HEADS_DATA[907].TrimRight();       // S2ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[908] );

      } else if (s_S2Nossle != "1" && s_S4Nossle == "1"){
         s_Text = G_HEADS_DATA[937].TrimRight();       // S4ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[938] );

      } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle == "1"){
         s_Text = G_HEADS_DATA[967].TrimRight();       // E2ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[968] );

      } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E3Nossle == "1"){
         s_Text = G_HEADS_DATA[997].TrimRight();       // E4ﾉｽﾞﾙ口径
         i_Kikaku = GetNzKikaku( G_HEADS_DATA[998] );

      } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle != "1"){
         s_Text = "";
      }

   }

   if (AllFloatChk(s_Text.c_str(),0) == 0){
      f_Naikei = StrToFloat( s_Text );
      if (f_Naikei == 0){
         s_Text = "   -";
      } else {
         s_Text = FormatFloat("0", f_Naikei);
         NaikeiGet( &s_Text, i_Kikaku );
         if (s_Text != "" ) {
            //2004.07.16 内径単位変更
            //s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
            s_Text = FormatFloat("##,###,##0.0",StrToFloat(s_Text)*10);
            //***********************
            f_Naikei = StrToFloat( s_Text );
         } else {
            G_ErrLog_Text = "配管の内径            変換できませんでした『" + FormatFloat("0", f_Naikei) + "』 です。";
            Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
            Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
            i_Errflg =1;
         }
      }
      Excel_Cell_DataSet(15, 36, s_Text);
      Excel_Cell_DataSet(20, 26, s_Text);
      G_Log_Text = "配管の内径         『" + s_Text + "』をセット。";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "配管の内径            不正な値『" + s_Text + "』 です。";
      Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
      Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      i_Errflg =1;
   }

   //***************************
   //***  ｽﾁｰﾑ送入量
   //***************************
   if( i_Errflg == 0 )
   {
      //2004.07.16 内径単位変更
      //f_Sounyu = 0.28 * 30 * f_Hijyu * f_Naikei * f_Naikei;
      f_Sounyu = 0.0028 * 20 * f_Hijyu * f_Naikei * f_Naikei;
      //***********************
      f_Sounyu = FourDownFiveUp( f_Sounyu, 1 );
      if (AllFloatChk( FloatToStr(f_Sounyu).c_str(), 0) == 0){
         s_Text = FormatFloat("#,###,##0",f_Sounyu);

         Excel_Cell_DataSet(22, 9, s_Text);

         G_Log_Text = "ｽﾁｰﾑ送入量         『" + s_Text + "』をセット。";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "ｽﾁｰﾑ送入量              不正な値『" + s_Text + "』 です。";
         Write_Error_Log(G_ErrLog_Text);     // ﾃｷｽﾄｴﾗｰﾛｸﾞ
         Excel_Err_DataSet(G_ErrLog_Text);   // ｴｸｾﾙｴﾗｰﾛｸﾞ
      }
   }

   //***************************
   //***  製造番号
   //***************************
   // 『B006』
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 製造番号桁数追加対応 Mod_E   
   Excel_Cell_DataSet(55, 41, s_Text);
   G_Log_Text = "製造番号           『" + s_Text + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // 『A005』
   i_RecNo = 302;
   if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   } else {
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(55, 12, s_Text);
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

//---------------------------------------------------------------------------
// 日本語関数名： ｽﾁｰﾑ比重量取得処理
// 概  要      ： ｽﾁｰﾑ比重のﾜｰｸｼｰﾄを参照し、TPVH(温度)に対応する比重量を取得
// 引  数      ： AnsiString inText  // TPVH(温度)
// 戻り値      ： AnsiString inText  // 比重量
// 備  考      ： なし
//---------------------------------------------------------------------------
int __fastcall HijyuGet(AnsiString *inText)
{
   AnsiString s_Old_SheetName;
   AnsiString s_CellText;

   int i;

   // 現在のシート名称を保存
   s_Old_SheetName = exWorksheet.OlePropertyGet("Name");

   // ｼｰﾄの指定(ｽﾁｰﾑ比重)
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SCP_SHEETNAME);

   // 最終行にセット
   s_CellText = "Def";
   i = 4;

   while (s_CellText != ""){
      exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i << 1);
      s_CellText = exWorkCell.Exec(PropertyGet("Value"));

      if( s_CellText == *inText ){
         *inText = exWorksheet.Exec(PropertyGet("Cells") << i << 2);

         // 保存しておいたシートを再指定
         exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

         return 0;
      }

      i++;
   }

   *inText = "";

   // 保存しておいたシートを再指定
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

   return 1;
}

//---------------------------------------------------------------------------
// 日本語関数名： ガス管内径取得処理
// 概  要      ： ガス管内径のﾜｰｸｼｰﾄを参照し、ノズル口径に対応する内径を取得
// 引  数      ： AnsiString inText  // ノズル口径
// 戻り値      ： AnsiString inText  // 内径
// 備  考      ： なし
//---------------------------------------------------------------------------
int __fastcall NaikeiGet(AnsiString *inText, int i_Kikaku)
{
   AnsiString s_Old_SheetName;
   AnsiString s_CellText;

   int i;

   // 現在のシート名称を保存
   s_Old_SheetName = exWorksheet.OlePropertyGet("Name");

   // ｼｰﾄの指定(内径変換)
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << EXCEL_SCP_NAIKEI);

   // 最終行にセット
   s_CellText = "Def";
   if (i_Kikaku == 0 ) {                // JIS
      i = 4;
   } else if ( i_Kikaku == 1 ) {        // ANSI
      i = 19;
   } else {                             // サニタリ
      i = 34;
   }

   while (s_CellText != ""){
      exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i << 1);
      s_CellText = exWorkCell.Exec(PropertyGet("Value"));

      if( s_CellText == *inText ){
         *inText = exWorksheet.Exec(PropertyGet("Cells") << i << 2);

         // 保存しておいたシートを再指定
         exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

         return 0;
      }

      i++;
   }

   *inText = "";

   // 保存しておいたシートを再指定
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

   return 1;
}

//-----------------------------------------------------------------------------
// 日本語関数名： 四捨五入処理ﾓｼﾞｭｰﾙ
// 概  要      ：
// パラメータ  ： なし
// 戻り値      ： 0:正常動作 1:異常動作
// 機能説明    ：
// 備  考      ：
//-----------------------------------------------------------------------------
double  __fastcall  FourDownFiveUp( double  data,   // In.  変換前ﾃﾞｰﾀ
                                    int     col     // In.  対象小数桁数(1～)
                                  )
{
    // FourDownFiveUp変数定義
            int     lop;        // ﾙｰﾌﾟｶｳﾝﾀ
            long    lwork;      // ﾜｰｸﾃﾞｰﾀ
            double  ret;        // ﾘﾀｰﾝﾃﾞｰﾀ
            double  dwork;      // ﾜｰｸﾃﾞｰﾀ
            double  mod;        // ﾜｰｸﾃﾞｰﾀ
            double  fixed;      // 定数

    // FourDownFiveUp変数初期化
    fixed = 1.0f;

    // 桁上がり定数ｾｯﾄ
    for ( lop = 1; lop < col; lop++ )
    {
        fixed *= 10.0f;
    }

    // 小数2桁まで整数化
    dwork = data * fixed;
    lwork = (long)dwork;

    // 四捨五入
    mod = (double)( dwork - (double)lwork );
    if ( (long)(mod*10.0f) >= 5L )
    {
        lwork += 1L;
    }

    // 変換値ｾｯﾄ
    ret = MdevideOnF( (double)lwork, fixed );

    // ﾘﾀｰﾝﾃﾞｰﾀ[変換数値ﾃﾞｰﾀ(double型)]
    return  ret;
}



