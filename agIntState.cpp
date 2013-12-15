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
   NULL
};

enum IntState
{
   IS_x = 0,
   IS_y
};

node_parameters
{
   AiParameterEnum("state", 0, IntStateNames);
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
   IntState which = (IntState) AiShaderEvalParamInt(p_state);
   switch (which)
   {
   case IS_x:
      sg->out.INT = sg->x;
      break;
   case IS_y:
      sg->out.INT = sg->y;
      break;
   default:
      sg->out.INT = 0;
   }
}
