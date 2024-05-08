#include <osgViewer/Viewer>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgUtil/Optimizer>
#include <osgDB/WriteFile>
#include <osg/PositionAttitudeTransform>


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
    
    // Definimos las coordenadas de textura de los vértices
    osg::ref_ptr<osg::Vec2Array> texCoords = new osg::Vec2Array(24);
    (*texCoords)[0].set(0.0, 0.0); (*texCoords)[1].set(1.0, 0.0); (*texCoords)[2].set(1.0, 1.0); (*texCoords)[3].set(0.0, 1.0);
    (*texCoords)[4].set(0.0, 0.0); (*texCoords)[5].set(1.0, 0.0); (*texCoords)[6].set(1.0, 1.0); (*texCoords)[7].set(0.0, 1.0);
    (*texCoords)[8].set(0.0, 0.0); (*texCoords)[9].set(1.0, 0.0); (*texCoords)[10].set(1.0, 1.0); (*texCoords)[11].set(0.0, 1.0);
    (*texCoords)[12].set(0.0, 0.0); (*texCoords)[13].set(1.0, 0.0); (*texCoords)[14].set(1.0, 1.0); (*texCoords)[15].set(0.0, 1.0);
    (*texCoords)[16].set(0.0, 0.0); (*texCoords)[17].set(1.0, 0.0); (*texCoords)[18].set(1.0, 1.0); (*texCoords)[19].set(0.0, 1.0);
    (*texCoords)[20].set(0.0, 0.0); (*texCoords)[21].set(1.0, 0.0); (*texCoords)[22].set(1.0, 1.0); (*texCoords)[23].set(0.0, 1.0);
    
    geom->setTexCoordArray(0, texCoords.get(), osg::Array::BIND_PER_VERTEX);

    
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
    
    //Generamos el cubo de luz
    osg::ref_ptr<osg::Light> light = new osg::Light;
    light->setLightNum(1);
    light->setPosition(osg::Vec4(0.0, 0.0, 0.0, 1.0));
    light->setDiffuse(osg::Vec4(0.0, 1.0, 0.0, 1.0));
    
    
    
    osg::ref_ptr<osg::Geode> lightGeode = new osg::Geode;
    osg::ref_ptr<osg::Geometry> lightGeometry = generateCube(0.25f);
    lightGeode->addDrawable(lightGeometry);
    
    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
    lightSource->setLight(light);
    
    osg::ref_ptr<osg::PositionAttitudeTransform> lightPAT = new osg::PositionAttitudeTransform;
    lightPAT->setPosition(osg::Vec3(5.0, 10.0, 2.5));
    lightPAT->setScale(osg::Vec3(0.2, 0.2, 0.2));
    lightPAT->addChild(lightSource);
    
    
    
    // Generamos textura
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile("texture.jpeg");
    if (image) {
    	texture->setImage(image.get());
    	texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
    	texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
    } else {
    	std::cerr << "Error al cargar la textura" << std::endl;
    	return 1;
    }
    
    // Asignamos la textura al cubo
    cubeGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());
    secondCubeGeometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture.get());

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
    
    root->addChild(lightGeode);
    root->addChild(lightPAT);
    
    osg::ref_ptr<osg::StateSet> ss = root->getOrCreateStateSet();
    ss->setMode(GL_LIGHT1, osg::StateAttribute::ON);

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
