//---------------------------------------------------------------------------

#ifndef HDP010H
#define HDP010H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
// 2021.06.18 FireDAC�ւ̈ڍs DEL_S
//#include <DBTables.hpp>
// 2021.06.18 FireDAC�ւ̈ڍs DEL_E
#include <Grids.hpp>

#include <Filectrl.hpp>

//---------------------------------------------------------------------------
class TfrmHDP010 : public TForm
{
__published:	// IDE �Ǘ��̃R���|�[�l���g
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall FormPaint(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);

private:	   // ���[�U�[�錾
   bool __fastcall Log_File_SizeCheck(void);                      // ���O�t�@�C���@�T�C�Y�`�F�b�N
   bool __fastcall Connect_DataBase(void);                        // �ް��ް��ڑ�����
   bool __fastcall Get_Sakuban(void);                             // ��Ԏ擾
   bool __fastcall OutPut_Folder_Check(AnsiString Check_Pass);    // �o�͐�̫�������
   bool __fastcall Read_CodeMasta_Data(void);                     // �K�i����,�ƊE����,�h���F �Ǎ�
   //2009.03.03 ���͗e�틭�x�v�Z�ύX
   bool __fastcall Aturyoku_Run(void);                            // ���͗e�틭�x�v�Z���쐬��۸���  �N��
   //*******************************
   //2017.10.19 �h�����匟���[_S
   bool __fastcall TosoJisyuKensa_Run(void);                      // �h�����匟���[�쐬��۸���  �N��
   //2017.10.19 �h�����匟���[_E
   //2020.04.20 ���������o��_S
   bool __fastcall HotAirDrying_Run(void);                        // ���������o����۸���  �N��
   //2020.04.20 ���������o��_E

   // EXCEL�p
   short __fastcall CallExcel(bool showflg);                      // Excel�N������
   void  __fastcall QuitExcel(void);

   void __fastcall Heads_Excel_Main(void);
   void __fastcall Excel_Run(void);

   // 2004.09.23 �ǉ�
   void __fastcall MovePDFFile(void);                             // PDF̧�ق̏o��
   void __fastcall OutPut_File_Move(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg);       // ̧�ق̌����A��߰
   void __fastcall MoveFolder_Job(void);                          // �t�H���_�̈ړ�
   bool __fastcall MoveDirectoryEvo(AnsiString FromFile, AnsiString ToFile);       //2004/11/02
   bool __fastcall TfrmHDP010::removeDirectoryEvo(AnsiString fileName);            //2004/11/03

   int prvFrmLoadFlg;

   void __fastcall MoveFolder_Job1(void);                          // �t�H���_�̈ړ�
   void __fastcall OutPut_File_Move1(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg);       // ̧�ق̌����A��߰
   bool __fastcall MoveDirectoryEvo1(AnsiString FromFile, AnsiString ToFile);       //2004/11/02

   bool __fastcall MoveDirectoryEvoRev(AnsiString FromFile, AnsiString ToFile, AnsiString sRev);       //2015/10/16
   void __fastcall OutPut_File_MoveRev(AnsiString FromFile, AnsiString ToFile, AnsiString Files, bool DelFlg, AnsiString sRev);       // ̧�ق̌����A��߰

   AnsiString P_SQLText;
   AnsiString P_Sakuban_Mode;    // ��Ԏ��(S:�����ԍ��CM:���ϔԍ�)
   AnsiString P_UtiKbn;          // �ŏo���敪

   //��2007.10.9 �~���[�d�l�Ή�
   HeadsData*  m_pcHeadsData;
   AnsiString P_HEADS_DATA[1500];
   //��2007.10.9 �~���[�d�l�Ή�

public:		// ���[�U�[�錾
   __fastcall TfrmHDP010(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHDP010 *frmHDP010;
//---------------------------------------------------------------------------
#endif
