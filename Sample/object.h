#pragma once
#include <GL/glew.h>


//�}�`�f�[�^
class Object {
	//���_�z��I�u�W�F�N�g��
	GLuint vao;
	
	//���_�o�b�t�@�I�u�W�F�N�g��
	GLuint vbo;

	//�C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
	GLuint ibo;

public:
	//���_����
	struct Vertex {
		//�ʒu
		GLfloat position[3];

		//�F
		GLfloat color[3];
	};



	//�R���X�g���N�^
	//size:���_�ʒu�̎���
	//vertexcount:���_�̐�
	//vertex:���_�������i�[�����z��
	//indexcount:���_�̃C���f�b�N�X�̗v�f��
	//index:���_�̃C���f�b�N�X���i�[�����z��
	Object(GLint size,GLsizei vertexcount, const Vertex* vertex,GLsizei indexcount=0,const GLuint *index=NULL) {
		//���_�z��I�u�W�F�N�g���쐬
		glGenVertexArrays(1, &vao);
		//���_�z��I�u�W�F�N�g������
		glBindVertexArray(vao);

		//���_�o�b�t�@�I�u�W�F�N�g���쐬
		glGenBuffers(1, &vbo);
		//���_�o�b�t�@�I�u�W�F�N�g������
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//���_�o�b�t�@�I�u�W�F�N�g�Ƀf�[�^ (���_����) ��]������
		glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

		//��������Ă��钸�_�o�b�t�@�I�u�W�F�N�g��in�ϐ�����Q�Ƃł���悤�ɂ���
		//���_�o�b�t�@�I�u�W�F�N�g��attribute�ϐ��Ɋ֘A�Â���
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex*>(0)->position);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex*>(0)->color);
		//Attribute�ϐ���L���ɂ���
		glEnableVertexAttribArray(1);


		//�C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	//�f�X�g���N�^
	virtual ~Object() {
		//���_�z��I�u�W�F�N�g���폜����
		glDeleteVertexArrays(1, &vao);

		//���_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &vbo);

		//�C���f�b�N�X�̒��_�o�b�t�@�I�u�W�F�N�g���폜����
		glDeleteBuffers(1, &ibo);
	}
private:
	//�R�s�[�R���X�g���N�^�ɂ��R�s�[�֎~
	Object(const Object& o);

	//����ɂ��R�s�[�֎~
	Object& operator=(const Object& o);

public:
	//���_�z��I�u�W�F�N�g�̌���
	void bind()const {
		//�`�悷�钸�_�z��I�u�W�F�N�g���w�肷��
		glBindVertexArray(vao);
	}
};