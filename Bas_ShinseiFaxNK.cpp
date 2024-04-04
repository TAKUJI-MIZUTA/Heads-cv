//----------------------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F�\����FAX�p��(NK)
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

#include "Bas_ShinseiFaxNK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
// ���{��֐����F �\����FAX�p��(NK)  �쐬
// �T  �v      �F
// ��  ��      �F �Ȃ�
// �߂�l      �F �������� [true:���� false:�ُ�]
// ��  �l      �F �Ȃ�
//---------------------------------------------------------------------------
bool __fastcall ShinseiFaxNK(void)
{
   AnsiString strSheetName;   // �g�p�V�[�g��
   AnsiString strWriteText;   // �`��f�[�^
   int nRecNo;                // HEADS�f�[�^�z��ԍ�

   // �V�[�g�̎w��
   strSheetName = "NK-FAX";   
   //exWorksheet = exWorksheets.Exec(PropertyGet("Item") << strSheetName.c_str());
   //Windows 2008Server �ڐA�Ή�
   exWorksheet = exWorksheets.OlePropertyGet("Item" , (OleVariant)strSheetName);

   //******************************
   //***  �쐬�J�n۸ނ̏��o��
   //******************************
   G_Log_Text = "-----------    �wNK-FAX�x�쐬�J�n    -----------";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����ԍ�
   //***************************
   strWriteText = GetSerialNoWithHyphen();
   Excel_Cell_DataSet(21, 4, strWriteText);
   G_Log_Text = "NK-FAX�F�����ԍ�        �w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �䐔
   //***************************
   nRecNo = 39;
   strWriteText = G_HEADS_DATA[nRecNo].TrimRight();
   Excel_Cell_DataSet(21, 6, strWriteText);
   G_Log_Text = "NK-FAX�F�䐔        RecNo:" + FormatFloat("0000",nRecNo) + "�w" + strWriteText + "�x���Z�b�g�B";
   Write_Log(G_Log_Text);

   //***************************
   //***  �����s��
   //***************************
   strWriteText = GetCarvedSeal1st();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(47, 3, strWriteText);
      G_Log_Text = "NK-FAX�F�����s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  �����s��
   //***************************
   strWriteText = GetCarvedSeal2nd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(48, 3, strWriteText);
      G_Log_Text = "NK-FAX�F�����s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  ����O�s��
   //***************************
   strWriteText = GetCarvedSeal3rd();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(49, 3, strWriteText);
      G_Log_Text = "NK-FAX�F����O�s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  ����l�s��
   //***************************
   strWriteText = GetCarvedSeal4th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(50, 3, strWriteText);
      G_Log_Text = "NK-FAX�F����l�s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
   //***************************
   //***  ����܍s��
   //***************************
   strWriteText = GetCarvedSeal5th();
   if( strWriteText.Trim().Length() > 0 )
   {
      Excel_Cell_DataSet(51, 3, strWriteText);
      G_Log_Text = "NK-FAX�F����܍s��        �w" + strWriteText + "�x���Z�b�g�B";
      Write_Log(G_Log_Text);
   }
return true;
}
