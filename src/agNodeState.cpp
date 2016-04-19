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

namespace SSTR
{
   extern AtString state;
   extern AtString linkable;
   extern AtString index;
}

node_parameters
{
   AiParameterEnum(SSTR::state, NS_Lp, NodeStateNames);
   AiParameterInt(SSTR::index, 0);
   
   AiMetaDataSetBool(mds, SSTR::state, SSTR::linkable, false);
   AiMetaDataSetBool(mds, SSTR::index, SSTR::linkable, false);
}

struct NodeData
{
   NodeState state;
   int index;
};

node_initialize
{
   AiNodeSetLocalData(node, new NodeData());
}

node_update
{
   NodeData *data = (NodeData*) AiNodeGetLocalData(node);
   data->state = (NodeState) AiNodeGetInt(node, SSTR::state);
   data->index = AiNodeGetInt(node, SSTR::index);
}

node_finish
{
   NodeData *data = (NodeData*) AiNodeGetLocalData(node);
   delete data;
}

shader_evaluate
{
   NodeData *data = (NodeData*) AiNodeGetLocalData(node);
   
   switch (data->state)
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
         if (data->index >= 0 && data->index < sg->nlights)
         {
            sg->out.PTR = sg->lights[data->index];
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
