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

#include <ai.h>
#include <cstring>

extern AtNodeMethods* agVectorStateMtd;
extern AtNodeMethods* agColorStateMtd;
extern AtNodeMethods* agIntStateMtd;
extern AtNodeMethods* agFloatStateMtd;
extern AtNodeMethods* agMatrixStateMtd;
extern AtNodeMethods* agNodeStateMtd;

namespace SSTR
{
   AtString state("state");
   AtString linkable("linkable");
   AtString frame("frame");
   AtString motion_start_frame("motion_start_frame");
   AtString motion_end_frame("motion_end_frame");
   AtString relative_motion_frame("relative_motion_frame");
   AtString shutter_start("shutter_start");
   AtString shutter_end("shutter_end");
   AtString fps("fps");
   AtString index("index");
}

node_loader
{
   if (i == 0)
   {
      node->name = PREFIX "state_v";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_VECTOR;
      node->methods = agVectorStateMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else if (i == 1)
   {
      node->name = PREFIX "state_f";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_FLOAT;
      node->methods = agFloatStateMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else if (i == 2)
   {
      node->name = PREFIX "state_c3";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_RGB;
      node->methods = agColorStateMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else if (i == 3)
   {
      node->name = PREFIX "state_i";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_INT;
      node->methods = agIntStateMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else if (i == 4)
   {
      node->name = PREFIX "state_m";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_MATRIX;
      node->methods = agMatrixStateMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else if (i == 5)
   {
      node->name = PREFIX "state_n";
      node->node_type = AI_NODE_SHADER;
      node->output_type = AI_TYPE_NODE;
      node->methods = agNodeStateMtd;
      strcpy(node->version, AI_VERSION);
      return true;
   }
   else
   {
      return false;
   }
}
