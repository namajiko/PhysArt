#pragma once
#include <maya/MPxLocatorNode.h>
#include <maya/MUserData.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MDrawRegistry.h>
#include <maya/MDrawContext.h>
#include <maya/MHWGeometryUtilities.h>
#include <gl/GLU.h>


class LocatorDrawUserData : public MUserData
{
public:
	LocatorDrawUserData() : MUserData(false)
	{
		this->is_wireframe = false;
		this->is_selected = false;
	}
	virtual ~LocatorDrawUserData() {}

	bool is_wireframe;
	bool is_selected;
};

class PALocator : public MPxLocatorNode
{
public:
	PALocator() {};
	virtual ~PALocator() {};

	static MTypeId typeId;
	static MString typeName;
	static MString drawDbClassification;
	static MString drawRegistrantId;

	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data);
	virtual void draw(M3dView& view,
		const MDagPath& path,
		M3dView::DisplayStyle style,
		M3dView::DisplayStatus status);
	static void display(const LocatorDrawUserData* drawData);
};

class LocatorDrawOverride : public MHWRender::MPxDrawOverride
{
public:
	LocatorDrawOverride(const MObject& obj) : MHWRender::MPxDrawOverride(obj, LocatorDrawOverride::draw) {}
	virtual ~LocatorDrawOverride() {};

	static MHWRender::MPxDrawOverride* Creator(const MObject& obj);
	virtual MHWRender::DrawAPI supportedDrawAPIs() const;
	virtual MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath,
		const MHWRender::MFrameContext& frameContext, MUserData* oldData);
	static void draw(const MHWRender::MDrawContext& context, const MUserData* userdata);
	virtual bool hasUIDrawables() const { return false; }
};


