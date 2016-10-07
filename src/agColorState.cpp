#include "agState.h"

AI_SHADER_NODE_EXPORT_METHODS(agColorStateMtd);

enum ColorStateParams
{
   p_state = 0
};

static const char* ColorStateNames[] =
{
   "light_intensity",
   "unoccluded_light_intensity",
   "shadow_occlusion",
   "volume_input_color",
   "volume_output_radiance",
   "output_opacity",
   NULL
};

enum ColorState
{
   CS_Li = 0,
   CS_Liu,
   CS_Lo,
   CS_Ci,
   CS_Vo,
   CS_out_opacity
};

namespace SSTR
{
   extern AtString state;
   extern AtString linkable;
}

node_parameters
{
   AiParameterEnum(SSTR::state, CS_Li, ColorStateNames);
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
   ColorState state = (ColorState) *((int*) AiNodeGetLocalData(node));
   
   switch (state)
   {
   case CS_Li:
      sg->out.RGB = sg->Li;
      break;
   case CS_Liu:
      sg->out.RGB = sg->Liu;
      break;
   case CS_Lo:
      sg->out.RGB = sg->Lo;
      break;
   case CS_Ci:
      sg->out.RGB = (sg->sc == AI_CONTEXT_VOLUME ? sg->Ci : AI_RGB_BLACK);
      break;
   case CS_Vo:
      sg->out.RGB = (sg->sc == AI_CONTEXT_VOLUME ? sg->Vo : AI_RGB_BLACK);
      break;
   case CS_out_opacity:
      sg->out.RGB = sg->out_opacity;
      break;
   default:
      sg->out.RGB = AI_RGB_BLACK;
   }
}
