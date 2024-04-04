//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���p�v�ڕ\  �쐬���W���[��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.05.30
//�@ �X �V ���F2002.05.30
//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "DataModule.h"

#include "Funeyou.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ���v�ڕ\�@�쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Funeyou(void)
{
   AnsiString s_Tmplate_Pass;    // ����ڰ�̧��(���߽)
   AnsiString s_MakeExcel_Pass;  // �쐬̧�ٖ�
   AnsiString s_Text;

   s_Tmplate_Pass = G_Template_Dir + "���p�v�ڕ\.xls";      // ����ڰ�̧��
   s_MakeExcel_Pass = G_xls_Output_Dir + "M" + G_SAKUBAN + ".xls";  // ��߰��̧��

   // ۸ލ쐬
   G_Log_Text = "----------    �w���p�v�ڕ\�x�쐬�J�n    ----------";
   Write_Log(G_Log_Text);
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
   exWorkbook  = exWorkbooks.Exec(Function("Open") << s_MakeExcel_Pass);
   // ��ĺڸ��ݎ擾
   exWorksheets = exWorkbook.Exec(PropertyGet("WorkSheets"));

   //******************************
   //***  �f�[�^�@�Z�b�g�i�a���j
   //******************************
   Funeyou_Data_Set("J");


   //******************************
   //***  Excel Book Close
   //******************************
   exApplication.Exec(PropertySet("DisplayAlerts") << "false");
   exApplication.Exec(PropertyGet("ActiveWorkBook")).Exec(Function("SaveAs") << s_MakeExcel_Pass);
   exWorkbook.OleFunction("Close");
   exWorkbooks.OleFunction("Close");

   // ۸ލ쐬
   G_Log_Text = "----------    �w���p�v�ڕ\�x�쐬�I��    ----------";
   Write_Log(G_Log_Text);

   return true;
}

//---------------------------------------------------------------------------
// ���{��֐����F ���v�ڕ\�@Excel�f�[�^�Z�b�g
// �T  �v      �F
// ��  ��      �F AnsiString Lang  // ����(J:�a���CE:�p��)
// �߂�l      �F �Ǎ����� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall Funeyou_Data_Set(AnsiString Lang)
{
   AnsiString s_Text;

   // ��Ă̎w��
   if (Lang == "J"){
      exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�a��");
   } else {
      exWorksheet = exWorksheets.Exec(PropertyGet("Item") << "�p��");
   }

   //***  DOC.No.  ***
   s_Text = "M" + G_SAKUBAN;
   Excel_Cell_DataSet(1, 13, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FDOC.No.      �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FDOC.No.      �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  MFG.No.  ***
   //2022/12/15 �����ǉ��Ή� Mod_S
   //s_Text = G_SAKUBAN.SubString(1,2) + "-" + G_SAKUBAN.SubString(3,4);
   s_Text = GetSeizouBangou(G_SAKUBAN);
   //2022/12/15 �����ǉ��Ή� Mod_E
      
   Excel_Cell_DataSet(3, 1, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FMFG.No.      �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FMFG.No.      �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  �^��  ***
   if (G_HEADS_DATA[1057].TrimRight() == "1"){
      s_Text = G_HEADS_DATA[540].TrimRight() + "�i���j";
   } else {
      s_Text = G_HEADS_DATA[540].TrimRight();
   }
   Excel_Cell_DataSet(3, 3, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���F�^��         �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���F�^��         �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  �䐔  ***
   s_Text = G_HEADS_DATA[39];
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      Excel_Cell_DataSet(2, 11, s_Text);
      if (Lang == "J"){
         G_Log_Text = "�a���F�䐔         �w" + s_Text + "�x���Z�b�g�B";
      } else {
         G_Log_Text = "�p���F�䐔         �w" + s_Text + "�x���Z�b�g�B";
      }
      Write_Log(G_Log_Text);
   } else {
      if (Lang == "J"){
         G_ErrLog_Text = "�a���F�䐔      �s���Ȓl�w" + s_Text + "�x �ł��B";
      } else {
         G_ErrLog_Text = "�p���F�䐔      �s���Ȓl�w" + s_Text + "�x �ł��B";
      }
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***  �K�p���H  ***
   s_Text = G_HEADS_DATA[29].TrimRight();
   Excel_Cell_DataSet(5, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���F�K�p���     �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���F�K�p���     �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  �D��  ***
   s_Text = G_HEADS_DATA[777].TrimRight();
   Excel_Cell_DataSet(6, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���F�D��         �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���F�D��         �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  �`�M�ʐ�  ***
   s_Text = G_HEADS_DATA[1067].TrimRight();
   Excel_Cell_DataSet(7, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���F�`�M�ʐ�     �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���F�`�M�ʐ�     �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  �@�펿�� DRY  ***
   s_Text = G_HEADS_DATA[1029].TrimRight();
   Excel_Cell_DataSet(8, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���F�@�펿�� DRY �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���F�@�펿�� DRY �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  �@�펿�� DRY  ***
   s_Text = G_HEADS_DATA[1030].TrimRight();
   Excel_Cell_DataSet(9, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���F�@�펿�� WET �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���F�@�펿�� WET �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  B���t�̖�  ***
   s_Text = G_HEADS_DATA[552].TrimRight();
   Excel_Cell_DataSet(11, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FB���t�̖�    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FB���t�̖�    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  A���t�̖�  ***
   s_Text = G_HEADS_DATA[583].TrimRight();
   Excel_Cell_DataSet(11, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FA���t�̖�    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FA���t�̖�    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  B�������x  ***
   s_Text = G_HEADS_DATA[852].TrimRight();
   Excel_Cell_DataSet(12, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FB�������x    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FB�������x    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  A�������x  ***
   s_Text = G_HEADS_DATA[853].TrimRight();
   Excel_Cell_DataSet(12, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FA�������x    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FA�������x    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  B�o�����x  ***
   s_Text = G_HEADS_DATA[854].TrimRight();
   Excel_Cell_DataSet(13, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FB�o�����x    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FB�o�����x    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  A�o�����x  ***
   s_Text = G_HEADS_DATA[855].TrimRight();
   Excel_Cell_DataSet(13, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FA�o�����x    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FA�o�����x    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  B����  ***
   s_Text = G_HEADS_DATA[856].TrimRight();
   Excel_Cell_DataSet(14, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FB����        �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FB����        �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  A����  ***
   s_Text = G_HEADS_DATA[857].TrimRight();
   Excel_Cell_DataSet(14, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FA����        �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FA����        �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);


   //***  B���͑���  ***
   s_Text = G_HEADS_DATA[858].TrimRight();
   Excel_Cell_DataSet(15, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FB���͑���    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FB���͑���    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  A���͑���  ***
   s_Text = G_HEADS_DATA[859].TrimRight();
   Excel_Cell_DataSet(15, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FA���͑���    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FA���͑���    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  B�݌v����  ***
   s_Text = G_HEADS_DATA[825].TrimRight();
   Excel_Cell_DataSet(16, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FB�݌v����    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FB�݌v����    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  A�݌v����  ***
   s_Text = G_HEADS_DATA[830].TrimRight();
   Excel_Cell_DataSet(16, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FA�݌v����    �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FA�݌v����    �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  B�e�X�g����  ***
   s_Text = G_HEADS_DATA[827].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(17, 4, s_Text);
      if (Lang == "J"){
         G_Log_Text = "�a���FB�e�X�g����  �w" + s_Text + "�x���Z�b�g�B";
      } else {
         G_Log_Text = "�p���FB�e�X�g����  �w" + s_Text + "�x���Z�b�g�B";
      }
      Write_Log(G_Log_Text);

   } else {
      if (Lang == "J"){
         G_ErrLog_Text = "�a���FB�e�X�g����  �s���Ȓl�w" + s_Text + "�x �ł��B";
      } else {
         G_ErrLog_Text = "�p���FB�e�X�g����  �s���Ȓl�w" + s_Text + "�x �ł��B";
      }
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***  A�e�X�g����  ***
   s_Text = G_HEADS_DATA[832].Trim();
   if (AllFloatChk(s_Text.c_str(),0) == 0){
      if (s_Text.ToDouble() == 0){
         s_Text = "-----";
      } else {
         s_Text = FormatFloat("#,###,##0.00",StrToCurr(s_Text));
      }

      Excel_Cell_DataSet(17, 10, s_Text);
      if (Lang == "J"){
         G_Log_Text = "�a���FA�e�X�g����  �w" + s_Text + "�x���Z�b�g�B";
      } else {
         G_Log_Text = "�p���FA�e�X�g����  �w" + s_Text + "�x���Z�b�g�B";
      }
      Write_Log(G_Log_Text);

   } else {
      if (Lang == "J"){
         G_ErrLog_Text = "�a���FA�e�X�g����  �s���Ȓl�w" + s_Text + "�x �ł��B";
      } else {
         G_ErrLog_Text = "�p���FA�e�X�g����  �s���Ȓl�w" + s_Text + "�x �ł��B";
      }
      Write_Error_Log(G_ErrLog_Text);     // ÷�Ĵװ۸�
      Excel_Err_DataSet(G_ErrLog_Text);   // ���ٴװ۸�
   }

   //***  Bΰ��ޗ�  ***
   s_Text = G_HEADS_DATA[1282].TrimRight();
   Excel_Cell_DataSet(18, 4, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FBΰ��ޗ�     �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FBΰ��ޗ�     �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   //***  Aΰ��ޗ�  ***
   s_Text = G_HEADS_DATA[1283].TrimRight();
   Excel_Cell_DataSet(18, 10, s_Text);
   if (Lang == "J"){
      G_Log_Text = "�a���FAΰ��ޗ�     �w" + s_Text + "�x���Z�b�g�B";
   } else {
      G_Log_Text = "�p���FAΰ��ޗ�     �w" + s_Text + "�x���Z�b�g�B";
   }
   Write_Log(G_Log_Text);

   return true;
}
