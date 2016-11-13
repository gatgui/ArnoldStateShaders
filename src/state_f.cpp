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
#include <iostream>

AI_SHADER_NODE_EXPORT_METHODS(agFloatStateMtd);

enum FloatStateParams
{
   p_state = 0
};

static const char* FloatStateNames[] =
{
   "surface_u",
   "surface_v",
   "barycentric_u",
   "barycentric_v",
   "screen_x",
   "screen_y",
   "sample_weight",
   "ray_length",
   "dudx",
   "dudy",
   "dvdx",
   "dvdy",
   "time",
   "area",
   "frame",
   "fps",
   "sample_frame",
   "shutter_open_frame",
   "shutter_close_frame",
   "shutter_open_time",
   "shutter_close_time",
   "light_distance",
   NULL
};

enum FloatState
{
   FS_u = 0,
   FS_v,
   FS_bu,
   FS_bv,
   FS_sx,
   FS_sy,
   FS_we,
   FS_Rl,
   FS_dudx,
   FS_dudy,
   FS_dvdx,
   FS_dvdy,
   FS_time,
   FS_area,
   FS_frame,
   FS_fps,
   FS_sample_frame,
   FS_shutter_open_frame,
   FS_shutter_close_frame,
   FS_shutter_open_time,
   FS_shutter_close_time,
   FS_Ldist
};

static bool GetNodeConstantFloat(AtNode *node, AtString name, float &val, const char *msg=NULL)
{
   const AtUserParamEntry *pe = AiNodeLookUpUserParameter(node, name);
   if (pe != 0)
   {
      int type = AiUserParamGetType(pe);
      int cat = AiUserParamGetCategory(pe);
      
      if (cat == AI_USERDEF_CONSTANT)
      {
         switch (type)
         {
         case AI_TYPE_BYTE:
            val = float(AiNodeGetByte(node, name));
            break;
         case AI_TYPE_INT:
            val = float(AiNodeGetInt(node, name));
            break;
         case AI_TYPE_UINT:
            val = float(AiNodeGetUInt(node, name));
            break;
         case AI_TYPE_FLOAT:
            val = AiNodeGetFlt(node, name);
            break;
         default:
            AiMsgWarning("[%sstate_f] \"%s\" parameter on node \"%s\" should be a float or an integer (%s)", PREFIX, name.c_str(), AiNodeGetName(node), (msg ? msg : ""));
            return false;
         }
         return true;
      }
      else
      {
         AiMsgWarning("[%sstate_f] \"%s\" parameter on node \"%s\" must be a constant (%s)", PREFIX, name.c_str(), AiNodeGetName(node), (msg ? msg : ""));
         return false;
      }
   }
   else
   {
      AiMsgWarning("[%sstate_f] \"%s\" parameter not defined on node \"%s\" (%s)", PREFIX, name.c_str(), AiNodeGetName(node), (msg ? msg : ""));
      return false;
   }
}

static bool GetNodeConstantBool(AtNode *node, AtString name, bool &val, const char *msg=NULL)
{
   const AtUserParamEntry *pe = AiNodeLookUpUserParameter(node, name);
   if (pe != 0)
   {
      int type = AiUserParamGetType(pe);
      int cat = AiUserParamGetCategory(pe);

      if (cat == AI_USERDEF_CONSTANT)
      {
         switch (type)
         {
         case AI_TYPE_BOOLEAN:
            val = AiNodeGetBool(node, name);
            break;
         case AI_TYPE_BYTE:
            val = (AiNodeGetByte(node, name) != 0);
            break;
         case AI_TYPE_INT:
            val = (AiNodeGetInt(node, name) != 0);
            break;
         case AI_TYPE_UINT:
            val = (AiNodeGetUInt(node, name) != 0);
            break;
         case AI_TYPE_FLOAT:
            val = (AiNodeGetFlt(node, name) != 0.0f);
            break;
         default:
            AiMsgWarning("[%sstate_f] \"%s\" parameter on node \"%s\" should be a bool, an integer or a float (%s)", PREFIX, name.c_str(), AiNodeGetName(node), (msg ? msg : ""));
            return false;
         }
         return true;
      }
      else
      {
         AiMsgWarning("[%sstate_f] \"%s\" parameter on node \"%s\" must be a constant (%s)", PREFIX, name.c_str(), AiNodeGetName(node), (msg ? msg : ""));
         return false;
      }
   }
   else
   {
      AiMsgWarning("[%sstate_f] \"%s\" parameter not defined on node \"%s\" (%s)", PREFIX, name.c_str(), AiNodeGetName(node), (msg ? msg : ""));
      return false;
   }
}

namespace SSTR
{
   extern AtString state;
   extern AtString linkable;
   extern AtString frame;
   extern AtString motion_start_frame;
   extern AtString motion_end_frame;
   extern AtString relative_motion_frame;
   extern AtString shutter_start;
   extern AtString shutter_end;
   extern AtString fps;
}

node_parameters
{
   AiParameterEnum(SSTR::state, 0, FloatStateNames);
}

struct NodeData
{
   int state;
   float frame; // The frame being rendered
   float motionStartFrame; // First motion sample frame
   float motionEndFrame; // Last motion sample frame
   float shutterOpenFrame; // Shutter open frame
   float shutterCloseFrame; // Shutter close frame
   float shutterOpenTime;
   float shutterCloseTime;
   float fps;
   bool relativeMotionFrame;
};

node_initialize
{
   AiNodeSetLocalData(node, new NodeData());
}

node_update
{
   NodeData *data = (NodeData*) AiNodeGetLocalData(node);
   
   data->state = AiNodeGetInt(node, SSTR::state);
   data->frame = 0.0f;
   data->motionStartFrame = 0.0f;
   data->motionEndFrame = 0.0f;
   data->shutterOpenFrame = 0.0f;
   data->shutterCloseFrame = 0.0f;
   data->shutterOpenTime = 0.0f;
   data->shutterCloseTime = 0.0f;
   data->fps = 24.0f;
   data->relativeMotionFrame = false;

   AtNode *opts = AiUniverseGetOptions();
   if (opts)
   {
      GetNodeConstantFloat(opts, SSTR::frame, data->frame, "Defaults to 0");
      
      GetNodeConstantBool(opts, SSTR::relative_motion_frame, data->relativeMotionFrame, "Defaults to false");
      
      data->motionStartFrame = data->frame;
      if (GetNodeConstantFloat(opts, SSTR::motion_start_frame, data->motionStartFrame, "Defaults to 'frame'"))
      {
         if (data->relativeMotionFrame)
         {
            data->motionStartFrame += data->frame;
         }
      }
      
      data->motionEndFrame = data->motionStartFrame;
      if (GetNodeConstantFloat(opts, SSTR::motion_end_frame, data->motionEndFrame, "Defaults to 'motion_start_frame'"))
      {
         if (data->relativeMotionFrame)
         {
            data->motionEndFrame += data->frame;
         }
      }
      
      AtNode *camera = AiUniverseGetCamera();
      if (camera)
      {
         data->shutterOpenTime = AiNodeGetFlt(camera, SSTR::shutter_start);
         data->shutterCloseTime = AiNodeGetFlt(camera, SSTR::shutter_end);
         
         float sampleLength = data->motionEndFrame - data->motionStartFrame;
         
         if (sampleLength <= 0.0f)
         {
            data->shutterOpenFrame = data->motionStartFrame;
            data->shutterCloseFrame = data->motionStartFrame;
         }
         else
         {
            data->shutterOpenFrame = data->motionStartFrame + data->shutterOpenTime * sampleLength;
            data->shutterCloseFrame = data->motionStartFrame + data->shutterCloseTime * sampleLength;
         }
      }
      else
      {
         AiMsgWarning("[%sstate_f] No render camera set. Default shutter_open_frame and shuffer_close_frame to 'motion_start_frame'", PREFIX);
         data->shutterOpenFrame = data->motionStartFrame;
         data->shutterCloseFrame = data->shutterOpenFrame;
      }
      
      GetNodeConstantFloat(opts, SSTR::fps, data->fps, "Defaults to 24");
   }
   else
   {
      AiMsgWarning("[%sstate_f] No options node. \"frame\", \"fps\", \"motion_start_frame\" and \"motion_end_frame\" default to 0, 24, 0 and 0 respectivelly", PREFIX);
   }
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
   case FS_u:
      sg->out.FLT = sg->u;
      break;
   case FS_v:
      sg->out.FLT = sg->v;
      break;
   case FS_bu:
      sg->out.FLT = sg->bu;
      break;
   case FS_bv:
      sg->out.FLT = sg->bv;
      break;
   case FS_sx:
      sg->out.FLT = sg->sx;
      break;
   case FS_sy:
      sg->out.FLT = sg->sy;
      break;
   case FS_we:
      sg->out.FLT = sg->we;
      break;
   case FS_Rl:
      sg->out.FLT = sg->Rl;
      break;
   case FS_dudx:
      sg->out.FLT = sg->dudx;
      break;
   case FS_dudy:
      sg->out.FLT = sg->dudy;
      break;
   case FS_dvdx:
      sg->out.FLT = sg->dvdx;
      break;
   case FS_dvdy:
      sg->out.FLT = sg->dvdy;
      break;
   case FS_time:
      sg->out.FLT = sg->time;
      break;
   case FS_area:
      sg->out.FLT = sg->area;
      break;
   case FS_frame:
      sg->out.FLT = data->frame;
      break;
   case FS_fps:
      sg->out.FLT = data->fps;
      break;
   case FS_shutter_open_time:
      sg->out.FLT = data->shutterOpenTime;
      break;
   case FS_shutter_close_time:
      sg->out.FLT = data->shutterCloseTime;
      break;
   case FS_shutter_open_frame:
      sg->out.FLT = data->shutterOpenFrame;
      break;
   case FS_shutter_close_frame:
      sg->out.FLT = data->shutterCloseFrame;
      break;
   case FS_sample_frame:
      sg->out.FLT = data->motionStartFrame + sg->time * (data->motionEndFrame - data->motionStartFrame);
      break;
   case FS_Ldist:
      sg->out.FLT = sg->Ldist;
      break;
   default:
      sg->out.FLT = 0.0f;
   }
}
