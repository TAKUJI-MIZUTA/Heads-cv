// ***************************************************************
// ���W���[����  �FHD_Data.h
// �쐬��        �F���_
// �쐬��        �F2007-10-02
// �ŏI�X�V��    �F2007-10-02
// �T�v          �FHeadsData �N���X��`
// ***************************************************************

#define DEF_HEADSCONVDATA_ALL 1500      // HEADS�e�L�X�g�t�@�C���s����
#define MAX_PATH               260      // 1�s������̍ő�o�C�g��
#define DEF_HEADSCONVDATA_REV  320      // HEADS�ϊ��f�[�^��REVNO�f�[�^�ʒu
#define DEF_HEADSCONVDATA_MRR  314      // HEADS�ϊ��f�[�^�̃~���[�d�l�f�[�^�ʒu

#define DEF_SPEC_TYPE_NONE  0           // HEADS�e�L�X�g�f�[�^ ���d�l
#define DEF_SPEC_TYPE_MRR_A 1           // HEADS�e�L�X�g�f�[�^ �~���[A�d�l
#define DEF_SPEC_TYPE_MRR_B 2           // HEADS�e�L�X�g�f�[�^ �~���[B�d�l

class HeadsData {
private:

public:
    HeadsData(); // �R���X�g���N�^
    ~HeadsData(); // �f�X�g���N�^
    bool ReadAllData(char* pchFilename); // HEADS�e�L�X�g�f�[�^�Ǎ�
    bool GetMirrorFlag(); // �~���[�d�l���ǂ�������
    char* GetSpecCode(); // �d�l�^�C�v�R�[�h���擾����
    void SetSpecCode(); // �d�l�^�C�v�R�[�h��ݒ肷��
    int GetSpecType(); // �d�l�^�C�v���擾����
    void SetSpecType(int nSpecType); // �d�l�^�C�v��ݒ肷��
    bool SetAnsiStringData(AnsiString asHeadsData[]);
    // HEADS÷���ް���AnsiString�z��Ɋi�[
    void GetOneLineData(int nIndex, char* pchData); // �w��v�f�ԍ���HEADS÷���ް����擾

private:
    char Data[DEF_HEADSCONVDATA_ALL][MAX_PATH]; // HEADS�e�L�X�g�f�[�^
    int nSpecType; // �d�l�^�C�v�@�@�@(0:���d�l/1:�~���[A�d�l/2:�~���[B�d�l)
    char chSpecCode[2]; // �d�l�^�C�v�R�[�h(""/"A"/"B")
};
