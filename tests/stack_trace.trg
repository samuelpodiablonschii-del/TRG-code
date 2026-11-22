// Stack trace regression test: an error inside inner_task() must show both calls.
define inner_task (
    unknown_stack_trace_probe_command
)
define outer_task (
    inner_task
)
outer_task
say "TRG_STACK_TRACE_PASSED"
