#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/GUIEventHandler>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/PrimitiveSet>
#include <osg/Camera>
#include <osgUtil/SmoothingVisitor>


class CameraSwitcher : public osgGA::GUIEventHandler {
public:
    CameraSwitcher(osgViewer::Viewer* viewer) : _viewer(viewer) {}

    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&) override {
        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN) {
            switch (ea.getKey()) {
                case '1': setCameraView(1); return true;
                case '2': setCameraView(2); return true;
                case '3': setCameraView(3); return true;
            }
        }
        return false;
    }

    void setCameraView(int id) {
        osg::ref_ptr<osgGA::TrackballManipulator> manip = new osgGA::TrackballManipulator();

        switch (id) {
            case 1: // Frontal
                _viewer->getCamera()->setProjectionMatrixAsPerspective(45.0, 4.0 / 3.0, 1.0, 1000.0);
                manip->setHomePosition({0.0f, -10.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
                break;

            case 2: // Cenital (ortográfica)
                _viewer->getCamera()->setProjectionMatrixAsOrtho(-6.0, 6.0, -4.5, 4.5, 1.0, 1000.0);
                manip->setHomePosition({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
                break;

            case 3: // Esquinada
                _viewer->getCamera()->setProjectionMatrixAsPerspective(45.0, 4.0 / 3.0, 1.0, 1000.0);
                manip->setHomePosition({10.0f, -10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
                break;
        }

        _viewer->setCameraManipulator(manip);
        manip->home(0.0);
    }

private:
    osgViewer::Viewer* _viewer;
};

osg::ref_ptr<osg::Material> createMaterial(const osg::Vec4& color) {
    osg::ref_ptr<osg::Material> mat = new osg::Material();
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, color);
    mat->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 1, 1, 1));
    mat->setShininess(osg::Material::FRONT_AND_BACK, 50.0f);
    return mat;
}

osg::ref_ptr<osg::Group> createScene() {
    osg::ref_ptr<osg::Group> root = new osg::Group();

    // Luz principal
    osg::ref_ptr<osg::Light> light = new osg::Light();
    light->setLightNum(0);
    light->setPosition(osg::Vec4(10.0f, -10.0f, 10.0f, 1.0f));
    light->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light->setSpecular(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    light->setAmbient(osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f));

    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource();
    lightSource->setLight(light);
    lightSource->setLocalStateSetModes(osg::StateAttribute::ON);
    root->addChild(lightSource);

    // Cubo azul
    osg::ref_ptr<osg::Geode> cube = new osg::Geode();
    cube->addDrawable(new osg::ShapeDrawable(new osg::Box({-3, 0, 0}, 1.5f)));
    cube->getOrCreateStateSet()->setAttributeAndModes(createMaterial(osg::Vec4(0, 0, 1, 1)), osg::StateAttribute::ON);
    root->addChild(cube);

    // Esfera roja
    osg::ref_ptr<osg::Geode> sphere = new osg::Geode();
    sphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere({0, 0, 0}, 1.0f)));
    sphere->getOrCreateStateSet()->setAttributeAndModes(createMaterial(osg::Vec4(1, 0, 0, 1)), osg::StateAttribute::ON);
    root->addChild(sphere);

    // Pirámide de base cuadrada verde
    osg::ref_ptr<osg::Geode> pyramid = new osg::Geode();
    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry();
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();

    vertices->push_back(osg::Vec3(-1, -1, 0)); 
    vertices->push_back(osg::Vec3( 1, -1, 0)); 
    vertices->push_back(osg::Vec3( 1,  1, 0)); 
    vertices->push_back(osg::Vec3(-1,  1, 0)); 
    vertices->push_back(osg::Vec3( 0,  0, 2)); 

    geometry->setVertexArray(vertices);

    // Triángulos: caras laterales 
    osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_TRIANGLES);
    indices->push_back(0); indices->push_back(1); indices->push_back(4);
    indices->push_back(1); indices->push_back(2); indices->push_back(4);
    indices->push_back(2); indices->push_back(3); indices->push_back(4);
    indices->push_back(3); indices->push_back(0); indices->push_back(4);

    // Base 
    indices->push_back(0); indices->push_back(1); indices->push_back(2);
    indices->push_back(0); indices->push_back(2); indices->push_back(3);

    geometry->addPrimitiveSet(indices);

    osgUtil::SmoothingVisitor::smooth(*geometry);

    pyramid->addDrawable(geometry);
    pyramid->getOrCreateStateSet()->setAttributeAndModes(createMaterial(osg::Vec4(0, 1, 0, 1)), osg::StateAttribute::ON);

    osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform();
    transform->setMatrix(osg::Matrix::translate(3, 0, -1.0)); 
    transform->addChild(pyramid);
    root->addChild(transform);


    return root;
}

int main() {
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Group> scene = createScene();
    viewer.setSceneData(scene);
    viewer.setUpViewInWindow(100, 100, 800, 600);

    CameraSwitcher* switcher = new CameraSwitcher(&viewer);
    viewer.addEventHandler(switcher);
    switcher->setCameraView(1); 

    return viewer.run();
}
