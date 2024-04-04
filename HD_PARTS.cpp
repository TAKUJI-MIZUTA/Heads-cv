//---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
//   ﾌｧｲﾙ概要：出図要求受信サービスシステム プロジェクトファイル
//   作 成 者：( 株式会社 アイ・シー・エス )
//   ﾊﾞｰｼﾞｮﾝ ：1.00 2002/05/25 初期リリース
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma hdrstop

#define  VDEF
#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用

USERES("HD_PARTS.res");
USEFORM("HDP20.cpp", frmHDP020);
USEUNIT("HDPCom.cpp");
USE("HDPConst.h", File);
USE("HDPValue.h", File);
USEFORM("DataModule.cpp", DataModule1); /* TDataModule: File Type */
USEUNIT("Bas_PlateDetails.cpp");
USEUNIT("Bas_FrameDetails.cpp");
USEUNIT("Bas_NozzleDetails.cpp");
USEUNIT("Bas_TempDetails.cpp");
USEUNIT("Bas_PressGDetails.cpp");
USEUNIT("Bas_AxsrsDetails.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
// ***  ｴﾗｰﾒｯｾｰｼﾞ[OK=MB_OK] ***
#define  CEMSYSTEMERR             1
#define  CEMSYSTEMWBOOTERR        2
#define  CEMDIATYTLEERROR       "エラー"
#define  CSEMAPHO_STR           "JTP_SUPPORT_SUB_SEM"

   // WinMain変数定義
   static char    *s_Emess[] = {
      "",                                       //  0
      "システム異常[%x]。",                     //  1
      "システムは既に起動しています。",         //  2
      ""                                        //  E
   };
   HANDLE  hsem;
   char    buf[128];

   try
   {
      // □[WINAPI:OpenSemaphore] 既存ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄｵｰﾌﾟﾝ
      hsem = OpenSemaphore( SEMAPHORE_ALL_ACCESS, false, CSEMAPHO_STR );

      if( hsem == NULL )  // 該当ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄなし?
      {
         // □[WINAPI:CreateSemaphore] 既存ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄ作成
         hsem = CreateSemaphore( NULL, 1, 1, CSEMAPHO_STR );

         if( hsem != NULL )  // 正常終了?
         {
            Application->Initialize();

            Application->Title = "部品展開処理";
            Application->CreateForm(__classid(TfrmHDP020), &frmHDP020);
                 Application->Run();

            // □[WINAPI:CloseHandle] ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄ解放
            CloseHandle( hsem );
         }
         else
         {
            // 異常ﾒｯｾｰｼﾞ
            sprintf( buf, s_Emess[CEMSYSTEMERR], GetLastError() );
            Application->MessageBox( buf, CEMDIATYTLEERROR, MB_ICONEXCLAMATION|MB_OK );
         }
      }
      else
      {
         // 異常ﾒｯｾｰｼﾞ
         Application->MessageBox( s_Emess[CEMSYSTEMWBOOTERR], CEMDIATYTLEERROR, MB_ICONEXCLAMATION|MB_OK );

         // □[WINAPI:CloseHandle] ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄ解放
         CloseHandle( hsem );
      }
   }
   catch( Exception &exception )
   {
      Application->ShowException(&exception);
   }
   return 0;
}
//---------------------------------------------------------------------------


