#pragma once
#include <memory>

//�}�`�f�[�^
#include "object.h"

//�}�`�̕`��
class Shape {
	//�}�`�f�[�^
	std::shared_ptr<const Object> object;


protected:
	//�`��Ɏg�����_�̐�
	const GLsizei vertexcount;

public:
	//�R���X�g���N�^
		//size:���_�̈ʒu�̎���
		//vertexcount:���_�̐�
		//vertex:���_�������i�[�����z��
		//indexcount:���_�̃C���f�b�N�X�̗v�f��
		//index:���_�̃C���f�b�N�X���i�[�����z��
		Shape(GLint size, GLsizei vertexcount, const Object::Vertex* vertex,GLsizei indexcount=0,const GLuint *index=NULL)
		:object(new Object(size, vertexcount, vertex,indexcount,index))
		, vertexcount(vertexcount) {

	}

	//�`��
		void draw()const {
			//���_�z��I�u�W�F�N�g����������
			object->bind();

			//�`������s����
			execute();
	}

	//�`��̎��s
		virtual void execute() const {
			//�܂���ŕ`�悷��
			glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}

};