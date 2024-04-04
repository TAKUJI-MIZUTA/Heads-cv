//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�FNK��
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "SINSEICom.h"  // �\�������ʊ֐���`�p

#include "Bas_ShomeiNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F NK��  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShomeiNK(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strWriteText;   // �`��f�[�^
   AnsiString strTmp;         // �ꎞ�ϐ�
   int nRecNo;                // HEADS�f�[�^�z��ԍ�

   // �V�[�g�̎w��
   strSheetName = "NK��";   
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);
   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �wNK�؁x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText = NumeralJtoEAllCapital(strWriteText);
   Excel_Cell_DataSet(15, 6, strWriteText);
   G_Log_Text = "NK�؁F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@�햼��
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(15, 13, strWriteText);
   G_Log_Text = "NK�؁F�@�햼��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // ----------------------------------------
   // ���X����
   // �D�ԂƔ[����͌������āAExcel�ɕ`�悷��B
   //***************************
   //***  �D��
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "NK�؁F�D��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   strTmp = strWriteText;
   strTmp += ",";

   //***************************
   //***  �[����
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "NK�؁F�[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;

   Excel_Cell_DataSet(19, 14, strTmp);
   // ----------------------------------------

   //***************************
   //***  �^��
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(25, 14, strWriteText);
   G_Log_Text = "NK�؁F�^��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(26, 11, strWriteText);
   G_Log_Text = "NK�؁F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���t��(�Z)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(28, 11, strWriteText);
   G_Log_Text = "NK�؁FB���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���݌v����
   //***************************
   nRecNo = 825;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 14, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK�؁FB���݌v����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���t��(�Z)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(28, 17, strWriteText);
   G_Log_Text = "NK�؁FA���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���݌v����
   //***************************
   nRecNo = 830;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 19, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK�؁FA���݌v����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �d�M�ʐ�
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 11, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK�؁F�d�M�ʐ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   // DATE
   //2008.07.24 ���t�o�͒�~
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 46 << 6);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('NK-FAX'!K47,'NK-FAX'!M47,'NK-FAX'!O47),\"mmm.dd.yyyy\")" );
   //***********************

   // Date of Manufacture
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 27 << 11);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 27, 11);
   //exWorkCell.Exec(PropertySet("Value") << "=TEXT(DATE('NK-FAX'!K49,'NK-FAX'!M49,'NK-FAX'!O49),\"mmm.dd.yyyy\")" );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value",OleVariant("=TEXT(DATE('NK-FAX'!K49,'NK-FAX'!M49,'NK-FAX'!O49),\"mmm.dd.yyyy\")" ));

   return true;
}
