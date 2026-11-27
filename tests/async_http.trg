// Regression test: HTTP requests run on worker threads and are polled without blocking.
add (network)

word request = ""
word response = ""
word status = pending

http_get_async not-a-valid-url request
loop status = pending (
    http_result request response status
    if status = pending (
        wait 5ms
    )
)

if status = error (
    say "TRG_ASYNC_HTTP_PASSED"
)
else (
    say "ERROR asynchronous HTTP did not reach a completed error state"
)

http_forget request
