//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���͗e�틭�x�v�Z��  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.06.05
//�@ �X �V ���F2002.09.20
//             2007.10.10 �װ�d�l�Ή�
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

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
// ���{��֐����F ���͗e�틭�x�v�Z���@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Kyodokeisan(HeadsData* pcHeadsData)
{
   //int i_Pos;
   AnsiString P_UtiKbn;
   int i_PrintFlg;         // 0:PDF���o��,1:PDF�o��
   int i_KikakuCode;       // �K�i����
   int i_BolOut;           // 0:���ĊO��,1:���ē���
   int i_BolNum;           // ���ĕБ��{��

   AnsiString s_Syurui;    // ���
   AnsiString s_Model;     // ����
   AnsiString s_Frame;     // �ڰ�
   AnsiString s_SQL;

   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_Text;            // �ėp������

   //------------------------------------
   // 2007.10.10 �װ�d�l�Ή�
   // ���d�l�ȊO�͏����s�v(����I��)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   s_Tmplate_Pass = G_Template_Dir + "���͗e��\����.xls";              // ����ڰ�̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "PVC" + G_SAKUBAN + ".xls";    // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���͗e�틭�x�v�Z���x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "�쐬�t�@�C�����F    �w" + s_MakeExcel_Pass + "�x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �e���v���[�g�@�R�s�[
   //******************************
   if (!Template_Copy(s_Tmplate_Pass,s_MakeExcel_Pass)){
      // �װ۸ޒǉ�
      G_ErrLog_Text = "�t�@�C���R�s�[�����s���܂����B�w" + s_Tmplate_Pass +
                      "�x���w" + s_MakeExcel_Pass + "�x";
      Write_Error_Log(G_ErrLog_Text);

      return false;
   } else {
      G_Log_Text = "�t�@�C���R�s�[�����B�w" + s_Tmplate_Pass +
                      "�x���w" + s_MakeExcel_Pass + "�x";
      Write_Log(G_Log_Text);
   }

   //******************************
   //***  Excel Book Open
   //******************************
   // ̧�ٵ����
   try {
      exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ��ĺڸ��ݎ擾
   try {
      exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //*********************************
   //***  ���͗e�틭�x�v�Z��  �쐬
   //*********************************
   i_PrintFlg = 0;

// �S���[�`�F�b�N�p
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

//2003.09.04 �o������ڰĕύX
   if (i_KikakuCode == 1 || i_KikakuCode == 2 || i_KikakuCode == 3){
//      s_Syurui = G_HEADS_DATA[34].TrimRight();    // ���
//      s_Model  = G_HEADS_DATA[35].TrimRight();    // ����
//      s_Frame  = G_HEADS_DATA[36].TrimRight();    // �ڰ�
      i_BolOut = G_HEADS_DATA[1277].ToIntDef(0);  // 0:���ĊO��,1:���ē���
      i_BolNum = G_HEADS_DATA[1020].ToIntDef(0);  // ���ĕБ��{��

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

   // ���ё����ʌv�Z��
   // RecNo.846(B����)��RecNo.847(A����)�̂ǂ��炩���A�P�̂Ƃ��쐬
   if (G_HEADS_DATA[846].TrimRight() == "1" || G_HEADS_DATA[847].TrimRight() == "1"){
      Steam();

      i_PrintFlg = 1;
   }

   //******************************
   //***  Excel Book Close
   //******************************
   // �}�N�����s
   if (i_PrintFlg == 1){
      exApplication.Exec(Function("Run") << "Gsub_PDFOut");
   }

   exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ۸ލ쐬
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���͗e�틭�x�v�Z���x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

