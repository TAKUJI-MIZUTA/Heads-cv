//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：BV証
//　 作 成 者：S.Ogita
//　 作 成 日：2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "SINSEICom.h"  // 申請書共通関数定義用

#include "Bas_ShomeiBV.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： BV証  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShomeiBV(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strWriteText;   // 描画データ
   AnsiString strTmp;         // 一時変数   
   int nRecNo;                // HEADSデータ配列番号

   // シートの指定
   strSheetName = "BV証";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『BV証』作成開始    -----------";
   Write_Log(G_Log_Text);


   //***************************
   //***  納入先
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(11, 5, strWriteText);
   Excel_Cell_DataSet(13, 5, strWriteText);   
   G_Log_Text = "BV証：納入先        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  船番
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(14, 16, strWriteText);
   G_Log_Text = "BV証：船番        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(21, 5, strWriteText);
   G_Log_Text = "BV証：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  型式
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(23, 9, strWriteText);
   G_Log_Text = "BV証：型式        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  L型液名
   //***************************
   nRecNo = 76;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(24, 9, strWriteText);
   G_Log_Text = "BV証：L型液名        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  機器名称
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(26, 9, strWriteText);   
   G_Log_Text = "BV証：機器名称        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(20, 11, strWriteText);
   Excel_Cell_DataSet(27,  9, strWriteText);
   Excel_Cell_DataSet(45,  12, strWriteText);
   G_Log_Text = "BV証：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  電熱面積
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 9, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV証：電熱面積        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  プレート材質(英文)
   //***************************
   nRecNo = 43;
   strWriteText = ConvertMaterialJtoE(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(25, 9, strWriteText);
   G_Log_Text = "BV証：プレート材質(英文)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  プレート材質
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   strWriteText += ")";
   Excel_Cell_DataSet(25, 14, strWriteText);
   G_Log_Text = "BV証：プレート材質        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側設計圧力
   //***************************
   nRecNo = 830;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 12, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV証：A側設計圧力        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);   

   //***************************
   //***  B側設計圧力
   //***************************
   nRecNo = 825;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29,16, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV証：B側設計圧力        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側液名(短)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(29, 9, strWriteText);
   Excel_Cell_DataSet(30, 9, strWriteText);
   G_Log_Text = "BV証：A側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側液名(短)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(29, 14, strWriteText);
   Excel_Cell_DataSet(30, 14, strWriteText);
   G_Log_Text = "BV証：B側液名(短)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  A側試験水圧
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(30, 12, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV証：A側試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  B側試験水圧
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(30,16, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV証：B側試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム巾
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31, 9, strWriteText);
   G_Log_Text = "BV証：フレーム巾        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム高
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31,12, strWriteText);
   G_Log_Text = "BV証：フレーム高        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  フレーム長
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31, 14, strWriteText);
   G_Log_Text = "BV証：フレーム長        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  最高試験水圧
   //***************************
   strWriteText = GetHighPressureValueAorB();
   Excel_Cell_DataSet(45, 8, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV証：最高試験水圧        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   return true;
}
