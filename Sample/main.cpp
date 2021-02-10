#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"

//�V�F�[�_�[�I�u�W�F�N�g�̃R���p�C�����ʂ�\������
//shader:�V�F�[�_�[�I�u�W�F�N�g��
//str:�R���p�C���G���[�����������ꏊ������������
GLboolean printShaderInfoLog(GLuint shader, const char* str) {
	//�R���p�C�����ʂ��擾����
	GLint status;
	//�V�F�[�_�I�u�W�F�N�g�̏������o��
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)std::cerr << "Compile Error in " << str << std::endl;

	//�V�F�[�_�[�R���p�C�����̃��O�̒������擾����
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		//�V�F�[�_�[�R���p�C�����̃��O���e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

//�v���O�����I�u�W�F�N�g�̃����N���ʂ�\������
//program:�v���O�����I�u�W�F�N�g��
GLboolean printProgramInfoLog(GLuint program) {
	//�����N���ʂ��擾����
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)std::cerr << "LinkError." << std::endl;

	//�V�F�[�_�[�����N���̃��O�̒������擾����
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		//�V�F�[�_�[�����N���̃��O�̓��e���擾����
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}


//�v���O�����I�u�W�F�N�g���쐬����
//vsrc:�o�[�e�b�N�X�V�F�[�_�[�̃\�[�X�v���O�����̕�����
//fsrc:�t���O�����g�V�F�[�_�[�̃\�[�X�v���O�����̕�����
GLuint createProgram(const char* vsrc, const char* fsrc) {
	//��̃I�u�W�F�N�g���쐬����
	const GLuint program(glCreateProgram());

	if (vsrc != NULL) {
		//�o�[�e�b�N�X�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g���쐬����
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		//�V�F�[�_�I�u�W�F�N�g�ɑ΂��ă\�[�X�v���O������ǂݍ���
		glShaderSource(vobj, 1, &vsrc, NULL);
		//�V�F�[�_�I�u�W�F�N�g�ɓǂݍ��܂ꂽ�\�[�X�t�@�C�����R���p�C������
		glCompileShader(vobj);

		//�������Ă���΃o�[�e�b�N�X�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if (printShaderInfoLog(vobj, "vertex shader"))
			//�v���O�����I�u�W�F�N�g�ɃV�F�[�_�I�u�W�F�N�g��g�ݍ���
			glAttachShader(program, vobj);
		//�폜�}�[�N������
		glDeleteShader(vobj);
	}

	if (fsrc != NULL) {
		//�t���O�����g�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g���쐬����
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		//�������Ă���΃t���O�����g�V�F�[�_�[�̃V�F�[�_�[�I�u�W�F�N�g���v���O�����I�u�W�F�N�g�ɑg�ݍ���
		if(printShaderInfoLog(fobj,"fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	//�v���O�����I�u�W�F�N�g�������N����
	//Attribute�ϐ��̏ꏊ���w�肵�Ă���(in�ϐ���out�ϐ�)
	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program,0,"fragment");
	//program�Ɏw�肵���v���O�����I�u�W�F�N�g�������N���Ă���
	glLinkProgram(program);

	//�������Ă���΍쐬�����v���O�����I�u�W�F�N�g��Ԃ�
	if(printProgramInfoLog(program))
		return program;

	//�v���O�����I�u�W�F�N�g���쐬�ł��Ȃ����0��Ԃ�
	glDeleteProgram(program);
	return 0;
}


//�V�F�[�_�[�̃\�[�X�t�@�C����ǂݍ��񂾃�������Ԃ�
//name:�V�F�[�_�[�̃\�[�X�t�@�C����
//buffer:�ǂݍ��񂾃\�[�X�t�@�C���̃e�L�X�g
bool readShaderSource(const char* name, std::vector<GLchar>& buffer) {
	//�t�@�C������NULL������
	if (name == NULL)return false;

	//�\�[�X�t�@�C�����J��
	std::ifstream file(name, std::ios::binary);
	if (file.fail()) {
		//�J���Ȃ�����
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}

	//�t�@�C���̖����Ɉړ������݈ʒu(=�t�@�C���T�C�Y)�𓾂�
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	//�t�@�C���T�C�Y�̃��������m��
	buffer.resize(length + 1);

	//�t�@�C����擪���疖���܂œǂݍ���
	file.seekg(0L,std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';
	
	if (file.fail()) {
		//���܂��ǂݍ��߂Ȃ�����
		std::cerr << "Error: Could not read souce file:" << name << std::endl;
		file.close();
		return false;
	}

	//�ǂݍ��ݐ���
	file.close();
	return true;
}

//�V�F�[�_�[�̃\�[�X�t�@�C����ǂݍ���Ńv���O�����I�u�W�F�N�g���쐬����
//vert:�o�[�e�b�N�X�V�F�[�_�[�̃\�[�X�t�@�C����
//frag:�t���O�����g�V�F�[�_�[�̃\�[�X�t�@�C����
GLuint loadProgram(const char* vert, const char* frag) {
	//�V�F�[�_�[�̃\�[�X�t�@�C����ǂݍ���
	//��肭�ǂݍ��܂�Ă�����vstat��fstat��true�ɂȂ�
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar>fsrc;
	const bool fstat(readShaderSource(frag, fsrc));
	
	//�v���O�����I�u�W�F�N�g���쐬����
	//�擪�ւ̃|�C���^�œn��
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

//�Z�`�̒��_�̈ʒu
constexpr Object::Vertex rectangleVertex[] = {
	{-0.5f,-0.5f},
	{ 0.5f,-0.5f},
	{ 0.5f, 0.5f},
	{-0.5f, 0.5f}
};


int main() {
	//GLFW������������
	if(glfwInit() == GL_FALSE) {
		//�������Ɏ��s����
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	//atexit�F�v���O�����I�����̏�����o�^����
	//glfwTerminate�FGLFW�ō쐬�����S�ẴE�B���h�E����m�ۂ������\�[�X�̑S�Ă��J������
	atexit(glfwTerminate);

	//OpenGL Version 3.2 Core Profile��I������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//�Â��@�\���폜�����肷��v���t�@�C�����g��
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//OpenGL3.0�ȑO�̋@�\���܂܂Ȃ�
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//�E�B���h�E���쐬����
	Window window;

	//�w�i�F���w�肷��
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);


	//�v���O�����I�u�W�F�N�g���쐬����create
	const GLuint program(loadProgram("point.vert", "point.frag"));

	//uniform�ϐ��̏ꏊ���擾����
	const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));
	const GLint modelLoc(glGetUniformLocation(program, "model"));

	//�}�`�f�[�^���쐬����
	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));



	//�E�B���h�E���J���Ă���ԌJ��Ԃ�
	while (window) {

		//�E�B���h�E����������
		glClear(GL_COLOR_BUFFER_BIT);

		//�����ŕ`�揈�����s��

		//�V�F�[�_�[�v���O�����̎g�p�J�n
		glUseProgram(program);

		//�g��k���̕ϊ��s������߂�
		const GLfloat* const size(window.getSize());
		const GLfloat scale(window.getScale() * 2.0f);
		const Matrix scaling(Matrix::scale(scale/size[0], scale / size[1], 1.0f));

		//���s�ړ��̕ϊ��s������߂�
		const GLfloat* const position(window.getLocation());
		const Matrix translation(Matrix::translate(position[0], position[1], 0.0f));

		//���f���ϊ��s������߂�
		const Matrix model(translation * scaling);

		//�r���[�ϊ��s������߂�
		const Matrix view(Matrix::lookat(0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f));

		//���f���r���[�ϊ��s������߂�
		const Matrix modelview(view * model);

		//uniform�ϐ��ɒl��ݒ肷��
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());

		//�}�`��`�悷��
		shape->draw();

		//�J���[�o�b�t�@�����ւ���
		window.swapBuffers();
	}
}