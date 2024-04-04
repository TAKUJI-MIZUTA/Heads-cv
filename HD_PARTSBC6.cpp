// ---------------------------------------------------------------------------
// 日阪製作所様向け　出図要求受信サービスシステム
// ﾌｧｲﾙ概要：出図要求受信サービスシステム プロジェクトファイル
// 作 成 者：( 応用技術株式会社 )
// ﾊﾞｰｼﾞｮﾝ ：1.00 2002/05/25 初期リリース
// ---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#pragma hdrstop

#define  VDEF
#include "HDPConst.h"   // 定数定義用
#include "HDPValue.h"   // 変数定義用
#include "HDPCom.h"     // 関数定義用


USEFORM("HDP20.cpp", frmHDP020);
USEFORM("DataModule.cpp", DataModule1); /* TDataModule: File Type */
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    // ***  ｴﾗｰﾒｯｾｰｼﾞ[OK=MB_OK] ***
    // Windows 2008Server 移植対応
#define  CEMSYSTEMERR             1
#define  CEMSYSTEMWBOOTERR        2
#define  CEMDIATYTLEERROR       L"エラー"
#define  CSEMAPHO_STR           "JTP_SUPPORT_SUB_SEM"

    // WinMain変数定義
    // static char    *s_Emess[] = {
    // Windows 2008Server 移植対応
    static wchar_t *s_Emess[] = {
        // static TCHAR    *s_Emess[] = {
        L"", // 0
        L"システム異常[%x]。", // 1
        L"システムは既に起動しています。", // 2
        L"" // E
    };
    HANDLE hsem;
    // char    buf[128];
    // Windows 2008Server 移植対応
    wchar_t buf[128];
    // TCHAR    buf[128];

    try {
//        // □[WINAPI:OpenSemaphore] 既存ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄｵｰﾌﾟﾝ
//        hsem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, CSEMAPHO_STR);
//
//        if (hsem == NULL) // 該当ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄなし?
//        {
//            // □[WINAPI:CreateSemaphore] 既存ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄ作成
//            hsem = CreateSemaphore(NULL, 1, 1, CSEMAPHO_STR);
//
//            if (hsem != NULL) // 正常終了?
//            {
                Application->Initialize();

                Application->Title = "部品展開処理";
                Application->CreateForm(__classid(TfrmHDP020), &frmHDP020);
        Application->Run();

//                // □[WINAPI:CloseHandle] ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄ解放
//                CloseHandle(hsem);
//            }
//            else {
//                // 異常ﾒｯｾｰｼﾞ
//                // sprintf( buf, s_Emess[CEMSYSTEMERR], GetLastError() );
//                // Windows 2008Server 移植対応
//                swprintf(buf, s_Emess[CEMSYSTEMERR], GetLastError());
//                Application->MessageBox(buf, CEMDIATYTLEERROR,
//                    MB_ICONEXCLAMATION | MB_OK);
//            }
//        }
//        else {
//            // 異常ﾒｯｾｰｼﾞ
//            Application->MessageBox(s_Emess[CEMSYSTEMWBOOTERR],
//                CEMDIATYTLEERROR, MB_ICONEXCLAMATION | MB_OK);
//
//            // □[WINAPI:CloseHandle] ｾﾏﾌｫｵﾌﾞｼﾞｪｸﾄ解放
//            CloseHandle(hsem);
//        }
    }
    catch (Exception &exception) {
        Application->ShowException(&exception);
    }
    return 0;
}
// ---------------------------------------------------------------------------
