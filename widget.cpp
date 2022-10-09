#include "widget.h"

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent), m_texture(0), m_index_buffer(QOpenGLBuffer::IndexBuffer)
{
}

Widget::~Widget()
{

}

void Widget::initializeGL()
{
    // Задаем черный фон RGB и Alpha канал
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
    // Включаем буффер глубины
    glEnable(GL_DEPTH_TEST);
    // Отсечение задних граней
    glEnable(GL_CULL_FACE);
    // Инициализируем шейдеры
    initShaders();
    // Инициализируем фигуру
    initCube(1.0f);
}

void Widget::paintGL()
{
    // Чистим буфер цвета и бувер глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 model_view_matrix;
    model_view_matrix.setToIdentity();
    model_view_matrix.translate(0.0f, 0.0f, -10.0f);
    model_view_matrix.rotate(30, 1.0, 0.0, 0.0);
    model_view_matrix.rotate(30, 0.0, 1.0, 0.0);

    m_texture->bind(0);

    m_program.bind();
    m_program.setUniformValue("qt_ModelViewProjectionMatrix", m_projection_matrix * model_view_matrix);
    m_program.setUniformValue("qt_Texture0", 0);

    m_arr_buffer.bind();

    int offset = 0;
    int vrt_loc = m_program.attributeLocation("qt_Vertex");
    m_program.enableAttributeArray(vrt_loc);
    m_program.setAttributeBuffer(vrt_loc, GL_FLOAT, offset, 3, sizeof(Vertexd));

    offset += sizeof(QVector3D);

    int tex_loc = m_program.attributeLocation("qt_MultiTexCoord0");
    m_program.enableAttributeArray(tex_loc);
    m_program.setAttributeBuffer(tex_loc, GL_FLOAT, offset, 2, sizeof(Vertexd));

    m_index_buffer.bind();

    glDrawElements(GL_TRIANGLES, m_index_buffer.size(), GL_UNSIGNED_INT, 0);
}

void Widget::resizeGL(int w, int h)
{
    float aspect = (float)w / (float)h;

    // Делаем матрицу единичной
    m_projection_matrix.setToIdentity();
    // Инициализируем ее как марицу перспективы
    // Угол обзора камерый, аспект поворота,
    // передняя пл. отсечения, дальная пл. отс.
    m_projection_matrix.perspective(45, aspect, 0.1f, -7.0f);
}

void Widget::initShaders()
{
    // Если шейдеры из ресурс файлов не открылись, закрываем программу
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh")){
        qDebug("FatalError! Couldn't open the vshader");
        close();
    }
    if(!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh")){
        qDebug("FatalError! Couldn't open the fshader");
        close();
    }
    if(!m_program.link()){
        qDebug("FatalError! The program could not be linked");
        close();
    }

}
/*     6_______4
      /.      /|
    0/______2/ |
    |  .    |  |
    |  7 .  |. 5
    | .     | /
    1_______3/
*/

void Widget::initCube(float wight)
{
    // Структура данных вершин
    QVector <Vertexd> vertexes;
    // Координаты вершин
    vec_vert_sign << QVector3D(-1,1,1);  // Вершина 0
    vec_vert_sign << QVector3D(-1,-1,1); //
    vec_vert_sign << QVector3D(1,1,1);   // .
    vec_vert_sign << QVector3D(1,-1,1);  // .
    vec_vert_sign << QVector3D(1,1,-1);  // .
    vec_vert_sign << QVector3D(1,-1,-1); //
    vec_vert_sign << QVector3D(-1,1,-1); //
    vec_vert_sign << QVector3D(-1,-1,-1);// Вершина 7
    // Направление нормали грани
    vec_front_face << QVector3D(0.0f, 0.0f, 1.0f);   // По оси Z
    vec_front_face << QVector3D(1.0f, 0.0f, 0.0f);   // По оси X
    vec_front_face << QVector3D(0.0f, 0.0f, -1.0f);  // По оси -Z
    vec_front_face << QVector3D(-1.0f, 0.0f, 0.0f);  // По оси -X
    vec_front_face << QVector3D(0.0f, 1.0f, 0.0f);   // По оси Y
    vec_front_face << QVector3D(0.0f, -1.0f, 0.0f);  // По оси -Y
    // Поворот текстуры
    vec_rot_txtr << QVector2D(0.0f, 0.0f);
    vec_rot_txtr << QVector2D(0.0f, 1.0f);
    vec_rot_txtr << QVector2D(1.0f, 0.0f);
    vec_rot_txtr << QVector2D(1.0f, 1.0f);

    vertexes.append(initCubeVertex(wight, 0, 0, 0));
    vertexes.append(initCubeVertex(wight, 1, 1, 0));
    vertexes.append(initCubeVertex(wight, 2, 2, 0));
    vertexes.append(initCubeVertex(wight, 3, 3, 0));

    vertexes.append(initCubeVertex(wight, 2, 0, 1));
    vertexes.append(initCubeVertex(wight, 3, 1, 1));
    vertexes.append(initCubeVertex(wight, 4, 2, 1));
    vertexes.append(initCubeVertex(wight, 5, 3, 1));

    vertexes.append(initCubeVertex(wight, 4, 0, 2));
    vertexes.append(initCubeVertex(wight, 5, 1, 2));
    vertexes.append(initCubeVertex(wight, 6, 2, 2));
    vertexes.append(initCubeVertex(wight, 7, 3, 2));

    vertexes.append(initCubeVertex(wight, 6, 0, 3));
    vertexes.append(initCubeVertex(wight, 7, 1, 3));
    vertexes.append(initCubeVertex(wight, 0, 2, 3));
    vertexes.append(initCubeVertex(wight, 1, 3, 3));

    vertexes.append(initCubeVertex(wight, 2, 0, 4));
    vertexes.append(initCubeVertex(wight, 4, 1, 4));
    vertexes.append(initCubeVertex(wight, 0, 2, 4));
    vertexes.append(initCubeVertex(wight, 6, 3, 4));

    vertexes.append(initCubeVertex(wight, 3, 0, 5));
    vertexes.append(initCubeVertex(wight, 5, 1, 5));
    vertexes.append(initCubeVertex(wight, 1, 2, 5));
    vertexes.append(initCubeVertex(wight, 7, 3, 5));

    QVector <GLuint> indexes;

    for (int i = 0; i < 24; i += 4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
    }

    m_arr_buffer.create();
    m_arr_buffer.bind();
    m_arr_buffer.allocate(vertexes.constData(), vertexes.size() * sizeof(Vertexd));
    m_arr_buffer.release();

    m_index_buffer.create();
    m_index_buffer.bind();
    m_index_buffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_index_buffer.release();

    m_texture = new QOpenGLTexture(QImage(":/1.jpg").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

Vertexd Widget::initCubeVertex(float edge_s, int v_i, int r_i, int f_i)
{
    // Центрирование (половина ребра куба)
    float edge_div_2 = edge_s / 2.0f;
    return  Vertexd(
                    QVector3D(edge_div_2 * vec_vert_sign[v_i][0], edge_div_2 * vec_vert_sign[v_i][1], edge_div_2 * vec_vert_sign[v_i][2]),
                    QVector2D(vec_rot_txtr[r_i][0], vec_rot_txtr[r_i][1]),
                    QVector3D(vec_front_face[f_i][0], vec_front_face[f_i][1], vec_front_face[f_i][2]));
}
