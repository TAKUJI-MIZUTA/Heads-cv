//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�\����(��)
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "SINSEICom.h"  // �\�������ʊ֐���`�p

#include "DataModule.h"

#include "Bas_ShinseiNonNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
// ���{��֐����F �\����(��NK)  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShinseiNonNK(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strWriteText;   // �`��f�[�^
   int nRecNo;                // HEADS�f�[�^�z��ԍ�
   //2015.09.02 ���������ǉ�
   AnsiString strText;        // ���ԃf�[�^
   //***********************�@

   // �V�[�g�̎w��
   strSheetName = "���\";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �w�\����(���\)�x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  �K�i�� ��ӏ��ɏ��o��
   //***************************
   nRecNo = 29;
   strWriteText = Search_KIKAKU_CODE(G_HEADS_DATA[nRecNo].TrimRight(),true,true, G_HEADS_DATA[1].TrimRight());
   Excel_Cell_DataSet(1, 4, strWriteText);
   Excel_Cell_DataSet(27, 6, strWriteText);
   G_Log_Text = "���\�F�K�i��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@�햼��
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(18, 6, strWriteText);
   G_Log_Text = "���\�F�@�햼��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(19, 6, strWriteText);
   G_Log_Text = "���\�F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(19, 9, strWriteText);
   G_Log_Text = "���\�F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(19, 13, strWriteText);
   G_Log_Text = "���\�F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(20, 8, strWriteText);
   G_Log_Text = "���\�F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@��d��
   //***************************
   nRecNo = 1029;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(21, 8, strWriteText);
   G_Log_Text = "���\�F�@��d��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �d�M�ʐ�
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(22, 8, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "���\�F�d�M�ʐ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(24, 6, strWriteText);
   G_Log_Text = "NK-FAX�F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ������
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(25, 6, strWriteText);
   G_Log_Text = "���\�F������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �D��
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(26, 6, strWriteText);
   G_Log_Text = "���\�F�D��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //2015.09.02 ���������ǉ�
   //***************************
   //***  ��������
   //***************************
   strWriteText = "Hydraulic Test  (A SIDE: ";
   nRecNo = 832;
   strText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText += FormatFloat("##0.00",StrToCurr(strText));
   strWriteText += " MPa , B SIDE: ";
   nRecNo = 827;
   strText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText += FormatFloat("##0.00",StrToCurr(strText));
   strWriteText += " MPa )";
   Excel_Cell_DataSet(28, 6, strWriteText);
   G_Log_Text = "���\�F��������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //***********************�@

   //2014.04.15 �v���[�g�ގ��ǉ�
   //***************************
   //***  �v���[�g�ގ�
   //***************************
   strWriteText = "Plate Material : ";
   nRecNo = 43;
   strWriteText += Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(31, 6, strWriteText);
   G_Log_Text = "���\�F�v���[�g�ގ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   //***************************�@�@

   // DATE
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 1 << 13);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 1, 13);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('��-FAX'!O48,'��-FAX'!Q48,'��-FAX'!S48),\"mmm.dd.yyyy\")" );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value",OleVariant("=TEXT(DATE('��-FAX'!O48,'��-FAX'!Q48,'��-FAX'!S48),\"mmm.dd.yyyy\")" ));


   // DATE2
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 30 << 6);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 30, 6);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('��-FAX'!O50,'��-FAX'!Q50,'��-FAX'!S50),\"mmm.dd.yyyy\")" );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value",OleVariant("=TEXT(DATE('��-FAX'!O50,'��-FAX'!Q50,'��-FAX'!S50),\"mmm.dd.yyyy\")") );

   return true;
}
