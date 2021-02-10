#pragma once
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

//�E�B���h�E�֘A�̏���
class Window {
	//�E�B���h�E�̃n���h��
	GLFWwindow* const window;

	//�E�B���h�E�̃T�C�Y
	GLfloat size[2];

	//���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦
	GLfloat scale;

	//�}�`�̐��K���f�o�C�X���W�n��ł̈ʒu
	GLfloat location[2];

	//�L�[�{�[�h�̏��
	int keyStatus;

public:
	//�R���X�g���N�^
	Window(int width = 640, int height = 480, const char* title = "Hello") :
		window(glfwCreateWindow(width, height, title, NULL, NULL)) 
		,scale(100.0f),location{0.0f,0.0f},keyStatus(GLFW_RELEASE)
	{

		if (window == NULL)
		{
			//�E�B���h�E���쐬�ł��Ȃ�����
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		//���݂̃E�B���h�E�������Ώۂɂ���֐�
		glfwMakeContextCurrent(window);

		//GLEW������������
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			//GLEW�̏������Ɏ��s����
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}

		//���������̃^�C�~���O��҂�
		//�J���[�o�b�t�@�̓���ւ��̃^�C�~���O���w�肷��
		glfwSwapInterval(1);

		//�E�B���h�E�T�C�Y�ύX���ɌĂяo�������̓o�^
		glfwSetWindowSizeCallback(window, resize);

		//�}�E�X�z�C�[�����쎞�ɌĂяo�������̓o�^
		glfwSetScrollCallback(window, wheel);

		//�L�[�{�[�h�̑��쎞�ɌĂяo�������̓o�^
		glfwSetKeyCallback(window, keyboard);

		//���̃C���X�^���X��this�|�C���^���L�^���Ă���
		glfwSetWindowUserPointer(window, this);

		//�J�����E�B���h�E�̏����ݒ�
		resize(window, width, height);
	}

	//�f�X�g���N�^
	virtual ~Window() {
		glfwDestroyWindow(window);
	}

	//�`�惋�[�v�̌p������
	explicit operator bool() {
		//�C�x���g�����o��
		if (keyStatus == GLFW_RELEASE) glfwWaitEvents();
		else {
			glfwPollEvents();
		}
		
		

		//�L�[�{�[�h�̏�Ԃ𒲂ׂ�
		if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE) location[0] -= 2.0f / size[0];
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE) location[0] += 2.0f / size[0];
		if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE) location[1] -= 2.0f / size[1];
		else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE) location[1] += 2.0f / size[1];

		//�}�E�X�̍��{�^���̏�Ԃ𒲂ׂ�
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1 != GLFW_RELEASE)) {
			//���{�^����������Ă�����}�E�X�J�[�\���̈ʒu���擾����
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			//�}�E�X�J�[�\���̐��K���f�o�C�X���W�n��ł̈ʒu�����߂�Ahieght�͔��]
			location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
			location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];

			//�E�B���h�E�����K�v���Ȃ����true��Ԃ�
		}
		return !glfwWindowShouldClose(window)&&!glfwGetKey(window,GLFW_KEY_ESCAPE);
	}

	//�_�u���o�b�t�@�����O
	void swapBuffers() const {
		//�J���[�o�b�t�@�����ւ���
		glfwSwapBuffers(window);
	}

	//�E�B���h�E�̃T�C�Y�ύX���̏���
	static void resize(GLFWwindow* const window, int width, int height) {
		//�t���[���o�b�t�@�̃T�C�Y�𒲂ׂ�
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		//�t���[���o�b�t�@�S�̂��r���[�|�[�g�ɐݒ肷��
		glViewport(0, 0, fbWidth, fbHeight);

		//���̃C���X�^���X��this�|�C���^�𓾂�
		Window* const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if (instance != NULL) {
			
			//�J�����E�B���h�E�̃T�C�Y��ۑ�����
			instance->size[0] = static_cast<GLfloat>(width);
			instance->size[1] = static_cast<GLfloat>(height);
		}
	}

	//�}�E�X�z�C�[�����쎞�̏���
	static void wheel(GLFWwindow* window, double x, double y) {
		//���̃C���X�^���X��this�|�C���^�𓾂�
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL) {
			//���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦���X�V����
			instance->scale += static_cast<GLfloat>(y);
		}
	}

	//�L�[�{�[�h���쎞�̏���
	static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
		//���̃C���X�^���X��this�|�C���^�𓾂�
		Window* const instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL) {
			//�L�[�̏�Ԃ�ۑ�����
			instance->keyStatus = action;
		}
	}


	//�E�B���h�E�̃T�C�Y�����o��
	const GLfloat* getSize() const { return size; }

	//���[���h���W�n�ɑ΂���f�o�C�X���W�n�̊g�嗦�����o��
	GLfloat getScale() const { return scale; }

	const GLfloat* getLocation() const { return location; }

};
