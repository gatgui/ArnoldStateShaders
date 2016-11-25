/*
MIT License

Copyright (c) 2016 Gaetan Guidet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "common.h"

AI_SHADER_NODE_EXPORT_METHODS(StateNMtd);

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
