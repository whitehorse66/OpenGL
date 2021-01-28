#pragma once
#include <GL/glew.h>


//図形データ
class Object {
	//頂点配列オブジェクト名
	GLuint vao;
	
	//頂点バッファオブジェクト名
	GLuint vbo;

public:
	//頂点属性
	struct Vertex {
		//位置
		GLfloat position[2];
	};



	//コンストラクタ
	//size:頂点位置の次元
	//vertexcount:頂点の数
	//vertex:頂点属性を格納した配列
	Object(GLint size,GLsizei vertexcount, const Vertex* vertex) {
		//頂点配列オブジェクトを作成
		glGenVertexArrays(1, &vao);
		//頂点配列オブジェクトを結合
		glBindVertexArray(vao);

		//頂点バッファオブジェクトを作成
		glGenBuffers(1, &vbo);
		//頂点バッファオブジェクトを結合
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//頂点バッファオブジェクトにデータ (頂点属性) を転送する
		glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

		//結合されている頂点バッファオブジェクトをin変数から参照できるようにする
		//頂点バッファオブジェクトをattribute変数に関連づける
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
		//Attribute変数を有効にする
		glEnableVertexAttribArray(0);
	}

	//デストラクタ
	virtual ~Object() {
		//頂点配列オブジェクトを削除する
		glDeleteVertexArrays(1, &vao);

		//頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &vbo);
	}
private:
	//コピーコンストラクタによるコピー禁止
	Object(const Object& o);

	//代入によるコピー禁止
	Object& operator=(const Object& o);

public:
	//頂点配列オブジェクトの結合
	void bind()const {
		//描画する頂点配列オブジェクトを指定する
		glBindVertexArray(vao);
	}
};