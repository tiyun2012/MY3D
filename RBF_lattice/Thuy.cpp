#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <queue>

#include <maya/MPxNode.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MPxCommand.h>

#include <maya/MTypeId.h>
#include <maya/MGlobal.h>
#include <maya/MPxSkinCluster.h>

#include <maya/MPointArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MDagPathArray.h>
#include <maya/MMeshIntersector.h>
#include <maya/MDagPath.h>
#include <maya/MDGModifier.h>

#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

#include <maya/MItDependencyGraph.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MFnMesh.h>

#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MQuaternion.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnMatrixData.h>

#include <vector>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>

#include <maya/MFloatArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnFloatArrayData.h>
#include <maya/MItGeometry.h>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

//#include "thirdParty/meshScatter/vec3_cu.hpp"
//#include "thirdParty/meshScatter/vec2_cu.hpp"
//#include "thirdParty/meshScatter/utils_sampling.hpp"

#pragma region test
// /////////////////////////////////////////////////
// //command
// /////////////////////////////////////////////////
// class thuyScatterPoint : public MPxCommand
// {
// public:
// 	thuyScatterPoint();
// 	~thuyScatterPoint();
// 	static void *creator();
// 	static const char * cmdName;
// 	static MSyntax newSyntax();
// 	virtual MStatus doIt(const MArgList& argList);
// private:
// };

// const char *thuyScatterPoint::cmdName = "thuyScatterPoint";

// thuyScatterPoint::thuyScatterPoint() {}
// thuyScatterPoint::~thuyScatterPoint() {}

// void *thuyScatterPoint::creator() { return new thuyScatterPoint; }

// MSyntax thuyScatterPoint::newSyntax()
// {
//     MStatus status;
//     MSyntax syntax;

//     status = syntax.addFlag("-r", "-radius", MSyntax::kDouble);
//     CHECK_MSTATUS_AND_RETURN(status, syntax);
//     status = syntax.addFlag("-c", "-count", MSyntax::kLong);
//     CHECK_MSTATUS_AND_RETURN(status, syntax);
//     // status = syntax.addFlag("-p", "-path", MSyntax::kstring);
//     // CHECK_MSTATUS_AND_RETURN(status, syntax);
//     // status = syntax.addFlag("-n", "-name", MSyntax::kstring);
//     // CHECK_MSTATUS_AND_RETURN(status, syntax);
//     // status = syntax.setobjecttype(MSyntax::kselectionlist);
//     // CHECK_MSTATUS_AND_RETURN(status, syntax);

//     return syntax;
// }

// MStatus thuyScatterPoint::doIt(const MArgList &argList)
// {
//     MStatus status;

//     MArgDatabase argData(syntax(), argList, &status);
//     CHECK_MSTATUS_AND_RETURN_IT(status);

//     double r;
//     int c;

//     if (argData.isFlagSet("-r"))
//     {
//         status = argData.getFlagArgument("-r", 0, r);
//         CHECK_MSTATUS_AND_RETURN_IT(status);
//     }
//     if (argData.isFlagSet("-c"))
//     {
//         status = argData.getFlagArgument("-c", 0, c);
//         CHECK_MSTATUS_AND_RETURN_IT(status);
//     }
//     else
//     {
//         return status;
//     }

//     std::vector<Vec3_cu> pos;
//     std::vector<Vec3_cu> nor;
//     std::vector<int> tris_id;
//     // pos.clear();
//     // nor.clear();
//     // tris_id.clear();

//     MPointArray p;
//     MIntArray _tris_id;

//     MSelectionList selection;
//     MGlobal::getActiveSelectionList(selection);
//     // MItSelectionList iter(selection);

//     // MObject oSel;
//     // MDagPath dSel;
//     // for (; !iter.isDone(); iter.next())
//     // {
//     //     status = iter.getDagPath(dSel, oSel);
//     //     CHECK_MSTATUS_AND_RETURN_IT(status);

//     //     // MItMeshPolygon pIter(dSel, oSel, &status);
//     //     // CHECK_MSTATUS_AND_RETURN_IT(status);

//     //     // MStringArray plugName;
//     //     // iter.getStrings(plugName);
//     //     // for (unsigned int i = 0; i<plugName.length(); i++)
//     //     // {
//     //     // 	//MGlobal::displayInfo(MString() + plugName[i]);
//     //     // 	MPointArray pPos;
//     //     // 	for (pIter.reset(); !pIter.isDone(); pIter.next())
//     //     // 	{
//     //     // 		// pIter.getPoints(pPos, MSpace::kWorld);
//     //     // 		// for (unsigned int ii = 0; ii< pPos.length(); ii++)
//     //     // 		// {
//     //     // 		// 	tris_id.append(pIter.index());
//     //     // 		// }
//     //     // 		tris_id.append(pIter.index());
//     //     // 	}
//     //     // }
//     // }

//     if(selection.length() == 0) return MS::kSuccess;

//     MDagPath srcMeshDagPath;
//     // MDagPath tarMeshDagPath;
//     selection.getDagPath(0, srcMeshDagPath);
//     // selection.getDagPath(0, tarMeshDagPath);

//     MFnMesh fn_srcMesh(srcMeshDagPath, &status);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

//     // MFnMesh fn_tarMesh(tarMeshDagPath, &status);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);

//     MIntArray tris_count;
//     fn_srcMesh.getTriangles(tris_count, _tris_id);
//     fn_srcMesh.getPoints(p, MSpace::kObject);

//     // for(int i = 0; i < p.length(); i++ )
//     // {
//     // 	std::cout << "pos: " << i << " _ " << p[i].x << " _ " << p[i].y << " _ " << p[i].z << "\n";
//     // }
//     // for(int i = 0; i < tris_id.length(); i++ )
//     // {
//     // 	std::cout << "id: " << tris_id[i] << "\n";
//     // }

//     for (int i = 0; i < p.length(); i++)
//     {
//         pos.push_back(Vec3_cu(p[i].x, p[i].y, p[i].z));
//         // nor[i]=(Vec3_cu(n[i].x, n[i].y, n[i].z));
//         nor.push_back(Vec3_cu(0,0,0));
//     }

//     for (int i = 0; i < _tris_id.length(); i++)
//     {
//         tris_id.push_back(_tris_id[i]);
//     }

//     std::vector<Vec3_cu> oPos, oNor;
//     std::vector<int> sample_tris_id;

//     Utils_sampling::poisson_disk(r,c,pos,nor,tris_id,oPos,oNor,sample_tris_id);

//     int faceId;
//     std::vector<Vec2_cu> uv;

//     MGlobal::displayInfo(MString() + (oPos.size()));
//     // MGlobal::displayInfo(MString() + (r) +"_"+ (c));
//     // thuythuy((p + "/" + n + ".exr").asChar(), oPos);

//     clearResult();
//     for(int i = 0; i < oPos.size(); i++ )
//     {
//         // std::cout << "pos: " << i << " _ " << oPos[i].x << " _ " << oPos[i].y << " _ " << oPos[i].z << "\n";
//         // status = MGlobal::executeCommand(MString() + "select -d; joint -p " + oPos[i].x +" "+ oPos[i].y +" "+ oPos[i].z);

//         float2 _uv;
//         status = fn_srcMesh.getUVAtPoint(MPoint(oPos[i].x, oPos[i].y, oPos[i].z, 1.0), _uv, MSpace::kWorld, NULL, &faceId);
//         CHECK_MSTATUS_AND_RETURN_IT(status);
//         // std::cout << "uv: " << i << " _ " << _uv[0] << " _ " << _uv[1] << "\n";

//         // uv.push_back(Vec2_cu(_uv[0],_uv[1]));
//         // status = MGlobal::executeCommand(MString() + "select -d; joint -p " + _uv[0] +" "+ _uv[1] +" 0");
//         // CHECK_MSTATUS_AND_RETURN_IT(status);

//         // mpoint _p;
//         // status = fn_tarmesh.getpointatuv(faceid, _p, _uv, mspace::kworld);
//         // check_mstatus_and_return_it(status);

//         // status = mglobal::executecommand(mstring() + "select -d; joint -p " + _p.x +" "+ _p.y +" "+ _p.z);

//         appendToResult((float)faceId);
//         appendToResult(_uv[0]);
//         appendToResult(_uv[1]);
//     }
//     return MS::kSuccess;
// }

// // ---------------------------------------

// class thuyGetPointAtUV : public MPxCommand
// {
// public:
// 	thuyGetPointAtUV();
// 	~thuyGetPointAtUV();
// 	static void *creator();
// 	static const char * cmdName;
// 	static MSyntax newSyntax();
// 	virtual MStatus doIt(const MArgList& argList);
// private:
// };

// const char *thuyGetPointAtUV::cmdName = "thuyGetPointAtUV";

// thuyGetPointAtUV::thuyGetPointAtUV() {}
// thuyGetPointAtUV::~thuyGetPointAtUV() {}

// void *thuyGetPointAtUV::creator() { return new thuyGetPointAtUV; }

// MSyntax thuyGetPointAtUV::newSyntax()
// {
//     MStatus status;
//     MSyntax syntax;

//     status = syntax.addFlag("-id", "-faceid", MSyntax::kLong);
//     CHECK_MSTATUS_AND_RETURN(status, syntax);
//     status = syntax.addFlag("-uv", "-uvdata", MSyntax::kDouble, MSyntax::kDouble);
//     CHECK_MSTATUS_AND_RETURN(status, syntax);
//     return syntax;

// }

// MStatus thuyGetPointAtUV::doIt(const MArgList &argList)
// {
//     MStatus status;

//     MArgDatabase argData(syntax(), argList, &status);
//     CHECK_MSTATUS_AND_RETURN_IT(status);

//     float2 uv;
//     int faceId;
//     MPoint _uv;

//     if (argData.isFlagSet("-id"))
//     {
//         status = argData.getFlagArgument("-faceid", 0, faceId);
//     }

//     if (argData.isFlagSet("-uv"))
//     {
//         status = argData.getFlagArgument("-uvdata", 0, _uv.x);
//         status = argData.getFlagArgument("-uvdata", 1, _uv.y);
//     }
//     else
//     {
//         return status;
//     }

//     MSelectionList selection;
//     MGlobal::getActiveSelectionList(selection);
//     if(selection.length() == 0) return MS::kSuccess;

//     MDagPath tarMeshDagPath;
//     selection.getDagPath(0, tarMeshDagPath);

//     MFnMesh fn_tarMesh(tarMeshDagPath, &status);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

//     MPoint _p;
//     uv[0] = _uv.x;
//     uv[1] = _uv.y;
//     status = fn_tarMesh.getPointAtUV(faceId, _p, uv, MSpace::kWorld);
//     CHECK_MSTATUS_AND_RETURN_IT(status);

//     status = MGlobal::executeCommand(MString() + "select -d; joint -p " + _p.x +" "+ _p.y +" "+ _p.z);

//     return MS::kSuccess;
// }

// /////////////////////////////////////////////////
// //Node
// /////////////////////////////////////////////////
// class MeshSampleNode : public MPxNode
// {
// public:
//   MeshSampleNode();
//   virtual ~MeshSampleNode();

//   virtual MStatus   		compute(const MPlug& Plug, MDataBlock& data);

//   static  void *          creator();
//   static  MStatus         initialize();

//     // static MObject aSrcMesh;
//     static MObject aTarMesh;
//     // static MObject aSampleNum;
//     // static MObject aSampleRadius;
//     static MObject aPointOnMesh;
//   static MObject outMat;
//   static MTypeId id;
// private:
// };

// MObject MeshSampleNode::aTarMesh;
// MObject MeshSampleNode::aPointOnMesh;
// MObject MeshSampleNode::outMat;
// MTypeId MeshSampleNode::id(0x0FF88FF80);

// MeshSampleNode::MeshSampleNode() {}

// MeshSampleNode::~MeshSampleNode() {}

// void *MeshSampleNode::creator()
// {
// 	return new MeshSampleNode();
// }

// MStatus MeshSampleNode::initialize()
// {
// 	MStatus status;
// 	MFnTypedAttribute tAttr;
// 	MFnMatrixAttribute mAttr;
// 	MFnNumericAttribute nAttr;

// 	outMat = mAttr.create("outMat", "outMat");
// 	mAttr.setStorable(false);
// 	mAttr.setKeyable(false);
// 	mAttr.setConnectable(true);
// 	mAttr.setArray(true);
// 	mAttr.setUsesArrayDataBuilder(true);
// 	status = addAttribute(outMat);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	// aSrcMesh = tAttr.create("srcMesh", "srcMesh", MFnData::kMesh);
// 	// tAttr.setStorable(true);
// 	// // tAttr.setKeyable(true);
// 	// status = addAttribute(aSrcMesh);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);
// 	// attributeAffects(aSrcMesh, outMat);

// 	aTarMesh = tAttr.create("tarMesh", "tarMesh", MFnData::kMesh);
// 	tAttr.setStorable(true);
// 	// tAttr.setKeyable(true);
// 	status = addAttribute(aTarMesh);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);
// 	attributeAffects(aTarMesh, outMat);

// 	// aSampleNum = nAttr.create("aSampleNum", "aSampleNum", MFnNumericData::kInt, 10);
// 	// nAttr.setStorable(true);
// 	// // nAttr.setKeyable(true);
// 	// nAttr.setMin(1);
// 	// status = addAttribute(aSampleNum);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);
// 	// attributeAffects(aSampleNum, outMat);

// 	// aSampleRadius = nAttr.create("aSampleRadius", "aSampleRadius", MFnNumericData::kFloat, 1);
// 	// nAttr.setStorable(true);
// 	// // nAttr.setKeyable(true);
// 	// status = addAttribute(aSampleRadius);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);
// 	// attributeAffects(aSampleRadius, outMat);

// 	aPointOnMesh = nAttr.create("aPointOnMesh", "aPointOnMesh", MFnNumericData::kFloat, 1);
// 	nAttr.setStorable(true);
// 	// nAttr.setConnectable(true);
// 	nAttr.setArray(true);
// 	nAttr.setUsesArrayDataBuilder(true);
// 	status = addAttribute(aPointOnMesh);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);
// 	attributeAffects(aPointOnMesh, outMat);

// 	return MS::kSuccess;
// }

// MQuaternion getQuaternionFromAxes(MVector vx, MVector vy, MVector vz){

// 	MMatrix m;
// 	m[0][0] = vx.x;
// 	m[0][1] = vx.y;
// 	m[0][2] = vx.z;
// 	m[1][0] = vy.x;
// 	m[1][1] = vy.y;
// 	m[1][2] = vy.z;
// 	m[2][0] = vz.x;
// 	m[2][1] = vz.y;
// 	m[2][2] = vz.z;

// 	MTransformationMatrix t(m);

// 	return t.rotation();
// }

// MStatus MeshSampleNode::compute(const MPlug &plug, MDataBlock &data)
// {
// 	MStatus status;
// 	unsigned int i;

// 	// input///////////////////////////////
// 	//  MDataHandle hSrcMesh = data.inputValue(aSrcMesh, &status);
// 	//  CHECK_MSTATUS_AND_RETURN_IT(status);
// 	MDataHandle hDriverGeo = data.inputValue(aTarMesh, &status);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);
// 	// int _sampleNum = data.inputValue(aSampleNum, &status).asInt();
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);
// 	// float _sampleRadius = data.inputValue(aSampleRadius, &status).asFloat();
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);

// 	// MObject oSrcMesh = hSrcMesh.asMesh();
// 	MObject oDriverGeo = hDriverGeo.asMesh();
// 	if (oDriverGeo.isNull())
// 	{
// 		return MS::kSuccess;
// 	}

// 	// MFnMesh fn_srcMesh(oSrcMesh, &status);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);

// 	MFnMesh fn_tarMesh(oDriverGeo, &status);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	MArrayDataHandle hPointOnMesh = data.inputArrayValue(aPointOnMesh, &status);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	MArrayDataHandle hOutputMatrices = data.outputArrayValue(outMat, &status);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	MArrayDataBuilder builder(outMat, 0, &status);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	// input transform
// 	MFloatArray faceIdAndUV;
// 	for (i = 0; i < hPointOnMesh.elementCount(); i++)
// 	{
// 		hPointOnMesh.jumpToElement(i);

// 		float curVal = hPointOnMesh.inputValue(&status).asFloat();
// 		CHECK_MSTATUS_AND_RETURN_IT(status);
// 		faceIdAndUV.append(curVal);
// 	}
// 	// set output transform//////////////////
// 	//  MItMeshPolygon itPoly(inMesh, &status);
// 	//  for (i = 0; i < inMats.length(); i++){
// 	//  MDataHandle outHandle = builder.addElement(i);
// 	//  MTransformationMatrix curMat = MTransformationMatrix(inMats[i]);
// 	//  MTransformationMatrix result;
// 	//  MPoint closetPt, outPos;

// 	// MPoint inPos = curMat.translation(MSpace::kWorld);
// 	// MQuaternion inRot = curMat.rotation();

// 	// int faceId;
// 	// status = fUVMesh.getClosestPoint(inPos, closetPt, MSpace::kWorld, &faceId);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);
// 	// double difDist =  (inPos - closetPt).length();
// 	// //closetPt = inUVPos[i];

// 	// //get point at uv
// 	// float2 uv;
// 	// status = fUVMesh.getUVAtPoint(closetPt, uv, MSpace::kWorld, NULL, &faceId);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);

// 	// if (status) {
// 	// 	status = fMesh.getPointAtUV(faceId, outPos, uv, MSpace::kWorld);
// 	// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	// 	if (!status) return MS::kSuccess;

// 	// 	//translation
// 	// 	MVector nor;
// 	// 	status = fMesh.getClosestNormal(outPos, nor, MSpace::kWorld, &faceId);
// 	// 	CHECK_MSTATUS_AND_RETURN_IT(status);
// 	// 	outPos += nor*difDist;
// 	// 	status = result.setTranslation(outPos, MSpace::kWorld);

// 	// 	//rotation
// 	// 	status = result.setRotationQuaternion(inRot.x, inRot.y, inRot.z, inRot.w);

// 	// 	//set result
// 	// outHandle.setMMatrix(result.asMatrix());
// 	// }

// 	// }

// 	std::vector<Vec3_cu> pos;
// 	std::vector<Vec3_cu> nor;
// 	std::vector<int> tris_id;
// 	// pos.clear();
// 	// nor.clear();
// 	// tris_id.clear();

// 	MPointArray p;
// 	MIntArray _tris_id;

// 	MSelectionList selection;
// 	MGlobal::getActiveSelectionList(selection);
// 	// MItSelectionList iter(selection);

// 	// MObject oSel;
// 	// MDagPath dSel;
// 	// for (; !iter.isDone(); iter.next())
// 	// {
// 	//     status = iter.getDagPath(dSel, oSel);
// 	//     CHECK_MSTATUS_AND_RETURN_IT(status);

// 	//     // MItMeshPolygon pIter(dSel, oSel, &status);
// 	//     // CHECK_MSTATUS_AND_RETURN_IT(status);

// 	//     // MStringArray plugName;
// 	//     // iter.getStrings(plugName);
// 	//     // for (unsigned int i = 0; i<plugName.length(); i++)
// 	//     // {
// 	//     // 	//MGlobal::displayInfo(MString() + plugName[i]);
// 	//     // 	MPointArray pPos;
// 	//     // 	for (pIter.reset(); !pIter.isDone(); pIter.next())
// 	//     // 	{
// 	//     // 		// pIter.getPoints(pPos, MSpace::kWorld);
// 	//     // 		// for (unsigned int ii = 0; ii< pPos.length(); ii++)
// 	//     // 		// {
// 	//     // 		// 	tris_id.append(pIter.index());
// 	//     // 		// }
// 	//     // 		tris_id.append(pIter.index());
// 	//     // 	}
// 	//     // }
// 	// }

// 	// if(selection.length() == 0) return MS::kSuccess;

// 	// MDagPath srcMeshDagPath;
// 	// MDagPath tarMeshDagPath;
// 	// selection.getDagPath(0, srcMeshDagPath);
// 	// selection.getDagPath(1, tarMeshDagPath);

// 	// MFnMesh fn_srcMesh(srcMeshDagPath, &status);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);

// 	// MFnMesh fn_tarMesh(tarMeshDagPath, &status);
// 	// CHECK_MSTATUS_AND_RETURN_IT(status);

// 	// MIntArray tris_count;
// 	// fn_srcMesh.getTriangles(tris_count, _tris_id);
// 	// fn_srcMesh.getPoints(p, MSpace::kObject);

// 	// for(int i = 0; i < p.length(); i++ )
// 	// {
// 	// 	std::cout << "pos: " << i << " _ " << p[i].x << " _ " << p[i].y << " _ " << p[i].z << "\n";
// 	// }
// 	// for(int i = 0; i < tris_id.length(); i++ )
// 	// {
// 	// 	std::cout << "id: " << tris_id[i] << "\n";
// 	// }

// 	// for (int i = 0; i < p.length(); i++)
// 	// {
// 	//     pos.push_back(Vec3_cu(p[i].x, p[i].y, p[i].z));
// 	//     // nor[i]=(Vec3_cu(n[i].x, n[i].y, n[i].z));
// 	//     nor.push_back(Vec3_cu(0,0,0));
// 	// }

// 	// for (int i = 0; i < _tris_id.length(); i++)
// 	// {
// 	//     tris_id.push_back(_tris_id[i]);
// 	// }

// 	// std::vector<Vec3_cu> oPos, oNor;
// 	// std::vector<int> sample_tris_id;

// 	// Utils_sampling::poisson_disk(_sampleRadius,_sampleNum,pos,nor,tris_id,oPos,oNor,sample_tris_id);

// 	// int faceId;
// 	// std::vector<Vec2_cu> uv;

// 	// MGlobal::displayInfo(MString() + (oPos.size()));
// 	// MGlobal::displayInfo(MString() + (r) +"_"+ (c));
// 	// thuythuy((p + "/" + n + ".exr").asChar(), oPos);

// 	for (int i = 0; i < faceIdAndUV.length() / 3; i++)
// 	{
// 		float2 _uv;
// 		int faceId = faceIdAndUV[i * 3];
// 		_uv[0] = faceIdAndUV[i * 3 + 1];
// 		_uv[1] = faceIdAndUV[i * 3 + 2];
// 		// for(int i = 0; i < oPos.size(); i++ )
// 		// {
// 		// std::cout << "pos: " << i << " _ " << oPos[i].x << " _ " << oPos[i].y << " _ " << oPos[i].z << "\n";
// 		// status = MGlobal::executeCommand(MString() + "select -d; joint -p " + oPos[i].x +" "+ oPos[i].y +" "+ oPos[i].z);

// 		// status = fn_srcMesh.getUVAtPoint(MPoint(oPos[i].x, oPos[i].y, oPos[i].z, 1.0), _uv, MSpace::kWorld, NULL, &faceId);
// 		// CHECK_MSTATUS_AND_RETURN_IT(status);
// 		// std::cout << "uv: " << i << " _ " << _uv[0] << " _ " << _uv[1] << "\n";

// 		// uv.push_back(Vec2_cu(_uv[0],_uv[1]));
// 		// status = MGlobal::executeCommand(MString() + "select -d; joint -p " + _uv[0] +" "+ _uv[1] +" 0");
// 		// CHECK_MSTATUS_AND_RETURN_IT(status);

// 		MPoint _p;
// 		status = fn_tarMesh.getPointAtUV(faceId, _p, _uv, MSpace::kWorld);
// 		CHECK_MSTATUS_AND_RETURN_IT(status);

// 		MDataHandle outHandle = builder.addElement(i);
// 		MTransformationMatrix result;

// 		status = result.setTranslation(_p, MSpace::kWorld);
// 		CHECK_MSTATUS_AND_RETURN_IT(status);

// 		// //rotation
// 		// MVector _x;
// 		// MFloatVectorArray __x;
// 		// MVector _y;

// 		// status = fn_tarMesh.getFaceVertexTangents(faceId, __x, MSpace::kWorld);
// 		// CHECK_MSTATUS_AND_RETURN_IT(status);
// 		// status = fn_tarMesh.getClosestNormal(_p, _y, MSpace::kWorld);
// 		// CHECK_MSTATUS_AND_RETURN_IT(status);

// 		// _x = __x[0];

// 		// MVector _z = _x ^ _y;
// 		// _z.normalize();

// 		// MQuaternion q = getQuaternionFromAxes(_x, _y, _z);
// 		// status = result.setRotationQuaternion(q.x, q.y, q.z, q.w);
// 		// CHECK_MSTATUS_AND_RETURN_IT(status);

// 		outHandle.setMMatrix(result.asMatrix());
// 		// status = MGlobal::executeCommand(MString() + "select -d; joint -p " + _p.x +" "+ _p.y +" "+ _p.z);
// 		// }
// 	}

// 	status = hOutputMatrices.set(builder);
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	status = hOutputMatrices.setAllClean();
// 	CHECK_MSTATUS_AND_RETURN_IT(status);

// 	data.setClean(plug);

// 	return MS::kSuccess;
// }

#pragma endregion

/////////////////////////////////////////////////
// deformer
/////////////////////////////////////////////////

#pragma region closetPoints

//#include "thirdParty/KDTree/KDTree.hpp"

struct Point {
    double x, y, z;
    int index;

    // Calculate distance to another point
    double distance(const Point& other) const {
        return std::sqrt((x - other.x) * (x - other.x) +
            (y - other.y) * (y - other.y) +
            (z - other.z) * (z - other.z));
    }
};

struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;

    KDNode(const Point& p) : point(p), left(nullptr), right(nullptr) {}
};

// Custom comparator for the max-heap
struct Compare {
    bool operator()(const std::pair<double, int>& a, const std::pair<double, int>& b) {
        return a.first < b.first; // Max-heap: larger distance at the top
    }
};

class KDTree {
public:
    KDTree(std::vector<Point>& points) {
        root = buildTree(points.begin(), points.end(), 0);
    }

    std::vector<int> findKNearest(const Point& target, int k) {
        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, Compare> maxHeap;
        findKNearest(root, target, k, 0, maxHeap);

        std::vector<int> indices;
        while (!maxHeap.empty()) {
            indices.push_back(maxHeap.top().second);
            maxHeap.pop();
        }
        std::reverse(indices.begin(), indices.end());
        return indices;
    }

private:
    KDNode* root;

    // Build the KD-Tree using iterators and nth_element for better performance
    KDNode* buildTree(std::vector<Point>::iterator start, std::vector<Point>::iterator end, int depth) {
        if (start >= end) return nullptr;

        int axis = depth % 3;
        auto comparator = [axis](const Point& a, const Point& b) {
            return (axis == 0) ? (a.x < b.x) : (axis == 1) ? (a.y < b.y) : (a.z < b.z);
            };

        auto medianIt = start + (end - start) / 2;
        std::nth_element(start, medianIt, end, comparator);

        KDNode* node = new KDNode(*medianIt);
        node->left = buildTree(start, medianIt, depth + 1);
        node->right = buildTree(medianIt + 1, end, depth + 1);

        return node;
    }

    // Optimized k-nearest neighbor search
    void findKNearest(KDNode* node, const Point& target, int k, int depth,
        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, Compare>& maxHeap) {
        if (!node) return;

        double dist = target.distance(node->point);
        if (maxHeap.size() < k) {
            maxHeap.emplace(dist, node->point.index);
        }
        else if (dist < maxHeap.top().first) {
            maxHeap.pop();
            maxHeap.emplace(dist, node->point.index);
        }

        int axis = depth % 3;
        double diff = (axis == 0 ? target.x - node->point.x :
            (axis == 1 ? target.y - node->point.y :
                target.z - node->point.z));

        KDNode* first = (diff < 0) ? node->left : node->right;
        KDNode* second = (diff < 0) ? node->right : node->left;

        findKNearest(first, target, k, depth + 1, maxHeap);

        if (maxHeap.size() < k || std::abs(diff) < maxHeap.top().first) {
            findKNearest(second, target, k, depth + 1, maxHeap);
        }
    }
};


#pragma endregion

#pragma region pointDeformer

struct BindData
{
    MPointArray inputPoints;

    std::vector<MDoubleArray> weights; // bone weight per vertex
    std::vector<MIntArray> boneIDs;    // bone weight per vertex
};

struct TaskData
{
    MPointArray points;
    MPointArray restPoints;
    MPointArray driverPoints;
    std::vector<MDoubleArray> weights; // bone weight per vertex
    std::vector<MIntArray> boneIDs;    // bone weight per vertex
};

class thuyPointDeformer : public MPxDeformerNode
{
public:
    static void* creator();
    static MStatus initialize();

    virtual MStatus deform(MDataBlock& data, MItGeometry& itGeo, const MMatrix& localToWorldMatrix, unsigned int geomIndex);
    virtual MStatus setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs);

    static const MTypeId id;
    // static MObject aInPoint;
    static MObject aDriverGeo;
    // static MObject aPointOnMesh;
    // static MObject aInRecalRBF;
    // static MObject aRadius;
    // static MObject aCount;
    // static MObject aThres;
    static MObject aBoneWeights;
    static MObject aBoneIDs;
    static MObject aBindData;
    static MObject aRestPoints;

private:
    MStatus getBindInfo(MDataBlock& data, unsigned int geomIndex, TaskData& taskData);
    // std::vector<std::vector<float>> _weights; // bone weight per vertex
    // std::vector<std::vector<int>> boneID;     // bone weight per vertex
    // MPointArray __p;
    // MPointArray drivenPts;
    // std::vector<Vec3_cu> oPos, oNor;
    // std::vector<indexArr> closePoints;
    // MPointArray driverPts;
    // MPointArray restPts;
    std::map<unsigned int, bool> dirty_;
    std::vector<TaskData> taskData_;  /**< Per geometry evaluation data. */
};

const MTypeId thuyPointDeformer::id(0x80FF808F88);
// MObject thuyPointDeformer::aInPoint;
MObject thuyPointDeformer::aDriverGeo;
// MObject thuyPointDeformer::aPointOnMesh;
// MObject thuyPointDeformer::aInRecalRBF;
// MObject thuyPointDeformer::aRadius;
// MObject thuyPointDeformer::aCount;
// MObject thuyPointDeformer::aThres;
MObject thuyPointDeformer::aBoneIDs;
MObject thuyPointDeformer::aBoneWeights;
MObject thuyPointDeformer::aBindData;
MObject thuyPointDeformer::aRestPoints;

void* thuyPointDeformer::creator()
{
    return new thuyPointDeformer();
}

MStatus thuyPointDeformer::initialize()
{
    MStatus status;
    MFnTypedAttribute tAttr;
    MFnNumericAttribute nAttr;
    MFnCompoundAttribute cAttr;

    aDriverGeo = tAttr.create("driverGeo", "driverGeo", MFnData::kMesh);
    status = addAttribute(aDriverGeo);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    attributeAffects(aDriverGeo, outputGeom);

    aBoneIDs = tAttr.create("boneID", "boneID", MFnData::kIntArray);
    tAttr.setArray(true);

    aBoneWeights = tAttr.create("boneWeights", "boneWeights", MFnData::kDoubleArray);
    tAttr.setArray(true);

    aRestPoints = nAttr.create("restPoints", "restPoints", MFnNumericData::k3Float);
    nAttr.setArray(true);
    status = addAttribute(aRestPoints);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    aBindData = cAttr.create("bindData", "bindData");
    cAttr.setArray(true);
    cAttr.addChild(aBoneIDs);
    cAttr.addChild(aBoneWeights);
    status = addAttribute(aBindData);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = attributeAffects(aBoneIDs, outputGeom);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = attributeAffects(aBoneWeights, outputGeom);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = MGlobal::executeCommandOnIdle("makePaintable -attrType multiFloat -sm deformer thuyPointDeformer weights");
    CHECK_MSTATUS_AND_RETURN_IT(status);

    return MStatus::kSuccess;
}

MStatus thuyPointDeformer::getBindInfo(MDataBlock& data, unsigned int geomIndex, TaskData& taskData)
{
    MStatus status;

    MArrayDataHandle hBindDataArray = data.inputArrayValue(thuyPointDeformer::aBindData);
    status = hBindDataArray.jumpToElement(geomIndex);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle hBindData = hBindDataArray.inputValue();

    MArrayDataHandle hBoneIDs = hBindData.child(thuyPointDeformer::aBoneIDs);
    MArrayDataHandle hBoneWeights = hBindData.child(thuyPointDeformer::aBoneWeights);

    unsigned int numComponents = hBoneIDs.elementCount();
    if (numComponents == 0)
    {
        return MS::kNotImplemented;
    }
    MGlobal::displayInfo(MString() + hBoneIDs.elementCount());
    MGlobal::displayInfo(MString() + hBoneWeights.elementCount());

    hBoneIDs.jumpToArrayElement(0);
    hBoneWeights.jumpToArrayElement(0);

    for (unsigned int i = 0; i < numComponents; ++i)
    {
        int logicalIndex = hBoneIDs.elementIndex();
        if (logicalIndex >= taskData.boneIDs.size())
        {
            taskData.weights.resize(logicalIndex + 1);
            taskData.boneIDs.resize(logicalIndex + 1);
        }
        // Get sample ids
        MObject oIndexData = hBoneIDs.inputValue().data();
        MFnIntArrayData fnIntData(oIndexData, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        taskData.boneIDs[logicalIndex] = fnIntData.array();

        // Get sample weights
        MObject oWeightData = hBoneWeights.inputValue().data();
        MFnDoubleArrayData fnDoubleData(oWeightData, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        taskData.weights[logicalIndex] = fnDoubleData.array();
        assert(taskData.weights[logicalIndex].length() == taskData.boneIDs[logicalIndex].length());

        hBoneIDs.next();
        hBoneWeights.next();
    }

    MArrayDataHandle hRestPoints = data.inputArrayValue(thuyPointDeformer::aRestPoints);
    unsigned int restNumComponents = hRestPoints.elementCount();
    hRestPoints.jumpToArrayElement(0);
    if (restNumComponents == 0)
    {
        return MS::kNotImplemented;
    }
    MGlobal::displayInfo(MString() + restNumComponents);
    for (unsigned int i = 0; i < restNumComponents; ++i)
    {
        int logicalIndex = hRestPoints.elementIndex();
        if (logicalIndex >= taskData.restPoints.length())
        {
            taskData.restPoints.setLength(logicalIndex + 1);
        }
        // Get sample rest point
        float3& oRestPoint = hRestPoints.inputValue().asFloat3();
        CHECK_MSTATUS_AND_RETURN_IT(status);
        taskData.restPoints[logicalIndex] = MPoint(oRestPoint[0], oRestPoint[1], oRestPoint[2], 1);
        hRestPoints.next();
    }

    return MS::kSuccess;
}

MStatus thuyPointDeformer::setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs) {
    // Extract the geom index from the dirty plug and set the dirty flag so we know that we need to
    // re-read the binding data.
    if (plugBeingDirtied.isElement()) {
        MPlug parent = plugBeingDirtied.array().parent();
        if (parent == aBindData) {
            unsigned int geomIndex = parent.logicalIndex();
            dirty_[geomIndex] = true;
        }
    }
    return MS::kSuccess;
}

MStatus thuyPointDeformer::deform(MDataBlock& data, MItGeometry& itGeo, const MMatrix& localToWorldMatrix, unsigned int geomIndex)
{
    MStatus status;

    if (geomIndex >= taskData_.size()) {
        taskData_.resize(geomIndex + 1);
    }
    TaskData& taskData = taskData_[geomIndex];

    // input///////////////////////////////
    MDataHandle hDriverGeo = data.inputValue(aDriverGeo, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    float env = data.inputValue(envelope, &status).asFloat();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MObject oDriverGeo = hDriverGeo.asMesh();
    if (oDriverGeo.isNull())
    {
        return MS::kSuccess;
    }

    // Only pull bind information from the data block if it is dirty
    if (dirty_[geomIndex] || taskData.boneIDs.size() == 0) {
        dirty_[geomIndex] = false;
        status = getBindInfo(data, geomIndex, taskData);
        if (status == MS::kNotImplemented) {
            // If no bind information is stored yet, don't do anything.
            return MS::kSuccess;
        }
        else if (MFAIL(status)) {
            CHECK_MSTATUS_AND_RETURN_IT(status);
        }
    }

    MFnMesh fnDriver(oDriverGeo, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = fnDriver.getPoints(taskData.driverPoints, MSpace::kWorld);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    itGeo.allPositions(taskData.points);

    double w;
    for (unsigned int i = 0; i < taskData.points.length(); ++i)
    {
        w = weightValue(data, geomIndex, i);
        MPoint skinPt;
        for (size_t j = 0; j < taskData.weights[i].length(); j++)
        {
            int id = taskData.boneIDs[i][j];
            skinPt += taskData.weights[i][j] * (taskData.driverPoints[id] - taskData.restPoints[id]);
        }

        taskData.points[i] += skinPt * env * w;
    }

    status = itGeo.setAllPositions(taskData.points);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    return status;
}

class thuyWrapCmd : public MPxCommand
{
public:
    thuyWrapCmd();
    virtual MStatus doIt(const MArgList&);
    virtual MStatus undoIt();
    virtual MStatus redoIt();
    virtual bool isUndoable() const;
    static void* creator();
    static MSyntax newSyntax();

    const static char* kName;

    const static char* kNameFlagShort;
    const static char* kNameFlagLong;
    const static char* kMaxInfluenceFlagShort;
    const static char* kMaxInfluenceFlagLong;

private:
    MStatus getCommandArgs(const MArgList& args);
    MStatus getGeoPaths();
    MStatus getLatestWrapNode();
    MStatus calBinding(MDagPath& pathBindingMesh, MDGModifier& dgMod);

    int maxInfluence = 1;

    MString name_;
    MDagPath pathDriver_;
    MDagPathArray pathDriven_;
    MSelectionList selectionList_;
    MDGModifier dgMod_;
    MObject oWrapNode_;
};

const char* thuyWrapCmd::kName = "thuyWrap";
const char* thuyWrapCmd::kNameFlagShort = "-n";
const char* thuyWrapCmd::kNameFlagLong = "-name";
const char* thuyWrapCmd::kMaxInfluenceFlagShort = "-mi";
const char* thuyWrapCmd::kMaxInfluenceFlagLong = "-maxInfluence";

thuyWrapCmd::thuyWrapCmd()
    : name_("thuyWrap#")
{
}

MSyntax thuyWrapCmd::newSyntax()
{
    MSyntax syntax;
    syntax.addFlag(kNameFlagShort, kNameFlagLong, MSyntax::kString);
    syntax.addFlag(kMaxInfluenceFlagShort, kMaxInfluenceFlagLong, MSyntax::kLong);
    syntax.setObjectType(MSyntax::kSelectionList, 0, 255);
    syntax.useSelectionAsDefault(true);
    return syntax;
}

bool IsShapeNode(MDagPath& path)
{
    return path.node().hasFn(MFn::kMesh) ||
        path.node().hasFn(MFn::kNurbsCurve) ||
        path.node().hasFn(MFn::kNurbsSurface);
}

MStatus GetShapeNode(MDagPath& path, bool intermediate = false)
{
    MStatus status;

    if (IsShapeNode(path))
    {
        // Start at the transform so we can honor the intermediate flag.
        path.pop();
    }

    if (path.hasFn(MFn::kTransform))
    {
        unsigned int shapeCount = path.childCount();

        for (unsigned int i = 0; i < shapeCount; ++i)
        {
            status = path.push(path.child(i));
            CHECK_MSTATUS_AND_RETURN_IT(status);
            if (!IsShapeNode(path))
            {
                path.pop();
                continue;
            }

            MFnDagNode fnNode(path, &status);
            CHECK_MSTATUS_AND_RETURN_IT(status);
            if ((!fnNode.isIntermediateObject() && !intermediate) ||
                (fnNode.isIntermediateObject() && intermediate))
            {
                return MS::kSuccess;
            }
            // Go to the next shape
            path.pop();
        }
    }

    // No valid shape node found.
    return MS::kFailure;
}

void* thuyWrapCmd::creator()
{
    return new thuyWrapCmd;
}

bool thuyWrapCmd::isUndoable() const
{
    return true;
}

MStatus thuyWrapCmd::doIt(const MArgList& args)
{
    MStatus status;

    // init info
    status = getCommandArgs(args);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = getGeoPaths();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // Add the cvWrap creation command to the modifier.
    MString command = "deformer -type thuyPointDeformer -n \"" + name_ + "\"";
    for (unsigned int i = 0; i < pathDriven_.length(); ++i)
    {
        MFnDagNode fnDriven(pathDriven_[i]);
        command += " " + fnDriven.partialPathName();
    }
    status = dgMod_.commandToExecute(command);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    return redoIt();
}

MStatus thuyWrapCmd::getGeoPaths()
{
    MStatus status;

    if (selectionList_.isEmpty())
    {
        MGlobal::displayError("Please select driver mesh and driven mesh");
        return MS::kFailure;
    }

    // The driver is selected last
    status = selectionList_.getDagPath(selectionList_.length() - 1, pathDriver_);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = GetShapeNode(pathDriver_);

    // The driver must be a mesh for this specific algorithm.
    if (!pathDriver_.hasFn(MFn::kMesh))
    {
        MGlobal::displayError("thuyWrap driver must be a mesh.");
        return MS::kFailure;
    }

    MItSelectionList iter(selectionList_);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    if (selectionList_.length() == 1)
    {
        MGlobal::displayError("thuyWrap need driven mesh");
        return MS::kFailure;
    }

    pathDriven_.clear();
    // drivenComponents_.clear();
    for (unsigned int i = 0; i < selectionList_.length() - 1; ++i, iter.next())
    {
        MDagPath path;
        MObject component;
        iter.getDagPath(path, component);
        status = GetShapeNode(path);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        pathDriven_.append(path);
        // drivenComponents_.append(component);
    }

    return MS::kSuccess;
}

MStatus thuyWrapCmd::getCommandArgs(const MArgList& args)
{
    MStatus status;

    MArgDatabase argData(syntax(), args);
    argData.getObjects(selectionList_);

    if (argData.isFlagSet(kNameFlagShort))
    {
        name_ = argData.flagArgumentString(kNameFlagShort, 0, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
    }
    if (argData.isFlagSet(kMaxInfluenceFlagShort))
    {
        maxInfluence = argData.flagArgumentInt(kMaxInfluenceFlagShort, 0, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
    }

    return MS::kSuccess;
}

MStatus thuyWrapCmd::redoIt()
{
    MStatus status;

    // init
    status = dgMod_.doIt();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // Reacquire the paths because on referenced geo, a new driven path is created (the ShapeDeformed).
    status = getGeoPaths();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // Get the created wrap deformer node
    status = getLatestWrapNode();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // calculation
    MDGModifier dgMod1;
    status = calBinding(pathDriver_, dgMod1);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = dgMod1.doIt();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // connect driver mesh input wrap deformer
    MFnDagNode fnDriver(pathDriver_);
    MPlug plugDriverMesh = fnDriver.findPlug("worldMesh", false, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = plugDriverMesh.selectAncestorLogicalIndex(0, plugDriverMesh.attribute());
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MPlug plugDriverGeo(oWrapNode_, thuyPointDeformer::aDriverGeo);
    MDGModifier dgMod2;
    dgMod2.connect(plugDriverMesh, plugDriverGeo);
    status = dgMod2.doIt();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // apply wrap

    MFnDependencyNode fnNode(oWrapNode_, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    setResult(fnNode.name());

    return MS::kSuccess;
}

MStatus thuyWrapCmd::undoIt()
{
    MStatus status;
    status = dgMod_.undoIt();
    CHECK_MSTATUS_AND_RETURN_IT(status);
    return MS::kSuccess;
}

MStatus thuyWrapCmd::getLatestWrapNode()
{
    MStatus status;
    MObject oDriven = pathDriven_[0].node();

    // Since we use MDGModifier to execute the deformer command, we can't get
    // the created deformer node, so we need to find it in the deformation chain.
    MItDependencyGraph itDG(oDriven,
        MFn::kGeometryFilt,
        MItDependencyGraph::kUpstream,
        MItDependencyGraph::kDepthFirst,
        MItDependencyGraph::kNodeLevel,
        &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MObject oDeformerNode;
    for (; !itDG.isDone(); itDG.next())
    {
        oDeformerNode = itDG.currentItem();
        MFnDependencyNode fnNode(oDeformerNode, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        if (fnNode.typeId() == thuyPointDeformer::id)
        {
            oWrapNode_ = oDeformerNode;
            return MS::kSuccess;
        }
    }
    return MS::kFailure;
}

MStatus thuyWrapCmd::calBinding(MDagPath& pathBindingMesh, MDGModifier& dgMod)
{
    MStatus status;

    MObject oBindMesh = pathBindingMesh.node();

    MFnMesh fnBindMesh(pathBindingMesh, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MPointArray restPoints;
    fnBindMesh.getPoints(restPoints, MSpace::kObject);

    // int maxInfluence = 1;

    std::vector<Point> points;
    //pointVec points;
    for (int i = 0; i < restPoints.length(); i++)
    {
        Point pt = { restPoints[i].x, restPoints[i].y, restPoints[i].z, i };
        points.push_back(pt);
    }
    KDTree* tree = new KDTree(points);

    MPlug plugRestPoints(oWrapNode_, thuyPointDeformer::aRestPoints);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    for (int i = 0; i < restPoints.length(); i++)
    {
        MFnNumericData fnNumericData;
        // store rest point
        MObject oPointData = fnNumericData.create(MFnNumericData::k3Float, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        status = fnNumericData.setData3Float(restPoints[i].x,
            restPoints[i].y,
            restPoints[i].z);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        MPlug plugRestPointsElement = plugRestPoints.elementByLogicalIndex(i, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        status = dgMod.newPlugValue(plugRestPointsElement, oPointData);
        CHECK_MSTATUS_AND_RETURN_IT(status);
    }

    MPlug plugBindData(oWrapNode_, thuyPointDeformer::aBindData);

    for (unsigned int geomIndex = 0; geomIndex < pathDriven_.length(); ++geomIndex)
    {
        BindData bindData;
        // get plug to each binding geo
        MPlug plugBind = plugBindData.elementByLogicalIndex(geomIndex, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        MPlug plugBoneIDs = plugBind.child(thuyPointDeformer::aBoneIDs, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        MPlug plugBoneWeights = plugBind.child(thuyPointDeformer::aBoneWeights, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        MItGeometry itGeo(pathDriven_[geomIndex], &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        int geoCount = itGeo.count();

        status = itGeo.allPositions(bindData.inputPoints, MSpace::kWorld);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        bindData.weights.resize(bindData.inputPoints.length());
        bindData.boneIDs.resize(bindData.inputPoints.length());
        
        for (size_t i = 0; i < bindData.inputPoints.length(); ++i)
        {
            float weightSum = 0.0f;

            //point_t pt = { bindData.inputPoints[i].x, bindData.inputPoints[i].y, bindData.inputPoints[i].z };
            //auto clsPts = tree.nearest_indices(pt, maxInfluence);

            Point pt = { bindData.inputPoints[i].x, bindData.inputPoints[i].y, bindData.inputPoints[i].z,-1 };
            std::vector<int> clsPts = tree->findKNearest(pt, maxInfluence); 

            for (size_t j = 0; j < clsPts.size(); ++j)
            {
                float distance = (bindData.inputPoints[i] - restPoints[clsPts[j]]).length();
                // Compute the weight as the inverse of the distance
                float _w = 1.0f / (distance + 1e-5f); // Adding a small value to avoid division by zero
                bindData.weights[i].append(_w);
                bindData.boneIDs[i].append(clsPts[j]);
                weightSum += _w;
            }

            // Normalize the weights so they sum to 1
            for (size_t j = 0; j < bindData.weights[i].length(); ++j)
            {
                bindData.weights[i][j] /= weightSum;
            }
        }

        MGlobal::displayInfo(MString("weight") + bindData.weights.size());
        MGlobal::displayInfo(MString("weightLength") + bindData.weights[0].length());

        // store data in datablock
        for (int i = 0; !itGeo.isDone(); itGeo.next(), ++i)
        {
            int logicalIndex = itGeo.index();

            // Store triangle vertices
            MFnNumericData fnNumericData;

            // Store bone id
            MFnIntArrayData fnIntData;
            MObject oIntData = fnIntData.create(bindData.boneIDs[i], &status);
            CHECK_MSTATUS_AND_RETURN_IT(status);
            MPlug plugBoneIDsElement = plugBoneIDs.elementByLogicalIndex(logicalIndex, &status);
            CHECK_MSTATUS_AND_RETURN_IT(status);
            status = dgMod.newPlugValue(plugBoneIDsElement, oIntData);
            CHECK_MSTATUS_AND_RETURN_IT(status);

            // Store weights
            MFnDoubleArrayData fnDoubleData;
            MObject oDoubleData = fnDoubleData.create(bindData.weights[i], &status);
            assert(bindData.weights[i].length() > 0);
            CHECK_MSTATUS_AND_RETURN_IT(status);
            MPlug plugSampleWeightsElement = plugBoneWeights.elementByLogicalIndex(logicalIndex, &status);
            CHECK_MSTATUS_AND_RETURN_IT(status);
            status = dgMod.newPlugValue(plugSampleWeightsElement, oDoubleData);
            CHECK_MSTATUS_AND_RETURN_IT(status);

        }

    }

    return MS::kSuccess;
}

#pragma endregion

// ---------------------------------------

MStatus initializePlugin(MObject obj)
{
    MStatus status;
    // Init the vendor string etc...
    MFnPlugin plugin(obj, "thuy.nguyen@virtuosgames.com", "0.0.1", "Any");

    // status = plugin.registerCommand(thuyScatterPoint::cmdName,
    // 	thuyScatterPoint::creator,
    // 	thuyScatterPoint::newSyntax);
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    // status = plugin.registerCommand(thuyGetPointAtUV::cmdName,
    // 	thuyGetPointAtUV::creator,
    // 	thuyGetPointAtUV::newSyntax);
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    // status = plugin.registerNode("MeshSampleNode",
    // 	MeshSampleNode::id,
    // 	MeshSampleNode::creator,
    // 	MeshSampleNode::initialize);
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.registerNode(
        "thuyPointDeformer",
        thuyPointDeformer::id,
        &thuyPointDeformer::creator,
        &thuyPointDeformer::initialize,
        MPxNode::kDeformerNode);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.registerCommand(thuyWrapCmd::kName,
        thuyWrapCmd::creator,
        thuyWrapCmd::newSyntax);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // status = plugin.registerNode(
    //     "RBFDeformerNode" ,
    //     RBFDeformerNode::id ,
    //     &RBFDeformerNode::creator ,
    //     &RBFDeformerNode::initialize ,
    //     MPxNode::kDeformerNode
    //     );
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);

    // status = plugin.deregisterCommand(thuyScatterPoint::cmdName);
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    // status = plugin.deregisterCommand(thuyGetPointAtUV::cmdName);
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    // status = plugin.deregisterNode(MeshSampleNode::id);
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.deregisterNode(thuyPointDeformer::id);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.deregisterCommand(thuyWrapCmd::kName);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // status = plugin.deregisterNode(RBFDeformerNode::id);
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    return MS::kSuccess;
}