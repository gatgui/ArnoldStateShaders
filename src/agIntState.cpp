#include "agState.h"

AI_SHADER_NODE_EXPORT_METHODS(agIntStateMtd);

enum IntStateParams
{
   p_state = 0
};

static const char* IntStateNames[] =
{
   "raster_x",
   "raster_y",
   "active_lights_count",
   "reflection_depth",
   "refraction_depth",
   "diffuse_depth",
   "glossy_depth",
   "ray_type",
   "ray_depth",
   NULL
};

enum IntState
{
   IS_x = 0,
   IS_y,
   IS_nlights,
   IS_refl_depth,
   IS_refr_depth,
   IS_diff_depth,
   IS_gloss_depth,
   IS_Rt,
   IS_Rr
};

namespace SSTR
{
   extern AtString state;
   extern AtString linkable;
}

node_parameters
{
   AiParameterEnum(SSTR::state, 0, IntStateNames);
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
   IntState state = (IntState) *((int*) AiNodeGetLocalData(node));

   switch (state)
   {
   case IS_x:
      sg->out.INT = sg->x;
      break;
   case IS_y:
      sg->out.INT = sg->y;
      break;
   case IS_nlights:
      sg->out.INT = sg->nlights;
      break;
   case IS_refl_depth:
      sg->out.INT = sg->Rr_refl;
      break;
   case IS_refr_depth:
      sg->out.INT = sg->Rr_refr;
      break;
   case IS_diff_depth:
      sg->out.INT = sg->Rr_diff;
      break;
   case IS_gloss_depth:
      sg->out.INT = sg->Rr_gloss;
      break;
   case IS_Rt:
      sg->out.INT = sg->Rt;
      break;
   case IS_Rr:
      sg->out.INT = sg->Rr;
      break;
   default:
      sg->out.INT = 0;
   }
}
