//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：申請書(他)
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

#include "Bas_ShinseiNonNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// 日本語関数名： 申請書(他NK)  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShinseiNonNK(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strWriteText;   // 描画データ
   int nRecNo;                // HEADSデータ配列番号
   //2015.09.02 試験水圧追加
   AnsiString strText;        // 中間データ
   //***********************　

   // シートの指定
   strSheetName = "他申";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『申請書(他申)』作成開始    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  規格名 二箇所に書出し
   //***************************
   nRecNo = 29;
   strWriteText = Search_KIKAKU_CODE(G_HEADS_DATA[nRecNo].TrimRight(),true,true, G_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(1, 4, strWriteText);
   Excel_Cell_DataSet(27, 6, strWriteText);
   G_Log_Text = "他申：規格名        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器名称
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(18, 6, strWriteText);
   G_Log_Text = "他申：機器名称        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム巾
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(19, 6, strWriteText);
   G_Log_Text = "他申：フレーム巾        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム高
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(19, 9, strWriteText);
   G_Log_Text = "他申：フレーム高        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム長
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(19, 13, strWriteText);
   G_Log_Text = "他申：フレーム長        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(20, 8, strWriteText);
   G_Log_Text = "他申：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器重量
   //***************************
   nRecNo = 1029;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(21, 8, strWriteText);
   G_Log_Text = "他申：機器重量        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  電熱面積
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(22, 8, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "他申：電熱面積        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(24, 6, strWriteText);
   G_Log_Text = "NK-FAX：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  注文主
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(25, 6, strWriteText);
   G_Log_Text = "他申：注文主        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  船番
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(26, 6, strWriteText);
   G_Log_Text = "他申：船番        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //2015.09.02 試験水圧追加
   //***************************
   //***  試験水圧
   //***************************
   strWriteText = "Hydraulic Test  (A SIDE: ";
   nRecNo = 832;
   strText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText += FormatFloat("##0.00",StrToCurr(strText));
   strWriteText += " MPa , B SIDE: ";
   nRecNo = 827;
   strText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText += FormatFloat("##0.00",StrToCurr(strText));
   strWriteText += " MPa )";
   Excel_Cell_DataSet(28, 6, strWriteText);
   G_Log_Text = "他申：試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);
   //***********************　

   //2014.04.15 プレート材質追加
   //***************************
   //***  プレート材質
   //***************************
   strWriteText = "Plate Material : ";
   nRecNo = 43;
   strWriteText += Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(31, 6, strWriteText);
   G_Log_Text = "他申：プレート材質        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);
   //***************************　　

   // DATE
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 1 << 13);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 1, 13);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('他-FAX'!O48,'他-FAX'!Q48,'他-FAX'!S48),\"mmm.dd.yyyy\")" );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value",OleVariant("=TEXT(DATE('他-FAX'!O48,'他-FAX'!Q48,'他-FAX'!S48),\"mmm.dd.yyyy\")" ));


   // DATE2
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 30 << 6);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 30, 6);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('他-FAX'!O50,'他-FAX'!Q50,'他-FAX'!S50),\"mmm.dd.yyyy\")" );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value",OleVariant("=TEXT(DATE('他-FAX'!O50,'他-FAX'!Q50,'他-FAX'!S50),\"mmm.dd.yyyy\")") );

   return true;
}
