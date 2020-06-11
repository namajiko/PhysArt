#include "drawGeometry.h"

MTypeId PALocator::typeId(0x70200);
MString PALocator::typeName("PALocator");
MString PALocator::drawDbClassification("drawdb/geometry/PALocator");
MString PALocator::drawRegistrantId("PALocatorNodePlugin");

static float vertexes[] = { 0.0f, 0.0f, -1.0f,
0.0f, 1.0f, 0.0f,
1.0f, 0.0f, 0.0f,
0.0f, 0.0f, 1.0f,
-1.0f, 0.0f, 0.0f,
0.0f, -1.0f, 0.0f };

static unsigned int tri_indexes[] = { 0, 1, 2,
2, 1, 3,
3, 1, 4,
4, 1, 0,
0, 5, 2,
2, 5, 3,
3, 4, 5,
4, 0, 5 };


void* PALocator::creator()
{
	return new PALocator();
}

MStatus PALocator::initialize()
{
	return MStatus::kSuccess;
}

MStatus PALocator::compute(const MPlug& plug, MDataBlock& data)
{
	return MStatus::kSuccess;
}

void PALocator::draw(M3dView& view,
	const MDagPath& path,
	M3dView::DisplayStyle style,
	M3dView::DisplayStatus status)
{

	LocatorDrawUserData drawData;

	drawData.is_wireframe = style == M3dView::kWireFrame;

	const unsigned int displayStatus = path.getDisplayStatus();
	drawData.is_selected = !(displayStatus == M3dView::kActive || displayStatus == M3dView::kLead);

	view.beginGL();
	PALocator::display(&drawData);
	view.endGL();
}

void PALocator::display(const LocatorDrawUserData* drawData)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexes);
	if (drawData->is_selected)
	{
		glColor3f(0.26f, 1.0f, 0.64f);
	}
	else
	{
		glColor3f(1.0f, 1.0f, 0.0f);
	}
	
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, tri_indexes);
	
	glDisableClientState(GL_VERTEX_ARRAY);
}


MHWRender::MPxDrawOverride* LocatorDrawOverride::Creator(const MObject& obj)
{
	return new LocatorDrawOverride(obj);
}

MHWRender::DrawAPI LocatorDrawOverride::supportedDrawAPIs() const
{
	return MHWRender::kOpenGL;
}

MUserData* LocatorDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath,
	const MHWRender::MFrameContext& frameContext, MUserData* oldData)
{

	LocatorDrawUserData* drawData = dynamic_cast<LocatorDrawUserData*>(oldData);
	if (drawData == NULL)
	{
		drawData = new LocatorDrawUserData();
	}

	drawData->is_wireframe = frameContext.getDisplayStyle() & MHWRender::MDrawContext::kWireFrame;

	const MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(objPath);
	drawData->is_selected = MHWRender::kActive == displayStatus || MHWRender::kLead == displayStatus;

	return drawData;
}

void LocatorDrawOverride::draw(const MHWRender::MDrawContext& context, const MUserData* userdata)
{
	MStatus status;
	const LocatorDrawUserData* drawData = dynamic_cast<const LocatorDrawUserData*>(userdata);
	const MMatrix transform = context.getMatrix(MHWRender::MFrameContext::kWorldViewMtx, &status);
	if (status != MStatus::kSuccess)
	{
		return;
	}
	const MMatrix projection = context.getMatrix(MHWRender::MFrameContext::kProjectionMtx, &status);
	if (status != MStatus::kSuccess)
	{
		return;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixd(transform.matrix[0]);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixd(projection.matrix[0]);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	PALocator::display(drawData);

	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}