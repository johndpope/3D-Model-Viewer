#ifndef MODEL_H
#define MODEL_H
#include <QQuaternion>
#include <map>
#include "objload.h"
typedef Polyhedron::HalfedgeDS HDS;
class model {
public:
    model();
    model(std::vector<float> _center, float radius);
    model(objLoad<HDS> objFile);
    Polyhedron computeNormals(Polyhedron poly);
    Polyhedron poly();
    //Nef_Polyhedron nefPoly;
    void drawMe();
    void drawPoly(Polyhedron poly);
    CGAL::Triangle_3<Kernel> makeTriangle(Polyhedron::Facet_const_handle f);
    std::vector<CGAL::Triangle_3<Kernel> > makeCellVec(Tr::Cell c);
    void drawTriangle(Polyhedron::Facet_const_handle f);
    bool generateVolumeMesh();
    void drawVolume();
    void drawTriangle(Point p1, Point p2, Point p3);
    std::vector<float> center();
    float radius();
    std::vector<float> max();
    std::vector<float> min();
    float getTransX();
    float getTransY();
    void setTransX(float f);
    void setTransY(float f);
    void setColor(double r, double g, double b);
    void translate(float xT, float yT);
    void moveToCenter();
    void seekIntersections(Plane plane_query);
    void seekIntersections2(Plane plane_query);
    void seekIntersections3(Plane plane_query);
    void drawIntersections();
    void drawIntersections2();
    void clearIntersections();
    std::vector<CGAL::Triangle_3<Kernel> > makeMap(Polyhedron poly);
    std::vector<std::vector<CGAL::Triangle_3<Kernel> > > makeMap(C3T3 c3t3);
    Tri_Pair triPair(CGAL::Triangle_3<Kernel> tri);
    Polyhedron volumePolyhedron();
    bool hasVolume();
    bool toggleMode();

private:
    std::vector<CGAL::Triangle_3<Kernel> > surface_triMap;
    std::vector<std::vector<CGAL::Triangle_3<Kernel> > > volume_triMap;
    objLoad<HDS> *objPtr = 0;
    Polyhedron volume_poly;
    Polyhedron surface_poly;
    //Polyhedron2 poly2;
    Mesh_Polyhedron mesh;
    QQuaternion currQ;
    C3T3 c3t3;
    Tr t;
    std::list<Plane_intersection> intersections;
    float m_radius;
    double red,green,blue;
    std::vector<float> m_center;
    std::vector<float> maxCoords;
    std::vector<float> minCoords;
    std::vector<float> currTrans;
    std::vector<std::pair<CGAL::Triangle_3<Kernel>,CGAL::Vector_3<Kernel> > > intersections2;
    std::vector<std::pair<CGAL::Triangle_3<Kernel>,CGAL::Vector_3<Kernel> > > intersections3;
    bool isPlane;
    bool volumeMode;
    bool hasVol;
};

// A modifier creating a triangle with the incremental builder.
template<class HDS>
class polyhedron_builder : public CGAL::Modifier_base<HDS> {
public:
 C3T3 c3t3;
 Tr t;
    polyhedron_builder(C3T3 _c3t3){
        c3t3 = _c3t3;
        t = c3t3.triangulation();
    }
    void operator()( HDS& hds) {
  typedef typename HDS::Vertex   Vertex;
  typedef typename Vertex::Point Point;

  // create a cgal incremental builder
        CGAL::Polyhedron_incremental_builder_3<HDS> builder(hds, true);
        builder.begin_surface(0,0);
        t = c3t3.triangulation();
        int i = 0;
        int v0,v1,v2,v3;
        for (Tr::Finite_cells_iterator fIt = t.finite_cells_begin(); fIt != t.finite_cells_end(); ++fIt) {
            builder.add_vertex(fIt->vertex(0)->point()); v0 = i++;
            builder.add_vertex(fIt->vertex(1)->point()); v1 = i++;
            builder.add_vertex(fIt->vertex(2)->point()); v2 = i++;
            builder.add_vertex(fIt->vertex(3)->point()); v3 = i++;

            builder.begin_facet();
            builder.add_vertex_to_facet(v0);
            builder.add_vertex_to_facet(v1);
            builder.add_vertex_to_facet(v2);
            builder.end_facet();

            builder.begin_facet();
            builder.add_vertex_to_facet(v0);
            builder.add_vertex_to_facet(v2);
            builder.add_vertex_to_facet(v3);
            builder.end_facet();

            builder.begin_facet();
            builder.add_vertex_to_facet(v0);
            builder.add_vertex_to_facet(v3);
            builder.add_vertex_to_facet(v1);
            builder.end_facet();

            builder.begin_facet();
            builder.add_vertex_to_facet(v1);
            builder.add_vertex_to_facet(v3);
            builder.add_vertex_to_facet(v2);
            builder.end_facet();
        }
        builder.end_surface();
    }
};

// A modifier creating a triangle with the incremental builder.
template <class HDS>
class Build_plane : public CGAL::Modifier_base<HDS> {
public:
    std::vector<float> center;
    float radius;
    Build_plane(std::vector<float> _center, float _radius) {
        center = _center;
        radius = _radius;
    }
    void operator()( HDS& hds) {
        // Postcondition: hds is a valid polyhedral surface.
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
        B.begin_surface( 0, 0, 0);
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;
        B.add_vertex( Point( center.at(0) - radius, center.at(1) - radius, center.at(2)));
        B.add_vertex( Point( center.at(0) - radius, center.at(1) + radius, center.at(2)));
        B.add_vertex( Point( center.at(0) + radius, center.at(1) - radius, center.at(2)));
        B.add_vertex( Point( center.at(0) + radius, center.at(1) + radius, center.at(2)));

        B.begin_facet();
        B.add_vertex_to_facet( 0);
        B.add_vertex_to_facet( 3);
        B.add_vertex_to_facet( 1);
        B.end_facet();

        B.begin_facet();
        B.add_vertex_to_facet( 0);
        B.add_vertex_to_facet( 2);
        B.add_vertex_to_facet( 3);
        B.end_facet();
        B.end_surface();
        std::cout << "ended surface" << std::endl;

    }
};


#endif // MODEL_H
