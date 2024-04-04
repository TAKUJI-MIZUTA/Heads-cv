//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：申請書FAX用紙(NK以外)
//　 作 成 者：S.Ogita
//　 作 成 日：2007.02
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "SINSEICom.h"  // 申請書共通関数定義用

#include "DataModule.h"

#include "Bas_ShinseiFaxNonNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 申請書FAX用紙(NK以外)  作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall ShinseiFaxNonNK(void)
{
   AnsiString strSheetName;   // 使用シート名
   AnsiString strGetText;     // 取得HEADSデータ
   AnsiString strWriteText;   // 描画データ
   int nRecNo;                // HEADSデータ配列番号

   strSheetName = "他-FAX";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "他-FAX");
   //Windows 2008Server 移植対応
   exWorksheet = exWorksheets.OlePropertyGet("Item",OleVariant("他-FAX"));

   //******************************
   //***  作成開始ﾛｸﾞの書出し
   //******************************
   G_Log_Text = "-----------    『他-FAX』作成開始2    -----------";
   Write_Log(G_Log_Text);

   // 規格コードの取得
   nRecNo = 29;
   strGetText = G_HEADS_DATA[nRecNo].Trim();

   //***************************
   //***  FAX番号
   //***************************
   switch( strGetText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strWriteText = "078-291-1309";
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strWriteText = "078-221-5744";
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strWriteText = "078-261-0780";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         strWriteText = "0798-36-6861";
         break;
      case SEISEKISYO_STANDARDNO_GL:
         //2014.01.07 FAX番号変更
         //strWriteText = "078-322-0700";
         strWriteText = "078-291-8500";
         //*******************
         break;
      case SEISEKISYO_STANDARDNO_KR:
         //2014.01.07 FAX番号変更
         //strWriteText = "078-232-1385";
         strWriteText = "078-325-3441";
         //*******************
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strWriteText = "078-331-3395";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strWriteText = "06-6534-1436";
         break;
      default :
         strWriteText = "078-221-5744";
         break;
   }   
   Excel_Cell_DataSet(1, 3, strWriteText);
   G_Log_Text = "他-FAX：FAX番号        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";   
   Write_Log(G_Log_Text);

   //***************************
   //***  規格名
   //***************************
   nRecNo = 29;
   strWriteText = Search_KIKAKU_CODE(G_HEADS_DATA[nRecNo].TrimRight(),true,true, G_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(3, 3, strWriteText);
   G_Log_Text = "他-FAX：規格名        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  製造番号
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(21, 4, strWriteText);
   G_Log_Text = "他-FAX：製造番号        『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  台数
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].TrimRight();
   Excel_Cell_DataSet(21, 10, strWriteText);
   G_Log_Text = "他-FAX：台数        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strWriteText + "』をセット。";
   Write_Log(G_Log_Text);

   //***************************
   //***  刻印一行目
   //***************************
   strWriteText = GetCarvedSeal1st();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(48, 3, strWriteText);
      G_Log_Text = "他-FAX：刻印一行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印二行目
   //***************************
   strWriteText = GetCarvedSeal2nd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(49, 3, strWriteText);
      G_Log_Text = "他-FAX：刻印二行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印三行目
   //***************************
   strWriteText = GetCarvedSeal3rd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(50, 3, strWriteText);
      G_Log_Text = "他-FAX：刻印三行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印四行目
   //***************************
   strWriteText = GetCarvedSeal4th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(51, 3, strWriteText);
      G_Log_Text = "他-FAX：刻印四行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  刻印五行目
   //***************************
   strWriteText = GetCarvedSeal5th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(52, 3, strWriteText);
      G_Log_Text = "他-FAX：刻印五行目        『" + strWriteText + "』をセット。";
      Write_Log(G_Log_Text);
   }

return true;
}
