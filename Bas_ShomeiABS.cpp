//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�FABS��
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "SINSEICom.h"  // �\�������ʊ֐���`�p

#include "Bas_ShomeiABS.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F ABS��  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShomeiABS(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strWriteText;   // �`��f�[�^
   AnsiString strTmp;         // �ꎞ�ϐ�   
   int nRecNo;                // HEADS�f�[�^�z��ԍ�

   // �V�[�g�̎w��
   strSheetName = "ABS��";   
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �wABS�؁x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(12, 8, strWriteText);
   G_Log_Text = "ABS�؁F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �[����
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(14, 2, strWriteText);
   G_Log_Text = "ABS�؁F�[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �D��
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(14, 8, strWriteText);
   G_Log_Text = "ABS�؁F�D��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@��d��
   //***************************
   nRecNo = 1029;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(18, 8, strWriteText);
   G_Log_Text = "ABS�؁F�@��d��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�("-"�Ȃ�)
   //***  20070429("-"�L��ɕύX)
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(18, 2, strWriteText);
   // ����B���t�� "-"�Ȃ�
   strWriteText = GetSerialNo();
   strWriteText = "B" + strWriteText;
   Excel_Cell_DataSet(22, 2, strWriteText);
   //G_Log_Text = "ABS�؁F�����ԍ�(- �Ȃ�)        �w" + strWriteText + "�x���Z�b�g�B";
   G_Log_Text = "ABS�؁F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // ----------------------------------------
   // ���X����
   // �䐔(�p)�Ƒ䐔�Ƌ@�햼�̂͌������āAExcel�ɕ`�悷��
   //***************************
   //***  �䐔(�p)
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText = NumeralJtoEAllCapital(strWriteText);   
   G_Log_Text = "ABS�؁F�䐔(�p)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp = strWriteText;
   strTmp += "(";

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "ABS�؁F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   strTmp += ")";

   //***************************
   //***  �@�햼��
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "ABS�؁F�@�햼��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   Excel_Cell_DataSet(6, 5, strTmp);
   strTmp += "�F";
   Excel_Cell_DataSet(27, 3, strTmp);
   // ----------------------------------------

   //***************************
   //***  �^��
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(30, 5, strWriteText);
   G_Log_Text = "ABS�؁F�^��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����(����؂�g�p)
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(35, 5, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 35 << 5);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 35, 5);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "ABS�؁F�t���[����(����؂�g�p)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����(����؂�g�p)
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(35, 7, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 35 << 7);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 35, 7);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "ABS�؁F�t���[����(����؂�g�p)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����(����؂�g�p)
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(35, 9, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 35 << 9);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 35, 9);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "ABS�؁F�t���[����(����؂�g�p)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �d�M�ʐ�
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31, 5, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS�؁F�d�M�ʐ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���t��(�Z)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(32, 5, strWriteText);
   Excel_Cell_DataSet(33, 5, strWriteText);
   G_Log_Text = "ABS�؁FB���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���݌v����
   //***************************
   nRecNo = 825;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(32, 7, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS�؁FB���݌v����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���t��(�Z)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(32, 9, strWriteText);
   Excel_Cell_DataSet(33, 9, strWriteText);
   G_Log_Text = "ABS�؁FA���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���݌v����
   //***************************
   nRecNo = 830;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(32, 11, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS�؁FA���݌v����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �v���[�g�ގ�(�p��)
   //***************************
   nRecNo = 43;
   strWriteText = ConvertMaterialJtoE(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(34, 5, strWriteText);
   G_Log_Text = "ABS�؁F�v���[�g�ގ�(�p��)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �v���[�g�ގ�
   //*** 20070510 �C��
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(34, 9, strWriteText);
   G_Log_Text = "ABS�؁F�v���[�g�ގ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B����������
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(33, 7, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS�؁FB����������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A����������
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(33, 11, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "ABS�؁FA����������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   return true;
}
