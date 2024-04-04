//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�\����(NK)
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

#include "Bas_ShinseiNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F �\����(��NK)  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShinseiNK(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strWriteText;   // �`��f�[�^
   int nRecNo;                // HEADS�f�[�^�z��ԍ�

   // �V�[�g�̎w��
   strSheetName = "NK�\";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �w�\����(NK�\)�x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@�햼��
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(21, 6, strWriteText);
   G_Log_Text = "NK�\�F�@�햼��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �`��
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(22, 6, strWriteText);
   G_Log_Text = "NK�\�F�`��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(22, 15, strWriteText);
   G_Log_Text = "NK�\�F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �d�M�ʐ�
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(23, 8, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "NK�\�F�d�M�ʐ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   // 2013.04.03 ���ޏ����ύX
   //***************************
   //***  �v���[�g�ގ�
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(24, 6, strWriteText);
   G_Log_Text = "NK�\�F�v���[�g�ގ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //**************************
   //***  �K�X�P�b�g�ގ�
   //***************************
   nRecNo = 835;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   nRecNo = 836;
   if ( G_HEADS_DATA[835].Trim() != G_HEADS_DATA[nRecNo].Trim() ) {
	  strWriteText += "�^";
	  strWriteText += Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   }
   Excel_Cell_DataSet(25, 6, strWriteText);
   G_Log_Text = "NK�\�F�K�X�P�b�g�ގ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);
   // ***********************

   //***************************
   //***  B���t��(�Z)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   // 2013.04.03 ���ޏ����ύX
   //Excel_Cell_DataSet(24, 6, strWriteText);
   Excel_Cell_DataSet(31, 8, strWriteText);
   // ***********************
   G_Log_Text = "NK�\�FB���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B����������
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   // 2013.04.03 ���ޏ����ύX
   //Excel_Cell_DataSet(24, 9, FormatFloat("##0.00",StrToCurr(strWriteText)));
   Excel_Cell_DataSet(31, 10, FormatFloat("##0.00",StrToCurr(strWriteText)));
   // ***********************
   G_Log_Text = "NK�\�FB����������       RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���t��(�Z)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   // 2013.04.03 ���ޏ����ύX
   //Excel_Cell_DataSet(24, 11, strWriteText);
   Excel_Cell_DataSet(32, 8, strWriteText);
   // ***********************
   G_Log_Text = "NK�\�FA���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A����������
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   // 2013.04.03 ���ޏ����ύX
   //Excel_Cell_DataSet(24, 15, FormatFloat("##0.00",StrToCurr(strWriteText)));
   Excel_Cell_DataSet(32, 10, FormatFloat("##0.00",StrToCurr(strWriteText)));
   // ***********************
   G_Log_Text = "NK�\�FA����������       RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(27, 6, strWriteText);
   G_Log_Text = "NK�\�F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  ������
   //***************************
   nRecNo = 1063;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 6, strWriteText);
   G_Log_Text = "NK�\�F������       RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �D��
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 8, strWriteText);
   G_Log_Text = "NK�\�F�D��       RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);


   // �W��N�����}�N��
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 6 << 11);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!K47" );
   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 6, 11);
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!K47") );

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 6 << 13);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!M47" );

   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 6, 13);
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!M47") );

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 6 << 15);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!O47" );

   //Windows 2008Server �ڐA�Ή�
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 6, 15);
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!O47") );

   // �� �� �� �� �� �{ �� �� �}�N��
   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 33 << 7);
   //exWorkCell.Exec(PropertySet("Value") << "=DATE('NK-FAX'!K49-1988,'NK-FAX'!M49,'NK-FAX'!O49)" );
   //Windows 2008Server �ڐA�Ή�
   // 2013.04.03 ���ޏ����ύX
   //exWorkCell = exWorksheet.OlePropertyGet("Cells", 33, 7);
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 34, 7);
   // ***********************
   exWorkCell.OlePropertySet("Value",OleVariant("=DATE('NK-FAX'!K49-1988,'NK-FAX'!M49,'NK-FAX'!O49)" ));

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 33 << 9);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!M49" );
   //Windows 2008Server �ڐA�Ή�
   // 2013.04.03 ���ޏ����ύX
   //exWorkCell = exWorksheet.OlePropertyGet("Cells", 33, 9);
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 34, 9);
   // ***********************
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!M49") );

   //exWorkCell = exWorksheet.Exec(PropertyGet("Cells") << 33 << 11);
   //exWorkCell.Exec(PropertySet("Value") << "='NK-FAX'!O49" );
   //Windows 2008Server �ڐA�Ή�
   // 2013.04.03 ���ޏ����ύX
   //exWorkCell = exWorksheet.OlePropertyGet("Cells", 33, 11);
   exWorkCell = exWorksheet.OlePropertyGet("Cells", 34, 11);
   // ***********************
   exWorkCell.OlePropertySet("Value",OleVariant("='NK-FAX'!O49") );

   return true;

}
