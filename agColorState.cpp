#include "agState.h"

AI_SHADER_NODE_EXPORT_METHODS(agColorStateMtd);

enum ColorStateParams
{
   p_state = 0
};

static const char* ColorStateNames[] =
{
   NULL
};

enum ColorState
{
   ColorStateMax
};

node_parameters
{
   AiParameterEnum("state", 0, ColorStateNames);
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
   sg->out.RGB = AI_RGB_BLACK;
}
