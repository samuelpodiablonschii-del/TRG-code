// Stack trace regression test: an error inside inner() must show both calls.
define inner (
    unknown_stack_trace_probe_command
)
define outer (
    inner
)
outer
say "TRG_STACK_TRACE_PASSED"
