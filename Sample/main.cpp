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

//シェーダーオブジェクトのコンパイル結果を表示する
//shader:シェーダーオブジェクト名
//str:コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char* str) {
	//コンパイル結果を取得する
	GLint status;
	//シェーダオブジェクトの情報を取り出す
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)std::cerr << "Compile Error in " << str << std::endl;

	//シェーダーコンパイル時のログの長さを取得する
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
		//シェーダオブジェクトに対してソースプログラムを読み込む
		glShaderSource(vobj, 1, &vsrc, NULL);
		//シェーダオブジェクトに読み込まれたソースファイルをコンパイルする
		glCompileShader(vobj);

		//成功していればバーテックスシェーダーのシェーダーオブジェクトをプログラムオブジェクトに組み込む
		if (printShaderInfoLog(vobj, "vertex shader"))
			//プログラムオブジェクトにシェーダオブジェクトを組み込む
			glAttachShader(program, vobj);
		//削除マークをつける
		glDeleteShader(vobj);
	}

	if (fsrc != NULL) {
		//フラグメントシェーダーのシェーダーオブジェクトを作成する
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		glShaderSource(fobj, 1, &fsrc, NULL);
		glCompileShader(fobj);

		//成功していればフラグメントシェーダーのシェーダーオブジェクトをプログラムオブジェクトに組み込む
		if(printShaderInfoLog(fobj,"fragment shader"))
			glAttachShader(program, fobj);
		glDeleteShader(fobj);
	}

	//プログラムオブジェクトをリンクする
	//Attribute変数の場所を指定している(in変数とout変数)
	glBindAttribLocation(program, 0, "position");
	glBindFragDataLocation(program,0,"fragment");
	//programに指定したプログラムオブジェクトをリンクしている
	glLinkProgram(program);

	//成功していれば作成したプログラムオブジェクトを返す
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

	//ファイルを先頭から末尾まで読み込む
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
	//上手く読み込まれていたらvstatとfstatはtrueになる
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar>fsrc;
	const bool fstat(readShaderSource(frag, fsrc));
	
	//プログラムオブジェクトを作成する
	//先頭へのポインタで渡す
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

//短形の頂点の位置
constexpr Object::Vertex rectangleVertex[] = {
	{-0.5f,-0.5f},
	{ 0.5f,-0.5f},
	{ 0.5f, 0.5f},
	{-0.5f, 0.5f}
};


int main() {
	//GLFWを初期化する
	if(glfwInit() == GL_FALSE) {
		//初期化に失敗した
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 1;
	}

	//atexit：プログラム終了時の処理を登録する
	//glfwTerminate：GLFWで作成した全てのウィンドウを閉じ確保したリソースの全てを開放する
	atexit(glfwTerminate);

	//OpenGL Version 3.2 Core Profileを選択する
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//古い機能を削除したりするプロファイルを使う
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//OpenGL3.0以前の機能を含まない
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//ウィンドウを作成する
	Window window;

	//背景色を指定する
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);


	//プログラムオブジェクトを作成するcreate
	const GLuint program(loadProgram("point.vert", "point.frag"));

	//uniform変数の場所を取得する
	const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));
	const GLint modelLoc(glGetUniformLocation(program, "model"));

	//図形データを作成する
	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));



	//ウィンドウが開いている間繰り返す
	while (window) {

		//ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		//ここで描画処理を行う

		//シェーダープログラムの使用開始
		glUseProgram(program);

		//拡大縮小の変換行列を求める
		const GLfloat* const size(window.getSize());
		const GLfloat scale(window.getScale() * 2.0f);
		const Matrix scaling(Matrix::scale(scale/size[0], scale / size[1], 1.0f));

		//平行移動の変換行列を求める
		const GLfloat* const position(window.getLocation());
		const Matrix translation(Matrix::translate(position[0], position[1], 0.0f));

		//モデル変換行列を求める
		const Matrix model(translation * scaling);

		//ビュー変換行列を求める
		const Matrix view(Matrix::lookat(0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f));

		//モデルビュー変換行列を求める
		const Matrix modelview(view * model);

		//uniform変数に値を設定する
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());

		//図形を描画する
		shape->draw();

		//カラーバッファを入れ替える
		window.swapBuffers();
	}
}