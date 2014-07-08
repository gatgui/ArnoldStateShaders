import sys
import glob
import excons
from excons.tools import arnold

env = excons.MakeBaseEnv()

prjs = [
  {"name": "agState",
   "type": "dynamicmodule",
   "ext": arnold.PluginExt(),
   "srcs": glob.glob("src/*.cpp"),
   "install": {"": "src/agState.mtd"},
   "custom": [arnold.Require]
  }
]

excons.DeclareTargets(env, prjs)
