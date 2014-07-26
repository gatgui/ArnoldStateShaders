#include "agState.h"

AI_SHADER_NODE_EXPORT_METHODS(agNodeStateMtd);

enum NodeStateParams
{
   p_state = 0,
   p_index
};

static const char* NodeStateNames[] =
{
   "current_light",
   "current_shader",
   "procedural",
   "object",
   "light",
   NULL
};

enum NodeState
{
   NS_Lp = 0,
   NS_shader,
   NS_proc,
   NS_Op,
   NS_Lpn
};

node_parameters
{
   AiParameterEnum("state", NS_Lp, NodeStateNames);
   AiParameterInt("index", 0);
   
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
   NodeState state = (NodeState) AiShaderEvalParamInt(p_state);
   switch (state)
   {
   case NS_Lp:
      sg->out.PTR = sg->Lp;
      break;
   case NS_shader:
      sg->out.PTR = sg->shader;
      break;
   case NS_proc:
      sg->out.PTR = sg->proc;
      break;
   case NS_Op:
      sg->out.PTR = sg->Op;
      break;
   case NS_Lpn:
      {
         int idx = AiShaderEvalParamInt(p_index);
         if (idx >= 0 && idx < sg->nlights)
         {
            sg->out.PTR = sg->lights[idx];
         }
         else
         {
            sg->out.PTR = NULL;
         }
      }
      break;
   default:
      sg->out.PTR = NULL;
   }
}
