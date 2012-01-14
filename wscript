import os
import Utils

srcdir = "."
blddir = "build"
VERSION = "0.1.0"

def listdirs(folder):
  list = []
  for root, folders, files in os.walk(folder):
    for folder in folders:
      list.append(os.path.join(root, folder))
  
  return list

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.recurse('chromium')

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.recurse('chromium')

def build(bld):
  bld.recurse('chromium')
  
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.cxxflags = ["-Wall"]
  obj.includes = '. chromium'
  obj.add_objects = 'chromium'
  obj.source = "src/cld.cc"
  obj.target = "cld"

def test(t):
  Utils.exec_command('make test')