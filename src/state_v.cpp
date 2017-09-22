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

AI_SHADER_NODE_EXPORT_METHODS(StateVMtd);

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
   "dNdx",
   "dNdy",
   NULL
};

enum VectorState
{
   VS_P = 0,
   VS_Po,
   VS_N,
   VS_Nf,
   VS_Ng,
   VS_Ngf,
   VS_Ns,
   VS_Ro,
   VS_Rd,
   VS_dPdx,
   VS_dPdy,
   VS_dPdu,
   VS_dPdv,
   VS_dDdx,
   VS_dDdy,
   VS_dNdx,
   VS_dNdy
};

namespace SSTR
{
   extern AtString state;
   extern AtString linkable;
}

node_parameters
{
   AiParameterEnum(SSTR::state, 0, VectorStateNames);
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
   VectorState state = (VectorState) *((int*) AiNodeGetLocalData(node));

   switch (state)
   {
   case VS_P:
      sg->out.VEC() = sg->P;
      break;
   case VS_Po:
      sg->out.VEC() = sg->Po;
      break;
   case VS_N:
      sg->out.VEC() = sg->N;
      break;
   case VS_Nf:
      sg->out.VEC() = sg->Nf;
      break;
   case VS_Ng:
      sg->out.VEC() = sg->Ng;
      break;
   case VS_Ngf:
      sg->out.VEC() = sg->Ngf;
      break;
   case VS_Ns:
      sg->out.VEC() = sg->Ns;
      break;
   case VS_Ro:
      sg->out.VEC() = sg->Ro;
      break;
   case VS_Rd:
      sg->out.VEC() = sg->Rd;
      break;
   case VS_dPdx:
      sg->out.VEC() = sg->dPdx;
      break;
   case VS_dPdy:
      sg->out.VEC() = sg->dPdy;
      break;
   case VS_dPdu:
      sg->out.VEC() = sg->dPdu;
      break;
   case VS_dPdv:
      sg->out.VEC() = sg->dPdv;
      break;
   case VS_dDdx:
      sg->out.VEC() = sg->dDdx;
      break;
   case VS_dDdy:
      sg->out.VEC() = sg->dDdy;
      break;
   case VS_dNdx:
      sg->out.VEC() = sg->dNdx;
      break;
   case VS_dNdy:
      sg->out.VEC() = sg->dNdy;
      break;
   default:
      sg->out.VEC() = AI_V3_ZERO;
   }
}
