//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���^���͗e�� ���׏� �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.06.04
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

#include "Bas_Meisaisyo.h"

#include "HeadsData.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ���^���͗e�� ���׏��@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Meisaisyo(HeadsData* pcHeadsData)
{
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_S
   //TQuery *wkQuery;
   //wkQuery = new TQuery(DataModule1);
   TFDQuery *wkQuery;
   wkQuery = new TFDQuery(DataModule1);
   // 2021.06.18 FireDAC�ւ̈ڍs MOD_E

   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_Text;            // �ėp������
   AnsiString s_SQL;             // SQL��
   int        i_RecNo;           // HEADS�ް� ں���No

   //------------------------------------
   // 2007.10.10 �װ�d�l�Ή�
   // ���d�l�ȊO�͏����s�v(����I��)
   if(pcHeadsData->GetSpecType() != DEF_SPEC_TYPE_NONE) return(true);
   // 2007.10.10
   //------------------------------------

   s_Tmplate_Pass = G_Template_Dir + "���^���͗e�햾�׏�.xls";          // ����ڰ�̧��
   //2023/4/21 �����ԍ������ǉ��Ή� Mod
   //s_MakeExcel_Pass = G_xls_Output_Dir + "PVI" + G_SAKUBAN + ".xls";    // ��߰��̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "PVI" + GetFileSeizouBangou(G_SAKUBAN) + ".xls";    // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���^���͗e��  ���׏��x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------------------";
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

   //******************************************************************************
   //***                                                                        ***
   //***                          ���^���͗e��@���׏�                          ***
   //***                                                                        ***
   //******************************************************************************
   // ��Ă̎w��
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "���׏�");
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",OleVariant("���׏�"));

   //*******************
   //***  �����ԍ�
   //*******************
   // �wA001�x
   //2004.12.21 ���L�ύX
   //s_Text = "No." + G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   //Excel_Cell_DataSet(3, 1, s_Text);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   
   Excel_Cell_DataSet(2, 1, s_Text);
   //*******************
   G_Log_Text = "�����ԍ�            �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �g�p���́@����M��
   //***************************
   // �wA002�x
   i_RecNo = 1098;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if (G_HEADS_DATA[i_RecNo].ToDouble() >= 999.0) {
       s_Text = "��C�J��";
     } else {
       s_Text = FormatFloat( "0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   //2021.12.09 �e���v���[�g�ύX_S
   //Excel_Cell_DataSet(10, 7, s_Text);
   Excel_Cell_DataSet(10, 10, s_Text);
   //2021.12.09 �e���v���[�g�ύX_E
   G_Log_Text = "�g�p���� ����M��    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �g�p���́@�M����
   //***************************
   // �wA003�x
   i_RecNo = 1100;
   if ( G_HEADS_DATA[i_RecNo] != "" ) {
     if (G_HEADS_DATA[i_RecNo].ToDouble() >= 999) {
       s_Text = "��C�J��";
     } else {
       s_Text = FormatFloat( "0.00", G_HEADS_DATA[i_RecNo].ToDouble());
     }
   } else {
     s_Text = "0.00";
   }
   //2021.12.09 �e���v���[�g�ύX_S
   //Excel_Cell_DataSet(11, 7, s_Text);
   Excel_Cell_DataSet(11, 10, s_Text);
   //2021.12.09 �e���v���[�g�ύX_E
   G_Log_Text = "�g�p���� �M����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  ���e�ρ@����M��,���M��
   //*********************************
   // �wA004�x,�wA005�x
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
         Excel_Cell_DataSet(13, 10, s_Text);
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
         Excel_Cell_DataSet(14, 10, s_Text);
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
         Excel_Cell_DataSet(13, 10, s_Text);
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
         Excel_Cell_DataSet(14, 10, s_Text);
         G_Log_Text = "���e�� �M����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "���e�� �M����    RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }
   }

   //***************************
   //***  ��ڰĂ̍ޗ�
   //***************************
   // �wA006�x
   i_RecNo = 43;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "��ڰĂ̍ޗ�          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���";
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
   //2004.12.21 ���p�ɕύX
   s_Text = ZenToHan(s_Text);
   //*********************
   Excel_Cell_DataSet(16, 4, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ��ڰĕ�
   //***************************
   // �wA007�x
   i_RecNo = 1268;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 11, s_Text);
   G_Log_Text = "��ڰĕ�              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ��ڰĒ�
   //***************************
   // �wA008�x
   i_RecNo = 1269;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 14, s_Text);
   G_Log_Text = "��ڰĒ�              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ��ڰČ�
   //***************************
   // �wA009�x
   i_RecNo = 1270;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(16, 17, s_Text);
   G_Log_Text = "��ڰČ�              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰт̍ޗ�
   //***************************
   // �wA010�x
   switch (G_HEADS_DATA[37].ToIntDef(0)) {
      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:           //�a�V���[�Y
         s_Text = G_HEADS_DATA[34].TrimRight();
         if ((s_Text.SubString(1,2) == "UX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==0) ||       //UX-01
             (s_Text.SubString(1,2) == "LX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ) {     //LX-10
            //2004.12.21 ���p�ɕύX
            //s_Text = "�r�t�r�R�O�S";
            s_Text = "SUS304";
            //*********************
         } else {
            //2004.12.21 ���p�ɕύX
            //s_Text = "�r�r�S�O�O";
            s_Text = "SS400";
            //*********************
         }

         break;
      default :
         i_RecNo = 1044;   // SPFRM
         s_Text = G_HEADS_DATA[i_RecNo].TrimRight();

         if (s_Text == "0000" || s_Text == "1040"){
            //2004.12.21 ���p�ɕύX
            //s_Text = "�r�r�S�O�O";
            s_Text = "SS400";
            //*********************
         } else {
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
               //2004.12.21 ���p�ɕύX
               s_Text = ZenToHan(s_Text);
               //*********************
            }
         }

         break;
   }

   Excel_Cell_DataSet(18, 4, s_Text);
   G_Log_Text = "�ڰэޗ�              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰь�
   //***************************
   // �wA011�x
//   i_RecNo = 1284;
   i_RecNo = 1106;            // ���x�v�Z���̃t���[�����ƍ��킹��
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2004.12.21 �����ύX
   double d_Text_Value;
   d_Text_Value = s_Text.ToDouble();
   d_Text_Value = Hasu_Round(0, d_Text_Value , 0);
   s_Text = FormatFloat("#,###,##0",d_Text_Value);
   //*******************
   Excel_Cell_DataSet(18, 14, s_Text);
   G_Log_Text = "�ڰь�               RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰђ��t���čޗ�
   //***************************
   // �wA012�x
//   i_RecNo = 306;
//   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1041;   // �ގ��w��
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if (( s_Text != "" ) && ( s_Text != "0000" ) ){
      s_Text  = Search_HD_MATERIAL(s_Text);
   } else {
      i_RecNo = 1021;   // BOL22
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(20, 4, s_Text);
   G_Log_Text = "�ڰђ��t���čޗ�     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰђ��t���ČĂьa
   //***************************
   // �wA013�x
   i_RecNo = 1019;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   G_Log_Text = "�ڰђ��t���ČĂьa   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���";
   if (s_Text == "42"){
      //2004.01.22
      //s_Text = "1 5/8-UN";
      s_Text = "1 5/8""-8UN";
      //**********
   } else if (s_Text == "52"){
      //2004.01.22
      //s_Text = "2-8UN";
      s_Text = "2""-8UN";
      //**********
   } else {
      s_Text = "M" + FormatFloat("0",s_Text.ToDouble());
   }

   Excel_Cell_DataSet(20, 11, s_Text);
   G_Log_Text = G_Log_Text + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰђ��t���Đ�
   //***************************
   // �wA014�x
   i_RecNo = 1020;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = s_Text.ToDouble() * 2;
      Excel_Cell_DataSet(20, 17, s_Text);
      G_Log_Text = "�ڰђ��t���Đ�       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�ڰђ��t���Đ�   RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //*********************************
   //***  �������� ����M��,�M�����@
   //*********************************
   // �wA015�x,�wA016�x
   if (StrToDblDef(G_HEADS_DATA[852].TrimRight(),0) >= StrToDblDef(G_HEADS_DATA[853].TrimRight(),0)) {
      //******************************************
      //***  (B�������x) >= (A�������x)�̂Ƃ�
      //******************************************

      // �������� ����M��
      i_RecNo = 832;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(29, 6, s_Text);
         G_Log_Text = "�������� ����M��    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "�������� ����M��  RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }

      // �������� �M����
      i_RecNo = 827;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(30, 6, s_Text);
         G_Log_Text = "�������� �M����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "�������� �M����  RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }

   } else {
      //******************************************
      //***  (B�������x) < (A�������x)�̂Ƃ�
      //******************************************

      // �������� ����M��
      i_RecNo = 827;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(29, 6, s_Text);
         G_Log_Text = "�������� ����M��    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "�������� ����M��  RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }

      // �������� �M����
      i_RecNo = 832;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
      if (AllFloatChk(s_Text.c_str(),0) == 0){
         d_HOLD_Value = s_Text.ToDouble();
         d_HOLD_Value = Hasu_Round(0, d_HOLD_Value, 4);

         s_Text = FormatFloat("#,###,##0.00",d_HOLD_Value);
         Excel_Cell_DataSet(30, 6, s_Text);
         G_Log_Text = "�������� �M����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
         Write_Log(G_Log_Text);
      } else {
         G_ErrLog_Text = "�������� �M����  RecNo:" + FormatFloat("0000",i_RecNo) + "  �s���Ȓl�w" + s_Text + "�x �ł��B";
         Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
         Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
      }
   }

   //************************
   //***  HEADS VER
   //************************
   // �wA017�x
   //2004.12.21 HEADS VER�폜
   //i_RecNo = 302;
   //if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
   //   i_RecNo = 1267;
   //   s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   //} else {
   //   i_RecNo = 1267;
   //   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //}
   //Excel_Cell_DataSet(40, 19, s_Text);
   //G_Log_Text = "HEADS VER           �w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   //************************

   //Windows 2008Server �ڐA�Ή�
   wkQuery->Close();
   delete wkQuery;
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
   G_Log_Text = "------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "----------    �w���^���͗e��  ���׏��x�쐬�I��    ----------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);
   G_Log_Text = "------------------------------------------------------------";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //delete wkQuery;

   return true;
}