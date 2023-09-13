/*----------------------------------------------------*/
/* mainApp.cpp - Volume Rendering Visualization       */
/*                                                    */
/* sunch_nal, 2010                                    */
/* Institute of Meteorology                           */
/* PLA University of Science and Technology           */
#pragma once
#include <osg/ShapeDrawable>
#include <osg/Texture3D>
#include "Util.h"
#define SIZE 100
#define STEP 0.3
#define DISPLAY_RATIO 0.001
#define MINI 0.000001f

namespace StreamLine {
	osg::Vec3 vectors[SIZE][SIZE][SIZE];
	void generateVectorField()
	{
		// 计算一个产生斥力的场
		osg::Vec3 center = osg::Vec3(SIZE * 3 / 4.0f, SIZE / 2.0f, SIZE / 2.0f);
		float power_coef = 1000.0f;

		// 随机生成x正向，y、z小波动的方向矢量
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				for (int k = 0; k < SIZE; k++)
				{
					//vectors[i][j][k] = osg::Vec3(1.0f, static_cast<float>(rand()) / RAND_MAX * .2f - .1f, static_cast<float>(rand()) / RAND_MAX * .5f - .25f);
					vectors[i][j][k] = osg::Vec3(1.0f, 0, 0);
					osg::Vec3 pos(i, j, k);
					vectors[i][j][k] += (pos - center) / (pos - center).length2() / (pos - center).length() * power_coef;
					vectors[i][j][k].normalize();
				}
			}
		}

	}
	osg::ref_ptr<osg::Geode> Generate(osg::ref_ptr<osg::Camera> camera, llhRange range = llhRange()) {

		generateVectorField();

		osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
		osg::ref_ptr<osg::Vec3Array> vt = new osg::Vec3Array();

		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

		int tot = 0, last = 0;

		// 设置x=0的平面上的点作为流线出发点
		for (int jBegin = 0; jBegin < SIZE; jBegin++) {
			for (int kBegin = 0; kBegin < SIZE; kBegin++) {
				if (static_cast<float>(rand()) / RAND_MAX < DISPLAY_RATIO)
				{
					float j = jBegin + static_cast<float>(rand()) / RAND_MAX;
					float k = kBegin + static_cast<float>(rand()) / RAND_MAX;
					float i = 0;
					while (i > -MINI && i<SIZE + MINI && j>-MINI && j < SIZE + MINI && k>-MINI && k < SIZE + MINI)
					{
						// 通过减小步长，使得割线逼近切线，从而得到流线下一位置
						osg::Vec3 delta = vectors[int(i)][int(j)][int(k)] * STEP;
						// 使用颜色来表示矢量的方向
						i += delta[0];
						j += delta[1];
						k += delta[2];
						double tx, ty, tz;
						llh2xyz_Ellipsoid(range,
							i / SIZE,
							j / SIZE,
							k / SIZE,
							tx,
							ty,
							tz);
						v->push_back(osg::Vec3(tx, ty, tz));
						vt->push_back(osg::Vec3(255, 0, 0));
						tot++;
					}
					osg::ref_ptr<osg::DrawArrays> lineStrip = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, last, tot - last);
					geometry->addPrimitiveSet(lineStrip.get());
					last = tot;
				}
			}
		}

		//int idx = 0;
		//std::vector<std::pair<int, int>> lineSegments;
		//for (int i = 0; i < SIZE; i++) {
		//	for (int j = 0; j < SIZE; j++) {
		//		for (int k = 0; k < SIZE; k++) {
		//			float x = (float) i / SIZE;
		//			float z = (float) j / SIZE;
		//			float y = (float) k / SIZE;
		//			osg::Vec3 delta = vectors[int(i)][int(j)][int(k)] * STEP;
		//			osg::Vec3 XYZ(x,y,z);
		//			v->push_back(osg::Vec3(x,y,z));
		//			v->push_back(XYZ + delta);
		//			osg::ref_ptr<osg::DrawArrays> lineStrip = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, idx , 2);
		//			geometry->addPrimitiveSet(lineStrip.get());
		//		}
		//	}
		//}


		geometry->setVertexAttribArray(0, v.get(), osg::Array::BIND_PER_VERTEX);
		geometry->setVertexAttribArray(1, vt.get(), osg::Array::BIND_PER_VERTEX);


		////// Create vertex and fragment shaders
		//osg::ref_ptr<osg::Shader> vertexShader = new osg::Shader(osg::Shader::VERTEX); // Define your vertex shader source
		//osg::ref_ptr<osg::Shader> fragmentShader = new osg::Shader(osg::Shader::FRAGMENT); // Define your fragment shader source
		//vertexShader->loadShaderSourceFromFile("../shaders/pass.vs");
		//fragmentShader->loadShaderSourceFromFile("../shaders/frag.fs");

		////// Create a shader program and attach the shaders
		//osg::ref_ptr<osg::Program> program = new osg::Program;
		//program->addBindAttribLocation("Vertex", 0);
		//program->addBindAttribLocation("TexCoord", 1);

		//program->addShader(vertexShader.get());
		//program->addShader(fragmentShader.get());

		//// Set stateset for the quad
		osg::ref_ptr<osg::StateSet> stateset = geometry->getOrCreateStateSet();

		//osg::Uniform* modelViewProjectionMatrix = new osg::Uniform(osg::Uniform::FLOAT_MAT4, "ModelViewProjectionMatrix");
		//modelViewProjectionMatrix->setUpdateCallback(new ModelViewProjectionMatrixCallback(camera));
		//stateset->addUniform(modelViewProjectionMatrix);

		//osg::Uniform* cameraEye = new osg::Uniform(osg::Uniform::FLOAT_VEC4, "CameraEye");
		//cameraEye->setUpdateCallback(new CameraEyeCallback(camera));
		//stateset->addUniform(cameraEye);

		//// 示例：如何绑定 3DTexture 到 Uniform 
		//// 设置 Unfirom 名称，设置 ID 为0，接着调用 setTextureAttributeAndModes 进行 texture 到 Uniform 的绑定
		//osg::ref_ptr<osg::Uniform> volumeTexture = new osg::Uniform("VolumeTexture", 0);
		//stateset->addUniform(volumeTexture);

		//stateset->setAttributeAndModes(program);

		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);//管理深度测试
		//stateset->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
		// Blend Rendering Related 使用颜色的ALPHA通道进行透明材质渲染
		//stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		//stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
		//osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc();
		//blendFunc->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
		//stateset->setAttributeAndModes(blendFunc);

		// Create a geode to hold the geometry,	add the quad to the geode
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(geometry.get());
		geode->setStateSet(stateset);

		return geode.get();
	}
}




