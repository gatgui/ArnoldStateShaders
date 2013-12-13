#include "agState.h"

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
   NULL
};

enum FloatState
{
   u = 0,
   v,
   bu,
   bv,
   sx,
   sy,
   we,
   Rl,
   dudx,
   dudy,
   dvdx,
   dvdy,
   time,
   area,
   frame,
   fps,
   FloatStateMax
};

struct FloatStateData
{
   float frame;
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
   case u:
      sg->out.FLT = sg->u;
      break;
   case v:
      sg->out.FLT = sg->v;
      break;
   case bu:
      sg->out.FLT = sg->bu;
      break;
   case bv:
      sg->out.FLT = sg->bv;
      break;
   case sx:
      sg->out.FLT = sg->sx;
      break;
   case sy:
      sg->out.FLT = sg->sy;
      break;
   case we:
      sg->out.FLT = sg->we;
      break;
   case Rl:
      sg->out.FLT = sg->Rl;
      break;
   case dudx:
      sg->out.FLT = sg->dudx;
      break;
   case dudy:
      sg->out.FLT = sg->dudy;
      break;
   case dvdx:
      sg->out.FLT = sg->dvdx;
      break;
   case dvdy:
      sg->out.FLT = sg->dvdy;
      break;
   case time:
      sg->out.FLT = sg->time;
      break;
   case area:
      sg->out.FLT = sg->area;
      break;
   case frame:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->frame;
      }
      break;
   case fps:
      {
         FloatStateData *data = (FloatStateData*) AiNodeGetLocalData(node);
         sg->out.FLT = data->fps;
      }
      break;
   default:
      sg->out.FLT = 0.0f;
   }
}
