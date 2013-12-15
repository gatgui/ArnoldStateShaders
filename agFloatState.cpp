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
   FS_sample_frame
};

struct FloatStateData
{
   float frame;
   float motion_start_frame;
   float motion_end_frame;
   float fps;
};

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
   
   data->frame = 1.0f;
   data->motion_start_frame = 1.0f;
   data->motion_end_frame = 1.0f;
   data->fps = 24.0f;

   AtNode *opts = AiUniverseGetOptions();
   if (opts)
   {
      const AtUserParamEntry *pe;

      pe = AiNodeLookUpUserParameter(opts, "frame");
      if (pe != 0)
      {
         int type = AiUserParamGetType(pe);
         int cat = AiUserParamGetCategory(pe);
         if (cat == AI_USERDEF_CONSTANT)
         {
            switch (type)
            {
            case AI_TYPE_BYTE:
               data->frame = float(AiNodeGetByte(opts, "frame"));
               break;
            case AI_TYPE_INT:
               data->frame = float(AiNodeGetInt(opts, "frame"));
               break;
            case AI_TYPE_UINT:
               data->frame = float(AiNodeGetUInt(opts, "frame"));
               break;
            case AI_TYPE_FLOAT:
               data->frame = AiNodeGetFlt(opts, "frame");
               break;
            default:
               AiMsgWarning("[float_state] Found \"frame\" attribute on options node, but invalid type. Default value to 1.0");
            }
         }
         else
         {
            AiMsgWarning("[float_state] Found \"frame\" attribute on options node, but not a constant. Default value to 1.0");
         }
      }
      else
      {
         AiMsgWarning("[float_state] Cannot find \"frame\" attribute in options node. Defaults to 1.0");
      }
      
      pe = AiNodeLookUpUserParameter(opts, "motion_start_frame");
      if (pe != 0)
      {
         int type = AiUserParamGetType(pe);
         int cat = AiUserParamGetCategory(pe);
         if (cat == AI_USERDEF_CONSTANT)
         {
            switch (type)
            {
            case AI_TYPE_BYTE:
               data->motion_start_frame = float(AiNodeGetByte(opts, "motion_start_frame"));
               break;
            case AI_TYPE_INT:
               data->motion_start_frame = float(AiNodeGetInt(opts, "motion_start_frame"));
               break;
            case AI_TYPE_UINT:
               data->motion_start_frame = float(AiNodeGetUInt(opts, "motion_start_frame"));
               break;
            case AI_TYPE_FLOAT:
               data->motion_start_frame = AiNodeGetFlt(opts, "motion_start_frame");
               break;
            default:
               AiMsgWarning("[float_state] Found \"motion_start_frame\" attribute on options node, but invalid type. Default value to 'frame'");
               data->motion_start_frame = data->frame;
            }
         }
         else
         {
            AiMsgWarning("[float_state] Found \"motion_start_frame\" attribute on options node, but not a constant. Default value to 'frame'");
            data->motion_start_frame = data->frame;
         }
      }
      else
      {
         AiMsgWarning("[float_state] Cannot find \"motion_start_frame\" attribute in options node. Defaults to 'frame'");
         data->motion_start_frame = data->frame;
      }

      pe = AiNodeLookUpUserParameter(opts, "motion_end_frame");
      if (pe != 0)
      {
         int type = AiUserParamGetType(pe);
         int cat = AiUserParamGetCategory(pe);
         if (cat == AI_USERDEF_CONSTANT)
         {
            switch (type)
            {
            case AI_TYPE_BYTE:
               data->motion_end_frame = float(AiNodeGetByte(opts, "motion_end_frame"));
               break;
            case AI_TYPE_INT:
               data->motion_end_frame = float(AiNodeGetInt(opts, "motion_end_frame"));
               break;
            case AI_TYPE_UINT:
               data->motion_end_frame = float(AiNodeGetUInt(opts, "motion_end_frame"));
               break;
            case AI_TYPE_FLOAT:
               data->motion_end_frame = AiNodeGetFlt(opts, "motion_end_frame");
               break;
            default:
               AiMsgWarning("[float_state] Found \"motion_end_frame\" attribute on options node, but invalid type. Default value to 'motion_start_frame'");
               data->motion_end_frame = data->motion_start_frame;
            }
         }
         else
         {
            AiMsgWarning("[float_state] Found \"motion_end_frame\" attribute on options node, but not a constant. Default value to 'motion_start_frame'");
            data->motion_end_frame = data->motion_start_frame;
         }
      }
      else
      {
         AiMsgWarning("[float_state] Cannot find \"motion_end_frame\" attribute in options node. Defaults to 'motion_start_frame'");
         data->motion_end_frame = data->motion_start_frame;
      }
      
      pe = AiNodeLookUpUserParameter(opts, "fps");
      if (pe != 0)
      {
         int type = AiUserParamGetType(pe);
         int cat = AiUserParamGetCategory(pe);
         if (cat == AI_USERDEF_CONSTANT)
         {
            switch (type)
            {
            case AI_TYPE_BYTE:
               data->fps = float(AiNodeGetByte(opts, "fps"));
               break;
            case AI_TYPE_INT:
               data->fps = float(AiNodeGetInt(opts, "fps"));
               break;
            case AI_TYPE_UINT:
               data->fps = float(AiNodeGetUInt(opts, "fps"));
               break;
            case AI_TYPE_FLOAT:
               data->fps = AiNodeGetFlt(opts, "fps");
               break;
            default:
               AiMsgWarning("[float_state] Found \"fps\" attribute on options node, but invalid type. Default value to 24.0");
            }
         }
         else
         {
            AiMsgWarning("[float_state] Found \"fps\" attribute on options node, but not a constant. Default value to 24.0");
         }
      }
      else
      {
         AiMsgWarning("[float_state] Cannot find \"fps\" attribute in options node. Defaults to 24.0");
      }
      
   }
   else
   {
      AiMsgWarning("[float_state] Cannot find \"frame\" or \"fps\" in options node. Defaults to 1.0 and 24.0 respectivelly");
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
