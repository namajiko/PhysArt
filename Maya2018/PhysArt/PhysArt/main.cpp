#include <maya/MFnPlugin.h>
#include "deform.h"
#include "drawGeometry.h"

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "namajiko.Inc", "1.0", "any");

	std::cout << "Load PhysArt Plugin" << std::endl;

	//deform
	MStatus resultDeform;
	resultDeform = plugin.registerNode("yTwist", yTwist::id, yTwist::creator,
		yTwist::initialize, MPxNode::kDeformerNode);

	//drawGeometry
	MStatus resultDrawGeometry = plugin.registerNode(PALocator::typeName,
		PALocator::typeId,
		PALocator::creator,
		PALocator::initialize,
		MPxNode::kLocatorNode,
		&PALocator::drawDbClassification);

	resultDrawGeometry = MHWRender::MDrawRegistry::registerDrawOverrideCreator(PALocator::drawDbClassification,
		PALocator::drawRegistrantId,
		LocatorDrawOverride::Creator);

	return MStatus::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	std::cout << "Unload PhysArt Plugin" << std::endl;
	
	//deform
	MStatus result;
	result = plugin.deregisterNode(yTwist::id);

	//drawGeometry
	MStatus resultDrawGeometry = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(PALocator::drawDbClassification,
		PALocator::drawRegistrantId);
	resultDrawGeometry = plugin.deregisterNode(PALocator::typeId);

	return MStatus::kSuccess;
}
