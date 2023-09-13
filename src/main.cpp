#include <cstdio>
#include <numbers>
#include <iostream>
#include <cmath>
#include <Windows.h>
#include <osgViewer/Viewer>
#include <osg/Texture3D>
#include <osg/Texture2D>
#include <osg/Point>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/TexGen>
#include <osg/StateSet>
#include <osg/BlendFunc>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>

#include "Util.h"
#include "VolumeLIC.h"
#include "PointCloud.h"
#include "StreamLine.h"
#include "OSGB.h"

static inline osg::ref_ptr<osg::CoordinateSystemNode> createEarth() {
	auto* hints = new osg::TessellationHints;
	hints->setDetailRatio(5.0f);

	auto* sd = new osg::ShapeDrawable(
		new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), osg::WGS_84_RADIUS_POLAR), hints);
	sd->setUseVertexBufferObjects(true);
	sd->setUseVertexArrayObject(true);

	auto* geode = new osg::Geode;
	geode->addDrawable(sd);

	auto filename = R"(..\data\land_shallow_topo_2048.jpg)";
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(
		0, new osg::Texture2D(osgDB::readImageFile(filename)));

	osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
	osg::EllipsoidModel* epmodel = new osg::EllipsoidModel();
	csn->setEllipsoidModel(epmodel);
	csn->addChild(geode);

	return csn;
}

//**������---------------------------------------------------------------------------/
int main(int argc, char** argv)
{
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(300, 300, 1920, 1080);
	osg::ref_ptr<osg::Camera> camera = viewer.getCamera();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::CoordinateSystemNode> csn = createEarth();
	root->addChild(csn);

	osg::ref_ptr<osg::Geode> licGeode = VolumeLIC::Generate(camera.get(), llhRange(30.f, 35.f, 130.f, 135.f, 0.f,500000.f));
	root->addChild(licGeode);
	osg::ref_ptr<osg::Geode> licGeode1 = VolumeLIC::Generate(camera.get(), llhRange(18.f, 25.f, 140.f, 145.f, 0.f, 300000.f));
	root->addChild(licGeode1);
	osg::ref_ptr<osg::Geode> licGeode2 = VolumeLIC::Generate(camera.get(), llhRange(5.f, 10.f, 138.f, 140.f, 0.f, 200000.f));
	root->addChild(licGeode2);

	osg::ref_ptr<osg::Geode> pcdGeode = PointCloud::Generate(camera.get(), llhRange(20.f, 30.f, 60.f, 120.f, 600000.f, 2000000.f));
	root->addChild(pcdGeode);

	osg::ref_ptr<osg::Geode> slGeode = StreamLine::Generate(camera.get(), llhRange(-45.f, 35.f, 140.f, 180.f, 1000000.f, 3000000.f));
	root->addChild(slGeode);

	osg::ref_ptr<osg::CoordinateSystemNode> osgbNode = OSGB::LoadFromPath("D:/vscodes/osg/Data/QJXC");
	root->addChild(osgbNode);

	viewer.setSceneData(root.get());
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.setLightingMode(osg::View::NO_LIGHT);
	viewer.realize();
	viewer.run();
	return 0;
}
//**---------------------------------------------------------------------------------/

