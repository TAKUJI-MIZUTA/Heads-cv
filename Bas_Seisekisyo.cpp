//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�������я�  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.05.31
//�@ �X �V ���F2002.09.09 2007.03
//             2007.10.10 �װ�d�l�Ή�
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include <utilcls.h>    // Excel�N���pͯ�
#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_Seisekisyo.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F �������я��@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Seisekisyo(HeadsData* pcHeadsData)
{
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

   AnsiString s_Tmplate_Pass;       // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;     // �쐬̧�ٖ�
   AnsiString s_Text;               // �ėp������
   // 2020.06.18 ��n�t���O�ǉ�_S
   AnsiString s_Text2;               // �ėp������
   // 2020.06.18 ��n�t���O�ǉ�_E
   AnsiString s_S1Nossle;           // S1ɽ�َ��
   AnsiString s_S2Nossle;           // S2ɽ�َ��
   AnsiString s_S3Nossle;           // S3ɽ�َ��
   AnsiString s_S4Nossle;           // S3ɽ�َ��
   AnsiString s_E1Nossle;           // E1ɽ�َ��
   AnsiString s_E2Nossle;           // E2ɽ�َ��
   AnsiString s_E3Nossle;           // E3ɽ�َ��
   AnsiString s_E4Nossle;           // E4ɽ�َ��

   AnsiString s_Nossle_Nm1;         // ɽ�ٌp�薼�a1(�e ����ɽ�ٍ쐬�p)
   AnsiString s_Nossle_Nm2;         // ɽ�ٌp�薼�a2(�e ����ɽ�ٍ쐬�p)
   AnsiString s_Nsl_Tugite;         // ɽ�ٌp��     (�e ����ɽ�ٍ쐬�p)

   int        i_RecNo;              // HEADS�ް� ں���No
   AnsiString s_SQL;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   AnsiString s_SpecCode;
   // 2011.02.09 ������Ή�
   AnsiString s_China;           // ����敪("0":���{��A"1":������)
   // *********************
   // 2016.06.08 �������я����p
   int i_PosM;                   // ���p�t���O ("0":���p�ȊO�A"1"�ȏ�:���p)
   // *************************
   // 2017.06.28 �΍s���敪�ǉ�_S
   bool bSYAKOU = false;
   // 2017.06.28 �΍s���敪�ǉ�_E

   // 2022.04.22 �ψ��ێ����Ԏw��ǉ�_S
   int w_count1;
   int w_count2;
   int w_count3;
   int w_count4;
   int w_count5;
   int w_count6;
   int w_count7;
   // 2022.04.22 �ψ��ێ����Ԏw��ǉ�

   //-----------------------------------
   // 2007.10.10 �װ�d�l�Ή�

   // �װ�d�l�̏ꍇ�AA�d�l�AB�d�l�ȊO�͏����s�v(����I��)
   if(pcHeadsData->GetMirrorFlag()){
       if(pcHeadsData->GetSpecType() == DEF_SPEC_TYPE_NONE) return(true);
   }

   // HEADS÷���ް���AnsiString�^�z��Ɏ擾
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);

   // �d�l���ߕ�����擾(̧�ٖ��Ŏg�p)
   s_SpecCode = pcHeadsData->GetSpecCode();

   // 2011.02.09 ������Ή�
   //s_Tmplate_Pass = G_Template_Dir + "�������я�.xls";                  // ����ڰ�̧��
   i_RecNo = 851;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   // 2016.06.08 �������я����p
   // 2016.08.02 �����C��
   i_PosM = 0;
   // *******************
   i_RecNo = 36;
   i_PosM = s_HEADS_DATA[i_RecNo].Pos("M");
   // *************************
   // 2020.06.18 ��n�t���O�ǉ�_S
   i_RecNo = 1107;
   s_Text2 = s_HEADS_DATA[i_RecNo].TrimRight();
   //if ( s_Text == "1" ) {
   if ( s_Text == "1" || s_Text2 == "CHINA" ) {
   // 2020.06.18 ��n�t���O�ǉ�_E
      s_China = "1";                                                 // ������
      // 2016.06.08 �������я����p
      //s_Tmplate_Pass = G_Template_Dir + "�������я�C.xls";           // ����ڰ�̧��
      // 2016.08.02 �����C��
      //if (i_PosM != 0){
      if (i_PosM >= 1){
      // *******************
         s_Tmplate_Pass = G_Template_Dir + "�������я����pC.xls";    // ����ڰ�̧��
      } else {
         s_Tmplate_Pass = G_Template_Dir + "�������я�C.xls";        // ����ڰ�̧��
      }
      // *************************
   } else {
      s_China = "0";                                                 // ���{��
      // 2016.06.08 �������я����p
      //s_Tmplate_Pass = G_Template_Dir + "�������я�.xls";            // ����ڰ�̧��
      // 2016.08.02 �����C��
      //if (i_PosM != 0){
	  if (i_PosM >= 1){
      // *******************
         s_Tmplate_Pass = G_Template_Dir + "�������я����p.xls";    // ����ڰ�̧��
      } else {
         s_Tmplate_Pass = G_Template_Dir + "�������я�.xls";        // ����ڰ�̧��
      }
      // *************************
   }
   // *********************
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + G_SAKUBAN + s_SpecCode + ".xls"; // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "RPT" + GetFileSeizouBangou(G_SAKUBAN) + s_SpecCode + ".xls"; // ��߰��̧��

   // 2007.10.10
   //-----------------------------------

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w�������я��x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "�쐬�t�@�C�����F    �w" + s_MakeExcel_Pass + "�x";
   Write_Log(G_Log_Text);

// --> 2007.03
   if( bCopySeisekishoFlag )
   {
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
   }
// <-- 2007.03
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
   //***                          �������я�(�a��)�@�f�[�^�Z�b�g                                 ***
   //***                                                                                         ***
   //***********************************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�a��");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�a��"));

   //***********************
   //***  �a���F�䒍����
   //***********************
   // �wA001�x
   i_RecNo = 1063;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(3, 4, s_Text);
   Excel_Cell_DataSet(3, 3, s_Text);
   //**********
   G_Log_Text = "�a���F�䒍����             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  �a���F�[����
   //***********************
   // �wA002�x
   i_RecNo = 1065;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(5, 4, s_Text);
   Excel_Cell_DataSet(5, 3, s_Text);
   //**********
   G_Log_Text = "�a���F�[����               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  �a���F�^��
   //***********************
   // �wA003�x
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      // 2011.02.09 ������Ή�
      //s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
      if ( s_China == "0" ) {
          s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
      } else {
          s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      }
      //**********************
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   //2004.02.16
   //Excel_Cell_DataSet(7, 4, s_Text);
   Excel_Cell_DataSet(7, 3, s_Text);
   //**********
   G_Log_Text = "�a���F�^��                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  �a���F�@��ԍ�
   //***********************
   // �wA004�x
   i_RecNo = 27;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(3, 11, ZenToHan(s_Text));
   //Excel_Cell_DataSet(3, 12, ZenToHan(s_Text));
   //2004.10.01
   int w_count = s_Text.AnsiPos("�`");

   if (w_count){
      s_Text = ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).SetLength(w_count-1)) + "�`" + ZenToHan((s_HEADS_DATA[i_RecNo].TrimRight()).Delete(1,w_count+1));
   }else{
      s_Text = ZenToHan(s_HEADS_DATA[i_RecNo].TrimRight());
   }

   Excel_Cell_DataSet(3, 12, s_Text);
   //**********
   G_Log_Text = "�a���F�@��ԍ�             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  �a���F�����ԍ�
   //***********************
   // �wA005�x
   //------------------------------------
   // 2007.10.15 �d�l���ߕ�����ǉ�
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   
   s_Text = s_Text + pcHeadsData->GetSpecCode();
   // 2007.10.15
   //------------------------------------

   //2004.02.16
   //Excel_Cell_DataSet(5, 11, s_Text);
   Excel_Cell_DataSet(5, 12, s_Text);
   //**********
   G_Log_Text = "�a���F�����ԍ�            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  �a���F�䐔
   //***********************
   // �wA006�x
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      // 2016.06.08 �������я����p
      ////2004.02.16
      ////Excel_Cell_DataSet(7, 11, s_Text);
      //Excel_Cell_DataSet(7, 12, s_Text);
      ////**********
      // 2016.08.02 �����C��
      //if (i_PosM != 0){
      if (i_PosM >= 1){
      // *******************
         Excel_Cell_DataSet(7, 10, s_Text);
      } else {
         Excel_Cell_DataSet(7, 12, s_Text);
      }
      // *************************
      G_Log_Text = "�a���F�䐔                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�a���F�䐔                RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   // 2016.06.08 �������я����p
   //***********************
   //***  �a���F�D��
   //***********************
   // 2016.08.02 �����C��
   //if (i_PosM != 0){
   if (i_PosM >= 1){
   // *******************
      i_RecNo = 270;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(7, 12, s_Text);
      G_Log_Text = "�a���F�D��                 RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   // *************************

   //2019/05/17 ���p�Z�p�ǉ�
   if (!bIsNotKatashiki) {
   // 2017.06.28 �΍s���敪�ǉ�_S
   bSYAKOU = false;
   // 2022.04.21 CX-01D�ǉ�_S
   if ( G_KS_Syurui == "�b�w" && G_KS_Model == "�O�P�c" ) {
       bSYAKOU = true;
   }
   // 2022.04.21 CX-01D�ǉ�_E
   if ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) {
       bSYAKOU = true;
   }
   if ( G_KS_Syurui == "�k�w" && G_KS_Model == "�X�O�c" ) {
       bSYAKOU = true;
   }
   if ( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) {
       bSYAKOU = true;
   }
   // 2022.04.21 YX-83�ǉ�_S
   if ( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�R" ) {
       bSYAKOU = true;
   }
   // 2022.04.21 YX-83�ǉ�_E
   // 2017.06.28 �΍s���敪�ǉ�_E

   //***************************
   //***  �a���FB���t�̖�
   //***************************
   // �wA007�x
   i_RecNo = 137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
      i_RecNo = 76;     // �`���̖�
   } else {
      i_RecNo = 45;     // �a���̖�
   }
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(11, 5, s_Text);
   G_Log_Text = "�a���FB���t�̖�            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���FB���݌v����
   //***************************
   // �wA008�x
   i_RecNo = 825;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        // 2011.02.09 ������Ή�
        //s_Text = "��C�J��";
        if ( s_China == "0" ) {
            s_Text = "��C�J��";
        } else {
            s_Text = "�埃/U+24320/U+��";
        }
        //**********************
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(12, 5, s_Text);
   G_Log_Text = "�a���FB���݌v����          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FB������ɽ��
   //*******************************
   // �wA009�x,�wA010�x
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ɽ�َ��
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   if (s_S1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1152].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1153].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1168].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1169].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��
      } else if (s_E4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   // ����ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(13, 5, s_Text);
   G_Log_Text = "�a���FB������ɽ�ُ�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(14, 5, s_Text);
   G_Log_Text = "�a���FB������ɽ�ى�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FB���o��ɽ��
   //*******************************
   // �wA011�x,�wA012�x
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ɽ�َ��
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   if (s_S1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1152].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1153].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1168].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1169].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��
      } else if (s_E4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   // �o��ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(15, 5, s_Text);
   G_Log_Text = "�a���FB���o��ɽ�ُ�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(16, 5, s_Text);
   G_Log_Text = "�a���FB���o��ɽ�ى�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FB���޽��čގ�
   //*******************************
   // �wA013�x
   i_RecNo = 835;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX�p�޽��čގ��ǉ�
   if ( G_KS_Syurui == "�v�w" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // �n�ڑ��͍�����
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂a��
            // 2011.02.09 ������Ή�
            //s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            if ( s_China == "0" ) {
                s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            } else {
                s_Text = "�o�ځ{" + s_Text + "(�n�|/U+29615/U+)";
            }
            //**********************
         } else {                                               // AB���]
            // �n�ڑ��͂`��
         }
      } else {                                               // �n�ڑ��͒ቷ��
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂`��
         } else {                                               // AB���]
            // �n�ڑ��͂a��
            // 2011.02.09 ������Ή�
            //s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            if ( s_China == "0" ) {
                s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            } else {
                s_Text = "�o�ځ{" + s_Text + "(�n�|/U+29615/U+)";
            }
            //**********************
         }
      }
   }

   Excel_Cell_DataSet(18, 5, s_Text);
   G_Log_Text = "�a���FB���޽��čގ�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FA���t�̖�
   //*******************************
   // �wA014�x
   i_RecNo = 137;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
      i_RecNo = 45;     // �a���̖�
   } else {
      i_RecNo = 76;     // �`���̖�
   }
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(11, 10, s_Text);
   G_Log_Text = "�a���FA���t�̖�            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FA���t�̈���
   //*******************************
   // �wA015�x
   i_RecNo = 830;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        // 2011.02.09 ������Ή�
        //s_Text = "��C�J��";
        if ( s_China == "0" ) {
            s_Text = "��C�J��";
        } else {
            s_Text = "�埃/U+24320/U+��";
        }
        //**********************
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(12, 10, s_Text);
   G_Log_Text = "�a���FA���t�̈���          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FA������ɽ��
   //*******************************
   // �wA016�x,�wA017�x
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ɽ�َ��
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ɽ�َ��
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ɽ�َ��

   if (s_S2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1156].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1157].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1172].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1173].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��
      } else if (s_E3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   //����ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(13, 10, s_Text);
   G_Log_Text = "�a���FA������ɽ�ُ�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(14, 10, s_Text);
   G_Log_Text = "�a���FA������ɽ�ى�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FA���o��ɽ��
   //*******************************
   // �wA018�x,�wA019�x
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ɽ�َ��
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ɽ�َ��
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ɽ�َ��

   if (s_S2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1156].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1157].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1164].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1165].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1172].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1173].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1180].TrimRight();     // ɽ�ٌp�薼�a1
      s_Nossle_Nm2 = s_HEADS_DATA[1181].TrimRight();     // ɽ�ٌp�薼�a2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1160].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1161].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��
      } else if (s_E3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1176].TrimRight();     // ɽ�ٌp�薼�a1
         s_Nossle_Nm2 = s_HEADS_DATA[1177].TrimRight();     // ɽ�ٌp�薼�a2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   // �o��ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(15, 10, s_Text);
   G_Log_Text = "�a���FA���o��ɽ�ُ�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(16, 10, s_Text);
   G_Log_Text = "�a���FA���o��ɽ�ى�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���FA���޽��čގ�
   //*******************************
   // �wA020�x
   i_RecNo = 836;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX�p�޽��čގ��ǉ�
   if ( G_KS_Syurui == "�v�w" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // �n�ڑ��͍�����
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂a��
         } else {                                               // AB���]
            // �n�ڑ��͂`��
            // 2011.02.09 ������Ή�
            //s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            if ( s_China == "0" ) {
                s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            } else {
                s_Text = "�o�ځ{" + s_Text + "(�n�|/U+29615/U+)";
            }
            //**********************
         }
      } else {                                               // �n�ڑ��͒ቷ��
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂`��
            // 2011.02.09 ������Ή�
            //s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            if ( s_China == "0" ) {
                s_Text = "�n�ځ{" + s_Text + "�i�n�|�����O�j";
            } else {
                s_Text = "�o�ځ{" + s_Text + "(�n�|/U+29615/U+)";
            }
            //**********************
         } else {                                               // AB���]
            // �n�ڑ��͂a��
         }
      }
   }
   //*****************************

   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "�a���FA���޽��čގ�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���F��ڰčގ�
   //*******************************
   // �wA021�x
   i_RecNo = 43;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   Excel_Cell_DataSet(17, 4, s_Text);
   G_Log_Text = "�a���F��ڰčގ�           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  �a���F��������B��(����)
   //********************************
   // �wA022�x
   i_RecNo = 827;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 �����ύX_S
      //Excel_Cell_DataSet(32, 7, s_Text);
      Excel_Cell_DataSet(36, 7, s_Text);
      // 2019.02.22 �����ύX_E
      G_Log_Text = "�a���F��������B��(����)   RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�a���F��������B��(����)   �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //********************************
   //***  �a���F��������B��(��)
   //********************************
   // �wA023�x
   i_RecNo = 826;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 �����ύX_S
      //Excel_Cell_DataSet(33, 7, s_Text);
      Excel_Cell_DataSet(37, 7, s_Text);
      // 2019.02.22 �����ύX_E
      G_Log_Text = "�a���F��������B��(��)         RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�a���F��������B��(��)  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //********************************
   //***  �a���F��������A��(����)
   //********************************
   // �wA024�x
   i_RecNo = 832;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 �����ύX_S
      //Excel_Cell_DataSet(35, 7, s_Text);
      Excel_Cell_DataSet(39, 7, s_Text);
      // 2019.02.22 �����ύX_E
      G_Log_Text = "�a���F��������A��(����)         RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�a���F��������A��(����)   �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //********************************
   //***  �a���F��������A��(��)
   //********************************
   // �wA025�x
   i_RecNo = 831;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      // 2019.02.22 �����ύX_S
      //Excel_Cell_DataSet(36, 7, s_Text);
      Excel_Cell_DataSet(40, 7, s_Text);
      // 2019.02.22 �����ύX_E
      G_Log_Text = "�a���F��������A��(��)         RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�a���F��������A��(��)  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //*******************************
   //***  �֖��^�E������
   //*******************************
   // �wA026�x
//2003.09.09 �֖��^�E������
   if (s_HEADS_DATA[155].Trim().ToIntDef(0) == 1){
//      s_Text = "�֖�����";
      // 2019.02.22 �����ύX_S
      //// 2011.02.09 ������Ή�
      ////s_Text = "�S�D�֖������i";
      //if ( s_China == "0" ) {
      //    s_Text = "�S�D�֖������i";
      //} else {
      //    s_Text = "�S�D�֖�/U+22788/U+���i";
      //}
      ////**********************
      if ( s_China == "0" ) {
          s_Text = "�T�D�֖������i";
      } else {
          s_Text = "�T�D�֖�/U+22788/U+���i";
      }
      // 2019.02.22 �����ύX_E
   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) == 1) {
//      s_Text = "�E������";
      // 2019.02.22 �����ύX_S
      //// 2011.02.09 ������Ή�
      ////s_Text = "�S�D�E�������i";
      //if ( s_China == "0" ) {
      //    s_Text = "�S�D�E�������i";
      //} else {
      //    s_Text = "�S�D�E��/U+22788/U+���i";
      //}
      ////**********************
       if ( s_China == "0" ) {
          s_Text = "�T�D�E�������i";
      } else {
          s_Text = "�T�D�E��/U+22788/U+���i";
      }
      // 2019.02.22 �����ύX_E
   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) != 1
                                                        && s_HEADS_DATA[157].Trim().ToIntDef(0) == 1) {
//      s_Text = "���E������";
      // 2019.02.22 �����ύX_S
      //// 2011.02.09 ������Ή�
      ////s_Text = "�S�D���E�������i";
      //if ( s_China == "0" ) {
      //    s_Text = "�S�D���E�������i";
      //} else {
      //    s_Text = "�S�D�y�E��/U+22788/U+���i";
      //}
      ////**********************
      if ( s_China == "0" ) {
          s_Text = "�T�D���E�������i";
      } else {
          s_Text = "�T�D�y�E��/U+22788/U+���i";
      }
      // 2019.02.22 �����ύX_E
   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) != 1
                                                        && s_HEADS_DATA[157].Trim().ToIntDef(0) != 1) {
      s_Text = "";
//      Excel_Cell_DataSet(37, 12, "");
//      Excel_Cell_DataSet(37, 13, "");
      // 2019.02.22 �����ύX_S
      ////2004.02.16
      ////Excel_Cell_DataSet(37, 12, "");
      ////Excel_Cell_DataSet(37, 13, "");
      //Excel_Cell_DataSet(37, 13, "");
      //Excel_Cell_DataSet(37, 14, "");
      ////**********
      Excel_Cell_DataSet(41, 13, "");
      Excel_Cell_DataSet(41, 14, "");
      // 2019.02.22 �����ύX_E
   }
//   Excel_Cell_DataSet(37, 3, s_Text);
//   Excel_Cell_DataSet(37, 7, s_Text);
   // 2019.02.22 �����ύX_S
   //Excel_Cell_DataSet(37, 2, s_Text);
   Excel_Cell_DataSet(41, 2, s_Text);
   // 2019.02.22 �����ύX_E
//*************************
   G_Log_Text = "�a���F�֖��^�E������      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   }

   //*******************************
   //***  �a���F�c�ƒS����
   //*******************************
   // �wA027�x
   i_RecNo = 3;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //2004.02.16
   //Excel_Cell_DataSet(45, 11, s_Text);
   Excel_Cell_DataSet(45, 12, s_Text);
   //**********
   G_Log_Text = "�a���F�c�ƒS����           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // �wA028�x
   i_RecNo = 302;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "S";
   // 2017.10.06 �Z�[���Ή�_S
   } else if (s_HEADS_DATA[i_RecNo].TrimRight() == "2") {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "T";
   // 2017.10.06 �Z�[���Ή�_E
   } else {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   //2004.02.16
   //Excel_Cell_DataSet(51, 12, s_Text);
   Excel_Cell_DataSet(51, 13, s_Text);
   //**********
   G_Log_Text = "�a���FHEADS VER           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  �a���F�ψ��ێ�����
   //*******************************
   // �wA029�x,�wA030�x
   if (Tokki_Code("2C", s_HEADS_DATA, 1500) == true) {
      // �ψ��ێ����ԕW��
      s_Text = "20���ԕێ�";
      s_Text2 = "�ێ�20��/U+38047/U+";
   }
   else if (Tokki_Code("2D", s_HEADS_DATA, 1500) == true) {
      // �ψ��ێ�����30��
      s_Text = "30���ԕێ�";
      s_Text2 = "�ێ�30��/U+38047/U+";
   }
   else if (Tokki_Code("2E", s_HEADS_DATA, 1500) == true) {
      // �ψ��ێ����Ԏw�肠��
      s_Text = Tokki_Zen("2E", s_HEADS_DATA, 1500);
      w_count1 = s_Text.AnsiPos("�F");
      w_count2 = s_Text.AnsiPos(":");

      if (w_count1){
         // "�F"�����݁@
         s_Text = s_Text.Delete(1,w_count1+1);
      } else if (w_count2){
         // ":"������
         s_Text = s_Text.Delete(1,w_count2);
      } else {
         // "�F",":"�����݂��Ȃ�
         s_Text = "";
      }

      w_count1 = s_Text.AnsiPos("��");
      w_count2 = s_Text.AnsiPos("��");
      w_count3 = s_Text.AnsiPos("m");
      w_count4 = s_Text.AnsiPos("M");
      w_count5 = s_Text.AnsiPos("h");
      w_count6 = s_Text.AnsiPos("H");
      w_count7 = s_Text.AnsiPos("��");

      if (w_count1){
         // "��"�����݁@
         s_Text = s_Text.SetLength(w_count1-1);
      } else if (w_count2){
         // "��"������
         s_Text = "";
      } else if (w_count3){
         // "m"�����݁@
         s_Text = s_Text.SetLength(w_count3-1);
      } else if (w_count4){
         // "M"������
         s_Text = s_Text.SetLength(w_count4-1);
      } else if (w_count5){
         // "h"������
         s_Text = "";
      } else if (w_count6){
         // "H"������
         s_Text = "";
      } else if (w_count7){
         // "��"������
         s_Text = "";
      } else if (s_Text == ""){
          // ���L�����݂��Ȃ�
      } else {
          // "��","m","M"�����݂��Ȃ�
      }

      if (s_Text == ""){
         // ���L�����݂��Ȃ�
         s_Text = "�ێ����ԓ��̓G���[";
         s_Text2 = "ERROR";
      } else {
         s_Text = s_Text.TrimRight();
         s_Text = ZenToHan(s_Text);
         s_Text2 = "�ێ�" + s_Text +"��/U+38047/U+";
         s_Text = s_Text + "���ԕێ�";
      }

   } else {
      // �ψ��ێ����Ԏw��Ȃ�
      s_Text = "20���ԕێ�";
      s_Text2 = "�ێ�20��/U+38047/U+";
   }

   if ( s_China == "0" ) {
      Excel_Cell_DataSet(36, 14, s_Text);
      Excel_Cell_DataSet(39, 14, s_Text);
      G_Log_Text = "�a���F�ψ��ێ�����        �w" + s_Text + "�x���Z�b�g�B";
   } else {
      Excel_Cell_DataSet(36, 14, s_Text2);
      Excel_Cell_DataSet(39, 14, s_Text2);
      G_Log_Text = "�����F�ψ��ێ�����        �w" + s_Text2 + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);
   // 2022.04.12 �ψ��ێ����Ԏw��ǉ�_E

   //2019/05/17 ���p�Z�p�ǉ�
   if (!bIsNotKatashiki) {
   // 2020.04.08 �o�[�R�[�h�ύX_S
   // ��Ă̎w��
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�o�[�R�[�h"));

   //// 2019.02.19 �o�[�R�[�h�ǉ�_S
   ////***************************
   ////***  �a���F�o�[�R�[�h���
   ////***************************
   //s_Text = G_SAKUBAN;
   //Excel_Cell_DataSet(10, 20, s_Text);
   //G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   // 2020.04.08 �o�[�R�[�h�ύX_E
   // 2023.05.30 �o�[�R�[�h�ύX_S
   s_Text = G_SAKUBAN;
   Excel_Cell_DataSet(4, 5, s_Text);
   G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   // 2023.05.30 �o�[�R�[�h�ύX_E

   //***************************
   //***  �a���F����
   //***************************
   i_RecNo = 319;
   s_Text = s_HEADS_DATA[i_RecNo];
   // 2020.04.08 �o�[�R�[�h�ύX_S
   //Excel_Cell_DataSet(14, 20, s_Text);
   Excel_Cell_DataSet(8, 2, s_Text);
   // 2020.04.08 �o�[�R�[�h�ύX_E
   G_Log_Text = "�a���F����            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //// 2019.02.19 �o�[�R�[�h�ǉ�_E

   // 2020.04.08 �o�[�R�[�h�ύX_S
   // ��Ă̎w��
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�a��"));
   // 2020.04.08 �o�[�R�[�h�ύX_E
   }


   //***********************************************************************************************
   //***                                                                                         ***
   //***                          �������я�(�p��)�@�f�[�^�Z�b�g                                 ***
   //***                                                                                         ***
   //***********************************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�p��");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�p��"));

   //***********************
   //***  PURCHASER
   //***********************
   // �wA001�x
   i_RecNo = 1261;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 5, s_Text);
   G_Log_Text = "�p���FPURCHASER            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  Costomer
   //***********************
   // �wA002�x
   i_RecNo = 1262;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(8, 5, s_Text);
   G_Log_Text = "�p���FCOSTOMER             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  Model
   //***********************
   // �wA003�x
   i_RecNo = 33;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(12, 5, s_Text);
   G_Log_Text = "�p���FMODEL                RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  ITEM NO
   //***********************
   // �wA004�x
   i_RecNo = 27;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(10, 5, s_Text);
   G_Log_Text = "�p���FITEM NO              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  M.F.G No
   //***********************
   // �wA005�x
   //------------------------------
   // 2007.10.15 �d�l���ߕ�����ǉ�
   //2022/12/15 �����ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ǉ��Ή� Mod_E
   s_Text = s_Text + pcHeadsData->GetSpecCode();
   // 2007.10.15
   //------------------------------

   Excel_Cell_DataSet(12, 10, s_Text);
   G_Log_Text = "�p���FM.F.G No            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***********************
   //***  QUANTITY
   //***********************
   // �wA006�x
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(12, 13, s_Text);
      G_Log_Text = "�p���FQUANTITY             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�p���FQUANTITY            RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   // 2016.06.08 �������я����p
   //************************
   //***  SHIP No.
   //************************
   // 2016.08.02 �����C��
   //if (i_PosM != 0){
   if (i_PosM >= 1){
   // *******************
      i_RecNo = 270;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
      Excel_Cell_DataSet(10, 10, s_Text);
      G_Log_Text = "�p���FSHIP No.             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   // *************************

   //2019/05/17 ���p�Z�p�ǉ�
   if (!bIsNotKatashiki) {
   //***************************
   //***  B.Fluid
   //***************************
   // �wA007�x
   //2003.10.31 �f�[�^�ʒu�ύX
   //i_RecNo = 137;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
   //   i_RecNo = 1074;     // �`���̖�
   //} else {
   //   i_RecNo = 1072;     // �a���̖�
   //}
   i_RecNo = 1072;     // �a�����̖�
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 5, s_Text);
   G_Log_Text = "�p���FB.Fluid              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B.Design Pressure
   //***************************
   // �wA008�x
   i_RecNo = 825;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        s_Text = "0.00";
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(17, 5, s_Text);
   G_Log_Text = "�p���FB.Design Pressure    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  B.N&R inlet
   //*******************************
   // �wA009�x,�wA010�x
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ɽ�َ��
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   if (s_S1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1154].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1155].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1170].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1171].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "1" && s_S3Nossle != "1" && s_E1Nossle != "1" && s_E3Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S3ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E3ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��
      } else if (s_E4Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   // ����ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(18, 5, s_Text);
   G_Log_Text = "�p���FB.N&R inlet Top     �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(19, 5, s_Text);
   G_Log_Text = "�p���FB.N&R inlet Bottom  �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  B.N&R outlet
   //*******************************
   // �wA011�x,�wA012�x
   s_S1Nossle = s_HEADS_DATA[891].TrimRight();      // S1ɽ�َ��
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E1Nossle = s_HEADS_DATA[951].TrimRight();      // E1ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   if (s_S1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1154].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1155].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[893].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1170].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1171].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[953].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��

   } else if (s_S1Nossle != "2" && s_S3Nossle != "2" && s_E1Nossle != "2" && s_E3Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S3ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E3ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��
      } else if (s_E4Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   // �o��ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(20, 5, s_Text);
   G_Log_Text = "�p���FB.N&R outlet Top    �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(21, 5, s_Text);
   G_Log_Text = "�p���FB.N&R outlet Bottom �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  B.Gasket Material
   //*******************************
   // �wA013�x
   i_RecNo = 835;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX�p�޽��čގ��ǉ�
   if ( G_KS_Syurui == "�v�w" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // �n�ڑ��͍�����
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂a��
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         } else {                                               // AB���]
            // �n�ڑ��͂`��
         }
      } else {                                               // �n�ڑ��͒ቷ��
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂`��
         } else {                                               // AB���]
            // �n�ڑ��͂a��
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         }
      }
   }
   //*****************************

   Excel_Cell_DataSet(23, 5, ZenToHan(s_Text));
   G_Log_Text = "�p���FB.Gasket Material   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.Fluid
   //*******************************
   // �wA014�x
   //2003.10.31 �f�[�^�ʒu�ύX
   //i_RecNo = 137;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if ( s_Text.ToIntDef(0) == 1 ) {     // �`�a�t
   //   i_RecNo = 1072;     // �a���̖�
   //} else {
   //   i_RecNo = 1074;     // �`���̖�
   //}
   i_RecNo = 1074;     // �`�����̖�
   //*************************
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 10, s_Text);
   G_Log_Text = "�p���FA.Fluid              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.Design Pressure
   //*******************************
   // �wA015�x
   i_RecNo = 830;
   if ( s_HEADS_DATA[i_RecNo] != "" ) {
     if ( s_HEADS_DATA[i_RecNo].ToDouble() >= 999 ) {
        s_Text = "0.00";
     } else {
        s_Text = FormatFloat( "0.00", s_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   Excel_Cell_DataSet(17, 10, s_Text);
   G_Log_Text = "�p���FA.Design Pressure    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.N&R inlet
   //*******************************
   // �wA016�x,�wA017�x
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ɽ�َ��
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ɽ�َ��
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ɽ�َ��

   if (s_S2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1158].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1159].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1174].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1175].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle == "1"){
      s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "1" && s_S4Nossle != "1" && s_E2Nossle != "1" && s_E4Nossle != "1"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��
      } else if (s_E3Nossle == "1"){
         s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   //����ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(18, 10, s_Text);
   G_Log_Text = "�p���FA.N&R inlet Top     �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(19, 10, s_Text);
   G_Log_Text = "�p���FA.N&R inlet Bottom  �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.N&R outlet
   //*******************************
   // �wA018�x,�wA019�x
   s_S2Nossle = s_HEADS_DATA[906].TrimRight();      // S2ɽ�َ��
   s_S4Nossle = s_HEADS_DATA[936].TrimRight();      // S4ɽ�َ��
   s_E2Nossle = s_HEADS_DATA[966].TrimRight();      // E2ɽ�َ��
   s_E4Nossle = s_HEADS_DATA[996].TrimRight();      // E4ɽ�َ��

   if (s_S2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1158].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1159].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[908].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1166].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1167].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[938].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1174].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1175].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[968].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle == "2"){
      s_Nossle_Nm1 = s_HEADS_DATA[1182].TrimRight();     // ɽ�ٌp�薼�p1
      s_Nossle_Nm2 = s_HEADS_DATA[1183].TrimRight();     // ɽ�ٌp�薼�p2
      s_Nsl_Tugite = s_HEADS_DATA[998].TrimRight();      // ɽ�ٌp��

   } else if (s_S2Nossle != "2" && s_S4Nossle != "2" && s_E2Nossle != "2" && s_E4Nossle != "2"){
      s_Nossle_Nm1 = "";
      s_Nossle_Nm2 = "";
      s_Nsl_Tugite = "";
   }

   //2008.07.24 CX-10D�ǉ�
   s_S3Nossle = s_HEADS_DATA[921].TrimRight();      // S3ɽ�َ��
   s_E3Nossle = s_HEADS_DATA[981].TrimRight();      // E3ɽ�َ��

   // 2017.06.28 �΍s���敪�ǉ�_S
   //if(( G_KS_Syurui == "�x�w" && G_KS_Model == "�W�O" ) ||
   //   ( G_KS_Syurui == "�b�w" && G_KS_Model == "�P�O�c" ) ) {
   if ( bSYAKOU == true ) {
   // 2017.06.28 �΍s���敪�ǉ�_E
      if (s_S3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1162].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1163].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[923].TrimRight();      // ɽ�ٌp��
      } else if (s_E3Nossle == "2"){
         s_Nossle_Nm1 = s_HEADS_DATA[1178].TrimRight();     // ɽ�ٌp�薼�p1
         s_Nossle_Nm2 = s_HEADS_DATA[1179].TrimRight();     // ɽ�ٌp�薼�p2
         s_Nsl_Tugite = s_HEADS_DATA[983].TrimRight();      // ɽ�ٌp��
      }
   }
   //*********************

   // �o��ɽ�ف@�Z�b�g�l�擾
   Get_Nossle_Value(s_Nossle_Nm1, s_Nossle_Nm2, s_Nsl_Tugite);

   s_Text = P_Nossle_Top;
   Excel_Cell_DataSet(20, 10, s_Text);
   G_Log_Text = "�p���FA.N&R outlet Top    �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   s_Text = P_Nossle_Bottom;
   Excel_Cell_DataSet(21, 10, s_Text);
   G_Log_Text = "�p���FA.N&R outlet Bottom �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  A.Gasket Material
   //*******************************
   // �wA020�x
   i_RecNo = 836;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
   }

   //2007.07.02 WX�p�޽��čގ��ǉ�
   if ( G_KS_Syurui == "�v�w" ) {
      if (s_HEADS_DATA[243].TrimRight().ToIntDef(0) != 1 ) { // �n�ڑ��͍�����
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂a��
         } else {                                               // AB���]
            // �n�ڑ��͂`��
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         }
      } else {                                               // �n�ڑ��͒ቷ��
         if (s_HEADS_DATA[137].TrimRight().ToIntDef(0) != 1 ) { // AB���]�ł͂Ȃ�
            // �n�ڑ��͂`��
            s_Text = "WELDING+" + s_Text + "(O-RING)";
         } else {                                               // AB���]
            // �n�ڑ��͂a��
         }
      }
   }
   //*****************************

   Excel_Cell_DataSet(23, 10, ZenToHan(s_Text));
   G_Log_Text = "�p���FA.Gasket Material   �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*******************************
   //***  Plate Material
   //*******************************
   // �wA021�x
   i_RecNo = 43;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->ConnectionName = ODBC_DSN_MAT;
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("ENGLISH_NAME")->AsString.TrimRight();
   }

   Excel_Cell_DataSet(22, 4, ZenToHan(s_Text));
   G_Log_Text = "�p���FPlate Material      �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************
   //***  Test Pressure B.(W)
   //********************************
   // �wA022�x
   i_RecNo = 827;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(37, 6, s_Text);
      G_Log_Text = "�p���FTest Pressure B.(W)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�p���FTest Pressure B.(W)   �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //********************************
   //***  Test Pressure B.(A)
   //********************************
   // �wA023�x
   i_RecNo = 826;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(38, 6, s_Text);
      G_Log_Text = "�p���FTest Pressure B.(A)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�p���FTest Pressure B.(A)   �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //********************************
   //***  Test Pressure A.(W)
   //********************************
   // �wA024�x
   i_RecNo = 832;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(40, 6, s_Text);
      G_Log_Text = "�p���FTest Pressure A.(W)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�p���FTest Pressure A.(W)   �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //********************************
   //***  Test Pressure A.(A)
   //********************************
   // �wA025�x
   i_RecNo = 831;
   s_Text = s_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(41, 6, s_Text);
      G_Log_Text = "�p���FTest Pressure A.(A)       RecNo:" + FormatFloat("0000",i_RecNo)
                 + "�w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_ErrLog_Text = "�p���FTest Pressure A.(A)   �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************************
   //***  Oilless Treatment�^Judgement
   //***************************************
   // �wA026�x,�wA027�x
   if (s_HEADS_DATA[155].Trim().ToIntDef(0) == 1 || s_HEADS_DATA[156].Trim().ToIntDef(0) == 1
                                                 || s_HEADS_DATA[157].Trim().ToIntDef(0) == 1){

      // 2019.02.22 �����ύX_S
      //s_Text = "4.Oilless Treatment";
      //Excel_Cell_DataSet(43, 2, s_Text);
      //G_Log_Text = "�p���F4.Oilless Treatment �w" + s_Text + "�x���Z�b�g�B";
      s_Text = "5.Oilless Treatment";
      Excel_Cell_DataSet(47, 2, s_Text);
      G_Log_Text = "�p���F5.Oilless Treatment �w" + s_Text + "�x���Z�b�g�B";
      // 2019.02.22 �����ύX_E
      Write_Log(G_Log_Text);

//2003.09.09 �֖��^�E������
//      s_Text = "Judgement : GOOD";
//      s_Text = "Judgement : ";
      s_Text = "Judgement : GOOD";
//*************************
      // 2019.02.22 �����ύX_S
      //Excel_Cell_DataSet(43, 10, s_Text);
      Excel_Cell_DataSet(47, 11, s_Text);
      // 2019.02.22 �����ύX_E
      G_Log_Text = "�p���FJudgement           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else if (s_HEADS_DATA[155].Trim().ToIntDef(0) != 1 && s_HEADS_DATA[156].Trim().ToIntDef(0) != 1
                                                        && s_HEADS_DATA[157].Trim().ToIntDef(0) != 1){

      s_Text = "";
      // 2019.02.22 �����ύX_S
      //Excel_Cell_DataSet(43, 2, s_Text);
      //G_Log_Text = "�p���F4.Oilless Treatment �w" + s_Text + "�x���Z�b�g�B";
      Excel_Cell_DataSet(47, 2, s_Text);
      G_Log_Text = "�p���F5.Oilless Treatment �w" + s_Text + "�x���Z�b�g�B";
      // 2019.02.22 �����ύX_E
      Write_Log(G_Log_Text);

      s_Text = "";
      // 2019.02.22 �����ύX_S
      //Excel_Cell_DataSet(43, 10, s_Text);
      Excel_Cell_DataSet(47, 11, s_Text);
      // 2019.02.22 �����ύX_E
      G_Log_Text = "�p���FJudgement           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   }

   //***************************
   //***  HEADS VER
   //***************************
   // �wA028�x
   i_RecNo = 302;
   if (s_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "S";
   // 2017.10.06 �Z�[���Ή�_S
   } else if (s_HEADS_DATA[i_RecNo].TrimRight() == "2") {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "T";
   // 2017.10.06 �Z�[���Ή�_E
   } else {
      i_RecNo = 1267;
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   // 2019.02.22 �����ύX_S
   //Excel_Cell_DataSet(50, 12, s_Text);
   Excel_Cell_DataSet(53, 12, s_Text);
   // 2019.02.22 �����ύX_E
   G_Log_Text = "�p���FHEADS VER           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2022.04.22 �ψ��ێ����Ԏw��ǉ�_S
   //***************************
   //***  Holding time
   //***************************
   if (Tokki_Code("2C", s_HEADS_DATA, 1500) == true) {
      // �ψ��ێ����ԕW��
      s_Text = "MPa G  (by water) Holding time   20   min.";
   }
   else if (Tokki_Code("2D", s_HEADS_DATA, 1500) == true) {
      // �ψ��ێ�����30��
      s_Text = "MPa G  (by water) Holding time   30   min.";
   }
   else if (Tokki_Code("2E", s_HEADS_DATA, 1500) == true) {
      // �ψ��ێ����Ԏw�肠��
      s_Text = Tokki_Zen("2E", s_HEADS_DATA, 1500);
      w_count1 = s_Text.AnsiPos("�F");
      w_count2 = s_Text.AnsiPos(":");

      if (w_count1){
         // "�F"�����݁@
         s_Text = s_Text.Delete(1,w_count1+1);
      } else if (w_count2){
         // ":"������
         s_Text = s_Text.Delete(1,w_count2);
      } else {
         // "�F",":"�����݂��Ȃ�
         s_Text = "";
      }

      w_count1 = s_Text.AnsiPos("��");
      w_count2 = s_Text.AnsiPos("��");
      w_count3 = s_Text.AnsiPos("m");
      w_count4 = s_Text.AnsiPos("M");
      w_count5 = s_Text.AnsiPos("h");
      w_count6 = s_Text.AnsiPos("H");

      if (w_count1){
         // "��"�����݁@
         s_Text = s_Text.SetLength(w_count1-1);
      } else if (w_count2){
         // "��"������
         s_Text = "";
      } else if (w_count3){
         // "m"�����݁@
         s_Text = s_Text.SetLength(w_count3-1);
      } else if (w_count4){
         // "M"������
         s_Text = s_Text.SetLength(w_count4-1);
      } else if (w_count5){
         // "h"������
         s_Text = "";
      } else if (w_count6){
         // "H"������
         s_Text = "";
      } else if (s_Text == ""){
          // ���L�����݂��Ȃ�
      } else {
          // "��","m","M"�����݂��Ȃ�
      }

      if (s_Text == ""){
         // ���L�����݂��Ȃ�
         s_Text = "���@�ێ����ԓ��̓G���[�B";
      } else {
         s_Text = s_Text.TrimRight();
         s_Text = ZenToHan(s_Text);
         s_Text = "MPa G  (by water) Holding time   " + s_Text + "   min.";
      }

   }
   else {
      // �ψ��ێ����Ԏw��Ȃ�
      s_Text = "MPa G  (by water) Holding time   20   min.";
   }

   Excel_Cell_DataSet(37, 8, s_Text);
   Excel_Cell_DataSet(40, 8, s_Text);
   G_Log_Text = "�p���FHolding time        �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   // 2022.04.22 �ψ��ێ����Ԏw��ǉ�_E

   //2019/05/17 ���p�Z�p�ǉ�
   if (!bIsNotKatashiki) {
   // 2020.04.08 �o�[�R�[�h�ύX_S
   // ��Ă̎w��
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�o�[�R�[�h"));

   //// 2019.02.19 �o�[�R�[�h�ǉ�_S
   ////***************************
   ////***  �p���F�o�[�R�[�h���
   ////***************************
   //s_Text = G_SAKUBAN;
   //Excel_Cell_DataSet(10, 20, s_Text);
   //G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   // 2020.04.08 �o�[�R�[�h�ύX_E
   // 2023.05.30 �o�[�R�[�h�ύX_S
   s_Text = G_SAKUBAN;
   Excel_Cell_DataSet(17, 5, s_Text);
   G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   // 2023.05.30 �o�[�R�[�h�ύX_E

   //***************************
   //***  �p���F����
   //***************************
   i_RecNo = 319;
   s_Text = s_HEADS_DATA[i_RecNo];
   // 2020.04.08 �o�[�R�[�h�ύX_S
   //Excel_Cell_DataSet(14, 20, s_Text);
   Excel_Cell_DataSet(21, 2, s_Text);
   // 2020.04.08 �o�[�R�[�h�ύX_E
   G_Log_Text = "�p���F����            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //// 2019.02.19 �o�[�R�[�h�ǉ�_E

   // 2020.04.08 �o�[�R�[�h�ύX_S
   // ��Ă̎w��
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�p��"));
   // 2020.04.08 �o�[�R�[�h�ύX_E
   }


   delete wkQuery;

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
   // --> 2007.03
   // ������Ȃ��V�[�g�ǉ�
   Excel_Cell_DataSet( 5, 2, "NK-FAX");
   Excel_Cell_DataSet( 6, 2, "��-FAX");
   Excel_Cell_DataSet( 7, 2, "NK�\");
   Excel_Cell_DataSet( 8, 2, "���\");
   Excel_Cell_DataSet( 9, 2, "ABS��");
   Excel_Cell_DataSet(10, 2, "LR��");
   Excel_Cell_DataSet(11, 2, "NK��");
   Excel_Cell_DataSet(12, 2, "BV��");
   // <-- 2007.03

   // 2020.04.08 �o�[�R�[�h�ύX_S
   // ��Ă̎w��
   if (s_HEADS_DATA[1].TrimRight() == "0") {
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�a��"));
   // 2021.10.19 �ۑ������ǉ�_S
   } else if (s_HEADS_DATA[1].TrimRight() == "2") {
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�a��"));
   // 2021.10.19 �ۑ������ǉ�_E
   } else {
       exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�p��"));
   }
   // 2020.04.08 �o�[�R�[�h�ύX_E
   // 2021.10.19 �ۑ������ǉ�_S
   exWorkbook.OleProcedure("Save");  //�ۑ�
   // 2021.10.19 �ۑ������ǉ�_E

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
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w�������я��x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
void __fastcall Get_Nossle_Value(AnsiString NossleNM1, AnsiString NossleNM2, AnsiString Tugite)
{
   int i_Len;

   P_Nossle_Top = "";
   P_Nossle_Bottom = "";

   // �a���FB������ɽ�� �㕔
//   if (NossleNM1.Trim() != ""){
//      i_Len = NossleNM1.Pos("-");
//      if (i_Len != 0){
//         P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
//      } else {
//         i_Len = NossleNM1.Pos("�|");
//         if (i_Len != 0){
//            P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
//         } else {
//            P_Nossle_Top = "";
//         }
//      }
//   } else {
//      P_Nossle_Top = "";
//   }
//
//   i_Len = NossleNM1.Pos("-");
//   if (i_Len != 0){
//      P_Nossle_Bottom = NossleNM1.SubString(i_Len+1, 48);
//   } else {
//      i_Len = NossleNM1.Pos("�|");
//      if (i_Len != 0){
//         P_Nossle_Bottom = NossleNM1.SubString(i_Len+2, 48);
//      } else {
//         P_Nossle_Bottom = "";
//      }
//   }


   // �a���FB������ɽ�� ����

   // 2011.11.29 ɽ�ًK�i�ύX
   //if (NossleNM2.Trim() != ""){
   // 2013.04.03 ɽ�ًK�i�ǉ�
   //if ( Tugite.SubString(1,2) == "11" || Tugite.SubString(1,2) == "12"
   //	 || Tugite.SubString(1,2) == "13" || Tugite.SubString(1,2) == "21"
   //	 || Tugite.SubString(1,2) == "22" || Tugite.SubString(1,2) == "23" ){
   if ( Tugite.SubString(1,2) == "11" || Tugite.SubString(1,2) == "12"
     || Tugite.SubString(1,2) == "13" || Tugite.SubString(1,2) == "14"
     || Tugite.SubString(1,2) == "21" || Tugite.SubString(1,2) == "22"
     || Tugite.SubString(1,2) == "23" || Tugite.SubString(1,2) == "24"
     || Tugite.SubString(1,2) == "31" || Tugite.SubString(1,2) == "32"
     || Tugite.SubString(1,2) == "33" || Tugite.SubString(1,2) == "34" ){
   // ***********************
      P_Nossle_Top = NossleNM1;
      if (Tugite.SubString(3,1) == "4"){
         // ɽ�ٌp�薼�a2   "("���O��
         i_Len = NossleNM2.Pos("(");
         if (i_Len != 0){
            P_Nossle_Bottom = NossleNM2.SubString(1,i_Len-1);
         } else {
            i_Len = NossleNM2.Pos("�i");
            if (i_Len != 0){
               P_Nossle_Bottom = NossleNM2.SubString(1,i_Len-1);
            } else {
               P_Nossle_Bottom = NossleNM2;
            }
         }

      }

   } else if (NossleNM2.Trim() != ""){
   // ***********************
      i_Len = NossleNM1.Pos("-");
      if (i_Len != 0){
         P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
      } else {
         i_Len = NossleNM1.Pos("�|");
         if (i_Len != 0){
             P_Nossle_Top = NossleNM1.SubString(1,i_Len-1);
         } else {
             P_Nossle_Top = "";
         }
      }         // ɽ�ٌp�薼�a1   "-"���㕔
      i_Len = NossleNM1.Pos("-");
      if (i_Len != 0){
         P_Nossle_Bottom = NossleNM1.SubString(i_Len+1, 48);
      } else {
         i_Len = NossleNM1.Pos("�|");
          if (i_Len != 0){
            P_Nossle_Bottom = NossleNM1.SubString(i_Len+2, 48);
          } else {
            P_Nossle_Bottom = "";
          }
      }

      // 2011.06.20 ɽ�ًK�i�ύX
      //if (Tugite.SubString(2,1) == "4"){
      if (Tugite.SubString(3,1) == "4"){
      // ***********************
         // ɽ�ٌp�薼�a2   "("���O��
         i_Len = NossleNM2.Pos("(");
         if (i_Len != 0){
            P_Nossle_Bottom = P_Nossle_Bottom + " " + NossleNM2.SubString(1,i_Len-1);
         } else {
            i_Len = NossleNM2.Pos("�i");
            if (i_Len != 0){
               P_Nossle_Bottom = P_Nossle_Bottom + " " + NossleNM2.SubString(1,i_Len-1);
            } else {
               P_Nossle_Bottom = P_Nossle_Bottom + " " + NossleNM2;
            }
         }

      }
   //2008.03.21 RX-00 ȼލ���ɽ�ْǉ�
   // 2011.06.20 ɽ�ًK�i�ύX
   //} else if (Tugite.SubString(1,1) == "7"){
   } else if (Tugite.SubString(1,2) == "07"){
   // ***********************
      // ɽ�� �㕔
      if (NossleNM1.Trim() != ""){
          P_Nossle_Top = NossleNM1.Trim();
      } else {
          P_Nossle_Top = "";
      }
      P_Nossle_Bottom = "";
   //********************************
   } else {
      P_Nossle_Bottom = "";
   }
}


//---------------------------------------------------------------------------
// ���{��֐����F ���L�R�[�h�L���m�F
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F ���L����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2022.04.22�ǉ�
//---------------------------------------------------------------------------
bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu)
{

   bool bSearch;

   bSearch = false;

   if ( s_HEADS_DATA[451].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[454].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[457].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[460].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[463].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[466].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[469].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[472].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[475].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[478].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[481].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[484].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[487].TrimRight() == Key ) {
      bSearch = true;
   }
   if ( s_HEADS_DATA[490].TrimRight() == Key ) {
      bSearch = true;
   }

   if( bSearch ){
      return(true);
   } else {
      return(false);
   }

}


//---------------------------------------------------------------------------
// ���{��֐����F ���L���ځi�S�p�j�擾
// �T  �v      �F
// ��  ��      �F AnsiString Key   �F ���L����
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F 2022.04.22�ǉ�
//---------------------------------------------------------------------------
AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu)
{

   AnsiString s_RetText;

   s_RetText = "";

   if ( s_HEADS_DATA[451].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[453].TrimRight();
   }
   if ( s_HEADS_DATA[454].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[456].TrimRight();
   }
   if ( s_HEADS_DATA[457].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[459].TrimRight();
   }
   if ( s_HEADS_DATA[460].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[462].TrimRight();
   }
   if ( s_HEADS_DATA[463].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[465].TrimRight();
   }
   if ( s_HEADS_DATA[466].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[468].TrimRight();
   }
   if ( s_HEADS_DATA[469].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[471].TrimRight();
   }
   if ( s_HEADS_DATA[472].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[474].TrimRight();
   }
   if ( s_HEADS_DATA[475].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[477].TrimRight();
   }
   if ( s_HEADS_DATA[478].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[480].TrimRight();
   }
   if ( s_HEADS_DATA[481].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[483].TrimRight();
   }
   if ( s_HEADS_DATA[484].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[486].TrimRight();
   }
   if ( s_HEADS_DATA[487].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[489].TrimRight();
   }
   if ( s_HEADS_DATA[490].TrimRight() == Key ) {
      s_RetText = s_HEADS_DATA[492].TrimRight();
   }

   return(s_RetText);

}

