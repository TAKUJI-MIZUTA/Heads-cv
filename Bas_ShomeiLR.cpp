//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�FLR��
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "SINSEICom.h"  // �\�������ʊ֐���`�p

#include "Bas_ShomeiLR.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F LR��  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShomeiLR(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strWriteText;   // �`��f�[�^
   AnsiString strTmp;         // �ꎞ�ϐ�   
   int nRecNo;                // HEADS�f�[�^�z��ԍ�

   // �V�[�g�̎w��
   strSheetName = "LR��";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �wLR�؁x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(15, 11, strWriteText);
   Excel_Cell_DataSet(27,  5, strWriteText);
   Excel_Cell_DataSet(48,  7, strWriteText);      
   G_Log_Text = "LR�؁F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �D��
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strTmp = strWriteText;
   //Excel_Cell_DataSet(17, 12, strWriteText);
   //G_Log_Text = "LR�؁F�D��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   //Write_Log(G_Log_Text);

   //***************************
   //***  �[����
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].TrimRight();
   Excel_Cell_DataSet(14, 4, strWriteText);
   G_Log_Text = "LR�؁F�[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   strTmp += " of " + strWriteText;
   strWriteText = strTmp;
   Excel_Cell_DataSet(17,12, strWriteText);
   G_Log_Text = "LR�؁F�[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   //***************************
   //***  �@�햼��
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(25, 5, strWriteText);   
   G_Log_Text = "LR�؁F�@�햼��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �^��
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(26, 4, strWriteText);
   G_Log_Text = "LR�؁F�^��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�("-"�Ȃ�)
   //***************************
   strWriteText = GetSerialNo();
   Excel_Cell_DataSet(27, 13, strWriteText);
   G_Log_Text = "LR�؁F�����ԍ�(- �Ȃ�)        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����(����؂�g�p)
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(28, 3, strWriteText);
   // DATE
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 28 << 3);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 28, 3);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "LR�؁F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����(����؂�g�p)
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   G_Log_Text = "LR�؁F�t���[����(����؂�g�p 1/2)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(28, 6, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 28 << 6);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 28, 6);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );
   G_Log_Text = "LR�؁F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����(����؂�g�p)
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
//   strWriteText = FormatFloat("#,##0",StrToCurr(strWriteText));
//   Excel_Cell_DataSet(28, 8, strWriteText);
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 28 << 8);
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 28, 8);
   //exWorkCell.Exec(PropertySet("Value") << strWriteText );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell.OlePropertySet("Value", (OleVariant)strWriteText );

   G_Log_Text = "LR�؁F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@��d��
   //***************************
   nRecNo = 1029;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 11, strWriteText);
   G_Log_Text = "LR�؁F�@��d��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �d�M�ʐ�
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 6, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR�؁F�d�M�ʐ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // ----------------------------------------   
   // ���X����
   // �䐔(�p)�Ƒ䐔�͌������āAExcel�ɕ`�悷��
   //***************************
   //***  �䐔(�p)
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   strWriteText = NumeralJtoEHeadCapital(strWriteText);
   G_Log_Text = "LR�؁F�䐔(�p)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp = strWriteText;
   strTmp += "(";

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   G_Log_Text = "LR�؁F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   strTmp += ")";
   Excel_Cell_DataSet(31, 2, strTmp);
   // ----------------------------------------

   // ----------------------------------------   
   // ���X����
   // �v���[�g�ގ�(�p��)�ƃv���[�g�ގ��͌������āAExcel�ɕ`�悷��
   //***************************
   //***  �v���[�g�ގ�(�p��)
   //***************************
   nRecNo = 43;
   strWriteText = ConvertMaterialJtoE(G_HEADS_DATA[nRecNo].Trim());
   G_Log_Text = "LR�؁F�v���[�g�ގ�(�p��)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp = strWriteText;
   strTmp += "(";

   //***************************
   //***  �v���[�g�ގ�
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   G_Log_Text = "LR�؁F�v���[�g�ގ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   strTmp += strWriteText;
   strTmp += ")";

   Excel_Cell_DataSet(35, 3, strTmp);
   // ----------------------------------------

   //***************************
   //***  B���t��(�Z)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(45, 2, strWriteText);
   G_Log_Text = "LR�؁FB���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B����������
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(45, 5, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR�؁FB����������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���t��(�Z)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(45, 9, strWriteText);
   G_Log_Text = "LR�؁FA���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A����������
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(45, 11, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR�؁FA����������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ō���������
   //***************************
   strWriteText = GetHighPressureValueAorB();
   Excel_Cell_DataSet(50, 4, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "LR�؁F�ō���������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   return true;
}
