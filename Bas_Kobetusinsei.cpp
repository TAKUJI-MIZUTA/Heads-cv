//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���^���͗e�� �ʌ���\����  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.06.03
//�@ �X �V ���F2002.09.20
//             2007.10.10 �װ�d�l�Ή�
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include <utilcls.h>    // Excel�N���pͯ�
#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_Kobetusinsei.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ���^���͗e�� �ʌ���\�����@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Kobetusinsei(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_Text;            // �ėp������
   int        i_RecNo;           // HEADS�ް� ں���No        

   //------------------------------------
   // 2007.10.10 �װ�d�l�Ή�
   // ���d�l�ȊO�͏����s�v(����I��)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   s_Tmplate_Pass = G_Template_Dir + "���^���͗e��ʌ���\����.xls";     // ����ڰ�̧��
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "PVD" + G_SAKUBAN + ".xls";       // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "PVD" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";       // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���^���͗e��  �ʌ���\�����x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------------------------";
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
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //***********************************************************************************************
   //***                                                                                         ***
   //***                          ���^���͗e��@�ʌ���\�����@�f�[�^�Z�b�g                     ***
   //***                                                                                         ***
   //***********************************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�ʌ���\����");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�ʌ���\����"));

   //*******************
   //***  �����ԍ�
   //*******************
   // �wA001�x
   //2004.12.21 ���L�ύX
   //s_Text = "No." + G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   //Excel_Cell_DataSet(4, 1, s_Text);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   
   Excel_Cell_DataSet(3, 1, s_Text);
   //*******************
   G_Log_Text = "�����ԍ�            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************
   //***  ��  ��
   //*******************
   // �wA002�x
   i_RecNo = 39;
   s_Text = G_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(6, 12, s_Text);
      G_Log_Text = "��  ��               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�䐔      RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �g�p���́@����M��
   //***************************
   // �wA003�x
   i_RecNo = 1098;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if ( G_HEADS_DATA[i_RecNo].ToDouble() >= 999.0 ) {
       s_Text = "��C�J��";
     } else {
       s_Text = FormatFloat("0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(7, 5, s_Text);
   G_Log_Text = "�g�p���� ����M��    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �g�p���́@�M����
   //***************************
   // �wA004�x
   i_RecNo = 1100;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if ( G_HEADS_DATA[i_RecNo].ToDouble() >= 999.0 ) {
       s_Text = "��C�J��";
     } else {
       s_Text = FormatFloat("0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   }else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(8, 5, s_Text);
   G_Log_Text = "�g�p���� �M����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  ���e�ρ@����M��,���M��
   //*********************************
   // �wA005�x,�wA006�x
   double d_HOLD_Value;

   if (StrToDblDef(G_HEADS_DATA[852].TrimRight(),0) >= StrToDblDef(G_HEADS_DATA[853].TrimRight(),0)) {
      //******************************************
      //***  (B�������x) >= (A�������x)�̂Ƃ�
      //******************************************

      // ���e�� ����M��
      i_RecNo = 1283;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(7, 11, s_Text);
         G_Log_Text = "���e�� ����M��      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "���e�� ����M��  RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }

      // ���e�� �M����
      i_RecNo = 1282;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(8, 11, s_Text);
         G_Log_Text = "���e�� �M����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "���e�� �M����    RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }

   } else {
      //******************************************
      //***  (B�������x) < (A�������x)�̂Ƃ�
      //******************************************

      // ���e�� ����M��
      i_RecNo = 1282;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(7, 11, s_Text);
         G_Log_Text = "���e�� ����M��      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "���e�� ����M��  RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }

      // ���e�� �M����
      i_RecNo = 1283;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble() / 1000;
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.0000",d_HOLD_Value);
         Excel_Cell_DataSet(8, 11, s_Text);
         G_Log_Text = "���e�� �M����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "���e�� �M����    RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }
   }

   //***************************
   //***  �ڰт̌���
   //***************************
   // �wA07�x
//   i_RecNo = 1284;
   i_RecNo = 1106;      // ���x�v�Z���̃t���[�����Ƃ��킹��
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2004.12.21 �����ύX
   double d_Text_Value;
   d_Text_Value = s_Text.ToDouble();
   d_Text_Value = Hasu_Round(0, d_Text_Value , 0);
   s_Text = FormatFloat("#,###,##0",d_Text_Value);
   //*******************
   Excel_Cell_DataSet(9, 2, s_Text);
   G_Log_Text = "�ڰт̌���           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ��ڰĂ̑傫��
   //***************************
   // �wA008�x
   i_RecNo = 1268;
   s_Text  = G_HEADS_DATA[i_RecNo].TrimRight() + "�~";
   i_RecNo = 1269;
   s_Text  = s_Text + G_HEADS_DATA[i_RecNo].TrimRight() + "�~";
   i_RecNo = 1270;
   s_Text  = s_Text + G_HEADS_DATA[i_RecNo].TrimRight();

   Excel_Cell_DataSet(9, 10, s_Text);
   G_Log_Text = "��ڰĂ̑傫��        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //******************************
   //***  Excel Book Close
   //******************************
   // �}�N�����s
   //PDF�t�@�C���o�͎��
   //exApplication.Exec(Function("Run") << "Gsub_PDFOut");

   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   //Windows 2008Server �ڐA�Ή�
   exApplication.OlePropertySet("DisplayAlerts", false);

   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���^���͗e��  �ʌ���\�����x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}