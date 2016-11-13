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

AI_SHADER_NODE_EXPORT_METHODS(agMatrixStateMtd);

enum  MatrixStateParams
{
   p_state = 0
};

static const char* MatrixStateNames[] =
{
   "object_to_world",
   "world_to_object",
   "camera_to_world",
   "world_to_camera",
   "world_to_screen",
   NULL
};

enum MatrixState
{
   MS_OtoW = 0,
   MS_WtoO,
   MS_CtoW,
   MS_WtoC,
   MS_WtoS
};

namespace SSTR
{
   extern AtString state;
   extern AtString linkable;
}

node_parameters
{
   AiParameterEnum(SSTR::state, 0, MatrixStateNames);
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
   MatrixState state = (MatrixState) *((int*) AiNodeGetLocalData(node));
   
   if (state > MS_WtoO)
   {
      sg->out.pMTX = (AtMatrix*) AiShaderGlobalsQuickAlloc(sg, sizeof(AtMatrix));
   }
   
   switch (state)
   {
   case MS_OtoW:
      sg->out.pMTX = &(sg->M);
      break;
   case MS_WtoO:
      sg->out.pMTX = &(sg->Minv);
      break;
   case MS_CtoW:
      AiCameraToWorldMatrix(AiUniverseGetCamera(), sg->time, *(sg->out.pMTX));
      break;
   case MS_WtoC:
      AiWorldToCameraMatrix(AiUniverseGetCamera(), sg->time, *(sg->out.pMTX));
      break;
   case MS_WtoS:
      AiWorldToScreenMatrix(AiUniverseGetCamera(), sg->time, *(sg->out.pMTX));
      break;
   default:
      AiM4Identity(*(sg->out.pMTX));
   }
}
