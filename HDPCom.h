//---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���ʊ֐��p�w�b�_�t�@�C��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.05.23
//�@ �X �V ���F2002.06.18
//             2007.10.04 �װ�d�l�Ή� HeadsData�׽�ǉ��A�֐��Ɉ����ǉ�
//---------------------------------------------------------------------------

#ifndef C_HDPCOMH
#define C_HDPCOMH

#include <dstring.h>
#include <systdate.h>
#include <Grids.hpp>
#define  DefExcelPageLine1  65
#define  DefExcelPageLine   69
//---------------------------------------------------------------
class HeadsData;
//---------------------------------------------------------------
extern void  __fastcall IniFileRead(void);                                 // .ini̧�ٓǍ��p
extern void  __fastcall IniFileRead1(void);                                // .ini̧��1�Ǎ��p
extern void  __fastcall IniFileRead_Parts(void);                           // .ini̧��1�Ǎ��p

extern short __fastcall Numeric_Check(AnsiString,double*,int,int,int);     // ���l�͈�����
extern void  __fastcall SetInputEnabled(TObject*,bool);                    // ���͍��ڗ��ݒ�Ӽޭ��
extern short __fastcall StrErrCheck(AnsiString,short=0);                   // �g�p�֎~��������
extern short __fastcall AllNumChk(char*,short=0);                          // ������������
extern short __fastcall AllFloatChk(char*,short=0);                        // ����������������
extern short __fastcall AllAlphaChk(char*,short=0);                        // �p������������
extern short __fastcall TextErrCheck(TObject*);                            // �g�p�֎~��������
extern short __fastcall DateCheck(AnsiString,AnsiString*);                 // ���t����
extern short __fastcall KanmaStr(AnsiString&,int);                         // ��ϕ�����擾�p
extern short __fastcall AddKanma(TObject*,int);                            // ��ϕҏW�p
extern AnsiString __fastcall TxtEdit(AnsiString,int);                      // ÷�Ē��ݒ�
extern AnsiString __fastcall TxtEditRt(AnsiString,int);                    // ÷�Ē��ݒ�(���󔒍폜��)
extern AnsiString __fastcall GetIniKey(AnsiString);                        // .ini̧�ٓǍ��p�����擾
extern AnsiString __fastcall NO_Haifun(AnsiString);                        // ʲ�ݍ폜�p
extern AnsiString __fastcall F_NO_Comma(AnsiString);                       // ��ύ폜�p
extern AnsiString __fastcall NO_CoronCut(AnsiString);                      // �R�����܂Ŏ擾�p
extern AnsiString __fastcall YYtoYYYY_Str(AnsiString,short=1);             // YY��YYYY�ϊ�����
extern short  __fastcall YYtoYYYY(int);                                    // YY��YYYY���l�ϊ�����
extern double __fastcall Hasu_Round(int,double,int);                       // �l�␳����
extern Extended __fastcall Hasu_RoundE(int,Extended,int);                  // �l�␳����(Extended �^)
extern short  __fastcall SetPosCenter(TObject*,TObject*,int=0,int=0);      // ��޼ު�Ĉʒu�����ݒ菈��

#pragma option push -b-
enum DateKindT  { dsDays, dsSetDay, dsMonth, dsSubYear, dsYear };
#pragma option pop
extern short  __fastcall DateSubChk(AnsiString,int,int=0,int=0,int=1);

extern TDateTime __fastcall DateConv(int,int,int);                            // ���t�ϊ�����
extern TDateTime __fastcall DateCalc(TDateTime,int,int,int);                  // ���t�����Z�o����
extern short  __fastcall DecodeYMD(char*,WORD*,WORD*,WORD*);                  // ���t������ϊ�����(Y/M/D)
extern short  __fastcall DecodeMD(char*,WORD*,WORD*);                         // ���t������ϊ�����(M/D)
extern int    __fastcall GetDateSclDays(TDateTime,TDateTime);                 // �͈͓����擾����
extern int    __fastcall GetMonthDays(int,int);                               // �w�茎�����擾����
extern short  __fastcall ZeroSupress(AnsiString,AnsiString&,int);             // ��ۻ��ڽ�p("1"��"01")
extern short  __fastcall HalfKanaConv(AnsiString,AnsiString&,int=0);          // ���p�J�^�J�i�ϊ�
extern short  __fastcall WinFileNameChk(char*,short);                         // �t�@�C����(�ިڸ�ؖ�)����Ӽޭ��
extern double __fastcall StrToDblDef(AnsiString,double=0);                    // ������ToDoubleDef=0
extern char* __fastcall GetToken(char *Str, char *Sep);                       // �g�[�N���擾����
extern int __fastcall  ByteChrChk(char a);

extern bool __fastcall Template_Copy(AnsiString F_filename,AnsiString T_filename);  // ����ڰ�̧�ٺ�߰
extern AnsiString __fastcall ZenToHan(AnsiString str);                              // �S�p���������p����
extern AnsiString HanToZen(AnsiString str);                                         // ���p�������S�p����

//----------------------------
// 2007.10.04 �װ�d�l�Ή�
extern AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code,bool bWaEi,bool bDocTyp, AnsiString LangCode);
//extern AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code,bool bWaEi,bool bDocTyp);        // �K�i����ð��ٌ���
//extern AnsiString __fastcall Search_KIKAKU_CODE(AnsiString Code,bool bWaEi);        // �K�i����ð��ٌ���
//----------------------------

extern AnsiString __fastcall Search_TOSOU_CODE(AnsiString Code, AnsiString Sw);     // �h���Fð��ٌ���

extern void __fastcall Excel_Cell_DataSet(int Row, int Col, AnsiString inText);     // EXCEL ���ް����
extern AnsiString __fastcall Excel_Cell_DataGet(int Row, int Col);                  // EXCEL ���ް��ޯ�
//Add_Str 2019/02/22 Ri 
extern void __fastcall Excel_Cell_SetUnderline(int Row1, int Col1, int Row2, int Col2);     				// EXCEL ���Ɍr��
//Add_End 2019/02/22 Ri
extern void __fastcall Excel_HeaderSet(bool=false);
extern void __fastcall Excel_Cell_Delete(int iLastPage);
extern bool __fastcall Write_Log(AnsiString LogText);             // ۸�̧�ٍ쐬
extern bool __fastcall Write_Error_Log(AnsiString LogText);       // �װ۸�̧�ٍ쐬
extern void __fastcall Excel_Err_DataSet(AnsiString inText);      // EXCEL�p �װ۸��ް����

extern bool __fastcall Read_HEADS_Data(AnsiString filepass);      // HEADS�f�[�^�Ǎ�����

//2004/10/26 add
extern bool __fastcall Read_HEADS_Data_Rev(AnsiString filepass);  // HEADS�f�[�^���r�W�����Ǎ�����
//2004/10/26 add

// 2002.08.03 Add
int __fastcall Syukei_Buhin_Yobisu( AnsiString a_Katasiki, AnsiString a_Zaisitu, HeadsData* pcHeadsData);   // �\���i���ް��W�v����
AnsiString __fastcall Search_HD_MATERIAL(AnsiString Key);            // �ގ����ތ���
AnsiString __fastcall Search_HD_E_MATERIAL(AnsiString Key);            // �ގ����ތ���
AnsiString __fastcall Search_HD_MAT_JtoE(AnsiString Key);
AnsiString __fastcall Chg_Zaisitu_Code(AnsiString Code,AnsiString Typ );
AnsiString __fastcall Search_HD_FRNG_MST(AnsiString Key, AnsiString FSize, AnsiString SEDiv, AnsiString PG, AnsiString TMP, AnsiString Drn, AnsiString Air, bool bRing);
extern AnsiString __fastcall Get_Yobi_Code(AnsiString Key);
// 2016.08.25 �ގ����ޕύX
AnsiString __fastcall Type_MAT(AnsiString Key);            // �ގ���ނ̎擾
// ***********************
// 2020.03.31 �����ܗʒǉ�_S
AnsiString __fastcall Get_Silicagel(AnsiString Key);       // �t�����W�V���J�Q���ʎ擾
// 2020.03.31 �����ܗʒǉ�_E
// 2020.03.31 �����������L�ǉ�_S
AnsiString __fastcall Get_HotAirDrying(void);              // �����������L�擾
// 2020.03.31 �����������L�ǉ�_E
// 2024.02.06 ALLOY�ގ��ϊ�_S
AnsiString __fastcall Alloy_MAT(AnsiString Key);           // ALLOY�ނ���JIS�ނւ̕ϊ�
// 2024.02.06 ALLOY�ގ��ϊ�_E

//----------------------------
// 2007.10.04 �װ�d�l�Ή�
//extern bool __fastcall Get_PHE_Model(AnsiString *s_Syurui, AnsiString *s_Model);
extern bool __fastcall Get_PHE_Model(AnsiString *rs_Syurui, AnsiString *rs_Model, AnsiString s_Syurui, AnsiString s_Model);
//----------------------------

extern int        __fastcall  GetNzKikaku( AnsiString a_KCd );
extern AnsiString __fastcall Get_Sakuban_File(void);
extern AnsiString __fastcall ChgIDFSize(double dSize);

//2022/12/15 �����ǉ��Ή� Add_S
extern AnsiString __fastcall GetSeizouBangou(AnsiString str);
//2022/12/15 �����ǉ��Ή� Add_E

//2023/4/21 �����ǉ��Ή� Add_S
extern AnsiString __fastcall GetFileSeizouBangou(AnsiString str);
//2023/4/21 �����ǉ��Ή� Add_E
#endif

// ---------------- <<< End of File >>> ---------------- //

