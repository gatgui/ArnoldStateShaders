#include "agState.h"
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
   FS_shutter_close_time
};

struct FloatStateData
{
   float frame; // The frame being rendered
   float motion_start_frame; // First motion sample frame
   float motion_end_frame; // Last motion sample frame
   float shutter_open_frame; // Shutter open frame
   float shutter_close_frame; // Shutter close frame
   float shutter_open_time;
   float shutter_close_time;
   float fps;
};

static bool GetNodeConstantFloat(AtNode *node, const char *name, float &val, const char *msg=NULL)
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
            AiMsgWarning("[float_state] \"%s\" parameter on node \"%s\" should be a float or an integer (%s)", name, AiNodeGetName(node), (msg ? msg : ""));
            return false;
         }
         return true;
      }
      else
      {
         AiMsgWarning("[float_state] \"%s\" parameter on node \"%s\" must be a constant (%s)", name, AiNodeGetName(node), (msg ? msg : ""));
         return false;
      }
   }
   else
   {
      AiMsgWarning("[float_state] \"%s\" parameter not defined on node \"%s\" (%s)", name, AiNodeGetName(node), (msg ? msg : ""));
      return false;
   }
}

node_parameters
{
   AiParameterEnum("state", 0, FloatStateNames);
}

node_initialize
{
   FloatStateData *data = (FloatStateData*) AiMalloc(sizeof(FloatStateData));
   AiNodeSetLocalData(node, data);
}

node_update
{
   FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
   
   data->frame = 0.0f;
   data->motion_start_frame = 0.0f;
   data->motion_end_frame = 0.0f;
   data->shutter_open_frame = 0.0f;
   data->shutter_close_frame = 0.0f;
   data->shutter_open_time = 0.0f;
   data->shutter_close_time = 0.0f;
   data->fps = 24.0f;

   AtNode *opts = AiUniverseGetOptions();
   if (opts)
   {
      GetNodeConstantFloat(opts, "frame", data->frame, "Defaults to 0");
      
      data->motion_start_frame = data->frame;
      GetNodeConstantFloat(opts, "motion_start_frame", data->motion_start_frame, "Defaults to 'frame'");
      
      data->motion_end_frame = data->motion_start_frame;
      GetNodeConstantFloat(opts, "motion_end_frame", data->motion_start_frame, "Defaults to 'motion_start_frame'");
      
      AtNode *camera = AiUniverseGetCamera();
      if (camera)
      {
         data->shutter_open_time = AiNodeGetFlt(camera, "shutter_start");
         data->shutter_close_time = AiNodeGetFlt(camera, "shutter_end");
         
         float sample_length = data->motion_end_frame - data->motion_start_frame;
         
         if (sample_length <= 0.0f)
         {
            data->shutter_open_frame = data->motion_start_frame;
            data->shutter_close_frame = data->motion_start_frame;
         }
         else
         {
            data->shutter_open_frame = data->motion_start_frame + data->shutter_open_time * sample_length;
            data->shutter_close_frame = data->motion_start_frame + data->shutter_close_time * sample_length;
         }
      }
      else
      {
         AiMsgWarning("[float_state] No render camera set. Default shutter_open_frame and shuffer_close_frame to 'motion_start_frame'");
         data->shutter_open_frame = data->motion_start_frame;
         data->shutter_close_frame = data->shutter_open_frame;
      }
      
      GetNodeConstantFloat(opts, "fps", data->fps, "Defaults to 24");
   }
   else
   {
      AiMsgWarning("[float_state] No options node. \"frame\", \"fps\", \"motion_start_frame\" and \"motion_end_frame\" default to 0, 24, 0 and 0 respectivelly");
   }
}

node_finish
{
   FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
   AiFree(data);
}

shader_evaluate
{
   int which = AiShaderEvalParamInt(p_state);
   switch (which)
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
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->frame;
      }
      break;
   case FS_fps:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->fps;
      }
      break;
   case FS_shutter_open_time:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->shutter_open_time;
      }
      break;
   case FS_shutter_close_time:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->shutter_close_time;
      }
      break;
   case FS_shutter_open_frame:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->shutter_open_frame;
      }
      break;
   case FS_shutter_close_frame:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->shutter_close_frame;
      }
      break;
   case FS_sample_frame:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->motion_start_frame + sg->time * (data->motion_end_frame - data->motion_start_frame);
      }
      break;
   default:
      sg->out.FLT = 0.0f;
   }
}
