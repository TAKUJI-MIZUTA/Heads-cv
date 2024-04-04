//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���͗e�틭�x�v�Z��(ATU21)  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.06.06
//�@ �X �V ���F2002.09.20
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_ATU21.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ���͗e�틭�x�v�Z��(ATU21)�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Atu21(void)
{
   TQuery *wkQuery;
   wkQuery = new TQuery(DataModule1);

   AnsiString s_Text;            // �ėp������
   AnsiString s_SheetName;       // ��Ė���
   AnsiString s_SQL;

   int        i_RecNo;           // HEADS�ް� ں���No

   //*******************
   //***  ��Ă̎w��
   //*******************
   s_SheetName = "���x�v�Z��21";
   // ��Ă̎w��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_SheetName);

   //********************************************
   //***  ���͗e�틭�x�v�Z���@�f�[�^�Z�b�g
   //********************************************
   G_Log_Text = "***************  " + s_SheetName + "�쐬�J�n  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //************************
   //***  �p�^�u��
   //************************
   // �wA001�x
   i_RecNo = 33;
   if (G_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "�i���j";
   } else {
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(3, 11, s_Text);
   G_Log_Text = "�p�^�u��          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �g�p���� �M����
   //***************************
   // �wA002�x
   i_RecNo = 1100;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( s_Text.ToDouble() >= 999) {
        s_Text = "��C�J��";
      } else {
        s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(5, 8, s_Text);
      G_Log_Text = "�g�p���� �M����     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�g�p���� �M����       �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �g�p���� ��M����
   //***************************
   // �wA003�x
   i_RecNo = 1098;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if ( s_Text.ToDouble() >= 999) {
        s_Text = "��C�J��";
      } else {
        s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      }
      Excel_Cell_DataSet(4, 8, s_Text);
      G_Log_Text = "�g�p���� ����M��   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�g�p���� ����M��     �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �g�p���x �M����
   //***************************
   // �wA004�x
   i_RecNo = 1101;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(5, 35, s_Text);
      G_Log_Text = "�g�p���x �M����     RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�g�p���x �M����       �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �g�p���x ��M����
   //***************************
   // �wA005�x
   i_RecNo = 1099;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(4, 35, s_Text);
      G_Log_Text = "�g�p���x ����M��   RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�g�p���x ����M��     �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �t���[���ގ�
   //***************************
   // �wA006�x
   switch (G_HEADS_DATA[37].ToIntDef(0)) {
      case 14: case 15: case 16: case 27: case 28: case 29: case 63: case 64:
         s_Text = G_HEADS_DATA[34].TrimRight();
         //2005.03.22 RX-10B�ǉ�
         //if ((s_Text.SubString(1,2) == "UX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==0) ||       //UX-01
         //    (s_Text.SubString(1,2) == "LX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ) {     //LX-10
         if ((s_Text.SubString(1,2) == "UX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==0) ||       //UX-01
             (s_Text.SubString(1,2) == "LX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ||      //LX-10
             (s_Text.SubString(1,2) == "RX" && G_HEADS_DATA[1011].TrimRight().ToIntDef(0)==10) ) {     //RX-10
         //*********************
            s_Text = "(�r�t�r�R�O�S)";

         } else {
            s_Text = "(�r�r�S�O�O)";
         }

         break;
      default :
         i_RecNo = 1044;   // SPFRM
         s_Text = G_HEADS_DATA[i_RecNo].TrimRight();

         if (s_Text == "0000" || s_Text == "1040"){
            s_Text = "(�r�r�S�O�O)";
         } else {
            s_SQL = "";
            s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
            s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

            wkQuery->Close();
            wkQuery->DatabaseName = ODBC_DSN_MAT;
            wkQuery->SQL->Clear();
            wkQuery->SQL->Add(s_SQL);
            wkQuery->Open();
            wkQuery->First();

            if(wkQuery->Eof){
               s_Text = "";
            } else {
               s_Text = "(" + wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight() + ")";
            }
         }

         break;
   }

   Excel_Cell_DataSet(7, 1, s_Text);
   G_Log_Text = "�t���[���ގ�     �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //*********************************
   //***  �ō��g�p���� (A007,A015)
   //*********************************
   // �wA007�x,�wA015�x
   i_RecNo = 1102;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(11, 32, s_Text);
      Excel_Cell_DataSet(28, 18, s_Text);
      G_Log_Text = "�ō��g�p����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�ō��g�p����          �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �ð�̕����߯�
   //***************************
   // �wA008�x,�wA010�x,�wA013�x
   i_RecNo = 1103;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(15, 32, s_Text);
   Excel_Cell_DataSet(17, 33, s_Text);
   Excel_Cell_DataSet(28, 10, s_Text);
   G_Log_Text = "�ð�̕����߯�       RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ő�~�̌a
   //***************************
   // �wA009�x,�wA011�x
   i_RecNo = 1104;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(17, 20, s_Text);
   Excel_Cell_DataSet(20, 32, s_Text);
   G_Log_Text = "�ő�~�̌a          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���e����
   //***************************
   // �wA012�x,�wA014�x
   i_RecNo = 1120;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(24, 33, s_Text);
   Excel_Cell_DataSet(29, 22, s_Text);
   G_Log_Text = "���e����            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �K�v��
   //***************************
   // �wA016�x
   i_RecNo = 1105;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(31, 10, s_Text);
   G_Log_Text = "�K�v��            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ڰє�
   //***************************
   // �wA017�x
   i_RecNo = 1106;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(33, 24, s_Text);
   G_Log_Text = "�ڰє�            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����߯�����
   //***************************
   // �wA018�x
   i_RecNo = 1115;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(22,  6, s_Text);
   G_Log_Text = "�����߯�����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����߯�����
   //***************************
   // �wA019�x
   i_RecNo = 1113;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(23,  6, s_Text);
   G_Log_Text = "�����߯�����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ������
   //***************************
   // �wB001�x
   i_RecNo = 1109;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 10, s_Text);
   G_Log_Text = "������            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ������
   //***************************
   // �wB002�x
   i_RecNo = 1110;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 13, s_Text);
   G_Log_Text = "������            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����()
   //***************************
   // �wB003�x
   i_RecNo = 1111;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(35, 18, s_Text);
   G_Log_Text = "����()            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �^�@��
   //***************************
   // �wB004�x
   s_Text = G_HEADS_DATA[1256].TrimRight() + "-" + G_HEADS_DATA[1257].TrimRight();
   Excel_Cell_DataSet(42, 3, s_Text);
   G_Log_Text = "�^  ��             �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2007.07.09 �o�͓��e�ύX
   ////***************************
   ////***  �@��
   ////***************************
   //// �wB005�x
   //i_RecNo = 1270;
   //s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //Excel_Cell_DataSet(42, 16, s_Text);
   //G_Log_Text = "��  ��              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);
   //***************************
   //***  ���@�y�є�
   //***************************
   // �wB005�x
   i_RecNo = 1268;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1269;
   s_Text = s_Text + "�~" + G_HEADS_DATA[i_RecNo].TrimRight();
   i_RecNo = 1270;
   s_Text = s_Text + "�~t" + G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(42, 16, s_Text);
   G_Log_Text = "���@�y�є�       �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //***********************

   //***************************
   //***  �ށ@��
   //***************************
   // �wB006�x
   i_RecNo = 43;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   s_SQL = "";
   s_SQL = s_SQL + "SELECT * FROM HD_MATERIAL";
   s_SQL = s_SQL + "  WHERE MAT_CODE = '" + s_Text + "';";

   wkQuery->Close();
   wkQuery->DatabaseName = ODBC_DSN_MAT;
   wkQuery->SQL->Clear();
   wkQuery->SQL->Add(s_SQL);
   wkQuery->Open();
   wkQuery->First();

   if(wkQuery->Eof){
      s_Text = "";
   } else {
      s_Text = wkQuery->FieldByName("JAPANESE_NAME")->AsString.TrimRight();
   }

   Excel_Cell_DataSet(42, 29, s_Text);
   G_Log_Text = "��  ��              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ō��g�p����
   //***************************
   // �wB007�x
   i_RecNo = 1107;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(44, 3, s_Text);
      G_Log_Text = "�ō��g�p����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�ō��g�p����          �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �ō��g�p���x
   //***************************
   // �wB008�x
   i_RecNo = 1108;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),1) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(44, 16, s_Text);
      G_Log_Text = "�ō��g�p���x        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�ō��g�p���x          �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �{��ō��g�p����
   //***************************
   // �wB009�x
   i_RecNo = 1102;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));
      Excel_Cell_DataSet(44, 29, s_Text);
      G_Log_Text = "�{��ō��g�p����    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�{��ō��g�p����      �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  ��ڰĔ�
   //***************************
   // �wB010�x
   i_RecNo = 1270;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(46, 20, s_Text);
   G_Log_Text = "��  ��              RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   // �wB011�x
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E
   Excel_Cell_DataSet(50, 32, s_Text);
   G_Log_Text = "�����ԍ�           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // �wB012�x
   i_RecNo = 302;
   if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   } else {
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(50, 3, s_Text);
   G_Log_Text = "HEADS VER          �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //********************************************
   //***
   //***  �o�c�e�o�͗p�@�ݒ���e��������
   //***
   //********************************************
   AnsiString s_Old_SheetName;
   AnsiString s_CellText;

   int i_ConfRow;

   // ���݂̃V�[�g���̂�ۑ�
   s_Old_SheetName = exWorksheet.OlePropertyGet("Name");
   // ��Ă̎w��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�o�c�e�ݒ�");

   // �ŏI�s�ɃZ�b�g
   s_CellText = "Def";
   i_ConfRow = 1;
   while (s_CellText != ""){
      exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << i_ConfRow << 2);
      s_CellText = exWorkCell.Exec(PropertyGet("Value"));

      i_ConfRow++;
   }

   // �o�c�e�o�͑Ώۼ�ĂƂ��Đݒ�
   exWorkCell.Exec(PropertySet("Value") << s_SheetName);

   // �ۑ����Ă������V�[�g���Ďw��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_Old_SheetName);

   G_Log_Text = "***************  " + s_SheetName + "�쐬�I��  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   delete wkQuery;
   
   return true;
}
