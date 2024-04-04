//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�FBV��
//�@ �� �� �ҁFS.Ogita
//�@ �� �� ���F2007.03
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "HDPConst.h"   // �萔��`�p
#include "HDPValue.h"   // �ϐ���`�p
#include "HDPCom.h"     // �֐���`�p

#include "SINSEICom.h"  // �\�������ʊ֐���`�p

#include "Bas_ShomeiBV.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F BV��  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShomeiBV(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strWriteText;   // �`��f�[�^
   AnsiString strTmp;         // �ꎞ�ϐ�   
   int nRecNo;                // HEADS�f�[�^�z��ԍ�

   // �V�[�g�̎w��
   strSheetName = "BV��";
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("item",(OleVariant)strSheetName);

   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �wBV�؁x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);


   //***************************
   //***  �[����
   //***************************
   nRecNo = 1262;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(11, 5, strWriteText);
   Excel_Cell_DataSet(13, 5, strWriteText);   
   G_Log_Text = "BV�؁F�[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �D��
   //***************************
   nRecNo = 28;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(14, 16, strWriteText);
   G_Log_Text = "BV�؁F�D��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(21, 5, strWriteText);
   G_Log_Text = "BV�؁F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �^��
   //***************************
   nRecNo = 33;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(23, 9, strWriteText);
   G_Log_Text = "BV�؁F�^��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  L�^�t��
   //***************************
   nRecNo = 76;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(24, 9, strWriteText);
   G_Log_Text = "BV�؁FL�^�t��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �@�햼��
   //***************************
   nRecNo = 26;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(26, 9, strWriteText);   
   G_Log_Text = "BV�؁F�@�햼��        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(20, 11, strWriteText);
   Excel_Cell_DataSet(27,  9, strWriteText);
   Excel_Cell_DataSet(45,  12, strWriteText);
   G_Log_Text = "BV�؁F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �d�M�ʐ�
   //***************************
   nRecNo = 1067;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(28, 9, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV�؁F�d�M�ʐ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �v���[�g�ގ�(�p��)
   //***************************
   nRecNo = 43;
   strWriteText = ConvertMaterialJtoE(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(25, 9, strWriteText);
   G_Log_Text = "BV�؁F�v���[�g�ގ�(�p��)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �v���[�g�ގ�
   //***************************
   nRecNo = 43;
   strWriteText = Search_HD_MATERIAL(G_HEADS_DATA[nRecNo].Trim());
   strWriteText += ")";
   Excel_Cell_DataSet(25, 14, strWriteText);
   G_Log_Text = "BV�؁F�v���[�g�ގ�        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���݌v����
   //***************************
   nRecNo = 830;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29, 12, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV�؁FA���݌v����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);   

   //***************************
   //***  B���݌v����
   //***************************
   nRecNo = 825;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(29,16, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV�؁FB���݌v����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A���t��(�Z)
   //***************************
   nRecNo = 1073;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(29, 9, strWriteText);
   Excel_Cell_DataSet(30, 9, strWriteText);
   G_Log_Text = "BV�؁FA���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B���t��(�Z)
   //***************************
   nRecNo = 1071;
   strWriteText = GetLiquidShortName(G_HEADS_DATA[nRecNo].Trim());
   Excel_Cell_DataSet(29, 14, strWriteText);
   Excel_Cell_DataSet(30, 14, strWriteText);
   G_Log_Text = "BV�؁FB���t��(�Z)        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  A����������
   //***************************
   nRecNo = 832;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(30, 12, FormatFloat("##,##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV�؁FA����������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  B����������
   //***************************
   nRecNo = 827;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(30,16, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV�؁FB����������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����
   //***************************
   nRecNo = 288;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31, 9, strWriteText);
   G_Log_Text = "BV�؁F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����
   //***************************
   nRecNo = 287;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31,12, strWriteText);
   G_Log_Text = "BV�؁F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �t���[����
   //***************************
   nRecNo = 1026;
   strWriteText = G_HEADS_DATA[nRecNo].Trim();
   Excel_Cell_DataSet(31, 14, strWriteText);
   G_Log_Text = "BV�؁F�t���[����        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �ō���������
   //***************************
   strWriteText = GetHighPressureValueAorB();
   Excel_Cell_DataSet(45, 8, FormatFloat("##0.00",StrToCurr(strWriteText)));
   G_Log_Text = "BV�؁F�ō���������        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   return true;
}
