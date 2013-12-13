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
   P = 0,
   Po,
   N,
   Nf,
   Ng,
   Ngf,
   Ns,
   Ro,
   Rd,
   dPdx,
   dPdy,
   dPdu,
   dPdv,
   dDdx,
   dDdy,
#ifdef ARNOLD_4_1_AND_UP
   dNdx,
   dNdy,
#endif
   VectorStateMax
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
   case P:
      sg->out.VEC = sg->P;
      break;
   case Po:
      sg->out.VEC = sg->Po;
      break;
   case N:
      sg->out.VEC = sg->N;
      break;
   case Nf:
      sg->out.VEC = sg->Nf;
      break;
   case Ng:
      sg->out.VEC = sg->Ng;
      break;
   case Ngf:
      sg->out.VEC = sg->Ngf;
      break;
   case Ns:
      sg->out.VEC = sg->Ns;
      break;
   case Ro:
      sg->out.VEC = sg->Ro;
      break;
   case Rd:
      sg->out.VEC = sg->Rd;
      break;
   case dPdx:
      sg->out.VEC = sg->dPdx;
      break;
   case dPdy:
      sg->out.VEC = sg->dPdy;
      break;
   case dPdu:
      sg->out.VEC = sg->dPdu;
      break;
   case dPdv:
      sg->out.VEC = sg->dPdv;
      break;
   case dDdx:
      sg->out.VEC = sg->dDdx;
      break;
   case dDdy:
      sg->out.VEC = sg->dDdy;
      break;
#ifdef ARNOLD_4_1_AND_UP
   case dNdx:
      sg->out.VEC = sg->dNdx;
      break;
   case dNdy:
      sg->out.VEC = sg->dNdy;
      break;
#endif
   default:
      sg->out.VEC = AI_V3_ZERO;
   }
}
