//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���F�}�\��  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.05.30
//�@ �X �V ���F2007.10.09
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <io.h>

#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_Syonin.h"
#define  ZUMENNO�@'aaaa'

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//2018.03.12 OGI ADD_S
//---------------------------------------------------------------------------
// ���{��֐����F ���}�̐}�Ԃ��擾����
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [���}�̐}��]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
AnsiString __fastcall GetMeibanZuban(void)
{
   AnsiString sRet = "";
   char  buf[1024];                            /* ������o�b�t�@   */
   char  *tok;                                 /* �g�[�N���|�C���^ */
   int   i;                                    /* ���[�v�J�E���^   */
   char  token[100][100];                      /* �g�[�N���o�b�t�@ */

   AnsiString P_PARTS[17]; // ���i�f�[�^
   AnsiString wk_FilePass;
   FILE *fp;

   //***************************
   //**  ���i�W�J�ް��Ǎ�
   //***************************
   if (AllNumChk(G_SAKUBAN.c_str(),0) == 0)
   {
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ORDER_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
   } else {
      wk_FilePass = IncludeTrailingBackslash(G_HEADS_DATA_Dir)
                  + IncludeTrailingBackslash(G_DAT_ESTIM_Dir)
                  + IncludeTrailingBackslash(G_SAKUBAN)
                  + "HD_PARTS.csv";
   }

   if ((fp = fopen(wk_FilePass.c_str(),"r")) == NULL){
      fclose(fp);    // ����
      return sRet;
   }

   //*****************
   // �Ǎ��݉\�̊�
   //*****************
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

      //*****************************************
      //***  ���i�f�[�^(HD_PARTS.csv)�̎擾
      //***  (���i����)
      //*****************************************
      P_PARTS[0] = token[0];     // ���i����
      P_PARTS[1] = token[1];     // ���ʎq
      P_PARTS[2] = token[2];     // ���i�^��
      P_PARTS[3] = token[3];     // ���i����
      P_PARTS[4] = token[4];     // �ގ��^��
      P_PARTS[5] = token[5];     // �}��
      P_PARTS[6] = token[6];     // ��}�p�}��
      P_PARTS[7] = token[7];     // �P�䕪����
      P_PARTS[8] = token[8];     // �\����
      P_PARTS[9] = token[9];     // ���v
      P_PARTS[10] = token[10];   // �d�l�P
      P_PARTS[11] = token[11];   // �d�l�Q
      P_PARTS[12] = token[12];   // �d�l�R
      P_PARTS[13] = token[13];   // �d�l�S
      P_PARTS[14] = token[14];   // �d�l�T
      P_PARTS[15] = token[15];   // �ގ�����
      P_PARTS[16] = token[16];   // �d�l����(""/"A"/"B")

      //---------------------------------------------------
      //  �����i
      //---------------------------------------------------
      if( (P_PARTS[0] == "ACCS") && (P_PARTS[1] == "MEIBAN-") )
      {
        sRet = P_PARTS[5];
        break;
      }
   }

   fclose(fp);    // ����
   return sRet;
}
//***************************************************************************
//2018/03/12 OGI ADD_E

//---------------------------------------------------------------------------
// ���{��֐����F ���F�}�\���@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Syonin(HeadsData* pcHeadsData)
{
   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_UtiKbn;          // �ŏo���敪
   AnsiString s_Text;            // �ėp������
   int        i_RecNo;           // HEADS�ް� ں���No
   //int        i_BuhinNo;         // �i��
   int        i_Pos;
   // 2010.08.19 ������Ή�
   AnsiString s_China;           // ����敪("0":���{��A"1":������)
   // *********************

   bool bRet;
   AnsiString s_HEADS_DATA[1500];

   // ���d�l�łȂ��ꍇ�͏����s�v(����I��)
   if(!pcHeadsData->GetSpecType()==DEF_SPEC_TYPE_NONE) return(true);

   // HEADS�ް���۰�ٕϐ��ɾ��
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return(false);
   // 2010.08.19 ������Ή�
   //s_Tmplate_Pass = G_Template_Dir + "���F�}�\��.xls";               // ����ڰ�̧��
   i_RecNo = 851;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "1" ) {
      s_China = "1";                                                 // ������
      // 2014.10.01 �k���A�L�B�x�X�ǉ�
      //s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C.xls";           // ����ڰ�̧��
      if (s_HEADS_DATA[4].TrimRight() == "C1") {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C1.xls";       // ����ڰ�̧��
      } else if (s_HEADS_DATA[4].TrimRight() == "C2") {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C2.xls";       // ����ڰ�̧��
      } else if (s_HEADS_DATA[4].TrimRight() == "C3") {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C3.xls";       // ����ڰ�̧��
      } else {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C.xls";        // ����ڰ�̧��
      }
      // *****************************
   // 2014.04.07 ���É��x�X�ǉ�
   } else if (s_HEADS_DATA[4].TrimRight() == "N") {
      //2114.05.30 ����C��
      //s_China = "N";                                                 // ���{��
      s_China = "0";                                                 // ���{��
      //*******************
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��N.xls";           // ����ڰ�̧��
   // ***************************
   // 2019.04.08 �k�C���c�Ə��ǉ�_S
   } else if (s_HEADS_DATA[4].TrimRight() == "D") {
      s_China = "0";                                                 // ���{��
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��D.xls";           // ����ڰ�̧��
   // 2019.04.08 �k�C���c�Ə��ǉ�_E
   // 2021.10.18 �����c�Ə��ǉ�_S
   } else if (s_HEADS_DATA[4].TrimRight() == "S") {
      s_China = "0";                                                 // ���{��
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��S.xls";           // ����ڰ�̧��
   // 2021.10.18 �����c�Ə��ǉ�_E
   // 2022.04.12 ��t�c�Ə��ǉ�_S
   } else if (s_HEADS_DATA[4].TrimRight() == "C") {
      s_China = "0";                                                 // ���{��
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��CH.xls";           // ����ڰ�̧��
   // 2022.04.12 ��t�c�Ə��ǉ�_E
   // 2014.10.01 �k���A�L�B�x�X�ǉ�
   } else if (s_HEADS_DATA[4].TrimRight() == "C1") {
      s_China = "1";                                                 // ������
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C1.xls";          // ����ڰ�̧��
   } else if (s_HEADS_DATA[4].TrimRight() == "C2") {
      s_China = "1";                                                 // ������
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C2.xls";          // ����ڰ�̧��
   } else if (s_HEADS_DATA[4].TrimRight() == "C3") {
      s_China = "1";                                                 // ������
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C3.xls";          // ����ڰ�̧��
   // *****************************
   } else {
      s_China = "0";                                                 // ���{��
      s_Tmplate_Pass = G_Template_Dir + "���F�}�\��.xls";            // ����ڰ�̧��
   }
   // *********************
   // 2011.05.30 ��n�p���F�}
   // 2012.01.20 ��n�t���O
   //i_RecNo = 5;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // ������
   //   s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C.xls";           // ����ڰ�̧��
   //}
   //i_RecNo = 401;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // ������
   //   s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C.xls";           // ����ڰ�̧��
   //}
   i_RecNo = 1107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "CHINA" ) {
      s_China = "1";                                                 // ������
      // 2014.10.01 �k���A�L�B�x�X�ǉ�
      //s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C.xls";           // ����ڰ�̧��
      if (s_HEADS_DATA[4].TrimRight() == "C1") {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C1.xls";       // ����ڰ�̧��
	  } else if (s_HEADS_DATA[4].TrimRight() == "C2") {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C2.xls";       // ����ڰ�̧��
      } else if (s_HEADS_DATA[4].TrimRight() == "C3") {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C3.xls";       // ����ڰ�̧��
      } else {
         s_Tmplate_Pass = G_Template_Dir + "���F�}�\��C.xls";        // ����ڰ�̧��
      }
      // *****************************
   }
   // *********************
   // ***********************
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "AMP" + G_SAKUBAN + ".xls"; // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "AMP" + GetFileSeizouBangou(G_SAKUBAN) + ".xls"; // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���F�}�\���x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   G_Log_Text = "�쐬�t�@�C�����F     �w" + s_MakeExcel_Pass + "�x";
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
      G_Log_Text = "�t�@�C���R�s�[�����B �w" + s_Tmplate_Pass +
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
      //Windows 2008Server �ڐA�Ή�
      //exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));
      exWorksheets = exWorkbook.OlePropertyGet("WorkSheets");
   }
   catch (...) {
      G_ErrLog_Text = "�w" + s_MakeExcel_Pass + "�x�̼�ĺڸ��݂̎擾�Ɏ��s���܂����I�I";
      Write_Error_Log(G_ErrLog_Text);
      return false;
   }

   //***********************************************************************************
   //***                                                                             ***
   //***                    ���F�}�\���i�a���j�@�f�[�^�Z�b�g                         ***
   //***                                                                             ***
   //***********************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("item") << "�a��");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�a��"));

   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);

   //***************************
   //***  �a���F�^�C�g��
   //***************************
   // �wA001�x
   // 2010.08.19 ������Ή�
   //if (s_UtiKbn == "1"){
   //   s_Text = "��  ��  �}";
   //
   //} else if (s_UtiKbn == "2"){
   //   s_Text = "��  �F  �}";
   //
   //} else if (s_UtiKbn == "3"){
   //   s_Text = "�m  ��  �}";
   //
   //} else if (s_UtiKbn == "4"){
   //   s_Text = "��  ��  �}";
   //
   //} else if (s_UtiKbn == "5"){
   //   s_Text = "�[  ��  �d  �l  ��";
   //
   //} else if (s_UtiKbn == "6"){
   //   s_Text = "�H  ��  �}";
   // 2010.08.19 ������Ή�
   if ( s_China == "0" && s_UtiKbn == "1" ) {
      s_Text = "��  ��  �}";
   } else if ( s_China == "0" && s_UtiKbn == "2" ) {
      s_Text = "��  �F  �}";
   } else if ( s_China == "0" && s_UtiKbn == "3" ) {
      s_Text = "�m  ��  �}";
   } else if ( s_China == "0" && s_UtiKbn == "4" ) {
      s_Text = "��  ��  �}";
   } else if ( s_China == "0" && s_UtiKbn == "5" ) {
      s_Text = "�[  ��  �d  �l  ��";
   } else if ( s_China == "0" && s_UtiKbn == "6" ) {
      s_Text = "�H  ��  �}";
   } else if ( s_China == "1" && s_UtiKbn == "1" ) {
      s_Text = "��  �e  /U+22270/U+";
   } else if ( s_China == "1" && s_UtiKbn == "2" ) {
      // 2011.05.30 ��n�p���F�}
      //s_Text = "�j  �y  /U+22270/U+";
      s_Text = "/U+35748/U+  ��  /U+22270/U+";
      // ***********************
   } else if ( s_China == "1" && s_UtiKbn == "3" ) {
      s_Text = "/U+30830/U+  ��  /U+22270/U+";
   } else if ( s_China == "1" && s_UtiKbn == "4" ) {
      s_Text = "��  ��  /U+22270/U+";
   } else if ( s_China == "1" && s_UtiKbn == "5" ) {
      // 2011.05.30 ��n�p���F�}
      //s_Text = "��  /U+32435/U+  /U+35268/U+  �i  /U+35828/U+  ��  /U+20070/U+";
      s_Text = "/U+35774/U+  /U+22791/U+  /U+35268/U+  �i  /U+20070/U+";
      // ***********************
   } else if ( s_China == "1" && s_UtiKbn == "6" ) {
      s_Text = "�{  �H  /U+22270/U+";
   // *********************
   } else {
      G_ErrLog_Text = "�s���ȑŏo���敪�w" + s_UtiKbn + "�x ���F�}�쐬�������I�����܂��B";
      Write_Error_Log(G_ErrLog_Text);

      // ��޼ު�ďI��
      //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
      //Windows 2008Server �ڐA�Ή�
      exApplication.OlePropertySet("DisplayAlerts", false);
      exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
      exWorkbook.OleFunction("Close");
      exWorkbooks.OleFunction("Close");

      return false;
   }

   Excel_Cell_DataSet(1, 3, s_Text);
   G_Log_Text = "�a���F�^�C�g��       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F��i���t
   //***************************
   // �wA002�x
// ���t�͋L�����Ȃ��@2002/11/23
//   s_Text = FormatDateTime("yyyy",Date()) + "�N"
//          + FormatDateTime("mm",Date()) + "��"
//          + FormatDateTime("dd",Date()) + "��";
//   Excel_Cell_DataSet(3, 14, s_Text);
//   G_Log_Text = "�a���F��i���t       �w" + s_Text + "�x���Z�b�g�B";
//   Write_Log(G_Log_Text);
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   // 2019.02.22 �N�����ǉ�_S�@
   if ( s_China == "1" ) {
      //����
   } else {
      if ( s_UtiKbn == "1" ) {
         //��  ��  �}
      } else if ( s_UtiKbn == "2" ) {
         //��  �F  �}
      } else if ( s_UtiKbn == "3" ) {
         //�m  ��  �}
      } else if ( s_UtiKbn == "4" ) {
         //��  ��  �}
      } else if ( s_UtiKbn == "5" ) {
         //�[  ��  �d  �l  ��
         Excel_Cell_DataSet(3, 14, s_Text);
         G_Log_Text = "�a���F�N�����Q        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else if ( s_UtiKbn == "6" ) {
         //�H  ��  �}
      }
   }
   // 2019.02.22 �N�����ǉ�_E�@

   //***************************
   //***  �a���F������
   //***************************
   // �wA003�x
   i_RecNo = 1063;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(5, 3, s_Text);
   G_Log_Text = "�a���F������          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F�[����
   //***************************
   // �wA004�x
   i_RecNo = 1065;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(6, 3, s_Text);
   G_Log_Text = "�a���F�[����          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F�����ԍ�
   //***************************
   // �wA005�x
   // 2017.10.06 �����ԍ��폜_S
   //i_RecNo = 1260;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(7, 3, s_Text);
   //G_Log_Text = "�a���F�����ԍ�        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   // 2017.10.06 �����ԍ��폜_E

   //***************************
   //***  �a���F���u�ԍ�
   //***************************
   // �wA006�x
   i_RecNo = 28;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(9, 3, s_Text);
   G_Log_Text = "�a���F���u�ԍ�        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // ���p�̏ꍇ�́A�u���u�ԍ��v���u�D�ԁv�ɒu��������
      //Excel_Cell_DataSet(9, 2, "�D��");
      // 2010.08.19 ������Ή�
      if ( s_China == "0" ) {
          Excel_Cell_DataSet(9, 2, "�D��");
      } else {
          Excel_Cell_DataSet(9, 2, "�D��");
      }
      // *********************
   }

   //***************************
   //***  �a���F�@�햼��
   //***************************
   // �wA007�x
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(10, 3, s_Text);
   G_Log_Text = "�a���F�@�햼��        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F�@��ԍ�
   //***************************
   // �wA008�x
   i_RecNo = 27;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(11, 3, s_Text);
   G_Log_Text = "�a���F�@��ԍ�        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F�����ԍ�
   //***************************
   // �wA009�x
   if(G_SAKUBAN.SubString(1,2).ToIntDef(999) != 999 ) {
     //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
     //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
     s_Text = GetSeizouBangou(G_SAKUBAN);
     //2022/12/15 �����ԍ������ǉ��Ή� Mod_E     

     //----------------------------------------------
     // 2007.10.09 �װ�d�l�̏ꍇ�A������"A/B"��ǉ�
     if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
     }
     // 2007.10.09
     //----------------------------------------------

     Excel_Cell_DataSet(13, 3, s_Text);
     G_Log_Text = "�a���F�����ԍ�       �w" + s_Text + "�x���Z�b�g�B";
     Write_Log(G_Log_Text);
   }

   //***************************
   //***  �a���F�^��
   //***************************
   // �wA010�x
   i_RecNo = 33;
   if (s_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
   } else {
      s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(15, 3, s_Text);
   G_Log_Text = "�a���F�^��            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �a���F�䐔
   //***************************
   // �wA011�x
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(16, 3, s_Text);
      G_Log_Text = "�a���F�䐔            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�a���F�䐔           RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }
   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // ���p�̏ꍇ�́A�u��o�v���u��^�ǁv�ɒu��������
      Excel_Cell_DataSet(16, 7, "��^��");
   }
   //***************************
   //***  �a���F�����M��
   //***************************
   // �wA012�x
   if (s_HEADS_DATA[20].TrimRight() == "19"){
      i_RecNo = 107;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(17, 3, s_Text);
      G_Log_Text = "�a���F�����M��        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
      // �P�ʂ�u��������
      s_Text = s_HEADS_DATA[192];
      if ( s_Text.ToIntDef(0) >= 1 && s_Text.ToIntDef(0) <= 4 ) {
        Excel_Cell_DataSet(17, 7, "kJ");
      }

   } else {
      s_Text = "";
      Excel_Cell_DataSet(17, 2, s_Text);
      Excel_Cell_DataSet(17, 3, s_Text);
      Excel_Cell_DataSet(17, 7, s_Text);
   }

   //***************************
   //***  �a���F�c�ƒS��(���)
   //***************************
   // �wA013�x
   // 2010.08.19 ������Ή�
   if ( s_China == "1" ) {
      G_Log_Text = "�����p�Ȃ̂ŉc�ƒS���̓Z�b�g���Ȃ��B";
      Write_Log(G_Log_Text);
   //if ((s_HEADS_DATA[4].TrimRight() == "O") || (s_HEADS_DATA[4].TrimRight() == "E")) {
   } else if ((s_HEADS_DATA[4].TrimRight() == "O") || (s_HEADS_DATA[4].TrimRight() == "E")) {
   // *********************
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(13, 15, s_Text);
      G_Log_Text = "�a���F�c�ƒS��(���)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      i_RecNo = 4;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(13, 15, "");
      G_Log_Text = "�a���F�c�ƒS��(���)  �n��敪 RecNo:" + FormatFloat("0000",i_RecNo) +
                   "���w" + s_Text + "�x�Ȃ̂Ńu�����N���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   //***************************
   //***  �a���F�c�ƒS��(����)
   //***************************
   // �wA014�x
   // 2010.08.19 ������Ή�
   if ( s_China == "1" ) {
      G_Log_Text = "�����p�Ȃ̂ŉc�ƒS���̓Z�b�g���Ȃ��B";
      Write_Log(G_Log_Text);
   //if (s_HEADS_DATA[4].TrimRight() == "T"){
   } else if (s_HEADS_DATA[4].TrimRight() == "T"){
   // *********************
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "�a���F�c�ƒS��(����)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // 2014.05.30 ���É��x�X�ǉ�
   } else if (s_HEADS_DATA[4].TrimRight() == "N"){
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "�a���F�c�ƒS��(���É�)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // *************************
   // 2019.04.08 �k�C���c�Ə��ǉ�_S
   } else if (s_HEADS_DATA[4].TrimRight() == "D") {
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "�a���F�c�ƒS��(�k�C��)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // 2019.04.08 �k�C���c�Ə��ǉ�_E
   // 2022.04.12 ��t�c�Ə��ǉ�_S
   } else if (s_HEADS_DATA[4].TrimRight() == "S") {
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "�a���F�c�ƒS��(����)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else if (s_HEADS_DATA[4].TrimRight() == "C") {
      i_RecNo = 3;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, s_Text);
      G_Log_Text = "�a���F�c�ƒS��(��t)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   // 2022.04.12 ��t�c�Ə��ǉ�_E
   } else {
      i_RecNo = 4;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(20, 15, "");
      G_Log_Text = "�a���F�c�ƒS��(����)  �n��敪 RecNo:" + FormatFloat("0000",i_RecNo) +
                   "���w" + s_Text + "�x�Ȃ̂Ńu�����N���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   // 2010.02.10 �G���[���L�ǉ�
   //***************************
   //*** �a���F���L(�װ)
   //***************************
   // �wA015�x
   i_RecNo = 1021;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.SubString(1,2) == "ER" ) {
       s_Text = "�G���[�F"+ s_Text;
       Excel_Cell_DataSet(2, 2, s_Text);
       G_Log_Text = "���L(�װ)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
       Write_Log(G_Log_Text);
   }
   // *************************

   // 2019.10.10 �߰�ؽĴװ_S
   //***************************
   //*** �a���F���L(�߰�ؽĴװ)
   //***************************
   // �wA016�x
   i_RecNo = 1123;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text != "" ) {
       Excel_Cell_DataSet(3, 2, s_Text);
       G_Log_Text = "���L(�߰�ؽĴװ)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
       Write_Log(G_Log_Text);
   }
   // 2019.10.10 �߰�ؽĴװ_E

   // B��
   //i_BuhinNo = 0;

   //********************************
   //***  �a���@�o�͐}�ʃZ�b�g
   //********************************
   s_UtiKbn = s_HEADS_DATA[1259].SubString(2,1);

   // 2018.07.26 �ł��o���敪�ύX_S
   //if (s_UtiKbn == "0" || s_UtiKbn == "9"){
   //   //***************  �ł��o���敪�@�O�C�X  **********************
   if (s_UtiKbn == "0"){
      //***************  �ł��o���敪�@�O  **********************
   // 2018.07.26 �ł��o���敪�ύX_E

      SyuturyokuZumen_Set('A', "J", pcHeadsData);     // �o�̓p�^�[���`

   } else if (s_UtiKbn == "1"){
      //***************  �ł��o���敪�@�P  **********************
      SyuturyokuZumen_Set('B', "J", pcHeadsData);     // �o�̓p�^�[���a

   } else if (s_UtiKbn == "2"){
      //***************  �ł��o���敪�@�Q  **********************

      SyuturyokuZumen_Set('C', "J", pcHeadsData);     // �o�̓p�^�[���b

   } else if (s_UtiKbn == "3"){
      //***************  �ł��o���敪�@�R  **********************

      if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
         // ����
         SyuturyokuZumen_Set('E', "J", pcHeadsData);  // �o�̓p�^�[���d
      } else {
         // �t
         SyuturyokuZumen_Set('D', "J", pcHeadsData);  // �o�̓p�^�[���c
      }

   } else if (s_UtiKbn == "4"){
      //***************  �ł��o���敪�@�S  **********************
      s_Text = s_HEADS_DATA[29].TrimRight();   // ���͗e��i�K�i���ށj

      if (s_HEADS_DATA[327].TrimRight() == ""){
         // ��������
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // ���p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('G', "J", pcHeadsData);  // �o�̓p�^�[���f
               } else {
                  // �t
                  SyuturyokuZumen_Set('F', "J", pcHeadsData);  // �o�̓p�^�[���e
               }
            } else {
               SyuturyokuZumen_Set('C', "J", pcHeadsData);     // �o�̓p�^�[���b
            }
         } else {
            // �񔕗p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('I', "J", pcHeadsData);  // �o�̓p�^�[���h
               } else {
                  // �t
                  SyuturyokuZumen_Set('H', "J", pcHeadsData);  // �o�̓p�^�[���g
               }
            } else {
               SyuturyokuZumen_Set('J', "J", pcHeadsData);     // �o�̓p�^�[���i
            }
         }

      } else {
         // �����L��
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // ���p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('L', "J", pcHeadsData);  // �o�̓p�^�[���k
               } else {
                  // �t
                  SyuturyokuZumen_Set('K', "J", pcHeadsData);  // �o�̓p�^�[���j
               }
            } else {
               SyuturyokuZumen_Set('M', "J", pcHeadsData);     // �o�̓p�^�[���l
            }
         } else {
            // �񔕗p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('O', "J", pcHeadsData);  // �o�̓p�^�[���n
               } else {
                  // �t
                  SyuturyokuZumen_Set('N', "J", pcHeadsData);  // �o�̓p�^�[���m
               }
            } else {
               SyuturyokuZumen_Set('P', "J", pcHeadsData);     // �o�̓p�^�[���o
            }
         }

      }

   } else if (s_UtiKbn == "5"){
      //***************  �ł��o���敪�@�T  **********************

      SyuturyokuZumen_Set('J', "J", pcHeadsData);

   // 2018.07.26 �ł��o���敪�ύX_S
   //} else if (s_UtiKbn == "6"){
   //   //***************  �ł��o���敪�@�U  **********************
   } else if (s_UtiKbn == "6" || s_UtiKbn == "9"){
   //   //***************  �ł��o���敪�@�U�C�X  **********************

   // 2018.07.26 �ł��o���敪�ύX_E
      if (s_HEADS_DATA[327].TrimRight() == ""){           // ��������
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // ���p
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('G', "J", pcHeadsData);  // �o�̓p�^�[���f
            } else {
               // �t
               SyuturyokuZumen_Set('F', "J", pcHeadsData);  // �o�̓p�^�[���e
            }
         } else  {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('R', "J", pcHeadsData);  // �o�̓p�^�[���q
            } else {
               // �t
               SyuturyokuZumen_Set('Q', "J", pcHeadsData);  // �o�̓p�^�[���p
            }
         }
      } else {                                            // �����L��
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // ���p
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('L', "J", pcHeadsData);  // �o�̓p�^�[���k
            } else {
               // �t
               SyuturyokuZumen_Set('K', "J", pcHeadsData);  // �o�̓p�^�[���j
            }
         } else {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('T', "J", pcHeadsData);  // �o�̓p�^�[���s
            } else {
               // �t
               SyuturyokuZumen_Set('S', "J", pcHeadsData);  // �o�̓p�^�[���r
            }
         }
      }

   } else if (s_UtiKbn == "7"){
      //***************  �ł��o���敪�@�V  **********************

      SyuturyokuZumen_Set('U', "J", pcHeadsData);        // �o�̓p�^�[���t

   } else if (s_UtiKbn == "8"){
      //***************  �ł��o���敪�@�W  **********************

      SyuturyokuZumen_Set('V', "J", pcHeadsData);        // �o�̓p�^�[���u

   }


   // C��
   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);
   if (s_HEADS_DATA[319].TrimRight().ToIntDef(0) != 0){
//2004/10/25 add??
//   if (G_HEADS_DATA_Rev != 0){
//2004/10/25 add??
      //***************************
      //***  �a���F����
      //***************************
      // �wC001�x
      i_RecNo = 319;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 1, s_Text);
      G_Log_Text = "�a���F����            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //***************************
      //***  �a���F�L��
      //***************************
      // �wC002�x
      //i_RecNo = 1088;
      //s_Text = s_HEADS_DATA[i_RecNo];

      // 2010.08.19 ������Ή�
      //s_Text = "�݌v�ύX";
      if ( s_China == "0" ) {
         s_Text = "�݌v�ύX";
      } else {
         s_Text = "/U+35774/U+/U+35745/U+/U+21464/U+�X";
      }
      // *********************

      Excel_Cell_DataSet(41, 2, s_Text);
      G_Log_Text = "�a���F�L��           �w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //***************************
      //***  �a���F�N����
      //***************************
      // �wC003�x
      i_RecNo = 1087;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 4, s_Text);
      G_Log_Text = "�a���F�N����          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //***************************
      //***  �a���F�݌v�S����
      //***************************
      // �wC004�x
      i_RecNo = 1079;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 7, s_Text);
      G_Log_Text = "�a���F�݌v�S����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }

   // 2017.12.01 �[���폜_S
   ////***************************
   ////***  �a���F���i���t
   ////***************************
   //// �wC005�x
   ////i_RecNo = 12;
   //i_RecNo = 411;
   //s_Text = s_HEADS_DATA[i_RecNo];
   //Excel_Cell_DataSet(44, 3, s_Text);
   //G_Log_Text = "�a���F���i���t        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   // 2017.12.01 �[���폜_E

   //***************************
   //***  �a���FHEADS VER
   //***************************
   // �wC006�x
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
   //s_Text = "HEADS VER." + s_Text;
   Excel_Cell_DataSet(45, 3, s_Text);
   G_Log_Text = "HEADS VER            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2021.11.02 �[���d�l���ԋp�����폜_S
   //// 2021.10.18 �[���d�l���ԋp�����ǉ�_S
   ////*******************************
   ////***  �a���F�[���d�l���ԋp����
   ////*******************************
   //// �ƊE�R�[�h�y13�F�D���z�y19�F�󒲁z�y21�F�v�����g�z�������a���ŕ������L�q�@�@�@
   //i_RecNo = 438;   // �ƊE����
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if (s_Text == "0013"){
   //    // �ƊE����13
   //} else if (s_Text == "0019"){
   //    // �ƊE����19
   //} else if   (s_Text == "0021"){
   //    // �ƊE����21
   //} else {
   //    // �ƊE����13,19,21�ȊO
   //    // �ԋp�������L�q
   //    s_Text = "���@��@��";
   //    Excel_Cell_DataSet(37, 14, s_Text);
   //    G_Log_Text = "�a���F�ԋp�����P     �w" + s_Text + "�x���Z�b�g�B";
   //    s_Text = "�{�}�������������A�M�Ў�̈���̏�A";
   //    Excel_Cell_DataSet(38, 11, s_Text);
   //    G_Log_Text = "�a���F�ԋp�����Q     �w" + s_Text + "�x���Z�b�g�B";
   //    s_Text = "���Ђւ��ԋp������l���肢�\���グ�܂��B";
   //    Excel_Cell_DataSet(39, 11, s_Text);
   //    G_Log_Text = "�a���F�ԋp�����R     �w" + s_Text + "�x���Z�b�g�B";
   //    s_Text = "���A "
   //           + FormatDateTime("yyyy",Date() + 14) + "�N"
   //           + FormatDateTime("mm",Date() + 14) + "��"
   //           + FormatDateTime("dd",Date() + 14) + "��"
   //           + " ���ɂ��ԋp�����Ȃ�";
   //    Excel_Cell_DataSet(40, 11, s_Text);
   //    G_Log_Text = "�a���F�ԋp�����S     �w" + s_Text + "�x���Z�b�g�B";
   //    s_Text = "�ꍇ�́A�{�}�ɂĐ����i�߂����Ē����܂��B";
   //    Excel_Cell_DataSet(41, 11, s_Text);
   //    G_Log_Text = "�a���F�ԋp�����T     �w" + s_Text + "�x���Z�b�g�B";
   //}
   //// 2021.10.18 �[���d�l���ԋp�����ǉ�_E
   // 2021.11.02 �[���d�l���ԋp�����폜_E

   // 2020.04.21 �o�[�R�[�h�ύX_S
   //// 2017.12.01 �o�[�R�[�h�ǉ�_S
   ////***************************
   ////***  �a���F�o�[�R�[�h
   ////***************************
   //s_Text = G_SAKUBAN;
   //// 2018.10.22 �o�[�R�[�h�ύX_S
   ////s_Text = "*" + s_Text + "*";
   ////Excel_Cell_DataSet(45, 15, s_Text);
   //Excel_Cell_DataSet(10, 19, s_Text);
   //// 2018.10.22 �o�[�R�[�h�ύX_E
   //G_Log_Text = "�o�[�R�[�h           �w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   //// 2018.10.22 �o�[�R�[�h�폜_E
   s_Text = G_SAKUBAN;
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�o�[�R�[�h"));
   // 2023.05.30 �o�[�R�[�h�ύX_S
   //// 2023.04.10 �o�[�R�[�h�ύX_S
   ////Excel_Cell_DataSet(4, 2, s_Text);
   //if (AllNumChk(G_SAKUBAN.c_str(),0) == 0) {
   //    // �I�[�_
   //} else {
   //    // �V����
   //    Excel_Cell_DataSet(3, 2, s_Text);
   //}
   // 2023.04.10 �o�[�R�[�h�ύX_E
   Excel_Cell_DataSet(4, 5, s_Text);
   G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   // 2023.05.30 �o�[�R�[�h�ύX_E
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�a��"));
   // 2020.04.21 �o�[�R�[�h�ύX_E

   //***********************************************************************************
   //***                                                                             ***
   //***                    ���F�}�\���i�p���j�@�f�[�^�Z�b�g                         ***
   //***                                                                             ***
   //***********************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�p��");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("Item" , OleVariant("�p��"));

   //i_BuhinNo = 0;

   //***************************
   //***  �p���F�^�C�g��
   //***************************
   // �wA001�x
   s_Text = "FINAL SPECIFICATION & DRAWING";
   if (s_UtiKbn == "1"){
      s_Text = "ESTIMATE DWG.";

   } else if (s_UtiKbn == "2"){
      s_Text = "FINAL SPECIFICATION & DRAWING";

   } else if (s_UtiKbn == "3"){
      s_Text = "FINAL SPECIFICATION & DRAWING";

   } else if (s_UtiKbn == "4"){
      s_Text = "FINAL SPECIFICATION & DRAWING";

   } else if (s_UtiKbn == "5"){
      s_Text = "ADMISSION PROPOSAL DWG.";

   } else if (s_UtiKbn == "6"){
      //2014.11.28 �H���}�ύX
      //s_Text = "CONSTRUCTION DWG.";
      s_Text = "WORKING DWG.";
      //*********************

   }
   Excel_Cell_DataSet(1, 3, s_Text);
   G_Log_Text = "�p���F�^�C�g��       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F��i���t
   //***************************
   // �wA002�x
// ���t�͋L�����Ȃ��@2002/11/23
//   s_Text = FormatDateTime("yyyy/mm/dd",Date());
//   Excel_Cell_DataSet(3, 11, s_Text);
//   G_Log_Text = "�p���F��i���t       �w" + s_Text + "�x���Z�b�g�B";
//   Write_Log(G_Log_Text);
   // 2019.02.22 �N�����ǉ�_S�@
   i_RecNo = 1087;
   s_Text = s_HEADS_DATA[i_RecNo];
   s_Text = JapDayToEngDay( s_Text );
   if ( s_China == "1" ) {
      //����
   } else {
      if ( s_UtiKbn == "1" ) {
         //��  ��  �}
      } else if ( s_UtiKbn == "2" ) {
         //��  �F  �}
      } else if ( s_UtiKbn == "3" ) {
         //�m  ��  �}
      } else if ( s_UtiKbn == "4" ) {
         //��  ��  �}
      } else if ( s_UtiKbn == "5" ) {
         //�[  ��  �d  �l  ��
         Excel_Cell_DataSet(3, 15, s_Text);
         G_Log_Text = "�p���F�N�����Q        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else if ( s_UtiKbn == "6" ) {
         //�H  ��  �}"
      }
   }
   // 2019.02.22 �N�����ǉ�_E�@

   //***************************
   //***  �p���F������
   //***************************
   // �wA003�x
   i_RecNo = 1261;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(5, 4, s_Text);
   G_Log_Text = "�p���FPURCHASER       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F�[����
   //***************************
   // �wA004�x
   i_RecNo = 1262;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(6, 4, s_Text);
   G_Log_Text = "�p���FCOSTOMER        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F�����ԍ�
   //***************************
   // �wA005�x
   // 2017.10.06 �����ԍ��폜_S
   //i_RecNo = 1260;
   //s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   //Excel_Cell_DataSet(7, 4, s_Text);
   //G_Log_Text = "�p���FPURCHASE ORDER NO  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   // 2017.10.06 �����ԍ��폜_E

   //***************************
   //***  �p���F�H���ԍ�
   //***************************
   // �wA006�x
   i_RecNo = 28;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(9, 3, s_Text);
   G_Log_Text = "�p���FJOB NO          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // ���p�̏ꍇ�́A�uJOB No�v���uSHIP No�v�ɒu��������
      Excel_Cell_DataSet(9, 2, "SHIP NO.:");
   }
   //***************************
   // �p���F�@�햼��
   //***************************
   // �wA007�x
   i_RecNo = 26;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(10, 3, s_Text);
   G_Log_Text = "�p���FEQPT.NAME:      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F�@��ԍ�
   //***************************
   // �wA008�x
   i_RecNo = 27;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(11, 3, s_Text);
   G_Log_Text = "�p���FITEM NO.        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F�����ԍ�
   //***************************
   // �wA009�x
   if( G_SAKUBAN.SubString(1,2).ToIntDef(999) != 999 ) {
     //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
     //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
     s_Text = GetSeizouBangou(G_SAKUBAN);
     //2022/12/15 �����ԍ������ǉ��Ή� Mod_E

     //----------------------------------------------
     // 2007.10.09 �װ�d�l�̏ꍇ�A������"A/B"��ǉ�
     if(pcHeadsData->GetMirrorFlag()){
        s_Text = s_Text + "A/B";
     }
     // 2007.10.09
     //----------------------------------------------

     Excel_Cell_DataSet(13, 3, s_Text);
     G_Log_Text = "�p���FMFG NO.        �w" + s_Text + "�x���Z�b�g�B";
     Write_Log(G_Log_Text);
   }

   //***************************
   //***  �p���F�^��
   //***************************
   // �wA010�x
   i_RecNo = 33;
   s_Text = s_Text = s_HEADS_DATA[i_RecNo];
   Excel_Cell_DataSet(15, 4, s_Text);
   G_Log_Text = "�p���FMODEL           RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �p���F�䐔
   //***************************
   // �wA011�x
   i_RecNo = 39;
   s_Text = s_HEADS_DATA[i_RecNo];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(16, 3, s_Text);
      G_Log_Text = "�p���FQUANTITY        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";

      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�p���FQUANTITY          RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);
   }
   if (s_HEADS_DATA[36].Pos("M") != 0 ) {    // ���p�̏ꍇ�́A�uUNIT(S)�v���uUNIT(S)/SHIP�v�ɒu��������
      Excel_Cell_DataSet(16, 7, "UNIT(S)/SHIP");
   }
   //***************************
   //***  �p���F�����M��
   //***************************
   // �wA012�x
   if (s_HEADS_DATA[490].TrimRight() == "19"){
      i_RecNo = 107;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(17, 4, s_Text);
      G_Log_Text = "�p���FHEAT EXCHANGED       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      s_Text = "";
      Excel_Cell_DataSet(17, 4, s_Text);
   }

   // 2010.02.10 �G���[���L�ǉ�
   //***************************
   //*** �p���F���L(�װ)
   //***************************
   // �wA013�x
   i_RecNo = 1021;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text.SubString(1,2) == "ER" ) {
       s_Text = "�G���[�F"+ s_Text;
       Excel_Cell_DataSet(2, 2, s_Text);
       G_Log_Text = "���L(�װ)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
       Write_Log(G_Log_Text);
   }
   // *************************

   // 2019.10.10 �߰�ؽĴװ_S
   //***************************
   //*** �p���F���L(�߰�ؽĴװ)
   //***************************
   // �wA016�x
   i_RecNo = 1123;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text != "" ) {
       Excel_Cell_DataSet(3, 2, s_Text);
       G_Log_Text = "���L(�߰�ؽĴװ)  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
       Write_Log(G_Log_Text);
   }
   // 2019.10.10 �߰�ؽĴװ_E

   //*****************************************
   //***
   //***      �p���a���@�o�͐}�ʃZ�b�g
   //***
   //*****************************************
   s_UtiKbn = s_HEADS_DATA[1259].SubString(2,1);

   // 2019.04.09 �ł��o���敪�ύX_S
   //if (s_UtiKbn == "0" || s_UtiKbn == "9"){
   //   //***************  �ł��o���敪�@�O�C�X  **********************
   if (s_UtiKbn == "0"){
      //***************  �ł��o���敪�@�O  **********************
   // 2019.04.09 �ł��o���敪�ύX_E
      SyuturyokuZumen_Set('A', "E", pcHeadsData);     // �o�̓p�^�[���`

   } else if (s_UtiKbn == "1"){
      //***************  �ł��o���敪�@�P  **********************
      SyuturyokuZumen_Set('B', "E", pcHeadsData);     // �o�̓p�^�[���a

   } else if (s_UtiKbn == "2"){
      //***************  �ł��o���敪�@�Q  **********************

      SyuturyokuZumen_Set('C', "E", pcHeadsData);     // �o�̓p�^�[���b

   } else if (s_UtiKbn == "3"){
      //***************  �ł��o���敪�@�R  **********************

      if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
         // ����
         SyuturyokuZumen_Set('E', "E", pcHeadsData);  // �o�̓p�^�[���d
      } else {
         // �t
         SyuturyokuZumen_Set('D', "E", pcHeadsData);  // �o�̓p�^�[���c
      }

   } else if (s_UtiKbn == "4"){
      //***************  �ł��o���敪�@�S  **********************
      s_Text = s_HEADS_DATA[29].TrimRight();   // ���͗e��i�K�i���ށj

      if (s_HEADS_DATA[327].TrimRight() == ""){
         // ��������
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // ���p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('G', "E", pcHeadsData);  // �o�̓p�^�[���f
               } else {
                  // �t
                  SyuturyokuZumen_Set('F', "E", pcHeadsData);  // �o�̓p�^�[���e
               }
            } else {
                  SyuturyokuZumen_Set('C', "E", pcHeadsData);     // �o�̓p�^�[���b
            }
         } else {
            // �񔕗p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('I', "E", pcHeadsData);  // �o�̓p�^�[���h
               } else {
                  // �t
                  SyuturyokuZumen_Set('H', "E", pcHeadsData);  // �o�̓p�^�[���g
               }
            } else {
               SyuturyokuZumen_Set('J', "E", pcHeadsData);     // �o�̓p�^�[���i
            }
         }

      } else {
         // �����L��
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){
            // ���p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('L', "E", pcHeadsData);  // �o�̓p�^�[���k
               } else {
                  // �t
                  SyuturyokuZumen_Set('K', "E", pcHeadsData);  // �o�̓p�^�[���j
               }
            } else {
               SyuturyokuZumen_Set('M', "E", pcHeadsData);     // �o�̓p�^�[���l
            }
         } else {
            // �񔕗p
            if (s_Text == "01" || s_Text == "02" || s_Text == "03"){
               if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
                  // ����
                  SyuturyokuZumen_Set('O', "E", pcHeadsData);  // �o�̓p�^�[���n
               } else {
                  // �t
                  SyuturyokuZumen_Set('N', "E", pcHeadsData);  // �o�̓p�^�[���m
               }
            } else {
               SyuturyokuZumen_Set('P', "E", pcHeadsData);     // �o�̓p�^�[���o
            }
         }

      }

   } else if (s_UtiKbn == "5"){
      //***************  �ł��o���敪�@�T  **********************

      SyuturyokuZumen_Set('J', "E", pcHeadsData);

   // 2019.04.09 �ł��o���敪�ύX_S
   //} else if (s_UtiKbn == "6"){
   //   //***************  �ł��o���敪�@�U  **********************
   } else if (s_UtiKbn == "6" || s_UtiKbn == "9"){
      //***************  �ł��o���敪�@�U�C�X  **********************
   // 2019.04.09 �ł��o���敪�ύX_E

      if (s_HEADS_DATA[327].TrimRight() == ""){         // ��������
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // ���p
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('G', "E", pcHeadsData);  // �o�̓p�^�[���f
            } else {
               // �t
               SyuturyokuZumen_Set('F', "E", pcHeadsData);  // �o�̓p�^�[���e
            }
         } else {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('R', "E", pcHeadsData);  // �o�̓p�^�[���q
            } else {
               // �t
               SyuturyokuZumen_Set('Q', "E", pcHeadsData);  // �o�̓p�^�[���p
            }
         }
      } else {         // �����L��
         i_Pos = s_HEADS_DATA[36].Pos("M");
         if (i_Pos != 0){       // ���p
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('L', "E", pcHeadsData);  // �o�̓p�^�[���k
            } else {
               // �t
               SyuturyokuZumen_Set('K', "E", pcHeadsData);  // �o�̓p�^�[���j
            }
         } else {
            if (s_HEADS_DATA[846].TrimRight() == "1" || s_HEADS_DATA[847].TrimRight() == "1"){
               // ����
               SyuturyokuZumen_Set('T', "E", pcHeadsData);  // �o�̓p�^�[���s
            } else {
               // �t
               SyuturyokuZumen_Set('S', "E", pcHeadsData);  // �o�̓p�^�[���r
            }
         }
      }

   } else if (s_UtiKbn == "7"){
      //***************  �ł��o���敪�@�V  **********************

      SyuturyokuZumen_Set('U', "E", pcHeadsData);        // �o�̓p�^�[���t

   } else if (s_UtiKbn == "8"){
      //***************  �ł��o���敪�@�W  **********************

      SyuturyokuZumen_Set('V', "E", pcHeadsData);        // �o�̓p�^�[���u

   }

   // C��
   s_UtiKbn = s_HEADS_DATA[1259].SubString(1,1);
   if (s_HEADS_DATA[319].TrimRight().ToIntDef(0) != 0) {
//2004/10/25 add???
//   if (G_HEADS_DATA_Rev != 0){
//2004/10/25 add???
      //***************************
      //***  �p���FRev
      //***************************
      // �wC001�x
      i_RecNo = 319;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 1, s_Text);
      G_Log_Text = "�p���FRev             RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //***************************
      //***  �p���FDATE
      //***************************
      // �wC002�x
      i_RecNo = 1087;
      s_Text = s_HEADS_DATA[i_RecNo];
      s_Text = JapDayToEngDay( s_Text );
      Excel_Cell_DataSet(41, 2, s_Text);
      G_Log_Text = "�p���F�N����          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

      //***************************
      //***  �p���FREVIEWED BY
      //***************************
      // �wC003�x
      i_RecNo = 1080;
      s_Text = s_HEADS_DATA[i_RecNo];
      Excel_Cell_DataSet(41, 4, s_Text);
      G_Log_Text = "�p���FREVIEWED BY     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);

   } else {

      s_Text = "";
      // 2024.04.08 �o�[�R�[�h�ύX_S
      //Excel_Cell_DataSet(41, 1, s_Text);  // �p���FRev
      // 2020.04.08 �o�[�R�[�h�ύX_E
      Excel_Cell_DataSet(41, 2, s_Text);  // �p���FDATE
      Excel_Cell_DataSet(41, 4, s_Text);  // �p���FREVIEWED BY
   }

   //***************************
   //***  �p���FHEADS VER
   //***************************
   // �wC006�x
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
   //s_Text = "HEADS VER." + s_Text;
   Excel_Cell_DataSet(45, 3, s_Text);
   G_Log_Text = "HEADS VER            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2020.04.21 �o�[�R�[�h�ύX_S
   //// 2017.12.01 �o�[�R�[�h�ǉ�_S
   ////***************************
   ////***  �p���F�o�[�R�[�h
   ////***************************
   //s_Text = G_SAKUBAN;
   //// 2018.10.22 �o�[�R�[�h�ύX_S
   ////s_Text = "*" + s_Text + "*";
   ////Excel_Cell_DataSet(45, 15, s_Text);
   //Excel_Cell_DataSet(10, 18, s_Text);
   //// 2018.10.22 �o�[�R�[�h�ύX_E
   //G_Log_Text = "�o�[�R�[�h           �w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   //// 2017.12.01 �o�[�R�[�h�ǉ�_E
   s_Text = G_SAKUBAN;
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�o�[�R�[�h"));
   // 2023.05.30 �o�[�R�[�h�ύX_S
   //// 2023.04.10 �o�[�R�[�h�ύX_S
   ////Excel_Cell_DataSet(22, 2, s_Text);
   //if (AllNumChk(G_SAKUBAN.c_str(),0) == 0) {
   //    // �I�[�_
   //} else {
   //    // �V����
   //    Excel_Cell_DataSet(21, 2, s_Text);
   //}
   // 2023.04.10 �o�[�R�[�h�ύX_E
   Excel_Cell_DataSet(22, 5, s_Text);
   G_Log_Text = "�o�[�R�[�h���         �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   // 2023.05.30 �o�[�R�[�h�ύX_E
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("�p��"));
   // 2020.04.21 �o�[�R�[�h�ύX_E

   //******************************
   //***  Excel Book Close
   //******************************
   // ������Ȃ��ق��̃V�[�g���o�c�e�ݒ�V�[�g�ɋL�q����
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�o�c�e�ݒ�");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("Item" , OleVariant("�o�c�e�ݒ�"));

   if (s_HEADS_DATA[1].TrimRight() == "0") {
     Excel_Cell_DataSet(4, 2, "�p��");
   } else {
     Excel_Cell_DataSet(4, 2, "�a��");
   }
   // 2020.05.28 �o�[�R�[�h�ύX_S
   Excel_Cell_DataSet(5, 2, "�o�[�R�[�h");
   // 2020.05.28 �o�[�R�[�h�ύX_E

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
   // 2012.11.07 ����PDF�폜
   //// 2012.10.03 PDF�o�͍Ēǉ�
   //////PDF�t�@�C���o�͎��
   ////exApplication.Exec(Function("Run") << "Gsub_PDFOut");
   //exApplication.OleFunction("Run" , OleVariant("Gsub_PDFOut"));
   if ( s_China == "0" ) {
     exApplication.OleFunction("Run" , OleVariant("Gsub_PDFOut"));
   }
   // **********************
   //exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.OlePropertySet("DisplayAlerts" , OleVariant("false"));
   // ************************
   //Windows 2008Server �ڐA�Ή�
   exApplication.OlePropertySet("DisplayAlerts", false);
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ۸ލ쐬
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���F�}�\���x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "--------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���F�}�\���@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
AnsiString __fastcall JapDayToEngDay( AnsiString s_Text )
{
   AnsiString yyyy, mm, dd;
   yyyy = s_Text.SubString(1,4);
   mm = s_Text.SubString(6,2);
   dd = s_Text.SubString(9,2);

   try
   {
      switch( mm.ToIntDef(0) )
      {
         case 1 :
            mm = "JAN";
            break;

         case 2 :
            mm = "FEB";
            break;

         case 3 :
            mm = "MAR";
            break;

         case 4 :
            mm = "APR";
            break;

         case 5 :
            mm = "MAY";
            break;

         case 6 :
            mm = "JUN";
            break;

         case 7 :
            mm = "JUL";
            break;

         case 8 :
            mm = "AUG";
            break;

         case 9 :
            mm = "SEP";
            break;

         case 10 :
            mm = "OCT";
            break;

         case 11 :
            mm = "NOV";
            break;

         case 12 :
            mm = "DEC";
            break;

         default :
            break;
      }

      s_Text = mm +","+ dd +","+ yyyy;
   }
   catch(...)
   {
      return s_Text;
   }

   return s_Text;
}

//---------------------------------------------------------------------------
// ���{��֐����F �o�͐}�ʁ@�Z�b�g
// �T  �v      �F
// ��  ��      �F Pattern [�o�̓p�^�[��]
// �߂�l      �F �Ȃ�
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
void __fastcall SyuturyokuZumen_Set(char Pattern, AnsiString Lang, HeadsData* pcHeadsData)
{
   AnsiString s_Text;
   AnsiString s_MeibanZuban;
   AnsiString s_MeibanStr;
   int        i_RowCnt;
   int        i_MirrorLoop;

   bool bRet;
   AnsiString s_HEADS_DATA[1500];
   // 2010.08.19 ������Ή�
   int        i_RecNo;           // HEADS�ް� ں���No
   AnsiString s_China;           // ����敪("0":���{��A"1":������)
   // *********************

   // AnsiString�^HEADS�ް���۰�ٕϐ��ɾ��
   bRet = pcHeadsData->SetAnsiStringData(s_HEADS_DATA);
   if(!bRet) return;

   // 2010.08.19 ������Ή�
   i_RecNo = 851;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "1" ) {
      s_China = "1";                                                 // ������
   } else {
      s_China = "0";                                                 // ���{��
   }
   // *********************
   // 2011.05.30 ��n�p���F�}
   // 2012.01.20 ��n�t���O
   //i_RecNo = 5;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // ������
   //}
   //i_RecNo = 401;
   //s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   //if(s_Text == "42842" || s_Text == "43935") {
   //   s_China = "1";                                                 // ������
   //}
   i_RecNo = 1107;
   s_Text = s_HEADS_DATA[i_RecNo].TrimRight();
   if ( s_Text == "CHINA" ) {
      s_China = "1";                                                 // ������
   }
   // ***********************

   //*******************
   //***  �}�ʗ�������
   //*******************
   for (i_RowCnt = 0; i_RowCnt < 10; i_RowCnt++){
      s_Text = "";
      Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);
      Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
      Excel_Cell_DataSet(23 + i_RowCnt, 9, s_Text);
   }

   i_RowCnt = 0;

   //*******************
   //***  �v�ڕ\
   //*******************
   switch (Pattern) {
      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M':
         if (Lang == "J"){
            // 2010.08.19 ������Ή�
            //s_Text = "�v�ڕ\";
            if ( s_China == "0" ) {
                s_Text = "�v�ڕ\";
            } else {
                // 2011.02.09 ������Ή�
                //s_Text = "��vU+39033/U+�ڕ\";
                s_Text = "��v/U+39033/U+�ڕ\";
                // *********************
            }
            // *********************
         } else {
            s_Text = "PRINCIPAL ITEMS";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 �����ԍ������ǉ��Ή� Mod
		 //s_Text = "M" + G_SAKUBAN;
		 s_Text = "M" + GetFileSeizouBangou(G_SAKUBAN);
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;
         break;

      default :
         break;
   }

    //-------------------------------------------------
    // 2007.11.02 �o�͏��ύX
    // ������č\���}�A���g���}���װ�d�lA/B���Ƃɂ܂Ƃ߂�

    i_MirrorLoop = 0;
    while( i_MirrorLoop <= 1 )
    {

        //***************************
        //***  �G�������g�\���}
        //***************************
        switch (Pattern) {
            case 'H':  case 'I': case 'J': case 'N': case 'O': case 'P':
            case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':

                //----------------------------------
                // 2007.10.09 �װ�d�l�Ή�

                if (Lang == "J"){
                    // 2010.08.19 ������Ή�
                    //s_Text = "�G�������g�\���}";
                    if ( s_China == "0" ) {
                        s_Text = "�G�������g�\���}";
                    } else {
                        // 2011.05.30 ��n�p���F�}
                        //s_Text = "�v�f/U+26500/U+��/U+22270/U+";
                        s_Text = "��/U+26500/U+��/U+22270/U+";
                        // ***********************
                    }
                    // *********************
                } else {
                    s_Text = "DWG. OF PLATE ARRANGEMENT";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

				//2023/4/21 �����ԍ������ǉ��Ή� Mod
				//s_Text = "A" + G_SAKUBAN;       // A[���]
				s_Text = "A" + GetFileSeizouBangou(G_SAKUBAN);       // A[���]

                // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // B[���]A
                    } else {
                        s_Text = s_Text + "B";   // B[���]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

            default :
                break;
        }

        //*******************
        //***  �g���}
        //*******************
        switch (Pattern) {
            case 'A': case 'U':
                break;

            default :

                //----------------------------------
                // 2007.10.09 �װ�d�l�Ή�

                if (Lang == "J"){
                    // 2010.08.19 ������Ή�
                    //s_Text = "�g���}";
                    if ( s_China == "0" ) {
                        s_Text = "�g���}";
                    } else {
                        // 2011.05.30 ��n�p���F�}
                        //s_Text = "���z/U+22270/U+";
                        s_Text = "�O�`/U+22270/U+";
                        // ***********************
                    }
                    // *********************
                } else {
                    s_Text = "ASSEMBLY DRAWING";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

				//2023/4/21 �����ԍ������ǉ��Ή� Mod
				//s_Text = "B" + G_SAKUBAN;       // B[���]
				s_Text = "B" + GetFileSeizouBangou(G_SAKUBAN);       // B[���]

                // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // B[���]A
                    } else {
                        s_Text = s_Text + "B";   // B[���]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

            // 2007.10.09 �װ�d�l�Ή�
            //----------------------------------
        }

        i_MirrorLoop++;
        if( !pcHeadsData->GetMirrorFlag() ) break;
    }

    // 2007.11.02 �o�͏��ύX
    //-------------------------------------------------

   //2004.02.20 ������ݒǉ�
   //***************************
   //***  ������ݐ}��
   //***************************
   //2020.11.17 �f�M�ޒǉ�_S
   ////2018.04.17 �f�M�ޒǉ�_S
   ////if (s_HEADS_DATA[264].TrimRight() != "1.00"){
   //if ((s_HEADS_DATA[264].TrimRight() != "1.00") && (s_HEADS_DATA[264].TrimRight() != "2.00")){
   ////2018.04.17 �f�M�ޒǉ�_E
   if ((s_HEADS_DATA[264].TrimRight() != "1.00") && (s_HEADS_DATA[264].TrimRight() != "2.00") && (s_HEADS_DATA[264].TrimRight() != "3.00")){
   //2020.11.17 �f�M�ޒǉ�_E
      //������� ���Ȃ����ߋL�q���܂���
   }
   else if (s_HEADS_DATA[1330].Trim() == ""){
      //�}�Ԃ��Ȃ����ߋL�q���܂���
   }
   else {
      switch (Pattern) {
         case 'H': case 'I': case 'J': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
            if (Lang == "J"){
               // 2010.08.19 ������Ή�
               //s_Text = "�h�����p��";
               if ( s_China == "0" ) {
                   s_Text = "�h�����p��";
               } else {
                   s_Text = "�r��/U+30424/U+";
               }
               // *********************
            } else {
               s_Text = "DRAINPAN";
            }
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

            s_Text = s_HEADS_DATA[1330].Trim();
            s_Text = s_Text.SubString(15,s_Text.Length()-14);
            s_Text = s_Text.SubString(1,s_Text.Pos("�@L")-1);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;
            break;

         default :
            break;
      }
   }
   //**********************************

   //2018.03.12 ���ǉ�
   //***************************
   //***  ���}��
   //***************************
   if (s_HEADS_DATA[272].TrimRight() != "1"){
      //���� ���Ȃ����ߋL�q���܂���
   }
   else {
      s_MeibanZuban = GetMeibanZuban();
      if (s_MeibanZuban != "") {
        if (Lang == "J"){
           s_Text = "���}";
        } else {
           s_Text = "NAMEPLATE";
        }

        // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
        if( pcHeadsData->GetMirrorFlag() )
        {
            if (Lang == "J"){
               s_Text = "���}A";
            } else {
               s_Text = "NAMEPLATE A";
            }

            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);
            //2019.08.08 ���}�ԕύX_S
            //s_Text = ZenToHan(s_MeibanZuban);
			//s_Text = s_Text.SubString(0, 7);
			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "NP" + G_SAKUBAN;
			s_Text = "NP" + GetFileSeizouBangou(G_SAKUBAN);
            //2019.08.08 ���}�ԕύX_E
            s_Text = s_Text + "A";
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
            i_RowCnt++;

            if (Lang == "J"){
               s_Text = "���}B";
            } else {
               s_Text = "NAMEPLATE B";
            }

            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);
            //2019.08.08 ���}�ԕύX_S
            //s_Text = ZenToHan(s_MeibanZuban);
            //s_Text = s_Text.SubString(0, 7);
            //2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "NP" + G_SAKUBAN;
			s_Text = "NP" + GetFileSeizouBangou(G_SAKUBAN);
			//2019.08.08 ���}�ԕύX_E
            s_Text = s_Text + "B";
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
            i_RowCnt++;

        } else {
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2019.08.08 ���}�ԕύX_S
            //s_Text = ZenToHan(s_MeibanZuban);
            //s_Text = s_Text.SubString(0, 7);
			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "NP" + G_SAKUBAN;
			s_Text = "NP" + GetFileSeizouBangou(G_SAKUBAN);
			//2019.08.08 ���}�ԕύX_E
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
            i_RowCnt++;
        }
      }
   }
   //**********************************

   //***************************
   //***  ���X�g
   //***************************
   switch (Pattern) {
      case 'C': case 'F': case 'G': case 'K': case 'L': case 'M': case 'Q': case 'R': case 'S': case 'T':
         if (Lang == "J"){
            // 2010.08.19 ������Ή�
            //s_Text = "�p�[�c���X�g";
            if ( s_China == "0" ) {
                s_Text = "�p�[�c���X�g";
            } else {
                s_Text = "�댏��/U+21333/U+";
            }
            // *********************
         } else {
            s_Text = "PARTS LIST";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 �����ԍ������ǉ��Ή� Mod
		 //s_Text = "PAR" + G_SAKUBAN;
		 s_Text = "PAR" + GetFileSeizouBangou(G_SAKUBAN);
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;

         if (Lang == "J"){
            // 2010.08.19 ������Ή�
            //s_Text = "�t���i���X�g";
            if ( s_China == "0" ) {
                s_Text = "�t���i���X�g";
            } else {
                // 2011.02.09 ������Ή�
                //s_Text = "������/U+21333/U+";
                s_Text = "�����i��/U+21333/U+";
                // *********************
            }
            // *********************
         } else {
            s_Text = "ACCESSORIES LIST";
         }
		 Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 �����ԍ������ǉ��Ή� Mod
		 //s_Text = "ACT" + G_SAKUBAN;
		 s_Text = "ACT" + GetFileSeizouBangou(G_SAKUBAN);
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;

         if (Lang == "J"){
            // 2010.08.19 ������Ή�
            //s_Text = "�H��X�g";
            if ( s_China == "0" ) {
                s_Text = "�H��X�g";
            } else {
                s_Text = "�H�/U+21333/U+";
            }
            // *********************
         } else {
            s_Text = "TOOL LIST";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 //2023/4/21 �����ԍ������ǉ��Ή� Mod
		 //s_Text = "TOL" + G_SAKUBAN;
		 s_Text = "TOL" + GetFileSeizouBangou(G_SAKUBAN);
		 Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;
         break;

      default :
         break;
   }

   //***************************
   //***  ���͗e��i�t�j
   //***************************
   switch (Pattern) {
//    case 'D': case 'F': case 'H': case 'K': case 'N': case 'Q': case 'S':
      case 'D':
         if (Lang == "J"){
            // 2010.08.19 ������Ή�
            //s_Text = "���x�v�Z��";
            if ( s_China == "0" ) {
                s_Text = "���x�v�Z��";
            } else {
                s_Text = "/U+5F3A/U+�x��/U+5355/U+";
            }
            // *********************
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "PVC" + G_SAKUBAN;
			s_Text = "PVC" + GetFileSeizouBangou(G_SAKUBAN);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;

           //----------------------------------
           // 2007.10.09 �װ�d�l�Ή�

           i_MirrorLoop = 0;
           while( i_MirrorLoop <= 1 )
           {
              // 2010.08.19 ������Ή�
              //s_Text = "�t���[�V�[�g";
              if ( s_China == "0" ) {
                  s_Text = "�t���[�V�[�g";
              } else {
                  s_Text = "FLOW SEAT";
              }
              // *********************
              Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			  //2023/4/21 �����ԍ������ǉ��Ή� Mod
			  //s_Text = "PVF" + G_SAKUBAN;       // PVF[���]
			  s_Text = "PVF" + GetFileSeizouBangou(G_SAKUBAN);       // PVF[���]

              // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
              if( pcHeadsData->GetMirrorFlag() )
              {
                 if( i_MirrorLoop == 0 ){
                    s_Text = s_Text + "A";   // PVF[���]A
                 } else {
                    s_Text = s_Text + "B";   // PVF[���]B
                 }
              }

              Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

              i_RowCnt++;
              i_MirrorLoop++;
              if( !pcHeadsData->GetMirrorFlag() ) break;
           }

           // 2007.10.09 �װ�d�l�Ή�
           //----------------------------------
         }

         break;

      default :
         break;
   }

   //***************************
   //***  ���͗e��i�X�`�[���j
   //***************************
   switch (Pattern) {
//    case 'E': case 'G': case 'I': case 'L': case 'O': case 'R': case 'T':
      case 'E':
         if (Lang == "J"){
            // 2010.08.19 ������Ή�
            //s_Text = "���x�v�Z��";
            if ( s_China == "0" ) {
                s_Text = "���x�v�Z��";
            } else {
                s_Text = "/U+5F3A/U+�x��/U+5355/U+";
            }
            // *********************
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "PVC" + G_SAKUBAN;
			s_Text = "PVC" + GetFileSeizouBangou(G_SAKUBAN);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;

            //----------------------------------
            // 2007.10.09 �װ�d�l�Ή�

            i_MirrorLoop = 0;
            while( i_MirrorLoop <= 1 )
            {
               // 2010.08.19 ������Ή�
               //s_Text = "�t���[�V�[�g";
               if ( s_China == "0" ) {
                   s_Text = "�t���[�V�[�g";
               } else {
                   s_Text = "FLOW SEAT";
               }
               // *********************
               Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			   //2023/4/21 �����ԍ������ǉ��Ή� Mod
			   //s_Text = "PVF" + G_SAKUBAN;       // PVF[���]
			   s_Text = "PVF" + GetFileSeizouBangou(G_SAKUBAN);       // PVF[���]

               // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
               if( pcHeadsData->GetMirrorFlag() )
               {
                  if( i_MirrorLoop == 0 ){
                     s_Text = s_Text + "A";   // PVF[���]A
                  } else {
                     s_Text = s_Text + "B";   // PVF[���]B
                  }
               }

               Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

               i_RowCnt++;
               i_MirrorLoop++;
               if( !pcHeadsData->GetMirrorFlag() ) break;
            }

            // 2007.10.09 �װ�d�l�Ή�
            //----------------------------------

            // 2010.08.19 ������Ή�
            //s_Text = "�X�`�[�������ʌv�Z��";
            if ( s_China == "0" ) {
                s_Text = "�X�`�[�������ʌv�Z��";
            } else {
                s_Text = "���D�񑗓��ʐ�/U+5355/U+";
            }
            // *********************
            Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			//2023/4/21 �����ԍ������ǉ��Ή� Mod
			//s_Text = "PVC" + G_SAKUBAN;
			s_Text = "PVC" + GetFileSeizouBangou(G_SAKUBAN);
            Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

            i_RowCnt++;
         }

         break;

      default :
         break;
   }

   //*******************
   //***  ���v�ڕ\
   //*******************
   switch (Pattern) {
      case 'K': case 'L': case 'M':
         if (Lang == "J"){
            // 2010.08.19 ������Ή�
            //s_Text = "���v�ڕ\";
            if ( s_China == "0" ) {
                s_Text = "���v�ڕ\";
            } else {
                s_Text = "����v/U+39033/U+�ڕ\";
            }
            // *********************
         } else {
            s_Text = "OLD PRINCIPAL ITEMS";
         }
         Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

		 s_Text = "M" + s_HEADS_DATA[327].TrimRight();
         Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

         i_RowCnt++;
         break;

      default :
         break;
   }

    //-------------------------------------------------
    // 2007.11.02 �o�͏��ύX
    // ������č\���}�A���g���}���װ�d�lA/B���Ƃɂ܂Ƃ߂�

    i_MirrorLoop = 0;
    while( i_MirrorLoop <= 1 )
    {
        //***************************
        //***  ���G�������g�\���}
        //***************************
        switch (Pattern) {
            case 'N': case 'O': case 'P': case 'S': case 'T':

                //----------------------------------
                // 2007.10.17 �װ�d�l�Ή�

                if (Lang == "J"){
                   // 2010.08.19 ������Ή�
                   //s_Text = "���G�������g�\���}";
                   if ( s_China == "0" ) {
                       s_Text = "���G�������g�\���}";
                   } else {
                       s_Text = "���v�f/U+26500/U+��/U+22270/U+";
                   }
                   // *********************
                } else {
                    s_Text = "OLD DWG. OF PLATE ARRANGEMENT";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

                s_Text = "A" + s_HEADS_DATA[327].TrimRight();

                // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // A[���]A
                    } else {
                        s_Text = s_Text + "B";   // A[���]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

                // 2007.10.17 �װ�d�l�Ή�
                //----------------------------------

            default :
                break;
        }

        //*******************
        //***  ���g���}
        //*******************
        switch (Pattern) {
            case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'S': case 'T':

                //----------------------------------
                // 2007.10.09 �װ�d�l�Ή�

                if (Lang == "J"){
                    // 2010.08.19 ������Ή�
                    //s_Text = "���g���}";
                    if ( s_China == "0" ) {
                        s_Text = s_Text = "���g���}";
                    } else {
                        s_Text = "�����z/U+22270/U+";
                    }
                    // *********************
                } else {
                    s_Text = "OLD ASSEMBLY DRAWING";
                }
                Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

                s_Text = "B" + s_HEADS_DATA[327].TrimRight();   // B[���]

                // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
                if( pcHeadsData->GetMirrorFlag() )
                {
                    if( i_MirrorLoop == 0 ){
                        s_Text = s_Text + "A";   // B[���]A
                    } else {
                        s_Text = s_Text + "B";   // B[���]B
                    }
                }

                Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);
                i_RowCnt++;
                break;

                // 2007.10.09 �װ�d�l�Ή�
                //----------------------------------

            default :
                break;
        }

        i_MirrorLoop++;
        if( !pcHeadsData->GetMirrorFlag() ) break;
    }

    // 2007.11.02 �o�͏��ύX
    //-------------------------------------------------

   //***************************
   //***  �����v�̏�
   //***************************
   switch (Pattern) {
      case 'V':
         //----------------------------------
         // 2007.10.17 �װ�d�l�Ή�

         i_MirrorLoop = 0;
         while( i_MirrorLoop <= 1 )
         {
             if (Lang == "J"){
                // 2010.08.19 ������Ή�
                //s_Text = "�����v�̏�";
                if ( s_China == "0" ) {
                    s_Text = "�����v�̏�";
                } else {
                    s_Text = "/U+26816/U+/U+26597/U+�v/U+39046/U+/U+20070/U+";
                }
                // *********************
             } else {
                s_Text = "INSPECTION PROCEDURE";
             }
             Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			 //2023/4/21 �����ԍ������ǉ��Ή� Mod
			 //s_Text = "D" + G_SAKUBAN;
			 s_Text = "D" + GetFileSeizouBangou(G_SAKUBAN);

              // �װ�d�l�̏ꍇ�A"A"�܂���"B"��ǉ�����(2�s�o��)
             if( pcHeadsData->GetMirrorFlag() )
             {
                if( i_MirrorLoop == 0 ){
                    s_Text = s_Text + "A";   // D[���]A
                } else {
                    s_Text = s_Text + "B";   // D[���]B
                }
             }

             Excel_Cell_DataSet(23 + i_RowCnt, 6, s_Text);

             i_RowCnt++;
             i_MirrorLoop++;
             if( !pcHeadsData->GetMirrorFlag() ) break;
         }
         //i_RowCnt++;
         break;

         // 2007.10.17 �װ�d�l�Ή�
         //----------------------------------

      default :
         break;
   }


    //-------------------------------------------------
    // OGI 2015.01.29 �A�L���[�����[�^���i�Ή�
    // �`���ɃA�L���[�����[�^���i�����݂���ꍇ�Ɂu�A�L���[�����[�^�v�̍s��ǉ�����

    //***************************
    //***  �A�L���[�����[�^
    //***************************
    AnsiString s_Nzl_SyuruiE3 = s_HEADS_DATA[981].TrimRight();
    AnsiString s_Nzl_SyuruiE4 = s_HEADS_DATA[996].TrimRight();
    if (s_Nzl_SyuruiE3 == "8" || s_Nzl_SyuruiE4 == "8"){
        switch (Pattern) {
          case 'C': case 'F': case 'G': case 'K': case 'L': case 'M': case 'Q': case 'R': case 'S': case 'T':
             if (Lang == "J"){
                if ( s_China == "0" ) {
                    s_Text = "�A�L���[�����[�^";
                } else {
                    s_Text = ""; // ������Ή��K�v
                }
                // *********************
             } else {
                s_Text = "ACCUMULATOR DRAWING";
             }
             Excel_Cell_DataSet(23 + i_RowCnt, 2, s_Text);

			 //2023/4/21 �����ԍ������ǉ��Ή� Mod
			 //s_Text = "TOL" + G_SAKUBAN;
			 s_Text = "TOL" + GetFileSeizouBangou(G_SAKUBAN);
             Excel_Cell_DataSet(23 + i_RowCnt, 6, "E430747");

             i_RowCnt++;
             break;

          default :
             break;
        }
    }
}

