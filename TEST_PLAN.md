# HSINXA Smart Street Lighting v1.1 Test Plan

## Test setup
- Board: Arduino Uno
- Entry PIR output: digital pin 2
- Exit PIR output: digital pin 3
- Existing lighting output: digital pin 6
- Serial Monitor: 9600 baud
- Hold time: 5 seconds
- Keep the existing LED wiring arrangement unchanged for this release.

## Functional tests

| ID | Test | Expected result | Pass |
| --- | --- | --- | --- |
| T01 | Start the simulation with both sensors clear | Lights remain off | [ ] |
| T02 | Trigger entry sensor | Lights turn on | [ ] |
| T03 | Trigger exit sensor | Lights turn on | [ ] |
| T04 | Trigger both sensors | Lights remain on | [ ] |
| T05 | Keep motion active | Hold timer keeps refreshing; lights stay on | [ ] |
| T06 | Clear both sensors | Five-second off timer begins from last motion | [ ] |
| T07 | Trigger motion before timeout | Lights stay on and timer refreshes | [ ] |
| T08 | Keep both sensors clear for five seconds | Lights turn off | [ ] |
| T09 | Observe Serial Monitor | Sensor and light transitions are reported without loop spam | [ ] |
| T10 | Repeat entry and exit tests several times | Consistent state transitions | [ ] |

## Validation record

- Simulation platform/version:
- Date tested:
- Tester:
- Test IDs passed:
- Test IDs failed:
- Notes / screenshots:

## Release gate

Do not mark v1.1 fully validated until all applicable tests pass in simulation. Record physical hardware testing separately. The existing LED wiring limitation is explicitly outside this release's scope.
