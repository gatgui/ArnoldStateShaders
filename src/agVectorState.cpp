#include "agState.h"

AI_SHADER_NODE_EXPORT_METHODS(agVectorStateMtd);

enum VectorStateParams
{
   p_state = 0
};

static const char* VectorStateNames[] =
{
   "world_position",
   "object_position",
   "shading_normal",
   "shading_normal_faceforward",
   "geometric_normal",
   "geometric_normal_faceforward",
   "smooth_normal",
   "ray_origin",
   "ray_direction",
   "dPdx",
   "dPdy",
   "dPdu",
   "dPdv",
   "dDdx",
   "dDdy",
   "dNdx",
   "dNdy",
   "incident_light_direction",
   NULL
};

enum VectorState
{
   VS_P = 0,
   VS_Po,
   VS_N,
   VS_Nf,
   VS_Ng,
   VS_Ngf,
   VS_Ns,
   VS_Ro,
   VS_Rd,
   VS_dPdx,
   VS_dPdy,
   VS_dPdu,
   VS_dPdv,
   VS_dDdx,
   VS_dDdy,
   VS_dNdx,
   VS_dNdy,
   VS_Ld
};

namespace SSTR
{
   extern AtString state;
   extern AtString linkable;
}

node_parameters
{
   AiParameterEnum(SSTR::state, 0, VectorStateNames);
   
   AiMetaDataSetBool(mds, SSTR::state, SSTR::linkable, false);
}

node_initialize
{
   AiNodeSetLocalData(node, AiMalloc(sizeof(int)));
}

node_update
{
   int *data = (int*) AiNodeGetLocalData(node);
   *data = AiNodeGetInt(node, SSTR::state);
}

node_finish
{
   AiFree(AiNodeGetLocalData(node));
}

shader_evaluate
{
   VectorState state = (VectorState) *((int*) AiNodeGetLocalData(node));

   switch (state)
   {
   case VS_P:
      sg->out.VEC = sg->P;
      break;
   case VS_Po:
      sg->out.VEC = sg->Po;
      break;
   case VS_N:
      sg->out.VEC = sg->N;
      break;
   case VS_Nf:
      sg->out.VEC = sg->Nf;
      break;
   case VS_Ng:
      sg->out.VEC = sg->Ng;
      break;
   case VS_Ngf:
      sg->out.VEC = sg->Ngf;
      break;
   case VS_Ns:
      sg->out.VEC = sg->Ns;
      break;
   case VS_Ro:
      sg->out.VEC = sg->Ro;
      break;
   case VS_Rd:
      sg->out.VEC = sg->Rd;
      break;
   case VS_dPdx:
      sg->out.VEC = sg->dPdx;
      break;
   case VS_dPdy:
      sg->out.VEC = sg->dPdy;
      break;
   case VS_dPdu:
      sg->out.VEC = sg->dPdu;
      break;
   case VS_dPdv:
      sg->out.VEC = sg->dPdv;
      break;
   case VS_dDdx:
      sg->out.VEC = sg->dDdx;
      break;
   case VS_dDdy:
      sg->out.VEC = sg->dDdy;
      break;
   case VS_dNdx:
      sg->out.VEC = sg->dNdx;
      break;
   case VS_dNdy:
      sg->out.VEC = sg->dNdy;
      break;
   case VS_Ld:
      sg->out.VEC = sg->Ld;
      break;
   default:
      sg->out.VEC = AI_V3_ZERO;
   }
}
