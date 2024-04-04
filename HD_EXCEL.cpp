//---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�o�}�v����M�T�[�r�X�V�X�e�� �v���W�F�N�g�t�@�C��
//   �� �� �ҁF( ������� �A�C�E�V�[�E�G�X )
//   �ް�ޮ� �F1.00 2002/05/25 ���������[�X
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>

#pragma hdrstop

#define  VDEF

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p


USERES("HD_EXCEL.res");
USEFORM("HDP010.cpp", frmHDP010);
USEUNIT("HDPCom.cpp");
USE("HDPConst.h", File);
USE("HDPValue.h", File);
USEFORM("DataModule.cpp", DataModule1); /* TDataModule: File Type */
USEUNIT("Bas_Syonin.cpp");
USEUNIT("Bas_Funeyou.cpp");
USEUNIT("Bas_Mokuroku.cpp");
USEUNIT("Bas_Seisekisyo.cpp");
USEUNIT("Bas_Kobetusinsei.cpp");
USEUNIT("Bas_Meisaisyo.cpp");
USEUNIT("Bas_Kyodokeisan.cpp");
USEUNIT("Bas_Youryosyo.cpp");
USEUNIT("Bas_ATU21.cpp");
USEUNIT("Bas_ATU22.cpp");
USEUNIT("Bas_ATU31.cpp");
USEUNIT("Bas_ATU32.cpp");
USEUNIT("Bas_ATU61.cpp");
USEUNIT("Bas_ATU52.cpp");
USEUNIT("Bas_Steam.cpp");
USEUNIT("Bas_Sijisyo.cpp");
USEUNIT("Bas_Buhinhyou.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
// ***  �װү����[OK=MB_OK] ***
#define  CEMSYSTEMERR             1
#define  CEMSYSTEMWBOOTERR        2
#define  CEMDIATYTLEERROR       "�G���["
#define  CSEMAPHO_STR           "JTP_SUPPORT_SUB_SEM"

   // WinMain�ϐ���`
   static char    *s_Emess[] = {
      "",                                       //  0
      "�V�X�e���ُ�[%x]�B",                     //  1
      "�V�X�e���͊��ɋN�����Ă��܂��B",         //  2
      ""                                        //  E
   };
   HANDLE  hsem;
   char    buf[128];

   try
   {
      // ��[WINAPI:OpenSemaphore] ������̫��޼ު�ĵ����
      hsem = OpenSemaphore( SEMAPHORE_ALL_ACCESS, false, CSEMAPHO_STR );

      if( hsem == NULL )  // �Y����̫��޼ު�ĂȂ�?
      {
         // ��[WINAPI:CreateSemaphore] ������̫��޼ު�č쐬
         hsem = CreateSemaphore( NULL, 1, 1, CSEMAPHO_STR );

         if( hsem != NULL )  // ����I��?
         {
            Application->Initialize();

            Application->Title = "Excel�쐬";
            Application->CreateForm(__classid(TfrmHDP010), &frmHDP010);
                 Application->Run();

            // ��[WINAPI:CloseHandle] ��̫��޼ު�ĉ��
            CloseHandle( hsem );
         }
         else
         {
            // �ُ�ү����
            sprintf( buf, s_Emess[CEMSYSTEMERR], GetLastError() );
            Application->MessageBox( buf, CEMDIATYTLEERROR, MB_ICONEXCLAMATION|MB_OK );
         }
      }
      else
      {
         // �ُ�ү����
         Application->MessageBox( s_Emess[CEMSYSTEMWBOOTERR], CEMDIATYTLEERROR, MB_ICONEXCLAMATION|MB_OK );

         // ��[WINAPI:CloseHandle] ��̫��޼ު�ĉ��
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




