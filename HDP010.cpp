//---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：出図要求受信サービス
//　 作 成 者：N.Uchida
//　 作 成 日：2002.05.23
//　 更 新 日：2002.09.17 △1 2007.02.01 △2 2007.03
//             2007.10.09 ﾐﾗｰ仕様対応
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>

#include <dir.h>
#include <io.h>
#include <direct.h>

#pragma hdrstop

#include <sys\stat.h>
#include <utilcls.h>          // Excel起動用ﾍｯﾀ
//2009.03.03 圧力容器強度計算変更
#include <process.h>
//*******************************
#include "HDPConst.h"         // 定数定義用
#include "HDPValue.h"         // 変数定義用
#include "HDPCom.h"           // 関数定義用

#include "DataModule.h"

#include "HDP010.h"
#include "Bas_Syonin.h"          // 承認図表紙
#include "Bas_Mokuroku.h"        // 図面目録
#include "Bas_Seisekisyo.h"      // 検査成績書
#include "Bas_Funeyou.h"         // 舶用要目表
#include "Bas_Kobetusinsei.h"    // 小型圧力容器 個別検定申請書
#include "Bas_Meisaisyo.h"       // 小型圧力容器 明細書
//2009.03.03 圧力容器強度計算変更
//#include "Bas_kyodokeisan.h"     // 圧力容器強度計算書
//*******************************
#include "Bas_Youryosyo.h"
#include "Bas_Sijisyo.h"
//Add_Str 2019/02/19 Ri
#include "Bas_SijisyoA3.h"
//Add_End 2019/02/19 Ri
#include "Bas_Steam.h"



// --> △2 追加
#include "Bas_Funeyousinsei.h"
//#include "Bas_SeisekishoDNV.h"   // 成績書 DNV規格
//#include "Bas_SeisekishoABS.h"   // 成績書 ABS規格
//#include "Bas_SeisekishoLR.h"    // 成績書 LR規格
//#include "Bas_SeisekishoNK.h"    // 成績書 NK規格
//#include "Bas_SeisekishoGL.h"    // 成績書 GL規格
//#include "Bas_SeisekishoKR.h"    // 成績書 KR規格
//#include "Bas_SeisekishoBV.h"    // 成績書 BV規格
//#include "Bas_SeisekishoCCS.h"   // 成績書 CCS規格
// <-- △2 追加

#include "Bas_Buhinhyou.h"       // 標準部品表(02.08.07追加)

#include "HeadsData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmHDP010 *frmHDP010;

//---------------------------------------------------------------------------
// ﾌｫｰﾑ ｺﾝｽﾄﾗｸﾀ
//---------------------------------------------------------------------------
__fastcall TfrmHDP010::TfrmHDP010(TComponent* Owner)
   : TForm(Owner)
{
}

//---------------------------------------------------------------------------
// 日本語関数名： フォーム  再描画
// 概  要      ： タスクバー　起動アプリケーション非表示化
// 引  数      ：
// 戻り値      ： なし
// 備  考      ：
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::FormPaint(TObject *Sender)
{
   ShowWindow(Application->Handle,SW_HIDE);
}

//---------------------------------------------------------------------------
// 日本語関数名： 作番　取得
// 概  要      ： 起動時に取得するパラメータから、作番を取得する。
// 引  数      ： なし
// 戻り値      ： 処理結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Get_Sakuban(void)
{
   //*******************
   //***  作番取得
   //*******************

   // 作番をHD_sakuban.txtから取得するよう修正
   G_SAKUBAN = Get_Sakuban_File();
   if ( G_SAKUBAN == "" ) {
      G_ErrLog_Text = "HD_sakuban.txtより作番が取得できませんでした！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   } else {
      G_HEADS_ID = G_SAKUBAN + ".txt";
   }
   // ｺﾏﾝﾄﾞﾗｲﾝからﾌｧｲﾙIDを取得
//   G_HEADS_ID = ParamStr(1);     // HEADSﾃﾞｰﾀ ﾌｧｲﾙID
//   //G_HEADS_ID = "084156.txt";
//
//   // ﾌｧｲﾙIDから作番を取得
//   int i_Pos;

//   i_Pos = G_HEADS_ID.Pos(".");
//   if (i_Pos != 0){
//      G_SAKUBAN = G_HEADS_ID.SubString(1, i_Pos-1);
//   } else {
//      G_ErrLog_Text = "引数(" + G_HEADS_ID + ")により作番が取得できませんでした！！";
//      Write_Error_Log(G_ErrLog_Text);
//      return false;
//   }

   // ﾛｸﾞ作成
   G_Log_Text = "ファイルＩＤ『" + G_HEADS_ID + "』から、作番(" + G_SAKUBAN + ")を取得しました";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //*************************
   //***  作番種類　取得
   //*************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0){
      P_Sakuban_Mode = "S";
   } else {
      P_Sakuban_Mode = "M";
   }

   //2019/05/17 応用技術追加
   gP_Sakuban_Mode = P_Sakuban_Mode;

   //**************************************************************
   // 02.08.07追加 HEADS入力ﾌｫﾙﾀﾞ･ﾌｧｲﾙ形式変更 csv→txt
   // 見積ﾃﾞｰﾀ "C:\HEADS_ROOT\HEADS_DATA\ESTIMATE\D00001\D00001.txt"
   // 発注ﾃﾞｰﾀ "C:\HEADS_ROOT\HEADS_DATA\ORDER\000001\000001.txt"
   //***************************************************************
   if( P_Sakuban_Mode == "S" )
   {
      //マルチサーバ対応 2009/1/13 MOD START
      //G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
      //                 + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //                 + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //                 + IncludeTrailingBackslash(G_SAKUBAN);
      G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                       + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                       + IncludeTrailingBackslash(G_SAKUBAN);
      //マルチサーバ対応 2009/1/13 MOD END
   }
   else if( P_Sakuban_Mode == "M" )
   {
      //マルチサーバ対応 2009/1/13 MOD START
      //G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
      //                 + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //                 + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //                 + IncludeTrailingBackslash(G_SAKUBAN);
      G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                       + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                       + IncludeTrailingBackslash(G_SAKUBAN);
      //マルチサーバ対応 2009/1/13 MOD END
   }

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： ウィンドウ生成時処理
// 概  要      ： 表示初期化をする
// 引  数      ： 初期値
// 戻り値      ： なし
// 備  考      ： ｼｽﾃﾑ設定読込み
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::FormCreate(TObject *Sender)
{

   // INI設定ファイルの読み込み
   IniFileRead();

   //*************************
   //***  ﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
   //*************************
   if(!Log_File_SizeCheck()){
      return;
   }

   Screen->Cursor = crHourGlass;
   prvFrmLoadFlg == 0;

   // ﾛｸﾞ作成
   G_Log_Text = "********************************************************************************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "*****                                                                      *****";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "*****                  EXCELデータ出力システムを起動しました。             *****";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "*****                                                                      *****";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "********************************************************************************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   //TEST

   //***************************
   //***  エクセル処理　開始
   //***************************
   Heads_Excel_Main();
   //TEST
   //***************************
   //***  今回フォルダ移動処理
   //***************************
   MoveFolder_Job1();
   //TEST
   //2009.03.03 圧力容器強度計算変更
   //************************************
   //***  圧力容器強度計算ﾌﾟﾛｸﾞﾗﾑ  開始
   //************************************
   ////Windows 2008Server 移植対応
   if (FileExists(G_Err_Log_FileFName) == false) {
        Aturyoku_Run();
   }
   //*******************************
   //2017.10.19 塗装自主検査票_S
   //************************************
   //***  塗装自主検査票ﾌﾟﾛｸﾞﾗﾑ  開始
   //************************************
   if (FileExists(G_Err_Log_FileFName) == false) {
        TosoJisyuKensa_Run();
   }
   //2017.10.19 塗装自主検査票_E

   Screen->Cursor = crDefault;
//   PostMessage(Handle, WM_CLOSE, 0,0);

//   Release();  //フォームを破棄し，関連付けられているメモリを解放します。

   Application->Terminate();
}

//---------------------------------------------------------------------------
// 日本語関数名： エクセル作成　メインルーチン
// 概  要      ：
// 引  数      ： 初期値
// 戻り値      ： なし
// 備  考      ： なし
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::FormActivate(TObject *Sender)
{
   //*********************
   //***  Excel 終了
   //*********************
   QuitExcel();

   Screen->Cursor = crDefault;
//   PostMessage(Handle, WM_CLOSE, 0,0);

//   Release();  //フォームを破棄し，関連付けられているメモリを解放します。

   Application->Terminate();

}

//---------------------------------------------------------------------------
// 日本語関数名： エクセル作成　メインルーチン
// 概  要      ：
// 引  数      ： 初期値
// 戻り値      ： なし
// 備  考      ： なし
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::Heads_Excel_Main(void)
{
   short       fret;
   //int         i_Pos;
   AnsiString  s_FilePass;
   //2004.11.16 出力フォルダ及び訂正印変更
   AnsiString  Work,Rev;
   //*************************************
   // 2007.10.09 ﾐﾗｰ仕様対応(HeadsDataｸﾗｽ)
   bool bRet;
   HeadsData *pcHeadsData;
   int Sakuban;

   try{
      if (prvFrmLoadFlg == 0){
         prvFrmLoadFlg = 1;

         Refresh();
         Screen->Cursor = crHourGlass;

         //*************************
         //***  作番取得
         //*************************
         if(!Get_Sakuban()){
            return;
         }

         //**********************************
         //***  規格ｺｰﾄﾞ,業界,塗装色  読込
         //**********************************
         if (!Read_CodeMasta_Data()){
            return;
         }

         //*************************
         //***  ﾃﾞｰﾀﾍﾞｰｽ接続
         //*************************
         if (!Connect_DataBase()){
            return;
         }

         //2004.11.16 出力フォルダ及び訂正印変更
         //*************************************
         //***  HEADSﾃﾞｰﾀのRevのみ　読込
         //*************************************
         //TEST
         //s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + "H.txt";
         s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + ".txt";
         //TEST
         if (!Read_HEADS_Data_Rev(s_FilePass)){
            G_ErrLog_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』のＯＰＥＮに失敗しました！！";
            Write_Error_Log(G_ErrLog_Text);
            return;
         } else {
            G_Log_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』を読み込みました。";
            Write_Log(G_Log_Text);
         }
         //*************************************

         // 2007.10.09 ﾐﾗｰ仕様対応 最大3ﾌｧｲﾙ読込
         //*************************************
         //***  HEADSﾃﾞｰﾀのRevのみ　読込
         //***  (元仕様、ﾐﾗｰA仕様、ﾐﾗｰB仕様)
         //*************************************
         m_pcHeadsData = new HeadsData[3];
         bRet = m_pcHeadsData[0].ReadAllData(s_FilePass.c_str());

         if (!bRet){
            G_ErrLog_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』(元仕様)のＯＰＥＮに失敗しました！！";
            Write_Error_Log(G_ErrLog_Text);
            return;
         } else {
            G_Log_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』(元仕様)を読み込みました。";
            Write_Log(G_Log_Text);
         }

         // ﾐﾗｰ仕様の場合
         if(m_pcHeadsData[0].GetMirrorFlag()){
            // ﾐﾗｰA仕様のHEADSﾃﾞｰﾀ 読込
            s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + "A" + ".txt";
            bRet = m_pcHeadsData[1].ReadAllData(s_FilePass.c_str());

            if (!bRet){
                G_ErrLog_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』(ﾐﾗｰA仕様)のＯＰＥＮに失敗しました！！";
                Write_Error_Log(G_ErrLog_Text);
                return;
            } else {
                G_Log_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』(ﾐﾗｰA仕様)を読み込みました。";
                Write_Log(G_Log_Text);
            }

            // ﾐﾗｰB仕様のHEADSﾃﾞｰﾀ 読込
            s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + "B" + ".txt";
            bRet = m_pcHeadsData[2].ReadAllData(s_FilePass.c_str());

            if (!bRet){
                G_ErrLog_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』(ﾐﾗｰB仕様)のＯＰＥＮに失敗しました！！";
                Write_Error_Log(G_ErrLog_Text);
                return;
            } else {
                G_Log_Text = "HEADSﾃﾞｰﾀRev『" + s_FilePass + "』(ﾐﾗｰB仕様)を読み込みました。";
                Write_Log(G_Log_Text);
            }
		 }
         //*************************************

         //*************************
         //***  出力先ﾌｫﾙﾀﾞﾁｪｯｸ
         //*************************
         //2004.11.16 出力フォルダ及び訂正印変更
         // 奇数の判断
		 //2022/12/15 桁数追加対応 Mod_S
		 //Work = G_SAKUBAN.SubString(1, 4);
		 //if( Work.ToIntDef(0) % 2 == 1 ){
		 //	sprintf(Work.c_str(),"%04d",Work.ToIntDef(0) - 1);
		 //}else{
		 //	sprintf(Work.c_str(),"%04d",Work.ToIntDef(0));
		 //}
		 if (G_SAKUBAN.Length() > 7)
		 {
			  Work = G_SAKUBAN.SubString(1, 6);
		 }
		 else
		 {
			  Work = G_SAKUBAN.SubString(1, 4);
		 }
		 Sakuban = Work.ToIntDef(0);
		 if( Sakuban % 2 == 1 ){
			 Sakuban --;
		 }
		 if (Sakuban > 9999)
		 {
		   if (Sakuban > 109999)
		   {
			 Work.sprintf("%06d",Sakuban);
		   }
		   else
		   {
			 Sakuban -= 100000;
			 Work.sprintf("%04d",Sakuban);
		   }
		 }
		 else
		 {
		   Work.sprintf("%04d",Sakuban);
		 }
		 //2022/12/15 桁数追加対応 Mod_E
		 if (G_HEADS_DATA_Rev - 1 <= 0 ) {
            Rev.sprintf("%02d", 0);
         }else{
            Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
         }
         //*************************************
         if (P_Sakuban_Mode == "S"){
            // ｵｰﾀﾞｰ
            G_xls_Output_Dir = G_xls_Output_Dir + G_OUT_ORDER_Dir + G_SAKUBAN;
            //2004.11.16 出力フォルダ及び訂正印変更
            //マルチサーバ対応 2008/11/17 MOD START
			//G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HD_Root) + IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
			//2023/4/21 製造番号桁数追加対応 Mod
			//G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
			G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + GetFileSeizouBangou(G_SAKUBAN) + "\\" + Rev.c_str();
            //マルチサーバ対応 2008/11/17 MOD END
            //*************************************
         } else {
            // 見積
			G_xls_Output_Dir = G_xls_Output_Dir + G_OUT_ESTIM_Dir + G_SAKUBAN;
            //2004.11.16 出力フォルダ及び訂正印変更
            //マルチサーバ対応 2008/11/17 MOD START
			//G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HD_Root) + IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ESTIM_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
			G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ESTIM_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
            //マルチサーバ対応 2008/11/17 MOD END
            //*************************************
         }

         //TEST
		 G_Log_Text = "G_xls_Output_Dir_old『" + G_xls_Output_Dir_old + "』";
         Write_Log(G_Log_Text);
         //TEST

         G_xls_Output_Dir = IncludeTrailingBackslash(G_xls_Output_Dir);
         if (!OutPut_Folder_Check(G_xls_Output_Dir)){
            return;
         }

         // ﾌﾟﾚｰﾄ孔明ﾃﾞｰﾀ格納先
         //マルチサーバ対応 2009/1/13 MOD START
         //G_AnaFilePass = IncludeTrailingBackslash(G_HD_Root)
         //              + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
         //              + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
         //              + IncludeTrailingBackslash(G_SAKUBAN);
         G_AnaFilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                       + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                       + IncludeTrailingBackslash(G_SAKUBAN);
         //マルチサーバ対応 2009/1/13 MOD END

         //*************************
         //***  HEADSﾃﾞｰﾀ　読込
         //*************************
         //s_FilePass = G_HD_Root + G_HEADS_INP_Dir + G_CSV_Dir + G_HEADS_ID;

         //*******************************************
         //***  HEADSﾃﾞｰﾀ　読込
         //***  ( 02.08.07 読込ﾌｧｲﾙ名変更 ) csv→txt
         //*******************************************
         //s_FilePass = G_Input_Csv_Dir + G_HEADS_ID;
         s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;

         if (!Read_HEADS_Data(s_FilePass)){
            G_ErrLog_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』のＯＰＥＮに失敗しました！！";
            Write_Error_Log(G_ErrLog_Text);
            return;
         } else {
            G_Log_Text = "HEADSﾃﾞｰﾀ『" + s_FilePass + "』を読み込みました。";
            Write_Log(G_Log_Text);
         }

         //TEST
         G_Log_Text = "REV『" + G_HEADS_DATA[319] + "』";
         Write_Log(G_Log_Text);
         //TEST

         //*************************
         //***  HEADSﾃﾞｰﾀ　読込
         //*************************
         // 打出し区分(RecNo[1259] 打ち出し区分２桁目)
         P_UtiKbn = G_HEADS_DATA[1259].SubString(2,1);
         // ﾛｸﾞ作成
         G_Log_Text = "RecNo:1259『" + G_HEADS_DATA[1259] + "』から打出し区分『" + P_UtiKbn
                    + "』を取得しました。";
         Write_Log(G_Log_Text);

         //TEST
         //***************************
         //***  フォルダ移動　処理
         //***************************
         MoveFolder_Job();
         //TEST

         // 2019/05/17 応用技術追加
         bIsNotKatashiki = false;
         bMirrorFlag = false;

         //Windows 2008Server 移植対応
         G_Log_Text = "未登録の型式対応開始";
         Write_Log(G_Log_Text);
         G_Err_Log_FileFName = G_xls_Output_Dir + G_Err_Log_FileName;
         Write_Log(G_Err_Log_FileFName);
         if (FileExists(G_Err_Log_FileFName)) {
            Screen->Cursor = crDefault;
            //PostMessage(Handle, WM_CLOSE, 0,0);
            //return ;
            // 2019/05/17 応用技術追加
            bIsNotKatashiki = true;
         }

         //2020.04.20 温風乾燥出力_S
         //************************************
         //***  温風乾燥出力ﾌﾟﾛｸﾞﾗﾑ  開始
         //************************************
         if (FileExists(G_Err_Log_FileFName) == false) {
             HotAirDrying_Run();
         }
         //2020.04.20 温風乾燥出力_E

         //*************************
         //***  Excel 起動
         //*************************
         fret = CallExcel(false);
         if(fret != 0) // Excel 呼出し 起動失敗？
         {
            Screen->Cursor = crDefault;
            //PostMessage(Handle, WM_CLOSE, 0,0);
            return;
         }

         //*********************
         //***  各種帳票　作成
         //*********************
         Excel_Run();

         //*********************
         //***  HeasDataｸﾗｽ終了
         //*********************
         delete[] m_pcHeadsData;

         //*********************
         //***  ﾃﾞｰﾀﾍﾞｰｽ破棄
         //*********************
         if (DataModule1){
            DataModule1 = NULL;
         }

         //*********************
         //***  Excel 終了
         //*********************
         QuitExcel();

         // ﾛｸﾞ作成
         G_Log_Text = "EXCELデータ出力システムを終了しました。\n\n\n\n\n\n";
         Write_Log(G_Log_Text);
         Write_Error_Log(G_Log_Text);
      }
   }
   __finally{
      //*********************
      //***  ﾃﾞｰﾀﾍﾞｰｽ破棄
      //*********************
      if (DataModule1){
         delete DataModule1;
         DataModule1 = NULL;
      }

      //*********************
      //***  Excel 終了
      //*********************
      QuitExcel();

      Screen->Cursor = crDefault;
      //PostMessage(Handle, WM_CLOSE, 0,0);
   }
}

//---------------------------------------------------------------------------
// 日本語関数名： エクセル　各種帳票作成
// 概  要      ：
// 引  数      ： なし
// 戻り値      ： 結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::Excel_Run(void)
{
   int         i_Pos,i_Mojisu;
   AnsiString s_Syurui,s_Model,s_Text;

    int i;
    int i_MirrorLoop;
    bool bRet;
    HeadsData* pcHeadsData;

    if(!m_pcHeadsData[0].GetMirrorFlag()){
        i_MirrorLoop = 1;  // 元仕様のみ
    }else{
        i_MirrorLoop = 3;  // 元仕様、A仕様、B仕様
    }

    // 元仕様、ﾐﾗｰA仕様、ﾐﾗｰB仕様でﾙｰﾌﾟ
    for(i=0;i<i_MirrorLoop;i++){

        pcHeadsData = &m_pcHeadsData[i];
        // AnsiString型のHeadsﾃﾞｰﾀをﾓｼﾞｭｰﾙ変数にｾｯﾄ
        bRet = pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
        if(!bRet) return;

        // 仕様ﾀｲﾌﾟｺｰﾄﾞ、仕様ﾀｲﾌﾟ文字列を取得
        pcHeadsData->SetSpecType(i);
        pcHeadsData->SetSpecCode();

        // --> △2
        bCopySeisekishoFlag = true;
        // <-- △2

        //  プレート型式の決定
        s_Syurui = P_HEADS_DATA[34].TrimRight();                // HEADSﾃﾞｰﾀ 種類

        // RecNo.542ﾓﾃﾞﾙの取得方法の変更
        s_Model  = P_HEADS_DATA[35].TrimRight(); // HEADSﾃﾞｰﾀ ﾓﾃﾞﾙ
        //2009.06.19 UX-100R追加
        //EXの場合
        //s_Model後端部に"C","P","R","A","B"がある場合
        //その文字を順にs_Modelから除いていく
        //
        //EX以外の場合
        //s_Model後端部に"C","P","A","B"がある場合
        //その文字を順にs_Modelから除いていく
        //次に条件に合わせて以下の文字ををs_Modelから除く
        //  最後の文字が"D"の場合
        //　　：最後から２文字目の文字が"C","P","A","B"の場合
        //　　：その文字を順にs_Modelから除いていく
        //　　：次に最後から２文字目の文字(数字)を除く
        //  最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
        //  その他の場合                       ：最後の文字(数字)
        ////2006.02.06 SX-90M,L追加
        ////s_Model後端部に"C","P","R","A","B"がある場合
        ////その文字を順にs_Modelから除いていく
        ////次に条件に合わせて以下の文字ををs_Modelから除く
        ////EXの場合                           ：除かない
        ////2008.07.24 CX-10D追加
        ////最後の文字が"D"の場合　　　　　　　　　
        ////　　：最後から２文字目の文字が "C","P","R","A","B"の場合
        ////　　：その文字を順にs_Modelから除いていく
        ////　　：次に最後から２文字目の文字(数字)を除く
        ////*********************
        //**********************
        //最後の文字が数字でない("M","L","S")：最後から2文字目の文字(数字)
        //その他の場合                       ：最後の文字(数字)
        //
        //i_Mojisu = s_Model.Length();
        //if (i_Mojisu == 3){
        //   s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,2);
        //} else if (i_Mojisu == 4){
        //   s_Text = G_HEADS_DATA[35].TrimRight().SubString(4,1);
        //   if (s_Text.ToIntDef(999) != 999){
        //      s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,3);
        //   } else {
        //      s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,2);
        //   }
        //} else if (i_Mojisu == 5){
        //   s_Model = G_HEADS_DATA[35].TrimRight().SubString(1,3);
        //}
        if ( s_Model.SubString(s_Model.Length(),1) == "B" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        if ( s_Model.SubString(s_Model.Length(),1) == "A" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        //2009.06.19 UX-100R追加
        //if ( s_Model.SubString(s_Model.Length(),1) == "R" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        if ( s_Syurui.SubString(1,2) == "EX" && s_Model.SubString(s_Model.Length(),1) == "R" ) {
            s_Model = s_Model.SubString(1,s_Model.Length()-1);
        }
        //**********************
        if ( s_Model.SubString(s_Model.Length(),1) == "P" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        if ( s_Model.SubString(s_Model.Length(),1) == "C" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        if ( s_Syurui.SubString(1,2) == "EX" ) {
        //2008.07.24 CX-10D追加
        } else if ( s_Model.SubString(s_Model.Length(),1) == "D" ) {
            if ( s_Model.SubString(s_Model.Length()-1,1) == "B" ) {
                s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
            }
            if ( s_Model.SubString(s_Model.Length()-1,1) == "A" ) {
                s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
            }
            //2009.06.19 UX-100R追加
            //if ( s_Model.SubString(s_Model.Length()-1,1) == "R" ) {
            //    s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
            //}
            //**********************
            if ( s_Model.SubString(s_Model.Length()-1,1) == "P" ) {
                s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
            }
            if ( s_Model.SubString(s_Model.Length()-1,1) == "C" ) {
                s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
            }

            s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
        //*********************
        } else if ( s_Model.SubString(s_Model.Length(),1).ToIntDef(999)==999 ) {
            s_Model = s_Model.SubString(1,s_Model.Length()-2) + s_Model.SubString(s_Model.Length(),1);
        } else {
            s_Model = s_Model.SubString(1,s_Model.Length()-1);
        }
        //***********************

        G_K_Syurui   = "";    // 型式種類
        G_K_Model1   = "";    // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式1)
        G_K_Model2   = "";    // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2)
        G_KS_Syurui  = "";    // 総称種類
        G_KS_Model   = "";    // 総称ﾓﾃﾞﾙ
        G_Model_Zen1 = "";    // 型式ﾓﾃﾞﾙ全角1
        G_Model_Zen2 = "";    // 型式ﾓﾃﾞﾙ全角2

        G_Log_Text = "ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙを、種類『" + s_Syurui
                   + "』モデル『" + s_Model + "』で検索開始。";
        Write_Log(G_Log_Text);

        for(int i=0; i < G_PHENKAN_RecCnt; i++)
        {
            if (G_PHENKAN[i][0] == s_Syurui && G_PHENKAN[i][1] == s_Model){
                G_K_Syurui   = G_PHENKAN[i][0];    // 型式種類
                G_K_Model1   = G_PHENKAN[i][5];    // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式1)
                G_K_Model2   = G_PHENKAN[i][6];    // 型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2)
                G_KS_Syurui  = G_PHENKAN[i][2];    // 総称種類
                G_KS_Model   = G_PHENKAN[i][3];    // 総称ﾓﾃﾞﾙ
                G_Model_Zen1 = G_PHENKAN[i][7];    // 型式ﾓﾃﾞﾙ全角1
                G_Model_Zen2 = G_PHENKAN[i][8];    // 型式ﾓﾃﾞﾙ全角2

                break;
            }
        }

        G_Log_Text = "型式種類            『" + G_K_Syurui + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式1) 『" + G_K_Model1 + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "型式ﾓﾃﾞﾙ(ﾐｯｸｽ型式2) 『" + G_K_Model2 + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "総称種類            『" + G_KS_Syurui + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "総称ﾓﾃﾞﾙ            『" + G_KS_Model + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "型式ﾓﾃﾞﾙ全角1       『" + G_Model_Zen1 + "』を取得。";
        Write_Log(G_Log_Text);
        G_Log_Text = "型式ﾓﾃﾞﾙ全角2       『" + G_Model_Zen2 + "』を取得。";
        Write_Log(G_Log_Text);

        G_Log_Text = "ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙ  検索終了。";
        Write_Log(G_Log_Text);

        if( G_Output_Mode == 1 )
        {
            G_Log_Text = "★★★ 【G_Output_Mode == 1】Excel出力テスト起動 ★★★";
            Write_Log(G_Log_Text);
            // テスト用
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            // --> △2 追加
            //2019/05/17 応用技術追加
            //if (!bIsNotKatashiki) {
                Funeyousinsei(pcHeadsData); // 舶用申請書
                if(i == 0) bCopySeisekishoFlag = false;
            //}

            Seisekisyo(pcHeadsData);    // 検査成績書
            // <-- △2 追加
            Syonin(pcHeadsData);        // 承認図表紙
            if (!bIsNotKatashiki) {
                Funeyou(pcHeadsData);       // 舶用要目表
                Kobetusinsei(pcHeadsData);  // 個別検定申請書
                //2009.03.03 圧力容器強度計算変更
                //Kyodokeisan(pcHeadsData);   // 圧力容器強度計算書
                //*******************************
                Meisaisyo(pcHeadsData);     // 小型圧力容器 明細書
            }
            Mokuroku(pcHeadsData);      // 図面目録
            Seisekisyo(pcHeadsData);    // 検査成績書
            if (!bIsNotKatashiki) {
//Mod_Str 2019/02/19 Ri
                //Sijisyo(pcHeadsData);       // 製作指示書
                if (G_OLD_FLG == 0) {        //新製作指示書作成
                    SijisyoA3(pcHeadsData);
                }else if(G_OLD_FLG == 1){    //旧製作指示書作成
                    Sijisyo(pcHeadsData);
                }else{                       //新・旧製作指示書両方作成
                    SijisyoA3(pcHeadsData);
                    Sijisyo(pcHeadsData);
                }
//Mod_End 2019/02/19 Ri
            }
            Youryosyo(pcHeadsData);     // 検査要領書
            Buhinhyou(pcHeadsData, m_pcHeadsData);     // 標準部品+表
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        }
        else
        {
            G_Log_Text = "★★★ 【G_Output_Mode == 0】本番稼動 ★★★";
            Write_Log(G_Log_Text);

            if (P_UtiKbn == "1" || P_UtiKbn == "5" || P_UtiKbn == "7"){
                //**************************************
                //***  打出し区分　『1』,『5』,『7』
                //**************************************
                Syonin(pcHeadsData);         // 承認図表紙

            } else if (P_UtiKbn == "2") {
                //**************************
                //***  打出し区分　『2』
                // **************************
                Syonin(pcHeadsData);         // 承認図表紙

                i_Pos = P_HEADS_DATA[36].Pos("M");
                if (i_Pos != 0){
                    if (!bIsNotKatashiki) {
                        Funeyou(pcHeadsData);     // 舶用要目表
                    }
                    // --> △2
                    Funeyousinsei(pcHeadsData);
                    if(i == 0) bCopySeisekishoFlag = false;
                    // <-- △2
                    //}
                }

            } else if (P_UtiKbn == "3") {
                //**************************
                //***  打出し区分　『3』
                //**************************
                Syonin(pcHeadsData);         // 承認図表紙
                //2009.03.03 圧力容器強度計算変更
                //Kyodokeisan(pcHeadsData);    // 圧力容器強度計算書
                //*******************************
                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // 規格ｺｰﾄﾞが03:小型圧力容器構造規格
                        Kobetusinsei(pcHeadsData);  // 個別検定申請書
                        Meisaisyo(pcHeadsData);     // 小型圧力容器 明細書
                    }
                }

            } else if (P_UtiKbn == "4" && P_Sakuban_Mode == "S") {
                //**************************
                //***  打出し区分『4』
                //**************************
                Syonin(pcHeadsData);         // 承認図表紙
                //2009.03.03 圧力容器強度計算変更
                //Kyodokeisan(pcHeadsData);    // 圧力容器強度計算書
                //*******************************

                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // 規格ｺｰﾄﾞが03:小型圧力容器構造規格
                        Kobetusinsei(pcHeadsData);  // 個別検定申請書
                        Meisaisyo(pcHeadsData);     // 小型圧力容器 明細書
                    }
                }

                // 図面目録・標準部品表
                if (P_Sakuban_Mode == "S"){
                    Mokuroku(pcHeadsData);    // 図面目録
                    Buhinhyou(pcHeadsData, m_pcHeadsData);   // 標準部品表
                }

                // 舶用要目表
                i_Pos = P_HEADS_DATA[36].Pos("M");
                if (i_Pos != 0){
                    if (!bIsNotKatashiki) {
                        Funeyou(pcHeadsData);
                }
                // --> △2
                Funeyousinsei(pcHeadsData);
                if(i == 0) bCopySeisekishoFlag = false;
                // <-- △2
                //}
                }

            //2015.09.17 出図区分変更
            } else if (P_UtiKbn == "4" && P_Sakuban_Mode != "S") {
                // 舶用要目表
                i_Pos = P_HEADS_DATA[36].Pos("M");
                    if (i_Pos != 0){
                        if (!bIsNotKatashiki) {
                            Funeyou(pcHeadsData);
                    }
                    Funeyousinsei(pcHeadsData);
                    if(i == 0) bCopySeisekishoFlag = false;
                    //}
                }
                Youryosyo(pcHeadsData);      // 検査要領書
            //***********************

            } else if (P_UtiKbn == "6") {
                //************************
                //***  打出し区分『6』
                //************************
                Syonin(pcHeadsData);         // 承認図表紙
                if (!bIsNotKatashiki) {
                    Funeyou(pcHeadsData);        // 舶用要目表
                }
                // --> △2
                Funeyousinsei(pcHeadsData);
                if(i == 0) bCopySeisekishoFlag = false;
                //}

                // <-- △2
                //2009.03.03 圧力容器強度計算変更
                //Kyodokeisan(pcHeadsData);    // 圧力容器強度計算書
                //*******************************
                Youryosyo(pcHeadsData);      // 検査要領書

                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // 規格ｺｰﾄﾞが03:小型圧力容器構造規格
                        Kobetusinsei(pcHeadsData);  // 個別検定申請書
                        Meisaisyo(pcHeadsData);     // 小型圧力容器 明細書
                    }
                }

                // 図面目録・標準部品表
                //2011/01/07 図面目録・標準部品表を出力するように変更
                //if (P_Sakuban_Mode == "S") {
                    Mokuroku(pcHeadsData);    // 図面目録
                    Buhinhyou(pcHeadsData, m_pcHeadsData);   // 標準部品表
                //}

            } else if (P_UtiKbn == "8" ){
                //************************
                //***  打出し区分『8』
                //************************
                Syonin(pcHeadsData);         // 承認図表紙
                Youryosyo(pcHeadsData);      // 検査要領書
            }

            // 2018.07.09 出図区分変更_S
            else if (P_UtiKbn == "9") {
                //************************
                //***  打出し区分『9』
                //************************
                Syonin(pcHeadsData);         // 承認図表紙
                if (!bIsNotKatashiki) {
                    Funeyou(pcHeadsData);        // 舶用要目表
                }
                    Funeyousinsei(pcHeadsData);
                    if(i == 0) bCopySeisekishoFlag = false;
                //}

                Youryosyo(pcHeadsData);      // 検査要領書

                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // 規格ｺｰﾄﾞが03:小型圧力容器構造規格
                        Kobetusinsei(pcHeadsData);  // 個別検定申請書
                        Meisaisyo(pcHeadsData);     // 小型圧力容器 明細書
                    }
                }

                Mokuroku(pcHeadsData);    // 図面目録
                Buhinhyou(pcHeadsData, m_pcHeadsData);   // 標準部品表

            }
            // 2018.07.09 出図区分変更_E

            //2015.09.17 出図区分変更
            //************************
            //***  検査要領書
            //************************
            //if ( P_UtiKbn != "8" && P_UtiKbn != "6" ) {
            if ( P_UtiKbn == "6" || P_UtiKbn == "8" ) {
            // 2018.06.22 出図区分変更_S
            } else if ( P_UtiKbn == "9" ) {
            // 2018.06.22 出図区分変更_E
            } else if (P_UtiKbn == "4" && P_Sakuban_Mode != "S") {
            } else {
            //***********************
                if ( P_HEADS_DATA[443].TrimRight() == "1") {
                    Youryosyo(pcHeadsData);      // 検査要領書
                }
            }

            //************************
            //***  検査成績書
            //************************
            //2011/01/07 打出し区分『6』の場合、検査成績書を出力するように変更
            //if (P_Sakuban_Mode == "S") {
            if ( (P_Sakuban_Mode == "S") || (P_UtiKbn == "6") ) {
                //2017.12.26 アルファラバルプレート_S
                //Seisekisyo(pcHeadsData);     // 検査成績書
                if (P_UtiKbn == "7"){
                } else {
                    Seisekisyo(pcHeadsData); // 検査成績書
                }
                //2017.12.26 アルファラバルプレート_E
            }
            // 2018.06.22 出図区分変更_S
            else if ( P_UtiKbn == "9" ) {
                Seisekisyo(pcHeadsData); // 検査成績書
            }
            // 2018.06.22 出図区分変更_E

            //************************
            //***  製作指示書
            //************************
			if (P_Sakuban_Mode == "S" && P_HEADS_DATA[302].TrimRight() == "1") {
//Mod_Str 2019/02/19 Ri
                if (!bIsNotKatashiki) {
                    //Sijisyo(pcHeadsData);       // 製作指示書
                    if (G_OLD_FLG == 0) {        //新製作指示書作成
                        SijisyoA3(pcHeadsData);
                    }else if(G_OLD_FLG == 1){    //旧製作指示書作成
                        Sijisyo(pcHeadsData);
                    }else{                       //新・旧製作指示書両方作成
                        SijisyoA3(pcHeadsData);
                        Sijisyo(pcHeadsData);
                    }
                }
//Mod_End 2019/02/19 Ri
                //************************
                //***  マル標移動処理
                //************************
                if (!bIsNotKatashiki) {
                    MovePDFFile();
                }
            }
            // 2017.10.06 短納期対応_S
            if (P_Sakuban_Mode == "S" && P_HEADS_DATA[302].TrimRight() == "2") {
//Mod_Str 2019/02/19 Ri
                if (!bIsNotKatashiki) {
                    //Sijisyo(pcHeadsData);       // 製作指示書
                    if (G_OLD_FLG == 0) {        //新製作指示書作成
                        SijisyoA3(pcHeadsData);
                    }else if(G_OLD_FLG == 1){    //旧製作指示書作成
                        Sijisyo(pcHeadsData);
                    }else{                       //新・旧製作指示書両方作成
                        SijisyoA3(pcHeadsData);
                        Sijisyo(pcHeadsData);
                    }
                }
//Mod_End 2019/02/19 Ri
                //************************
                //***  マル標移動処理
                //************************
                if (!bIsNotKatashiki) {
                    MovePDFFile();
                }
            }
            // 2017.10.06 短納期対応_E
        }
   } // ﾐﾗｰﾙｰﾌﾟFor文End
}

//---------------------------------------------------------------------------
// 日本語関数名： ログファイル　サイズチェック
// 概  要      ： ﾛｸﾞﾌｧｲﾙのｻｲｽﾞを取得し、規定値を超えていたら過去ﾌｫﾙﾀﾞに移動させる
// 引  数      ： なし
// 戻り値      ： 結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Log_File_SizeCheck(void)
{
   AnsiString to_filename;    // 作成過去ﾛｸﾞﾌｧｲﾙ(ﾌﾙﾊﾟｽ)
   struct     stat statbuf;
   double     d_FileSize;

   //***************************
   //***  ﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
   //***************************
   // 作成先ﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
   stat(G_LogFilePath.c_str(), &statbuf);
   d_FileSize = statbuf.st_size;

   // ﾛｸﾞﾌｧｲﾙのｻｲｽﾞが設定値以上なら『年月日時分秒』+ﾌｧｲﾙ名で過去ﾛｸﾞﾌｫﾙﾀﾞへｺﾋﾟｰ
   if (d_FileSize >= G_Log_Max_Size){

      // 過去ﾛｸﾞ用ﾊﾟｽ作成
      to_filename = G_Rsrv_Log_Dir;
      to_filename = to_filename + FormatDateTime("yyyymmdd",Date());
      to_filename = to_filename + FormatDateTime("hhnnss",Time()) + CSYSLOGFILENAME;

      if(CopyFile(G_LogFilePath.c_str(),to_filename.c_str(), false))
      {
         DeleteFile(G_LogFilePath.c_str());
      } else {
         return false;
      }
   }

   //***************************
   //***  ｴﾗｰﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
   //***************************
   // 作成先ｴﾗｰﾛｸﾞﾌｧｲﾙ　ｻｲｽﾞﾁｪｯｸ
   stat(G_ErrLogFilePath.c_str(), &statbuf);
   d_FileSize = statbuf.st_size;

   // ｴﾗｰﾛｸﾞﾌｧｲﾙのｻｲｽﾞが設定値以上なら『年月日時分秒』+ﾌｧｲﾙ名で過去ｴﾗｰﾛｸﾞﾌｫﾙﾀﾞへｺﾋﾟｰ
   if (d_FileSize >= G_ErrLog_Max_Size){

      // 過去ｴﾗｰﾛｸﾞ用ﾊﾟｽ作成
      to_filename = G_Rsrv_ErrLog_Dir;
      to_filename = to_filename + FormatDateTime("yyyymmdd",Date());
      to_filename = to_filename + FormatDateTime("hhnnss",Time()) + CSYSERRLOGFILENAME;

      if(CopyFile(G_ErrLogFilePath.c_str(),to_filename.c_str(), false))
      {
         DeleteFile(G_ErrLogFilePath.c_str());
      } else {
         return false;
      }
   }

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： データベース　接続
// 概  要      ： mdbをODBC経由で接続する
// 引  数      ： なし
// 戻り値      ： 接続結果 [true:成功 false:失敗]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Connect_DataBase(void)
{
   try
   {
      // 2021.06.18 FireDACへの移行 ADD_S
	  AnsiString AIniPath; // 定義ﾌｧｲﾙﾊﾟｽ
	  AnsiString AIniFile; // 定義ﾌｧｲﾙﾊﾟｽ + ﾌｧｲﾙ名
	  // 2021.06.18 FireDACへの移行 ADD_E

      // AccessへのODBC接続
      DataModule1 = new TDataModule1(this);

      // 2021.06.18 FireDACへの移行 ADD_S
	  // 接続定義ファイル設定
	  AIniPath = ExtractFilePath(Application->ExeName);
	  AIniFile = AIniPath + CSYSCONNDEFFILENAME;
	  DataModule1->FDManagerDN->ConnectionDefFileName = AIniFile;
	  DataModule1->FDManagerDN->ConnectionDefFileAutoLoad = True;
	  DataModule1->FDManagerDN->Active = True;
	  // 2021.06.18 FireDACへの移行 ADD_E

      // 図番データベース(HD_DNO.mdb)
      G_ErrLog_Text = "図番データベース(HD_DNO.mdb)";

	  // 2021.06.18 FireDACへの移行 MOD_S
	  //DataModule1->Database_DNO->DatabaseName = ODBC_DSN_DNO;
	  DataModule1->Database_DNO->ConnectionName = ODBC_DSN_DNO;
	  // 2021.06.18 FireDACへの移行 MOD_E
      DataModule1->Database_DNO->Params->Clear();
      DataModule1->Database_DNO->LoginPrompt = false;
      DataModule1->Database_DNO->Connected = true;

      G_Log_Text = "図番データベース(HD_DNO.mdb)と接続しました。";
      Write_Log(G_Log_Text);

      // 検査要領書データベース(HD_INSPCTL.mdb)
      G_ErrLog_Text = "検査要領書データベース(HD_INSPCTL.mdb)";

	  // 2021.06.18 FireDACへの移行 MOD_S
	  //DataModule1->Database_MAT->DatabaseName = ODBC_DSN_INS;
	  DataModule1->Database_MAT->ConnectionName = ODBC_DSN_INS;
	  // 2021.06.18 FireDACへの移行 MOD_E
      DataModule1->Database_MAT->Params->Clear();
      DataModule1->Database_MAT->LoginPrompt = false;
      DataModule1->Database_MAT->Connected = true;

      G_Log_Text = "検査要領書データベース(HD_INSPCTL.mdb)と接続しました。";
      Write_Log(G_Log_Text);

      // 材質データベース(HD_MATERIAL.mdb)
      G_ErrLog_Text = "材質データベース(HD_MATERIAL.mdb)";

      // 2021.06.18 FireDACへの移行 MOD_S
	  //DataModule1->Database_INS->DatabaseName = ODBC_DSN_MAT;
	  DataModule1->Database_INS->ConnectionName = ODBC_DSN_MAT;
	  // 2021.06.18 FireDACへの移行 MOD_E
      DataModule1->Database_INS->Params->Clear();
      DataModule1->Database_INS->LoginPrompt = false;
      DataModule1->Database_INS->Connected = true;

      G_Log_Text = "材質データベース(HD_MATERIAL.mdb)と接続しました。";
      Write_Log(G_Log_Text);
   }
   catch(EDatabaseError &Error)
   {
      G_ErrLog_Text = G_ErrLog_Text + "への接続に失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 規格ｺｰﾄﾞ，業界ｺｰﾄﾞ，塗装色　読込
// 概  要      ： 『KIKAKU.CSV』,『GYOKAI.CSV』,『TOSOU.CSV』を読み込む
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Read_CodeMasta_Data(void)
{
   char  buf[1024];                            /* 文字列バッファ   */
   char  *tok;                                 /* トークンポインタ */
   int   i;                                    /* ループカウンタ   */
   char  token[100][100];                      /* トークンバッファ */

   int   RecNo;

   AnsiString wk_FilePass;
   FILE *fp;

   //***********************
   //**  規格ｺｰﾄﾞ ｾｯﾄ
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "KIKAKU.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>70) {
            break;
         }
      }

      G_KIKAKU[RecNo][0] = token[0];
      G_KIKAKU[RecNo][1] = token[1];
      G_KIKAKU[RecNo][2] = token[2];
      G_KIKAKU[RecNo][3] = token[3];
      G_KIKAKU[RecNo][4] = token[4];

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_KIKAKU_RecCnt = RecNo;

   //***********************
   //**  業界ｺｰﾄﾞ ｾｯﾄ
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "GYOKAI.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>70) {
            break;
         }
      }

      G_GYOKAI[RecNo][0] = token[0];
      G_GYOKAI[RecNo][1] = token[1];

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_GYOKAI_RecCnt = RecNo;

   //***********************
   //**  塗装色 ｾｯﾄ
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "TOSOU.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>70) {
            break;
         }
      }

      G_TOSOU[RecNo][0] = token[0];
      G_TOSOU[RecNo][1] = token[1];
      G_TOSOU[RecNo][2] = token[2];

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_TOSOU_RecCnt = RecNo;

   //***************************
   //**  ﾌﾟﾚｰﾄ型式変換ﾃｰﾌﾞﾙ
   //***************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "PHENKAN.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>100) {
            break;
         }
      }

      G_PHENKAN[RecNo][0] = token[0];
      G_PHENKAN[RecNo][1] = token[1];
      G_PHENKAN[RecNo][2] = token[2];
      G_PHENKAN[RecNo][3] = token[3];
      G_PHENKAN[RecNo][4] = token[4];
      G_PHENKAN[RecNo][5] = token[5];
      G_PHENKAN[RecNo][6] = token[6];
      G_PHENKAN[RecNo][7] = token[7];
      G_PHENKAN[RecNo][8] = token[8];
      G_PHENKAN[RecNo][9] = token[9];

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_PHENKAN_RecCnt = RecNo;

   //******************************
   //**  ﾉｽﾞﾙ構成部品ｺｰﾄﾞ ﾃｰﾌﾞﾙ
   //******************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "NOZZLECD.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>100) {
            break;
         }
      }

      G_NOZZLECD[RecNo][0] = token[0];    // ｺｰﾄﾞ
      G_NOZZLECD[RecNo][1] = token[1];    // 部品分類ｺｰﾄﾞ
      G_NOZZLECD[RecNo][2] = token[2];    // 備考(部品区分)
      G_NOZZLECD[RecNo][3] = token[3];    // 備考(部品名称)
      G_NOZZLECD[RecNo][4] = token[4];    // 予備品集計用(?)
      G_NOZZLECD[RecNo][5] = token[5];    // 連番(?)

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_NOZZLECD_RecCnt = RecNo;

   //******************************
   //**  温度計構成部品ｺｰﾄﾞ ﾃｰﾌﾞﾙ
   //******************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "TEMPCD.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>100) {
            break;
         }
      }

      G_TEMPCD[RecNo][0] = token[0];    // ｺｰﾄﾞ
      G_TEMPCD[RecNo][1] = token[1];    // 部品分類ｺｰﾄﾞ
      G_TEMPCD[RecNo][2] = token[2];    // 備考(部品区分)
      G_TEMPCD[RecNo][3] = token[3];    // 備考(部品名称)
      G_TEMPCD[RecNo][4] = token[4];    // 予備品集計用(?)
      G_TEMPCD[RecNo][5] = token[5];    // 連番(?)

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_TEMPCD_RecCnt = RecNo;

   //******************************
   //**  圧力計構成部品ｺｰﾄﾞ ﾃｰﾌﾞﾙ
   //******************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "PRESSGCD.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>100) {
            break;
         }
      }

      G_PRESSCD[RecNo][0] = token[0];    // ｺｰﾄﾞ
      G_PRESSCD[RecNo][1] = token[1];    // 識別子
      G_PRESSCD[RecNo][2] = token[2];    // 備考(?)
      G_PRESSCD[RecNo][3] = token[3];    // 部品名称
      G_PRESSCD[RecNo][4] = token[4];    // 予備品

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_PRESSCD_RecCnt = RecNo;

   // 2015.01.16 ノズル色変更
   //***********************
   //**  ブーツ色 ｾｯﾄ
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "BOOTSCOLOR.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>70) {
            break;
         }
      }

      G_BOOTCL[RecNo][0] = token[0];
      G_BOOTCL[RecNo][1] = token[1];
      G_BOOTCL[RecNo][2] = token[2];

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_BOOTCL_RecCnt = RecNo;
   // ***********************　

   // 2016/09/14 ステッカー追加
   //***********************
   //**  ステッカー ｾｯﾄ
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "STACKER.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // ｴﾗｰﾛｸﾞ作成
      G_ErrLog_Text = "『" + wk_FilePass + "』のＯＰＥＮに失敗しました！！";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // 閉じる
      return false;
   }

   // 読込み可能の間
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // トークンカウンタ初期化
      memset(token,0x00,sizeof(token));      // トークンバッファ初期化
      tok = GetToken(buf,",");               // ","で項目を区切る
      while(tok){                            // トークンが存在する間
         sprintf(token[i++],"%-.99s",tok);   // トークンをバッファに転送
         tok = GetToken(NULL,",");           // 次のトークンを検索

         if (i>70) {
            break;
         }
      }

      G_STACKER[RecNo][0] = token[0];
      G_STACKER[RecNo][1] = token[1];
      G_STACKER[RecNo][2] = token[2];

      RecNo++;
   }

   fclose(fp);    // 閉じる

   G_STACKER_RecCnt = RecNo;
   // *************************　

   //delete fp;
   return true;
}

//---------------------------------------------------------------------------
// 日本語関数名： 作成ファイル出力先フォルダ　チェック
// 概  要      ： 調査対象のﾌｫﾙﾀﾞを検索し、ﾌｫﾙﾀﾞが存在しなければ作成する。
// 引  数      ： AnsiString Check_Pass  // HEADS出力ﾌｫﾙﾀﾞ + 作番
// 戻り値      ： 検査結果 [true:正常 false:異常]
// 備  考      ： なし
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::OutPut_Folder_Check(AnsiString Check_Pass)
{
   if (!DirectoryExists(Check_Pass)){
      if (!ForceDirectories(Check_Pass)){
         // フォルダ作成　失敗
         G_ErrLog_Text = "フォルダ『 " + Check_Pass + "』が作成できませんでした！！";
         Write_Error_Log(G_ErrLog_Text);
         return false;
      } else {
         // フォルダ作成　成功
         G_Log_Text = "フォルダ『 " + Check_Pass + "』を新規作成しました。";;
         Write_Log(G_Log_Text);
      }
   }

   return true;
}

//-----------------------------------------------------------------------------
// 日本語関数名： Excel起動処理
// 概要        ： Excelの起動を行う
// パラメタ    ： bool  showflg   起動表示フラグ
// 戻り値      ： 起動結果[0:正常起動,1:ｴﾗｰ]
// 機能説明    ：
// 備考        ：
//-----------------------------------------------------------------------------
short __fastcall TfrmHDP010::CallExcel(bool showflg)
{
   try
   {
      if( exApplication.Type() != varDispatch )   // Excel未起動？
      {
         // Excelｵﾌﾞｼﾞｪｸﾄを作成
         exApplication = Variant::CreateObject( "Excel.Application" );
         // Excelｵﾌﾞｼﾞｪｸﾄを非表示とする
         exApplication.OlePropertySet( "Visible", false );
      }

      // ExcelｵﾌﾞｼﾞｪｸﾄのWorkBooksｺﾚｸｼｮﾝを取得
      exWorkbooks = exApplication.OlePropertyGet( "Workbooks" );

      // Excelｵﾌﾞｼﾞｪｸﾄを表示/非表示とする
      exApplication.OlePropertySet( "Visible", showflg );
      // 警告ﾒｯｾｰｼﾞを表示しないにする
      //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
      //Windows 2008Server 移植対応
      exApplication.OlePropertySet("DisplayAlerts", false);

      return 0;        // 戻り値[0:正常起動]
   }
   catch(...)
   {
      // ｴﾗｰﾛｸﾞ追加
      G_ErrLog_Text = "Excelを起動できませんでした！！hogehoge";
      Write_Error_Log(G_ErrLog_Text);

      QuitExcel();
      return (-1);    // 戻り値[-1:起動異常]
   }
}

//-----------------------------------------------------------------------------
// 日本語関数名： Excel終了処理
// 概要        ： Excelの終了を行う
// パラメタ    ：
// 戻り値      ：
// 機能説明    ：
// 備考        ：
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::QuitExcel(void)
{
   //Variantの開放
   exWorkCell.Clear();
   exWorkbooks.Clear();
   exWorkbook.Clear();
   exWorksheets.Clear();
   exWorksheet.Clear();

   // Excel終了処理
   if( exApplication.Type() == varDispatch )
   {
      // Excelｵﾌﾞｼﾞｪｸﾄを閉じる
      exApplication.Exec(Function("Quit"));
      //Variantの開放
      exApplication.Clear();
   }

}
//-----------------------------------------------------------------------------
// 日本語関数名： PDFﾌｧｲﾙ移動処理
// 概要        ： ﾏﾙ標の場合にPDFのｺﾋﾟｰを行う
// パラメタ    ：
// 戻り値      ：
// 機能説明    ：
// 備考        ： 2004/09/23 追加
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::MovePDFFile(void)
{
   //マル標判断
   if (P_HEADS_DATA[301].TrimRight() == "0"){
      // マル標以外
      G_Log_Text = "----------------マル標以外----------------";
      Write_Log(G_Log_Text);
      return;
   }

   // ファイルコピー
   G_Log_Text = "----------------マル標ファイルコピー開始----------------";
   Write_Log(G_Log_Text);

   // フォルダの作成
   G_OUT_DIRECT_Dir = G_OUT_DIRECT_Dir + FormatDateTime("yyyymmdd",Date());
   G_OUT_DIRECT_Dir = IncludeTrailingBackslash(G_OUT_DIRECT_Dir);

   if (!OutPut_Folder_Check(G_OUT_DIRECT_Dir)){
      return;
   }

   // 対象ファイルの検索、コピー
   OutPut_File_Move(G_xls_Output_Dir, G_OUT_DIRECT_Dir, "MAN*.pdf",false);

   // ファイルコピー
   G_Log_Text = "----------------マル標ファイルコピー終了----------------";
   Write_Log(G_Log_Text);

}

//-----------------------------------------------------------------------------
// 日本語関数名： PDFﾌｧｲﾙ移動処理
// 概要        ： ﾌｧｲﾙの検索、ｺﾋﾟｰ
// パラメタ    ：
// 戻り値      ：
// 機能説明  ：
// 備考        ： 2004/09/27 追加
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::OutPut_File_Move(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg)
{
   HANDLE han;
   WIN32_FIND_DATA wfd;
   bool bRet;
   AnsiString SearchFile;

   SearchFile = FromFile + Files;


   G_Log_Text = "検索ファイル『" + SearchFile + "』";
   Write_Log(G_Log_Text);

   if((han=FindFirstFile((SearchFile).c_str(), &wfd))!=INVALID_HANDLE_VALUE)
   {
      do
      {
         //2004.11.16 出力フォルダ及び訂正印変更
         //if ((((AnsiString)wfd.cFileName).c_str() == ".") || (((AnsiString)wfd.cFileName).c_str() == "..")) continue;
         //bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
         //if(bRet == false){
         //   G_Log_Text = "ファイルのコピーに失敗しました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
         //}else{
         //   G_Log_Text = "ファイルのコピーにしました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
         //}
         //Write_Log(G_Log_Text);
         //
         //// ファイルの削除
         //if(DelFlg){
         //   bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
         //   if(bRet == false){
         //      G_Log_Text = "ファイルの削除に失敗しました。『 " + FromFile + (AnsiString)wfd.cFileName + " 』";
         //      Write_Log(G_Log_Text);
         //   }
         //}
         if(strcmp(wfd.cFileName,".")!=0 && strcmp(wfd.cFileName,"..")!=0){
            bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
            if(bRet == false){
               G_Log_Text = "ファイルのコピーに失敗しました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
            }else{
               G_Log_Text = "ファイルのコピーにしました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
            }
            Write_Log(G_Log_Text);

            // ファイルの削除
            if(DelFlg){
               bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
               if(bRet == false){
                  G_Log_Text = "ファイルの削除に失敗しました。『 " + FromFile + (AnsiString)wfd.cFileName + " 』";
                  Write_Log(G_Log_Text);
               }
            }
         }
         //*************************************
      }
      while(FindNextFile(han,&wfd));
      FindClose(han);
   }
}

//-----------------------------------------------------------------------------
// 日本語関数名： フォルダ移動処理
// 概要        ： ﾌｫﾙﾀﾞ内全てのﾌｧｲﾙの移動
// パラメタ    ：
// 戻り値      ：
// 機能説明  ：
// 備考        ： 2004/09/27 追加
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::MoveFolder_Job(void)
{
   bool bRet, bRet1;
   AnsiString FormFolder, ToFolder, FormFolder1, ToFolder1, Rev;
   AnsiString Work, TooFolder, TooFolder1;
   int Sakuban, Number;

   // フォルダの移動
   G_Log_Text = "----------------フォルダ移動開始----------------";
   Write_Log(G_Log_Text);

   // 移動先フォルダの作成
   //2022/12/15 桁数追加対応 Mod_S
   //Work = G_SAKUBAN.SubString(1, 4);
   if (G_SAKUBAN.Length() > 7)
   {
	  Work = G_SAKUBAN.SubString(1, 6);
   }
   else
   {
	  Work = G_SAKUBAN.SubString(1, 4);
   }
   //2022/12/15 桁数追加対応 Mod_E
   if( P_Sakuban_Mode == "S" )
   {
	  // 奇数の判断
	  Sakuban = Work.ToIntDef(0);
	  if( Sakuban % 2 == 1 ){
		 Sakuban --;
	  }
	  //2022/12/15 桁数追加対応 Mod_S
	  //sprintf(Work.c_str(),"%04d",Sakuban);
	  if (Sakuban > 9999)
	  {
		  if (Sakuban > 109999)
		  {
			  Work.sprintf("%06d",Sakuban);
		  }
		  else
		  {
			  Sakuban -= 100000;
			  Work.sprintf("%04d",Sakuban);
		  }
	  }
	  else
	  {
		  Work.sprintf("%04d",Sakuban);
	  }
	  //2022/12/15 桁数追加対応 Mod_E

      ToFolder  = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
      ToFolder1 = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
      FormFolder  = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
      FormFolder1 = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
   }
   else if( P_Sakuban_Mode == "M" )
   {
      ToFolder  = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
      ToFolder1 = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
      FormFolder  = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
      FormFolder1 = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);

   }

   // HEADS_DATA
   //マルチサーバ対応 2009/1/13 MOD START
   //FormFolder  = IncludeTrailingBackslash(G_HD_Root)
   //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //            + IncludeTrailingBackslash(FormFolder)
   //            + IncludeTrailingBackslash(G_SAKUBAN);

   //ToFolder  = IncludeTrailingBackslash(G_HD_Root)
   //          + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //          + IncludeTrailingBackslash(ToFolder)
   //          + IncludeTrailingBackslash(Work)
   //          + IncludeTrailingBackslash(G_SAKUBAN);
   FormFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
               + IncludeTrailingBackslash(FormFolder)
               + IncludeTrailingBackslash(G_SAKUBAN);

   //2023/4/21 製造番号桁数追加対応 Mod
   //ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //			 + IncludeTrailingBackslash(ToFolder)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
			 + IncludeTrailingBackslash(ToFolder)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //マルチサーバ対応 2009/1/13 MOD END

   // HEADS_OUTPUT
   //マルチサーバ対応 2008/11/17 MOD START
   //FormFolder1  = IncludeTrailingBackslash(G_HD_Root)
   //            + IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //            + IncludeTrailingBackslash(FormFolder1)
   //            + IncludeTrailingBackslash(G_SAKUBAN);

   //ToFolder1 = IncludeTrailingBackslash(G_HD_Root)
   //          + IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //          + IncludeTrailingBackslash(ToFolder1)
   //          + IncludeTrailingBackslash(Work)
   //          + IncludeTrailingBackslash(G_SAKUBAN);
   FormFolder1  = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
               + IncludeTrailingBackslash(FormFolder1)
               + IncludeTrailingBackslash(G_SAKUBAN);

   //2023/4/21 製造番号桁数追加対応 Mod
   //ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //			 + IncludeTrailingBackslash(ToFolder1)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
			 + IncludeTrailingBackslash(ToFolder1)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //マルチサーバ対応 2008/11/17 MOD END

   // ログ
   G_Log_Text = "検索フォルダ（FormFolder）『" + FormFolder + "』";
   Write_Log(G_Log_Text);
   G_Log_Text = "検索フォルダ（FormFolder1）『" + FormFolder1 + "』";
   Write_Log(G_Log_Text);
   G_Log_Text = "検索フォルダ（ToFolder）『" + ToFolder + "』";
   Write_Log(G_Log_Text);
   G_Log_Text = "検索フォルダ（ToFolder1）『" + ToFolder1 + "』";
   Write_Log(G_Log_Text);

   // HEADS_OUTPUT
   // フォルダの有無
   bRet = DirectoryExists(ToFolder);
   if (bRet == FALSE){
      // フォルダの作成
      if (!OutPut_Folder_Check(ToFolder)){
         G_Log_Text = "!!!!!!!!!!!!!!!!フォルダ移動異常!!!!!!!!!!!!!!!!";
         Write_Log(G_Log_Text);
         return;
      }
   }

   // HEADS_DATA
   // フォルダの有無
   bRet1 = DirectoryExists(ToFolder1);
   if (bRet1 == FALSE){
      // フォルダの作成
      if (!OutPut_Folder_Check(ToFolder1)){
         G_Log_Text = "!!!!!!!!!!!!!!!!フォルダ移動異常!!!!!!!!!!!!!!!!";
         Write_Log(G_Log_Text);
         return;
      }
   }

   //2004.11.16 出力フォルダ及び訂正印変更
   // 既存に対応するためのフラグをセットする
   bRetRev = false;

   // 既存のフォルダをチェックする
   AnsiString nextFileName;
   WIN32_FIND_DATA foundFile;
   HANDLE hFile = FindFirstFile((FormFolder + "*.*").c_str(),&foundFile);

   if(hFile != INVALID_HANDLE_VALUE)
   {
     do
     {
       //If a found file is . or .. then skip
       if(strcmp(foundFile.cFileName,".")!=0 && strcmp(foundFile.cFileName,"..")!=0)
       {
         //If the file is a directory
         if((foundFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
         {
            G_Log_Text = "フォルダ発見!!!『" + (AnsiString)(foundFile.cFileName) + "』";
            Write_Log(G_Log_Text);
            bRetRev = true;
         }
       }
     }
     while(FindNextFile(hFile,&foundFile)!=0);
   }

   FindClose(hFile);
   //*************************************

   // 作番フォルダがない場合
   if (bRet == FALSE || bRet1 == FALSE){
        // コピー
     //MoveDirectoryEvo(FormFolder, ToFolder);
     MoveDirectoryEvo1(FormFolder, ToFolder);
     G_Log_Text = "フォルダのコピーしました from  『 " + FormFolder + " 』  to  『 " + ToFolder + " 』";
     Write_Log(G_Log_Text);

     //MoveDirectoryEvo(FormFolder1, ToFolder1);
     MoveDirectoryEvo1(FormFolder1, ToFolder1);
     G_Log_Text = "フォルダのコピーしました from  『 " + FormFolder1 + " 』  to  『 " + ToFolder1 + " 』";
     Write_Log(G_Log_Text);

   }else{
      // Revの取得
      //2004.11.16 出力フォルダ及び訂正印変更
      //Number = G_HEADS_DATA[318].ToIntDef(0);
      //Rev.sprintf("%02d",Number);
      if (G_HEADS_DATA_Rev -1 <= 0 ) {
         Rev.sprintf("%02d",0 );
      }else{
         Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
      }
      //*************************************
      G_Log_Text = "リビジョン (Rev) 『 " + Rev + " 』";
      Write_Log(G_Log_Text);

      // HEADS_OUTPUT
      TooFolder  = IncludeTrailingBackslash(ToFolder) + Rev + "\\";

      // HEADS_DATA
      TooFolder1 = IncludeTrailingBackslash(ToFolder1) + Rev + "\\";

      // HEADS_OUTPUT
      // フォルダの有無
      bRet = DirectoryExists(TooFolder);
      if (bRet == FALSE){
         // フォルダの作成
         if (!OutPut_Folder_Check(TooFolder)){
            G_Log_Text = "!!!!!!!!!!!!!!!!フォルダ移動異常!!!!!!!!!!!!!!!!";
            Write_Log(G_Log_Text);
            return;
         }
      }

      // HEADS_DATA
      // フォルダの有無
      bRet1 = DirectoryExists(TooFolder1);
      if (bRet1 == FALSE){
         // フォルダの作成
         if (!OutPut_Folder_Check(TooFolder1)){
            G_Log_Text = "!!!!!!!!!!!!!!!!フォルダ移動異常!!!!!!!!!!!!!!!!";
            Write_Log(G_Log_Text);
            return;
         }
      }

     // TEST
     // Revフォルダがない場合
     // if (bRet == FALSE || bRet1 == FALSE){
        // コピー
        //MoveDirectoryEvo(ToFolder, TooFolder);
        MoveDirectoryEvo1(ToFolder, TooFolder);
        G_Log_Text = "フォルダのコピーしました・ from  『 " + ToFolder + " 』  to  『 " + TooFolder + " 』";
        Write_Log(G_Log_Text);

        //MoveDirectoryEvo(ToFolder1, TooFolder1);
        MoveDirectoryEvo1(ToFolder1, TooFolder1);
        G_Log_Text = "フォルダのコピーしました・ from  『 " + ToFolder1 + " 』  to  『 " + TooFolder1 + " 』";
        Write_Log(G_Log_Text);

        //MoveDirectoryEvo(FormFolder, ToFolder);
        MoveDirectoryEvo1(FormFolder, ToFolder);
        G_Log_Text = "フォルダのコピーしました。 from  『 " + FormFolder + " 』  to  『 " + ToFolder + " 』";
        Write_Log(G_Log_Text);

        //MoveDirectoryEvo(FormFolder1, ToFolder1);
        MoveDirectoryEvo1(FormFolder1, ToFolder1);
        G_Log_Text = "フォルダのコピーしました。 from  『 " + FormFolder1 + " 』  to  『 " + ToFolder1 + " 』";
        Write_Log(G_Log_Text);


     //}else{
     //   // 上書きコピー
     //   MoveDirectoryEvo(FormFolder, ToFolder);
     //   G_Log_Text = "フォルダのコピーしました。 from  『 " + FormFolder + " 』  to  『 " + ToFolder + " 』";
     //   Write_Log(G_Log_Text);
     //
     //   MoveDirectoryEvo(FormFolder1, ToFolder1);
     //   G_Log_Text = "フォルダのコピーしました。 from  『 " + FormFolder1 + " 』  to  『 " + ToFolder1 + " 』";
     //   Write_Log(G_Log_Text);
     //
     //}
   }
   // フォルダの削除
   //2004.11.16 出力フォルダ及び訂正印変更
   //if(!RemoveDirectory(FormFolder.c_str())){
   //TEST
   //if(!removeDirectoryEvo(FormFolder.c_str())){
   ////*************************************
   //   G_Log_Text = "フォルダの移動に失敗しました。『 " + FormFolder + " 』";
   //   Write_Log(G_Log_Text);
   //}
   //TEST

   //2004.11.16 出力フォルダ及び訂正印変更
   //if(!RemoveDirectory(FormFolder1.c_str())){
   //TEST
   //if(!removeDirectoryEvo(FormFolder1.c_str())){
   ////*************************************
   //   G_Log_Text = "フォルダの移動に失敗しました。『 " + FormFolder1 + " 』";
   //   Write_Log(G_Log_Text);
   //}
   //TEST

   // フォルダの移動
   G_Log_Text = "----------------フォルダ移動終了----------------";
   Write_Log(G_Log_Text);
}

//-----------------------------------------------------------------------------
// 日本語関数名： WindowsAPIのRemoveDirectory改造版
// 概要        ： ﾌｫﾙﾀﾞ内全てのﾌｧｲﾙの移動
// パラメタ    ：
// 戻り値      ：
// 機能説明  ：
// 備考        ： 2004/10/01 追加
//-----------------------------------------------------------------------------
bool __fastcall TfrmHDP010::MoveDirectoryEvo(AnsiString FromFile, AnsiString ToFile)
{
  bool retVal =true;
  AnsiString nextFileName;

  WIN32_FIND_DATA foundFile;

  G_Log_Text = "フォルダ Start FromFile 『 " + FromFile + " 』";
  Write_Log(G_Log_Text);
  G_Log_Text = "フォルダ Start ToFile 『 " + ToFile + " 』";
  Write_Log(G_Log_Text);

  HANDLE hFile = FindFirstFile((FromFile + "*.*").c_str(),&foundFile);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      Write_Log(foundFile.cFileName);

      //If a found file is . or .. then skip
      if(strcmp(foundFile.cFileName,".")!=0 && strcmp(foundFile.cFileName,"..")!=0)
      {
        //If the file is a directory
        if((foundFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
        {
          //2004.11.16 出力フォルダ及び訂正印変更
          //OutPut_Folder_Check(ToFile);
          //MoveDirectoryEvo(FromFile + foundFile.cFileName + "\\", ToFile + foundFile.cFileName + "\\");
          if (bRetRev == true ){
             OutPut_Folder_Check(ToFile);
             MoveDirectoryEvo(FromFile + foundFile.cFileName + "\\", ToFile + foundFile.cFileName + "\\");
          }
          //*************************************
        }
        //If the file is a file
        else
        {
          OutPut_Folder_Check(ToFile);
          OutPut_File_Move(FromFile , ToFile, "*.*", true);
        }
      }
    }
    while(FindNextFile(hFile,&foundFile)!=0);
  }

  FindClose(hFile);

  //2004.11.16 出力フォルダ及び訂正印変更
  //G_Log_Text = "フォルダ RemoveDirectory FromFile 『 " + FromFile + " 』";
  //Write_Log(G_Log_Text);
  //
  ////Delete starting point itseft
  //if(RemoveDirectory(FromFile.c_str())==0)retVal=false;
  //*************************************

  return retVal;
}

//-----------------------------------------------------------------------------
// 日本語関数名： RemoveDirectory改造版
// 概要        ： ﾌｫﾙﾀﾞ内全ての削除
// パラメタ    ：
// 戻り値      ：
// 機能説明    ：
// 備考        ： 2004/11/04 add
//-----------------------------------------------------------------------------
bool __fastcall TfrmHDP010::removeDirectoryEvo(AnsiString fileName)
{
  bool retVal =true;
  AnsiString nextFileName;

  WIN32_FIND_DATA foundFile;

  HANDLE hFile = FindFirstFile((fileName + "*.*").c_str(),&foundFile);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(strcmp(foundFile.cFileName,".")!=0 && strcmp(foundFile.cFileName,"..")!=0)
      {

        nextFileName = fileName + "\\" + foundFile.cFileName;

        //If the file is a directory
        if((foundFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
        {
          removeDirectoryEvo(nextFileName.c_str());
          RemoveDirectory(nextFileName.c_str());
        }
        //If the file is a file
        else
        {
          DeleteFile(nextFileName.c_str());
        }
      }
    }
    while(FindNextFile(hFile,&foundFile)!=0);
  }

  FindClose(hFile);

  //Delete starting point itseft
  if(RemoveDirectory(fileName.c_str())==0)retVal=false;

  return retVal;
}


//-----------------------------------------------------------------------------
// 日本語関数名： 今回作成ファイルの移動
// 概要        ： 今回作成ﾌｧｲﾙ(ﾌｫﾙﾀﾞのﾙｰﾄ直下のﾌｧｲﾙ)の移動
// パラメタ    ：
// 戻り値      ：
// 機能説明    ：
// 備考        ： TEST
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::MoveFolder_Job1(void)
{
   bool bRet, bRet1;
   AnsiString FormFolder, ToFolder, FormFolder1, ToFolder1, Rev;
   AnsiString Work, TooFolder, TooFolder1;
   int Sakuban, Number;
   AnsiString sRev;
   AnsiString sFirstChar;
   int i_PosIdx;
   AnsiString sNumberList;


   // フォルダの移動
   G_Log_Text = "----------------フォルダ移動開始----------------";
   Write_Log(G_Log_Text);

   // 移動先フォルダの作成
   //2022/12/15 桁数追加対応 Mod_S
   //Work = G_SAKUBAN.SubString(1, 4);
   if (G_SAKUBAN.Length() > 7)
   {
	  Work = G_SAKUBAN.SubString(1, 6);
   }
   else
   {
	  Work = G_SAKUBAN.SubString(1, 4);
   }
   //2022/12/15 桁数追加対応 Mod_E
   if( P_Sakuban_Mode == "S" )
   {
      // 奇数の判断
      Sakuban = Work.ToIntDef(0);
	  if( Sakuban % 2 == 1 ){
         Sakuban --;
	  }
	  //2022/12/15 桁数追加対応 Mod_S
	  //sprintf(Work.c_str(),"%04d",Sakuban);
	  if (Sakuban > 9999)
	  {
		  if (Sakuban > 109999)
		  {
			  Work.sprintf("%06d",Sakuban);
		  }
		  else
		  {
			  Sakuban -= 100000;
			  Work.sprintf("%04d",Sakuban);
		  }
	  }
	  else
	  {
		  Work.sprintf("%04d",Sakuban);
	  }
	  //2022/12/15 桁数追加対応 Mod_E

	  ToFolder  = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
      ToFolder1 = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
      FormFolder  = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
      FormFolder1 = IncludeTrailingBackslash(G_DAT_ORDER_Dir);
   }
   else if( P_Sakuban_Mode == "M" )
   {
	  ToFolder  = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
      ToFolder1 = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
      FormFolder  = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);
      FormFolder1 = IncludeTrailingBackslash(G_DAT_ESTIM_Dir);

   }
   sFirstChar = G_SAKUBAN.SubString(1, 1);
   sNumberList = "0123456789";
   i_PosIdx = sNumberList.Pos(sFirstChar);
   if (i_PosIdx > 0) {
      sRev.sprintf("R%02d", G_HEADS_DATA_Rev );
   } else {
      sRev.sprintf("R%02d", 0 );
   }
   Write_Log(sRev);

   // HEADS_DATA
   //マルチサーバ対応 2009/1/13 MOD START
   //FormFolder  = IncludeTrailingBackslash(G_HD_Root)
   //            + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //            + IncludeTrailingBackslash(FormFolder)
   //            + IncludeTrailingBackslash(G_SAKUBAN);

   //ToFolder  = IncludeTrailingBackslash(G_HD_Root)
   //          + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //          + IncludeTrailingBackslash(ToFolder)
   //          + IncludeTrailingBackslash(Work)
   //          + IncludeTrailingBackslash(G_SAKUBAN);
   FormFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
               + IncludeTrailingBackslash(FormFolder)
               + IncludeTrailingBackslash(G_SAKUBAN);

   //2023/4/21 製造番号桁数追加対応 Mod
   //ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //			 + IncludeTrailingBackslash(ToFolder)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
			 + IncludeTrailingBackslash(ToFolder)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //マルチサーバ対応 2009/1/13 MOD END

   // HEADS_OUTPUT
   //マルチサーバ対応 2008/11/17 MOD START
   //FormFolder1  = IncludeTrailingBackslash(G_HD_Root)
   //            + IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //            + IncludeTrailingBackslash(FormFolder1)
   //            + IncludeTrailingBackslash(G_SAKUBAN);

   //ToFolder1 = IncludeTrailingBackslash(G_HD_Root)
   //          + IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //          + IncludeTrailingBackslash(ToFolder1)
   //          + IncludeTrailingBackslash(Work)
   //          + IncludeTrailingBackslash(G_SAKUBAN);
   FormFolder1  = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
               + IncludeTrailingBackslash(FormFolder1)
			   + IncludeTrailingBackslash(G_SAKUBAN);
   //2023/4/21 製造番号桁数追加対応 Mod
   //ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //			 + IncludeTrailingBackslash(ToFolder1)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
			 + IncludeTrailingBackslash(ToFolder1)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //マルチサーバ対応 2008/11/17 MOD END

   // ログ
   G_Log_Text = "検索フォルダ（FormFolder）『" + FormFolder + "』";
   Write_Log(G_Log_Text);
   G_Log_Text = "検索フォルダ（FormFolder1）『" + FormFolder1 + "』";
   Write_Log(G_Log_Text);
   G_Log_Text = "検索フォルダ（ToFolder）『" + ToFolder + "』";
   Write_Log(G_Log_Text);
   G_Log_Text = "検索フォルダ（ToFolder1）『" + ToFolder1 + "』";
   Write_Log(G_Log_Text);

   //Windows 2008Server 移植対応
   //パスの変更
   G_Err_Log_FileFName = ToFolder1 + G_Err_Log_FileName;

   // コピー
   MoveDirectoryEvo(FormFolder, ToFolder);
   G_Log_Text = "フォルダのコピーしました from  『 " + FormFolder + " 』  to  『 " + ToFolder + " 』";
   Write_Log(G_Log_Text);

//   MoveDirectoryEvo(FormFolder1, ToFolder1);
   MoveDirectoryEvoRev(FormFolder1, ToFolder1, sRev);
   G_Log_Text = "フォルダのコピーしました from  『 " + FormFolder1 + " 』  to  『 " + ToFolder1 + " 』";
   Write_Log(G_Log_Text);

   // フォルダの削除
   if(!removeDirectoryEvo(FormFolder.c_str())){
      G_Log_Text = "フォルダの移動に失敗しました。『 " + FormFolder + " 』";
      Write_Log(G_Log_Text);
   }

   //2004.11.16 出力フォルダ及び訂正印変更
   if(!removeDirectoryEvo(FormFolder1.c_str())){
      G_Log_Text = "フォルダの移動に失敗しました。『 " + FormFolder1 + " 』";
      Write_Log(G_Log_Text);
   }

   // フォルダの移動
   G_Log_Text = "----------------フォルダ移動終了----------------";
   Write_Log(G_Log_Text);
}



//-----------------------------------------------------------------------------
// 日本語関数名： PDFﾌｧｲﾙ移動処理
// 概要        ： ﾌｧｲﾙの検索、ｺﾋﾟｰ
// パラメタ    ：
// 戻り値      ：
// 機能説明  ：
// 備考        ： 2005/09/28 追加
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::OutPut_File_Move1(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg)
{
   HANDLE han;
   WIN32_FIND_DATA wfd;
   bool bRet;
   AnsiString SearchFile;

   SearchFile = FromFile + Files;


   G_Log_Text = "検索ファイル『" + SearchFile + "』";
   Write_Log(G_Log_Text);

   if((han=FindFirstFile((SearchFile).c_str(), &wfd))!=INVALID_HANDLE_VALUE)
   {
      do
      {
         //2004.11.16 出力フォルダ及び訂正印変更
         //if ((((AnsiString)wfd.cFileName).c_str() == ".") || (((AnsiString)wfd.cFileName).c_str() == "..")) continue;
         //bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
         //if(bRet == false){
         //   G_Log_Text = "ファイルのコピーに失敗しました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
         //}else{
         //   G_Log_Text = "ファイルのコピーにしました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
         //}
         //Write_Log(G_Log_Text);
         //
         //// ファイルの削除
         //if(DelFlg){
         //   bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
         //   if(bRet == false){
         //      G_Log_Text = "ファイルの削除に失敗しました。『 " + FromFile + (AnsiString)wfd.cFileName + " 』";
         //      Write_Log(G_Log_Text);
         //   }
         //}
         if(strcmp(wfd.cFileName,".")!=0 && strcmp(wfd.cFileName,"..")!=0){
            bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
            if(bRet == false){
               G_Log_Text = "ファイルのコピーに失敗しました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
            }else{
               G_Log_Text = "ファイルのコピーにしました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + (AnsiString)wfd.cFileName + " 』";
            }
            Write_Log(G_Log_Text);

            // ファイルの削除
//          if(DelFlg){
//             bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
//             if(bRet == false){
//                G_Log_Text = "ファイルの削除に失敗しました。『 " + FromFile + (AnsiString)wfd.cFileName + " 』";
//                Write_Log(G_Log_Text);
//             }
//          }
         }
         //*************************************
      }
      while(FindNextFile(han,&wfd));
      FindClose(han);
   }
}


//-----------------------------------------------------------------------------
// 日本語関数名： WindowsAPIのRemoveDirectory改造版
// 概要        ： ﾌｫﾙﾀﾞ内全てのﾌｧｲﾙの移動
// パラメタ    ：
// 戻り値      ：
// 機能説明  ：
// 備考        ： 2005/09/28 追加
//-----------------------------------------------------------------------------
bool __fastcall TfrmHDP010::MoveDirectoryEvo1(AnsiString FromFile, AnsiString ToFile)
{
  bool retVal =true;
  AnsiString nextFileName;

  WIN32_FIND_DATA foundFile;

  G_Log_Text = "フォルダ Start FromFile 『 " + FromFile + " 』";
  Write_Log(G_Log_Text);
  G_Log_Text = "フォルダ Start ToFile 『 " + ToFile + " 』";
  Write_Log(G_Log_Text);

  HANDLE hFile = FindFirstFile((FromFile + "*.*").c_str(),&foundFile);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      Write_Log(foundFile.cFileName);

      //If a found file is . or .. then skip
      if(strcmp(foundFile.cFileName,".")!=0 && strcmp(foundFile.cFileName,"..")!=0)
      {
        //If the file is a directory
        if((foundFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
        {
          //2004.11.16 出力フォルダ及び訂正印変更
          //OutPut_Folder_Check(ToFile);
          //MoveDirectoryEvo(FromFile + foundFile.cFileName + "\\", ToFile + foundFile.cFileName + "\\");
          if (bRetRev == true ){
             OutPut_Folder_Check(ToFile);
             MoveDirectoryEvo1(FromFile + foundFile.cFileName + "\\", ToFile + foundFile.cFileName + "\\");
          }
          //*************************************
        }
        //If the file is a file
        else
        {
          OutPut_Folder_Check(ToFile);
          OutPut_File_Move1(FromFile , ToFile, "*.*", true);
        }
      }
    }
    while(FindNextFile(hFile,&foundFile)!=0);
  }

  FindClose(hFile);

  //2004.11.16 出力フォルダ及び訂正印変更
  //G_Log_Text = "フォルダ RemoveDirectory FromFile 『 " + FromFile + " 』";
  //Write_Log(G_Log_Text);
  //
  ////Delete starting point itseft
  //if(RemoveDirectory(FromFile.c_str())==0)retVal=false;
  //*************************************

  return retVal;
}


//---------------------------------------------------------------------------
// 日本語関数名：  圧力容器強度計算書作成ﾌﾟﾛｸﾞﾗﾑ  起動
// 概  要      ： 『Aturyoku.EXE』を起動する
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2009.03.03追加
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Aturyoku_Run(void)
{
    int i_KikakuCode;             // 規格ｺｰﾄﾞ
    AnsiString s_Text;            // 汎用文字列

    i_KikakuCode = G_HEADS_DATA[29].ToIntDef(0);

    if( G_Output_Mode == 1 )
    {
        G_Log_Text = "★★★ 【G_Output_Mode == 1】Excel出力テスト起動 ★★★";
        Write_Log(G_Log_Text);

        s_Text = G_SAKUBAN;
        spawnl(P_WAIT, "Aturyoku.EXE", "Aturyoku.EXE", s_Text, NULL);

    }
    else
    {
        G_Log_Text = "★★★ 【G_Output_Mode == 1】本番稼動 ★★★";
        Write_Log(G_Log_Text);

        if (P_UtiKbn == "3" || P_UtiKbn == "6"){
            //********************************
            //***  打出し区分　『1』,『6』
            //********************************

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

            s_Text = G_SAKUBAN;
            spawnl(P_WAIT, "Aturyoku.EXE", "Aturyoku.EXE", s_Text, NULL);

        } else if (P_UtiKbn == "4" && P_Sakuban_Mode == "S") {
            //**************************
            //***  打出し区分『4』
            //**************************

            // 圧力容器判定
            // 2018/11/19 NK,JG追加_S
            ////// 2018.05.02 JBA追加_S
            //////2014.04.18 強度計算条件変更
            //////if (i_KikakuCode == 1 ||i_KikakuCode == 2 ||i_KikakuCode == 3 ||i_KikakuCode == 27){
            ////if (i_KikakuCode == 1 ||i_KikakuCode == 2 ||i_KikakuCode == 3 ||
            ////    i_KikakuCode == 16 ||i_KikakuCode == 27){
            //////***************************
            //if (i_KikakuCode == 1 ||i_KikakuCode == 2 ||i_KikakuCode == 3 ||
            //    i_KikakuCode == 16 ||i_KikakuCode == 25||i_KikakuCode == 27){
            //// 2018.05.02 JBA追加_E
            if (i_KikakuCode == 1 || i_KikakuCode == 2 || i_KikakuCode == 3 ||
                i_KikakuCode == 11 || i_KikakuCode == 15 ||i_KikakuCode == 16 ||
                i_KikakuCode == 25||i_KikakuCode == 27){
            // 2018/11/19 NK,JG追加_E
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

                s_Text = G_SAKUBAN;
                spawnl(P_WAIT, "Aturyoku.EXE", "Aturyoku.EXE", s_Text, NULL);

            }

        }

    }

    return true;
}


//-----------------------------------------------------------------------------
// 日本語関数名： リビジョン番号の追加でフォルダのコピー
// 概要        ： ﾌｫﾙﾀﾞ内全てのﾌｧｲﾙの移動
// パラメタ    ：
// 戻り値      ：
// 機能説明  ：
// 備考        ： 2015/10/16 追加
//-----------------------------------------------------------------------------
bool __fastcall TfrmHDP010::MoveDirectoryEvoRev(AnsiString FromFile, AnsiString ToFile, AnsiString sRev)
{
  bool retVal =true;
  AnsiString nextFileName;

  WIN32_FIND_DATA foundFile;

  G_Log_Text = "フォルダ Start FromFile 『 " + FromFile + " 』";
  Write_Log(G_Log_Text);
  G_Log_Text = "フォルダ Start ToFile 『 " + ToFile + " 』";
  Write_Log(G_Log_Text);

  HANDLE hFile = FindFirstFile((FromFile + "*.*").c_str(),&foundFile);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      Write_Log(foundFile.cFileName);

      //If a found file is . or .. then skip
      if(strcmp(foundFile.cFileName,".")!=0 && strcmp(foundFile.cFileName,"..")!=0)
      {
        //If the file is a directory
        if((foundFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
        {
          if (bRetRev == true ){
             OutPut_Folder_Check(ToFile);
             MoveDirectoryEvoRev(FromFile + foundFile.cFileName + "\\", ToFile + foundFile.cFileName + "\\", sRev);
          }
        }
        //If the file is a file
        else
        {
          OutPut_Folder_Check(ToFile);
          OutPut_File_MoveRev(FromFile , ToFile, "*.*", true, sRev);
        }
      }
    }
    while(FindNextFile(hFile,&foundFile)!=0);
  }

  FindClose(hFile);

  return retVal;
}

//-----------------------------------------------------------------------------
// 日本語関数名： リビジョン番号の追加でﾌｧｲﾙ移動処理
// 概要        ： ﾌｧｲﾙの検索、ｺﾋﾟｰ
// パラメタ    ：
// 戻り値      ：
// 機能説明  ：
// 備考        ： 2015/10/16 追加
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::OutPut_File_MoveRev(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg, AnsiString sRev)
{
   HANDLE han;
   WIN32_FIND_DATA wfd;
   bool bRet;
   AnsiString SearchFile;
   AnsiString DestFileName;
   AnsiString DestFileTmp;
//Add_Str 2019/02/19 Ri
   AnsiString OLDTmp;
//Add_End 2019/02/19 Ri
   AnsiString firstTmp;
   AnsiString secondTmp;
   AnsiString DestFilePath;
   AnsiString sTmpRev;
   int i_Pos1;
   int i_Pos2;
   int i_Pos3;
   int i_Pos4;
   int i_Len;
   int i;

   SearchFile = FromFile + Files;


   G_Log_Text = "検索ファイル『" + SearchFile + "』";
   Write_Log(G_Log_Text);

   if((han=FindFirstFile((SearchFile).c_str(), &wfd))!=INVALID_HANDLE_VALUE)
   {
      do
      {
         if(strcmp(wfd.cFileName,".")!=0 && strcmp(wfd.cFileName,"..")!=0){

            DestFileName = (AnsiString)wfd.cFileName;
            DestFileTmp = DestFileName;
            DestFileTmp = DestFileTmp.UpperCase();
            i_Pos1 = DestFileTmp.Pos(".DWG");
            i_Pos2 = DestFileTmp.Pos(".PDF");
            i_Pos3 = DestFileTmp.Pos(".XLS");
            i_Pos4 = DestFileTmp.Pos(".PLT");
            i_Len = DestFileName.Length();
                if ( (i_Pos1 != 0) || (i_Pos2 != 0) || (i_Pos3 != 0) || (i_Pos4 != 0) ){
//Add_Str 2019/02/19 RI
                    OLDTmp = DestFileName.SubString(i_Len-6, 3);
                    if (OLDTmp == "OLD") {
                        firstTmp =  DestFileName.SubString(1, i_Len - 7);
                        secondTmp = "OLD" + DestFileName.SubString(i_Len-3, 4);
                    }else{
//Add_End 2019/02/19 RI
                        firstTmp = DestFileName.SubString(1, i_Len - 4);
                        secondTmp = DestFileName.SubString(i_Len-3, 4);
                    }
                DestFileName = firstTmp + sRev + secondTmp;
                for(i=0;i<10;i++){
                    sTmpRev.sprintf("R%02d", i );
                    if (sTmpRev.AnsiCompare(sRev) != 0) {
                        DestFilePath = ToFile + firstTmp + sTmpRev + secondTmp;
                        if (FileExists(DestFilePath)) {
                            DeleteFile(DestFilePath.c_str());
                        }
                    }
                }
            }

            DestFilePath = ToFile + (AnsiString)wfd.cFileName;
            if (FileExists(DestFilePath)) {
                DeleteFile(DestFilePath.c_str());
            }

            bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + DestFileName).c_str() , false);
            if(bRet == false){
               G_Log_Text = "ファイルのコピーに失敗しました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + DestFileName + " 』";
            }else{
               G_Log_Text = "ファイルのコピーにしました。from  『 " + FromFile + (AnsiString)wfd.cFileName + " 』  to  『 " + ToFile + DestFileName + " 』";
            }
            Write_Log(G_Log_Text);

            // ファイルの削除
            if(DelFlg){
               bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
               if(bRet == false){
                  G_Log_Text = "ファイルの削除に失敗しました。『 " + FromFile + (AnsiString)wfd.cFileName + " 』";
                  Write_Log(G_Log_Text);
               }
            }
         }
         //*************************************
      }
      while(FindNextFile(han,&wfd));
      FindClose(han);
   }
}


//---------------------------------------------------------------------------
// 日本語関数名：  塗装自主検査票作成ﾌﾟﾛｸﾞﾗﾑ  起動
// 概  要      ： 『TosoJisyuKensa.EXE』を起動する
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2017.10.19追加
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::TosoJisyuKensa_Run(void)
{
    int i_TosoJisyuKensa;         // 起動フラグ
    int i_RecNo;
    AnsiString s_Text1;           // 製造番号
    AnsiString s_Text2;           // 型式
    AnsiString s_Text3;           // 台数
    AnsiString s_Text4;           // REV


    if( G_Output_Mode == 1 ) {
        i_TosoJisyuKensa = 1;
    } else if (P_Sakuban_Mode == "S" && G_HEADS_DATA[302].TrimRight() == "2") {
        i_TosoJisyuKensa = 2;
    } else {
        i_TosoJisyuKensa = 0;
    }


    if (i_TosoJisyuKensa != 0) {
        //*********************
        //***  製造番号
        //*********************
        s_Text1 = G_SAKUBAN;

        //*********************
        //***  型    式
        //*********************
        i_RecNo = 33;
        if (G_HEADS_DATA[1057].TrimRight() == "1"){
            s_Text2 = G_HEADS_DATA[i_RecNo].TrimRight() + "（改）";
        } else {
            s_Text2 = G_HEADS_DATA[i_RecNo].TrimRight();
        }

        //*******************
        //***  台    数
        //*******************
        i_RecNo = 39;
        s_Text3 = G_HEADS_DATA[i_RecNo].TrimRight();

        //*******************
        //***  REV
        //*******************
        i_RecNo = 319;
        s_Text4 = G_HEADS_DATA[i_RecNo].TrimRight();
    }


    if( i_TosoJisyuKensa == 1 )
    {
        G_Log_Text = "★★★ 【G_Output_Mode == 1】Excel出力テスト起動 ★★★";
        Write_Log(G_Log_Text);

        spawnl(P_WAIT, "TosoJisyuKensa.EXE", "TosoJisyuKensa.EXE", s_Text1, s_Text2, s_Text3, s_Text4, NULL);

    }
    else if( i_TosoJisyuKensa == 2 )
    {
        G_Log_Text = "★★★ 【G_Output_Mode == 0】本番稼動 ★★★";
        Write_Log(G_Log_Text);

        // ﾛｸﾞ作成
        G_Log_Text = "----------------------------------------------------------";
        Write_Log(G_Log_Text);
        Write_Error_Log(G_Log_Text);
        G_Log_Text = "----------      『塗装自主検査票』作成開始      ----------";
        Write_Log(G_Log_Text);
        Write_Error_Log(G_Log_Text);
        G_Log_Text = "----------------------------------------------------------";
        Write_Log(G_Log_Text);
        Write_Error_Log(G_Log_Text);

        spawnl(P_WAIT, "TosoJisyuKensa.EXE", "TosoJisyuKensa.EXE", s_Text1, s_Text2, s_Text3, s_Text4, NULL);

    }

    return true;
}


//---------------------------------------------------------------------------
// 日本語関数名：  温風乾燥出力ﾌﾟﾛｸﾞﾗﾑ  起動
// 概  要      ： 『HotAirDrying_Run.EXE』を起動する
// 引  数      ： なし
// 戻り値      ： 読込結果 [true:正常 false:異常]
// 備  考      ： 2020.04.20追加
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::HotAirDrying_Run(void)
{
    int i_HotAirDrying;           // 起動フラグ
    int i_RecNo;
    AnsiString s_Toki1;           // 特記コード1
    AnsiString s_Toki2;           // 特記コード2
    AnsiString s_Toki3;           // 特記コード3
    AnsiString s_Toki4;           // 特記コード4
    AnsiString s_Toki5;           // 特記コード5
    AnsiString s_Toki6;           // 特記コード6
    AnsiString s_Toki7;           // 特記コード7
    AnsiString s_Toki8;           // 特記コード8
    AnsiString s_Toki9;           // 特記コード9
    AnsiString s_Toki10;          // 特記コード10
    AnsiString s_Toki11;          // 特記コード11
    AnsiString s_Toki12;          // 特記コード12
    AnsiString s_Toki13;          // 特記コード13
    AnsiString s_Toki14;          // 特記コード14
    AnsiString s_BMat;            // B側端管材質
    AnsiString s_AMat;            // A側端管材質
    AnsiString s_BAtu;            // B側水圧
    AnsiString s_AAtu;            // A側水圧
    AnsiString s_Sakuban;         // 製造番号
    AnsiString s_Kubun;           // 区分
    bool b_BOnpu;                 // B側温風乾燥
    bool b_AOnpu;                 // A側温風乾燥


    //***********************
    //***  特記コード1
    //***********************
    i_RecNo = 451;
    s_Toki1 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード2
    //***********************
    i_RecNo = 454;
    s_Toki2 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード3
    //***********************
    i_RecNo = 457;
    s_Toki3 = G_HEADS_DATA[i_RecNo].TrimRight();

     //***********************
    //***  特記コード4
    //***********************
    i_RecNo = 460;
    s_Toki4 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード5
    //***********************
    i_RecNo = 463;
    s_Toki5 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード6
    //***********************
    i_RecNo = 466;
    s_Toki6 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード7
    //***********************
    i_RecNo = 469;
    s_Toki7 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード8
    //***********************
    i_RecNo = 472;
    s_Toki8 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード9
    //***********************
    i_RecNo = 475;
    s_Toki9 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード10
    //***********************
    i_RecNo = 478;
    s_Toki10 = G_HEADS_DATA[i_RecNo].TrimRight();

      //***********************
    //***  特記コード11
    //***********************
    i_RecNo = 481;
    s_Toki11 = G_HEADS_DATA[i_RecNo].TrimRight();

     //***********************
    //***  特記コード12
    //***********************
    i_RecNo = 484;
    s_Toki12 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード13
    //***********************
    i_RecNo = 487;
    s_Toki13 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  特記コード14
    //***********************
    i_RecNo = 490;
    s_Toki14 = G_HEADS_DATA[i_RecNo].TrimRight();


    if (s_Toki1 == "1M" || s_Toki1 == "1N" || s_Toki1 == "1P" ) {
        s_Kubun = s_Toki1;
        i_HotAirDrying = 2;

    } else if (s_Toki2 == "1M" || s_Toki2 == "1N" || s_Toki2 == "1P" ) {
        s_Kubun = s_Toki2;
        i_HotAirDrying = 2;

    } else if (s_Toki3 == "1M" || s_Toki3 == "1N" || s_Toki3 == "1P" ) {
        s_Kubun = s_Toki3;
        i_HotAirDrying = 2;

    } else if (s_Toki4 == "1M" || s_Toki4 == "1N" || s_Toki4 == "1P" ) {
        s_Kubun = s_Toki4;
        i_HotAirDrying = 2;

    } else if (s_Toki5 == "1M" || s_Toki5 == "1N" || s_Toki5 == "1P" ) {
        s_Kubun = s_Toki5;
        i_HotAirDrying = 2;

    } else if (s_Toki6 == "1M" || s_Toki6 == "1N" || s_Toki6 == "1P" ) {
        s_Kubun = s_Toki6;
        i_HotAirDrying = 2;

    } else if (s_Toki7 == "1M" || s_Toki7 == "1N" || s_Toki7 == "1P" ) {
        s_Kubun = s_Toki7;
        i_HotAirDrying = 2;

    } else if (s_Toki8 == "1M" || s_Toki8 == "1N" || s_Toki8 == "1P" ) {
        s_Kubun = s_Toki8;
        i_HotAirDrying = 2;

    } else if (s_Toki9 == "1M" || s_Toki9 == "1N" || s_Toki9 == "1P" ) {
        s_Kubun = s_Toki9;
        i_HotAirDrying = 2;

    } else if (s_Toki10 == "1M" || s_Toki10 == "1N" || s_Toki10 == "1P" ) {
        s_Kubun = s_Toki10;
        i_HotAirDrying = 2;

    } else if (s_Toki11 == "1M" || s_Toki11 == "1N" || s_Toki11 == "1P" ) {
        s_Kubun = s_Toki11;
        i_HotAirDrying = 2;

    } else if (s_Toki12 == "1M" || s_Toki12 == "1N" || s_Toki12 == "1P" ) {
        s_Kubun = s_Toki12;
        i_HotAirDrying = 2;

    } else if (s_Toki13 == "1M" || s_Toki13 == "1N" || s_Toki13 == "1P" ) {
        s_Kubun = s_Toki13;
        i_HotAirDrying = 2;

    } else if (s_Toki14 == "1M" || s_Toki14 == "1N" || s_Toki14 == "1P" ) {
        s_Kubun = s_Toki14;
        i_HotAirDrying = 2;

    } else {
      s_Kubun = "";

    }

    if (s_Kubun == "") {
        i_RecNo = 1060;
        s_BMat = G_HEADS_DATA[i_RecNo].SubString(1,4);
        s_BMat = Type_MAT(s_BMat);            // B側端管材質
        i_RecNo = 1061;
        s_AMat = G_HEADS_DATA[i_RecNo].SubString(1,4);
        s_AMat = Type_MAT(s_AMat);            // A側端管材質
        i_RecNo = 827;
        s_BAtu = G_HEADS_DATA[i_RecNo].Trim(); // B側水圧
        i_RecNo = 832;
        s_AAtu = G_HEADS_DATA[i_RecNo].Trim(); // A側水圧

        //B側温風乾燥
        if ( s_BMat == "STPG" || s_BMat == "SGP" || s_BMat == "CS" ) {
            // CSﾘﾝｸﾞ端管
            if (StrToFloat(s_BAtu) > 0 ) {
                // 水圧試験
                b_BOnpu = true;  // B側温風乾燥

            } else {
                // 空圧試験
                b_BOnpu = false; // B側温風乾燥不要

            }
        } else {
            b_BOnpu = false;     // B側温風乾燥不要

        }

        //A側温風乾燥
        if ( s_AMat == "STPG" || s_AMat == "SGP" || s_AMat == "CS" ) {
            // CSﾘﾝｸﾞ端管
            if (StrToFloat(s_AAtu) > 0 ) {
                // 水圧試験
                b_AOnpu = true;  // A側温風乾燥

            } else {
                // 空圧試験
                b_AOnpu = false; // A側温風乾燥不要

            }
        } else {
            b_AOnpu = false;     // A側温風乾燥不要

        }

        if ( b_BOnpu == true && b_AOnpu == false ) {
            // B側温風乾燥
            i_RecNo = 137;
            if ( G_HEADS_DATA[i_RecNo].TrimRight().ToIntDef(0) == 1 ) { // ＡＢ逆
                // 2020.06.24 出力特記コード変更_S
                //// 低温側温風乾燥
                //s_Kubun = "1N";
                // 高温側温風乾燥
                s_Kubun = "1M";
                // 2020.06.24 出力特記コード変更_E
                i_HotAirDrying = 2;

            } else {
                // 高温側温風乾燥
                s_Kubun = "1M";
                i_HotAirDrying = 2;

            }
        } else if ( b_BOnpu == false && b_AOnpu == true ) {
            // A側温風乾燥
            if ( G_HEADS_DATA[i_RecNo].TrimRight().ToIntDef(0) == 1 ) { // ＡＢ逆
                // 高温側温風乾燥
                s_Kubun = "1M";
                i_HotAirDrying = 2;

            } else {
                // 2020.06.24 出力特記コード変更_S
                //// 低温側温風乾燥
                //s_Kubun = "1N";
                // 高温側温風乾燥
                s_Kubun = "1M";
                // 2020.06.24 出力特記コード変更_E
                i_HotAirDrying = 2;

            }
        } else if ( b_BOnpu == true && b_AOnpu == true ) {
            // 2020.06.24 出力特記コード変更_S
            //// 両側温風乾燥
            //s_Kubun = "1P";
            // 高温側温風乾燥
            s_Kubun = "1M";
            // 2020.06.24 出力特記コード変更_E
            i_HotAirDrying = 2;

        } else {
            s_Kubun = "";
            i_HotAirDrying = 2;

        }

    }


    if( G_Output_Mode == 1 ) {
        i_HotAirDrying = 1;

    }


    if (i_HotAirDrying != 0) {
        //*********************
        //***  製造番号
        //*********************
        s_Sakuban = G_SAKUBAN;

    }

     if( i_HotAirDrying == 1 )
    {
        G_Log_Text = "★★★ 【G_Output_Mode == 1】Excel出力テスト起動 ★★★";
        Write_Log(G_Log_Text);
        G_Log_Text = "★★★ 温風乾燥プログラム起動 ★★★";
        Write_Log(G_Log_Text);

        spawnl(P_WAIT, "./HotAirDrying/HotAirDrying.EXE", "./HotAirDrying/HotAirDrying.EXE", s_Sakuban, s_Kubun, NULL);

    }
    else if( i_HotAirDrying == 2 )
    {
        G_Log_Text = "★★★ 【G_Output_Mode == 0】本番稼動 ★★★";
        Write_Log(G_Log_Text);
        G_Log_Text = "★★★ 温風乾燥プログラム起動 ★★★";
        Write_Log(G_Log_Text);

        spawnl(P_WAIT, "./HotAirDrying/HotAirDrying.EXE", "./HotAirDrying/HotAirDrying.EXE", s_Sakuban, s_Kubun, NULL);

    }

    return true;
}

// ---------------- <<< End of File >>> ---------------- //

