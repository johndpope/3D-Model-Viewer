#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16);
}

void GLWidget::initializeGL(){
    glClearColor(.753, 0, .46, 0);
    glShadeModel (GL_SMOOTH);

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glRotatef(1,1,1,0);
    /* load coordinates into triangles through loop */
    /* refer to boxes for examples on interaction */
    glBegin(GL_TRIANGLES);

    if (objPtr){
        Vertex v;
        Vertex f;
        int v1, v2, v3;

            for (unsigned i = 0 ; i < faces.size() ; i++)
            {
                /* x, y, z in face f are the indices of the vertices that make that face */
                /* need to glVertex3f for each vertex */
                f = faces.at(i);
                v1 = f.getX();
                v2 = f.getY();
                v3 = f.getZ();

                v = vertices.at(v1 - 1);
                glColor3f(0,1,1);
                glVertex3f(v.getX(), v.getY(), v.getZ());
               // glColor3f(0,0,0);
                v = vertices.at(v2 - 1);
                glVertex3f(v.getX(), v.getY(), v.getZ());
                v = vertices.at(v3 - 1);
                glVertex3f(v.getX(), v.getY(), v.getZ());
            }
    }
    glEnd();

}

void GLWidget::grabObj(objLoad objFile){
    objPtr = &objFile;
    vertices = objPtr->getVertices();
    faces = objPtr->getFacets();

    /* load coordinates into triangles through loop */
    /* refer to boxes for examples on interaction */

}


void GLWidget::resizeGL(int w, int h){

}
