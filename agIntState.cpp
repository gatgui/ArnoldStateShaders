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
   x = 0,
   y,
   IntStateMax
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
   case x:
      sg->out.INT = sg->x;
      break;
   case y:
      sg->out.INT = sg->y;
      break;
   default:
      sg->out.INT = 0;
   }
}
