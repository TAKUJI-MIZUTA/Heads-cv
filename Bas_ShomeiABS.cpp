//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：ABS証
//　 作 成 者：S.Ogita
//　 作 成 日：2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "SINSEICom.h"  // 申請書共通関数定義用

#include "Bas_ShomeiABS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： ABS証  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShomeiABS(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strWriteText;   // 描画データ
   AnsiString strTmp;         // 一時変数   
   int nRecNo;                // HEADSデータ配列番号

   // シートの指定
   strSheetName = "ABS証";   
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『ABS証』作成開始    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(12, 8, strWriteText);
   G_Log_Text = "ABS証：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  納入先
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(14, 2, strWriteText);
   G_Log_Text = "ABS証：納入先        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  船番
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(14, 8, strWriteText);
   G_Log_Text = "ABS証：船番        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器重量
   //***************************
   nRecNo = 1029;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(18, 8, strWriteText);
   G_Log_Text = "ABS証：機器重量        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号("-"なし)
   //***  20070429("-"有りに変更)
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(18, 2, strWriteText);
   // 頭にBが付く "-"なし
   strWriteText = GetSerialNo();
   strWriteText = "B" + strWriteText;
   Excel_Cell_DataSet(22, 2, strWriteText);
   //G_Log_Text = "ABS証：製造番号(- なし)        『" + strWriteText + "』をセット。";
   G_Log_Text = "ABS証：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   // ----------------------------------------
   // 少々特殊
   // 台数(英)と台数と機器名称は結合して、Excelに描画する
   //***************************
   //***  台数(英)
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText = NumeralJtoEAllCapital(strWriteText);   
   G_Log_Text = "ABS証：台数(英)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp = strWriteText;
   strTmp += "(";

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "ABS証：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   strTmp += ")";

   //***************************
   //***  機器名称
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "ABS証：機器名称        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   Excel_Cell_DataSet(6, 5, strTmp);
   strTmp += "：";
   Excel_Cell_DataSet(27, 3, strTmp);
   // ----------------------------------------

   //***************************
   //***  型式
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(30, 5, strWriteText);
   G_Log_Text = "ABS証：型式        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム巾(桁区切り使用)
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(35, 5, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 35 << 5);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 35, 5);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "ABS証：フレーム巾(桁区切り使用)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム高(桁区切り使用)
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(35, 7, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 35 << 7);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 35, 7);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "ABS証：フレーム高(桁区切り使用)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム長(桁区切り使用)
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(35, 9, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 35 << 9);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 35, 9);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "ABS証：フレーム長(桁区切り使用)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  電熱面積
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31, 5, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS証：電熱面積        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側液名(短)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(32, 5, strWriteText);
   Excel_Cell_DataSet(33, 5, strWriteText);
   G_Log_Text = "ABS証：B側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側設計圧力
   //***************************
   nRecNo = 825;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(32, 7, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS証：B側設計圧力        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側液名(短)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(32, 9, strWriteText);
   Excel_Cell_DataSet(33, 9, strWriteText);
   G_Log_Text = "ABS証：A側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側設計圧力
   //***************************
   nRecNo = 830;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(32, 11, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS証：A側設計圧力        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  プレート材質(英文)
   //***************************
   nRecNo = 43;
   strWriteText = ConvertMaterialJtoE(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(34, 5, strWriteText);
   G_Log_Text = "ABS証：プレート材質(英文)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  プレート材質
   //*** 20070510 修正
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(34, 9, strWriteText);
   G_Log_Text = "ABS証：プレート材質        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側試験水圧
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(33, 7, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS証：B側試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側試験水圧
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(33, 11, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS証：A側試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   return true;
}
