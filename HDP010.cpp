//---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�o�}�v����M�T�[�r�X
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.05.23
//�@ �X �V ���F2002.09.17 ��1 2007.02.01 ��2 2007.03
//             2007.10.09 �װ�d�l�Ή�
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>

#include <dir.h>
#include <io.h>
#include <direct.h>

#pragma hdrstop

#include <sys\stat.h>
#include <utilcls.h>          // Excel�N���pͯ�
//2009.03.03 ���͗e�틭�x�v�Z�ύX
#include <process.h>
//*******************************
#include "HDPConst.h"         // �萔��`�p
#include "HDPValue.h"         // �ϐ���`�p
#include "HDPCom.h"           // �֐���`�p

#include "DataModule.h"

#include "HDP010.h"
#include "Bas_Syonin.h"          // ���F�}�\��
#include "Bas_Mokuroku.h"        // �}�ʖژ^
#include "Bas_Seisekisyo.h"      // �������я�
#include "Bas_Funeyou.h"         // ���p�v�ڕ\
#include "Bas_Kobetusinsei.h"    // ���^���͗e�� �ʌ���\����
#include "Bas_Meisaisyo.h"       // ���^���͗e�� ���׏�
//2009.03.03 ���͗e�틭�x�v�Z�ύX
//#include "Bas_kyodokeisan.h"     // ���͗e�틭�x�v�Z��
//*******************************
#include "Bas_Youryosyo.h"
#include "Bas_Sijisyo.h"
//Add_Str 2019/02/19 Ri
#include "Bas_SijisyoA3.h"
//Add_End 2019/02/19 Ri
#include "Bas_Steam.h"



// --> ��2 �ǉ�
#include "Bas_Funeyousinsei.h"
//#include "Bas_SeisekishoDNV.h"   // ���я� DNV�K�i
//#include "Bas_SeisekishoABS.h"   // ���я� ABS�K�i
//#include "Bas_SeisekishoLR.h"    // ���я� LR�K�i
//#include "Bas_SeisekishoNK.h"    // ���я� NK�K�i
//#include "Bas_SeisekishoGL.h"    // ���я� GL�K�i
//#include "Bas_SeisekishoKR.h"    // ���я� KR�K�i
//#include "Bas_SeisekishoBV.h"    // ���я� BV�K�i
//#include "Bas_SeisekishoCCS.h"   // ���я� CCS�K�i
// <-- ��2 �ǉ�

#include "Bas_Buhinhyou.h"       // �W�����i�\(02.08.07�ǉ�)

#include "HeadsData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmHDP010 *frmHDP010;

//---------------------------------------------------------------------------
// ̫�� �ݽ�׸�
//---------------------------------------------------------------------------
__fastcall TfrmHDP010::TfrmHDP010(TComponent* Owner)
   : TForm(Owner)
{
}

//---------------------------------------------------------------------------
// ���{��֐����F �t�H�[��  �ĕ`��
// �T  �v      �F �^�X�N�o�[�@�N���A�v���P�[�V������\����
// ��  ��      �F
// �߂�l      �F �Ȃ�
// ��  �l      �F
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::FormPaint(TObject *Sender)
{
   ShowWindow(Application->Handle,SW_HIDE);
}

//---------------------------------------------------------------------------
// ���{��֐����F ��ԁ@�擾
// �T  �v      �F �N�����Ɏ擾����p�����[�^����A��Ԃ��擾����B
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Get_Sakuban(void)
{
   //*******************
   //***  ��Ԏ擾
   //*******************

   // ��Ԃ�HD_sakuban.txt����擾����悤�C��
   G_SAKUBAN = Get_Sakuban_File();
   if ( G_SAKUBAN == "" ) {
      G_ErrLog_Text = "HD_sakuban.txt����Ԃ��擾�ł��܂���ł����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   } else {
      G_HEADS_ID = G_SAKUBAN + ".txt";
   }
   // �����ײ݂���̧��ID���擾
//   G_HEADS_ID = ParamStr(1);     // HEADS�ް� ̧��ID
//   //G_HEADS_ID = "084156.txt";
//
//   // ̧��ID�����Ԃ��擾
//   int i_Pos;

//   i_Pos = G_HEADS_ID.Pos(".");
//   if (i_Pos != 0){
//      G_SAKUBAN = G_HEADS_ID.SubString(1, i_Pos-1);
//   } else {
//      G_ErrLog_Text = "����(" + G_HEADS_ID + ")�ɂ���Ԃ��擾�ł��܂���ł����I�I";
//      Write_Error_Log(G_ErrLog_Text);
//      return false;
//   }

   // ۸ލ쐬
   G_Log_Text = "�t�@�C���h�c�w" + G_HEADS_ID + "�x����A���(" + G_SAKUBAN + ")���擾���܂���";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //*************************
   //***  ��Ԏ�ށ@�擾
   //*************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0){
      P_Sakuban_Mode = "S";
   } else {
      P_Sakuban_Mode = "M";
   }

   //2019/05/17 ���p�Z�p�ǉ�
   gP_Sakuban_Mode = P_Sakuban_Mode;

   //**************************************************************
   // 02.08.07�ǉ� HEADS����̫��ޥ̧�ٌ`���ύX csv��txt
   // �����ް� "C:\HEADS_ROOT\HEADS_DATA\ESTIMATE\D00001\D00001.txt"
   // �����ް� "C:\HEADS_ROOT\HEADS_DATA\ORDER\000001\000001.txt"
   //***************************************************************
   if( P_Sakuban_Mode == "S" )
   {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
      //                 + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //                 + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
      //                 + IncludeTrailingBackslash(G_SAKUBAN);
      G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                       + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                       + IncludeTrailingBackslash(G_SAKUBAN);
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END
   }
   else if( P_Sakuban_Mode == "M" )
   {
      //�}���`�T�[�o�Ή� 2009/1/13 MOD START
      //G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HD_Root)
      //                 + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
      //                 + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
      //                 + IncludeTrailingBackslash(G_SAKUBAN);
      G_Input_Txt_Dir  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                       + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                       + IncludeTrailingBackslash(G_SAKUBAN);
      //�}���`�T�[�o�Ή� 2009/1/13 MOD END
   }

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F �E�B���h�E����������
// �T  �v      �F �\��������������
// ��  ��      �F �����l
// �߂�l      �F �Ȃ�
// ��  �l      �F ���ѐݒ�Ǎ���
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::FormCreate(TObject *Sender)
{

   // INI�ݒ�t�@�C���̓ǂݍ���
   IniFileRead();

   //*************************
   //***  ۸�̧�ف@��������
   //*************************
   if(!Log_File_SizeCheck()){
      return;
   }

   Screen->Cursor = crHourGlass;
   prvFrmLoadFlg == 0;

   // ۸ލ쐬
   G_Log_Text = "********************************************************************************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "*****                                                                      *****";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "*****                  EXCEL�f�[�^�o�̓V�X�e�����N�����܂����B             *****";
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
   //***  �G�N�Z�������@�J�n
   //***************************
   Heads_Excel_Main();
   //TEST
   //***************************
   //***  ����t�H���_�ړ�����
   //***************************
   MoveFolder_Job1();
   //TEST
   //2009.03.03 ���͗e�틭�x�v�Z�ύX
   //************************************
   //***  ���͗e�틭�x�v�Z��۸���  �J�n
   //************************************
   ////Windows 2008Server �ڐA�Ή�
   if (FileExists(G_Err_Log_FileFName) == false) {
        Aturyoku_Run();
   }
   //*******************************
   //2017.10.19 �h�����匟���[_S
   //************************************
   //***  �h�����匟���[��۸���  �J�n
   //************************************
   if (FileExists(G_Err_Log_FileFName) == false) {
        TosoJisyuKensa_Run();
   }
   //2017.10.19 �h�����匟���[_E

   Screen->Cursor = crDefault;
//   PostMessage(Handle, WM_CLOSE, 0,0);

//   Release();  //�t�H�[����j�����C�֘A�t�����Ă��郁������������܂��B

   Application->Terminate();
}

//---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���쐬�@���C�����[�`��
// �T  �v      �F
// ��  ��      �F �����l
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::FormActivate(TObject *Sender)
{
   //*********************
   //***  Excel �I��
   //*********************
   QuitExcel();

   Screen->Cursor = crDefault;
//   PostMessage(Handle, WM_CLOSE, 0,0);

//   Release();  //�t�H�[����j�����C�֘A�t�����Ă��郁������������܂��B

   Application->Terminate();

}

//---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���쐬�@���C�����[�`��
// �T  �v      �F
// ��  ��      �F �����l
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
void __fastcall TfrmHDP010::Heads_Excel_Main(void)
{
   short       fret;
   //int         i_Pos;
   AnsiString  s_FilePass;
   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   AnsiString  Work,Rev;
   //*************************************
   // 2007.10.09 �װ�d�l�Ή�(HeadsData�׽)
   bool bRet;
   HeadsData *pcHeadsData;
   int Sakuban;

   try{
      if (prvFrmLoadFlg == 0){
         prvFrmLoadFlg = 1;

         Refresh();
         Screen->Cursor = crHourGlass;

         //*************************
         //***  ��Ԏ擾
         //*************************
         if(!Get_Sakuban()){
            return;
         }

         //**********************************
         //***  �K�i����,�ƊE,�h���F  �Ǎ�
         //**********************************
         if (!Read_CodeMasta_Data()){
            return;
         }

         //*************************
         //***  �ް��ް��ڑ�
         //*************************
         if (!Connect_DataBase()){
            return;
         }

         //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
         //*************************************
         //***  HEADS�ް���Rev�̂݁@�Ǎ�
         //*************************************
         //TEST
         //s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + "H.txt";
         s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + ".txt";
         //TEST
         if (!Read_HEADS_Data_Rev(s_FilePass)){
            G_ErrLog_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
            Write_Error_Log(G_ErrLog_Text);
            return;
         } else {
            G_Log_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x��ǂݍ��݂܂����B";
            Write_Log(G_Log_Text);
         }
         //*************************************

         // 2007.10.09 �װ�d�l�Ή� �ő�3̧�ٓǍ�
         //*************************************
         //***  HEADS�ް���Rev�̂݁@�Ǎ�
         //***  (���d�l�A�װA�d�l�A�װB�d�l)
         //*************************************
         m_pcHeadsData = new HeadsData[3];
         bRet = m_pcHeadsData[0].ReadAllData(s_FilePass.c_str());

         if (!bRet){
            G_ErrLog_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x(���d�l)�̂n�o�d�m�Ɏ��s���܂����I�I";
            Write_Error_Log(G_ErrLog_Text);
            return;
         } else {
            G_Log_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x(���d�l)��ǂݍ��݂܂����B";
            Write_Log(G_Log_Text);
         }

         // �װ�d�l�̏ꍇ
         if(m_pcHeadsData[0].GetMirrorFlag()){
            // �װA�d�l��HEADS�ް� �Ǎ�
            s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + "A" + ".txt";
            bRet = m_pcHeadsData[1].ReadAllData(s_FilePass.c_str());

            if (!bRet){
                G_ErrLog_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x(�װA�d�l)�̂n�o�d�m�Ɏ��s���܂����I�I";
                Write_Error_Log(G_ErrLog_Text);
                return;
            } else {
                G_Log_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x(�װA�d�l)��ǂݍ��݂܂����B";
                Write_Log(G_Log_Text);
            }

            // �װB�d�l��HEADS�ް� �Ǎ�
            s_FilePass = G_Input_Txt_Dir + G_SAKUBAN + "B" + ".txt";
            bRet = m_pcHeadsData[2].ReadAllData(s_FilePass.c_str());

            if (!bRet){
                G_ErrLog_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x(�װB�d�l)�̂n�o�d�m�Ɏ��s���܂����I�I";
                Write_Error_Log(G_ErrLog_Text);
                return;
            } else {
                G_Log_Text = "HEADS�ް�Rev�w" + s_FilePass + "�x(�װB�d�l)��ǂݍ��݂܂����B";
                Write_Log(G_Log_Text);
            }
		 }
         //*************************************

         //*************************
         //***  �o�͐�̫�������
         //*************************
         //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
         // ��̔��f
		 //2022/12/15 �����ǉ��Ή� Mod_S
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
		 //2022/12/15 �����ǉ��Ή� Mod_E
		 if (G_HEADS_DATA_Rev - 1 <= 0 ) {
            Rev.sprintf("%02d", 0);
         }else{
            Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
         }
         //*************************************
         if (P_Sakuban_Mode == "S"){
            // ���ް
            G_xls_Output_Dir = G_xls_Output_Dir + G_OUT_ORDER_Dir + G_SAKUBAN;
            //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
            //�}���`�T�[�o�Ή� 2008/11/17 MOD START
			//G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HD_Root) + IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
			G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ORDER_Dir + Work.c_str() + "\\" + GetFileSeizouBangou(G_SAKUBAN) + "\\" + Rev.c_str();
            //�}���`�T�[�o�Ή� 2008/11/17 MOD END
            //*************************************
         } else {
            // ����
			G_xls_Output_Dir = G_xls_Output_Dir + G_OUT_ESTIM_Dir + G_SAKUBAN;
            //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
            //�}���`�T�[�o�Ή� 2008/11/17 MOD START
			//G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HD_Root) + IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ESTIM_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
			G_xls_Output_Dir_old = IncludeTrailingBackslash(G_HEADS_OUT_Dir) + G_OUT_ESTIM_Dir + Work.c_str() + "\\" + G_SAKUBAN + "\\" + Rev.c_str();
            //�}���`�T�[�o�Ή� 2008/11/17 MOD END
            //*************************************
         }

         //TEST
		 G_Log_Text = "G_xls_Output_Dir_old�w" + G_xls_Output_Dir_old + "�x";
         Write_Log(G_Log_Text);
         //TEST

         G_xls_Output_Dir = IncludeTrailingBackslash(G_xls_Output_Dir);
         if (!OutPut_Folder_Check(G_xls_Output_Dir)){
            return;
         }

         // ��ڰčE���ް��i�[��
         //�}���`�T�[�o�Ή� 2009/1/13 MOD START
         //G_AnaFilePass = IncludeTrailingBackslash(G_HD_Root)
         //              + IncludeTrailingBackslash(G_HEADS_DATA_Dir)
         //              + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
         //              + IncludeTrailingBackslash(G_SAKUBAN);
         G_AnaFilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                       + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                       + IncludeTrailingBackslash(G_SAKUBAN);
         //�}���`�T�[�o�Ή� 2009/1/13 MOD END

         //*************************
         //***  HEADS�ް��@�Ǎ�
         //*************************
         //s_FilePass = G_HD_Root + G_HEADS_INP_Dir + G_CSV_Dir + G_HEADS_ID;

         //*******************************************
         //***  HEADS�ް��@�Ǎ�
         //***  ( 02.08.07 �Ǎ�̧�ٖ��ύX ) csv��txt
         //*******************************************
         //s_FilePass = G_Input_Csv_Dir + G_HEADS_ID;
         s_FilePass = G_Input_Txt_Dir + G_HEADS_ID;

         if (!Read_HEADS_Data(s_FilePass)){
            G_ErrLog_Text = "HEADS�ް��w" + s_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
            Write_Error_Log(G_ErrLog_Text);
            return;
         } else {
            G_Log_Text = "HEADS�ް��w" + s_FilePass + "�x��ǂݍ��݂܂����B";
            Write_Log(G_Log_Text);
         }

         //TEST
         G_Log_Text = "REV�w" + G_HEADS_DATA[319] + "�x";
         Write_Log(G_Log_Text);
         //TEST

         //*************************
         //***  HEADS�ް��@�Ǎ�
         //*************************
         // �ŏo���敪(RecNo[1259] �ł��o���敪�Q����)
         P_UtiKbn = G_HEADS_DATA[1259].SubString(2,1);
         // ۸ލ쐬
         G_Log_Text = "RecNo:1259�w" + G_HEADS_DATA[1259] + "�x����ŏo���敪�w" + P_UtiKbn
                    + "�x���擾���܂����B";
         Write_Log(G_Log_Text);

         //TEST
         //***************************
         //***  �t�H���_�ړ��@����
         //***************************
         MoveFolder_Job();
         //TEST

         // 2019/05/17 ���p�Z�p�ǉ�
         bIsNotKatashiki = false;
         bMirrorFlag = false;

         //Windows 2008Server �ڐA�Ή�
         G_Log_Text = "���o�^�̌^���Ή��J�n";
         Write_Log(G_Log_Text);
         G_Err_Log_FileFName = G_xls_Output_Dir + G_Err_Log_FileName;
         Write_Log(G_Err_Log_FileFName);
         if (FileExists(G_Err_Log_FileFName)) {
            Screen->Cursor = crDefault;
            //PostMessage(Handle, WM_CLOSE, 0,0);
            //return ;
            // 2019/05/17 ���p�Z�p�ǉ�
            bIsNotKatashiki = true;
         }

         //2020.04.20 ���������o��_S
         //************************************
         //***  ���������o����۸���  �J�n
         //************************************
         if (FileExists(G_Err_Log_FileFName) == false) {
             HotAirDrying_Run();
         }
         //2020.04.20 ���������o��_E

         //*************************
         //***  Excel �N��
         //*************************
         fret = CallExcel(false);
         if(fret != 0) // Excel �ďo�� �N�����s�H
         {
            Screen->Cursor = crDefault;
            //PostMessage(Handle, WM_CLOSE, 0,0);
            return;
         }

         //*********************
         //***  �e�풠�[�@�쐬
         //*********************
         Excel_Run();

         //*********************
         //***  HeasData�׽�I��
         //*********************
         delete[] m_pcHeadsData;

         //*********************
         //***  �ް��ް��j��
         //*********************
         if (DataModule1){
            DataModule1 = NULL;
         }

         //*********************
         //***  Excel �I��
         //*********************
         QuitExcel();

         // ۸ލ쐬
         G_Log_Text = "EXCEL�f�[�^�o�̓V�X�e�����I�����܂����B\n\n\n\n\n\n";
         Write_Log(G_Log_Text);
         Write_Error_Log(G_Log_Text);
      }
   }
   __finally{
      //*********************
      //***  �ް��ް��j��
      //*********************
      if (DataModule1){
         delete DataModule1;
         DataModule1 = NULL;
      }

      //*********************
      //***  Excel �I��
      //*********************
      QuitExcel();

      Screen->Cursor = crDefault;
      //PostMessage(Handle, WM_CLOSE, 0,0);
   }
}

//---------------------------------------------------------------------------
// ���{��֐����F �G�N�Z���@�e�풠�[�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F ���� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
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
        i_MirrorLoop = 1;  // ���d�l�̂�
    }else{
        i_MirrorLoop = 3;  // ���d�l�AA�d�l�AB�d�l
    }

    // ���d�l�A�װA�d�l�A�װB�d�l��ٰ��
    for(i=0;i<i_MirrorLoop;i++){

        pcHeadsData = &m_pcHeadsData[i];
        // AnsiString�^��Heads�ް���Ӽޭ�ٕϐ��ɾ��
        bRet = pcHeadsData->SetAnsiStringData(P_HEADS_DATA);
        if(!bRet) return;

        // �d�l���ߺ��ށA�d�l���ߕ�������擾
        pcHeadsData->SetSpecType(i);
        pcHeadsData->SetSpecCode();

        // --> ��2
        bCopySeisekishoFlag = true;
        // <-- ��2

        //  �v���[�g�^���̌���
        s_Syurui = P_HEADS_DATA[34].TrimRight();                // HEADS�ް� ���

        // RecNo.542���ق̎擾���@�̕ύX
        s_Model  = P_HEADS_DATA[35].TrimRight(); // HEADS�ް� ����
        //2009.06.19 UX-100R�ǉ�
        //EX�̏ꍇ
        //s_Model��[����"C","P","R","A","B"������ꍇ
        //���̕���������s_Model���珜���Ă���
        //
        //EX�ȊO�̏ꍇ
        //s_Model��[����"C","P","A","B"������ꍇ
        //���̕���������s_Model���珜���Ă���
        //���ɏ����ɍ��킹�Ĉȉ��̕�������s_Model���珜��
        //  �Ō�̕�����"D"�̏ꍇ
        //�@�@�F�Ōォ��Q�����ڂ̕�����"C","P","A","B"�̏ꍇ
        //�@�@�F���̕���������s_Model���珜���Ă���
        //�@�@�F���ɍŌォ��Q�����ڂ̕���(����)������
        //  �Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
        //  ���̑��̏ꍇ                       �F�Ō�̕���(����)
        ////2006.02.06 SX-90M,L�ǉ�
        ////s_Model��[����"C","P","R","A","B"������ꍇ
        ////���̕���������s_Model���珜���Ă���
        ////���ɏ����ɍ��킹�Ĉȉ��̕�������s_Model���珜��
        ////EX�̏ꍇ                           �F�����Ȃ�
        ////2008.07.24 CX-10D�ǉ�
        ////�Ō�̕�����"D"�̏ꍇ�@�@�@�@�@�@�@�@�@
        ////�@�@�F�Ōォ��Q�����ڂ̕����� "C","P","R","A","B"�̏ꍇ
        ////�@�@�F���̕���������s_Model���珜���Ă���
        ////�@�@�F���ɍŌォ��Q�����ڂ̕���(����)������
        ////*********************
        //**********************
        //�Ō�̕����������łȂ�("M","L","S")�F�Ōォ��2�����ڂ̕���(����)
        //���̑��̏ꍇ                       �F�Ō�̕���(����)
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
        //2009.06.19 UX-100R�ǉ�
        //if ( s_Model.SubString(s_Model.Length(),1) == "R" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        if ( s_Syurui.SubString(1,2) == "EX" && s_Model.SubString(s_Model.Length(),1) == "R" ) {
            s_Model = s_Model.SubString(1,s_Model.Length()-1);
        }
        //**********************
        if ( s_Model.SubString(s_Model.Length(),1) == "P" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        if ( s_Model.SubString(s_Model.Length(),1) == "C" ) { s_Model = s_Model.SubString(1,s_Model.Length()-1); }
        if ( s_Syurui.SubString(1,2) == "EX" ) {
        //2008.07.24 CX-10D�ǉ�
        } else if ( s_Model.SubString(s_Model.Length(),1) == "D" ) {
            if ( s_Model.SubString(s_Model.Length()-1,1) == "B" ) {
                s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
            }
            if ( s_Model.SubString(s_Model.Length()-1,1) == "A" ) {
                s_Model = s_Model.SubString(1,s_Model.Length()-2) + "D";
            }
            //2009.06.19 UX-100R�ǉ�
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

        G_K_Syurui   = "";    // �^�����
        G_K_Model1   = "";    // �^������(Я���^��1)
        G_K_Model2   = "";    // �^������(Я���^��2)
        G_KS_Syurui  = "";    // ���̎��
        G_KS_Model   = "";    // ��������
        G_Model_Zen1 = "";    // �^�����ّS�p1
        G_Model_Zen2 = "";    // �^�����ّS�p2

        G_Log_Text = "��ڰČ^���ϊ�ð��ق��A��ށw" + s_Syurui
                   + "�x���f���w" + s_Model + "�x�Ō����J�n�B";
        Write_Log(G_Log_Text);

        for(int i=0; i < G_PHENKAN_RecCnt; i++)
        {
            if (G_PHENKAN[i][0] == s_Syurui && G_PHENKAN[i][1] == s_Model){
                G_K_Syurui   = G_PHENKAN[i][0];    // �^�����
                G_K_Model1   = G_PHENKAN[i][5];    // �^������(Я���^��1)
                G_K_Model2   = G_PHENKAN[i][6];    // �^������(Я���^��2)
                G_KS_Syurui  = G_PHENKAN[i][2];    // ���̎��
                G_KS_Model   = G_PHENKAN[i][3];    // ��������
                G_Model_Zen1 = G_PHENKAN[i][7];    // �^�����ّS�p1
                G_Model_Zen2 = G_PHENKAN[i][8];    // �^�����ّS�p2

                break;
            }
        }

        G_Log_Text = "�^�����            �w" + G_K_Syurui + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�^������(Я���^��1) �w" + G_K_Model1 + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�^������(Я���^��2) �w" + G_K_Model2 + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "���̎��            �w" + G_KS_Syurui + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "��������            �w" + G_KS_Model + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�^�����ّS�p1       �w" + G_Model_Zen1 + "�x���擾�B";
        Write_Log(G_Log_Text);
        G_Log_Text = "�^�����ّS�p2       �w" + G_Model_Zen2 + "�x���擾�B";
        Write_Log(G_Log_Text);

        G_Log_Text = "��ڰČ^���ϊ�ð���  �����I���B";
        Write_Log(G_Log_Text);

        if( G_Output_Mode == 1 )
        {
            G_Log_Text = "������ �yG_Output_Mode == 1�zExcel�o�̓e�X�g�N�� ������";
            Write_Log(G_Log_Text);
            // �e�X�g�p
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
            // --> ��2 �ǉ�
            //2019/05/17 ���p�Z�p�ǉ�
            //if (!bIsNotKatashiki) {
                Funeyousinsei(pcHeadsData); // ���p�\����
                if(i == 0) bCopySeisekishoFlag = false;
            //}

            Seisekisyo(pcHeadsData);    // �������я�
            // <-- ��2 �ǉ�
            Syonin(pcHeadsData);        // ���F�}�\��
            if (!bIsNotKatashiki) {
                Funeyou(pcHeadsData);       // ���p�v�ڕ\
                Kobetusinsei(pcHeadsData);  // �ʌ���\����
                //2009.03.03 ���͗e�틭�x�v�Z�ύX
                //Kyodokeisan(pcHeadsData);   // ���͗e�틭�x�v�Z��
                //*******************************
                Meisaisyo(pcHeadsData);     // ���^���͗e�� ���׏�
            }
            Mokuroku(pcHeadsData);      // �}�ʖژ^
            Seisekisyo(pcHeadsData);    // �������я�
            if (!bIsNotKatashiki) {
//Mod_Str 2019/02/19 Ri
                //Sijisyo(pcHeadsData);       // ����w����
                if (G_OLD_FLG == 0) {        //�V����w�����쐬
                    SijisyoA3(pcHeadsData);
                }else if(G_OLD_FLG == 1){    //������w�����쐬
                    Sijisyo(pcHeadsData);
                }else{                       //�V�E������w���������쐬
                    SijisyoA3(pcHeadsData);
                    Sijisyo(pcHeadsData);
                }
//Mod_End 2019/02/19 Ri
            }
            Youryosyo(pcHeadsData);     // �����v�̏�
            Buhinhyou(pcHeadsData, m_pcHeadsData);     // �W�����i+�\
            //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        }
        else
        {
            G_Log_Text = "������ �yG_Output_Mode == 0�z�{�ԉғ� ������";
            Write_Log(G_Log_Text);

            if (P_UtiKbn == "1" || P_UtiKbn == "5" || P_UtiKbn == "7"){
                //**************************************
                //***  �ŏo���敪�@�w1�x,�w5�x,�w7�x
                //**************************************
                Syonin(pcHeadsData);         // ���F�}�\��

            } else if (P_UtiKbn == "2") {
                //**************************
                //***  �ŏo���敪�@�w2�x
                // **************************
                Syonin(pcHeadsData);         // ���F�}�\��

                i_Pos = P_HEADS_DATA[36].Pos("M");
                if (i_Pos != 0){
                    if (!bIsNotKatashiki) {
                        Funeyou(pcHeadsData);     // ���p�v�ڕ\
                    }
                    // --> ��2
                    Funeyousinsei(pcHeadsData);
                    if(i == 0) bCopySeisekishoFlag = false;
                    // <-- ��2
                    //}
                }

            } else if (P_UtiKbn == "3") {
                //**************************
                //***  �ŏo���敪�@�w3�x
                //**************************
                Syonin(pcHeadsData);         // ���F�}�\��
                //2009.03.03 ���͗e�틭�x�v�Z�ύX
                //Kyodokeisan(pcHeadsData);    // ���͗e�틭�x�v�Z��
                //*******************************
                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // �K�i���ނ�03:���^���͗e��\���K�i
                        Kobetusinsei(pcHeadsData);  // �ʌ���\����
                        Meisaisyo(pcHeadsData);     // ���^���͗e�� ���׏�
                    }
                }

            } else if (P_UtiKbn == "4" && P_Sakuban_Mode == "S") {
                //**************************
                //***  �ŏo���敪�w4�x
                //**************************
                Syonin(pcHeadsData);         // ���F�}�\��
                //2009.03.03 ���͗e�틭�x�v�Z�ύX
                //Kyodokeisan(pcHeadsData);    // ���͗e�틭�x�v�Z��
                //*******************************

                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // �K�i���ނ�03:���^���͗e��\���K�i
                        Kobetusinsei(pcHeadsData);  // �ʌ���\����
                        Meisaisyo(pcHeadsData);     // ���^���͗e�� ���׏�
                    }
                }

                // �}�ʖژ^�E�W�����i�\
                if (P_Sakuban_Mode == "S"){
                    Mokuroku(pcHeadsData);    // �}�ʖژ^
                    Buhinhyou(pcHeadsData, m_pcHeadsData);   // �W�����i�\
                }

                // ���p�v�ڕ\
                i_Pos = P_HEADS_DATA[36].Pos("M");
                if (i_Pos != 0){
                    if (!bIsNotKatashiki) {
                        Funeyou(pcHeadsData);
                }
                // --> ��2
                Funeyousinsei(pcHeadsData);
                if(i == 0) bCopySeisekishoFlag = false;
                // <-- ��2
                //}
                }

            //2015.09.17 �o�}�敪�ύX
            } else if (P_UtiKbn == "4" && P_Sakuban_Mode != "S") {
                // ���p�v�ڕ\
                i_Pos = P_HEADS_DATA[36].Pos("M");
                    if (i_Pos != 0){
                        if (!bIsNotKatashiki) {
                            Funeyou(pcHeadsData);
                    }
                    Funeyousinsei(pcHeadsData);
                    if(i == 0) bCopySeisekishoFlag = false;
                    //}
                }
                Youryosyo(pcHeadsData);      // �����v�̏�
            //***********************

            } else if (P_UtiKbn == "6") {
                //************************
                //***  �ŏo���敪�w6�x
                //************************
                Syonin(pcHeadsData);         // ���F�}�\��
                if (!bIsNotKatashiki) {
                    Funeyou(pcHeadsData);        // ���p�v�ڕ\
                }
                // --> ��2
                Funeyousinsei(pcHeadsData);
                if(i == 0) bCopySeisekishoFlag = false;
                //}

                // <-- ��2
                //2009.03.03 ���͗e�틭�x�v�Z�ύX
                //Kyodokeisan(pcHeadsData);    // ���͗e�틭�x�v�Z��
                //*******************************
                Youryosyo(pcHeadsData);      // �����v�̏�

                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // �K�i���ނ�03:���^���͗e��\���K�i
                        Kobetusinsei(pcHeadsData);  // �ʌ���\����
                        Meisaisyo(pcHeadsData);     // ���^���͗e�� ���׏�
                    }
                }

                // �}�ʖژ^�E�W�����i�\
                //2011/01/07 �}�ʖژ^�E�W�����i�\���o�͂���悤�ɕύX
                //if (P_Sakuban_Mode == "S") {
                    Mokuroku(pcHeadsData);    // �}�ʖژ^
                    Buhinhyou(pcHeadsData, m_pcHeadsData);   // �W�����i�\
                //}

            } else if (P_UtiKbn == "8" ){
                //************************
                //***  �ŏo���敪�w8�x
                //************************
                Syonin(pcHeadsData);         // ���F�}�\��
                Youryosyo(pcHeadsData);      // �����v�̏�
            }

            // 2018.07.09 �o�}�敪�ύX_S
            else if (P_UtiKbn == "9") {
                //************************
                //***  �ŏo���敪�w9�x
                //************************
                Syonin(pcHeadsData);         // ���F�}�\��
                if (!bIsNotKatashiki) {
                    Funeyou(pcHeadsData);        // ���p�v�ڕ\
                }
                    Funeyousinsei(pcHeadsData);
                    if(i == 0) bCopySeisekishoFlag = false;
                //}

                Youryosyo(pcHeadsData);      // �����v�̏�

                if (!bIsNotKatashiki) {
                    if ( P_HEADS_DATA[29].ToIntDef(0) == 3 ) {     // �K�i���ނ�03:���^���͗e��\���K�i
                        Kobetusinsei(pcHeadsData);  // �ʌ���\����
                        Meisaisyo(pcHeadsData);     // ���^���͗e�� ���׏�
                    }
                }

                Mokuroku(pcHeadsData);    // �}�ʖژ^
                Buhinhyou(pcHeadsData, m_pcHeadsData);   // �W�����i�\

            }
            // 2018.07.09 �o�}�敪�ύX_E

            //2015.09.17 �o�}�敪�ύX
            //************************
            //***  �����v�̏�
            //************************
            //if ( P_UtiKbn != "8" && P_UtiKbn != "6" ) {
            if ( P_UtiKbn == "6" || P_UtiKbn == "8" ) {
            // 2018.06.22 �o�}�敪�ύX_S
            } else if ( P_UtiKbn == "9" ) {
            // 2018.06.22 �o�}�敪�ύX_E
            } else if (P_UtiKbn == "4" && P_Sakuban_Mode != "S") {
            } else {
            //***********************
                if ( P_HEADS_DATA[443].TrimRight() == "1") {
                    Youryosyo(pcHeadsData);      // �����v�̏�
                }
            }

            //************************
            //***  �������я�
            //************************
            //2011/01/07 �ŏo���敪�w6�x�̏ꍇ�A�������я����o�͂���悤�ɕύX
            //if (P_Sakuban_Mode == "S") {
            if ( (P_Sakuban_Mode == "S") || (P_UtiKbn == "6") ) {
                //2017.12.26 �A���t�@���o���v���[�g_S
                //Seisekisyo(pcHeadsData);     // �������я�
                if (P_UtiKbn == "7"){
                } else {
                    Seisekisyo(pcHeadsData); // �������я�
                }
                //2017.12.26 �A���t�@���o���v���[�g_E
            }
            // 2018.06.22 �o�}�敪�ύX_S
            else if ( P_UtiKbn == "9" ) {
                Seisekisyo(pcHeadsData); // �������я�
            }
            // 2018.06.22 �o�}�敪�ύX_E

            //************************
            //***  ����w����
            //************************
			if (P_Sakuban_Mode == "S" && P_HEADS_DATA[302].TrimRight() == "1") {
//Mod_Str 2019/02/19 Ri
                if (!bIsNotKatashiki) {
                    //Sijisyo(pcHeadsData);       // ����w����
                    if (G_OLD_FLG == 0) {        //�V����w�����쐬
                        SijisyoA3(pcHeadsData);
                    }else if(G_OLD_FLG == 1){    //������w�����쐬
                        Sijisyo(pcHeadsData);
                    }else{                       //�V�E������w���������쐬
                        SijisyoA3(pcHeadsData);
                        Sijisyo(pcHeadsData);
                    }
                }
//Mod_End 2019/02/19 Ri
                //************************
                //***  �}���W�ړ�����
                //************************
                if (!bIsNotKatashiki) {
                    MovePDFFile();
                }
            }
            // 2017.10.06 �Z�[���Ή�_S
            if (P_Sakuban_Mode == "S" && P_HEADS_DATA[302].TrimRight() == "2") {
//Mod_Str 2019/02/19 Ri
                if (!bIsNotKatashiki) {
                    //Sijisyo(pcHeadsData);       // ����w����
                    if (G_OLD_FLG == 0) {        //�V����w�����쐬
                        SijisyoA3(pcHeadsData);
                    }else if(G_OLD_FLG == 1){    //������w�����쐬
                        Sijisyo(pcHeadsData);
                    }else{                       //�V�E������w���������쐬
                        SijisyoA3(pcHeadsData);
                        Sijisyo(pcHeadsData);
                    }
                }
//Mod_End 2019/02/19 Ri
                //************************
                //***  �}���W�ړ�����
                //************************
                if (!bIsNotKatashiki) {
                    MovePDFFile();
                }
            }
            // 2017.10.06 �Z�[���Ή�_E
        }
   } // �װٰ��For��End
}

//---------------------------------------------------------------------------
// ���{��֐����F ���O�t�@�C���@�T�C�Y�`�F�b�N
// �T  �v      �F ۸�̧�ق̻��ނ��擾���A�K��l�𒴂��Ă�����ߋ�̫��ނɈړ�������
// ��  ��      �F �Ȃ�
// �߂�l      �F ���� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Log_File_SizeCheck(void)
{
   AnsiString to_filename;    // �쐬�ߋ�۸�̧��(���߽)
   struct     stat statbuf;
   double     d_FileSize;

   //***************************
   //***  ۸�̧�ف@��������
   //***************************
   // �쐬��۸�̧�ف@��������
   stat(G_LogFilePath.c_str(), &statbuf);
   d_FileSize = statbuf.st_size;

   // ۸�̧�ق̻��ނ��ݒ�l�ȏ�Ȃ�w�N���������b�x+̧�ٖ��ŉߋ�۸�̫��ނֺ�߰
   if (d_FileSize >= G_Log_Max_Size){

      // �ߋ�۸ޗp�߽�쐬
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
   //***  �װ۸�̧�ف@��������
   //***************************
   // �쐬��װ۸�̧�ف@��������
   stat(G_ErrLogFilePath.c_str(), &statbuf);
   d_FileSize = statbuf.st_size;

   // �װ۸�̧�ق̻��ނ��ݒ�l�ȏ�Ȃ�w�N���������b�x+̧�ٖ��ŉߋ��װ۸�̫��ނֺ�߰
   if (d_FileSize >= G_ErrLog_Max_Size){

      // �ߋ��װ۸ޗp�߽�쐬
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
// ���{��֐����F �f�[�^�x�[�X�@�ڑ�
// �T  �v      �F mdb��ODBC�o�R�Őڑ�����
// ��  ��      �F �Ȃ�
// �߂�l      �F �ڑ����� [true:���� false:���s]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Connect_DataBase(void)
{
   try
   {
      // 2021.06.18 FireDAC�ւ̈ڍs ADD_S
	  AnsiString AIniPath; // ��`̧���߽
	  AnsiString AIniFile; // ��`̧���߽ + ̧�ٖ�
	  // 2021.06.18 FireDAC�ւ̈ڍs ADD_E

      // Access�ւ�ODBC�ڑ�
      DataModule1 = new TDataModule1(this);

      // 2021.06.18 FireDAC�ւ̈ڍs ADD_S
	  // �ڑ���`�t�@�C���ݒ�
	  AIniPath = ExtractFilePath(Application->ExeName);
	  AIniFile = AIniPath + CSYSCONNDEFFILENAME;
	  DataModule1->FDManagerDN->ConnectionDefFileName = AIniFile;
	  DataModule1->FDManagerDN->ConnectionDefFileAutoLoad = True;
	  DataModule1->FDManagerDN->Active = True;
	  // 2021.06.18 FireDAC�ւ̈ڍs ADD_E

      // �}�ԃf�[�^�x�[�X(HD_DNO.mdb)
      G_ErrLog_Text = "�}�ԃf�[�^�x�[�X(HD_DNO.mdb)";

	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	  //DataModule1->Database_DNO->DatabaseName = ODBC_DSN_DNO;
	  DataModule1->Database_DNO->ConnectionName = ODBC_DSN_DNO;
	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
      DataModule1->Database_DNO->Params->Clear();
      DataModule1->Database_DNO->LoginPrompt = false;
      DataModule1->Database_DNO->Connected = true;

      G_Log_Text = "�}�ԃf�[�^�x�[�X(HD_DNO.mdb)�Ɛڑ����܂����B";
      Write_Log(G_Log_Text);

      // �����v�̏��f�[�^�x�[�X(HD_INSPCTL.mdb)
      G_ErrLog_Text = "�����v�̏��f�[�^�x�[�X(HD_INSPCTL.mdb)";

	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	  //DataModule1->Database_MAT->DatabaseName = ODBC_DSN_INS;
	  DataModule1->Database_MAT->ConnectionName = ODBC_DSN_INS;
	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
      DataModule1->Database_MAT->Params->Clear();
      DataModule1->Database_MAT->LoginPrompt = false;
      DataModule1->Database_MAT->Connected = true;

      G_Log_Text = "�����v�̏��f�[�^�x�[�X(HD_INSPCTL.mdb)�Ɛڑ����܂����B";
      Write_Log(G_Log_Text);

      // �ގ��f�[�^�x�[�X(HD_MATERIAL.mdb)
      G_ErrLog_Text = "�ގ��f�[�^�x�[�X(HD_MATERIAL.mdb)";

      // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
	  //DataModule1->Database_INS->DatabaseName = ODBC_DSN_MAT;
	  DataModule1->Database_INS->ConnectionName = ODBC_DSN_MAT;
	  // 2021.06.18 FireDAC�ւ̈ڍs MOD_E
      DataModule1->Database_INS->Params->Clear();
      DataModule1->Database_INS->LoginPrompt = false;
      DataModule1->Database_INS->Connected = true;

      G_Log_Text = "�ގ��f�[�^�x�[�X(HD_MATERIAL.mdb)�Ɛڑ����܂����B";
      Write_Log(G_Log_Text);
   }
   catch(EDatabaseError &Error)
   {
      G_ErrLog_Text = G_ErrLog_Text + "�ւ̐ڑ��Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F �K�i���ށC�ƊE���ށC�h���F�@�Ǎ�
// �T  �v      �F �wKIKAKU.CSV�x,�wGYOKAI.CSV�x,�wTOSOU.CSV�x��ǂݍ���
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Read_CodeMasta_Data(void)
{
   char  buf[1024];                            /* ������o�b�t�@   */
   char  *tok;                                 /* �g�[�N���|�C���^ */
   int   i;                                    /* ���[�v�J�E���^   */
   char  token[100][100];                      /* �g�[�N���o�b�t�@ */

   int   RecNo;

   AnsiString wk_FilePass;
   FILE *fp;

   //***********************
   //**  �K�i���� ���
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "KIKAKU.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

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

   fclose(fp);    // ����

   G_KIKAKU_RecCnt = RecNo;

   //***********************
   //**  �ƊE���� ���
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "GYOKAI.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

         if (i>70) {
            break;
         }
      }

      G_GYOKAI[RecNo][0] = token[0];
      G_GYOKAI[RecNo][1] = token[1];

      RecNo++;
   }

   fclose(fp);    // ����

   G_GYOKAI_RecCnt = RecNo;

   //***********************
   //**  �h���F ���
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "TOSOU.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

         if (i>70) {
            break;
         }
      }

      G_TOSOU[RecNo][0] = token[0];
      G_TOSOU[RecNo][1] = token[1];
      G_TOSOU[RecNo][2] = token[2];

      RecNo++;
   }

   fclose(fp);    // ����

   G_TOSOU_RecCnt = RecNo;

   //***************************
   //**  ��ڰČ^���ϊ�ð���
   //***************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "PHENKAN.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

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

   fclose(fp);    // ����

   G_PHENKAN_RecCnt = RecNo;

   //******************************
   //**  ɽ�ٍ\�����i���� ð���
   //******************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "NOZZLECD.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

         if (i>100) {
            break;
         }
      }

      G_NOZZLECD[RecNo][0] = token[0];    // ����
      G_NOZZLECD[RecNo][1] = token[1];    // ���i���޺���
      G_NOZZLECD[RecNo][2] = token[2];    // ���l(���i�敪)
      G_NOZZLECD[RecNo][3] = token[3];    // ���l(���i����)
      G_NOZZLECD[RecNo][4] = token[4];    // �\���i�W�v�p(?)
      G_NOZZLECD[RecNo][5] = token[5];    // �A��(?)

      RecNo++;
   }

   fclose(fp);    // ����

   G_NOZZLECD_RecCnt = RecNo;

   //******************************
   //**  ���x�v�\�����i���� ð���
   //******************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "TEMPCD.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

         if (i>100) {
            break;
         }
      }

      G_TEMPCD[RecNo][0] = token[0];    // ����
      G_TEMPCD[RecNo][1] = token[1];    // ���i���޺���
      G_TEMPCD[RecNo][2] = token[2];    // ���l(���i�敪)
      G_TEMPCD[RecNo][3] = token[3];    // ���l(���i����)
      G_TEMPCD[RecNo][4] = token[4];    // �\���i�W�v�p(?)
      G_TEMPCD[RecNo][5] = token[5];    // �A��(?)

      RecNo++;
   }

   fclose(fp);    // ����

   G_TEMPCD_RecCnt = RecNo;

   //******************************
   //**  ���͌v�\�����i���� ð���
   //******************************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "PRESSGCD.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

         if (i>100) {
            break;
         }
      }

      G_PRESSCD[RecNo][0] = token[0];    // ����
      G_PRESSCD[RecNo][1] = token[1];    // ���ʎq
      G_PRESSCD[RecNo][2] = token[2];    // ���l(?)
      G_PRESSCD[RecNo][3] = token[3];    // ���i����
      G_PRESSCD[RecNo][4] = token[4];    // �\���i

      RecNo++;
   }

   fclose(fp);    // ����

   G_PRESSCD_RecCnt = RecNo;

   // 2015.01.16 �m�Y���F�ύX
   //***********************
   //**  �u�[�c�F ���
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "BOOTSCOLOR.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

         if (i>70) {
            break;
         }
      }

      G_BOOTCL[RecNo][0] = token[0];
      G_BOOTCL[RecNo][1] = token[1];
      G_BOOTCL[RecNo][2] = token[2];

      RecNo++;
   }

   fclose(fp);    // ����

   G_BOOTCL_RecCnt = RecNo;
   // ***********************�@

   // 2016/09/14 �X�e�b�J�[�ǉ�
   //***********************
   //**  �X�e�b�J�[ ���
   //***********************
   wk_FilePass = IncludeTrailingBackslash(G_Input_Csv_Dir)
               + "STACKER.csv";

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      // �װ۸ލ쐬
      G_ErrLog_Text = "�w" + wk_FilePass + "�x�̂n�o�d�m�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      fclose(fp);    // ����
      return false;
   }

   // �Ǎ��݉\�̊�
   RecNo = 0;
   memset(buf,0x00,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp)!=NULL){
      i = 0;                                 // �g�[�N���J�E���^������
      memset(token,0x00,sizeof(token));      // �g�[�N���o�b�t�@������
      tok = GetToken(buf,",");               // ","�ō��ڂ���؂�
      while(tok){                            // �g�[�N�������݂����
         sprintf(token[i++],"%-.99s",tok);   // �g�[�N�����o�b�t�@�ɓ]��
         tok = GetToken(NULL,",");           // ���̃g�[�N��������

         if (i>70) {
            break;
         }
      }

      G_STACKER[RecNo][0] = token[0];
      G_STACKER[RecNo][1] = token[1];
      G_STACKER[RecNo][2] = token[2];

      RecNo++;
   }

   fclose(fp);    // ����

   G_STACKER_RecCnt = RecNo;
   // *************************�@

   //delete fp;
   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F �쐬�t�@�C���o�͐�t�H���_�@�`�F�b�N
// �T  �v      �F �����Ώۂ�̫��ނ��������A̫��ނ����݂��Ȃ���΍쐬����B
// ��  ��      �F AnsiString Check_Pass  // HEADS�o��̫��� + ���
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::OutPut_Folder_Check(AnsiString Check_Pass)
{
   if (!DirectoryExists(Check_Pass)){
      if (!ForceDirectories(Check_Pass)){
         // �t�H���_�쐬�@���s
         G_ErrLog_Text = "�t�H���_�w " + Check_Pass + "�x���쐬�ł��܂���ł����I�I";
         Write_Error_Log(G_ErrLog_Text);
         return false;
      } else {
         // �t�H���_�쐬�@����
         G_Log_Text = "�t�H���_�w " + Check_Pass + "�x��V�K�쐬���܂����B";;
         Write_Log(G_Log_Text);
      }
   }

   return true;
}

//-----------------------------------------------------------------------------
// ���{��֐����F Excel�N������
// �T�v        �F Excel�̋N�����s��
// �p�����^    �F bool  showflg   �N���\���t���O
// �߂�l      �F �N������[0:����N��,1:�װ]
// �@�\����    �F
// ���l        �F
//-----------------------------------------------------------------------------
short __fastcall TfrmHDP010::CallExcel(bool showflg)
{
   try
   {
      if( exApplication.Type() != varDispatch )   // Excel���N���H
      {
         // Excel��޼ު�Ă��쐬
         exApplication = Variant::CreateObject( "Excel.Application" );
         // Excel��޼ު�Ă��\���Ƃ���
         exApplication.OlePropertySet( "Visible", false );
      }

      // Excel��޼ު�Ă�WorkBooks�ڸ��݂��擾
      exWorkbooks = exApplication.OlePropertyGet( "Workbooks" );

      // Excel��޼ު�Ă�\��/��\���Ƃ���
      exApplication.OlePropertySet( "Visible", showflg );
      // �x��ү���ނ�\�����Ȃ��ɂ���
      //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
      //Windows 2008Server �ڐA�Ή�
      exApplication.OlePropertySet("DisplayAlerts", false);

      return 0;        // �߂�l[0:����N��]
   }
   catch(...)
   {
      // �װ۸ޒǉ�
      G_ErrLog_Text = "Excel���N���ł��܂���ł����I�Ihogehoge";
      Write_Error_Log(G_ErrLog_Text);

      QuitExcel();
      return (-1);    // �߂�l[-1:�N���ُ�]
   }
}

//-----------------------------------------------------------------------------
// ���{��֐����F Excel�I������
// �T�v        �F Excel�̏I�����s��
// �p�����^    �F
// �߂�l      �F
// �@�\����    �F
// ���l        �F
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::QuitExcel(void)
{
   //Variant�̊J��
   exWorkCell.Clear();
   exWorkbooks.Clear();
   exWorkbook.Clear();
   exWorksheets.Clear();
   exWorksheet.Clear();

   // Excel�I������
   if( exApplication.Type() == varDispatch )
   {
      // Excel��޼ު�Ă����
      exApplication.Exec(Function("Quit"));
      //Variant�̊J��
      exApplication.Clear();
   }

}
//-----------------------------------------------------------------------------
// ���{��֐����F PDF̧�وړ�����
// �T�v        �F �ٕW�̏ꍇ��PDF�̺�߰���s��
// �p�����^    �F
// �߂�l      �F
// �@�\����    �F
// ���l        �F 2004/09/23 �ǉ�
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::MovePDFFile(void)
{
   //�}���W���f
   if (P_HEADS_DATA[301].TrimRight() == "0"){
      // �}���W�ȊO
      G_Log_Text = "----------------�}���W�ȊO----------------";
      Write_Log(G_Log_Text);
      return;
   }

   // �t�@�C���R�s�[
   G_Log_Text = "----------------�}���W�t�@�C���R�s�[�J�n----------------";
   Write_Log(G_Log_Text);

   // �t�H���_�̍쐬
   G_OUT_DIRECT_Dir = G_OUT_DIRECT_Dir + FormatDateTime("yyyymmdd",Date());
   G_OUT_DIRECT_Dir = IncludeTrailingBackslash(G_OUT_DIRECT_Dir);

   if (!OutPut_Folder_Check(G_OUT_DIRECT_Dir)){
      return;
   }

   // �Ώۃt�@�C���̌����A�R�s�[
   OutPut_File_Move(G_xls_Output_Dir, G_OUT_DIRECT_Dir, "MAN*.pdf",false);

   // �t�@�C���R�s�[
   G_Log_Text = "----------------�}���W�t�@�C���R�s�[�I��----------------";
   Write_Log(G_Log_Text);

}

//-----------------------------------------------------------------------------
// ���{��֐����F PDF̧�وړ�����
// �T�v        �F ̧�ق̌����A��߰
// �p�����^    �F
// �߂�l      �F
// �@�\����  �F
// ���l        �F 2004/09/27 �ǉ�
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::OutPut_File_Move(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg)
{
   HANDLE han;
   WIN32_FIND_DATA wfd;
   bool bRet;
   AnsiString SearchFile;

   SearchFile = FromFile + Files;


   G_Log_Text = "�����t�@�C���w" + SearchFile + "�x";
   Write_Log(G_Log_Text);

   if((han=FindFirstFile((SearchFile).c_str(), &wfd))!=INVALID_HANDLE_VALUE)
   {
      do
      {
         //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
         //if ((((AnsiString)wfd.cFileName).c_str() == ".") || (((AnsiString)wfd.cFileName).c_str() == "..")) continue;
         //bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
         //if(bRet == false){
         //   G_Log_Text = "�t�@�C���̃R�s�[�Ɏ��s���܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
         //}else{
         //   G_Log_Text = "�t�@�C���̃R�s�[�ɂ��܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
         //}
         //Write_Log(G_Log_Text);
         //
         //// �t�@�C���̍폜
         //if(DelFlg){
         //   bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
         //   if(bRet == false){
         //      G_Log_Text = "�t�@�C���̍폜�Ɏ��s���܂����B�w " + FromFile + (AnsiString)wfd.cFileName + " �x";
         //      Write_Log(G_Log_Text);
         //   }
         //}
         if(strcmp(wfd.cFileName,".")!=0 && strcmp(wfd.cFileName,"..")!=0){
            bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
            if(bRet == false){
               G_Log_Text = "�t�@�C���̃R�s�[�Ɏ��s���܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
            }else{
               G_Log_Text = "�t�@�C���̃R�s�[�ɂ��܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
            }
            Write_Log(G_Log_Text);

            // �t�@�C���̍폜
            if(DelFlg){
               bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
               if(bRet == false){
                  G_Log_Text = "�t�@�C���̍폜�Ɏ��s���܂����B�w " + FromFile + (AnsiString)wfd.cFileName + " �x";
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
// ���{��֐����F �t�H���_�ړ�����
// �T�v        �F ̫��ޓ��S�Ă�̧�ق̈ړ�
// �p�����^    �F
// �߂�l      �F
// �@�\����  �F
// ���l        �F 2004/09/27 �ǉ�
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::MoveFolder_Job(void)
{
   bool bRet, bRet1;
   AnsiString FormFolder, ToFolder, FormFolder1, ToFolder1, Rev;
   AnsiString Work, TooFolder, TooFolder1;
   int Sakuban, Number;

   // �t�H���_�̈ړ�
   G_Log_Text = "----------------�t�H���_�ړ��J�n----------------";
   Write_Log(G_Log_Text);

   // �ړ���t�H���_�̍쐬
   //2022/12/15 �����ǉ��Ή� Mod_S
   //Work = G_SAKUBAN.SubString(1, 4);
   if (G_SAKUBAN.Length() > 7)
   {
	  Work = G_SAKUBAN.SubString(1, 6);
   }
   else
   {
	  Work = G_SAKUBAN.SubString(1, 4);
   }
   //2022/12/15 �����ǉ��Ή� Mod_E
   if( P_Sakuban_Mode == "S" )
   {
	  // ��̔��f
	  Sakuban = Work.ToIntDef(0);
	  if( Sakuban % 2 == 1 ){
		 Sakuban --;
	  }
	  //2022/12/15 �����ǉ��Ή� Mod_S
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
	  //2022/12/15 �����ǉ��Ή� Mod_E

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
   //�}���`�T�[�o�Ή� 2009/1/13 MOD START
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

   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //			 + IncludeTrailingBackslash(ToFolder)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
			 + IncludeTrailingBackslash(ToFolder)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //�}���`�T�[�o�Ή� 2009/1/13 MOD END

   // HEADS_OUTPUT
   //�}���`�T�[�o�Ή� 2008/11/17 MOD START
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

   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //			 + IncludeTrailingBackslash(ToFolder1)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
			 + IncludeTrailingBackslash(ToFolder1)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //�}���`�T�[�o�Ή� 2008/11/17 MOD END

   // ���O
   G_Log_Text = "�����t�H���_�iFormFolder�j�w" + FormFolder + "�x";
   Write_Log(G_Log_Text);
   G_Log_Text = "�����t�H���_�iFormFolder1�j�w" + FormFolder1 + "�x";
   Write_Log(G_Log_Text);
   G_Log_Text = "�����t�H���_�iToFolder�j�w" + ToFolder + "�x";
   Write_Log(G_Log_Text);
   G_Log_Text = "�����t�H���_�iToFolder1�j�w" + ToFolder1 + "�x";
   Write_Log(G_Log_Text);

   // HEADS_OUTPUT
   // �t�H���_�̗L��
   bRet = DirectoryExists(ToFolder);
   if (bRet == FALSE){
      // �t�H���_�̍쐬
      if (!OutPut_Folder_Check(ToFolder)){
         G_Log_Text = "!!!!!!!!!!!!!!!!�t�H���_�ړ��ُ�!!!!!!!!!!!!!!!!";
         Write_Log(G_Log_Text);
         return;
      }
   }

   // HEADS_DATA
   // �t�H���_�̗L��
   bRet1 = DirectoryExists(ToFolder1);
   if (bRet1 == FALSE){
      // �t�H���_�̍쐬
      if (!OutPut_Folder_Check(ToFolder1)){
         G_Log_Text = "!!!!!!!!!!!!!!!!�t�H���_�ړ��ُ�!!!!!!!!!!!!!!!!";
         Write_Log(G_Log_Text);
         return;
      }
   }

   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   // �����ɑΉ����邽�߂̃t���O���Z�b�g����
   bRetRev = false;

   // �����̃t�H���_���`�F�b�N����
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
            G_Log_Text = "�t�H���_����!!!�w" + (AnsiString)(foundFile.cFileName) + "�x";
            Write_Log(G_Log_Text);
            bRetRev = true;
         }
       }
     }
     while(FindNextFile(hFile,&foundFile)!=0);
   }

   FindClose(hFile);
   //*************************************

   // ��ԃt�H���_���Ȃ��ꍇ
   if (bRet == FALSE || bRet1 == FALSE){
        // �R�s�[
     //MoveDirectoryEvo(FormFolder, ToFolder);
     MoveDirectoryEvo1(FormFolder, ToFolder);
     G_Log_Text = "�t�H���_�̃R�s�[���܂��� from  �w " + FormFolder + " �x  to  �w " + ToFolder + " �x";
     Write_Log(G_Log_Text);

     //MoveDirectoryEvo(FormFolder1, ToFolder1);
     MoveDirectoryEvo1(FormFolder1, ToFolder1);
     G_Log_Text = "�t�H���_�̃R�s�[���܂��� from  �w " + FormFolder1 + " �x  to  �w " + ToFolder1 + " �x";
     Write_Log(G_Log_Text);

   }else{
      // Rev�̎擾
      //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
      //Number = G_HEADS_DATA[318].ToIntDef(0);
      //Rev.sprintf("%02d",Number);
      if (G_HEADS_DATA_Rev -1 <= 0 ) {
         Rev.sprintf("%02d",0 );
      }else{
         Rev.sprintf("%02d",G_HEADS_DATA_Rev - 1);
      }
      //*************************************
      G_Log_Text = "���r�W���� (Rev) �w " + Rev + " �x";
      Write_Log(G_Log_Text);

      // HEADS_OUTPUT
      TooFolder  = IncludeTrailingBackslash(ToFolder) + Rev + "\\";

      // HEADS_DATA
      TooFolder1 = IncludeTrailingBackslash(ToFolder1) + Rev + "\\";

      // HEADS_OUTPUT
      // �t�H���_�̗L��
      bRet = DirectoryExists(TooFolder);
      if (bRet == FALSE){
         // �t�H���_�̍쐬
         if (!OutPut_Folder_Check(TooFolder)){
            G_Log_Text = "!!!!!!!!!!!!!!!!�t�H���_�ړ��ُ�!!!!!!!!!!!!!!!!";
            Write_Log(G_Log_Text);
            return;
         }
      }

      // HEADS_DATA
      // �t�H���_�̗L��
      bRet1 = DirectoryExists(TooFolder1);
      if (bRet1 == FALSE){
         // �t�H���_�̍쐬
         if (!OutPut_Folder_Check(TooFolder1)){
            G_Log_Text = "!!!!!!!!!!!!!!!!�t�H���_�ړ��ُ�!!!!!!!!!!!!!!!!";
            Write_Log(G_Log_Text);
            return;
         }
      }

     // TEST
     // Rev�t�H���_���Ȃ��ꍇ
     // if (bRet == FALSE || bRet1 == FALSE){
        // �R�s�[
        //MoveDirectoryEvo(ToFolder, TooFolder);
        MoveDirectoryEvo1(ToFolder, TooFolder);
        G_Log_Text = "�t�H���_�̃R�s�[���܂����E from  �w " + ToFolder + " �x  to  �w " + TooFolder + " �x";
        Write_Log(G_Log_Text);

        //MoveDirectoryEvo(ToFolder1, TooFolder1);
        MoveDirectoryEvo1(ToFolder1, TooFolder1);
        G_Log_Text = "�t�H���_�̃R�s�[���܂����E from  �w " + ToFolder1 + " �x  to  �w " + TooFolder1 + " �x";
        Write_Log(G_Log_Text);

        //MoveDirectoryEvo(FormFolder, ToFolder);
        MoveDirectoryEvo1(FormFolder, ToFolder);
        G_Log_Text = "�t�H���_�̃R�s�[���܂����B from  �w " + FormFolder + " �x  to  �w " + ToFolder + " �x";
        Write_Log(G_Log_Text);

        //MoveDirectoryEvo(FormFolder1, ToFolder1);
        MoveDirectoryEvo1(FormFolder1, ToFolder1);
        G_Log_Text = "�t�H���_�̃R�s�[���܂����B from  �w " + FormFolder1 + " �x  to  �w " + ToFolder1 + " �x";
        Write_Log(G_Log_Text);


     //}else{
     //   // �㏑���R�s�[
     //   MoveDirectoryEvo(FormFolder, ToFolder);
     //   G_Log_Text = "�t�H���_�̃R�s�[���܂����B from  �w " + FormFolder + " �x  to  �w " + ToFolder + " �x";
     //   Write_Log(G_Log_Text);
     //
     //   MoveDirectoryEvo(FormFolder1, ToFolder1);
     //   G_Log_Text = "�t�H���_�̃R�s�[���܂����B from  �w " + FormFolder1 + " �x  to  �w " + ToFolder1 + " �x";
     //   Write_Log(G_Log_Text);
     //
     //}
   }
   // �t�H���_�̍폜
   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   //if(!RemoveDirectory(FormFolder.c_str())){
   //TEST
   //if(!removeDirectoryEvo(FormFolder.c_str())){
   ////*************************************
   //   G_Log_Text = "�t�H���_�̈ړ��Ɏ��s���܂����B�w " + FormFolder + " �x";
   //   Write_Log(G_Log_Text);
   //}
   //TEST

   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   //if(!RemoveDirectory(FormFolder1.c_str())){
   //TEST
   //if(!removeDirectoryEvo(FormFolder1.c_str())){
   ////*************************************
   //   G_Log_Text = "�t�H���_�̈ړ��Ɏ��s���܂����B�w " + FormFolder1 + " �x";
   //   Write_Log(G_Log_Text);
   //}
   //TEST

   // �t�H���_�̈ړ�
   G_Log_Text = "----------------�t�H���_�ړ��I��----------------";
   Write_Log(G_Log_Text);
}

//-----------------------------------------------------------------------------
// ���{��֐����F WindowsAPI��RemoveDirectory������
// �T�v        �F ̫��ޓ��S�Ă�̧�ق̈ړ�
// �p�����^    �F
// �߂�l      �F
// �@�\����  �F
// ���l        �F 2004/10/01 �ǉ�
//-----------------------------------------------------------------------------
bool __fastcall TfrmHDP010::MoveDirectoryEvo(AnsiString FromFile, AnsiString ToFile)
{
  bool retVal =true;
  AnsiString nextFileName;

  WIN32_FIND_DATA foundFile;

  G_Log_Text = "�t�H���_ Start FromFile �w " + FromFile + " �x";
  Write_Log(G_Log_Text);
  G_Log_Text = "�t�H���_ Start ToFile �w " + ToFile + " �x";
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
          //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
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

  //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
  //G_Log_Text = "�t�H���_ RemoveDirectory FromFile �w " + FromFile + " �x";
  //Write_Log(G_Log_Text);
  //
  ////Delete starting point itseft
  //if(RemoveDirectory(FromFile.c_str())==0)retVal=false;
  //*************************************

  return retVal;
}

//-----------------------------------------------------------------------------
// ���{��֐����F RemoveDirectory������
// �T�v        �F ̫��ޓ��S�Ă̍폜
// �p�����^    �F
// �߂�l      �F
// �@�\����    �F
// ���l        �F 2004/11/04 add
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
// ���{��֐����F ����쐬�t�@�C���̈ړ�
// �T�v        �F ����쐬̧��(̫��ނ�ٰĒ�����̧��)�̈ړ�
// �p�����^    �F
// �߂�l      �F
// �@�\����    �F
// ���l        �F TEST
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


   // �t�H���_�̈ړ�
   G_Log_Text = "----------------�t�H���_�ړ��J�n----------------";
   Write_Log(G_Log_Text);

   // �ړ���t�H���_�̍쐬
   //2022/12/15 �����ǉ��Ή� Mod_S
   //Work = G_SAKUBAN.SubString(1, 4);
   if (G_SAKUBAN.Length() > 7)
   {
	  Work = G_SAKUBAN.SubString(1, 6);
   }
   else
   {
	  Work = G_SAKUBAN.SubString(1, 4);
   }
   //2022/12/15 �����ǉ��Ή� Mod_E
   if( P_Sakuban_Mode == "S" )
   {
      // ��̔��f
      Sakuban = Work.ToIntDef(0);
	  if( Sakuban % 2 == 1 ){
         Sakuban --;
	  }
	  //2022/12/15 �����ǉ��Ή� Mod_S
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
	  //2022/12/15 �����ǉ��Ή� Mod_E

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
   //�}���`�T�[�o�Ή� 2009/1/13 MOD START
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

   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
   //			 + IncludeTrailingBackslash(ToFolder)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder  = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
			 + IncludeTrailingBackslash(ToFolder)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //�}���`�T�[�o�Ή� 2009/1/13 MOD END

   // HEADS_OUTPUT
   //�}���`�T�[�o�Ή� 2008/11/17 MOD START
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
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
   //			 + IncludeTrailingBackslash(ToFolder1)
   //			 + IncludeTrailingBackslash(Work)
   //			 + IncludeTrailingBackslash(G_SAKUBAN);
   ToFolder1 = IncludeTrailingBackslash(G_HEADS_OUT_Dir)
			 + IncludeTrailingBackslash(ToFolder1)
			 + IncludeTrailingBackslash(Work)
			 + IncludeTrailingBackslash(GetFileSeizouBangou(G_SAKUBAN));
   //�}���`�T�[�o�Ή� 2008/11/17 MOD END

   // ���O
   G_Log_Text = "�����t�H���_�iFormFolder�j�w" + FormFolder + "�x";
   Write_Log(G_Log_Text);
   G_Log_Text = "�����t�H���_�iFormFolder1�j�w" + FormFolder1 + "�x";
   Write_Log(G_Log_Text);
   G_Log_Text = "�����t�H���_�iToFolder�j�w" + ToFolder + "�x";
   Write_Log(G_Log_Text);
   G_Log_Text = "�����t�H���_�iToFolder1�j�w" + ToFolder1 + "�x";
   Write_Log(G_Log_Text);

   //Windows 2008Server �ڐA�Ή�
   //�p�X�̕ύX
   G_Err_Log_FileFName = ToFolder1 + G_Err_Log_FileName;

   // �R�s�[
   MoveDirectoryEvo(FormFolder, ToFolder);
   G_Log_Text = "�t�H���_�̃R�s�[���܂��� from  �w " + FormFolder + " �x  to  �w " + ToFolder + " �x";
   Write_Log(G_Log_Text);

//   MoveDirectoryEvo(FormFolder1, ToFolder1);
   MoveDirectoryEvoRev(FormFolder1, ToFolder1, sRev);
   G_Log_Text = "�t�H���_�̃R�s�[���܂��� from  �w " + FormFolder1 + " �x  to  �w " + ToFolder1 + " �x";
   Write_Log(G_Log_Text);

   // �t�H���_�̍폜
   if(!removeDirectoryEvo(FormFolder.c_str())){
      G_Log_Text = "�t�H���_�̈ړ��Ɏ��s���܂����B�w " + FormFolder + " �x";
      Write_Log(G_Log_Text);
   }

   //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
   if(!removeDirectoryEvo(FormFolder1.c_str())){
      G_Log_Text = "�t�H���_�̈ړ��Ɏ��s���܂����B�w " + FormFolder1 + " �x";
      Write_Log(G_Log_Text);
   }

   // �t�H���_�̈ړ�
   G_Log_Text = "----------------�t�H���_�ړ��I��----------------";
   Write_Log(G_Log_Text);
}



//-----------------------------------------------------------------------------
// ���{��֐����F PDF̧�وړ�����
// �T�v        �F ̧�ق̌����A��߰
// �p�����^    �F
// �߂�l      �F
// �@�\����  �F
// ���l        �F 2005/09/28 �ǉ�
//-----------------------------------------------------------------------------
void __fastcall TfrmHDP010::OutPut_File_Move1(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg)
{
   HANDLE han;
   WIN32_FIND_DATA wfd;
   bool bRet;
   AnsiString SearchFile;

   SearchFile = FromFile + Files;


   G_Log_Text = "�����t�@�C���w" + SearchFile + "�x";
   Write_Log(G_Log_Text);

   if((han=FindFirstFile((SearchFile).c_str(), &wfd))!=INVALID_HANDLE_VALUE)
   {
      do
      {
         //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
         //if ((((AnsiString)wfd.cFileName).c_str() == ".") || (((AnsiString)wfd.cFileName).c_str() == "..")) continue;
         //bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
         //if(bRet == false){
         //   G_Log_Text = "�t�@�C���̃R�s�[�Ɏ��s���܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
         //}else{
         //   G_Log_Text = "�t�@�C���̃R�s�[�ɂ��܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
         //}
         //Write_Log(G_Log_Text);
         //
         //// �t�@�C���̍폜
         //if(DelFlg){
         //   bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
         //   if(bRet == false){
         //      G_Log_Text = "�t�@�C���̍폜�Ɏ��s���܂����B�w " + FromFile + (AnsiString)wfd.cFileName + " �x";
         //      Write_Log(G_Log_Text);
         //   }
         //}
         if(strcmp(wfd.cFileName,".")!=0 && strcmp(wfd.cFileName,"..")!=0){
            bRet = CopyFile((FromFile + (AnsiString)wfd.cFileName).c_str(), (ToFile + (AnsiString)wfd.cFileName).c_str() , false);
            if(bRet == false){
               G_Log_Text = "�t�@�C���̃R�s�[�Ɏ��s���܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
            }else{
               G_Log_Text = "�t�@�C���̃R�s�[�ɂ��܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + (AnsiString)wfd.cFileName + " �x";
            }
            Write_Log(G_Log_Text);

            // �t�@�C���̍폜
//          if(DelFlg){
//             bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
//             if(bRet == false){
//                G_Log_Text = "�t�@�C���̍폜�Ɏ��s���܂����B�w " + FromFile + (AnsiString)wfd.cFileName + " �x";
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
// ���{��֐����F WindowsAPI��RemoveDirectory������
// �T�v        �F ̫��ޓ��S�Ă�̧�ق̈ړ�
// �p�����^    �F
// �߂�l      �F
// �@�\����  �F
// ���l        �F 2005/09/28 �ǉ�
//-----------------------------------------------------------------------------
bool __fastcall TfrmHDP010::MoveDirectoryEvo1(AnsiString FromFile, AnsiString ToFile)
{
  bool retVal =true;
  AnsiString nextFileName;

  WIN32_FIND_DATA foundFile;

  G_Log_Text = "�t�H���_ Start FromFile �w " + FromFile + " �x";
  Write_Log(G_Log_Text);
  G_Log_Text = "�t�H���_ Start ToFile �w " + ToFile + " �x";
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
          //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
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

  //2004.11.16 �o�̓t�H���_�y�ђ�����ύX
  //G_Log_Text = "�t�H���_ RemoveDirectory FromFile �w " + FromFile + " �x";
  //Write_Log(G_Log_Text);
  //
  ////Delete starting point itseft
  //if(RemoveDirectory(FromFile.c_str())==0)retVal=false;
  //*************************************

  return retVal;
}


//---------------------------------------------------------------------------
// ���{��֐����F  ���͗e�틭�x�v�Z���쐬��۸���  �N��
// �T  �v      �F �wAturyoku.EXE�x���N������
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2009.03.03�ǉ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::Aturyoku_Run(void)
{
    int i_KikakuCode;             // �K�i����
    AnsiString s_Text;            // �ėp������

    i_KikakuCode = G_HEADS_DATA[29].ToIntDef(0);

    if( G_Output_Mode == 1 )
    {
        G_Log_Text = "������ �yG_Output_Mode == 1�zExcel�o�̓e�X�g�N�� ������";
        Write_Log(G_Log_Text);

        s_Text = G_SAKUBAN;
        spawnl(P_WAIT, "Aturyoku.EXE", "Aturyoku.EXE", s_Text, NULL);

    }
    else
    {
        G_Log_Text = "������ �yG_Output_Mode == 1�z�{�ԉғ� ������";
        Write_Log(G_Log_Text);

        if (P_UtiKbn == "3" || P_UtiKbn == "6"){
            //********************************
            //***  �ŏo���敪�@�w1�x,�w6�x
            //********************************

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

            s_Text = G_SAKUBAN;
            spawnl(P_WAIT, "Aturyoku.EXE", "Aturyoku.EXE", s_Text, NULL);

        } else if (P_UtiKbn == "4" && P_Sakuban_Mode == "S") {
            //**************************
            //***  �ŏo���敪�w4�x
            //**************************

            // ���͗e�픻��
            // 2018/11/19 NK,JG�ǉ�_S
            ////// 2018.05.02 JBA�ǉ�_S
            //////2014.04.18 ���x�v�Z�����ύX
            //////if (i_KikakuCode == 1 ||i_KikakuCode == 2 ||i_KikakuCode == 3 ||i_KikakuCode == 27){
            ////if (i_KikakuCode == 1 ||i_KikakuCode == 2 ||i_KikakuCode == 3 ||
            ////    i_KikakuCode == 16 ||i_KikakuCode == 27){
            //////***************************
            //if (i_KikakuCode == 1 ||i_KikakuCode == 2 ||i_KikakuCode == 3 ||
            //    i_KikakuCode == 16 ||i_KikakuCode == 25||i_KikakuCode == 27){
            //// 2018.05.02 JBA�ǉ�_E
            if (i_KikakuCode == 1 || i_KikakuCode == 2 || i_KikakuCode == 3 ||
                i_KikakuCode == 11 || i_KikakuCode == 15 ||i_KikakuCode == 16 ||
                i_KikakuCode == 25||i_KikakuCode == 27){
            // 2018/11/19 NK,JG�ǉ�_E
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

                s_Text = G_SAKUBAN;
                spawnl(P_WAIT, "Aturyoku.EXE", "Aturyoku.EXE", s_Text, NULL);

            }

        }

    }

    return true;
}


//-----------------------------------------------------------------------------
// ���{��֐����F ���r�W�����ԍ��̒ǉ��Ńt�H���_�̃R�s�[
// �T�v        �F ̫��ޓ��S�Ă�̧�ق̈ړ�
// �p�����^    �F
// �߂�l      �F
// �@�\����  �F
// ���l        �F 2015/10/16 �ǉ�
//-----------------------------------------------------------------------------
bool __fastcall TfrmHDP010::MoveDirectoryEvoRev(AnsiString FromFile, AnsiString ToFile, AnsiString sRev)
{
  bool retVal =true;
  AnsiString nextFileName;

  WIN32_FIND_DATA foundFile;

  G_Log_Text = "�t�H���_ Start FromFile �w " + FromFile + " �x";
  Write_Log(G_Log_Text);
  G_Log_Text = "�t�H���_ Start ToFile �w " + ToFile + " �x";
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
// ���{��֐����F ���r�W�����ԍ��̒ǉ���̧�وړ�����
// �T�v        �F ̧�ق̌����A��߰
// �p�����^    �F
// �߂�l      �F
// �@�\����  �F
// ���l        �F 2015/10/16 �ǉ�
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


   G_Log_Text = "�����t�@�C���w" + SearchFile + "�x";
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
               G_Log_Text = "�t�@�C���̃R�s�[�Ɏ��s���܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + DestFileName + " �x";
            }else{
               G_Log_Text = "�t�@�C���̃R�s�[�ɂ��܂����Bfrom  �w " + FromFile + (AnsiString)wfd.cFileName + " �x  to  �w " + ToFile + DestFileName + " �x";
            }
            Write_Log(G_Log_Text);

            // �t�@�C���̍폜
            if(DelFlg){
               bRet = DeleteFile((FromFile + (AnsiString)wfd.cFileName).c_str());
               if(bRet == false){
                  G_Log_Text = "�t�@�C���̍폜�Ɏ��s���܂����B�w " + FromFile + (AnsiString)wfd.cFileName + " �x";
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
// ���{��֐����F  �h�����匟���[�쐬��۸���  �N��
// �T  �v      �F �wTosoJisyuKensa.EXE�x���N������
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2017.10.19�ǉ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::TosoJisyuKensa_Run(void)
{
    int i_TosoJisyuKensa;         // �N���t���O
    int i_RecNo;
    AnsiString s_Text1;           // �����ԍ�
    AnsiString s_Text2;           // �^��
    AnsiString s_Text3;           // �䐔
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
        //***  �����ԍ�
        //*********************
        s_Text1 = G_SAKUBAN;

        //*********************
        //***  �^    ��
        //*********************
        i_RecNo = 33;
        if (G_HEADS_DATA[1057].TrimRight() == "1"){
            s_Text2 = G_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
        } else {
            s_Text2 = G_HEADS_DATA[i_RecNo].TrimRight();
        }

        //*******************
        //***  ��    ��
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
        G_Log_Text = "������ �yG_Output_Mode == 1�zExcel�o�̓e�X�g�N�� ������";
        Write_Log(G_Log_Text);

        spawnl(P_WAIT, "TosoJisyuKensa.EXE", "TosoJisyuKensa.EXE", s_Text1, s_Text2, s_Text3, s_Text4, NULL);

    }
    else if( i_TosoJisyuKensa == 2 )
    {
        G_Log_Text = "������ �yG_Output_Mode == 0�z�{�ԉғ� ������";
        Write_Log(G_Log_Text);

        // ۸ލ쐬
        G_Log_Text = "----------------------------------------------------------";
        Write_Log(G_Log_Text);
        Write_Error_Log(G_Log_Text);
        G_Log_Text = "----------      �w�h�����匟���[�x�쐬�J�n      ----------";
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
// ���{��֐����F  ���������o����۸���  �N��
// �T  �v      �F �wHotAirDrying_Run.EXE�x���N������
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F 2020.04.20�ǉ�
//---------------------------------------------------------------------------
bool __fastcall TfrmHDP010::HotAirDrying_Run(void)
{
    int i_HotAirDrying;           // �N���t���O
    int i_RecNo;
    AnsiString s_Toki1;           // ���L�R�[�h1
    AnsiString s_Toki2;           // ���L�R�[�h2
    AnsiString s_Toki3;           // ���L�R�[�h3
    AnsiString s_Toki4;           // ���L�R�[�h4
    AnsiString s_Toki5;           // ���L�R�[�h5
    AnsiString s_Toki6;           // ���L�R�[�h6
    AnsiString s_Toki7;           // ���L�R�[�h7
    AnsiString s_Toki8;           // ���L�R�[�h8
    AnsiString s_Toki9;           // ���L�R�[�h9
    AnsiString s_Toki10;          // ���L�R�[�h10
    AnsiString s_Toki11;          // ���L�R�[�h11
    AnsiString s_Toki12;          // ���L�R�[�h12
    AnsiString s_Toki13;          // ���L�R�[�h13
    AnsiString s_Toki14;          // ���L�R�[�h14
    AnsiString s_BMat;            // B���[�Ǎގ�
    AnsiString s_AMat;            // A���[�Ǎގ�
    AnsiString s_BAtu;            // B������
    AnsiString s_AAtu;            // A������
    AnsiString s_Sakuban;         // �����ԍ�
    AnsiString s_Kubun;           // �敪
    bool b_BOnpu;                 // B����������
    bool b_AOnpu;                 // A����������


    //***********************
    //***  ���L�R�[�h1
    //***********************
    i_RecNo = 451;
    s_Toki1 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h2
    //***********************
    i_RecNo = 454;
    s_Toki2 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h3
    //***********************
    i_RecNo = 457;
    s_Toki3 = G_HEADS_DATA[i_RecNo].TrimRight();

     //***********************
    //***  ���L�R�[�h4
    //***********************
    i_RecNo = 460;
    s_Toki4 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h5
    //***********************
    i_RecNo = 463;
    s_Toki5 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h6
    //***********************
    i_RecNo = 466;
    s_Toki6 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h7
    //***********************
    i_RecNo = 469;
    s_Toki7 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h8
    //***********************
    i_RecNo = 472;
    s_Toki8 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h9
    //***********************
    i_RecNo = 475;
    s_Toki9 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h10
    //***********************
    i_RecNo = 478;
    s_Toki10 = G_HEADS_DATA[i_RecNo].TrimRight();

      //***********************
    //***  ���L�R�[�h11
    //***********************
    i_RecNo = 481;
    s_Toki11 = G_HEADS_DATA[i_RecNo].TrimRight();

     //***********************
    //***  ���L�R�[�h12
    //***********************
    i_RecNo = 484;
    s_Toki12 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h13
    //***********************
    i_RecNo = 487;
    s_Toki13 = G_HEADS_DATA[i_RecNo].TrimRight();

    //***********************
    //***  ���L�R�[�h14
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
        s_BMat = Type_MAT(s_BMat);            // B���[�Ǎގ�
        i_RecNo = 1061;
        s_AMat = G_HEADS_DATA[i_RecNo].SubString(1,4);
        s_AMat = Type_MAT(s_AMat);            // A���[�Ǎގ�
        i_RecNo = 827;
        s_BAtu = G_HEADS_DATA[i_RecNo].Trim(); // B������
        i_RecNo = 832;
        s_AAtu = G_HEADS_DATA[i_RecNo].Trim(); // A������

        //B����������
        if ( s_BMat == "STPG" || s_BMat == "SGP" || s_BMat == "CS" ) {
            // CS�ݸޒ[��
            if (StrToFloat(s_BAtu) > 0 ) {
                // ��������
                b_BOnpu = true;  // B����������

            } else {
                // �󈳎���
                b_BOnpu = false; // B�����������s�v

            }
        } else {
            b_BOnpu = false;     // B�����������s�v

        }

        //A����������
        if ( s_AMat == "STPG" || s_AMat == "SGP" || s_AMat == "CS" ) {
            // CS�ݸޒ[��
            if (StrToFloat(s_AAtu) > 0 ) {
                // ��������
                b_AOnpu = true;  // A����������

            } else {
                // �󈳎���
                b_AOnpu = false; // A�����������s�v

            }
        } else {
            b_AOnpu = false;     // A�����������s�v

        }

        if ( b_BOnpu == true && b_AOnpu == false ) {
            // B����������
            i_RecNo = 137;
            if ( G_HEADS_DATA[i_RecNo].TrimRight().ToIntDef(0) == 1 ) { // �`�a�t
                // 2020.06.24 �o�͓��L�R�[�h�ύX_S
                //// �ቷ����������
                //s_Kubun = "1N";
                // ��������������
                s_Kubun = "1M";
                // 2020.06.24 �o�͓��L�R�[�h�ύX_E
                i_HotAirDrying = 2;

            } else {
                // ��������������
                s_Kubun = "1M";
                i_HotAirDrying = 2;

            }
        } else if ( b_BOnpu == false && b_AOnpu == true ) {
            // A����������
            if ( G_HEADS_DATA[i_RecNo].TrimRight().ToIntDef(0) == 1 ) { // �`�a�t
                // ��������������
                s_Kubun = "1M";
                i_HotAirDrying = 2;

            } else {
                // 2020.06.24 �o�͓��L�R�[�h�ύX_S
                //// �ቷ����������
                //s_Kubun = "1N";
                // ��������������
                s_Kubun = "1M";
                // 2020.06.24 �o�͓��L�R�[�h�ύX_E
                i_HotAirDrying = 2;

            }
        } else if ( b_BOnpu == true && b_AOnpu == true ) {
            // 2020.06.24 �o�͓��L�R�[�h�ύX_S
            //// ������������
            //s_Kubun = "1P";
            // ��������������
            s_Kubun = "1M";
            // 2020.06.24 �o�͓��L�R�[�h�ύX_E
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
        //***  �����ԍ�
        //*********************
        s_Sakuban = G_SAKUBAN;

    }

     if( i_HotAirDrying == 1 )
    {
        G_Log_Text = "������ �yG_Output_Mode == 1�zExcel�o�̓e�X�g�N�� ������";
        Write_Log(G_Log_Text);
        G_Log_Text = "������ ���������v���O�����N�� ������";
        Write_Log(G_Log_Text);

        spawnl(P_WAIT, "./HotAirDrying/HotAirDrying.EXE", "./HotAirDrying/HotAirDrying.EXE", s_Sakuban, s_Kubun, NULL);

    }
    else if( i_HotAirDrying == 2 )
    {
        G_Log_Text = "������ �yG_Output_Mode == 0�z�{�ԉғ� ������";
        Write_Log(G_Log_Text);
        G_Log_Text = "������ ���������v���O�����N�� ������";
        Write_Log(G_Log_Text);

        spawnl(P_WAIT, "./HotAirDrying/HotAirDrying.EXE", "./HotAirDrying/HotAirDrying.EXE", s_Sakuban, s_Kubun, NULL);

    }

    return true;
}

// ---------------- <<< End of File >>> ---------------- //

