#pragma once
#include <memory>
#include <GL/glew.h>

//���j�t�H�[���o�b�t�@�I�u�W�F�N�g
template<typename T>
class Uniform {
	struct UniformBuffer {
		//���j�t�H�[���o�b�t�@�I�u�W�F�N�g��
		GLuint ubo;

		//���j�t�H�[���u���b�N�̃T�C�Y
		GLsizeiptr blocksize;

		//�R���X�g���N�^
		//data:uniform�u���b�N�Ɋi�[����f�[�^
		//count�F�m�ۂ���uniform�u���b�N�̐�
		UniformBuffer(const T* data,unsigned int count) {
			//���j�t�H�[���u���b�N�̃T�C�Y�����߂�
			GLint alignment;
			glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
			blocksize = (((sizeof(T) - 1) / alignment) + 1) * alignment;
			//���j�t�H�[���o�b�t�@�I�u�W�F�N�g���쐬����
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, count*blocksize, NULL, GL_STATIC_DRAW);
			for (unsigned int i = 0; i < count; ++i) {
				glBufferSubData(GL_UNIFORM_BUFFER, i * blocksize, sizeof(T), data + i);
			}
		}

		//�f�X�g���N�^
		~UniformBuffer() {
			//���j�t�H�[���o�b�t�@�I�u�W�F�N�g���폜����
			glDeleteBuffers(1, &ubo);
		}

	};

	const std::shared_ptr<const UniformBuffer> buffer;

public:
	//�R���X�g���N�^
	//data�Funiform�u���b�N�Ɋi�[����f�[�^
	//count�F�m�ۂ���uniform�u���b�N�̐�
	Uniform(const T* data = NULL,unsigned int count=1) :buffer(new UniformBuffer(data,count)){
	}

	//�f�X�g���N�^
	virtual ~Uniform(){}

	//���j�t�H�[���o�b�t�@�I�u�W�F�N�g�Ƀf�[�^���i�[����
	//data�Funiform�u���b�N�Ɋi�[����f�[�^
	void set(const T* data,unsigned int start=0,unsigned int count=1) const {
		glBindBuffer(GL_UNIFORM_BUFFER, buffer->ubo);
		for (unsigned int i = 0; i < count; ++i) {
			//�f�[�^��]��
			glBufferSubData(GL_UNIFORM_BUFFER, (start+i,*buffer->blocksize, sizeof(T), data+i));
		}

	}

	//���̃��j�t�H�[���o�b�t�@�I�u�W�F�N�g���g�p����
	//bp�F�����|�C���g
	//i�F��������uniform�u���b�N�̈ʒu
	void select(GLuint bp,unsigned int i=0) const {
		//�ގ��ɐݒ肷�郆�j�t�H�[���o�b�t�@�I�u�W�F�N�g���w�肷��
		glBindBufferRange(GL_UNIFORM_BUFFER, bp, buffer->ubo,i*buffer->blocksize,sizeof(T));
	}
};