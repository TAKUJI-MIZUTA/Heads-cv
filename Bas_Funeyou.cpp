	//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���p�v�ڕ\  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.06.04
//�@ �X �V ���F2002.08.23
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_Funeyou.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ���v�ڕ\�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Funeyou(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_Tosou_CD;        // �h���F����
   AnsiString s_Text;            //
   AnsiString s_WkText1;
   AnsiString s_WkText2;

   int        i_RecNo;           // HEADS�ް� ں���No
   int i_Value;
   double     d_BpP;            // B��������
   double     d_BpH;            // B��������
   double     d_ApP;            // A��������
   double     d_ApH;            // A��������

   bool bRet;
   AnsiString s_HEADS_DATA[1500];

   // ���d�l�łȂ��ꍇ�͏����s�v(����I��)
   if(!pcHeadsData->GetSpecType()==DEF_SPEC_TYPE_NONE) return(true);

   // HEADS�ް���۰�ٕϐ��ɾ��
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   s_Tmplate_Pass = G_Template_Dir + "���p�v�ڕ\.xls";               // ����ڰ�̧��
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "M" + G_SAKUBAN + ".xls";   // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "M" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";   // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���p�v�ڕ\�x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "�쐬�t�@�C�����F      �w" + s_MakeExcel_Pass + "�x";
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
      G_Log_Text = "�t�@�C���R�s�[�����B  �w" + s_Tmplate_Pass +
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

   //**********************************************************************************
   //***                                                                            ***
   //***                    ���p�v�ڕ\(�a��)�@�f�[�^�Z�b�g                          ***
   //***                                                                            ***
   //**********************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�a��");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�a��"));
   //*********************
   //***  �a���FDOC.No.
   //*********************
   // �wA001�x
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_Text = "M" + G_SAKUBAN;
   s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
   Excel_Cell_DataSet(1, 13, s_Text);
   G_Log_Text = "�a���FDOC.No.         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �a���FMFG.No.
   //*********************
   // �wA002�x
   //2022/12/15 �����ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ǉ��Ή� Mod_E

   //-------------------------------------
   // 2007.10.09 �װ�d�l�Ή� "A/B"��t��
   if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
   }
   // 2007.10.09
   //-------------------------------------

   Excel_Cell_DataSet(3, 1, s_Text);
   G_Log_Text = "�a���FMFG.No.         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �a���F�^��
   //*********************
   // �wA003�x
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(2, 4, s_Text);
   //Excel_Cell_DataSet(3, 3, s_Text);
   //*********************
   G_Log_Text = "�a���F�^��             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.04.26 ̫�ϯĒ���
   //*********************
   //***  �a���FEQPT.NAME
   //*********************
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(3, 4, s_Text);
   G_Log_Text = "�a���FEQPT.NAME        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*********************

   //*********************
   //***  �a���F�䐔
   //*********************
   // �wA004�x
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(2, 13, s_Text);
      G_Log_Text = "�a���F�䐔             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�a���F�䐔      �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //************************
   //***  �a���F�K�p�K�i
   //************************
   // �wA005�x
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "�a���F�K�p�K�i         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x�ɂ��A";
   if ( s_Text.ToIntDef(0) == 0 ) {
      s_Text = "";
   } else {
      s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),true,true, s_HEADS_DATA[1].TrimRight());
   }
   Excel_Cell_DataSet(5, 4, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �a���F�D��
   //************************
   // �wA006�x
   i_RecNo = 270;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 4, s_Text);
   G_Log_Text = "�a���F�D��             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �a���F�`�M�ʐ�
   //************************
   // �wA007�x
   i_RecNo = 1067;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 4, s_Text);
   G_Log_Text = "�a���F�`�M�ʐ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.04.26 ̫�ϯĒ���
   //************************
   //***  �a���F�����M��
   //************************
   i_RecNo = 107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(8, 4, s_Text);
   G_Log_Text = "�a���F�����M��         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  �a���F�@�펿�� DRY
   //****************************
   // �wA008�x
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(9, 4, s_Text);
   //Excel_Cell_DataSet(8, 4, s_Text);
   //*********************
   G_Log_Text = "�a���F�@�펿�� DRY     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F�@�펿�� WET
   //****************************
   // �wA009�x
   i_RecNo = 1030;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(10, 4, s_Text);
   //Excel_Cell_DataSet(9, 4, s_Text);
   //*********************
   G_Log_Text = "�a���F�@�펿�� WET     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   ///****************************
   //***  �a���FB���t�̖�
   //****************************
   // �wA010�x
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
//      i_RecNo = 76;     // �`���̖�
//   } else {
//      i_RecNo = 45;     // �a���̖�
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1071].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(12, 4, s_Text);
   //Excel_Cell_DataSet(11, 4, s_Text);
   //*********************
   G_Log_Text = "�a���FB���t�̖�        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FA���t�̖�
   //****************************
   // �wA011�x
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
//      i_RecNo = 45;     // �a���̖�
//   } else {
//      i_RecNo = 76;     // �`���̖�
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1073].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(12, 10, s_Text);
   //Excel_Cell_DataSet(11, 10, s_Text);
   //*********************
   G_Log_Text = "�a���FA���t�̖�        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FB�������x
   //****************************
   // �wA012�x
   i_RecNo = 852;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(13, 4, s_Text);
   //Excel_Cell_DataSet(12, 4, s_Text);
   //*********************
   G_Log_Text = "�a���FB�������x        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FA�������x
   //****************************
   // �wA013�x
   i_RecNo = 853;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(13, 10, s_Text);
   //Excel_Cell_DataSet(12, 10, s_Text);
   //*********************
   G_Log_Text = "�a���FA�������x        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FB�o�����x
   //****************************
   i_RecNo = 854;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(14, 4, s_Text);
   //Excel_Cell_DataSet(13, 4, s_Text);
   //*********************
   G_Log_Text = "�a���FB�o�����x        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FA�o�����x
   //****************************
   i_RecNo = 855;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(14, 10, s_Text);
   //Excel_Cell_DataSet(13, 10, s_Text);
   //*********************
   G_Log_Text = "�a���FA�o�����x        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.20 �v�����ݒ�ɂ��ϕ�����Ή�
   s_WkText1 = s_HEADS_DATA[846].TrimRight();  // B�X�`�[���敪(CASE1)
   s_WkText2 = s_HEADS_DATA[847].TrimRight();  // A�X�`�[���敪(CASE1)
   if( s_WkText1 == "0" && s_WkText2 == "0" ){
      // �t-�t�F[m3/h]
      Excel_Cell_DataSet(15, 1,"����[m3/h]");
   }else if( s_WkText1 == "1" && s_WkText2 == "0" ){
      // ���C-�t�F[kg/h m3/h]
      Excel_Cell_DataSet(15, 1,"����[kg/h m3/h]");
   }else if( s_WkText1 == "0" && s_WkText2 == "1" ){
      // �t-���C�F[m3/h kg/h]
      Excel_Cell_DataSet(15, 1,"����[m3/h kg/h]");
   }
   //-------------------------------------------------


   //*********************
   //***  �a���FB����
   //*********************
   i_RecNo = 856;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(15, 4, s_Text);
   //Excel_Cell_DataSet(14, 4, s_Text);
   //*********************
   G_Log_Text = "�a���FB����            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �a���FA����
   //*********************
   i_RecNo = 857;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(15, 10, s_Text);
   //Excel_Cell_DataSet(14, 10, s_Text);
   //*********************
   G_Log_Text = "�a���FA����            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FB���͑���
   //****************************
   i_RecNo = 858;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 �����ύX
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(16, 4, s_Text);
   //Excel_Cell_DataSet(15, 4, s_Text);
   //*********************
   G_Log_Text = "�a���FB���͑���        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FA���͑���
   //****************************
   i_RecNo = 859;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 �����ύX
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(16, 10, s_Text);
   //Excel_Cell_DataSet(15, 10, s_Text);
   //*********************
   G_Log_Text = "�a���FA���͑���        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FB�݌v����
   //****************************
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "��C�J��";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(17, 4, s_Text);
   //Excel_Cell_DataSet(16, 4, s_Text);
   //*********************
   G_Log_Text = "�a���FB�݌v����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FA�݌v����
   //****************************
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "��C�J��";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(17, 10, s_Text);
   //Excel_Cell_DataSet(16, 10, s_Text);
   //*********************
   G_Log_Text = "�a���FA�݌v����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.04.26 ̫�ϯĒ���
   //****************************
   //***  �a���FB�݌v���x
   //****************************
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 4, s_Text);
   G_Log_Text = "�a���FB�݌v���x        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FA�݌v���x
   //****************************
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "�a���FA�݌v���x        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  �a���FB�e�X�g����
   //****************************
   //****************************
   //***  �a���FA�e�X�g����
   //****************************
   s_Text = s_HEADS_DATA[62].Trim();   // �a��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpP = s_Text.ToDouble();
   } else {
      d_BpP = 0.0;
   }
   s_Text = s_HEADS_DATA[61].Trim();   // �a��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpH = s_Text.ToDouble();
   } else {
      d_BpH = 0.0;
   }
   s_Text = s_HEADS_DATA[93].Trim();   // �`��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApP = s_Text.ToDouble();
   } else {
      d_ApP = 0.0;
   }
   s_Text = s_HEADS_DATA[92].Trim();   // �`��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApH = s_Text.ToDouble();
   } else {
      d_ApH = 0.0;
   }
   if( d_BpP > 0.0 || d_ApP > 0.0) {         // �a�������́��O���͂`�������́��O
      if( d_BpH > 0.0 || d_ApH ) {       // �a�����������O���͂`�����������O
         // �����̏�������������ꍇ�̓e�X�g���͍��ڂ��󗓂Ƃ���
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 1, "");
         Excel_Cell_DataSet(19, 4, "");
         Excel_Cell_DataSet(19, 10, "");
         //Excel_Cell_DataSet(17, 1, "");
         //Excel_Cell_DataSet(17, 4, "");
         //Excel_Cell_DataSet(17, 10, "");
         //*********************
         G_ErrLog_Text = "�a���FB�e�X�g����  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      } else {
         //2004.05.21 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 1, "ýĈ��́i�󈳁j[MPaG]");
         //Excel_Cell_DataSet(17, 1, "ýĈ��́i�󈳁j[MPaG]");
         //*********************
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB�t�ł͂Ȃ�
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "�a���FB�e�X�g����      RecNo:62�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "�a���FA�e�X�g����      RecNo:61�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);
         } else {
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "�a���FB�e�X�g����      RecNo:62�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "�a���FA�e�X�g����      RecNo:61�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);
         }
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // �a�����������O���͂`�����������O
      //2004.05.21 ̫�ϯĒ���
      Excel_Cell_DataSet(19, 1, "ýĈ��́i�����j[MPaG]");
      //Excel_Cell_DataSet(17, 1, "ýĈ��́i�����j[MPaG]");
      //*********************
      if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB�t�ł͂Ȃ�
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "�a���FB�e�X�g����      RecNo:93�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "�a���FB�e�X�g����      RecNo:92�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "�a���FB�e�X�g����      RecNo:93�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "�a���FB�e�X�g����      RecNo:92�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);
      }
   } else {
      //2004.04.26 ̫�ϯĒ���
      Excel_Cell_DataSet(19, 1, "");
      Excel_Cell_DataSet(19, 4, "");
      Excel_Cell_DataSet(19, 10, "");
      //Excel_Cell_DataSet(17, 1, "");
      //Excel_Cell_DataSet(17, 4, "");
      //Excel_Cell_DataSet(17, 10, "");
      //*********************
      G_ErrLog_Text = "�a���FB�e�X�g����  �Y�����鐔�l������܂���B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }


   //****************************
   //***  �a���FBΰ��ޗ�
   //****************************
   i_RecNo = 1282;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(20, 4, s_Text);
   //Excel_Cell_DataSet(18, 4, s_Text);
   //*********************
   G_Log_Text = "�a���FBΰ��ޗ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���FAΰ��ޗ�
   //****************************
   i_RecNo = 1283;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(20, 10, s_Text);
   //Excel_Cell_DataSet(18, 10, s_Text);
   //*********************
   G_Log_Text = "�a���FAΰ��ޗ�         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // �i�^�̓`�F�[���i�ړ��~�߁j�s�v
   if(s_HEADS_DATA[36].Pos("J") != 0) {
      //2004.04.26 ̫�ϯĒ���
      Excel_Cell_DataSet(26, 4, "----");
      //Excel_Cell_DataSet(24, 4, "----");
      //*********************
   }


   //-------------------------------------------------
   // OGI 2015.02.19 �A�L���[�����[�^���i�Ή�
   // ���f�K�X�[�U�H��A���f�K�X�[�U����[MPa]�A���숳�́yMPa�z�A�A�L���[�����[�^ �̒ǉ��Ή�

   //***************************
   //*** �a���F���f�K�X�[�U�H��
   //***************************
   s_Text = s_HEADS_DATA[1117].TrimRight();
   if (s_Text == "1") {
      Excel_Cell_DataSet(27, 4, "1");
   } else {
      Excel_Cell_DataSet(27, 4, "----");
   }

   //***************************
   //*** �a���F���f�K�X�[�U����[MPa]
   //***************************
   Excel_Cell_DataSet(29, 4, "----");

   //***************************
   //*** �a���F���숳�́yMPa�z
   //***************************
   Excel_Cell_DataSet(30, 4, "----");

   //***************************
   //***  �a���F�A�L���[�����[�^
   //***************************
   AnsiString s_Nzl_SyuruiE3 = s_HEADS_DATA[981].TrimRight();
   AnsiString s_Nzl_SyuruiE4 = s_HEADS_DATA[996].TrimRight();
   if (s_Nzl_SyuruiE3 == "8" || s_Nzl_SyuruiE4 == "8"){
      Excel_Cell_DataSet(30, 12, "1");
   } else {
      Excel_Cell_DataSet(30, 12, "----");
   }


   //****************************
   //***  �a���F���������
   //****************************
   i_Value = 0;
   // 2011.06.20 ɽ�ًK�i�ύX
   //if( (s_HEADS_DATA[905].SubString(1,1) == "7")&&(s_HEADS_DATA[905].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[920].SubString(1,1) == "7")&&(s_HEADS_DATA[920].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[965].SubString(1,1) == "7")&&(s_HEADS_DATA[965].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[980].SubString(1,1) == "7")&&(s_HEADS_DATA[980].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[905].SubString(1,2) == "07")
    && (s_HEADS_DATA[905].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[920].SubString(1,2) == "07")
    && (s_HEADS_DATA[920].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[965].SubString(1,2) == "07")
    && (s_HEADS_DATA[965].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[980].SubString(1,2) == "07")
    && (s_HEADS_DATA[980].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(23, 12, s_Text);
   //Excel_Cell_DataSet(21, 12, s_Text);
   //*********************
   G_Log_Text = "�a���F���������       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F���ݔ�����
   //****************************
   i_Value = 0;
   // 2011.06.20 ɽ�ًK�i�ύX
   //if( (s_HEADS_DATA[935].SubString(1,1) == "7")&&(s_HEADS_DATA[935].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[950].SubString(1,1) == "7")&&(s_HEADS_DATA[950].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[995].SubString(1,1) == "7")&&(s_HEADS_DATA[995].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[1010].SubString(1,1) == "7")&&(s_HEADS_DATA[1010].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[935].SubString(1,2) == "07")
    && (s_HEADS_DATA[935].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[950].SubString(1,2) == "07")
    && (s_HEADS_DATA[950].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[995].SubString(1,2) == "07")
    && (s_HEADS_DATA[995].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[1010].SubString(1,2) == "07")
    && (s_HEADS_DATA[1010].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(24, 12, s_Text);
   //Excel_Cell_DataSet(22, 12, s_Text);
   //*********************
   G_Log_Text = "�a���F���ݔ�����      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F����
   //****************************
   s_Text = "1";
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(25, 12, s_Text);
   //Excel_Cell_DataSet(23, 12, s_Text);
   //*********************
   G_Log_Text = "�a���F����            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.03.14 �k�^���x�v�̔����Ɂh11�h��ǉ�
   //****************************
   //***  �a���F���x�v L�^
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() == "06" || s_HEADS_DATA[898].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() == "06" || s_HEADS_DATA[913].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() == "06" || s_HEADS_DATA[928].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() == "06" || s_HEADS_DATA[943].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() == "06" || s_HEADS_DATA[958].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() == "06" || s_HEADS_DATA[973].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() == "06" || s_HEADS_DATA[988].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() == "06" || s_HEADS_DATA[1003].TrimRight() == "11"){
      i_Value++;
   }
   //-------------------------------------------------

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(26, 13, s_Text);
   //Excel_Cell_DataSet(24, 13, s_Text);
   //*********************
   G_Log_Text = "�a���F���x�v L�^      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F���x�v I�^
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() != "06" && s_HEADS_DATA[898].TrimRight() != "07" && s_HEADS_DATA[898].TrimRight() != "11" && s_HEADS_DATA[898].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() != "06" && s_HEADS_DATA[913].TrimRight() != "07" && s_HEADS_DATA[913].TrimRight() != "11" && s_HEADS_DATA[913].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() != "06" && s_HEADS_DATA[928].TrimRight() != "07" && s_HEADS_DATA[928].TrimRight() != "11" && s_HEADS_DATA[928].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() != "06" && s_HEADS_DATA[943].TrimRight() != "07" && s_HEADS_DATA[943].TrimRight() != "11" && s_HEADS_DATA[943].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() != "06" && s_HEADS_DATA[958].TrimRight() != "07" && s_HEADS_DATA[958].TrimRight() != "11" && s_HEADS_DATA[958].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() != "06" && s_HEADS_DATA[973].TrimRight() != "07" && s_HEADS_DATA[973].TrimRight() != "11" && s_HEADS_DATA[973].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() != "06" && s_HEADS_DATA[988].TrimRight() != "07" && s_HEADS_DATA[988].TrimRight() != "11" && s_HEADS_DATA[988].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() != "06" && s_HEADS_DATA[1003].TrimRight() != "07" && s_HEADS_DATA[1003].TrimRight() != "11" && s_HEADS_DATA[1003].ToIntDef(0) > 0){
      i_Value++;
   }


   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(27, 13, s_Text);
   //Excel_Cell_DataSet(25, 13, s_Text);
   //*********************
   G_Log_Text = "�a���F���x�v I�^      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F��ڰ�
   //****************************
   if (s_HEADS_DATA[891].ToIntDef(0) != 5 && s_HEADS_DATA[906].ToIntDef(0) != 5
                                          && s_HEADS_DATA[921].ToIntDef(0) != 5
                                          && s_HEADS_DATA[936].ToIntDef(0) != 5
                                          && s_HEADS_DATA[951].ToIntDef(0) != 5
                                          && s_HEADS_DATA[966].ToIntDef(0) != 5
                                          && s_HEADS_DATA[981].ToIntDef(0) != 5
                                          && s_HEADS_DATA[996].ToIntDef(0) != 5){

      s_Text = "----";
   } else {
      s_Text = "1";
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(28, 12, s_Text);
   //Excel_Cell_DataSet(26, 12, s_Text);
   //*********************
   G_Log_Text = "�a���F��ڰ�           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F��j�����͌v
   //****************************
   i_Value=0;
   //2015.04.28 ���ʕύX
   //if (s_HEADS_DATA[901].ToIntDef(0) == 16 || s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[916].ToIntDef(0) == 16 || s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[961].ToIntDef(0) == 16 || s_HEADS_DATA[961].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[976].ToIntDef(0) == 16 || s_HEADS_DATA[976].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[901].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[931].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[946].ToIntDef(0) == 17 )  i_Value++;
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_S
   if (s_HEADS_DATA[901].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_E
   i_Value = i_Value / 2;
   //*******************
   if (i_Value == 0 ) {
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(29, 12, s_Text);
   //Excel_Cell_DataSet(27, 12, s_Text);
   //*********************
   G_Log_Text = "�a���F��j�����͌v           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.20 �v�����ݒ�ɂ��ϕ�����Ή�
   s_WkText1 = s_HEADS_DATA[901].TrimRight();  // S1���͌v���
   s_WkText2 = s_HEADS_DATA[916].TrimRight();  // S2���͌v���
   if( s_WkText1 == "16" || s_WkText2 == "16" ){
      Excel_Cell_DataSet(29, 7, "��j�����͌v");
      //2015.04.28 ���ʕύX
      //Excel_Cell_DataSet(29, 12, "2");
      //*******************
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_S
   } else if ( s_WkText1 == "21" || s_WkText2 == "21" ){
      Excel_Cell_DataSet(29, 7, "���͌v");
   } else if ( s_WkText1 == "22" || s_WkText2 == "22" ){
      Excel_Cell_DataSet(29, 7, "���͌v");
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_E
   } else {
      Excel_Cell_DataSet(29, 7, "��j�����͌v");
   }
   //-------------------------------------------------


   //****************************
   //***  �a���F�h���F
   //****************************
   i_RecNo = 31;
   s_Tosou_CD = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Tosou_CD.ToIntDef(0) == 1){
      // RecNo:539 �h���F���
      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   } else if (s_Tosou_CD.ToIntDef(0) == 3 || s_Tosou_CD.ToIntDef(0) == 5) {
      // �h���F����ð��ٓ��a���� + RecNo:539 �h���F���
//      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "J");

      i_RecNo = 32;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   } else {
      // �h���F����ð��ٓ��a���̂��
      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "J");
   }
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(32, 1, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(31, 1, s_Text);
   //Excel_Cell_DataSet(29, 1, s_Text);
   //*********************
   G_Log_Text = "�a���F�h���F          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F�����ӔC��
   //****************************
   i_RecNo = 1085;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(34, 3, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(33, 3, s_Text);
   //Excel_Cell_DataSet(31, 3, s_Text);
   //*********************
   G_Log_Text = "�a���F�����ӔC��       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F�����S����
   //****************************
   i_RecNo = 1083;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(35, 3, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(34, 3, s_Text);
   //Excel_Cell_DataSet(32, 3, s_Text);
   //*********************
   G_Log_Text = "�a���F�����S����       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �a���F�݌v�S����
   //****************************
   //2017.04.17 ���p�S���ҕύX
   //i_RecNo = 1079;
   i_RecNo = 1271;
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(36, 3, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(35, 3, s_Text);
   //Excel_Cell_DataSet(33, 3, s_Text);
   //*********************
   G_Log_Text = "�a���F�݌v�S����       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F���t1
   //***************************
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(34, 5, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(33, 5, s_Text);
   //Excel_Cell_DataSet(31, 5, s_Text);
   //*********************
   G_Log_Text = "�a���F���t�P           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F���t2
   //***************************
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(35, 5, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(34, 5, s_Text);
   //Excel_Cell_DataSet(32, 5, s_Text);
   //*********************
   G_Log_Text = "�a���F���t�Q           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F���t3
   //***************************
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(36, 5, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(35, 5, s_Text);
   //Excel_Cell_DataSet(33, 5, s_Text);
   //*********************
   G_Log_Text = "�a���F���t�R           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���FHEADS VER
   //***************************
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(39, 1, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(38, 1, s_Text);
   //Excel_Cell_DataSet(36, 1, s_Text);
   //*********************
   G_Log_Text = "�a���FHEADS VER        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //-----------------------------------
   //2004.11.15 [���ޔԍ�]�o�͒ǉ�

   //***************************
   //***  �a���F���ޔԍ�
   //***************************
   if(pcHeadsData->GetMirrorFlag()){
      s_Text = "���ނU";     // �~���[�d�l
   }else{
      s_Text = "���ނS";     // �ʏ�d�l
   }
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(36, 15, s_Text);
   //Excel_Cell_DataSet(35, 15, s_Text);
   G_Log_Text = "�a���F���ޔԍ�         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2007.11.15
   //-----------------------------------

   //2008.04.14 RX-00M[����ĥ���p��׼��ް����ڰĕs�v]�o�͒ǉ�


   //-------------------------------------------------
   // OGI 2015.02.20 �v�����ݒ�ɂ��ϕ�����Ή�
   //***************************
   //***  �a���F���`�F�b�g����
   //***************************
//   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
//    s_Text = "----";
//    Excel_Cell_DataSet(23, 4, s_Text);
//   }

   // ���`�F�b�g�X�p�i�̗L�����o�͕�����ύX����
   // s_HEADS_DATA[262]
   //   0   �F�X�p�i�L�A���p�p��L
   //   1   �F�X�p�i���A���p�p�
   //   2   �F�X�p�i�L�A���p�p��L
   //   3   �F�X�p�i���A���p�p�
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
        // RX-00�̏ꍇ
        s_Text = "----";
        Excel_Cell_DataSet(23, 4, s_Text);
   }else{
        // ���`�F�b�g�L�薳������
        if (s_HEADS_DATA[262].TrimRight() == "0" || s_HEADS_DATA[262].TrimRight() == "2") {
          Excel_Cell_DataSet(23, 4, "1");
        } else {
          Excel_Cell_DataSet(23, 4, "----");
        }
   }

   G_Log_Text = "�a���F���`�F�b�g����   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //-------------------------------------------------


   //***************************
   //***  �a���F���p�u���V����
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
   }
   G_Log_Text = "�a���F���p�u���V����   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F�Q�[�W�v���[�g����
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(25, 4, s_Text);
   }
   G_Log_Text = "�a���F�Q�[�W�v���[�g����   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.03.04 �v�����ݒ�ɂ��ϕ�����Ή�
   //***************************
   //***  �a���F���p�p��̏ꍇ�́A���p�u���V�A�Q�[�W�v���[�g�A�`�F�[���̐��ʂ�0�ɂ���
   //***************************
   if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
      Excel_Cell_DataSet(25, 4, s_Text);
      Excel_Cell_DataSet(26, 4, s_Text);
   }
   //-------------------------------------------------


   //**********************************************************


   //**********************************************************************************
   //***                                                                            ***
   //***                    ���p�v�ڕ\(�p��)�@�f�[�^�Z�b�g                          ***
   //***                                                                            ***
   //**********************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�p��");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�p��"));

   //*********************
   //***  �p���FDOC.No.
   //*********************
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_Text = "M" + G_SAKUBAN;
   s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
   Excel_Cell_DataSet(1, 13, s_Text);
   G_Log_Text = "�p���FDOC.No.         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �p���FMFG.No.
   //*********************
   //2022/12/15 �����ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ǉ��Ή� Mod_E   

   //-------------------------------------
   // 2007.10.09 �װ�d�l�Ή� "A/B"��t��
   if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
   }
   // 2007.10.09
   //-------------------------------------

   Excel_Cell_DataSet(3, 1, s_Text);
   G_Log_Text = "�p���FMFG.No.         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************
   //***  �p���FMODEL
   //*********************
   i_RecNo = 33;
// �p���̏ꍇ�i���j�͕s�v
//   if (s_HEADS_DATA[1057].TrimRight() == "1"){
//      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
//   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(2, 4, s_Text);
   //Excel_Cell_DataSet(3, 3, s_Text);
   //*********************
   G_Log_Text = "�p���FMODEL            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.04.26 ̫�ϯĒ���
   //*********************
   //***  �p���FEQPT.NAME
   //*********************
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(3, 4, s_Text);
   G_Log_Text = "�p���FEQPT.NAME        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*********************

   //*********************
   //***  �p���FQUANTITY
   //*********************
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(2, 13, s_Text);
      G_Log_Text = "�p���FQUANTITY         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�p���FQUANTITY  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //********************************
   //***  �p���FAPPLICABLE CODE
   //********************************
   i_RecNo = 29;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "�p���FAPPLICABLE CODE  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x�ɂ��A";
   s_Text = Search_KIKAKU_CODE(s_HEADS_DATA[i_RecNo].TrimRight(),false,true, s_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(5, 4, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //************************
   //***  �p���FSHIP No.
   //************************
   i_RecNo = 270;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 4, s_Text);
   G_Log_Text = "�p���FSHIP No.         RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �p���FHEAT TRANSFER
   //*******************************
   i_RecNo = 1067;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(7, 4, s_Text);
   G_Log_Text = "�p���FHEAT TRANSFER    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.04.26 ̫�ϯĒ���
   //*******************************
   //***  �p���FHEAT EXCHANGED
   //*******************************
   i_RecNo = 107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(8, 4, s_Text);
   G_Log_Text = "�p���FHEAT EXCHANGED   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  �p���FMASS[kg] DRY
   //****************************
   i_RecNo = 1029;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(9, 4, s_Text);
   //Excel_Cell_DataSet(8, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FMASS[kg] DRY     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FMASS[kg] WET
   //****************************
   i_RecNo = 1030;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(10, 4, s_Text);
   //Excel_Cell_DataSet(9, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FMASS[kg] WET     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   ///****************************
   //***  �p���FB FLUID NAME
   //****************************
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
//      i_RecNo = 76;     // �`���̖�
//   } else {
//      i_RecNo = 45;     // �a���̖�
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1072].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(12, 4, s_Text);
   //Excel_Cell_DataSet(11, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FB FLUID NAME     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FA FLUID NAME
   //****************************
//   i_RecNo = 137;
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
//   if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
//      i_RecNo = 45;     // �a���̖�
//   } else {
//      i_RecNo = 76;     // �`���̖�
//   }
//   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = s_HEADS_DATA[1074].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(12, 10, s_Text);
   //Excel_Cell_DataSet(11, 10, s_Text);
   //*********************
   G_Log_Text = "�p���FA FLUID NAME     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  �p���FB INLET TEMP[��]
   //********************************
   i_RecNo = 852;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(13, 4, s_Text);
   //Excel_Cell_DataSet(12, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FB INLET TEMP     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  �p���FA INLET TEMP[��]
   //********************************
   i_RecNo = 853;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(13, 10, s_Text);
   //Excel_Cell_DataSet(12, 10, s_Text);
   //*********************
   G_Log_Text = "�p���FA INLET TEMP     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  �p���FB OUTLET TEMP[��]
   //********************************
   i_RecNo = 854;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(14, 4, s_Text);
   //Excel_Cell_DataSet(13, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FB OUTLET TEMP    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  �p���FA OUTLET TEMP[��]
   //********************************
   i_RecNo = 855;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(14, 10, s_Text);
   //Excel_Cell_DataSet(13, 10, s_Text);
   //*********************
   G_Log_Text = "�p���FA OUTLET TEMP    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.23 �v�����ݒ�ɂ��ϕ�����Ή��i�p���j
   s_WkText1 = s_HEADS_DATA[846].TrimRight();  // B�X�`�[���敪(CASE1)
   s_WkText2 = s_HEADS_DATA[847].TrimRight();  // A�X�`�[���敪(CASE1)
   if( s_WkText1 == "0" && s_WkText2 == "0" ){
      // �t-�t�F[m3/h]
      Excel_Cell_DataSet(15, 1,"FLOW RATE[m3/h]");
   }else if( s_WkText1 == "1" && s_WkText2 == "0" ){
      // ���C-�t�F[kg/h m3/h]
      Excel_Cell_DataSet(15, 1,"FLOW RATE[kg/h m3/h]");
   }else if( s_WkText1 == "0" && s_WkText2 == "1" ){
      // �t-���C�F[m3/h kg/h]
      Excel_Cell_DataSet(15, 1,"FLOW RATE[m3/h kg/h]");
   }
   //-------------------------------------------------


   //********************************
   //***  �p���FB FLOW RATE[m3/h]
   //********************************
   i_RecNo = 856;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(15, 4, s_Text);
   //Excel_Cell_DataSet(14, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FB FLOW RATE      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  �p���FA FLOW RATE[m3/h]
   //********************************
   i_RecNo = 857;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(15, 10, s_Text);
   //Excel_Cell_DataSet(14, 10, s_Text);
   //*********************
   G_Log_Text = "�p���FA FLOW RATE      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  �p���FB PRESSURE DROP[MPa]
   //*********************************
   i_RecNo = 858;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 �����ύX
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(16, 4, s_Text);
   //Excel_Cell_DataSet(15, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FB PRESSURE DROP  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  �p���FA PRESSURE DROP[MPa]
   //*********************************
   i_RecNo = 859;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.08 �����ύX
   s_Text = FormatFloat("#,###,##0.000",StrToCurr(s_Text));
   //*******************
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(16, 10, s_Text);
   //Excel_Cell_DataSet(15, 10, s_Text);
   //*********************
   G_Log_Text = "�p���FA PRESSURE DROP  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  �p���FB DESIGN PRESS[MPaG]
   //*********************************
   i_RecNo = 825;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "0.00";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(17, 4, s_Text);
   //Excel_Cell_DataSet(16, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FB DESIGN PRESS   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  �p���FA DESIGN PRESS[MPaG]
   //*********************************
   i_RecNo = 830;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.TrimRight() != "" ) {
      if ( s_Text.ToDouble() >= 999 ) {
         s_Text = "0.00";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }
   } else {
      s_Text = "";
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(17, 10, s_Text);
   //Excel_Cell_DataSet(16, 10, s_Text);
   //*********************
   G_Log_Text = "�p���FA DESIGN PRESS   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2004.04.26 ̫�ϯĒ���
   //********************************
   //***  �p���FB DESIGN TEMP.[��]
   //********************************
   i_RecNo = 829;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 4, s_Text);
   G_Log_Text = "�p���FB DESIGN TEMP.   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  �p���FA DESIGN TEMP.[��]
   //********************************
   i_RecNo = 834;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "�p���FA DESIGN TEMP.   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //*********************

   //****************************
   //***  �p���FB�e�X�g����
   //****************************
   //****************************
   //***  �p���FA�e�X�g����
   //****************************
   s_Text = s_HEADS_DATA[62].Trim();   // �a��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpP = s_Text.ToDouble();
   } else {
      d_BpP = 0.0;
   }
   s_Text = s_HEADS_DATA[61].Trim();   // �a��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_BpH = s_Text.ToDouble();
   } else {
      d_BpH = 0.0;
   }
   s_Text = s_HEADS_DATA[93].Trim();   // �`��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApP = s_Text.ToDouble();
   } else {
      d_ApP = 0.0;
   }
   s_Text = s_HEADS_DATA[92].Trim();   // �`��������
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      d_ApH = s_Text.ToDouble();
   } else {
      d_ApH = 0.0;
   }

   if( d_BpP > 0.0 || d_ApP > 0.0) {         // �a�������́��O���͂`�������́��O
      if( d_BpH > 0.0 || d_ApH ) {       // �a�����������O���͂`�����������O
         // �����̏�������������ꍇ�̓e�X�g���͍��ڂ��󗓂Ƃ���
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 1, "");
         Excel_Cell_DataSet(19, 4, "");
         Excel_Cell_DataSet(19, 10, "");
         //Excel_Cell_DataSet(17, 1, "");
         //Excel_Cell_DataSet(17, 4, "");
         //Excel_Cell_DataSet(17, 10, "");
         //*********************
         G_ErrLog_Text = "�p���FB�e�X�g����  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      } else {
         //2003.10.08 ������
         //Excel_Cell_DataSet(17, 1, "PNEU. PRESS[MPaG]");
         //2004.05.21 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 1, "TEST PRESS(PNEU.)[MPaG]");
         //Excel_Cell_DataSet(17, 1, "TEST PRESS(PNEU.)[MPaG]");
         //*********************
         //*******************
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB�t�ł͂Ȃ�
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "�p���FB�e�X�g����      RecNo:62�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "�p���FA�e�X�g����      RecNo:61�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);
         } else {
            s_Text = FormatFloat("#,###,##0.00",d_BpP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 10, s_Text);
            //Excel_Cell_DataSet(17, 10, s_Text);
            //*********************
            G_Log_Text = "�p���FB�e�X�g����      RecNo:62�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);

            s_Text = FormatFloat("#,###,##0.00",d_ApP);
            //2004.04.26 ̫�ϯĒ���
            Excel_Cell_DataSet(19, 4, s_Text);
            //Excel_Cell_DataSet(17, 4, s_Text);
            //*********************
            G_Log_Text = "�p���FA�e�X�g����      RecNo:61�w" + s_Text + "�x(��)���Z�b�g�B";
            Write_Log(G_Log_Text);
         }
      }
   } else if( d_BpH > 0.0 || d_ApH ) {       // �a�����������O���͂`�����������O
      //2003.10.08 ��������
      //Excel_Cell_DataSet(17, 1, "HYDRO. PRESS[MPaG]");
      //2004.05.21 ̫�ϯĒ���
      Excel_Cell_DataSet(19, 1, "TEST PRESS(HYDR.)[MPaG]");
      //Excel_Cell_DataSet(17, 1, "TEST PRESS(HYDR.)[MPaG]");
      //*********************
      //*******************
      if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) {     // AB�t�ł͂Ȃ�
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "�p���FB�e�X�g����      RecNo:93�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "�p���FB�e�X�g����      RecNo:92�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         s_Text = FormatFloat("#,###,##0.00",d_BpH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 10, s_Text);
         //Excel_Cell_DataSet(17, 10, s_Text);
         //*********************
         G_Log_Text = "�p���FB�e�X�g����      RecNo:93�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);

         s_Text = FormatFloat("#,###,##0.00",d_ApH);
         //2004.04.26 ̫�ϯĒ���
         Excel_Cell_DataSet(19, 4, s_Text);
         //Excel_Cell_DataSet(17, 4, s_Text);
         //*********************
         G_Log_Text = "�p���FB�e�X�g����      RecNo:92�w" + s_Text + "�x(����)���Z�b�g�B";
         Write_Log(G_Log_Text);
      }
   } else {
      //2004.04.26 ̫�ϯĒ���
      Excel_Cell_DataSet(19, 1, "");
      Excel_Cell_DataSet(19, 4, "");
      Excel_Cell_DataSet(19, 10, "");
      //Excel_Cell_DataSet(17, 1, "");
      //Excel_Cell_DataSet(17, 4, "");
      //Excel_Cell_DataSet(17, 10, "");
      //*********************
      G_ErrLog_Text = "�p���FB�e�X�g����  �Y�����鐔�l������܂���B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }


   //*********************************
   //***  �p���FB HOLDING VOLUME[lit]
   //*********************************
   i_RecNo = 1282;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(20, 4, s_Text);
   //Excel_Cell_DataSet(18, 4, s_Text);
   //*********************
   G_Log_Text = "�p���FB HOLDING VOLUME RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  �p���FA HOLDING VOLUME[lit]
   //*********************************
   i_RecNo = 1283;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(20, 10, s_Text);
   //Excel_Cell_DataSet(18, 10, s_Text);
   //*********************
   G_Log_Text = "�p���FA HOLDING VOLUME RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

    // �i�^�̓`�F�[���i�ړ��~�߁j�s�v
   if(s_HEADS_DATA[36].Pos("J") != 0) {
      //2004.04.26 ̫�ϯĒ���
      Excel_Cell_DataSet(26, 4, "----");
      //Excel_Cell_DataSet(24, 4, "----");
      //*********************
   }


   //-------------------------------------------------
   // OGI 2015.02.19 �A�L���[�����[�^���i�Ή�
   // GAS CHARGING TOOLS, N2 GAS CHARGING PRESS[MPa], WORKING PRESS[MPa], ACCUMULATOR �̒ǉ��Ή�

   //***************************
   //*** �p���FGAS CHARGING TOOLS
   //***************************
   s_Text = s_HEADS_DATA[1117].TrimRight();
   if (s_Text == "1") {
      Excel_Cell_DataSet(27, 4, "1");
   } else {
      Excel_Cell_DataSet(27, 4, "----");
   }

   //***************************
   //*** �p���FN2 GAS CHARGING PRESS[MPa]
   //***************************
   Excel_Cell_DataSet(29, 4, "----");

   //***************************
   //*** �p���FWORKING PRESS[MPa]
   //***************************
   Excel_Cell_DataSet(30, 4, "----");

   //***************************
   //***  �p���FACCUMULATOR
   //***************************
   if (s_Nzl_SyuruiE3 == "8" || s_Nzl_SyuruiE4 == "8"){
      Excel_Cell_DataSet(30, 12, "1");
   } else {
      Excel_Cell_DataSet(30, 12, "----");
   }


   //*********************************
   //***  �p���FAIR VENT
   //*********************************
   i_Value = 0;
   // 2011.06.20 ɽ�ًK�i�ύX
   //if( (s_HEADS_DATA[905].SubString(1,1) == "7")&&(s_HEADS_DATA[905].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[920].SubString(1,1) == "7")&&(s_HEADS_DATA[920].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[965].SubString(1,1) == "7")&&(s_HEADS_DATA[965].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[980].SubString(1,1) == "7")&&(s_HEADS_DATA[980].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[905].SubString(1,2) == "07")
    && (s_HEADS_DATA[905].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[920].SubString(1,2) == "07")
    && (s_HEADS_DATA[920].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[965].SubString(1,2) == "07")
    && (s_HEADS_DATA[965].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[980].SubString(1,2) == "07")
    && (s_HEADS_DATA[980].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(23, 12, s_Text);
   //Excel_Cell_DataSet(21, 12, s_Text);
   //*********************
   G_Log_Text = "�p���FAIR VENT        �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FDRAIN BROW
   //****************************
   i_Value = 0;
   // 2011.06.20 ɽ�ًK�i�ύX
   //if( (s_HEADS_DATA[935].SubString(1,1) == "7")&&(s_HEADS_DATA[935].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[950].SubString(1,1) == "7")&&(s_HEADS_DATA[950].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[995].SubString(1,1) == "7")&&(s_HEADS_DATA[995].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   //if( (s_HEADS_DATA[1010].SubString(1,1) == "7")&&(s_HEADS_DATA[1010].SubString(3,1) == "9") ){
   //   i_Value++;
   //}
   if( (s_HEADS_DATA[935].SubString(1,2) == "07")
    && (s_HEADS_DATA[935].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[950].SubString(1,2) == "07")
    && (s_HEADS_DATA[950].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[995].SubString(1,2) == "07")
    && (s_HEADS_DATA[995].SubString(4,1) == "9") ){
      i_Value++;
   }
   if( (s_HEADS_DATA[1010].SubString(1,2) == "07")
    && (s_HEADS_DATA[1010].SubString(4,1) == "9") ){
      i_Value++;
   }
   // ***********************

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(24, 12, s_Text);
   //Excel_Cell_DataSet(22, 12, s_Text);
   //*********************
   G_Log_Text = "�p���FDRAIN BLOW      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FNAME PLATE
   //****************************
   s_Text = "1";
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(25, 12, s_Text);
   //Excel_Cell_DataSet(23, 12, s_Text);
   //*********************
   G_Log_Text = "�p���FNAME PLATE      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //-------------------------------------------------
   // OGI 2015.03.14 �k�^���x�v�̔����Ɂh11�h��ǉ�
   //****************************
   //***  �p���FTHERMOMETER L
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() == "06" || s_HEADS_DATA[898].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() == "06" || s_HEADS_DATA[913].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() == "06" || s_HEADS_DATA[928].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() == "06" || s_HEADS_DATA[943].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() == "06" || s_HEADS_DATA[958].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() == "06" || s_HEADS_DATA[973].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() == "06" || s_HEADS_DATA[988].TrimRight() == "11"){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() == "06" || s_HEADS_DATA[1003].TrimRight() == "11"){
      i_Value++;
   }
   //-------------------------------------------------

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(26, 13, s_Text);
   //Excel_Cell_DataSet(24, 13, s_Text);
   //*********************
   G_Log_Text = "�p���FTHERMOMETER L   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FTHERMOMETER I
   //****************************
   i_Value = 0;
   if (s_HEADS_DATA[898].TrimRight() != "06" && s_HEADS_DATA[898].TrimRight() != "07" && s_HEADS_DATA[898].TrimRight() != "11" && s_HEADS_DATA[898].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[913].TrimRight() != "06" && s_HEADS_DATA[913].TrimRight() != "07" && s_HEADS_DATA[913].TrimRight() != "11" && s_HEADS_DATA[913].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[928].TrimRight() != "06" && s_HEADS_DATA[928].TrimRight() != "07" && s_HEADS_DATA[928].TrimRight() != "11" && s_HEADS_DATA[928].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[943].TrimRight() != "06" && s_HEADS_DATA[943].TrimRight() != "07" && s_HEADS_DATA[943].TrimRight() != "11" && s_HEADS_DATA[943].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[958].TrimRight() != "06" && s_HEADS_DATA[958].TrimRight() != "07" && s_HEADS_DATA[958].TrimRight() != "11" && s_HEADS_DATA[958].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[973].TrimRight() != "06" && s_HEADS_DATA[973].TrimRight() != "07" && s_HEADS_DATA[973].TrimRight() != "11" && s_HEADS_DATA[973].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[988].TrimRight() != "06" && s_HEADS_DATA[988].TrimRight() != "07" && s_HEADS_DATA[988].TrimRight() != "11" && s_HEADS_DATA[988].ToIntDef(0) > 0){
      i_Value++;
   }
   if (s_HEADS_DATA[1003].TrimRight() != "06" && s_HEADS_DATA[1003].TrimRight() != "07" && s_HEADS_DATA[1003].TrimRight() != "11" && s_HEADS_DATA[1003].ToIntDef(0) > 0){
      i_Value++;
   }

   if (i_Value == 0){
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(27, 13, s_Text);
   //Excel_Cell_DataSet(25, 13, s_Text);
   //*********************
   G_Log_Text = "�p���FTHERMOMETER I   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FSTRAINER
   //****************************
   if (s_HEADS_DATA[891].ToIntDef(0) != 5 && s_HEADS_DATA[906].ToIntDef(0) != 5
                                          && s_HEADS_DATA[921].ToIntDef(0) != 5
                                          && s_HEADS_DATA[936].ToIntDef(0) != 5
                                          && s_HEADS_DATA[951].ToIntDef(0) != 5
                                          && s_HEADS_DATA[966].ToIntDef(0) != 5
                                          && s_HEADS_DATA[981].ToIntDef(0) != 5
                                          && s_HEADS_DATA[996].ToIntDef(0) != 5){

      s_Text = "----";
   } else {
      s_Text = "1";
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(28, 12, s_Text);
   //Excel_Cell_DataSet(26, 12, s_Text);
   //*********************
   G_Log_Text = "�p���FSTRAINER        �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FDUPLEX GAUGE
   //****************************
   i_Value=0;
   //2015.04.28 ���ʕύX
   //if (s_HEADS_DATA[901].ToIntDef(0) == 16 || s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[916].ToIntDef(0) == 16 || s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[961].ToIntDef(0) == 16 || s_HEADS_DATA[961].ToIntDef(0) == 17 )  i_Value++;
   //if (s_HEADS_DATA[976].ToIntDef(0) == 16 || s_HEADS_DATA[976].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[901].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 16 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[916].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[931].ToIntDef(0) == 17 )  i_Value++;
   if (s_HEADS_DATA[946].ToIntDef(0) == 17 )  i_Value++;
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_S
   if (s_HEADS_DATA[901].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 21 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[901].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[916].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[931].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   if (s_HEADS_DATA[946].ToIntDef(0) == 22 )  i_Value = i_Value + 2;
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_E
   i_Value = i_Value / 2;
   //*******************
   if (i_Value == 0 ) {
      s_Text = "----";
   } else {
      s_Text = AnsiString(i_Value);
   }
   //2004.04.26 ̫�ϯĒ���
   Excel_Cell_DataSet(29, 12, s_Text);
   //Excel_Cell_DataSet(27, 12, s_Text);
   //*********************
   G_Log_Text = "�p���FDUPLEX GAUGE           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.02.23 �v�����ݒ�ɂ��ϕ�����Ή��i�p���j
   s_WkText1 = s_HEADS_DATA[901].TrimRight();  // S1���͌v���
   s_WkText2 = s_HEADS_DATA[916].TrimRight();  // S2���͌v���
   if( s_WkText1 == "16" || s_WkText2 == "16" ){
      Excel_Cell_DataSet(29, 7, "PRESSURE GAUGE");
      //2015.04.28 ���ʕύX
      //Excel_Cell_DataSet(29, 12, "2");
      //*********************
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_S
   } else if ( s_WkText1 == "21" || s_WkText2 == "21" ){
      Excel_Cell_DataSet(29, 7, "PRESSURE GAUGE");
   } else if ( s_WkText1 == "22" || s_WkText2 == "22" ){
      Excel_Cell_DataSet(29, 7, "PRESSURE GAUGE");
   //2020.01.08 SGP�ݸޒ[�Ǘp���͌v�ǉ��ύX_E
   } else {
      Excel_Cell_DataSet(29, 7, "DUPLEX GAUGE");
   }
   //-------------------------------------------------


   //****************************
   //***  �p���FMUNSELL No.
   //****************************
   i_RecNo = 31;
   s_Tosou_CD = s_HEADS_DATA[i_RecNo].TrimRight();
   if (s_Tosou_CD.ToIntDef(0) == 1){
      // RecNo:539 �h���F���
   //2003.09.24 �p���h���F
   //   i_RecNo = 32;
      i_RecNo = 539;
   //*********************
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();

   } else if (s_Tosou_CD.ToIntDef(0) == 3 || s_Tosou_CD.ToIntDef(0) == 5) {
      // �h���F����ð��ٓ��a���� + RecNo:539 �h���F���
//      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "J");
   //2003.09.24 �p���h���F
   //   i_RecNo = 32;
      i_RecNo = 539;
   //*********************
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   } else {
      // �h���F����ð��ٓ��a���̂��
      s_Text = Search_TOSOU_CODE(s_Tosou_CD, "E");
   }
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(32, 1, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(31, 1, s_Text);
   //Excel_Cell_DataSet(29, 1, s_Text);
   //*********************
   G_Log_Text = "�p���FMUNSELL No.     �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FAPPROVED BY
   //****************************
   i_RecNo = 1086;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(34, 3, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(33, 3, s_Text);
   //Excel_Cell_DataSet(31, 3, s_Text);
   //*********************
   G_Log_Text = "�p���FAPPROVED BY      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FREVIEWED BY
   //****************************
   i_RecNo = 1084;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(35, 3, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(34, 3, s_Text);
   //Excel_Cell_DataSet(32, 3, s_Text);
   //*********************
   G_Log_Text = "�p���FREVIEWED BY      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //****************************
   //***  �p���FPREPARED BY
   //****************************
   //2017.04.17 ���p�S���ҕύX
   //i_RecNo = 1080;
   i_RecNo = 1272;
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(36, 3, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(35, 3, s_Text);
   //Excel_Cell_DataSet(33, 3, s_Text);
   //*********************
   G_Log_Text = "�p���FPREPARED BY      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F���t1
   //***************************
   //2005.02.23 �p�����t�C��
   //i_RecNo = 1087;
   i_RecNo = 1056;
   //***********************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(34, 5, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(33, 5, s_Text);
   //Excel_Cell_DataSet(31, 5, s_Text);
   //*********************
   G_Log_Text = "�p���F���t�P           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F���t2
   //***************************
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(35, 5, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(34, 5, s_Text);
   //Excel_Cell_DataSet(32, 5, s_Text);
   //*********************
   G_Log_Text = "�p���F���t�Q           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F���t3
   //***************************
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(36, 5, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(35, 5, s_Text);
   //Excel_Cell_DataSet(33, 5, s_Text);
   //*********************
   G_Log_Text = "�p���F���t�R           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���FHEADS VER
   //***************************
   //2015.02.19 ̫�ϯĒ���
   Excel_Cell_DataSet(39, 1, s_Text);
   //2004.04.26 ̫�ϯĒ���
   //Excel_Cell_DataSet(38, 1, s_Text);
   //Excel_Cell_DataSet(36, 1, s_Text);
   //*********************
   G_Log_Text = "�p���FHEADS VER        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //-----------------------------------
   //2004.11.15 [���ޔԍ�]�o�͒ǉ�

   //***************************
   //***  �p���F���ޔԍ�
   //***************************
   if(pcHeadsData->GetMirrorFlag()){
      s_Text = "SHEET No.6";     // �~���[�d�l
   }else{
      s_Text = "SHEET No.4";     // �ʏ�d�l
   }
   //2015.02.19 ̫�ϯĒ���
   //Excel_Cell_DataSet(35, 15, s_Text);
   Excel_Cell_DataSet(36, 15, s_Text);
   G_Log_Text = "�p���FSHEET NO         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2007.11.15
   //-----------------------------------

   //2008.04.14 RX-00M[����ĥ���p��׼��ް����ڰĕs�v]�o�͒ǉ�


   //-------------------------------------------------
   // OGI 2015.02.23 �v�����ݒ�ɂ��ϕ�����Ή��i�p���j
   //***************************
   //***  �p���F���`�F�b�g����
   //***************************
//   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
//    s_Text = "----";
//    Excel_Cell_DataSet(23, 4, s_Text);
//   }

   // ���`�F�b�g�X�p�i�̗L�����o�͕�����ύX����
   // s_HEADS_DATA[262]
   //   0   �F�X�p�i�L�A���p�p��L
   //   1   �F�X�p�i���A���p�p�
   //   2   �F�X�p�i�L�A���p�p��L
   //   3   �F�X�p�i���A���p�p�
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
        // RX-00�̏ꍇ
        s_Text = "----";
        Excel_Cell_DataSet(23, 4, s_Text);
   }else{
        // ���`�F�b�g�L�薳������
        if (s_HEADS_DATA[262].TrimRight() == "0" || s_HEADS_DATA[262].TrimRight() == "2") {
          Excel_Cell_DataSet(23, 4, "1");
        } else {
          Excel_Cell_DataSet(23, 4, "----");
        }
   }

   G_Log_Text = "�p���F���`�F�b�g����   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //-------------------------------------------------


   //***************************
   //***  �p���F���p�u���V����
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
   }
   G_Log_Text = "�p���F���p�u���V����   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F�Q�[�W�v���[�g����
   //***************************
   if (s_HEADS_DATA[823].ToIntDef(0) == 8 && s_HEADS_DATA[1011].TrimRight().ToIntDef(0) == 0) {   // RX-00
      s_Text = "----";
      Excel_Cell_DataSet(25, 4, s_Text);
   }
   G_Log_Text = "�p���F�Q�[�W�v���[�g����   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //-------------------------------------------------
   // OGI 2015.03.04 �v�����ݒ�ɂ��ϕ�����Ή�
   //***************************
   //***  �p���F���p�p��̏ꍇ�́A���p�u���V�A�Q�[�W�v���[�g�A�`�F�[���̐��ʂ�0�ɂ���
   //***************************
   if (s_HEADS_DATA[262].TrimRight() == "1" || s_HEADS_DATA[262].TrimRight() == "2") {
      s_Text = "----";
      Excel_Cell_DataSet(24, 4, s_Text);
      Excel_Cell_DataSet(25, 4, s_Text);
      Excel_Cell_DataSet(26, 4, s_Text);
   }
   //-------------------------------------------------


   //**********************************************************

   //******************************
   //***  Excel Book Close
   //******************************
   // ������Ȃ��ق��̃V�[�g���o�c�e�ݒ�V�[�g�ɋL�q����
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�o�c�e�ݒ�");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�o�c�e�ݒ�"));
   if (s_HEADS_DATA[1].TrimRight() == "0") {
     Excel_Cell_DataSet(4, 2, "�p��");
   } else {
     Excel_Cell_DataSet(4, 2, "�a��");
   }
   // �}�N�����s
   // 2012.11.27 PDF�Ēǉ�
   //PDF�t�@�C���o�͎��
   //exApplication.Exec(Function("Run") << "Gsub_PDFOut");

   AnsiString s_China;           // ����敪("0":���{��A"1":������)
   i_RecNo = 1107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "CHINA" ) {
      s_China = "1";                                                 // ������
   // 2013.04.03 �����ǉ�
      // ��������
   } else if ( s_Text == "KYOGEN") {
      s_China = "1";                                                 // ����
   // *******************
   } else {
      s_China = "0";                                                 // ���{��
   }

   if ( s_China == "0" ) {
     exApplication.OleFunction("Run" , OleVariant("Gsub_PDFOut"));
   }

   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.OlePropertySet("DisplayAlerts" , OleVariant("false"));
   // ******************
   //Windows 2008Server �ڐA�Ή�
   exApplication.OlePropertySet("DisplayAlerts", false);
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���p�v�ڕ\�x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}


