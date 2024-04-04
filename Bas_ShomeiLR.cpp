//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：LR証
//　 作 成 者：S.Ogita
//　 作 成 日：2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "SINSEICom.h"  // 申請書共通関数定義用

#include "Bas_ShomeiLR.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： LR証  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShomeiLR(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strWriteText;   // 描画データ
   AnsiString strTmp;         // 一時変数   
   int nRecNo;                // HEADSデータ配列番号

   // シートの指定
   strSheetName = "LR証";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『LR証』作成開始    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(15, 11, strWriteText);
   Excel_Cell_DataSet(27,  5, strWriteText);
   Excel_Cell_DataSet(48,  7, strWriteText);      
   G_Log_Text = "LR証：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  船番
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strTmp = strWriteText;
   //Excel_Cell_DataSet(17, 12, strWriteText);
   //G_Log_Text = "LR証：船番        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   //Write_Log(G_Log_Text);

   //***************************
   //***  納入先
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].TrimRight();
   Excel_Cell_DataSet(14, 4, strWriteText);
   G_Log_Text = "LR証：納入先        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);
   strTmp += " of " + strWriteText;
   strWriteText = strTmp;
   Excel_Cell_DataSet(17,12, strWriteText);
   G_Log_Text = "LR証：納入先        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);


   //***************************
   //***  機器名称
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(25, 5, strWriteText);   
   G_Log_Text = "LR証：機器名称        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  型式
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(26, 4, strWriteText);
   G_Log_Text = "LR証：型式        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号("-"なし)
   //***************************
   strWriteText = GetSerialNo();
   Excel_Cell_DataSet(27, 13, strWriteText);
   G_Log_Text = "LR証：製造番号(- なし)        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム巾(桁区切り使用)
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(28, 3, strWriteText);
   // DATE
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 28 << 3);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 28, 3);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "LR証：フレーム巾        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム高(桁区切り使用)
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   G_Log_Text = "LR証：フレーム高(桁区切り使用 1/2)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(28, 6, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 28 << 6);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 28, 6);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );
   G_Log_Text = "LR証：フレーム高        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム長(桁区切り使用)
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(28, 8, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 28 << 8);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 28, 8);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "LR証：フレーム長        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器重量
   //***************************
   nRecNo = 1029;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 11, strWriteText);
   G_Log_Text = "LR証：機器重量        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  電熱面積
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 6, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR証：電熱面積        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   // ----------------------------------------   
   // 少々特殊
   // 台数(英)と台数は結合して、Excelに描画する
   //***************************
   //***  台数(英)
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText = NumeralJtoEHeadCapital(strWriteText);
   G_Log_Text = "LR証：台数(英)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp = strWriteText;
   strTmp += "(";

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "LR証：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   strTmp += ")";
   Excel_Cell_DataSet(31, 2, strTmp);
   // ----------------------------------------

   // ----------------------------------------   
   // 少々特殊
   // プレート材質(英文)とプレート材質は結合して、Excelに描画する
   //***************************
   //***  プレート材質(英文)
   //***************************
   nRecNo = 43;
   strWriteText = ConvertMaterialJtoE(G_HEADS_DATA[nRecNo].Trim());
   G_Log_Text = "LR証：プレート材質(英文)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp = strWriteText;
   strTmp += "(";

   //***************************
   //***  プレート材質
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   G_Log_Text = "LR証：プレート材質        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   strTmp += ")";

   Excel_Cell_DataSet(35, 3, strTmp);
   // ----------------------------------------

   //***************************
   //***  B側液名(短)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(45, 2, strWriteText);
   G_Log_Text = "LR証：B側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側試験水圧
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(45, 5, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR証：B側試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側液名(短)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(45, 9, strWriteText);
   G_Log_Text = "LR証：A側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側試験水圧
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(45, 11, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR証：A側試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  最高試験水圧
   //***************************
   strWriteText = GetHighPressureValueAorB();
   Excel_Cell_DataSet(50, 4, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR証：最高試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);


   return true;
}
