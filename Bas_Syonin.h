//---------------------------------------------------------------------------
// ���㐻�쏊�l�����@�o�}�v����M�T�[�r�X�V�X�e��
//   ̧�يT�v�F���F�}�\���쐬���W���[��  �w�b�_�t�@�C��
//�@ �� �� �ҁFN.Uchida
//�@ �� �� ���F2002.05.30
//�@ �X �V ���F2002.09.05
//---------------------------------------------------------------------------

#ifndef Bas_SyoninH
#define Bas_SyoninH
//---------------------------------------------------------------------------
#endif
class HeadsData;

extern bool __fastcall Syonin(HeadsData* pcHeadsData);       // ���F�}�\��

extern AnsiString __fastcall JapDayToEngDay(AnsiString);     // ���t�ϊ�����
extern AnsiString __fastcall GetMeibanZuban(void);
extern void __fastcall SyuturyokuZumen_Set(char Pattern,AnsiString Lang, HeadsData* pcHeadsData);

