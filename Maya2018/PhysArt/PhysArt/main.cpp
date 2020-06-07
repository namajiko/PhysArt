#include <maya/MFnPlugin.h>


MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, "namajiko.Inc", "1.0", "any");

	return MStatus::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	return MStatus::kSuccess;
}
