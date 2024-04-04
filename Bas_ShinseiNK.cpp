//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：申請書(NK)
//　 作 成 者：S.Ogita
//　 作 成 日：2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "SINSEICom.h"  // 申請書共通関数定義用

#include "DataModule.h"

#include "Bas_ShinseiNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 申請書(他NK)  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShinseiNK(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strWriteText;   // 描画データ
   int nRecNo;                // HEADSデータ配列番号

   // シートの指定
   strSheetName = "NK申";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『申請書(NK申)』作成開始    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器名称
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(21, 6, strWriteText);
   G_Log_Text = "NK申：機器名称        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  形式
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(22, 6, strWriteText);
   G_Log_Text = "NK申：形式        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(22, 15, strWriteText);
   G_Log_Text = "NK申：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  電熱面積
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(23, 8, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK申：電熱面積        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   // 2013.04.03 書類書式変更
   //***************************
   //***  プレート材質
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(24, 6, strWriteText);
   G_Log_Text = "NK申：プレート材質        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //**************************
   //***  ガスケット材質
   //***************************
   nRecNo = 835;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   nRecNo = 836;
   if ( G_HEADS_DATA[835].Trim() != G_HEADS_DATA[nRecNo].Trim() ) {
	  strWriteText += "／";
	  strWriteText += Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   }
   Excel_Cell_DataSet(25, 6, strWriteText);
   G_Log_Text = "NK申：ガスケット材質        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);
   // ***********************

   //***************************
   //***  B側液名(短)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   // 2013.04.03 書類書式変更
   //Excel_Cell_DataSet(24, 6, strWriteText);
   Excel_Cell_DataSet(31, 8, strWriteText);
   // ***********************
   G_Log_Text = "NK申：B側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側試験水圧
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   // 2013.04.03 書類書式変更
   //Excel_Cell_DataSet(24, 9, FormatFloat("##0.00",StrToCurr(strWriteText)));
   Excel_Cell_DataSet(31, 10, FormatFloat("##0.00",StrToCurr(strWriteText)));
   // ***********************
   G_Log_Text = "NK申：B側試験水圧       RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側液名(短)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   // 2013.04.03 書類書式変更
   //Excel_Cell_DataSet(24, 11, strWriteText);
   Excel_Cell_DataSet(32, 8, strWriteText);
   // ***********************
   G_Log_Text = "NK申：A側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側試験水圧
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   // 2013.04.03 書類書式変更
   //Excel_Cell_DataSet(24, 15, FormatFloat("##0.00",StrToCurr(strWriteText)));
   Excel_Cell_DataSet(32, 10, FormatFloat("##0.00",StrToCurr(strWriteText)));
   // ***********************
   G_Log_Text = "NK申：A側試験水圧       RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(27, 6, strWriteText);
   G_Log_Text = "NK申：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  注文主
   //***************************
   nRecNo = 1063;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 6, strWriteText);
   G_Log_Text = "NK申：注文主       RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  船番
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 8, strWriteText);
   G_Log_Text = "NK申：船番       RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);


   // 標題年月日マクロ
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 6 << 11);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!K47" );
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 6, 11);
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!K47") );

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 6 << 13);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!M47" );

   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 6, 13);
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!M47") );

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 6 << 15);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!O47" );

   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 6, 15);
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!O47") );

   // 検 査 試 験 実 施 日 時 マクロ
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 33 << 7);
   //exWorkCell.Exec(PropertySet("Value") << "=DATE('NK-FAX'!K49-1988,'NK-FAX'!M49,'NK-FAX'!O49)" );
   //Windows 2008Server 移植対応
   // 2013.04.03 書類書式変更
   //exWorkCell = exWorksheet.OlePropertyGet("Cells", 33, 7);
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 34, 7);
   // ***********************
   exWorkCell.OlePropertySet("Value",OleVariant("=DATE('NK-FAX'!K49-1988,'NK-FAX'!M49,'NK-FAX'!O49)" ));

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 33 << 9);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!M49" );
   //Windows 2008Server 移植対応
   // 2013.04.03 書類書式変更
   //exWorkCell = exWorksheet.OlePropertyGet("Cells", 33, 9);
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 34, 9);
   // ***********************
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!M49") );

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 33 << 11);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!O49" );
   //Windows 2008Server 移植対応
   // 2013.04.03 書類書式変更
   //exWorkCell = exWorksheet.OlePropertyGet("Cells", 33, 11);
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 34, 11);
   // ***********************
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!O49") );

   return true;

}
