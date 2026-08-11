# Testing Strategy Guide

Timer420 utilizes a decoupled core architecture allowing 100% unit test coverage of timing logic without waiting for real time.

## FakeClock Mock

The test framework injects a `FakeClock` mock into `TimerEngine`.

```c
Clock fake = fake_clock_create(1000);
TimerEngine engine;
timer_engine_init(&engine, fake);
timer_engine_set_duration(&engine, 7200); /* 2 Hours */

timer_engine_start(&engine);
fake_clock_advance_hours(&engine.clock, 2); /* Advance 2 hours instantly */
timer_engine_update(&engine);

assert(engine.state == TIMER_STATE_COMPLETED); /* Verified in milliseconds! */
```

## Running Tests

```cmd
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```
