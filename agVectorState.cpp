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
#ifdef ARNOLD_4_1_AND_UP
   "dNdx",
   "dNdy",
#endif
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
#ifdef ARNOLD_4_1_AND_UP
   VS_dNdx,
   VS_dNdy,
#endif
};

node_parameters
{
   AiParameterEnum("state", 0, VectorStateNames);
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
   VectorState which = (VectorState) AiShaderEvalParamInt(p_state);
   switch (which)
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
#ifdef ARNOLD_4_1_AND_UP
   case VS_dNdx:
      sg->out.VEC = sg->dNdx;
      break;
   case VS_dNdy:
      sg->out.VEC = sg->dNdy;
      break;
#endif
   default:
      sg->out.VEC = AI_V3_ZERO;
   }
}
