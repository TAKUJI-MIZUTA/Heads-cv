//----------------------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：圧力容器強度計算書  作成モジュール
//　 作 成 者：N.Uchida
//　 作 成 日：2002.06.05
//　 更 新 日：2002.09.20
//             2007.10.10 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

#include "DataModule.h"

#include "Bas_kyodokeisan.h"
#include "Bas_ATU21.h"
#include "Bas_ATU22.h"
#include "Bas_ATU31.h"
#include "Bas_ATU32.h"
#include "Bas_ATU61.h"
#include "Bas_ATU52.h"
#include "Bas_Steam.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// 日本語関数名： 圧力容器強度計算書　作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall Kyodokeisan(HeadsData* pcHeadsData)
{
   //int i_Pos;
   AnsiString P_UtiKbn;
   int i_PrintFlg;         // 0:PDF未出力,1:PDF出力
   int i_KikakuCode;       // 規格ｺｰﾄﾞ
   int i_BolOut;           // 0:ﾎﾞﾙﾄ外側,1:ﾎﾞﾙﾄ内側
   int i_BolNum;           // ﾎﾞﾙﾄ片側本数

   AnsiString s_Syurui;    // 種類
   AnsiString s_Model;     // ﾓﾃﾞﾙ
   AnsiString s_Frame;     // ﾌﾚｰﾑ
   AnsiString s_SQL;

   AnsiString s_Tmplate_Pass;    // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   AnsiString s_MakeExcel_Pass;  // 作成ﾌｧｲﾙ名
   AnsiString s_Text;            // 汎用文字列

   //------------------------------------
   // 2007.10.10 ﾐﾗｰ仕様対応
   // 元仕様以外は処理不要(正常終了)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   s_Tmplate_Pass = G_Template_Dir + "圧力容器申請書.xls";              // ﾃﾝﾌﾟﾚｰﾄﾌｧｲﾙ
   s_MakeExcel_Pass = G_xls_Output_Dir + "PVC" + G_SAKUBAN + ".xls";    // ｺﾋﾟｰ先ﾌｧｲﾙ

   // ﾛｸﾞ作成
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『圧力容器強度計算書』作成開始    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "作成ファイル名：    『" + s_MakeExcel_Pass + "』";
   Write_Log(G_Log_Text);

   //******************************
   //***  テンプレート　コピー
   //******************************
   if (!Template_Copy(s_Tmplate_Pass,s_MakeExcel_Pass)){
      // ｴﾗｰﾛｸﾞ追加
      G_ErrLog_Text = "ファイルコピーを失敗しました。『" + s_Tmplate_Pass +
                      "』→『" + s_MakeExcel_Pass + "』";
      Write_Error_Log(G_ErrLog_Text);

      return false;
   } else {
      G_Log_Text = "ファイルコピー成功。『" + s_Tmplate_Pass +
                      "』→『" + s_MakeExcel_Pass + "』";
      Write_Log(G_Log_Text);
   }

   //******************************
   //***  Excel Book Open
   //******************************
   // ﾌｧｲﾙｵｰﾌﾟﾝ
   try {
      exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のOPENに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ｼｰﾄｺﾚｸｼｮﾝ取得
   try {
      exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
   }
   catch (...) {
      G_ErrLog_Text = "『" + s_MakeExcel_Pass + "』のｼｰﾄｺﾚｸｼｮﾝの取得に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //*********************************
   //***  圧力容器強度計算書  作成
   //*********************************
   i_PrintFlg = 0;

// 全帳票チェック用
//         Atu21();
//         Atu22();
//         Atu31();
//         Atu32();
//G_HEADS_DATA[1020]="2";
//         Atu61();
//G_HEADS_DATA[1020]="3";
//         Atu61();
//G_HEADS_DATA[1020]="4";
//         Atu61();
//G_HEADS_DATA[1020]="5";
//         Atu61();
//G_HEADS_DATA[1020]="6";
//         Atu61();
//G_HEADS_DATA[1020]="7";
//         Atu61();
//         Atu52();

   Get_PHE_Model(&s_Syurui, &s_Model, G_HEADS_DATA[34].TrimRight(), G_HEADS_DATA[35].TrimRight());

   i_KikakuCode = G_HEADS_DATA[29].ToIntDef(0);

//2003.09.04 出力ﾃﾝﾌﾟﾚｰﾄ変更
   if (i_KikakuCode == 1 || i_KikakuCode == 2 || i_KikakuCode == 3){
//      s_Syurui = G_HEADS_DATA[34].TrimRight();    // 種類
//      s_Model  = G_HEADS_DATA[35].TrimRight();    // ﾓﾃﾞﾙ
//      s_Frame  = G_HEADS_DATA[36].TrimRight();    // ﾌﾚｰﾑ
      i_BolOut = G_HEADS_DATA[1277].ToIntDef(0);  // 0:ﾎﾞﾙﾄ外側,1:ﾎﾞﾙﾄ内側
      i_BolNum = G_HEADS_DATA[1020].ToIntDef(0);  // ﾎﾞﾙﾄ片側本数

//      if (s_Syurui == "LX" && s_Model == "00" && s_Frame == "NP"){
//      if (s_Syurui == "LX" && s_Model == "00" && (s_Frame.Pos("NP") !=0 || s_Frame.Pos("NJ") !=0)){
      if (i_BolOut == 1 && i_BolNum == 2){

         Atu21();
         Atu22();

         i_PrintFlg = 1;
//      } else if ( (s_Syurui == "LX" && s_Model == "00" && s_Frame == "NHP") ||
//      } else if ( (s_Syurui == "LX" && s_Model == "00" && (s_Frame.Pos("NHP") !=0 || s_Frame.Pos("NHJ") !=0)) ||
//                  (s_Syurui == "RX" && s_Model == "10") ) {
      } else if (i_BolOut == 1 && i_BolNum == 3){

         Atu31();
         Atu32();

         i_PrintFlg = 1;
      } else {

         Atu61();
         Atu52();

         i_PrintFlg = 1;
      }
   }
//**************************

   // ｽﾁｰﾑ送入量計算書
   // RecNo.846(Bｽﾁｰﾑ)かRecNo.847(Aｽﾁｰﾑ)のどちらかが、１のとき作成
   if (G_HEADS_DATA[846].TrimRight() == "1" || G_HEADS_DATA[847].TrimRight() == "1"){
      Steam();

      i_PrintFlg = 1;
   }

   //******************************
   //***  Excel Book Close
   //******************************
   // マクロ実行
   if (i_PrintFlg == 1){
      exApplication.Exec(Function("Run") << "Gsub_PDFOut");
   }

   exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ﾛｸﾞ作成
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    『圧力容器強度計算書』作成終了    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

