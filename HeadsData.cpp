// #include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <system.hpp>
#include "HeadsData.h"

// -----------------------------------------------------------
// �֐���        HeadsData
//
// �����T�v    �FHeadsData�N���X �R���X�g���N�^
//
// ����        �F(�Ȃ�)
//
// �߂�l      �F(�Ȃ�)
//
// ���l          2007.10.01 �V�K�쐬
// -----------------------------------------------------------
HeadsData::HeadsData() {
}

// -----------------------------------------------------------
// �֐���        HeadsData
//
// �����T�v    �FHeadsData�N���X �f�X�g���N�^
//
// ����        �F(�Ȃ�)
//
// �߂�l      �F(�Ȃ�)
//
// ���l          2007.10.01 �V�K�쐬
// -----------------------------------------------------------
HeadsData::~HeadsData() {
}

// -----------------------------------------------------------
// �֐���        ReadAllData
//
// �����T�v    �FHEADS�ϊ��f�[�^��ǂݍ��݁A�f�[�^��z��Ɋi�[����
//
// ����        �F*pchFilename:HEADS�e�L�X�g�f�[�^�t�@�C���p�X
//
// �߂�l      �FTRUE�i����I���j
//               FALSE�i���s�s�j
//
// ���l          2007.10.01 �~���[�d�l�Ή� (�V�K�쐬)
// -----------------------------------------------------------
bool HeadsData::ReadAllData(char *pchFilename) {
    FILE *fp;
    char wkChar[MAX_PATH];
    char *tmpP;
    int i;
    char tmp; // Data[34]��3������
    char tmpSub[2]; // Data[34]��1�`2������

    // �z�񏉊���
    memset(Data, '\0', sizeof(Data));

    // HEADS�ϊ��f�[�^�t�@�C�����J��
    fp = fopen(pchFilename, "r");
    if (fp != NULL) {
        // HEADS�ϊ��f�[�^��ǂݍ���
        for (i = 0; i < DEF_HEADSCONVDATA_ALL; i++) {
            if (!feof(fp)) {
                fgets(wkChar, MAX_PATH, fp);
                tmpP = strtok(wkChar, "\r\n"); // ���s����菜��
                if (tmpP != NULL) {
                    tmpP = strtok(wkChar + 1, "\""); // �ŏ��̃J���}���玟�̃J���}�܂ł̊Ԃ𒊏o����
                    if (tmpP != NULL) {
                        strcpy(Data[i], tmpP);
                    }
                }
            }
        }
        fclose(fp);

        // =========================================
        // �f�[�^�␳����
        // HDPCom.cpp��Read_HEADS_Data�֐����ڐA
        // =========================================
        // -----------------------------------------
        // �R���f���T�Ή��H
        // Data[34]��3�����ڂ�"C"�Ȃ�A1�`2�����ڂ̂ݎ��o��
        // -----------------------------------------
        strncpy(&tmp, Data[34] + 2, 1);
        if (tmp == 'C') {
            strncpy(tmpSub, Data[34], 2);
            memset(Data[34], 0x00, sizeof(Data[34]));
            strncpy(Data[34], tmpSub, 2);
        }

        // -----------------------------------------
        // No:0�`315 ���ް��� No:507�`822�ɺ�߰
        // ���ۂ�HEADS�ް��ɂ͊i�[����Ă��Ȃ��ׁB
        // -----------------------------------------
        for (i = 507; i < 822; i++) {
            //2019.10.31 NK�ʎY�O���L_S
            //if (i != 539) { // �h���F�̘a�p�Ή��̂���539���g�p�B�㏑������ƃ}�Y���̂Ń`�F�b�N�ŊO��   2003/06/09
            if (i == 536) {
                // �m�j�ʎY�O�^�����ӊ��N�o�͂̂���536���g�p�B�㏑������ƃ}�Y���̂Ń`�F�b�N�ŊO��
            } else if (i == 539) {
                // �h���F�̘a�p�Ή��̂���539���g�p�B�㏑������ƃ}�Y���̂Ń`�F�b�N�ŊO��
            } else {
            //2019.10.31 NK�ʎY�O���L_E
                strcpy(Data[i], Data[i - 507]);
            }
        }
    }
    else {
        // �t�@�C���Ǎ����s
        return (false);
    }
    return (true);
}

// -----------------------------------------------------------
// �֐���        GetMirrorfromHeadsConvData
//
// �����T�v    �FHEADS�ϊ��f�[�^����~���[�d�l���擾����
//
// ����        �Forder:�I�[�_No
//
// �߂�l      �FTRUE�i�~���[�d�l�ł���j
//               FALSE�i�~���[�d�l�łȂ��j
//
// ���l          2007.10.01 �~���[�d�l�Ή� (�V�K�쐬)
// -----------------------------------------------------------
bool HeadsData::GetMirrorFlag() {
    char chMirrorVal;
    strncpy(&chMirrorVal, Data[DEF_HEADSCONVDATA_MRR - 1], sizeof(chMirrorVal));

    if (chMirrorVal == '1') {
        return (true);
    }
    else {
        return (false);
    }
}

// -----------------------------------------------------------
// �֐���        GetSpecType
//
// �����T�v    �FHEADS�e�L�X�g�@�d�l�^�C�v�擾
//
// ����        �F(�Ȃ�)
//
// �߂�l      �F�d�l�^�C�v(0:���d�l/1:�~���[A�d�l/2:�~���[B�d�l)
//
// ���l          2007.10.02 �V�K�쐬
// -----------------------------------------------------------
int HeadsData::GetSpecType() {
    return (nSpecType);
}

// -----------------------------------------------------------
// �֐���        SetSpecType
//
// �����T�v    �FHEADS�e�L�X�g�@�d�l�^�C�v�ݒ�
//
// ����        �F(�Ȃ�)
//
// �߂�l      �F�d�l�^�C�v("":���d�l/"A":�~���[A�d�l/"B":�~���[B�d�l)
//
// ���l          2007.10.02 �V�K�쐬
// -----------------------------------------------------------
void HeadsData::SetSpecType(int nSpec) {
    nSpecType = nSpec;
    return;
}

// -----------------------------------------------------------
// �֐���        GetSpecCode
//
// �����T�v    �FHEADS�e�L�X�g�@�d�l�^�C�v�R�[�h�擾
//
// ����        �F(�Ȃ�)
//
// �߂�l      �F�d�l�^�C�v("":���d�l/"A":�~���[A�d�l/"B":�~���[B�d�l)
//
// ���l          2007.10.02 �V�K�쐬
// -----------------------------------------------------------
char* HeadsData::GetSpecCode() {
    return (chSpecCode);
}

// -----------------------------------------------------------
// �֐���        SetSpecCode
//
// �����T�v    �FHEADS�e�L�X�g�@�d�l�^�C�v�R�[�h�ݒ�
//
// ����        �F(�Ȃ�)
//
// �߂�l      �F(�Ȃ�)
//
// ���l          2007.10.02 �V�K�쐬
// -----------------------------------------------------------
void HeadsData::SetSpecCode() {
    switch (nSpecType) {
    case DEF_SPEC_TYPE_NONE:
        strcpy(chSpecCode, "");
        break;
    case DEF_SPEC_TYPE_MRR_A:
        strcpy(chSpecCode, "A");
        break;
    case DEF_SPEC_TYPE_MRR_B:
        strcpy(chSpecCode, "B");
        break;
    default:
        strcpy(chSpecCode, "");
    }

    return;
}

// -----------------------------------------------------------
// �֐���        SetAnsiStringData
//
// �����T�v    �FHEADS�ϊ��f�[�^��ǂݍ��݁A�f�[�^��z��Ɋi�[����
//
// ����        �F*pchFilename:HEADS�e�L�X�g�f�[�^�t�@�C���p�X
//
// �߂�l      �FTRUE�i����I���j
//               FALSE�i���s�s�j
//
// ���l          2007.10.01 �~���[�d�l�Ή� (�V�K�쐬)
// -----------------------------------------------------------
bool HeadsData::SetAnsiStringData(AnsiString asHeadsData[]) {
    int i;
    char *pchData;

    // �z�񏉊���
    for (i = 0; i < DEF_HEADSCONVDATA_ALL; i++) {
        asHeadsData[i] = "";

    }

    // HEADS�ϊ��f�[�^��char�z��AnsiString�z��ɾ��
    for (i = 0; i < DEF_HEADSCONVDATA_ALL; i++) {
        pchData = Data[i];
        asHeadsData[i] = pchData;
    }

    return (true);
}

// -----------------------------------------------------------
// �֐���        GetOneLineData
//
// �����T�v    �FHEADS�ϊ��f�[�^�̎w��v�f�ԍ��̃f�[�^��Ԃ�
//
// ����        �FnIndex:HEADS�e�L�X�g�f�[�^�s�ԍ�(0�`)
//
// �߂�l      �F�w��v�f�ԍ���HEADS�e�L�X�g�f�[�^
//
// ���l          2007.10.04 �~���[�d�l�Ή� (�V�K�쐬)
// -----------------------------------------------------------
void HeadsData::GetOneLineData(int nIndex, char* pchData) {
    strcpy(pchData, Data[nIndex]);
    return;
}
