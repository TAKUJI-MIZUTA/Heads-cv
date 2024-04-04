//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���p�\�����쐬���W���[��
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.03
//�@ �X �V ���F2007.10.10 �װ�d�l�Ή�
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "SINSEICom.h"  // �\�������ʊ֐���`�p

#include "Bas_ShomeiBV.h"
#include "Bas_ShomeiLR.h"
#include "Bas_ShomeiABS.h"
#include "Bas_ShomeiNK.h"
#include "Bas_ShinseiNK.h"
#include "Bas_ShinseiNonNK.h"
#include "Bas_ShinseiFaxNK.h"
#include "Bas_ShinseiFaxNonNK.h"

#include "Bas_Funeyousinsei.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ���{��֐����F ���p�\�����쐬���C��
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Funeyousinsei(HeadsData* pcHeadsData)
{
   AnsiString strStdCode;
   int nRecNo;
   // 2011.02.09 ������Ή�
   AnsiString s_Text;
   // *********************
   // 2016.08.02 �������я����p
   int i_PosM;                   // ���p�t���O ("0":���p�ȊO�A"1"�ȏ�:���p)
   // *************************

   //------------------------------------
   // 2007.10.10 �װ�d�l�Ή�
   // ���d�l�ȊO�͏����s�v(����I��)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   // �K�i�R�[�h�̎擾
   nRecNo = 29;
   strStdCode = G_HEADS_DATA[nRecNo].Trim();

   // 2016.08.02 �������я����p
   i_PosM = 0;
   nRecNo = 36;
   i_PosM = G_HEADS_DATA[nRecNo].Pos("M");
   // *************************

   //******************************
   //***�e���v���[�g/�쐬Excel�t�@�C���p�X�̐���
   //******************************
   // 2011.02.09 ������Ή�
   //s_Tmplate_Pass = G_Template_Dir + "�������я�.xls";                  // ����ڰ�̧��
   nRecNo = 851;
   s_Text = G_HEADS_DATA[nRecNo].TrimRight();
   if ( s_Text == "1" ) {
      // 2016.08.02 �������я����p
      //// 2016.06.08 �������я����p
      ////s_Tmplate_Pass = G_Template_Dir + "�������я�C.xls";              // ����ڰ�̧��
      //s_Tmplate_Pass = G_Template_Dir + "�������я����pC.xls";          // ����ڰ�̧��
      //// *************************
      if (i_PosM >= 1){
         s_Tmplate_Pass = G_Template_Dir + "�������я����pC.xls";    // ����ڰ�̧��
      } else {
         s_Tmplate_Pass = G_Template_Dir + "�������я�C.xls";        // ����ڰ�̧��
      }
      // *************************
   } else {
      // 2016.08.02 �������я����p
      //// 2016.06.08 �������я����p
      ////s_Tmplate_Pass = G_Template_Dir + "�������я�.xls";               // ����ڰ�̧��
      //s_Tmplate_Pass = G_Template_Dir + "�������я����p.xls";           // ����ڰ�̧��
      //// *************************
      if (i_PosM >= 1){
         s_Tmplate_Pass = G_Template_Dir + "�������я����p.xls";    // ����ڰ�̧��
      } else {
         s_Tmplate_Pass = G_Template_Dir + "�������я�.xls";        // ����ڰ�̧��
      }
      // *************************
   }
   // *********************
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + G_SAKUBAN + ".xls";    // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";    // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "----------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "-----------    �w���p�\�����x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);


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

   G_Log_Text = "�쐬�t�@�C�����F    �w" + s_MakeExcel_Pass + "�x";
   Write_Log(G_Log_Text);

   switch( strStdCode.ToIntDef(-1) )
   {
      case SEISEKISYO_STANDARDNO_DNV:
         FuneyousinseiDNV();           // ���p�\����DNV�쐬
         break;
      case SEISEKISYO_STANDARDNO_ABS:
         FuneyousinseiABS();        // ���p�\����ABS�쐬
         break;
      case SEISEKISYO_STANDARDNO_LR:
         FuneyousinseiLR();         // ���p�\����LR�쐬
         break;
      case SEISEKISYO_STANDARDNO_NK:
         FuneyousinseiNK();         // ���p�\����NK�쐬
         break;
      case SEISEKISYO_STANDARDNO_GL:
         FuneyousinseiGL();         // ���p�\����GL�쐬
         break;
      case SEISEKISYO_STANDARDNO_KR:
         FuneyousinseiKR();         // ���p�\����KR�쐬
         break;
      case SEISEKISYO_STANDARDNO_BV:
         FuneyousinseiBV();         // ���p�\����BV�쐬
         break;
      case SEISEKISYO_STANDARDNO_CCS:
         FuneyousinseiCCS();        // ���p�\����CCS�쐬
         break;
      default :
         FuneyousinseiABS();        // ���p�\����ABS�쐬
         break;
   }

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���p�\�����x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;

}


//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����DNV�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiDNV(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w DNV �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "��-FAX-DNV.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"��-FAX");

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "���\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"���\");

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
	  G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  �V�[�g�쐬 (��FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  �V�[�g�쐬 (���\)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server �ڐA�Ή�
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
	  exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����ABS�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiABS(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w ABS �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "��-FAX-ABS.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"��-FAX");
   G_Log_Text = "�V�[�g�R�s�[ (��FAX)";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "���\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"���\");
   G_Log_Text = "�V�[�g�R�s�[ (���\)";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (ABS��)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "ABS��.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"ABS��");
   G_Log_Text = "�V�[�g�R�s�[ (ABS��)";
   Write_Log(G_Log_Text);

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
      //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
      //Windows 2008Server �ڐA�Ή�
      exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      //Windows 2008Server �ڐA�Ή�
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");

      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }


   //******************************
   //***  �V�[�g�쐬 (��FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  �V�[�g�쐬 (���\)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  �V�[�g�쐬 (ABS��)
   //******************************
   ShomeiABS();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
      //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
      //Windows 2008Server �ڐA�Ή�
      exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����LR�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiLR(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w LR �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "��-FAX-LR.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"��-FAX");

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "���\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"���\");

   //******************************
   //***  �V�[�g�R�s�[ (LR��)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "LR��.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"LR��");

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
      //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
      //Windows 2008Server �ڐA�Ή�
      exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      //Windows 2008Server �ڐA�Ή�
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  �V�[�g�쐬 (��FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  �V�[�g�쐬 (���\)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  �V�[�g�쐬 (LR��)
   //******************************
   ShomeiLR();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server �ڐA�Ή�
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����NK�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiNK(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w NK �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "NK-FAX.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"NK-FAX");

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "NK�\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"NK�\");

   //******************************
   //***  �V�[�g�R�s�[ (LR��)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "NK��.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"NK��");

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
	  G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
	  G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  �V�[�g�쐬 (NK-FAX)
   //******************************
   ShinseiFaxNK();

   //******************************
   //***  �V�[�g�쐬 (NK�\)
   //******************************
   ShinseiNK();

   //******************************
   //***  �V�[�g�쐬 (NK��)
   //******************************
   ShomeiNK();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server �ڐA�Ή�
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����GL�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiGL(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w GL �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "��-FAX-GL.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"��-FAX");

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "���\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"���\");   

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  �V�[�g�쐬 (��FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  �V�[�g�쐬 (���\)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server �ڐA�Ή�
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}


//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����KR�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiKR(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w KR �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "��-FAX-KR.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"��-FAX");

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "���\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"���\");

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  �V�[�g�쐬 (��FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  �V�[�g�쐬 (���\)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server �ڐA�Ή�
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }


}


//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����BV�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiBV(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w BV �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "��-FAX-BV.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"��-FAX");

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "���\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"���\");

   //******************************
   //***  �V�[�g�R�s�[ (BV��)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "BV��.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"BV��");

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
	  G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  �V�[�g�쐬 (��FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  �V�[�g�쐬 (���\)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  �V�[�g�쐬 (BV��)
   //******************************
   ShomeiBV();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server �ڐA�Ή�
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

}

//---------------------------------------------------------------------------
// ���{��֐����F ���p�\����CCS�쐬����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �{���ڂ̃R�s�[�̏��Ԃ�"�a��"�V�[�g�ɋ߂�������R�s�[���Ă����B
//             �F ���R�F"�a��"�V�[�g�̒��O�ɃR�s�[�����
//---------------------------------------------------------------------------
void __fastcall FuneyousinseiCCS(void)
{
   //******************************
   //***  �K�i۸ނ̏��o��
   //******************************
   G_Log_Text = "�\�����K�i�F    �w CCS �x";
   Write_Log(G_Log_Text);

   //******************************
   //***  �V�[�g�R�s�[ (��FAX)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "��-FAX-CCS.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"��-FAX");

   //******************************
   //***  �V�[�g�R�s�[ (���\)
   //******************************
   s_Tmplate_Pass = G_Template_Dir + "���\.xls";
   ExcelSheetCopy( s_MakeExcel_Pass,"�a��",s_Tmplate_Pass,"���\");

   //******************************
   //***  Excel Open����
   //******************************
   // ̧�ٵ����
   try {
	  //exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
	  //Windows 2008Server �ڐA�Ή�
	  exWorkbook  = exWorkbooks.OleFunction("Open", (OleVariant)s_MakeExcel_Pass );
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xOpen";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x��OPEN�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   // ��ĺڸ��ݎ擾
   try {
	  //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
	  //Windows 2008Server �ڐA�Ή�
	  exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
      G_Log_Text = "�t�@�C�����w"+s_MakeExcel_Pass+"�xWorksheets Open";
      Write_Log(G_Log_Text);
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }

   //******************************
   //***  �V�[�g�쐬 (��FAX)
   //******************************
   ShinseiFaxNonNK();

   //******************************
   //***  �V�[�g�쐬 (���\)
   //******************************
   ShinseiNonNK();

   //******************************
   //***  Excel Close����
   //******************************
   // �x���_�C�A���O��\������
   try{
	  //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
	  //Windows 2008Server �ڐA�Ή�
	  exApplication.OlePropertySet("DisplayAlerts", false);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application DisplayAlerts = false�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // ���O��t���ĕۑ�����
   try{
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Application ���O��t���ĕۑ��x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBook�N���[�Y����
   try{
      exWorkbook.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbook Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
   // WorkBooks�N���[�Y����
   try{
      exWorkbooks.OleFunction("Close");
   }
   catch (...) {
      G_ErrLog_Text = "�wExcel Workbooks Close�x�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return;
   }
}
