#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

int main(int argc, char **argv)
{
  NOB_GO_REBUILD_URSELF(argc, argv);
  char *nob = shift(argv, argc);
  bool run = false;
  bool release = false;

  while (argc > 0) {
    char *arg = shift(argv, argc);
    if (strcmp("run", arg) == 0) run = true;
    if (strcmp("release", arg) == 0) release = true;
  }


  Cmd cmd = {0};

  if (release) {
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    cmd_append(&cmd,"-O3");
    nob_cc_output(&cmd, "rayout");
    nob_cc_inputs(&cmd, "main.c", "rayout.c");

    cmd_append(&cmd, "-I./raylib-5.5_macos/include");
    cmd_append(&cmd, "-L./raylib-5.5_macos/lib", "-lraylib");
    cmd_append(&cmd, "-Wl,-rpath,./raylib-5.5_macos/lib");
    if (!cmd_run(&cmd)) return 1;
  } else {
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    cmd_append(&cmd,"-dynamiclib", "-fPIC", "-ggdb");
    nob_cc_output(&cmd, "librayout.dylib");
    nob_cc_inputs(&cmd, "rayout.c");
    cmd_append(&cmd, "-I./raylib-5.5_macos/include");
    cmd_append(&cmd, "-L./raylib-5.5_macos/lib", "-lraylib");
    if (!cmd_run(&cmd)) return 1;
    
    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    cmd_append(&cmd,"-ggdb");
    nob_cc_output(&cmd, "rayout");
    nob_cc_inputs(&cmd, "main.c");
    cmd_append(&cmd, "-I./raylib-5.5_macos/include");
    cmd_append(&cmd, "-L./raylib-5.5_macos/lib", "-lraylib");
    cmd_append(&cmd, "-Wl,-rpath,./raylib-5.5_macos/lib");
    if (!cmd_run(&cmd)) return 1;
  }

  if (run) {
    cmd_append(&cmd, "./rayout");
    if (!cmd_run(&cmd)) return 1;
  }

  return 0;
}
