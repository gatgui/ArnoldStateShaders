import sys
import glob
import excons
import excons.config
from excons.tools import arnold


env = excons.MakeBaseEnv()

arniver = arnold.Version(asString=False)
if arniver[0] < 4 or (arniver[0] == 4 and (arniver[1] < 2 or (arniver[1] == 2 and arniver[2] < 12))):
  print("Arnold 4.2.12.0 or above required")
  sys.exit(1)

def toMayaName(name):
  spl = name.split("_")
  return spl[0] + "".join(map(lambda x: x[0].upper() + x[1:], spl[1:]))

prefix = excons.GetArgument("prefix", "gf_")
name = "%sstate" % prefix
statei_maya_name = toMayaName(prefix + "state_i")
statef_maya_name = toMayaName(prefix + "state_f")
statev_maya_name = toMayaName(prefix + "state_v")
statec3_maya_name = toMayaName(prefix + "state_c3")
statem_maya_name = toMayaName(prefix + "state_m")
staten_maya_name = toMayaName(prefix + "state_n")
opts = {"PREFIX": prefix,
        "STATEI_MAYA_NODENAME": statei_maya_name,
        "STATEF_MAYA_NODENAME": statef_maya_name,
        "STATEV_MAYA_NODENAME": statev_maya_name,
        "STATEC3_MAYA_NODENAME": statec3_maya_name,
        "STATEM_MAYA_NODENAME": statem_maya_name,
        "STATEN_MAYA_NODENAME": staten_maya_name}

GenerateMtd = excons.config.AddGenerator(env, "mtd", opts)
GenerateMayaAE = excons.config.AddGenerator(env, "mayaAE", opts)
mtd = GenerateMtd("src/%s.mtd" % name, "src/state.mtd.in")
ae  = GenerateMayaAE("maya/%sTemplate.py" % statei_maya_name, "maya/StateITemplate.py.in")
ae += GenerateMayaAE("maya/%sTemplate.py" % statef_maya_name, "maya/StateFTemplate.py.in")
ae += GenerateMayaAE("maya/%sTemplate.py" % statev_maya_name, "maya/StateVTemplate.py.in")
ae += GenerateMayaAE("maya/%sTemplate.py" % statec3_maya_name, "maya/StateC3Template.py.in")
ae += GenerateMayaAE("maya/%sTemplate.py" % statem_maya_name, "maya/StateMTemplate.py.in")
ae += GenerateMayaAE("maya/%sTemplate.py" % staten_maya_name, "maya/StateNTemplate.py.in")

if sys.platform != "win32":
   env.Append(CPPFLAGS=" -Wno-unused-parameter")
else:
   env.Append(CPPFLAGS=" /wd4100") # unreferenced format parameter

prjs = [
  {"name": name,
   "type": "dynamicmodule",
   "prefix": "arnold",
   "ext": arnold.PluginExt(),
   "defs": ["PREFIX=\\\"%s\\\"" % prefix],
   "srcs": glob.glob("src/*.cpp"),
   "install": {"arnold": mtd,
               "maya/ae": ae},
   "custom": [arnold.Require]
  }
]

targets = excons.DeclareTargets(env, prjs)

targets[name].extend(mtd)
targets["maya"] = ae

excons.EcosystemDist(env, "state.env", {name: "", "maya": "/maya/ae"}, name=name)

Default([name])
