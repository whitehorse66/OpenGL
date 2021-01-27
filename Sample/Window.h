#pragma once
#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

//ウィンドウ関連の処理
class Window {
	//ウィンドウのハンドル
	GLFWwindow* const window;

	//縦横比
	GLfloat aspect;


public:
	//コンストラクタ
	Window(int width = 640, int height = 480, const char* title = "Hello") :
		window(glfwCreateWindow(width, height, title, NULL, NULL)) 
	{
		if (window == NULL)
		{
			//ウィンドウが作成できなかった
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		//現在のウィンドウを処理対象にする関数
		glfwMakeContextCurrent(window);

		//GLEWを初期化する
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			//GLEWの初期化に失敗した
			std::cerr << "Can't initialize GLEW" << std::endl;
			exit(1);
		}

		//垂直同期のタイミングを待つ
		//カラーバッファの入れ替えのタイミングを指定する
		glfwSwapInterval(1);

		//このインスタンスのthisポインタを記録しておく
		glfwSetWindowUserPointer(window, this);

		//ウィンドウサイズ変更時に呼び出す処理の登録
		glfwSetWindowSizeCallback(window, resize);

		//開いたウィンドウの初期設定
		resize(window, width, height);
	}

	//デストラクタ
	virtual ~Window() {
		glfwDestroyWindow(window);
	}

	//描画ループの継続判定
	explicit operator bool() {
		//イベントを取り出す
		glfwWaitEvents();

		//ウィンドウを閉じる必要がなければtrueを返す
		return !glfwWindowShouldClose(window);
	}

	//ダブルバッファリング
	void swapBuffers() const {
		//カラーバッファを入れ替える
		glfwSwapBuffers(window);
	}

	//ウィンドウのサイズ変更時の処理
	static void resize(GLFWwindow* const window, int width, int height) {
		//フレームバッファのサイズを調べる
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		//フレームバッファ全体をビューポートに設定する
		glViewport(0, 0, fbWidth, fbHeight);

		//このインスタンスのthisポインタを得る
		Window* const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));
		if (instance != NULL) {
			//このインスタンスが保持する縦横比を更新する
			instance->aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		}
	}
	//縦横比を取り出す
	GLfloat getAspect() const { return aspect; }
};
