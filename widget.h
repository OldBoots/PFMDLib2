#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

struct Vertexd{
    Vertexd(){}
    Vertexd(QVector3D p, QVector2D t, QVector3D n) :
        posicion_point(p), texture_coord(t), normal(n)
    {
    }
    QVector3D posicion_point;
    QVector2D texture_coord;
    // Куда будет направлена грань
    QVector3D normal;
};

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
private:
    void initShaders();
    void initCube(float wight);
    Vertexd initCubeVertex(float edge_s, int vert_i, int f_i, int r_i);
private:
    QMatrix4x4 m_projection_matrix;
    QOpenGLShaderProgram m_program;
    QOpenGLTexture *m_texture;
    QOpenGLBuffer m_arr_buffer;
    QOpenGLBuffer m_index_buffer;
    QVector <QVector3D> vec_vert_sign;
    QVector <QVector3D> vec_front_face;
    QVector <QVector2D> vec_rot_txtr;
};

#endif // WIDGET_H
