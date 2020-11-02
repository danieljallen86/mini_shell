#!/bin/bash
gcc --std=gnu99 -o smallsh smallsh.c get_args.c built_ins.c run_process.c signals.c 
