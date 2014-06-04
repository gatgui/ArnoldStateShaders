#include "agState.h"

AI_SHADER_NODE_EXPORT_METHODS(agMatrixStateMtd);

enum  MatrixStateParams
{
   p_state = 0
};

static const char* MatrixStateNames[] =
{
   "object_to_world",
   "world_to_object",
   "camera_to_world",
   "world_to_camera",
   "world_to_screen",
   NULL
};

enum MatrixState
{
   MS_OtoW = 0,
   MS_WtoO,
   MS_CtoW,
   MS_WtoC,
   MS_WtoS
};

node_parameters
{
   AiParameterEnum("state", 0, MatrixStateNames);
   
   AiMetaDataSetBool(mds, "state", "linkable", false);
}

node_initialize
{
}

node_update
{
}

node_finish
{
}

shader_evaluate
{
   MatrixState state = (MatrixState) AiShaderEvalParamInt(p_state);
   
   if (state > MS_WtoO)
   {
      sg->out.pMTX = (AtMatrix*) AiShaderGlobalsQuickAlloc(sg, sizeof(AtMatrix));
   }
   
   switch (state)
   {
   case MS_OtoW:
      sg->out.pMTX = &(sg->M);
      break;
   case MS_WtoO:
      sg->out.pMTX = &(sg->Minv);
      break;
   case MS_CtoW:
      AiCameraToWorldMatrix(AiUniverseGetCamera(), sg->time, *(sg->out.pMTX));
      break;
   case MS_WtoC:
      AiWorldToCameraMatrix(AiUniverseGetCamera(), sg->time, *(sg->out.pMTX));
      break;
   case MS_WtoS:
      AiWorldToScreenMatrix(AiUniverseGetCamera(), sg->time, *(sg->out.pMTX));
      break;
   default:
      AiM4Identity(*(sg->out.pMTX));
   }
}
