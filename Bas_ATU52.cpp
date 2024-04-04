//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���͗e�틭�x�v�Z��(ATU52)  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.06.06
//�@ �X �V ���F2002.08.23
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Bas_ATU52.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ���͗e�틭�x�v�Z��(ATU52)�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Atu52(void)
{
   AnsiString s_Text;            // �ėp������
   AnsiString s_SheetName;       // ��Ė���
   int        i_RecNo;           // HEADS�ް� ں���No

   //*******************
   //***  ��Ă̎w��
   //*******************
   s_SheetName = "���x�v�Z��52";
   // ��Ă̎w��
   exWorksheet = exWorksheets.Exec(PropertyGet("Item") << s_SheetName);

   //********************************************
   //***  ���͗e�틭�x�v�Z���@�f�[�^�Z�b�g
   //********************************************
   G_Log_Text = "***************  " + s_SheetName + "�쐬�J�n  ***************";
   Write_Log(G_Log_Text);
   Write_Error_Log(G_Log_Text);

   //***************************
   //***  ���čގ�
   //***************************
   // �wA001�x
   i_RecNo = 1021;
   s_Text = "(" + G_HEADS_DATA[i_RecNo].TrimRight() + ")";
   Excel_Cell_DataSet(3, 1, s_Text);
   G_Log_Text = "���čގ�            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �޽��Ď���
   //***************************
   // �wA002�x
   i_RecNo = 1271;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(5, 14, s_Text);
   Excel_Cell_DataSet(9, 25, s_Text);
   G_Log_Text = "�޽��Ď���          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ���t�d��
   //***************************
   // �wA003�x
   i_RecNo = 1273;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(5, 22, s_Text);
   Excel_Cell_DataSet(19, 9, s_Text);
   G_Log_Text = "���t�d��            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ��t�ʐ�
   //***************************
   // �wA004�x
   i_RecNo = 1272;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

      Excel_Cell_DataSet(12, 9, s_Text);
      Excel_Cell_DataSet(15, 25, s_Text);

      G_Log_Text = "��t�ʐ�            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "��t�ʐ�              �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //***  �ō��g�p����
   //***************************
   // �wA005�x
   i_RecNo = 1102;
   s_Text = G_HEADS_DATA[i_RecNo].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      s_Text = FormatFloat("#,###,##0.00",StrToFloat(s_Text));

      Excel_Cell_DataSet(12, 17, s_Text);
      Excel_Cell_DataSet(16, 25, s_Text);

      G_Log_Text = "�ō��g�p����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   } else {
      G_ErrLog_Text = "�ō��g�p����          �s���Ȓl�w" + s_Text + "�x �ł��B";
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***************************
   //*** �����ɂ��׏d
   //***************************
   // �wA006�x
   i_RecNo = 1274;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(12, 27, s_Text);
   Excel_Cell_DataSet(19, 19, s_Text);
   G_Log_Text = "�����ɂ��׏d      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** ���v�׏d
   //***************************
   // �wA007�x
   i_RecNo = 1275;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(19, 29, s_Text);
   Excel_Cell_DataSet(26, 9, s_Text);
   G_Log_Text = "���v�׏d            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** ���Ė{���ɂ��萔
   //***************************
   // �wA008�x
   i_RecNo = 1112;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(24, 8, s_Text);
   Excel_Cell_DataSet(24, 31, s_Text);
   Excel_Cell_DataSet(26, 18, s_Text);
   G_Log_Text = "���Ė{���ɂ��萔  RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** �����߯�����
   //***************************
   // �wA009�x
   i_RecNo = 1113;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(26, 27, s_Text);
   Excel_Cell_DataSet(31, 30, s_Text);
   G_Log_Text = "�����߯�����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** �������߯�����
   //***************************
   // �wA010�x
   i_RecNo = 1114;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(27, 20, s_Text);
   Excel_Cell_DataSet(32, 30, s_Text);
   G_Log_Text = "�������߯�����      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** ���Ĉ�{�̍ő�׏d
   //***************************
   // �wA011�x
   i_RecNo = 1116;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(41, 8, s_Text);
   Excel_Cell_DataSet(29, 8, s_Text);
   G_Log_Text = "���Ĉ�{�ő�׏d    RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** ���ċ��e����
   //***************************
   // �wB001�x
   i_RecNo = 1118;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(42, 16, s_Text);
   Excel_Cell_DataSet(44, 28, s_Text);
   G_Log_Text = "���ċ��e����        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** ���ĕK�v�a
   //***************************
   // �wB002�x
   i_RecNo = 1117;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   Excel_Cell_DataSet(41, 24, s_Text);
   G_Log_Text = "���ĕK�v�a          RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** �ڰђ��t���ČĂьa
   //***************************
   // �wB003�x
   i_RecNo = 1019;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.10 UNȼޒ���
   //if (s_Text == "42") {
   if (s_Text == "1.630") {
   //   s_Text = "1 5/8-8UN";
      //2004.01.24
      //s_Text = "1 5/8-8UN�i�J�a";
      s_Text = "1 5/8""-8UN�i�J�a";
      //**********
   //} else if (s_Text == "52"){
   } else if (s_Text == "2.000"){
   //   s_Text = "2-8UN";
      //2004.01.24
      //s_Text = "2-8UN�i�J�a";
      s_Text = "2""-8UN�i�J�a";
      //**********
   } else {
   //   s_Text = "M" + FormatFloat("0",s_Text.ToDouble());
      s_Text = "M" + FormatFloat("0",s_Text.ToDouble()) + " �i�J�a";
   }
   //********************
   Excel_Cell_DataSet(46, 4, s_Text);
   G_Log_Text = "���t���ČĂьa      RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + G_HEADS_DATA[i_RecNo].TrimRight()
              + "�x���w" +  s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** ���ĒJ�a
   //***************************
   // �wB004�x
   i_RecNo = 1119;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   //2003.09.10 UNȼޒ���
   //Excel_Cell_DataSet(46, 11, s_Text);
   Excel_Cell_DataSet(46, 14, s_Text);
   //********************
   G_Log_Text = "���ĒJ�a            RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //*** �Б����Ė{��
   //***************************
   // �wB005�x
   i_RecNo = 1020;
   s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   s_Text = FormatFloat("0", s_Text.ToDouble() * 2);
   //2003.09.10 UNȼޒ���
   //Excel_Cell_DataSet(46, 23, s_Text);
   Excel_Cell_DataSet(46, 24, s_Text);
   //********************
   G_Log_Text = "�Б����Ė{��        RecNo:" + FormatFloat("0000",i_RecNo) + "�w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   // �wB006�x
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ԍ������ǉ��Ή� Mod_E   
   Excel_Cell_DataSet(56, 26, s_Text);
   G_Log_Text = "�����ԍ�           �w" + s_Text + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  HEADS VER
   //***************************
   // �wB007�x
   i_RecNo = 302;
   if (G_HEADS_DATA[i_RecNo].TrimRight() == "1"){
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight() + "S";
   } else {
      i_RecNo = 1267;
      s_Text = G_HEADS_DATA[i_RecNo].TrimRight();
   }
   Excel_Cell_DataSet(56, 3, s_Text);
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
      
   return true;
}