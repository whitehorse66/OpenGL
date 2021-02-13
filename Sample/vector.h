#pragma once
#include <array>

//�ϊ��s��
#include "Matrix.h"

//�x�N�g��
using Vector = std::array<GLfloat, 4>;


//�s��ƃx�N�g���̏�Z(�I�[�o�[���C�h)
//m�FMatrix�^�̍s��
//v�FVector�^�̍s��
Vector operator* (const Matrix& m, const Vector& v) {
	Vector t;
	for (int i = 0; i < 4; ++i) {
		t[i] = m[i] * v[0] + m[i + 4] * v[1] + m[i + 8] * v[2] + m[i + 12] * v[3];
	}
	return t;
}
