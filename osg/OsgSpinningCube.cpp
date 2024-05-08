#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgUtil/Optimizer>
#include <osgDB/WriteFile>

osg::Geometry* generateCube(float size) {
    
    // Establecemos las coordenadas de cada vértice
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(8);
    (*vertices)[0].set(-size, -size, -size);
    (*vertices)[1].set(size, -size, -size);
    (*vertices)[2].set(size, size, -size);
    (*vertices)[3].set(-size, size, -size);
    (*vertices)[4].set(-size, -size, size);
    (*vertices)[5].set(size, -size, size);
    (*vertices)[6].set(size, size, size);
    (*vertices)[7].set(-size, size, size);

    // Creamos la geometría que contenga dicho array
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->setVertexArray(vertices.get());

    
    // Creamos las caras del cubo
    osg::ref_ptr<osg::DrawElementsUInt> faces = new osg::DrawElementsUInt(GL_QUADS, 24);
    faces->push_back(0); faces->push_back(1); faces->push_back(2); faces->push_back(3); // Front
    faces->push_back(7); faces->push_back(6); faces->push_back(5); faces->push_back(4); // Back
    faces->push_back(0); faces->push_back(4); faces->push_back(5); faces->push_back(1); // Bottom
    faces->push_back(3); faces->push_back(2); faces->push_back(6); faces->push_back(7); // Top
    faces->push_back(1); faces->push_back(5); faces->push_back(6); faces->push_back(2); // Right
    faces->push_back(0); faces->push_back(3); faces->push_back(7); faces->push_back(4); // Left

    
    // Le decimos a la geometría que utilice la distribución anterior
    geom->addPrimitiveSet(faces.get());
    return geom.release(); //Se libera el ref_ptr antes de retornar su valor
}

int main() {
    // Generamos la geometría
    osg::ref_ptr<osg::Geometry> cubeGeometry = generateCube(1.0f);
    osg::ref_ptr<osg::Geometry> secondCubeGeometry = generateCube(1.0f);

    // Creamos Geode para la geometría
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::ref_ptr<osg::Geode> secondGeode = new osg::Geode;
    geode->addDrawable(cubeGeometry.get());
    secondGeode-> addDrawable(secondCubeGeometry.get());

    // Creamos la matriz para la rotación
    osg::ref_ptr<osg::MatrixTransform> rotationTransform = new osg::MatrixTransform;
    osg::ref_ptr<osg::MatrixTransform> secondRotationTransform = new osg::MatrixTransform;
    rotationTransform->addChild(geode.get());
    secondRotationTransform->addChild(secondGeode.get());
    
    
    osg::Matrix matrix = osg::Matrix::translate(osg::Vec3(-5.0, -1.0, 0.0)); // Posición del segundo cubo
    rotationTransform->setMatrix(matrix);
    osg::Matrix matrix2 = osg::Matrix::translate(osg::Vec3(5.0, 1.0, 0.0)); // Posición del segundo cubo
    secondRotationTransform->setMatrix(matrix2);
    
    float moveSpeed = 0.1f;

    // Crear un nodo raíz de la escena
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(rotationTransform.get());
    root->addChild(secondRotationTransform.get());

    // Visor con manipulación de cámara y escena
    osgViewer::Viewer viewer;
    viewer.setCameraManipulator(new osgGA::TrackballManipulator);
    viewer.setSceneData(root.get());

    // Bucle de render
    while (!viewer.done()) {
        double time = viewer.elapsedTime();
        float angle = osg::DegreesToRadians(static_cast<float>(time) * 45.0f);
        osg::Matrix rotationMatrix, secondRotationMatrix;
        rotationMatrix.makeRotate(angle, osg::Vec3(1.0, 1.0, 0.0));
        secondRotationMatrix.makeRotate(angle, osg::Vec3(-1.0, -1.0, 0.0));
        rotationTransform->setMatrix(rotationMatrix);
        secondRotationTransform->setMatrix(secondRotationMatrix);
        
        osg::Vec3d translationVector(std::sin(time * moveSpeed) * 2.0, std::cos(time* moveSpeed) * 1.7, std::sin(time * 1.3) * std::cos(time * 1.5) * 2.0);
        osg::Matrix translationMatrix;
        translationMatrix.makeTranslate(translationVector);
        
        osg::Vec3d secondTranslationVector(std::sin(time * moveSpeed) * -2.0, std::cos(time* moveSpeed) * -1.7, std::sin(time * 1.3) * std::cos(time * 1.5) * -2.0);
        osg::Matrix secondTranslationMatrix;
        secondTranslationMatrix.makeTranslate(secondTranslationVector);
        
        rotationTransform->setMatrix(translationMatrix * rotationMatrix);
        secondRotationTransform->setMatrix(secondTranslationMatrix * secondRotationMatrix);
        viewer.frame();
    }

    return 0;
}
