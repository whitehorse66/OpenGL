#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Shape.h"

//シェーダーオブジェクトのコンパイル結果を表示する
//shader:シェーダーオブジェクト名
//str:コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char* str) {
	//コンパイル結果を取得する
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)std::cerr << "Compile Error in " << str << std::endl;

	//シェーダーコンパイル時のログの長さを首都高する
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		//シェーダーコンパイル時のログ内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

//プログラムオブジェクトのリンク結果を表示する
//program:プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program) {
	//リンク結果を取得する
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)std::cerr << "LinkError." << std::endl;

	//シェーダーリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if (bufSize > 1) {
		//シェーダーリンク時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}


//プログラムオブジェクトを作成する
//vsrc:バーテックスシェーダーのソースプログラムの文字列
//fsrc:フラグメントシェーダーのソースプログラムの文字列
GLuint createProgram(const char* vsrc, const char* fsrc) {
	//空のオブジェクトを作成する
	const GLuint program(glCreateProgram());

	if (vsrc != NULL) {
		//バーテックスシェーダーのシェーダーオブジェクトを作成する
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		//バーテックスシェーダーのシェーダーオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}

	if (fsrc != NULL) {
		//フラグメントシェーダーのシェーダーオブジェクトを作成する
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		//フラグメントシェーダーのシェーダーオブジェクトをプログラムオブジェクトに組み込む
		if(printShaderInfoLog(fobj,"fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	//プログラムオブジェクトをリンクする
	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program,0,"fragment");
	glLinkProgram(program);

	//作成したプログラムオブジェクトを返す
	if(printProgramInfoLog(program))
		return program;

	//プログラムオブジェクトが作成できなければ0を返す
	glDeleteProgram(program);
	return 0;
}


//シェーダーのソースファイルを読み込んだメモリを返す
//name:シェーダーのソースファイル名
//buffer:読み込んだソースファイルのテキスト
bool readShaderSource(const char* name, std::vector<GLchar>& buffer) {
	//ファイル名がNULLだった
	if (name == NULL)return false;

	//ソースファイルを開く
	std::ifstream file(name, std::ios::binary);
	if (file.fail()) {
		//開けなかった
		std::cerr << "Error: Can't open source file: " << name << std::endl;
		return false;
	}

	//ファイルの末尾に移動し現在位置(=ファイルサイズ)を得る
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	//ファイルサイズのメモリを確保
	buffer.resize(length + 1);

	//ファイルを先頭から読み込む
	file.seekg(0L,std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';
	
	if (file.fail()) {
		//うまく読み込めなかった
		std::cerr << "Error: Could not read souce file:" << name << std::endl;
		file.close();
		return false;
	}

	//読み込み成功
	file.close();
	return true;
}

//シェーダーのソースファイルを読み込んでプログラムオブジェクトを作成する
//vert:バーテックスシェーダーのソースファイル名
//frag:フラグメントシェーダーのソースファイル名
GLuint loadProgram(const char* vert, const char* frag) {
	//シェーダーのソースファイルを読み込む
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar>fsrc;
	const bool fstat(readShaderSource(frag, fsrc));
	
	//プログラムオブジェクトを作成する
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

//短形の頂点の位置
constexpr Object::Vertex rectangleVertex[] = {
	{-0.5f,-0.5f},
	{ 1.5f,-0.5f},
	{ 1.5f, 1.5f},
	{-0.5f, 1.5f}
};


int main() {
	//GLFWを初期化する
	if(glfwInit() == GL_FALSE) {
		//初期化に失敗した
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	//プログラム終了時の処理を登録する
	atexit(glfwTerminate);

	//OpenGL Version 3.2 Core Profileを選択する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//ウィンドウを作成する
	Window window;

	//背景色を指定する
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//ビューポートを設定する
	glViewport(100, 50, 300, 300);

	//プログラムオブジェクトを作成するcreate
	
	const GLuint program(loadProgram("point.vert", "point.frag"));

	//図形データを作成する
	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));



	//ウィンドウが開いている間繰り返す
	while (window) {

		//ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		//ここで描画処理を行う

		//シェーダープログラムの使用開始
		glUseProgram(program);

		//図形を描画する
		shape->draw();

		//カラーバッファを入れ替える
		window.swapBuffers();
	}
}