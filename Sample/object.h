#pragma once
#include <GL/glew.h>


//図形データ
class Object {
	//頂点配列オブジェクト名
	GLuint vao;
	
	//頂点バッファオブジェクト名
	GLuint vbo;

	//インデックスの頂点バッファオブジェクト
	GLuint ibo;

public:
	//頂点属性
	struct Vertex {
		//位置
		GLfloat position[3];

		//色
		GLfloat color[3];
	};



	//コンストラクタ
	//size:頂点位置の次元
	//vertexcount:頂点の数
	//vertex:頂点属性を格納した配列
	//indexcount:頂点のインデックスの要素数
	//index:頂点のインデックスを格納した配列
	Object(GLint size,GLsizei vertexcount, const Vertex* vertex,GLsizei indexcount=0,const GLuint *index=NULL) {
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
		glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex*>(0)->position);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex*>(0)->color);
		//Attribute変数を有効にする
		glEnableVertexAttribArray(1);


		//インデックスの頂点バッファオブジェクト
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
	}

	//デストラクタ
	virtual ~Object() {
		//頂点配列オブジェクトを削除する
		glDeleteVertexArrays(1, &vao);

		//頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &vbo);

		//インデックスの頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &ibo);
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