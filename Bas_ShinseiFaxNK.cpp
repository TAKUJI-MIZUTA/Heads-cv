//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：申請書FAX用紙(NK)
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

#include "Bas_ShinseiFaxNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 申請書FAX用紙(NK)  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShinseiFaxNK(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strWriteText;   // 描画データ
   int nRecNo;                // HEADSデータ配列番号

   // シートの指定
   strSheetName = "NK-FAX";   
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("Item" , (OleVariant)strSheetName);

   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『NK-FAX』作成開始    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(21, 4, strWriteText);
   G_Log_Text = "NK-FAX：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].TrimRight();
   Excel_Cell_DataSet(21, 6, strWriteText);
   G_Log_Text = "NK-FAX：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  刻印一行目
   //***************************
   strWriteText = GetCarvedSeal1st();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(47, 3, strWriteText);
      G_Log_Text = "NK-FAX：刻印一行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印二行目
   //***************************
   strWriteText = GetCarvedSeal2nd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(48, 3, strWriteText);
      G_Log_Text = "NK-FAX：刻印二行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印三行目
   //***************************
   strWriteText = GetCarvedSeal3rd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(49, 3, strWriteText);
      G_Log_Text = "NK-FAX：刻印三行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印四行目
   //***************************
   strWriteText = GetCarvedSeal4th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(50, 3, strWriteText);
      G_Log_Text = "NK-FAX：刻印四行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印五行目
   //***************************
   strWriteText = GetCarvedSeal5th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(51, 3, strWriteText);
      G_Log_Text = "NK-FAX：刻印五行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
return true;
}
