//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：申請書共通関数
//　 作 成 者：S.Ogita
//　 作 成 日：2007.03
//　 更 新 日：
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "SINSEICom.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// 日本語関数名： 製造番号取得("-"あり)
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 製造番号("-"あり)
// 備  考      ： 仕様書 【注記 注１】
//---------------------------------------------------------------------------
AnsiString __fastcall GetSerialNoWithHyphen(void)
{
   AnsiString strRetSerialNo = "";  // 戻り値
   AnsiString strSerial = "";       // 製造番号
   AnsiString strCount = "";        // 台数
   int nRecNo;

   // HEADSデータから製造番号の取得
   nRecNo = 316;
   strSerial = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "製造番号 -付き (1/3)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strSerial + "』をセット。";
   Write_Log(G_Log_Text);
   
   // ２文字目と３文字目の間に”-”を入れる
   //2022/12/15 製造番号桁数追加対応 Mod_S
   //strSerial.Insert("-",3);
   strSerial = GetSeizouBangou(strSerial);
   //2022/12/15 製造番号桁数追加対応 Mod_E   

   // HEADSデータから台数の取得
   nRecNo = 39;
   strCount = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "製造番号 -付き (2/3)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strCount + "』をセット。";
   Write_Log(G_Log_Text);

   switch( strCount.ToIntDef(-1) )
   {
      case 2:
         strRetSerialNo = strSerial + "A/B";
         break;
      case 3:
         strRetSerialNo = strSerial + "A/B/C";
         break;
      case 4:
         strRetSerialNo = strSerial + "A/B/C/D";
         break;
      case 5:
         strRetSerialNo = strSerial + "A/B/C/D/E";
         break;
      default:
         strRetSerialNo = strSerial;
         break;
   }

   G_Log_Text = "製造番号 -付き (3/3)        『" + strRetSerialNo + "』をセット。";
   Write_Log(G_Log_Text);
   
   return strRetSerialNo;

}


//---------------------------------------------------------------------------
// 日本語関数名： 製造番号取得("-"なし)
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 製造番号("-"なし)
// 備  考      ： 仕様書 【注記 注３】
//---------------------------------------------------------------------------
AnsiString __fastcall GetSerialNo(void)
{
   AnsiString strRetSerialNo = "";  // 戻り値
   AnsiString strSerial = "";       // 製造番号
   AnsiString strCount = "";        // 台数
   int nRecNo;

   // HEADSデータから製造番号の取得
   nRecNo = 316;
   strSerial = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "製造番号 -なし (1/3)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strSerial + "』をセット。";
   Write_Log(G_Log_Text);

   // HEADSデータから台数の取得
   nRecNo = 39;
   strCount = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "製造番号 -なし (2/3)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strCount + "』をセット。";
   Write_Log(G_Log_Text);


   switch( strCount.ToIntDef(-1) )
   {
      case 2:
         strRetSerialNo = strSerial + "A/B";
         break;
      case 3:
         strRetSerialNo = strSerial + "A/B/C";
         break;
      case 4:
         strRetSerialNo = strSerial + "A/B/C/D";
         break;
      case 5:
         strRetSerialNo = strSerial + "A/B/C/D/E";
         break;
      default:
         strRetSerialNo = strSerial;
         break;
   }

   G_Log_Text = "製造番号 -なし (3/3)        『" + strRetSerialNo + "』をセット。";
   Write_Log(G_Log_Text);

   return strRetSerialNo;

}


//---------------------------------------------------------------------------
// 日本語関数名： 短い液名の取得
// 概  要      ：
// 引  数      ： 長い液名
// 戻り値      ： 長い液名から変換した短い液名
// 備  考      ： 仕様書 【注記 注４】
//---------------------------------------------------------------------------
AnsiString __fastcall GetLiquidShortName(AnsiString strLiquidLongName)
{
   AnsiString strLiquidShortName = "";

   G_Log_Text = "短い液名 (変換前)        『" + strLiquidLongName + "』が入力された。";
   Write_Log(G_Log_Text);

   if( strLiquidLongName.Trim() == "SEA WATER" )
   {
      strLiquidShortName = "S.W.";
   }
   else if( strLiquidLongName.Trim() == "S.W." )
   {
      strLiquidShortName = "S.W.";
   }
   else if( ( strLiquidLongName.Trim() == "fresh water" ) ||
            ( strLiquidLongName.Trim() == "FRESH WATER" ) )
   {
      strLiquidShortName = "F.W.";   
   }
   else if( strLiquidLongName.Trim() == "F.W." )
   {
      strLiquidShortName = "F.W.";
   }
   else if( strLiquidLongName.Trim() == "Lubricatng Oil" )
   {
      strLiquidShortName = "L.O.";
   }
   else if( strLiquidLongName.Trim() == "LO" )
   {
	  strLiquidShortName = "L.O.";
   }
   // 2013.04.03 書類書式変更
   else if( strLiquidLongName.Trim() == "LUB.OIL" )
   {
	  strLiquidShortName = "L.O.";
   }
   // ***********************
   else
   {
      strLiquidShortName = strLiquidLongName;
   }

   G_Log_Text = "短い液名 (変換後)        『" + strLiquidShortName + "』が出力された。";
   Write_Log(G_Log_Text);

   return strLiquidShortName;

}

//---------------------------------------------------------------------------
// 日本語関数名： 高温側試験水圧の取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 高温側試験水圧
// 備  考      ： 仕様書 【注記 注５】
//---------------------------------------------------------------------------
AnsiString __fastcall HigherTempPressure(void)
{
   AnsiString strRet = "";          // 戻り値
   AnsiString strABReversal = "";   // AB逆転文字列
   int nRecNo = 0;                  // HEADSデータの配列番号

   // AB逆転文字列の取得
   nRecNo = 137;
   strABReversal = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "高温側試験水圧(1/2)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strABReversal + "』をセット。";
   Write_Log(G_Log_Text);


   // AB逆転文字列の判別（高温側試験水圧の配列番号設定）
   if( strABReversal.ToIntDef(-1) == 0 )
   {
      nRecNo = 827;
   }
   else
   {
      nRecNo = 832;
   }
   // 高温側試験水圧の取得
   strRet = G_HEADS_DATA[nRecNo].Trim();
   strRet = FormatFloat("##0.00",StrToCurr(strRet));

   G_Log_Text = "高温側試験水圧(2/2)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strRet + "』をセット。";
   Write_Log(G_Log_Text);

   return strRet;
}

//---------------------------------------------------------------------------
// 日本語関数名： ABの水圧の高い方の数値を取得する
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： ABの水圧の高い方の数値
// 備  考      ： 
//---------------------------------------------------------------------------
AnsiString __fastcall GetHighPressureValueAorB(void)
{
   AnsiString strRet = "";          // 戻り値
   AnsiString strAVal,strBVal;
   int nRecNo = 0;                  // HEADSデータの配列番号

   nRecNo = 832;
   strAVal = G_HEADS_DATA[nRecNo].Trim();

   nRecNo = 827;
   strBVal = G_HEADS_DATA[nRecNo].Trim();

   G_Log_Text = "AB試験水圧の高圧側:AVal(1/3)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strAVal + "』を取得。";
   Write_Log(G_Log_Text);
   G_Log_Text = "AB試験水圧の高圧側:BVal(2/3)        RecNo:" + FormatFloat("0000",nRecNo) + "『" + strBVal + "』を取得。";
   Write_Log(G_Log_Text);   

   double dwAVal,dwBVal;
   try{ dwAVal = strAVal.ToDouble();
   }catch( EConvertError &e ){
      dwAVal = 0.0;
   }
   try{ dwBVal = strBVal.ToDouble();
   }catch( EConvertError &e ){
      dwBVal = 0.0;
   }


   if( dwAVal > dwBVal )
   {
      strRet = strAVal;
   }
   else
   {
      strRet = strBVal;
   }

   strRet = FormatFloat("##0.00",StrToCurr(strRet));

   G_Log_Text = "AB試験水圧の高圧側:Result(3/3)        『" + strRet + "』をセット。";
   Write_Log(G_Log_Text);

   return strRet;
}


//---------------------------------------------------------------------------
// 日本語関数名： 刻印項目一行目取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 刻印項目一行目
// 備  考      ： 仕様書 【注記 注２  １／５】
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal1st(void)
{
   AnsiString strRet = "";    // 戻り値
   AnsiString strText = "";   // 作業変数
   int nRecNo = 0;              // HEADSデータの配列番号

   // 規格コードの取得
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "ABSマークA    B";      
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "LR TEST KOB証明書番号";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         strRet = "NK   K";
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "証明書番号KOB";
         break;
      case SEISEKISYO_STANDARDNO_KR:
         strRet = "KRマーク KOBEM-証明書番号";
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "BVマーク証明書番号";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "DBP証明書番号";
         break;
      default:
         strRet = "ABSマークA    B";
         break;
   }

   return strRet;
}

//---------------------------------------------------------------------------
// 日本語関数名： 刻印項目二行目取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 刻印項目二行目
// 備  考      ： 仕様書 【注記 注２  ２／５】
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal2nd(void)
{
   AnsiString strRet = "";    // 戻り値
   AnsiString strText = "";   // 作業変数
   int nRecNo = 0;            // HEADSデータの配列番号

   // 規格コードの取得
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strRet = "MFG " + GetSerialNoWithHyphen() ;
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "KO証明書番号";
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_NK:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_GL:
         {
            AnsiString strTmp;
            int nRecNoTypeB = 827;
            int nRecNoTypeA = 832;
            strRet  = "W.T.P ";
            strTmp  = G_HEADS_DATA[nRecNoTypeB].Trim();
            strRet += FormatFloat("##0.00",StrToCurr(strTmp));
            strRet += "/";
            strTmp  = G_HEADS_DATA[nRecNoTypeA].Trim();
            strRet += FormatFloat("##0.00",StrToCurr(strTmp));
            strRet += " Mpa :B側/A側 T.P."; 
         }
         break;
      case SEISEKISYO_STANDARDNO_KR:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "W.T.P. "+GetHighPressureValueAorB()+"MPa";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = "KO証明書番号";
         break;
   }

   return strRet;
}


//---------------------------------------------------------------------------
// 日本語関数名： 刻印項目三行目取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 刻印項目三行目
// 備  考      ： 仕様書 【注記 注２  ３／５】
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal3rd(void)
{
   AnsiString strRet = "";    // 戻り値
   AnsiString strText = "";   // 作業変数
   int nRecNo = 0;            // HEADSデータの配列番号

   // 規格コードの取得
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1071;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 827;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = "W.T.P. "+strLiquidName+" "+strPressure+"MPa";
            
            G_Log_Text = "刻印項目三行目規格"+strText+" 液名[1071] ：["+G_HEADS_DATA[1071]+"]["+strLiquidName+"] 水圧[827]"+strPressure;
            Write_Log(G_Log_Text);            
         }
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "W.T.P. "+GetHighPressureValueAorB()+"MPa";
         break;
	  case SEISEKISYO_STANDARDNO_NK:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_KR:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1071;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 827;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = "W.T.P. "+strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "刻印項目三行目規格"+strText+" 液名[1071] ：["+G_HEADS_DATA[1071]+"]["+strLiquidName+"] 水圧[827]"+strPressure;
            Write_Log(G_Log_Text);
         }
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = GetSerialNoWithHyphen();
         break;
   }

   return strRet;
}


//---------------------------------------------------------------------------
// 日本語関数名： 刻印項目四行目取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 刻印項目四行目
// 備  考      ： 仕様書 【注記 注２  ４／５】
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal4th(void)
{
   AnsiString strRet = "";    // 戻り値
   AnsiString strText = "";   // 作業変数
   int nRecNo = 0;            // HEADSデータの配列番号

   // 規格コードの取得
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1073;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 832;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "刻印項目四行目規格"+strText+" 液名[1073] ：["+G_HEADS_DATA[1073]+"]["+strLiquidName+"] 水圧[832]"+strPressure;
            Write_Log(G_Log_Text);
         }
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "W.T.P. "+HigherTempPressure()+"MPa";      
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1071;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 827;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = "W.T.P. "+strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "刻印項目四行目規格[1071]"+strText+" 液名 ：["+G_HEADS_DATA[1071]+"]["+strLiquidName+"] 水圧[827]"+strPressure;
            Write_Log(G_Log_Text);            
         }
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_KR:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1073;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 832;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = strLiquidName+" "+strPressure+"MPa";

            G_Log_Text = "刻印項目四行目規格"+strText+" 液名[1073] ：["+G_HEADS_DATA[1073]+"]["+strLiquidName+"] 水圧[832]"+strPressure;
            Write_Log(G_Log_Text);            
         }
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = GetSerialNoWithHyphen();
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = "W.T.P. "+HigherTempPressure()+"MPa";
         break;
   }


   return strRet;

}


//---------------------------------------------------------------------------
// 日本語関数名： 刻印項目五行目取得
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 刻印項目五行目
// 備  考      ： 仕様書 【注記 注２  ５／５】
//---------------------------------------------------------------------------
AnsiString __fastcall GetCarvedSeal5th(void)
{
   AnsiString strRet = "";    // 戻り値
   AnsiString strText = "";   // 作業変数
   int nRecNo = 0;            // HEADSデータの配列番号

   // 規格コードの取得
   nRecNo = 29;
   strText = G_HEADS_DATA[nRecNo].Trim();

   switch( strText.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_LR:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_NK:
         {
            AnsiString strLiquidName = "";
            AnsiString strPressure = "";
            int nTmpRecNo = 0;

            nTmpRecNo = 1073;
            strLiquidName = GetLiquidShortName(G_HEADS_DATA[nTmpRecNo].Trim());

            nTmpRecNo = 832;
            strPressure = G_HEADS_DATA[nTmpRecNo].Trim();
            strPressure = FormatFloat("##0.00",StrToCurr(strPressure));

            strRet = strLiquidName+" "+strPressure+"MPa";
            
            G_Log_Text = "刻印項目五行目規格"+strText+" 液名[1073] ：["+G_HEADS_DATA[1073]+"]["+strLiquidName+"] 水圧[832]"+strPressure;
            Write_Log(G_Log_Text);
         }
         break;
      case SEISEKISYO_STANDARDNO_GL:
         strRet = "";
         break;
	  case SEISEKISYO_STANDARDNO_KR:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_BV:
         strRet = "";
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         strRet = "";
         break;
      default:
         strRet = "";
         break;
   }

   return strRet;
}


//---------------------------------------------------------------------------
// 日本語関数名： 材質コードの日本語変換
// 概  要      ：
// 引  数      ： 材質コード
// 戻り値      ： 材質の日本語名
// 備  考      ： なし
//---------------------------------------------------------------------------
AnsiString __fastcall ConvertMaterialJtoE(AnsiString strMaterialCode)
{
   AnsiString strMaterialName = "";
   // 2016.11.08 材質ｺｰﾄﾞ変更
   //AnsiString strCode = "";
   //int nCode;
   AnsiString s_Text;
   // ***********************

   // 規格コードの取得
   // 2016.11.08 材質ｺｰﾄﾞ変更
   //int nRecNo = 29;
   //strCode = G_HEADS_DATA[nRecNo].Trim();
   //nCode = strCode.ToIntDef(-1);
   //
   //switch( strMaterialCode.ToIntDef(-1) )
   //{
   //   case 1014:
   //   case 1015:
   //   case 1032:
   //   case 1033:
   //   case 1057:
   //   case 1058:
   //   //2010.10.29 SB-265-Gr.7,SB-265-Gr.2,B-265-Gr.2追加
   //   case 1075:
   //   case 1089:
   //   case 1090:
   //   //*************************************************
   //   //2010.12.02 SB-265-Gr.11追加
   //   case 1076:
   //   //***************************
   //   case 1085:
   //   case 1098:
   //      strMaterialName = "Titanium";
   //      break;
   //   default:
   //      /*
   //      if( nCode == 14 )
   //      {
   //         strMaterialName = "Stainless steel";
   //      }
   //      else
   //      {
   //         strMaterialName = "Stainless";
   //      }
   //      */
   //      // 20070510 ABS BVのStainless → Stainless steelに変更のため
   //      // 上記if文をコメントアウト(ABS BV LR)のみ現状利用
   //      strMaterialName = "Stainless steel";
   //      break;
   //}
   s_Text = Type_MAT(strMaterialCode);
   if( s_Text == "Ti" ) {           // チタンの場合は仕上５
      strMaterialName = "Titanium";
   } else {
      strMaterialName = "Stainless steel";
   }
   // ***********************


   return strMaterialName;

}


//---------------------------------------------------------------------------
// 日本語関数名： Excelシートのコピー関数
// 概  要      ：
// 引  数      ： コピー先ファイル名,コピー先シート(Before),コピー元ファイル名,コピー元シート名
// 戻り値      ： 材質の日本語名
// 備  考      ： Beforeの意味・・・指定したシートの前にシートをコピーする。
//---------------------------------------------------------------------------
bool __fastcall ExcelSheetCopy( AnsiString strToFilePass, AnsiString strToSheetName, AnsiString strFromFilePath, AnsiString strFromSheetName)
{
   // コピー先用変数
   Variant exWorkbookTo;
   Variant exWorksheetsTo;
   Variant exWorksheetTo;
   // コピー元用変数
   Variant exWorkbookFrom;
   Variant exWorksheetsFrom;
   Variant exWorksheetFrom;

   //******************************
   //***  コピー先Excel Book Open
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbookTo  = exWorkbooks.Exec(Function("Open") << strToFilePass.c_str());
	  //Windows 2008Server 移植対応
	  exWorkbookTo  = exWorkbooks.OleFunction("Open", (OleVariant)strToFilePass );
   }
   catch (...) {
      G_ErrLog_Text = "『" + strToFilePass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheetsTo = exWorkbookTo.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheetsTo = exWorkbookTo.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "『" + strToFilePass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ｼｰﾄ取得
   try {
	  //exWorksheetTo = exWorksheetsTo.Exec(PropertyGet("Item") << strToSheetName.c_str());
	  //Windows 2008Server 移植対応
	  exWorksheetTo = exWorksheetsTo.OlePropertyGet("item",(OleVariant)strToSheetName );
   }
   catch (...) {
	  G_ErrLog_Text = "『" + strToSheetName + "』のｼｰﾄの取得に失敗しました！！";
	  Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //******************************
   //***  コピー元Excel Book Open
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
	  //exWorkbookFrom  = exWorkbooks.Exec(Function("Open") << strFromFilePath.c_str());
	  //Windows 2008Server 移植対応
	  exWorkbookFrom  = exWorkbooks.OleFunction("Open", (OleVariant)strFromFilePath );
   }
   catch (...) {
      G_ErrLog_Text = "『" + strFromFilePath + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
	  //exWorksheetsFrom = exWorkbookFrom.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server 移植対応
	  exWorksheetsFrom = exWorkbookFrom.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "『" + strFromFilePath + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ｼｰﾄ取得
   try {
	  //exWorksheetFrom = exWorksheetsFrom.Exec(PropertyGet("Item") << strFromSheetName.c_str());
	  //Windows 2008Server 移植対応
	  exWorksheetFrom = exWorksheetsFrom.OlePropertyGet("Item",(OleVariant)strFromSheetName);
   }
   catch (...) {
	  G_ErrLog_Text = "『" + strFromSheetName + "』のｼｰﾄの取得に失敗しました！！";
	  Write_Error_Log(G_ErrLog_Text);
	  return false;
   }

   //******************************
   //***  コピーの実行
   //******************************
   // コピーの実行
   try {
	  //exWorksheetFrom.Exec(Function("Copy") << Variant(exWorksheetTo));
	  exWorksheetFrom.OleFunction("Copy", (OleVariant)exWorksheetTo);
	  //exWorksheetTo.OleFunction("Copy", (OleVariant)exWorksheetFrom);
   }
   catch (...) {
      G_ErrLog_Text = "『" + strFromSheetName + "』のｼｰﾄｺﾋﾟｰに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //******************************
   //***  編集Excelbookの保存
   //******************************
   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server 移植対応
   exApplication.OlePropertySet("DisplayAlerts",false);
   //exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << strToFilePass.c_str());
   exWorkbookTo.OleFunction("SaveAs", (OleVariant)strToFilePass);
   //******************************
   //***  Excelオブジェクトの解放
   //******************************
   exWorkbookFrom.OleFunction("Close");
   exWorkbookTo.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   return true;


}


//---------------------------------------------------------------------------
// 日本語関数名： 数値の英語変換（全て大文字）
// 概  要      ：
// 引  数      ： 数値
// 戻り値      ： 英語化した数値
// 備  考      ：
//---------------------------------------------------------------------------
AnsiString __fastcall NumeralJtoEAllCapital( AnsiString strNumeral)
{
   AnsiString strRet = "";
   int nInput,nTen,nOne;

   nInput = strNumeral.ToIntDef(-1);

   if( nInput > -1 )
   {
      // 2021.06.18 FireDACへの移行 MOD_S
	  nTen = floor((double)(nInput / 10));
	  // 2021.06.18 FireDACへの移行 MOD_E
      switch (nTen)
	   {
	      case 0:
            if( nInput == 0 )
            {
               strRet = "ZERO";
            }
	         break;
	      case 1:
            nOne = nInput % 10;
            switch(nOne)
            {
               case 0:
                  strRet = "TEN";
                  break;
               case 1:
                  strRet = "ELEVEN";
                  break;
               case 2:
                  strRet = "TWELVE";
                  break;
               case 3:
                  strRet = "THIRTEEN";
                  break;
               case 4:
                  strRet = "FOURTEEN";
                  break;
               case 5:
                  strRet = "FIFTEEN";
                  break;
               case 6:
                  strRet = "SIXTEEN";
                  break;
               case 7:
                  strRet = "SEVENTEEN";
                  break;
               case 8:
                  strRet = "EIGHTEEN";
                  break;
               case 9:
                  strRet = "NINETEEN";
                  break;
               default:
                  strRet = "";
                  break;
            }
	         break;
	      case 2:
            strRet = "TWENTY";
	         break;
	      case 3:
            strRet = "THIRTY";
	         break;
	      case 4:
            strRet = "FORTY";
	         break;
	      case 5:
            strRet = "FIFTY";
	         break;
	      case 6:
            strRet = "SIXTY";
	         break;
	      case 7:
            strRet = "SEVENTY";
	         break;
	      case 8:
            strRet = "EIGHTY";
	         break;
	      case 9:
            strRet = "NINETY";        
	         break;
         default:
            break;
	   }

      if( (nInput >= 20) || ((nInput >= 1) && (nInput <= 9)) )
      {
   		nOne = nInput % 10;
	      switch (nOne)
		   {
		      case 0:
		         break;
		      case 1:
	               strRet += " ONE";
		         break;
		      case 2:
	            strRet += " TWO";
		         break;
		      case 3:
	            strRet += " THREE";
		         break;
		      case 4:
	            strRet += " FOUR";
		         break;
		      case 5:
	            strRet += " FIVE";
		         break;
		      case 6:
	            strRet += " SIX";
		         break;
		      case 7:
	            strRet += " SEVEN";
		         break;
		      case 8:
	            strRet += " EIGHT";
		         break;
		      case 9:
	            strRet += " NINE";
		         break;
	         default:
               strRet += "";
	            break;
		   }
      }

   }

   return strRet;
}


//---------------------------------------------------------------------------
// 日本語関数名： 数値の英語変換（先頭だけ大文字）
// 概  要      ：
// 引  数      ： 数値
// 戻り値      ： 英語化した数値
// 備  考      ：
//---------------------------------------------------------------------------
AnsiString __fastcall NumeralJtoEHeadCapital( AnsiString strNumeral)
{
   AnsiString strRet = "";
   int nInput,nTen,nOne;

   nInput = strNumeral.ToIntDef(-1);

   if( nInput > -1 )
   {
	  // 2021.06.18 FireDACへの移行 MOD_S
	  nTen = floor((double)(nInput / 10));
	  // 2021.06.18 FireDACへの移行 MOD_E
      switch (nTen)
	   {
	      case 0:
            nOne = nInput % 10;
            switch(nOne)
            {
               case 0:
                  strRet = "Zero";
                  break;
               case 1:
                  strRet = "One";
                  break;
               case 2:
                  strRet = "Two";
                  break;
               case 3:
                  strRet = "Three";
                  break;
               case 4:
                  strRet = "Four";
                  break;
               case 5:
                  strRet = "Five";
                  break;
               case 6:
                  strRet = "Six";
                  break;
               case 7:
                  strRet = "Seven";
                  break;
               case 8:
                  strRet = "Eight";
                  break;
               case 9:
                  strRet = "Nine";
                  break;
               default:
                  strRet = "";
                  break;
            }
	         break;
	      case 1:
            nOne = nInput % 10;
            switch(nOne)
            {
               case 0:
                  strRet = "Ten";
                  break;
               case 1:
                  strRet = "Eleven";
                  break;
               case 2:
                  strRet = "Twelve";
                  break;
               case 3:
                  strRet = "Thirteen";
                  break;
               case 4:
                  strRet = "Fourteen";
                  break;
               case 5:
                  strRet = "Fifteen";
                  break;
               case 6:
                  strRet = "Sixteen";
                  break;
               case 7:
                  strRet = "Seventeen";
                  break;
               case 8:
                  strRet = "Eighteen";
                  break;
               case 9:
                  strRet = "Nineteen";
                  break;
               default:
                  strRet = "";
                  break;
            }
	         break;
	      case 2:
            strRet = "Twenty";
	         break;
	      case 3:
            strRet = "Thirty";
	         break;
	      case 4:
            strRet = "Forty";
	         break;
	      case 5:
            strRet = "Fifty";
	         break;
	      case 6:
            strRet = "Sixty";
	         break;
	      case 7:
            strRet = "Seventy";
	         break;
	      case 8:
            strRet = "Eighty";
	         break;
	      case 9:
            strRet = "Ninety";        
	         break;
         default:
            break;
	   }

      if( nInput >= 20 )
      {
   		nOne = nInput % 10;
	      switch (nOne)
		   {
		      case 0:
		         break;
		      case 1:
	               strRet += " one";
		         break;
		      case 2:
	            strRet += " two";
		         break;
		      case 3:
	            strRet += " three";
		         break;
		      case 4:
	            strRet += " four";
		         break;
		      case 5:
	            strRet += " five";
		         break;
		      case 6:
	            strRet += " six";
		         break;
		      case 7:
	            strRet += " seven";
		         break;
		      case 8:
	            strRet += " eight";
		         break;
		      case 9:
	            strRet += " nine";
		         break;
	         default:
               strRet = "";
	            break;
		   }
      }

   }

   return strRet;


}
