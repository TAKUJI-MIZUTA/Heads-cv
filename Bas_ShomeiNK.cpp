//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：NK証
//　 作 成 者：S.Ogita
//　 作 成 日：2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "SINSEICom.h"  // 申請書共通関数定義用

#include "Bas_ShomeiNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： NK証  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShomeiNK(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strWriteText;   // 描画データ
   AnsiString strTmp;         // 一時変数
   int nRecNo;                // HEADSデータ配列番号

   // シートの指定
   strSheetName = "NK証";   
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);
   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『NK証』作成開始    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText = NumeralJtoEAllCapital(strWriteText);
   Excel_Cell_DataSet(15, 6, strWriteText);
   G_Log_Text = "NK証：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器名称
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(15, 13, strWriteText);
   G_Log_Text = "NK証：機器名称        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   // ----------------------------------------
   // 少々特殊
   // 船番と納入先は結合して、Excelに描画する。
   //***************************
   //***  船番
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "NK証：船番        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);


   strTmp = strWriteText;
   strTmp += ",";

   //***************************
   //***  納入先
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "NK証：納入先        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;

   Excel_Cell_DataSet(19, 14, strTmp);
   // ----------------------------------------

   //***************************
   //***  型式
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(25, 14, strWriteText);
   G_Log_Text = "NK証：型式        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(26, 11, strWriteText);
   G_Log_Text = "NK証：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側液名(短)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(28, 11, strWriteText);
   G_Log_Text = "NK証：B側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側設計圧力
   //***************************
   nRecNo = 825;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 14, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK証：B側設計圧力        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側液名(短)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(28, 17, strWriteText);
   G_Log_Text = "NK証：A側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側設計圧力
   //***************************
   nRecNo = 830;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 19, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK証：A側設計圧力        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  電熱面積
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 11, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK証：電熱面積        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);


   // DATE
   //2008.07.24 日付出力停止
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 46 << 6);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('NK-FAX'!K47,'NK-FAX'!M47,'NK-FAX'!O47),\"mmm.dd.yyyy\")" );
   //***********************

   // Date of Manufacture
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 27 << 11);
   //Windows 2008Server 移植対応
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 27, 11);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('NK-FAX'!K49,'NK-FAX'!M49,'NK-FAX'!O49),\"mmm.dd.yyyy\")" );
   //Windows 2008Server 移植対応
   exWorkCell.OlePropertySet("Value",OleVariant("=TEXT(DATE('NK-FAX'!K49,'NK-FAX'!M49,'NK-FAX'!O49),\"mmm.dd.yyyy\")" ));

   return true;
}
